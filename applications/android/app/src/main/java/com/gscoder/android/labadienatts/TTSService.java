package com.gscoder.android.labadienatts;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.res.AssetManager;
import android.media.AudioFormat;
import android.speech.tts.SynthesisCallback;
import android.speech.tts.SynthesisRequest;
import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeechService;
import android.content.Intent;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Set;

public class TTSService extends TextToSpeechService {
    private final static String LOG_TAG = "Laba_Diena_TTS_Java_" + TTSService.class.getSimpleName();
    private NativeTTS mEngine;

    private static final String DEFAULT_LANGUAGE = "lit";
    private static final String DEFAULT_COUNTRY = "LTU";
    private static final String DEFAULT_VARIANT = "Aiste";

    private String mLanguage = DEFAULT_LANGUAGE;
    private String mCountry = DEFAULT_COUNTRY;
    private String mVariant = DEFAULT_VARIANT;

    private String mPassiveLoadedCountry = "";
    private String mPassiveLoadedLanguage = "";
    private String mPassiveLoadedVariant = "";

    private SynthesisCallback mCallback;

    private boolean mPassiveLanguageLoad = true;

    @Override
    public void onCreate() {
        try {
            initializeLiepaEngine();
        } catch (Exception e) {
            e.printStackTrace();
        }

        // This calls onIsLanguageAvailable() and must run after Initialization
        super.onCreate();
    }


