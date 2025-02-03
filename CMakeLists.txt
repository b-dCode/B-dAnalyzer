cmake_minimum_required(VERSION 3.28)
project(BadAnalyzer)

set(CMAKE_CXX_STANDARD 23)

add_executable(BadAnalyzer main.cpp
        scanning/Scanner.cpp
        scanning/Scanner.h
        parsing/Parser.cpp
        parsing/Parser.h
        scanning/token_types.h
        parsing/AST.cpp
        parsing/AST.h
        scanning/Token.cpp
        scanning/Token.h)
