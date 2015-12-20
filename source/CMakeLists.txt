cmake_minimum_required(VERSION 2.8)

project(Raynder)

set(RAYNEDER_SOURCES
    "main.cpp"
    "Camera.hpp"
    "Camera.cpp"
    "CoordinateSystem.hpp"
    "CoordinateSystem.cpp"
    "Intersection.hpp"
    "Intersection.cpp"
    "Material.hpp"
    "Material.cpp"
    "Ray.cpp"
    "Ray.hpp"
    "ObjectGraph.cpp"
    "ObjectGraph.hpp"
    "Sphere.hpp"
    "Sphere.cpp"
    "Object.hpp"
    "Object.cpp"
    "Helper.hpp"
    "Mesh.hpp"
    "Mesh.cpp"
)

add_executable("Raynder" ${RAYNEDER_SOURCES})
target_link_libraries("Raynder" sfml-graphics sfml-window sfml-system)

if(${MSVC})
    add_custom_command(TARGET Raynder
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:Raynder>
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:Raynder>
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-system> $<TARGET_FILE_DIR:Raynder>
    )
endif()
