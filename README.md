brainduck
=========

brainduck is a small Brainfuck interpreter with shebang support.

* Memory cells are 8 bit allowing you to store 0 through 255 inclusive
* Incrementing 255 and decrementing 0 sets value to 0 and 255 accordingly
* 30000 memory cells in total, accessing out of bounds cells yields error
* `,` at EOF sets current cell to 0
* Shebang line will be stripped from program code.
