# Simple (not simple to play) rhythm game for Nintendo Game Boy

This game is actually a hUGEDriver demo

![screenshot](/screenshot.png)

## Requirements and building tips

You need GBDK-2020 v4.0 and GNU make.

## Music
The Music module was written by @Kabcorp. He also did some graphics. Thank you!

He used this version of hUGETracker: https://github.com/untoxa/hUGEBuild/releases/latest/

Original author of hUGETracker is @superdisk, here is a link to the original repo: https://github.com/superDisk/hUGETracker/

## Interaction with the game
hUgeDriver has a feature, that is called "routines". It is actually a user code, that is called when the special effect is encountered. There is a simple way to obtain those calls in your C handlers: just declare those handlers and put them into the routines pointer array. Look into the source code to find out how to implement that in your own software.

Tony.
