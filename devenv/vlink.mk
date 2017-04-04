include common.mk
URL= http://sun.hasenbraten.de/vlink/release/vlink.tar.gz
BASENAME=vlink
FILENAME=$(BASENAME).tar.gz


ARCHIVE=$(DOWNLOADS)/$(FILENAME)
SRC_DIR=$(BUILD)/$(BASENAME)
SRC=$(SRC_DIR)/.exists
CONFIGURE=$(SRC_DIR)/configure

all: build
install: install

$(ARCHIVE):
	cd $(DOWNLOADS); curl -OL $(URL)

$(SRC): $(ARCHIVE)
	cd $(BUILD) ; tar zxfv $(ARCHIVE)
	mkdir $(SRC_DIR)/build
	touch $(SRC)

.PHONY build:
build: $(SRC)
	make -C $(SRC_DIR)

.PHONY install:
install: build
	cp -f $(SRC_DIR)/vlink $(INSTALL_DIR)/bin