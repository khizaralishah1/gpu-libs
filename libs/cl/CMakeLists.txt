file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(cl_lib ${SOURCES} ${HEADERS})
target_link_libraries(cl_lib PUBLIC kern_lib ${OpenCL_LIB})
target_include_directories(cl_lib PUBLIC include)