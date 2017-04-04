ifeq ($(GIT), 1)
SRC_DIR=$(REPOS)/$(BASENAME)
else
ARCHIVE=$(DOWNLOADS)/$(FILENAME)
SRC_DIR=$(BUILD)/$(BASENAME)
endif
SRC=$(SRC_DIR)/.exists
CONFIGURE=$(SRC_DIR)/configure

all: build
install: install

ifeq ($(GIT), 1)
$(SRC):
	cd $(REPOS); git clone $(URL)
	touch $(SRC)
else
$(ARCHIVE):
	cd $(DOWNLOADS); curl -OL $(URL)

$(SRC): $(ARCHIVE)
	cd $(BUILD) ; tar zxfv $(ARCHIVE)
	touch $(SRC)
endif

$(CONFIGURE): $(SRC)
ifeq ($(AUTORECONF),1)
	cd $(SRC_DIR) &&  autoreconf -i
endif
ifeq ($(AUTOCONF), 1)
	cd $(SRC_DIR) && aclocal && autoheader && automake -a && autoconf
endif
	echo need to configure $(CONFIGURE)
	cd $(SRC_DIR); ./configure --prefix=$(LOCAL) $(CONFIGURE_EXTRA)
	touch $(CONFIGURE)

.PHONY build:
build: $(CONFIGURE)
	make -j4 -C $(SRC_DIR)

.PHONY install:
install: build
	make -C $(SRC_DIR) install