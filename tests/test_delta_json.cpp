#include <cassert>
#include <cmath>
#include <cstring>
#include <pypilot_signalk_connector.hpp>
#include <pypilot_data_model.hpp>

using namespace pypilot_signalk_connector;
using namespace pypilot_data_model;

static void add_char(char* out, size_t& pos, char c) { out[pos++] = c; out[pos] = 0; }
static void add_lit(char* out, size_t& pos, const char* s) { while (*s) add_char(out, pos, *s++); }
static void add_q(char* out, size_t& pos, const char* s) { add_char(out, pos, '"'); add_lit(out, pos, s); add_char(out, pos, '"'); }
static void add_pair(char* out, size_t& pos, const char* path, const char* value, bool comma) {
    if (comma) add_char(out, pos, ',');
    add_char(out, pos, '{');
    add_q(out, pos, "path"); add_char(out, pos, ':'); add_q(out, pos, path);
    add_char(out, pos, ',');
    add_q(out, pos, "value"); add_char(out, pos, ':'); add_lit(out, pos, value);
    add_char(out, pos, '}');
}

int main() {
    char json[1536]{};
    size_t pos = 0;
    add_char(json, pos, '{');
    add_q(json, pos, "context"); add_char(json, pos, ':'); add_q(json, pos, "vessels.self"); add_char(json, pos, ',');
    add_q(json, pos, "updates"); add_char(json, pos, ':'); add_char(json, pos, '['); add_char(json, pos, '{');
    add_q(json, pos, "values"); add_char(json, pos, ':'); add_char(json, pos, '[');
    add_pair(json, pos, SignalKPath::wind_speed_apparent, "5.144456333854638", false);
    add_pair(json, pos, SignalKPath::wind_angle_apparent, "1.5707963267948966", true);
    add_pair(json, pos, SignalKPath::gps_position_latitude, "40.1234", true);
    add_pair(json, pos, SignalKPath::gps_position_longitude, "-74.5678", true);
    add_pair(json, pos, SignalKPath::gps_speed_over_ground, "2.572228166927319", true);
    add_pair(json, pos, SignalKPath::gps_course_over_ground_true, "2.1467549799530254", true);
    add_char(json, pos, ']'); add_char(json, pos, '}'); add_char(json, pos, ']'); add_char(json, pos, '}');

    DataModel<> model;
    SignalKConnector<> connector;
    const size_t applied = apply_signalk_delta_json(json, connector, model, 12345);
    assert(applied == 6);
    assert(std::fabs(model.wind.apparent.speed_kn.value - 10.0f) < 0.0002f);
    assert(std::fabs(model.wind.apparent.direction_deg.value - 90.0f) < 0.0002f);
    assert(std::fabs(model.navigation.gps.fix_lat_deg.value - 40.1234f) < 0.0001f);
    assert(std::fabs(model.navigation.gps.fix_lon_deg.value + 74.5678f) < 0.0001f);
    assert(std::fabs(model.navigation.gps.speed_kn.value - 5.0f) < 0.0002f);
    assert(std::fabs(model.navigation.gps.track_deg.value - 123.0f) < 0.0002f);
    return 0;
}
