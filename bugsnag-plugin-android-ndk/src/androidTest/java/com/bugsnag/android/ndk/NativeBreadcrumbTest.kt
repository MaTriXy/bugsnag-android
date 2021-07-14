package com.bugsnag.android.ndk

import org.junit.Test

class NativeBreadcrumbTest {

    companion object {
        init {
            System.loadLibrary("bugsnag-ndk")
            System.loadLibrary("bugsnag-ndk-test")
        }
    }

    external fun run(): Int

    @Test
    fun testPassesNativeSuite() {
        verifyNativeRun(run())
    }
}
