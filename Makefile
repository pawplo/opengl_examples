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

all: geometry_shader geometry_shader2 geometry_shader3 geometry_shader_line geometry_shader_line2 texture
#all: geometry_shader geometry_shader_line2

geometry_shader: geometry_shader.c
	$(CC) shader.c gl.c  geometry_shader.c -o geometry_shader -Iinclude/ -I./ $(OPTIONS)

geometry_shader2: geometry_shader2.c
	$(CC) shader.c gl.c geometry_shader2.c -o geometry_shader2 -Iinclude/ -I./ $(OPTIONS)

geometry_shader3: geometry_shader3.c
	$(CC) shader.c gl.c geometry_shader3.c -o geometry_shader3 -Iinclude/ -I./ $(OPTIONS)

geometry_shader_line: geometry_shader_line.c
	$(CC) shader.c gl.c geometry_shader_line.c -o geometry_shader_line -Iinclude/ -I./ $(OPTIONS)

geometry_shader_line2: geometry_shader_line2.c
	$(CC) shader.c gl.c geometry_shader_line2.c -o geometry_shader_line2 -Iinclude/ -I./ $(OPTIONS)

texture: texture.c
	$(CC) shader.c gl.c texture.c -o texture -Iinclude/ -I./ $(OPTIONS)

clean:
	rm -rf geometry_shader geometry_shader2 geometry_shader3 geometry_shader_line geometry_shader_line2 texture
