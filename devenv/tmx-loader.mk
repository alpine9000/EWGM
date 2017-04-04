include common.mk
URL=https://github.com/baylej/tmx.git
BASENAME=tmx

SRC_DIR=$(REPOS)/$(BASENAME)
SRC=$(SRC_DIR)/.exists

all: build
install: install

$(SRC):
	cd $(REPOS); git clone $(URL)
	touch $(SRC)

.PHONY build:
build: $(SRC)
	mkdir -p $(SRC_DIR)/build
	cd $(SRC_DIR)/build;/Applications/CMake.app/Contents/bin/cmake ..
	make -C $(SRC_DIR)/build

.PHONY install:
install: build
	make -C $(SRC_DIR)/build install