#include <LithUSS.h>
#include <LithUSS_Error.h>

#include <jni.h>

#ifndef FLITE_DEBUG_ENABLED
// Don't debug by default
#define FLITE_DEBUG_ENABLED 0
#endif

#if FLITE_DEBUG_ENABLED

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__)

#else

#define LOGI(...)
#define LOGE(...)
#define LOGV(...)
#define LOGD(...)
#define LOGW(...)

#endif

#define DEBUG_LOG_FUNCTION if (FLITE_DEBUG_ENABLED) LOGV("%s", __FUNCTION__)


extern "C"
JNIEXPORT void JNICALL
Java_com_gscoder_android_liepa_NativeLiepaTTS_loadLithUSS(
        JNIEnv* env,
        jobject /* this */) {
    loadLUSS();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gscoder_android_liepa_NativeLiepaTTS_unloadLithUSS(
        JNIEnv* env,
        jobject /* this */) {
    unloadLUSS();
}

extern "C"
JNIEXPORT int JNICALL
Java_com_gscoder_android_liepa_NativeLiepaTTS_initLithUSS(
        JNIEnv* env,
        jobject /* this */, jstring dllPath, jstring voicePath) {
    jboolean isCopy;

    char * katVoice = (char*)env->GetStringUTFChars(voicePath, &isCopy) ;
    char * katDll = (char*)env->GetStringUTFChars(dllPath, &isCopy) ;

    return initLUSS(katDll, katVoice);
}

extern "C"
JNIEXPORT int JNICALL
Java_com_gscoder_android_liepa_NativeLiepaTTS_synthesizeWholeText(
        JNIEnv* env,
        jobject /* this */, jbyteArray text, jint speed, jint tone, jbyteArray signalByteBuffer, jint bufferShortSize) {

    jboolean isCopy;
    char * szText = (char*)env->GetByteArrayElements(text, &isCopy);
    signed char * signbbuf = env->GetByteArrayElements(signalByteBuffer, &isCopy);

    unsigned int signbufsize = bufferShortSize;

    event evarr[1024];
    int evarrsize = 1024;

    int result = synthesizeWholeText(szText, (short *) signbbuf, &signbufsize, evarr, &evarrsize, speed, tone);

    if (result == NO_ERR) {
        return signbufsize;
    }
    return result;
}

EXPORT int initLUSS(char *katDll, char *katVoice);
