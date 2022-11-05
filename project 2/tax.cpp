#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
	//variables
	string name;
	double income;
	string occupation;
	int children;
	double taxDue;

	//income brackets and tax rates
	const int B1 = 55000;
	const double B1_RATE = 0.04; 
	const int B2 = 70000;
	const double B2_RATE = 0.07;
	const double B2_RATE_RED = 0.05;
	const int B3 = B1 + B2;
	const double B3_RATE = 0.093;
	const int CHILD_RED = 200;

	//inputs
	string input;

	cout << "Name: ";
	getline(cin, input);
	name = input;
	cout << "Taxable income: ";
	cin >> income;
	cin.ignore(10000, '\n');
	cout << "Occupation: ";
	getline(cin, input);
	occupation = input;
	cout << "Number of children: ";
	cin >> children;
	cin.ignore(10000, '\n');
	cout << "---" << endl;

	/*
	cout << "Name: ";
	getline(cin, input);
	name = input;
	cout << "Taxable income: ";
	getline(cin, input);
	stringstream(input) >> income;
	cout << "Occupation: ";
	getline(cin, input);
	occupation = input;
	cout << "Number of children: ";
	getline(cin, input);
	stringstream(input) >> children;
	cout << "---" << endl;
	*/

	//formatting
	cout.setf(ios::fixed);
	cout.precision(2);

	//exit errors
	if (name == "")
		cout << "You must enter a name" << endl;
	else if (income < 0)
		cout << "The taxable income must not be negative" << endl;
	else if (occupation == "")
		cout << "You must enter an occupation" << endl;
	else if (children < 0)
		cout << "The number of children must not be negative" << endl;
	
	//tax calculation
	else
	{
		if (income <= B1) //income in first tax bracket
		{
			taxDue = income * B1_RATE;
		}
		else if (income <= B3) //income in second tax bracket 
		{
			if (occupation == "teacher" || occupation == "nurse")
				taxDue = (B1 * B1_RATE) + ((income - B1) * B2_RATE_RED);
			else
				taxDue = (B1 * B1_RATE) + ((income - B1) * B2_RATE);
		}
		else //income in third tax bracket
		{
			if (occupation == "teacher" || occupation == "nurse")
				taxDue = (B1 * B1_RATE) + (B2 * B2_RATE_RED) + ((income - B3) * B3_RATE);
			else
				taxDue = (B1 * B1_RATE) + (B2 * B2_RATE) + ((income - B3) * B3_RATE);
		}

		if (income < 125000)
			taxDue -= children * CHILD_RED;
		if (taxDue < 0)
			taxDue = 0;

		cout << name << " would pay $" << taxDue << endl; 
	}
}