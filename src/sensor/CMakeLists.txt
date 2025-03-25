#CMakeLists.txt in src/sensor
set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(
  APPEND source
  "${dir}/sensor.c"
)

set(source "${source}" PARENT_SCOPE)
