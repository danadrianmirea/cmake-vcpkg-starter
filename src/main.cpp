#include "client.hpp"

#include <iostream>
#include <thread>

Client bitmex_client("wss://ws.bitmex.com/realtime?subscribe=orderBookL2:XBTUSD");

void terminate(int sig) {
    bitmex_client.shutdown();
}

int main() {

    std::signal(SIGTERM, terminate);
    std::signal(SIGKILL, terminate);
    std::signal(SIGINT, terminate);
    std::signal(SIGABRT, terminate);

    std::thread t1([&]() { bitmex_client.run(); });
    t1.join();

    return 0;
}
