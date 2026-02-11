cmake_minimum_required(VERSION 3.16)
project(worksheet5_installer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Targets ----
add_library(maths SHARED adder.cpp)
target_include_directories(maths PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(maths PRIVATE maths_EXPORTS)

add_executable(calc calc.cpp)
target_include_directories(calc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(calc PRIVATE maths)

# ---- Install layout (bin / lib / include) ----
install(TARGETS calc maths
  RUNTIME DESTINATION bin          # .exe and needed .dll at runtime
  LIBRARY DESTINATION lib          # .dll import library (on non-Windows)
  ARCHIVE DESTINATION lib          # .lib (import or static archive)
)

install(FILES adder.h DESTINATION include)

# ---- CPack (installer) ----
set(CPACK_PACKAGE_NAME "worksheet5-calc")
set(CPACK_PACKAGE_VENDOR "YourName")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "worksheet5-calc")

# On Windows, use NSIS if installed
if(WIN32)
  set(CPACK_GENERATOR "NSIS")
endif()

include(CPack)
