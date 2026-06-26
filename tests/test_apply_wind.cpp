#include <cassert>
#include <cmath>
#include <pypilot_signalk_connector.hpp>
#include <ship_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace ship_data_model;

int main() {
    DataModel<> model;
    SignalKConnector<> c;
    assert(c.apply_number(SignalKPath::wind_speed_apparent, 5.144456333854638f, model, 100));
    assert(std::fabs(model.wind.apparent.speed_kn.value - 10.0f) < 0.0002f);
    assert(model.wind.apparent.source.value == SensorSource::signalk);

    assert(c.apply_number(SignalKPath::wind_angle_apparent, radians_per_degree<float>() * 45.0f, model, 200));
    assert(std::fabs(model.wind.apparent.direction_deg.value - 45.0f) < 0.0002f);

    assert(c.apply_number(SignalKPath::wind_speed_true, 2.572228166927319f, model, 300));
    assert(std::fabs(model.wind.truewind.speed_kn.value - 5.0f) < 0.0002f);

    assert(c.apply_number(SignalKPath::wind_angle_true, radians_per_degree<float>() * -20.0f, model, 400));
    assert(std::fabs(model.wind.truewind.direction_deg.value + 20.0f) < 0.0002f);
    return 0;
}
