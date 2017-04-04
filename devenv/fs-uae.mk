include common.mk
URL=https://github.com/alpine9000/fs-uae.git
BASENAME=fs-uae
SRC_DIR=$(PROJECT_DIR)/$(BASENAME)
SRC=$(SRC_DIR)/.exists
CONFIGURE=$(SRC_DIR)/configure

all: build
install: install

$(SRC):
	cd $(PROJECT_DIR); git clone $(URL)
	cp  $(SRC_DIR)/readline.pc $(LOCAL)/lib/pkgconfig
	cd $(SRC_DIR); ./bootstrap
	touch $(SRC)

$(CONFIGURE): $(SRC)
	cd $(SRC_DIR); ./configure CC=cc CXX=c++ --with-readline --enable-debug-symbols
	touch $(CONFIGURE)

.PHONY build:
build: $(CONFIGURE)
	make -j4 -C $(SRC_DIR)

.PHONY install:
install: build
	make -C $(SRC_DIR) install