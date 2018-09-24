.PHONY: build test

OBJS=src/lua-urlencode.o

build: $(OBJS)
	luarocks build

test: build
	LUA_CPATH=./?.so lua test/test.lua

