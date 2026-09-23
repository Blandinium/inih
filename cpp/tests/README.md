# C++98 tests

From the repository root, run:

```sh
sh cpp/tests/run.sh
CC=clang CXX=clang++ sh cpp/tests/run.sh
SANITIZE=1 sh cpp/tests/run.sh
```

The runner works from any working directory, compiles the parser as C89 and
the wrapper and tests as strict C++98, and removes its temporary build files.
Set `CC` and `CXX` to select the C and C++ compilers. `SANITIZE=1` enables
AddressSanitizer and UndefinedBehaviorSanitizer on supported toolchains.

`INIReaderTest.cpp` exercises the example configuration. `INIReaderCopyTest.cpp`
checks copying, assignment over a populated reader, self-assignment, and use
after other copies are replaced or destroyed.

The copy tests use their own fixtures so upstream example changes do not
change the regression test inputs.
