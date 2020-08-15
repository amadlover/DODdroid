package com.ntkinteractive.doddroid

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class MainActivity : AppCompatActivity () {
    private var TAG = "MainActivity";

    override fun onCreate (savedInstanceState : Bundle?) {
        super.onCreate(savedInstanceState);
        game_init()
    }

    override fun onDestroy() {
        super.onDestroy()
        game_shutdown();
    }

    external fun game_init ();

    external fun game_update ();

    external fun game_shutdown ();

    companion object {
        init {
            System.loadLibrary("game");
        }
    }
}
