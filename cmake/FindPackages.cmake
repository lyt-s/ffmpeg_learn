# Function to find multiple libraries in the same folder.
function(find_libraries name)
  cmake_parse_arguments(ARG "" "" "HINTS" ${ARGN})
  set(LIB_PATHS "")
  foreach(LIB ${ARG_UNPARSED_ARGUMENTS})
    find_library(LIB_PATH ${LIB} REQUIRED HINTS ${ARG_HINTS})
    list(APPEND LIB_PATHS ${LIB_PATH})
  endforeach()
  set(${name}
      ${LIB_PATHS}
      PARENT_SCOPE)
endfunction()

# Set directory of third party libraries. CMAKE_CURRENT_SOURCE_DIR is the
# project root directory, not the directory of this FindPackage.cmake.
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# First, we need find packages installed system-widely. QtS
# find_package(glog REQUIRED)
# message(STATUS "Find glog in: ${glog_DIR}")
# message(STATUS "glog version: ${glog_VERSION}")

# Find packages installed in third_party directory. Note: for those packagas
# that cmake doesn't provide FindXXX.cmake, we better use "PATHS <package_path>"
# to find <Package>-config.cmake, otherwise we add CMAKE_PREFIX_PATH so that we
# can call FindXXX function.

# leveldb (call leveldb)
# list(APPEND CMAKE_PREFIX_PATH ${THIRD_PARTY_DIR}/leveldb)
# find_package(leveldb REQUIRED)
# message(STATUS "Find leveldb in: ${leveldb_DIR}")
# message(STATUS "leveldb version: ${leveldb_VERSION}")
# set(leveldb_INCLUDE_DIRS ${THIRD_PARTY_DIR}/leveldb/include)



# ffmpeg 不支持find_package

# find_package(PkgConfig REQUIRED)

# message(STATUS "Find PkgConfig in ${PkgConfig}")
# pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
#     libavcodec
#     libavdevice
#     libavfilter
#     libavformat
#     libavutil
#     libpostproc
#     libswresample
#     libswscale
# )

# message(" ${THIRD_PARTY_DIR}" )

set(ffmpeg_INCLUDE_DIRS ${THIRD_PARTY_DIR}/ffmepg-4.4/include)

# find_library(ffmpeg_LIBS 
#                 avcodec
#                 avdevice
#                 avfilter
#                 avformat
#                 avutil
#                 postproc
#                 swresample
#                 swscale
#                 HINTS
#                 ${THIRD_PARTY_DIR}/ffmepg-4.4-ubuntu/lib
# )
# message(STATUS "Find ffmpeg_LIBS in ${ffmpeg_LIBS}")
