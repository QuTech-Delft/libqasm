You can build the C++ binaries from the project's root directory.
The following line will also build and cache the `libqasm` Conan package.

!!! note
    You may need to execute the `conan profile detect` command if this is the first time you run Conan.

```shell
conan profile detect
conan create --version 0.6.8 . -pr:a=tests-debug -b missing
```

You can test the C++ binaries:

```shell
cd test/Debug
ctest -C Debug --output-on-failure
```
