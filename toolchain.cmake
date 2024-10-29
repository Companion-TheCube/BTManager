# toolchain.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Cross-compilation setup
set(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)
set(CMAKE_LINKER /usr/bin/aarch64-linux-gnu-ld)

# Additional flags (adjust as needed)
set(CMAKE_CXX_FLAGS "-march=armv8-a")