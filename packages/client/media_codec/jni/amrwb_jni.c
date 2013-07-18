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

#include "amrwb_jni.h"
#include "amrwb/typedef.h"
#include "amrwb/dec_if.h"
#include "amrwb/dec_dtx.h"
#include <android/log.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>

const UWord8 amr_block_size[16]= {18, 24, 33, 37, 41, 47, 51, 59, 61, 6, 6, 0, 0, 0, 1, 1};

unsigned int   outputFrameSize = L_FRAME16k;
void* dec_st = NULL;
void* dec_src = NULL;
void* dec_buf = NULL;
double m_ms_out = 16000 * sizeof(short); 

jint JNI_OnLoad(JavaVM* vm, void* reserved) {  
	jint result = JNI_ERR; 
  
    result = JNI_VERSION_1_4;  

    return result;  
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_AMRWBDecoder_open
  (JNIEnv *env, jobject obj, jint sample_rate,  jint compression) {

	__android_log_print(ANDROID_LOG_INFO, "amrwb_jni", "amrwb_jni.open"); 
	dec_st = D_IF_init();
	dec_src = malloc(128);
	dec_buf = malloc(2049);
	__android_log_print(ANDROID_LOG_INFO, "amrwb_jni", "amrwb_jni.opened"); 
	return (jint)0;
}

JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_AMRWBDecoder_decode
    (JNIEnv *env, jobject obj, jbyteArray src, jint size, jbyteArray dest, jint out_size) {		
	int mode = 0;
	int frame_length = 0;
	UWord8* cur = 0;

	(*env)->GetByteArrayRegion(env, src, 0, size, dec_src);
	
	cur = (UWord8 *) dec_src;
	mode = ((cur[0] >> 3) & 0x0F);
        frame_length = amr_block_size[mode];
        if(size < frame_length){
            return 0;
        }
        
        int frametype;
        if(mode <= 9){
            frametype = _good_frame;
        }else if(mode == 14){
            frametype = _lost_frame;
        }else if(mode == 15){
            frametype = _no_frame;
        }else if(mode == 10){
            frametype = _bad_frame;
        }else{
            return 0;
        }
        
        D_IF_decode(dec_st, (UWord8 *) cur, dec_buf, frametype);
        (*env)->SetByteArrayRegion(env, dest, 0, outputFrameSize, (const jbyte*)dec_buf);
	__android_log_print(ANDROID_LOG_INFO, "amrwb_jni", "amrwb_jni.decode, out_len=%d", outputFrameSize); 

	return (jint)outputFrameSize;
}

JNIEXPORT void JNICALL Java_com_qihoo_mediasdk_codec_AMRWBDecoder_close
    (JNIEnv *env, jobject obj) {
    if(dec_st != NULL){
        D_IF_exit( dec_st);
        dec_st = NULL;
    }
    if( dec_buf != NULL ) {
    	free(dec_buf);
    	dec_buf = NULL;
    }
}
