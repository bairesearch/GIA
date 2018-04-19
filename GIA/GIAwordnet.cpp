/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAwordnet.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f8b 18-April-2018
 * Requirements: requires wordnet libraries to be installed
 * Description: Wordnet - searches wordnet database and parses wordnet output
 * /
 *******************************************************************************/


#include "GIAwordnet.hpp"


static int synonymnDetectionStatus;
#define WORDNET_SENSE_STRING "Sense"

int GIAwordnetClass::getSynonymnDetectionStatus()
{
	return synonymnDetectionStatus;
}
void GIAwordnetClass::initialiseWordNet(const int newSynonymnDetectionStatus)
{
	synonymnDetectionStatus = newSynonymnDetectionStatus;

	wninit();

	#ifdef GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP
  	string wordExample = "go";
	char* wordExampleCharStar = const_cast<char*>(wordExample.c_str());
	char* output = findtheinfo(wordExampleCharStar,1,5,0);
	cerr << "wordnet output = " << output << endl;
	//exit(EXIT_ERROR);
	string wordExample = "pretty";
	string wordExample2 = "beautiful";	//boring
	bool wordIsFound = false;
	string listOfSynonyms[WORDNET_FINDTHEINFO_OUTPUT_MAX_NUMBER_SYNONYMS];
	int wordNetPOS = ADJ;	//NOUN	VERB
	bool synFound = checkIfWordIsContainedWithinAnotherWordsSynsets(&wordExample, &wordExample2, wordNetPOS);
	cerr << "wordExample = " << wordExample << endl;
	cerr << "wordExample2 = " << wordExample2 << endl;
	cerr << "synFound = " << synFound << endl;
	//findSynonymsOLD(wordExample, &wordIsFound, listOfSynonyms, wordNetPOS);
	exit(EXIT_ERROR);
	#endif

}


