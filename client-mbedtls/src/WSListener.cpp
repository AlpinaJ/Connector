#include "WSListener.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>

void WSListener::onPing(const WebSocket &socket, const oatpp::String &message)
{
  std::cout << "onPing" << std::endl;
  std::lock_guard<std::mutex> lock(m_writeMutex);
  socket.sendPong(message);
}

void WSListener::onPong(const WebSocket &socket, const oatpp::String &message)
{
  std::cout << "onPong" << std::endl;
}

void WSListener::onClose(const WebSocket &socket, v_word16 code, const oatpp::String &message)
{
  std::cout << "onClose" << std::endl;
}

void WSListener::readMessage(const WebSocket &socket, p_char8 data, oatpp::data::v_io_size size)
{
  if (size == 0)
  {
    auto wholeMessage = m_messageBuffer.toString();
    m_messageBuffer.clear();

    if (isohlc)
    {
      std::string str = wholeMessage->c_str();

      //parse string to find price
      auto price_it = str.find("p");
      str = str.substr(price_it + 4, 10);
      double price = std::stod(str);

      //parse string to find volume
      auto volume_it = str.find("q");
      str = str.substr(volume_it + 4, 10);
      double cur_volume = std::stod(str);

      //if ohlc begin
      if (first_time)
      {
        high_price = price;
        low_price = price;
        open_price = price;
        first_time = false;
      }

      high_price = std::max(price, high_price);
      low_price = std::min(price, low_price);
      volume += cur_volume;

      // Here we stop out interval and start from the beginning
      if (std::chrono::system_clock::now() - start_time >= std::chrono::seconds(duration * 60))
      {
        std::time_t t = std::chrono::system_clock::to_time_t(start_time);
        close_price = price;
        start_time = std::chrono::system_clock::now();
        first_time = true;
        stream << name_of_stock << " " << name_of_pair << " " << open_price << " " << high_price << " " << low_price << " " << close_price
               << " " << volume << " " << t << " " << duration << std::endl;
        volume = 0;
      }
    }
    else
    {
      stream << wholeMessage->c_str() << std::endl;
    }
  }
  else if (size > 0)
  {
    m_messageBuffer.write(data, size);
  }
}
