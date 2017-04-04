MAKEADFDIR=../tools/makeadf/
MAKEADF=$(MAKEADFDIR)/out/makeadf
IMAGECONDIR=../tools/imagecon
IMAGECON=$(IMAGECONDIR)/out/imagecon
RESIZEDIR=../tools/resize
RESIZE=$(RESIZEDIR)/out/resize
CROPPADIR=../tools/croppa
CROPPA=$(CROPPADIR)/out/croppa
MAPGENDIR=../tools/mapgen
MAPGEN=$(MAPGENDIR)/out/mapgen
FADEDIR=../tools/fade
FADE=$(FADEDIR)/out/fade
BIN2CDIR=../tools/bin2c
BIN2C=$(BIN2CDIR)/out/bin2c

ALL_TOOLS=$(MAKEADF) $(IMAGECON) $(MAPGEN) $(FADE) $(CROPPA) $(RESIZE) $(BIN2C)

$(IMAGECON):
	make -C $(IMAGECONDIR)

$(BIN2C):
	make -C $(BIN2CDIR)

$(SHRINKLEREXE):
	make -C $(SHRINKLERDIR)

$(RESIZE):
	make -C $(RESIZEDIR)

$(CROPPA):
	make -C $(CROPPADIR)

$(MAPGEN):
	make -C $(MAPGENDIR)

$(FADE):
	make -C $(FADEDIR)

$(MAKEADF):
	make -C $(MAKEADFDIR)

$(DOYNAMITE68K):
	make -C $(DOYNAMITE68KDIR)