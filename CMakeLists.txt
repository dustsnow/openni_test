cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()
rosbuild_make_distribution(0.1.3)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

include(FindPkgConfig)
pkg_check_modules(OPENNI libopenni)

include_directories(${OPENNI_INCLUDEDIR} /usr/include/nite)
#rosbuild_add_executable(openni_tracker src/openni_tracker.cpp)
#rosbuild_add_executable(openni_gesture src/openni_gesture.cpp)
rosbuild_add_executable(openni_broadcaster src/openni_broadcaster.cpp)
#target_link_libraries(openni_tracker ${OPENNI_LIBRARIES})
#target_link_libraries(openni_gesture ${OPENNI_LIBRARIES})
target_link_libraries(openni_broadcaster ${OPENNI_LIBRARIES})
target_link_libraries(openni_broadcaster XnVNite_1_5_2)
