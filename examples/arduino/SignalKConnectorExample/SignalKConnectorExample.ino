#include <Arduino.h>
#include <pypilot_signalk_connector.hpp>
#include <pypilot_data_model.hpp>

pypilot_data_model::DataModel<> model;
pypilot_signalk_connector::SignalKConnector<> connector;

void setup() {
  Serial.begin(115200);
  connector.apply_number(pypilot_signalk_connector::SignalKPath::wind_speed_apparent,
                         5.144456333854638f,
                         model,
                         1000000);
  Serial.println(model.wind.apparent.speed_kn.value);
}

void loop() {
}
