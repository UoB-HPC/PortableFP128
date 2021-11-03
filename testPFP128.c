//===-- testPFP128.c - test the pfp128.h header --------------*- C -*-===//
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//
// A simple test code which simply checks that our interface
// is finding all of the underlying functions.
// We are *not* checking that those underlying functions behave
// correctly, that is an SEP (Someone Else's Problem), merely that
// our shim layer is working.
// As such failures are most likely at compile or link time.
//

#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define PFP128_SHOW_CONFIG 1
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

#if (__APPLE__ && __MACH__)
#define TARGET_OS_NAME "MacOS"
#elif (defined(LINUX) || defined(__linux__) || defined(__linux))
#define TARGET_OS_NAME "Linux"
#else
#error Unknown operating system
#endif

// Cheat somewhat; here we extract the underlying function on whatever our
// system is, and then compare the output for each of the functions going
// directly to the underlying function, or via the FP128 interface. That does
// mean that we're copying a bunch of code from the header, but if it is wrong
// that should hurt here too.

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
#define FP128Name(base) base##l
#define FP128ConstName(base) base##L
#else
// long double is not the IEEE 128b format.
#define FP128Name(base) base##q
#define FP128ConstName(base) base##Q
#endif // end of architecture spcific setup.

// From here on the code is common no matter what the underlying implementation.
// clang-format really messes up the multiple line macro definitions :-(
// clang-format off

static void printError(char const * baseName,
		       char const * underlyingName,
		       FP128 baseResult, FP128 ourResult) {
  char brstr[100];
  char orstr[100];

  FP128_snprintf(&brstr[0], sizeof(brstr),
		 "%12.10" FP128_FMT_TAG "f", baseResult);
  FP128_snprintf(&orstr[0], sizeof(orstr),
		 "%12.10" FP128_FMT_TAG "f", ourResult);
  printf("*** %s FAILED: %sFP128 => %s %s => %s\n",
	 baseName, baseName, orstr, underlyingName, brstr);				
}
		       
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
// acosh is not happy with pi/4as an argument, so we treat it separately
// exp2 does not seem to be available everywhere...
//   op(exp2, FP128, FP128)                     

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
      if (verbose)                                                      \
        printf ("%-9s passed\n", STRINGIFY(basename));                  \
      passes++;                                                         \
    } else {                                                            \
      printError(STRINGIFY(basename), STRINGIFY(FP128Name(basename)), baseResult, ourResult); \
      failures++;                                                       \
    }                                                                   \
  }

static int passes = 0;
static int failures = 0;
static int verbose = 0;

