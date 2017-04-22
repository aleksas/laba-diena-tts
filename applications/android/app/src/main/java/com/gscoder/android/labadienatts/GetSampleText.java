package com.gscoder.android.labadienatts;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.util.Log;

import com.gscoder.android.labadienatts.R;

import java.util.Locale;

public class GetSampleText extends Activity {
    private final static String LOG_TAG = "Laba_Diena_TTS_Java_" + GetSampleText.class.getSimpleName();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final Locale locale = getLocaleFromIntent(getIntent());

        final String language = (locale == null) ? "eng" : locale.getISO3Language();

        int result = TextToSpeech.LANG_AVAILABLE;
        Intent returnData = new Intent();

        if (language.equals("lit")) {
            returnData.putExtra("sampleText", getString(R.string.ltu_sample));
            Log.v(LOG_TAG, "Returned SampleText: " + getString(R.string.ltu_sample));
        } else if (language.equals("ltu")) {
            returnData.putExtra("sampleText", getString(R.string.ltu_sample));
            Log.v(LOG_TAG, "Returned SampleText: " + getString(R.string.ltu_sample));
        } else {
            Log.v(LOG_TAG, "Unsupported Language: " + language);
            result = TextToSpeech.LANG_NOT_SUPPORTED;
            returnData.putExtra("sampleText", "");
        }

        setResult(result, returnData);

        finish();
    }

    private static Locale getLocaleFromIntent(Intent intent) {
        if (intent != null) {
            final String language = intent.getStringExtra("language");

            if (language != null) {
                return new Locale(language);
            }
        }

        return Locale.getDefault();
    }
}
