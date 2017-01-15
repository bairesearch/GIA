/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAwordnet.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2b2a 21-December-2013
 * Requirements: requires wordnet libraries to be installed
 * Description: searches wordnet database and parses wordnet output
 *
 *******************************************************************************/



#include "GIAwordnet.h"



static int synonymnDetectionStatus;
#define WORDNET_SENSE_STRING "Sense"

int getSynonymnDetectionStatus()
{
	return synonymnDetectionStatus;
}
void initialiseWordNet(int newSynonymnDetectionStatus)
{
	synonymnDetectionStatus = newSynonymnDetectionStatus;

	wninit();

	#ifdef GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP
  	string wordExample = "go";
	char * wordExampleCharStar = const_cast<char*>(wordExample.c_str());
	char * output = findtheinfo(wordExampleCharStar,1,5,0);
	cout << "wordnet output = " << output << endl;
	//exit(0);
	string wordExample = "pretty";
	string wordExample2 = "beautiful";	//boring
	bool wordIsFound = false;
	string listOfSynonyms[WORDNET_FINDTHEINFO_OUTPUT_MAX_NUMBER_SYNONYMS];
	int wordNetPOS = ADJ;	//NOUN	VERB
	bool synFound = checkIfWordIsContainedWithinAnotherWordsSynsets(&wordExample, &wordExample2, wordNetPOS);
	cout << "wordExample = " << wordExample << endl;
	cout << "wordExample2 = " << wordExample2 << endl;
	cout << "synFound = " << synFound << endl;
	//findSynonymsOLD(wordExample, &wordIsFound, listOfSynonyms, wordNetPOS);
	exit(0);
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


//assumes word and otherWord have the same wordNetPOS
bool checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(string * word, string * otherWord, int wordNetPOS)
{
	bool entityNamesAreSynonymous = false;


	#ifdef GIA_WORDNET_DEBUG_OUTPUT_SYNONYMNS
	*word = "ride";
	*otherWord = "lift";
	#endif

	if(checkIfWordIsContainedWithinAnotherWordsSynsets(word, otherWord, wordNetPOS))
	{
		entityNamesAreSynonymous = true;
		#ifdef GIA_WORDNET_DEBUG
		//cout << "\t synon FOUND" << endl;
		#endif
	}


	if(checkIfWordIsContainedWithinAnotherWordsSynsets(otherWord, word, wordNetPOS))
	{
		entityNamesAreSynonymous = true;
		#ifdef GIA_WORDNET_DEBUG
		//cout << "\t synon FOUND" << endl;
		#endif
	}

	#ifdef GIA_WORDNET_DEBUG_OUTPUT_SYNONYMNS
	if(entityNamesAreSynonymous)
	{
		exit(0);
	}
	#endif

	return entityNamesAreSynonymous;
}

bool checkIfWordIsContainedWithinAnotherWordsSynsets(string * word, string * otherWord, int wordNetPOS)
{
	bool entityNamesAreSynonymous = false;

	for(int similarityTypeIndex = 0; similarityTypeIndex<WORDNET_DATA_ENTRY_POINTERS_INDICATING_SIMILAR_SYNSETS_NUMBER_OF_TYPES; similarityTypeIndex++)
	{
		int similarityType = wordnetDataEntryPointersIndicatingSimilarSynsetsArray[similarityTypeIndex];

		bool wordIsFound = false;

		#ifdef GIA_WORDNET_DEBUG
		/*
		cout << "*word = " << *word << endl;
		cout << "*otherWord = " << *otherWord << endl;
		cout << "wordNetPOS = " << wordNetPOS << endl;
		cout << "similarityType = " << similarityType << endl;
		*/
		#endif

		SynsetPtr firstSenseInList = findSynsets(word, &wordIsFound, wordNetPOS, similarityType);

		if(wordIsFound)
		{
			int irrelevantNotUsed = 0;
			bool senseOutputWithHighestTagsPassedNewSynsetMustFree = false;
			SynsetPtr senseOutputWithHighestTags = checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(firstSenseInList, wordNetPOS, &irrelevantNotUsed, &entityNamesAreSynonymous, word, otherWord, true, &senseOutputWithHighestTagsPassedNewSynsetMustFree);
			#ifdef GIA_FREE_MEMORY3
			if(senseOutputWithHighestTagsPassedNewSynsetMustFree)
			{
				free_synset(senseOutputWithHighestTags);	//Free a synset	//CHECK THIS; senseOutputWithHighestTags may not have been allocated as yet (do not assume free_synset safe to dealloc a NULL SynsetPtr)
			}
			#endif
		}

		#ifdef GIA_FREE_MEMORY3
		free_syns(firstSenseInList);	//Free a synset linked list allocated by findtheinfo_ds()
		#endif
	}

	return entityNamesAreSynonymous;
}


SynsetPtr findMostPopularSynsets(string * word, bool * wordIsFound, int wordNetPOS)
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
				#ifdef GIA_FREE_MEMORY3
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
				#endif
				maximumNumberOfTagsAcrossSimilarityTypes = maximumNumberOfTags;
				senseOutputWithHighestTagsAcrossSimilarityTypes = senseOutputWithHighestTags;
			}
			else
			{
				#ifdef GIA_FREE_MEMORY3
				if(senseOutputWithHighestTagsPassedNewSynsetMustFree)
				{
					free_synset(senseOutputWithHighestTags);	//Free a synset	//CHECK THIS; senseOutputWithHighestTags may not have been allocated as yet (do not assume free_synset safe to dealloc a NULL SynsetPtr)
				}
				#endif
			}
		}
	}
	return senseOutputWithHighestTagsAcrossSimilarityTypes;
}

