include_directories(${LevelZero_INCLUDE_DIRS})
file(GLOB SOURCES *.cpp)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})