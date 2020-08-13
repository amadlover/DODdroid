package com.ntkinteractive.doddroid

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class MainActivity : AppCompatActivity () {
    private var TAG = "MainActivity";

    override fun onCreate (savedInstanceState : Bundle?) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate: " + stringFromJNI());
    }

    external fun stringFromJNI (): String?

    companion object {
        init {
            System.loadLibrary("hello-jni");
        }
    }
}
