set(PLUGIN_NAME festival)
set(INCLUDES ${CSOUND_INCLUDE_DIRS})
set(LIBS "")

# Dependencies
find_package(EST)
check_deps(EST)
list(APPEND INCLUDES ${EST_INCLUDE_DIR})
list(APPEND LIBS ${EST_LIBRARIES})

find_package(Festival)
check_deps(Festival)
list(APPEND INCLUDES ${Festival_INCLUDE_DIR})
list(APPEND LIBS ${Festival_LIBRARIES})

# Source files
set(CPPFILES src/opcodes.cpp)
make_plugin(${PLUGIN_NAME} "${CPPFILES}" ${LIBS})
target_include_directories(${PLUGIN_NAME} PRIVATE ${INCLUDES})
