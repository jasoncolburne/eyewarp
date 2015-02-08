/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include "he_type.h"
#include "he_context.h"

/**
 * constructs a 256 entry circularly gradient ARGB palette
 * packing is as per libav/AV_PIX_FMT_RGB32/AV_PIX_FMT_PAL8
 */
extern
int
conjureGradientPalette(
    void*      palette,
    red_u32*   seed_colors,
    red_u8*    seed_positions,    
    size_t     seed_count
    );

/**
 * generate a plasma field width x height in size 
 * 
 *
 * surface should be initialized with for at least width x height characters
 */
extern
int
conjurePlasmaSurface(
    void*      surface,
    size_t     width,
    size_t     height,
    float      grain,
    red_u32    seed
    /*    RedContext rCtx*/
    );
