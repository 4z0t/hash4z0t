
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
  "src/Compression/shannon_fano.h" "src/Compression/compression.h" "src/Compression/lz77.h" "src/BitsAndBytes/BitsAndBytes.h")

add_library(decoder OBJECT 
   src/Decoder.h
    src/Decoder.cpp
    "src/Compression/shannon_fano.h"
    "src/Compression/compression.h"
    "src/Compression/lz77.h" "src/BitsAndBytes/BitsAndBytes.h")



    
add_library(internal4z0t OBJECT 
   src/internalhash4z0t.cpp
    src/internalhash4z0t.h
    "src/BitsAndBytes/BitsAndBytes.h"

 )


set( CMAKE_PROJECT_NAME_TEST_SF "${CMAKE_PROJECT_NAME}_TestSF")
set( CMAKE_PROJECT_NAME_TEST_LZ "${CMAKE_PROJECT_NAME}_TestLZ")
set( CMAKE_PROJECT_NAME_TEST_HAMMING "${CMAKE_PROJECT_NAME}_TestHAMMING")

add_executable(${CMAKE_PROJECT_NAME}

    src/main.cpp
    src/h4z0t.h
       "src/ArgumentsManager.h"
       "src/FileData.h"
"src/Compression/shannon_fano.h" 
"src/Compression/compression.h" 
"src/Compression/lz77.h"
 "src/BitsAndBytes/BitsAndBytes.h"
    )





add_executable(${CMAKE_PROJECT_NAME_TEST_SF}
    src/test_compression/sf.cpp
     "src/Compression/compression.h"
     "src/Compression/shannon_fano.h"
      "src/BitsAndBytes/BitsAndBytes.h")



add_executable(${CMAKE_PROJECT_NAME_TEST_LZ}
    src/test_compression/lz77.cpp
     "src/Compression/compression.h"
     "src/Compression/lz77.h"
      "src/BitsAndBytes/BitsAndBytes.h")



      add_executable(${CMAKE_PROJECT_NAME_TEST_HAMMING}
    src/test_noiseprotection/hamming.cpp
    src/NoiseProtection/hamming.h
      "src/BitsAndBytes/BitsAndBytes.h")


set_target_properties(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME_TEST_LZ} ${CMAKE_PROJECT_NAME_TEST_SF} ${CMAKE_PROJECT_NAME_TEST_HAMMING} encoder decoder internal4z0t PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

target_link_libraries(${CMAKE_PROJECT_NAME}
    encoder
    decoder
    internal4z0t
    )
