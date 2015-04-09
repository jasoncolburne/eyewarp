/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include "red_return.h"
#include "red_memory.h"

#include "red_audio.h"
#include "_red_audio.h"

int
redAudioResamplingContextCreate(
    RedAudioResamplingContext* arCtx, 		  
    size_t     input_channel_count,
    red_u32    input_sample_format,
    red_u32    input_sample_rate,
    red_u32    input_frame_format,
    size_t     output_channel_count,
    red_u32    output_sample_format,
    red_u32    output_sample_rate,
    red_u32    output_frame_format,
    red_u32    resampling_algorithm,
    RedContext rCtx
    )
{
  return RED_NOT_IMPLEMENTED;
}

int
redAudioResamplingContextDestroy(
    RedAudioResamplingContext* arCtx
    )
{
  if (!arCtx)
    return RED_ERR_NULL_POINTER;

  if (*arCtx)
    return redFree( (*arCtx)->rCtx, (void**)arCtx, 0 );

  return RED_SUCCESS;
}

int
redAudioResampleFrame(
    void*  source,
    void*  destination,
    size_t sample_count, /* TODO: reconsider this as input or output */
    RedAudioResamplingContext arCtx
    )
{
  return RED_NOT_IMPLEMENTED;
}
