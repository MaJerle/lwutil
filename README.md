# C language utility library

Set of different utility functions and macros usually used in the various applications.

## Features

- Written in C (C11), compatible with `stdint.h` data types
- Get the minimum or maximum of two values, constrain a value to a range, or map it between two ranges
- Get the absolute value of a signed input
- Silence "unused variable" compiler warnings
- Dereference and assign through a pointer, but only when it is not `NULL`
- Compute the number of elements in a statically allocated array
- Store and load `16-bit` and `32-bit` values to and from a byte buffer, in little- or big-endian format
  - Pointer-advancing extended variants are available for sequential (de)serialization
- Set, clear, toggle or check bits against a bit mask
- Convert `8/16/32-bit` values to their hexadecimal ASCII representation
- Encode and decode `32-bit` values in variable-length integer (`varint`) format
- Check whether a time period has elapsed against a rolling time reference, useful for non-blocking periodic tasks
- Calculate a rolling (sliding-window) linear regression slope over fixed-step sample data
- Assert an expression at compile time
- User friendly MIT license

## How to use

Usage is very simply. Add `lwutil.c` file to compilation flag and make sure compiler has access to `lwutil.h` for include paths.

## Contribute

Fresh contributions are always welcome. Simple instructions to proceed:

1. Fork Github repository
2. Follow [C style & coding rules](https://github.com/MaJerle/c-code-style) and use `clang-format` to format the code
3. Create a pull request to `develop` branch with new features or bug fixes

Alternatively you may:

1. Report a bug
2. Ask for a feature request