/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
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
 * File Name: GIAlrp.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h17b 27-January-2015
 * Requirements: requires plain text file
 * Description: Language Reduction Preprocessor
 *
 *******************************************************************************/



#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include "GIAlrp.h"
#include "SHAREDvars.h"	//file io

static string lrpDataFolderName;
static bool useLRP;
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
GIALRPtag* firstTagInVerbListGlobal;
bool verbListLoaded;
#endif
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
GIALRPtag* firstTagInIrregularVerbListGlobal;
bool irregularVerbListLoaded;
#endif
#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
GIALRPtag* firstTagInPrepositionsInverseListGlobal;
bool prepositionsInverseListLoaded;
#endif

bool initialiseLRP(string newLRPDataFolderName, bool newUseLRP)
{
	bool result = true;

	useLRP = newUseLRP;
	lrpDataFolderName = newLRPDataFolderName;

	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
	string verbListFileName = lrpDataFolderName + GIA_LRP_VERB_DATABASE_FILE_NAME;
	verbListLoaded = false;
	firstTagInVerbListGlobal = new GIALRPtag();
	if(!loadVerbList(verbListFileName, firstTagInVerbListGlobal))
	{
		cout << "!loadVerbList (OpenGIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL requires -lrpfolder to be set): verbListFileName = " << verbListFileName << endl;
		result = false;
	}
	else
	{
		verbListLoaded = true;
	}
	#endif
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
	string irregularVerbListFileName = lrpDataFolderName + GIA_LRP_IRREGULARVERB_DATABASE_FILE_NAME;
	irregularVerbListLoaded = false;
	firstTagInIrregularVerbListGlobal = new GIALRPtag();
	if(!loadIrregularVerbList(irregularVerbListFileName, firstTagInIrregularVerbListGlobal))
	{
		cout << "!loadIrregularVerbList (OpenGIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE requires -lrpfolder to be set): irregularVerbListFileName = " << irregularVerbListFileName << endl;
		result = false;
	}
	else
	{
		irregularVerbListLoaded = true;
	}
	#endif
	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	string prepositionsInverseListFileName = lrpDataFolderName + GIA_LRP_PREPOSITIONS_DATABASE_FILE_NAME;
	prepositionsInverseListLoaded = false;
	firstTagInPrepositionsInverseListGlobal = new GIALRPtag();
	if(!loadPrepositionsInverseList(prepositionsInverseListFileName, firstTagInPrepositionsInverseListGlobal))
	{
		cout << "!loadPrepositionsInverseList (OpenGIA with GIA_LRP_NORMALISE_PREPOSITIONS requires -lrpfolder to be set): prepositionsInverseListFileName = " << prepositionsInverseListFileName << endl;
		result = false;
	}
	else
	{
		prepositionsInverseListLoaded = true;
	}
	#endif
		
	return result;
}
bool getUseLRP()
{
	return useLRP;
}

