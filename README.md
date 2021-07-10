## Introduction
csound-festival provides a simple way of generating speech using the TTS engine Festival. It runs an init time and generates a ftable containing the synthesised speech which can then be used elsewhere. Currently only pitch is a supported parameter but there is certainly scope for adding further parameters.
The opcode has only been tested on Linux as of writing.


## Requirements
 - Csound development libraries
 - [Festival](https://www.cstr.ed.ac.uk/projects/festival/) which also depends on [The Edinburgh Speech Tools Library](https://www.cstr.ed.ac.uk/projects/speech_tools/), both probably handled by your relevant package manager.
 - Cmake 3.8 or later


## Building
Create a build directory at the top of the source tree, execute *cmake ..*, *make* and optionally *make install* as root. If the latter is not used/possible then the resulting module can be used with the *--opcode-lib* flag in Csound. eg:
	
	cd csound-festival
	mkdir build && cd build
	cmake ..
	make && sudo make install


## Opcode overview
### ftts
*ifn ftts Stext, ipitch*

Generate speech

 - ifn : the function table number as created containing the generated speech
 - Stext : the text string to synthesise
 - ipitch : the fundamental frequency for synthesis in CPS.

