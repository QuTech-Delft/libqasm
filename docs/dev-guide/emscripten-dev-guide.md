You can build the Emscripten binaries from the project's root directory.
The generation of Emscripten binaries has been tested as a cross-compilation from an ubuntu/x64 platform.

```shell
conan build . -pr=conan/profiles/release-clang-emscripten-wasm -pr:b=conan/profiles/release -b missing
```

The output of this build lives in `build/Release/emscripten`:

- `cqasm_emscripten.js`.
- `cqasm_emscripten.wasm`.

!!! note

    `cqasm_emscripten.js` is an ES6 module.
    Its extension has to be renamed to `.mjs` before using it from Typescript code.

You can test the Emscripten binaries:

```shell
cd build/Release/emscripten
mv cqasm_emscripten.js cqasm_emscripten.mjs
cd ../../../emscripten
deno run -A test_libqasm.ts
```
