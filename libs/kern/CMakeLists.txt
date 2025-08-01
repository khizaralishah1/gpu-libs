file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(kern_lib ${SOURCES} ${HEADERS})
target_link_libraries(kern_lib PUBLIC ${OpenCL_LIB})
target_include_directories(kern_lib PUBLIC include)

# Kernel Files (.cl)
file(GLOB KERNEL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cl")

foreach(KERNEL_FILE ${KERNEL_FILES})
    configure_file(${KERNEL_FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()