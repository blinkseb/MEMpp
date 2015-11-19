file(GLOB SOURCES *.c)

include_directories(.)

add_library(tommath STATIC ${SOURCES})
