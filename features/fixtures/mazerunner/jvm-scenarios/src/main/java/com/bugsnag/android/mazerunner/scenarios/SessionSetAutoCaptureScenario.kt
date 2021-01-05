package com.bugsnag.android.mazerunner.scenarios

import android.content.Context
import android.content.Intent
import com.bugsnag.android.Configuration

/**
 * Sets automatic capture of sessions in Bugsnag and flushes 1 session
 */
internal class SessionSetAutoCaptureScenario(
    config: Configuration,
    context: Context
) : Scenario(config, context) {

    init {
        config.autoTrackSessions = true
    }

    override fun run() {
        super.run()
        context.startActivity(Intent("com.bugsnag.android.mazerunner.UPDATE_CONTEXT"))
    }
}
