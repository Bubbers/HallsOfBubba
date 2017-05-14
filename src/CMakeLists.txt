
# Files to compile
set(GAME_SOURCE_FILES main.cpp controls.h ObjectIdentifiers.h)

# Subdirectories
add_subdirectory(cameras)
add_subdirectory(components)
add_subdirectory(ui)
add_subdirectory(particles)
add_subdirectory(map)


# Define the executable
set(EXECUTABLE_NAME HallsOfBubba)
add_executable(${EXECUTABLE_NAME} ${GAME_SOURCE_FILES} ${GAME_HEADER_FILES})

target_link_libraries(${EXECUTABLE_NAME} LINK_PRIVATE Bubba3D)
