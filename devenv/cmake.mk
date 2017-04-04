include common.mk
URL=https://cmake.org/files/v3.5/cmake-3.5.1-Darwin-x86_64.tar.gz
BASENAME=cmake-3.5.1-Darwin-x86_64
FILENAME=$(BASENAME).tar.gz
ARCHIVE=$(DOWNLOADS)/$(FILENAME)
SRC_DIR=$(BUILD)/$(BASENAME)
SRC=$(SRC_DIR)/.exists

all: $(SRC)
install:  /Applications/CMake.app

$(ARCHIVE):
	cd $(DOWNLOADS); curl -OL $(URL)

$(SRC): $(ARCHIVE)
	cd $(BUILD) ; tar zxfv $(ARCHIVE)
	touch $(SRC)

/Applications/CMake.app: $(SRC)
	rm -rf /Applications/CMake.app
	cp -r $(SRC_DIR)/CMake.app /Applications/CMake.app
