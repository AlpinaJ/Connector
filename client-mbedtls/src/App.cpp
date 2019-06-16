
#include "WSListener.hpp"

#include "oatpp-websocket/WebSocket.hpp"
#include "oatpp-websocket/Connector.hpp"
#include "oatpp-mbedtls/client/ConnectionProvider.hpp"
#include "oatpp-mbedtls/Config.hpp"

#include <thread>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <chrono>
#include <ctime>

using namespace std;

const string stock = "binance";
namespace
{
const char *TAG = "websocket-client-mbedtls";
bool finished = false;
void socketTask(const std::shared_ptr<oatpp::websocket::WebSocket> &websocket)
{
    websocket->listen();
}
} // namespace


void run(const std::vector<std::pair<std::string, int>> &v)
{
    //make a connection to binance
    auto config = oatpp::mbedtls::Config::createDefaultClientConfigShared();
    auto connectionProvider = oatpp::mbedtls::client::ConnectionProvider::createShared(config, "stream.binance.com", 9443);
    auto connector = oatpp::websocket::Connector::createShared(connectionProvider);

    std::vector<std::mutex> socketWriteMutex(v.size());
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<oatpp::websocket::WebSocket>> socket(v.size());
    vector<ofstream> files(v.size());

    for (int i = 0; i < v.size(); i++)
    {
        string connection_name = v.at(i).first;          //string for connection
        string file_name = "outputs/" + connection_name; //string for name of the output file

        //open the output file
        if (v.at(i).second != 0)
            file_name = file_name + "_" + to_string(v.at(i).second);
        files[i].open(file_name);

        //make the connection
        connection_name = "ws/" + connection_name + "@trade";
        auto connection = connector->connect(connection_name.c_str());
        socket[i] = oatpp::websocket::WebSocket::createShared(connection, true);
        if (v.at(i).second != 0)
            socket[i]->setListener(std::make_shared<WSListener>(socketWriteMutex[0], files[i], stock, v.at(i).first, true, v.at(i).second)); //for ohlc format
        else
        {
            socket[i]->setListener(std::make_shared<WSListener>(socketWriteMutex[0], files[i], stock, v.at(i).first, false, 0)); //for trades
        }
        threads.emplace_back(socketTask, socket[i]);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }
}

int main()
{
    // vector keeps a pair.1-name of pair, 2-0, if it is trade,otherwise time for ohcl
    std::vector<std::pair<std::string, int>> v{{"ethbtc", 2}, {"bnbbtc", 1}, {"bnbbtc", 0}};
    oatpp::base::Environment::init();
    run(v);
    oatpp::base::Environment::destroy();
    return 0;
}
