#include "Trade.h"

Trade::Trade(std::string stock_,
             std::string pair_,
             double price_,
             double amount_,
             unsigned long long int time_) {
  stock = stock_;
  pair = pair_;
  price = price_;
  amount = amount_;
  time = time_;
}