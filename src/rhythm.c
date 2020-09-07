#include <gb/gb.h>
#include <bcd.h>

#include "hUGEDriver.h"

#include "tiledata.h"
#include "song.h"
#include "routines.h"

#define SPR_A_IDX (0x20)
#define SPR_B_IDX (0x20 + 4)
#define SPR_L_IDX (0x20 + 8)
#define SPR_R_IDX (0x20 + 12)
#define SPR_OK_IDX (0x20 + 16)
#define SPR_BAD_IDX (0x20 + 20)

typedef struct note_t {
    UWORD xpos, ypos, typ;
    UINT8 allocated_tiles[4];
    struct note_t * next;
} note_t;

#define L_OFFSET (48 + 8 - 6)
#define R_OFFSET (L_OFFSET + 16 + 4)
#define B_OFFSET (R_OFFSET + 16 + 4)
#define A_OFFSET (B_OFFSET + 16 + 4)

#define NOTE_Y_OFFSET 16u
#define KEY_Y_OFFSET (UBYTE)((144 / 2) + 16)

#define SPRITE_Y_LIMIT (UBYTE)(144 + 16)
#define HIT_Y_LIMIT (UBYTE)(80 + 16)
#define FAIL_Y_LIMIT (UBYTE)(112 + 16)

#define MAX_HOLD 10u

#define DEF_FALL_SPEED (UBYTE)(16 * 2)

const unsigned char pattern1[] = {0x80,0x80,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x01,0x01};
const unsigned char pattern2[] = {0xAA,0xAA,0x00,0x00,0xAA,0xAA,0x00,0x00,0xAA,0xAA,0x00,0x00,0xAA,0xAA,0x00,0x00};

const unsigned char digits[]   = {0x00,0x00,0x7C,0x00,0xEE,0x00,0xC6,0x00,0xC6,0x00,0xC6,0x00,0xEE,0x00,0x7C,0x00,
                                  0x00,0x00,0x18,0x00,0x38,0x00,0x38,0x00,0x18,0x00,0x18,0x00,0x3E,0x00,0x7C,0x00,
                                  0x00,0x00,0x7C,0x00,0xCE,0x00,0x9C,0x00,0x38,0x00,0x70,0x00,0xE6,0x00,0xFE,0x00,
                                  0x00,0x00,0x7E,0x00,0xCC,0x00,0x98,0x00,0x3C,0x00,0x0E,0x00,0xCE,0x00,0x7C,0x00,
                                  0x08,0x00,0x60,0x00,0xC8,0x00,0xD8,0x00,0x7E,0x00,0x18,0x00,0x18,0x00,0x10,0x00,
                                  0x00,0x00,0xFC,0x00,0x66,0x00,0x60,0x00,0x7C,0x00,0x0E,0x00,0xCE,0x00,0x7C,0x00,
                                  0x00,0x00,0x3C,0x00,0x60,0x00,0xDC,0x00,0xF6,0x00,0xC2,0x00,0xE6,0x00,0x7C,0x00,
                                  0x00,0x00,0x7E,0x00,0xE6,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x30,0x00,0x30,0x00,
                                  0x00,0x00,0x7C,0x00,0xEE,0x00,0xC6,0x00,0x7C,0x00,0xC6,0x00,0xEE,0x00,0x7C,0x00,
                                  0x00,0x00,0x7C,0x00,0xCE,0x00,0x86,0x00,0xDE,0x00,0x76,0x00,0x0C,0x00,0x78,0x00};

const unsigned char zone_map[] = {0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A};

const unsigned char ok_map[]   = {SPR_OK_IDX, SPR_OK_IDX+1, SPR_OK_IDX+2, SPR_OK_IDX+3};
const unsigned char fail_map[] = {SPR_BAD_IDX, SPR_BAD_IDX+1, SPR_BAD_IDX+2, SPR_BAD_IDX+3};

#define MAX_FALLING_NOTES 10
struct note_t notes[MAX_FALLING_NOTES];
struct note_t * notes_root, * free_notes;

void initialize_notes() {
    UBYTE k = 0;
    free_notes = notes;
    for (UBYTE i = 0; i < MAX_FALLING_NOTES; i++) {
        free_notes->ypos = 0;
        for (UBYTE j = 0; j < 4; j++) {
            free_notes->allocated_tiles[j] = k;
            k++;
        }
        free_notes->next = free_notes + 1;
        free_notes++;
    }
    free_notes->next = 0;
    free_notes = notes;
    notes_root = 0;
}

void allocate_note(UBYTE x, UBYTE y, UBYTE tile) {
    struct note_t * note;
    if (free_notes) {
        note = free_notes;
        free_notes = free_notes->next;
        note->next = notes_root;
        notes_root = note;
        note->xpos = x, note->ypos = y;
        note->typ = 0;
        for (UBYTE i = 0; i < 4; i++) set_sprite_tile(note->allocated_tiles[i], tile + i);
    }
}

void set_new_tiles(struct note_t * note, UBYTE tile) {
    if (!note) return;
    UINT8 * tmp = note->allocated_tiles;
    UINT8 t = tile;
    set_sprite_tile(*tmp, t), tmp++, t++;
    set_sprite_tile(*tmp, t), tmp++, t++;
    set_sprite_tile(*tmp, t), tmp++, t++;
    set_sprite_tile(*tmp, t);
}

