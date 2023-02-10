#include "hUGEDriver.h"

#include "routines.h"

UBYTE pending_notes = 0;

void Routine0(unsigned char tick, unsigned int param) {
    param;
    if (tick) return; // return if not zero tick    
    pending_notes |= NOTE_B;
}

void Routine1(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= NOTE_A;
}

void Routine2(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_B | NOTE_A);
}

void Routine3(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= NOTE_L;
}

void Routine4(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_L | NOTE_B);
}

void Routine5(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_L | NOTE_A);
}

void Routine6(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_L | NOTE_B | NOTE_A);
}

void Routine7(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= NOTE_R;
}

void Routine8(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_R | NOTE_B);
}

void Routine9(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_R | NOTE_A);
}

void RoutineA(unsigned char tick, unsigned int param) {
    param; 
    if (tick) return; // return if not zero tick

    pending_notes |= (NOTE_R | NOTE_B | NOTE_A);
}

const hUGERoutine_t routines[] = {
    Routine0, Routine1, Routine2, Routine3, Routine4, Routine5, Routine6, Routine7, Routine8, Routine9, RoutineA 
};
