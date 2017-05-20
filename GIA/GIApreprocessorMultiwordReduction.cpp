/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIApreprocessorMultiwordReduction.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b1a 19-May-2017
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 *
 *******************************************************************************/


#include "GIApreprocessorMultiwordReduction.hpp"

static string lrpDataFolderName;
static bool useLRP;
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_IRREGULAR_VERB_LIST
GIApreprocessorMultiwordReductionSentence* firstTagInIrregularVerbListGlobal;
bool irregularVerbListLoaded;
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
GIApreprocessorMultiwordReductionSentence* firstTagInPrepositionInverseListGlobal;
bool prepositionInverseListLoaded;
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
GIApreprocessorMultiwordReductionWord* firstTagInVerbListGlobal;
bool verbListLoaded;
GIApreprocessorMultiwordReductionWord* firstTagInPrepositionListGlobal;
bool prepositionListLoaded;
GIApreprocessorMultiwordReductionWord* firstTagInAdverbListGlobal;
bool adverbListLoaded;
GIApreprocessorMultiwordReductionWord* firstTagInAdjectiveListGlobal;
bool adjectiveListLoaded;
GIApreprocessorMultiwordReductionWord* firstTagInNounListGlobal;
bool nounListLoaded;
#endif

GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* textGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* queryGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* activeGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;


GIApreprocessorMultiwordReductionWord::GIApreprocessorMultiwordReductionWord(void)
{
	tagName = "";
	tagNameLemma = "";

	tagSpecialArbitraryName = false;
	tagSpecialArbitraryNameType = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_UNDEFINED;
	optional = false;
	/*
	primary = false;
	primaryPhrasalVerbReplacementString = "";
	primaryPhrasalVerbReplacementStringNLPonly = "";
	*/

	base = false;
	for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
	{
		grammaticalTenseFormsArray[i][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = "";
		grammaticalTenseFormsArray[i][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = "";
	}
	grammaticalTenseFormDetected = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE;

	entityIndex = false;
	collapsedPhrasalVerbExactDefinedSection = false;
	collapsedMultiwordWord = false;
	collapsedMultiwordWordType = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_UNDEFINED_TYPE;
	collapsedMultiwordWordTemp = false;
	
	alternateTag = NULL;
	nextTag = NULL;
}
GIApreprocessorMultiwordReductionWord::~GIApreprocessorMultiwordReductionWord(void)
{
}

GIApreprocessorMultiwordReductionSentence::GIApreprocessorMultiwordReductionSentence(void)
{
	sentenceIndex = false;

	firstTagInSentence = new GIApreprocessorMultiwordReductionWord();
	
	alternateSentence = NULL;
	nextSentence = NULL;
}
GIApreprocessorMultiwordReductionSentence::~GIApreprocessorMultiwordReductionSentence(void)
{
}



GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo::GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(void)
{
	wordWithLRP = "";
	wordWithLRPforNLPonly = "";

	entityIndex = INT_DEFAULT_VALUE;
	sentenceIndex = INT_DEFAULT_VALUE;

	next = NULL;
}
GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo::~GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(void)
{
}


bool GIApreprocessorMultiwordReductionClass::initialiseLRP(const string newLRPDataFolderName, const bool newUseLRP)
{
	bool result = true;

	useLRP = newUseLRP;
	lrpDataFolderName = newLRPDataFolderName;

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_IRREGULAR_VERB_LIST
	string irregularVerbListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_IRREGULARVERB_DATABASE_FILE_NAME;
	irregularVerbListLoaded = false;
	firstTagInIrregularVerbListGlobal = new GIApreprocessorMultiwordReductionSentence();
	if(!this->loadIrregularVerbList(irregularVerbListFileName, firstTagInIrregularVerbListGlobal))
	{
		cout << "!loadIrregularVerbList (GIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE requires -lrpfolder to be set): irregularVerbListFileName = " << irregularVerbListFileName << endl;
		result = false;
	}
	else
	{
		irregularVerbListLoaded = true;
	}
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
	string prepositionsInverseListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_FILE_NAME;
	prepositionInverseListLoaded = false;
	firstTagInPrepositionInverseListGlobal = new GIApreprocessorMultiwordReductionSentence();
	if(!this->loadPrepositionsInverseList(prepositionsInverseListFileName, firstTagInPrepositionInverseListGlobal))
	{
		cout << "!loadPrepositionsInverseList (GIA with GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS requires -lrpfolder to be set): prepositionsInverseListFileName = " << prepositionsInverseListFileName << endl;
		result = false;
	}
	else
	{
		prepositionInverseListLoaded = true;
	}
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
	if(!this->loadWordListWrapper(lrpDataFolderName, &verbListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_FILE_NAME, &firstTagInVerbListGlobal))
	{
		result = false;	
	}	
	if(!this->loadWordListWrapper(lrpDataFolderName, &adverbListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PREPOSITION_DATABASE_FILE_NAME, &firstTagInPrepositionListGlobal))
	{
		result = false;	
	}
	if(!this->loadWordListWrapper(lrpDataFolderName, &adverbListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_ADVERB_DATABASE_FILE_NAME, &firstTagInAdverbListGlobal))
	{
		result = false;	
	}
	if(!this->loadWordListWrapper(lrpDataFolderName, &adjectiveListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_ADJECTIVE_DATABASE_FILE_NAME, &firstTagInAdjectiveListGlobal))
	{
		result = false;	
	}
	if(!this->loadWordListWrapper(lrpDataFolderName, &nounListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_FILE_NAME, &firstTagInNounListGlobal))
	{
		result = false;	
	}
	
	if(!parseVerbDataGenerateAllTenseVariants())	//this is required to make verbList usable
	{
		result = false;
	}
	#endif
	


	
	return result;
}
bool GIApreprocessorMultiwordReductionClass::getUseLRP()
{
	return useLRP;
}

bool GIApreprocessorMultiwordReductionClass::loadWordListWrapper(const string lrpDataFolderName, bool* wordListLoaded, const string wordListFileName, GIApreprocessorMultiwordReductionWord** firstTagInWordListGlobal)
{
	bool result = true;
	
	string wordListFileNameFull = lrpDataFolderName + wordListFileName;
	*wordListLoaded = false;
	*firstTagInWordListGlobal = new GIApreprocessorMultiwordReductionWord();
	if(!this->loadWordList(wordListFileNameFull, *firstTagInWordListGlobal))
	{
		cout << "!loadWordList (GIA with wordListFileName requires -lrpfolder to be set): wordListFileNameFull = " << wordListFileNameFull << endl;
		result = false;
	}
	else
	{
		*wordListLoaded = true;
	}
	return result;
}

bool GIApreprocessorMultiwordReductionClass::loadWordList(const string wordListFileName, GIApreprocessorMultiwordReductionWord* firstTagInWordList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionWord* currentTagInWordList = firstTagInWordList;

	ifstream parseFileObject(wordListFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//txt file does not exist in current directory.
		cout << "Error: Word list File does not exist in current directory: " << wordListFileName << endl;
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
				currentTagInWordList->tagName = currentWord;
				currentTagInWordList->nextTag = new GIApreprocessorMultiwordReductionWord();
				currentTagInWordList = currentTagInWordList->nextTag;
				currentWord = "";
			}
			else
			{
				string c = "";
				c = c + currentToken;
				
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
				if(currentToken == CHAR_SPACE)
				{
					c = GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER;	//assuming that multiword preprocessor has been executed
				}
				#endif
				
				currentWord = currentWord + c;
			}

			charCount++;
		}
		parseFileObject.close();
	}

	return result;
}









GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* GIApreprocessorMultiwordReductionClass::getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo()
{
	return activeGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
}
void GIApreprocessorMultiwordReductionClass::setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery)
{
	if(isQuery)
	{
		activeGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo = queryGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
	}
	else
	{
		activeGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo = textGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
	}
}
void GIApreprocessorMultiwordReductionClass::initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery)
{
	if(isQuery)
	{
		queryGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo = new GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	}
	else
	{
		textGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo = new GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	}
}
void GIApreprocessorMultiwordReductionClass::deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery)
{
	if(isQuery)
	{
		delete queryGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
	}
	else
	{
		delete textGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
	}
}


