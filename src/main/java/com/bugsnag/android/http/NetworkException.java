package com.bugsnag.android.http;

import java.io.IOException;

public class NetworkException extends IOException {
    public NetworkException(String message, Throwable cause) {
        super(message);
        initCause(cause);
    }
}
