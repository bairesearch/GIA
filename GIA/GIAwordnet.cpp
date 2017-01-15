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
http://wordnet.princeton.edu/man/wnsearch.3WN.html
/usr/include/wn.h

int wordType = 
#define NOUN		1
#define VERB		2
#define ADJ		3
#define ADV		4
#define SATELLITE	5	//not really a part of speech
#define ADJSAT		SATELLITE

int similarityType = 
#define ANTPTR           1	// !
#define HYPERPTR         2	// @
#define HYPOPTR          3	// ~
#define ENTAILPTR        4	// *
#define SIMPTR           5	// &

#define ISMEMBERPTR      6	// #m
#define ISSTUFFPTR       7	// #s
#define ISPARTPTR        8	// #p

#define HASMEMBERPTR     9	// %m
#define HASSTUFFPTR     10	// %s
#define HASPARTPTR      11	// %p

#define MERONYM         12	// % (not valid in lexicographer file)
#define HOLONYM         13	// # (not valid in lexicographer file)
#define CAUSETO         14	// >
#define PPLPTR	        15	// <
#define SEEALSOPTR	16	// ^
#define PERTPTR		17	// \
#define ATTRIBUTE	18	// =
#define VERBGROUP	19	// $
#define DERIVATION      20	// +
#define CLASSIFICATION  21	// ;
#define CLASS           22	// -

#define LASTTYPE	CLASS

#define SYNS            (LASTTYPE + 1)
#define FREQ            (LASTTYPE + 2)
#define FRAMES          (LASTTYPE + 3)
#define COORDS          (LASTTYPE + 4)
#define RELATIVES	(LASTTYPE + 5)
#define HMERONYM        (LASTTYPE + 6)
#define HHOLONYM	(LASTTYPE + 7)
#define WNGREP		(LASTTYPE + 8)
#define OVERVIEW	(LASTTYPE + 9)

*/


//assumes word and queryWord have the same wordType
bool checkIfQueryWordIsContainedWithinAnotherWordsSynsets(string word, string queryWord, int wordType)
{
	bool wordIsFound = false;
	bool entityNamesAreSynonymous = false;
	SynsetPtr firstSenseInList = findSynsets(word, &wordIsFound, wordType);
	if(wordIsFound)
	{
		//now search the synsets for equivalent words
		int senseIndex = 0;
		bool stillSensesToGo = true;
		SynsetPtr currentSenseInList = firstSenseInList;
		while(stillSensesToGo)
		{
			for(int w=0; w<currentSenseInList->wcount; w++)
			{
				//cout << "word = " << currentSenseInList->words[w] << endl;
				if(currentSenseInList->words[w] == queryWord)
				{
					#ifdef GIA_WORDNET_DEBUG
					cout << "match found - entityNamesAreSynonymous:" << endl;
					cout << "currentSenseInList->words[w] = " << currentSenseInList->words[w] << endl;
					cout << "queryWord = " << queryWord << endl;
					#endif
				}
			}
			
			if(currentSenseInList->nextss == NULL)
			{
				stillSensesToGo = false;
			}
			else
			{
				currentSenseInList = currentSenseInList->nextss;
			}

			senseIndex++;

		}

		//entityNamesAreSynonymous = true;
	}
	
	return entityNamesAreSynonymous;
}
	
SynsetPtr findMostPopularSynset(string word, bool * wordIsFound, int wordType)
{
	SynsetPtr firstSenseInList = findSynsets(word, wordIsFound, wordType);
	SynsetPtr senseOutputWithHighestTags = NULL;
	if(*wordIsFound)
	{
		senseOutputWithHighestTags = findMostPopularSynset(firstSenseInList, wordType);
	}
	return senseOutputWithHighestTags;
}

SynsetPtr findSynsets(string word, bool * wordIsFound, int wordType)
{
	bool result = true;
	
	int similarityType = SIMPTR;	//SIMPTR (similar) or SYNS (synonymn) - they both appear to give same output
	char * wordCharStar = const_cast<char*>(word.c_str());
	
	SynsetPtr firstSenseInList = findtheinfo_ds(wordCharStar, wordType, similarityType, 0);	//returns pointer to the first Synset struct in a Synset struct linked list containing word/searchStr

	//char * sensePrintedOutput = findtheinfo(wordCharStar, wordType, similarityType, 0);		//similarityType/OVERVIEW
	//cout << "findtheinfo sensePrintedOutput = " << sensePrintedOutput << endl;
	
	if(firstSenseInList == NULL)
	{
		*wordIsFound = false;
	}
	else
	{
		*wordIsFound = true;
	}
		
	return firstSenseInList;
}

