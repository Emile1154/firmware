
#ifdef ARCH_PORTDUINO
#pragma once
#include "StreamAPI.h"

#include "Stream.h"


#include <iostream>
#include <cstring> // For memset
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()
#include <stdexcept>
#include <atomic>
#include <iostream>
#include <sys/ioctl.h>

class TCPSocketStream : public Stream {
private:
  
    int client_fd;

    struct sockaddr_in server_addr;
    uint16_t port;

public:
    int socket_fd;
    TCPSocketStream(uint16_t port) : port(port), socket_fd(-1) {
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any interface

        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            throw std::runtime_error("Socket creation failed");
        }

        if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            close(socket_fd);
            throw std::runtime_error("Bind failed");
        }
        if (listen(socket_fd, 5) != 0) {
            close(socket_fd);
            throw std::runtime_error("listen failed");
        } else {
            struct sockaddr_in  cli; 
            int len = sizeof(cli);
             client_fd = accept(socket_fd, (sockaddr* ) &cli, (socklen_t *) &len );
            if (client_fd < 0) {
                close(socket_fd);
                throw std::runtime_error("Socket accept failed");
            }
        }
    }


    int available() override {
        int count;
        ioctl(client_fd, FIONREAD, &count);
        return count;
    }

    int read() override {
        uint8_t character;
        recv(client_fd, &character, sizeof(character), 0);
        return character;
    }

    size_t write(const uint8_t * data, size_t len) override {
        int bytes_written = send(client_fd, data, len, 0);
        return bytes_written;
    }

    void flush() override {
        // Typically flush might involve sending any buffered data.
        // In simple TCP cases, it might be a no-op.
    }

    int peek() override { return 0; }

    size_t write(uint8_t) override {return 0; }
    int availableForWrite() override {return 0; }

    ~TCPSocketStream() {
        if (client_fd >= 0) {
            close(client_fd); // Close client socket
        }
        if (socket_fd >= 0) {
            close(socket_fd); // Close listening socket
        }
    }
};
class TCPServer : public StreamAPI {
private:
    int serverSocket;
    struct sockaddr_in serverAddr;
    std::thread read_thread;
    std::atomic<bool> enabled;
    int socket;

public:
    TCPServer(Stream* stream, int socket) : StreamAPI(stream), socket(socket) {}

    
    ~TCPServer() {
        enabled = false;
        if (read_thread.joinable()) {
            read_thread.join();
        }
        // close(serverSocket); // Close the server socket
    }
    void init(){
        // stream->init();
        enabled = true;
        read_thread = std::thread(&TCPServer::run, this);
        
    }

    
    virtual void onConnectionChanged(bool connected) override {}

   

    bool checkIsConnected() override {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        // Try to get peer name; if it fails, the socket is not connected
        return (getpeername(socket, (struct sockaddr*)&addr, &len) == 0);
    }

    void run() {
       
        while (enabled) {
            // if (connected()) {
            StreamAPI::runOncePart();
            // } else {
                // LOG_INFO("Client dropped connection, suspend API service");
                // enabled = false; // we no longer need to run
            // }
        }
    }
};
static TCPServer *apiPort = nullptr;
inline void initApiServer(int port ) {
    LOG_INFO("API server listening on TCP port %d", port);
    if (apiPort == nullptr){
        TCPSocketStream *stream = new TCPSocketStream(port);
        
        apiPort = new TCPServer(stream, stream->socket_fd);
        apiPort->init();
    }
}

#endif