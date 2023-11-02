cmake_minimum_required(VERSION 3.26) #минимальная версия cmake
project(HashTable) #название проекта

set(CMAKE_CXX_STANDARD 17) #переменной CMAKE_CXX_STANDART присваивается значение 17

set(SOURCE_FILES main.cpp) #переменной SOURCE_FILES присваивается значение main.cpp

add_executable(HashTable_run ${SOURCE_FILES}) #создаёт исполняемый файл с именем DLinkeList_Hastable_run из исходников перечисленных в source_files

include_directories(src) #Расположение файлов и дополнительных CMaleLists.txt
add_subdirectory(src) # добавление подпроекта, указывается имя директории

target_link_libraries(HashTable_run HashTable_lib) #линкуем библиотеку к испольняемому файлу
add_subdirectory(tests) # добавляем подпроект для тестирования