include common.mk
URL=https://www.libsdl.org/release/SDL2-2.0.5.tar.gz
BASENAME=SDL2-2.0.5
FILENAME=$(BASENAME).tar.gz
CONFIGURE_EXTRA=CC=clang

include simple.mk