bool GIApreprocessorMultiwordReductionClass::parseTextFileAndReduceLanguage(const string plainTextInputFileName, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName)
{
	bool result = true;


	string irregularVerbListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_IRREGULARVERB_DATABASE_FILE_NAME;
	GIApreprocessorMultiwordReductionSentence* firstTagInIrregularVerbList = new GIApreprocessorMultiwordReductionSentence();
	if(!this->loadIrregularVerbList(irregularVerbListFileName, firstTagInIrregularVerbList))
	{
		result = false;
	}

	string phrasalVerbListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_FILE_NAME;
	GIApreprocessorMultiwordReductionSentence* firstTagInPhrasalVerbList = new GIApreprocessorMultiwordReductionSentence();
	if(!this->loadPhrasalVerbDataAndGenerateAllTenseVariants(phrasalVerbListFileName, firstTagInPhrasalVerbList, firstTagInIrregularVerbList))
	{
		result = false;
	}

	GIApreprocessorMultiwordReductionSentence* firstTagInPlainText = new GIApreprocessorMultiwordReductionSentence();
	if(!this->loadPlainTextFile(plainTextInputFileName, firstTagInPlainText))
	{
		result = false;
	}

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo = this->getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	if(!this->searchAndReplacePhrasalVerbs(firstTagInPlainText, firstTagInPhrasalVerbList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo))
	{
		result = false;
	}

	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_PREPOSITION_DATABASE_FILE_NAME, firstTagInPlainText, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_TYPE))
	{
		result = false;
	}
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_ALL_WORD_TYPES
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_ADVERB_DATABASE_FILE_NAME, firstTagInPlainText, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADVERB_TYPE))
	{
		result = false;
	}
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_ADJECTIVE_DATABASE_FILE_NAME, firstTagInPlainText, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADJECTIVE_TYPE))
	{
		result = false;
	}
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_NOUN_DATABASE_FILE_NAME, firstTagInPlainText, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_NOUN_TYPE))
	{
		result = false;
	}
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_VERB_DATABASE_FILE_NAME, firstTagInPlainText, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_TYPE))
	{
		result = false;
	}			
	#endif

	SHAREDvars.setCurrentDirectory(outputFolder);
	if(!this->writeTagListToFile(firstTagInPlainText, plainTextLRPoutputFileName, plainTextLRPforNLPoutputFileName, true))
	{
		result = false;
	}

	return result;
}

