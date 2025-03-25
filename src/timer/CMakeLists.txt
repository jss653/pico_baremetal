#CMakeLists.txt in src/timer
set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(
  APPEND source
  "${dir}/timer.c"
)

set(source "${source}" PARENT_SCOPE)
