package com.gscoder.android.labadienatts;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 * Created by alex on 3/19/2017.
 */


public class Utility {

    public static boolean pathExists(String pathname){
        File tempFile = new File(pathname);
        return tempFile.exists();
    }

    public static ArrayList<String> readLines(String filename) throws IOException {
        ArrayList<String> strLines = new ArrayList<String>();
        FileInputStream fstream = new FileInputStream(filename);
        DataInputStream in = new DataInputStream(fstream);
        BufferedReader br = new BufferedReader(new InputStreamReader(in),1024);
        String strLine;
        while ((strLine = br.readLine()) != null)   {
            strLines.add(strLine);
        }
        in.close();
        return strLines;
    }
}