/*
http://wordnet.princeton.edu/man/wnsearch.3WN.html
/usr/include/wn.h

int wordNetPOS =
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
#define ENTAILPTR        4	//*
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


//assumes word and otherWord have the same wordNetPOS
bool GIAwordnetClass::checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(string* word, string* otherWord, const int wordNetPOS)
{
	bool entityNamesAreSynonymous = false;



	if(checkIfWordIsContainedWithinAnotherWordsSynsets(word, otherWord, wordNetPOS))
	{
		entityNamesAreSynonymous = true;
	}


	if(checkIfWordIsContainedWithinAnotherWordsSynsets(otherWord, word, wordNetPOS))
	{
		entityNamesAreSynonymous = true;
	}


	return entityNamesAreSynonymous;
}

bool GIAwordnetClass::checkIfWordIsContainedWithinAnotherWordsSynsets(const string* word, const string* otherWord, const int wordNetPOS)
{
	bool entityNamesAreSynonymous = false;

	for(int similarityTypeIndex = 0; similarityTypeIndex<WORDNET_DATA_ENTRY_POINTERS_INDICATING_SIMILAR_SYNSETS_NUMBER_OF_TYPES; similarityTypeIndex++)
	{
		int similarityType = wordnetDataEntryPointersIndicatingSimilarSynsetsArray[similarityTypeIndex];

		bool wordIsFound = false;


		SynsetPtr firstSenseInList = findSynsets(word, &wordIsFound, wordNetPOS, similarityType);

		if(wordIsFound)
		{
			int irrelevantNotUsed = 0;
			bool senseOutputWithHighestTagsPassedNewSynsetMustFree = false;
			SynsetPtr senseOutputWithHighestTags = checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(firstSenseInList, wordNetPOS, &irrelevantNotUsed, &entityNamesAreSynonymous, word, otherWord, true, &senseOutputWithHighestTagsPassedNewSynsetMustFree);
			if(senseOutputWithHighestTagsPassedNewSynsetMustFree)
			{
				free_synset(senseOutputWithHighestTags);	//Free a synset	//CHECK THIS; senseOutputWithHighestTags may not have been allocated as yet (do not assume free_synset safe to dealloc a NULL SynsetPtr)
			}
		}

		free_syns(firstSenseInList);	//Free a synset linked list allocated by findtheinfo_ds()
	}

	return entityNamesAreSynonymous;
}


SynsetPtr GIAwordnetClass::findMostPopularSynsets(const string* word, bool* wordIsFound, const int wordNetPOS)
{
	int maximumNumberOfTagsAcrossSimilarityTypes = 0;
	SynsetPtr senseOutputWithHighestTagsAcrossSimilarityTypes = NULL;
	bool senseOutputWithHighestTagsAcrossSimilarityTypesMustFree = false;

	for(int similarityTypeIndex = 0; similarityTypeIndex<WORDNET_DATA_ENTRY_POINTERS_INDICATING_SIMILAR_SYNSETS_NUMBER_OF_TYPES; similarityTypeIndex++)
	{
		int similarityType = wordnetDataEntryPointersIndicatingSimilarSynsetsArray[similarityTypeIndex];
		SynsetPtr firstSenseInList = findSynsets(word, wordIsFound, wordNetPOS, similarityType);

		if(*wordIsFound)
		{
			int maximumNumberOfTags = 0;
			bool entityNamesAreSynonymousIrrelevantNotUsed = false;
			string otherWordIrrelevantNotUsed = "";
			bool senseOutputWithHighestTagsPassedNewSynsetMustFree = false;
			SynsetPtr senseOutputWithHighestTags = checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(firstSenseInList, wordNetPOS, &maximumNumberOfTags, &entityNamesAreSynonymousIrrelevantNotUsed, word, &otherWordIrrelevantNotUsed, false, &senseOutputWithHighestTagsPassedNewSynsetMustFree);

			if(maximumNumberOfTags > maximumNumberOfTagsAcrossSimilarityTypes)
			{
				if(senseOutputWithHighestTagsAcrossSimilarityTypes != NULL)
				{//senseOutputWithHighestTagsAcrossSimilarityTypes may not have been allocated as yet (do not assume free_synset safe to dealloc a NULL SynsetPtr)
					if(senseOutputWithHighestTagsAcrossSimilarityTypesMustFree)
					{
						free_synset(senseOutputWithHighestTagsAcrossSimilarityTypes);	//Free a synset
					}
				}
				if(senseOutputWithHighestTagsPassedNewSynsetMustFree)
				{
					senseOutputWithHighestTagsAcrossSimilarityTypesMustFree = true;
				}
				else
				{
					senseOutputWithHighestTagsAcrossSimilarityTypesMustFree = false;
				}

				maximumNumberOfTagsAcrossSimilarityTypes = maximumNumberOfTags;
				senseOutputWithHighestTagsAcrossSimilarityTypes = senseOutputWithHighestTags;
			}
			else
			{
				if(senseOutputWithHighestTagsPassedNewSynsetMustFree)
				{
					free_synset(senseOutputWithHighestTags);	//Free a synset	//CHECK THIS; senseOutputWithHighestTags may not have been allocated as yet (do not assume free_synset safe to dealloc a NULL SynsetPtr)
				}
			}
		}
	}
	return senseOutputWithHighestTagsAcrossSimilarityTypes;
}

SynsetPtr GIAwordnetClass::findSynsets(const string* word, bool* wordIsFound, const int wordNetPOS, const int similarityType)
{
	char* wordCharStar = const_cast<char*>(word->c_str());


	SynsetPtr firstSenseInList = findtheinfo_ds(wordCharStar, wordNetPOS, similarityType, 0);	//returns pointer to the first Synset struct in a Synset struct linked list containing word/searchStr


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

SynsetPtr GIAwordnetClass::checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(SynsetPtr firstSenseInList, const int wordNetPOS, int* maximumNumberOfTags, bool* entityNamesAreSynonymous, const string* word, const string* otherWord, const bool compareEntityNames, bool* senseOutputWithHighestTagsPassedNewSynsetMustFree)
{

	SynsetPtr currentSenseInList = firstSenseInList;
	SynsetPtr senseOutputWithHighestTags = firstSenseInList;
	*senseOutputWithHighestTagsPassedNewSynsetMustFree = false;
	*maximumNumberOfTags = 0;

	int senseIndex = 0;

	bool stillSensesToGo = true;
	while(stillSensesToGo)
	{
		for(int pointerIndex = CURRENTSYNSETPOINTERINDEX; pointerIndex<currentSenseInList->ptrcount; pointerIndex++)	//updated 2 June 2012 to properly account for CURRENTSYNSETPOINTERINDEX
		{
			for(int similarityTypeIndex = 0; similarityTypeIndex<WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES; similarityTypeIndex++)
			{
				SynsetPtr currentRelatedSense = NULL;
				bool passed = false;
				bool passedNewSynsetMustFree = false;

				if(pointerIndex == CURRENTSYNSETPOINTERINDEX)
				{
					if(similarityTypeIndex == 0)
					{//only execute the loop once when (pointerIndex == CURRENTSYNSETPOINTERINDEX)
						passed = true;
						currentRelatedSense = currentSenseInList;
					}
				}
				else
				{
					if(currentSenseInList->ptrtyp[pointerIndex] == wordnetDataEntryPointersIndicatingRelatedSynsetsArray[similarityTypeIndex])
					{
						//if ptrtyp indicates related synset, then go to ptroff
						passed = true;
						char* wordInterestedIn = const_cast<char*>(word->c_str());
						currentRelatedSense = read_synset(wordNetPOS, currentSenseInList->ptroff[pointerIndex], wordInterestedIn);
						passedNewSynsetMustFree = true;
					}
				}

				if(passed)
				{
					if(compareEntityNames)
					{
						for(int w=0; w< currentRelatedSense->wcount; w++)
						{

							string currentWord = currentRelatedSense->words[w];
							if(currentWord == *otherWord)
							{
								*entityNamesAreSynonymous = true;

								/*
								if(compareEntityNames)
								{
									if(tagCount >* maximumNumberOfTags)
									{
										*maximumNumberOfTags = tagCount;
										senseOutputWithHighestTags = currentRelatedSense;
									}
								}
								*/
							}
						}
					}
					else
					{

						Index* idxOfFirstWordInWords = getindex(currentRelatedSense->words[0], wordNetPOS);	//returns pointer to Index struct representing first word in words[]
						int senseNumber = *(currentRelatedSense->wnsns);
						int tagCount = GetTagcnt(idxOfFirstWordInWords, senseNumber);



						free_index(idxOfFirstWordInWords);	//Free an index structure

						if(tagCount >* maximumNumberOfTags)
						{
							*maximumNumberOfTags = tagCount;
							if(senseOutputWithHighestTags != NULL)
							{//senseOutputWithHighestTags may not have been allocated as yet (do not assume free_synset safe to dealloc a NULL SynsetPtr)
								if(*senseOutputWithHighestTagsPassedNewSynsetMustFree)
								{
									free_synset(senseOutputWithHighestTags);	//Free a synset
								}
							}
							if(passedNewSynsetMustFree)
							{
								*senseOutputWithHighestTagsPassedNewSynsetMustFree = true;
							}
							else
							{
								*senseOutputWithHighestTagsPassedNewSynsetMustFree = false;
							}
							senseOutputWithHighestTags = currentRelatedSense;
						}
					}

				}

				if(passedNewSynsetMustFree)
				{
					if(currentRelatedSense != senseOutputWithHighestTags)
					{
						free_synset(currentRelatedSense);	//Free a synset
					}
				}
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

	return senseOutputWithHighestTags;
}



