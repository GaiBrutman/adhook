# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(adhook_utests
  test_hook.cpp
  test_symsize.cpp
)

target_link_libraries(adhook_utests
  adhook
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(adhook_utests)
