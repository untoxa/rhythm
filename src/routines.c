#include "routines.h"

UBYTE pending_notes = 0;

void Routine0(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param;
    if (tick) return; // return if not zero tick    
    pending_notes |= NOTE_B;
}

void Routine1(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= NOTE_A;
}

void Routine2(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_B | NOTE_A);
}

void Routine3(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= NOTE_L;
}

void Routine4(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_L | NOTE_B);
}

void Routine5(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_L | NOTE_A);
}

void Routine6(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_L | NOTE_B | NOTE_A);
}

void Routine7(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= NOTE_R;
}

void Routine8(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_R | NOTE_B);
}

void Routine9(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_R | NOTE_A);
}

void RoutineA(unsigned char ch, unsigned char param, unsigned char tick) {
    ch; param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_R | NOTE_B | NOTE_A);
}