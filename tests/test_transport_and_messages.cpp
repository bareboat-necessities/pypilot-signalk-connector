#include <cassert>
#include <cstring>
#include <pypilot_signalk_connector.hpp>

using namespace pypilot_signalk_connector;

int main() {
    MemorySignalKTokenStore store;
    char token[64]{};
    assert(!store.load(token, sizeof(token)));
    assert(store.save("abc123"));
    assert(store.load(token, sizeof(token)));
    assert(std::strcmp(token, "abc123") == 0);
    assert(store.clear());
    assert(!store.load(token, sizeof(token)));

    SignalKReconnectBackoff backoff(100, 1000);
    assert(backoff.next_delay_ms() == 100);
    assert(backoff.next_delay_ms() == 200);
    assert(backoff.next_delay_ms() == 400);
    backoff.reset();
    assert(backoff.next_delay_ms() == 100);

    char msg[2048]{};
    make_subscription_request(msg, sizeof(msg));
    assert(std::strstr(msg, "subscribe") != nullptr);
    assert(std::strstr(msg, SignalKPath::gps_speed_over_ground) != nullptr);

    make_unsubscribe_request(msg, sizeof(msg));
    assert(std::strstr(msg, "unsubscribe") != nullptr);
    assert(std::strstr(msg, SignalKPath::wind_speed_apparent) != nullptr);

    make_put_bool_request(msg, sizeof(msg), SignalKPath::put_engaged, true, "r1");
    assert(std::strstr(msg, "steering.autopilot.engaged") != nullptr);
    assert(std::strstr(msg, "true") != nullptr);

    SignalKPutResponse ok = parse_put_response("state COMPLETED requestId r1");
    assert(ok.complete);
    assert(!ok.error);
    SignalKPutResponse fail = parse_put_response("state FAILED error denied");
    assert(fail.error);

    return 0;
}
