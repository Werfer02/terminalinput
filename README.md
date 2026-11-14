# terminalinput

mini library for reading and parsing virtual terminal input.

## Building

### building library

```
cmake -S . -B build
```
```
cmake --build build
```

### building demo
```
cmake -S . -B build -Dtilib_BUILD_DEMO
```
```
cmake --build build
```
demo in build/demo

## Including in another cmake project

1. Obtain in some way, e.g. FetchContent
```
FetchContent_Declare(
    terminalinput
    GIT_REPOSITORY https://github.com/Werfer02/terminalinput.git
    GIT_TAG master
)
FetchContent_MakeAvailable(terminalinput)
```
2. Link library
```
target_link_libraries(app PRIVATE tilib)
```
