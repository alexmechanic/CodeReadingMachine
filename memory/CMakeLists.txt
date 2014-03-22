cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(memory)				# Название проекта

set(SOURCES main.cpp)
include_directories(Config)
include_directories(LogKeeper)
include_directories(TLB)
include_directories(MemoryUnit)

set(SOURCES ${SOURCES} Config/Config.cpp LogKeeper/LogKeeper.cpp TLB/TLB.cpp MemoryUnit/MemoryUnit.cpp)

add_executable (main ${SOURCES})