SynsetPtr findSynsets(string * word, bool * wordIsFound, int wordNetPOS, int similarityType)
{
	char * wordCharStar = const_cast<char*>(word->c_str());

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "findSynsets()" << endl;
	cout << "wordCharStar = " << wordCharStar << endl;
	cout << "wordNetPOS = " << wordNetPOS << endl;
	*/
	#endif

	SynsetPtr firstSenseInList = findtheinfo_ds(wordCharStar, wordNetPOS, similarityType, 0);	//returns pointer to the first Synset struct in a Synset struct linked list containing word/searchStr

	#ifdef GIA_WORDNET_DEBUG
	//char * sensePrintedOutput = findtheinfo(wordCharStar, wordNetPOS, similarityType, 0);		//similarityType/OVERVIEW
	//cout << "findtheinfo sensePrintedOutput = " << sensePrintedOutput << endl;
	#endif

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

SynsetPtr checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(SynsetPtr firstSenseInList, int wordNetPOS, int * maximumNumberOfTags, bool * entityNamesAreSynonymous, string * word, string * otherWord, bool compareEntityNames, bool * senseOutputWithHighestTagsPassedNewSynsetMustFree)
{

	SynsetPtr currentSenseInList = firstSenseInList;
	SynsetPtr senseOutputWithHighestTags = firstSenseInList;
	*senseOutputWithHighestTagsPassedNewSynsetMustFree = false;
	*maximumNumberOfTags = 0;

	int senseIndex = 0;

	bool stillSensesToGo = true;
	while(stillSensesToGo)
	{
		#ifdef GIA_WORDNET_DEBUG
		//cout << "currentSenseInList->ptrcount = " << currentSenseInList->ptrcount << endl;
		#endif
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
					#ifdef GIA_WORDNET_DEBUG
					//cout << "similarityTypeIndex = " << similarityTypeIndex << endl;
					#endif
					if(currentSenseInList->ptrtyp[pointerIndex] == wordnetDataEntryPointersIndicatingRelatedSynsetsArray[similarityTypeIndex])
					{
						#ifdef GIA_WORDNET_DEBUG
						//cout << "passed" << endl;
						//cout << "currentSenseInList->ptrtyp[pointerIndex] = " << currentSenseInList->ptrtyp[pointerIndex] << endl;
						#endif
						//if ptrtyp indicates related synset, then go to ptroff
						passed = true;
						char * wordInterestedIn = const_cast<char*>(word->c_str());
						currentRelatedSense = read_synset(wordNetPOS, currentSenseInList->ptroff[pointerIndex], wordInterestedIn);
						passedNewSynsetMustFree = true;
					}
				}

				if(passed)
				{
					#ifdef GIA_WORDNET_DEBUG
					cout << "senseIndex = " << senseIndex << endl;
					cout << "currentRelatedSense->wcount = " << currentRelatedSense->wcount << endl;
					#endif
					if(compareEntityNames)
					{
						for(int w=0; w< currentRelatedSense->wcount; w++)
						{
							#ifdef GIA_WORDNET_DEBUG
							cout << "word = " << currentRelatedSense->words[w] << endl;
							#endif

							string currentWord = currentRelatedSense->words[w];
							if(currentWord == *otherWord)
							{
								*entityNamesAreSynonymous = true;
								#ifdef GIA_WORDNET_DEBUG
								cout << "match found - entityNamesAreSynonymous:" << endl;
								cout << "currentRelatedSense->words[w] = " << currentRelatedSense->words[w] << endl;
								cout << "otherWord = " << *otherWord << endl;
								#endif

								/*
								if(compareEntityNames)
								{
									if(tagCount > *maximumNumberOfTags)
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

						Index * idxOfFirstWordInWords = getindex(currentRelatedSense->words[0], wordNetPOS);	//returns pointer to Index struct representing first word in words[]
						int senseNumber = *(currentRelatedSense->wnsns);
						int tagCount = GetTagcnt(idxOfFirstWordInWords, senseNumber);

						#ifdef GIA_WORDNET_DEBUG
						cout << "whichword = " << currentRelatedSense->whichword << endl;		//which word in words[] corresponds to word/searchStr
						cout << "indexInData = " << currentRelatedSense->hereiam << endl;	      //index [first column] from data.wordNetPOS (eg data.noun) for the sense/usage of word/searchStr
						cout << "idxOfFirstWordInWords = " << idxOfFirstWordInWords << endl;
						cout << "idxOfFirstWordInWords->off_cnt " << idxOfFirstWordInWords->off_cnt << endl;  //total number of senses of first word in words[] (see index.wordNetPOS (eg index.noun) - the total number of pointers listed)
						cout << "senseNumber = " << senseNumber << endl;      //sense number of the first word in words[] (corresponding to the sense of word/searchStr) - second last column of sense.wordNetPOS (eg sense.noun)
						cout << "tagCount = " << tagCount << endl;	      //popularity of sense (number of times the sense passed has been tagged according to the cntlist file) - last column of sense.wordNetPOS (eg sense.noun)
						cout << " " << endl;
						#endif


						#ifdef GIA_FREE_MEMORY3
						free_index(idxOfFirstWordInWords);	//Free an index structure
						#endif


						if(tagCount > *maximumNumberOfTags)
						{
							*maximumNumberOfTags = tagCount;
							#ifdef GIA_FREE_MEMORY3
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
							#endif
							senseOutputWithHighestTags = currentRelatedSense;
						}
					}

					#ifdef GIA_WORDNET_DEBUG
					/*
					for(int q=1; q<= idxOfFirstWordInWords->off_cnt; q++)
					{
						cout << "tagcount = " << GetTagcnt(idxOfFirstWordInWords, q) << endl;
					}

					cout << "hereiam = " << currentRelatedSense->hereiam << endl;		//index [first column] from data.wordNetPOS (eg data.noun) for the sense/usage of word/searchStr
					cout << "sstype = " << currentRelatedSense->sstype << endl;
					cout << "fnum = " << currentRelatedSense->fnum << endl;
					cout << "pos = " << currentRelatedSense->pos << endl;
					cout << "wcount = " << currentRelatedSense->wcount << endl;
					cout << "words = " << currentRelatedSense->words << endl;
					cout << "lexid = " << currentRelatedSense->lexid << endl;
					cout << "wnsns = " << *(currentRelatedSense->wnsns) << endl;			//sense number of the first word in words[] - corresponding to the sense of word/searchStr
					cout << "whichword = " << currentRelatedSense->whichword << endl;		//which word in words[] corresponds to word/searchStr
					cout << "ptrcount = " << currentRelatedSense->ptrcount << endl;
					cout << "ptrtyp = " << currentRelatedSense->ptrtyp << endl;
					cout << "ptroff = " << currentRelatedSense->ptroff << endl;
					cout << "ppos = " << currentRelatedSense->ppos << endl;
					cout << "pto = " << currentRelatedSense->pto << endl;
					cout << "pfrm = " << currentRelatedSense->pfrm << endl;
					cout << "fcount = " << currentRelatedSense->fcount << endl;
					cout << "frmid = " << currentRelatedSense->frmid << endl;
					cout << "frmto = " << currentRelatedSense->frmto << endl;
					cout << "defn = " << currentRelatedSense->defn << endl;
					cout << "key = " << currentRelatedSense->key << endl;
					cout << "searchtype = " << currentRelatedSense->searchtype << endl;
					cout << " " << endl;
					*/
					#endif
				}

				#ifdef GIA_FREE_MEMORY3
				if(passedNewSynsetMustFree)
				{
					if(currentRelatedSense != senseOutputWithHighestTags)
					{
						free_synset(currentRelatedSense);	//Free a synset
					}
				}
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

	return senseOutputWithHighestTags;
}



void findSynonymsOLD(string word, bool * wordIsFound, string listOfSynonyms[], int wordNetPOS)
{
	bool result = true;

	int similarityType = SIMPTR;	//SIMPTR (similar) or SYNS (synonymn) - they both appear to give same output
	char * wordCharStar = const_cast<char*>(word.c_str());

	char * output = findtheinfo(wordCharStar, wordNetPOS, similarityType, 0);

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

	if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, lineString, CHAR_NEWLINE, CHAR_END_OF_STRING))	//wait till end of header
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
		if(c != CHAR_NEWLINE)
		{
			cout << "findSynonyms error: deformation (double new line not found)" << endl;
			cout << "charIndex = " << charIndex << endl;
			cout << "lineIndex = " << lineIndex << endl;
			exit(0);
		}
		charIndex++;
		lineIndex++;

		if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, lineString, CHAR_NEWLINE, CHAR_END_OF_STRING))	//wait till end of header
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
		#ifdef GIA_WORDNET_DEBUG
		//cout << "senseEntryTitleStringExpected = " << senseEntryTitleStringExpected << endl;
		#endif

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

		if(!recordUntilCharacterOrEscapeCharacterOLD(charIndex, output, &charIndex, lineString, CHAR_NEWLINE, CHAR_END_OF_STRING))	//wait till end of line
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



