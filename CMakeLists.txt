cmake_minimum_required(VERSION 3.12)
project(wxwidgets_hello)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set wxWidgets path for macOS
set(wxWidgets_ROOT_DIR "/opt/homebrew/opt/wxwidgets")
set(wxWidgets_CONFIGURATION mswu)

# Find wxWidgets
find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})

# Add executable
add_executable(${PROJECT_NAME}
        MainFrame.cpp
        MainFrame.h
        App.cpp
        App.h
        GameModeDialog.cpp
        GameModeDialog.h
)

# Link wxWidgets
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})