#pragma once

#include <pypilot_mdns.hpp>

namespace pypilot_signalk_connector {

struct SignalKDiscoveryResult {
    char host[128]{};
    char address[64]{};
    uint16_t port = 0;
};

class SignalKMdnsDiscovery final {
public:
    explicit SignalKMdnsDiscovery(pypilot_mdns::PypilotMdns& mdns) : mdns_(mdns) {}

    bool discover(SignalKDiscoveryResult& out, uint32_t timeout_ms = 3000) {
        pypilot_mdns::MdnsServiceInfo info;
        if (!mdns_.discover_signalk(info, timeout_ms)) return false;
        pypilot_mdns::mdns_copy(out.host, sizeof(out.host), info.host);
        pypilot_mdns::mdns_copy(out.address, sizeof(out.address), info.address);
        out.port = info.port;
        return true;
    }

    const char* last_error() const { return mdns_.last_error(); }

private:
    pypilot_mdns::PypilotMdns& mdns_;
};

} // namespace pypilot_signalk_connector
