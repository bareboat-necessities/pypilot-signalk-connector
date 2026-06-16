#include <cassert>
#include <cstring>
#include <pypilot_signalk_connector.hpp>

using namespace pypilot_signalk_connector;

int main() {
    char out[4096];
    make_subscription_request(out, sizeof(out), SignalKPolicy::fixed, 500);
    assert(std::strstr(out, "vessels.self"));
    assert(std::strstr(out, "fixed"));
    assert(std::strstr(out, "environment.wind.speedApparent"));
    assert(std::strstr(out, "navigation.speedThroughWater"));
    assert(std::strstr(out, "steering.autopilot.target.headingTrue"));
    return 0;
}