GIALRPtag::GIALRPtag(void)
{
	tagName = "";
	tagNameLemma = "";

	tagSpecialArbitraryName = false;
	tagSpecialArbitraryNameType = GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_UNDEFINED;

	optional = false;

	/*
	primary = false;
	primaryPhrasalVerbReplacementString = "";
	primaryPhrasalVerbReplacementStringNLPonly = "";
	*/

	base = false;
	for(int i=0; i<GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
	{
		grammaticalTenseFormsArray[i][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = "";
		grammaticalTenseFormsArray[i][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = "";
	}
	grammaticalTenseFormDetected = GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE;

	entityIndex = false;
	sentenceIndex = false;
	collapsedPhrasalVerbExactDefinedSection = false;
	collapsedMultiwordPreposition = false;

	alternateTag = NULL;
	alternateSentence = NULL;
	nextTag = NULL;
	nextSentence = NULL;
}
GIALRPtag::~GIALRPtag(void)
{
}


GIALRPtagTextCorrespondenceInfo::GIALRPtagTextCorrespondenceInfo(void)
{
	wordWithLRP = "";
	wordWithLRPforNLPonly = "";

	entityIndex = INT_DEFAULT_VALUE;
	sentenceIndex = INT_DEFAULT_VALUE;

	next = NULL;
}
GIALRPtagTextCorrespondenceInfo::~GIALRPtagTextCorrespondenceInfo(void)
{
}

GIALRPtagTextCorrespondenceInfo* textGIALRPtagTextCorrespondenceInfo;
GIALRPtagTextCorrespondenceInfo* queryGIALRPtagTextCorrespondenceInfo;
GIALRPtagTextCorrespondenceInfo* currentGIALRPtagTextCorrespondenceInfo;
GIALRPtagTextCorrespondenceInfo* getCurrentGIALRPtagTextCorrespondenceInfo()
{
	return currentGIALRPtagTextCorrespondenceInfo;
}
void setCurrentGIALRPtagTextCorrespondenceInfo(bool isQuery)
{
	if(isQuery)
	{
		currentGIALRPtagTextCorrespondenceInfo = queryGIALRPtagTextCorrespondenceInfo;
	}
	else
	{
		currentGIALRPtagTextCorrespondenceInfo = textGIALRPtagTextCorrespondenceInfo;
	}
}
void initialiseCurrentGIALRPtagTextCorrespondenceInfo(bool isQuery)
{
	if(isQuery)
	{
		queryGIALRPtagTextCorrespondenceInfo = new GIALRPtagTextCorrespondenceInfo();
	}
	else
	{
		textGIALRPtagTextCorrespondenceInfo = new GIALRPtagTextCorrespondenceInfo();
	}
}
void deinitialiseCurrentGIALRPtagTextCorrespondenceInfo(bool isQuery)
{
	if(isQuery)
	{
		delete queryGIALRPtagTextCorrespondenceInfo;
	}
	else
	{
		delete textGIALRPtagTextCorrespondenceInfo;
	}
}


bool parseTextFileAndReduceLanguage(string plainTextInputFileName, string plainTextLRPoutputFileName, string plainTextLRPforNLPoutputFileName)
{
	bool result = true;

	#ifdef GIA_LRP_DEBUG
	cout << "lrpDataFolderName= " << lrpDataFolderName << endl;
	cout << "plainTextInputFileName = " << plainTextInputFileName << endl;
	cout << "plainTextLRPoutputFileName = " << plainTextLRPoutputFileName << endl;
	cout << "plainTextLRPforNLPoutputFileName = " << plainTextLRPforNLPoutputFileName << endl;
	#endif

	string irregularVerbListFileName = lrpDataFolderName + GIA_LRP_IRREGULARVERB_DATABASE_FILE_NAME;
	GIALRPtag* firstTagInIrregularVerbList = new GIALRPtag();
	if(!loadIrregularVerbList(irregularVerbListFileName, firstTagInIrregularVerbList))
	{
		result = false;
	}

	string phrasalVerbListFileName = lrpDataFolderName + GIA_LRP_PHRASALVERB_DATABASE_FILE_NAME;
	GIALRPtag* firstTagInPhrasalVerbList = new GIALRPtag();
	if(!loadPhrasalVerbDataAndGenerateAllTenseVariants(phrasalVerbListFileName, firstTagInPhrasalVerbList, firstTagInIrregularVerbList))
	{
		result = false;
	}

	string multiwordPrepositionListFileName = lrpDataFolderName + GIA_LRP_MULTIWORDPREPOSITION_DATABASE_FILE_NAME;
	GIALRPtag* firstTagInMultiwordPrepositionList = new GIALRPtag();
	if(!loadMultiWordPrepositionData(multiwordPrepositionListFileName, firstTagInMultiwordPrepositionList))
	{
		result = false;
	}

	GIALRPtag* firstTagInPlainText = new GIALRPtag();
	if(!loadPlainTextFile(plainTextInputFileName, firstTagInPlainText))
	{
		result = false;
	}
	
	setCurrentDirectory(tempFolderCharStar);

	GIALRPtagTextCorrespondenceInfo* firstGIALRPtagCorrespondenceInfo = getCurrentGIALRPtagTextCorrespondenceInfo();
	if(!searchAndReplacePhrasalVerbs(firstTagInPlainText, firstTagInPhrasalVerbList, firstGIALRPtagCorrespondenceInfo))
	{
		result = false;
	}

	if(!searchAndReplaceMultiwordPrepositions(firstTagInPlainText, firstTagInMultiwordPrepositionList, firstGIALRPtagCorrespondenceInfo))
	{
		result = false;
	}	
	
	if(!writeTagListToFile(firstTagInPlainText, plainTextLRPoutputFileName, plainTextLRPforNLPoutputFileName))
	{
		result = false;
	}

	return result;
}

#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
bool loadVerbList(string irregularVerbListFileName, GIALRPtag* firstTagInIrregularVerbList)
{
	bool result = true;

	GIALRPtag* currentTagInIrregularVerbList = firstTagInIrregularVerbList;

	ifstream parseFileObject(irregularVerbListFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//txt file does not exist in current directory.
		cout << "Error: Irregular Verb Database File does not exist in current directory: " << irregularVerbListFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		int wordIndex = 0;
		string currentWord = "";
		bool currentWordAlternate = false;
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEWLINE)
			{
				currentTagInIrregularVerbList->tagName = currentWord;
				currentTagInIrregularVerbList->nextSentence = new GIALRPtag();
				currentTagInIrregularVerbList = currentTagInIrregularVerbList->nextSentence;
				currentWord = "";

			}
			else
			{
				currentWord = currentWord + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}

	return result;
}
#endif


bool loadIrregularVerbList(string irregularVerbListFileName, GIALRPtag* firstTagInIrregularVerbList)
{
	bool result = true;

	GIALRPtag* currentTagInIrregularVerbList = firstTagInIrregularVerbList;
	GIALRPtag* firstTagInIrregularVerb = currentTagInIrregularVerbList;
	GIALRPtag* currentTagInIrregularVerb = firstTagInIrregularVerb;

	ifstream parseFileObject(irregularVerbListFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//txt file does not exist in current directory.
		cout << "Error: Irregular Verb Database File does not exist in current directory: " << irregularVerbListFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		int wordIndex = 0;
		string currentWord = "";
		bool currentWordAlternate = false;
		while(parseFileObject.get(currentToken))
		{
			if((currentToken == CHAR_NEWLINE) || (currentToken == CHAR_SPACE))
			{
				//create "LRP tag" to store phrasal verb base or past/past participle tense variant
				if(currentWordAlternate)
				{
					currentTagInIrregularVerb->alternateTag = new GIALRPtag();
					currentTagInIrregularVerb->alternateTag->tagName = currentWord;
				}
				else
				{
					currentTagInIrregularVerb->tagName = currentWord;
				}
				#ifdef GIA_LRP_DEBUG
				cout <<	"adding Tag: tagName = " << currentTagInIrregularVerb->tagName << endl;
				if(currentWordAlternate)
				{
					cout <<	"adding Tag: alternateTag = " << currentTagInIrregularVerb->alternateTag->tagName << endl;
				}
				#endif

				currentTagInIrregularVerb->nextTag = new GIALRPtag();
				currentTagInIrregularVerb = currentTagInIrregularVerb->nextTag;

				if(currentToken == CHAR_NEWLINE)
				{
					currentTagInIrregularVerbList->nextSentence = new GIALRPtag();
					currentTagInIrregularVerbList = currentTagInIrregularVerbList->nextSentence;
					currentTagInIrregularVerb = currentTagInIrregularVerbList;
				}

				wordIndex = 0;
				currentWord = "";
				currentWordAlternate = false;

				//start parsing new line

				//UP TO HERE
			}
			else if(currentToken == GIA_LRP_PHRASALVERB_DATABASE_TAG_ALTERNATE)
			{
				currentTagInIrregularVerb->tagName = currentWord;

				//wordIndex does not change...
				currentWordAlternate = true;
				currentWord = "";
			}
			else
			{
				currentWord = currentWord + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}

	return result;
}


/*
---
LRP rules;
add alternate phrasal verb based on 'or'
add alternate tag based upon '/'
take (at least) first word in phrasal verb definition string as base
generate all tenses variations of the verb based upon a) rules and b) irregular verbs
*/

//NB current implementation cannot take into account 3 alternate tags (ie x/y/z)
bool loadPhrasalVerbDataAndGenerateAllTenseVariants(string phrasalVerbDatabaseFileName, GIALRPtag* firstTagInPhrasalVerbList, GIALRPtag* firstTagInIrregularVerbList)
{
	bool result = true;

	GIALRPtag* currentTagInPhrasalVerbList = firstTagInPhrasalVerbList;
	GIALRPtag* recordOfNonAlternateTagInPhrasalVerbList = currentTagInPhrasalVerbList;

	GIALRPtag* currentTagInPhrasalVerb = firstTagInPhrasalVerbList;
	GIALRPtag* recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;

	ifstream parseFileObject(phrasalVerbDatabaseFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//txt file does not exist in current directory.
		cout << "Error: Phrasal Verb Database File does not exist in current directory: " << phrasalVerbDatabaseFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		bool parsingVerbBase = true;
		string currentWord = "";
		bool currentWordOptional = false;
		bool currentWordAlternate = false;
		bool currentPhrasalVerbAlternate = false;
		string phrasalVerbBaseNameRecord = "";
		while(parseFileObject.get(currentToken))
		{
			if((currentToken == CHAR_NEWLINE) || (currentToken == CHAR_SPACE))
			{
				if(currentWord == GIA_LRP_PHRASALVERB_DATABASE_TAG_OR)
				{
					currentTagInPhrasalVerbList->alternateSentence = new GIALRPtag();
					currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->alternateSentence;
					currentTagInPhrasalVerb = currentTagInPhrasalVerbList;
					currentPhrasalVerbAlternate = true;
				}
				else
				{//(currentToken == CHAR_NEWLINE) || (currentToken == CHAR_SPACE)


					//COPY1
					//add LRP tag to LRP string (LRP tag list)
					currentTagInPhrasalVerb->tagName = currentWord;
					if(currentWordOptional)
					{
						currentTagInPhrasalVerb->optional = true;
					}
					if(parsingVerbBase)
					{
						currentTagInPhrasalVerb->base = true;
					}
					else
					{
						if(currentWord == phrasalVerbBaseNameRecord)
						{
							currentTagInPhrasalVerb->base = true;
						}
					}
					bool foundTagArbitraryName = false;
					for(int i=0; i<GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES; i++)
					{
						if(currentWord == lrpPhrasalVerbDatabaseTagArbitraryNameArray[i])
						{
							foundTagArbitraryName = true;
							currentTagInPhrasalVerb->tagSpecialArbitraryName = true;
							currentTagInPhrasalVerb->tagSpecialArbitraryNameType = i;
						}
					}
					if(currentTagInPhrasalVerb->base)
					{
						generateTenseVariantsOfVerbBase(currentTagInPhrasalVerb, firstTagInIrregularVerbList);
					}
					#ifdef GIA_LRP_DEBUG
					cout <<	"adding Tag: tagName = " << currentTagInPhrasalVerb->tagName << endl;
					if(currentWordAlternate)
					{
						cout << "(alternate)" << endl;
					}
					#endif


					currentTagInPhrasalVerb->nextTag = new GIALRPtag();
					currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
					if(currentWordAlternate)
					{
						//revert to non-alternate tag...
						currentTagInPhrasalVerb = recordOfNonAlternateTagInPhrasalVerb;
						currentTagInPhrasalVerb->nextTag = new GIALRPtag();
						currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
					}

					if(currentToken == CHAR_NEWLINE)
					{
						#ifdef GIA_LRP_DEBUG
						//cout << "adding Tag: tagName = " << currentTagInPhrasalVerbList->tagName << endl;
						#endif
						currentTagInPhrasalVerbList->nextSentence = new GIALRPtag();
						currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->nextSentence;
						if(currentPhrasalVerbAlternate)
						{
							//revert to non-alternate phrasal verb...
							#ifdef GIA_LRP_DEBUG
							cout << "currentPhrasalVerbAlternate" << endl;
							#endif
							currentTagInPhrasalVerbList = recordOfNonAlternateTagInPhrasalVerbList;
							currentTagInPhrasalVerbList->nextSentence = new GIALRPtag();
							currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->nextSentence;
							currentPhrasalVerbAlternate = false;
						}
						currentTagInPhrasalVerb = currentTagInPhrasalVerbList;
						recordOfNonAlternateTagInPhrasalVerbList = currentTagInPhrasalVerbList;
						recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;
					}

					if(!currentWordAlternate)
					{
						recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;
					}
					if(parsingVerbBase)
					{
						phrasalVerbBaseNameRecord = currentWord;
					}
				}

				parsingVerbBase = false;
				//currentWordOptional = false;	//removed 1p1aTEMP5b
				currentWordAlternate = false;
				currentWord = "";
				if((currentToken == CHAR_NEWLINE) || (currentWord == GIA_LRP_PHRASALVERB_DATABASE_TAG_OR))
				{
					parsingVerbBase = true;
					phrasalVerbBaseNameRecord = "";
				}
				//start parsing new

			}
			else if(currentToken == GIA_LRP_PHRASALVERB_DATABASE_TAG_ALTERNATE)
			{

				//COPY2

				currentTagInPhrasalVerb->tagName = currentWord;		//NB current implementation can take into account 2 or more alternate tags (eg x/y/z)...
				#ifdef GIA_LRP_DEBUG
				cout <<	"adding Tag: tagName = " << currentTagInPhrasalVerb->tagName << endl;
				if(currentWordAlternate)
				{
					cout << "(alternate)" << endl;
				}
				#endif
				if(currentWordOptional)
				{
					currentTagInPhrasalVerb->optional = true;
				}
				if(parsingVerbBase)
				{
					//cout << "warning: parsingVerbBase && (currentToken == GIA_LRP_PHRASALVERB_DATABASE_TAG_ALTERNATE) - ignoring second alternate base" << endl;
					currentTagInPhrasalVerb->base = true;
				}
				else
				{
					if(currentWord == phrasalVerbBaseNameRecord)
					{
						currentTagInPhrasalVerb->base = true;
					}
				}
				bool foundTagArbitraryName = false;
				for(int i=0; i<GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES; i++)
				{
					if(currentWord == lrpPhrasalVerbDatabaseTagArbitraryNameArray[i])
					{
						foundTagArbitraryName = true;
						currentTagInPhrasalVerb->tagSpecialArbitraryName = true;
						currentTagInPhrasalVerb->tagSpecialArbitraryNameType = i;
					}
				}
				if(currentTagInPhrasalVerb->base)
				{
					generateTenseVariantsOfVerbBase(currentTagInPhrasalVerb, firstTagInIrregularVerbList);
				}
				#ifdef GIA_LRP_DEBUG
				cout <<	"adding Tag: tagName = " << currentTagInPhrasalVerb->tagName << endl;
				cout << "(alternate)" << endl;
				#endif
				if(!currentWordAlternate)
				{//added 1p1aTEMP6d
					recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;
				}

				currentTagInPhrasalVerb->alternateTag = new GIALRPtag();
				currentTagInPhrasalVerb = currentTagInPhrasalVerb->alternateTag;

				//wordIndex does not change...
				//parsingVerbBase = false;		//should not be parsing verb base here, as it should never have alternate cases eg base/base
				currentWordAlternate = true;
				//currentWordOptional = false;	//removed 1p1aTEMP5b
				currentWord = "";
			}
			else if(currentToken == GIA_LRP_PHRASALVERB_DATABASE_TAG_OPEN_OPTIONAL)
			{
				currentWordOptional = true;
			}
			else if(currentToken == GIA_LRP_PHRASALVERB_DATABASE_TAG_CLOSE_OPTIONAL)
			{//moved 1p1aTEMP5b
				currentWordOptional = false;
			}
			else
			{
				currentWord = currentWord + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}


	#ifdef GIA_LRP_DEBUG
	currentTagInPhrasalVerbList = firstTagInPhrasalVerbList;
	while(currentTagInPhrasalVerbList->nextSentence != NULL)
	{
		cout << "currentTagInPhrasalVerbList->tagName = " << currentTagInPhrasalVerbList->tagName << endl;
		currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->nextSentence;
	}
	//exit(0);
	#endif


	return result;
}



bool generateTenseVariantsOfVerbBase(GIALRPtag* baseTag, GIALRPtag* firstTagInIrregularVerbList)
{
	bool result = true;

	string base = convertStringToLowerCase(&(baseTag->tagName));

	//1. check if irregular past / past participle case
	bool irregularVerbFound = false;
	GIALRPtag* currentTagInIrregularVerbList = firstTagInIrregularVerbList;
	while(currentTagInIrregularVerbList->nextSentence != NULL)
	{
		if(currentTagInIrregularVerbList->tagName == base)
		{
			irregularVerbFound = true;
			GIALRPtag* firstTagInIrregularVerb = currentTagInIrregularVerbList;
			GIALRPtag* currentTagInIrregularVerb = firstTagInIrregularVerb->nextTag;
			int irregularVerbTagIndex = GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST;
			while(currentTagInIrregularVerb->nextTag != NULL)
			{
				//first index is past, second is past participle
				if(irregularVerbTagIndex == GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST)
				{
					baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = currentTagInIrregularVerb->tagName;
					if(currentTagInIrregularVerb->alternateTag != NULL)
					{
						baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->alternateTag->tagName;
					}
					else
					{
						baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->tagName;	//just use the same version (standard)
					}
				}
				else if(irregularVerbTagIndex == GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE)
				{
					baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = currentTagInIrregularVerb->tagName;
					if(currentTagInIrregularVerb->alternateTag != NULL)
					{
						baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->alternateTag->tagName;
					}
					else
					{
						baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->tagName;	//just use the same version (standard)
					}
				}

				irregularVerbTagIndex++;
				currentTagInIrregularVerb = currentTagInIrregularVerb->nextTag;
			}
		}
		currentTagInIrregularVerbList = currentTagInIrregularVerbList->nextSentence;
	}

	//2. generate verb tenses based upon rules (now wouldn't it be simpler if universities just published their data?) - http://a4esl.org/q/h/9807/km-doubles.html
	int baseStringLength = base.length();
	int indexOfLastCharacterInBase = baseStringLength-1;
	int indexOfSecondLastCharacterInBase = baseStringLength-2;
	int indexOfThirdLastCharacterInBase = baseStringLength-3;
	char lastCharacterInBase = base[indexOfLastCharacterInBase];
	char secondLastCharacterInBase = base[indexOfSecondLastCharacterInBase];
	char thirdLastCharacterInBase = base[indexOfThirdLastCharacterInBase];
	#ifdef GIA_LRP_DEBUG
	/*
	cout << "baseStringLength = " << baseStringLength << endl;
	cout << "indexOfLastCharacterInBase = " << indexOfLastCharacterInBase << endl;
	cout << "indexOfSecondLastCharacterInBase = " << indexOfSecondLastCharacterInBase << endl;
	cout << "indexOfThirdLastCharacterInBase = " << indexOfThirdLastCharacterInBase << endl;
	cout << "lastCharacterInBase = " << lastCharacterInBase << endl;
	cout << "secondLastCharacterInBase = " << secondLastCharacterInBase << endl;
	cout << "thirdLastCharacterInBase = " << thirdLastCharacterInBase << endl;
	*/
	#endif
	bool lastCharacterIsVowel = false;
	bool secondLastCharacterIsVowel = false;
	bool thirdLastCharacterIsVowel = false;
	//bool verbDoubleConsonantRule1LastLetterException = false;
	for(int i = 0; i<GIA_LRP_NUMBER_OF_VOWELS; i++)
	{
		if(lastCharacterInBase == englishVowelArray[i])
		{
			lastCharacterIsVowel = true;
		}
	}
	for(int i = 0; i<GIA_LRP_NUMBER_OF_VOWELS; i++)
	{
		if(secondLastCharacterInBase == englishVowelArray[i])
		{
			secondLastCharacterIsVowel = true;
		}
	}
	for(int i = 0; i<GIA_LRP_NUMBER_OF_VOWELS; i++)
	{
		if(thirdLastCharacterInBase == englishVowelArray[i])
		{
			thirdLastCharacterIsVowel = true;
		}
	}
	/*
	for(int i = 0; i<GIA_LRP_VERB_DOUBLE_CONSONANT_RULE1_LAST_LETTER_EXCEPTIONS_NUMBER_OF_TYPES; i++)
	{
		if(base[indexOfLastCharacterInBase] == lrpVerbDoubleConsonantRuleOneLastLetterExceptions[i])
		{
			verbDoubleConsonantRule1LastLetterException = true;
		}
	}
	*/

	bool rule1 = false;
	bool rule1b = false;
	bool rule2 = false;
	bool rule3a = false;
	bool rule3b = false;
	bool rule4 = false;

	if(!lastCharacterIsVowel && secondLastCharacterIsVowel && !thirdLastCharacterIsVowel)
	{
		// && !verbDoubleConsonantRule1LastLetterException - this requirement cannot be asserted as it is apparently only enforced for one syllable words
		rule1 = true;
		rule1b = true; //unknown [therefore have to assume both cases: alternate+ non-alternate)	//Two-syllable words: ED = If the stress is on the first syllable, the word only gets one consonant
	}

	if(lastCharacterInBase == 'e')
	{
		rule2 = true;
	}

	if(lastCharacterInBase == 'y')
	{
		if(secondLastCharacterIsVowel)
		{
			rule3b = true;
		}
		else
		{
			rule3a = true;
		}
	}

	if((!lastCharacterIsVowel && secondLastCharacterIsVowel && thirdLastCharacterIsVowel) || (!lastCharacterIsVowel && !secondLastCharacterIsVowel))
	{
		rule4 = true;
	}

	string baseWithLastLetterDropped = base.substr(0, baseStringLength-1);
	#ifdef GIA_LRP_DEBUG
	//cout << "baseWithLastLetterDropped = " << baseWithLastLetterDropped << endl;
	#endif

	//generate (ie record) infinitive tense form [for consistency; even though this is the same as tagName...]
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base;
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base;

	//Rule 1: Words ending with a Consonant-Vowel-Consonant Pattern
	if(rule1)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule1)" << endl;
		#endif

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;
		if(rule1b)
		{
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;
		}

		//b. generate continuous tense form
		//happening/entering
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;
		if(rule1b)
		{
			//hopping/sitting
			//beginning/permitting
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;	//double consonant
		}

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//visited/opened
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
			if(rule1b)
			{
				//rubbed/stopped
				//referred/admitted
				baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;	//double consonant
			}

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
			if(rule1b)
			{
				baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;	//double consonant
			}
		}

		if(!rule1b)
		{
			copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
		}
	}
	//Rule 2: Words ending in E
	else if(rule2)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule2)" << endl;
		#endif

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;

		//b. generate continuous tense form
		//dancing/skating
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//smiled/fined
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
		}

		copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}
	//Rule 3: Words ending in Y
	else if(rule3a)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule3a)" << endl;
		#endif

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_CASE3;

		//b. generate continuous tense form
		//carrying/replying
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//studied/married
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3;
		}

		copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}
	//Rule 4: Other words...
	else if(rule3b || rule4)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule3b || rule4)" << endl;
		cout << "rule3b = " << rule3b << endl;
		cout << "rule4 = " << rule4 << endl;
		#endif

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;

		//b. generate continuous tense form
		//enjoying/straying
		//needing/beeping
		//needing/laughing
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//played/stayed
			//dreamed/rained
			//parked/earned
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
		}

		copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}

	#ifdef GIA_LRP_DEBUG
	cout << "baseTag->tagName = " << baseTag->tagName << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][STANDARD] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][STANDARD] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD]  << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][STANDARD] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD]  << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][STANDARD] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD]  << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][ALTERNATE] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][ALTERNATE] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE]  << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][ALTERNATE] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE]  << endl;
	cout << "baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][ALTERNATE] = " << baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE]  << endl;
	#endif


	return result;
}

