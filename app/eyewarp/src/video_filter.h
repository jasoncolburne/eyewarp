#ifndef __VIDEO_FILTER_H__
#define __VIDEO_FILTER_H__

#include "he_context.h"
#include "fractal.h"

struct _RedVideoFilter;
typedef struct _RedVideoFilter* RedVideoFilter;

typedef int (*redVideoFilterDataDestroyFunction)(void*, RedContext);
typedef int       (*redVideoFilterApplyFunction)(RedVideoFilter, void*, void*, size_t);

#define RED_VIDEO_FILTER_TYPE_PLASMA  0x0001
#define RED_VIDEO_FILTER_TYPE_LEGO    0x0002
#define RED_VIDEO_FILTER_TYPE_REPLACE 0x0003
#define RED_VIDEO_FILTER_TYPE_CYPHER  0x0004

/* TODO: privatize */
struct _RedVideoFilter {
  redVideoFilterDataDestroyFunction fnDestroy;
  redVideoFilterApplyFunction       fnApply;

  size_t width;
  size_t height;

  void*  data;

  RedContext rCtx;
};


struct _RedVideoFilterDataPlasma {
  float  grain;
  size_t dimensions;

  /* floats so we can use velocities accurately
     and round during rendering */
  float x_offset;
  float y_offset;
  float palette_offset;
  float opacity;

  void* palette;
  void* surface;
};
typedef struct _RedVideoFilterDataPlasma* RedVideoFilterDataPlasma;

/* Don't touch */
#define PALETTE_ENTRIES 256

int
redVideoFilterPlasmaCreate(
    RedVideoFilter*    filter,
    PaletteSchema8x256 schema,
    size_t             width,
    size_t             height,
    float              grain,
    red_u32            seed,
    RedContext         rCtx
    );

int
redVideoFilterDestroy(
    RedVideoFilter* filter
    );

int
redVideoFilterApply(
    RedVideoFilter filter,
    void*          destination,
    void*          source,
    size_t         line_length
    );

int
redVideoFilterPlasmaCopyPalette(
    RedVideoFilter filter,
    void*          destination
    );

#endif /* __VIDEO_FILTER_H__ */
