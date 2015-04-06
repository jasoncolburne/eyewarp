
#include "colormaps.h"

int
redPaletteSchemaPurpleHaze(
    PaletteSchema8x256* schema,
    RedContext          rCtx
    )
{
  size_t count = 3;

  static red_u32 values[3] = {
    0xFF881084,
    0xFF648B24,
    0xFF0CA890
  };
  
  static red_u8 positions[3] = { 
      0, 
     85, 
    170 
  };

  return redPaletteSchemaCreate(schema, values, positions, count, rCtx);
}

int
redPaletteSchemaRedSea(
    PaletteSchema8x256* schema,
    RedContext          rCtx
    )
{
  size_t count = 4;

  red_u32 values[4] = {
    0xFF1F0000,
    0xFF990000,
    0xFF3D0000,
    0xFF6B0000
  };
  
  red_u8 positions[4] = { 
      0, 
     64,
    128,
    192,
  };

  return redPaletteSchemaCreate(schema, values, positions, count, rCtx);
}

int
redPaletteSchemaFireStorm(
    PaletteSchema8x256* schema,
    RedContext          rCtx
    )
{
  size_t count = 7;

  static red_u32 values[7] = {
    0xFF900AE5,
    0xFFBF00BF,
    0xFFFF413E,
    0xFFBFBF00,
    0xFF40FF40,
    0xFF00BFBF,
    0xFF4040FF
  };
  
  static red_u8 positions[7] = { 
      0, 
     36, 
     73,
    109,
    146,
    182,
    219
  };

  return redPaletteSchemaCreate(schema, values, positions, count, rCtx);
}
