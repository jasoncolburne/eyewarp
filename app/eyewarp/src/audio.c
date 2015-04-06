

#if 0
int
candlelitOverlayCreate(
    RedContext rCtx
    )
{
			    
  
  rc = redAudioCombiningContextCreate(&acCtx,
				      1,     /* channel */
				      44100, /* rate */
				      RED_AUDIO_SAMPLE_FORMAT_S16,
				      RED_AUDIO_COMBINE_NAIVE,
				      rCtx);
  if (rc) goto end;

  return 0;
}
#endif
