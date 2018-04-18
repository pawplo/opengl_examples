OS := $(shell uname)
OPTIONS :=

ifeq ($(OS),Darwin)
	OPTIONS += -framework OpenGL
else
	OPTIONS += -lGL
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

geometry_shader: geometry_shader.c
	$(CC) shader.c gl3w.c geometry_shader.c -o geometry_shader -Iinclude/ $(OPTIONS)

clean:
	rm -rf geometry_shader