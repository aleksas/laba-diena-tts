package com.gscoder.android.liepa;

import android.app.ListActivity;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

public class LiepaInfoViewer extends ListActivity {
    private final static String LOG_TAG = "Liepa_Java_" + LiepaInfoViewer.class.getSimpleName();
    private NativeLiepaTTS mLiepaEngine;
    private SimpleCursorAdapter mAdapter;
    private float mBenchmark = -1;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        try {
            mLiepaEngine = new NativeLiepaTTS(Voice.getDataStorageBasePath(), null);
            //mLiepaEngine.setLanguage("lit", "LTU", "");

            ProgressDialog progress = new ProgressDialog(this);
            progress.setMessage("Wait a few seconds");
            progress.setCancelable(false);
            new GetInformation(progress).execute();
        } catch (Exception e) {
            Log.e(LOG_TAG, e.getMessage(), e);
        }
    }

    private class GetInformation extends AsyncTask<Void, Void, Void> {

        private ProgressDialog progress;

        public GetInformation(ProgressDialog progress) {
            this.progress = progress;
        }

        @Override
        public void onPreExecute() {
            progress.show();
        }

        @Override
        public Void doInBackground(Void... arg0) {
            populateInformation();
            return null;
        }

        @Override
        public void onPostExecute(Void unused) {
            progress.dismiss();
        }


    }

    private void populateInformation() {
        if (mBenchmark <0) {
            mBenchmark = 0;// mLiepaEngine.getNativeBenchmark();
        }
        final String[] Info = new String[] {
                "Copyright",
                "URL",
                "Android Version",
                "Build ABI",
                "Phone Model",
        };
        final String[] Data = new String[] {
                "© (2017) Aleksas Pielikis",
                "https://github.com/aleksas/laba-diena-tts",
                "",
                android.os.Build.VERSION.RELEASE,
                android.os.Build.CPU_ABI,
                android.os.Build.MODEL,

        };

        runOnUiThread(new Runnable() {

            @Override
            public void run() {
                setListAdapter(new SettingsArrayAdapter(LiepaInfoViewer.this, Info, Data));
            }
        });

    }

    private class SettingsArrayAdapter extends ArrayAdapter<String> {
        private final Context context;
        private final String[] values;
        private final String[] data;

        public SettingsArrayAdapter(Context context, String[] values, String[] data) {
            super(context, R.layout.liepa_info, values);
            this.context = context;
            this.values = values;
            this.data = data;
        }

        @Override
        public int getViewTypeCount() {
            return 2;
        }

        @Override
        public int getItemViewType(int position) {
            return 1;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            LayoutInflater inflater = (LayoutInflater) context
                    .getSystemService(Context.LAYOUT_INFLATER_SERVICE);

            if (convertView == null) {
                convertView = inflater.inflate(R.layout.liepa_info, parent, false);
            }

            TextView infoType = (TextView) convertView.findViewById(R.id.infotitle);
            TextView infoDetail = (TextView) convertView.findViewById(R.id.infodetail);

            infoType.setText(values[position]);
            infoDetail.setText(data[position]);

            return convertView;
        }
    }
}
