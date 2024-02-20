set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Create list of cpp files
file(GLOB CODE_FILES "*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${CODE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)