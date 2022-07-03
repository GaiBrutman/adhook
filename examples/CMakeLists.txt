add_executable(patch
  patch.c
)

target_link_libraries(patch
  adhook
)
