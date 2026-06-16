#include <cassert>
#include <cmath>
#include <pypilot_signalk_connector.hpp>
#include <pypilot_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace pypilot_data_model;

int main() {
    DataModel<> model;
    SignalKConnector<> c;
    assert(c.apply_number(SignalKPath::water_speed, 1.5433369001563914f, model, 100));
    assert(std::fabs(model.water.speed_kn.value - 3.0f) < 0.0002f);
    assert(model.water.source.value == SensorSource::signalk);
    assert(c.apply_number(SignalKPath::water_leeway, radians_per_degree<float>() * -10.0f, model, 200));
    assert(std::fabs(model.water.leeway_deg.value + 10.0f) < 0.0002f);
    assert(model.water.leeway_source.value == SensorSource::signalk);
    return 0;
}
