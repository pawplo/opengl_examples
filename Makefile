OS := $(shell uname)
OPTIONS :=

ifeq ($(OS),Darwin)
	OPTIONS += -framework OpenGL
else
	OPTIONS += -lGL -ldl
endif

ifeq ($(CC), )
	CC := gcc
endif

PKG_CONFIG_OK := $(shell which pkg-config 1>/dev/null 2>/dev/null; echo $$?)
ifneq ($(PKG_CONFIG_OK), 0)
    $(error "pkg-config not installed !!!")
endif

GLFW3_OK := $(shell pkg-config --cflags --libs glfw3 1>/dev/null 2>/dev/null; echo $$?)
ifneq ($(GLFW3_OK), 0)
    $(error "glfw3 not installed !!!")
endif

OPTIONS += $(shell pkg-config --cflags --libs glfw3)

default_target: all

PROGRAMS = geometry_shader geometry_shader2 geometry_shader3 geometry_shader_line geometry_shader_line2 texture

all: $(PROGRAMS)

%: %.c
	$(CC) shader.c gl.c $< -o $@ -Iinclude/ -I./ $(OPTIONS)

clean:
	rm -rf $(PROGRAMS)