UBYTE fall_speed = DEF_FALL_SPEED; // 16 == 1px per frame

UBYTE current_notes;

UBYTE joy, old_joy, joy_diff;

BCD hits_count = MAKE_BCD(0);
BCD miss_count = MAKE_BCD(0);

const BCD score_inc = MAKE_BCD(1);
unsigned char textbuf[10];
UBYTE redraw_scores;

void main() {
    LCDC_REG = 0xD1;
    BGP_REG  = 0b11100100;
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    set_bkg_data(0, 1, pattern1);
    set_bkg_data(0x1A, 1, pattern2);

    set_bkg_data(0x40, 10, digits);

    set_bkg_tiles(0, (HIT_Y_LIMIT / 8), 20, 1, zone_map);
    set_bkg_tiles(0, ((HIT_Y_LIMIT / 8) + 1), 20, 1, zone_map);
    set_bkg_tiles(0, ((HIT_Y_LIMIT / 8) + 2), 20, 1, zone_map);

    set_sprite_data(SPR_A_IDX, 4, SPR_A);
    set_sprite_data(SPR_B_IDX, 4, SPR_B);
    set_sprite_data(SPR_L_IDX, 4, SPR_L);
    set_sprite_data(SPR_R_IDX, 4, SPR_R);
    set_sprite_data(SPR_OK_IDX, 4, SPR_OK);
    set_sprite_data(SPR_BAD_IDX, 4, SPR_BAD);

    set_bkg_tiles(17, 0, 2, 2, ok_map);
    set_bkg_tiles(17, 4, 2, 2, fail_map);

    for (UBYTE i = 0; i < 16u; i++) set_sprite_tile(i, 0x20u);
    SHOW_SPRITES;

    __critical {
        hUGE_init(&song);
        add_VBL(hUGE_dosound);
    }

    initialize_notes();

    old_joy = 0u;
    while(1) {
        wait_vbl_done();
        joy = joypad();
        joy_diff = joy ^ old_joy;
        old_joy = joy;

        __critical {
            current_notes = pending_notes, pending_notes = 0;
        }

        if (current_notes & NOTE_A) allocate_note(A_OFFSET, NOTE_Y_OFFSET, SPR_A_IDX);
        if (current_notes & NOTE_B) allocate_note(B_OFFSET, NOTE_Y_OFFSET, SPR_B_IDX);
        if (current_notes & NOTE_L) allocate_note(L_OFFSET, NOTE_Y_OFFSET, SPR_L_IDX);
        if (current_notes & NOTE_R) allocate_note(R_OFFSET, NOTE_Y_OFFSET, SPR_R_IDX);

        if (notes_root) {
            struct note_t * tmp = notes_root->next, * otmp = notes_root;
            redraw_scores = 0;
            UBYTE y;
            while (tmp) {
                tmp->ypos += fall_speed;
                y = tmp->ypos >> 4;
                if (!tmp->typ) {
                    if (y > FAIL_Y_LIMIT) {
                        tmp->typ = 1;
                        set_new_tiles(tmp, SPR_BAD_IDX);
                        bcd_add(&miss_count, &score_inc);
                        redraw_scores = 1;
                    } else if (y > HIT_Y_LIMIT) {
                        switch (tmp->xpos) {
                            case A_OFFSET:
                                if (((joy_diff) & J_A) && (joy & J_A)) tmp->typ = 2, joy_diff &= !J_A;
                                break;
                            case B_OFFSET:
                                if (((joy_diff) & J_B) && (joy & J_B)) tmp->typ = 2, joy_diff &= !J_B;
                                break;
                            case L_OFFSET:
                                if (((joy_diff) & J_LEFT) && (joy & J_LEFT)) tmp->typ = 2, joy_diff &= !J_LEFT;
                                break;
                            case R_OFFSET:
                                if (((joy_diff) & J_RIGHT) && (joy & J_RIGHT)) tmp->typ = 2, joy_diff &= !J_RIGHT;
                                break;
                        }
                        if (tmp->typ == 2) {
                            set_new_tiles(tmp, SPR_OK_IDX);
                            bcd_add(&hits_count, &score_inc);
                            redraw_scores = 1;
                        }
                    }
                } 
                if (y > SPRITE_Y_LIMIT) y = 0;
                move_sprite(tmp->allocated_tiles[0], tmp->xpos, y);
                move_sprite(tmp->allocated_tiles[1], tmp->xpos + 8, y);
                move_sprite(tmp->allocated_tiles[2], tmp->xpos, y + 8);
                move_sprite(tmp->allocated_tiles[3], tmp->xpos + 8, y + 8);
                if (y) {
                    otmp = tmp;
                    tmp = tmp->next;
                } else {
                    otmp->next = tmp->next;
                    tmp->next = free_notes;
                    free_notes = tmp;
                    tmp = otmp->next;    
                }
            }
        }
        if (joy_diff & joy) 
            bcd_add(&miss_count, &score_inc), redraw_scores = 1;
            
        if (redraw_scores) {
            bcd2text(&miss_count, 0x40, textbuf);
            set_bkg_tiles(16, 2, 4, 1, textbuf + 4);
            bcd2text(&hits_count, 0x40, textbuf);
            set_bkg_tiles(16, 6, 4, 1, textbuf + 4);
        }
    }    
}
