# minimum CMake version required
cmake_minimum_required(VERSION 3.26)
# project info
project(libboorudl DESCRIPTION "Api consumer and web scraper for Gelbooru Beta")

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

