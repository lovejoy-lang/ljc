# `ljc`
Lovejoy compiler for the ARMv8-A AArch64 instruction set.

## Pipeline
- Parse with incremental lexing.
- Macro expansion onto parse tree, spawn new compiler instance for ACE*.
- Generate HIR, similar to AST, closest to source.
- Type checking and semantic analysis.
- Reduce to MIR, mid-level, simplistic closer to machine-code representation.
- Optimisations (TCO, LICM, etc.)
- Peephole optimisations.
- Final translation into ARM machine code.

*[Include a graphic here representing pipeline]*

## Features
*Incomplete list of features, most yet to be implemented.*
- Minimal, low-level, simple, C inspired.
- ACE *(Arbitrary Compile-time Execution)* (May require implementing a viritual machine, which could also be used for a REPL).
- Minimal grammar, very few keywords, instead very strong macro system.
- Patter matching.
- Strong type system, with inference, (exact system to be decided).
- Fat pointers.
- Closures.
- Overloading.
- First class types and functions.
- Generics and higher kinded types.
- Incremental compilation. (?)
- Safety features (e.g. nullable v. non-nullable pointers).
- Integrated build system, build-configuaration to be written *in Lovejoy itself*.  i.e. no scripts, Makefiles, JSON, TOML, YAML, etc.

## Future Goals
- Become self-hosting.
