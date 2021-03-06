package com.bugsnag.android.mazerunner.scenarios

import android.content.Context
import com.bugsnag.android.Bugsnag
import com.bugsnag.android.Configuration

/**
 * Attempts to send a handled exception to Bugsnag, when the release stage is set.
 */
internal class InsideReleaseStageScenario(config: Configuration,
                                          context: Context) : Scenario(config, context) {

    override fun run() {
        super.run()
        Bugsnag.setReleaseStage("prod")
        Bugsnag.setNotifyReleaseStages("prod")
        Bugsnag.notify(RuntimeException("InsideReleaseStageScenario"))
    }

}
