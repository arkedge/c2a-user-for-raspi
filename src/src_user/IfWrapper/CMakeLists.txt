cmake_minimum_required(VERSION 3.13)

project(C2A_USER_IF_WRAPPER)

set(C2A_SRCS
  if_list.c
)

if(NOT USE_C2A)
  list(APPEND C2A_SRCS
    RaspberryPi/ccsds_raspi.c
    RaspberryPi/uart_raspi.c
    RaspberryPi/wdt_raspi.c
  )
else()
  list(APPEND C2A_SRCS
    Sils/ccsds_sils.cpp
    Sils/uart_sils.cpp
    Sils/wdt_sils.cpp
  )
endif()

if(USE_SCI_COM_WINGS)
  add_definitions(-DUSE_SCI_COM_WINGS)
  #target_sources(${PROJECT_NAME} PUBLIC
  list(APPEND C2A_SRCS
    Sils/ccsds_sils_sci_if.cpp
  )
  message("USE SCI_COM_WINGS")
endif()

if(USE_SCI_COM_UART)
  add_definitions(-DUSE_SCI_COM_UART)
  #target_sources(${PROJECT_NAME} PUBLIC
  list(APPEND C2A_SRCS
    Sils/uart_sils_sci_if.cpp
  )
  message("USE SCI_COM_UART")
endif()

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
