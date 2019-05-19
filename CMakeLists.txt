project(edgedetection)
cmake_minimum_required(VERSION 3.0)

option(USE_VCPKG "Using VCPKG" ON)

if(USE_VCPKG)
	set(TOOLCHAIN_FILE $ENV{VCPKG_TOOLCHAIN_FILE})
	if (TOOLCHAIN_FILE STREQUAL "")
		message(FATAL_ERROR "No VCPKG file definied")
		return()
	endif()

	message("Using VCPKG Toolchain file: '${TOOLCHAIN_FILE}'")
	set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_TOOLCHAIN_FILE}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/W3 /WX)
endif()


set(INCLUDE
	3rdparty
)
include_directories(${INCLUDE})

add_subdirectory(lib)
add_subdirectory(test)
