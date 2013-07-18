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
#include <jni.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <android/log.h>
#include <speex/speex.h>
#include <speex/speex_preprocess.h>

static int dec_frame_size;
static int enc_frame_size;

static SpeexBits ebits, dbits;
void *enc_state;
void *dec_state;


jbyte* spx_enc_src_buffer = 0;
jbyte* spx_enc_dest_buffer = 0;
jbyte* spx_dec_src_buffer = 0;
jbyte* spx_dec_dest_buffer = 0;

SpeexPreprocessState *preprocess_state = 0;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {  
	jint result = JNI_ERR; 
	JNIEnv* env = NULL;
	
    __android_log_print(ANDROID_LOG_INFO, "speex_jni", "speex_jni, loading . . .");  
	__android_log_print(ANDROID_LOG_INFO, "speex_jni", "speex_jni, loaded");  
  
    result = JNI_VERSION_1_4;  

    return result;  
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Speex_open
  (JNIEnv *env, jobject obj, jint sample_rate,  jint quality) {
	int tmp;
	speex_bits_init(&ebits);
	speex_bits_init(&dbits);

	enc_state = speex_encoder_init(&speex_nb_mode); 
	dec_state = speex_decoder_init(&speex_nb_mode); 
	
	tmp = quality;
	speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &tmp);
	speex_encoder_ctl(enc_state, SPEEX_SET_SAMPLING_RATE, &sample_rate);
	speex_encoder_ctl(enc_state, SPEEX_GET_FRAME_SIZE, &enc_frame_size);
	tmp = 1;
	//speex_encoder_ctl(enc_state, SPEEX_SET_VBR, &tmp);
	speex_encoder_ctl(enc_state, SPEEX_SET_DTX, &tmp);
	//speex_encoder_ctl(enc_state, SPEEX_SET_VAD, &tmp);
	preprocess_state = speex_preprocess_state_init(enc_frame_size, sample_rate);
	int enable=1;
	speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_DENOISE, &enable);
	speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_AGC, &enable);
	
	//decoder
	speex_encoder_ctl(dec_state, SPEEX_SET_QUALITY, &tmp);
	speex_decoder_ctl(dec_state, SPEEX_SET_SAMPLING_RATE, &sample_rate);
	speex_decoder_ctl(dec_state, SPEEX_GET_FRAME_SIZE, &dec_frame_size);
		
	spx_enc_src_buffer = (jbyte*)malloc(dec_frame_size*10);
	spx_enc_dest_buffer = (jbyte*)malloc(dec_frame_size*10);
	spx_dec_src_buffer = (jbyte*)malloc(dec_frame_size*10);
	spx_dec_dest_buffer = (jbyte*)malloc(dec_frame_size*10);
	__android_log_print(ANDROID_LOG_INFO, "speex_jni", "speex_jni.open, enc_frame_size/dec_frame_size=%d, %d", enc_frame_size, dec_frame_size); 
	
	return (jint)0;
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Speex_encode
    (JNIEnv *env, jobject obj, jbyteArray src, jint size, jbyteArray dest, jint out_size) {

	int nsamples = (size)/enc_frame_size;
	int i, tot_bytes = 0;
	int offset = 0;
	
	speex_bits_reset(&ebits);
	for( i=0; i<nsamples; i++ ) {		
		(*env)->GetByteArrayRegion(env, src, i*enc_frame_size, enc_frame_size, spx_enc_src_buffer);
		speex_preprocess_run(preprocess_state, (spx_int16_t*)spx_enc_src_buffer);
		speex_encode_int(enc_state, (short*)spx_enc_src_buffer, &ebits);
		tot_bytes = speex_bits_write(&ebits, (char*)spx_enc_dest_buffer, out_size);	
		(*env)->SetByteArrayRegion(env, dest, offset, tot_bytes, spx_enc_dest_buffer);
		offset += tot_bytes;
	}	
	__android_log_print(ANDROID_LOG_INFO, "speex_jni", "speex_jni.encode, size/nsamples/out_len=%d, %d, %d", size, nsamples, tot_bytes); 

	return (jint)tot_bytes;
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Speex_decode
    (JNIEnv *env, jobject obj, jbyteArray src, jint size, jbyteArray dest, jint out_size) {
	int ret = 0;
	int remain = 0;
	int i = 0;
	int out_len = 0;

	(*env)->GetByteArrayRegion(env, src, 0, size, spx_dec_src_buffer);

	speex_bits_reset(&dbits);
	speex_bits_read_from(&dbits, (char*)spx_dec_src_buffer, size);
	while(1) {
		remain = speex_bits_remaining(&dbits)/8;
		__android_log_print(ANDROID_LOG_INFO, "speex_jni", "speex_jni.decode, remaining=%d.", remain); 
		if( remain <= 0 ) {
			break;
		}

		ret = speex_decode_int(dec_state, &dbits, (spx_int16_t*)spx_dec_dest_buffer);
		if( ret != 0 ) {
			__android_log_print(ANDROID_LOG_INFO, "speex_jni", "speex_jni.decode, ret!=0."); 
			break;
		}
		(*env)->SetByteArrayRegion(env, dest, i*dec_frame_size, dec_frame_size, (const jbyte*)spx_dec_dest_buffer);
		out_len += dec_frame_size;
		i++;
	}
	__android_log_print(ANDROID_LOG_INFO, "speex_jni", "speex_jni.decode, out_len=%d", out_len); 

	return (jint)out_len;
}

JNIEXPORT void JNICALL Java_com_qihoo_mediasdk_codec_Speex_close
    (JNIEnv *env, jobject obj) {
	speex_bits_destroy(&ebits);
	speex_bits_destroy(&dbits);
	speex_decoder_destroy(dec_state); 
	speex_encoder_destroy(enc_state); 
	speex_preprocess_state_destroy(preprocess_state);
}
