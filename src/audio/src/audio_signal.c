/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include <math.h>
#include <stdio.h>

#include "he_return.h"
#include "he_memory.h"

#include "red_audio.h"
#include "_red_audio.h"

int
redAudioSignalContextCreate(
    RedAudioSignalContext* asCtx,
    size_t     channel_count,
    red_u32    sample_rate,
    red_u32    sample_format,
    red_u32    frame_format,
    red_u32    signal_type,
    float      frequency,
    float      amplitude,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  if (!asCtx)
    return RED_ERR_NULL_POINTER;
  if (*asCtx)
    return RED_ERR_INITIALIZED_POINTER;

  /* by design, each of these flags should be a single bit,
     and fall within their designated mask */
  if (!(   POWER_OF_TWO(sample_format)
        && POWER_OF_TWO(frame_format)
	&& POWER_OF_TWO(signal_type)   
        && POWER_OF_TWO(sample_format & RED_AUDIO_SAMPLE_FORMAT_MASK)
	&& POWER_OF_TWO(frame_format  & RED_AUDIO_FRAME_FORMAT_MASK)
	&& POWER_OF_TWO(signal_type   & RED_AUDIO_SIGNAL_MASK))
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

  rc = redMalloc( rCtx, (void**)asCtx, sizeof(**asCtx) );
  if (rc) goto end;

  (*asCtx)->format        = sample_format | frame_format | signal_type;
  (*asCtx)->sample_rate   = sample_rate;
  (*asCtx)->channel_count = channel_count;
  (*asCtx)->frequency     = frequency;
  (*asCtx)->amplitude     = amplitude;
  (*asCtx)->time_offset   = 0.0;
  (*asCtx)->rCtx          = rCtx;

 end:
  if (rc && *asCtx)
    redFree( rCtx, (void**)asCtx, 0 );

  return rc;
}

int
redAudioSignalContextDestroy(
    RedAudioSignalContext* asCtx
    )
{
  if (!asCtx)
    return RED_ERR_NULL_POINTER;

  if (*asCtx)
    return redFree( (*asCtx)->rCtx, (void**)asCtx, 0 );

  return RED_SUCCESS;
}


static
float
rfSineMap(
    float frequency,
    float time
    )
{
  return sin( 2 * M_PI * frequency * time );
}

static
float
rfSquareMap(
    float frequency,
    float time
    )
{
  return (rfSineMap(frequency, time) >= 0 ? 1.0 : -1.0);
}

/*   */

typedef float (*redfuncSignal)(float, float);

int
redAudioSignalConjureFrame(
    void*                 signal_frame,
    size_t                sample_count,
    RedAudioSignalContext asCtx
    )
{
  int    i = 0,   j = 0;
  float  y = 0.0, t = 0.0;

  red_i16* p16 = (red_i16*)signal_frame;

  /* this convention turned into an unfortunately ambiguous name */
  redfuncSignal rfSignal = NULL;

  if (!asCtx)
    return RED_ERR_NULL_CONTEXT;

  if (!signal_frame)
    return RED_ERR_NULL_POINTER;

  switch(asCtx->format & RED_AUDIO_SIGNAL_MASK) {
  case RED_AUDIO_SIGNAL_SINE:
    rfSignal = rfSineMap;
    break;
  case RED_AUDIO_SIGNAL_SQUARE:
    rfSignal = rfSquareMap;
    break;
  default:
    return RED_NOT_IMPLEMENTED;
  }

  for (i = 0; i < sample_count; i++) {
    /* i considered computing d = 1/sample_rate for efficiency,
       but that seems likely to introduce rounding error and
       artifacts at frame boundaries (or inaccurate pitch, if
       the offset was adjusted to eliminate artifacts), i suppose
       i could compute the error */
    t = asCtx->time_offset + (float)i / (float)(asCtx->sample_rate); 
    y = rfSignal(asCtx->frequency, t) * asCtx->amplitude;
    
    switch(asCtx->format & (RED_AUDIO_SAMPLE_FORMAT_MASK | RED_AUDIO_FRAME_FORMAT_MASK)) {
    case RED_AUDIO_SAMPLE_FORMAT_S16 | RED_AUDIO_FRAME_FORMAT_INTERLEAVED:
      for (j = 0; j < asCtx->channel_count; j++)
	p16[i * asCtx->channel_count + j] = y * 32767;
      break;
    default:
      return RED_NOT_IMPLEMENTED;
    }
  }

  asCtx->time_offset += (float)sample_count / (float)(asCtx->sample_rate);

  return RED_SUCCESS;
}


/*

gotos - i use them to fall through on error, and code allocs to
        magically free themselves if badness occurs

I assume that the sources and destination are all the same format
in the combine step.

I'd profile if performance wasn't adequate, this is a very naive
implementation but should be quite readable.

If we wanted to optimize for a single use case we could use build
flags to conditionally compile the exact format we were concerned
with. 

*/
