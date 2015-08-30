#!/bin/bash

cd build && cmake .. && make -j3 && cd .. && ./build/bin/pxljm
