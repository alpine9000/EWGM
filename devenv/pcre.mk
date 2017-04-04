include common.mk
URL=http://internode.dl.sourceforge.net/project/pcre/pcre/8.39/pcre-8.39.tar.gz
BASENAME=pcre-8.39
FILENAME=$(BASENAME).tar.gz
CONFIGURE_EXTRA=CC=clang --enable-utf8 --enable-unicode-properties

include simple.mk