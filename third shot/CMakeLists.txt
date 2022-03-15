cmake_minimum_required(VERSION 3.13)

project( ScheduleTest )

find_package(HPX REQUIRED)
add_executable( ${PROJECT_NAME} )
target_link_libraries( ${PROJECT_NAME} PUBLIC HPX::hpx HPX::wrap_main hpx_init)
target_sources( ${PROJECT_NAME} PUBLIC ScheduleTest.cpp )
#target_link_libraries( ${PROJECT_NAME} PUBLIC hpx_init )
