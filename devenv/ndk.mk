include common.mk
URL=http://www.haage-partner.de/download/AmigaOS/NDK39.lha
BASENAME=NDK39
FILENAME=$(BASENAME).lha
ARCHIVE=$(DOWNLOADS)/$(FILENAME)
SRC=$(BUILD)/NDK_3.9

all: $(SRC)
	cp -r $(SRC)/Include/include_i/* $(INSTALL_DIR)/ndk/include/
	cp -r $(SRC)/Include/include_h/* $(INSTALL_DIR)/ndk/include/
	cp $(SRC)/Include/linker_libs/* $(INSTALL_DIR)/ndk/lib

$(SRC): $(ARCHIVE)
	cd $(BUILD); lha x $(ARCHIVE)

$(ARCHIVE):
	cd $(DOWNLOADS); curl -OL $(URL)


