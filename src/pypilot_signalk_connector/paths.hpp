#pragma once

namespace pypilot_signalk_connector {

struct SignalKPath {
    static constexpr const char* wind_speed_apparent = "environment.wind.speedApparent";
    static constexpr const char* wind_angle_apparent = "environment.wind.angleApparent";
    static constexpr const char* wind_speed_true = "environment.wind.speedTrue";
    static constexpr const char* wind_angle_true = "environment.wind.angleTrue";

    static constexpr const char* gps_course_over_ground_true = "navigation.courseOverGroundTrue";
    static constexpr const char* gps_speed_over_ground = "navigation.speedOverGround";
    static constexpr const char* gps_position_latitude = "navigation.position.latitude";
    static constexpr const char* gps_position_longitude = "navigation.position.longitude";

    static constexpr const char* rudder_angle = "steering.rudderAngle";
    static constexpr const char* apb_heading_true = "steering.autopilot.target.headingTrue";

    static constexpr const char* imu_heading_magnetic = "navigation.headingMagnetic";
    static constexpr const char* imu_attitude_pitch = "navigation.attitude.pitch";
    static constexpr const char* imu_attitude_roll = "navigation.attitude.roll";
    static constexpr const char* imu_attitude_yaw = "navigation.attitude.yaw";
    static constexpr const char* imu_rate_of_turn = "navigation.rateOfTurn";

    static constexpr const char* water_speed = "navigation.speedThroughWater";
    static constexpr const char* water_leeway = "navigation.leewayAngle";

    static constexpr const char* put_heading_magnetic = "steering.autopilot.target.headingMagnetic";
    static constexpr const char* put_heading_true = "steering.autopilot.target.headingTrue";
    static constexpr const char* put_state = "steering.autopilot.state";
    static constexpr const char* put_engaged = "steering.autopilot.engaged";
};

inline bool streq(const char* a, const char* b) {
    if (!a || !b) return false;
    while (*a && *b && *a == *b) { ++a; ++b; }
    return *a == '\0' && *b == '\0';
}

} // namespace pypilot_signalk_connector
