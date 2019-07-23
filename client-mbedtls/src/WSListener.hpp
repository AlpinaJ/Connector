#ifndef WSLISTENER_HPP
#define WSLISTENER_HPP

#include <fstream>
#include "stocks/Stock.h"
#include "stocks/Binance.h"
#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/WebSocket.hpp"

struct TradeData {
  double open_price_;
  double close_price_;
  double high_price_;
  double low_price_;
  std::chrono::time_point<std::chrono::system_clock> start_time_;
  TradeData() {}
  TradeData(double open_price,
            double close_price,
            double high_price,
            double low_price,
            std::chrono::time_point<std::chrono::system_clock> start_time)
      : open_price_(open_price),
        close_price_(close_price),
        high_price_(high_price),
        low_price_(low_price),
        start_time_(start_time) {}
};

class WSListener : public oatpp::websocket::WebSocket::Listener {
 private:
  static constexpr const char *TAG = "Client_WSListener";

 private:
  std::ofstream &stream;
  std::mutex &m_writeMutex;
  std::string name_of_stock, name_of_pair;
  bool isohlc;
  std::chrono::time_point<std::chrono::system_clock> start_time;
  int duration;
  double open_price, close_price, high_price, low_price, volume = 0;
  bool first_time = true;
  std::shared_ptr<Stock> req;

  oatpp::data::stream::ChunkedBuffer m_messageBuffer;

 public:
  WSListener(std::mutex &writeMutex,
             std::ofstream &stream_,
             std::shared_ptr<Stock> &req);

  /**
   * Called on "ping" frame.
   */
  void onPing(const WebSocket &socket, const oatpp::String &message) override;

  /**
   * Called on "pong" frame
   */
  void onPong(const WebSocket &socket, const oatpp::String &message) override;

  /**
   * Called on "close" frame
   */
  void onClose(const WebSocket &socket,
               v_word16 code,
               const oatpp::String &message) override;

  /**
   * Called on each message frame. After the last message will be called
   * once-again with size == 0 to designate end of the message.
   */
  void readMessage(const WebSocket &socket,
                   p_char8 data,
                   oatpp::data::v_io_size size) override;

  // Setters

 public:
  void setTradeData(const TradeData &data) {
    open_price = data.open_price_;
    close_price = data.close_price_;
    high_price = data.high_price_;
    low_price = data.low_price_;
    start_time = data.start_time_;
  }

  TradeData getTradeData() const {
    return {open_price, close_price, high_price, low_price, start_time};
  }
};

#endif  // WSLISTENER_HPP
