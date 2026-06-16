#include <cassert>
#include <cstring>
#include <pypilot_signalk_connector.hpp>

using namespace pypilot_signalk_connector;

int main() {
    char out[256];
    make_delta_number(out, sizeof(out), SignalKPath::wind_speed_apparent, 1.25, 2);
    assert(std::strstr(out, "environment.wind.speedApparent"));
    assert(std::strstr(out, "1.25"));
    make_delta_bool(out, sizeof(out), SignalKPath::put_engaged, true);
    assert(std::strstr(out, "true"));
    make_position_delta(out, sizeof(out), 40.1234567, -74.7654321);
    assert(std::strstr(out, "navigation.position"));
    assert(std::strstr(out, "latitude"));
    assert(std::strstr(out, "longitude"));
    return 0;
}
