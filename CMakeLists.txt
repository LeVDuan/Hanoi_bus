add_executable(bus_main bus_main.c)

add_subdirectory(libfdr)
add_subdirectory(cgraph)

target_link_libraries(bus_main cgraph fdr)