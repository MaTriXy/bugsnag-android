#include <cstring>

#include <parson/parson.h>
#include <utils/serializer/buffered_writer.h>

#include "utils.hpp"

static void *create_payload_info_event() {
  auto event = (bugsnag_report_v11 *)calloc(1, sizeof(bugsnag_report_v11));

  strcpy(event->api_key, "5d1e5fbd39a74caa1200142706a90b20");
  strcpy(event->notifier.name, "Test Library");
  strcpy(event->notifier.url, "https://example.com/test-lib");
  strcpy(event->notifier.version, "2.0.11");

  return event;
}

/**
 * Create a new event in v10 format
 */
static void *create_full_event() {
  auto event = (bugsnag_report_v11 *)calloc(1, sizeof(bugsnag_report_v11));

  strcpy(event->context,
         "00000000000m0r3.61ee9e6e099d3dd7448f740d395768da6b2df55d5.m4g1c");
  strcpy(event->grouping_hash,
         "a1d34088a096987361ee9e6e099d3dd7448f740d395768da6b2df55d5160f33");
  event->severity = BSG_SEVERITY_INFO;

  // app
  strcpy(event->app.binary_arch, "mips");
  strcpy(event->app.build_uuid, "1234-9876-adfe");
  event->app.duration = 81395165021;
  event->app.duration_in_foreground = 81395165010;
  event->app.in_foreground = true;
  event->app.is_launching = true;
  strcpy(event->app.id, "com.example.PhotoSnapPlus");
  strcpy(event->app.release_stage, "リリース");
  strcpy(event->app.type, "red");
  strcpy(event->app.version, "2.0.52");
  event->app.version_code = 8139512718;

  // breadcrumbs
  auto max = 50;
  event->crumb_first_index = 2; // test the circular buffer logic
  char name[30];
  for (int i = event->crumb_first_index; i < max; i++) {
    sprintf(name, "mission %d", i - event->crumb_first_index);
    insert_crumb(event->breadcrumbs, i, name, BSG_CRUMB_STATE, 1638992630014,
                 "Now we know what they mean by 'advanced' tactical training.");
  }
  for (int i = 0; i < event->crumb_first_index; i++) {
    sprintf(name, "mission %d", (max - event->crumb_first_index) + i);
    insert_crumb(event->breadcrumbs, i, name, BSG_CRUMB_STATE, 1638992630014,
                 "Now we know what they mean by 'advanced' tactical training.");
  }
  event->crumb_count = max;

  // device
  event->device.cpu_abi_count = 1;
  strcpy(event->device.cpu_abi[0].value, "mipsx");
  strcpy(event->device.id, "ffffa");
  event->device.jailbroken = true;
  strcpy(event->device.locale, "en_AU#Melbun");
  strcpy(event->device.manufacturer, "HI-TEC™");
  strcpy(event->device.model, "🍨");
  strcpy(event->device.orientation, "sideup");
  strcpy(event->device.os_name, "BOX BOX");
  strcpy(event->device.os_version, "98.7");
  { // -- runtime versions
    strcpy(event->device.os_build, "beta1-2");
    event->device.api_level = 32;
  }
  event->device.time = 1638992630;
  event->device.total_memory = 3839512576;

  // feature flags
  event->feature_flag_count = 4;
  event->feature_flags =
      (bsg_feature_flag *)calloc(4, sizeof(bsg_feature_flag));
  event->feature_flags[0].name = strdup("bluebutton");
  event->feature_flags[0].variant = strdup("on");
  event->feature_flags[1].name = strdup("redbutton");
  event->feature_flags[1].variant = strdup("off");
  event->feature_flags[2].name = strdup("nobutton");
  event->feature_flags[3].name = strdup("switch");
  event->feature_flags[3].variant = strdup("left");

  // exceptions
  strcpy(event->error.errorClass, "SIGBUS");
  strcpy(event->error.errorMessage, "POSIX is serious about oncoming traffic");
  strcpy(event->error.type, "C");
  event->error.frame_count = 2;
  event->error.stacktrace[0].frame_address = (uintptr_t)4294967294;
  event->error.stacktrace[0].load_address = (uintptr_t)2367523;
  event->error.stacktrace[0].symbol_address = 776;
  event->error.stacktrace[0].line_number = (uintptr_t)4194967233;
  strcpy(event->error.stacktrace[0].method, "makinBacon");
  strcpy(event->error.stacktrace[0].filename, "lib64/libfoo.so");
  event->error.stacktrace[1].frame_address =
      (uintptr_t)3011142731; // will become method hex

  // metadata
  strcpy(event->app.active_screen, "Menu");
  bugsnag_event_add_metadata_bool(event, "metrics", "experimentX", false);
  bugsnag_event_add_metadata_string(event, "metrics", "subject", "percy");
  bugsnag_event_add_metadata_string(event, "app", "weather", "rain");
  bugsnag_event_add_metadata_double(event, "metrics", "counter", 47.5);

  // session info
  event->handled_events = 5;
  event->unhandled_events = 2;
  strcpy(event->session_id, "aaaaaaaaaaaaaaaa");
  strcpy(event->session_start, "2031-07-09T11:08:21+00:00");

  // threads
  event->thread_count = 8;
  for (int i = 0; i < event->thread_count; i++) {
    event->threads[i].id = 1000 + i;
    sprintf(event->threads[i].name, "Thread #%d", i);
    sprintf(event->threads[i].state, "paused-%d", i);
  }

  // user
  strcpy(event->user.email, "fenton@io.example.com");
  strcpy(event->user.name, "Fenton");
  strcpy(event->user.id, "fex01");

  return event;
}

