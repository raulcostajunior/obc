# obc 
### An Oberon-07 to LLVM IR compiler

A compiler for the [![Oberon-07](https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf)] programming language targeting the [![LLVM IR](https://llvm.org/docs/LangRef.html)]. 

The LLVM IR output by obc can then be further compiled and linked as fully native binary modules by using the [![LLVM llc](https://llvm.org/docs/CommandGuide/llc.html)] compiler and the [![LLVM lld](https://lld.llvm.org)] linker. 

WebAssembly modules are also a possible output if the [![WebAssembly port of lld](https://lld.llvm.org/WebAssembly.html)] is used. 


[![Build and Test Status](https://github.com/raulcostajunior/obc/actions/workflows/cmake.yml/badge.svg)](https://github.com/raulcostajunior/obc/actions/workflows/cmake.yml)
