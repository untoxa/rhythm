# Simple (not simple to play) rythm game for Nintendo Game Boy

This game is actually a hUGEDriver demo

![screenshot](/screenshot.png)

## Requirements and building tips

You need the latest version of GBDK-2020 (3.2 for now) and a latest version of SDCC to compile this example. Latest SDCC may be found here: http://sdcc.sourceforge.net/snap.php I suggest at least #11845

Also you may need some changes to the @make.bat, because the version of GBDK I'm currently using is slightly different. If you get errors on the linking stage, change:
	-g _shadow_OAM=0xC000 
into:
	-g .OAM=0xC000

## Music
The Music module was written by @Kabcorp. 

He used this version of hUGETracker: https://github.com/untoxa/hUGEBuild/releases/tag/0.41b1

Original author of hUGETracker is @superdisk, here is a link to the original repo: https://github.com/superDisk/hUGETracker/

Module was converted into the C source by this wonderful online tool from @Daid: https://daid.github.io/rgbds-live/tracker/index.html
It is "Online hUGETracker", actually!

## Interaction with the game
hUgeDriver has a feature, called "routines". It is actually a user code, that is called when the special effect is encountered. There is a simple way to obtain those calls in your C handlers.


Tony.