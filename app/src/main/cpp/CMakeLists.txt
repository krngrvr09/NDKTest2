cmake_minimum_required(VERSION 3.4.1)

add_library(test_file SHARED
            test_file.c)

# Include libraries needed for test_file lib
target_link_libraries(test_file
                      android
                      log)