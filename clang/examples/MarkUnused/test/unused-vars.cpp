// RUN: %clang_cc1 -load %llvmshlibdir/MarkUnused.so -fsyntax-only -Wunused-variable -Wunused-parameter %s

int foo(int a, int b, int c) {
    double value = 0.0;
    return a + b;
}
