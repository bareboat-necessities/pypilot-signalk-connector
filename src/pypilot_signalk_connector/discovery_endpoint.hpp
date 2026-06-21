#pragma once

#include "transport.hpp"

#if defined(PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS)
#include "mdns_discovery.hpp"
#endif

namespace pypilot_signalk_connector {

#if defined(PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS)
inline bool mdns_result_to_endpoint(const SignalKDiscoveryResult& result, SignalKEndpoint& endpoint) {
    endpoint.set_host(result.host);
    endpoint.set_address(result.address);
    endpoint.http_port = result.port ? result.port : 3000;
    endpoint.websocket_port = endpoint.http_port;
    endpoint.secure = false;
    return endpoint.host[0] != '\0' || endpoint.address[0] != '\0';
}
#endif

} // namespace pypilot_signalk_connector
