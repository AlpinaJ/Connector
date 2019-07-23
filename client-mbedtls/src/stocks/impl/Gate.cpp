#include <stocks/Gate.h>

using namespace std;

namespace {
  shared_ptr<oatpp::websocket::Connector> connector;
  inline static bool inited = false;
} 

Gate::Gate(string pair_, int time_) {
  if (!inited) {
    auto config = oatpp::mbedtls::Config::createDefaultClientConfigShared();
    auto connection_provider =
        oatpp::mbedtls::client::ConnectionProvider::createShared(
            config, "ws.gate.io", 443);
    connector = oatpp::websocket::Connector::createShared(connection_provider);
    inited = true;
  }
  pair = pair_;
  time = time_;
  stock = "gate";
}

void Gate::makeConnection(shared_ptr<oatpp::websocket::WebSocket> &websocket) {
  string connection_name = "/v3/";
  auto connection = connector->connect(connection_name.c_str());
  websocket = oatpp::websocket::WebSocket::createShared(connection, true);
  oatpp::String text =
      "{\"id\":2735285,\"method\":\"trades.subscribe\",\"params\":[\""
      + oatpp::String(pair.c_str()) + "\"]}";
  websocket->sendOneFrameText(text);
}

vector<Trade> Gate::convert(Document &json) {
  vector<Trade> trades;
  auto stock = "gate";
  auto pair = json["params"][0].GetString();
  Value &json_ = json["params"][1];

  for (SizeType i = 0; i < json_.Size(); i++) {
    auto price = std::stod(json_[i]["price"].GetString());
    auto amount = std::stod(json_[i]["amount"].GetString());
    auto time = (unsigned long long int)(json_[i]["time"].GetDouble());
    trades.push_back(Trade{stock, pair, price, amount, time*1000});
  }
  return trades;
}