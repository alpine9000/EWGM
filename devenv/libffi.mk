include common.mk
URL=ftp://sourceware.org/pub/libffi/libffi-3.2.1.tar.gz
BASENAME=libffi-3.2.1
FILENAME=$(BASENAME).tar.gz
CONFIGURE_EXTRA=CC=clang

include simple.mk