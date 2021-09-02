//===-- pfp128.h - Architecture and compiler neutral shims for 128b IEEE FP --------------*- C -*-===//
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/*
 * A portability shim header to provide support for IEEE FP128
 * on machines/compilers which have the underlying support, but don't
 * provide the C standard interfaces.
 *
 */
// Header monotonicity.
#if (!defined(__PFP128_H_INCLUDED))
#define __PFP128_H_INCLUDED 1

#if (!(__x86_64__ || __aarch64__ || __riscv))
#error Unknown target architecture.
#endif

// If the compiler has already set this, that's fine with us;
// if not, set it for the cases we know about.
#if (!defined(__LONG_DOUBLE_IEEE128__))
// Add platforms for which the 128b IEEE float is long double here.
// This seems to be the case for Aarch64 and RISC-V both 32b and 64b, but not
// 32b Arm.
if (__aarch64__ || __riscv)
#define __LONG_DOUBLE_IEEE128__ 1
#endif
#endif

#if (defined(__LONG_DOUBLE_IEEE128__))
  // FP128 is long double which is  supported by the compiler.
  // (Note than the C standard only requires that long double
  // is at least as large as double, so it  may be accepted
  // syntactically, but not get you any more precision.
  // That is why we need to check architectures above...
  typedef long double FP128; // Both LLVM and GCC support the long float
  typedef _Complex long double COMPLEX_FP128;

#define FP128_CONST(val) val##L // the L suffix on constants, and
#define FP128_FMT_TAG "L"       // as a format suffix in printf,
#define FP128_FUNCTION_TAG l    // and we want the functions from libm
                                // tagged with l
#else
// long double is not the IEEE 128b format.
// We expect this to be x86_64, and haven't checked other places,
// so for now error out if it's not,
#if (!__x86_64__)
#error On an architecture this code does not understand.
#endif
#include <quadmath.h>

// Need to step very carefully around 80b floats!
typedef __float128 FP128; // Both LLVM and GCC support the __float128 type,
typedef __complex128 COMPLEX_FP128;

#define FP128_CONST(val) val##Q // the Q suffix on constants, and
#define FP128_FMT_TAG "Q"       // as a format suffix in printf,
#define FP128_FUNCTION_TAG q    // and we want the functions tagged with q
#endif                          // end of architecture spcific setup.

// From here on the code is common no matter what the underlying implementation.
// Macro for adding the tag
#define FP128MathFunction(base) base##FP128_FUNCTION_TAG

// Maths functions
// Macro on the simple name, rather than a function application in case
// there's code like
// FP128 (func)(FP128) = acosFP128;
//
#define acosFP128 FP128MathFunction(acos)
#define acoshFP128 FP128MathFunction(acosh)
#define asinFP128 FP128MathFunction(asin)
#define asinhFP128 FP128MathFunction(asinh)
#define atanFP128 FP128MathFunction(atan)
#define atanhFP128 FP128MathFunction(atanh)
#define atan2FP128 FP128MathFunction(atan2)
#define cbrtFP128 FP128MathFunction(cbrt)
#define ceilFP128 FP128MathFunction(ceil)
#define copysignFP128 FP128MathFunction(copysign)
#define coshFP128 FP128MathFunction(cosh)
#define cosFP128 FP128MathFunction(cos)
#define erfFP128 FP128MathFunction(erf)
#define erfcFP128 FP128MathFunction(erfc)
#define exp2FP128 FP128MathFunction(exp2)
#define expFP128 FP128MathFunction(exp)
#define expm1FP128 FP128MathFunction(expm1)
#define fabsFP128 FP128MathFunction(fabs)
#define fdimFP128 FP128MathFunction(fdim)
#define finiteFP128 FP128MathFunction(finite)
#define floorFP128 FP128MathFunction(floor)
#define fmaFP128 FP128MathFunction(fma)
#define fmaxFP128 FP128MathFunction(fmax)
#define fminFP128 FP128MathFunction(fmin)
#define fmodFP128 FP128MathFunction(fmod)
#define frexpFP128 FP128MathFunction(frexp)
#define hypotFP128 FP128MathFunction(hypot)
#define isinfFP128 FP128MathFunction(isinf)
#define ilogbFP128 FP128MathFunction(ilogb)
#define isnanFP128 FP128MathFunction(isnan)
#define issignalingFP128 FP128MathFunction(issignaling)
#define j0FP128 FP128MathFunction(j0)
#define j1FP128 FP128MathFunction(j1)
#define jnFP128 FP128MathFunction(jn)
#define ldexpFP128 FP128MathFunction(ldexp)
#define lgammaFP128 FP128MathFunction(lgamma)
#define llrintFP128 FP128MathFunction(llrint)
#define llroundFP128 FP128MathFunction(llround)
#define logbFP128 FP128MathFunction(logb)
#define logFP128 FP128MathFunction(log)
#define log10FP128 FP128MathFunction(log10)
#define log2FP128 FP128MathFunction(log2)
#define log1pFP128 FP128MathFunction(log1p)
#define lrintFP128 FP128MathFunction(lrint)
#define lroundFP128 FP128MathFunction(lround)
#define modfFP128 FP128MathFunction(modf)
#define nanFP128 FP128MathFunction(nan)
#define nearbyintFP128 FP128MathFunction(nearbyint)
#define nextafterFP128 FP128MathFunction(nextafter)
#define powFP128 FP128MathFunction(pow)
#define remainderFP128 FP128MathFunction(remainder)
#define remquoFP128 FP128MathFunction(remquo)
#define rintFP128 FP128MathFunction(rint)
#define roundFP128 FP128MathFunction(round)
#define scalblnFP128 FP128MathFunction(scalbln)
#define scalbnFP128 FP128MathFunction(scalbn)
#define signbitFP128 FP128MathFunction(signbit)
#define sincosFP128 FP128MathFunction(sincos)
#define sinhFP128 FP128MathFunction(sinh)
#define sinFP128 FP128MathFunction(sin)
#define sqrtFP128 FP128MathFunction(sqrt)
#define tanFP128 FP128MathFunction(tan)
#define tanhFP128 FP128MathFunction(tanh)
#define tgammaFP128 FP128MathFunction(tgamma)
#define truncFP128 FP128MathFunction(trunc)
#define y0FP128 FP128MathFunction(y0)
#define y1FP128 FP128MathFunction(y1)
#define ynFP128 FP128MathFunction(yn)

