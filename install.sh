#!/bin/bash

/opt/clion/bin/cmake/bin/cmake --build ./cmake-build-debug --target clean -- -j 2
/opt/clion/bin/cmake/bin/cmake --build ./cmake-build-debug --target grid_sort -- -j 2
echo "binary file is ./cmake-build-debug/grid_sort"