void copyDefaultVerbTenseFormsToAlternateTenseForms(GIALRPtag* baseTag, bool irregularVerbFound)
{
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	if(!irregularVerbFound)
	{
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	}

}

bool loadMultiWordPrepositionData(string multiwordPrepositionListFileName, GIALRPtag* firstTagInMultiwordPrepositionList)
{
	bool result = true;

	GIALRPtag* currentTagInMultiwordPrepositionList = firstTagInMultiwordPrepositionList;
	GIALRPtag* firstTagInMultiwordPreposition = currentTagInMultiwordPrepositionList;
	GIALRPtag* currentTagInMultiwordPreposition = firstTagInMultiwordPreposition;

	ifstream parseFileObject(multiwordPrepositionListFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//txt file does not exist in current directory.
		cout << "Error: Multiword Preposition Database File does not exist in current directory: " << multiwordPrepositionListFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		string currentMultiwordPreposition = "";	//not used [multiword prepositions are matched word by word; like phrasal verbs]
		string currentPreposition = "";
		while(parseFileObject.get(currentToken))
		{
			if((currentToken == CHAR_NEWLINE) || (currentToken == CHAR_SPACE))
			{
				currentTagInMultiwordPreposition->tagName = currentPreposition;
				#ifdef GIA_LRP_DEBUG
				cout <<  "adding Tag: " << currentTagInMultiwordPreposition->tagName << endl;
				#endif
				currentTagInMultiwordPreposition->nextTag = new GIALRPtag();
				currentTagInMultiwordPreposition = currentTagInMultiwordPreposition->nextTag;
				if(currentToken == CHAR_NEWLINE)
				{
					currentTagInMultiwordPrepositionList->nextSentence = new GIALRPtag();
					currentTagInMultiwordPrepositionList = currentTagInMultiwordPrepositionList->nextSentence;
					currentTagInMultiwordPreposition = currentTagInMultiwordPrepositionList;
					currentMultiwordPreposition = "";
				}
				else if(currentToken == CHAR_SPACE)
				{
					currentMultiwordPreposition = currentMultiwordPreposition + currentToken;
				}
				currentPreposition = "";
			}
			else
			{
				currentPreposition = currentPreposition + currentToken;
				currentMultiwordPreposition = currentMultiwordPreposition + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}
	return result;
}

bool loadPlainTextFile(string plainTextInputFileName, GIALRPtag* firstTagInPlainText)
{
	bool result = true;

	GIALRPtag* currentTagInPlainText = firstTagInPlainText;
	GIALRPtag* firstTagInPlainTextSentence = currentTagInPlainText;
	GIALRPtag* currentTagInPlainTextSentence = firstTagInPlainTextSentence;

	ifstream parseFileObject(plainTextInputFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//txt file does not exist in current directory.
		cout << "Error: Plain Text Input File does not exist in current directory: " << plainTextInputFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		bool whiteSpace = false;
		string currentWord = "";
		int entityIndex = GIA_NLP_START_ENTITY_INDEX;	//only assigned after collapse?
		int sentenceIndex = GIA_NLP_START_SENTENCE_INDEX;	//only assigned after collapse?
		#ifdef GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS
		bool readingQuotation = false;
		#endif
		while(parseFileObject.get(currentToken))
		{
			bool punctuationMarkFound = false;
			for(int i=0; i<GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS; i++)
			{
				if(currentToken == nlpPunctionMarkCharacterArray[i])
				{
					punctuationMarkFound = true;
				}
			}
			bool whiteSpaceFound = false;
			for(int i=0; i<GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS; i++)
			{
				if(currentToken == nlpWhitespaceCharacterArray[i])
				{
					whiteSpaceFound = true;
				}
			}
			#ifdef GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS
			bool quotationMarkFound = false;
			for(int i=0; i<GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS; i++)
			{
				if(currentToken == nlpQuotationMarkCharacterArray[i])
				{
					quotationMarkFound = true;
				}
			}
			if(quotationMarkFound)
			{//NB imbedded/recursive quotation marks not currently supported eg "'hello'"
				if(!readingQuotation)
				{
					readingQuotation = true;
				}
				else
				{
					readingQuotation = false;
				}
				if(!whiteSpace)
				{
					currentTagInPlainTextSentence->tagName = currentWord;
					currentTagInPlainTextSentence->entityIndex = entityIndex;
					currentTagInPlainTextSentence->sentenceIndex = sentenceIndex;
					currentTagInPlainTextSentence->nextTag = new GIALRPtag();
					#ifdef GIA_LRP_DEBUG
					cout <<  "adding Tag: " << currentTagInPlainTextSentence->tagName << endl;
					//cout << "currentTagInPlainTextSentence->sentenceIndex = " << currentTagInPlainTextSentence->sentenceIndex << endl;
					#endif
					currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
					entityIndex++;
					currentWord = "";
				}
			}
			else
			{
			#endif
				if(whiteSpaceFound || punctuationMarkFound)
				{
					#ifdef GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS
					if(readingQuotation)
					{
						currentWord = currentWord + GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS_FILLER;
					}
					else
					{
					#endif
						if(!whiteSpace)
						{
							#ifdef GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS
							if(currentWord != "")
							{//do not add empty tag after closing quotation marks
							#endif
								currentTagInPlainTextSentence->tagName = currentWord;
								currentTagInPlainTextSentence->entityIndex = entityIndex;
								currentTagInPlainTextSentence->sentenceIndex = sentenceIndex;
								currentTagInPlainTextSentence->nextTag = new GIALRPtag();
								#ifdef GIA_LRP_DEBUG
								cout <<  "adding Tag: " << currentTagInPlainTextSentence->tagName << endl;
								//cout << "currentTagInPlainTextSentence->sentenceIndex = " << currentTagInPlainTextSentence->sentenceIndex << endl;
								#endif
								currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
								entityIndex++;
							#ifdef GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS
							}
							#endif
							if(punctuationMarkFound)
							{
								currentTagInPlainTextSentence->tagName = currentToken;
								currentTagInPlainTextSentence->entityIndex = entityIndex;
								currentTagInPlainTextSentence->sentenceIndex = sentenceIndex;
								currentTagInPlainTextSentence->nextTag = new GIALRPtag();
								#ifdef GIA_LRP_DEBUG
								cout <<  "adding Tag: " << currentTagInPlainTextSentence->tagName << endl;
								#endif
								currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;

								bool endOfSentencePunctuationMarkFound = false;
								for(int i=0; i<GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE; i++)
								{
									if(currentToken == nlpPunctionMarkCharacterEndOfSentenceArray[i])
									{
										endOfSentencePunctuationMarkFound = true;
									}
								}
								if(endOfSentencePunctuationMarkFound)
								{
									#ifdef GIA_LRP_DEBUG
									//cout << "endOfSentencePunctuationMarkFound" << endl;
									//cout << "currentTagInPlainText->sentenceIndex = " << currentTagInPlainText->sentenceIndex << endl;
									#endif
									currentTagInPlainText->nextSentence = new GIALRPtag();
									currentTagInPlainText = currentTagInPlainText->nextSentence;
									currentTagInPlainTextSentence = currentTagInPlainText;
									sentenceIndex++;
									entityIndex = 0;
								}
								else
								{
									entityIndex++;
								}
							}
							currentWord = "";
						}
						else
						{//skip (do not parse) multiple white space/punctuation characters (eg ". ")
						}
						whiteSpace = true;
					#ifdef GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS
					}
					#endif
				}
				else
				{
					whiteSpace = false;
					currentWord = currentWord + currentToken;
				}
			#ifdef GIA_LRP_REDUCE_QUOTES_TO_SINGLE_WORDS
			}
			#endif
			charCount++;
		}
		parseFileObject.close();
	}

	#ifdef GIA_LRP_DEBUG
	currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{
		firstTagInPlainTextSentence = currentTagInPlainText;
		currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			cout << "currentTagInPlainTextSentence->tagName = " << currentTagInPlainTextSentence->tagName << endl;

			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}
		currentTagInPlainText = currentTagInPlainText->nextSentence;
	}
	#endif

	return result;
}


//NB the collapsed phrasal verb contains precisely 2 entities: phrasalVerbCollapsed, entity2: thing/place/body (eg belong_to + sb/Tom) - thing/place/bodies are not currently being differentiated by the LRP as this information is only first generated at NLP/GIA parse stage
bool searchAndReplacePhrasalVerbs(GIALRPtag* firstTagInPlainText, GIALRPtag* firstTagInPhrasalVerbList, GIALRPtagTextCorrespondenceInfo* firstGIALRPtagCorrespondenceInfo)
{
	bool result = true;

	GIALRPtagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIALRPtagCorrespondenceInfo;	//new correspondence info for each found phrasal verb

	GIALRPtag* currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{
		#ifdef GIA_LRP_DEBUG
		//cout << "qcurrentTagInPlainText->sentenceIndex = " << currentTagInPlainText->sentenceIndex << endl;
		#endif

		GIALRPtag* firstTagInPlainTextSentence = currentTagInPlainText;
		GIALRPtag* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIALRPtag* previousTagInPlainTextSentence = NULL;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			#ifdef GIA_LRP_DEBUG
			//cout << "currentTagInPlainTextSentence->tagName = " << currentTagInPlainTextSentence->tagName << endl;
			#endif

			bool foundAtLeastOnePhrasalVerbInSentenceAndCollapsed = false;
			GIALRPtag* currentTagInPhrasalVerbList = firstTagInPhrasalVerbList;
			while(currentTagInPhrasalVerbList->nextSentence != NULL)
			{
				#ifdef GIA_LRP_DEBUG
				//cout << "currentTagInPhrasalVerbList->tagName = " << currentTagInPhrasalVerbList->tagName << endl;
				#endif

				GIALRPtag* currentTagInPhrasalVerbListAlternate = currentTagInPhrasalVerbList;
				bool alternatePhrasalVerb = false;
				while(!alternatePhrasalVerb || (currentTagInPhrasalVerbListAlternate->alternateSentence != NULL))
				{
					/*
					if(tolower((currentTagInPhrasalVerbList->tagName)[0]) == tolower((currentTagInPlainTextSentence->tagName)[0]))
					{//optimisation; only deal with phrasal verbs that start with the same character...
					*/
					GIALRPtag* firstTagInPhrasalVerb = currentTagInPhrasalVerbListAlternate;
					GIALRPtag* currentTagInPhrasalVerb = firstTagInPhrasalVerb;
					bool stillFoundVerbMatchOfArbitraryTense = true;
					bool foundAtLeastOneMatch = false;
					GIALRPtag* currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
					GIALRPtag* lastTagInPlainTextSentenceTemp = NULL;	//pointer used to change original plain text to collapsed phrasal verb text
					GIALRPtag* firstTagInCollapsedPhrasalVerb = new GIALRPtag();
					GIALRPtag* currentTagInCollapsedPhrasalVerb = firstTagInCollapsedPhrasalVerb;
					bool currentlyParsingTagSpecial = false;
					int tagInPhrasalVerbSpecialAndNotFoundCount = 0;
					int numberTagSpecialTagsFound = 0;
					int numberTagConsecutiveOptionalTagsFound = 0;
					bool phrasalVerbHasTagSpecial = false;
					while((currentTagInPhrasalVerb->nextTag != NULL) && (currentTagInPlainTextSentenceTemp->nextTag != NULL) && (stillFoundVerbMatchOfArbitraryTense))
					{
						#ifdef GIA_LRP_DEBUG
						//cout << "currentTagInPhrasalVerb->tagName = " << currentTagInPhrasalVerb->tagName << endl;
						//cout << "currentTagInPlainTextSentenceTemp->tagName = " << currentTagInPlainTextSentenceTemp->tagName << endl;
						#endif
						bool currentTagInPhrasalVerbOptionalAndNotFound = false;
						bool currentTagInPhrasalVerbSpecialAndNotFound = false;

						if(currentTagInPhrasalVerb->tagSpecialArbitraryName)
						{
							#ifdef GIA_LRP_DEBUG
							//cout << "u1" << endl;
							#endif
							phrasalVerbHasTagSpecial = true;

							//NB the collapsed phrasal verb contains precisely 2 entities: phrasalVerbCollapsed, entity2: thing/place/body (eg belong_to + sb/Tom) - thing/place/bodies are not currently being differentiated by the LRP as this information is only first generated at NLP/GIA parse stage
							currentTagInCollapsedPhrasalVerb->nextTag = new GIALRPtag();
							currentTagInCollapsedPhrasalVerb->nextTag->tagName = currentTagInPlainTextSentenceTemp->tagName + " ";		//arbitrary thing/place/body name
							currentTagInCollapsedPhrasalVerb->nextTag->collapsedPhrasalVerbExactDefinedSection = false;
							/*
							//removed 7 Sept 2012b - now interpret: ask sb over/round -> ask_over sb
							currentTagInCollapsedPhrasalVerb = currentTagInCollapsedPhrasalVerb->nextTag;
							*/
							currentlyParsingTagSpecial = true;
							numberTagSpecialTagsFound++;
						}
						else
						{
							#ifdef GIA_LRP_DEBUG
							//cout << "u2" << endl;
							#endif
							bool foundVerbMatchOfArbitraryTenseTemp = false;
							GIALRPtag* currentTagInPlainTextSentenceTempAlternate = currentTagInPlainTextSentenceTemp;
							bool alternateTag = false;
							string generatedTagNameLemma = "";
							while(!alternateTag || (currentTagInPlainTextSentenceTempAlternate->alternateTag != NULL))
							{
								#ifdef GIA_LRP_DEBUG
								//cout << "u3" << endl;
								#endif
								if(currentTagInPhrasalVerb->base)
								{
									for(int i=0; i<GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
									{
										for(int j=0; j<GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS; j++)
										{
											#ifdef GIA_LRP_DEBUG
											/*
											//cout << "currentTagInPhrasalVerb->tagName =" << currentTagInPhrasalVerb->tagName << endl;
											//cout << "(currentTagInPhrasalVerb->tagName).length()  =" << (currentTagInPhrasalVerb->tagName).length() << endl;
											if(currentTagInPhrasalVerb->tagName == "up")
											{
												cout << "currentTagInPhrasalVerb->grammaticalTenseFormsArray[i][j] = " << currentTagInPhrasalVerb->grammaticalTenseFormsArray[i][j] << endl;
												cout << "currentTagInPlainTextSentenceTempAlternate->tagName = " << currentTagInPlainTextSentenceTempAlternate->tagName << endl;
											}
											*/
											#endif
											if(currentTagInPhrasalVerb->grammaticalTenseFormsArray[i][j] == currentTagInPlainTextSentenceTempAlternate->tagName)		//USED TO BE currentTagInCollapsedPhrasalVerb before 6 Sept 2012
											{
												#ifdef GIA_LRP_DEBUG
												//cout << "u4" << endl;
												#endif
												currentTagInCollapsedPhrasalVerb->grammaticalTenseFormDetected = i;								//USED TO BE AND STILL IS currentTagInCollapsedPhrasalVerb before 6 Sept 2012
												generatedTagNameLemma = currentTagInPhrasalVerb->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
												foundVerbMatchOfArbitraryTenseTemp = true;
												#ifdef GIA_LRP_DEBUG
												cout << "foundVerbMatchOfArbitraryTenseTemp: " << currentTagInPlainTextSentenceTempAlternate->tagName << ", i = " << i << endl;
												#endif
											}
										}
									}
								}
								else
								{
									if(currentTagInPhrasalVerb->tagName == currentTagInPlainTextSentenceTempAlternate->tagName)
									{
										foundVerbMatchOfArbitraryTenseTemp = true;
										#ifdef GIA_LRP_DEBUG
										cout << "foundVerbMatchOfArbitraryTenseTemp: " << currentTagInPlainTextSentenceTempAlternate->tagName << endl;
										#endif
									}
								}
								if(currentTagInPlainTextSentenceTempAlternate->alternateTag != NULL)
								{
									#ifdef GIA_LRP_DEBUG
									//cout << "u4b" << endl;
									#endif
									currentTagInPlainTextSentenceTempAlternate = currentTagInPlainTextSentenceTempAlternate->alternateTag;
								}
								alternateTag = true;
								#ifdef GIA_LRP_DEBUG
								//cout << "u5" << endl;
								#endif
							}


							if(!foundVerbMatchOfArbitraryTenseTemp)
							{
								#ifdef GIA_LRP_DEBUG
								//cout << "u6" << endl;
								#endif
								if(currentTagInPhrasalVerb->optional)
								{
									#ifdef GIA_LRP_DEBUG
									//cout << "u7" << endl;
									#endif
									currentTagInPhrasalVerbOptionalAndNotFound = true;
								}
								else if(currentlyParsingTagSpecial && (tagInPhrasalVerbSpecialAndNotFoundCount <= GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_MAX_NUMBER_WORDS))
								{
									#ifdef GIA_LRP_DEBUG
									//cout << "u8" << endl;
									#endif
									currentTagInCollapsedPhrasalVerb->nextTag->tagName = currentTagInCollapsedPhrasalVerb->nextTag->tagName + currentTagInPlainTextSentenceTemp->tagName + " ";
									/*
									//removed 7 Sept 2012b - now interpret: ask sb over/round -> ask_over sb
									currentTagInCollapsedPhrasalVerb->tagName = currentTagInCollapsedPhrasalVerb->tagName + currentTagInPlainTextSentenceTemp->tagName;
									*/
									currentTagInPhrasalVerbSpecialAndNotFound = true;
									tagInPhrasalVerbSpecialAndNotFoundCount++;
								}
								else
								{
									#ifdef GIA_LRP_DEBUG
									//cout << "u9" << endl;
									#endif
									stillFoundVerbMatchOfArbitraryTense = false;
								}
							}
							else
							{
								if(currentlyParsingTagSpecial)
								{
									#ifdef GIA_LRP_DEBUG
									//cout << "u10" << endl;
									#endif
									/*
									//removed 7 Sept 2012b - now interpret: ask sb over/round -> ask_over sb
									currentTagInCollapsedPhrasalVerb->nextTag = new GIALRPtag();
									currentTagInCollapsedPhrasalVerb = currentTagInCollapsedPhrasalVerb->nextTag;
									*/
									currentlyParsingTagSpecial = false;
								}

								//cout << "u11" << endl;
								foundAtLeastOneMatch = true;
								currentTagInCollapsedPhrasalVerb->collapsedPhrasalVerbExactDefinedSection = true;
								currentTagInCollapsedPhrasalVerb->tagName = currentTagInCollapsedPhrasalVerb->tagName + currentTagInPlainTextSentenceTemp->tagName + "_";		//this is part of the defined/main section of the phrasal verb
								if(currentTagInPhrasalVerb->base)
								{
									currentTagInCollapsedPhrasalVerb->tagNameLemma = currentTagInCollapsedPhrasalVerb->tagNameLemma + generatedTagNameLemma + "_";
								}
								else
								{
									currentTagInCollapsedPhrasalVerb->tagNameLemma = currentTagInCollapsedPhrasalVerb->tagNameLemma + currentTagInPlainTextSentenceTemp->tagName + "_";
								}
								#ifdef GIA_LRP_DEBUG
								cout << "currentTagInCollapsedPhrasalVerb->tagName  = " << currentTagInCollapsedPhrasalVerb->tagName << endl;
								#endif
							}
							#ifdef GIA_LRP_DEBUG
							//cout << "u11b" << endl;
							#endif
						}

						#ifdef GIA_LRP_DEBUG
						//cout << "u11c" << endl;
						#endif
						if(currentTagInPhrasalVerb->tagSpecialArbitraryName)
						{
							if(currentTagInPhrasalVerb->optional)
							{
								#ifdef GIA_LRP_DEBUG
								//cout << "u12" << endl;
								#endif
								//special+optional tag found - just increment the phrasal verb for now [for this round]...
								currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
							}
							else
							{
								#ifdef GIA_LRP_DEBUG
								//cout << "u13" << endl;
								#endif
								currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
								currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
							}
						}
						else if(currentTagInPhrasalVerbOptionalAndNotFound)
						{
							#ifdef GIA_LRP_DEBUG
							//cout << "u14" << endl;
							#endif
							//optional phrasal verb tag not found - just increment the phrasal verb tag list...
							currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
						}
						else if(currentTagInPhrasalVerbSpecialAndNotFound)
						{
							#ifdef GIA_LRP_DEBUG
							//cout << "u15" << endl;
							#endif
							//next phrasal verb tag not found yet - just increment the sentence tag list...
							currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
						}
						else
						{
							#ifdef GIA_LRP_DEBUG
							//cout << "u16" << endl;
							#endif
							currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
							currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
						}
					}

					if(stillFoundVerbMatchOfArbitraryTense && foundAtLeastOneMatch)
					{
						if(currentTagInPhrasalVerb->nextTag == NULL)
						{//make sure the entire multiword phrasal verb is matched (in case currentTagInPlainTextSentenceTemp reaches end of sentence without matching the multiword phrasal verb in its entirety)
							#ifdef GIA_LRP_DEBUG
							//cout << "u17" << endl;
							#endif
							if(numberTagSpecialTagsFound <= 1)
							{//do not preprocess phrasal verbs with more than one special tag (ie sth/sb/swh) - as this generally involves more than a verb [verb sth preposition sth1] - added 1p1aTEMP5
								//reduce all entities
								#ifdef GIA_LRP_DEBUG
								//cout << "u18" << endl;
								#endif
								#ifdef GIA_LRP_DEBUG
								cout << "currentTagInPlainTextSentenceTemp->tagName = " << currentTagInPlainTextSentenceTemp->tagName << endl;
								#endif
								if(phrasalVerbHasTagSpecial)
								{
									currentTagInCollapsedPhrasalVerb->nextTag->nextTag = currentTagInPlainTextSentenceTemp;		//NB currentTagInCollapsedPhrasalVerb->nextTag is the collapsed phrasal verb thing/place/body entity
								}
								else
								{
									currentTagInCollapsedPhrasalVerb->nextTag = currentTagInPlainTextSentenceTemp;
								}
								#ifdef GIA_LRP_DEBUG
								//cout << "u18b" << endl;
								#endif
								previousTagInPlainTextSentence->nextTag = firstTagInCollapsedPhrasalVerb;

								foundAtLeastOnePhrasalVerbInSentenceAndCollapsed = true;
								#ifdef GIA_LRP_DEBUG
								//cout << "u18c" << endl;
								#endif
							}
						}
					}
					/*
					}
					*/
					#ifdef GIA_LRP_DEBUG
					//cout << "u19" << endl;
					#endif
					if(currentTagInPhrasalVerbListAlternate->alternateSentence != NULL)
					{
						//cout << "u20" << endl;
						currentTagInPhrasalVerbListAlternate = currentTagInPhrasalVerbListAlternate->alternateSentence;
					}
					#ifdef GIA_LRP_DEBUG
					//cout << "u21" << endl;
					#endif
					alternatePhrasalVerb = true;
				}
				/*
				if(currentTagInPhrasalVerbList->alternateSentence != NULL)
				{
					currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->alternateSentence;
				}
				*/
				currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->nextSentence;
			}
			#ifdef GIA_LRP_DEBUG
			//cout << "u23" << endl;
			#endif
			if(foundAtLeastOnePhrasalVerbInSentenceAndCollapsed)
			{
				#ifdef GIA_LRP_DEBUG
				cout << "foundAtLeastOnePhrasalVerbInSentenceAndCollapsed" << endl;
				#endif
				#ifdef GIA_LRP_DEBUG
				//cout << "u23b" << endl;
				#endif
				//renumberEntityIndiciesInSentence();
				GIALRPtag* currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
				int newEntityIndex = GIA_NLP_START_ENTITY_INDEX;
				while(currentTagInPlainTextSentenceTemp2->nextTag != NULL)
				{
					#ifdef GIA_LRP_DEBUG
					cout << "u23c currentTagInPlainTextSentenceTemp2->tagName = " << currentTagInPlainTextSentenceTemp2->tagName << endl;
					#endif
					if(currentTagInPlainTextSentenceTemp2->collapsedPhrasalVerbExactDefinedSection)
					{//create a new correspondenceInfo
						#ifdef GIA_LRP_DEBUG
						cout << "u23d" << endl;
						#endif
						string tagNameLemma = currentTagInPlainTextSentenceTemp2->tagNameLemma;
						string tagName = currentTagInPlainTextSentenceTemp2->tagName;
						string tagNameLemmaWithLastLetterDropped = tagNameLemma.substr(0, tagNameLemma.length()-1);
						string tagNameWithLastLetterDropped = tagName.substr(0, tagName.length()-1);
						currentTagInPlainTextSentenceTemp2->tagNameLemma = tagNameLemmaWithLastLetterDropped;
						currentTagInPlainTextSentenceTemp2->tagName = tagNameWithLastLetterDropped;
						currentCorrespondenceInfo->sentenceIndex = currentTagInPlainText->sentenceIndex;
						currentCorrespondenceInfo->entityIndex = newEntityIndex;
						currentCorrespondenceInfo->lemmaWithLRP = currentTagInPlainTextSentenceTemp2->tagNameLemma;
						currentCorrespondenceInfo->wordWithLRP = currentTagInPlainTextSentenceTemp2->tagName;
						currentCorrespondenceInfo->wordWithLRPforNLPonly = lrpDummyCollapsedPhrasalVerbNameForNLPgrammaticalTenseFormsArray[currentTagInPlainTextSentenceTemp2->grammaticalTenseFormDetected];
						#ifdef GIA_LRP_DEBUG
						cout << "currentTagInPlainTextSentenceTemp2->grammaticalTenseFormDetected = " << currentTagInPlainTextSentenceTemp2->grammaticalTenseFormDetected << endl;
						cout << "currentTagInPlainText->sentenceIndex = " << currentTagInPlainText->sentenceIndex << endl;
						cout << "currentCorrespondenceInfo->entityIndex = " << currentCorrespondenceInfo->entityIndex << endl;
						cout << "currentCorrespondenceInfo->lemmaWithLRP = " << currentCorrespondenceInfo->lemmaWithLRP << endl;
						cout << "currentCorrespondenceInfo->wordWithLRP = " << currentCorrespondenceInfo->wordWithLRP << endl;
						cout << "currentCorrespondenceInfo->wordWithLRPforNLPonly = " << currentCorrespondenceInfo->wordWithLRPforNLPonly << endl;
						cout << "\t\t\tnew correspondenceInfo: " << currentCorrespondenceInfo->wordWithLRP << ", wordWithLRPforNLPonly = " << currentCorrespondenceInfo->wordWithLRPforNLPonly << endl;
						cout << "\tcurrentCorrespondenceInfo->sentenceIndex = " << currentCorrespondenceInfo->sentenceIndex << endl;
						cout << "\tcurrentCorrespondenceInfo->entityIndex = " << currentCorrespondenceInfo->entityIndex << endl;
						#endif
						currentCorrespondenceInfo->next = new GIALRPtagTextCorrespondenceInfo();
						currentCorrespondenceInfo = currentCorrespondenceInfo->next;
					}
					#ifdef GIA_LRP_DEBUG
					//cout << "u23e" << endl;
					#endif
					currentTagInPlainTextSentenceTemp2->entityIndex = newEntityIndex;
					currentTagInPlainTextSentenceTemp2 = currentTagInPlainTextSentenceTemp2->nextTag;
					#ifdef GIA_LRP_DEBUG
					//cout << "u23g" << endl;
					#endif
					newEntityIndex++;
				}
			}
			#ifdef GIA_LRP_DEBUG
			//cout << "u24" << endl;
			#endif
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}


		currentTagInPlainText = currentTagInPlainText->nextSentence;
	}
	
	return result;
}


bool searchAndReplaceMultiwordPrepositions(GIALRPtag* firstTagInPlainText, GIALRPtag* firstTagInMultiwordPrepositionList, GIALRPtagTextCorrespondenceInfo* firstGIALRPtagCorrespondenceInfo)
{
	bool result = true;
	
	GIALRPtagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIALRPtagCorrespondenceInfo;	//new correspondence info for each found multiword preposition

	GIALRPtag* currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{
		#ifdef GIA_LRP_DEBUG
		//cout << "qcurrentTagInPlainText->sentenceIndex = " << currentTagInPlainText->sentenceIndex << endl;
		#endif
			
		GIALRPtag* firstTagInPlainTextSentence = currentTagInPlainText;
		GIALRPtag* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIALRPtag* previousTagInPlainTextSentence = NULL;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			GIALRPtag* currentTagInMultiwordPrepositionList = firstTagInMultiwordPrepositionList;
			bool foundAtLeastOneMultiwordPrepositionInSentenceAndCollapsed = false;
			while(currentTagInMultiwordPrepositionList->nextSentence != NULL)
			{
				#ifdef GIA_LRP_DEBUG
				//cout << "u24a" << endl;
				#endif
				bool stillFoundPrepositionMatch = true;
				bool foundAtLeastOneMatch = false;
				GIALRPtag* currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
				GIALRPtag* firstTagInCollapsedMultiwordPreposition = new GIALRPtag();
				GIALRPtag* currentTagInCollapsedMultiwordPreposition = firstTagInCollapsedMultiwordPreposition;
				GIALRPtag* firstTagInMultiwordPreposition = currentTagInMultiwordPrepositionList;
				GIALRPtag* currentTagInMultiwordPreposition = firstTagInMultiwordPreposition;
				//cout << "currentTagInPlainTextSentenceTemp->tagName = " << currentTagInPlainTextSentenceTemp->tagName << endl;
				//cout << "currentTagInMultiwordPreposition->tagName = " << currentTagInMultiwordPreposition->tagName << endl;
				while((currentTagInMultiwordPreposition->nextTag != NULL) && (currentTagInPlainTextSentenceTemp->nextTag != NULL) && (stillFoundPrepositionMatch))
				{
					#ifdef GIA_LRP_DEBUG
					//cout << "u24b" << endl;
					#endif
					if(currentTagInMultiwordPreposition->tagName != currentTagInPlainTextSentenceTemp->tagName)
					{
						#ifdef GIA_LRP_DEBUG
						//cout << "u24c" << endl;
						#endif
						stillFoundPrepositionMatch = false;
					}
					else
					{
						#ifdef GIA_LRP_DEBUG
						cout << "u24b" << endl;
						cout << "currentTagInPlainTextSentenceTemp->tagName  = " << currentTagInPlainTextSentenceTemp->tagName << endl;
						#endif
						currentTagInCollapsedMultiwordPreposition->collapsedMultiwordPreposition = true;
						currentTagInCollapsedMultiwordPreposition->tagName = currentTagInCollapsedMultiwordPreposition->tagName + currentTagInPlainTextSentenceTemp->tagName + "_";
						foundAtLeastOneMatch = true;
					}
					currentTagInMultiwordPreposition = currentTagInMultiwordPreposition->nextTag;
					currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
				}

				if(stillFoundPrepositionMatch && foundAtLeastOneMatch)
				{
					if(currentTagInMultiwordPreposition->nextTag == NULL)
					{//make sure the entire multiword preposition is matched (in case currentTagInPlainTextSentenceTemp reaches end of sentence without matching the multiword preposition in its entirety)
						#ifdef GIA_LRP_DEBUG
						cout << "u24d" << endl;
						#endif
						//reduce all entities
						currentTagInCollapsedMultiwordPreposition->nextTag = currentTagInPlainTextSentenceTemp;
						previousTagInPlainTextSentence->nextTag = firstTagInCollapsedMultiwordPreposition;

						foundAtLeastOneMultiwordPrepositionInSentenceAndCollapsed = true;
					}
				}

				currentTagInMultiwordPrepositionList = currentTagInMultiwordPrepositionList->nextSentence;
			}
			#ifdef GIA_LRP_DEBUG
			//cout << "u25a" << endl;
			#endif
			if(foundAtLeastOneMultiwordPrepositionInSentenceAndCollapsed)
			{
				//renumberEntityIndiciesInSentence();
				GIALRPtag* currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
				int newEntityIndex = GIA_NLP_START_ENTITY_INDEX;
				//int collapsedMultiwordPrepositionIndex = 0;
				#ifdef GIA_LRP_DEBUG
				cout << "foundAtLeastOneMultiwordPrepositionInSentenceAndCollapsed" << endl;
				#endif
				while(currentTagInPlainTextSentenceTemp2->nextTag != NULL)
				{
					#ifdef GIA_LRP_DEBUG
					cout << "currentTagInPlainTextSentenceTemp2->tagName = " << currentTagInPlainTextSentenceTemp2->tagName << endl;
					#endif
					if(currentTagInPlainTextSentenceTemp2->collapsedMultiwordPreposition)
					{//create a new correspondenceInfo
						string tagName = currentTagInPlainTextSentenceTemp2->tagName;
						string tagNameWithLastLetterDropped = tagName.substr(0, tagName.length()-1);
						currentTagInPlainTextSentenceTemp2->tagName = tagNameWithLastLetterDropped;
						currentCorrespondenceInfo->sentenceIndex = currentTagInPlainText->sentenceIndex;
						currentCorrespondenceInfo->entityIndex = newEntityIndex;	//this is not currently used for LRP collapsed multiword prepositions
						currentCorrespondenceInfo->wordWithLRP = currentTagInPlainTextSentenceTemp2->tagName;
						currentCorrespondenceInfo->wordWithLRPforNLPonly = GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP; //lrpDummyCollapsedMultiwordPrepositionLemmaNameForNLPArray[collapsedMultiwordPrepositionIndex];
						//collapsedMultiwordPrepositionIndex++;
						#ifdef GIA_LRP_DEBUG
						cout << "new correspondenceInfo: " << currentCorrespondenceInfo->wordWithLRP << ", wordWithLRPforNLPonly = " << currentCorrespondenceInfo->wordWithLRPforNLPonly << endl;
						cout << "\tcurrentCorrespondenceInfo->sentenceIndex = " << currentCorrespondenceInfo->sentenceIndex << endl;
						cout << "\tcurrentCorrespondenceInfo->entityIndex = " << currentCorrespondenceInfo->entityIndex << endl;
						#endif
						currentCorrespondenceInfo->next = new GIALRPtagTextCorrespondenceInfo();
						currentCorrespondenceInfo = currentCorrespondenceInfo->next;
					}
					currentTagInPlainTextSentenceTemp2->entityIndex = newEntityIndex;
					currentTagInPlainTextSentenceTemp2 = currentTagInPlainTextSentenceTemp2->nextTag;
					newEntityIndex++;
				}
			}
			#ifdef GIA_LRP_DEBUG
			//cout << "u25" << endl;
			#endif
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}
		currentTagInPlainText = currentTagInPlainText->nextSentence;
	}

	return result;
}

