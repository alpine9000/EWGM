include common.mk
URL=https://pkg-config.freedesktop.org/releases/pkg-config-0.29.tar.gz
BASENAME=pkg-config-0.29
FILENAME=$(BASENAME).tar.gz
CONFIGURE_EXTRA=CC=clang --with-internal-glib LDFLAGS="-framework CoreFoundation -framework Carbon"

include simple.mk