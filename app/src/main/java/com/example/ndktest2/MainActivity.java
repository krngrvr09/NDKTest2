package com.example.ndktest2;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    public native String  stringFromJNI();
    static {
        System.loadLibrary("test_file");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toast toast = Toast.makeText(getApplicationContext(),
                stringFromJNI(),
                Toast.LENGTH_SHORT);

        toast.show();

    }
}