/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#ifndef __FRACTAL_H__
#define __FRACTAL_H__

#include "he_type.h"
#include "he_context.h"

struct _PaletteSchema8x256 {
  size_t   count;
  red_u32* values;
  red_u8*  positions;

  RedContext rCtx;
};
typedef struct _PaletteSchema8x256* PaletteSchema8x256;

extern
int
redPaletteSchemaCreate(
    PaletteSchema8x256* schema,
    red_u32*            values,
    red_u8*             positions,    
    size_t              count,
    RedContext          rCtx
    );

extern
int
redPaletteSchemaDestroy(
    PaletteSchema8x256* schema
    );

/**
 * constructs a 256 entry circularly gradient ARGB palette
 * packing is as per libav/AV_PIX_FMT_RGB32/AV_PIX_FMT_PAL8
 */
extern
int
conjureGradientPalette(
    void*              palette,
    PaletteSchema8x256 schema
    );

/**
 * generate a 2d diamond square fractal
 * 
 *
 * surface should be initialized and dimensions must be 2^n
 */
extern
int
conjureDiamondSquareSurface(
    red_u8*    surface,
    size_t     dimensions,
    float      grain,
    red_u32    seed
    );

#endif /* __FRACTAL_H__ */
