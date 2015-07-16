#!/bin/bash

g++ -I ../msgpack-c/include markov.cpp gen.cpp -lz -std=c++0x -o gen -g
g++ -I ../msgpack-c/include markov.cpp seed.cpp -lz -std=c++0x -o seed -g
