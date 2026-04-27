
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
#include <fcntl.h>
#include <errno.h>

class TCPSocketStream : public Stream {
private:
  
    

    struct sockaddr_in server_addr;
    uint16_t port;

public:
    int client_fd;
    int socket_fd;
    int res;
    TCPSocketStream(uint16_t port) : port(port), socket_fd(-1), client_fd(-1) {
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any interface

        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            throw std::runtime_error("Socket creation failed");
        }
        int flags = fcntl(socket_fd, F_GETFL, 0);
        fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
        int opt = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        res =bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (res < 0) {
            close(socket_fd);
            throw std::runtime_error("Bind failed");
        }
        
        if (listen(socket_fd, 1) < 0) {
            close(socket_fd);
            throw std::runtime_error("Listen failed");
        }
        
        struct sockaddr_in  cli; 
        int len = sizeof(cli);
        

        // need run in the task 
        // if (listen(socket_fd, 5) == 0) {
        //     close(socket_fd);
        //     throw std::runtime_error("listen failed");
        // }
        // client_fd = accept(socket_fd, (sockaddr* ) &cli, (socklen_t *) &len );
        // if (client_fd < 0) {
        //     close(socket_fd);
        //     throw std::runtime_error("accept failed");
        // }
        
     
    }

    bool isConnected() {
        if (client_fd < 0) return false;
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        if (getpeername(client_fd, (struct sockaddr*)&addr, &len) < 0) {
            close(client_fd);
            client_fd = -1;
            return false;
        }
        return true;
    }

    int available() override {
        if (client_fd < 0) {
            struct sockaddr_in cli;
            socklen_t len = sizeof(cli);
            client_fd = accept(socket_fd, (struct sockaddr*)&cli, &len);
            if (client_fd >= 0) {
                int flags = fcntl(client_fd, F_GETFL, 0);
                fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
                LOG_INFO("TCP client connected");
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return 0;
            }
        }
        if (client_fd < 0) return 0;
        int count;
        if (ioctl(client_fd, FIONREAD, &count) < 0) {
            close(client_fd);
            client_fd = -1;
            return 0;
        }
        return count;
    }

    int read() override {
        if (client_fd < 0) return -1;
        uint8_t character;
        int r = recv(client_fd, &character, sizeof(character), 0);
        if (r <= 0) {
            if (r == 0) {
                LOG_INFO("TCP client disconnected");
            }
            close(client_fd);
            client_fd = -1;
            return -1;
        }
        return character;
    }

    size_t write(const uint8_t * data, size_t len) override {
        if (client_fd < 0) return 0;
        int bytes_written = send(client_fd, data, len, 0);
        if (bytes_written < 0) {
            close(client_fd);
            client_fd = -1;
            return 0;
        }
        return bytes_written;
    }

    void flush() override {
        // Typically flush might involve sending any buffered data.
        // In simple TCP cases, it might be a no-op.
    }

    int peek() override { return 0; }

    size_t write(uint8_t) override {return 0; }
    int availableForWrite() override {return 0; }

    void cleanup() {
        if (client_fd >= 0) {
            close(client_fd); 
            client_fd = -1; 
        }
        if (socket_fd >= 0) {
            close(socket_fd); 
            socket_fd = -1; 
        }
    }


};
class TCPServer : public StreamAPI {
private:
    TCPSocketStream* tcpStream;
    

public:
    std::thread read_thread;
    std::atomic<bool> enabled;
    TCPServer(TCPSocketStream* stream) : StreamAPI(stream), tcpStream(stream) {}

    
    ~TCPServer() {
        enabled = false;
        if (read_thread.joinable()) {
            read_thread.join();
        }
    }

    void init(){
        // stream->init();
        enabled = true;
        read_thread = std::thread(&TCPServer::run, this);
    }

    virtual void onConnectionChanged(bool connected) override {}

   
    bool checkIsConnected() override {
        return tcpStream->isConnected();
    }

    void run() {
       
        while (enabled) {
            StreamAPI::runOncePart();
        }
    }
};




extern TCPSocketStream *stream;
extern TCPServer *apiPort;
inline void initApiServer(int port ) {
    LOG_INFO("API server listening on TCP port %d", port);
    if (!stream) {
        stream = new TCPSocketStream(port);
        apiPort = new TCPServer(stream);
        apiPort->init();
    }
}

inline void deInitApiServer(){
    if (apiPort){
        delete apiPort;
        apiPort = nullptr;
    }
    if (stream ){
        stream->cleanup(); 
        delete stream;
        stream = nullptr;
    }
}

#endif
