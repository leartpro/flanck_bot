	# minimum CMake version required
    cmake_minimum_required(VERSION 3.15)
    # Project name, version and description
    project(flanck_bot VERSION 1.0 DESCRIPTION "A Discord Integration for Flanck")
    # Add DPP and Flanck as dependency
    add_subdirectory(libs/DPP)
    add_subdirectory(libs/flanck)
    # Create an executable
    add_executable(
            ${PROJECT_NAME}
            src/main.cpp
            src/DiscordToken.h
    )
    # Linking libraries
    target_link_libraries(
            ${PROJECT_NAME}
            dpp
            flanck
    )
    # Specify includes
    target_include_directories(
            ${PROJECT_NAME} PRIVATE
            libs/DPP/include
    )
    # Set C++ version
    set_target_properties(
            ${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
    )