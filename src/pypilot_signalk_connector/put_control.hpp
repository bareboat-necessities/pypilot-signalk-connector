#pragma once

#include "apply_path_value.hpp"

namespace pypilot_signalk_connector {

// pypilot/signalk.py exposes only these PUT paths:
// - steering.autopilot.target.headingMagnetic
// - steering.autopilot.target.headingTrue
// - steering.autopilot.state
// - steering.autopilot.engaged
// They are implemented by SignalKConnector::apply_put_number,
// SignalKConnector::apply_put_bool, and SignalKConnector::apply_put_string.

} // namespace pypilot_signalk_connector
