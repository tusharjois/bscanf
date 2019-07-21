# bscanf

`bscanf` is a standalone `sscanf` implementation with bounds checking, written
in ANSI C (C89). It aims to be a faithful recreation of `sscanf`, but includes
mandatory bounds checking on character sequence-esque types (read on for more
information).

It was designed primarily because Intel SGX does not support the `<stdio.h>`
header file [1], as SGX limits I/O inside of an enclave. However, `sscanf`
functionality is helpful in parsing data and has no dependencies on I/O. Hence,
`bscanf` was born.

## Usage

To use `bscanf`, simply add copy the source file `bscanf.c` and include the
header file `bscanf.h` in your project. It has been tested on Linux (GCC) and
macOS (Clang) under `--std=c89`, but it (should) work anywhere you have an ANSI
C environment. Open an issue if you are having trouble.

For the most part, `bscanf` works like the C89 version of `sscanf` does. It
works based on "conversion specifications": the `printf` format strings you know
and love. It accepts the length modifiers `h` (for type `short`) and `l` (for
type `long`). You can also suppress assignment like in `sscanf` with `*`. The
return value of a call to `bscanf` is the number of variables it sets. Refer to
online documentation [2] for more information on these specifiers.

However, there is one major caveat to `bscanf`: **all character sequence types
must have a maximum field width**. These look like `"%*3c"` and `"%8s"`.
`bscanf` does bounds checking on `c` and `s` types to make sure it is not
pushing more data than the maximum allowed width into a buffer. Note that the
specifier for `s` (string) types must hold room for the width + 1 (for the `\0`
terminator), just like in `sscanf`; a specifier `%8s` would need to be 9 bytes
long. To load a single character, use `%1c`.

If there is no width set for these specifiers, `bscanf` terminates the parsing
of the buffer early. It also stops parsing partway when it encounters "undefined
behavior" (such as `%*n` as a specifier). Make sure you check the result of a
call to `bscanf` before using variables!

Please refer to the tests in `test_bscanf.c` for example usage. You can make the
tests with the included `Makefile`.

## Unsupported Specifiers

The following specifiers are _not_ supported at this time. I hope to implement
these in a future version of `bscanf`.

- `%[`: match characters from a set.
- `%ls, %lc`: match characters/strings of `wchar_t`.
- `%p`: match a pointer.
- `%a,%A`: match a float in C99's hexadecimal exponent notation [3].

Additionally, `bscanf` does _not_ implement the length modifiers `hh`, `ll`,
`j`, `z`, `t` or `L`.

[1]: <https://download.01.org/intel-sgx/linux-1.9/docs/Intel_SGX_SDK_Developer_Reference_Linux_1.9_Open_Source.pdf>
[2]: <https://en.cppreference.com/w/c/io/fscanf>
[3]: <https://en.wikipedia.org/wiki/Hexadecimal#Hexadecimal_exponential_notation>
