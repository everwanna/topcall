/***************************************************************************\
 *
 *               (C) copyright Fraunhofer - IIS (2012)
 *                        All Rights Reserved
 *
 *   Project:  CoreAudio AAC-ELD Example Source Code
 *
 *   By using this Example Source Code, you agree to the "Terms & Conditions 
 *   for Use of Fraunhofer Example Source Code", which is provided as a 
 *   separate document together with this file. The "Terms & Conditions for 
 *   Use of Fraunhofer Example Source Code" must be part of any redistribution
 *   of the Example Source Code or parts thereof, or modifications of the 
 *   Example Source Code.
 *
 \***************************************************************************/

#ifndef AACELD_ENCODER_H
#define AACELD_ENCODER_H

#include <AudioToolbox/AudioToolbox.h>

/* Opaque structure to keep the internal encoder state. */
typedef struct AACELDEncoder_ AACELDEncoder;

/* Structure to keep the encoder configuration */
typedef struct EncoderProperties_
{
  Float64 samplingRate;
  UInt32  inChannels;
  UInt32  outChannels;
  UInt32  frameSize;
  UInt32  bitrate;
} EncoderProperties;

/* Structure to keep the magic cookie */
typedef struct MagicCookie_
{
  void *data;
  int byteSize;
} MagicCookie;

/* Structure to keep one encoded AU */
typedef struct EncodedAudioBuffer_
{
  UInt32 mChannels;
  UInt32 mDataBytesSize;
  void *data;
} EncodedAudioBuffer;

/* Create a new AAC-ELD encoder */
AACELDEncoder* CreateAACELDEncoder();
/* Initialize the encoder and get the magic cookie */
int  InitAACELDEncoder(AACELDEncoder* encoder, EncoderProperties props, MagicCookie *outCookie);
/* Encode one LPCM frame (512 samples) to one AAC-ELD AU */ 
int  EncodeAACELD(AACELDEncoder* encoder, AudioBuffer *inSamples, EncodedAudioBuffer *outData);
/* Destroy the encoder and free associated memory */
void DestroyAACELDEncoder(AACELDEncoder *encoder, MagicCookie *cookie);

#endif /* AACELD_ENCODER_H */