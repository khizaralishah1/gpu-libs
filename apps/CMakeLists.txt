add_executable(app Main.cpp)
target_link_libraries(app PRIVATE cl_lib)
target_include_directories(app PRIVATE include)