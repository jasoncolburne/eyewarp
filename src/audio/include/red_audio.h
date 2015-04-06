/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 */

#include "he_type.h"
#include "he_context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: move this somewhere accessible by all modules */
#define POWER_OF_TWO(x) (((x)!=0) && !((x)&((x)-1)))

struct _RedAudioResamplingContext;
typedef struct _RedAudioResamplingContext* RedAudioResamplingContext;

struct _RedAudioCombiningContext;
typedef struct _RedAudioCombiningContext*  RedAudioCombiningContext;

struct _RedAudioSignalContext;
typedef struct _RedAudioSignalContext*     RedAudioSignalContext;

/* Audio specific error placeholder */
#define RED_ERR_BASE_AUDIO                     0xA000

/* Sample formats */
#define RED_AUDIO_SAMPLE_FORMAT_MASK       0x000000FF
/* Fixed */
#define RED_AUDIO_SAMPLE_FORMAT_U8         0x00000001
#define RED_AUDIO_SAMPLE_FORMAT_S16        0x00000002
#define RED_AUDIO_SAMPLE_FORMAT_S32        0x00000004
/* Floating Point, probably poorly named */
#define RED_AUDIO_SAMPLE_FORMAT_F32        0x00000010
#define RED_AUDIO_SAMPLE_FORMAT_F64        0x00000020

/* Frame formats */
#define RED_AUDIO_FRAME_FORMAT_MASK        0x0000FF00
#define RED_AUDIO_FRAME_FORMAT_INTERLEAVED 0x00000100
#define RED_AUDIO_FRAME_FORMAT_PLANAR      0x00000200

/* Signal Types */
#define RED_AUDIO_SIGNAL_MASK              0x00FF0000
#define RED_AUDIO_SIGNAL_SINE              0x00010000
#define RED_AUDIO_SIGNAL_TRIANGLE          0x00020000
#define RED_AUDIO_SIGNAL_SAWTOOTH          0x00040000
#define RED_AUDIO_SIGNAL_SQUARE            0x00080000

/* Resampling Algorithms (TODO) */

/* Combining Algorithms */
#define RED_AUDIO_COMBINE_MASK             0x00FF0000
/* Sum and clip. */
#define RED_AUDIO_COMBINE_NAIVE            0x00010000
/* Consider relative loudness (no idea if this is handled somewhere else) */
#define RED_AUDIO_COMBINE_LOGARITHMIC      0x00020000
/* Useful for a very specific application */
#define RED_AUDIO_COMBINE_XOR              0x00800000



/* very simply signal generation
 * 
 * frequency - in Hz
 * amplitude - in [0.0, 1.0]
 */
extern
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
    );

extern
int
redAudioSignalContextDestroy(
    RedAudioSignalContext* asCtx
    );
  
/* this will generate a monophonic waveform, frame by frame
 * saving state within the signal context.
 *
 */
extern
int
redAudioSignalConjureFrame(
    void*                 signal_frame,
    size_t                sample_count,
    RedAudioSignalContext asCtx
    );


/*
 */
extern
int
redAudioResamplingContextCreate(
    RedAudioResamplingContext* arCtx, 		  
    size_t     source_channel_count,
    red_u32    source_sample_rate,
    red_u32    source_sample_format,
    red_u32    source_frame_format,
    size_t     destination_channel_count,
    red_u32    destination_sample_rate,
    red_u32    destination_sample_format,
    red_u32    destination_frame_format,
    red_u32    resampling_algorithm,
    RedContext rCtx
    );

extern
int
redAudioResamplingContextDestroy(
    RedAudioResamplingContext* arCtx
    );

/* it would be faster to resample frames during mixing in many cases,
 * but that code would be convoluted so we'll start with this.
 *
 * input - the input frame
 * ouput - an appropriately sized output frame (consider allocating)
 * arCtx - the resampling context
 */
extern
int
redAudioResampleFrame(
    void*  source,
    void*  destination,
    size_t sample_count, /* TODO: reconsider this as input or output */
    RedAudioResamplingContext arCtx
    );


/*
 */
extern
int
redAudioCombiningContextCreate(
    RedAudioCombiningContext* acCtx,
    size_t     channel_count,
    red_u32    sample_rate,
    red_u32    sample_format,
    red_u32    combining_algorithm,
    RedContext rCtx
    );

extern
int
redAudioCombiningContextDestroy(
    RedAudioCombiningContext* arCtx
    );

/* TODO: consider using varargs for n frames (only works for 2 frames)
 *
 * input_frames      - an array of pointers to input frames
 * input_frame_count - the number of input frames to combine
 * output_frame      - pre-allocated framebuffer for output
 * acCtx             - details surrounding frame format
 */
extern
int
redAudioCombineFrames(
    void** source_frames,
    size_t source_frame_count,
    void*  destination_frame,
    size_t sample_count,
    RedAudioCombiningContext acCtx
    );

#ifdef _cplusplus
}
#endif
