include common.mk
URL=git://github.com/pornel/pngquant.git
BASENAME=pngquant
SRC_DIR=$(REPOS)/$(BASENAME)
SRC=$(SRC_DIR)/.exists
CONFIGURE=$(SRC_DIR)/configure

all: build
install: install

$(SRC):
	cd $(REPOS); git clone $(URL)
	touch $(SRC)

$(CONFIGURE): $(SRC)
	cd $(SRC_DIR); ./configure --prefix=$(LOCAL)
	cd $(SRC_DIR)/lib; ./configure --prefix=$(LOCAL)
	touch $(CONFIGURE)

.PHONY build:
build: $(CONFIGURE)
	make -j4 -C $(SRC_DIR)/lib

.PHONY install:
install: build
	mkdir -p $(LOCAL)/include/pngquant
	mkdir -p $(LOCAL)/lib
	cp $(SRC_DIR)/lib/*.h $(LOCAL)/include/pngquant/
	cp $(SRC_DIR)/lib/*.a $(LOCAL)/lib