bool GIApreprocessorMultiwordReductionClass::loadIrregularVerbList(const string irregularVerbListFileName, GIApreprocessorMultiwordReductionSentence* firstTagInIrregularVerbList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionSentence* currentTagInIrregularVerbList = firstTagInIrregularVerbList;
	GIApreprocessorMultiwordReductionWord* firstTagInIrregularVerb = currentTagInIrregularVerbList->firstTagInSentence;
	GIApreprocessorMultiwordReductionWord* currentTagInIrregularVerb = firstTagInIrregularVerb;

	ifstream parseFileObject(irregularVerbListFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
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
					currentTagInIrregularVerb->alternateTag = new GIApreprocessorMultiwordReductionWord();
					currentTagInIrregularVerb->alternateTag->tagName = currentWord;
				}
				else
				{
					currentTagInIrregularVerb->tagName = currentWord;
				}

				currentTagInIrregularVerb->nextTag = new GIApreprocessorMultiwordReductionWord();
				currentTagInIrregularVerb = currentTagInIrregularVerb->nextTag;

				if(currentToken == CHAR_NEWLINE)
				{
					currentTagInIrregularVerbList->nextSentence = new GIApreprocessorMultiwordReductionSentence();
					currentTagInIrregularVerbList = currentTagInIrregularVerbList->nextSentence;
					currentTagInIrregularVerb = currentTagInIrregularVerbList->firstTagInSentence;
				}

				wordIndex = 0;
				currentWord = "";
				currentWordAlternate = false;

				//start parsing new line

				//UP TO HERE
			}
			else if(currentToken == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ALTERNATE)
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
bool GIApreprocessorMultiwordReductionClass::loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPhrasalVerbList, GIApreprocessorMultiwordReductionSentence* firstTagInIrregularVerbList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionSentence* currentTagInPhrasalVerbList = firstTagInPhrasalVerbList;
	GIApreprocessorMultiwordReductionSentence* recordOfNonAlternateTagInPhrasalVerbList = currentTagInPhrasalVerbList;

	GIApreprocessorMultiwordReductionWord* currentTagInPhrasalVerb = firstTagInPhrasalVerbList->firstTagInSentence;
	GIApreprocessorMultiwordReductionWord* recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;

	ifstream parseFileObject(phrasalVerbDatabaseFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
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
				if(currentWord == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_OR)
				{
					currentTagInPhrasalVerbList->alternateSentence = new GIApreprocessorMultiwordReductionSentence();
					currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->alternateSentence;
					currentTagInPhrasalVerb = currentTagInPhrasalVerbList->firstTagInSentence;
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
					int i;
					if(SHAREDvars.textInTextArray(currentWord, lrpPhrasalVerbDatabaseTagArbitraryNameArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES, &i))
					{
						foundTagArbitraryName = true;
						currentTagInPhrasalVerb->tagSpecialArbitraryName = true;
						currentTagInPhrasalVerb->tagSpecialArbitraryNameType = i;
					}
					if(currentTagInPhrasalVerb->base)
					{
						this->generateTenseVariantsOfVerbBase(currentTagInPhrasalVerb, firstTagInIrregularVerbList);
					}


					currentTagInPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionWord();
					currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
					if(currentWordAlternate)
					{
						//revert to non-alternate tag...
						currentTagInPhrasalVerb = recordOfNonAlternateTagInPhrasalVerb;
						currentTagInPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionWord();
						currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
					}

					if(currentToken == CHAR_NEWLINE)
					{
						currentTagInPhrasalVerbList->nextSentence = new GIApreprocessorMultiwordReductionSentence();
						currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->nextSentence;
						if(currentPhrasalVerbAlternate)
						{
							//revert to non-alternate phrasal verb...
							currentTagInPhrasalVerbList = recordOfNonAlternateTagInPhrasalVerbList;
							currentTagInPhrasalVerbList->nextSentence = new GIApreprocessorMultiwordReductionSentence();
							currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->nextSentence;
							currentPhrasalVerbAlternate = false;
						}
						currentTagInPhrasalVerb = currentTagInPhrasalVerbList->firstTagInSentence;
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
				if((currentToken == CHAR_NEWLINE) || (currentWord == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_OR))
				{
					parsingVerbBase = true;
					phrasalVerbBaseNameRecord = "";
				}
				//start parsing new

			}
			else if(currentToken == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ALTERNATE)
			{

				//COPY2

				currentTagInPhrasalVerb->tagName = currentWord;		//NB current implementation can take into account 2 or more alternate tags (eg x/y/z)...
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
				int i;
				if(SHAREDvars.textInTextArray(currentWord, lrpPhrasalVerbDatabaseTagArbitraryNameArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES, &i))
				{
					foundTagArbitraryName = true;
					currentTagInPhrasalVerb->tagSpecialArbitraryName = true;
					currentTagInPhrasalVerb->tagSpecialArbitraryNameType = i;
				}
				if(currentTagInPhrasalVerb->base)
				{
					this->generateTenseVariantsOfVerbBase(currentTagInPhrasalVerb, firstTagInIrregularVerbList);
				}
				if(!currentWordAlternate)
				{//added 1p1aTEMP6d
					recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;
				}

				currentTagInPhrasalVerb->alternateTag = new GIApreprocessorMultiwordReductionWord();
				currentTagInPhrasalVerb = currentTagInPhrasalVerb->alternateTag;

				//wordIndex does not change...
				//parsingVerbBase = false;		//should not be parsing verb base here, as it should never have alternate cases eg base/base
				currentWordAlternate = true;
				//currentWordOptional = false;	//removed 1p1aTEMP5b
				currentWord = "";
			}
			else if(currentToken == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_OPEN_OPTIONAL)
			{
				currentWordOptional = true;
			}
			else if(currentToken == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_CLOSE_OPTIONAL)
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




	return result;
}



bool GIApreprocessorMultiwordReductionClass::generateTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, GIApreprocessorMultiwordReductionSentence* firstTagInIrregularVerbList)
{
	bool result = true;

	string base = SHAREDvars.convertStringToLowerCase(&(baseTag->tagName));

	//1. check if irregular past / past participle case
	bool irregularVerbFound = false;
	GIApreprocessorMultiwordReductionSentence* currentTagInIrregularVerbList = firstTagInIrregularVerbList;
	while(currentTagInIrregularVerbList->nextSentence != NULL)
	{
		if(currentTagInIrregularVerbList->firstTagInSentence->tagName == base)
		{
			irregularVerbFound = true;
			GIApreprocessorMultiwordReductionWord* firstTagInIrregularVerb = currentTagInIrregularVerbList->firstTagInSentence;
			GIApreprocessorMultiwordReductionWord* currentTagInIrregularVerb = firstTagInIrregularVerb->nextTag;
			int irregularVerbTagIndex = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST;
			while(currentTagInIrregularVerb->nextTag != NULL)
			{
				//first index is past, second is past participle
				if(irregularVerbTagIndex == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST)
				{
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = currentTagInIrregularVerb->tagName;
					if(currentTagInIrregularVerb->alternateTag != NULL)
					{
						baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->alternateTag->tagName;
					}
					else
					{
						baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->tagName;	//just use the same version (standard)
					}
				}
				else if(irregularVerbTagIndex == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE)
				{
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = currentTagInIrregularVerb->tagName;
					if(currentTagInIrregularVerb->alternateTag != NULL)
					{
						baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->alternateTag->tagName;
					}
					else
					{
						baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = currentTagInIrregularVerb->tagName;	//just use the same version (standard)
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
	bool lastCharacterIsVowel = false;
	bool secondLastCharacterIsVowel = false;
	bool thirdLastCharacterIsVowel = false;
	//bool verbDoubleConsonantRule1LastLetterException = false;
	if(SHAREDvars.charInCharArray(lastCharacterInBase, englishVowelArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NUMBER_OF_VOWELS))
	{
		lastCharacterIsVowel = true;
	}
	if(SHAREDvars.charInCharArray(secondLastCharacterInBase, englishVowelArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NUMBER_OF_VOWELS))
	{
		secondLastCharacterIsVowel = true;
	}
	if(SHAREDvars.charInCharArray(thirdLastCharacterInBase, englishVowelArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NUMBER_OF_VOWELS))
	{
		thirdLastCharacterIsVowel = true;
	}
	/*
	for(int i = 0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DOUBLE_CONSONANT_RULE1_LAST_LETTER_EXCEPTIONS_NUMBER_OF_TYPES; i++)
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

	//generate (ie record) infinitive tense form [for consistency; even though this is the same as tagName...]
	baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base;
	baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base;

	//Rule 1: Words ending with a Consonant-Vowel-Consonant Pattern
	if(rule1)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;
		if(rule1b)
		{
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;
		}

		//b. generate continuous tense form
		//happening/entering
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;
		if(rule1b)
		{
			//hopping/sitting
			//beginning/permitting
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;	//double consonant
		}

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//visited/opened
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
			if(rule1b)
			{
				//rubbed/stopped
				//referred/admitted
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;	//double consonant
			}

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
			if(rule1b)
			{
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = base + lastCharacterInBase + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;	//double consonant
			}
		}

		if(!rule1b)
		{
			this->copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
		}
	}
	//Rule 2: Words ending in E
	else if(rule2)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;

		//b. generate continuous tense form
		//dancing/skating
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//smiled/fined
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
		}

		this->copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}
	//Rule 3: Words ending in Y
	else if(rule3a)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_CASE3;

		//b. generate continuous tense form
		//carrying/replying
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//studied/married
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3;
		}

		this->copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}
	//Rule 4: Other words...
	else if(rule3b || rule4)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND;

		//b. generate continuous tense form
		//enjoying/straying
		//needing/beeping
		//needing/laughing
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//played/stayed
			//dreamed/rained
			//parked/earned
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND;
		}

		this->copyDefaultVerbTenseFormsToAlternateTenseForms(baseTag, irregularVerbFound);
	}



	return result;
}

void GIApreprocessorMultiwordReductionClass::copyDefaultVerbTenseFormsToAlternateTenseForms(GIApreprocessorMultiwordReductionWord* baseTag, const bool irregularVerbFound)
{
	baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	if(!irregularVerbFound)
	{
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
	}

}

bool GIApreprocessorMultiwordReductionClass::loadPlainTextFile(const string plainTextInputFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPlainText)
{
	bool result = true;

	GIApreprocessorMultiwordReductionSentence* currentTagInPlainText = firstTagInPlainText;
	GIApreprocessorMultiwordReductionWord* firstTagInPlainTextSentence = currentTagInPlainText->firstTagInSentence;
	GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;

	string fileContents = SHAREDvars.getFileContents(plainTextInputFileName);

	int charCount = 0;
	char currentToken;
	bool whiteSpace = false;
	string currentWord = "";
	int entityIndex = GIA_NLP_START_ENTITY_INDEX;	//only assigned after collapse?
	int sentenceIndex = GIA_NLP_START_SENTENCE_INDEX;	//only assigned after collapse?
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
	bool readingQuotation = false;
	#endif
	while(charCount < fileContents.length())
	{
		currentToken = fileContents[charCount];
		bool punctuationMarkFound = false;
		if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
		{
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			if(!this->isIntrawordPunctuationMark(charCount, &fileContents))
			{
			#endif
				punctuationMarkFound = true;
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			}
			#endif
		}
		bool whiteSpaceFound = false;
		if(SHAREDvars.charInCharArray(currentToken, nlpWhitespaceCharacterArray, GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS))
		{
			whiteSpaceFound = true;
		}
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
		bool quotationMarkFound = false;
		if(SHAREDvars.charInCharArray(currentToken, nlpQuotationMarkCharacterArray, GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS))
		{
			quotationMarkFound = true;
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
				currentTagInPlainTextSentence->nextTag = new GIApreprocessorMultiwordReductionWord();
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
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
				if(readingQuotation)
				{
					currentWord = currentWord + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_QUOTES_DELIMITER;
				}
				else
				{
				#endif
					if(!whiteSpace)
					{//do not currently support punctuation marks with preceeding white space " ," " ." 
					
						#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
						if(currentWord != "")
						{//do not add empty tag after closing quotation marks
						#endif
							currentTagInPlainTextSentence->tagName = currentWord;
							currentTagInPlainTextSentence->entityIndex = entityIndex;
							currentTagInPlainTextSentence->nextTag = new GIApreprocessorMultiwordReductionWord();
							currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
							entityIndex++;
						#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
						}
						#endif
						if(punctuationMarkFound)
						{
							currentTagInPlainTextSentence->tagName = currentToken;
							currentTagInPlainTextSentence->entityIndex = entityIndex;
							currentTagInPlainTextSentence->nextTag = new GIApreprocessorMultiwordReductionWord();
							currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;

							bool endOfSentencePunctuationMarkFound = false;
							if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
							{
								#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
								if(!this->isIntrawordPunctuationMark(charCount, &fileContents))
								{
								#endif
									endOfSentencePunctuationMarkFound = true;
								#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
								}
								#endif
							}
							if(endOfSentencePunctuationMarkFound)
							{
								currentTagInPlainText->sentenceIndex = sentenceIndex;	//added 3a1a
								currentTagInPlainText->nextSentence = new GIApreprocessorMultiwordReductionSentence();
								currentTagInPlainText = currentTagInPlainText->nextSentence;
								currentTagInPlainTextSentence = currentTagInPlainText->firstTagInSentence;
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
					{//skip (do not parse) multiple white space/punctuation characters (eg ". "/".."/"  "/" .")
					}
					whiteSpace = true;
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
				}
				#endif
			}
			else
			{
				whiteSpace = false;
				currentWord = currentWord + currentToken;
			}
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
		}
		#endif
		charCount++;
	}


	return result;
}

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
bool GIApreprocessorMultiwordReductionClass::isIntrawordPunctuationMark(const int indexOfCurrentToken, const string* lineContents)
{
	bool intrawordPunctuationMark = false;
	char currentToken = (*lineContents)[indexOfCurrentToken];
	if((currentToken == CHAR_FULLSTOP) || (currentToken == CHAR_COLON))	//updated 2j6e (added CHAR_COLON for times, eg 06:45)
	{
		if(indexOfCurrentToken < lineContents->length()-1)	//ensure fullstop is not immediately succeded by an alphabetical character, which indicates that the fullstop is part of a filename, eg "people.xml"
		{
			char characterImmediatelySucceedingPunctuationMark = (*lineContents)[indexOfCurrentToken+1];
			bool isPunctuationMarkImmediatelySucceededByAlphanumericCharacter = SHAREDvars.charInCharArray(characterImmediatelySucceedingPunctuationMark, GIApreprocessorMultiwordReductionNLPparsableCharacters, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES);
			if(isPunctuationMarkImmediatelySucceededByAlphanumericCharacter)
			{
				intrawordPunctuationMark = true;
			}
		}
	}
	return intrawordPunctuationMark;
}
#endif


//NB the collapsed phrasal verb contains precisely 2 entities: phrasalVerbCollapsed, entity2: thing/place/body (eg belong_to + sb/Tom) - thing/place/bodies are not currently being differentiated by the LRP as this information is only first generated at NLP/GIA parse stage
bool GIApreprocessorMultiwordReductionClass::searchAndReplacePhrasalVerbs(GIApreprocessorMultiwordReductionSentence* firstTagInPlainText, GIApreprocessorMultiwordReductionSentence* firstTagInPhrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo)
{
	bool result = true;

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;	//new correspondence info for each found phrasal verb

	GIApreprocessorMultiwordReductionSentence* currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{

		GIApreprocessorMultiwordReductionWord* firstTagInPlainTextSentence = currentTagInPlainText->firstTagInSentence;
		GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIApreprocessorMultiwordReductionWord* previousTagInPlainTextSentence = NULL;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{

			bool foundAtLeastOnePhrasalVerbInSentenceAndCollapsed = false;
			GIApreprocessorMultiwordReductionSentence* currentTagInPhrasalVerbList = firstTagInPhrasalVerbList;
			while(currentTagInPhrasalVerbList->nextSentence != NULL)
			{

				GIApreprocessorMultiwordReductionSentence* currentTagInPhrasalVerbListNormOrAlternate = currentTagInPhrasalVerbList;
				bool alternatePhrasalVerb = false;
				while(!alternatePhrasalVerb || (currentTagInPhrasalVerbListNormOrAlternate->alternateSentence != NULL))
				{
					/*
					if(tolower((currentTagInPhrasalVerbList->tagName)[0]) == tolower((currentTagInPlainTextSentence->tagName)[0]))
					{//optimisation; only deal with phrasal verbs that start with the same character...
					*/
					GIApreprocessorMultiwordReductionWord* firstTagInPhrasalVerb = currentTagInPhrasalVerbListNormOrAlternate->firstTagInSentence;
					GIApreprocessorMultiwordReductionWord* currentTagInPhrasalVerb = firstTagInPhrasalVerb;
					bool stillFoundVerbMatchOfArbitraryTense = true;
					bool foundAtLeastOneMatch = false;
					GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
					GIApreprocessorMultiwordReductionWord* lastTagInPlainTextSentenceTemp = NULL;	//pointer used to change original plain text to collapsed phrasal verb text
					GIApreprocessorMultiwordReductionWord* firstTagInCollapsedPhrasalVerb = new GIApreprocessorMultiwordReductionWord();
					GIApreprocessorMultiwordReductionWord* currentTagInCollapsedPhrasalVerb = firstTagInCollapsedPhrasalVerb;
					bool currentlyParsingTagSpecial = false;
					int tagInPhrasalVerbSpecialAndNotFoundCount = 0;
					int numberTagSpecialTagsFound = 0;
					int numberTagConsecutiveOptionalTagsFound = 0;
					bool phrasalVerbHasTagSpecial = false;
					while((currentTagInPhrasalVerb->nextTag != NULL) && (currentTagInPlainTextSentenceTemp->nextTag != NULL) && (stillFoundVerbMatchOfArbitraryTense))
					{
						bool currentTagInPhrasalVerbOptionalAndNotFound = false;
						bool currentTagInPhrasalVerbSpecialAndNotFound = false;

						if(currentTagInPhrasalVerb->tagSpecialArbitraryName)
						{
							phrasalVerbHasTagSpecial = true;

							//NB the collapsed phrasal verb contains precisely 2 entities: phrasalVerbCollapsed, entity2: thing/place/body (eg belong_to + sb/Tom) - thing/place/bodies are not currently being differentiated by the LRP as this information is only first generated at NLP/GIA parse stage
							currentTagInCollapsedPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionWord();
							currentTagInCollapsedPhrasalVerb->nextTag->tagName = currentTagInPlainTextSentenceTemp->tagName + " ";		//arbitrary thing/place/body name
							currentTagInCollapsedPhrasalVerb->nextTag->collapsedPhrasalVerbExactDefinedSection = false;
							//currentTagInCollapsedPhrasalVerb->nextTag->collapsedPhrasalVerbExactDefinedSectionTemp = false;
							/*
							//removed 7 Sept 2012b - now interpret: ask sb over/round -> ask_over sb
							currentTagInCollapsedPhrasalVerb = currentTagInCollapsedPhrasalVerb->nextTag;
							*/
							currentlyParsingTagSpecial = true;
							numberTagSpecialTagsFound++;
						}
						else
						{
							bool foundVerbMatchOfArbitraryTenseTemp = false;
							GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentenceTempNormOrAlternate = currentTagInPlainTextSentenceTemp;
							bool alternateTag = false;
							string generatedTagNameLemma = "";
							while(!alternateTag || (currentTagInPlainTextSentenceTempNormOrAlternate->alternateTag != NULL))
							{
								if(currentTagInPhrasalVerb->base)
								{
									for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
									{
										for(int j=0; j<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS; j++)
										{
											if(currentTagInPhrasalVerb->grammaticalTenseFormsArray[i][j] == currentTagInPlainTextSentenceTempNormOrAlternate->tagName)		//USED TO BE currentTagInCollapsedPhrasalVerb before 6 Sept 2012
											{
												currentTagInCollapsedPhrasalVerb->grammaticalTenseFormDetected = i;								//USED TO BE AND STILL IS currentTagInCollapsedPhrasalVerb before 6 Sept 2012
												generatedTagNameLemma = currentTagInPhrasalVerb->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
												foundVerbMatchOfArbitraryTenseTemp = true;
											}
										}
									}
								}
								else
								{
									if(currentTagInPhrasalVerb->tagName == currentTagInPlainTextSentenceTempNormOrAlternate->tagName)
									{
										foundVerbMatchOfArbitraryTenseTemp = true;
									}
								}
								if(currentTagInPlainTextSentenceTempNormOrAlternate->alternateTag != NULL)
								{
									currentTagInPlainTextSentenceTempNormOrAlternate = currentTagInPlainTextSentenceTempNormOrAlternate->alternateTag;
								}
								alternateTag = true;
							}


							if(!foundVerbMatchOfArbitraryTenseTemp)
							{
								if(currentTagInPhrasalVerb->optional)
								{
									currentTagInPhrasalVerbOptionalAndNotFound = true;
								}
								else if(currentlyParsingTagSpecial && (tagInPhrasalVerbSpecialAndNotFoundCount <= GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_MAX_NUMBER_WORDS))
								{
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
									stillFoundVerbMatchOfArbitraryTense = false;
								}
							}
							else
							{
								if(currentlyParsingTagSpecial)
								{
									/*
									//removed 7 Sept 2012b - now interpret: ask sb over/round -> ask_over sb
									currentTagInCollapsedPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionWord();
									currentTagInCollapsedPhrasalVerb = currentTagInCollapsedPhrasalVerb->nextTag;
									*/
									currentlyParsingTagSpecial = false;
								}

								foundAtLeastOneMatch = true;
								currentTagInCollapsedPhrasalVerb->collapsedPhrasalVerbExactDefinedSection = true;
								//currentTagInCollapsedPhrasalVerb->collapsedPhrasalVerbExactDefinedSectionTemp = true;
								currentTagInCollapsedPhrasalVerb->tagName = currentTagInCollapsedPhrasalVerb->tagName + currentTagInPlainTextSentenceTemp->tagName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER;		//this is part of the defined/main section of the phrasal verb
								if(currentTagInPhrasalVerb->base)
								{
									currentTagInCollapsedPhrasalVerb->tagNameLemma = currentTagInCollapsedPhrasalVerb->tagNameLemma + generatedTagNameLemma + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER;
								}
								else
								{
									currentTagInCollapsedPhrasalVerb->tagNameLemma = currentTagInCollapsedPhrasalVerb->tagNameLemma + currentTagInPlainTextSentenceTemp->tagName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER;
								}
							}
						}

						if(currentTagInPhrasalVerb->tagSpecialArbitraryName)
						{
							if(currentTagInPhrasalVerb->optional)
							{
								//special+optional tag found - just increment the phrasal verb for now [for this round]...
								currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
							}
							else
							{
								currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
								currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
							}
						}
						else if(currentTagInPhrasalVerbOptionalAndNotFound)
						{
							//optional phrasal verb tag not found - just increment the phrasal verb tag list...
							currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
						}
						else if(currentTagInPhrasalVerbSpecialAndNotFound)
						{
							//next phrasal verb tag not found yet - just increment the sentence tag list...
							currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
						}
						else
						{
							currentTagInPhrasalVerb = currentTagInPhrasalVerb->nextTag;
							currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
						}
					}

					if(stillFoundVerbMatchOfArbitraryTense && foundAtLeastOneMatch)
					{
						if(currentTagInPhrasalVerb->nextTag == NULL)
						{//make sure the entire multiword phrasal verb is matched (in case currentTagInPlainTextSentenceTemp reaches end of sentence without matching the multiword phrasal verb in its entirety)
							if(numberTagSpecialTagsFound <= 1)
							{//do not preprocess phrasal verbs with more than one special tag (ie sth/sb/swh) - as this generally involves more than a verb [verb sth preposition sth1] - added 1p1aTEMP5
								//reduce all entities
								if(phrasalVerbHasTagSpecial)
								{
									currentTagInCollapsedPhrasalVerb->nextTag->nextTag = currentTagInPlainTextSentenceTemp;		//NB currentTagInCollapsedPhrasalVerb->nextTag is the collapsed phrasal verb thing/place/body entity
								}
								else
								{
									currentTagInCollapsedPhrasalVerb->nextTag = currentTagInPlainTextSentenceTemp;
								}
								if(previousTagInPlainTextSentence != NULL)
								{
									previousTagInPlainTextSentence->nextTag = firstTagInCollapsedPhrasalVerb;
								}
								else
								{
									currentTagInPlainText->firstTagInSentence = firstTagInCollapsedPhrasalVerb;
									firstTagInPlainTextSentence = firstTagInCollapsedPhrasalVerb;								
								}
								
								foundAtLeastOnePhrasalVerbInSentenceAndCollapsed = true;
							}
						}
					}
					/*
					}
					*/
					if(currentTagInPhrasalVerbListNormOrAlternate->alternateSentence != NULL)
					{
						currentTagInPhrasalVerbListNormOrAlternate = currentTagInPhrasalVerbListNormOrAlternate->alternateSentence;
					}
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
			if(foundAtLeastOnePhrasalVerbInSentenceAndCollapsed)
			{
				//renumberEntityIndiciesInCorrespondenceInfo(firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, currentTagInPlainText->sentenceIndex, numberWordsInMultiwordMatched); - this is not required because searchAndReplaceMultiwordWordList is executed after searchAndReplacePhrasalVerbs
				GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
				int newEntityIndex = GIA_NLP_START_ENTITY_INDEX;
				while(currentTagInPlainTextSentenceTemp2->nextTag != NULL)
				{
					//if(currentTagInPlainTextSentenceTemp2->collapsedPhrasalVerbExactDefinedSectionTemp)
					if(currentTagInPlainTextSentenceTemp2->collapsedPhrasalVerbExactDefinedSection)
					{//create a new correspondenceInfo
						//currentTagInPlainTextSentenceTemp2->collapsedPhrasalVerbExactDefinedSectionTemp = false;
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
						currentCorrespondenceInfo->wordWithLRPforNLPonly = generateWordWithLRPforNLPonly(currentTagInPlainTextSentenceTemp2);
						currentCorrespondenceInfo->next = new GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
						currentCorrespondenceInfo = currentCorrespondenceInfo->next;
					}
					currentTagInPlainTextSentenceTemp2->entityIndex = newEntityIndex;
					currentTagInPlainTextSentenceTemp2 = currentTagInPlainTextSentenceTemp2->nextTag;
					newEntityIndex++;
				}
			}
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}


		currentTagInPlainText = currentTagInPlainText->nextSentence;
	}

	return result;
}


bool GIApreprocessorMultiwordReductionClass::loadMultiwordWordListAndSearchAndReplace(const string multiwordWordListFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPlainText, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
{
	bool result = true;
	
	string multiwordWordListFileNameFullPath = lrpDataFolderName + multiwordWordListFileName;
	GIApreprocessorMultiwordReductionSentence* firstTagInMultiwordWordList = new GIApreprocessorMultiwordReductionSentence();
	if(!this->loadMultiwordWordList(multiwordWordListFileNameFullPath, firstTagInMultiwordWordList))
	{
		result = false;
	}
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;
	while(currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo->next != NULL)
	{
		currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo = currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo->next;	//added 2j6d (add to end of list)
	}
	if(!this->searchAndReplaceMultiwordWordList(firstTagInPlainText, firstTagInMultiwordWordList, currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, wordListType))
	{
		result = false;
	}
	delete firstTagInMultiwordWordList;
	
	return result;
}	
	
bool GIApreprocessorMultiwordReductionClass::loadMultiwordWordList(const string multiwordWordListFileName, GIApreprocessorMultiwordReductionSentence* firstTagInMultiwordWordList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionSentence* currentTagInMultiwordWordList = firstTagInMultiwordWordList;
	GIApreprocessorMultiwordReductionWord* firstTagInMultiwordWord = currentTagInMultiwordWordList->firstTagInSentence;
	GIApreprocessorMultiwordReductionWord* currentTagInMultiwordWord = firstTagInMultiwordWord;

	ifstream parseFileObject(multiwordWordListFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//txt file does not exist in current directory.
		cout << "Error: Multiword word Database File does not exist in current directory: " << multiwordWordListFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		string currentMultiwordWord = "";	//not used [multiword words are matched word by word; like phrasal verbs]
		string currentWord = "";
		while(parseFileObject.get(currentToken))
		{
			if((currentToken == CHAR_NEWLINE) || (currentToken == CHAR_SPACE))
			{
				currentTagInMultiwordWord->tagName = currentWord;
				currentTagInMultiwordWord->nextTag = new GIApreprocessorMultiwordReductionWord();
				currentTagInMultiwordWord = currentTagInMultiwordWord->nextTag;
				if(currentToken == CHAR_NEWLINE)
				{
					currentTagInMultiwordWordList->nextSentence = new GIApreprocessorMultiwordReductionSentence();
					currentTagInMultiwordWordList = currentTagInMultiwordWordList->nextSentence;
					currentTagInMultiwordWord = currentTagInMultiwordWordList->firstTagInSentence;
					currentMultiwordWord = "";
				}
				else if(currentToken == CHAR_SPACE)
				{
					currentMultiwordWord = currentMultiwordWord + currentToken;
				}
				currentWord = "";
			}
			else
			{
				currentWord = currentWord + currentToken;
				currentMultiwordWord = currentMultiwordWord + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}
	return result;
}

bool GIApreprocessorMultiwordReductionClass::searchAndReplaceMultiwordWordList(GIApreprocessorMultiwordReductionSentence* firstTagInPlainText, const GIApreprocessorMultiwordReductionSentence* firstTagInMultiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
{
	bool result = true;

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo;	//new correspondence info for each found multiword word

	GIApreprocessorMultiwordReductionSentence* currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{		
		GIApreprocessorMultiwordReductionWord* firstTagInPlainTextSentence = currentTagInPlainText->firstTagInSentence;
		GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIApreprocessorMultiwordReductionWord* previousTagInPlainTextSentence = NULL;
		int entityIndex = GIA_NLP_START_ENTITY_INDEX;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{			
			const GIApreprocessorMultiwordReductionSentence* currentTagInMultiwordWordList = firstTagInMultiwordWordList;
			bool foundAtLeastOneMultiwordWordInSentenceAndCollapsed = false;
			int numberWordsInMultiwordMatched = 0;
			while(currentTagInMultiwordWordList->nextSentence != NULL)
			{				
				int numberWordsInMultiword = 0;
				bool foundAtLeastOneMultiwordWordInSentenceAndCollapsedTemp = false;
				bool stillFoundWordMatch = true;
				bool foundAtLeastOneMatch = false;
				GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
				GIApreprocessorMultiwordReductionWord* firstTagInCollapsedMultiwordWord = new GIApreprocessorMultiwordReductionWord();
				const GIApreprocessorMultiwordReductionWord* firstTagInMultiwordWord = currentTagInMultiwordWordList->firstTagInSentence;
				const GIApreprocessorMultiwordReductionWord* currentTagInMultiwordWord = firstTagInMultiwordWord;
				while((currentTagInMultiwordWord->nextTag != NULL) && (currentTagInPlainTextSentenceTemp->nextTag != NULL) && (stillFoundWordMatch))
				{
					if(currentTagInMultiwordWord->tagName != currentTagInPlainTextSentenceTemp->tagName)
					{
						stillFoundWordMatch = false;
					}
					else
					{
						firstTagInCollapsedMultiwordWord->collapsedMultiwordWord = true;
						firstTagInCollapsedMultiwordWord->collapsedMultiwordWordTemp = true;	//this is reset everytime searchAndReplaceMultiwordWordList is executed to prevent collapsedMultiwordWord from being redetected
						firstTagInCollapsedMultiwordWord->collapsedMultiwordWordType = wordListType;
						firstTagInCollapsedMultiwordWord->tagName = firstTagInCollapsedMultiwordWord->tagName + currentTagInPlainTextSentenceTemp->tagName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER;
						foundAtLeastOneMatch = true;
						numberWordsInMultiword++;
					}
					currentTagInMultiwordWord = currentTagInMultiwordWord->nextTag;
					currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentenceTemp->nextTag;
				}

				if(stillFoundWordMatch && foundAtLeastOneMatch)
				{
					if(currentTagInMultiwordWord->nextTag == NULL)
					{//make sure the entire multiword word is matched (in case currentTagInPlainTextSentenceTemp reaches end of sentence without matching the multiword word in its entirety)
						//reduce all entities
						firstTagInCollapsedMultiwordWord->nextTag = currentTagInPlainTextSentenceTemp;
						if(previousTagInPlainTextSentence != NULL)
						{
							previousTagInPlainTextSentence->nextTag = firstTagInCollapsedMultiwordWord;
						}
						else
						{
							currentTagInPlainText->firstTagInSentence = firstTagInCollapsedMultiwordWord;
							firstTagInPlainTextSentence = firstTagInCollapsedMultiwordWord;
						}
						foundAtLeastOneMultiwordWordInSentenceAndCollapsed = true;
						foundAtLeastOneMultiwordWordInSentenceAndCollapsedTemp = true;
						numberWordsInMultiwordMatched = numberWordsInMultiword;
					}
				}
				if(!foundAtLeastOneMultiwordWordInSentenceAndCollapsedTemp)
				{
					delete firstTagInCollapsedMultiwordWord;
				}

				currentTagInMultiwordWordList = currentTagInMultiwordWordList->nextSentence;
			}
			if(foundAtLeastOneMultiwordWordInSentenceAndCollapsed)
			{
				renumberEntityIndiciesInCorrespondenceInfo(firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, currentTagInPlainText->sentenceIndex, entityIndex, numberWordsInMultiwordMatched);	//this is required for revertNLPtagNameToOfficialLRPtagName	//this is required because searchAndReplaceMultiwordWordList (zero or more times)/searchAndReplacePhrasalVerbs is executed before searchAndReplaceMultiwordWordList 
				GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
				int newEntityIndex = GIA_NLP_START_ENTITY_INDEX;
				//int collapsedMultiwordWordIndex = 0;
				while(currentTagInPlainTextSentenceTemp2->nextTag != NULL)
				{
					if(currentTagInPlainTextSentenceTemp2->collapsedMultiwordWordTemp)
					{//create a new correspondenceInfo
						if(newEntityIndex != entityIndex)
						{
							cout << "GIApreprocessorMultiwordReductionClass::searchAndReplaceMultiwordWordList error: (newEntityIndex != entityIndex)" << endl;
							exit(EXIT_ERROR);
						}
						currentTagInPlainTextSentenceTemp2->collapsedMultiwordWordTemp = false;
						string tagName = currentTagInPlainTextSentenceTemp2->tagName;
						string tagNameWithLastLetterDropped = tagName.substr(0, tagName.length()-1);
						currentTagInPlainTextSentenceTemp2->tagName = tagNameWithLastLetterDropped;	//remove last GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER
						currentCorrespondenceInfo->sentenceIndex = currentTagInPlainText->sentenceIndex;
						currentCorrespondenceInfo->entityIndex = newEntityIndex;	//this is not currently used for LRP collapsed multiword word
						//#ifdef GIA_SEMANTIC_PARSER
						currentCorrespondenceInfo->lemmaWithLRP = currentTagInPlainTextSentenceTemp2->tagName;	//added 2j6c	//required for GIA2 only?
						//#endif
						currentCorrespondenceInfo->wordWithLRP = currentTagInPlainTextSentenceTemp2->tagName;
						currentCorrespondenceInfo->wordWithLRPforNLPonly = generateWordWithLRPforNLPonly(currentTagInPlainTextSentenceTemp2); //lrpDummyCollapsedMultiwordWordLemmaNameForNLPArray[collapsedMultiwordWordIndex];
						//collapsedMultiwordWordIndex++;
						
						currentCorrespondenceInfo->next = new GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
						currentCorrespondenceInfo = currentCorrespondenceInfo->next;
					}
					currentTagInPlainTextSentenceTemp2->entityIndex = newEntityIndex;
					currentTagInPlainTextSentenceTemp2 = currentTagInPlainTextSentenceTemp2->nextTag;
					newEntityIndex++;
				}
			}
			else
			{
				
			}
			entityIndex++;
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}
		currentTagInPlainText = currentTagInPlainText->nextSentence;
	}

	return result;
}

void GIApreprocessorMultiwordReductionClass::renumberEntityIndiciesInCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, int sentenceIndex, int entityIndex, int numberWordsInMultiwordMatched)
{
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;
	while(currentCorrespondenceInfo->next != NULL)
	{
		if(currentCorrespondenceInfo->sentenceIndex == sentenceIndex)
		{
			if(currentCorrespondenceInfo->entityIndex > entityIndex)
			{
				currentCorrespondenceInfo->entityIndex = currentCorrespondenceInfo->entityIndex - (numberWordsInMultiwordMatched-1);
			}
		}
		currentCorrespondenceInfo = currentCorrespondenceInfo->next;
	}
}

bool GIApreprocessorMultiwordReductionClass::writeTagListToFile(const GIApreprocessorMultiwordReductionSentence* firstTagInPlainText, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPforNLPoutput)
{
	bool result = true;

	ofstream plainTextLRPOutput(plainTextLRPoutputFileName.c_str());
	ofstream* plainTextLRPforNLPOutput;
	if(performLRPforNLPoutput)
	{
		plainTextLRPforNLPOutput = new ofstream(plainTextLRPforNLPoutputFileName.c_str());
	}
	
	
	bool firstCharacterInFile = true;

	const GIApreprocessorMultiwordReductionSentence* currentTagInPlainText = firstTagInPlainText;
	while(currentTagInPlainText->nextSentence != NULL)
	{
		bool firstCharacterInSentence = true;
		const GIApreprocessorMultiwordReductionWord* firstTagInPlainTextSentence = currentTagInPlainText->firstTagInSentence;
		const GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			string plainTextLRPOutputTag = currentTagInPlainTextSentence->tagName;
			string plainTextLRPforNLPOutputTag = "";
			if(performLRPforNLPoutput)
			{
				plainTextLRPforNLPOutputTag = generateWordWithLRPforNLPonly(currentTagInPlainTextSentence);
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
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE
			if(!punctuationMarkFound && !firstCharacterInFile && !firstCharacterInSentence)
			#else
			if(!punctuationMarkFound && !firstCharacterInFile)			
			#endif
			{
				plainTextLRPOutputTag = string(STRING_SPACE) + plainTextLRPOutputTag;
				if(performLRPforNLPoutput)
				{
					plainTextLRPforNLPOutputTag = string(STRING_SPACE) + plainTextLRPforNLPOutputTag;
				}
			}

			plainTextLRPOutput.write(plainTextLRPOutputTag.c_str(), plainTextLRPOutputTag.length());
			if(performLRPforNLPoutput)
			{
				plainTextLRPforNLPOutput->write(plainTextLRPforNLPOutputTag.c_str(), plainTextLRPforNLPOutputTag.length());
			}
			
			firstCharacterInFile = false;
			firstCharacterInSentence = false;

			currentTagInPlainTextSentence = currentTagInPlainTextSentence->nextTag;
		}
		
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE
		//added 3a1a;
		plainTextLRPOutput.put(CHAR_NEWLINE);
		if(performLRPforNLPoutput)
		{
			plainTextLRPforNLPOutput->put(CHAR_NEWLINE);
		}
		#endif

		currentTagInPlainText = currentTagInPlainText->nextSentence;
	}

	plainTextLRPOutput.close();
	if(performLRPforNLPoutput)
	{
		plainTextLRPforNLPOutput->close();
	}
	
	return result;
}

string GIApreprocessorMultiwordReductionClass::generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductionWord* currentTagInPlainTextSentence)
{
	string wordWithLRPforNLPonly = currentTagInPlainTextSentence->tagName;
	if(currentTagInPlainTextSentence->collapsedPhrasalVerbExactDefinedSection)
	{
		wordWithLRPforNLPonly = lrpDummyCollapsedPhrasalVerbNameForNLPgrammaticalTenseFormsArray[currentTagInPlainTextSentence->grammaticalTenseFormDetected];
	}
	else if(currentTagInPlainTextSentence->collapsedMultiwordWord)
	{
		if(currentTagInPlainTextSentence->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_TYPE)
		{
			wordWithLRPforNLPonly = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP;
		}
		else if(currentTagInPlainTextSentence->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADVERB_TYPE)
		{
			wordWithLRPforNLPonly = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADVERB_NAME_FOR_NLP;
		}
		else if(currentTagInPlainTextSentence->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADJECTIVE_TYPE)
		{
			wordWithLRPforNLPonly = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADJECTIVE_NAME_FOR_NLP;
		}
		else if(currentTagInPlainTextSentence->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_NOUN_TYPE)
		{
			wordWithLRPforNLPonly = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_NOUN_NAME_FOR_NLP;
		}
		else if(currentTagInPlainTextSentence->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_TYPE)
		{
			wordWithLRPforNLPonly = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_NAME_FOR_NLP;
		}
	}
	return wordWithLRPforNLPonly;
}
			
			
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY

//NB preposition reversion routine will not work for RelEx as RelEx defines dependency relations based on lemmas not words...
void GIApreprocessorMultiwordReductionClass::revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInListForPrepositionsOnly, const bool isPreposition, bool* foundOfficialLRPreplacementString)
{
	int entityIndexForNonPrepositionsOnly = feature->entityIndex;
	
	//save original values for NLP only (required during a multiword preposition replacement with an adjacent multiword verb
	feature->wordWithLRPforNLPonly = feature->word;		
						
	string word = feature->word;
	//string lemma = feature->lemma;	//only used for prepositions (dependency relation) calculations, where lemma has already been calculated via revertNLPtagNameToOfficialLRPtagName()

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo = this->getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();

	int sentenceIndex = currentSentenceInList->sentenceIndex;

	bool foundCorrespondingLRPtag = false;
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;
	while(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next != NULL)
	{
		if(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex == sentenceIndex)
		{
			if(word == currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly)
			{

				if(isPreposition)
				{
					//now search entire sentence->feature list and find entity/word that has same name, and has the governor/dependent closest to it...
					string relationGovernor = currentRelationInListForPrepositionsOnly->relationGovernor;
					string relationDependent = currentRelationInListForPrepositionsOnly->relationDependent;
					const GIAfeature* firstFeatureInList = currentSentenceInList->firstFeatureInList;
					const GIAfeature* currentFeatureInList = firstFeatureInList;
					int indexOfPrepositionWithMinimumProximityOfGovernorDependentWords = GIA_ENTITY_INDEX_UNDEFINED;
					int minimumProximityOfGovernorDependentWords = MAXIMUM_NUMBER_WORDS_PER_SENTENCE;
					int indexOfLastInstanceOfPreposition = GIA_ENTITY_INDEX_UNDEFINED;
					int indexOfLastInstanceOfGovernor = GIA_ENTITY_INDEX_UNDEFINED;
					int indexOfLastInstanceOfDependent = GIA_ENTITY_INDEX_UNDEFINED;
					bool foundPrepositionGovernorAndDependentInFeatureList = false;

					while(currentFeatureInList->next != NULL)
					{
						if(currentFeatureInList->wordWithLRPforNLPonly == relationGovernor)
						{
							indexOfLastInstanceOfGovernor = currentFeatureInList->entityIndex;
						}
						if(currentFeatureInList->word == currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP)	//NB currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP has already been assigned to feature lemma via previous non-preposition execution of revertNLPtagNameToOfficialLRPtagName
						{
							indexOfLastInstanceOfPreposition = currentFeatureInList->entityIndex;
						}
						if(currentFeatureInList->wordWithLRPforNLPonly == relationDependent)
						{		
							indexOfLastInstanceOfDependent = currentFeatureInList->entityIndex;
							if((indexOfLastInstanceOfPreposition != GIA_ENTITY_INDEX_UNDEFINED) && (indexOfLastInstanceOfGovernor != GIA_ENTITY_INDEX_UNDEFINED))
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
							feature->featureRevertedToOfficialLRPTemp = true;
							foundCorrespondingLRPtag = true;
						}
					}
					else
					{
						cout << "error: !foundPrepositionGovernorAndDependentInFeatureList;" << endl;
						cout << word << "(" << relationGovernor << ", " << relationDependent << ")" << endl;
						exit(EXIT_ERROR);
					}
				}
				else
				{
					if(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex == entityIndexForNonPrepositionsOnly)
					{
						feature->word = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP;
						feature->lemma = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->lemmaWithLRP;
						feature->featureRevertedToOfficialLRPTemp = true;

						foundCorrespondingLRPtag = true;
					}
				}
			}
		}
		currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next;
	}
	if(!foundCorrespondingLRPtag)
	{
	}
	else
	{
		*foundOfficialLRPreplacementString = true;
	}
}
#endif



#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
//warning: this function is only currently developed for infinitive and continuous case
bool GIApreprocessorMultiwordReductionClass::determineVerbCaseWrapper(const string word, string* baseNameFound, int* grammaticalTenseModifier)
{
	bool result = true;
	bool foundVerbCase = false;
	string verbListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_FILE_NAME;
	if(!verbListLoaded)
	{
		cout << "!verbListLoaded (GIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL requires -lrpfolder to be set): verbListFileName = " << verbListFileName << endl;
		result = false;
	}
	else
	{
		foundVerbCase = this->determineVerbCaseAdditional(word, firstTagInVerbListGlobal, baseNameFound, grammaticalTenseModifier);
	}
	return foundVerbCase;
}

//warning: this function is only currently developed for infinitive and continuous case
bool GIApreprocessorMultiwordReductionClass::determineVerbCaseAdditional(const string word, GIApreprocessorMultiwordReductionWord* firstTagInVerbList, string* baseNameFound, int* grammaticalTenseModifier)
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

	GIApreprocessorMultiwordReductionWord* currentTagInVerbList = firstTagInVerbList;
	int numberOfCharactersInBaseTenseFormAppend = 0;
	while(currentTagInVerbList->nextTag != NULL)
	{
		string wordLowerCase = SHAREDvars.convertStringToLowerCase(&word);
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
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				//continuous rule 1b: running - "run" + "n" [run n] + "ing"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				//continuous rule 2: changing - "chang" [change e] + "ing"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				/*
				//continuous rule 3a: N/A !marriing (use marrying)
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
				*/

				#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
				//added 2h2a
				//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND _able:
				//potential rule 1a/3b/4: thinkable/changeable
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				//potential rule 1b: running - "run" + "n" [run n] + "able"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				/*
				//potential rule 2: N/A !changable (use changeable)
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				*/
				//potential rule 3a: running - "marr" + "i" + "able"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
				#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
				//added 2h2c
				//GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP _ive:
				//potential rule 1ai: -> ive eg resistive/adaptive
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 1aii: -> itive eg additive
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 1aiii: -> ative eg affirmative
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				/*
				//potential rule 1b: running - "run" + "n" [run n] + "itive"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				*/
				//potential rule 2i: e -> itive eg competitive/definitive/accomodative
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 2ii: e -> ment + ive eg judgementive
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				//potential rule 3a: y -> iment + ive eg supplimentive
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
				#endif
				#endif

				#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
				//added 2h2a
				//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND _ed:
				//possible state rule 1a/3b/4: visited/opened
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				//possible state rule 1b: rubbed/stopped/referred/admitted - "rub" + "b" + "ed"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				//possible state rule 2: smiled/fined - "smil" [change e] + "ed"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				//possible state rule 3a: studied/married - "marr" + "i" + "ed"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
				#endif

				#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
				//added 2h2d
				//GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP _ment:
				//potential rule 1ai: -> ment eg movement/government/derailment/detainment/enjoyment
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aii: -> ament eg disarmament
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aiii: -> lment eg enrollment/installment/fulfillment
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				/*
				//potential rule 1b: running - "run" + "n" [run n] + "itive"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				*/
				//potential rule 2i: e -> ment eg judgement/dislodgment
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE2, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				/*
				//potential rule 2ii: e -> ment + ive eg judgementive
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				*/
				//potential rule 3a: y -> iment eg worriment/suppliment/embodiment
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 5: pt -> pment eg entrapment/equipment
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast2LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE5, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);

				//GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP _ion:
				//potential rule 1ai: -> ion eg absorption/abstraction/adaptation
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aii: -> ition eg addition
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 1aiii: -> ation eg acceptation/affirmation
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				/*
				//potential rule 1b: running - "run" + "n" [run n] + "itive"
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_INVERSE_APPEND, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				*/
				//potential rule 2i: e -> ion eg relation/acclimatisation/accommodation/activation/accretion
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 2ii: e -> ition + ive eg competition/composition/definition
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 2iii: e -> ation + ive eg admiration/organisation
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 3a: ify -> ification eg subjectification/amplification/ammonification/identification/beautification
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE3, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 6i: aim -> amation eg acclamation {acclimation?}
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6I, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 6i: ide -> ision eg division
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6II, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				//potential rule 6iii: ish -> ition eg abolition/demolition
				this->testVerbCase(currentTagInVerbList->tagName, wordLowerCase, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6III, &numberOfCharactersInBaseTenseFormAppend, &foundVerbCase, baseNameFound, grammaticalTenseModifier, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
				#endif
			}
		}

		currentTagInVerbList = currentTagInVerbList->nextTag;
	}


	return foundVerbCase;
}

void GIApreprocessorMultiwordReductionClass::testVerbCase(string tagName, const string wordLowerCase, const string baseTenseFormStart, string baseTenseFormAppend, int* numberOfCharactersInBaseTenseFormAppend, bool* foundVerbCase, string* baseNameFound, int* grammaticalTenseModifier, int grammaticalTenseModifierNew)
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
bool GIApreprocessorMultiwordReductionClass::determineIfWordIsIrregularVerbContinuousCaseWrapper(const string word, string* baseNameFound)
{
	bool result = true;
	bool foundIrregularVerbContinuousCase = false;
	string irregularVerbListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_IRREGULARVERB_DATABASE_FILE_NAME;
	if(!irregularVerbListLoaded)
	{
		cout << "!irregularVerbListLoaded (GIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE requires -lrpfolder to be set): irregularVerbListFileName = " << irregularVerbListFileName << endl;
		result = false;
	}
	else
	{
		foundIrregularVerbContinuousCase = this->determineIfWordIsIrregularVerbContinuousCase(word, firstTagInIrregularVerbListGlobal, baseNameFound);
	}
	return foundIrregularVerbContinuousCase;
}


bool GIApreprocessorMultiwordReductionClass::determineIfWordIsIrregularVerbContinuousCase(const string word, GIApreprocessorMultiwordReductionWord* firstTagInIrregularVerbList, string* baseNameFound)
{
	bool foundIrregularVerbContinuousCase = false;

	GIApreprocessorMultiwordReductionWord* currentTagInIrregularVerbList = firstTagInIrregularVerbList;
	while(currentTagInIrregularVerbList->nextSentence != NULL)
	{
		int irregularVerbTagIndex = 0;
		GIApreprocessorMultiwordReductionWord* currentTagInIrregularVerb = currentTagInIrregularVerbList;
		while(currentTagInIrregularVerb->nextTag != NULL)
		{
			string wordLowerCase = SHAREDvars.convertStringToLowerCase(&word);

			if(irregularVerbTagIndex == 3)
			{
				if(wordLowerCase == currentTagInIrregularVerb->tagName)
				{
					foundIrregularVerbContinuousCase = true;
					*baseNameFound = currentTagInIrregularVerbList->tagName;
				}
			}
			currentTagInIrregularVerb = currentTagInIrregularVerb->nextTag;
			irregularVerbTagIndex++;
		}


		/*OLD (before RBB addition of continuous cases to wikipedia english irregular verb list - ie final/4th column of WikipediaIrregularVerbs.txt):

		string irregularVerbBaseForm = currentTagInIrregularVerbList->tagName;
		int irregularVerbBaseFormLength = irregularVerbBaseForm.length();

		string baseWithLastLetterDropped = irregularVerbBaseForm.substr(0, irregularVerbBaseFormLength-1);
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
		//cout << "baseWithLastLetterDropped = " << baseWithLastLetterDropped << endl;
		#endif

		string irregularVerbContinuousForm1 =  irregularVerbBaseForm + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;
		string irregularVerbContinuousForm2 = baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;

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



#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
bool GIApreprocessorMultiwordReductionClass::loadPrepositionsInverseList(const string prepositionsInverseListFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPrepositionsInverseList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionSentence* currentTagInPrepositionsInverseList = firstTagInPrepositionsInverseList;
	GIApreprocessorMultiwordReductionWord* firstTagInRow = currentTagInPrepositionsInverseList->firstTagInSentence;
	GIApreprocessorMultiwordReductionWord* currentTagInRow = firstTagInRow;

	ifstream parseFileObject(prepositionsInverseListFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
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

				currentTagInRow->tagName = currentWord;
				currentTagInRow->nextTag = new GIApreprocessorMultiwordReductionWord();
				currentTagInRow = currentTagInRow->nextTag;

				currentTagInPrepositionsInverseList->nextSentence = new GIApreprocessorMultiwordReductionSentence();
				currentTagInPrepositionsInverseList = currentTagInPrepositionsInverseList->nextSentence;
				currentTagInRow = currentTagInPrepositionsInverseList->firstTagInSentence;

				currentWord = "";
				lineIndex++;
			}
			else if(currentToken == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_NEXTCOLUMN)
			{

				currentTagInRow->tagName = currentWord;
				currentTagInRow->nextTag = new GIApreprocessorMultiwordReductionWord();
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
#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
void GIApreprocessorMultiwordReductionClass::detectIfInverseOrTwoWayConditionRequired(const string conditionName, bool* inverseConditionRequired, bool* twoWayConditionRequired, string* inverseConditionName)
{
	GIApreprocessorMultiwordReductionSentence* firstTagInPrepositionsInverseList = firstTagInPrepositionInverseListGlobal;

	*inverseConditionRequired = false;
	*twoWayConditionRequired = false;

	//invert condition if necessary
	GIApreprocessorMultiwordReductionSentence* currentTagInPrepositionsInverseList = firstTagInPrepositionsInverseList;
	while(currentTagInPrepositionsInverseList->nextSentence != NULL)
	{

		GIApreprocessorMultiwordReductionWord* firstTagInPrepositionsInverseListSentence = currentTagInPrepositionsInverseList->firstTagInSentence;
		GIApreprocessorMultiwordReductionWord* currentTagInPrepositionsInverseListSentence = firstTagInPrepositionsInverseListSentence;
		bool foundConditionToInvert = false;
		string conditionNameNew = "";	//= currentTagInPrepositionsInverseList->tagName;
		for(int i=1; i<=GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_NUMBER_OF_TAGS; i++)
		{
			if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_CONDITION)
			{
				conditionNameNew = currentTagInPrepositionsInverseListSentence->tagName;	//same as currentTagInPrepositionsInverseList->tagName
			}
			else if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_REVERSE_CONDITION)
			{
				if(currentTagInPrepositionsInverseListSentence->tagName == conditionName)
				{
					if(currentTagInPrepositionsInverseListSentence->tagName == conditionNameNew)
					{
						*twoWayConditionRequired = true;
					}
					else
					{
						foundConditionToInvert = true;
					}
				}
			}
			else if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID)
			{
				if(foundConditionToInvert)
				{
					if(currentTagInPrepositionsInverseListSentence->tagName == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID_VALUE_TRUE)
					{
						*inverseConditionRequired = true;
						*inverseConditionName = conditionNameNew;
					}
				}
			}
			currentTagInPrepositionsInverseListSentence = currentTagInPrepositionsInverseListSentence->nextTag;
		}

		currentTagInPrepositionsInverseList = currentTagInPrepositionsInverseList->nextSentence;
	}
}

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DETECT_PREPOSITION_TYPE
bool GIApreprocessorMultiwordReductionClass::identifyConditionType(GIAentityNode* conditionRelationshipEntity)
{
	bool conditionTypeIdentified = false;

	GIApreprocessorMultiwordReductionSentence* firstTagInPrepositionsInverseList = firstTagInPrepositionInverseListGlobal;

	//identify condition type
	GIApreprocessorMultiwordReductionSentence* currentTagInPrepositionsInverseList = firstTagInPrepositionsInverseList;
	while(currentTagInPrepositionsInverseList->nextSentence != NULL)
	{

		if(currentTagInPrepositionsInverseList->firstTagInSentence->tagName == conditionRelationshipEntity->entityName)	//the first tag in the sentence object corresponds to the preposition name
		{
			GIApreprocessorMultiwordReductionWord* firstTagInPrepositionsInverseListSentence = currentTagInPrepositionsInverseList->firstTagInSentence;
			GIApreprocessorMultiwordReductionWord* currentTagInPrepositionsInverseListSentence = firstTagInPrepositionsInverseListSentence;
			for(int i=1; i<=GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_NUMBER_OF_TAGS; i++)
			{
				if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_TYPE)
				{
					conditionRelationshipEntity->conditionType2 = currentTagInPrepositionsInverseListSentence->tagName;
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


#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
//warning: this function is only currently developed for infinitive and continuous case
bool GIApreprocessorMultiwordReductionClass::parseVerbDataGenerateAllTenseVariants()
{
	bool result = true;
	if(!verbListLoaded || !irregularVerbListLoaded)
	{
		cout << "!loadVerbDataAndGenerateAllTenseVariants (GIA with GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE requires -lrpfolder to be set)" << endl;
		result = false;
	}
	else
	{
		//get global variables
		GIApreprocessorMultiwordReductionWord* firstTagInVerbList = firstTagInVerbListGlobal;
		GIApreprocessorMultiwordReductionSentence* firstTagInIrregularVerbList = firstTagInIrregularVerbListGlobal;
		
		GIApreprocessorMultiwordReductionWord* currentTagInVerbList = firstTagInVerbList;
		while(currentTagInVerbList->nextTag != NULL)
		{			
			string base = currentTagInVerbList->tagName;

			this->generateTenseVariantsOfVerbBase(currentTagInVerbList, firstTagInIrregularVerbList);
			
			currentTagInVerbList = currentTagInVerbList->nextTag;
		}
	}
	return result;
}
#endif

#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET

bool GIApreprocessorMultiwordReductionClass::determineVerbCaseStandard(const string word, int* grammaticalBaseTenseForm)
{
	bool foundVerbCase = false;
	
	GIApreprocessorMultiwordReductionWord* firstTagInVerbList = firstTagInVerbListGlobal;
	
	GIApreprocessorMultiwordReductionWord* currentTagInVerbList = firstTagInVerbList;
	while(currentTagInVerbList->nextTag != NULL)
	{
		//cout << "currentTagInVerbList = " << currentTagInVerbList->tagName << endl;
		
		for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
		{
			for(int j=0; j<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS; j++)
			{
				//cout << "\tcurrentTagInVerbList->grammaticalTenseFormsArray[i][j] = " << currentTagInVerbList->grammaticalTenseFormsArray[i][j] << endl;

				if(currentTagInVerbList->grammaticalTenseFormsArray[i][j] == word)
				{					
					foundVerbCase = true;
					*grammaticalBaseTenseForm = i;
				}
			}
		}
		
		currentTagInVerbList = currentTagInVerbList->nextTag;
	}

	return foundVerbCase;	
}

bool GIApreprocessorMultiwordReductionClass::determineVerbCaseStandardWithAdditional(const string word, int* grammaticalBaseTenseForm)
{
	bool foundVerbCaseStandardOrAdditional = false;
	
	bool foundVerbCaseStandard = determineVerbCaseStandard(word, grammaticalBaseTenseForm);
	if(foundVerbCaseStandard)
	{
		foundVerbCaseStandardOrAdditional = true;
	}
	
	string determineVerbCaseAdditionalBaseNameFound = "";
	int determineVerbCaseAdditionalTenseModifier = INT_DEFAULT_VALUE;
	bool foundVerbCaseAdditional = determineVerbCaseAdditional(word, firstTagInVerbListGlobal, &determineVerbCaseAdditionalBaseNameFound, &determineVerbCaseAdditionalTenseModifier);

	if(foundVerbCaseAdditional)
	{
		if(determineVerbCaseAdditionalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE)
		{
			*grammaticalBaseTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS;
			foundVerbCaseStandardOrAdditional = true;
		}
		else if(determineVerbCaseAdditionalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP)
		{
			*grammaticalBaseTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST;
			foundVerbCaseStandardOrAdditional = true;
		}
	}
	
	return foundVerbCaseStandardOrAdditional;

}

bool GIApreprocessorMultiwordReductionClass::determineIsPreposition(const string word)
{
	GIApreprocessorMultiwordReductionWord* firstTagInPrepositionsList = firstTagInPrepositionListGlobal;
	return determineIsInWordList(firstTagInPrepositionsList, word);
}

bool GIApreprocessorMultiwordReductionClass::determineIsAdverb(const string word)
{
	GIApreprocessorMultiwordReductionWord* firstTagInAdverbList = firstTagInAdverbListGlobal;
	return determineIsInWordList(firstTagInAdverbList, word);
}

bool GIApreprocessorMultiwordReductionClass::determineIsAdjective(const string word)
{
	GIApreprocessorMultiwordReductionWord* firstTagInAdjectiveList = firstTagInAdjectiveListGlobal;
	return determineIsInWordList(firstTagInAdjectiveList, word);
}


bool GIApreprocessorMultiwordReductionClass::determineIsInWordList(GIApreprocessorMultiwordReductionWord* firstTagInWordList, const string word)
{
	bool prepositionFound = false;
	GIApreprocessorMultiwordReductionWord* currentTagInWordList = firstTagInWordList;
	while(currentTagInWordList->nextTag != NULL)
	{
		if(currentTagInWordList->tagName == word)	//the first tag in the sentence object corresponds to the preposition name
		{
			prepositionFound = true;
		}
		currentTagInWordList = currentTagInWordList->nextTag;
	}

	return prepositionFound;
}


#endif

/*
//FUTURE GIA;
#ifdef GIA_NLG
#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
bool GIApreprocessorMultiwordReductionClass::generateVerbCase(const string baseName, string* wordGrammatised, int grammaticalTenseForm, int version)
{
	bool foundVerbCase = false;
	
	GIApreprocessorMultiwordReductionWord* firstTagInVerbList = firstTagInVerbListGlobal;
	GIApreprocessorMultiwordReductionWord* currentTagInVerbList = firstTagInVerbList;
	while(currentTagInVerbList->nextSentence != NULL)
	{
		if(currentTagInVerbList->tagName == baseName)
		{
			foundVerbCase = true;
			*wordGrammatised = >grammaticalTenseFormsArray[grammaticalTenseForm][version];
		}
		
		currentTagInVerbList = currentTagInVerbList->nextSentence;
	}

	return foundVerbCase;	
}
#endif
#endif
*/
