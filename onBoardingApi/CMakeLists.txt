cmake_minimum_required(VERSION 3.19)

project(onBoardingApi LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core HttpServer)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup()

qt_add_executable(onBoardingApi
    main.cpp
    hellocontroller.h hellocontroller.cpp
    commanderepository.h commanderepository.cpp
    MyDatabase.cpp
    commandeservice.h commandeservice.cpp
    commandecontroller.h commandecontroller.cpp

)

target_link_libraries(onBoardingApi
    PRIVATE
        Qt6::Core
        Qt6::HttpServer
        Qt6::Sql
)

install(TARGETS onBoardingApi
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
