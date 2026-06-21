#pragma once

#include <stddef.h>
#include <stdio.h>
#include "subscription.hpp"

namespace pypilot_signalk_connector {

inline size_t make_unsubscribe_request(char* out, size_t out_size) {
    if (!out || out_size == 0) return 0;
    size_t pos = 0;
    int n = snprintf(out, out_size, R"({"context":"vessels.self","unsubscribe":[)"
    );
    if (n < 0) return 0;
    pos = static_cast<size_t>(n);
    size_t count = 0;
    const char* const* paths = signalk_default_input_paths(count);
    for (size_t i = 0; i < count; ++i) {
        n = snprintf(out + pos, out_size - pos, R"(%s{"path":"%s"})", i ? "," : "", paths[i]);
        if (n < 0) return pos;
        pos += static_cast<size_t>(n);
        if (pos >= out_size) return out_size - 1;
    }
    if (pos + 3 < out_size) {
        out[pos++] = ']';
        out[pos++] = '}';
        out[pos] = '\0';
    }
    return pos;
}

} // namespace pypilot_signalk_connector
