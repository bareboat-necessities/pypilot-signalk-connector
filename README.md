# pypilot-signalk-connector

C++17 Signal K connector for pypilot-style data models on Linux and Arduino.

This project translates Signal K path/value updates into `pypilot-data-model` and formats pypilot data as Signal K deltas. When `pypilot-mdns` is available as a sibling checkout, it also exposes a small Signal K mDNS discovery helper.

It deliberately does not include websocket, HTTP, JWT token storage, TLS, reconnect logic, or a Signal K server runtime. Network transport stays in higher-level daemon/application code.

## Public include

```cpp
#include <pypilot_signalk_connector.hpp>
```

## Dependencies

Required:

```text
pypilot-data-model
```

Optional:

```text
pypilot-mdns
```

The default CMake sibling layout is:

```text
../pypilot-data-model/src
../pypilot-mdns
```

A standalone configure without `../pypilot-mdns` succeeds. In that case `PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS` is not defined and the mDNS discovery helper is not exported from the umbrella header.

## Signal K mDNS discovery

Original Python pypilot discovers Signal K by browsing `_http._tcp.local.` and filtering TXT records for:

```text
swname=signalk-server
```

When `pypilot-mdns` is present, this connector exposes that through:

```cpp
pypilot_mdns::PypilotMdns mdns;
mdns.begin("pypilot");

pypilot_signalk_connector::SignalKMdnsDiscovery discovery(mdns);
pypilot_signalk_connector::SignalKDiscoveryResult result;
if (discovery.discover(result, 3000)) {
    // result.host, result.address, result.port
}
```

## Scope from pypilot/signalk.py

Input path/value mappings:

```text
environment.wind.speedApparent      m/s -> wind.apparent.speed_kn
environment.wind.angleApparent      rad -> wind.apparent.direction_deg
environment.wind.speedTrue          m/s -> wind.truewind.speed_kn
environment.wind.angleTrue          rad -> wind.truewind.direction_deg
navigation.courseOverGroundTrue     rad -> gps.track_deg
navigation.speedOverGround          m/s -> gps.speed_kn
navigation.position.latitude        deg -> gps.fix_lat_deg
navigation.position.longitude       deg -> gps.fix_lon_deg
steering.rudderAngle                rad -> rudder.angle_deg with pypilot sign
steering.autopilot.target.headingTrue rad -> apb.track_deg
navigation.headingMagnetic          rad -> imu.heading_lowpass_deg
navigation.attitude.pitch           rad -> imu.pitch_deg
navigation.attitude.roll            rad -> imu.roll_deg
navigation.attitude.yaw             rad -> imu.heading_lowpass_deg
navigation.rateOfTurn               rad/s -> imu.heading_rate_lowpass_deg_s
navigation.speedThroughWater        m/s -> water.speed_kn
navigation.leewayAngle              rad -> water.leeway_deg
```

PUT/control paths:

```text
steering.autopilot.target.headingMagnetic -> ap.heading_command_deg
steering.autopilot.target.headingTrue     -> ap.heading_command_deg
steering.autopilot.state                  -> ap.mode
steering.autopilot.engaged                -> ap.enabled
```

## Build

Standalone build:

```bash
cmake -S . -B build \
  -DPYPILOT_DATA_MODEL_DIR=../pypilot-data-model/src
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Build with mDNS discovery enabled:

```bash
cmake -S . -B build \
  -DPYPILOT_DATA_MODEL_DIR=../pypilot-data-model/src \
  -DPYPILOT_MDNS_DIR=../pypilot-mdns
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```
