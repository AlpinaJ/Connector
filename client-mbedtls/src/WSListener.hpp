#ifndef WSListener_hpp
#define WSListener_hpp
#include <fstream>
#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/WebSocket.hpp"

/**
 * WebSocket listener listens on incoming WebSocket events.
 */
class WSListener : public oatpp::websocket::WebSocket::Listener
{
private:
  static constexpr const char *TAG = "Client_WSListener";

private:
  std::ofstream &stream;
  std::mutex &m_writeMutex;
  std::string name_of_stock,name_of_pair;
  bool isohlc;
  std::chrono::time_point<std::chrono::system_clock> start_time;
  int duration; 
  double open_price, close_price, high_price, low_price,volume=0;
  bool first_time = true;
  //std::string name_of_stock;
  /**
   * Buffer for messages. Needed for multi-frame messages.
   */
  oatpp::data::stream::ChunkedBuffer m_messageBuffer;

public:
  WSListener(std::mutex &writeMutex, std::ofstream &stream_, std::string name_of_stock_,std::string name_of_pair,bool isohlc_,int duration_)
      : m_writeMutex(writeMutex), stream(stream_), name_of_stock(name_of_stock_),name_of_pair(name_of_pair),
      isohlc(isohlc_),duration(duration_),start_time(std::chrono::system_clock::now())
  {
  }

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
  void onClose(const WebSocket &socket, v_word16 code, const oatpp::String &message) override;

  /**
   * Called on each message frame. After the last message will be called once-again with size == 0 to designate end of the message.
   */
  void readMessage(const WebSocket &socket, p_char8 data, oatpp::data::v_io_size size) override;
};

#endif // WSListener_hpp