bool writeTagListToFile(GIALRPtag* firstTagInPlainText, string plainTextLRPoutputFileName, string plainTextLRPforNLPoutputFileName)
{
	bool result = true;

	ofstream plainTextLRPOutput(plainTextLRPoutputFileName.c_str());
	ofstream plainTextLRPforNLPOutput(plainTextLRPforNLPoutputFileName.c_str());

	bool firstCharacterInFile = true;

	GIALRPtag* currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{
		GIALRPtag* firstTagInPlainTextSentence = currentTagInPlainText;
		GIALRPtag* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			string plainTextLRPOutputTag = currentTagInPlainTextSentence->tagName;
			string plainTextLRPforNLPOutputTag = "";
			if(currentTagInPlainTextSentence->collapsedPhrasalVerbExactDefinedSection)
			{//create a new correspondenceInfo
				plainTextLRPforNLPOutputTag = lrpDummyCollapsedPhrasalVerbNameForNLPgrammaticalTenseFormsArray[currentTagInPlainTextSentence->grammaticalTenseFormDetected];
			}
			else if(currentTagInPlainTextSentence->collapsedMultiwordPreposition)
			{
				plainTextLRPforNLPOutputTag = GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP;
			}
			else
			{
				plainTextLRPforNLPOutputTag = plainTextLRPOutputTag;
			}

			bool punctuationMarkFound = false;
			for(int i=0; i<GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS; i++)
			{
				string nlpPunctionMarkCharacterString = "";
				nlpPunctionMarkCharacterString = nlpPunctionMarkCharacterString + nlpPunctionMarkCharacterArray[i];
				if(plainTextLRPOutputTag == nlpPunctionMarkCharacterString)
				{
					punctuationMarkFound = true;
				}
			}
			if(!punctuationMarkFound && !firstCharacterInFile)
			{
				plainTextLRPOutputTag = string(STRING_SPACE) + plainTextLRPOutputTag;
				plainTextLRPforNLPOutputTag = string(STRING_SPACE) + plainTextLRPforNLPOutputTag;
			}

			plainTextLRPOutput.write(plainTextLRPOutputTag.c_str(), plainTextLRPOutputTag.length());
			plainTextLRPforNLPOutput.write(plainTextLRPforNLPOutputTag.c_str(), plainTextLRPforNLPOutputTag.length());

			firstCharacterInFile = false;

			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}
		currentTagInPlainText = currentTagInPlainText->nextSentence;
	}

	plainTextLRPOutput.close();
	plainTextLRPforNLPOutput.close();

	return result;
}

