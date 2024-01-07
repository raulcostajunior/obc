# obc

[![Build and Test Status](https://github.com/raulcostajunior/obc/actions/workflows/cmake.yml/badge.svg)](https://github.com/raulcostajunior/obc/actions/workflows/cmake.yml)

An [Oberon-07](https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf)
to [LLVM IR](https://llvm.org/docs/LangRef.html) compiler.

LLVM IR code emitted by **obc** can then be compiled to native binary modules for a number of different architectures by
using the [LLVM llc](https://llvm.org/docs/CommandGuide/llc.html) compiler and the [LLVM lld](https://lld.llvm.org)
linker. Possible target architectures for LLVM's backend stack include AArch64, ARM, PowerPC, RISC-V, x86_32, and
x86_64.

WebAssembly modules are also a possible target when the [WebAssembly port of lld](https://lld.llvm.org/WebAssembly.html)
is used.

> Oberon-07 is one of the amazing programming languages created
> by [Prof. Niklaus Wirth](https://people.inf.ethz.ch/wirth/), who unfortunately, passed away on
> January 1st. 2024. A summary of the projects of Prof. Niklaus Wirth between 1962 and 1999 can be
> found [here](https://people.inf.ethz.ch/wirth/projects.html).

## On Oberon

#### Oberon+

**obc** is only my first take on writing a compiler and most probably nothing too useful will come out of it.

To get really serious about Oberon and its future, take a GOOD look at [Oberon+](https://oberon-lang.github.io/).

#### Oberon - the Uranus' moon, not the language - in the news:

> "This has led NASA scientists to conclude that four of Uranus’ largest moons—Ariel, Umbriel, Titania, and **Oberon**
> —probably contain water oceans below their icy crusts. These oceans are likely dozens of kilometers deep and probably
> fairly salty in being sandwiched between the upper ice and inner rock core. These inner cores are likely producing
> enough heat from radioactive decay to create layers of liquid water, the scientists say. Additionally, chlorides, as
> well as ammonia, are likely abundant in the oceans of the icy giant’s largest moons and may be helping to keep them
> unfrozen."
>
>
from <cite> [ArsTechnica](https://arstechnica.com/science/2023/05/as-many-as-four-moons-around-uranus-may-have-oceans-below-the-surface/). </cite>

## Build

To build **obc**, [CMake](https://cmake.org) version 3.20 or later and [Conan](https://conan.io) are required.

Once CMake and Conan are available on your system, please follow
this [sequence of instructions described on the Conan Documentation site](https://docs.conan.io/2/tutorial/consuming_packages/build_simple_cmake_project.html)
to build obc. 
