set (SOURCES
	core/src/log.cpp
	image/src/stb.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
	core
	image
	PRIVATE
	core/include
	image/include
)
