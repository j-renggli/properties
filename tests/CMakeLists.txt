set(src
    main.cpp

    basic_properties.cpp
)

add_executable(test_properties ${src})

target_link_libraries(test_properties
    properties
    Catch2::Catch2
)

include(Catch)
catch_discover_tests(test_properties)
