# minimum CMake version required
cmake_minimum_required(VERSION 3.26)
# project info
project(libboorudl DESCRIPTION "Api consumer and web scraper for Gelbooru Beta")

option(COMPILE_REQUEST_EXAMPLE "Compiles the request example code located in the examples folder" OFF)

# create the library
add_library(boorudl SHARED
    src/batches.cpp
    src/comment.cpp
    src/credentials.cpp
    src/curl_handler.cpp
    src/downloader.cpp
    src/enums.cpp
    src/favorites.cpp
    src/ids.cpp
    src/options.cpp
    src/page.cpp
    src/post.cpp
    src/request.cpp
    src/requester.cpp
    src/source.cpp
    src/tags.cpp
)

# include folder
target_include_directories(boorudl
    PUBLIC include
    PRIVATE src/include
)

# link libraries
target_link_libraries(boorudl
    curl
    pugixml
    #spdlog
)

# set c++ version
set_target_properties(boorudl PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# compile options
target_compile_options(boorudl PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra --pedantic-errors>
)

# examples build
if (COMPILE_REQUEST_EXAMPLE)
    add_executable(request_ex examples/request.cpp)

    target_include_directories(request_ex PRIVATE include)

    target_link_libraries(request_ex boorudl)

    set_target_properties(request_ex PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_compile_options(request_ex PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra --pedantic-errors>
    )
endif (COMPILE_REQUEST_EXAMPLE)
unset(COMPILE_REQUEST_EXAMPLE CACHE)
