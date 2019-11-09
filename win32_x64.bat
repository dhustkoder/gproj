@echo off

set BUILD_TYPE=%1
set BUILD_AND_RUN="%2"=="run"


set GPROJ_DEFINES=/DGPROJ_OS_WINDOWS /DGPROJ_PLATFORM_WIN32
set GPROJ_DEBUG_DEFINES=/DDEBUG /DGPROJ_DEBUG
set GPROJ_PROFILING_DEFINES=/DNDEBUG /DGPROJ_PROFILING
set GPROJ_RELEASE_DEFINES=/DGPROJ_RELEASE


set SRC=src\*.c src\platform\win32\*.c src\game\*.c src\ogl\*.c
set CC=cl

set LIBS=opengl32.lib

set INCLUDE_FLAGS=/Isrc /Isrc\platform /Isrc\platform\win32 /Isrc\game /Isrc\ogl


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

set LDFLAGS=/link /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup %LIBS%
	
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
set CFLAGS=%CFLAGS% %CFLAGS_RELEASE%
set LDFLAGS=%LDFLAGS% %LDFLAGS_RELEASE%
GOTO BUILD

:PROFILING_SETUP
set CFLAGS=%CFLAGS% %CFLAGS_PROFILING%
set LDFLAGS=%LDFLAGS% %LDFLAGS_PROFILING%
GOTO BUILD

:DEBUG_SETUP
set CFLAGS=%CFLAGS% %CFLAGS_DEBUG%
set LDFLAGS=%LDFLAGS% %LDFLAGS_DEBUG%
GOTO BUILD


:BUILD
if not exist "build" mkdir build
@echo on
%CC% %SRC% %CFLAGS% /Febuild\gproj.exe %LDFLAGS%
@echo off
set ERROR=%errorLevel%
if NOT %ERROR% == 0 (
	echo "BUILD FAILED"
	GOTO CLEAN
)


:CLEAN
del *.obj
del *.pdb
if NOT %ERROR% == 0 (
	GOTO EOF
)

if %BUILD_AND_RUN% ( GOTO RUN ) else GOTO EOF

:RUN
cd build\\
gproj.exe
cd ..


:EOF


