# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appGroundStation_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appGroundStation_autogen.dir\\ParseCache.txt"
  "appGroundStation_autogen"
  )
endif()