static void test128to128UnaryFunctions() {
  FOREACH_128TO128_UNARY_FUNCTION(TestUnary128to128Function)
    // acosh needs a different argument!    
  {                                                  
    FP128 baseResult = FP128Name(acosh)(M_PI_FP128);
    FP128 ourResult  = acoshFP128(M_PI_FP128);

    if (baseResult == ourResult) {
      if (verbose)
        printf ("acosh     passed\n");
      passes++;
    } else {
      printError(STRINGIFY(acosh), STRINGIFY(FP128Name(acosh)), baseResult, ourResult); 
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
      if (verbose)                                                      \
        printf ("%-9s passed\n", STRINGIFY(basename));                  \
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
      if (verbose)                                                      \
        printf ("%-9s passed\n", STRINGIFY(basename));                  \
      passes++;                                                         \
    } else {                                                            \
      printf("*** " #basename " FAILED: base=%12.10" FP128_FMT_TAG      \
             "f ours=%12.10" FP128_FMT_TAG "f\n",                       \
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
      if (verbose)                                                      \
        printf ("%-9s passed\n", STRINGIFY(basename));                  \
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

// Missing ldexp, modf

#define Test128BinaryFunction(basename, restype, at1, at2)              \
  {                                                                     \
    restype baseResult = FP128Name(basename)(M_PI_FP128/4.0, 1.0);      \
    restype ourResult  = basename##FP128(M_PI_FP128/4.0, 1.0);          \
                                                                        \
    if (baseResult == ourResult) {                                      \
      if (verbose)                                                      \
         printf ("%-9s passed\n", STRINGIFY(basename));                 \
      passes++;                                                         \
    } else {                                                            \
      printError(STRINGIFY(basename), STRINGIFY(FP128Name(basename)), baseResult, ourResult); \
      failures++;                                                       \
    }                                                                   \
  }

// Missing ternary functions, requo, fma

// clang-format on

static void test128BinaryFunctions() {
  FOREACH_128BINARY_FUNCTION(Test128BinaryFunction)
}

static void testInput() {
  FP128 value = strtoFP128("2.718281828459045235360287471352662498", (void *)0);
  if (value == M_E_FP128) {
    if (verbose)
      printf("strtoFP128 passed\n");
    passes++;
  } else {
    printf("*** strtoFP128 FAILED\n");
    failures++;
  }
}

// Does snprintf work with the tag we believe should be used?
static void testPrintf() {
  char line[64];
  char const * correct = "2.718281828459045235360287471352662";
  
  (void)FP128_snprintf(&line[0], sizeof(line),"%35.33" FP128_FMT_TAG "f", M_E_FP128);
  if (!strcmp(&line[0],correct)) {
    if (verbose)
      printf("FP128_snprintf passed\n");
    passes++;
  } else {
    printf("*** FP128_snprintf FAILED\n"
	   "*** returns '%s'\n"
	   "*** not     '%s'\n",&line[0],correct);
    failures++;
  }

  // And what about the system version?
  (void)snprintf(&line[0], sizeof(line),"%35.33" FP128_FMT_TAG "f", M_E_FP128);
  if (!strcmp(&line[0],correct)) {
    if (verbose)
      printf("snprintf  passed\n");
    passes++;
  } else {
    printf("*** snprintf FAILED\n"
	   "*** returns '%s'\n"
	   "*** not     '%s'\n",&line[0],correct);
    failures++;
  }

  if (sizeof(FP128) != 16) {
    printf ("*** These failures are expected, since the notional FP128 type is not really that long!\n");
  }
}

static int bytesUsed(uint8_t const *p) {
  // Assume little endian and 64B allocation
  // Assume little-endian byte layout.
  for (int i = 15; i >= 0; i--) {
    if (p[i] & 0x80)
      return i + 1;
  }
  return -1;
}

static void checkSize() {
  union {
    long double d;
    uint8_t bytes[16];
  } u;
  memset(u.bytes, 0, sizeof(u.bytes));
  u.d = -1.0l;
  int bu = bytesUsed(u.bytes);
  printf("sizeof(long double) = %lu bytes: %lu bits; data used = %d bytes : %d "
         "bits\n",
         sizeof(long double), 8 * sizeof(long double), bu, bu * 8);

  union {
    FP128 d;
    uint8_t bytes[16];
  } v;
  // On x86_64, the long double type is only 80b, but is padded to 128b for
  // alignment, so sizeof will still return 16 even though only 10 bytes are
  // used. Assuming we're on a little endian machine, we can check for that by
  // looking at the sign bit...
  memset(v.bytes, 0, sizeof(v.bytes));
  v.d = FP128_CONST(-1.0);
  bu = bytesUsed(&v.bytes[0]);
  printf("sizeof(FP128)       = %lu bytes: %lu bits; data used = %d bytes : %d "
         "bits\n",
         sizeof(FP128), 8 * sizeof(FP128), bu, bu * 8);

  if (sizeof(FP128) != 128 / 8) {
    printf("*** FP128 is not being converted to a 128b underlying type! ***\n");
    printf("*** Results merely show that the shorter (%lub) type works. ***\n",
           sizeof(FP128) * 8);
    return;
  }
}

int main(int argc, char **argv) {
  verbose = argc > 1;

  printf(COMPILER_NAME " targeting " TARGET_OS_NAME
                       " running on " TARGET_ARCH_NAME "\n");
  checkSize();

  test128to128UnaryFunctions();
  test128toIntUnaryFunctions();
  testComplexTo128UnaryFunctions();
  testComplexToComplexUnaryFunctions();
  test128BinaryFunctions();
  testInput();
  testPrintf();
  printf("(Not tested: exp2, ldexp, modf, remquo, fma)\n");

  printf("*** %d pass%s, %d failure%s ***\n", passes, passes == 1 ? "" : "es",
         failures, failures == 1 ? "" : "s");

  return failures;
}
