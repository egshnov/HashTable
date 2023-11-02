project(HashTable)

set(HEADER_FILES INTERFACE DLinkedList.hpp HashTable.hpp) #HashTable2.hpp

add_library(HashTable_lib ${HEADER_FILES})