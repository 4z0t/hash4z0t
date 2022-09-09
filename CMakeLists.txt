
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(h4z0t)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set(WIN64 true)
    # 64 bits
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
endif()

add_library(encoder OBJECT 
    src/Encoder.h
    src/Encoder.cpp
)

add_library(decoder OBJECT 
   src/Decoder.h
    src/Decoder.cpp
)


add_executable(${CMAKE_PROJECT_NAME}

    src/main.cpp
    src/h4z0t.h
      
)

set_target_properties(${CMAKE_PROJECT_NAME} encoder decoder PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

target_link_libraries(${CMAKE_PROJECT_NAME}
    encoder
    decoder
    )
#set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)
