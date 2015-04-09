/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include <stdio.h>
#include <math.h>

#include "red_memory.h"

#include "fractal.h"

#define POWER_OF_TWO(x) (((x)!=0) && !((x)&((x)-1)))

static
int
partitionSquare(
    red_u8* square,
    size_t  side_length,
    size_t  line_length,
    float   grain,
    red_u8  quadrant
    )
{
  red_u8 *a, *b, *c, *d, *w, *x, *y, *z, *p;
  red_u16 u = 0;

  size_t slm1        = side_length - 1;
  size_t next_length = slm1 / 2 + 1;

  double n = log(128) / log((line_length - 3) / 2);

  if (!square)
    return RED_ERR_NULL_POINTER;

  /*******
   *
   * A-----X-----B
   * |           |
   * |  0     1  |
   * W     P     Y
   * |  3     2  |
   * |           |
   * D-----Z-----C
   *
   */

  /* corners */
  a = square;
  b = square + slm1;
  c = square + slm1 * (line_length + 1);
  d = square + slm1 * line_length;

  /* center */
  p = square + slm1 * (line_length + 1) / 2;
  
  /* diamond */
  w = square + slm1 * line_length / 2; 
  x = square + slm1 / 2;
  y = square + slm1 * (line_length + 2) / 2;
  z = square + slm1 / 2 + slm1 * line_length;

  if (side_length == line_length) {
    *a = rand() % 256;
    *b = rand() % 256;
    *c = rand() % 256; 
    *d = rand() % 256;
  }

  u = pow((side_length - 3) / 2, n) / grain;

  *w = (*d + *a) / 2;
  *x = (*a + *b) / 2;
  *y = (*b + *c) / 2;
  *z = (*c + *d) / 2;  

  *p = (*a + *b + *c + *d) / 4 + (u ? rand() % (2 * u) - u : 0);

  /* recurse now... */

  srand(rand());

  if (next_length > 2) {
    partitionSquare(a, next_length, line_length, grain, 0);
    partitionSquare(x, next_length, line_length, grain, 1);
    partitionSquare(p, next_length, line_length, grain, 2);
    partitionSquare(w, next_length, line_length, grain, 3);
  }

  return RED_SUCCESS;
}

int
conjureDiamondSquareSurface(
    red_u8*    square_surface,
    size_t     side_length,
    float      grain,
    red_u32    seed
    )
{
  if (!square_surface)
    return RED_ERR_NULL_POINTER;

  if (side_length > 2049 || side_length < 8 || !POWER_OF_TWO(side_length - 1))
    return RED_ERR_INVALID_ARGUMENT;

  srand(seed);

  return partitionSquare(square_surface, side_length, side_length, grain, 0);
}
