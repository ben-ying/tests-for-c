CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(${MODULE_NAME})

FILE( GLOB_RECURSE SOURCES
      public/*.[ch]pp
      src/*.[ch]pp
   )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

BUILD_ARTIFACT(lib-static ${SOURCES})
MODULE_INCLUDE_DIRECTORIES(PUBLIC "${MODULE_PATH}/public/"
                           PRIVATE "${MODULE_PATH}/src/")
MODULE_DEPENDENCIES(PUBLIC "tsd.common"
                    PUBLIC "tsd.sds.utils")

