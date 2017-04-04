/*
 * Unloved program to convert a binary on stdin to a C include on stdout
 *
 * Jan 1999 Matt Mackall <mpm@selenic.com>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
  int ch, total = 0;

  do {
    while ((ch = getchar()) != EOF) {
      total++;
      printf("0x%02x,\n", ch);
      if (total % 16 == 0)
	break;
    }
  } while (ch != EOF);


  return 0;
}