bool bsg_write_feature_flags(bugsnag_event *event, bsg_buffered_writer *writer);

bool bsg_write_opaque_metadata(bugsnag_event *event,
                               bsg_buffered_writer *writer);

bool bsg_report_header_write(bsg_report_header *header, int fd) {
  ssize_t len = write(fd, header, sizeof(bsg_report_header));

  return len == sizeof(bsg_report_header);
}

bool bsg_event_write_v11(bsg_environment *env) {
  bsg_buffered_writer writer;
  if (!bsg_buffered_writer_open(&writer, env->next_event_path)) {
    return false;
  }

  bool result =
      // write header - determines format version, etc
      bsg_report_header_write(&env->report_header, writer.fd) &&
      // add cached event info
      writer.write(&writer, &env->next_event, sizeof(bugsnag_event)) &&
      // append feature flags after event structure
      bsg_write_feature_flags(&env->next_event, &writer) &&
      // append opaque metadata after the feature flags
      bsg_write_opaque_metadata(&env->next_event, &writer);

  writer.dispose(&writer);

  if (result && env->static_json_data != NULL) {
    // Attempt to write the static data, but don't worry if it fails.
    // We'll check for truncated/missing static data on load.
    if (bsg_buffered_writer_open(&writer, env->next_event_static_data_path)) {
      writer.write(&writer, env->static_json_data,
                   strlen(env->static_json_data));
      writer.dispose(&writer);
    }
  }

  return result;
}

bool bsg_lastrun_write(bsg_environment *env) {
  char *path = env->last_run_info_path;
  int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    return false;
  }

  int size = strlen(env->next_last_run_info);
  ssize_t len = write(fd, env->next_last_run_info, size);
  return len == size;
}

static bool write_feature_flag(bsg_buffered_writer *writer,
                               bsg_feature_flag *flag) {
  if (!writer->write_string(writer, flag->name)) {
    return false;
  }

  if (flag->variant) {
    if (!writer->write_byte(writer, 1)) {
      return false;
    }

    if (!writer->write_string(writer, flag->variant)) {
      return false;
    }
  } else {
    if (!writer->write_byte(writer, 0)) {
      return false;
    }
  }

  return true;
}

