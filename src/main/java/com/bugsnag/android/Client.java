package com.bugsnag.android;

import android.app.Activity;
import android.content.Context;

import com.bugsnag.android.http.NetworkException;
import com.bugsnag.android.http.BadResponseException;
import com.bugsnag.android.utils.Async;

public class Client {
    private Context applicationContext;
    private Configuration config;
    private Diagnostics diagnostics;
    private ErrorStore errorStore;

    public Client(Context androidContext, String apiKey, boolean enableMetrics) {
        this(androidContext, apiKey, enableMetrics, true);
    }

    public Client(Context androidContext, String apiKey, boolean enableMetrics, boolean installHandler) {
        if(androidContext == null) {
            throw new RuntimeException("You must provide a non-null android context");
        }

        if(apiKey == null) {
            throw new RuntimeException("You must provide a Bugsnag API key");
        }

        config = new Configuration(apiKey);

        // Get the application context, many things need this
        applicationContext = androidContext.getApplicationContext();
        diagnostics = new Diagnostics(config, applicationContext, this);
        errorStore = new ErrorStore(config, applicationContext);

        // Send metrics data (DAU/MAU etc) if enabled
        if(enableMetrics) {
            makeMetricsRequest();
        }

        // Flush any queued exceptions
        errorStore.flush();

        // Install a default exception handler with this client
        if(installHandler) {
            ExceptionHandler.install(this);
        }

        Logger.info("Bugsnag is loaded and ready to handle exceptions");
    }

    public void setContext(String context) {
        config.context.setLocked(context);
    }

    public void setContext(Activity context) {
        String contextString = ActivityStack.getContextName(context);
        setContext(contextString);
    }

    public void setUser(String id, String email, String name) {
        config.setUser(id, email, name);
    }

    public void setReleaseStage(String releaseStage) {
        config.releaseStage.setLocked(releaseStage);
    }

    public void setNotifyReleaseStages(String... notifyReleaseStages) {
        config.setNotifyReleaseStages(notifyReleaseStages);
    }

    public void setAutoNotify(boolean autoNotify) {
        config.setAutoNotify(autoNotify);
    }

    public void setUseSSL(boolean useSSL) {
        config.setUseSSL(useSSL);
    }

    public void setEndpoint(String endpoint) {
        config.setEndpoint(endpoint);
    }

    public void setFilters(String... filters) {
        config.setFilters(filters);
    }

    public void setProjectPackages(String... projectPackages) {
        config.setProjectPackages(projectPackages);
    }

    public void setOsVersion(String osVersion) {
        config.osVersion.setLocked(osVersion);
    }

    public void setAppVersion(String appVersion) {
        config.appVersion.setLocked(appVersion);
    }

    public void setIgnoreClasses(String... ignoreClasses) {
        config.setIgnoreClasses(ignoreClasses);
    }

    public void setSendThreads(boolean sendThreads) {
        config.setSendThreads(sendThreads);
    }

    public void addBeforeNotify(BeforeNotify beforeNotify) {
        config.addBeforeNotify(beforeNotify);
    }

    public void notify(Throwable e, String severity, MetaData overrides) {
        try {
            // Check if we should notify on this releasestage
            if(!config.shouldNotify()) return;

            // Check if we should ignore this error class
            if(config.shouldIgnore(e.getClass().getName())) return;

            // Create the error object to send
            final Error error = new Error(e, severity, overrides, config, diagnostics);

            // Run beforeNotify callbacks
            if(!beforeNotify(error)) return;

            // Send the error
            Async.safeAsync(new Runnable() {
                @Override
                public void run() {
                    try {
                        Notification notif = new Notification(config, error);
                        notif.deliver();
                    } catch (NetworkException ex) {
                        // Write error to disk for later sending
                        Logger.info("Could not send error(s) to Bugsnag, saving to disk to send later");
                        Logger.info(ex.toString());
                        errorStore.write(error);
                    }
                }
            });
        } catch(Exception ex) {
            Logger.warn("Error notifying Bugsnag", ex);
        }
    }

    public void notify(Throwable e, MetaData metaData) {
        notify(e, null, metaData);
    }

    public void notify(Throwable e, String severity) {
        notify(e, severity, null);
    }

    public void notify(Throwable e) {
        notify(e, null, null);
    }

    public void autoNotify(Throwable e) {
        if(config.autoNotify) {
            notify(e, "error");
        }
    }

    public void addToTab(String tab, String key, Object value) {
        config.addToTab(tab, key, value);
    }

    public void clearTab(String tab) {
        config.clearTab(tab);
    }

    protected boolean beforeNotify(Error error) {
        for (BeforeNotify beforeNotify : config.beforeNotify) {
            try {
                if (!beforeNotify.run(error)) {
                    return false;
                }
            } catch (Throwable ex) {
                Logger.warn("BeforeNotify threw an Exception", ex);
            }
        }

        // By default, allow the error to be sent if there were no objections
        return true;
    }

    private void makeMetricsRequest() {
        Async.safeAsync(new Runnable() {
            @Override
            public void run() {
                try {
                    Metrics metrics = new Metrics(config, diagnostics);
                    metrics.deliver();
                } catch (NetworkException ex) {
                    Logger.info("Could not send metrics to Bugsnag");
                } catch (BadResponseException ex) {
                    // The notification was delivered, but Bugsnag sent a non-200 response
                    Logger.warn(ex.getMessage());
                }
            }
        });
    }
}
