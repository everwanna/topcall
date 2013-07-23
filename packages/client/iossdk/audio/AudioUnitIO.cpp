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

#include "AudioUnitIO.h"
#include "AACELDEncoder.h"
#include "AACELDDecoder.h"

static AudioBuffer            g_inputBuffer;
static AudioBuffer            g_outputBuffer;
static AudioComponentInstance g_audioUnit;
static AudioUnitElement       g_outputBus      = 0; 
static AudioUnitElement       g_inputBus       = 1;
static UInt32                 g_outChannels    = 2;
static UInt32                 g_inChannels     = 1;
static UInt32                 g_frameSize      = 512;
static UInt32                 g_inputByteSize  = 0; 
static UInt32                 g_outputByteSize = 0; 
static unsigned int           g_initialized    = 0;
static AACELDEncoder         *g_encoder        = NULL;
static AACELDDecoder         *g_decoder        = NULL;
static MagicCookie            g_cookie;

/* Initialize the AAC-ELD encoder and decoder */
void InitAACELD()
{
  EncoderProperties p;
  p.samplingRate = 44100.0;
  p.inChannels   = 1;
  p.outChannels  = 1;
  p.frameSize    = 512;
  p.bitrate      = 32000;
  
  g_encoder = CreateAACELDEncoder();
  InitAACELDEncoder(g_encoder, p, &g_cookie);
  
  DecoderProperties dp;
  dp.samplingRate = 44100.0;
  dp.inChannels   = 1;
  dp.outChannels  = 2;
  dp.frameSize    = p.frameSize;
  
  g_decoder = CreateAACELDDecoder();
  InitAACELDDecoder(g_decoder, dp, &g_cookie);
}

/* Cleanup */
void DestroyAACELD()
{
  DestroyAACELDEncoder(g_encoder, &g_cookie);
  DestroyAACELDDecoder(g_decoder);
}

/* AudioSession listener callbacks - empty implementation for simplicity */
void interruptionListener(void *inClientData, 
                          UInt32 inInterruption) 
{}
void routeChangeListener(void *inClientData, 
                         AudioSessionPropertyID	inID, 
                         UInt32 inDataSize, 
                         const void *inData) 
{}

/* The render call back that is called by the audio system */
static OSStatus audioUnitRenderCallback(void                       *inRefCon, 
                                        AudioUnitRenderActionFlags *ioActionFlags,
                                        const AudioTimeStamp       *inTimeStamp,
                                        UInt32                      inBusNumber,
                                        UInt32                      inNumberOfFrames,
                                        AudioBufferList            *ioData)
{
  
  /* Get the input samples */
  AudioUnitRender(g_audioUnit,
                  ioActionFlags,
                  inTimeStamp,
                  g_inputBus,
                  inNumberOfFrames,
                  ioData);
  
  /* Copy to global input buffer */
  memcpy(g_inputBuffer.mData, ioData->mBuffers[0].mData, g_inputBuffer.mDataByteSize);
  
  /* Encode with AudioConverter */
  EncodedAudioBuffer encodedAU;
  EncodeAACELD(g_encoder, &g_inputBuffer, &encodedAU);
  
  /* Decode with AudioConverter */
  g_outputBuffer.mDataByteSize = g_outputByteSize;
  DecodeAACELD(g_decoder, &encodedAU, &g_outputBuffer);
  
  /* Copy output samples to Audio Units' IO buffer */
  ioData->mBuffers[0].mNumberChannels = g_outputBuffer.mNumberChannels;
  ioData->mBuffers[0].mDataByteSize   = g_outputBuffer.mDataByteSize;
  memcpy(ioData->mBuffers[0].mData, g_outputBuffer.mData, g_outputBuffer.mDataByteSize); 
  
  return noErr;
}


