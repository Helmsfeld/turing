Crude Turing Machine Emulator
=============================

Compilation
-----------

A simple

	make

should do.

Running the examples
--------------------

Type

	cat examples/Turing1.in - | ./turing

to get startet.


Format of the input files
-------------------------

The input files should have the following format, see the example files.

	<Number of states>
	<Alphabet>
	[Optional: One or more lines of comments, starting with '!']
	(<Symbol read> ( <New state> ( <Symbol to be written>  | L | R  ) )+ )+
	<Initial tape>
	<Initial position of read/write head, 0 is leftmost position>

Remarks
-------

The parser for the turing machine description file is not very robust, therefore some remarks are in order.

* The transition table is by current design not very readable, i know, sorry, that makes it hard to write a turing machine description.
* Comments can only occur directly before the transition table.
* Internally, the states are numbered from `0` to `<number of states>-1`.
* There must be exactly one entry (column) for each state.
* The number of the halting state is `<number of states>`
* There must be exactly one space to seperate each entry in the transition table and no trailing spaces at the end of a line!
* An empty entry is assumed to be `#`, a symbol that should occur in any alphabet.

There are probably some other implicit assumptions i forgot to mention (no empty lines, or so), if you find any bugs, 
you are welcome to either tell me or to fix them.
