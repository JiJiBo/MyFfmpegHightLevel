package com.rulerbug.mycpp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.rulerbug.myldemo.Demo
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val demo = Demo()
        // Example of a call to a native method
        sample_text.text = demo.stringFromJNI()
    }


}
