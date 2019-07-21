#ifndef STOCK_HPP
#define STOCK_HPP

#include <string.h>
#include <iostream>
#include <memory>
#include <vector>

#include "Trade.h"

#include "oatpp-mbedtls/Config.hpp"
#include "oatpp-mbedtls/client/ConnectionProvider.hpp"
#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/WebSocket.hpp"

#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

class Stock {
 public:
  virtual ~Stock() = default;

  int time;
  string pair;
  string stock;
  virtual void makeConnection(
      shared_ptr<oatpp::websocket::WebSocket> &websocket) = 0;

  virtual vector<Trade> convert(Document &json) = 0;
};
#endif