#CMakeLists.txt in src/pubsub
set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(
  APPEND source
  "${dir}/pubsub.c"
)

set(source "${source}" PARENT_SCOPE)
