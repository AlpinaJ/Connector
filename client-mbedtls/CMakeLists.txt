cmake_minimum_required(VERSION 3.1)

set(project_name websocket-client-mbedtls) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_library(${project_name}-lib
        src/WSListener.cpp
        src/WSListener.hpp
)

## link libs

find_package(oatpp              0.19.4 REQUIRED)
find_package(oatpp-websocket    0.19.4 REQUIRED)
find_package(oatpp-mbedtls      0.19.4 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-websocket
        PUBLIC oatpp::oatpp-mbedtls
)

#################################################################
## link mbedtls

link_directories(
        /usr/local/lib/
)

#################################################################

## add executables

add_executable(${project_name}-exe
        src/App.cpp
        src/Trade.cpp
        src/stocks/impl/Binance.cpp
        src/stocks/impl/Gate.cpp
        src/stocks/impl/Hitbtc.cpp
)

target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/WSTest.cpp
        test/WSTest.hpp
)
target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        LINKER_LANGUAGE CXX
)
