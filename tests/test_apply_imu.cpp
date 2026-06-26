#include <cassert>
#include <cmath>
#include <pypilot_signalk_connector.hpp>
#include <ship_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace ship_data_model;

int main() {
    DataModel<> model;
    SignalKConnector<> c;
    assert(c.apply_number(SignalKPath::imu_heading_magnetic, radians_per_degree<float>() * 90.0f, model, 100));
    assert(std::fabs(model.imu.heading_lowpass_deg.value - 90.0f) < 0.0002f);
    assert(c.apply_number(SignalKPath::imu_attitude_pitch, radians_per_degree<float>() * 3.0f, model, 200));
    assert(c.apply_number(SignalKPath::imu_attitude_roll, radians_per_degree<float>() * -4.0f, model, 300));
    assert(c.apply_number(SignalKPath::imu_attitude_yaw, radians_per_degree<float>() * 123.0f, model, 400));
    assert(c.apply_number(SignalKPath::imu_rate_of_turn, radians_per_degree<float>() * 1.5f, model, 500));
    assert(std::fabs(model.imu.pitch_deg.value - 3.0f) < 0.0002f);
    assert(std::fabs(model.imu.roll_deg.value + 4.0f) < 0.0002f);
    assert(std::fabs(model.imu.heading_lowpass_deg.value - 123.0f) < 0.0002f);
    assert(std::fabs(model.imu.heading_rate_lowpass_deg_s.value - 1.5f) < 0.0002f);
    return 0;
}
