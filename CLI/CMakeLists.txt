# yaml-cpp: FetchContent 方式引入，无需系统预装
find_package(yaml-cpp)

add_executable(ptd_cli
    main.cpp
    PCDReader.cpp
    LASWriter.cpp
)
target_include_directories(ptd_cli PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/ptd
)
target_link_libraries(ptd_cli PRIVATE ptd yaml-cpp)
set_target_properties(ptd_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
