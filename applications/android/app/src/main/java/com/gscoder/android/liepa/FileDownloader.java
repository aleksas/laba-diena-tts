package com.gscoder.android.liepa;

import android.util.Log;

import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;


public class FileDownloader {
    private final static String LOG_TAG = "Liepa_Java_" + FileDownloader.class.getSimpleName();
    public int totalFileLength = 0;
    public int finishedFileLength;

    public boolean abortDownload;
    public boolean finished;
    public boolean success;

    public void saveUrlAsFile(final String url, final String filename) {
        finished = false;
        success = false;
        new Thread() {
            public void run() {
                save(url, filename);
            }
        }.start();
    }

    private boolean save(String url, String filename) {
        try {
            //TODO (aup): Improve the exception handling. This is cruel.

            abortDownload = false;

            Log.v(LOG_TAG,"Trying to save "+url+" as "+filename);
            URL u = new URL(url);
            URLConnection uc = u.openConnection();

            uc.setDoInput(true);
            uc.setDoOutput(true);
            uc.getOutputStream();

            int contentLength = uc.getContentLength();

            totalFileLength = contentLength;
            finishedFileLength = 0;

            InputStream raw = uc.getInputStream();
            InputStream in = new BufferedInputStream(raw,1024);
            FileOutputStream out = new FileOutputStream(filename);

            int bytesRead = 0;
            byte[] data = new byte[1024];

            while (bytesRead != -1) {
                bytesRead = in.read(data, 0, 1024);
                if (bytesRead == -1)
                    break;
                finishedFileLength += bytesRead;
                out.write(data, 0, bytesRead);
                if(abortDownload)
                    break;
            }
            Log.v(LOG_TAG, "FinishedFileLength: " + finishedFileLength);
            in.close();

            out.flush();
            out.close();

            if(abortDownload) {
                Log.e(LOG_TAG, "File download aborted by user");
                success = false;
                finished = true;
                new java.io.File(filename).delete();
                return false;
            }

            if ((contentLength > 0) && (finishedFileLength != contentLength)) {
                throw new IOException("Only read " + finishedFileLength + " bytes; Expected " + contentLength + " bytes");
            }

            finished = true;
            success = true;
            return true;
        }
        catch (Exception e) {
            Log.e("Flite Utility","Could not save url as file.: "+e.getMessage());
            finished = true;
            return false;
        }
    }

    public void abort() {
        abortDownload = true;
    }

}
