/*******************************************************************************
 *
 * File Name: GIAwordnet.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1a 20-Apr-2012
 * Requirements: requires wordnet libraries to be installed
 * Description: searches wordnet database and parses wordnet output
 *
 *******************************************************************************/



#include "GIAwordnet.h"
#include "wn.h"

#define CHAR_COMMA ','
#define CHAR_SPACE ' '
#define CHAR_NEW_LINE '\n'
#define CHAR_DASH '-'
#define CHAR_END_OF_STRING '\0'
#define CHAR_DEL (127)
#define CHAR_IRRELEVANT (CHAR_DEL)

#define WORDNET_SENSE_STRING "Sense"


void initialiseWordNet()
{
	wninit();
	
	/*
	#ifdef GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP
  	string wordExample = "go";
	char * wordExampleCharStar = const_cast<char*>(wordExample.c_str());
	char * output = findtheinfo(wordExampleCharStar,1,5,0);
	cout << "wordnet output = " << output << endl;
	exit(0);
	#endif
	*/
}

/*
bool recordUntilCharacterOrEscapeCharacter(int charIndex, char * output, int * newCharIndex, char * lineString, char characterToRecordUntil, char escapeCharacter, int numberOfConsecutiveEscapeCharacters)
{
	bool result = true;
	int i = 0;
	int numConsecutiveEscapeCharactersDetected = 0;
	
	char c = CHAR_IRRELEVANT;
	while((c != characterToRecordUntil) && result)
	{	
		c = output[i+charIndex];
		
		if(c == escapeCharacter)
		{
			numConsecutiveEscapeCharactersDetected++;
			if(numConsecutiveEscapeCharactersDetected == numberOfConsecutiveEscapeCharacters)
			{
				result = false;
			}
		}
		else
		{
			numConsecutiveEscapeCharactersDetected = 0;
		}
		lineString[i] = c;
		i++;
	}
	
	lineString[i] = '\0';
	
	*newCharIndex = i+charIndex;
	
	return result;
}
*/


bool recordUntilCharacterOrEscapeCharacter(int charIndex, char * output, int * newCharIndex, char * lineString, char characterToRecordUntil, char escapeCharacter)
{
	bool result = true;
	int i = 0;
	
	char c = CHAR_IRRELEVANT;
	while((c != characterToRecordUntil) && result)
	{	
		if(c == escapeCharacter)
		{
			result = false;
		}
		else
		{
			c = output[i+charIndex];

			lineString[i] = c;
			i++;
		}
	}
	
	*newCharIndex = i+charIndex;
	
	i--;
	lineString[i] = '\0';
	
	return result;
}





void findSynonyms(string word, bool * wordIsFound, string listOfSynonyms[])
{
	bool result = true;
	
	char * wordCharStar = const_cast<char*>(word.c_str());
	char * output = findtheinfo(wordCharStar,1,5,0);

	char currentItemString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT] = "";
	char lineString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_LINE] = "";
	char synonymString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_LINE] = "";
	char numberOfSensesString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_NUMBER_OF_SENSES] = "";
	
	int charIndex = 0;
	int lineIndex = 0;
	
	if(!recordUntilCharacterOrEscapeCharacter(charIndex, output, &charIndex, numberOfSensesString, CHAR_SPACE, CHAR_END_OF_STRING))	//wait till end of header
	{
		cout << "findSynonyms error: number of senses string not found" << endl;
		cout << "charIndex = " << charIndex << endl;
		cout << "lineIndex = " << lineIndex << endl;
		exit(0);
	}	
	
	//now convert numberOfSensesString to number (this becomes the number of 'senses')
	int numberSenses = atoi(numberOfSensesString);
	#ifdef GIA_WORDNET_DEBUG
	cout << "numberSenses = " << numberSenses << endl;
	#endif
	
	if(!recordUntilCharacterOrEscapeCharacter(charIndex, output, &charIndex, lineString, CHAR_NEW_LINE, CHAR_END_OF_STRING))	//wait till end of header
	{
		cout << "findSynonyms error: new line not found" << endl;
		cout << "charIndex = " << charIndex << endl;
		cout << "lineIndex = " << lineIndex << endl;
		exit(0);
	}
	lineIndex++;
	
	int numberOfSynonyms = 0;
	
	for(int sense=1; sense <= numberSenses; sense++)
	{
		char c = output[charIndex];
		if(c != CHAR_NEW_LINE)
		{
			cout << "findSynonyms error: deformation (double new line not found)" << endl;
			cout << "charIndex = " << charIndex << endl;
			cout << "lineIndex = " << lineIndex << endl;		
			exit(0);
		}
		charIndex++;
		lineIndex++;
		
		if(!recordUntilCharacterOrEscapeCharacter(charIndex, output, &charIndex, lineString, CHAR_NEW_LINE, CHAR_END_OF_STRING))	//wait till end of header
		{
			cout << "findSynonyms error: new line not found" << endl;
			cout << "charIndex = " << charIndex << endl;
			cout << "lineIndex = " << lineIndex << endl;
			exit(0);
		}
		lineIndex++;	
		
		char senseString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_NUMBER_OF_SENSES];
		sprintf(senseString, "%d", sense);
		string senseEntryTitleStringExpected = "";
		senseEntryTitleStringExpected = senseEntryTitleStringExpected + WORDNET_SENSE_STRING + " " + senseString;
		//cout << "senseEntryTitleStringExpected = " << senseEntryTitleStringExpected << endl;
		
		string lineStringTemp = lineString;
		if(senseEntryTitleStringExpected != lineStringTemp)
		{
			cout << "findSynonyms error: (senseEntryTitleStringExpected != lineString)" << endl;
			cout << "charIndex = " << charIndex << endl;
			cout << "lineIndex = " << lineIndex << endl;
			cout << "senseEntryTitleStringExpected = " << senseEntryTitleStringExpected << endl;
			cout << "lineString = " << lineString << endl;
			exit(0);			
		}
		
		if(sense == 1)
		{//CHECK THIS; only take the meaning/sense from the most popular/likely sense [Future: need to search context for most relevant sense]

			int synonymnIndex = 0;
			while(recordUntilCharacterOrEscapeCharacter(charIndex, output, &charIndex, synonymString, CHAR_COMMA, CHAR_SPACE))
			{
				listOfSynonyms[synonymnIndex] = synonymString;
				synonymnIndex++;
				
				c = output[charIndex];
				if(c != CHAR_SPACE)
				{//deal with additional space
					cout << "findSynonyms error: space not found after synonymn" << endl;
					cout << "charIndex = " << charIndex << endl;
					cout << "lineIndex = " << lineIndex << endl;
					exit(0);				
					result = false;
				}
				charIndex++;
			}
			listOfSynonyms[synonymnIndex] = synonymString;
			synonymnIndex++;
							
			numberOfSynonyms = synonymnIndex;
		}		

		if(!recordUntilCharacterOrEscapeCharacter(charIndex, output, &charIndex, lineString, CHAR_NEW_LINE, CHAR_END_OF_STRING))	//wait till end of line
		{
			cout << "findSynonyms error: new line not found" << endl;
			cout << "charIndex = " << charIndex << endl;
			cout << "lineIndex = " << lineIndex << endl;
			exit(0);
		}

	}
	
	#ifdef GIA_WORDNET_DEBUG		
	for(int i=0;i<numberOfSynonyms ; i++)
	{
		cout << "listOfSynonyms[i] = " << listOfSynonyms[i] << endl;
	}
	#endif
	
	if(numberSenses > 0)
	{
		*wordIsFound = true;
	}
	else
	{
		*wordIsFound = false;
	}
}



