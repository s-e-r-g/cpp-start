cmake_minimum_required(VERSION 3.21)
project(CppPerfTemplate LANGUAGES CXX)

# Options
option(BUILD_TESTING "Build unit tests" ON)
option(BUILD_BENCHMARKS "Build Google Benchmark targets" ON)
option(ENABLE_LINUX_PERF "Enable Linux perf_event counters in app" ON)

# C++ standard & policies
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build types
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Warnings & Sanitizers
if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wnon-virtual-dtor)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  if(NOT MSVC)
    add_link_options(-fsanitize=address,undefined)
    add_compile_options(-fsanitize=address,undefined)
  endif()
endif()

# ---- Library: mylib ----
add_library(mylib
  src/alg.cpp
)

target_include_directories(mylib PUBLIC include)

# Prefer speed in Release
if(NOT MSVC)
  target_compile_options(mylib PRIVATE $<$<CONFIG:Release>:-O3 -DNDEBUG>)
endif()

# ---- App: binary that can use perf counters ----
add_executable(app
  src/main.cpp
  src/perf_counters.hpp
)

# OS-specific perf sources
if(ENABLE_LINUX_PERF AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_sources(app PRIVATE src/perf_linux.cpp)
  target_compile_definitions(app PRIVATE HAVE_LINUX_PERF=1)
else()
  target_sources(app PRIVATE src/perf_dummy.cpp)
endif()

target_link_libraries(app PRIVATE mylib)

# ---- Fetch GoogleTest ----
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
# For MSVC: avoid overriding parent settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---- Tests ----
if(BUILD_TESTING)
  enable_testing()
  add_executable(alg_tests tests/alg_tests.cpp)
  target_link_libraries(alg_tests PRIVATE mylib GTest::gtest_main)
  include(GoogleTest)
  gtest_discover_tests(alg_tests)
endif()

# ---- Fetch Google Benchmark ----
if(BUILD_BENCHMARKS)
  FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.4
  )
  # Benchmark needs GoogleTest for its own tests off and pthreads
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googlebenchmark)

  add_executable(alg_bench benchmarks/alg_bench.cpp)
  target_link_libraries(alg_bench PRIVATE mylib benchmark::benchmark benchmark::benchmark_main)
  if(NOT MSVC)
    target_link_libraries(alg_bench PRIVATE pthread)
  endif()
endif()

# Install targets (optional)
install(TARGETS mylib app)
