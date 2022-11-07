 //game.cpp

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6262)

#include <iostream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <random>
#include <utility>
#include <cassert>

#include "utilities.h"
using namespace std;

int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum);
bool isValidInput(char input[]);
bool isValidWord(const char wordList[][MAXWORDLEN + 1], int numWords, char input[]);
int countGolds(char target[], char input[]);
int countSilvers(char target[], char input[]);
void removeChar(char input[], int pos);

const char WORDFILENAME[] = "C:/Users/bryan/Documents/school yuck/fall 2022/cs 31/source/p5/p5/words.txt";
const int MAXWORDS = 9000;

int main() 
{
	int numRounds;
	int wordLength;
	int numWords;
	int wordIndex;
	int score;

	double scoreAverage;
	int scoreMin;
	int scoreMax;

	char wordList[MAXWORDS][MAXWORDLEN + 1];

	//create wordList and store number of words
	numWords = getWords(wordList, MAXWORDS, WORDFILENAME);
	cerr << "words loaded: " << numWords << endl;
	if (numWords < 1)
	{
		cout << "No words were loaded, so I can't play the game." << endl;
		return -1;
	}

	assert(playOneRound(wordList, numWords, -1) == -1);

	//user input for rounds to play
	cout << "How many rounds do you want to play? ";
	cin >> numRounds;
	if (numRounds < 0)
	{
		cout << "The number of rounds must be positive." << endl;
		return -2;
	}

	//loop for numRounds
	for (int i = 1; i <= numRounds; i++)
	{
		cout << endl << "Round " << i << endl;

		//generate random word for round
		wordIndex = randInt(0, numWords - 1);
		wordLength = strlen(wordList[wordIndex]);
		cout << "The hidden word is " << wordLength << " letters long." << endl;
		//debug cerr << "hidden word: " << wordList[wordIndex] << endl;

		//stores playOneRound to score
		score = playOneRound(wordList, numWords, wordIndex);

		//print score
		if (score == 1)
			cout << "You got it in 1 try." << endl;
		else
			cout << "You got it in " << score << " tries." << endl;

		//compute average, min, max
		if (i == 1)
		{
			scoreAverage = score;
			scoreMin = score;
			scoreMax = score;
		}
		else
		{
			scoreAverage = (scoreAverage * (i - 1) + score) / i;
			if (score > scoreMax)
				scoreMax = score;
			if (score < scoreMin)
				scoreMin = score;

		}
		
		//print average, min, max
		cout.setf(ios::fixed);
		cout.precision(2);
		cout << "Average: " << scoreAverage << ", minimum: " << scoreMin << ", maximum: " << scoreMax << endl;

	}

}

/*****************************
* int playOneRound
* takes words[wordnum] as hiddenWord, asks for user guesses and outputs golds/silvers until user guesses correct word, returns amount of guesses user takes
*****************************/
int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
	int score = 1;
	int golds = 0;
	int silvers = 0;

	//exception
	if (nWords < 0 || wordnum < 0 || wordnum >= nWords)
		return -1;

	//store hiddenword
	char hiddenWord[MAXWORDLEN + 1];
	strcpy(hiddenWord, words[wordnum]);

	//loop for attempts
	char guess[101];
	char target[MAXWORDLEN + 1];
	while (strcmp(hiddenWord, guess) != 0)
	{
		cout << "Probe word: ";
		cin >> guess;
		cin.ignore(10000, '\n');

		//breaks to next iteration if input is not valid
		if (!isValidInput(guess))
		{
			cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
			continue;
		}

		//breaks to next iteration if guess is not in words[]
		if (!isValidWord(words, nWords, guess))
		{
			cout << "I don't know that word." << endl;
			continue;
		}

		//make a copy of hiddenWord for processing
		strcpy(target, hiddenWord);
		//debug cerr << "target: " << target << endl;

		//break out of loop if guessed correctly
		if (strcmp(target, guess) == 0)
			break;

		golds = countGolds(target, guess);
		silvers = countSilvers(target, guess); 

		cout << "Golds: " << golds << ", Silvers: " << silvers << endl;

		score++;

	}

	return score;
}

/******************************
* bool isValidInput
* checks if input string is a 4-6 letter lowercase word
******************************/
bool isValidInput(char instr[])
{
	if (!(strlen(instr) >= MINWORDLEN && strlen(instr) <= MAXWORDLEN))
		return false;
	for (int i = 0; i < strlen(instr); i++)
	{
		if (!(instr[i] >= 'a' && instr[i] <= 'z'))
			return false;
	}
	return true;
}

/*****************************
* bool isValidWord
* checks if input string is found in wordList
*****************************/
bool isValidWord(const char wordList[][MAXWORDLEN + 1], int numWords, char instr[])
{
	for (int i = 0; i < numWords; i++)
	{
		if (strcmp(wordList[i], instr) == 0)
			return true;
	}
	return false;
}

/*****************************
* int countGolds
* finds matches between characters in target and input, then removes those matches from target and input
*****************************/
int countGolds(char target[], char input[])
{
	int goldCount = 0;

	//compute length to loop through
	int length;
	if (strlen(target) > strlen(input))
		length = strlen(input);
	else
		length = strlen(target);

	//search for matches
	for (int i = length - 1; i >= 0; i--)
	{
		if (target[i] == input[i])
		{
			goldCount++;
			removeChar(target, i);
			removeChar(input, i);
			//debug cerr << "target: " << target << " input: " << input << endl;
		}
	}

	return goldCount;
}

/*****************************
* int countSilvers
* searches for characters from input in target, then removes those characters from target and input
*****************************/
int countSilvers(char target[], char input[])
{
	int silverCount = 0; 

	//search for matches
	for (int i = strlen(target) - 1; i >= 0; i--)
	{
		for (int j = strlen(input) - 1; j >= 0; j--)
		{
			if (target[i] == input[j])
			{
				silverCount++;
				removeChar(target, i);
				removeChar(input, j);
				//debug cerr << "target: " << target << " input: " << input << endl;
			}
		}
	}

	return silverCount;
}

/*****************************
* void removeChar
* removes character at position pos from input
*****************************/
void removeChar(char input[], int pos)
{
	for (int i = pos; i < strlen(input) - 1; i++)		//sets each element in a[] from pos to the element to its right
		input[i] = input[i + 1];
	input[strlen(input) - 1] = '\0';	//sets last element in a[] to null 
}
