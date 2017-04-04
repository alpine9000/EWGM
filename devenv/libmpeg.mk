include common.mk
URL=http://libmpeg2.sourceforge.net/files/libmpeg2-0.5.1.tar.gz
BASENAME=libmpeg2-0.5.1
FILENAME=$(BASENAME).tar.gz
CONFIGURE_EXTRA=CC="clang -std=gnu89"

include simple.mk