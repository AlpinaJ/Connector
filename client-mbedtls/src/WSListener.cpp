#include "WSListener.hpp"
#include <string.h>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "Binance.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

void printTrade(Trade trade_, std::ofstream &stream) {
  // auto time = trade_.stock == "gate" ? trade_.time * 1000 : trade_.time;

  stream << setw(10) << left << trade_.stock << setw(10) << left << trade_.pair
         << setw(12) << left << trade_.price << setw(12) << left
         << trade_.amount << setw(13) << left << trade_.time << endl;
}

WSListener::WSListener(std::mutex &writeMutex,
                       std::ofstream &stream_,
                       shared_ptr<Stock> &req_)
    : m_writeMutex(writeMutex),
      stream(stream_),
      name_of_stock(req_->stock),
      name_of_pair(req_->pair),
      isohlc(req_->time > 0),
      duration(req_->time),
      start_time(std::chrono::system_clock::now()),
      req(req_) {}

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
      std::vector<Trade> trades = this->req->convert(document);

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
          stream << setw(10) << left << name_of_stock << setw(10) << left
                 << name_of_pair << setw(12) << left << open_price << setw(12)
                 << left << high_price << setw(12) << left << low_price
                 << setw(12) << left << close_price << setw(12) << left
                 << volume << setw(13) << left << now_ms << " ";
          stream << duration << std::endl;
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
