
# Fetch lbspng
include(FetchContent)
FetchContent_Declare(
  libspng
  GIT_REPOSITORY https://github.com/randy408/libspng.git
  GIT_TAG        75c39ce0948d3b9c623a674ebfeb63a703a7c116
)
FetchContent_MakeAvailable(libspng)


