set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_AR                        ${ARM_TOOLCHAIN}-ar)
set(CMAKE_ASM_COMPILER              ${ARM_TOOLCHAIN}-gcc)
set(CMAKE_C_COMPILER                ${ARM_TOOLCHAIN}-gcc)
set(CMAKE_CXX_COMPILER              ${ARM_TOOLCHAIN}-g++)
set(CMAKE_LINKER                    ${ARM_TOOLCHAIN}-ld)
set(CMAKE_OBJCOPY                   ${ARM_TOOLCHAIN}-objcopy CACHE INTERNAL "")
set(CMAKE_RANLIB                    ${ARM_TOOLCHAIN}-ranlib CACHE INTERNAL "")
set(CMAKE_SIZE                      ${ARM_TOOLCHAIN}-size CACHE INTERNAL "")
set(CMAKE_STRIP                     ${ARM_TOOLCHAIN}-strip CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM     NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY     ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE     ONLY)

# Optionally reduce compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE         STATIC_LIBRARY)
