#include <cassert>
#include <cmath>
#include <pypilot_signalk_connector.hpp>
#include <ship_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace ship_data_model;

int main() {
    DataModel<> model;
    SignalKConnector<> c;
    assert(c.apply_number(SignalKPath::rudder_angle, radians_per_degree<float>() * 10.0f, model, 100));
    assert(std::fabs(model.rudder.angle_deg.value + 10.0f) < 0.0002f);
    assert(model.rudder.source.value == SensorSource::signalk);
    return 0;
}
