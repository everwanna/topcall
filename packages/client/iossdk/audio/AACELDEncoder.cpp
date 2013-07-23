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

#include "AACELDEncoder.h"


/* Internal representation of an AAC-ELD encoder abstracting from the AudioConverter API */
typedef struct AACELDEncoder_ {
  AudioStreamBasicDescription  sourceFormat;
  AudioStreamBasicDescription  destinationFormat;
  AudioConverterRef            audioConverter;
  AudioBuffer                 *currentSampleBuffer;
  
  UInt32                       bytesToEncode;
  void                        *encoderBuffer;
  AudioStreamPacketDescription packetDesc[1];
  
  Float64                      samplingRate;
  UInt32                       inChannels;
  UInt32                       outChannels;
  UInt32                       frameSize;
  UInt32                       bitrate;
  UInt32                       maxOutputPacketSize;
} AACELDEncoder;


AACELDEncoder* CreateAACELDEncoder()
{
  /* Create an initialize a new instance of the encoder object */
  AACELDEncoder *encoder = (AACELDEncoder*)malloc(sizeof(AACELDEncoder));
  
  memset(&(encoder->sourceFormat), 0, sizeof(AudioStreamBasicDescription));
  memset(&(encoder->destinationFormat), 0, sizeof(AudioStreamBasicDescription));
  
  encoder->currentSampleBuffer = NULL;
  encoder->bytesToEncode       = 0;
  encoder->encoderBuffer       = NULL;
  encoder->samplingRate        = 0;
  encoder->inChannels          = 0;
  encoder->outChannels         = 0;
  encoder->frameSize           = 0;
  encoder->maxOutputPacketSize = 0;
  
  return encoder;
}

void DestroyAACELDEncoder(AACELDEncoder *encoder, MagicCookie *cookie)
{
  /* Clean up */
  AudioConverterDispose(encoder->audioConverter);
  free(encoder->encoderBuffer);
  free(encoder);
  free(cookie->data);
  cookie->byteSize = 0;
}

int InitAACELDEncoder(AACELDEncoder *encoder, EncoderProperties props, MagicCookie *outCookie)
{
  /* Copy the provided encoder properties */
  encoder->inChannels   = props.inChannels;
  encoder->outChannels  = props.outChannels;
  encoder->samplingRate = props.samplingRate;
  encoder->frameSize    = props.frameSize;
  encoder->bitrate      = props.bitrate;
  
  /* Convenience macro to fill out the ASBD structure.
     Available only when __cplusplus is defined! */
  FillOutASBDForLPCM(encoder->sourceFormat, 
                     encoder->samplingRate, 
                     encoder->inChannels, 
                     8*sizeof(AudioSampleType), 
                     8*sizeof(AudioSampleType), 
                     false, 
                     false);
 
  /* Set the format parameters for AAC-ELD encoding. */
  encoder->destinationFormat.mFormatID         = kAudioFormatMPEG4AAC_ELD;
  encoder->destinationFormat.mChannelsPerFrame = encoder->outChannels;
  encoder->destinationFormat.mSampleRate       = encoder->samplingRate;
 
  /* Get the size of the formatinfo structure */
  UInt32 dataSize = sizeof(encoder->destinationFormat);
  
  /* Request the propertie from CoreAudio */
  AudioFormatGetProperty(kAudioFormatProperty_FormatInfo, 
                         0, 
                         NULL, 
                         &dataSize, 
                         &(encoder->destinationFormat));
  
  /* Create a new audio converter */
  AudioConverterNew(&(encoder->sourceFormat), 
                    &(encoder->destinationFormat), 
                    &(encoder->audioConverter));
  
  if (!encoder->audioConverter)
  {
    return -1;
  }
  
  /* Try to set the desired output bitrate */
  UInt32 outputBitrate = encoder->bitrate;
  dataSize = sizeof(outputBitrate);
  
  AudioConverterSetProperty(encoder->audioConverter, 
                            kAudioConverterEncodeBitRate, 
                            dataSize, 
                            &outputBitrate);
  
  /* Query the maximum possible output packet size */
  if (encoder->destinationFormat.mBytesPerPacket == 0) 
  {
    UInt32 maxOutputSizePerPacket = 0;
    dataSize = sizeof(maxOutputSizePerPacket);
    AudioConverterGetProperty(encoder->audioConverter, 
                              kAudioConverterPropertyMaximumOutputPacketSize, 
                              &dataSize, 
                              &maxOutputSizePerPacket);
    encoder->maxOutputPacketSize = maxOutputSizePerPacket;
  }
  else
  {
    encoder->maxOutputPacketSize = encoder->destinationFormat.mBytesPerPacket;
  }
  
  /* Fetch the Magic Cookie from the ELD implementation */
  UInt32 cookieSize = 0;
  AudioConverterGetPropertyInfo(encoder->audioConverter, 
                                kAudioConverterCompressionMagicCookie, 
                                &cookieSize, 
                                NULL);
  
  char* cookie = (char*)malloc(cookieSize*sizeof(char));
  AudioConverterGetProperty(encoder->audioConverter, 
                            kAudioConverterCompressionMagicCookie, 
                            &cookieSize, 
                            cookie);
  
  outCookie->data     = cookie;
  outCookie->byteSize = cookieSize;
  
  /* Prepare the temporary AU buffer for encoding */
  encoder->encoderBuffer = malloc(encoder->maxOutputPacketSize);
  
  return 0;
}


