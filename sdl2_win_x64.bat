@echo off

set LIBS_ROOT=C:\LIBS
set SDL2_ROOT=%LIBS_ROOT%\SDL2-2.0.10
set SDL2_TTF_ROOT=%LIBS_ROOT%\SDL2_ttf-2.0.15
set SDL2_IMAGE_ROOT=%LIBS_ROOT%\SDL2_image-2.0.5
set SDL2_MIXER_ROOT=%LIBS_ROOT%\SDL2_mixer-2.0.4

set SDL2_INCLUDE_FLAGS=/I%SDL2_ROOT%\include ^
	/I%SDL2_TTF_ROOT%\include ^
	/I%SDL2_IMAGE_ROOT%\include ^
	/I%SDL2_MIXER_ROOT%\include

set SDL2_LIBS=%SDL2_ROOT%\lib\x64\SDL2.lib ^
	%SDL2_ROOT%\lib\x64\SDL2main.lib ^
	%SDL2_TTF_ROOT%\lib\x64\SDL2_ttf.lib ^
	%SDL2_IMAGE_ROOT%\lib\x64\SDL2_image.lib ^
	%SDL2_MIXER_ROOT%\lib\x64\SDL2_mixer.lib

set GPROJ_DEFINES=/DGPROJ_OS_WINDOWS

set EXTERNAL_INCLUDE_FLAGS=/Iexternals\SDL_FontCache
set EXTERNAL_LIBS=externals\SDL_FontCache\SDL_FontCache.lib
set INCLUDE_FLAGS=/Isrc /Isrc\sdl2 /Isrc\game /Isrc\ogl ^
                  %SDL2_INCLUDE_FLAGS% %EXTERNAL_INCLUDE_FLAGS%

set SRC=src\*.c src\sdl2\*.c src\game\*.c src\ogl\*.c
set CC=cl

set CFLAGS=^
	/TC ^
	/cgthreads8 ^
	/D_CRT_SECURE_NO_WARNINGS ^
	/DPLATFORM_SDL2 ^
	/wd4028 ^
	/wd4214 ^
	/wd4204 ^
	/wd4244 ^
	/W4 ^
	%GPROJ_DEFINES% ^
	%INCLUDE_FLAGS%


set LDFLAGS=/link /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup ^
	%SDL2_LIBS% %EXTERNAL_LIBS% opengl32.lib

set CFLAGS_RELEASE=/Ox
set LDFLAGS_RELEASE=

set CFLAGS_DEBUG=/DEBUG:FULL /Od /Zi /DGPROJ_DEBUG /DGPROJ_PROFILING /DDEBUG
set LDFLAGS_DEBUG=

set CFLAGS=%CFLAGS% %CFLAGS_DEBUG%
set LDFLAGS=%LDFLAGS% %LDFLAGS_DEBUG%

@echo on
pushd .\externals\SDL_FontCache
call win_x64.bat
popd
@echo off

if not exist "build" mkdir build
@echo on
del build\gproj.exe
%CC% %SRC% %CFLAGS% /Febuild\gproj.exe %LDFLAGS%
@echo off
set ERROR=%errorLevel%
if NOT %ERROR% == 0 (
	echo "BUILD FAILED"
	GOTO CANCEL
)

copy %SDL2_ROOT%\lib\x64\SDL2.dll build\
copy %SDL2_TTF_ROOT%\lib\x64\libfreetype-6.dll build\
copy %SDL2_IMAGE_ROOT%\lib\x64\SDL2_image.dll build\
copy %SDL2_IMAGE_ROOT%\lib\x64\libpng16-16.dll build\
copy %SDL2_MIXER_ROOT%\lib\x64\SDL2_mixer.dll build\
copy *.pdb build\


:CANCEL
del *.obj
del *.pdb


