# Finding a pattern in a text
Project made for Algorithm and Data Structers classes.
Main goal of this project is to implement three algorithms intended to search for pattern in a text: 
  * Knuth–Morris–Pratt algorithm
  * Karp-Rabin algorithm
  * Boyet Moore algorithm
Results of search are positions of pattern occurance in a text.
First I had did it in a way, in which text and pattern had have to be given inside code in a classic C string form, but later I decided to reshape code in order that program
could be execute from command line, where user might to give path to file with text, pattern and also choose algorithm for searching. In that program is more useful, because 
now text can have any length. How text is read from file is descirbed below.

# Reading text from file
Text in program is stored in a dynamicly allocated array.
Whole text is not read at once, but in parts in a loop, until EOF is reached. User can choose the number of characters read in a one iteration, it can be 1000, 10 000 
or 100 000. If text is shorter, program automaticly adjust length of array. Longer texts are readed in a loop by parts, if in last iteration remaining characters
can be fit in a smaller array, it is also adjusted.


# Command line arguments
Arguments in a command line should come in the order :
  * path to the file containing text
  * number of character read at once:
    * 'S' - 1000
    * 'L' - 10 000
    *  'X' - 100 000
  * path to the file containing pattern
  * selected algorithm
    * "KMP"	- Knuth–Morris–Pratt 
    * "KR"	- Karp-Rabin
    * "BM"	- Boyet Moore (UNFINISHED)
