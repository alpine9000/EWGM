include common.mk
URL= http://gemmei.acc.umu.se/pub/GNOME/sources/glib/2.50/glib-2.50.1.tar.xz
BASENAME=glib-2.50.1
FILENAME=$(BASENAME).tar.xz
CONFIGURE_EXTRA=CC=clang

include simple.mk