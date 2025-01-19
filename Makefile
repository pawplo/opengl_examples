CC ?= gcc

ifeq ($(shell uname),Darwin)
	CFLAGS = -I./glfw-3.4.bin.MACOS/include
	LDFLAGS = -framework OpenGL -lglfw -Xlinker -rpath -Xlinker "@executable_path"
else
    ifneq ($(shell which pkg-config 1>/dev/null 2>/dev/null; echo $$?), 0)
        $(error "pkg-config not installed !!!")
    endif

    ifneq ($(shell pkg-config --cflags --libs glfw3 1>/dev/null 2>/dev/null; echo $$?), 0)
        $(error "glfw3 not installed !!!")
    endif

    CFLAGS = $(shell pkg-config --cflags glfw3)
	LDFLAGS = -lGL -ldl $(shell pkg-config --libs glfw3)

endif

CFLAGS += -Wno-macro-redefined

default_target: all

PROGRAMS = geometry_shader geometry_shader2 geometry_shader3 geometry_shader_line geometry_shader_line2 texture gamepad gui

PROGRAMS_OBJS = $(PROGRAMS:.c=.o)

.PHONY: all
all: $(PROGRAMS)

%: %.c shader.c gl.c shader.h gl.h
	$(CC) -Iinclude/ -I./ $(CFLAGS) -L./ $(LDFLAGS) shader.c gl.c $< -o $@

.PHONY: clean

clean:
	rm -rf $(PROGRAMS) ${PROGRAMS_OBJS}
