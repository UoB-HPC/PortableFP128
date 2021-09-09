//===-- testPFP128.c - test the pfp128.h header --------------*- C -*-===//
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>
#include "pfp128.h"

// Expand a macro and convert the result into a string
#define STRINGIFY1(...) #__VA_ARGS__
#define STRINGIFY(...) STRINGIFY1(__VA_ARGS__)
// Determine the target architecture
#if (__x86_64__)
#define TARGET_ARCH_NAME "x86_64"
#elif (__aarch64__)
#define TARGET_ARCH_NAME "aarch64"
#elif (__arm__)
#define TARGET_ARCH_NAME "armv7l"
#elif (__riscv)
#define TARGET_ARCH_NAME "riscv"
#else
#error "Unknown target architecture"
#endif
#if defined(__cray__)
#define COMPILER_NAME "Cray: " __VERSION__
#elif defined(__INTEL_COMPILER)
#define COMPILER_NAME                                                          \
  "Intel: " STRINGIFY(__INTEL_COMPILER) "v" STRINGIFY(                         \
      __INTEL_COMPILER_BUILD_DATE)
#elif defined(__clang__)
#define COMPILER_NAME                                                          \
  "LLVM: " STRINGIFY(__clang_major__) ":" STRINGIFY(                           \
      __clang_minor__) ":" STRINGIFY(__clang_patchlevel__)
#elif defined(__GNUC__)
#define COMPILER_NAME "GCC: " __VERSION__
#else
#define COMPILER_NAME "Unknown compiler"
#endif

// Cheat somewhat; here we extract the underlying function on whatever our system is,
// and then compare the output for each of the functions going directly to the underlying function,
// or via the FP128 interface.
// That does mean that we're cpopying a bunch of code from the header, but
// if its wrong that should hurt here too.

// If the compiler has already set this, that's fine with us;
// if not, set it for the cases we know about.
#if (!defined(__LONG_DOUBLE_IEEE128__))
// Add platforms for which the 128b IEEE float is long double here.
// This seems to be the case for Aarch64 and RISC-V both 32b and 64b, but not
// 32b Arm.
#if (__aarch64__ || __riscv)
#define __LONG_DOUBLE_IEEE128__ 1
#endif
#endif
  
#if (defined(__LONG_DOUBLE_IEEE128__))
#define FP128Name(base) base ## l
#define FP128ConstName(base) base ## L
#else
// long double is not the IEEE 128b format.
#define FP128Name(base) base ## q
#define FP128ConstName(base) base ## Q
#endif // end of architecture spcific setup.

// From here on the code is common no matter what the underlying implementation.
// clang-format really messes up the multiple line macro definitions :-(
// clang-format off

// These functions are all happy with pi/4 as an argument:-)
#define FOREACH_128TO128_UNARY_FUNCTION(op)     \
  op(acos, FP128, FP128)                        \
  op(asin, FP128, FP128)                        \
  op(asinh, FP128, FP128)                       \
  op(atan, FP128, FP128)                        \
  op(atanh, FP128, FP128)                       \
  op(cbrt, FP128, FP128)                        \
  op(ceil, FP128, FP128)                        \
  op(cosh, FP128, FP128)                        \
  op(cos, FP128, FP128)                         \
  op(erf, FP128, FP128)                         \
  op(erfc, FP128, FP128)                        \
  op(exp2, FP128, FP128)                        \
  op(exp, FP128, FP128)                         \
  op(expm1, FP128, FP128)                       \
  op(fabs, FP128, FP128)                        \
  op(floor, FP128, FP128)                       \
  op(lgamma, FP128, FP128)                      \
  op(logb, FP128, FP128)                        \
  op(log, FP128, FP128)                         \
  op(log10, FP128, FP128)                       \
  op(log2, FP128, FP128)                        \
  op(log1p, FP128, FP128)                       \
  op(nearbyint, FP128, FP128)                   \
  op(rint, FP128, FP128)                        \
  op(round, FP128, FP128)                       \
  op(sinh, FP128, FP128)                        \
  op(sin, FP128, FP128)                         \
  op(sqrt, FP128, FP128)                        \
  op(tan, FP128, FP128)                         \
  op(tanh, FP128, FP128)                        \
  op(tgamma, FP128, FP128)                      \
  op(trunc, FP128, FP128)                       
// acosh is not hgappy with pi/4as an argument, so we treat it separately

#define FOREACH_128TOINT_UNARY_FUNCTION(op)     \
  op(ilogb, int, FP128,"d")                     \
  op(llrint, long long int, FP128,"lld")        \
  op(llround, long long int, FP128,"lld")       \
  op(lrint, long int, FP128,"ld")               \
  op(lround, long int, FP128,"ld")

