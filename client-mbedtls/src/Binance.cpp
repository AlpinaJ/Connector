#include "Binance.h"

using namespace std;

namespace {
  shared_ptr<oatpp::websocket::Connector> connector;
  inline static bool inited = false;
} 

Binance::Binance(string pair_, int time_) {
  if (!inited) {
    auto config = oatpp::mbedtls::Config::createDefaultClientConfigShared();
    auto connection_provider =
        oatpp::mbedtls::client::ConnectionProvider::createShared(
            config, "stream.binance.com", 9443);
    connector =
        oatpp::websocket::Connector::createShared(connection_provider);
    inited = true;
  }
  pair = pair_;
  time = time_;
  stock = "binance";
}

void Binance::makeConnection(
    shared_ptr<oatpp::websocket::WebSocket> &websocket) {
  string connection_name = "ws/" + this->pair + "@trade";
  auto connection = connector->connect(connection_name.c_str());
  websocket = oatpp::websocket::WebSocket::createShared(connection, true);
}

vector<Trade> Binance::convert(Document &json) {
  vector<Trade> trades;
  auto stock = "binance";
  auto pair = json["s"].GetString();
  auto price = std::stod(json["p"].GetString());
  auto amount = std::stod(json["q"].GetString());
  auto time = json["T"].GetUint64();

  trades.push_back(Trade{stock, pair, price, amount, time});
  return trades;
}