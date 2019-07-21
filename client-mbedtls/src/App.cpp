#include <stdio.h>
#include <string.h>
#include <time.h>
#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include "Binance.h"
#include "Gate.h"
#include "Hitbtc.h"
#include "Stock.h"
#include "WSListener.hpp"
#include "oatpp-mbedtls/Config.hpp"
#include "oatpp-mbedtls/client/ConnectionProvider.hpp"
#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/WebSocket.hpp"

using namespace std;

struct request {
  string stock;
  string pair;
  int t;
};

void socketTask(shared_ptr<oatpp::websocket::WebSocket> &websocket,
                mutex &mutex,
                ofstream &file,
                shared_ptr<Stock> &req) {
  bool first = true;
  TradeData prev_data;

  while (true) {
    req->makeConnection(websocket);
    auto listener_ptr = make_shared<WSListener>(mutex, file, req);
    websocket->setListener(listener_ptr);
    if (!first) {
      listener_ptr->setTradeData(prev_data);
    }

    websocket->listen();

    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();

    prev_data = listener_ptr->getTradeData();
    first = false;
  }
}

void run(vector<shared_ptr<Stock>> v) {
  vector<shared_ptr<oatpp::websocket::WebSocket>> socket(v.size());
  vector<mutex> socketWriteMutex(v.size());
  vector<ofstream> files(v.size());
  vector<thread> threads;

  for (int i = 0; i < v.size(); i++) {
    string file_name =
        "outputs/" + v.at(i)->stock + v.at(i)->pair + to_string(v.at(i)->time);
    files[i].open(file_name);

    threads.emplace_back(socketTask,
                         ref(socket[i]),
                         ref(socketWriteMutex[i]),
                         ref(files[i]),
                         ref(v[i]));
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

int main() {
  // Binance b("ethbtc",1);
  // cout<<b.stock<<" "<<b.pair<<" "<<b.time;
  vector<shared_ptr<Stock>> v;
  // //"ethbtc"
  v.push_back(make_shared<Hitbtc>("ETHBTC", 0));
  v.push_back(make_shared<Hitbtc>("ETHBTC", 1));
  v.push_back(make_shared<Gate>("ETH_BTC", 0));
  v.push_back(make_shared<Gate>("ETH_BTC", 1));
  v.push_back(make_shared<Binance>("ethbtc", 1));
  v.push_back(make_shared<Binance>("ethbtc", 0));
  oatpp::base::Environment::init();
  run(move(v));
  oatpp::base::Environment::destroy();
  return 0;
}
