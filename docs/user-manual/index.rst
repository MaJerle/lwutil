.. _um:

User manual
===========

LwUTIL is a lightweight utility library that collects the small helper macros and functions
that get hand-written again and again in day-to-day C/C++ embedded development: value
comparisons, bit manipulation, endian-aware serialization and a few common algorithms.

It has no dependencies besides the standard C library, performs no dynamic memory allocation,
and every helper can be used independently - take only what you need.

What it can do
^^^^^^^^^^^^^^

* Compute the number of elements in a statically allocated array
* Get the minimum or maximum of two values, constrain a value to a range, or map it between two ranges
* Get the absolute value of a signed input
* Silence "unused variable" compiler warnings
* Dereference and assign through a pointer, but only when it is not ``NULL``
* Set, clear, toggle or check bits against a bit mask
* Store and load ``16-bit`` and ``32-bit`` values to and from a byte buffer, in little- or big-endian format

  * Pointer-advancing extended variants are available for sequential (de)serialization

* Convert ``8/16/32-bit`` values to their hexadecimal ASCII representation
* Encode and decode ``32-bit`` values in variable-length integer (``varint``) format
* Check whether a time period has elapsed against a rolling time reference, useful for non-blocking periodic tasks
* Calculate a rolling (sliding-window) linear regression slope over fixed-step sample data
* Assert an expression at compile time

Check :ref:`api_reference` for the detailed list of all supported functions and macros.