//NB preposition reversion routine will not work for RelEx as RelEx defines dependency relations based on lemmas not words...
void revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, GIAsentence* currentSentenceInList, GIArelation* currentRelationInListForPrepositionsOnly, bool isPreposition, bool* foundOfficialLRPreplacementString)
{
	int entityIndexForNonPrepositionsOnly = feature->entityIndex;

	//save original values for NLP only (required during a multiword preposition replacement with an adjacent multiword verb
	feature->wordWithLRPforNLPonly = feature->word;

	string word = feature->word;
	//string lemma = feature->lemma;	//only used for prepositions (dependency relation) calculations, where lemma has already been calculated via revertNLPtagNameToOfficialLRPtagName()

	GIALRPtagTextCorrespondenceInfo* firstGIALRPtagCorrespondenceInfo = getCurrentGIALRPtagTextCorrespondenceInfo();

	int sentenceIndex = currentSentenceInList->sentenceIndex;

	bool foundCorrespondingLRPtag = false;
	GIALRPtagTextCorrespondenceInfo* currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = firstGIALRPtagCorrespondenceInfo;
	while(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next != NULL)
	{
		#ifdef GIA_LRP_DEBUG2
		cout << "word = " << word << endl;
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->lemmaWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->lemmaWithLRP << endl;
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl;
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly << endl;
		cout << "sentenceIndex = " << sentenceIndex << endl;
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex << endl;
		#endif

		if(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex == sentenceIndex)
		{
			#ifdef GIA_LRP_DEBUG2
			cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl;
			#endif

			if(word == currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly)
			{
				#ifdef GIA_LRP_DEBUG2
				cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl;
				cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly << endl;
				#endif

				if(isPreposition)
				{
					#ifdef GIA_LRP_DEBUG2
					cout << "isPreposition" << endl;
					#endif

					//now search entire sentence->feature list and find entity/word that has same name, and has the governor/dependent closest to it...
					string relationGovernor = currentRelationInListForPrepositionsOnly->relationGovernor;
					string relationDependent = currentRelationInListForPrepositionsOnly->relationDependent;
					GIAfeature* firstFeatureInList = currentSentenceInList->firstFeatureInList;
					GIAfeature* currentFeatureInList = firstFeatureInList;
					int indexOfPrepositionWithMinimumProximityOfGovernorDependentWords = GIA_ENTITY_INDEX_UNDEFINED;
					int minimumProximityOfGovernorDependentWords = MAXIMUM_NUMBER_WORDS_PER_SENTENCE;
					int indexOfLastInstanceOfPreposition = GIA_ENTITY_INDEX_UNDEFINED;
					int indexOfLastInstanceOfGovernor = GIA_ENTITY_INDEX_UNDEFINED;
					int indexOfLastInstanceOfDependent = GIA_ENTITY_INDEX_UNDEFINED;
					bool foundPrepositionGovernorAndDependentInFeatureList = false;
					#ifdef GIA_LRP_DEBUG2
					cout << "relationGovernor = " << relationGovernor << endl;
					cout << "relationDependent = " << relationDependent << endl;
					cout << "word = " << word << endl;
					#endif

					while(currentFeatureInList->next != NULL)
					{
						#ifdef GIA_LRP_DEBUG2
						cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
						#endif
						if(currentFeatureInList->wordWithLRPforNLPonly == relationGovernor)
						{
							#ifdef GIA_LRP_DEBUG2
							cout << "(currentFeatureInList->wordWithLRPforNLPonly == relationGovernor)" << endl;
							#endif
							indexOfLastInstanceOfGovernor = currentFeatureInList->entityIndex;
						}
						if(currentFeatureInList->word == currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP)	//NB currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP has already been assigned to feature lemma via previous non-preposition execution of revertNLPtagNameToOfficialLRPtagName
						{
							#ifdef GIA_LRP_DEBUG2
							cout << "(currentFeatureInList->word == word)" << endl;
							#endif
							indexOfLastInstanceOfPreposition = currentFeatureInList->entityIndex;
						}
						if(currentFeatureInList->wordWithLRPforNLPonly == relationDependent)
						{
							#ifdef GIA_LRP_DEBUG2
							cout << "(currentFeatureInList->wordWithLRPforNLPonly == relationDependent)" << endl;
							#endif

							indexOfLastInstanceOfDependent = currentFeatureInList->entityIndex;
							if((indexOfLastInstanceOfPreposition != GIA_ENTITY_INDEX_UNDEFINED) && (indexOfLastInstanceOfGovernor != GIA_ENTITY_INDEX_UNDEFINED))
							{
								#ifdef GIA_LRP_DEBUG2
								//cout << "(indexOfLastInstanceOfPreposition != GIA_ENTITY_INDEX_UNDEFINED) && (indexOfLastInstanceOfGovernor != GIA_ENTITY_INDEX_UNDEFINED)" << endl;
								#endif

								int proximityOfPrepositionToGovernor = indexOfLastInstanceOfPreposition - indexOfLastInstanceOfGovernor;
								int proximityOfPrepositionToDependent = indexOfLastInstanceOfDependent - indexOfLastInstanceOfPreposition;
								int totalProximityOfPrepositionToGovernorAndDependent = proximityOfPrepositionToGovernor + proximityOfPrepositionToDependent;
								if(totalProximityOfPrepositionToGovernorAndDependent < minimumProximityOfGovernorDependentWords)
								{
									#ifdef GIA_LRP_DEBUG2
									//cout << "minimumProximityOfGovernorDependentWords = " << minimumProximityOfGovernorDependentWords << endl;
									#endif
									minimumProximityOfGovernorDependentWords = totalProximityOfPrepositionToGovernorAndDependent;
									indexOfPrepositionWithMinimumProximityOfGovernorDependentWords = indexOfLastInstanceOfPreposition;
									foundPrepositionGovernorAndDependentInFeatureList = true;
								}
							}
						}

						currentFeatureInList = currentFeatureInList->next;
					}
					if(foundPrepositionGovernorAndDependentInFeatureList)
					{
						if(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex == indexOfPrepositionWithMinimumProximityOfGovernorDependentWords)
						{
							feature->word = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP;
							//feature->lemma = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP;			//lemma is not defined for prepositions
							#ifdef GIA_LRP_DEBUG
							cout << "(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex == indexOfPrepositionWithMinimumProximityOfGovernorDependentWords)" << endl;
							cout << "revertNLPtagNameToOfficialLRPtagName(): foundCorrespondingLRPtag, wordWithLRPforNLPonly = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly << ", wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl;
							#endif
							foundCorrespondingLRPtag = true;
						}
					}
					else
					{
						cout << "error: !foundPrepositionGovernorAndDependentInFeatureList;" << endl;
						cout << word << "(" << relationGovernor << ", " << relationDependent << ")" << endl;
						exit(0);
					}
				}
				else
				{
					#ifdef GIA_LRP_DEBUG
					/*
					cout << "!isPreposition" << endl;
					cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex << endl;
					cout << "entityIndexForNonPrepositionsOnly = " << entityIndexForNonPrepositionsOnly << endl;
					*/
					#endif
					if(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex == entityIndexForNonPrepositionsOnly)
					{
						feature->word = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP;
						feature->lemma = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->lemmaWithLRP;

						#ifdef GIA_LRP_DEBUG
						cout << "(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex == entityIndexForNonPrepositionsOnly)" << endl;
						cout << "feature->word = " << feature->word << endl;
						cout << "feature->lemma = " << feature->lemma << endl;
						cout << "revertNLPtagNameToOfficialLRPtagName(): foundCorrespondingLRPtag, wordWithLRPforNLPonly = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly << ", wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl;
						#endif
						foundCorrespondingLRPtag = true;
					}
				}
			}
		}
		currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next;
	}
	if(!foundCorrespondingLRPtag)
	{
		//cout << "revertNLPtagNameToOfficialLRPtagName() warning: !foundCorrespondingLRPtag; " << lemma << endl;
	}
	else
	{
		*foundOfficialLRPreplacementString = true;
	}
}




