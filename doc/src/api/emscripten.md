libQASM is also deployed as an Emscripten binary with a Typescript frontend.

libQASM Typescript API is defined in `emscripten/emscripten_wrapper.hpp`.

The only exported class is `EmscriptenWrapper`.
This is actually a C++ class that works as a binding for accessing C++ code from Typescript.

::: EmscriptenWrapper
    handler: emscripten