#define FOREACH_COMPLEXTO128_UNARY_FUNCTION(op) \
  op(cabs, FP128, COMPLEX_FP128)                \
  op(carg, FP128, COMPLEX_FP128)                \
  op(cimag, FP128, COMPLEX_FP128)               \
  op(creal, FP128, COMPLEX_FP128)

#define FOREACH_COMPLEXTOCOMPLEX_UNARY_FUNCTION(op)     \
  op(cacos, COMPLEX_FP128, COMPLEX_FP128)               \
  op(cacosh, COMPLEX_FP128, COMPLEX_FP128)              \
  op(casin, COMPLEX_FP128, COMPLEX_FP128)               \
  op(casinh, COMPLEX_FP128, COMPLEX_FP128)              \
  op(catan, COMPLEX_FP128, COMPLEX_FP128)               \
  op(catanh, COMPLEX_FP128, COMPLEX_FP128)              \
  op(ccos, COMPLEX_FP128, COMPLEX_FP128)                \
  op(ccosh, COMPLEX_FP128, COMPLEX_FP128)               \
  op(cexp, COMPLEX_FP128, COMPLEX_FP128)                \
  op(clog, COMPLEX_FP128, COMPLEX_FP128)                \
  op(conj, COMPLEX_FP128, COMPLEX_FP128)                \
  op(cproj, COMPLEX_FP128, COMPLEX_FP128)               \
  op(csin, COMPLEX_FP128, COMPLEX_FP128)                \
  op(csinh, COMPLEX_FP128, COMPLEX_FP128)               \
  op(csqrt, COMPLEX_FP128, COMPLEX_FP128)               \
  op(ctan, COMPLEX_FP128, COMPLEX_FP128)                \
  op(ctanh, COMPLEX_FP128, COMPLEX_FP128)
 
//  op(nan, FP128, const char *)                

