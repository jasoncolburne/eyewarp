

#include <stdio.h> /* temporary */

#include "red_memory.h"
#include "eyewarp.h"
#include "video_filter.h"

int
redVideoFilterApply(
    RedVideoFilter filter,
    void*          destination,
    void*          source,
    size_t         line_length
    )
{
  if (!filter)
    return RED_ERR_NULL_CONTEXT;

  return filter->fnApply(filter, destination, source, line_length);
}

int
redVideoFilterDestroy(
    RedVideoFilter* filter
    )
{
  RedContext               rCtx = NULL;
  RedVideoFilterDataPlasma data = NULL;
  
  if (!filter)
    return RED_ERR_NULL_POINTER;

  if (*filter) {
    rCtx = (*filter)->rCtx;
    data = (*filter)->data;
    
    if (!rCtx)
      return RED_ERR_NULL_CONTEXT;

    (*filter)->fnDestroy(data, rCtx);

    redFree(rCtx, (void**)&data,  0);
    redFree(rCtx, (void**)filter, 0);
  }

  return RED_SUCCESS;
}
		      
static
int
redVideoFilterPlasmaApply(
    RedVideoFilter filter,
    void*          destination,
    void*          source,
    size_t         line_length
    )
{
  

  return RED_SUCCESS;
}

static
int
redVideoFilterPlasmaDestroy(
    void*      data,
    RedContext rCtx
    )
{
  RedVideoFilterDataPlasma d = data;

  if (d) {
    redFree(rCtx, (void**)&(d->palette), 0);
    redFree(rCtx, (void**)&(d->surface), 0);
  }

  return RED_SUCCESS;
}

int
redVideoFilterPlasmaCopyPalette(
    RedVideoFilter filter,
    void*          destination
    )
{
  int    rc    = RED_SUCCESS;
  red_u8 index = 0;

  RedVideoFilterDataPlasma data = NULL;

  if (!filter || !(filter->rCtx))
    return RED_ERR_NULL_CONTEXT;
  if (!destination)
    return RED_ERR_NULL_POINTER;

  data = filter->data;

  if (!data || !(data->palette))
    return RED_ERR_MALFORMED_CONTEXT;

  if (data->palette_offset != 0.0)
    index = (red_u8)roundf(palette->offset);

  return redMemcpy(filter->rCtx, destination, 
		   (void*)&(((red_u32)(data->palette))[index]), PALETTE_ENTRIES);
}

int
redVideoFilterPlasmaCreate(
    RedVideoFilter*    filter,
    PaletteSchema8x256 schema,
    size_t             width,
    size_t             height,
    float              grain,
    red_u32            seed,
    RedContext         rCtx
    )
{
  int rc = RED_SUCCESS;

  size_t dimensions = 0;
  
  RedVideoFilterDataPlasma data = NULL;

  if (!filter)
    return RED_ERR_NULL_POINTER;
  if (*filter)
    return RED_ERR_INITIALIZED_POINTER;
  if (!schema || !rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc(rCtx, (void**)filter, sizeof(*filter));
  if (rc) goto end;

  rc = redMalloc(rCtx, (void**)&data, sizeof(data));
  if (rc) goto end;

  (*filter)->fnApply   = redVideoFilterPlasmaApply;
  (*filter)->fnDestroy = redVideoFilterPlasmaDestroy;
  
  (*filter)->width   = width;
  (*filter)->height  = height;
  (*filter)->data    = data;
  (*filter)->rCtx    = rCtx;

  dimensions = (width > height ? height : width);
  dimensions--;
  dimensions |= dimensions >> 1;
  dimensions |= dimensions >> 2;
  dimensions |= dimensions >> 4;
  dimensions |= dimensions >> 8;
  dimensions |= dimensions >> 16;
  dimensions += 2;
  printf("w: %u, h: %u, d: %u\r\n", width, height, dimensions);

  data->palette_offset = 0.0;
  data->x_offset       = 0.0;
  data->y_offset       = 0.0;
  data->opacity        = 1.0;
  data->grain          = grain;
  data->dimensions     = dimensions;
  data->palette        = NULL;
  data->surface        = NULL;

  /* 256 color palette of 32bit values, doubled for rotation */
  rc = redMalloc(rCtx, &(data->palette), PALETTE_ENTRIES * sizeof(red_u32) * 2);
  if (rc) goto end;

  rc = redMalloc(rCtx, &(data->surface), dimensions * dimensions);
  if (rc) goto end;

  rc = conjureGradientPalette(data->palette, schema);
  if (rc) goto end;

  /* we double the palette for rotation */
  rc = redMemcpy(rCtx, (((red_u32*)(data->palette)) + PALETTE_ENTRIES),
                 data->palette, PALETTE_ENTRIES * sizeof(red_u32));
  if (rc) goto end;
  
  rc = conjureDiamondSquareSurface(data->surface, dimensions, grain, seed);
  if (rc) goto end;

 end:
  if (rc && *filter) {
    if (data) {
      redFree(rCtx, (void**)&(data->palette),   0);
      redFree(rCtx, (void**)&(data->surface),   0);
      redFree(rCtx, (void**)&((*filter)->data), 0);
    }

    redFree(rCtx, (void**)filter, 0);
  }

  return rc;
}