// Complex function names
#define cabsFP128 FP128MathFunction(cabs)
#define cargFP128 FP128MathFunction(carg)
#define cimagFP128 FP128MathFunction(cimag)
#define crealFP128 FP128MathFunction(creal)
#define cacosFP128 FP128MathFunction(cacos)
#define cacoshFP128 FP128MathFunction(cacosh)
#define casinFP128 FP128MathFunction(casin)
#define casinhFP128 FP128MathFunction(casinh)
#define catanFP128 FP128MathFunction(catan)
#define catanhFP128 FP128MathFunction(catanh)
#define ccosFP128 FP128MathFunction(ccos)
#define ccoshFP128 FP128MathFunction(ccosh)
#define cexpFP128 FP128MathFunction(cexp)
#define cexpiFP128 FP128MathFunction(cexpi)
#define clogFP128 FP128MathFunction(clog)
#define clog10FP128 FP128MathFunction(clog10)
#define conjFP128 FP128MathFunction(conj)
#define cpowFP128 FP128MathFunction(cpow)
#define cprojFP128 FP128MathFunction(cproj)
#define csinFP128 FP128MathFunction(csin)
#define csinhFP128 FP128MathFunction(csinh)
#define csqrtFP128 FP128MathFunction(csqrt)
#define ctanFP128 FP128MathFunction(ctan)
#define ctanhFP128 FP128MathFunction(ctanh)

// Constants
#define FP128_MAX FP128_CONST(1.18973149535723176508575932662800702e4932)
#define FP128_MIN FP128_CONST(3.36210314311209350626267781732175260e-4932)
#define FP128_EPSILON FP128_CONST(1.92592994438723585305597794258492732e-34)
#define FP128_DENORM_MIN                                                       \
  FP128_CONST(6.475175119438025110924438958227646552e-4966)
#define FP128_MANT_DIG 113
#define FP128_MIN_EXP (-16381)
#define FP128_MAX_EXP 16384
#define FP128_DIG 33
#define FP128_MIN_10_EXP (-4931)
#define FP128_MAX_10_EXP 4932

// #define HUGE_VALFP128 __builtin_huge_valq()
#define M_EFP128 FP128_CONST(2.718281828459045235360287471352662498) /* e */
#define M_LOG2EFP128                                                           \
  FP128_CONST(1.442695040888963407359924681001892137) /* log_2 e */
#define M_LOG10EFP128                                                          \
  FP128_CONST(0.434294481903251827651128918916605082) /* log_10 e */
#define M_LN2FP128                                                             \
  FP128_CONST(0.693147180559945309417232121458176568) /* log_e 2 */
#define M_LN10FP128                                                            \
  FP128_CONST(2.302585092994045684017991454684364208) /* log_e 10 */
#define M_PIFP128 FP128_CONST(3.141592653589793238462643383279502884) /* pi */
#define M_PI_2FP128                                                            \
  FP128_CONST(1.570796326794896619231321691639751442) /* pi/2 */
#define M_PI_4FP128                                                            \
  FP128_CONST(0.785398163397448309615660845819875721) /* pi/4 */
#define M_1_PIFP128                                                            \
  FP128_CONST(0.318309886183790671537767526745028724) /* 1/pi */
#define M_2_PIFP128                                                            \
  FP128_CONST(0.636619772367581343075535053490057448) /* 2/pi */
#define M_2_SRTPIFP128                                                         \
  FP128_CONST(1.128379167095512573896158903121545172) /* 2/sqrt(pi) */
#define M_SRT2FP128                                                            \
  FP128_CONST(1.414213562373095048801688724209698079) /* sqrt(2) */
#define M_SRT1_2FP128                                                          \
  FP128_CONST(0.707106781186547524400844362104849039) /* 1/sqrt(2) */

// Cleanliness
#undef FP128MathFunction

#endif // Header monotonicity
