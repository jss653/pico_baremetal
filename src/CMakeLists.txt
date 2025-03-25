#CMakeLists.txt in src

set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory("timer")
add_subdirectory("server")
add_subdirectory("wifi")
add_subdirectory("sensor")
add_subdirectory("ntp")
add_subdirectory("pubsub")
add_subdirectory("errorhandler")

set(source "${source}" PARENT_SCOPE)

