#pragma once

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <pypilot_data_model.hpp>
#include "apply_path_value.hpp"

namespace pypilot_signalk_connector {

enum class SignalKJsonValueKind {
    none,
    number,
    boolean,
    string_value,
    null_value
};

struct SignalKJsonValue {
    char path[160]{};
    SignalKJsonValueKind kind = SignalKJsonValueKind::none;
    double number = 0.0;
    bool boolean = false;
    char text[128]{};
};

inline const char* signalk_skip_ws(const char* p) {
    while (p && *p && isspace(static_cast<unsigned char>(*p))) ++p;
    return p;
}

inline const char* signalk_find_key(const char* p, const char* key) {
    if (!p || !key) return nullptr;
    char quoted[96]{};
    snprintf(quoted, sizeof(quoted), "\"%s\"", key);
    return strstr(p, quoted);
}

inline const char* signalk_parse_json_string(const char* p, char* out, size_t out_size) {
    p = signalk_skip_ws(p);
    if (!p || *p != '"' || !out || out_size == 0) return nullptr;
    ++p;
    size_t n = 0;
    while (*p && *p != '"') {
        if (*p == '\\') {
            ++p;
            if (!*p) return nullptr;
        }
        if (n + 1 >= out_size) return nullptr;
        out[n++] = *p++;
    }
    if (*p != '"') return nullptr;
    out[n] = '\0';
    return p + 1;
}

inline const char* signalk_key_value_start(const char* key_pos) {
    if (!key_pos) return nullptr;
    const char* colon = strchr(key_pos, ':');
    if (!colon) return nullptr;
    return signalk_skip_ws(colon + 1);
}

inline const char* signalk_parse_json_value(const char* p, SignalKJsonValue& out) {
    p = signalk_skip_ws(p);
    if (!p) return nullptr;
    if (*p == '"') {
        out.kind = SignalKJsonValueKind::string_value;
        return signalk_parse_json_string(p, out.text, sizeof(out.text));
    }
    if (strncmp(p, "true", 4) == 0) {
        out.kind = SignalKJsonValueKind::boolean;
        out.boolean = true;
        return p + 4;
    }
    if (strncmp(p, "false", 5) == 0) {
        out.kind = SignalKJsonValueKind::boolean;
        out.boolean = false;
        return p + 5;
    }
    if (strncmp(p, "null", 4) == 0) {
        out.kind = SignalKJsonValueKind::null_value;
        return p + 4;
    }
    char* end = nullptr;
    const double value = strtod(p, &end);
    if (end && end != p) {
        out.kind = SignalKJsonValueKind::number;
        out.number = value;
        return end;
    }
    return nullptr;
}

class SignalKDeltaParser final {
public:
    SignalKDeltaParser() : cursor_(nullptr) {}

    void reset(const char* json) { cursor_ = json; }

    bool next(SignalKJsonValue& out) {
        if (!cursor_) return false;
        out = SignalKJsonValue();

        const char* path_key = signalk_find_key(cursor_, "path");
        if (!path_key) return false;
        const char* path_value = signalk_key_value_start(path_key);
        const char* after_path = signalk_parse_json_string(path_value, out.path, sizeof(out.path));
        if (!after_path) {
            cursor_ = path_key + 6;
            return next(out);
        }

        const char* value_key = signalk_find_key(after_path, "value");
        if (!value_key) return false;
        const char* value_start = signalk_key_value_start(value_key);
        const char* after_value = signalk_parse_json_value(value_start, out);
        if (!after_value) {
            cursor_ = value_key + 7;
            return next(out);
        }
        cursor_ = after_value;
        return true;
    }

private:
    const char* cursor_;
};

template<typename Real>
inline size_t apply_signalk_delta_json(const char* json,
                                       SignalKConnector<Real>& connector,
                                       pypilot_data_model::DataModel<Real>& model,
                                       uint64_t now_us) {
    SignalKDeltaParser parser;
    parser.reset(json);
    SignalKJsonValue value;
    size_t applied = 0;
    while (parser.next(value)) {
        if (value.kind == SignalKJsonValueKind::number) {
            if (connector.apply_number(value.path, static_cast<Real>(value.number), model, now_us)) ++applied;
        }
    }
    return applied;
}

} // namespace pypilot_signalk_connector
