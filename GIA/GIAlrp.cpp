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
 * File Name: GIAlrp.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1p1a 08-September-2012
 * Requirements: requires plain text file
 * Description: Language Reduction Preprocessor
 *
 *******************************************************************************/



#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include "GIAlrp.h"



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

	entityIndex = -1;
	sentenceIndex = -1;

	next = NULL;
}
GIALRPtagTextCorrespondenceInfo::~GIALRPtagTextCorrespondenceInfo(void)
{
}

GIALRPtagTextCorrespondenceInfo * textGIALRPtagTextCorrespondenceInfo;
GIALRPtagTextCorrespondenceInfo * queryGIALRPtagTextCorrespondenceInfo;
GIALRPtagTextCorrespondenceInfo * currentGIALRPtagTextCorrespondenceInfo;
GIALRPtagTextCorrespondenceInfo * getCurrentGIALRPtagTextCorrespondenceInfo()
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
void initialiseCurrentGIALRPtagTextCorrespondenceInfo()
{
	queryGIALRPtagTextCorrespondenceInfo = new GIALRPtagTextCorrespondenceInfo();
	textGIALRPtagTextCorrespondenceInfo = new GIALRPtagTextCorrespondenceInfo();
}



bool parseTextFileAndReduceLanguage(string plainTextInputFileName, string plainTextLRPOutputFileName, string plainTextLRPforNLPOutputFileName)
{
	bool result = true;

	string irregularVerbListFileName = GIA_LRP_IRREGULARVERB_DATABASE_FILE_NAME;
	GIALRPtag * firstTagInIrregularVerbList = new GIALRPtag();
	if(!loadIrregularVerbList(irregularVerbListFileName, firstTagInIrregularVerbList))
	{
		result = false;
	}

	string phrasalVerbListFileName = GIA_LRP_PHRASALVERB_DATABASE_FILE_NAME;
	GIALRPtag * firstTagInPhrasalVerbList = new GIALRPtag();
	if(!loadPhrasalVerbDataAndGenerateAllTenseVariants(phrasalVerbListFileName, firstTagInPhrasalVerbList, firstTagInIrregularVerbList))
	{
		result = false;
	}
	
	string multiwordPrepositionListFileName = GIA_LRP_MULTIWORDPREPOSITION_DATABASE_FILE_NAME;
	GIALRPtag * firstTagInMultiwordPrepositionList = new GIALRPtag();
	if(!loadMultiWordPrepositionData(multiwordPrepositionListFileName, firstTagInMultiwordPrepositionList))
	{
		result = false;
	}
	
	GIALRPtag * firstTagInPlainText = new GIALRPtag();
	if(!loadPlainTextFile(plainTextInputFileName, firstTagInPlainText))
	{
		result = false;
	}
	
	GIALRPtagTextCorrespondenceInfo * firstGIALRPtagCorrespondenceInfo = getCurrentGIALRPtagTextCorrespondenceInfo();
	if(!searchAndReplaceAllPhrasalVerbsAndMultiwordPrepositions(firstTagInPlainText, firstTagInPhrasalVerbList, firstTagInMultiwordPrepositionList, plainTextLRPOutputFileName, plainTextLRPforNLPOutputFileName, firstGIALRPtagCorrespondenceInfo))
	{
		result = false;
	}

	return result;
}


