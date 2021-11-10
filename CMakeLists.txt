cmake_minimum_required(VERSION 3.20)
project(COMP_345_PROJECT_TEAM_N12)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES src/main_driver.cpp src/cards/cards.cpp src/game_engine/game_engine.cpp src/map/map.cpp src/orders/orders.cpp src/player/player.cpp src/command_processor/command_processing.cpp)

set(HEADERS src/cards/cards.h src/game_engine/game_engine.h src/map/map.h src/orders/orders.h src/player/player.h src/command_processor/command_processing.h src/game_log/log_observer.cpp src/game_log/log_observer.h src/observer/logging_observer.cpp src/observer/logging_observer.h)

add_executable(WARZONE_APPLICATION
        ${SOURCES}
        ${HEADERS})

target_include_directories(WARZONE_APPLICATION PRIVATE src)
