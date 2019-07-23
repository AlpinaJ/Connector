# Connector
In order to build the project:
```mkdir build
   cd build
   cmake ..
   make```    

In order to run:
 ```./websocket-client-mbedtls-exe binance ethbtc 0 ethbtc 1 gate ETH_BTC 2 hitbtc ETHBTC 0 ETHBTC 1 (name of stock: pair time pair time ...)```

If you want to add new stock, add to `CmakeLists.txt` to `add_executable(${project_name}-exe` src/stocks/impl/YourStockName.cpp
