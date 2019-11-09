@echo off

set BUILD_TYPE=%1
set BUILD_AND_RUN="%2"=="run"

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

set GPROJ_DEFINES=/DGPROJ_OS_WINDOWS /DGPROJ_PLATFORM_SDL2

set GPROJ_DEBUG_DEFINES=/DDEBUG /DGPROJ_DEBUG

set GPROJ_PROFILING_DEFINES=/DNDEBUG /DGPROJ_PROFILING

set GPROJ_RELEASE_DEFINES=/DGPROJ_RELEASE

set EXTERNAL_INCLUDE_FLAGS=/Iexternals\SDL_FontCache
set EXTERNAL_LIBS=externals\SDL_FontCache\SDL_FontCache.lib

set INCLUDE_FLAGS=/Isrc /Isrc\platform /Isrc\platform\sdl2 /Isrc\game /Isrc\ogl ^
                  %SDL2_INCLUDE_FLAGS% %EXTERNAL_INCLUDE_FLAGS%

set SRC=src\*.c src\platform\sdl2\*.c src\game\*.c src\ogl\*.c
set CC=cl

set CFLAGS=^
	/TC ^
	/cgthreads8 ^
	/MP8^
	/D_CRT_SECURE_NO_WARNINGS ^
	/wd4028 ^
	/wd4214 ^
	/wd4204 ^
	/wd4244 ^
	/W4 ^
	%GPROJ_DEFINES% ^
	%INCLUDE_FLAGS%

set LDFLAGS=/link /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup ^
	%SDL2_LIBS% %EXTERNAL_LIBS% opengl32.lib
	
set PERFORMANCE_FLAGS=/Ox

set CFLAGS_RELEASE=%PERFORMANCE_FLAGS% %GPROJ_RELEASE_DEFINES%
set LDFLAGS_RELEASE=

set CFLAGS_PROFILING=%PERFORMANCE_FLAGS% %GPROJ_PROFILING_DEFINES%
set LDFLAGS_PROFILING=

set CFLAGS_DEBUG=/DEBUG:FULL /Od /Zi %GPROJ_DEBUG_DEFINES%
set LDFLAGS_DEBUG=

if "%BUILD_TYPE%"=="Release" GOTO RELEASE_SETUP
if "%BUILD_TYPE%"=="Profiling" ( GOTO PROFILING_SETUP ) else GOTO DEBUG_SETUP

:RELEASE_SETUP
set BUILD_TYPE="Release"
set CFLAGS=%CFLAGS% %CFLAGS_RELEASE%
set LDFLAGS=%LDFLAGS% %LDFLAGS_RELEASE%
GOTO BUILD

:PROFILING_SETUP
set BUILD_TYPE="Profiling"
set CFLAGS=%CFLAGS% %CFLAGS_PROFILING%
set LDFLAGS=%LDFLAGS% %LDFLAGS_PROFILING%
GOTO BUILD

:DEBUG_SETUP
set BUILD_TYPE="Debug"
set CFLAGS=%CFLAGS% %CFLAGS_DEBUG%
set LDFLAGS=%LDFLAGS% %LDFLAGS_DEBUG%
GOTO BUILD


:BUILD
@echo on
pushd .\externals\SDL_FontCache
call win_x64.bat
popd
@echo off

if not exist "build" mkdir build
@echo on
%CC% %SRC% %CFLAGS% /Febuild\gproj.exe %LDFLAGS%
@echo off

set ERROR=%errorLevel%
if NOT %ERROR% == 0 (
	echo "BUILD FAILED"
	GOTO CLEAN
)

copy %SDL2_ROOT%\lib\x64\SDL2.dll build\
copy %SDL2_TTF_ROOT%\lib\x64\SDL2_ttf.dll build\
copy %SDL2_TTF_ROOT%\lib\x64\zlib1.dll build\
copy %SDL2_TTF_ROOT%\lib\x64\libfreetype-6.dll build\
copy %SDL2_IMAGE_ROOT%\lib\x64\SDL2_image.dll build\
copy %SDL2_IMAGE_ROOT%\lib\x64\libpng16-16.dll build\
copy %SDL2_MIXER_ROOT%\lib\x64\SDL2_mixer.dll build\
if "%BUILD_TYPE%"=="Debug" (
	move *.pdb build\
)


:CLEAN
del *.obj
if NOT %ERROR% == 0 (
	GOTO EOF
)

if %BUILD_AND_RUN% ( GOTO RUN ) else GOTO EOF

:RUN
cd build\\
gproj.exe
cd ..


:EOF


