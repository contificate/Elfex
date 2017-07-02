# Elfex
Tiny attempt at a capstone-based disassembler

A simple, very early, benchtest of an ARM disassembler abstraction.
The underlying efx::Disassembler parent handles all of the actual disassembly (through capstone).
It's up to derived classes such as efx::ARMDisassembler to create models from instruction callbacks.
I'm considering creating a generic model class and adding opt-in support for potential features (basic block building, graphed significance, rop gadgets, etc.). I also want to make more instruction callbacks for specific types of instructions; will have to do more research to find generic stuff across ISAs that would be worth implementing.

![Alt text](https://u.teknik.io/OlOB2.png "Simple ARM benchtest without models")
