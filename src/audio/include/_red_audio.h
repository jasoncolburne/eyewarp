/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#ifndef ___RED_AUDIO_H__
#define ___RED_AUDIO_H__

#include "he_context.h"

struct _RedAudioSignalContext {
  red_u32    format;
  red_u32    sample_rate;
  size_t     channel_count;

  float      frequency;
  float      amplitude;

  float      time_offset;

  RedContext rCtx;
};

/* TODO: consider policies for reconciliation of 
 *       channel count disparity
 */
struct _RedAudioResamplingContext {
  red_u32    input_format;
  red_u32    input_sample_rate;
  size_t     input_channel_count;

  red_u32    output_format;
  red_u32    output_sample_rate;
  size_t     output_channel_count;

  RedContext rCtx;
};

struct _RedAudioCombiningContext {
  red_u32    format;
  size_t     channel_count;

  RedContext rCtx;
};

#endif /* ___RED_AUDIO_H__ */
