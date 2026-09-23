# C++11 tests

From r57cpp onward, the wrapper requires C++11 or later. **r56cpp was the last
version compatible with C++98.**

From the repository root, run:

```sh
sh cpp/tests/run.sh
CC=clang CXX=clang++ sh cpp/tests/run.sh
SANITIZE=1 sh cpp/tests/run.sh
```
