package com.gscoder.android.liepa;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.webkit.WebView;

public class OpenSourceLicenses extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_source_licenses);

        WebView wv;
        wv = (WebView) findViewById(R.id.webview);
        wv.loadUrl("file:///android_asset/opensourcelicenses.html");
    }
}
