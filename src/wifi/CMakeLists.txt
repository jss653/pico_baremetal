#CMakeLists.txt in src/wifi
set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(
  APPEND source
  "${dir}/wifi.c"
)

set(source "${source}" PARENT_SCOPE)
