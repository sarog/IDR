# IDR - Interactive Delphi Reconstructor

IDR is originally written by [crypto](https://github.com/crypto2011/IDR).

This fork is based on commit [PR #38](https://github.com/crypto2011/IDR/pull/38) merged with the latest IDR release.

## Overview

IDR is a decompiler of executable files (EXE) and dynamic libraries (DLL) that are written in Delphi and executed in a Windows environment.
It is mainly targeted for use by the anti-malware and forensics industry. It can also help developers recover lost source code of programs. 
Since the analyzed file is not loaded to memory, this makes IDR a safe tool for forensic investigations and malware analysis.

Currently, IDR can decompile Delphi programs written in Delphi 2 to Delphi XE4. Versions past XE4 can be loaded with limited support.

## Building IDR

C++ Builder 10.2 (Tokyo) or higher is required. You can also use JetBrains CLion with a custom compiler.

The `Bins` directory is where the Knowledge Base (.bin) files are stored.

## Roadmap

- Ensure all existing features work correctly
- Cleanup/format existing code
- Add Unicode support
- Add support for newer Delphi versions
- Add 64-bit support
