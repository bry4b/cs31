#include <iostream>
#include <string>
#include <cassert>
using namespace std;

//*************************************
// isValidUppercaseStateCode 
// Return true if the argument is a two-uppercase-letter state code, or false otherwise.
// input requires uppercase
// code taken from isValidUppercaseStateCode.txt from project 3 spec
//*************************************
bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
		"LA.ME.MA.MD.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
		"OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

//*************************************
// isValidResultString
// return true if the result string is valid and false otherwise
// input requires uppercase
// calls isValidUppercaseStateCode
//*************************************
bool isValidResultString(string partyResult)
{
	//max length of valid result string is 3 (two digit number plus a letter)
	if (partyResult.size() > 3)
		return false;

	//last char is a letter
	if (!(partyResult[partyResult.size() - 1] >= 'A' && partyResult[partyResult.size() - 1] <= 'Z'))
		return false;

	//test if rest of chars are numbers
	for (int i = 0; i < partyResult.size() - 1; i++)
	{
		if (!(partyResult[i] >= '0' && partyResult[i] <= '9'))
			return false;
	}
	return true;
}

//*************************************
// isValidStatePollString
// return true if a state poll is valid and false otherwise
// input requires uppercase
// calls isValidResultString
//*************************************
bool isValidStatePollString(string statePollString)
{
	//test for empty string
	if (statePollString.size() == 0)
		return true;

	//test for single character
	if (statePollString.size() == 1)
		return false;

	//test for state code
	if (!isValidUppercaseStateCode(statePollString.substr(0, 2)))
		return false;

	//searches for next letter in string and calls isValidResultString, or calls isValidResultString when loop reaches end of statePollString
	else
	{
		int codeStart = 2;
		int codeEnd = 2;

		while (codeEnd < statePollString.size())
		{
			if ( (statePollString[codeEnd] >= 'A' && statePollString[codeEnd] <= 'Z') || codeEnd == statePollString.size() - 1)
			{
				string resultString = statePollString.substr(codeStart, (codeEnd - codeStart) + 1);
				//debug cerr << resultString << endl;
				if (!isValidResultString(resultString))
					return false;
				codeStart = codeEnd + 1;
			}
			codeEnd++;
		}
		return true;
	}
}

//*************************************
// isValidPollString
// return true if a poll string is valid and false otherwise
// converts all inputs to uppercase
// calls isValidStatePollString
//*************************************
bool isValidPollString(string pollData)
{
	// convert to uppercase
	string pollString = pollData;
	for (int i = 0; i < pollData.size(); i++)
	{
		pollString[i] = toupper(pollData[i]);
	}

	int codeStart = 0;
	int codeEnd = 0;
	//debug cerr << pollString << endl; 
	while (codeEnd < pollString.size())
	{
		if (pollString[codeEnd] == ',' && codeEnd != pollString.size() - 1) //looks for state codes within poll string
		{
			//debug cerr << pollString.substr(codeStart, codeEnd - codeStart) << endl;
			if (!isValidStatePollString(pollString.substr(codeStart, (codeEnd - codeStart) )))
				return false;
			codeStart = codeEnd + 1;
		}
		else if (codeEnd == pollString.size() - 1) //loop runs to last index
		{
			if (!isValidStatePollString(pollString.substr(codeStart, (codeEnd - codeStart) + 1)))
				return false;
		}
		codeEnd++;
	}

	return true;

}

//*************************************
// processResultString
// return int of seats won by input party if input party is found in input resultString
// input requires uppercase
//*************************************
int processResultString(string resultString)
{
	//returns int value of numbers before party code
	if (resultString.size() == 2)
		return stoi(resultString.substr(0, 1));
	else if (resultString.size() == 3)
		return stoi(resultString.substr(0, 2));
	else
		return 0;

}

//*************************************
// processStatePollString
// return int of seats won by input party if such a resultString exists in statePollString
// input requires uppercase
//*************************************
int processStatePollString(string statePollString, char party)
{
	int codeStart = 2;
	int codeEnd = 2;
	int seats = 0;

	//searches for party letter in statePollString and calls processResultString
	while (codeEnd < statePollString.size())
	{
		if ((statePollString[codeEnd] >= 'A' && statePollString[codeEnd] <= 'Z') || codeEnd == statePollString.size() - 1)
		{
			if (statePollString[codeEnd] == party)
			{
				string resultString = statePollString.substr(codeStart, (codeEnd - codeStart) + 1);
				seats += processResultString(resultString);

			}
			codeStart = codeEnd + 1;
		}
		codeEnd++;
	}
	return seats;
	
}

