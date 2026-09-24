# C++11 tests

From r57cpp onward, the wrapper requires C++11 or later. **r56cpp was the last
version compatible with C++98.**

From the repository root, run:

```sh
sh cpp/tests/run.sh
CC=clang CXX=clang++ sh cpp/tests/run.sh
SANITIZE=1 sh cpp/tests/run.sh
```

The runner builds everything twice: once with the parser defaults, and once with
`INI_CALL_HANDLER_ON_NEW_SECTION=1` so sections without fields are reported.
