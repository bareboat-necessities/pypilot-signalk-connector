#pragma once

#include <stddef.h>
#include "paths.hpp"

namespace pypilot_signalk_connector {

inline bool sk_append_char(char* out, size_t out_size, size_t& pos, char c) {
    if (!out || out_size == 0 || pos + 1 >= out_size) return false;
    out[pos++] = c;
    out[pos] = 0;
    return true;
}

inline bool sk_append_lit(char* out, size_t out_size, size_t& pos, const char* text) {
    if (!text) return false;
    while (*text) {
        if (!sk_append_char(out, out_size, pos, *text++)) return false;
    }
    return true;
}

inline bool sk_append_quoted(char* out, size_t out_size, size_t& pos, const char* text) {
    if (!sk_append_char(out, out_size, pos, '"')) return false;
    if (!sk_append_lit(out, out_size, pos, text ? text : "")) return false;
    return sk_append_char(out, out_size, pos, '"');
}

inline size_t make_unsubscribe_request(char* out, size_t out_size) {
    if (!out || out_size == 0) return 0;
    size_t pos = 0;
    out[0] = 0;
    const char* paths[] = {
        SignalKPath::wind_speed_apparent,
        SignalKPath::wind_angle_apparent,
        SignalKPath::wind_speed_true,
        SignalKPath::wind_angle_true,
        SignalKPath::gps_course_over_ground_true,
        SignalKPath::gps_speed_over_ground,
        SignalKPath::gps_position_latitude,
        SignalKPath::gps_position_longitude,
        SignalKPath::rudder_angle,
        SignalKPath::apb_heading_true,
        SignalKPath::imu_heading_magnetic,
        SignalKPath::imu_attitude_pitch,
        SignalKPath::imu_attitude_roll,
        SignalKPath::imu_attitude_yaw,
        SignalKPath::imu_rate_of_turn,
        SignalKPath::water_speed,
        SignalKPath::water_leeway
    };
    sk_append_char(out, out_size, pos, '{');
    sk_append_quoted(out, out_size, pos, "context");
    sk_append_char(out, out_size, pos, ':');
    sk_append_quoted(out, out_size, pos, "vessels.self");
    sk_append_char(out, out_size, pos, ',');
    sk_append_quoted(out, out_size, pos, "unsubscribe");
    sk_append_char(out, out_size, pos, ':');
    sk_append_char(out, out_size, pos, '[');
    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i) {
        if (i) sk_append_char(out, out_size, pos, ',');
        sk_append_char(out, out_size, pos, '{');
        sk_append_quoted(out, out_size, pos, "path");
        sk_append_char(out, out_size, pos, ':');
        sk_append_quoted(out, out_size, pos, paths[i]);
        sk_append_char(out, out_size, pos, '}');
    }
    sk_append_char(out, out_size, pos, ']');
    sk_append_char(out, out_size, pos, '}');
    return pos;
}

} // namespace pypilot_signalk_connector