//*************************************
// countSeats
// return 0 for success, 1 or 2 for error
// calls isValidPollString and checks for party letter possibility before calling countSeats
// stores seats won by party into seatCount
// input may be any case
//*************************************
int countSeats(string pollData, char party, int& seatCount)
{
	char partyLetter = toupper(party);

	if (!isValidPollString(pollData))
		return 1;

	else if (! (partyLetter >= 'A' && partyLetter <= 'Z') )
		return 2;

	else
	{
		seatCount = 0;
		//convert to uppercase
		string pollString = pollData;
		for (int i = 0; i < pollData.size(); i++)
		{
			pollString[i] = toupper(pollData[i]);
		}
		
		int codeStart = 0;
		int codeEnd = 0;

		while (codeEnd < pollString.size())
		{
			//searches through pollString for state code then adds to seatCount
			if (pollString[codeEnd] == ',')
			{
				seatCount += processStatePollString(pollString.substr(codeStart, (codeEnd - codeStart) ), partyLetter);
				codeStart = codeEnd + 1;
			}
			else if (codeEnd == pollString.size() - 1)
			{
				seatCount += processStatePollString(pollString.substr(codeStart, (codeEnd - codeStart) + 1), partyLetter);
				codeStart = codeEnd + 1;
			}
			codeEnd++;

		}
	}
	return 0;
	
}

int main()
{
	//testing main function
	int seats;

	assert(isValidPollString(""));													//test empty pollString 
	assert(!isValidPollString("c"));												//test invalid pollString
	assert(isValidPollString("ca")); 												//test state code only
	assert(!isValidPollString("ca2")); 												//test state code with invalid result
	assert(isValidPollString("ca2d"));												//test state code with valid result
	assert(!isValidPollString("ca2%"));												//test state code with invalid party
	assert(!isValidPollString("ca 2d"));											//test state code with invalid character
	assert(isValidPollString("ca2d4f3d"));											//test state code with multiple results
	assert(isValidPollString("cA2d,nv4T")); 										//test multiple state codes different case
	assert(!isValidPollString("ca2d, nv4t"));										//test multiple state codes with invalid character

	seats = -1;
	assert(countSeats("", 'd', seats) == 0 && seats == 0);							//test empty pollString
	seats = -1;	
	assert(countSeats("c", 'd', seats) == 1 && seats == -1);						//test invalid state code
	seats = -1;
	assert(countSeats("ca", 'd', seats) == 0 && seats == 0);						//test state code only
	seats = -1;
	assert(countSeats("ca2", 'd', seats) == 1 && seats == -1);						//test invalid result
	seats = -1;
	assert(countSeats("ca2d,", 'd', seats) == 1 && seats == -1);					//test end with comma
	seats = -1;
	assert(countSeats("ca2d", 'd', seats) == 0 && seats == 2);						//test valid result
	seats = -1;
	assert(countSeats("ca2%", 'd', seats) == 1 && seats == -1);						//test invalid party char
	seats = -1;
	assert(countSeats("ca 2d", 'd', seats) == 1 && seats == -1);					//test invalid char
	seats = -1;
	assert(countSeats("ca2d4f3d", 'd', seats) == 0 && seats == 5);					//test multiple results in a state code
	seats = -1;
	assert(countSeats("ca2d4f3d,nv4t16d9f", 'd', seats) == 0 && seats == 21);		//test mutliple state polls 
	seats = -1;
	assert(countSeats("ca2d4f3d,^nv4t16d9f%", 'd', seats) == 1 && seats == -1);		//test invalid char
	seats = -1;
	assert(countSeats("cA2d4f3d,Nv4t16D9f", 'd', seats) == 0 && seats == 21);		//test different case
	seats = -1;
	assert(countSeats("ca2d4f3d,nv4t16d9f", ' ', seats) == 2 && seats == -1);		//test invalid party 		


	cerr << "All tests succeeded" << endl;

}