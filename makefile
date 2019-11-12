
ifeq ($(OS),Windows_NT)
	TARGET := windows
else
	TARGET := unix
endif

.PHONY: unix windows clean

clean: all

run: all
all: $(TARGET)

unix:
	@$(MAKE) -f sdl2_unix.mak $(MAKECMDGOALS)

windows:



