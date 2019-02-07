set(src
    main.cpp

    basic_properties.cpp
    group_properties.cpp
    numeric_properties.cpp

    deserialise.cpp
    serialise.cpp
)

add_executable(test_properties ${src})

target_link_libraries(test_properties
    properties
    Catch2::Catch2
)

include(Catch)
catch_discover_tests(test_properties)
