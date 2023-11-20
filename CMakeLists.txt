cmake_minimum_required(VERSION 3.23)
project(optimisation_d_une_ligne_d_assemblage_ing2_tg_2023_2024_9_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(optimisation_d_une_ligne_d_assemblage_ing2_tg_2023_2024_9_3
        exclusions.txt
        operations.txt
        precedence.txt
        Victor.c
        Victor.h)
