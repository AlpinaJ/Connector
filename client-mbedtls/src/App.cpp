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
#include "WSListener.hpp"
#include "oatpp-mbedtls/Config.hpp"
#include "oatpp-mbedtls/client/ConnectionProvider.hpp"
#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/WebSocket.hpp"

using namespace std;

namespace {

  struct BinanceConnector {
    shared_ptr<oatpp::websocket::Connector> connector_binance;
    explicit BinanceConnector(
        shared_ptr<oatpp::websocket::Connector> connector_binance_)
        : connector_binance(move(connector_binance_)) {}
  };

  struct HitbtcConnector {
    shared_ptr<oatpp::websocket::Connector> connector_hitbtc;
    explicit HitbtcConnector(
        shared_ptr<oatpp::websocket::Connector> connector_hitbtc_)
        : connector_hitbtc(move(connector_hitbtc_)) {}
  };

  struct GateConnector {
    shared_ptr<oatpp::websocket::Connector> connector_gate;
    explicit GateConnector(
        shared_ptr<oatpp::websocket::Connector> connector_gate_)
        : connector_gate(move(connector_gate_)) {}
  };

  //***ADD HERE MORE CONNECTORS***

  struct Connectors {
    shared_ptr<oatpp::websocket::Connector> connector_binance;
    shared_ptr<oatpp::websocket::Connector> connector_hitbtc;
    shared_ptr<oatpp::websocket::Connector> connector_gate;
    //***ADD HERE MORE CONNECTORS***

    explicit Connectors(BinanceConnector binance,
                        HitbtcConnector hitbtc,
                        GateConnector gate)
        : connector_binance(move(binance.connector_binance)),
          connector_hitbtc(move(hitbtc.connector_hitbtc)),
          connector_gate(move(gate.connector_gate)) {}
  };

  using SetUpFunction =
      std::function<void(shared_ptr<oatpp::websocket::WebSocket> &websocket,
                         string &pair_name,
                         Connectors &connectors)>;

  Connectors createConnectors() {
    auto config = oatpp::mbedtls::Config::createDefaultClientConfigShared();

    // Binance
    auto connection_provider_binance =
        oatpp::mbedtls::client::ConnectionProvider::createShared(
            config, "stream.binance.com", 9443);
    auto connector_binance =
        oatpp::websocket::Connector::createShared(connection_provider_binance);

    // Hitbtc
    auto connection_provider_hitbtc =
        oatpp::mbedtls::client::ConnectionProvider::createShared(
            config, "api.hitbtc.com", 443);
    auto connector_hitbtc =
        oatpp::websocket::Connector::createShared(connection_provider_hitbtc);

    // Gate
    auto connection_provider_gate =
        oatpp::mbedtls::client::ConnectionProvider::createShared(
            config, "ws.gate.io", 443);
    auto connector_gate =
        oatpp::websocket::Connector::createShared(connection_provider_gate);

    //***ADD HERE MORE CONNECTORS***

    return Connectors(BinanceConnector(connector_binance),
                      HitbtcConnector(connector_hitbtc),
                      GateConnector(connector_gate));
  };

  void binanceCase(shared_ptr<oatpp::websocket::WebSocket> &websocket,
                   string &pair_name,
                   Connectors &connectors) {
    string connection_name = "ws/" + pair_name + "@trade";
    auto connection =
        connectors.connector_binance->connect(connection_name.c_str());
    websocket = oatpp::websocket::WebSocket::createShared(connection, true);
  }

  void gateCase(shared_ptr<oatpp::websocket::WebSocket> &websocket,
                string &pair_name,
                Connectors &connectors) {
    string connection_name = "/v3/";
    auto connection =
        connectors.connector_gate->connect(connection_name.c_str());
    websocket = oatpp::websocket::WebSocket::createShared(connection, true);
    // add here your id
    oatpp::String text =
        "{\"id\":2735285,\"method\":\"trades.subscribe\",\"params\":[\""
        + oatpp::String(pair_name.c_str()) + "\"]}";
    websocket->sendOneFrameText(text);
  }

  void hitbtcCase(shared_ptr<oatpp::websocket::WebSocket> &websocket,
                  string &pair_name,
                  Connectors &connectors) {
    string connection_name = "api/2/ws";
    auto connection =
        connectors.connector_hitbtc->connect(connection_name.c_str());
    websocket = oatpp::websocket::WebSocket::createShared(connection, true);
    // add here your id
    oatpp::String text =
        "{\"method\": \"subscribeTrades\",\"params\": {\"symbol\": \""
        + oatpp::String(pair_name.c_str())
        + "\",\"limit\": 100},\"id\": 1562740083}";
    websocket->sendOneFrameText(text);
  }

  //***ADD HERE MORE CONNECTORS***

} 

void socketTask(shared_ptr<oatpp::websocket::WebSocket> &websocket,
                mutex &mutex,
                ofstream &file,
                request &req,
                SetUpFunction set_up,
                Connectors &connectors) {
  bool first = true;
  TradeData prev_data;

  //reconnection, if connection failed
  while (true) {
    set_up(websocket, req.pair_name, connectors);
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

void run(vector<request> &v) {
  Connectors connectors = createConnectors();

  vector<shared_ptr<oatpp::websocket::WebSocket>> socket(v.size());
  vector<mutex> socketWriteMutex(v.size());
  vector<ofstream> files(v.size());
  vector<thread> threads;

  for (int i = 0; i < v.size(); i++) {
    string file_name = "outputs/" + v.at(i).stock_name + v.at(i).pair_name
        + to_string(v.at(i).ohlc_time);
    files[i].open(file_name);

    SetUpFunction set_up;
    if (v.at(i).stock_name == "binance") {
      set_up = binanceCase;
    } else if (v.at(i).stock_name == "gate") {
      set_up = gateCase;
    } else if (v.at(i).stock_name == "hitbtc") {
      set_up = hitbtcCase;
    } else {
      cerr << "Undefined stock_name = " << v.at(i).stock_name << std::endl;
      continue;
    }

    threads.emplace_back(socketTask,
                         ref(socket[i]),
                         ref(socketWriteMutex[i]),
                         ref(files[i]),
                         ref(v[i]),
                         set_up,
                         ref(connectors));
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

int main() {
  vector<request> v{{"gate", "ETH_BTC", 0},{"binance", "ethbtc", 0},{"hitbtc", "ETHBTC", 0}};

  oatpp::base::Environment::init();
  run(v);
  oatpp::base::Environment::destroy();
  return 0;
}
