package com.bugsnag.android;

import com.bugsnag.android.internal.ImmutableConfig;
import com.bugsnag.android.internal.ImmutableConfigKt;

import androidx.annotation.NonNull;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;

final class BugsnagTestUtils {

    static HashMap<String, Object> runtimeVersions = new HashMap<>();

    static {
        runtimeVersions.put("osBuild", "bulldog");
        runtimeVersions.put("androidApiLevel", "24");
    }

    static Configuration generateConfiguration() {
        Configuration configuration = new Configuration("5d1ec5bd39a74caa1267142706a7fb21");
        configuration.setDelivery(generateDelivery());
        configuration.setLogger(NoopLogger.INSTANCE);
        configuration.setProjectPackages(Collections.singleton("com.example.foo"));
        try {
            File dir = Files.createTempDirectory("test").toFile();
            configuration.setPersistenceDirectory(dir);
        } catch (IOException ignored) {
            // ignore IO exception
        }
        return configuration;
    }

    static ImmutableConfig generateImmutableConfig() {
        return convert(generateConfiguration());
    }

    static ImmutableConfig generateImmutableConfig(Configuration config) {
        config.setDelivery(generateDelivery());
        config.setLogger(NoopLogger.INSTANCE);
        config.setProjectPackages(Collections.singleton("com.example.foo"));
        try {
            File dir = Files.createTempDirectory("test").toFile();
            config.setPersistenceDirectory(dir);
        } catch (IOException ignored) {
            // ignore IO exception
        }
        return ImmutableConfigKt.convertToImmutableConfig(config);
    }

    static FeatureFlagState generateFeatureFlagState() {
        return new FeatureFlagState();
    }

    static EventPayload generateEventPayload(ImmutableConfig config) {
        return new EventPayload(config.getApiKey(), generateEvent(), new Notifier(), config);
    }

    static Session generateSession() {
        return new Session("test", new Date(), new User(), false,
                new Notifier(), NoopLogger.INSTANCE);
    }

    static Event generateEvent() {
        Throwable exc = new RuntimeException();
        Event event = new Event(
                exc,
                BugsnagTestUtils.generateImmutableConfig(),
                SeverityReason.newInstance(SeverityReason.REASON_HANDLED_EXCEPTION),
                NoopLogger.INSTANCE
        );
        event.setApp(generateAppWithState());
        event.setDevice(generateDeviceWithState());
        return event;
    }

    static ImmutableConfig convert(Configuration config) {
        return ImmutableConfigKt.convertToImmutableConfig(config, null);
    }

    static DeviceBuildInfo generateDeviceBuildInfo() {
        return new DeviceBuildInfo(
                "samsung", "s7", "7.1", 24, "bulldog",
                "foo-google", "prod,build", "google", new String[]{"armeabi-v7a"}
        );
    }

    static Device generateDevice() {
        DeviceBuildInfo buildInfo = generateDeviceBuildInfo();
        return new Device(buildInfo, new String[]{}, null, null, null, 10923250000L,
                runtimeVersions);
    }

    static DeviceWithState generateDeviceWithState() {
        DeviceBuildInfo buildInfo = generateDeviceBuildInfo();
        return new DeviceWithState(buildInfo,null, null, null,
                109230923452L, runtimeVersions, 22234423124L,
                92340255592L, "portrait", new Date(0));
    }

    public static Delivery generateDelivery() {
        return new Delivery() {
            @NonNull
            @Override
            public DeliveryStatus deliver(@NonNull EventPayload payload,
                                          @NonNull DeliveryParams deliveryParams) {
                return DeliveryStatus.DELIVERED;
            }

            @NonNull
            @Override
            public DeliveryStatus deliver(@NonNull Session payload,
                                          @NonNull DeliveryParams deliveryParams) {
                return DeliveryStatus.DELIVERED;
            }
        };
    }

    public static AppWithState generateAppWithState() {
        return new AppWithState(generateImmutableConfig(), null, null, null,
                null, null, null, null, null, null);
    }

    public static App generateApp() {
        return new App(generateImmutableConfig(), null, null, null, null, null);
    }

    static MetadataState generateMetadataState() {
        return new MetadataState();
    }
}
