cmake_minimum_required(VERSION 3.21)
project(blockchain)

set(CMAKE_CXX_STANDARD 17)
add_executable(blockchain main.cpp transaction.cpp node.cpp block.h node.h transaction.h sha256.cpp sha256.h block.cpp)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(blockchain PRIVATE Threads::Threads)
