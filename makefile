
ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
	TARGET := windows
else
	TARGET := unix
endif

clean: all

all: $(TARGET)

unix:
	@$(MAKE) -f sdl2_unix.mak $(MAKECMDGOALS)

windows: 
	@$(MAKE) -f sdl2_win.mak $(MAKECMDGOALS)


