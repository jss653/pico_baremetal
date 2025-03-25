#CMakeLists.txt in src/server
set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(
  APPEND source
  "${dir}/server.c"
)

set(source "${source}" PARENT_SCOPE)
