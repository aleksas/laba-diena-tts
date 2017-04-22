package com.gscoder.android.labadienatts;

/**
 * Created by alex on 3/19/2017.
 */


import java.io.File;
import java.util.Locale;

import android.content.Context;
import android.os.Environment;
import android.util.Log;
import android.os.StatFs;

public class Voice {
    private final static String LOG_TAG = "Laba_Diena_TTS_Java_" + Voice.class.getSimpleName();
    private final static String VOICE_BASE_URL = "http://286840.s.dedikuoti.lt/liepa/";
    private final static String[] DEFAULT_VOICES = new String[] {"Aiste", "Regina", "Edvardas", "Vladas"};

    private String mVoiceName;
    private String mVoiceMD5;
    private String mVoiceLanguage;
    private String mVoiceCountry;
    private String mVoiceVariant;
    private boolean mIsValidVoice;
    private String mVoicePath;
    private boolean mIsVoiceAvailable;

    public static String getDataStorageBasePath(Context context) {
        if (Environment.getExternalStorageState().equals("mounted")) {
            File targetDir = null;
            long available = 0;

            for (File dir : context.getExternalFilesDirs(null)) {
                StatFs stat = new StatFs(dir.getAbsolutePath());
                if (stat.getAvailableBytes() > available) {
                    available = stat.getAvailableBytes();
                    targetDir = dir;
                }
            }

            for (File dir : context.getExternalFilesDirs(null)) {
                for (String voice : DEFAULT_VOICES) {
                    File f = new File(dir.getAbsolutePath(), voice);
                    if (f.exists() && f.isDirectory())
                    {
                        targetDir = dir;
                        break;
                    }
                }
            }

            return targetDir.getAbsolutePath();
        }
        return null;
    }

    /**
     * @return base URL to download voices and other flite data
     */
    public static String getDownloadURLBasePath() {
        return VOICE_BASE_URL;
    }

    /**
     * @param voiceInfoLine is the line that is found in "voices.list" file
     * as downloaded on the server and cached. This line has text in the format:
     * language-country-variant<TAB>MD5SUM
     */
    Voice(Context context, String voiceInfoLine) {
        boolean parseSuccessful = false;

        String[] voiceInfo = voiceInfoLine.split("\t");
        if (voiceInfo.length != 2) {
            Log.e(LOG_TAG, "Voice line could not be read: " + voiceInfoLine);
        }
        else {
            mVoiceName = voiceInfo[0];
            mVoiceMD5 = voiceInfo[1];

            String[] voiceParams = mVoiceName.split("-");
            if(voiceParams.length != 3) {
                if (voiceParams.length == 1) {
                    Voice v = CheckVoiceData.getAnyVoiceAvailable(context, voiceParams[0]);

                    mVoiceLanguage = v.getLanguage();
                    mVoiceCountry = v.getCountry();
                    mVoiceVariant = v.getVariant();

                    parseSuccessful = true;
                } else if (voiceParams.length == 2) {
                    Voice v = CheckVoiceData.getAnyVoiceAvailable(context, voiceParams[0], voiceParams[1]);

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
            mVoicePath = new File(getDataStorageBasePath(context), mVoiceVariant).getAbsolutePath();
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

    public String getDownloadMD5() {
        return mVoiceMD5;
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