#pragma once

namespace pypilot_signalk_connector {

template<typename Real>
inline Real pi_value() { return static_cast<Real>(3.1415926535897932384626433832795); }

template<typename Real>
inline Real radians_per_degree() { return pi_value<Real>() / static_cast<Real>(180); }

template<typename Real>
inline Real meters_per_second_per_knot() { return static_cast<Real>(0.5144456333854638); }

template<typename Real>
inline Real rad_to_deg(Real radians) { return radians / radians_per_degree<Real>(); }

template<typename Real>
inline Real deg_to_rad(Real degrees) { return degrees * radians_per_degree<Real>(); }

template<typename Real>
inline Real mps_to_knots(Real meters_per_second) { return meters_per_second / meters_per_second_per_knot<Real>(); }

template<typename Real>
inline Real knots_to_mps(Real knots) { return knots * meters_per_second_per_knot<Real>(); }

} // namespace pypilot_signalk_connector
