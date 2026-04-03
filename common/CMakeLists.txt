# Build the common library used by the top-level executable
add_library(common_lib STATIC
    ControllerAgent.cpp
    RateLimiter.cpp
    # headers for IDE grouping
    ConnectionHandler.h
    ControllerAgent.h
    Enums.h
    RateLimiter.h

)

target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# BLE implementation lives in common/BLE and provides target 'ble_lib'
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/BLE)

# Link common_lib to BLE implementation and Qt Core
target_link_libraries(common_lib
    PUBLIC ble_lib Qt${QT_VERSION_MAJOR}::Core
)
