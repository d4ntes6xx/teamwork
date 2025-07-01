#pragma once
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <queue>

namespace asio = boost::asio;
namespace websocket = boost::beast::websocket;
using tcp = asio::ip::tcp;

class Network {
    asio::io_context ioc;
    websocket::stream<tcp::socket> ws;
    std::thread thread;
    std::mutex mtx;
    std::queue<std::pair<int, int>> moves;
    bool running = true;

public:
    Network(bool isServer, std::string host = "localhost", int port = 9002) : ws(ioc) {
        if (isServer) {
            tcp::acceptor acceptor{ ioc, {tcp::v4(), port} };
            tcp::socket socket{ ioc };
            acceptor.accept(socket);
            ws = websocket::stream<tcp::socket>{ std::move(socket) };
            ws.accept();
        }
        else {
            tcp::resolver resolver{ ioc };
            auto results = resolver.resolve(host, std::to_string(port));
            asio::connect(ws.next_layer(), results.begin(), results.end());
            ws.handshake(host, "/");
        }

        thread = std::thread([this]() {
            while (running) {
                boost::beast::flat_buffer buffer;
                ws.read(buffer);
                std::string msg = boost::beast::buffers_to_string(buffer.data());
                int x, y;
                if (sscanf(msg.c_str(), "%d,%d", &x, &y) == 2) {
                    std::lock_guard<std::mutex> lock(mtx);
                    moves.emplace(x, y);
                }
            }
            });
    }

    void send(int x, int y) {
        std::string msg = std::to_string(x) + "," + std::to_string(y);
        ws.write(asio::buffer(msg));
    }

    bool receive(int& x, int& y) {
        std::lock_guard<std::mutex> lock(mtx);
        if (moves.empty()) return false;
        std::tie(x, y) = moves.front(); moves.pop();
        return true;
    }

    ~Network() {
        running = false;
        ws.close(websocket::close_code::normal);
        if (thread.joinable()) thread.join();
    }
};