SynsetPtr findMostPopularSynset(SynsetPtr firstSenseInList, int wordType)
{
	bool result = true;
	
	SynsetPtr currentSenseInList = firstSenseInList;
	SynsetPtr senseOutputWithHighestTags = firstSenseInList;
	int maximumNumberOfTags = 0;
	
	int senseIndex = 0;
	
	bool stillSensesToGo = true;
	while(stillSensesToGo)
	{
		#ifdef GIA_WORDNET_DEBUG
		cout << "senseIndex = " << senseIndex << endl;
		cout << "currentSenseInList->wcount = " << currentSenseInList->wcount << endl;
		for(int w=0; w< currentSenseInList->wcount; w++)
		{
			cout << "word = " << currentSenseInList->words[w] << endl;
		}
		cout << "whichword = " << currentSenseInList->whichword << endl;		//which word in words[] corresponds to word/searchStr
		#endif
		
		Index * idxOfFirstWordInWords = getindex(currentSenseInList->words[0], wordType);	//returns pointer to Index struct representing first word in words[]
		int senseNumber = *(currentSenseInList->wnsns);	
		int tagCount = GetTagcnt(idxOfFirstWordInWords, senseNumber);

		#ifdef GIA_WORDNET_DEBUG
		cout << "indexInData = " << currentSenseInList->hereiam << endl;	      //index [first column] from data.wordType (eg data.noun) for the sense/usage of word/searchStr	      
		cout << "idxOfFirstWordInWords = " << idxOfFirstWordInWords << endl;
		cout << "idxOfFirstWordInWords->off_cnt " << idxOfFirstWordInWords->off_cnt << endl;  //total number of senses of first word in words[] (see index.wordType (eg index.noun) - the total number of pointers listed)
		cout << "senseNumber = " << senseNumber << endl;      //sense number of the first word in words[] (corresponding to the sense of word/searchStr) - second last column of sense.wordType (eg sense.noun)
		cout << "tagCount = " << tagCount << endl;	      //popularity of sense (number of times the sense passed has been tagged according to the cntlist file) - last column of sense.wordType (eg sense.noun)
		cout << " " << endl;
		#endif
		
		if(tagCount > maximumNumberOfTags)
		{
			senseOutputWithHighestTags = currentSenseInList;
		}
		
		#ifdef GIA_WORDNET_DEBUG
		/*
		for(int q=1; q<= idxOfFirstWordInWords->off_cnt; q++)
		{
			cout << "tagcount = " << GetTagcnt(idxOfFirstWordInWords, q) << endl;
		}
				
		cout << "hereiam = " << currentSenseInList->hereiam << endl;		//index [first column] from data.wordType (eg data.noun) for the sense/usage of word/searchStr
		cout << "sstype = " << currentSenseInList->sstype << endl; 
		cout << "fnum = " << currentSenseInList->fnum << endl;
		cout << "pos = " << currentSenseInList->pos << endl;
		cout << "wcount = " << currentSenseInList->wcount << endl; 
		cout << "words = " << currentSenseInList->words << endl;
		cout << "lexid = " << currentSenseInList->lexid << endl;
		cout << "wnsns = " << *(currentSenseInList->wnsns) << endl;			//sense number of the first word in words[] - corresponding to the sense of word/searchStr
		cout << "whichword = " << currentSenseInList->whichword << endl;		//which word in words[] corresponds to word/searchStr
		cout << "ptrcount = " << currentSenseInList->ptrcount << endl;
		cout << "ptrtyp = " << currentSenseInList->ptrtyp << endl;
		cout << "ptroff = " << currentSenseInList->ptroff << endl;
		cout << "ppos = " << currentSenseInList->ppos << endl;
		cout << "pto = " << currentSenseInList->pto << endl;
		cout << "pfrm = " << currentSenseInList->pfrm << endl;
		cout << "fcount = " << currentSenseInList->fcount << endl;
		cout << "frmid = " << currentSenseInList->frmid << endl;
		cout << "frmto = " << currentSenseInList->frmto << endl;
		cout << "defn = " << currentSenseInList->defn << endl;
		cout << "key = " << currentSenseInList->key << endl;
		cout << "searchtype = " << currentSenseInList->searchtype << endl;
		cout << " " << endl;
		*/
		#endif
		
		if(currentSenseInList->nextss == NULL)
		{
			stillSensesToGo = false;
		}
		else
		{
			currentSenseInList = currentSenseInList->nextss;
		}
		
		senseIndex++;

	}
	
	return senseOutputWithHighestTags;
}

		
			
void findSynonymsOLD(string word, bool * wordIsFound, string listOfSynonyms[], int wordType)
{
	bool result = true;
	
	int similarityType = SIMPTR;	//SIMPTR (similar) or SYNS (synonymn) - they both appear to give same output
	char * wordCharStar = const_cast<char*>(word.c_str());
	
	char * output = findtheinfo(wordCharStar, wordType, similarityType, 0);

	char currentItemString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT] = "";
	char lineString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_LINE] = "";
	char synonymString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_LINE] = "";
	char numberOfSensesString[MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_NUMBER_OF_SENSES] = "";
	
	int charIndex = 0;
	int lineIndex = 0;
	
	if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, numberOfSensesString, CHAR_SPACE, CHAR_END_OF_STRING))	//wait till end of header
	{
		cout << "findSynonyms error: number of senses string not found" << endl;
		cout << "charIndex = " << charIndex << endl;
		cout << "lineIndex = " << lineIndex << endl;
		exit(0);
	}	
	
	//now convert numberOfSensesString to number (this becomes the number of 'senses')
	int numberSenses = atoi(numberOfSensesString);
	#ifdef GIA_WORDNET_DEBUG
	cout << "output = " << output << endl;
	cout << "numberSenses = " << numberSenses << endl;
	#endif
	
	if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, lineString, CHAR_NEW_LINE, CHAR_END_OF_STRING))	//wait till end of header
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
		
		if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, lineString, CHAR_NEW_LINE, CHAR_END_OF_STRING))	//wait till end of header
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
			while(recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, synonymString, CHAR_COMMA, CHAR_SPACE))
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

		if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, lineString, CHAR_NEW_LINE, CHAR_END_OF_STRING))	//wait till end of line
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

bool recordUntilCharacterOrEscapeCharacterOLD(int charIndex, char * output, int * newCharIndex, char * lineString, char characterToRecordUntil, char escapeCharacter)
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



