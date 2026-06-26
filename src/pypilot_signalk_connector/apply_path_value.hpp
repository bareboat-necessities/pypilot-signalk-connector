#pragma once

#include <ship_data_model.hpp>
#include "constants.hpp"
#include "paths.hpp"

namespace pypilot_signalk_connector {

template<typename Real = float>
class SignalKConnector {
public:
    SignalKConnector() : last_error_("") {}

    const char* last_error() const { return last_error_; }

    bool apply_number(const char* path,
                      Real value,
                      ship_data_model::DataModel<Real>& model,
                      uint64_t now_us,
                      ship_data_model::SensorSource source = ship_data_model::SensorSource::signalk) {
        last_error_ = "";

        if (streq(path, SignalKPath::wind_speed_apparent)) {
            model.wind.apparent.speed_kn.set(mps_to_knots(value), now_us);
            set_source(model.wind.apparent.source, source);
            model.wind.apparent.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::wind_angle_apparent)) {
            model.wind.apparent.direction_deg.set(rad_to_deg(value), now_us);
            set_source(model.wind.apparent.source, source);
            model.wind.apparent.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::wind_speed_true)) {
            model.wind.truewind.speed_kn.set(mps_to_knots(value), now_us);
            set_source(model.wind.truewind.source, source);
            model.wind.truewind.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::wind_angle_true)) {
            model.wind.truewind.direction_deg.set(rad_to_deg(value), now_us);
            set_source(model.wind.truewind.source, source);
            model.wind.truewind.last_update_us = now_us;
            return true;
        }

        if (streq(path, SignalKPath::gps_course_over_ground_true)) {
            model.navigation.gps.track_deg.set(rad_to_deg(value), now_us);
            set_source(model.navigation.gps.source, source);
            model.navigation.gps.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::gps_speed_over_ground)) {
            model.navigation.gps.speed_kn.set(mps_to_knots(value), now_us);
            set_source(model.navigation.gps.source, source);
            model.navigation.gps.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::gps_position_latitude)) {
            model.navigation.gps.fix_lat_deg.set(value, now_us);
            set_source(model.navigation.gps.source, source);
            model.navigation.gps.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::gps_position_longitude)) {
            model.navigation.gps.fix_lon_deg.set(value, now_us);
            set_source(model.navigation.gps.source, source);
            model.navigation.gps.last_update_us = now_us;
            return true;
        }

        if (streq(path, SignalKPath::rudder_angle)) {
            model.rudder.angle_deg.set(-rad_to_deg(value), now_us);
            set_source(model.rudder.source, source);
            model.rudder.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::apb_heading_true)) {
            model.navigation.apb.track_deg.set(rad_to_deg(value), now_us);
            set_source(model.navigation.apb.source, source);
            model.navigation.apb.last_update_us = now_us;
            return true;
        }

        if (streq(path, SignalKPath::imu_heading_magnetic)) {
            model.imu.heading_lowpass_deg.set(rad_to_deg(value), now_us);
            return true;
        }
        if (streq(path, SignalKPath::imu_attitude_pitch)) {
            model.imu.pitch_deg.set(rad_to_deg(value), now_us);
            return true;
        }
        if (streq(path, SignalKPath::imu_attitude_roll)) {
            model.imu.roll_deg.set(rad_to_deg(value), now_us);
            return true;
        }
        if (streq(path, SignalKPath::imu_attitude_yaw)) {
            model.imu.heading_lowpass_deg.set(rad_to_deg(value), now_us);
            return true;
        }
        if (streq(path, SignalKPath::imu_rate_of_turn)) {
            model.imu.heading_rate_lowpass_deg_s.set(rad_to_deg(value), now_us);
            return true;
        }

        if (streq(path, SignalKPath::water_speed)) {
            model.water.speed_kn.set(mps_to_knots(value), now_us);
            set_source(model.water.source, source);
            model.water.last_update_us = now_us;
            return true;
        }
        if (streq(path, SignalKPath::water_leeway)) {
            model.water.leeway_deg.set(rad_to_deg(value), now_us);
            set_source(model.water.leeway_source, source);
            model.water.last_update_us = now_us;
            return true;
        }

        last_error_ = "unsupported SignalK number path";
        return false;
    }

    bool apply_put_number(const char* path,
                          Real value,
                          ship_data_model::DataModel<Real>& model,
                          uint64_t now_us) {
        last_error_ = "";
        if (streq(path, SignalKPath::put_heading_magnetic) || streq(path, SignalKPath::put_heading_true)) {
            model.ap.heading_command_deg.set(rad_to_deg(value), now_us);
            return true;
        }
        last_error_ = "unsupported SignalK PUT number path";
        return false;
    }

    bool apply_put_bool(const char* path,
                        bool value,
                        ship_data_model::DataModel<Real>& model,
                        uint64_t) {
        last_error_ = "";
        if (streq(path, SignalKPath::put_engaged)) {
            model.ap.enabled.value = value;
            return true;
        }
        last_error_ = "unsupported SignalK PUT boolean path";
        return false;
    }

    bool apply_put_string(const char* path,
                          const char* value,
                          ship_data_model::DataModel<Real>& model,
                          uint64_t) {
        last_error_ = "";
        if (streq(path, SignalKPath::put_state)) {
            ship_data_model::AutopilotMode mode;
            if (ship_data_model::autopilot_mode_from_name(value, mode)) {
                model.ap.mode.value = mode;
                return true;
            }
            last_error_ = "unsupported autopilot state";
            return false;
        }
        last_error_ = "unsupported SignalK PUT string path";
        return false;
    }

private:
    const char* last_error_;

    template<typename Setting>
    void set_source(Setting& setting, ship_data_model::SensorSource source) {
        if (source != ship_data_model::SensorSource::none) setting.value = source;
    }
};

} // namespace pypilot_signalk_connector
