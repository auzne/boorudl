# boorudl

boorudl is a C++17 library for api consuming and web scraping of Gelbooru Beta websites using [cURL](https://curl.se/libcurl/) and [pugixml](https://pugixml.org/)

this project started in 2023 for a single Gelbooru Beta website and had some updates till 2025, now it's renamed and compatible with every website that uses the same template

## Building

Dependencies must be installed in the system to compile

Build can be done using cmake on project root
```bash
cmake -B build
cmake --build build --target boorudl
```

Examples can be build by adding `-D<example option>=ON` after `cmake -B build` and setting the target to the example's or `all`
```bash
cmake -B build -DCOMPILE_REQUEST_EXAMPLE=ON
cmake --build build --target all
```

Output will be available at `build` directory
