#include "imagecon.h"

void palette_loadFile(imagecon_image_t* ic)
{
  FILE* fp = file_openRead(config.overridePalette);
  int paletteIndex;

  if (config.verbose) {  
    printf("palette_loadFile:\n");
  }

  int index;
  for (paletteIndex = 0, index = 0; paletteIndex < MAX_PALETTE; paletteIndex++) {

    char buffer[255];
    char* line = fgets(buffer, 255, fp);
    if (!line) {
      break;
    }

    if (!config.fullColorPaletteFile) {
      unsigned int c;
      sscanf(buffer, "%x\n", &c);
      if (paletteIndex >= config.paletteOffset && index < config.maxColors) {
	ic->palette[index].r = (c >> 8 & 0xF) << 4;
	ic->palette[index].g = (c >> 4 & 0xF) << 4;
	ic->palette[index].b = (c >> 0 & 0xF) << 4;
	ic->palette[index].a = 255;
	
	if (config.verbose) {
	  printf("%03d %03d %03d\n", ic->palette[index].r, ic->palette[index].g, ic->palette[index].b);
	}
	index++;
      }
    } else {
      if (paletteIndex >= config.paletteOffset && index < config.maxColors) {
	if (index == config.overrideIndex) {
	  ic->palette[index] = config.overrideColor;
	} else {
	  sscanf(buffer, "%d %d %d %d",
		 &ic->palette[index].r,
		 &ic->palette[index].g,
		 &ic->palette[index].b,
		 &ic->palette[index].a);

	if (config.verbose) {
	  printf("b: r: %03d g: %03d b: %03d a: %03d\n", ic->palette[index].r, ic->palette[index].g, ic->palette[index].b, ic->palette[index].a);
	}

	  ic->palette[index].r = CLAMP8(((ic->palette[index].r+8)>>4)<<4);
	  ic->palette[index].g = CLAMP8(((ic->palette[index].g+8)>>4)<<4);
	  ic->palette[index].b = CLAMP8(((ic->palette[index].b+8)>>4)<<4);

	}
	
	if (config.verbose) {
	  printf("a: r: %03d g: %03d b: %03d a: %03d\n", ic->palette[index].r, ic->palette[index].g, ic->palette[index].b, ic->palette[index].a);
	}
	index++;
      }   
    }
  }

  ic->numColors = paletteIndex;
}


