/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include "he_return.h"

#include "plasma.h"

int
conjureGradientPalette(
    void*      palette,
    red_u32*   seed_colors,
    red_u8*    seed_positions,    
    size_t     seed_count
    )
{
  int i, j, k;
  int origin = 0, range = 256;

  red_u32 a = 0, b = 0;  
  red_i16 t = 0;

  red_u8* p = (red_u8*)palette;

  if (!p || !seed_colors || !seed_positions)
    return RED_ERR_NULL_POINTER;

  if (seed_count < 2 || seed_count > 256)
    return RED_ERR_INVALID_ARGUMENT;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 256; j++) {
      for (k = 0; k < seed_count; k++) {
	if (j == seed_positions[k]) {
	  int n = (k == (seed_count - 1) ? 0 : k + 1);

	  a = seed_colors[k];
	  b = seed_colors[n];
	  
	  origin =  seed_positions[k];
	  range  = (seed_positions[n] ? seed_positions[n] : 256) - j;

	  /* pointer sorcery */
	  t = (((red_u8*)(&b))[i] - ((red_u8*)(&a))[i]);
	}
      }

      p[j * 4 + i] = ((red_u8*)(&a))[i] + t * (j - origin) / range;
    }
  }

  return RED_SUCCESS;
}
