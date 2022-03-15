# issue-reproduce

1. m-diers provide the CMakeLists.txt as following: ( files from m-diers folder)

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
      
  always complain cannt find the include files...
  
  2. I modified the CMakeLists.txt as following:  (lastest try folder)
  
              cmake_minimum_required(VERSION 3.13)

              project( ScheduleTest )

              find_package(HPX REQUIRED)
              add_executable( ${PROJECT_NAME} )
              target_link_libraries( ${PROJECT_NAME} PUBLIC HPX::hpx HPX::wrap_main hpx_init)
              target_sources( ${PROJECT_NAME} PUBLIC ScheduleTest.cpp )
              #target_link_libraries( ${PROJECT_NAME} PUBLIC hpx_init )
              
     which complain:   /usr/bin/ld: cannot find -lhpx_init
       