void 
palette_output(imagecon_image_t* ic, char* outFilename)
{
  if (config.verbose) {
    printf("outputPalette...%d colors -> %s\n", ic->numColors, outFilename);
  }

  FILE* fp = 0;
  FILE* paletteFP = 0;
  FILE* paletteAsmFP = 0;
  FILE* paletteGreyFP = 0;
  FILE* paletteGreyPaletteFP = 0;
  FILE* paletteTableFP = 0;
  FILE* paletteGreyTableFP = 0;
  FILE* paletteGreyCopperFP = 0;
  FILE* paletteC = 0;  

  if (config.outputCopperList) {
    fp = file_openWrite("%s-copper-list.s", outFilename);
    paletteTableFP = file_openWrite("%s-palette-table.s", outFilename);
  }

  if (config.outputPalette) {
    paletteFP = file_openWrite("%s.pal", outFilename);
    paletteC = file_openWrite("%s/palette_%s.h", dirname(strdup(outFilename)), basename(strdup(outFilename)));    
  }

  if (config.outputPaletteGrey) {
    paletteGreyFP = file_openWrite("%s-grey.s", outFilename);
    paletteGreyPaletteFP = file_openWrite("%s-grey.pal", outFilename);
    paletteGreyTableFP = file_openWrite("%s-grey-table.s", outFilename);
    paletteGreyCopperFP = file_openWrite("%s-grey-copper.s", outFilename);
    // fprintf(paletteGreyFP, "\tmovem.l d0-a6,-(sp)\n\tlea CUSTOM,a6\n");
  }

  if (config.outputPaletteAsm) {
     if (config.verbose) {
       printf("outputPaletteAsm: %s\n", outFilename);
     }
    paletteAsmFP = file_openWrite("%s-palette.s", outFilename);
    //  fprintf(paletteAsmFP, "\tmovem.l d0-a6,-(sp)\n\tlea CUSTOM,a6\n");
  }

  if (config.verbose) {
    printf("outputPalette:\n");
  }
  
  for (int i = 0; i < (config.ehbMode ? ic->numColors/2 : ic->numColors); i++) {
    if (config.darken != 0) {
      ic->palette[i].r *= config.darken;
      ic->palette[i].g *= config.darken;
      ic->palette[i].b *= config.darken;
    }
    if (config.verbose) {
      printf("%02d: hex=%03x r=%03d g=%03d b=%03d a=%03d\n", i , RGB24TORGB12(ic->palette[i].r) << 8 | RGB24TORGB12(ic->palette[i].g) << 4 | RGB24TORGB12(ic->palette[i].b), ic->palette[i].r, ic->palette[i].g, ic->palette[i].b, ic->palette[i].a);
    }

    if (paletteC) {
      fprintf(paletteC, "\t0x%x,\n",  RGB24TORGB12(ic->palette[i].r) << 8 | RGB24TORGB12(ic->palette[i].g) << 4 | RGB24TORGB12(ic->palette[i].b));
    }
    
    if (paletteFP) {

      if (!config.fullColorPaletteFile) {
	fprintf(paletteFP, "%03x\n",  RGB24TORGB12(ic->palette[i].r) << 8 | RGB24TORGB12(ic->palette[i].g) << 4 | RGB24TORGB12(ic->palette[i].b));
      } else {
	fprintf(paletteFP, "%03d %03d %03d %03d\n",  CLAMP8(((ic->palette[i].r+8)>>4)<<4) , CLAMP8(((ic->palette[i].g+8)>>4)<<4) , CLAMP8(((ic->palette[i].b+8)>>4)<<4), ic->palette[i].a);
      }
    }
    if (paletteAsmFP) {
      fprintf(paletteAsmFP, "\tmove.w #$%03x,COLOR%02d(a6)\n",  RGB24TORGB12(ic->palette[i].r) << 8 | RGB24TORGB12(ic->palette[i].g) << 4 | RGB24TORGB12(ic->palette[i].b), i+config.paletteOffset);
    }
    if (paletteGreyFP) {
      // TODO: this is for compat, can be better
      unsigned grey = ((RGB24TORGB12(ic->palette[i].r) + RGB24TORGB12(ic->palette[i].g) + RGB24TORGB12(ic->palette[i].b))/3);
      fprintf(paletteGreyFP, "\tmove.w #$%03x,COLOR%02d(a6)\n", grey << 8 | grey << 4 | grey, i);
      grey = (((ic->palette[i].r) + (ic->palette[i].g) + (ic->palette[i].b))/3);
      fprintf(paletteGreyPaletteFP, "%03d %03d %03d 255\n", grey, grey, grey);      
    }

    if (paletteGreyTableFP) {
      // TODO: this is for compat, can be better
      unsigned grey = ((RGB24TORGB12(ic->palette[i].r) + RGB24TORGB12(ic->palette[i].g) + RGB24TORGB12(ic->palette[i].b))/3);
      fprintf(paletteGreyTableFP, "\tdc.w\t$%03x\n", grey << 8 | grey << 4 | grey);
    }

    if (paletteTableFP) {
      fprintf(paletteTableFP, "\tdc.w\t$%03x\n", RGB24TORGB12(ic->palette[i].r) << 8 | RGB24TORGB12(ic->palette[i].g) << 4 | RGB24TORGB12(ic->palette[i].b));
    }

    if (fp) {
      fprintf(fp, "\tdc.w $%x,$%x\n", 0x180+((i+config.paletteOffset)*2), RGB24TORGB12(ic->palette[i].r) << 8 | RGB24TORGB12(ic->palette[i].g) << 4 | RGB24TORGB12(ic->palette[i].b));
    }

    if (paletteGreyCopperFP) {
      unsigned grey = ((RGB24TORGB12(ic->palette[i].r) + RGB24TORGB12(ic->palette[i].g) + RGB24TORGB12(ic->palette[i].b))/3);
      fprintf(paletteGreyCopperFP, "\tdc.w $%x,$%03x\n", 0x180+((i+config.paletteOffset)*2), grey << 8 | grey << 4 | grey);
    }
  }


  if (paletteGreyTableFP) {
    fclose(paletteGreyTableFP);
  }

  if (paletteTableFP) {
    fclose(paletteTableFP);
  }

  if (paletteC) {
    fclose(paletteC);
  }

  if (paletteFP) {
    fclose(paletteFP);
  }

  if (paletteGreyFP) {
    //   fprintf(paletteGreyFP, "\tmovem.l (sp)+,d0-a6\n");
    fclose(paletteGreyFP);
  }

  if (paletteGreyPaletteFP) {
    fprintf(paletteGreyPaletteFP, "\n");
    fclose(paletteGreyPaletteFP);
  }

  if (paletteAsmFP) {
    //   fprintf(paletteAsmFP, "\tmovem.l (sp)+,d0-a6\n");
    fclose(paletteAsmFP);
  }

  if (fp) {
    fclose(fp);
  }

  if (config.verbose) {
    printf("done\n\n");
  }
}
