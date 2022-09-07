//
// Created by Karl Stenerud on 05.09.22.
//

#ifndef BUGSNAG_ANDROID_INTERNAL_METRICS_H
#define BUGSNAG_ANDROID_INTERNAL_METRICS_H

#include "event.h"

/**
 * APIs whose calls will be recorded.
 *
 * This enum must remain consistent with bsg_called_api_names in bugsnag_ndk.c.
 * The ordering must not be changed. Add new APIs to the end, not in
 * alphabetical order. Naming of enums can be changed to denote deprecation
 * (e.g. BSG_API_APP_GET_ID_DEPRECATED), but their enum values must not be
 * re-used.
 */
typedef enum {
  BSG_API_APP_GET_BINARY_ARCH = 0,
  BSG_API_APP_GET_BUILD_UUID,
  BSG_API_APP_GET_DURATION,
  BSG_API_APP_GET_DURATION_IN_FOREGROUND,
  BSG_API_APP_GET_ID,
  BSG_API_APP_GET_IN_FOREGROUND,
  BSG_API_APP_GET_IS_LAUNCHING,
  BSG_API_APP_GET_RELEASE_STAGE,
  BSG_API_APP_GET_TYPE,
  BSG_API_APP_GET_VERSION,
  BSG_API_APP_GET_VERSION_CODE,
  BSG_API_APP_SET_BINARY_ARCH,
  BSG_API_APP_SET_BUILD_UUID,
  BSG_API_APP_SET_DURATION,
  BSG_API_APP_SET_DURATION_IN_FOREGROUND,
  BSG_API_APP_SET_ID,
  BSG_API_APP_SET_IN_FOREGROUND,
  BSG_API_APP_SET_IS_LAUNCHING,
  BSG_API_APP_SET_RELEASE_STAGE,
  BSG_API_APP_SET_TYPE,
  BSG_API_APP_SET_VERSION,
  BSG_API_APP_SET_VERSION_CODE,
  BSG_API_DEVICE_GET_ID,
  BSG_API_DEVICE_GET_JAILBROKEN,
  BSG_API_DEVICE_GET_LOCALE,
  BSG_API_DEVICE_GET_MANUFACTURER,
  BSG_API_DEVICE_GET_MODEL,
  BSG_API_DEVICE_GET_ORIENTATION,
  BSG_API_DEVICE_GET_OS_NAME,
  BSG_API_DEVICE_GET_OS_VERSION,
  BSG_API_DEVICE_GET_TIME,
  BSG_API_DEVICE_GET_TOTAL_MEMORY,
  BSG_API_DEVICE_SET_ID,
  BSG_API_DEVICE_SET_JAILBROKEN,
  BSG_API_DEVICE_SET_LOCALE,
  BSG_API_DEVICE_SET_MANUFACTURER,
  BSG_API_DEVICE_SET_MODEL,
  BSG_API_DEVICE_SET_ORIENTATION,
  BSG_API_DEVICE_SET_OS_NAME,
  BSG_API_DEVICE_SET_OS_VERSION,
  BSG_API_DEVICE_SET_TIME,
  BSG_API_DEVICE_SET_TOTAL_MEMORY,
  BSG_API_ERROR_GET_ERROR_CLASS,
  BSG_API_ERROR_GET_ERROR_MESSAGE,
  BSG_API_ERROR_GET_ERROR_TYPE,
  BSG_API_ERROR_SET_ERROR_CLASS,
  BSG_API_ERROR_SET_ERROR_MESSAGE,
  BSG_API_ERROR_SET_ERROR_TYPE,
  BSG_API_EVENT_ADD_METADATA_BOOL,
  BSG_API_EVENT_ADD_METADATA_DOUBLE,
  BSG_API_EVENT_ADD_METADATA_STRING,
  BSG_API_EVENT_CLEAR_METADATA,
  BSG_API_EVENT_CLEAR_METADATA_SECTION,
  BSG_API_EVENT_GET_API_KEY,
  BSG_API_EVENT_GET_CONTEXT,
  BSG_API_EVENT_GET_GROUPING_HASH,
  BSG_API_EVENT_GET_METADATA_BOOL,
  BSG_API_EVENT_GET_METADATA_DOUBLE,
  BSG_API_EVENT_GET_METADATA_STRING,
  BSG_API_EVENT_GET_SEVERITY,
  BSG_API_EVENT_GET_STACKFRAME,
  BSG_API_EVENT_GET_STACKTRACE_SIZE,
  BSG_API_EVENT_GET_USER,
  BSG_API_EVENT_HAS_METADATA,
  BSG_API_EVENT_IS_UNHANDLED,
  BSG_API_EVENT_SET_API_KEY,
  BSG_API_EVENT_SET_CONTEXT,
  BSG_API_EVENT_SET_GROUPING_HASH,
  BSG_API_EVENT_SET_SEVERITY,
  BSG_API_EVENT_SET_UNHANDLED,
  BSG_API_EVENT_SET_USER,
} bsg_called_api;

extern const char *const bsg_called_api_names[];
extern const int bsg_called_apis_count;

void bsg_set_internal_metrics_enabled(bool enabled);

void bsg_notify_api_called(bugsnag_event *event, bsg_called_api api);
bool bsg_was_api_called(const bugsnag_event *event, bsg_called_api api);

void bsg_set_callback_count(bugsnag_event *event, const char *api,
                            int32_t count);
void bsg_notify_add_callback(bugsnag_event *event, const char *api);
void bsg_notify_remove_callback(bugsnag_event *event, const char *api);

#endif // BUGSNAG_ANDROID_INTERNAL_METRICS_H
