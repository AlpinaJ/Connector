#ifndef HITBTC_HPP
#define HITBTC_HPP

#include <stdio.h>
#include <iostream>
#include <stocks/Stock.h>
#include "oatpp-mbedtls/Config.hpp"
#include "oatpp-mbedtls/client/ConnectionProvider.hpp"
#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/WebSocket.hpp"
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

class Hitbtc : public Stock {
 public:
  Hitbtc(string pair_, int time_);
  void makeConnection(
      shared_ptr<oatpp::websocket::WebSocket> &websocket) override;
  vector<Trade> convert(Document &json) override;
};
#endif