#############################################################################
## Set minimum required version of cmake, project name and compile options ##
#############################################################################
cmake_minimum_required(VERSION 2.8.3)
project(hal_tool)

## Compilation options
add_compile_options(-std=c++17 -Wall -Wextra -pedantic -Werror)

#############################################################################
## Find catkin packages and libraries for catkin and system dependencies   ##
#############################################################################
find_package(catkin REQUIRED COMPONENTS
  roscpp
  agribot_msgs
  agribot_common
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES hal_tool
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

#############################################################################
## Build                                                                   ##
#############################################################################

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${agribot_common_SOURCE_DIR}/include
)

link_directories("${PROJECT_SOURCE_DIR}")

add_executable(hal_tpl_status_node src/hal_tpl_status_node.cpp src/TplStatus/TplStatus.cpp)
target_link_libraries(hal_tpl_status_node ${agribot_common_LIBRARIES} ${catkin_LIBRARIES})
add_dependencies(hal_tpl_status_node ${catkin_EXPORTED_TARGETS})

#############
## Install ##
#############

install(TARGETS
  hal_tpl_status_node
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  PATTERN ".svn" EXCLUDE
)

#############
## Testing ##
#############

include_directories(
  test/include
)

## Add gtest based cpp test target and link libraries
catkin_add_gtest(${PROJECT_NAME}-test test/utest.cpp src/TplStatus/TplStatus.cpp)
target_link_libraries(${PROJECT_NAME}-test ${agribot_common_LIBRARIES} gmock gtest)
