package com.bugsnag.android;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import android.content.Context;

import com.bugsnag.android.http.NetworkException;
import com.bugsnag.android.utils.Async;

public class ErrorStore {
    private static final String UNSENT_ERROR_PATH = "/bugsnag-errors/";

    private String cachePath;
    private Configuration config;

    public ErrorStore(Configuration config, Context context) {
        this.config = config;

        try {
            cachePath = context.getCacheDir().getAbsolutePath() + UNSENT_ERROR_PATH;

            File outFile = new File(cachePath);
            outFile.mkdirs();
            if(!outFile.exists()) {
                Logger.warn("Could not prepare cache directory");
                cachePath = null;
            }
        } catch(Exception e) {
            Logger.warn("Could not prepare cache directory", e);
            cachePath = null;
        }
    }

    public void write(Error error) {
        if(cachePath == null || error == null) return;

        try {
            String filename = String.format("%s%d.json", cachePath, System.currentTimeMillis());

            String errorString = error.toString();
            if(errorString.length() > 0) {
                FileWriter writer = null;
                try {
                    writer = new FileWriter(filename);
                    writer.write(errorString);
                    writer.flush();
                    Logger.debug(String.format("Saved unsent error to disk (%s) ", filename));
                } finally {
                    if(writer != null) {
                        writer.close();
                    }
                }
            }
        } catch (IOException e) {
            Logger.warn("Unable to save bugsnag error", e);
        }
    }

    public void flush() {
        if(cachePath == null) return;

        Async.safeAsync(new Runnable() {
            @Override
            public void run() {
                // Look up all saved error files
                File exceptionDir = new File(cachePath);
                if(exceptionDir.exists() && exceptionDir.isDirectory()) {
                    Notification notif = null;

                    for(File errorFile : exceptionDir.listFiles()) {
                        try {
                            if(notif == null) notif = new Notification(config);
                            notif.setError(errorFile);
                            notif.deliver();

                            Logger.debug("Deleting sent error file " + errorFile.getName());
                            errorFile.delete();
                        } catch (NetworkException e) {
                            Logger.warn("Could not send error(s) to Bugsnag, will try again later", e);
                        } catch (Exception e) {
                            Logger.warn("Problem sending unsent error from disk", e);
                            errorFile.delete();
                        }
                    }
                }
            }
        });
    }
}
