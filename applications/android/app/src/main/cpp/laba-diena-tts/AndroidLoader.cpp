#include <LithUSS.h>
#include <LithUSS_Error.h>

#include <android/log.h>
#include <jni.h>

#ifndef LABA_DIENA_TTS_DEBUG_ENABLED
// Don't debug by default
    #define LOG_TAG "Laba_Diena_TTS_Native_Engine"
    #define LABA_DIENA_TTS_DEBUG_ENABLED 1
#endif

#if LABA_DIENA_TTS_DEBUG_ENABLED

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

#define DEBUG_LOG_FUNCTION if (LABA_DIENA_TTS_DEBUG_ENABLED) LOGV("%s", __FUNCTION__)

extern "C"
JNIEXPORT void JNICALL
Java_com_gscoder_android_labadienatts_NativeTTS_loadLithUSS(
        JNIEnv* env,
        jobject /* this */) {
    DEBUG_LOG_FUNCTION;
    loadLUSS();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gscoder_android_labadienatts_NativeTTS_unloadLithUSS(
        JNIEnv* env,
        jobject /* this */) {
    DEBUG_LOG_FUNCTION;
    unloadLUSS();
}

extern "C"
JNIEXPORT int JNICALL
Java_com_gscoder_android_labadienatts_NativeTTS_initLithUSS(
        JNIEnv* env,
        jobject /* this */, jstring dllPath, jstring voicePath) {
    DEBUG_LOG_FUNCTION;
    char * katVoice = (char*)env->GetStringUTFChars(voicePath, 0) ;
    char * katDll = (char*)env->GetStringUTFChars(dllPath, 0);

    int result = NO_ERR;
    result = initLUSS(katDll, katVoice);

    env->ReleaseStringUTFChars(voicePath, katVoice);
    env->ReleaseStringUTFChars(dllPath, katDll);

    return  result;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_gscoder_android_labadienatts_NativeTTS_getErrorString(
        JNIEnv* env,
        jobject /* this */, int code) {
    DEBUG_LOG_FUNCTION;
    return env->NewStringUTF(getLUSSErrorMessages(code));
}

extern "C"
JNIEXPORT int JNICALL
Java_com_gscoder_android_labadienatts_NativeTTS_synthesizeWholeText(
        JNIEnv* env,
        jobject /* this */, jbyteArray text, jint speed, jint tone, jbyteArray signalByteBuffer, jint bufferShortSize) {
    DEBUG_LOG_FUNCTION;

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

//EXPORT int initLUSS(char *katDll, char *katVoice);
