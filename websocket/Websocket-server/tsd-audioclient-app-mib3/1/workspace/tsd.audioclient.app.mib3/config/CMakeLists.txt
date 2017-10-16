CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

###############################
# BUILD TSD_SDS_ANDROID_APP_MIB3 #
###############################

FILE( GLOB_RECURSE SOURCES
      public/*.[ch]pp
      src/*.[ch]pp
   )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

BUILD_ARTIFACT(lib-static ${SOURCES})

MODULE_INCLUDE_DIRECTORIES(PRIVATE "${MODULE_PATH}/src/"
                           PUBLIC "${MODULE_PATH}/public/")

MODULE_DEPENDENCIES(PUBLIC "tsd.common"
                    PUBLIC "tsd.sds.android"
                    PUBLIC "tsd.sds.android.common"
                    PUBLIC "tsd.audioclient.app.audio"
                    PUBLIC "tsd.audioclient.app.common"
                    PUBLIC "tsd.sds.utils")





