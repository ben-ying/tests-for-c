###############################
# BUILD TSD_SDS_ANDROID #
###############################

FILE( GLOB_RECURSE SOURCES
      public/*.[ch]pp
      src/*.[ch]pp
   )

INCLUDE_DIRECTORIES(${MODULE_PATH}/public)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

BUILD_ARTIFACT("lib-static" ${SOURCES})

MODULE_INCLUDE_DIRECTORIES(PUBLIC "${MODULE_PATH}/public/"
                           )

MODULE_DEPENDENCIES(PUBLIC "tsd.common"
                    PUBLIC "tsd.sds.utils")
#add_subdirectory(prompt)
