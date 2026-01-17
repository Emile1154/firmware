#pragma once 

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

class Transport {
public:
    virtual uint16_t begin(int timeout_ms = -1) = 0;
    virtual size_t send(const uint8_t *buf, size_t len) = 0;
    virtual size_t receive(uint8_t *buf, size_t len) = 0;

    virtual ~Transport() = default;
};

class UDPTransport : public Transport {

public:
    UDPTransport(const char *remote_ip, uint16_t remote_port, uint16_t local_port, const char *local_ip = "0.0.0.0")
        : sockfd_(-1), remote_port_(remote_port), local_port_(local_port)
    {
        if (remote_ip) strncpy(remote_ip_str_, remote_ip, sizeof(remote_ip_str_) - 1);
        if (local_ip) strncpy(local_ip_str_, local_ip, sizeof(local_ip_str_) - 1);
        remote_ip_str_[sizeof(remote_ip_str_)-1] = 0;
        local_ip_str_[sizeof(local_ip_str_)-1] = 0;
    }

    ~UDPTransport() override { 
        if (sockfd_ >= 0) close(sockfd_); 
    }

    uint16_t begin(int timeout_ms = -1) override {
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd_ < 0) {
            perror("socket");
            return -1;
        }

        // Bind local address
        memset(&local_addr_, 0, sizeof(local_addr_));
        local_addr_.sin_family = AF_INET;
        local_addr_.sin_port = htons(local_port_);
        inet_pton(AF_INET, local_ip_str_, &local_addr_.sin_addr);

        if (bind(sockfd_, (struct sockaddr*)&local_addr_, sizeof(local_addr_)) < 0) {
            perror("bind");
            close(sockfd_);
            sockfd_ = -1;
            return -1;
        }

        // Setup remote address
        memset(&remote_addr_, 0, sizeof(remote_addr_));
        remote_addr_.sin_family = AF_INET;
        remote_addr_.sin_port = htons(remote_port_);
        inet_pton(AF_INET, remote_ip_str_, &remote_addr_.sin_addr);


        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sockfd_, &fds);

        struct timeval tv;
        struct timeval* tv_ptr = nullptr;
        if (timeout_ms >= 0) {
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;
            tv_ptr = &tv;
        }

        int ret = select(sockfd_ + 1, &fds, nullptr, nullptr, tv_ptr);
        if (ret < 0) {
            perror("select");
            return 0;
        } else if (ret == 0) {
            // timeout
            return 0;
        }

        struct sockaddr_in src_addr;
        socklen_t addrlen = sizeof(src_addr);

        return 0;
    }

    size_t send(const uint8_t* buf, size_t len) override {
        return (size_t) sendto(sockfd_, buf, len, 0, (struct sockaddr*)&remote_addr_, sizeof(remote_addr_));
    }

    size_t receive(uint8_t* buf, size_t len) override {
        return (size_t) recvfrom(sockfd_, buf, len, 0, NULL, NULL);
    }

private:
    int sockfd_;
    char remote_ip_str_[64];
    char local_ip_str_[64];
    uint16_t remote_port_;
    uint16_t local_port_;
    struct sockaddr_in remote_addr_;
    struct sockaddr_in local_addr_;
};
