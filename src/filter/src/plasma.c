/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include "plasma.h"

int
conjurePlasmaSurface(
    void*      surface,
    size_t     width,
    size_t     height,
    float      grain,
    red_u32    seed
    /*RedContext rCtx*/
    )
{
  int rc = RED_SUCCESS;

  /* 
  red_u8* fractal_points = NULL;
  size_t  side_length    = 0;
  */
  if (width > 2049 || height > 2049 || width < 8 || height < 8)
    return RED_ERR_INVALID_ARGUMENT;

  if (!surface)
    return RED_ERR_NULL_POINTER;
  /*
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  */

  if (width == height && POWER_OF_TWO(width - 1)) {
    rc = conjureDiamondSquareFractal(surface, width, grain, seed);
    if (rc != RED_SUCCESS) goto end;
  } else {
  /*
    TODO: compute new width
    side_length = ??;

    rc = redMalloc(rCtx, &fractal_points, side_length * side_length);
    if (rc != RED_SUCCESS) return rc;

    rc = conjureDiamondSquareFractalSquare(surface, width + 1, grain, seed, rCtx);
    if (rc != RED_SUCCESS) return rc;
  */

    /* TODO: 
     * apply fractal grid to pixels by averaging 
     * free fractal points
     */

    /* for now we require 2^n + 1 */
    return RED_ERR_INVALID_ARGUMENT;
  }

end:
  /*
  if (fractal_points) 
    redFree(rCtx, (void**)&fractal_points, side_length * side_length);
  */
  return RED_SUCCESS;
}

