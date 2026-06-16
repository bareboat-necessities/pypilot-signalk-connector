#pragma once

#include <stdio.h>
#include <string.h>
#ifdef ARDUINO
#include <Arduino.h>
#endif

namespace pypilot_signalk_connector {

inline void format_signalk_number(char* out, size_t out_size, double value, unsigned decimals = 6) {
    if (!out || out_size == 0) return;
#ifdef ARDUINO
    dtostrf(value, 0, decimals, out);
    char* first = out;
    while (*first == ' ') ++first;
    if (first != out) memmove(out, first, strlen(first) + 1);
#else
    snprintf(out, out_size, "%.*f", static_cast<int>(decimals), value);
#endif
}

inline size_t make_delta_number(char* out, size_t out_size, const char* path, double value, unsigned decimals = 6) {
    if (!out || out_size == 0 || !path) return 0;
    char number[40];
    format_signalk_number(number, sizeof(number), value, decimals);
    int n = snprintf(out, out_size, "{\"updates\":[{\"values\":[{\"path\":\"%s\",\"value\":%s}]}]}", path, number);
    if (n < 0) return 0;
    return static_cast<size_t>(n) < out_size ? static_cast<size_t>(n) : out_size - 1;
}

inline size_t make_delta_bool(char* out, size_t out_size, const char* path, bool value) {
    if (!out || out_size == 0 || !path) return 0;
    int n = snprintf(out, out_size, "{\"updates\":[{\"values\":[{\"path\":\"%s\",\"value\":%s}]}]}", path, value ? "true" : "false");
    if (n < 0) return 0;
    return static_cast<size_t>(n) < out_size ? static_cast<size_t>(n) : out_size - 1;
}

inline size_t make_position_delta(char* out, size_t out_size, double latitude, double longitude, unsigned decimals = 7) {
    if (!out || out_size == 0) return 0;
    char lat[40];
    char lon[40];
    format_signalk_number(lat, sizeof(lat), latitude, decimals);
    format_signalk_number(lon, sizeof(lon), longitude, decimals);
    int n = snprintf(out, out_size, "{\"updates\":[{\"values\":[{\"path\":\"navigation.position\",\"value\":{\"latitude\":%s,\"longitude\":%s}}]}]}", lat, lon);
    if (n < 0) return 0;
    return static_cast<size_t>(n) < out_size ? static_cast<size_t>(n) : out_size - 1;
}

} // namespace pypilot_signalk_connector
