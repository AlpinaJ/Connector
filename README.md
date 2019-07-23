# Connector
In order to build the project:
    <p>mkdir build</p>    
    <p>cd build</p>    
    <p>cmake ..</p>    
    <p>make</p>    

In order to run:
    ./websocket-client-mbedtls-exe binance ethbtc 0 ethbtc 1 gate ETH_BTC 2 hitbtc ETHBTC 0 ETHBTC 1 (name of stock: pair time pair time ...)

If you want to add new stock, add to CmakeLists.txt to add_executable(${project_name}-exe src/stocks/impl/YourStockName.cpp
