#include <iostream>
#include <string>
#include <cassert>
#include <array>
using namespace std;

/****************************************
* appendToAll
* adds value to each element in a[] 
* returns -1 if n<0, otherwise returns n
****************************************/
int appendToAll(string a[], int n, string value)
{
	if (n < 0)		//undefined behavior
		return -1;

	for (int i = 0; i < n; i++)
		a[i] = a[i] + value;		//add value to each element 

	return n;
}

/****************************************
* lookup
* searches a[] for an element that matches target
* returns index of first element found, otherwise returns -1
****************************************/
int lookup(const string a[], int n, string target)
{
	if (n < 0)		//undefined behavior
		return -1;

	for (int i = 0; i < n; i++)
	{
		if (a[i] == target)		//match found, return index 
			return i;
	}
	return -1;		//no match found
}

/****************************************
* positionOfMax
* searches for element in a[] that is >= all other elements
* returns -1 if n<=0, otherwise returns index of max string
****************************************/
int positionOfMax(const string a[], int n)
{
	if (n <= 0)		//undefined behavior
		return -1;

	string maxString = "";
	int maxIndex = 0;

	for (int i = 0; i < n; i++)
	{
		if (a[i] > maxString)		//compare to maxString then update maxString and maxIndex
		{
			maxString = a[i];
			maxIndex = i;
		}
	}
	return maxIndex;

}

/****************************************
* rotateLeft
* moves element at pos to the end of a[] and moves all elements to the right leftward by 1 index
* returns -1 if n<0, pos>=n, or pos<0, otherwise returns original position of item
****************************************/
int rotateLeft(string a[], int n, int pos)
{
	if (n < 0 || pos < 0 || pos >= n)		//undefined behavior
		return -1;

	string temp = a[pos];
	for (int i = pos; i < n-1; i++)		//sets each element in a[] from pos to the element to its right
		a[i] = a[i + 1];

	a[n-1] = temp; //sets last element in a[] to element originally at pos
	return pos;
}

/****************************************
* countRuns
* searches a[] for consecutive sequences of elements
* returns -1 if n<0, otherwise returns number of consecutive sequences of elements
****************************************/
int countRuns(const string a[], int n)
{
	if (n < 0)		//undefined behavior
		return -1;
	if (n == 0)		//array of size 0 has 0 runs
		return 0;

	int seqCount = 0;
	int count = 1;
	for (int i = 0; i < n-1; i++)
	{
		if (a[i] == a[i + 1])	//increment count if two consecutive elements
			count++;
		else		//reset count if not consecutive
		{
			if (count > 1)		//update seqCount
				seqCount++;
			count = 1;
		}

	}
	if (count > 1)
		seqCount++;
	return seqCount;
}

/****************************************
* flip
* reverses order of a[]
* returns -1 if n<0, otherwise returns n
****************************************/
int flip(string a[], int n)
{
	if (n < 0)		//undefined behavior
		return -1;

	string temp;

	for (int i = 0; i < n / 2; i++)		//iterate through first half of a[] to switch elements with second half
	{
		temp = a[i];
		a[i] = a[n - 1 - i];
		a[n - 1 - i] = temp;
	}
	return n;
}

/****************************************
* differ
* searches a1[] concurrently with a2[] and compares corresponding values
* returns -1 if n1 or n2<0, otherwise returns either n1 or n2 of first differing element
****************************************/
int differ(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0 || (n1 == 0 && n2 == 0))		//undefined behavior
		return -1;

	for (int i = 0; (i < n1 && i < n2); i++)
	{
		if (a1[i] != a2[i])
			return i;		//return index of a1[] and a2[] where elements differ
	}

	if (n1 < n2)		//reached end of loop, return n1 or n2 whichever is less
		return n1;
	else
		return n2;
}

/****************************************
* subsequence
* searches a1[] for entirety of a2[]
* returns -1 if n1 or n2<0 or n1 < n2, otherwise returns starting index in a1[] of a2[] 
****************************************/
int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0 || n1 < n2)	//undefined behavior
		return -1;
	else if (n2 == 0)	//empty array is a subsequence of any array at index 0
		return 0;

	int startIndex = 0;
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < n2; j++)
		{
			if (a1[i + j] != a2[j])		//break to next element of a1 if elements dont match
			{
				startIndex = i + j + 1;		//startIndex incremements by amount of elements checked in a2[] before failure + 1
				break;
			}
			if (j == n2 - 1)		//reaches end of j and all elements match, return startIndex
				return startIndex;
		}
	}
	return -1;

}

