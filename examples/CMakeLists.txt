# minimum CMake version required
cmake_minimum_required(VERSION 3.26)

# examples toggles
option(COMPILE_REQUESTER_EXAMPLE "Compiles the requester example code" OFF)
option(COMPILE_FAVORITES_EXAMPLE "Compiles the favorites example code" OFF)

# build requester example
if (COMPILE_REQUESTER_EXAMPLE)
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
endif (COMPILE_REQUESTER_EXAMPLE)
unset(COMPILE_REQUESTER_EXAMPLE CACHE)

# build favorites example
if (COMPILE_FAVORITES_EXAMPLE)
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
endif (COMPILE_FAVORITES_EXAMPLE)
unset(COMPILE_FAVORITES_EXAMPLE CACHE)
