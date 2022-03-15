project( ScheduleTest )
add_executable( ${PROJECT_NAME} )
target_link_libraries( ${PROJECT_NAME} PUBLIC pthread
                                              hpx_component_storage
                                              hpx_core
                                              hpx_iostreams
                                              hpx_partitioned_vector
                                              hpx_parallelism
                                              hpx_process
                                              hpx
                                              hpx_unordered )
target_sources( ${PROJECT_NAME} PUBLIC ScheduleTest.cpp )
target_link_libraries( ${PROJECT_NAME} PUBLIC hpx_init )
