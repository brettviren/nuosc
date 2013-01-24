cmake_minimum_required (VERSION 2.6) 
project (nuosc) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


file(GLOB sources "source/*.cc")
file(GLOB mainsrc "main/*.cc")
add_library(nuosc++ SHARED ${sources})
target_link_libraries(nuosc++ blitz)

add_executable(nuosc ${mainsrc} ${sources})
target_link_libraries(nuosc blitz)

install(TARGETS nuosc nuosc++
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)