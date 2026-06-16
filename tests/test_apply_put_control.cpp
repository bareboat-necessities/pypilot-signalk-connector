#include <cassert>
#include <cmath>
#include <pypilot_signalk_connector.hpp>
#include <pypilot_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace pypilot_data_model;

int main() {
    DataModel<> model;
    SignalKConnector<> c;
    assert(c.apply_put_number(SignalKPath::put_heading_true, radians_per_degree<float>() * 180.0f, model, 100));
    assert(std::fabs(model.ap.heading_command_deg.value - 180.0f) < 0.0002f);
    assert(c.apply_put_bool(SignalKPath::put_engaged, true, model, 200));
    assert(model.ap.enabled.value);
    assert(c.apply_put_string(SignalKPath::put_state, "wind", model, 300));
    assert(model.ap.mode.value == AutopilotMode::wind);
    assert(!c.apply_put_string(SignalKPath::put_state, "unsupported", model, 400));
    return 0;
}
