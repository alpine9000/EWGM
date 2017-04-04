include common.mk
URL=http://sun.hasenbraten.de/vasm/release/vasm.tar.gz
BASENAME=vasm
FILENAME=$(BASENAME).tar.gz

ARCHIVE=$(DOWNLOADS)/$(FILENAME)
SRC_DIR=$(BUILD)/$(BASENAME)
SRC=$(SRC_DIR)/.exists
CONFIGURE=$(SRC_DIR)/configure

all: build

$(ARCHIVE):
	cd $(DOWNLOADS); curl -OL $(URL)

$(SRC): $(ARCHIVE)
	cd $(BUILD) ; tar zxfv $(ARCHIVE)
	mkdir $(SRC_DIR)
	touch $(SRC)

.PHONY build:
build: $(SRC)
	make -C $(SRC_DIR) CPU=m68k SYNTAX=mot

.PHONY install:
install: build
	cp -f $(SRC_DIR)/vasmm68k_mot $(INSTALL_DIR)/bin
