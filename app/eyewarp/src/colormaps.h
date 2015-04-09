

#ifndef __COLORMAPS_H__
#define __COLORMAPS_H__

#include "red_context.h"
#include "fractal.h"

typedef int (*redPaletteSchemaFunction)(PaletteSchema8x256*, RedContext);

extern
int
redPaletteSchemaPurpleHaze(
    PaletteSchema8x256* schema,
    RedContext          rCtx
    );

extern
int
redPaletteSchemaRedSea(
    PaletteSchema8x256* schema,
    RedContext          rCtx
    );

extern
int
redPaletteSchemaFireStorm(
    PaletteSchema8x256* schema,
    RedContext          rCtx
    );

#endif /* __COLORMAPS_H__ */
