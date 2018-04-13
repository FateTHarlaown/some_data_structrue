cmake_minimum_required(VERSION 3.7)
project(some_data_structrue)

set(CMAKE_CXX_STANDARD 11)

include_directories(spin_lock skip_list test)
link_libraries(pthread)

set(SOURCE_FILES main.cpp spin_lock/SpinLock.h spin_lock/SpinLockTest.cpp spin_lock/SpinLockTest.h test/MyTest.cpp test/MyTest.h test/TestCase.h)
add_executable(some_data_structrue ${SOURCE_FILES})