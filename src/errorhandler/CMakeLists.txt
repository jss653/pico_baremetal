#CMakeLists.txt in src/errorhandler
set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(
  APPEND source
  "${dir}/errorhandler.c"
)

set(source "${source}" PARENT_SCOPE)
