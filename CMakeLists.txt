cmake_minimum_required(VERSION 2.8)

project("Raynder")

add_subdirectory("external/SFML")

include_directories(
    "external/SFML/include"
    "external/glm/"
)

add_subdirectory("source")