#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
//warning: this function is only currently developed for infinitive and continuous case
bool determineVerbCaseWrapper(string word, string* baseNameFound, int* grammaticalTenseModifier)
{
	bool result = true;
	bool foundVerbCase = false;
	string verbListFileName = lrpDataFolderName + GIA_LRP_VERB_DATABASE_FILE_NAME;
	//cout << "determineVerbCaseWrapper: " << endl;
	if(!verbListLoaded)
	{
		cout << "!verbListLoaded (OpenGIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL requires -lrpfolder to be set): verbListFileName = " << verbListFileName << endl;
		result = false;
	}
	else
	{
		//cout << "determineVerbCase: " << endl;
		foundVerbCase = determineVerbCase(word, firstTagInVerbListGlobal, baseNameFound, grammaticalTenseModifier);
		//cout << "determineVerbCase end: " << endl;
	}
	return foundVerbCase;
}

//warning: this function is only currently developed for infinitive and continuous case
bool determineVerbCase(string word, GIALRPtag* firstTagInVerbList, string* baseNameFound, int* grammaticalTenseModifier)
{
	bool foundVerbCase = false;

	/*
	detectContinuousVerbBasic Algorithm:

	Note a simple check for "ing" is not posssible as;
		some nouns end in ing also eg "thing"
	Note a simple "ing" appendition check against a verb infinitive list is not possible because there are some grammatical variants;
		Case 1: thinking
		Case 2: running - "run" + "n" [run n] + "ing"
		Case 3: changing - "chang" [change e] + "ing"

		ongoing?
		outstanding?
		being?
		becoming?

	This code uses a subset of code from generateTenseVariantsOfVerbBase (note generateTenseVariantsOfVerbBase is not used itself as this code is designed to be liberal/robust and detect all possible verbs without being subject to theoretical grammar rules)
	*/

	GIALRPtag* currentTagInVerbList = firstTagInVerbList;
	int numberOfCharactersInBaseTenseFormAppend = 0;
	while(currentTagInVerbList->nextSentence != NULL)
	{
		//cout << "\t currentTagInVerbList->tagName = " << currentTagInVerbList->tagName << endl;
		string wordLowerCase = convertStringToLowerCase(&word);
		string base = currentTagInVerbList->tagName;

		if(wordLowerCase == base)
		{
			//GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP _:
			*grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP;
			foundVerbCase = true;
		}
		else
		{
			if(base.length() >= 3)	//match to baseWithLast3LettersDropped
			{
				int baseStringLength = base.length();
				int indexOfLastCharacterInBase = baseStringLength-1;
				char lastCharacterInBase = base[indexOfLastCharacterInBase];
				string baseWithLast1LettersDropped = base.substr(0, baseStringLength-1);
				string baseWithLast2LettersDropped = base.substr(0, baseStringLength-2);
				string baseWithLast3LettersDropped = base.substr(0, baseStringLength-3);

				//GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP _ing:
				//continuous rule 1a/3b/4: thinking/happening/entering
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				//continuous rule 1b: running - "run" + "n" [run n] + "ing"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				//continuous rule 2: changing - "chang" [change e] + "ing"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				/*
				//continuous rule 3a: N/A !marriing (use marrying)
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				*/

				#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
				//added 2h2a
				//GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND _able:
				//potential rule 1a/3b/4: thinkable/changeable
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				//potential rule 1b: running - "run" + "n" [run n] + "able"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				/*
				//potential rule 2: N/A !changable (use changeable)
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				*/
				//potential rule 3a: running - "marr" + "i" + "able"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
				//added 2h2c
				//GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP _ive:
				//potential rule 1ai: -> ive eg resistive/adaptive
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 1aii: -> itive eg additive
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 1aiii: -> ative eg affirmative
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				/*
				//potential rule 1b: running - "run" + "n" [run n] + "itive"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				*/
				//potential rule 2i: e -> itive eg competitive/definitive/accomodative
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 2ii: e -> ment + ive eg judgementive
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 3a: y -> iment + ive eg supplimentive
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				#endif
				#endif

				#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
				//added 2h2a
				//GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND _ed:
				//possible state rule 1a/3b/4: visited/opened
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				//possible state rule 1b: rubbed/stopped/referred/admitted - "rub" + "b" + "ed"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				//possible state rule 2: smiled/fined - "smil" [change e] + "ed"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				//possible state rule 3a: studied/married - "marr" + "i" + "ed"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				#endif

				#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
				//added 2h2d
				//GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP _ment:
				//potential rule 1ai: -> ment eg movement/government/derailment/detainment/enjoyment
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aii: -> ament eg disarmament
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aiii: -> lment eg enrollment/installment/fulfillment
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				/*
				//potential rule 1b: running - "run" + "n" [run n] + "itive"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				*/
				//potential rule 2i: e -> ment eg judgement/dislodgment
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE2, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				/*
				//potential rule 2ii: e -> ment + ive eg judgementive
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				*/
				//potential rule 3a: y -> iment eg worriment/suppliment/embodiment
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 5: pt -> pment eg entrapment/equipment
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast2LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE5, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);

				//GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP _ion:
				//potential rule 1ai: -> ion eg absorption/abstraction/adaptation
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aii: -> ition eg addition
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aiii: -> ation eg acceptation/affirmation
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				/*
				//potential rule 1b: running - "run" + "n" [run n] + "itive"
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				*/
				//potential rule 2i: e -> ion eg relation/acclimatisation/accommodation/activation/accretion
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 2ii: e -> ition + ive eg competition/composition/definition
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 2iii: e -> ation + ive eg admiration/organisation
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 3a: ify -> ification eg subjectification/amplification/ammonification/identification/beautification
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 6i: aim -> amation eg acclamation {acclimation?}
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast3LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6I, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 6i: ide -> ision eg division
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast3LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 6iii: ish -> ition eg abolition/demolition
				testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast3LettersDropped, GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				#endif
			}
		}

		currentTagInVerbList = currentTagInVerbList->nextSentence;
	}
	
	#ifdef GIA_LRP_DEBUG
	if(foundVerbCase)
	{
		cout << "foundVerbCase baseNameFound = " <<* baseNameFound << ", grammaticalTenseModifier = " <<* grammaticalTenseModifier << endl;
	}
	//cout << "baseWithLast1LettersDropped = " << baseWithLast1LettersDropped << endl;
	#endif

	return foundVerbCase;
}

