cmake_minimum_required(VERSION 3.0.0)
project(MahjongGL VERSION 0.1.0)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(external/glfw)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

add_subdirectory(external/spdlog)

if(NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
endif()

add_subdirectory(external/glad)
add_subdirectory(external/glm)


include_directories(include/
                    external/glad/include/
                    external/glfw/include/
                    external/glm/
                    external/stb/
                    external/spdlog/include/)

file(GLOB EXTERNAL_SOURCES external/glad/src/glad.c)
file(GLOB PROJECT_HEADERS include/*.hpp)
file(GLOB PROJECT_SOURCES src/*.cpp include/)
file(GLOB PROJECT_SHADERS shaders/*.cs
                            shaders/*.fs
                            shaders/*.gs
                            shaders/*.vs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                            README.md
                            .gitignore
                            .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("External" FILES ${EXTERNAL_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\" )

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${EXTERNAL_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw
                               ${GLFW_LIBRARIES} glad ${GLAD_LIBRARIES} ${CMAKE_DL_LIBS} spdlog::spdlog)

set_target_properties(${PROJECT_NAME} PROPERTIES
             RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
                DEPENDS ${PROJECT_SHADERS} COMMENT "Copying shaders" VERBATIM)
