package com.gscoder.android.labadienatts;

import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebView;

import com.gscoder.android.labadienatts.R;

public class OpenSourceLicenses extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_source_licenses);

        WebView wv;
        wv = (WebView) findViewById(R.id.webview);
        wv.loadUrl("file:///android_asset/opensourcelicenses.html");
    }
}
