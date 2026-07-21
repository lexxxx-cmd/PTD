find_package(yaml-cpp REQUIRED)
find_package(PCL CONFIG REQUIRED COMPONENTS common io)

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
target_link_libraries(ptd_cli PRIVATE ptd yaml-cpp pcl_common pcl_io)
if(MSVC)
    target_compile_options(ptd_cli PRIVATE /wd4068 /wd4267 /wd4244 /wd4018)
endif()
set_target_properties(ptd_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
