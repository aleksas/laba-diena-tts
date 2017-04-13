package com.gscoder.android.liepa;

import android.Manifest;
import android.app.AlertDialog;
import android.app.DownloadManager;
import android.app.ListActivity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class DownloadVoiceData extends ListActivity {
    private final static String LOG_TAG = "Flite_Java_" + DownloadVoiceData.class.getSimpleName();
    private VoiceListAdapter mListAdapter;
    private Context mContext;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        registerReceiver(onComplete,
                new IntentFilter(DownloadManager.ACTION_DOWNLOAD_COMPLETE));

        mListAdapter = new VoiceListAdapter(this);
        setListAdapter(mListAdapter);
        mContext = this;


        ActivityCompat.requestPermissions(this,
                new String[]{
                        Manifest.permission.INTERNET,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE,
                }, 0);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        unregisterReceiver(onComplete);
    }

    public void onResume() {
        super.onResume();
        mListAdapter.refresh();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(R.string.voice_list_update);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Only works for a single menu option.
        // User must have requested a refresh of the voice list.

        Toast toast = Toast.makeText(mContext, "Downloading Voice List", Toast.LENGTH_SHORT);
        toast.show();

        Thread thread = new Thread() {
            @Override
            public void run() {
                CheckVoiceData.DownloadVoiceList(new Runnable() {
                    @Override
                    public void run() {
                        runOnUiThread(new Runnable() {

                            @Override
                            public void run() {
                                mListAdapter.refresh();

                            }
                        });
                    }
                });

            }
        };

        thread.start();
        return true;

    }

    private class VoiceListAdapter extends BaseAdapter {

        private Context mContext;
        private ArrayList<Voice> mVoiceList;
        private LayoutInflater mInflater;

        public VoiceListAdapter(Context context) {
            mContext = context;
            mInflater = LayoutInflater.from(mContext);

            // Get Information about voices
            mVoiceList = CheckVoiceData.getVoices();

            if (mVoiceList.isEmpty()) {
                Intent intent = new Intent(mContext, CheckVoiceData.class);
                startActivity(intent);
            }
        }

        public void refresh() {
            mVoiceList = CheckVoiceData.getVoices();
            notifyDataSetChanged();
        }

        @Override
        public int getCount() {
            // TODO Auto-generated method stub
            return mVoiceList.size();
        }

        @Override
        public Object getItem(int position) {
            // TODO Auto-generated method stub
            return position;
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(final int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                convertView = mInflater.inflate(R.layout.view_voice_manager, parent, false);
            }

            ((TextView) convertView.findViewById(R.id.voice_manager_voice_language)).setText(mVoiceList.get(position).getDisplayLanguage());
            ((TextView) convertView.findViewById(R.id.voice_manager_voice_variant)).setText(mVoiceList.get(position).getVariant());
            final ImageButton actionButton = (ImageButton) convertView.findViewById(R.id.voice_manager_action_image);
            actionButton.setImageResource(
                    mVoiceList.get(position).isAvailable()?R.drawable.ic_action_delete:R.drawable.ic_action_download);
            actionButton.setVisibility(View.VISIBLE);

            actionButton.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    final Voice vox = mVoiceList.get(position);
                    if (!vox.isAvailable()) {
                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setMessage("Data Alert: Download Size up to 500MB.");
                        builder.setCancelable(false);
                        builder.setPositiveButton("Download Voice", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                // Create destination directory
                                File f = new File(vox.getPath() + ".zip");
                                if (f.exists()) {
                                    f.delete();
                                }
                                //String url = Voice.getDownloadURLBasePath() + vox.getVariant() + ".zip";
                                String url = vox.getDownloadURL();
                                DownloadManager.Request request = new DownloadManager.Request(Uri.parse(url));
                                request.setDescription("Downloading Liepa Voice: " + vox.getName());
                                request.setTitle(vox.getVariant() + ".zip");
                                request.setDestinationUri(Uri.fromFile(new File(vox.getPath() + ".zip")));

                                DownloadManager manager = (DownloadManager) mContext.getSystemService(Context.DOWNLOAD_SERVICE);
                                manager.enqueue(request);
                                Toast toast = Toast.makeText(mContext, "Download Started", Toast.LENGTH_SHORT);
                                toast.show();
                                actionButton.setVisibility(View.INVISIBLE);

                            }
                        });
                        builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.cancel();
                            }
                        });
                        AlertDialog alert = builder.create();
                        alert.show();
                    }
                    else {
                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setMessage("Sure? Deleting " + vox.getDisplayName());
                        builder.setCancelable(false);
                        builder.setPositiveButton("Delete Voice", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                File f = new File(vox.getPath());

                                deleteRecursive(f);

                                refresh();
                                Toast toast = Toast.makeText(mContext, "Voice Deleted", Toast.LENGTH_SHORT);
                                toast.show();
                            }
                        });
                        builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.cancel();
                            }
                        });
                        AlertDialog alert = builder.create();
                        alert.show();
                    }
                }
            });

            convertView.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    actionButton.performClick();
                }
            });

            return convertView;
        }

    }

    void deleteRecursive(File fileOrDirectory) {
        if (fileOrDirectory.isDirectory())
            for (File child : fileOrDirectory.listFiles())
                deleteRecursive(child);

        fileOrDirectory.delete();
    }

    BroadcastReceiver onComplete=new BroadcastReceiver() {
        public void onReceive(Context ctxt, Intent intent) {
            Toast toast = Toast.makeText(ctxt, "Liepa TTS Voice Data Downloaded!", Toast.LENGTH_SHORT);
            toast.show();

            new ProcessNext().execute(intent);
        }
    };

    class ProcessNext extends AsyncTask<Intent, Integer, Integer> {
        @Override
        protected Integer doInBackground(Intent... params) {
            return processNext(params[0]);
        }

        protected int processNext(Intent intent) {
            DownloadManager.Query query = new DownloadManager.Query();
            ObjectOutputStream oos = null;
            Long dwnId = intent.getLongExtra(DownloadManager.EXTRA_DOWNLOAD_ID, 0);
            query.setFilterById(dwnId);
            DownloadManager manager = (DownloadManager) mContext.getSystemService(Context.DOWNLOAD_SERVICE);
            Cursor cursor = manager.query(query);
            if (cursor.moveToFirst()) {
                int status = cursor.getInt(cursor.getColumnIndex(DownloadManager.COLUMN_STATUS));
                if (status == DownloadManager.STATUS_SUCCESSFUL) {
                    String title = cursor.getString(cursor.getColumnIndex(DownloadManager.COLUMN_TITLE));
                    File f1 = new File(Voice.getDataStorageBasePath() + title);
                    if (f1.exists()) {
                        unpackZip(Voice.getDataStorageBasePath(), title);
                        mListAdapter.refresh();
                    }
                }
            }
            return 0;
        }

        private boolean unpackZip(String path, String zipname) {
            InputStream is;
            ZipInputStream zis;
            try {
                String filename;
                String fullPath = new File(path, zipname).getPath();
                is = new FileInputStream(fullPath);
                zis = new ZipInputStream(new BufferedInputStream(is));
                ZipEntry ze;
                byte[] buffer = new byte[1024];
                int count;

                Log.i(LOG_TAG, "Unpacking " + fullPath);
                while ((ze = zis.getNextEntry()) != null) {
                    // zapis do souboru
                    filename = ze.getName();

                    // Need to create directories if not exists, or
                    // it will generate an Exception...
                    File fmd = new File(path, filename);

                    if (ze.isDirectory()) {
                        fmd.mkdirs();
                        continue;
                    }

                    FileOutputStream fout = new FileOutputStream(fmd);

                    // cteni zipu a zapis
                    while ((count = zis.read(buffer)) != -1) {
                        fout.write(buffer, 0, count);
                    }

                    fout.close();
                    zis.closeEntry();
                    Log.i(LOG_TAG, filename);
                }

                zis.close();
                Log.i(LOG_TAG, "Deleting " + fullPath);
                new File(fullPath).delete();
            } catch (IOException e) {
                e.printStackTrace();
                return false;
            }

            return true;
        }
    }
}
