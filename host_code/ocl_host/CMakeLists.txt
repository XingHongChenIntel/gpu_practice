include_directories(${OpenCL_INCLUDE_DIRS})
file(GLOB SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})