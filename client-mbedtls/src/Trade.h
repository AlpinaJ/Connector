#ifndef TRADE_HPP
#define TRADE_HPP
#include <iostream>

class Trade {
 public:
  std::string stock;
  std::string pair;
  double price;
  double amount;
  unsigned long long int time;

  Trade(std::string stock_,
        std::string pair_,
        double price_,
        double amount_,
        unsigned long long int time_);
};
#endif