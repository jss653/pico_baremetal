#CMakeLists.txt in src/ntp
set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(
  APPEND source
  "${dir}/ntp.c"
)

set(source "${source}" PARENT_SCOPE)
