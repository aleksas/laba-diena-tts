package com.gscoder.android.liepa;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.ActivityCompat;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.TextView;

public class LiepaManager extends Activity implements AdapterView.OnItemClickListener {
    private final static String LOG_TAG = "Laba_Diena_TTS_Java_" + LiepaManager.class.getSimpleName();

    static LauncherIcon[] ICONS = {
            new LauncherIcon(R.drawable.custom_dialog_tts,  R.string.tts_demo_link, TTSDemo.class),
            new LauncherIcon(R.drawable.custom_dialog_manage, R.string.manage_link, DownloadVoiceData.class),
            new LauncherIcon(R.drawable.custom_dialog_info, R.string.info_link, LiepaInfoViewer.class),
            new LauncherIcon(R.drawable.custom_dialog_info, R.string.open_source_link, OpenSourceLicenses.class),
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.liepamanager);

        GridView gridview = (GridView) findViewById(R.id.dashboard_grid);
        gridview.setAdapter(new ImageAdapter(this));
        gridview.setOnItemClickListener(this);

        // Hack to disable GridView scrolling
        gridview.setOnTouchListener(new GridView.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                return event.getAction() == MotionEvent.ACTION_MOVE;
            }
        });

        ActivityCompat.requestPermissions(this,
                new String[]{
                    Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
                }, 0);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View v, int position, long id) {

        Intent intent = new Intent(this, ICONS[position].activity);
        startActivity(intent);

    }

    static class LauncherIcon {
        final int textId;
        final int imgId;
        final Class activity;

        public LauncherIcon(int imgId, int textId, Class activity) {
            super();
            this.imgId = imgId;
            this.textId = textId;
            this.activity = activity;
        }

    }

    static class ImageAdapter extends BaseAdapter {
        private Context mContext;

        public ImageAdapter(Context c) {
            mContext = c;
        }

        @Override
        public int getCount() {
            return ICONS.length;
        }

        @Override
        public LauncherIcon getItem(int position) {
            return null;
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        static class ViewHolder {
            public ImageView icon;
            public TextView text;
        }

        // Create a new ImageView for each item referenced by the Adapter
        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            View v = convertView;
            ViewHolder holder;
            if (v == null) {
                LayoutInflater vi = (LayoutInflater) mContext.getSystemService(
                        Context.LAYOUT_INFLATER_SERVICE);

                v = vi.inflate(R.layout.dashboard_icon, null);
                holder = new ViewHolder();
                holder.text = (TextView) v.findViewById(R.id.dashboard_icon_text);
                holder.icon = (ImageView) v.findViewById(R.id.dashboard_icon_img);
                v.setTag(holder);
            } else {
                holder = (ViewHolder) v.getTag();
            }

            holder.icon.setImageResource(ICONS[position].imgId);
            holder.text.setText(mContext.getString(ICONS[position].textId));

            return v;
        }

    }
}
