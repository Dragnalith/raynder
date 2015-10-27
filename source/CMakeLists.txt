cmake_minimum_required(VERSION 2.8)

project(Raynder)

set(RAYNEDER_SOURCES
    "main.cpp"
)

add_executable("Raynder" ${RAYNEDER_SOURCES})
target_link_libraries("Raynder" glm sfml-graphics sfml-window sfml-system)