void testVerbCase(string tagName, string wordLowerCase, string baseTenseFormStart, string baseTenseFormAppend, int* numberOfCharactersInBaseTenseFormAppend, bool* foundVerbCase, string* baseNameFound, int* grammaticalTenseModifier, int grammaticalTenseModifierNew)
{
	if(baseTenseFormAppend.length() >* numberOfCharactersInBaseTenseFormAppend)
	{
		string hypotheticalVerbVariantCase = baseTenseFormStart + baseTenseFormAppend;
		if(wordLowerCase == hypotheticalVerbVariantCase)
		{
			*foundVerbCase = true;
			*grammaticalTenseModifier = grammaticalTenseModifierNew;
			*baseNameFound = tagName;
			*numberOfCharactersInBaseTenseFormAppend = baseTenseFormAppend.length();
		}	
	}
}
#endif


#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
//NB determineIfWordIsIrregularVerbContinuousCaseWrapper() can be used instead of determineVerbCaseWrapper(), as Stanford only has a problem identifying verbs (pos tag "VBG") when they are irregular varbs
bool determineIfWordIsIrregularVerbContinuousCaseWrapper(string word, string* baseNameFound)
{
	bool result = true;
	bool foundIrregularVerbContinuousCase = false;
	string irregularVerbListFileName = lrpDataFolderName + GIA_LRP_IRREGULARVERB_DATABASE_FILE_NAME;
	if(!irregularVerbListLoaded)
	{
		cout << "!irregularVerbListLoaded (OpenGIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE requires -lrpfolder to be set): irregularVerbListFileName = " << irregularVerbListFileName << endl;
		result = false;
	}
	else
	{
		foundIrregularVerbContinuousCase = determineIfWordIsIrregularVerbContinuousCase(word, firstTagInIrregularVerbListGlobal, baseNameFound);
	}
	return foundIrregularVerbContinuousCase;
}


