#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <magick/api.h>
#include <getopt.h>

#define MAX_TILES 4096

typedef struct {
  int verbose;
  unsigned int tileWidth;
  unsigned int tileHeight;
  unsigned int imageWidth;
  unsigned int imageHeight;
  char** argv;
} config_t;


typedef struct {
  Image *image;  
  ImageInfo *info;
} image_t;

image_t image = {0};
image_t output = {0};
image_t tiles[MAX_TILES];
unsigned int tileIndex = 0;

config_t config = { 
  .verbose = 1,
  .tileWidth = 16,
  .tileHeight = 16,
  .imageWidth = 256,
  .imageHeight = 0
};

void
cleanup()
{
  if (image.image != (Image *) NULL) {
    DestroyImage(image.image);
  }

  if (image.info != (ImageInfo *) NULL) {
    DestroyImageInfo(image.info);
  }
  DestroyMagick();
}

void 
abort_(const char * s, ...)
{
  fprintf(stderr, "%s: ", config.argv[0]);
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  cleanup();
  exit(1);
}

void
usage()
{
  fprintf(stderr,
	  "%s:  --input <input.png> --output <output.png> --verbose\n", config.argv[0]);
  exit(1);
}

int add_tile(Image* tile)
{
  for (unsigned int i = 0; i < tileIndex; i++) {
    if (IsImagesEqual(tile, tiles[i].image)) {
      return 0;
    }
  }

  if (tileIndex < MAX_TILES) {
    tiles[tileIndex].image = tile;
    tileIndex++;
    return 1;
  } else {
    abort_("Too many tiles!\n");
    return 0;
  }
}

int 
main(int argc, char **argv)
{
  int c;
  char* inputFile = 0;
  char* outputFile = 0;

  ExceptionInfo  exception;

  config.argv = argv;

  InitializeMagick(NULL);
  image.info=CloneImageInfo(0);
  GetExceptionInfo(&exception);
  
  while (1) {
    static struct option long_options[] = {
      {"verbose", no_argument, &config.verbose, 1},
      {"output",  required_argument, 0, 'o'},
      {"input",   required_argument, 0, 'i'},
      {0, 0, 0, 0}
    };
    
    int option_index = 0;

    
    c = getopt_long (argc, argv, "", long_options, &option_index);
    
    if (c == -1)
      break;
    
    switch (c) {
    case 0:
      break;
    case 'i':
      inputFile = optarg;
      break;	
    case 'o':
      outputFile = optarg;
      break;	
    case '?':
      usage();
      break;	
    default:
      usage();
      break;
    }
  }

  
  if (inputFile == 0 || outputFile == 0) {
    usage();
    abort();
  }

  (void) strcpy(image.info->filename, inputFile);
  image.image = ReadImage(image.info, &exception);
  if (image.image == (Image *) NULL) {
    CatchException(&exception);
    abort_("Failed to read image %s\n", inputFile);
  }

  printf("input image loaded: %ld %ld\n", image.image->columns, image.image->rows);


  RectangleInfo rect = {
    .width = config.tileWidth,
    .height = config.tileHeight
  };
  

  unsigned count = 0;
  for (unsigned int x = 0; x < image.image->columns-config.tileWidth; x+= config.tileWidth) {
    for (unsigned int y = 0; y <= image.image->rows-config.tileHeight; y+= config.tileHeight) {
      rect.x = x;
      rect.y = y;
      Image* tile = CropImage(image.image, &rect, &exception);
      if (!tile) {
	CatchException(&exception);
	abort_("%d %d Image* tile = CropImage(image.image, &rect, &exception); failed\n", x, y);
      }
      if (!add_tile(tile)) {
	DestroyImage(tile);
      }
      count++;
    }
  }

  printf("found %d unique tiles out of %d\n", tileIndex, count);



  output.info = CloneImageInfo(image.info);
  Image* rows[MAX_TILES];
  
  unsigned int index = 0;
  unsigned int r = 0;
  while (index < tileIndex) {
    unsigned int start = index;
    for (unsigned int i = 0; i < config.imageWidth/config.tileWidth; i++) {
      if (index+1 < tileIndex) {
	tiles[index].image->next = tiles[index+1].image;
	index++;
      } else {
	index++;
	break;
      }
    }
    if (!(rows[r] = AppendImages(tiles[start].image, 0, &exception))) {
      CatchException(&exception);
    }
    r++;
  }

  printf("%d rows\n", r);
  
  for (unsigned int i = 0; i < r-1; i++) {
    rows[i]->next = rows[i+1];
  }

  if (!(output.image = AppendImages(rows[0], 1, &exception))) {
    CatchException(&exception);
  }

  strcpy(output.image->filename, outputFile);

  if (!WriteImage(output.info, output.image)) {
    CatchException(&output.image->exception);
    abort_("Failed to write image %s\n", outputFile);
  }

  /*
  for (int x = config.x, count = 0; x < config.x+(config.cols*config.dx); x += config.dx) {
    for (int y = config.y; y < config.y+(config.rows*config.dy); y += config.dy, count++) {
      RectangleInfo rect = {
	.x = x,
	.y =  y,
	.width = config.width,
	.height = config.height
      };
      
      if (config.verbose) {
	printf("row %d cols %d %ld %ld %ld %ld\n", (y-config.y)/config.dy, (x-config.x)/config.dx, rect.x, rect.y, rect.width, rect.height);
      }
      
      image.croppedImage = CropImage(image.image, &rect, &exception);

      if (config.rows > 1 || config.cols > 1) {
	sprintf(image.croppedImage->filename, "%s_%d.png", outputFile, count);
      } else {
	strcpy(image.croppedImage->filename, outputFile);
      }

      if (!WriteImage(image.imageInfo, image.croppedImage)) {
	CatchException(&image.croppedImage->exception);
	abort_("Failed to write image %d\n", outputFile);
      }

      if (image.croppedImage != (Image *)NULL) {
	DestroyImage(image.croppedImage);
      }      
    }
    }*/

  cleanup();

  return 0;
}
