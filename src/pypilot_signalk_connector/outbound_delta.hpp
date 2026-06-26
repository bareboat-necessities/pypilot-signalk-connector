#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <ship_data_model.hpp>
#include "paths.hpp"
#include "constants.hpp"

namespace pypilot_signalk_connector {

inline size_t signalk_append_number_value(char* out,
                                          size_t out_size,
                                          size_t pos,
                                          const char* path,
                                          double value,
                                          bool comma) {
    if (!out || pos >= out_size) return pos;
    const int n = snprintf(out + pos, out_size - pos,
                           "%s{\"path\":\"%s\",\"value\":%.9g}",
                           comma ? "," : "", path, value);
    if (n < 0) return pos;
    const size_t next = pos + static_cast<size_t>(n);
    return next < out_size ? next : out_size - 1;
}

template<typename Real>
inline size_t make_signalk_delta(char* out,
                                 size_t out_size,
                                 const ship_data_model::DataModel<Real>& model,
                                 const char* source_label = "pypilot") {
    if (!out || out_size == 0) return 0;
    int n = snprintf(out, out_size, "{\"context\":\"vessels.self\",\"updates\":[{\"source\":{\"label\":\"%s\"},\"values\":[", source_label ? source_label : "pypilot");
    if (n < 0) return 0;
    size_t pos = static_cast<size_t>(n);
    bool comma = false;

    if (model.wind.apparent.speed_kn.valid) {
        pos = signalk_append_number_value(out, out_size, pos, SignalKPath::wind_speed_apparent,
                                          knots_to_mps(model.wind.apparent.speed_kn.value), comma);
        comma = true;
    }
    if (model.wind.apparent.direction_deg.valid) {
        pos = signalk_append_number_value(out, out_size, pos, SignalKPath::wind_angle_apparent,
                                          deg_to_rad(model.wind.apparent.direction_deg.value), comma);
        comma = true;
    }
    if (model.navigation.gps.speed_kn.valid) {
        pos = signalk_append_number_value(out, out_size, pos, SignalKPath::gps_speed_over_ground,
                                          knots_to_mps(model.navigation.gps.speed_kn.value), comma);
        comma = true;
    }
    if (model.navigation.gps.track_deg.valid) {
        pos = signalk_append_number_value(out, out_size, pos, SignalKPath::gps_course_over_ground_true,
                                          deg_to_rad(model.navigation.gps.track_deg.value), comma);
        comma = true;
    }
    if (model.imu.heading_lowpass_deg.valid) {
        pos = signalk_append_number_value(out, out_size, pos, SignalKPath::imu_heading_magnetic,
                                          deg_to_rad(model.imu.heading_lowpass_deg.value), comma);
        comma = true;
    }
    if (model.rudder.angle_deg.valid) {
        pos = signalk_append_number_value(out, out_size, pos, SignalKPath::rudder_angle,
                                          deg_to_rad(-model.rudder.angle_deg.value), comma);
        comma = true;
    }

    if (pos + 4 < out_size) {
        out[pos++] = ']';
        out[pos++] = '}';
        out[pos++] = ']';
        out[pos++] = '}';
        out[pos] = '\0';
    }
    return pos;
}

} // namespace pypilot_signalk_connector
