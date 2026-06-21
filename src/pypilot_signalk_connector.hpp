#pragma once

#include "pypilot_signalk_connector/constants.hpp"
#include "pypilot_signalk_connector/paths.hpp"
#include "pypilot_signalk_connector/apply_path_value.hpp"
#include "pypilot_signalk_connector/format_delta.hpp"
#include "pypilot_signalk_connector/subscription.hpp"
#include "pypilot_signalk_connector/unsubscribe.hpp"
#include "pypilot_signalk_connector/put_control.hpp"
#include "pypilot_signalk_connector/put_messages.hpp"
#include "pypilot_signalk_connector/json_delta.hpp"
#include "pypilot_signalk_connector/outbound_delta.hpp"
#include "pypilot_signalk_connector/transport.hpp"
#include "pypilot_signalk_connector/discovery_endpoint.hpp"

#if defined(PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS)
#include "pypilot_signalk_connector/mdns_discovery.hpp"
#endif
