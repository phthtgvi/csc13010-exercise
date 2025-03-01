cmake_minimum_required(VERSION 3.30)
project(csc13010_exercise)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(nlohmann)

add_executable(csc13010_exercise
    nlohmann/json.hpp
    ConfigManager.hpp
    Logger.hpp
    main.cpp
    RecordIO.hpp
    StatusRulesManager.hpp
    Student.hpp)
