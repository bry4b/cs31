# Programming Assignment 5
## What Word'll You Guess Next?

### Part 1

Go through the following sections of the class zyBook, doing the Participation Activities and Challenge Activities. We will be looking at whether you have ever successfully completed them; it does not matter how many attempts you make before a successful completion (or how many attempts you make after a successful completion if you want to experiment).

6.6 through 6.9
7.2 and 7.3 (Part 2 does not depend on these)
8.1 and 8.2 (Part 2 does not depend on these)

### Part 2

You have been hired to write a program that plays a word guessing game similar to the one from which Wordle was derived. Here's how one round of the game works: The computer picks a hidden word of four to six letters and tells the player how many letters are in the word. The player tries to determine the hidden word by presenting the computer with a series of probe words. Each probe word is a four to six letter word. If the probe word is the hidden word, the player wins. Otherwise, the computer responds to the probe word with two integers: the number of golds and the number of silvers. Golds and silvers are pairings between a letter in the probe word and the same letter in the hidden word:

A gold is a pairing of a letter in the probe word and the same letter in the hidden word in the same position. For example, if the hidden word is EGRET and the probe word is AGATE, there's one gold: The Gs in both words are in the same position, the second letter.

A silver is a pairing between a letter in the probe word and the same letter in the hidden word, but not in the same position as in the probe word, provided that neither of the two letters are involved in a gold or another silver. For example, if the hidden and probe words are EGRET and AGATE, the Ts form a silver, since we can pair them up but they're not in the same position, since one is the fifth letter of EGRET and the other is the fourth letter of AGATE. The E in AGATE and, say, the first E in EGRET can be paired up to form another silver; the second E in EGRET would then not be part of a silver — we can't pair it up with the E in AGATE because that E is already paired up with the first E in EGRET. If instead we paired the E in AGATE with the second E in EGRET, then the first E in EGRET would have to remain unpaired.

If a letter at a particular position in a word could be considered part of a gold or part of a silver, it must be treated as part of a gold. For example, if the hidden and probe words were EGRET and VIXEN, the E in VIXEN must be paired up with the second E in EGRET to form a gold; that takes priority over pairing it with the first E in EGRET to form a silver.

The player's score for each round is the number of probe words needed to determine the correct word (counting the probe word that matched the hidden word).
As an example, suppose the hidden word is EGRET. Here are some examples of the gold and silver counts for various probe words:


        EGRET        EGRET        EGRET        EGRET        EGRET
        GOOSE        RAGE         SIREN        EERIE        EERIE
        s   s        s sg           gg         gsg     or   g g s
      0 g / 2 s    1 g / 2 s    2 g / 0 s    2 g / 1 s    2 g / 1 s
 
        EGRET        EGRET        EGRET        EGRET
        GREET        OKAPI        REGRET  or   REGRET
        sssgg                     sss ss        sssss
      2 g / 3 s    0 g / 0 s    0 g / 5 s    0 g / 5 s
Your program must ask the player how many rounds to play, and then play that many rounds of the game. After each round, the program must display some statistics about how well the player has played the rounds so far: the average score, the minimum score, and the maximum score.

Here is an example of how the program must interact with the player (player input is in boldface):

	How many rounds do you want to play? 3

	Round 1
	The hidden word is 5 letters long.
	Probe word: assert
	Golds: 1, Silvers: 2
	Probe word: xyzzy
	I don't know that word.
	Probe word: bred
	Golds: 0, Silvers: 2
	Probe word: mucus
	Golds: 0, Silvers: 0
	Probe word: never
	Golds: 2, Silvers: 2
	Probe word: enter
	Golds: 1, Silvers: 2
	Probe word: river
	Golds: 3, Silvers: 0
	Probe word: raven
	You got it in 7 tries.
	Average: 7.00, minimum: 7, maximum: 7

	Round 2
	The hidden word is 5 letters long.
	Probe word: eerie
	Golds: 2, Silvers: 1
	Probe word: rage
	Golds: 1, Silvers: 2
	Probe word: greet
	Golds: 2, Silvers: 3
	Probe word: egret
	You got it in 4 tries.
	Average: 5.50, minimum: 4, maximum: 7

	Round 3
	The hidden word is 4 letters long.
	Probe word: monkey
	Golds: 0, Silvers: 0
	Probe word: puma
	Golds: 0, Silvers: 0
	Probe word: Hello
	Your probe word must be a word of 4 to 6 lower case letters.
	Probe word: what?
	Your probe word must be a word of 4 to 6 lower case letters.
	Probe word: wrap-up
	Your probe word must be a word of 4 to 6 lower case letters.
	Probe word: stop it
	Your probe word must be a word of 4 to 6 lower case letters.
	Probe word: sigh
	You got it in 3 tries.
	Average: 4.67, minimum: 3, maximum: 7
