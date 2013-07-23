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

#ifndef AACELD_DECODER_H
#define AACELD_DECODER_H

#include "AACELDEncoder.h"

/* Opaque structure to keep the internal decoder state. */
typedef struct AACELDDecoder_ AACELDDecoder;

/* Structure to keep the decoder configuration */
typedef struct DecoderProperties_
{
  Float64 samplingRate;
  UInt32  inChannels;
  UInt32  outChannels;
  UInt32  frameSize;
} DecoderProperties;

/* Create a new AAC-ELD decoder */
AACELDDecoder* CreateAACELDDecoder();
/* Initialize the decoder and set the magic cookie */
int  InitAACELDDecoder(AACELDDecoder* decoder, DecoderProperties props, const MagicCookie *cookie);
/* Decode one AAC-ELD AU to one LPCM frame (512 samples) */ 
int  DecodeAACELD(AACELDDecoder* decoder, EncodedAudioBuffer *inData, AudioBuffer *outSamples);
/* Destroy the decoder and free associated memory */
void DestroyAACELDDecoder(AACELDDecoder* decoder);

#endif /* AACELD_DECODER_H */