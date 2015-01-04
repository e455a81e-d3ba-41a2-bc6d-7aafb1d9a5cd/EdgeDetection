project(edgedetection)
cmake_minimum_required(VERSION 2.8)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-Wno-variadic-macros -Wuninitialized -Wcast-align -Wredundant-decls -Wno-unknown-pragmas -fdata-sections -ffunction-sections -pedantic -Wextra -Wold-style-cast -Wstrict-overflow -Woverloaded-virtual -Wsign-promo -Wabi -Wmissing-declarations -Wall -std=c++1y -O0") 
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/TestData/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/TestData)

find_package( OpenCV REQUIRED )
add_executable(edgedetection main.cpp ImageProcessing.cpp)
target_link_libraries(edgedetection ${OpenCV_LIBS})