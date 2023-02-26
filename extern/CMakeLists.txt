
# Fetch lbspng
include(FetchContent)
FetchContent_Declare(
  libspng
  GIT_REPOSITORY https://github.com/randy408/libspng.git
  GIT_TAG        75c39ce0948d3b9c623a674ebfeb63a703a7c116
)
FetchContent_MakeAvailable(libspng)

# Fetch lbconfig
include(FetchContent)
FetchContent_Declare(
  libconfig
  GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
  GIT_TAG        525922a6b49ca19235cdf307376e7c9c539d1b93
)
FetchContent_MakeAvailable(libconfig)