Notice that unknown words and probe strings that don't consist of exactly 4 to 6 lower case letters don't count toward the number of tries for a round.

You can assume the player will always enter an integer for the number of rounds (since you haven't learned a clean way to check that yet). If the number of rounds entered is not positive, write the message

	The number of rounds must be positive.
(not preceded by an empty line) to cout and terminate the program immediately.

The program will be divided across three files: game.cpp, which you will write; utilities.h, which we have written and which you must not change; and utilities.cpp, which we have written and you must not change. You will turn in only game.cpp; when we test your program, our test framework will supply utilities.h and our own special testing version of utilities.cpp.

In order for us to thoroughly test your program, it must have at least the following components:

In game.cpp, a main routine that declares an array of C strings. This array exists to hold the list of words from which the hidden word will be selected. The response to a probe word will be the number of golds and silvers only if the probe word is in this array. (From the example transcript above, we deduce that "xyzzy" is not in the array.) The declared number of C strings in the array must be at least 9000. (You can declare it to be larger if you like, and you don't have to use every element.)

Each element of the array must be capable of holding a C string of length up to 6 letters (thus 7 characters counting the zero byte). So a declaration such as char wordList[9000][7]; is fine, although something like char wordList[MAXWORDS][MAXWORDLEN+1];, with the constants suitably defined, would be stylistically better.

Along with the array, your main routine must declare an int that will contain the actual number of words in the array (i.e., elements 0 through one less than that number are the elements that contain the C strings of interest). The number may well be smaller than the declared size of the array, because for test purposes you may not want to fill the entire array.

Before prompting the player for the number of rounds to play, your main routine must call getWords (see below) to fill the array. The only valid words in the game will be those that getWords puts into this array.

If the player's score for a round is not 1, the message written to cout reporting the score must be

	You got it in n tries.
where n is the score. If the score is 1, the message must be

	You got it in 1 try.
In utilities.cpp, a function named getWords with the following prototype:

	int getWords(char words[][7], int maxWords, const char wordfilename[]);
(Instead of 7, our utilities.cpp actually says MAXWORDLEN+1, where MAXWORDLEN is declared to be the constant 6 in utilities.h.) This function puts words into the words array and returns the number of words put into the array. The array must be able to hold at least maxWords words. The file named by the third argument is the plain text file that contains the words, one per line, that will be put into the array.

You must call getWords exactly once, before you start playing any of the rounds of the game. If your main routine declares wordList to be an array of 10000 C strings and nWords to be an int, you'll probably invoke this function like this:

	const char WORDFILENAME[] = "the path for the word file";
	...
	int nWords = getWords(wordList, 10000, WORDFILENAME);
You may use this 7265-word file if you want a challenging game. Here's how you'd specify the path to the word file for various systems:

Windows: Provide a path for the filename, but use / in the string instead of the \ that Windows paths use, e.g. "C:CS31/P5/mywordfile.txt".
Mac: It's probably easiest to use the complete pathname to the words file, e.g. "/Users/yourUsername/words.txt" or "/Users/yourUsername/CS31/P5/words.txt".
Linux: If you put the words.txt file in the same directory as your .cpp file, you can use "words.txt" as the file name string.
We have given you an implementation of getWords. (Don't worry if you don't understand every part of the implementation.) It fills the array with the four-to-six-letter words found in the file named as its third argument. To do simple testing, you can temporarily change the implementation of getWords to something like this that ignores the file name and hard codes a small number of words to be put in the array:

	int getWords(char words[][7], int maxWords, const char wordfilename[])
	{
	    if (maxWords < 2)
	        return 0;
	    strcpy(words[0], "eagle");
	    strcpy(words[1], "goose");
	    return 2;
	}
Whatever implementation of getWords you use, each C string that it puts into the array must consist of four to six lower case letters; the C strings must not contain any characters that aren't lower case letters. If you have made a temporary change to getWords for test purposes, be sure to restore utilities.cpp back to its original state and verify that your program still runs correctly.

The getWords function must return an int no greater than maxWords. If it returns a value less than 1, your main routine must write

	No words were loaded, so I can't play the game.
to cout and terminate the program immediately, without asking the player for the number of rounds to play, etc.

When we test your program, we will replace utilities.cpp (and thus any changed implementation of getWords you might have made) with our own special testing implementation that will ignore the third argument and fill the array with the test words we want to use.

If getWords returns a value in the range from 1 to maxWords inclusive, your program must write no output to cout other than what is required by this spec. If you want to print things out for debugging purposes, write to cerr instead of cout. When we test your program, we will cause everything written to cerr to be discarded instead — we will never see that output, so you may leave those debugging output statements in your program if you wish.

In game.cpp, a function named playOneRound with the following prototype:
	int playOneRound(const char words[][7], int nWords, int wordnum);
(Again, instead of 7, you can use something like MAXWORDLEN+1.) Using words[wordnum] as the hidden word, this function plays one round of the game, reading from cin and writing to cout. It returns the score for that round. In the transcript above, for round 1, for example, this function is responsible for this much of the round 1 output, no more, no less:

        Probe word: assert
        Golds: 1, Silvers: 2
        Probe word: xyzzy
        I don't know that word.
        Probe word: bred
        Golds: 0, Silvers: 2
        Probe word: mucus
        Golds: 0, Silvers: 0
        Probe word: never
        Golds: 2, Silvers: 2
        Probe word: enter
        Golds: 1, Silvers: 2
        Probe word: river
        Golds: 3, Silvers: 0
        Probe word: raven
Your program must call this function to play each round of the game. Notice that this function does not select a random number and does not tell the user the length of the hidden word; the caller of this function does, and passes the random number as the third argument. Notice also that this function does not write the message about the player successfully determining the hidden word; the caller of this function does. If you do not observe these requirements, your program will fail most test cases.

The parameter nWords represents the number of words in the array; if it is not positive, or if wordnum is less than 0 or greater than or equal to nWords, then playOneRound must return −1 without writing anything to cout.

If for a probe word the player enters a string that does not contain four to six lower case letters or contains any character that is not a lower case letter, the response written to cout must be

	Your probe word must be a word of 4 to 6 lower case letters.
If the player enters a string consisting of exactly four to six lower case letters, but that string is not one of the words in the array of valid words, then the response written to cout must be

	I don't know that word.
To make things interesting, your program must pick hidden words at random using the function randInt, contained in utilities.cpp:

	int randInt(int min, int max);
Every call to randInt returns a random integer between min and max, inclusive. If you use it to generate a random position in an array of n interesting items, you should invoke it as randInt(0, n-1), not randInt(0, n), since the latter might return n, which is not a valid position in an n-element array.

Your program must not use any std::string objects (C++ strings); you must use C strings. Your program must not use any standard library containers (such as vectors, maps, etc.).

You may assume (i.e., we promise when testing your program) that any line entered in response to the probe word prompt will contain fewer than 100 characters (not counting the newline at the end). You may also assume that for any platform on which we run your program, 'z'-'a' == 25 && 'Z'-'A' == 25 is true; in other words, that the character encodings for lower case letters are consecutive, as are those for upper case letters.

Your program must not use any global variables whose values may change during execution. Global constants are all right; our utilities.h declares const int MINWORDLEN = 4; globally, for example, and it's fine for your game.cpp to declare your own additional global constants. The reason for this restriction is that part of our testing will involve replacing your playOneRound function with ours to test some aspects of your main function, or replacing your main with ours to test aspects of your playOneRound. For this reason, you must not use any non-const global variables to communicate between these functions, because our versions won't know about them; all communication between these functions must be through the parameters (for main to tell playOneRound the words, number of words, and hidden word number for a round), and the return value (for playOneRound to tell main the score for that round). Global constants are OK because no function can change their value in order to use them to pass information to another function.

Standard C++ requires that the number of elements in an array you declare to be known at compile time. Since the g31 command on cs31.seas.ucla.edu enforces that requirement, and your program must run under that compiler, you must meet that requirement. Thus, you must not do something like this:

        void somefunction(char someword[])
        {
            char a[strlen(someword)+1]; // Error! strlen(someword) not known at compile time

### What to turn in

You won't turn anything in through the CS 31 web site for Part 1; the zyBook system notes your successful completion of the PAs and CAs. For Part 2, you will turn in a zip file containing these two files and nothing more:

A text file named game.cpp that contains main, playOneRound, and other functions you choose to write that they might call. (You must not put implementations of getWords or randInt in game.cpp; they belong in utilities.cpp, which you are not submitting.) Your source code should have helpful comments that tell the purpose of the major program segments and explain any tricky code.

A file named report.docx or report.doc (in Microsoft Word format), or report.txt (an ordinary text file) that contains:
A brief description of notable obstacles you overcame.
A description of the design of your program. You should use pseudocode in this description where it clarifies the presentation. Document the design of your main routine, playOneRound, and any other functions you write. Do not document the getWords or randInt functions.
Your report does not need to describe the data you might use to test this program.
By November 13, there will be links on the class webpage that will enable you to turn in your zip file electronically. Turn in the file by the due time above.

### Getting started

This project is having you do some things we haven't done before: Run a program split across multiple source files, and run a program that reads from a file. Before you delve into the details of writing the code to play the game, you would be wise to ensure that you can do these new things correctly.
First, follow these instructions to set up a project with the three files that will make up the program. Building and running the program verifies that you can correctly deal with a multi-file program.

Then, place this two-word file at a location of your choosing. On a Windows or Mac system, make sure you know the complete path name to the file (e.g., C:CS31\P5\smallwords.txt on a Windows system, or /Users/yourUsername/CS31/P5/smallwords.txt on a Mac). Build and run the project consisting of our utilities.cpp, our utilities.h, and a game.cpp that has this text:

	#include "utilities.h"
	#include <iostream>
	using namespace std;

	const char WORDFILENAME[] = "the path to your two-word file";
	    // Windows users:  Use / instead of \ for pathname separators.

	int main()
	{
	    char w[9000][7];
	    int n = getWords(w, 9000, WORDFILENAME);
	    if (n == 2)
	    {
	        cout << "getWords successfully found the file and read its two words." << endl;
	        cout << "You're ready to start working on Project 5." << endl;
	    }
	    else if (n == -1)
	        cout << "The path to your file of words is probably incorrect" << endl;
	    else
            cout << "getWords found the file, but loaded " << n  << " words instead of 2" << endl;
	}
Building and running the program verifies that you can wite a program that successfully finds and opens the file it wants to read from.

### Running the project using g31 with Linux

To take a three-file project you've developed with Visual C++ or Xcode and run it with g31 under Linux, follow these steps.

Follow steps 1 through 4 of the g++ with Linux writeup to transfer the three files (utilities.h, utilities.cpp, and game.cpp) to the Linux server to log in to cs31.seas.ucla.edu.

Create a new directory for this project; let's call it proj5:

	mkdir proj5
Move the files into this directory:

	mv utilities.* game.cpp proj5
Make proj5 the current directory (i.e., the default directory for now in which files will be found or created):

	cd proj5
Verify that the expected three files are present by listing the contents of the current directory:

	ls
Copy over the two-word word file into the current directory:

	curl -s -L http://cs.ucla.edu/classes/fall22/cs31/Projects/5/smallwords.txt > smallwords.txt
Use the Nano editor to change the initializer for WORDFILENAME to "smallwords.txt".

	nano game.cpp
You can navigate with the arrow keys. The bottom two lines of the display show you some commands you can type. For example, control-O (indicated in the bottom display as ^O) saves any changes you make to the file, and control-X exits the editor.

Build an executable file from the source files. If we would like the executable file to be named testgame, we'd say

	g31 -o testgame *.cpp
The *.cpp saves us typing individual file names by matching all the files whose names end in .cpp. (Notice that we do not list the .h file.).

To execute the program testgame that you built, you'd just say

	./testgame
Later on, when you're ready use g31 to test the program you've been developing with Visual C++ or Xcode, transfer that game.cpp to the Linux server, move it into proj5 (mv game.cpp proj5), make proj5 the current directory, and if you want to use our 7265-word file, do this:

	curl -s -L http://cs.ucla.edu/classes/fall22/cs31/Projects/5/words.txt > words.txt
Use the Nano editor to change the initializer for WORDFILENAME to "words.txt". Build and run the program as in the last two steps above.

### Notes for Visual C++ users

Microsoft made a controversial decision to issue by default a warning in some cases when your code uses certain functions from the standard C and C++ libraries (e.g., strcpy). These warnings call those functions unsafe and recommend using different functions in their place; those other functions, though, are not Standard C++ function, so will cause a compilation failure when you try to build your program under g31 or clang++. Therefore, for this class, we want to use functions like strcpy without getting that warning from Visual C++; to eliminate the warning messages, put the following line in your program before any of your #includes:

	#define _CRT_SECURE_NO_WARNINGS
It is OK and harmless to leave that line in when you build your program using g31 or clang++ and when you turn it in.

If the compiler issues a warning C6262: Function uses 'NNNNN' bytes of stack: exceeds /analyze:stacksize '16384'. Consider moving some data to heap., where NNNNN is some number, you can eliminate that harmless warning by adding this line at the top of your program:

	#pragma warning(disable : 6262)
It is OK and harmless to leave that line in when you build your program using g31 or clang++ and when you turn it in, even if you get a warning about the pragma being ignored.

Alternatively, in Visual Studio, select Project / yourProjectName properties, then select Configuration Properties / Code Analysis / General, and then in Code Analysis's stacksize, modify 16384 to, say, 100000.

If your program dies under Visual C++ with a dialog box appearing saying "Debug Assertion Failed! ... File: ...\src\isctype.c ... expression: (unsigned)(c+1)<=256", then you called one of the functions defined by <cctype>, such as isalpha or tolower, with a character whose encoding is outside the range of 0 through 127. Since all the normal characters you would use (space, letters, punctuation, '\0', etc.) fall inside that range, you're probably passing an uninitialized character to the function. Perhaps you're examining a character past the '\0' marking the end of a C string, or perhaps you built what you thought was a C string but forgot to end it with a '\0'.
