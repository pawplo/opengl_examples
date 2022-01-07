
ifeq ($(shell uname),Darwin)
	OPTIONS = -framework OpenGL
else
	OPTIONS = -lGL -ldl
endif

ifeq ($(CC), )
	CC := gcc
endif

ifneq ($(shell which pkg-config 1>/dev/null 2>/dev/null; echo $$?), 0)
    $(error "pkg-config not installed !!!")
endif

ifneq ($(shell pkg-config --cflags --libs glfw3 1>/dev/null 2>/dev/null; echo $$?), 0)
    $(error "glfw3 not installed !!!")
endif

OPTIONS += $(shell pkg-config --cflags --libs glfw3)

default_target: all

PROGRAMS = geometry_shader geometry_shader2 geometry_shader3 geometry_shader_line geometry_shader_line2 texture gamepad

.PHONY: all
all: $(PROGRAMS)

%: %.c
	$(CC) shader.c gl.c $< -o $@ -Iinclude/ -I./ $(OPTIONS)

.PHONY: clean
clean:
	rm -rf $(PROGRAMS)
