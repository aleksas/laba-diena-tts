package com.gscoder.android.liepa;

/**
 * Created by alex on 3/19/2017.
 */


import java.io.File;
import java.util.Locale;

import android.os.Environment;
import android.util.Log;

public class Voice {
    private final static String LOG_TAG = "Liepa_Java_" + Voice.class.getSimpleName();
    private final static String LIEPA_DATA_PATH = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES)  + "/Liepa/";
    private final static String VOICE_BASE_URL = "https://netassist.dl.sourceforge.net/project/liepa-tts-andoid/";
    private final static String VOICE_LIST_URL = "https://raw.githubusercontent.com/aleksas/laba-diena-tts/master/resources/voices.list";

    private String mVoiceName;
    private String mVoiceURL;
    private String mVoiceLanguage;
    private String mVoiceCountry;
    private String mVoiceVariant;
    private boolean mIsValidVoice;
    private String mVoicePath;
    private boolean mIsVoiceAvailable;

    /**
     * @return absolute path to the flite-data directory
     */
    public static String getDataStorageBasePath() {
        if (Environment.getExternalStorageState().equals("mounted")) {
            return LIEPA_DATA_PATH;
        }
        return null;
    }

    /**
     * @return base URL to download voices and other flite data
     */
    public static String getDownloadURLBasePath() {
        return VOICE_BASE_URL;
    }

    public static String getDownloadVoiceListURL() {
        return VOICE_LIST_URL;
    }

    /**
     * @param voiceInfoLine is the line that is found in "voices.list" file
     * as downloaded on the server and cached. This line has text in the format:
     * language-country-variant<TAB>MD5SUM
     */
    Voice(String voiceInfoLine) {
        boolean parseSuccessful = false;

        String[] voiceInfo = voiceInfoLine.split("\t");
        if (voiceInfo.length != 2) {
            Log.e(LOG_TAG, "Voice line could not be read: " + voiceInfoLine);
        }
        else {
            mVoiceName = voiceInfo[0];
            mVoiceURL = voiceInfo[1];

            String[] voiceParams = mVoiceName.split("-");
            if(voiceParams.length != 3) {
                if (voiceParams.length == 1) {
                    Voice v = CheckVoiceData.getAnyVoiceAvailable(voiceParams[0]);

                    mVoiceLanguage = v.getLanguage();
                    mVoiceCountry = v.getCountry();
                    mVoiceVariant = v.getVariant();

                    parseSuccessful = true;
                } else if (voiceParams.length == 2) {
                    Voice v = CheckVoiceData.getAnyVoiceAvailable(voiceParams[0], voiceParams[1]);

                    mVoiceLanguage = v.getLanguage();
                    mVoiceCountry = v.getCountry();
                    mVoiceVariant = v.getVariant();

                    parseSuccessful = true;
                } else {
                    Log.e(LOG_TAG, "Incorrect voicename:" + mVoiceName);
                }
            }
            else {
                mVoiceLanguage = voiceParams[0];
                mVoiceCountry = voiceParams[1];
                mVoiceVariant = voiceParams[2];
                parseSuccessful = true;
            }
        }

        if (parseSuccessful) {
            mIsValidVoice = true;
            mVoicePath = getDataStorageBasePath() + mVoiceVariant;
            checkVoiceAvailability();
        }
        else {
            mIsValidVoice = false;
        }

    }

    private void checkVoiceAvailability() {
        Log.v(LOG_TAG, "Checking for Voice Available: " + mVoiceName);

        mIsVoiceAvailable = false;

        // The file should exist, as well as the MD5 sum should match.
        // Only then do we mark a voice as available.
        //
        // We can attempt getting an MD5sum, and an IOException will
        // tell us if the file didn't exist at all.

        File f = new File(mVoicePath);
        mIsVoiceAvailable = f.exists();
    }

    public boolean isValid() {
        return mIsValidVoice;
    }

    public boolean isAvailable() {
        return mIsVoiceAvailable;
    }

    public String getName() {
        return mVoiceName;
    }

    public String getDownloadURL() {
        return mVoiceURL;
    }

    public String getDisplayName() {
        Locale loc = new Locale(mVoiceLanguage, mVoiceCountry, mVoiceVariant);
        String displayName = loc.getDisplayLanguage() +
                "(" + loc.getDisplayCountry() + "," + loc.getVariant() + ")";
        return displayName;
    }

    public String getVariant() {
        return mVoiceVariant;
    }

    public String getCountry() {
        return mVoiceCountry;
    }

    public String getLanguage() {
        return mVoiceLanguage;
    }

    public String getDisplayLanguage() {
        Locale loc = new Locale(mVoiceLanguage, mVoiceCountry, mVoiceVariant);
        String displayLanguage = loc.getDisplayLanguage() +
                " (" + loc.getDisplayCountry() + ")";

        return displayLanguage;
    }

    public String getPath() {
        return mVoicePath;
    }

    public Locale getLocale() {
        return new Locale(mVoiceLanguage, mVoiceCountry, mVoiceVariant);
    }
}