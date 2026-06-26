#include <cassert>
#include <cmath>
#include <pypilot_signalk_connector.hpp>
#include <ship_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace ship_data_model;

int main() {
    DataModel<> model;
    SignalKConnector<> c;
    assert(c.apply_number(SignalKPath::gps_course_over_ground_true, radians_per_degree<float>() * 123.0f, model, 100));
    assert(std::fabs(model.navigation.gps.track_deg.value - 123.0f) < 0.0002f);
    assert(c.apply_number(SignalKPath::gps_speed_over_ground, 2.572228166927319f, model, 200));
    assert(std::fabs(model.navigation.gps.speed_kn.value - 5.0f) < 0.0002f);
    assert(c.apply_number(SignalKPath::gps_position_latitude, 40.1234f, model, 300));
    assert(c.apply_number(SignalKPath::gps_position_longitude, -74.5678f, model, 400));
    assert(std::fabs(model.navigation.gps.fix_lat_deg.value - 40.1234f) < 0.0001f);
    assert(std::fabs(model.navigation.gps.fix_lon_deg.value + 74.5678f) < 0.0001f);
    assert(model.navigation.gps.source.value == SensorSource::signalk);
    return 0;
}