bool determineIfWordIsIrregularVerbContinuousCase(string word, GIALRPtag* firstTagInIrregularVerbList, string* baseNameFound)
{
	bool foundIrregularVerbContinuousCase = false;

	GIALRPtag* currentTagInIrregularVerbList = firstTagInIrregularVerbList;
	while(currentTagInIrregularVerbList->nextSentence != NULL)
	{
		int irregularVerbTagIndex = 0;
		GIALRPtag* currentTagInIrregularVerb = currentTagInIrregularVerbList;
		while(currentTagInIrregularVerb->nextTag != NULL)
		{
			//cout << "\t currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
			string wordLowerCase = convertStringToLowerCase(&word);

			if(irregularVerbTagIndex == 3)
			{
				if(wordLowerCase == currentTagInIrregularVerb->tagName)
				{
					//cout << "foundIrregularVerbContinuousCase" << endl;
					//cout << "irregularVerbBaseForm = " << currentTagInIrregularVerbList->tagName << endl;
					foundIrregularVerbContinuousCase = true;
					*baseNameFound = currentTagInIrregularVerbList->tagName;
				}
			}
			currentTagInIrregularVerb = currentTagInIrregularVerb->nextTag;
			irregularVerbTagIndex++;
		}

		//cout << "currentTagInIrregularVerbList->tagName = " << currentTagInIrregularVerbList->tagName << endl;

		/*OLD (before RBB addition of continuous cases to wikipedia english irregular verb list - ie final/4th column of WikipediaIrregularVerbs.txt):
		cout << "currentTagInIrregularVerbList->tagName = " << currentTagInIrregularVerbList->tagName << endl;

		string irregularVerbBaseForm = currentTagInIrregularVerbList->tagName;
		int irregularVerbBaseFormLength = irregularVerbBaseForm.length();

		string baseWithLastLetterDropped = irregularVerbBaseForm.substr(0, irregularVerbBaseFormLength-1);
		#ifdef GIA_LRP_DEBUG
		//cout << "baseWithLastLetterDropped = " << baseWithLastLetterDropped << endl;
		#endif

		string irregularVerbContinuousForm1 =  irregularVerbBaseForm + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;
		string irregularVerbContinuousForm2 = baseWithLastLetterDropped + GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if((word == irregularVerbContinuousForm1) || (word == irregularVerbContinuousForm2))
		{
			cout << "foundIrregularVerbContinuousCase" << endl;
			cout << "irregularVerbBaseForm = " << irregularVerbBaseForm << endl;
			foundIrregularVerbContinuousCase = true;
			*baseNameFound = irregularVerbBaseForm;
		}
		*/

		currentTagInIrregularVerbList = currentTagInIrregularVerbList->nextSentence;
	}

	return foundIrregularVerbContinuousCase;
}
#endif



#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
bool loadPrepositionsInverseList(string prepositionsInverseListFileName, GIALRPtag* firstTagInPrepositionsInverseList)
{
	bool result = true;

	GIALRPtag* currentTagInPrepositionsInverseList = firstTagInPrepositionsInverseList;
	GIALRPtag* firstTagInRow = currentTagInPrepositionsInverseList;
	GIALRPtag* currentTagInRow = firstTagInRow;

	ifstream parseFileObject(prepositionsInverseListFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//txt file does not exist in current directory.
		cout << "Error: Prepositions Inverse Database File does not exist in current directory: " << prepositionsInverseListFileName << endl;
		result = false;
	}
	else
	{
		int lineIndex = 0;
		int charCount = 0;
		char currentToken;
		string currentWord = "";
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEWLINE)
			{
				#ifdef GIA_LRP_DEBUG
				cout <<	"adding Tag: tagName = " << currentWord << endl;
				#endif

				currentTagInRow->tagName = currentWord;
				currentTagInRow->nextTag = new GIALRPtag();
				currentTagInRow = currentTagInRow->nextTag;

				currentTagInPrepositionsInverseList->nextSentence = new GIALRPtag();
				currentTagInPrepositionsInverseList = currentTagInPrepositionsInverseList->nextSentence;
				currentTagInRow = currentTagInPrepositionsInverseList;

				currentWord = "";
				lineIndex++;
			}
			else if(currentToken == GIA_LRP_PREPOSITIONS_DATABASE_TAG_NEXTCOLUMN)
			{
				#ifdef GIA_LRP_DEBUG
				cout <<	"adding Tag: tagName = " << currentWord << endl;
				#endif
				
				currentTagInRow->tagName = currentWord;
				currentTagInRow->nextTag = new GIALRPtag();
				currentTagInRow = currentTagInRow->nextTag;
				
				currentWord = "";
			}
			else
			{
				currentWord = currentWord + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}

	return result;
}			
			
void detectIfInverseOrTwoWayConditionRequired(string conditionName, bool* inverseConditionRequired, bool* twoWayConditionRequired, string* inverseConditionName)
{	
	GIALRPtag* firstTagInPrepositionsInverseList = firstTagInPrepositionsInverseListGlobal;
	
	*inverseConditionRequired = false;
	*twoWayConditionRequired = false;
	
	//invert condition if necessary
	GIALRPtag* currentTagInPrepositionsInverseList = firstTagInPrepositionsInverseList;
	while(currentTagInPrepositionsInverseList->nextSentence != NULL)
	{
		#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
		//cout << "qcurrentTagInPlainText->sentenceIndex = " << currentTagInPlainText->sentenceIndex << endl;
		#endif
		
		GIALRPtag* firstTagInPrepositionsInverseListSentence = currentTagInPrepositionsInverseList;
		GIALRPtag* currentTagInPrepositionsInverseListSentence = firstTagInPrepositionsInverseListSentence;
		bool foundConditionToInvert = false;
		string conditionNameNew = "";	//= currentTagInPrepositionsInverseList->tagName;
		for(int i=1; i<=GIA_LRP_PREPOSITIONS_DATABASE_NUMBER_OF_TAGS; i++)
		{
			if(i == GIA_LRP_PREPOSITIONS_DATABASE_TAG_CONDITION)
			{
				conditionNameNew = currentTagInPrepositionsInverseListSentence->tagName;	//same as currentTagInPrepositionsInverseList->tagName
			}
			else if(i == GIA_LRP_PREPOSITIONS_DATABASE_TAG_REVERSE_CONDITION)
			{
				if(currentTagInPrepositionsInverseListSentence->tagName == conditionName)
				{
					if(currentTagInPrepositionsInverseListSentence->tagName == conditionNameNew)
					{
						*twoWayConditionRequired = true;
						#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
						cout << "detectIfInverseOrTwoWayConditionRequired(): twoWayConditionRequired: conditionName = " << conditionName << endl;
						#endif
					}
					else
					{
						foundConditionToInvert = true;
						#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
						cout << "detectIfInverseOrTwoWayConditionRequired(): foundConditionToInvert: conditionName = " << conditionName << endl;
						#endif
					}
				}
			}	
			else if(i == GIA_LRP_PREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID)
			{
				if(foundConditionToInvert)
				{
					if(currentTagInPrepositionsInverseListSentence->tagName == GIA_LRP_PREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID_VALUE_TRUE)
					{
						*inverseConditionRequired = true;
						*inverseConditionName = conditionNameNew;
						#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
						cout << "detectIfInverseOrTwoWayConditionRequired(): inverseConditionRequired: conditionNameNew = " << conditionNameNew << endl;
						#endif
					}
				}
			}	
			currentTagInPrepositionsInverseListSentence = currentTagInPrepositionsInverseListSentence->nextTag;
		}
		
		currentTagInPrepositionsInverseList = currentTagInPrepositionsInverseList->nextSentence;
	}
}

#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
bool identifyConditionType(GIAentityNode* conditionEntity)
{
	bool conditionTypeIdentified = false;
	
	GIALRPtag* firstTagInPrepositionsInverseList = firstTagInPrepositionsInverseListGlobal;
	
	//identify condition type
	GIALRPtag* currentTagInPrepositionsInverseList = firstTagInPrepositionsInverseList;
	while(currentTagInPrepositionsInverseList->nextSentence != NULL)
	{
		#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
		//cout << "qcurrentTagInPlainText->sentenceIndex = " << currentTagInPlainText->sentenceIndex << endl;
		#endif
		
		if(currentTagInPrepositionsInverseList->tagName == conditionEntity->entityName)
		{
			GIALRPtag* firstTagInPrepositionsInverseListSentence = currentTagInPrepositionsInverseList;
			GIALRPtag* currentTagInPrepositionsInverseListSentence = firstTagInPrepositionsInverseListSentence;
			for(int i=1; i<=GIA_LRP_PREPOSITIONS_DATABASE_NUMBER_OF_TAGS; i++)
			{
				if(i == GIA_LRP_PREPOSITIONS_DATABASE_TAG_TYPE)
				{
					conditionEntity->conditionType2 = currentTagInPrepositionsInverseListSentence->tagName;
					#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
					cout << "identifyConditionTypeAndInvertIfNecessary(): conditionEntity->conditionType2 = " << conditionEntity->conditionType2 << endl;
					#endif
					conditionTypeIdentified = true;
				}		
				currentTagInPrepositionsInverseListSentence = currentTagInPrepositionsInverseListSentence->nextTag;
			}
		}
		
		currentTagInPrepositionsInverseList = currentTagInPrepositionsInverseList->nextSentence;
	}
		
	return conditionTypeIdentified;
}
#endif
		
#endif
