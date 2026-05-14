# minimum CMake version required
cmake_minimum_required(VERSION 3.26)

# examples toggles
option(REQUESTER_EXAMPLE "Compiles the requester example code" OFF)
option(FAVORITES_EXAMPLE "Compiles the favorites example code" OFF)

# build requester example
if (REQUESTER_EXAMPLE)
    add_executable(requester_ex examples/requester.cpp)

    target_include_directories(requester_ex PRIVATE include)

    target_link_libraries(requester_ex boorudl)

    set_target_properties(requester_ex PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_compile_options(requester_ex PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra --pedantic-errors>
    )
endif (REQUESTER_EXAMPLE)
unset(REQUESTER_EXAMPLE CACHE)

# build favorites example
if (FAVORITES_EXAMPLE)
    add_executable(favorites_ex examples/favorites.cpp)

    target_include_directories(favorites_ex PRIVATE include)

    target_link_libraries(favorites_ex boorudl)

    set_target_properties(favorites_ex PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_compile_options(favorites_ex PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra --pedantic-errors>
    )
endif (FAVORITES_EXAMPLE)
unset(FAVORITES_EXAMPLE CACHE)
