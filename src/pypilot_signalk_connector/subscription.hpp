#pragma once

#include <stdio.h>
#include "paths.hpp"

namespace pypilot_signalk_connector {

enum class SignalKPolicy {
    instant,
    fixed,
    ideal
};

inline const char* signalk_policy_name(SignalKPolicy policy) {
    switch (policy) {
    case SignalKPolicy::instant: return "instant";
    case SignalKPolicy::fixed: return "fixed";
    case SignalKPolicy::ideal: return "ideal";
    }
    return "instant";
}

inline size_t append_subscription(char* out, size_t out_size, size_t pos, const char* path, const char* policy, unsigned period_ms, bool comma) {
    if (pos >= out_size) return out_size ? out_size - 1 : 0;
    int n = snprintf(out + pos, out_size - pos,
                     "%s{\"path\":\"%s\",\"period\":%u,\"policy\":\"%s\"}",
                     comma ? "," : "", path, period_ms, policy);
    if (n < 0) return pos;
    pos += static_cast<size_t>(n);
    return pos < out_size ? pos : out_size - 1;
}

inline size_t make_subscription_request(char* out, size_t out_size, SignalKPolicy policy = SignalKPolicy::instant, unsigned period_ms = 1000) {
    if (!out || out_size == 0) return 0;
    const char* p = signalk_policy_name(policy);
    size_t pos = 0;
    int n = snprintf(out, out_size, "{\"context\":\"vessels.self\",\"subscribe\":[");
    if (n < 0) return 0;
    pos = static_cast<size_t>(n);
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
    for (size_t i = 0; i < sizeof(paths)/sizeof(paths[0]); ++i) {
        pos = append_subscription(out, out_size, pos, paths[i], p, period_ms, i != 0);
    }
    if (pos + 3 < out_size) {
        out[pos++] = ']';
        out[pos++] = '}';
        out[pos] = '\0';
    }
    return pos;
}

} // namespace pypilot_signalk_connector
