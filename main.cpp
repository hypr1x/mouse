#include <windows.h>
#include <iostream>
#include <thread>
#include "zmq.hpp"
int screenWidthHalf = GetSystemMetrics(SM_CXSCREEN) / 2;
int screenHeightHalf = GetSystemMetrics(SM_CYSCREEN) / 2;
zmq::context_t context(1);
zmq::socket_t web_socket(context, ZMQ_PUSH);


void SendAim(const std::string message) {
    zmq::message_t zmq_message(message.size());
    memcpy(zmq_message.data(), message.data(), message.size());
    web_socket.send(zmq_message, zmq::send_flags::dontwait);
}


int main() {
    web_socket.connect("tcp://192.168.50.160:12345");
    while (true) {
        SetCursorPos(screenWidthHalf, screenHeightHalf);
        auto start = std::chrono::high_resolution_clock::now();

        while (true) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            if (duration.count() >= 1500) {
                break;
            }
        }
        POINT pos;
        GetCursorPos(&pos);
        int deltaX = pos.x - screenWidthHalf;
        int deltaY = pos.y - screenHeightHalf;
        if (deltaX != 0 || deltaY != 0) {
            //std::cout << "Mouse moved: (" << deltaX << ", " << deltaY << ")" << std::endl;
            std::string xString = std::to_string(int(deltaX));
            std::string yString = std::to_string(int(deltaY));
            std::string aimAt = xString + "," + yString;
            SendAim(aimAt);
        }
        if (GetAsyncKeyState(0x01) != 0) {
            SendAim("click");
        }
    }
    return 0;
}