static OSStatus encodeProc(AudioConverterRef inAudioConverter, 
                           UInt32 *ioNumberDataPackets, 
                           AudioBufferList *ioData, 
                           AudioStreamPacketDescription **outDataPacketDescription, 
                           void *inUserData)
{
  /* Get the current encoder state from the inUserData parameter */
  AACELDEncoder *encoder = (AACELDEncoder*) inUserData;
  
  /* Compute the maximum number of output packets */
  UInt32 maxPackets = encoder->bytesToEncode / encoder->sourceFormat.mBytesPerPacket;
  
  if (*ioNumberDataPackets > maxPackets)
  {
    /* If requested number of packets is bigger, adjust */
    *ioNumberDataPackets = maxPackets;
  }
  
  /* Check to make sure we have only one audio buffer */
  if (ioData->mNumberBuffers != 1)
  {
    return 1;
  }
  
  /* Set the data to be encoded */
  ioData->mBuffers[0].mDataByteSize   = encoder->currentSampleBuffer->mDataByteSize;
  ioData->mBuffers[0].mData           = encoder->currentSampleBuffer->mData;
  ioData->mBuffers[0].mNumberChannels = encoder->currentSampleBuffer->mNumberChannels;
  
  if (outDataPacketDescription)
  {
    *outDataPacketDescription = NULL;
  }

  if (encoder->bytesToEncode == 0)
  {
    // We are currently out of data but want to keep on processing 
    // See Apple Technical Q&A QA1317
    return 1; 
  }
  
  encoder->bytesToEncode = 0;
  
    
  return noErr;
}


int EncodeAACELD(AACELDEncoder *encoder, AudioBuffer *inSamples, EncodedAudioBuffer *outData)
{
  /* Clear the encoder buffer */
  memset(encoder->encoderBuffer, 0, sizeof(encoder->maxOutputPacketSize));
  
  /* Keep a reference to the samples that should be encoded */
  encoder->currentSampleBuffer = inSamples;
  encoder->bytesToEncode       = inSamples->mDataByteSize;
  
  UInt32 numOutputDataPackets = 1;
  
  AudioStreamPacketDescription outPacketDesc[1];
  
  /* Create the output buffer list */
  AudioBufferList outBufferList;
  outBufferList.mNumberBuffers = 1;
  outBufferList.mBuffers[0].mNumberChannels = encoder->outChannels;
  outBufferList.mBuffers[0].mDataByteSize   = encoder->maxOutputPacketSize;
  outBufferList.mBuffers[0].mData           = encoder->encoderBuffer;

  /* Start the encoding process */
  OSStatus status = AudioConverterFillComplexBuffer(encoder->audioConverter,
                                                    encodeProc, 
                                                    encoder, 
                                                    &numOutputDataPackets, 
                                                    &outBufferList, 
                                                    outPacketDesc);
  
  if (status != noErr)
  {
    return -1;
  }
  
  /* Set the ouput data */
  outData->mChannels      = encoder->outChannels;
  outData->data           = encoder->encoderBuffer;
  outData->mDataBytesSize = outPacketDesc[0].mDataByteSize;
  
  return 0;
}


