package com.gscoder.android.liepa;

import android.speech.tts.TextToSpeech;

import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;

/**
 * Created by alex on 3/18/2017.
 */

public class NativeLiepaTTS {
    private final static String LOG_TAG = "Liepa_Java_" + NativeLiepaTTS.class.getSimpleName();

    private static NativeLiepaTTS instance = null;

    private final SynthReadyCallback mCallback;

    private String mDataPath = "";
    private String mVoicePath = "";
    private int mSpeechRate = 150;
    private int mPitch = 100;

    protected  NativeLiepaTTS(String datapath, SynthReadyCallback callback) throws Exception
    {
        mDataPath = datapath;
        mCallback = callback;
        loadLithUSS();
    }

    public int isLanguageAvailable(String language, String country,	String variant) {
        if (language.equals("lit"))
        {
            if (new Voice(String.format("%s-%s-%s\tMD5", language, country, variant)).isAvailable()) {
                return TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE;
            }
            else if ((variant == null || variant.isEmpty()) && CheckVoiceData.isLanguageAvailable(language))
            {
                return TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE;
            }

            /*
            File dir = new File(mDatapath, variant + "/");

            if (dir.isDirectory()) {
                return TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE;
            }
            else {
                return TextToSpeech.LANG_MISSING_DATA;
            }*/

        }

        return TextToSpeech.LANG_NOT_SUPPORTED;
    }

    public boolean setLanguage(String language, String country, String variant) {
        if (language.equals("lit")) {
            String storage = mDataPath + variant + "/";
            if (!storage.equals(mVoicePath)) {
                boolean result = initLithUSS(mDataPath, storage) == 0;
                if (result) {
                    mVoicePath = storage;
                }
                return result;
            }
        }
        return false;
    }

    public void stop()
    {
    }

    private static byte[] encode(CharSequence cs, Charset charset) {
        ByteBuffer byteBuffer = charset.encode(CharBuffer.wrap(cs));
        int length = byteBuffer.remaining();
        byte[] result = new byte[length + 1];
        byteBuffer.get(result, 0, length);
        return result;
    }

    public void synthesize(CharSequence text) throws Exception
    {
        byte[] bytes = encode(text, Charset.forName("ISO-8859-13"));
        int bufferSize = bytes.length * 1024 * 7;

        byte[] signalBufferByte = new byte[bufferSize * Short.SIZE];

        int res = synthesizeWholeText(bytes, mSpeechRate, mPitch, signalBufferByte, bufferSize);
        if (res < 0) {
            throw new Exception("Failed to synthesize text!");
        }

        nativeSynthCallback(signalBufferByte, res * Short.SIZE);
    }

    private void nativeSynthCallback(byte[] audioData, int length) {
        if (mCallback == null)
            return;

        if (audioData == null) {
            mCallback.onSynthDataComplete();
        } else {
            mCallback.onSynthDataReady(audioData, length);
        }
    }

    @Override
    protected void finalize() {
        unloadLithUSS();
    }

    public void setSpeechRate(int speechrate) {
        mSpeechRate = speechrate;
    }

    public void setPitch(int pitch) {
        mPitch = pitch;
    }

    public int getSampleRate() {
        return 22050;
    }

    public interface SynthReadyCallback {
        void onSynthDataReady(byte[] audioData, int length);

        void onSynthDataComplete();
    }

    public static NativeLiepaTTS getInstance(String datapath, SynthReadyCallback callback) throws Exception {
        if(instance == null) {
            instance = new NativeLiepaTTS(datapath, callback);
        }
        return instance;
    }

    static native void loadLithUSS();
    static native void unloadLithUSS();
    static native int initLithUSS(String katDll, String katVoice);
    static native int synthesizeWholeText(byte[] text, int speed, int tone, byte[] byteBuffer, int shortSize);

    static {
        System.loadLibrary("RateChange");
        System.loadLibrary("TextNormalization");
        System.loadLibrary("transcrLUSS");
        System.loadLibrary("UnitSelection");
        System.loadLibrary("LithUSS");
        System.loadLibrary("laba-diena-tts");
    }
}
