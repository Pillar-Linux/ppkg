#cmake min version
cmake_minimum_required(VERSION 3.10)

#project
project(ppkg C)

#set C89/90
set(CMAKE_C_STANDARD 90)

#CFLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fpic")

#output binary to /bin
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#check libraries
#libtoml
find_library(LIBTOML libtoml.a REQUIRED)

#check programs
#git
find_package(Git REQUIRED)
#svn
find_package(Subversion REQUIRED)
#curl
find_package(CURL REQUIRED)
#tar
find_package(UnixCommands REQUIRED)

# include /src /include
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

# include /include/*.h /src/*.c
file(GLOB all_SRCS
	"${PROJECT_SOURCE_DIR}/include/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.c"
)

#ppkg
add_executable(ppkg ${all_SRCS})

#link libraries
target_link_libraries(ppkg ${LIBTOML})
