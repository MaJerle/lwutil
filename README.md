# C language utility library

Set of different functions and macros usually used in the various applications.

## Features

- Written in C (C11), compatible with `stdint.h` data types
- Support for minimal and maximal values, value constrain and range mapping
- Support for absolute values
- Support for unused macros
- Support for conditional pointer dereference and value assignment
- Support for storing and loading data from array in little- or big- endian formats, including pointer-advancing extended variants
- Support for retrieving number of elements for statically allocated array
- Support for bitwise operations to set, clear, check or toggle bits
- Support for converting `8/16/32-bit` values to their hexadecimal ASCII representation
- Support for encoding and decoding `32-bit` values in variable-length integer (varint) format
- Support for time-elapsed checks against a rolling time reference, useful for periodic tasks
- Support for rolling (sliding-window) linear regression slope calculation over fixed-step data
- Compilation-time assert feature
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