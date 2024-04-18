plugins {
    id("bugsnag-build-plugin")
}

apply(plugin = "com.android.library")

dependencies {
    add("api", project(":bugsnag-android-core"))
}

apply(from = "../gradle/kotlin.gradle")

dependencies {
    "compileOnly"("com.squareup.okhttp3:okhttp:4.9.1") {
        exclude(group = "org.jetbrains.kotlin")
    }

    "testImplementation"("com.squareup.okhttp3:mockwebserver:4.9.1") {
        exclude(group = "org.jetbrains.kotlin")
    }
}
