package com.gscoder.android.liepa;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;


public class CheckVoiceData extends Activity {
    private final static String LOG_TAG = "Liepa_Java_" + CheckVoiceData.class.getSimpleName();
    private final static String LIEPA_DATA_PATH = Voice.getDataStorageBasePath();
    public final static String VOICE_LIST_FILE = LIEPA_DATA_PATH + "voices.list";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        int result = TextToSpeech.Engine.CHECK_VOICE_DATA_PASS;
        Intent returnData = new Intent();
        returnData.putExtra(TextToSpeech.Engine.EXTRA_VOICE_DATA_ROOT_DIRECTORY,
                LIEPA_DATA_PATH);

        ArrayList<String> available = new ArrayList<String>();
        ArrayList<String> unavailable = new ArrayList<String>();

		/* First, make sure that the directory structure we need exists
		 * There should be a "cg" folder inside the flite data directory
		 * which will store all the clustergen voice data files.
		 */

        if(!Utility.pathExists(LIEPA_DATA_PATH)) {
            // Create the directory.
            Log.e(LOG_TAG, "Liepa data directory missing. Trying to create it.");
            boolean success = false;

            try {
                Log.e(LOG_TAG,LIEPA_DATA_PATH);
                success = new File(LIEPA_DATA_PATH).mkdirs();
            }
            catch (Exception e) {
                Log.e(LOG_TAG,"Could not create directory structure. "+e.getMessage());
                success = false;
            }

            if(!success) {
                Log.e(LOG_TAG, "Failed");
                // Can't do anything without appropriate directory structure.
                result = TextToSpeech.Engine.CHECK_VOICE_DATA_FAIL;
                setResult(result, returnData);
                finish();
            }
        }

		/* Connect to CMU TTS server and get the list of voices available,
		 * if we don't already have a file.
		 */

        if(!Utility.pathExists(VOICE_LIST_FILE)) {
            Log.e(LOG_TAG, "Voice list file doesn't exist. Try getting it from server.");

            DownloadVoiceList(null);
        }

		/* At this point, we MUST have a voices.list file. If this file is not there,
		 * possibly because Internet connection was not available, we must create a dummy
		 *
		 */
        if(!Utility.pathExists(VOICE_LIST_FILE)) {
            try {
                Log.w(LOG_TAG, "Voice list not found, creating dummy list.");
                BufferedWriter out = new BufferedWriter(new FileWriter(VOICE_LIST_FILE));
                out.write("ltu-LTU-Aiste\t123");
                out.write("ltu-LTU-Regina\t123");
                out.write("ltu-LTU-Edvardas\t123");
                out.write("ltu-LTU-Vladas\t123");
                out.close();
            } catch (IOException e) {
                Log.e(LOG_TAG, "Failed to create voice list dummy file.");
                // Can't do anything without that file.
                result = TextToSpeech.Engine.CHECK_VOICE_DATA_FAIL;
                setResult(result, returnData);
                finish();
            }
        }
		/* Go through each line in voices.list file and see
		 * if the data for that voice is installed.
		 */

        ArrayList<Voice> voiceList = getVoices();
        if (voiceList.isEmpty()) {
            Log.e(LOG_TAG,"Problem reading voices list. This shouldn't happen!");
            result = TextToSpeech.Engine.CHECK_VOICE_DATA_FAIL;
            setResult(result, returnData);
            finish();
        }

        for(Voice vox:voiceList) {
            if(vox.isAvailable()) {
                available.add(vox.getName());
            } else {
                unavailable.add(vox.getName());
            }
        }
        returnData.putStringArrayListExtra("availableVoices", available);
        returnData.putStringArrayListExtra("unavailableVoices", unavailable);
        setResult(result, returnData);
        finish();
    }

    public static void DownloadVoiceList(Runnable callback) {
        // Download the voice list and call back to notify of update
        String voiceListURL = Voice.getDownloadURLBasePath() + "voices.list?r=&ts=" + System.currentTimeMillis();

        FileDownloader fdload = new FileDownloader();
        fdload.saveUrlAsFile(voiceListURL, VOICE_LIST_FILE);
        while(!fdload.finished) {}
        boolean savedVoiceList = fdload.success;

        if(!savedVoiceList)
            Log.w(LOG_TAG,"Could not update voice list from server");
        else
            Log.w(LOG_TAG,"Successfully updated voice list from server");

        if (callback != null) {
            callback.run();
        }

    }

    public static ArrayList<Voice> getVoices() {
        ArrayList<String> voiceList = null;
        try {
            voiceList = Utility.readLines(VOICE_LIST_FILE);
        } catch (IOException e) {
            // Ignore exception, since we will return empty anyway.
        }
        if (voiceList == null) {
            voiceList = new ArrayList<String>();
        }

        ArrayList<Voice> voices = new ArrayList<Voice>();

        for(String strLine:voiceList) {
            Voice vox = new Voice(strLine);
            if (!vox.isValid())
                continue;
            voices.add(vox);
        }

        return voices;
    }

    public static ArrayList<Voice> getAvailableVoices() {
        ArrayList<Voice> voices = new ArrayList<Voice>();

        for(Voice vox:getVoices()) {
            if (!vox.isAvailable())
                continue;
            voices.add(vox);
        }

        return voices;
    }

    public static Voice getAnyVoiceAvailable(String language) {
        for (Voice v : getVoices()) {
            if (v.getLanguage().equals(language) && v.isAvailable()) {
                return v;
            }
        }
        return null;
    }

    public static Voice getAnyVoiceAvailable(String language, String country) {
        for (Voice v : getVoices()) {
            if (v.getLanguage().equals(language) && v.getCountry().equals(country) && v.isAvailable())
            {
                return v;
            }
        }
        return null;
    }

    public static boolean isLanguageAvailable(String language) {
        for (Voice v : getVoices()) {
            if (v.getLanguage().equals(language) && v.isAvailable())
            {
                return true;
            }
        }
        return false;
    }
}