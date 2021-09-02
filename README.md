# Portable FP128
The aim here is to provide a way to write C (or C++) code that uses
IEEE 128b floating point without being bound to specific,
non-standard, syntax.

Using this will require changes to code that is already running on
X86_64 using GCC and  <quadmath.h>, but once whose changes have been made it should
then be possible to continue to use the implementation that provides,
while having code that can also compile using LLVM and target AArch64
machines (such as modern Apple Macs).
