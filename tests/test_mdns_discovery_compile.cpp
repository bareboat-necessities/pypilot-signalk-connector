#include <cassert>

#include <pypilot_signalk_connector.hpp>

int main() {
    pypilot_mdns::PypilotMdns mdns;
    pypilot_signalk_connector::SignalKMdnsDiscovery discovery(mdns);
    assert(discovery.last_error() != nullptr);
    return 0;
}
