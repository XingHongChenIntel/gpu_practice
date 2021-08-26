include_directories(${LevelZero_INCLUDE_DIRS})
include_directories(utility)
file(GLOB SOURCES *.cpp)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})