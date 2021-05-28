#pragma once

const double PI = 3.14159265358979323846264338327950288419716;

static inline double sin(double x) {
    double res;
    __asm__ __volatile__(
        "fldl %1;"
        "fsin;"
        "fstpl %0;"
        : // output
            "=m"(res)
        : // input
            "m"(x)
    );
    return res;
}

static inline double cos(double x) {
    double res;
    __asm__ __volatile__(
        "fldl %1;"
        "fcos;"
        "fstpl %0;"
        : // output
            "=m"(res)
        : // input
            "m"(x)
    );
    return res;
}

static inline double sqrt(double x) {
    double res;
    __asm__ __volatile__(
        "fldl %1;"
        "fsqrt;"
        "fstpl %0;"
        : // output
            "=m"(res)
        : // input
            "m"(x)
    );
    return res;
}