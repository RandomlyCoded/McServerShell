cmake_minimum_required(VERSION 3.16)

project(McServerShell LANGUAGES CXX)

find_package(Curses REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(McServerShell
    src/main.cpp
    src/processhandler.h src/processhandler.cpp
    src/ansi_to_ncurses.h src/ansi_to_ncurses.cpp)

add_custom_target(Documentation SOURCES
    LICENSE
    README.md
)

target_link_libraries(McServerShell ${CURSES_LIBRARY})

include(GNUInstallDirs)
install(TARGETS McServerShell
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
