include(FetchContent)

set(CMAKE_SYSTEM_PREFIX_PATH "/usr/lib/arm-linux-gnueabihf")
set(CMAKE_FIND_NO_INSTALL_PREFIX TRUE)

FetchContent_Declare(
        simpledbus
        GIT_REPOSITORY https://github.com/OpenBluetoothToolbox/SimpleBLE.git
        GIT_TAG  v0.7.3
        GIT_SHALLOW YES
        SOURCE_SUBDIR simpledbus
)
FetchContent_MakeAvailable(simpledbus)
