# Connector

Задание для летней стажировке в университете.

## 📌 Описание
Программу подключается к разным биржам по API и собирает данные по заданным парам валют.

## ⛏ Стек
- C++
- cmake

## 💻 Установка зависимостей
Для сборки проекта:
- mkdir build  
- cd build
- cmake ..
- make

Для запуска проекта:
./websocket-client-mbedtls-exe binance ethbtc 0 ethbtc 1 gate ETH_BTC 2 hitbtc ETHBTC 0 ETHBTC 1 (<название биржы>: <код пары> <время> <код пары> <время> ...)

Для добавления новой биржы:
Добавить `src/stocks/impl/YourStockName.cpp` в `add_executable(${project_name}-exe` в `CmakeLists.txt` 

