-keepattributes LineNumberTable,SourceFile
-keep class com.bugsnag.android.ndk.OpaqueValue {
    java.lang.String getJson();
}
-keep class com.bugsnag.android.ndk.NativeBridge { *; }
-keep class com.bugsnag.android.NdkPlugin { *; }
