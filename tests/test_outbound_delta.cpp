#include <cassert>
#include <cstring>
#include <pypilot_signalk_connector.hpp>
#include <ship_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace ship_data_model;

int main() {
    DataModel<> model;
    model.wind.apparent.speed_kn.set(10.0f, 100);
    model.wind.apparent.direction_deg.set(90.0f, 100);
    model.navigation.gps.speed_kn.set(5.0f, 100);
    model.navigation.gps.track_deg.set(123.0f, 100);

    char json[1536]{};
    const size_t n = make_signalk_delta(json, sizeof(json), model);
    assert(n > 0);
    assert(std::strstr(json, "updates") != nullptr);
    assert(std::strstr(json, SignalKPath::wind_speed_apparent) != nullptr);
    assert(std::strstr(json, SignalKPath::gps_course_over_ground_true) != nullptr);
    assert(std::strstr(json, "pypilot") != nullptr);
    return 0;
}
