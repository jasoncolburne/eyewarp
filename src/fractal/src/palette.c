/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include "red_return.h"
#include "red_memory.h"

#include "fractal.h"

int
redPaletteSchemaCreate(
    PaletteSchema8x256* schema,
    red_u32*            values,
    red_u8*             positions,    
    size_t              count,
    RedContext          rCtx
    )
{
  int rc = RED_SUCCESS;

  if (!schema || !values || !positions) return RED_ERR_NULL_POINTER;

  if (!rCtx)     return RED_ERR_NULL_CONTEXT;
  if (*schema)   return RED_ERR_INITIALIZED_POINTER;
  if (count < 2) return RED_ERR_INVALID_ARGUMENT;

  rc = redMalloc( rCtx, (void**)schema, sizeof(**schema) );
  if (rc) goto end;
  
  (*schema)->count     = count;
  (*schema)->values    = values;
  (*schema)->positions = positions;

  (*schema)->rCtx      = rCtx;

 end:
  if (rc && *schema)
    redFree( rCtx, (void**)schema, 0 );

  return rc;
}

int
redPaletteSchemaDestroy(
    PaletteSchema8x256* schema
    )
{
  if (!schema)
    return RED_ERR_NULL_POINTER;

  if (*schema)
    return redFree( (*schema)->rCtx, (void**)schema, 0 );

  return RED_SUCCESS;  
}

int
conjureGradientPalette(
    void*              palette,
    PaletteSchema8x256 schema    
    )
{
  int i, j, k, n;
  int origin = 0, range = 256;

  red_u32 a = 0, b = 0;  
  red_i16 t = 0;

  red_u8* p = (red_u8*)palette;

  if (!p)
    return RED_ERR_NULL_POINTER;
  if (!schema)
    return RED_ERR_NULL_CONTEXT;
  if (schema->count < 2 || schema->count > 256)
    return RED_ERR_INVALID_ARGUMENT;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 256; j++) {
      for (k = 0; k < schema->count; k++) {
	if (j == schema->positions[k]) {
	  n = (k == (schema->count - 1) ? 0 : k + 1);

	  a = schema->values[k];
	  b = schema->values[n];
	  
	  origin =  schema->positions[k];
	  range  = (schema->positions[n] ? schema->positions[n] : 256) - j;

	  /* pointer sorcery */
	  t = (((red_u8*)(&b))[i] - ((red_u8*)(&a))[i]);
	}
      }

      p[j * 4 + i] = ((red_u8*)(&a))[i] + t * (j - origin) / range;
    }
  }

  return RED_SUCCESS;
}
