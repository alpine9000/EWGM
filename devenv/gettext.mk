include common.mk
URL=http://ftp.gnu.org/pub/gnu/gettext/gettext-0.19.8.tar.xz
BASENAME=gettext-0.19.8
FILENAME=$(BASENAME).tar.xz
CONFIGURE_EXTRA=CC=clang

include simple.mk