bool bsg_write_feature_flags(bugsnag_event *event,
                             bsg_buffered_writer *writer) {
  const uint32_t feature_flag_count = event->feature_flag_count;
  if (!writer->write(writer, &feature_flag_count, sizeof(feature_flag_count))) {
    return false;
  }

  for (uint32_t index = 0; index < feature_flag_count; index++) {
    if (!write_feature_flag(writer, &event->feature_flags[index])) {
      return false;
    }
  }

  // write a single byte to indicate whether the feature flags were modified
  // when writing them out, we always write 0 here to mark them as valid
  writer->write_byte(writer, 0);

  return true;
}

static bool bsg_write_opaque_metadata_unit(bugsnag_metadata *metadata,
                                           bsg_buffered_writer *writer) {

  for (size_t index = 0; index < metadata->value_count; index++) {
    uint32_t value_size = metadata->values[index].opaque_value_size;
    if (metadata->values[index].type == BSG_METADATA_OPAQUE_VALUE &&
        value_size > 0) {
      if (!writer->write(writer, metadata->values[index].opaque_value,
                         value_size)) {
        return false;
      }
    }
  }

  return true;
}

bool bsg_write_opaque_metadata(bugsnag_event *event,
                               bsg_buffered_writer *writer) {

  if (!bsg_write_opaque_metadata_unit(&event->metadata, writer)) {
    return false;
  }

  for (int breadcrumb_index = 0; breadcrumb_index < event->crumb_count;
       breadcrumb_index++) {
    if (!bsg_write_opaque_metadata_unit(
        &event->breadcrumbs[breadcrumb_index].metadata, writer)) {
      return false;
    }
  }

  return true;
}

static const char *write_event_v11(JNIEnv *env, jstring temp_file,
                                  void *(event_generator)()) {
  auto event_ctx = (bsg_environment *)calloc(1, sizeof(bsg_environment));
  event_ctx->report_header.version = 11;
  const char *path = (*env).GetStringUTFChars(temp_file, nullptr);
  sprintf(event_ctx->next_event_path, "%s", path);

  // (old format) event struct -> file on disk
  void *old_event = event_generator();
  memcpy(&event_ctx->next_event, old_event, sizeof(bugsnag_event));
  free(old_event);
  bsg_event_write_v11(event_ctx);
  free(event_ctx);
  return path;
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL
Java_com_bugsnag_android_ndk_migrations_EventMigrationV11Tests_migratePayloadInfo(
    JNIEnv *env, jobject _this, jstring temp_file) {
  const char *path = write_event_v11(env, temp_file, create_payload_info_event);

  // file on disk -> latest event type
  bugsnag_event *parsed_event = bsg_deserialize_event_from_file((char *)path);

  // write json object
  JSON_Value *event_val = json_value_init_object();
  JSON_Object *event_obj = json_value_get_object(event_val);
  json_object_set_string(event_obj, "apiKey", parsed_event->api_key);
  json_object_set_string(event_obj, "notifierName",
                         parsed_event->notifier.name);
  json_object_set_string(event_obj, "notifierURL", parsed_event->notifier.url);
  json_object_set_string(event_obj, "notifierVersion",
                         parsed_event->notifier.version);
  char *json_str = json_serialize_to_string(event_val);
  auto result = (*env).NewStringUTF(json_str);
  free(json_str);

  return result;
}

JNIEXPORT void JNICALL
Java_com_bugsnag_android_ndk_migrations_EventMigrationV11Tests_migrateEvent(
    JNIEnv *env, jobject _this, jstring temp_file) {
  const char *path = write_event_v11(env, temp_file, create_full_event);

  // file on disk -> latest event type
  bugsnag_event *parsed_event = bsg_deserialize_event_from_file((char *)path);
  char *output = bsg_serialize_event_to_json_string(parsed_event);
  for (int i = 0; i < parsed_event->feature_flag_count; i++) {
    free(parsed_event->feature_flags[i].name);
    free(parsed_event->feature_flags[i].variant);
  }
  free(parsed_event->feature_flags);
  free(parsed_event);

  // latest event type -> temp file
  write_str_to_file(output, path);
  free(output);
}

#ifdef __cplusplus
}
#endif
