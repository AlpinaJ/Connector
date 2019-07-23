#include <stocks/Hitbtc.h>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
  shared_ptr<oatpp::websocket::Connector> connector;
  inline static bool inited = false;
}

Hitbtc::Hitbtc(string pair_, int time_) {
  if (!inited) {
    auto config = oatpp::mbedtls::Config::createDefaultClientConfigShared();
    auto connection_provider =
        oatpp::mbedtls::client::ConnectionProvider::createShared(
            config, "api.hitbtc.com", 443);
    connector = oatpp::websocket::Connector::createShared(connection_provider);
    inited = true;
  }
  pair = pair_;
  time = time_;
  stock = "hitbtc";
}

void Hitbtc::makeConnection(shared_ptr<oatpp::websocket::WebSocket> &websocket) {
  string connection_name = "api/2/ws";
  auto connection = connector->connect(connection_name.c_str());
  websocket = oatpp::websocket::WebSocket::createShared(connection, true);
  oatpp::String text =
      "{\"method\": \"subscribeTrades\",\"params\": {\"symbol\": \""
      + oatpp::String(pair.c_str())
      + "\",\"limit\": 100},\"id\": 1562740083}";
  websocket->sendOneFrameText(text);
}


  vector<Trade> Hitbtc::convert(Document & json) {
    vector<Trade> trades;
    auto stock = "hitbtc";
    auto pairName = pair;
    Value &json_ = json["params"];
    Value &json__ = json_["data"];

    for (SizeType i = 0; i < json__.Size(); i++) {
      auto price = std::stod(json__[i]["price"].GetString());
      auto amount = std::stod(json__[i]["quantity"].GetString());
      std::string datetime = json__[i]["timestamp"].GetString();

      auto iter_T = datetime.find('T');
      string date = datetime.substr(0, iter_T);
      string time = datetime.substr(iter_T + 1, datetime.size() - iter_T - 2);

      int year, month, day;
      istringstream date_stream(date);
      date_stream >> year;
      date_stream.ignore(1);
      date_stream >> month;
      date_stream.ignore(1);
      date_stream >> day;

      int hour, min;
      double sec;
      istringstream time_stream(time);
      time_stream >> hour;
      time_stream.ignore(1);
      time_stream >> min;
      time_stream.ignore(1);
      time_stream >> sec;

      std::tm zero = {0}, timer = {0};
      unsigned long long int seconds;

      timer.tm_hour = hour;
      timer.tm_min = min;
      timer.tm_sec = sec;
      timer.tm_year = year - 1970;
      timer.tm_mon = month - 1;
      timer.tm_mday = day - 1;

      seconds = difftime(mktime(&timer), mktime(&zero));
      
      trades.push_back(Trade{stock, pairName, price, amount, seconds*1000});
    }
    return trades;
  }