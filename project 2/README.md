# Programming Assignment 2
## Certain, but not Death

(Be sure you also do the homework accompanying this project.)

A state legislator is proposing a major reform of the state income tax code, with a provision to incentivize working as a nurse or a teacher. Your job is to write a program that determines how much a taxpayer would pay under the plan.

Your program must accept as input the taxpayer's name, taxable income, occupation, and number of children. The output will tell how much tax that the taxpayer would pay under the plan.

Here is an example of a dialog with the program (user input is shown here in boldface):

	Name: Clara Barton
	Taxable income: 115000
	Occupation: nurse
	Number of children: 5
	---
	Clara Barton would pay $4200.00
According to the plan:

The first $55000 of taxable income is taxed at 4%.
The next $70000 of taxable income is taxed at 7% in general. However, if the taxpayer's occupation is nurse or teacher (so spelled, entirely in lower case), the income in this bracket is taxed at only 5%.
The amount of taxable income that exceeds $125000 is taxed at 9.3%.
If the taxable income is under $125000, the tax is reduced by $200 per child; however, the tax is never allowed to go below zero. if this would cause the tax to be negative, then the tax is zero instead.
As an example, Clara above would incur $2200 of tax liability for the first $55000 of her income, plus $3000 for the next $60000 of income in the second bracket (since she's a nurse), for a total of $5200. That tax is reduced by $200 per child, so she would pay $4200.

Here's another example:

	Name: Rachel Green
	Taxable income: 29544.69
	Occupation: waitress
	Number of children: 0
	---
	Rachel Green would pay $1181.79
For the examples above, if your program's behavior is not exactly as shown, it will probably fail most of our other test cases as well.

You can test your understanding of the tax schedule by experimenting with the tax proposal calculator we found at the legislature's website.

Your program must collect the information for one taxpayer in the manner indicated by the examples, and then write to cout a line with three hyphens only (no spaces or other characters), followed by exactly one line in a format required below. Our grading tool will judge the correctness of your program by examining only the line following the line with three hyphens (and verifying that there are no additional lines). That one line you write must be in one of the following five forms; the text must be identical to what is shown, except that italicized items are replaced as appropriate:

If an empty string was provided for the name:
   You must enter a name
If the taxable income is negative:
   The taxable income must not be negative
If an empty string was provided for the occupation:
   You must enter an occupation
If the number of children is negative:
   The number of children must not be negative
If the input is valid and none of the preceding situations holds:
   name would pay $amount
In the last case, name must be the name the user entered, and amount must be the correct answer, shown as a non-negative number with exactly two digits to the right of the decimal point. The lines you write must not start with any spaces. If you are not a good speller or typist, or if English is not your first language, be especially careful about duplicating the messages exactly. Here are some foolish mistakes that may cause you to get very few points for correctness on this project, no matter how much time you put into it, because the mistake will cause your program to fail most or all of the test cases we run:

Not writing to cout a line with exactly three hyphens in all cases.
Writing any spaces or any other text on the line that is supposed to have three hyphens.
Writing more than one line after the line with three hyphens. Don't, for example, add a gratuitous "Tax plan details subject to change."
Writing lines to cerr instead of cout.
Writing lines like these:
	Mildred Ratched woud pay $4286.90   misspelling
	Mildred Ratched Would pay $4286.90  wrong capitalization
	Mildred Ratched pays $4286.90       wrong text
	 Mildred Ratched would pay $4286.90 extra space at start of line
	Mildred Ratched would pay $ 4286.90 extra space
	Mildred Ratched would pay $4286.90. extra period
	Mildred Ratched would pay $4286.900 extra digit
	Mildred Ratched would pay 4286.90   missing dollar sign
	Jaime Escalante would pay $2763     missing decimal point and digits
Your program must gather the name, the taxable income, the occupation, and the number of children in that order. However, if you detect an error in an item, you do not have to request or get the remaining items if you don't want to; just be sure you write to cout the line of three hyphens, the required message, and nothing more after that. If instead you choose to gather all input first before checking for errors, and you detect more than one error, then after writing the line of three hyphens, write only the error message for the earliest erroneous item.

You will not write any loops in this program. This means that each time you run the program, it handles only one taxpayer. It also means that in the case of bad input, you must not keep prompting the user until you get something acceptable; our grading tool will not recognize that you're doing that.

A string with no characters in it is the empty string. A string with at least one character in it is not the empty string, even if the only characters in it are things like spaces or tabs. Although realistically it would be silly to have a taxpayer name consisting of seventeen spaces and nothing more, treat that as you would any other non-empty string: as a valid name. (Since you don't yet know how to check for that kind of situation anyway, we're not requiring you to.)

The one kind of input error that your program does not have to deal with (because you don't yet know enough to know how to do this nicely) is the case of not finding a number in the input where a number is expected. We promise that our grading tool will not, for example, supply the text not enough when your program requests the income or the number of children. We also promise that our grading tool will supply an integer for the number of children, not a number like 2.5.

The correctness of your program must not depend on undefined program behavior. Your program could not, for example, assume anything about n's value at the point indicated:

	int main()
	{
	    int n;
	    int m = 42 * n;  // n's value is undefined
	    …
Your program must build successfully under both g31 and either Visual C++ or clang++.

What you will turn in for this assignment is a zip file containing these three files and nothing more:

A text file named tax.cpp that contains the source code for your C++ program. Your source code should have helpful comments that tell the purpose of the major program segments and explain any tricky code.
A file named report.docx or report.doc (in Microsoft Word format) or report.txt (an ordinary text file) that contains:
A brief description of notable obstacles you overcame. (In Project 1, for example, some people had the problem of figuring out how to work with more than one version of a program in Visual C++.)
A list of the test data that could be used to thoroughly test your program, along with the reason for each test. You don't have to include the results of the tests, but you must note which test cases your program does not handle correctly. (This could happen if you didn't have time to write a complete solution, or if you ran out of time while still debugging a supposedly complete solution.) For Project 1, for example, such a list, had it been required, might have started off like this:
More voters than the total for Gavin and Brian (1000, 413, 382 )
Fewer voters than the total for Gavin and Brian (500, 413, 382 )
Zero total voters (0, 100, 100)
Data giving a non-integer percentage (1000, 413, 382)
More voters for Gavin than Brian (1000, 413, 382)
Equal number of voters for Gavin and Brian (1000, 500, 500)
…
A file named hw.docx or hw.doc (in Microsoft Word format) or hw.txt (an ordinary text file) that contains your solution to the homework accompanying Project 2.
By October 12, there will be links on the class webpage that will enable you to turn in your zip file electronically. Turn in the file by the due time above. Give yourself enough time to be sure you can turn something in, because we will not accept excuses like "My computer died the afternoon of the day the assignment was due." There's a lot to be said for turning in a preliminary version of your program, report, and homework early (You can always overwrite it with a later submission). That way you have something submitted in case there's a problem later. Notice that most of the test data portion of your report can be written from the requirements in this specification, before you even start designing your program.

The writeup Some Things about Strings tells you what you need to know about strings for this project.

As you develop your program, periodically try it out under another compiler (g31 on cs31.seas.ucla.edu if you're doing your primary development using Visual C++ or Xcode). Sometimes one compiler will warn you about something that another is silent about, so you may be able to find and fix more errors sooner. If running your program under both environments with the same input gives you different results, your program is probably relying on undefined behavior (such as using the value of an uninitialized variable), which we prohibit.