/****************************************
* lookupAny
* searches a1[] for any element of a2[]
* returns -1 if n1 or n2<0 or if no elements match, otherwise returns the first position in a1[] where the element is also in a2[]
****************************************/
int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0)	//undefined behavior
		return -1;

	for (int i = 0; i < n1; i++)
	{
		if (lookup(a2, n2, a1[i]) != -1)		//lookup() finds a match 
			return i;
	}
	return -1;		//not found
}

/****************************************
* split
* compares each element of a[] to splitter, if element > splitter rotate left, if element < splitter rotate right
* returns -1 if n<0, otherwise returns first element that is not < splitter, or n if there are no such elements
****************************************/
int split(string a[], int n, string splitter)
{
	if (n < 0)		//undefined behavior
		return -1;

	for (int i = 0; i < n; i++)			//sort elements larger to the right
	{
		if (a[i] > splitter)
			rotateLeft(a, n, i);
	}

	
	for (int i = 0; i < n; i++)			//sort elements smaller to the left
	{
		if (a[i] < splitter)
		{
			string temp = a[i];
			for (int j = i; j > 0; j--)		//sets each element in a[] from pos to the element to its left
				a[j] = a[j-1];
			a[0] = temp;

		}
	}
	

	for (int i = 0; i < n; i++)			//search for first element not less than splitter
	{
		if (!(a[i] < splitter))
			return i;
	}

	return n;
}

/****************************************
* printArray
* prints every element of a[] to cerr
****************************************/
void printArray(string a[], int n)
{
	for (int i = 0; i < n; i++)
		cerr << a[i] << endl;
}
/****************************************/

int main() 
{
	string a[7] = { "rishi", "margaret", "gordon", "tony", "", "john", "liz" };
	string b[4] = { "rishi", "margaret", "liz", "theresa" };
	string c[4] = { "gordon", "tony", "", "john" };
	string d[5] = { "margaret", "margaret", "margaret", "tony", "tony" };
	string e[3] = { "liz", "gordon", "tony" };
	string f[10] = { "margaret", "margaret", "margaret", "tony", "tony", "margaret", "tony", "tony", "margaret", "tony" };

	assert(lookup(a, 7, "john") == 5);			//search for john
	assert(lookup(a, 5, "john") == -1);			//john outside of n
	assert(lookup(b, 4, "john") == -1);			//john does not exist

	assert(positionOfMax(a, 7) == 3);			//max in full array
	assert(positionOfMax(a, 3) == 0);			//max in truncated array
	assert(positionOfMax(d, 5) == 3);			//multiple elements are max

	assert(countRuns(a, 7) == 0);				//no runs
	assert(countRuns(d, 5) == 2);				//two consecutive runs
	assert(countRuns(f, 10) == 3);				//nonconsecutive repeats

	assert(differ(a, 7, b, 4) == 2);			//two full arrays
	assert(differ(a, 2, b, 3) == 2);			//truncated array runs out
	assert(differ(d, 5, f, 10) == 5);			//full array runs out

	assert(subsequence(a, 7, b, 4) == -1);		//b not subsequence of a
	assert(subsequence(a, 7, b, 2) == 0);		//truncated b found in a
	assert(subsequence(a, 7, c, 4) == 2);		//c found in middle of a
	assert(subsequence(f, 10, d, 1) == 0);		//truncated d found multiple times in a

	assert(lookupAny(a, 7, c, 4) == 2);			//element of c in a
	assert(lookupAny(e, 3, b, 2) == -1);		//no elements from b in e

	assert(appendToAll(a, 7, "!") == 7 && a[0] == "rishi!" && a[6] == "liz!");		//test appendToAll
	assert(rotateLeft(a, 7, 2) == 2 && a[2] == "tony!" && a[6] == "gordon!");		//test rotateLeft
	assert(flip(a, 7) == 7 && a[0] == "gordon!" && a[3] == "!" && a[6] == "rishi!");		//test flip
	assert(split(a, 7, "john!") == 2 && a[2] == "john!");		//test split

	cerr << "All tests succeeded" << endl;
}