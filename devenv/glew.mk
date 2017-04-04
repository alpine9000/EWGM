include common.mk
URL=https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.tgz/download > glew-1.13.0.tgz
BASENAME=glew-1.13.0
FILENAME=$(BASENAME).tgz
CONFIGURE_EXTRA=CC=clang

ARCHIVE=$(DOWNLOADS)/$(FILENAME)
SRC_DIR=$(BUILD)/$(BASENAME)
SRC=$(SRC_DIR)/.exists

all: build
install: install

$(ARCHIVE):
	cd $(DOWNLOADS); curl -L $(URL)

$(SRC): $(ARCHIVE)
	cd $(BUILD) ; tar zxfv $(ARCHIVE)
	touch $(SRC)

.PHONY build:
build: $(SRC)
	make -j4 -C $(SRC_DIR) GLEW_DEST=$(LOCAL)

.PHONY install:
install: build
	make -C $(SRC_DIR) install GLEW_DEST=$(LOCAL)