#define TestUnary128to128Function(basename, restype, argtype)           \
  {                                                                     \
    restype baseResult = FP128Name(basename)(M_PI_FP128/4.0);           \
    restype ourResult  = basename##FP128(M_PI_FP128/4.0);               \
                                                                        \
    if (baseResult == ourResult) {                                      \
      printf (#basename " passed\n");                                   \
      passes++;                                                         \
    } else {                                                            \
      printf("*** " STRINGIFY(basename) " FAILED: "                     \
             STRINGIFY(basename##FP128) "(%12.10" FP128_FMT_TAG "f) "   \
             STRINGIFY(FP128Name(basename))"(%12.10" FP128_FMT_TAG "f)\n", \
             baseResult, ourResult);                                    \
      failures++;                                                       \
    }                                                                   \
  }

static int passes = 0;
static int failures = 0;

static void test128to128UnaryFunctions() {
  FOREACH_128TO128_UNARY_FUNCTION(TestUnary128to128Function)
    // acosh needs a different argument!    
  {                                                  
    FP128 baseResult = FP128Name(acosh)(M_PI_FP128);
    FP128 ourResult  = acoshFP128(M_PI_FP128);

    if (baseResult == ourResult) {
      printf ("acosh passed\n");
      passes++;
    } else {
      printf("*** acosh FAILED: base=%12.10" FP128_FMT_TAG "f ours=%12.10" FP128_FMT_TAG "f\n",
             baseResult, ourResult);
      failures++;
    }
  }
}

#define TestUnary128toIntFunction(basename, restype, argtype,fmt)       \
  {                                                                     \
    restype baseResult = FP128Name(basename)(M_PI_FP128/4.0);           \
    restype ourResult  = basename##FP128(M_PI_FP128/4.0);               \
                                                                        \
    if (baseResult == ourResult) {                                      \
      printf (#basename " passed\n");                                   \
      passes++;                                                         \
    } else {                                                            \
      printf("*** " #basename " FAILED: base=%12" fmt " ours=%12" fmt "\n", \
             baseResult, ourResult);                                    \
      failures++;                                                       \
    }                                                                   \
  }

static void test128toIntUnaryFunctions() {
  FOREACH_128TOINT_UNARY_FUNCTION(TestUnary128toIntFunction)             
}

#define TestComplexTo128Function(basename,restype, argtype) {           \
    argtype arg = 1.0 + 2.0 * _Complex_I;                               \
    restype baseResult = FP128Name(basename)(arg);                      \
    restype ourResult  = basename##FP128(arg);                          \
                                                                        \
    if (baseResult == ourResult) {                                      \
      printf (#basename " passed\n");                                   \
      passes++;                                                         \
    } else {                                                            \
      printf("*** " #basename " FAILED: base=%12.10" FP128_FMT_TAG      \
             "f ours=%12.10" FP128_FMT_TAG "f\n",                        \
             baseResult,                                                \
             ourResult);                                                \
      failures++;                                                       \
    }                                                                   \
  }

static void testComplexTo128UnaryFunctions() {
  FOREACH_COMPLEXTO128_UNARY_FUNCTION(TestComplexTo128Function)
}    


#define TestComplexToComplexFunction(basename,restype, argtype) {       \
    argtype arg = 1.0 + 2.0 * _Complex_I;                               \
    restype baseResult = FP128Name(basename)(arg);                      \
    restype ourResult  = basename##FP128(arg);                          \
                                                                        \
    if (baseResult == ourResult) {                                      \
      printf (#basename " passed\n");                                   \
      passes++;                                                         \
    } else {                                                            \
      printf("*** " #basename " FAILED: base=(%12.10" FP128_FMT_TAG     \
             "f,%12.10" FP128_FMT_TAG "f) ours=(%12.10" FP128_FMT_TAG   \
             "f,%12.10" FP128_FMT_TAG "f)\n",                           \
             FP128Name(creal)(baseResult),                              \
             FP128Name(cimag)(baseResult),                              \
             FP128Name(creal)(ourResult),                               \
             FP128Name(cimag)(ourResult));                              \
      failures++;                                                       \
    }                                                                   \
  }

static void testComplexToComplexUnaryFunctions() {
  FOREACH_COMPLEXTOCOMPLEX_UNARY_FUNCTION(TestComplexToComplexFunction)
}    

#define FOREACH_128BINARY_FUNCTION(op)          \
  op(atan2, FP128, FP128, FP128)                \
  op(nextafter, FP128, FP128, FP128)            \
  op(pow, FP128, FP128, FP128)                  \
  op(remainder, FP128, FP128, FP128)            \
  op(copysign, FP128, FP128, FP128)             \
  op(fdim, FP128, FP128, FP128)                 \
  op(fmax, FP128, FP128, FP128)                 \
  op(fmin, FP128, FP128, FP128)                 \
  op(fmod, FP128, FP128, FP128)                 \
  op(hypot, FP128, FP128, FP128)

#define Test128BinaryFunction(basename, restype, at1, at2)              \
  {                                                                     \
    restype baseResult = FP128Name(basename)(M_PI_FP128/4.0, 1.0);      \
    restype ourResult  = basename##FP128(M_PI_FP128/4.0, 1.0);          \
                                                                        \
    if (baseResult == ourResult) {                                      \
      printf (#basename " passed\n");                                   \
      passes++;                                                         \
    } else {                                                            \
      printf("*** " #basename " FAILED: base=%12.10" FP128_FMT_TAG      \
             "f ours=%12.10" FP128_FMT_TAG "f\n",                       \
             baseResult, ourResult);                                    \
      failures++;                                                       \
    }                                                                   \
  }

// clang-format on

static void test128BinaryFunctions() {
  FOREACH_128BINARY_FUNCTION(Test128BinaryFunction)
}    

static int checkSize() {
  printf ("sizeof(long double) = %lu bytes - %lu bits\n",
          sizeof(long double), 8*sizeof(long double));
  printf ("sizeof(FP128)       = %lu bytes - %lu bits\n",
          sizeof(FP128), 8*sizeof(FP128));
  if (sizeof(FP128) != 128/8) {
    printf ("*** FP128 is not being converted to a 128b underlying type! ***\n");
    printf ("*** Results merely show that the shorter (%lub) type works. ***\n", sizeof(FP128)*8);
    return 1;
  }
  // On x86_64, the long double type is only 80b, but is padded to 128b for alignment,
  // so sizeof will still return 16 even though only 10 bytes are used.
  // Assuming we're on a little endian machine, we can check for that by looking at the
  // sign bit...
  union {
    FP128 d;
    uint8_t bytes[16];
  } u;
  // I know, memset ...
  for (int i=0; i<16; i++) {
    u.bytes[i] = 0;
  }
  u.d = FP128_CONST(-1.0);
  int i;
  for (i=15; i>=0; i--) {
    if ((u.bytes[i] & 0x80)) {
      break;
    }
  }
  if (i != 15) {
    printf ("FP128 seems to be using %d bytes (not all 16)\n", i+1);
    return 0;
  }
  return 1;
}

int main (int argc, char ** argv) {

  printf (COMPILER_NAME " targetting " TARGET_ARCH_NAME "\n");
  if (!checkSize()) {
    printf ("checkSize failed\n");
    return 1;
  }
  test128to128UnaryFunctions();
  test128toIntUnaryFunctions();
  testComplexTo128UnaryFunctions();
  testComplexToComplexUnaryFunctions();
  test128BinaryFunctions();
  
  printf ("*** %d pass%s, %d failure%s ***\n", passes, passes==1?"":"es", failures, failures==1?"":"s");
  return failures;
}
