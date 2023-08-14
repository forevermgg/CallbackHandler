package com.mgg.callbackhandler

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.mgg.callbackhandler.databinding.ActivityMainBinding
import org.chromium.base.library_loader.Linker

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        val linker = Linker()
        linker.loadLibrary("callbackhandler")
        binding.sampleText.text = stringFromJNI()
    }

    /**
     * A native method that is implemented by the 'callbackhandler' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
}