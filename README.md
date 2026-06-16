# pypilot-signalk-connector

Header-only C++11 SignalK connector for pypilot-style data models on Linux and Arduino.

This project is a pure translator between SignalK path/value updates and `pypilot-data-model`.  It deliberately does not include websocket, HTTP, JWT token storage, zeroconf, TLS, reconnect logic, or a SignalK server runtime.

## Public include

```cpp
#include <pypilot_signalk_connector.hpp>
```

## Dependency

```text
pypilot-data-model
```

The default CMake sibling layout is:

```text
../pypilot-data-model/src
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

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```
