/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include "he_type.h"
#include "he_return.h"
#include "he_memory.h"

#include "red_audio.h"
#include "_red_audio.h"

int
redAudioCombiningContextCreate(
    RedAudioCombiningContext* acCtx,
    size_t     channel_count,
    red_u32    sample_rate,
    red_u32    sample_format,
    red_u32    combining_algorithm,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (!acCtx)
    return RED_ERR_NULL_POINTER;
  if (*acCtx)
    return RED_ERR_INITIALIZED_POINTER;

  /* by design, each of these flags should be a single bit,
     and fall within their designated mask */
  if (!(   POWER_OF_TWO(sample_format)
        && POWER_OF_TWO(combining_algorithm) 
        && POWER_OF_TWO(sample_format & RED_AUDIO_SAMPLE_FORMAT_MASK)
	&& POWER_OF_TWO(combining_algorithm & RED_AUDIO_COMBINE_MASK))
     ) {
    rc = RED_ERR_INVALID_ARGUMENT;
    goto end;
  }

  /* TODO: validate sample rate */

  /* TODO: this is likely too simple of a check */
  if (channel_count < 0 || channel_count > 8) {
    rc = RED_ERR_INVALID_ARGUMENT;
    goto end;
  }

  rc = redMalloc( rCtx, (void**)acCtx, sizeof(**acCtx) );
  if (rc) goto end;

  (*acCtx)->format        = sample_format | combining_algorithm;
  (*acCtx)->channel_count = channel_count;
  (*acCtx)->rCtx          = rCtx;

 end:
  if (rc && *acCtx)
    redFree( rCtx, (void**)acCtx, 0 );

  return rc;
}

int
redAudioCombiningContextDestroy(
    RedAudioCombiningContext* acCtx
    )
{
  if (!acCtx)
    return RED_ERR_NULL_POINTER;

  if (*acCtx)
    return redFree( (*acCtx)->rCtx, (void**)acCtx, 0 );

  return RED_SUCCESS;
}

static
void
rfCombineS16Naive(
    void** s,
    size_t n,
    void*  d
    )
{
  int     i = 0;
  red_i32 b = 0;

  for (; i < n; i++) b += *((red_i16*)(s[i])); 

  /* assuming two's complement */
  *((red_i16*)d) = (b > 32767 ? 32767 : (b < -32768 ? -32768 : b));
}

typedef void (*redfuncCombineSamples)(void**, size_t, void*); 

int
redAudioCombineFrames(
    void** source_frames,
    size_t source_frame_count,
    void*  destination_frame,
    size_t sample_count,
    RedAudioCombiningContext acCtx
    )
{
  int i = 0, j = 0;

  red_u8* s[6] = {0};
  red_u8* d = (red_u8*)destination_frame;

  size_t n = 1;

  redfuncCombineSamples rfCombine = NULL;

  /* arbitrary number of voices */
  if (source_frame_count < 1 || source_frame_count > 6)
    return RED_ERR_INVALID_ARGUMENT;

  switch(acCtx->format & (RED_AUDIO_SAMPLE_FORMAT_MASK | RED_AUDIO_COMBINE_MASK)) {
  case RED_AUDIO_SAMPLE_FORMAT_S16 | RED_AUDIO_COMBINE_NAIVE:
    rfCombine = rfCombineS16Naive;
    n = sizeof(red_i16);
    break;
  default:
    return RED_ERR_NOT_SUPPORTED;
  }


  for (j = 0; j < source_frame_count; j++)
    s[j] = (red_u8*)source_frames[j];

  for (i = 0; i < sample_count * acCtx->channel_count; i++) {
    rfCombine((void**)s, source_frame_count, (void*)d);
    for (j = 0; j < source_frame_count; j++) s[j] += n;
    d += n;
  }

  return RED_SUCCESS;
}
