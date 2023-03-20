cmake_minimum_required(VERSION 3.9)

project(test)

execute_process(
        COMMAND
        source env.sh
)

message("env:" $ENV{TESTENV})