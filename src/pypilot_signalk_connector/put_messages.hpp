#pragma once

#include <stddef.h>
#include <stdio.h>
#include <string.h>

namespace pypilot_signalk_connector {

inline size_t make_put_number_request(char* out, size_t out_size, const char* path, double value, const char* request_id = "pypilot-put") {
    if (!out || out_size == 0 || !path) return 0;
    int n = snprintf(out, out_size,
                     "{\"context\":\"vessels.self\",\"requestId\":\"%s\",\"put\":{\"path\":\"%s\",\"value\":%.9g}}",
                     request_id ? request_id : "pypilot-put", path, value);
    return n > 0 ? static_cast<size_t>(n) : 0;
}

inline size_t make_put_bool_request(char* out, size_t out_size, const char* path, bool value, const char* request_id = "pypilot-put") {
    if (!out || out_size == 0 || !path) return 0;
    int n = snprintf(out, out_size,
                     "{\"context\":\"vessels.self\",\"requestId\":\"%s\",\"put\":{\"path\":\"%s\",\"value\":%s}}",
                     request_id ? request_id : "pypilot-put", path, value ? "true" : "false");
    return n > 0 ? static_cast<size_t>(n) : 0;
}

inline size_t make_put_string_request(char* out, size_t out_size, const char* path, const char* value, const char* request_id = "pypilot-put") {
    if (!out || out_size == 0 || !path) return 0;
    int n = snprintf(out, out_size,
                     "{\"context\":\"vessels.self\",\"requestId\":\"%s\",\"put\":{\"path\":\"%s\",\"value\":\"%s\"}}",
                     request_id ? request_id : "pypilot-put", path, value ? value : "");
    return n > 0 ? static_cast<size_t>(n) : 0;
}

struct SignalKPutResponse {
    bool complete = false;
    bool error = false;
};

inline SignalKPutResponse parse_put_response(const char* json) {
    SignalKPutResponse out;
    if (!json) return out;
    out.complete = strstr(json, "COMPLETED") != nullptr || strstr(json, "completed") != nullptr;
    out.error = strstr(json, "FAILED") != nullptr || strstr(json, "failed") != nullptr || strstr(json, "error") != nullptr;
    return out;
}

} // namespace pypilot_signalk_connector
