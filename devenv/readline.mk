include common.mk
URL=https://ftp.gnu.org/gnu/readline/readline-6.3.tar.gz
BASENAME=readline-6.3
FILENAME=$(BASENAME).tar.gz
CONFIGURE_EXTRA=CC=clang

include simple.mk