    private void copyAssetFile(String filename, String srcDirectory, String dstDirectory) {
        AssetManager assetManager = this.getAssets();

        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(new File(srcDirectory, filename).getPath());
            String newFileName = new File(dstDirectory, filename).getPath();
            out = new FileOutputStream(newFileName);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
        } catch (Exception e) {
            Log.e("tag", e.getMessage());
        }

    }

    private void initializeLiepaEngine() throws Exception {
        if (mEngine == null) {
            //mEngine.stop();
            //mEngine = null;
            mEngine = NativeTTS.getInstance(Voice.getDataStorageBasePath(this), mSynthCallback);
        }

        AssetManager assetManager = getAssets();
        String[] files = null;
        String subDir = "liepa_rules";
        try {
            files = assetManager.list(subDir);

            if (files != null) {
                for (String filename : files) {
                    copyAssetFile(filename, subDir, Voice.getDataStorageBasePath(this));
                }
            }
        } catch (IOException ex) {
            Log.e("tag", "I/O Exception", ex);
        }
    }

    @Override
    protected String[] onGetLanguage() {
        Log.v(LOG_TAG, "onGetLanguage");
        return new String[] {
                mLanguage, mCountry, mVariant
        };
    }

    @Override
    protected int onIsLanguageAvailable(String language, String country, String variant) {
        Log.v(LOG_TAG, "onIsLanguageAvailable");
        if (language.equals("lit")) {
            //return TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE;
            int res = mEngine.isLanguageAvailable(this, language, country, variant);
            return res;
        } else {
            return TextToSpeech.LANG_NOT_SUPPORTED;
        }
    }

    @Override
    protected int onLoadLanguage(String language, String country, String variant) {
        return setLanguage(language, country, variant);
    }

    @Override
    protected void onStop() {
        Log.v(LOG_TAG, "onStop");
        mEngine.stop();
    }

    @Override
    public List<android.speech.tts.Voice> onGetVoices() {
        ArrayList<android.speech.tts.Voice> voices = new ArrayList<android.speech.tts.Voice>();
        for (Voice vox: CheckVoiceData.getAvailableVoices(this)) {
            Locale locale = vox.getLocale();
            Set<String> features = onGetFeaturesForLanguage(locale.getISO3Language(),
                    locale.getISO3Country(), locale.getVariant());
            String voiceName = onGetDefaultVoiceNameFor(locale.getISO3Language(),
                    locale.getISO3Country(), locale.getVariant());
            voices.add(new android.speech.tts.Voice(voiceName, locale, android.speech.tts.Voice.QUALITY_NORMAL,
                    android.speech.tts.Voice.LATENCY_NORMAL, false, features));
        }
        return voices;
    }

    private int setLanguage(String language, String country, String variant) {
        Log.v(LOG_TAG, "onLoadLanguage");
        int result = TextToSpeech.LANG_NOT_SUPPORTED;

        if (mEngine.isLanguageAvailable(this, language, country, variant) == TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE) {
            if (mPassiveLanguageLoad) {
                mLanguage = language;
                mCountry = country;
                mVariant = variant;

                result = TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE;
            } else {
                try {
                    mEngine.setLanguage(language, country, variant);
                    mLanguage = language;
                    mCountry = country;
                    mVariant = variant;

                    result = TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE;
                } catch (Exception e) {
                    Log.e(LOG_TAG, e.getMessage());
                }
            }
        }

        if (language.equals("lit") || language.equals("lt") || language.equals("LTU")) {
            return TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE;
        } else {
            return TextToSpeech.LANG_NOT_SUPPORTED;
        }
    }


    @Override
    protected synchronized void onSynthesizeText(SynthesisRequest request, SynthesisCallback callback) {
        Log.v(LOG_TAG, "onSynthesize");

        String language = request.getLanguage();
        String country = request.getCountry();
        String variant = request.getVariant();
        CharSequence text = request.getCharSequenceText();

        Integer pitch = (int)(request.getPitch() / 200.0f * (133 - 75) + 75);
        Integer speechRate = (int)(((200 - request.getSpeechRate()) / 200.0f) * (300 - 30) + 30); // atvirksciai proporcingos reiksmes
        try {
            if (mPassiveLanguageLoad ) {
                if(!((mPassiveLoadedLanguage.equals(language)) && (mPassiveLoadedCountry.equals(country)) && (mPassiveLoadedVariant.equals(variant)))) {
                    mEngine.setLanguage(language, country, variant);

                    mPassiveLoadedLanguage = language;
                    mPassiveLoadedCountry = country;
                    mPassiveLoadedVariant = variant;
                }
            } else if (! ((mLanguage == language) && (mCountry == country) && (mVariant == variant ))) {
                mEngine.setLanguage(language, country, variant);
            }
        } catch (Exception e) {
            e.printStackTrace();
            Log.e(LOG_TAG, e.getMessage());
            return;
        }

        mEngine.setSpeechRate(speechRate);
        mEngine.setPitch(pitch);

        mCallback = callback;
        Integer rate = new Integer(mEngine.getSampleRate());
        Log.e(LOG_TAG, rate.toString());
        mCallback.start(mEngine.getSampleRate(), AudioFormat.ENCODING_PCM_16BIT, 1);
        int start = 0;
        int end = 0;
        try {
           /* while (start < text.length()) {
                end = text.length();
                for (int i = start; i < text.length(); i++) {
                    if (".!?;".indexOf(text.charAt(i)) != -1) {
                        end = i + 1;
                        break;
                    }
                }

                mEngine.synthesize(text.subSequence(start, end));
                start = end;
            }*/
            mEngine.synthesize(text);
        } catch (Exception e) {
            Log.e(LOG_TAG, e.getMessage(), e);
        }
    }

    private final NativeTTS.SynthReadyCallback mSynthCallback = new NativeTTS.SynthReadyCallback() {
        @Override
        public void onSynthDataReady(byte[] audioData, int length) {
            if ((audioData == null) || (audioData.length == 0) || length == 0) {
                onSynthDataComplete();
                return;
            }
            Log.v(LOG_TAG, "SynthReadyCallback provide");

            final int maxBytesToCopy = mCallback.getMaxBufferSize();

            int offset = 0;

            while (offset < length) {
                final int bytesToWrite = Math.min(maxBytesToCopy, (length - offset));
                mCallback.audioAvailable(audioData, offset, bytesToWrite);
                offset += bytesToWrite;
            }
            Log.v(LOG_TAG, "SynthReadyCallback done");
        }

        @Override
        public void onSynthDataComplete() {
            Log.v(LOG_TAG, "onSynthDataComplete");
            if (mCallback != null) {
                mCallback.done();
            }
        }
    };
    /**
     * Listens for language update broadcasts and initializes the flite engine.
     */
    private final BroadcastReceiver mBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            try {
                initializeLiepaEngine();
            } catch (Exception e) {
                Log.e(LOG_TAG, e.getMessage(), e);
            }
        }
    };
}