void GIAwordnetClass::findSynonymsOLD(const string word, bool* wordIsFound, string listOfSynonyms[], const int wordNetPOS)
{
	bool result = true;

	int similarityType = SIMPTR;	//SIMPTR (similar) or SYNS (synonymn) - they both appear to give same output
	char* wordCharStar = const_cast<char*>(word.c_str());

	char* output = findtheinfo(wordCharStar, wordNetPOS, similarityType, 0);

	string lineString = "";
	string synonymString = "";
	string numberOfSensesString = "";

	int charIndex = 0;
	int lineIndex = 0;

	if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, &numberOfSensesString, CHAR_SPACE, CHAR_END_OF_STRING))	//wait till end of header
	{
		cerr << "findSynonyms error: number of senses string not found" << endl;
		cerr << "charIndex = " << charIndex << endl;
		cerr << "lineIndex = " << lineIndex << endl;
		exit(EXIT_ERROR);
	}

	//now convert numberOfSensesString to number (this becomes the number of 'senses')
	int numberSenses = SHAREDvars.convertStringToInt(numberOfSensesString);

	if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, &lineString, CHAR_NEWLINE, CHAR_END_OF_STRING))	//wait till end of header
	{
		cerr << "findSynonyms error: new line not found" << endl;
		cerr << "charIndex = " << charIndex << endl;
		cerr << "lineIndex = " << lineIndex << endl;
		exit(EXIT_ERROR);
	}
	lineIndex++;

	int numberOfSynonyms = 0;

	for(int sense=1; sense <= numberSenses; sense++)
	{
		char c = output[charIndex];
		if(c != CHAR_NEWLINE)
		{
			cerr << "findSynonyms error: deformation (double new line not found)" << endl;
			cerr << "charIndex = " << charIndex << endl;
			cerr << "lineIndex = " << lineIndex << endl;
			exit(EXIT_ERROR);
		}
		charIndex++;
		lineIndex++;

		if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, &lineString, CHAR_NEWLINE, CHAR_END_OF_STRING))	//wait till end of header
		{
			cerr << "findSynonyms error: new line not found" << endl;
			cerr << "charIndex = " << charIndex << endl;
			cerr << "lineIndex = " << lineIndex << endl;
			exit(EXIT_ERROR);
		}
		lineIndex++;

		string senseString = SHAREDvars.convertIntToString(sense);
		string senseEntryTitleStringExpected = "";
		senseEntryTitleStringExpected = senseEntryTitleStringExpected + WORDNET_SENSE_STRING + " " + senseString;

		if(senseEntryTitleStringExpected != lineString)
		{
			cerr << "findSynonyms error: (senseEntryTitleStringExpected != lineString)" << endl;
			cerr << "charIndex = " << charIndex << endl;
			cerr << "lineIndex = " << lineIndex << endl;
			cerr << "senseEntryTitleStringExpected = " << senseEntryTitleStringExpected << endl;
			cerr << "lineString = " << lineString << endl;
			exit(EXIT_ERROR);
		}

		if(sense == 1)
		{//CHECK THIS; only take the meaning/sense from the most popular/likely sense [Future: need to search context for most relevant sense]

			int synonymnIndex = 0;
			while(recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, &synonymString, CHAR_COMMA, CHAR_SPACE))
			{
				listOfSynonyms[synonymnIndex] = synonymString;
				synonymnIndex++;

				c = output[charIndex];
				if(c != CHAR_SPACE)
				{//deal with additional space
					cerr << "findSynonyms error: space not found after synonymn" << endl;
					cerr << "charIndex = " << charIndex << endl;
					cerr << "lineIndex = " << lineIndex << endl;
					exit(EXIT_ERROR);
					result = false;
				}
				charIndex++;
			}
			listOfSynonyms[synonymnIndex] = synonymString;
			synonymnIndex++;

			numberOfSynonyms = synonymnIndex;
		}

		if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, &lineString, CHAR_NEWLINE, CHAR_END_OF_STRING))	//wait till end of line
		{
			cerr << "findSynonyms error: new line not found" << endl;
			cerr << "charIndex = " << charIndex << endl;
			cerr << "lineIndex = " << lineIndex << endl;
			exit(EXIT_ERROR);
		}

	}


	if(numberSenses > 0)
	{
		*wordIsFound = true;
	}
	else
	{
		*wordIsFound = false;
	}
}

bool GIAwordnetClass::recordUntilCharacterOrEscapeCharacterOLD(int charIndex, const char* output, int* newCharIndex, string* lineString, const char characterToRecordUntil, const char escapeCharacter)
{
	*lineString = "";

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

			*lineString = *lineString + c;
			i++;
		}
	}

	*newCharIndex = i+charIndex;

	return result;
}



