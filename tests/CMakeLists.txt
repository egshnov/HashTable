project(test)
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES DLinkedListTests.cpp HashTableTests.cpp) #HashTable2Tests.cpp
set(HEADER_FILE)
add_executable(Google_Tests_run ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Google_Tests_run HashTable_lib)
target_link_libraries(Google_Tests_run gtest gtest_main)