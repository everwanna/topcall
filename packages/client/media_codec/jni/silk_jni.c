/*
 * Copyright (C) 2009 The Sipdroid Open Source Project
 * 
 * This file is part of Sipdroid (http://www.sipdroid.org)
 * 
 * Sipdroid is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This source code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this source code; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "speex_jni.h"

#include <SKP_Silk_SDK_API.h>
#include <jni.h>
#include <string.h>
#include <unistd.h>
#include <android/log.h>

#define FRAME_LENGTH_MS         20
static int silkbps[] = {5000,  8000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 40000, 100000	};

int 	m_sampleRate;
int	m_channels = 1;
int	m_quality = 4;
int	m_inputFrameSize = 0;
void*   m_pSilkEnc = NULL;
SKP_SILK_SDK_EncControlStruct m_encControl;

void*   m_pSilkDec = NULL;
SKP_SILK_SDK_DecControlStruct m_DecControl;

void*	m_pEncSrcBuf = NULL;
void*	m_pEncDestBuf = NULL;
void*	m_pDecSrcBuf = NULL;
void*	m_pDecDestBuf = NULL;

inline int FrameLength(const unsigned char* audio, int len) {
	if(len < 2)
		return -1;

	short header = (*(short*)audio);
	if((header&0xfc00) != 0xfc00)
		return 0;

	return (header & 0x3ff);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {  
    jint result = JNI_ERR; 
    JNIEnv* env = NULL;	
	
    __android_log_print(ANDROID_LOG_INFO, "silk_jni", "silk_jni, loading . . .");  
    __android_log_print(ANDROID_LOG_INFO, "silk_jni", "silk_jni, loaded");  
  
    result = JNI_VERSION_1_4;  

    return result;  
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Silk_open
  (JNIEnv *env, jobject obj, jint sample_rate,  jint compression) {
	
	int ret;
	int encSize = 0;	

	m_sampleRate = sample_rate;
	m_channels = 1;	
	m_quality = compression;
	if( m_quality < 0 || m_quality >= 10 ) 
		m_quality = 8;
	m_inputFrameSize = m_sampleRate* FRAME_LENGTH_MS  / 1000 * 16/8;

	m_encControl.sampleRate = m_sampleRate;
	ret = SKP_Silk_SDK_Get_Encoder_Size( &encSize );
	if( ret ) {
		return -1;
	}

	m_pSilkEnc = (void*)malloc( encSize );
	if(m_pSilkEnc == NULL){
		return -1;
	}

	ret = SKP_Silk_SDK_InitEncoder( m_pSilkEnc, &m_encControl );
	if( ret ) {
		return -1;
	}
		/* Set Encoder parameters */
	m_encControl.sampleRate       = m_sampleRate;	
	m_encControl.packetSize           = m_sampleRate * FRAME_LENGTH_MS/1000;
	m_encControl.packetLossPercentage = 3;
	m_encControl.useInBandFEC         = 0;
	m_encControl.useDTX               = 1;
	m_encControl.complexity           = 0;
	m_encControl.bitRate              = silkbps[m_quality];	
	
	
	//init decoder:
	int decSizeBytes = 0;
	m_DecControl.sampleRate = m_sampleRate;
	ret = SKP_Silk_SDK_Get_Decoder_Size( &decSizeBytes );
	if( ret ) {
		return -1;
	}

	m_pSilkDec = (void*)malloc( decSizeBytes );
	if(m_pSilkDec == NULL){
		return -1;
	}
	ret = SKP_Silk_SDK_InitDecoder( m_pSilkDec );
	if( ret ) {
		return -1;
	}
	
	//init buffers:
	
	m_pEncSrcBuf = malloc(m_inputFrameSize*10);
	m_pEncDestBuf = malloc(m_inputFrameSize*10);
	m_pDecSrcBuf = malloc(m_inputFrameSize*10);
	m_pDecDestBuf = malloc(m_inputFrameSize*10);
	
	__android_log_print(ANDROID_LOG_INFO, "silk_jni", "silk_jni.open, m_inputFrameSize=%d", m_inputFrameSize); 
	
	return (jint)0;
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Silk_encode
    (JNIEnv *env, jobject obj, jbyteArray src, jint size, jbyteArray dest, jint out_size) {
	
	int in_size = size;
	(*env)->GetByteArrayRegion(env, src, 0, in_size, m_pEncSrcBuf);
	
	const unsigned char* cur =  m_pEncSrcBuf;
	const unsigned char* end =  cur + in_size;    	
	unsigned char* out_temp = m_pEncDestBuf;
	int 	nFrameLen = 0;
	short out_len_temp = 0;
	short ret = 0;
	short a = 0;
	while(cur < end)
	{
		out_len_temp = out_size;
		nFrameLen = m_inputFrameSize;
		SKP_Silk_SDK_Encode( m_pSilkEnc, &m_encControl, (short*)(cur), nFrameLen/2, (SKP_uint8 *)(out_temp+2), &out_len_temp );
		a = ((0xFC<<8) | out_len_temp);
		out_temp[0] = a&0xFF;
		out_temp[1] = (a&0xFF00)>>8;

		cur  += nFrameLen;
		in_size  -= nFrameLen;
		out_temp += (out_len_temp + 2);
		ret += (out_len_temp + 2);
	}
	(*env)->SetByteArrayRegion(env, dest, 0, ret, m_pEncDestBuf);
	__android_log_print(ANDROID_LOG_INFO, "silk_jni", "silk_jni.encode, size/out_size=%d, %d", size, ret); 

	return  ret;
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Silk_decode
    (JNIEnv *env, jobject obj, jbyteArray src, jint size, jbyteArray dest, jint out_size) {
	
	int in_size = size;
	(*env)->GetByteArrayRegion(env, src, 0, in_size, m_pDecSrcBuf);

	const unsigned char* cur =  m_pDecSrcBuf;
	const unsigned char* end =  cur + in_size;    	
	unsigned char* out_temp = m_pDecDestBuf;
	int 	nFrameLen = 0;
	short out_len_temp = 0;
	short ret = 0;
	while(cur < end)
	{		
		nFrameLen = FrameLength(cur, size);
		__android_log_print(ANDROID_LOG_INFO, "silk_jni", "silk_jni.decode, framelen=%d", nFrameLen); 
		
		SKP_Silk_SDK_Decode( m_pSilkDec, &m_DecControl, 0,(SKP_uint8 *)(cur + 2), nFrameLen, (short*)out_temp, &out_len_temp );

		cur  += (2 + nFrameLen);
		in_size  -= (2 + nFrameLen);
		out_temp += out_len_temp*2;
		ret += out_len_temp*2;
	}
	if( ret >= 0 ) {
		(*env)->SetByteArrayRegion(env, dest, 0, ret, m_pDecDestBuf);	
	} else {
		ret = 0;
	}
	
	__android_log_print(ANDROID_LOG_INFO, "silk_jni", "silk_jni.decode, size/out_size=%d, %d", size, ret); 

	return  ret;
}

JNIEXPORT void JNICALL Java_com_qihoo_mediasdk_codec_Silk_close
    (JNIEnv *env, jobject obj) {
	if(m_pSilkEnc){
		free( m_pSilkEnc);
		m_pSilkEnc = NULL;
	}
}