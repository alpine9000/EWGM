include common.mk

all:	dirs
	make -f autoconf.mk
	make -f automake.mk
	make -f lha.mk
	make -f pkg-config.mk
	make -f libtool.mk
	make -f libpng.mk
	make -f pngquant.mk
	make -f graphicsmagic.mk
	make -f cmake.mk
	make -f tmx-loader.mk
	make -f sox.mk
	make -f vlink.mk
	make -f vasm.mk
	make -f wget.mk
	make -f gcc-vasm.mk

install: dirs
	make -f autoconf.mk install
	make -f automake.mk install
	make -f lha.mk install
	make -f pkg-config.mk install
	make -f libtool.mk install
	make -f libpng.mk install
	make -f pngquant.mk install
	make -f graphicsmagic.mk install
	make -f cmake.mk install
	make -f tmx-loader.mk install
	#make -f sox.mk install
	make -f vlink.mk install
	make -f vasm.mk install
	make -f wget.mk
	make -f gcc-vasm.mk install

fs-uae: dirs
	make -f sdl.mk
	make -f libffi.mk
	make -f gettext.mk
	make -f pcre.mk
	make -f glib.mk
	make -f glew.mk
	make -f readline.mk
	make -f libmpeg.mk
	make -f fs-uae.mk

fs-uae-install:
	make -f sdl.mk install
	make -f libffi.mk install
	make -f gettext.mk install
	make -f pcre.mk install
	make -f glib.mk install
	make -f glew.mk install
	make -f readline.mk install
	make -f libmpeg.mk install

ndk: dirs
	make -f ndk.mk

dirs: 
	@if [ ! -d $(PROJECT_DIR)/downloads ] ; then  mkdir -p $(PROJECT_DIR)/downloads ; fi 
	@if [ ! -d $(PROJECT_DIR)/build ] ; then  mkdir -p $(PROJECT_DIR)/build ; fi 
	@if [ ! -d $(PROJECT_DIR)/repos ] ; then  mkdir -p $(PROJECT_DIR)/repos ; fi 
	@mkdir -p $(INSTALL_DIR)
	@mkdir -p $(INSTALL_DIR)/bin
	@mkdir -p $(INSTALL_DIR)/ndk
	@mkdir -p $(INSTALL_DIR)/ndk/include
	@mkdir -p $(INSTALL_DIR)/ndk/lib

