/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_qihoo_mediasdk_codec_Speex */

#ifndef _Included_com_qihoo_mediasdk_codec_Silk
#define _Included_com_qihoo_mediasdk_codec_Silk
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_qihoo_mediasdk_codec_Speex
 * Method:    open
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Silk_open
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_qihoo_mediasdk_codec_Speex
 * Method:    decode
 * Signature: ([BI[BI)I
 */
JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Silk_decode
  (JNIEnv *, jobject, jbyteArray, jint, jbyteArray, jint);

/*
 * Class:     com_qihoo_mediasdk_codec_Speex
 * Method:    encode
 * Signature: ([BI[BI)I
 */
JNIEXPORT jint JNICALL Java_com_qihoo_mediasdk_codec_Silk_encode
  (JNIEnv *, jobject, jbyteArray, jint, jbyteArray, jint);

/*
 * Class:     com_qihoo_mediasdk_codec_Speex
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_qihoo_mediasdk_codec_Silk_close
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
