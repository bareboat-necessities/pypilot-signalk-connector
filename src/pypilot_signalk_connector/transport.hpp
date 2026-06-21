#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace pypilot_signalk_connector {

struct SignalKEndpoint {
    char host[128]{};
    char address[64]{};
    uint16_t http_port = 3000;
    uint16_t websocket_port = 3000;
    bool secure = false;

    void set_host(const char* value) {
        copy(host, sizeof(host), value);
    }

    void set_address(const char* value) {
        copy(address, sizeof(address), value);
    }

    static void copy(char* dst, size_t dst_size, const char* src) {
        if (!dst || dst_size == 0) return;
        if (!src) src = "";
        size_t i = 0;
        for (; i + 1 < dst_size && src[i]; ++i) dst[i] = src[i];
        dst[i] = '\0';
    }
};

class ISignalKWebSocketClient {
public:
    virtual ~ISignalKWebSocketClient() = default;
    virtual bool connect(const SignalKEndpoint& endpoint, const char* token) = 0;
    virtual bool send_text(const char* text) = 0;
    virtual int receive_text(char* out, size_t out_size) = 0;
    virtual bool connected() const = 0;
    virtual void close() = 0;
};

class ISignalKHttpClient {
public:
    virtual ~ISignalKHttpClient() = default;
    virtual bool request_access_token(const SignalKEndpoint& endpoint,
                                      const char* client_id,
                                      char* token_out,
                                      size_t token_out_size) = 0;
};

class ISignalKTokenStore {
public:
    virtual ~ISignalKTokenStore() = default;
    virtual bool load(char* token_out, size_t token_out_size) = 0;
    virtual bool save(const char* token) = 0;
    virtual bool clear() = 0;
};

class MemorySignalKTokenStore final : public ISignalKTokenStore {
public:
    bool load(char* token_out, size_t token_out_size) override {
        if (!token_out || token_out_size == 0 || token_[0] == '\0') return false;
        SignalKEndpoint::copy(token_out, token_out_size, token_);
        return true;
    }

    bool save(const char* token) override {
        SignalKEndpoint::copy(token_, sizeof(token_), token);
        return token_[0] != '\0';
    }

    bool clear() override {
        token_[0] = '\0';
        return true;
    }

private:
    char token_[192]{};
};

class SignalKReconnectBackoff final {
public:
    SignalKReconnectBackoff(uint32_t initial_ms = 1000, uint32_t max_ms = 60000)
        : initial_ms_(initial_ms), max_ms_(max_ms), current_ms_(initial_ms) {}

    uint32_t next_delay_ms() {
        const uint32_t delay = current_ms_;
        if (current_ms_ < max_ms_ / 2) current_ms_ *= 2;
        else current_ms_ = max_ms_;
        return delay;
    }

    void reset() { current_ms_ = initial_ms_; }
    uint32_t current_ms() const { return current_ms_; }

private:
    uint32_t initial_ms_;
    uint32_t max_ms_;
    uint32_t current_ms_;
};

} // namespace pypilot_signalk_connector
