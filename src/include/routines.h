#ifndef ROUTINES_H_INCLUDE
#define ROUTINES_H_INCLUDE

#include <gb/gb.h> 

#define NOTE_A 1
#define NOTE_B 2
#define NOTE_L 4
#define NOTE_R 8

extern UBYTE pending_notes;

extern void Routine0(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine1(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine2(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine3(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine4(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine5(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine6(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine7(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine8(unsigned char ch, unsigned char param, unsigned char tick);
extern void Routine9(unsigned char ch, unsigned char param, unsigned char tick);
extern void RoutineA(unsigned char ch, unsigned char param, unsigned char tick);


#endif