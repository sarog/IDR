# IDR - Interactive Delphi Reconstructor

IDR is originally written by [crypto](https://github.com/crypto2011/IDR).

This **experimental** fork is based on the most [recent IDR commit](https://github.com/crypto2011/IDR/commit/03f38fc0b2e5b972c644e0c80a24872c447aa5b7) with C++Builder 10.2 support code taken from [PR #38](https://github.com/crypto2011/IDR/pull/38).

Please note:
- There are bugs in this fork, namely improper handling of ANSI/Unicode conversions (they are being resolved).
- Many features have not been extensively tested, such as the Process Dumper tool.

## Overview

IDR is a decompiler of executable files (EXE) and dynamic libraries (DLL) that are written in Delphi and executed in a Windows environment.
It is mainly targeted for use by the anti-malware and forensics industry. It can also help developers recover lost source code of programs.
Since the analyzed file is not loaded to memory, this makes IDR a safe tool for forensic investigations and malware analysis.

Currently, IDR can decompile Delphi programs written in Delphi 2 to Delphi XE4. Versions past XE4 can be loaded with limited support.

## Building IDR

C++ Builder 12.1 (Athens) or higher is required. The [Community Edition](https://www.embarcadero.com/products/cbuilder/starter) by Embarcadero can be used to build this fork. You can also use JetBrains CLion with a custom compiler.

For a successful build, ensure compiler optimizations are disabled.

## Roadmap

- Ensure all existing features work correctly
- Cleanup/format existing code
- Add Unicode support
- Add support for newer Delphi versions
- Add 64-bit support
- Add Dark Mode support
- Improve documentation
- Expand KB files by improving [KBBuilder](https://github.com/sarog/kbbuilder)
