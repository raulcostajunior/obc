# obc 
### An Oberon-07 to LLVM IR compiler

A compiler for the ![Oberon-07](https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf) programming language targeting the ![LLVM IR](https://llvm.org/docs/LangRef.html). 

LLVM IR code emitted by obc should then be compiled to native binary modules for a number of different architectures by using the [![LLVM llc](https://llvm.org/docs/CommandGuide/llc.html)] compiler and the [![LLVM lld](https://lld.llvm.org)] linker. Possible target architectures of llc + lld include AArc64, ARM, PowerPC. RISC-V, x86_32, and x86_64.

WebAssembly modules are also a possible target if the [![WebAssembly port of lld](https://lld.llvm.org/WebAssembly.html)] is used. 



[![Build and Test Status](https://github.com/raulcostajunior/obc/actions/workflows/cmake.yml/badge.svg)](https://github.com/raulcostajunior/obc/actions/workflows/cmake.yml)
