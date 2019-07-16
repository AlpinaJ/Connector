#include "WSListener.hpp"
#include <string.h>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;
using TimeType = unsigned long long int;

struct trade {
  std::string stock;
  std::string pair;
  double price;
  double amount;
  TimeType time;
};

using TimeType = unsigned long long int;

//parse date to miliseconds
static TimeType date_to_ms(const string &datetime) {
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
  TimeType seconds;

  timer.tm_hour = hour;
  timer.tm_min = min;
  timer.tm_sec = sec;
  timer.tm_year = year - 1970;
  timer.tm_mon = month - 1;
  timer.tm_mday = day - 1;

  seconds = difftime(mktime(&timer), mktime(&zero));

  return seconds * 1000;
}

void printTrade(trade trade_, std::ofstream &stream) {
  auto time = trade_.stock == "gate" ? trade_.time * 1000 : trade_.time;

  stream << setw(12) << left << trade_.stock << setw(12) << left << trade_.pair
         << setw(12) << left << trade_.price << setw(12) << left
         << trade_.amount << setw(12) << left << time << endl;
}

std::vector<trade> convert(const std::string &stock_name,
                           const std::string &pair_name,
                           Document &json,
                           std::ofstream &stream) {
  std::vector<trade> trades;

  // Binance
  if (stock_name == "binance") {
    trade cur_trade;
    cur_trade.stock = stock_name;
    cur_trade.pair = json["s"].GetString();
    cur_trade.price = std::stod(json["p"].GetString());
    cur_trade.amount = std::stod(json["q"].GetString());
    cur_trade.time = json["T"].GetUint64();
    trades.push_back(cur_trade);
    return trades;
  }

  // Gate
  if (stock_name == "gate") {
    trade cur_trade;
    cur_trade.stock = stock_name;
    cur_trade.pair = json["params"][0].GetString();
    Value &json_ = json["params"][1];

    for (SizeType i = 0; i < json_.Size(); i++) {
      cur_trade.price = std::stod(json_[i]["price"].GetString());
      cur_trade.amount = std::stod(json_[i]["amount"].GetString());
      cur_trade.time = (unsigned long long int)(json_[i]["time"].GetDouble());
      trades.push_back(cur_trade);
    }
    return trades;
  }

  // HitBtc
  if (stock_name == "hitbtc") {
    trade cur_trade;
    cur_trade.stock = stock_name;
    cur_trade.pair = pair_name;
    Value &json_ = json["params"];
    Value &json__ = json_["data"];

    for (SizeType i = 0; i < json__.Size(); i++) {
      cur_trade.price = std::stod(json__[i]["price"].GetString());
      cur_trade.amount = std::stod(json__[i]["quantity"].GetString());
      std::string s = json__[i]["timestamp"].GetString();
      cur_trade.time = date_to_ms(s);
      trades.push_back(cur_trade);
    }
  }
  
  //***ADD CONVERTATIONS FOR OTHER STOCKS***
}
WSListener::WSListener(std::mutex &writeMutex,
                       std::ofstream &stream_,
                       const request &req)
    : m_writeMutex(writeMutex),
      stream(stream_),
      name_of_stock(req.stock_name),
      name_of_pair(req.pair_name),
      isohlc(req.ohlc_time > 0),
      duration(req.ohlc_time),
      start_time(std::chrono::system_clock::now()) {}

void WSListener::onPing(const WebSocket &socket, const oatpp::String &message) {
  std::lock_guard<std::mutex> lock(m_writeMutex);
  socket.sendPong(message);
}

void WSListener::onPong(const WebSocket &socket, const oatpp::String &message) {
}

void WSListener::onClose(const WebSocket &socket,
                         v_word16 code,
                         const oatpp::String &message) {}

void WSListener::readMessage(const WebSocket &socket,
                             p_char8 data,
                             oatpp::data::v_io_size size) {
  if (size == 0) {
    auto wholeMessage = m_messageBuffer.toString();
    m_messageBuffer.clear();
    const char *str = wholeMessage->c_str();
    Document document;
    document.Parse(str);

    if ((document.HasMember("error") == false && name_of_stock == "gate")
        || (name_of_stock == "hitbtc" && document.HasMember("result") == false)
        || (name_of_stock == "binance")) {
      std::vector<trade> trades =
          convert(name_of_stock, name_of_pair, document, stream);

      if (isohlc) {
        double cur_volume = 0;
        for (auto t : trades) {
          cur_volume += t.amount;
        }

        if (first_time) {
          high_price = trades[0].price;
          low_price = trades[0].price;
          open_price = trades[0].price;
          first_time = false;
        }

        for (auto t : trades) {
          high_price = std::max(high_price, t.price);
          low_price = std::min(low_price, t.price);
        }
        volume += cur_volume;

        if (std::chrono::system_clock::now() - start_time
            >= std::chrono::seconds(duration * 60)) {
          for (auto t : trades) close_price = t.price;
          start_time = std::chrono::system_clock::now();
          first_time = true;
          auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            start_time.time_since_epoch())
                            .count();
          stream << setw(12) << left << name_of_stock << setw(12) << left
                 << name_of_pair << setw(12) << left << open_price << setw(12)
                 << left << high_price << setw(12) << left << low_price
                 << setw(12) << left << close_price << setw(12) << left
                 << volume << setw(12) << left << now_ms<<std::endl;
          volume = 0;
        }
      } else {
        for (auto t : trades) {
          printTrade(t, stream);
        }
      }
    }
  } else if (size > 0) {
    m_messageBuffer.write(data, size);
  }
}