bool loadIrregularVerbList(string irregularVerbListFileName, GIALRPtag * firstTagInIrregularVerbList)
{
	bool result = true;

	GIALRPtag * currentTagInIrregularVerbList = firstTagInIrregularVerbList;

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
					currentTagInIrregularVerbList->alternateTag = new GIALRPtag();
					currentTagInIrregularVerbList->alternateTag->tagName = currentWord;
				}
				else
				{
					currentTagInIrregularVerbList->tagName = currentWord;
				}
				#ifdef GIA_LRP_DEBUG
				cout <<	"adding Tag: tagName = " << currentTagInIrregularVerbList->tagName << endl;
				if(currentWordAlternate)
				{
					cout <<	"adding Tag: alternateTag = " << currentTagInIrregularVerbList->alternateTag->tagName << endl;
				}
				#endif

				currentTagInIrregularVerbList->nextTag = new GIALRPtag();
				currentTagInIrregularVerbList = currentTagInIrregularVerbList->nextTag;

				wordIndex = 0;
				currentWord = "";
				currentWordAlternate = false;

				//start parsing new line

				//UP TO HERE
			}
			else if(currentToken == GIA_LRP_PHRASALVERB_DATABASE_TAG_ALTERNATE)
			{
				currentTagInIrregularVerbList->tagName = currentWord;

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
bool loadPhrasalVerbDataAndGenerateAllTenseVariants(string phrasalVerbDatabaseFileName, GIALRPtag * firstTagInPhrasalVerbList, GIALRPtag * firstTagInIrregularVerbList)
{
	bool result = true;

	GIALRPtag * currentTagInPhrasalVerbList = firstTagInPhrasalVerbList;
	GIALRPtag * recordOfNonAlternateTagInPhrasalVerbList = currentTagInPhrasalVerbList;

	GIALRPtag * currentTagInPhrasalVerb = firstTagInPhrasalVerbList;
	GIALRPtag * recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;

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
						//cout <<	"adding Tag: tagName = " << currentTagInPhrasalVerbList->tagName << endl;				
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
					cout << "warning: parsingVerbBase && (currentToken == GIA_LRP_PHRASALVERB_DATABASE_TAG_ALTERNATE) - ignoring second alternate base" << endl;
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



bool generateTenseVariantsOfVerbBase(GIALRPtag * baseTag, GIALRPtag * firstTagInIrregularVerbList)
{
	bool result = true;

	string base = convertStringToLowerCase(&(baseTag->tagName));

	//1. check if irregular past / past participle case
	bool irregularVerbFound = false;
	GIALRPtag * currentTagInIrregularVerbList = firstTagInIrregularVerbList;
	while(currentTagInIrregularVerbList->nextSentence != NULL)
	{
		if(currentTagInIrregularVerbList->tagName == base)
		{
			irregularVerbFound = true;
			GIALRPtag *	firstTagInIrregularVerb = currentTagInIrregularVerbList;
			GIALRPtag * currentTagInIrregularVerb = firstTagInIrregularVerb->nextTag;
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
	/*
	cout << "baseStringLength = " << baseStringLength << endl;	
	cout << "indexOfLastCharacterInBase = " << indexOfLastCharacterInBase << endl;
	cout << "indexOfSecondLastCharacterInBase = " << indexOfSecondLastCharacterInBase << endl;
	cout << "indexOfThirdLastCharacterInBase = " << indexOfThirdLastCharacterInBase << endl;
	cout << "lastCharacterInBase = " << lastCharacterInBase << endl;
	cout << "secondLastCharacterInBase = " << secondLastCharacterInBase << endl;
	cout << "thirdLastCharacterInBase = " << thirdLastCharacterInBase << endl;
	*/
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
		if(base[indexOfLastCharacterInBase] == lrpVerbDoubleConsonantRule1LastLetterExceptions[i])
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
	//cout << "baseWithLastLetterDropped = " << baseWithLastLetterDropped << endl;
	
	//generate (ie record) infinitive tense form [for consistency; even though this is the same as tagName...]
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base;
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base;

	if(rule1)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule1)" << endl;
		#endif

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;
		if(rule1b)
		{
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;
		}

		//b. generate continuous tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;
		if(rule1b)
		{
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;	//double consonant
		}

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
			if(rule1b)
			{
				baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;	//double consonant
			}

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
			if(rule1b)
			{
				baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;	//double consonant
			}
		}

		if(!rule1b)
		{
			copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
		}
	}
	else if(rule2)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule2)" << endl;
		#endif

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;

		//b. generate continuous tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
		}

		copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}
	else if(rule3a)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule3a)" << endl;
		#endif

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_CASE3;

		//b. generate continuous tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3;
		}

		copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}
	else if(rule3b || rule4)
	{
		#ifdef GIA_LRP_DEBUG
		cout << "(rule3b || rule4)" << endl;
		cout << "rule3b = " << rule3b << endl;
		cout << "rule4 = " << rule4 << endl;		
		#endif
		
		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;

		//b. generate continuous tense form
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
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

void copyDefaultVerbTenseFormsToAlternateTenseForms(GIALRPtag * baseTag, bool irregularVerbFound)
{
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	if(!irregularVerbFound)
	{
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
		baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	}

}

bool loadMultiWordPrepositionData(string multiwordPrepositionListFileName, GIALRPtag * firstTagInMultiwordPrepositionList)
{
	bool result = true;

	GIALRPtag * currentTagInMultiwordPrepositionList = firstTagInMultiwordPrepositionList;
	GIALRPtag * firstTagInMultiwordPreposition = currentTagInMultiwordPrepositionList;
	GIALRPtag * currentTagInMultiwordPreposition = firstTagInMultiwordPreposition;

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

bool loadPlainTextFile(string plainTextInputFileName, GIALRPtag * firstTagInPlainText)
{
	bool result = true;
	
	GIALRPtag * currentTagInPlainText = firstTagInPlainText;
	GIALRPtag * firstTagInPlainTextSentence = currentTagInPlainText;
	GIALRPtag * currentTagInPlainTextSentence = firstTagInPlainTextSentence;

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
			if(whiteSpaceFound || punctuationMarkFound)
			{
				if(!whiteSpace)
				{
					currentTagInPlainTextSentence->tagName = currentWord;
					currentTagInPlainTextSentence->entityIndex = entityIndex;
					currentTagInPlainTextSentence->sentenceIndex = sentenceIndex;					
					currentTagInPlainTextSentence->nextTag = new GIALRPtag();
					#ifdef GIA_LRP_DEBUG
					cout <<  "adding Tag: " << currentTagInPlainTextSentence->tagName << endl;								
					#endif
					currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
					entityIndex++;
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
						
						currentTagInPlainText->nextSentence = new GIALRPtag();
						currentTagInPlainText = currentTagInPlainText->nextSentence;

						if(currentToken == CHAR_FULLSTOP)
						{
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
			}
			else
			{
				whiteSpace = false;
				currentWord = currentWord + currentToken;
			}
			charCount++;
		}
		parseFileObject.close();
	}
	
	return result;
}


//NB the collapsed phrasal verb contains precisely 2 entities: phrasalVerbCollapsed, entity2: thing/place/body (eg belong_to + sb/Tom) - thing/place/bodies are not currently being differentiated by the LRP as this information is only first generated at NLP/GIA parse stage
bool searchAndReplaceAllPhrasalVerbsAndMultiwordPrepositions(GIALRPtag * firstTagInPlainText, GIALRPtag * firstTagInPhrasalVerbList, GIALRPtag * firstTagInMultiwordPrepositionList, string plainTextLRPOutputFileName, string plainTextLRPforNLPOutputFileName, GIALRPtagTextCorrespondenceInfo * firstGIALRPtagCorrespondenceInfo)
{
	bool result = false;

	//string plainTextInput = "";
	string plainTextLRPOutput = "";
	string plainTextLRPforNLPOutput = "";

	GIALRPtagTextCorrespondenceInfo * currentCorrespondenceInfo = firstGIALRPtagCorrespondenceInfo;	//new correspondence info for each found phrasal verb

	GIALRPtag * currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{		
		GIALRPtag * firstTagInPlainTextSentence = currentTagInPlainText;
		GIALRPtag * currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIALRPtag * previousTagInPlainTextSentence = NULL;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			//cout << "currentTagInPlainTextSentence->tagName = " << currentTagInPlainTextSentence->tagName << endl;
			
			bool foundAtLeastOnePhrasalVerbInSentenceAndCollapsed = false;
			GIALRPtag * currentTagInPhrasalVerbList = firstTagInPhrasalVerbList;
			while(currentTagInPhrasalVerbList->nextSentence != NULL)
			{
				//cout << "currentTagInPhrasalVerbList->tagName = " << currentTagInPhrasalVerbList->tagName << endl;
				
				GIALRPtag * currentTagInPhrasalVerbListAlternate = currentTagInPhrasalVerbList;
				bool alternatePhrasalVerb = false;
				while(!alternatePhrasalVerb || (currentTagInPhrasalVerbListAlternate->alternateSentence != NULL))
				{
					/*
					if(tolower((currentTagInPhrasalVerbList->tagName)[0]) == tolower((currentTagInPlainTextSentence->tagName)[0]))
					{//optimisation; only deal with phrasal verbs that start with the same character...
					*/
					GIALRPtag * firstTagInPhrasalVerb = currentTagInPhrasalVerbListAlternate;
					GIALRPtag * currentTagInPhrasalVerb = firstTagInPhrasalVerb;
					bool stillFoundVerbMatchOfArbitraryTense = true;
					bool foundAtLeastOneMatch = false;
					GIALRPtag * currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
					GIALRPtag * lastTagInPlainTextSentenceTemp = NULL;	//pointer used to change original plain text to collapsed phrasal verb text
					GIALRPtag * firstTagInCollapsedPhrasalVerb = new GIALRPtag();
					GIALRPtag * currentTagInCollapsedPhrasalVerb = firstTagInCollapsedPhrasalVerb;
					bool currentlyParsingTagSpecial = false;	
					int tagInPhrasalVerbSpecialAndNotFoundCount = 0;
					int numberTagSpecialTagsFound = 0;
					int numberTagConsecutiveOptionalTagsFound = 0;
					bool phrasalVerbHasTagSpecial = false;
					while((currentTagInPhrasalVerb->nextTag != NULL) && (currentTagInPlainTextSentenceTemp->nextTag != NULL) && (stillFoundVerbMatchOfArbitraryTense))
					{
						//cout << "currentTagInPhrasalVerb->tagName = " << currentTagInPhrasalVerb->tagName << endl;
						//cout << "currentTagInPlainTextSentenceTemp->tagName = " << currentTagInPlainTextSentenceTemp->tagName << endl;
						
						bool currentTagInPhrasalVerbOptionalAndNotFound = false;
						bool currentTagInPhrasalVerbSpecialAndNotFound = false;
						
						if(currentTagInPhrasalVerb->tagSpecialArbitraryName)
						{
							//cout << "u1" << endl;
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
							//cout << "u2" << endl;
														
							bool foundVerbMatchOfArbitraryTenseTemp = false;
							GIALRPtag * currentTagInPlainTextSentenceTempAlternate = currentTagInPlainTextSentenceTemp;
							bool alternateTag = false;
							string generatedTagNameLemma = "";
							while(!alternateTag || (currentTagInPlainTextSentenceTempAlternate->alternateTag != NULL))
							{
								//cout << "u3" << endl;
								
								if(currentTagInPhrasalVerb->base)
								{
									for(int i=0; i<GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
									{
										for(int j=0; j<GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS; j++)
										{
											/*
											//cout << "currentTagInPhrasalVerb->tagName =" << currentTagInPhrasalVerb->tagName << endl;
											//cout << "(currentTagInPhrasalVerb->tagName).length()  =" << (currentTagInPhrasalVerb->tagName).length() << endl;
											if(currentTagInPhrasalVerb->tagName == "up")
											{
												cout << "currentTagInPhrasalVerb->grammaticalTenseFormsArray[i][j] = " << currentTagInPhrasalVerb->grammaticalTenseFormsArray[i][j] << endl;
												cout << "currentTagInPlainTextSentenceTempAlternate->tagName = " << currentTagInPlainTextSentenceTempAlternate->tagName << endl;
											}
											*/

											if(currentTagInPhrasalVerb->grammaticalTenseFormsArray[i][j] == currentTagInPlainTextSentenceTempAlternate->tagName)		//USED TO BE currentTagInCollapsedPhrasalVerb before 6 Sept 2012
											{
												//cout << "u4" << endl;
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
									cout << "u4" << endl;
									currentTagInPlainTextSentenceTempAlternate = currentTagInPlainTextSentenceTempAlternate->alternateTag;
								}
								alternateTag = true;
								//cout << "u5" << endl;
							}


							if(!foundVerbMatchOfArbitraryTenseTemp)
							{
								//cout << "u6" << endl;
								if(currentTagInPhrasalVerb->optional)
								{
									//cout << "u7" << endl;
									currentTagInPhrasalVerbOptionalAndNotFound = true;
								}
								else if(currentlyParsingTagSpecial && (tagInPhrasalVerbSpecialAndNotFoundCount <= GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_MAX_NUMBER_WORDS))
								{
									//cout << "u8" << endl;
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
									//cout << "u9" << endl;
									stillFoundVerbMatchOfArbitraryTense = false;
								}
							}
							else
							{
								if(currentlyParsingTagSpecial)
								{
									//cout << "u10" << endl;
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
							//cout << "u11b" << endl;
						}
						
						//cout << "u11c" << endl;
						
						if(currentTagInPhrasalVerb->tagSpecialArbitraryName)
						{
							if(currentTagInPhrasalVerb->optional)
							{
								//cout << "u12" << endl;
								//special+optional tag found - just increment the phrasal verb for now [for this round]...
								currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
							}
							else
							{
								//cout << "u13" << endl;
								currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
								currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
							}
						}
						else if(currentTagInPhrasalVerbOptionalAndNotFound)
						{
							//cout << "u14" << endl;
							//optional phrasal verb tag not found - just increment the phrasal verb tag list...
							currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
						}
						else if(currentTagInPhrasalVerbSpecialAndNotFound)
						{
							//cout << "u15" << endl;
							//next phrasal verb tag not found yet - just increment the sentence tag list...
							currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
						}
						else
						{
							//cout << "u16" << endl;
							currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
							currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;						
						}
					}

					if(stillFoundVerbMatchOfArbitraryTense && foundAtLeastOneMatch)
					{
						//cout << "u17" << endl;
						if(numberTagSpecialTagsFound <= 1)
						{//do not preprocess phrasal verbs with more than one special tag (ie sth/sb/swh) - as this generally involves more than a verb [verb sth preposition sth1] - added 1p1aTEMP5
							//reduce all entities
							//cout << "u18" << endl;
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
							//cout << "u18b" << endl;
							previousTagInPlainTextSentence->nextTag = firstTagInCollapsedPhrasalVerb;

							foundAtLeastOnePhrasalVerbInSentenceAndCollapsed = true;
							//cout << "u18c" << endl;
						}
					}
					/*
					}
					*/
					//cout << "u19" << endl;

					if(currentTagInPhrasalVerbListAlternate->alternateSentence != NULL)
					{
						//cout << "u20" << endl;
						currentTagInPhrasalVerbListAlternate = currentTagInPhrasalVerbListAlternate->alternateSentence;
					}
					//cout << "u21" << endl;
					alternatePhrasalVerb = true;
				}
				/*
				cout << "u22" << endl;
				if(currentTagInPhrasalVerbList->alternateSentence != NULL)
				{
					cout << "u23" << endl;
					currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->alternateSentence;
				}
				*/
				currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->nextSentence;
			}
			//cout << "u23" << endl;
			if(foundAtLeastOnePhrasalVerbInSentenceAndCollapsed)
			{
				//cout << "u23b" << endl;
				//renumberEntityIndiciesInSentence();
				GIALRPtag * currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
				int newEntityIndex = GIA_NLP_START_ENTITY_INDEX;
				while(currentTagInPlainTextSentenceTemp2->nextTag != NULL)
				{
					//cout << "u23c currentTagInPlainTextSentenceTemp2->tagName = " << currentTagInPlainTextSentenceTemp2->tagName << endl;
					if(currentTagInPlainTextSentenceTemp2->collapsedPhrasalVerbExactDefinedSection)
					{//create a new correspondenceInfo
						//cout << "u23d" << endl;
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
					//cout << "u23e" << endl;
					currentTagInPlainTextSentenceTemp2->entityIndex = newEntityIndex;
					//cout << "u23f" << endl;
					currentTagInPlainTextSentenceTemp2 = currentTagInPlainTextSentenceTemp2->nextTag;
					//cout << "u23g" << endl;
					newEntityIndex++;
				}
			}
			//cout << "u24" << endl;

			GIALRPtag * currentTagInMultiwordPrepositionList = firstTagInMultiwordPrepositionList;
			bool foundAtLeastOneMultiwordPrepositionInSentenceAndCollapsed = false;
			while(currentTagInMultiwordPrepositionList->nextSentence != NULL)
			{
				bool stillFoundPrepositionMatch = true;
				bool foundAtLeastOneMatch = false;
				GIALRPtag * currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
				GIALRPtag * firstTagInCollapsedMultiwordPreposition = new GIALRPtag();
				GIALRPtag * currentTagInCollapsedMultiwordPreposition = firstTagInCollapsedMultiwordPreposition;
				while((currentTagInMultiwordPrepositionList->nextTag != NULL) && (currentTagInPlainTextSentenceTemp->nextTag != NULL) && (stillFoundPrepositionMatch))
				{
					if(currentTagInMultiwordPrepositionList->tagName != currentTagInPlainTextSentenceTemp->tagName)
					{
						stillFoundPrepositionMatch = false;
					}
					else
					{
						currentTagInCollapsedMultiwordPreposition->tagName = currentTagInCollapsedMultiwordPreposition->tagName + currentTagInPlainTextSentenceTemp->tagName;
						foundAtLeastOneMatch = true;
					}
				}

				if(stillFoundPrepositionMatch && foundAtLeastOneMatch)
				{
					//reduce all entities
					firstTagInCollapsedMultiwordPreposition->nextTag = currentTagInPlainTextSentenceTemp;
					previousTagInPlainTextSentence->nextTag = firstTagInCollapsedMultiwordPreposition;

					foundAtLeastOneMultiwordPrepositionInSentenceAndCollapsed = true;
				}

				currentTagInMultiwordPrepositionList = currentTagInMultiwordPrepositionList->nextSentence;
			}
			if(foundAtLeastOneMultiwordPrepositionInSentenceAndCollapsed)
			{
				//renumberEntityIndiciesInSentence();
				GIALRPtag * currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
				int newEntityIndex = 0;
				//int collapsedMultiwordPrepositionIndex = 0;
				while(currentTagInPlainTextSentenceTemp2->nextTag != NULL)
				{
					if(currentTagInPlainTextSentenceTemp2->collapsedMultiwordPreposition)
					{//create a new correspondenceInfo
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
			//cout << "u25" << endl;

			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}
		currentTagInPlainText = currentTagInPlainText->nextTag;
	}

	if(!writeTagListToFile(firstTagInPlainText, plainTextLRPOutputFileName, plainTextLRPforNLPOutputFileName))
	{
		result = false;
	}

	return result;
}

bool writeTagListToFile(GIALRPtag * firstTagInPlainText, string plainTextLRPOutputFileName, string plainTextLRPforNLPOutputFileName)
{
	bool result = true;
	
	ofstream plainTextLRPOutput(plainTextLRPOutputFileName.c_str());
	ofstream plainTextLRPforNLPOutput(plainTextLRPforNLPOutputFileName.c_str());
	
	bool firstCharacterInFile = true;
	
	GIALRPtag * currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{				
		GIALRPtag * firstTagInPlainTextSentence = currentTagInPlainText;
		GIALRPtag * currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			string plainTextLRPOutputTag = currentTagInPlainTextSentence->tagName;		
			string plainTextLRPforNLPOutputTag = "";
			if(currentTagInPlainTextSentence->collapsedPhrasalVerbExactDefinedSection)
			{//create a new correspondenceInfo
				//cout << "q2" << endl;
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
				

void revertNLPtagNameToOfficialLRPtagName(Feature * feature, Sentence * currentSentenceInList, Relation * currentRelationInListForPrepositionsOnly, bool isPreposition, bool foundOfficialLRPreplacementString)
{
	int entityIndexForNonPrepositionsOnly = feature->entityIndex;
	string word = feature->word;
	
	GIALRPtagTextCorrespondenceInfo * firstGIALRPtagCorrespondenceInfo = getCurrentGIALRPtagTextCorrespondenceInfo();
	
	int sentenceIndex = currentSentenceInList->sentenceIndex;

	bool foundCorrespondingLRPtag = false;
	GIALRPtagTextCorrespondenceInfo * currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = firstGIALRPtagCorrespondenceInfo;
	while(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next != NULL)
	{
		/*
		cout << "word = " << word << endl;
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->lemmaWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->lemmaWithLRP << endl; 
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl; 
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly << endl;
		cout << "sentenceIndex = " << sentenceIndex << endl;
		cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex << endl;
		*/
		
		if(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex == sentenceIndex)
		{	
			//cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl;
					
			if(word == currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly)
			{	
							
				if(isPreposition) 
				{
					//cout << "isPreposition" << endl;
					
					//now search entire sentence->feature list and find entity/word that has same name, and has the governor/dependent closest to it... 
					string relationGovernor = currentRelationInListForPrepositionsOnly->relationGovernor;
					string relationDependent = currentRelationInListForPrepositionsOnly->relationDependent;
					Feature * firstFeatureInList = currentSentenceInList->firstFeatureInList;
					Feature * currentFeatureInList = firstFeatureInList;
					int indexOfPrepositionWithMinimumProximityOfGovernorDependentWords = ENTITY_INDEX_UNDEFINED;
					int minimumProximityOfGovernorDependentWords = MAXIMUM_NUMBER_WORDS_PER_SENTENCE;
					int indexOfLastInstanceOfPreposition = ENTITY_INDEX_UNDEFINED;
					int indexOfLastInstanceOfGovernor = ENTITY_INDEX_UNDEFINED;
					int indexOfLastInstanceOfDependent = ENTITY_INDEX_UNDEFINED;
					bool foundPrepositionGovernorAndDependentInFeatureList = false;
					while(currentFeatureInList->next != NULL)
					{
						if(currentFeatureInList->word == relationGovernor)
						{
							indexOfLastInstanceOfGovernor = currentFeatureInList->entityIndex;
						}					
						if(currentFeatureInList->word == word)	//or currentFeatureInList->word ; as preposition lemmas are the same as their words
						{
							indexOfLastInstanceOfPreposition = currentFeatureInList->entityIndex;
						}
						if(currentFeatureInList->word == relationDependent)
						{
							indexOfLastInstanceOfDependent = currentFeatureInList->entityIndex;
							if((indexOfLastInstanceOfPreposition != ENTITY_INDEX_UNDEFINED) && (indexOfLastInstanceOfGovernor != ENTITY_INDEX_UNDEFINED))
							{
								int proximityOfPrepositionToGovernor = indexOfLastInstanceOfPreposition - indexOfLastInstanceOfGovernor;
								int proximityOfPrepositionToDependent = indexOfLastInstanceOfDependent - indexOfLastInstanceOfPreposition;
								int totalProximityOfPrepositionToGovernorAndDependent = proximityOfPrepositionToGovernor + proximityOfPrepositionToDependent;
								if(totalProximityOfPrepositionToGovernorAndDependent < minimumProximityOfGovernorDependentWords)
								{
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
					//cout << "!isPreposition" << endl;
					//cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex << endl;
					//cout << "entityIndexForNonPrepositionsOnly = " << entityIndexForNonPrepositionsOnly << endl;
					
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
		//cout << "q25" << endl;
		currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next;
		//cout << "q26" << endl;
	}
	if(!foundCorrespondingLRPtag)
	{
		//cout << "revertNLPtagNameToOfficialLRPtagName() warning: !foundCorrespondingLRPtag; " << lemma << endl;
	}
	//cout << "q27" << endl;
}

string convertStringToLowerCase(string * arbitraryCaseString)
{
	string lowerCaseString = *arbitraryCaseString;
	for(int i=0; i<arbitraryCaseString->length(); i++)
	{
		lowerCaseString[i] = tolower((*arbitraryCaseString)[i]);
	}
	return lowerCaseString;
}
