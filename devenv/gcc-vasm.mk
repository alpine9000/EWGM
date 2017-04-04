include common.mk
URL=https://github.com/alpine9000/gcc.git
BASENAME=gcc
SRC_DIR=$(REPOS)/$(BASENAME)
SRC=$(SRC_DIR)/.exists
BUILDDIR=$(BUILD)/gcc
CONFIGURE=$(SRC_DIR)/configure

all: build
install: install

$(SRC):
	cd $(REPOS); git clone $(URL)
	cd $(SRC_DIR);./contrib/download_prerequisites
	touch $(SRC)

$(CONFIGURE): $(SRC)
	mkdir -p $(BUILDDIR)
	echo $(BUILDDIR)
	cd $(BUILDDIR); $(SRC_DIR)/configure --prefix=$(INSTALL_DIR) --target=m68k-amigaos --enable-languages=c --with-as=$(INSTALL_DIR)/bin/vasmm68k_mot
	touch $(CONFIGURE)

.PHONY build:
build: $(CONFIGURE)
	echo  $(BUILDDIR)
	make -j4 -C $(BUILDDIR) all-gcc

.PHONY install:
install: build
	make -C $(BUILDDIR) install-gcc