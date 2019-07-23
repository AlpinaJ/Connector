# Connector
In order to build the project:
`mkdir build
 cd build
 cmake ..
 make
`  
In order to run:
 `./websocket-client-mbedtls-exe binance ethbtc 0 ethbtc 1 gate ETH_BTC 2 hitbtc ETHBTC 0 ETHBTC 1 (name of stock: pair time pair time ...)`

If you want to add new stock, add src/stocks/impl/YourStockName.cpp to `add_executable(${project_name}-exe` in `CmakeLists.txt` 