void InitAudioUnit()
{
  /* Calculate the required input and output buffer sizes */
  g_inputByteSize  = g_frameSize * g_inChannels  * sizeof(AudioSampleType);
  g_outputByteSize = g_frameSize * g_outChannels * sizeof(AudioSampleType);
  
  /* Initialize the I/O buffers */
  g_inputBuffer.mNumberChannels = g_inChannels;
  g_inputBuffer.mDataByteSize   = g_inputByteSize;
  
  if (g_initialized)
    free(g_inputBuffer.mData);
  g_inputBuffer.mData           = malloc(sizeof(unsigned char)*g_inputByteSize);
  memset(g_inputBuffer.mData, 0, g_inputByteSize);
  
  g_outputBuffer.mNumberChannels = g_outChannels;
  g_outputBuffer.mDataByteSize   = g_outputByteSize;
  if (g_initialized)
    free(g_outputBuffer.mData);
  g_outputBuffer.mData           = malloc(sizeof(unsigned char)*g_outputByteSize);
  memset(g_outputBuffer.mData, 0, g_outputByteSize);
  g_initialized = 1;
  
  /* Initialize the audio session */
  AudioSessionInitialize(NULL, NULL, interruptionListener, NULL);
  /* Activate the audio session */
  AudioSessionSetActive(TRUE);
  
  /* Enable recording for full-duplex I/O */
  UInt32 audioCategory = kAudioSessionCategory_PlayAndRecord;
  AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, 
                          sizeof(audioCategory), 
                          &audioCategory);
  /* Set the route change listener */
  AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, 
                                  routeChangeListener, 
                                  NULL);
  
  /* Set the preferred buffer time */
  Float32 preferredBufferTime = 512.0 / 44100.0;
  AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, 
                          sizeof(preferredBufferTime), 
                          &preferredBufferTime);
  
  /* Setup the audio component for I/O */
  AudioComponentDescription componentDesc;
  memset(&componentDesc, 0, sizeof(componentDesc));
  
  componentDesc.componentType         = kAudioUnitType_Output;
  componentDesc.componentSubType      = kAudioUnitSubType_RemoteIO; 
  componentDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
  
  /* Find and create the audio component */
  AudioComponent auComponent = AudioComponentFindNext(NULL, &componentDesc);
  AudioComponentInstanceNew(auComponent, &g_audioUnit);
  
  /* Enable the audio input */
  UInt32 enableAudioInput = 1;
  AudioUnitSetProperty(g_audioUnit, 
                       kAudioOutputUnitProperty_EnableIO, 
                       kAudioUnitScope_Input, 
                       g_inputBus, 
                       &enableAudioInput, 
                       sizeof(enableAudioInput));
  
  /* Setup the render callback */
  AURenderCallbackStruct renderCallbackInfo;
  renderCallbackInfo.inputProc       = audioUnitRenderCallback;
  renderCallbackInfo.inputProcRefCon = NULL;
  AudioUnitSetProperty(g_audioUnit, 
                       kAudioUnitProperty_SetRenderCallback, 
                       kAudioUnitScope_Input, 
                       g_outputBus, 
                       &renderCallbackInfo, 
                       sizeof(renderCallbackInfo));
  
  /* Set the input and output audio stream formats */
  AudioStreamBasicDescription audioFormat;
  audioFormat.mSampleRate       = 44100;
  audioFormat.mFormatID         = kAudioFormatLinearPCM;
  audioFormat.mFormatFlags      = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
  audioFormat.mFramesPerPacket  = 1;
  audioFormat.mBitsPerChannel   = 8 * sizeof(AudioSampleType);
  audioFormat.mChannelsPerFrame = g_inChannels;
  audioFormat.mBytesPerFrame    = audioFormat.mChannelsPerFrame * sizeof(AudioSampleType);
  audioFormat.mBytesPerPacket   = audioFormat.mBytesPerFrame;
  
  AudioUnitSetProperty(g_audioUnit, 
                       kAudioUnitProperty_StreamFormat, 
                       kAudioUnitScope_Output, 
                       g_inputBus, 
                       &audioFormat, 
                       sizeof(audioFormat));
  
  audioFormat.mChannelsPerFrame = g_outChannels;
  audioFormat.mBytesPerFrame    = audioFormat.mChannelsPerFrame * sizeof(AudioSampleType);
  audioFormat.mBytesPerPacket   = audioFormat.mBytesPerFrame;
  
  AudioUnitSetProperty(g_audioUnit, 
                       kAudioUnitProperty_StreamFormat, 
                       kAudioUnitScope_Input, 
                       g_outputBus, 
                       &audioFormat, 
                       sizeof(audioFormat));
  
  /* Initialize the ELD codec */
  InitAACELD();
}

/* Start and stop functions */
void StartAudioUnit()
{
  AudioUnitInitialize(g_audioUnit);
  AudioOutputUnitStart(g_audioUnit);
}

void StopAudioUnit()
{
  AudioOutputUnitStop(g_audioUnit);
  AudioUnitUninitialize(g_audioUnit);
  DestroyAACELD();
}
