#include <iostream>
#include <pypilot_signalk_connector.hpp>
#include <ship_data_model.hpp>

int main() {
    ship_data_model::DataModel<> model;
    pypilot_signalk_connector::SignalKConnector<> connector;
    connector.apply_number(pypilot_signalk_connector::SignalKPath::wind_speed_apparent,
                           5.144456333854638f,
                           model,
                           1000000);
    std::cout << "apparent wind speed kn=" << model.wind.apparent.speed_kn.value << "\n";
    return 0;
}
