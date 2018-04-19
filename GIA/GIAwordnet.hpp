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
 * File Name: GIAwordnet.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f3c 10-April-2018
 * Requirements: requires wordnet libraries to be installed
 * Description: Wordnet - searches wordnet database and parses wordnet output
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_WORDNET
#define HEADER_GIA_WORDNET

#include "GIAglobalDefs.hpp"
#include "wn.h"
#include "SHAREDvars.hpp"

#define SYNONYMN_DETECTION_STATUS_OFF 0
#define SYNONYMN_DETECTION_STATUS_QUERIES_ONLY 1
#define SYNONYMN_DETECTION_STATUS_QUERIES_AND_ADVANCED_REFERENCING 2

#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT (10000)
#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_LINE (1000)
#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_NUMBER_OF_SENSES (10)	//supports e^10 senses
#define WORDNET_FINDTHEINFO_OUTPUT_MAX_NUMBER_SYNONYMS (100)

#define WORDNET_DATA_ENTRY_POINTERS_INDICATING_SIMILAR_SYNSETS_NUMBER_OF_TYPES (1)
static int wordnetDataEntryPointersIndicatingSimilarSynsetsArray[WORDNET_DATA_ENTRY_POINTERS_INDICATING_SIMILAR_SYNSETS_NUMBER_OF_TYPES] = {SIMPTR};

//#define CURRENTSYNSET (99)

#ifdef WORDNET_SEARCH_RELATED_SYNSETS
	#define WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES (2)
	static int wordnetDataEntryPointersIndicatingRelatedSynsetsArray[WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES] = {SIMPTR, HYPOPTR};
#else
	#define WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES (0)
	static int wordnetDataEntryPointersIndicatingRelatedSynsetsArray[WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES] = {};
#endif
#define CURRENTSYNSETPOINTERINDEX (-1)

class GIAwordnetClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void initialiseWordNet(const int newSynonymnDetectionStatus);
	public: int getSynonymnDetectionStatus();

	public: bool checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(string* word, string* otherWord, const int wordNetPOS);
		private: bool checkIfWordIsContainedWithinAnotherWordsSynsets(const string* word, const string* otherWord, const int wordNetPOS);
	private: SynsetPtr findMostPopularSynsets(const string* word, bool* wordIsFound, const int wordNetPOS);
			private: SynsetPtr findSynsets(const string* word, bool* wordIsFound, const int wordNetPOS, const int similarityType);
			private: SynsetPtr checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(SynsetPtr firstSenseInList, const int wordNetPOS, int* maximumNumberOfTags, bool* entityNamesAreSynonymous, const string* word, const string* otherWord, const bool compareEntityNames, bool* senseOutputWithHighestTagsPassedNewSynsetMustFree);



	private: void findSynonymsOLD(const string word, bool* wordIsFound, string listOfSynonyms[], const int wordNetPOS);
	private: bool recordUntilCharacterOrEscapeCharacterOLD(int charIndex, const char* output, int* newCharIndex, string* lineString, const char characterToRecordUntil, const char escapeCharacter);
};

#endif
