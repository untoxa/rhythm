@echo off
@set PROJ=rhythm
@set GBDK=..\..\gbdk\
@set GBDKLIB=%GBDK%lib\small\asxxxx\
@set OBJ=build\
@set SRC=src\
@set RES=%SRC%gfx\
@set MAP=%SRC%maps\
@set TOOLS=tools\

@set PROFILING=0
@set DEBUGGING=0
@set OPTIMIZE=0

@set CFLAGS=-mgbz80 --fsigned-char --no-std-crt0 -I%GBDK%include -I%GBDK%include\asm -I%SRC%include -I%OBJ%. -c
@set CFLAGS=%CFLAGS% --max-allocs-per-node 50000

@set LNAMES=-g _shadow_OAM=0xC000 -g .STACK=0xE000 -g .refresh_OAM=0xFF80 -b _DATA=0xc0a0 -b _CODE=0x0200
@set LFLAGS=-n -m -j -w -i -k %GBDKLIB%\gbz80\ -l gbz80.lib -k %GBDKLIB%\gb\ -l gb.lib %LNAMES%
@set LFILES=%GBDKLIB%gb\crt0.o %SRC%driver\driver_lite.obj.o

@set ASMFLAGS=-plosgff -I%GBDKLIB%

@rem @set BINFLAGS=-yt 0x1a -yo 4 -ya 4
@set BINFLAGS=-yt 0

@if %1. == debug. (
    @echo Debugging mode ON
    @set DEBUGGING=1
)
 
@if %1. == profile. (
    @set PROFILING=1 
    @echo Profilig mode ON
)

@if %1. == optimize. (
    @set OPTIMIZE=1 
    @echo Optimization rules ON
)

@if %OPTIMIZE%==1 @set CFLAGS=%CFLAGS% --peep-file peephole\gbz80.rul --fverbose-asm
@if %PROFILING%==1 @set CFLAGS=%CFLAGS% --profile -DPROFILING
@if %DEBUGGING%==1 (
    @set CFLAGS=%CFLAGS% --debug -DDEBUGGING
@rem    @set CFLAGS=%CFLAGS% --fverbose-asm
    @set LFLAGS=%LFLAGS% -y
)

@echo Cleanup...

@if exist %OBJ% rd /s/q %OBJ%
@if exist %PROJ%.gb del %PROJ%.gb
@if exist %PROJ%.sym del %PROJ%.sym
@if exist %PROJ%.map del %PROJ%.map
@if exist %PROJ%.noi del %PROJ%.noi
@if exist %PROJ%.cdb del %PROJ%.cdb
@if exist %PROJ%.ihx del %PROJ%.ihx

@if not exist %OBJ% mkdir %OBJ%

@echo COMPILING RESOURCES...

@for %%x in (
       ) do (
         %TOOLS%gbr2c.exe %RES%%%x %OBJ%
         call :docompile %OBJ% %%x.c
       )

@echo COMPILING...

@for %%x in (
       ) do call :doassemble %SRC% %%x

@for %%x in (
        song.c
        routines.c
        tiledata.c
        %PROJ%.c
       ) do call :docompile %SRC% %%x

@echo LINKING...
sdldgb %LFLAGS% %PROJ%.ihx %LFILES%
IF %ERRORLEVEL% NEQ 0 goto :error

@echo MAKING BIN...
makebin -Z %BINFLAGS% %PROJ%.ihx %PROJ%.gb

@echo DONE!
@goto :eof

:docompile
@echo %2
sdcc %CFLAGS% %1%2 -o %OBJ%%2.rel
IF %ERRORLEVEL% NEQ 0 goto :error
@set LFILES=%LFILES% %OBJ%%2.rel
goto :eof

:doassemble
@echo %2
sdasgb %ASMFLAGS% %OBJ%%2.rel %1%2
IF %ERRORLEVEL% NEQ 0 goto :error
@set LFILES=%LFILES% %OBJ%%2.rel
goto :eof

:error
@echo ERROR !
exit 0
