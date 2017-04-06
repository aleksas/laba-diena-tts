package com.gscoder.android.liepa;

/**
 * Created by alex on 3/19/2017.
 */


import java.util.ArrayList;
import java.util.Locale;

import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

public class TTSDemo extends ListActivity implements OnClickListener, OnKeyListener, OnInitListener {
    private final static String LOG_TAG = "Liepa_Java_" + TTSDemo.class.getSimpleName();

    private EditText mUserText;
    private ImageButton mSendButton;
    private ArrayAdapter<String> mAdapter;
    private ArrayAdapter<String> mVoiceAdapter;
    private ArrayList<Voice> mVoices;
    private ArrayList<String> mStrings = new ArrayList<String>();
    private Spinner mVoiceSpinner;
    private Spinner mRateSpinner;
    private SeekBar mRateBar;
    private SeekBar mPitchBar;
    private TextToSpeech mTts;
    private int mSelectedVoice;

    @TargetApi(14)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ArrayList<Voice> allVoices = CheckVoiceData.getVoices();
        mVoices = new ArrayList<Voice>();
        for(Voice vox:allVoices) {
            if (vox.isAvailable()) {
                mVoices.add(vox);
            }
        }

        if (mVoices.isEmpty()) {
            // We can't demo anything if there are no voices installed.
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("Liepa voices not installed. Please add voices in order to run the demo");
            builder.setNegativeButton("OK", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    dialog.cancel();
                    finish();
                }
            });
            AlertDialog alert = builder.create();
            alert.show();
        }
        else {
            // Initialize the TTS
            if (android.os.Build.VERSION.SDK_INT >=
                    android.os.Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
                mTts = new TextToSpeech(this, this, "com.gscoder.android.liepa");
            }
            else {
                mTts = new TextToSpeech(this, this);
            }
            mSelectedVoice = -1;

        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mTts != null)
            mTts.shutdown();
    }

    private void buildUI() {

        ArrayList<String> voiceNames = new ArrayList<String>();

        for (Voice vox: mVoices) {
            voiceNames.add(vox.getDisplayName()); // vox.getVariant());
        }

        mVoiceAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_dropdown_item,
                voiceNames);


        setContentView(R.layout.activity_tts_demo);
        mStrings.add("Laba diena!");

        mAdapter = new InputHistoryAdapter(this, R.layout.list_tts_history, mStrings);

        setListAdapter(mAdapter);

        mUserText = (EditText) findViewById(R.id.userText);
        mSendButton = (ImageButton) findViewById(R.id.sendButton);

        mVoiceSpinner = (Spinner) findViewById(R.id.voice);
        mVoiceSpinner.setAdapter(mVoiceAdapter);

        mRateBar = (SeekBar) findViewById(R.id.seekBarRate);
        mRateBar.setProgress(100);
        mRateBar.setMax(200);

        mPitchBar = (SeekBar) findViewById(R.id.seekBarPitch);
        mPitchBar.setProgress(100);
        mPitchBar.setMax(200);

        mUserText.setOnClickListener(this);
        mSendButton.setOnClickListener(this);
        mUserText.setOnKeyListener(this);
    }

    public void onClick(View v) {
        sendText();
    }

    private void sendText() {
        String text = mUserText.getText().toString();
        if (text.isEmpty())
            return;
        mAdapter.add(text);
        mUserText.setText(null);
        sayText(text);
    }

    private void sayText(String text) {
        Log.v(LOG_TAG, "Speaking: " + text);
        int currentVoiceID = mVoiceSpinner.getSelectedItemPosition();
        boolean available = false;
        Locale loc = null;
        Locale loc2 = null;
        if (currentVoiceID != mSelectedVoice) {
            mSelectedVoice = currentVoiceID;
            Voice v = mVoices.get(currentVoiceID);
            int langSet = mTts.setLanguage(v.getLocale());
        }

        mTts.setSpeechRate(mRateBar.getProgress() / 100.0f);
        mTts.setPitch(mPitchBar.getProgress() / 100.0f);

        mTts.speak(text, TextToSpeech.QUEUE_FLUSH, null);
    }

    public boolean onKey(View v, int keyCode, KeyEvent event) {
        if (event.getAction() == KeyEvent.ACTION_DOWN) {
            switch (keyCode) {
                case KeyEvent.KEYCODE_DPAD_CENTER:
                case KeyEvent.KEYCODE_ENTER:
                    sendText();
                    return true;
            }
        }
        return false;
    }

    private class InputHistoryAdapter extends ArrayAdapter<String> {
        private ArrayList<String> items;

        public InputHistoryAdapter(Context context,
                                   int textViewResourceId, ArrayList<String> items) {
            super(context, textViewResourceId, items);
            this.items = items;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                LayoutInflater vi = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                convertView = vi.inflate(R.layout.list_tts_history, null);
            }
            String s = items.get(position);
            TextView tt = (TextView) convertView.findViewById(R.id.inputText);
            tt.setText(s);
            return convertView;
        }

    }

    @SuppressWarnings("deprecation")
    @Override
    public void onInit(int status) {
        boolean success = true;
        if (status == TextToSpeech.ERROR) {
            success = false;
        }

        if (success &&
                (android.os.Build.VERSION.SDK_INT >=
                        android.os.Build.VERSION_CODES.ICE_CREAM_SANDWICH)) {
            status = mTts.setEngineByPackageName("com.gscoder.android.liepa");
        }

        if (status == TextToSpeech.ERROR) {
            success = false;
        }

        // REALLY check that it is liepa engine that has been initialized
        // This is done using a hack, for now, since for API < 14
        // there seems to be no way to check which engine is being used.

        int available = mTts.isLanguageAvailable(new Locale("lit", "LTU", ""));
        if (available < TextToSpeech.LANG_AVAILABLE || available > TextToSpeech.LANG_COUNTRY_VAR_AVAILABLE) {
            success = false;
        }

        if (!success) {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("Liepa TTS Engine could not be initialized. Check that Liepa is enabled on your phone!. In some cases, you may have to select Liepa as the default engine.");
            builder.setNegativeButton("Open TTS Settings", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    dialog.cancel();
                    Intent intent = new Intent();
                    intent.setComponent(new ComponentName("com.android.settings", "com.android.settings.TextToSpeechSettings"));
                    startActivity(intent);
                    finish();
                }
            });
            AlertDialog alert = builder.create();
            alert.show();
        }
        else {
            buildUI();
        }
    }

    @Override
    public void onListItemClick(ListView parent, View view, int position, long id) {
        String text = (String) parent.getItemAtPosition(position);
        sayText(text);

    }
}