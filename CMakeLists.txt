cmake_minimum_required(VERSION 3.14)
project(CaclulatorProject)

set(CMAKE_CXX_STANDARD 17)

# 1. Include the FetchContent Module
include(FetchContent)

# 2. Declare the library we want
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.1.1 # A stable version
)

# 3. Download and make the library available
FetchContent_MakeAvailable(fmt)

# 4. Your existing prject setup
add_executable(physics_sim
    src/main.cpp
    src/math_utils.cpp
    src/physics_engine.cpp
)
target_include_directories(physics_sim PRIVATE include)

# 5. Link the library to your executable
target_link_libraries(physics_sim PRIVATE fmt::fmt)
