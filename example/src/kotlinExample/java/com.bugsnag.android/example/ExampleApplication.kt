package com.bugsnag.android.example

import android.app.Application
import com.bugsnag.android.Bugsnag

class ExampleApplication : Application() {

    override fun onCreate() {
        super.onCreate()
        Bugsnag.init(this)
    }

}
