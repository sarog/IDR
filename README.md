# IDR - Interactive Delphi Reconstructor

IDR is originally written by [crypto](https://github.com/crypto2011/IDR).

This (experimental) fork is based on commit 03f38fc0 with C++Builder 10.2 support code taken from [PR #38](https://github.com/crypto2011/IDR/pull/38).

Please note that the Process Dumper tool has not been tested.

## Overview

IDR is a decompiler of executable files (EXE) and dynamic libraries (DLL) that are written in Delphi and executed in a Windows environment.
It is mainly targeted for use by the anti-malware and forensics industry. It can also help developers recover lost source code of programs.
Since the analyzed file is not loaded to memory, this makes IDR a safe tool for forensic investigations and malware analysis.

Currently, IDR can decompile Delphi programs written in Delphi 2 to Delphi XE4. Versions past XE4 can be loaded with limited support.

## Building IDR

C++ Builder 10.2.3 (Tokyo) or higher is required. You can also use JetBrains CLion with a custom compiler.

## Roadmap

- Ensure all existing features work correctly
- Cleanup/format existing code
- Add Unicode support
- Add support for newer Delphi versions
- Add 64-bit support
