# Portable FP128
This code attempts to provide a way to write C (or C++) code that uses
IEEE 128b floating point without being bound to specific,
non-standard, syntax.

Using this will require changes to code that is already running on
X86_64 using GCC and  `<quadmath.h>`, but once whose changes have been made it should
then be possible to continue to use the implementation that provides,
while having code that can also compile using LLVM and target AArch64
machines (though note that MacOS on AArch64 does not provide any support for 128b floats,
and, as a result, neither do compilers targeting that environment). 

# How to Use This
Copy the `pfp128.h` header somewhere where you can find it, then include it in source files that need the portability shims.
Edit your code to replace 

 - references to `_Float128` or `__float128` with `FP128`
 - constant declararations of 128b constants with `FP128_CONST(...)`
 - conversion of strings to 128b floats to use `FP128 strtoFP128(char const *, char **);`
 - conversion of output to strings either to use `FP128_snprintf`, or, slightly more riskily, use `printf` and friends, but change the format tags to use `FP128_FMT_TAG`, rather than an explicit tag.

# Settings
The header file ccontains a number of `#warning` directives which can be used to show you what it thinks is going on.
THese can be enabled by `#define PFP128_SHOW_CONFIG 1` before including the header. 

# Background
See the blog post at https://cpufun.substack.com/p/portable-support-for-128b-floats for more details on how we got to here!

