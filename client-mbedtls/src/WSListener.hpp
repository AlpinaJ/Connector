#ifndef WSListener_hpp
#define WSListener_hpp
#include <fstream>
#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/WebSocket.hpp"

struct request {
  std::string stock_name;
  std::string pair_name;
  int ohlc_time;
};
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

/**
 * WebSocket listener listens on incoming WebSocket events.
 */
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

  /**
   * Buffer for messages. Needed for multi-frame messages.
   */
  oatpp::data::stream::ChunkedBuffer m_messageBuffer;

 public:
  WSListener(std::mutex &writeMutex,
             std::ofstream &stream_,
             const request &req);

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

  // Do we need these setter? If not they should be deleted.
  // void setOpenPrice(double price) {
  //   open_price = price;
  // }
  // void setClosePrice(double price) {
  //   close_price = price;
  // }
  // void setHighPrice(double price) {
  //   high_price = price;
  // }
  // void setLowPrice(double price) {
  //   low_price = price;
  // }
  // void setVolume(double volume) {
  //   open_price = new_open_price;
  // }
  // void setStartTime(std::chrono::time_point<std::chrono::system_clock> time)
  // {
  //   start_time = time;
  // }
};

#endif  // WSListener_hpp
