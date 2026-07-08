package com.example.modloader;
import android.app.Activity;
import android.os.Bundle;
public class MainActivity extends Activity {
    static { System.loadLibrary("mod"); }
    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // A lib já foi carregada, o construtor já rodou
    }
}