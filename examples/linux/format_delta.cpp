#include <iostream>
#include <pypilot_signalk_connector.hpp>

int main() {
    char out[256];
    pypilot_signalk_connector::make_delta_number(out, sizeof(out),
        pypilot_signalk_connector::SignalKPath::wind_speed_apparent,
        5.144456333854638,
        6);
    std::cout << out << "\n";
    return 0;
}
