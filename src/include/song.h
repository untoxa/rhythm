#ifndef __SONG_H_INCLUDE
#define __SONG_H_INCLUDE

#include "hUGEDriver.h"

#ifndef SONG_VAR_NAME
#define SONG_VAR_NAME song
#endif

extern const hUGESong_t SONG_VAR_NAME;

void Routine0(unsigned char ch, unsigned char param, unsigned char tick);
void Routine1(unsigned char ch, unsigned char param, unsigned char tick);
void Routine2(unsigned char ch, unsigned char param, unsigned char tick);
void Routine3(unsigned char ch, unsigned char param, unsigned char tick);
void Routine4(unsigned char ch, unsigned char param, unsigned char tick);
void Routine5(unsigned char ch, unsigned char param, unsigned char tick);
void Routine6(unsigned char ch, unsigned char param, unsigned char tick);
void Routine7(unsigned char ch, unsigned char param, unsigned char tick);
void Routine8(unsigned char ch, unsigned char param, unsigned char tick);
void Routine9(unsigned char ch, unsigned char param, unsigned char tick);
void RoutineA(unsigned char ch, unsigned char param, unsigned char tick);

#endif