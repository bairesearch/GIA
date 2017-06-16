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
 * Project Version: 3c3a 16-June-2017
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 *
 *******************************************************************************/


#include "GIApreprocessorMultiwordReduction.hpp"

bool lrpInitialised = false;
static string lrpDataFolderName;
static bool useLRP;
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*> prepositionInverseListGlobal;
bool prepositionInverseListLoaded;
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
unordered_map<string, GIApreprocessorMultiwordReductionWord*> verbListGlobal;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> verbCaseStandardListGlobal;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> verbCaseAdditionalListGlobal;
bool verbListLoaded;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> prepositionListGlobal;
bool prepositionListLoaded;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> adverbListGlobal;
bool adverbListLoaded;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> adjectiveListGlobal;
bool adjectiveListLoaded;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> nounListGlobal;
bool nounListLoaded;
unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*> irregularVerbListGlobal;
bool irregularVerbListLoaded;
#endif

GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* textGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* queryGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* activeGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;




bool GIApreprocessorMultiwordReductionClass::initialiseLRP(const string newLRPDataFolderName, const bool newUseLRP)
{
	bool result = true;
	
	useLRP = newUseLRP;
	lrpDataFolderName = newLRPDataFolderName;
	
	if(!lrpInitialised)
	{
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
		string prepositionsInverseListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_FILE_NAME;
		prepositionInverseListLoaded = false;
		if(!this->loadPrepositionsInverseList(prepositionsInverseListFileName, &prepositionInverseListGlobal))
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
		string irregularVerbListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_IRREGULARVERB_DATABASE_FILE_NAME;
		irregularVerbListLoaded = false;
		if(!this->loadIrregularVerbList(irregularVerbListFileName, &irregularVerbListGlobal))
		{
			cout << "!loadIrregularVerbList (GIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE requires -lrpfolder to be set): irregularVerbListFileName = " << irregularVerbListFileName << endl;
			result = false;
		}
		else
		{
			irregularVerbListLoaded = true;
		}

		if(!this->loadWordListWrapper(lrpDataFolderName, &verbListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_FILE_NAME, &verbListGlobal))
		{
			result = false;	
		}	
		if(!this->loadWordListWrapper(lrpDataFolderName, &adverbListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PREPOSITION_DATABASE_FILE_NAME, &prepositionListGlobal))
		{
			result = false;	
		}
		if(!this->loadWordListWrapper(lrpDataFolderName, &adverbListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_ADVERB_DATABASE_FILE_NAME, &adverbListGlobal))
		{
			result = false;	
		}
		if(!this->loadWordListWrapper(lrpDataFolderName, &adjectiveListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_ADJECTIVE_DATABASE_FILE_NAME, &adjectiveListGlobal))
		{
			result = false;	
		}
		if(!this->loadWordListWrapper(lrpDataFolderName, &nounListLoaded, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_FILE_NAME, &nounListGlobal))
		{
			result = false;	
		}

		if(!generateVerbCaseStandardList())	//this is required to make verbList usable
		{
			result = false;
		}

		if(!generateVerbCaseAdditionalList(&verbListGlobal, &verbCaseAdditionalListGlobal))	//this is required to make verbList usable
		{
			result = false;
		}
		#endif

		lrpInitialised = true;
	}
	
	return result;
}
bool GIApreprocessorMultiwordReductionClass::getUseLRP()
{
	return useLRP;
}

bool GIApreprocessorMultiwordReductionClass::loadWordListWrapper(const string lrpDataFolderName, bool* wordListLoaded, const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList)
{
	bool result = true;
	
	string wordListFileNameFull = lrpDataFolderName + wordListFileName;
	*wordListLoaded = false;
	if(!this->loadWordList(wordListFileNameFull, wordList))
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

bool GIApreprocessorMultiwordReductionClass::loadWordList(const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList)
{
	bool result = true;

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
				GIApreprocessorMultiwordReductionWord* currentTagInWordList = new GIApreprocessorMultiwordReductionWord();
				currentTagInWordList->tagName = currentWord;
				wordList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(currentWord, currentTagInWordList));
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


bool GIApreprocessorMultiwordReductionClass::parseTextFileAndReduceLanguage(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName)
{
	bool result = true;

	string phrasalVerbListFileName = lrpDataFolderName + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_FILE_NAME;
	multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*> phrasalVerbList;
	if(!this->loadPhrasalVerbDataAndGenerateAllTenseVariants(phrasalVerbListFileName, &phrasalVerbList, &irregularVerbListGlobal))
	{
		result = false;
	}

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo = this->getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	if(!this->searchAndReplacePhrasalVerbs(firstGIApreprocessorSentenceInList, &phrasalVerbList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo))
	{
		result = false;
	}

	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_PREPOSITION_DATABASE_FILE_NAME, firstGIApreprocessorSentenceInList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_TYPE))
	{
		result = false;
	}
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_ALL_WORD_TYPES
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_ADVERB_DATABASE_FILE_NAME, firstGIApreprocessorSentenceInList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADVERB_TYPE))
	{
		result = false;
	}
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_ADJECTIVE_DATABASE_FILE_NAME, firstGIApreprocessorSentenceInList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADJECTIVE_TYPE))
	{
		result = false;
	}
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_NOUN_DATABASE_FILE_NAME, firstGIApreprocessorSentenceInList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_NOUN_TYPE))
	{
		result = false;
	}
	if(!loadMultiwordWordListAndSearchAndReplace(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_VERB_DATABASE_FILE_NAME, firstGIApreprocessorSentenceInList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_TYPE))
	{
		result = false;
	}			
	#endif

	SHAREDvars.setCurrentDirectory(outputFolder);
	if(!this->writeTagListToFile(firstGIApreprocessorSentenceInList, plainTextLRPoutputFileName, plainTextLRPforNLPoutputFileName, true, true))
	{
		result = false;
	}
	
	return result;
}

bool GIApreprocessorMultiwordReductionClass::loadIrregularVerbList(const string irregularVerbListFileName, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionIrregularVerbWord* firstTagInIrregularVerb = new GIApreprocessorMultiwordReductionIrregularVerbWord();
	GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInIrregularVerb = firstTagInIrregularVerb;

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
					currentTagInIrregularVerb->alternateTag = new GIApreprocessorMultiwordReductionIrregularVerbWord();
					currentTagInIrregularVerb->alternateTag->tagName = currentWord;
				}
				else
				{
					currentTagInIrregularVerb->tagName = currentWord;
				}

				currentTagInIrregularVerb->nextTag = new GIApreprocessorMultiwordReductionIrregularVerbWord();
				currentTagInIrregularVerb = static_cast<GIApreprocessorMultiwordReductionIrregularVerbWord*>(currentTagInIrregularVerb->nextTag);

				if(currentToken == CHAR_NEWLINE)
				{
					GIApreprocessorMultiwordReductionIrregularVerbSentence* sentence = new GIApreprocessorMultiwordReductionIrregularVerbSentence();
					sentence->firstTagInSentence = firstTagInIrregularVerb;
					irregularVerbList->insert(pair<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>(firstTagInIrregularVerb->tagName, sentence));
					firstTagInIrregularVerb = new GIApreprocessorMultiwordReductionIrregularVerbWord();
					currentTagInIrregularVerb = firstTagInIrregularVerb;
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
bool GIApreprocessorMultiwordReductionClass::loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionPhrasalVerbSentence* currentTagInPhrasalVerbList = new GIApreprocessorMultiwordReductionPhrasalVerbSentence();
	GIApreprocessorMultiwordReductionPhrasalVerbSentence* recordOfNonAlternateTagInPhrasalVerbList = currentTagInPhrasalVerbList;

	GIApreprocessorMultiwordReductionPhrasalVerbWord* firstTagInPhrasalVerb = currentTagInPhrasalVerbList->firstTagInSentence;
	GIApreprocessorMultiwordReductionPhrasalVerbWord* currentTagInPhrasalVerb = firstTagInPhrasalVerb;
	GIApreprocessorMultiwordReductionPhrasalVerbWord* recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;
	
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
					currentTagInPhrasalVerbList->alternateSentence = new GIApreprocessorMultiwordReductionPhrasalVerbSentence();
					currentTagInPhrasalVerbList = currentTagInPhrasalVerbList->alternateSentence;
					currentTagInPhrasalVerb = currentTagInPhrasalVerbList->firstTagInSentence;
					currentPhrasalVerbAlternate = true;
				}
				else
				{
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
						this->generateStandardTenseVariantsOfVerbBase(currentTagInPhrasalVerb, irregularVerbList);
					}


					currentTagInPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionPhrasalVerbWord();
					currentTagInPhrasalVerb = static_cast<GIApreprocessorMultiwordReductionPhrasalVerbWord*>(currentTagInPhrasalVerb->nextTag);
					if(currentWordAlternate)
					{
						//revert to non-alternate tag...
						currentTagInPhrasalVerb = recordOfNonAlternateTagInPhrasalVerb;
						currentTagInPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionPhrasalVerbWord();
						currentTagInPhrasalVerb = static_cast<GIApreprocessorMultiwordReductionPhrasalVerbWord*>(currentTagInPhrasalVerb->nextTag);
					}

					if(currentToken == CHAR_NEWLINE)
					{
						if(currentPhrasalVerbAlternate)
						{
							//revert to non-alternate phrasal verb...
							currentTagInPhrasalVerbList = recordOfNonAlternateTagInPhrasalVerbList;
							currentPhrasalVerbAlternate = false;
						}
						
						phrasalVerbList->insert(pair<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>(firstTagInPhrasalVerb->tagName, currentTagInPhrasalVerbList));
						if(firstTagInPhrasalVerb->base)
						{	
							//for each variant of the firstTagInPhrasalVerb, insert the sentence into the list keyed by the variant name
							for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
							{
								for(int j=0; j<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS; j++)
								{
									string verbVariant = firstTagInPhrasalVerb->grammaticalTenseFormsArray[i][j];
									phrasalVerbList->insert(pair<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>(verbVariant, currentTagInPhrasalVerbList));
								}
							}
						}
						
						currentTagInPhrasalVerbList = new GIApreprocessorMultiwordReductionPhrasalVerbSentence();
						firstTagInPhrasalVerb = currentTagInPhrasalVerbList->firstTagInSentence;
						currentTagInPhrasalVerb = firstTagInPhrasalVerb; 
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
					this->generateStandardTenseVariantsOfVerbBase(currentTagInPhrasalVerb, irregularVerbList);								
				}
				if(!currentWordAlternate)
				{//added 1p1aTEMP6d
					recordOfNonAlternateTagInPhrasalVerb = currentTagInPhrasalVerb;
				}

				currentTagInPhrasalVerb->alternateTag = new GIApreprocessorMultiwordReductionPhrasalVerbWord();
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
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
			else if(currentToken == CHAR_APOSTROPHE)
			{
				currentTagInPhrasalVerb->tagName = currentWord;
				currentTagInPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionPhrasalVerbWord();
				currentTagInPhrasalVerb = static_cast<GIApreprocessorMultiwordReductionPhrasalVerbWord*>(currentTagInPhrasalVerb->nextTag);
				currentWord = "";
				currentWord = currentWord + currentToken;
			}
			#endif
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



bool GIApreprocessorMultiwordReductionClass::generateStandardTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList)
{
	bool result = true;

	string base = SHAREDvars.convertStringToLowerCase(&(baseTag->tagName));

	//1. check if irregular past / past participle case
	bool irregularVerbFound = false;
	GIApreprocessorMultiwordReductionIrregularVerbSentence* sentenceFound = NULL;
	if(findSentenceInSentenceListIrregularVerb(irregularVerbList, base, &sentenceFound))
	{
		irregularVerbFound = true;
		GIApreprocessorMultiwordReductionIrregularVerbWord* firstTagInIrregularVerb = sentenceFound->firstTagInSentence;
		GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInIrregularVerb = static_cast<GIApreprocessorMultiwordReductionIrregularVerbWord*>(firstTagInIrregularVerb->nextTag);
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
			currentTagInIrregularVerb = static_cast<GIApreprocessorMultiwordReductionIrregularVerbWord*>(currentTagInIrregularVerb->nextTag);
		}
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



	

//NB the collapsed phrasal verb contains precisely 2 entities: phrasalVerbCollapsed, entity2: thing/place/body (eg belong_to + sb/Tom) - thing/place/bodies are not currently being differentiated by the LRP as this information is only first generated at NLP/GIA parse stage
bool GIApreprocessorMultiwordReductionClass::searchAndReplacePhrasalVerbs(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo)
{
	bool result = true;

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;	//new correspondence info for each found phrasal verb

	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		GIApreprocessorMultiwordReductionClassObject.generateFlatSentenceWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), &firstTagInPlainTextSentence);
		GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIApreprocessorMultiwordReductionPlainTextWord* previousTagInPlainTextSentence = NULL;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			bool foundAtLeastOnePhrasalVerbInSentenceAndCollapsed = false;
			int numberWordsInMultiwordMatched = 0;
			
			pair<multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>::iterator, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>::iterator> range;
			range = phrasalVerbList->equal_range(currentTagInPlainTextSentence->tagName);
			for(multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>::iterator currentTagInPhrasalVerbListNormOrAlternateIterator = range.first; currentTagInPhrasalVerbListNormOrAlternateIterator != range.second; ++currentTagInPhrasalVerbListNormOrAlternateIterator)
			{	
				GIApreprocessorMultiwordReductionPhrasalVerbSentence* currentTagInPhrasalVerbListNormOrAlternate = currentTagInPhrasalVerbListNormOrAlternateIterator->second;
				
				bool alternatePhrasalVerb = false;
				while(!alternatePhrasalVerb || (currentTagInPhrasalVerbListNormOrAlternate->alternateSentence != NULL))
				{
					/*
					if(tolower((currentTagInPhrasalVerbList->tagName)[0]) == tolower((currentTagInPlainTextSentence->tagName)[0]))
					{//optimisation; only deal with phrasal verbs that start with the same character...
					*/
					GIApreprocessorMultiwordReductionPhrasalVerbWord* firstTagInPhrasalVerb = currentTagInPhrasalVerbListNormOrAlternate->firstTagInSentence;
								
					GIApreprocessorMultiwordReductionPhrasalVerbWord* currentTagInPhrasalVerb = firstTagInPhrasalVerb;
					bool stillFoundVerbMatchOfArbitraryTense = true;
					int numberWordsInMultiword = 0;
					bool foundAtLeastOneMatch = false;
					GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
					GIApreprocessorMultiwordReductionPlainTextWord* lastTagInPlainTextSentenceTemp = NULL;	//pointer used to change original plain text to collapsed phrasal verb text
					GIApreprocessorMultiwordReductionPlainTextWord* firstTagInCollapsedPhrasalVerb = new GIApreprocessorMultiwordReductionPlainTextWord();
					GIApreprocessorMultiwordReductionPlainTextWord* currentTagInCollapsedPhrasalVerb = firstTagInCollapsedPhrasalVerb;
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
							currentTagInCollapsedPhrasalVerb->nextTag = new GIApreprocessorMultiwordReductionPlainTextWord();
							GIApreprocessorMultiwordReductionPlainTextWord* currentTagInCollapsedPhrasalVerbSpecialArbitrarySection = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInCollapsedPhrasalVerb->nextTag);
							currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->tagName = currentTagInPlainTextSentenceTemp->tagName;		//arbitrary thing/place/body name
							currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->collapsedPhrasalVerbExactDefinedSection = false;
							//currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->collapsedPhrasalVerbExactDefinedSectionTemp = false;
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
							GIApreprocessorMultiwordReductionPhrasalVerbWord* currentTagInPhrasalVerbNormOrAlternate = currentTagInPhrasalVerb;
							bool normalTag = true;
							bool alternateTag = false;
							string generatedTagNameLemma = "";
							while(normalTag || alternateTag)
							{
								if(currentTagInPhrasalVerb->base)
								{
									for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
									{
										for(int j=0; j<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS; j++)
										{
											if(currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[i][j] == currentTagInPlainTextSentenceTemp->tagName)		//USED TO BE currentTagInCollapsedPhrasalVerb before 6 Sept 2012
											{
												currentTagInCollapsedPhrasalVerb->grammaticalTenseFormDetected = i;								//USED TO BE AND STILL IS currentTagInCollapsedPhrasalVerb before 6 Sept 2012
												generatedTagNameLemma = currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD];
												foundVerbMatchOfArbitraryTenseTemp = true;
											}
										}
									}
								}
								else
								{
									if(currentTagInPhrasalVerbNormOrAlternate->tagName == currentTagInPlainTextSentenceTemp->tagName)
									{
										foundVerbMatchOfArbitraryTenseTemp = true;
									}
								}
								if(currentTagInPhrasalVerbNormOrAlternate->alternateTag != NULL)
								{
									currentTagInPhrasalVerbNormOrAlternate = currentTagInPhrasalVerbNormOrAlternate->alternateTag;
									alternateTag = true;
								}
								else
								{
									alternateTag = false;
								}
								normalTag = false;
							}


							if(foundVerbMatchOfArbitraryTenseTemp)
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

								#ifdef GIA_DEBUG_PREPROCESSOR
								cout << "numberWordsInMultiword++" << endl;
								cout << "currentTagInPhrasalVerb = " << currentTagInPhrasalVerb->tagName << endl;
								cout << "currentTagInPlainTextSentenceTemp = " << currentTagInPlainTextSentenceTemp->tagName << endl;
								cout << "generateTextFromPreprocessorSentenceWordList(firstTagInPhrasalVerb) = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromPreprocessorSentenceWordList(firstTagInPhrasalVerb) << endl;
								#endif
								
								foundAtLeastOneMatch = true;
								numberWordsInMultiword++;
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
							else
							{
								if(currentTagInPhrasalVerb->optional)
								{
									currentTagInPhrasalVerbOptionalAndNotFound = true;
								}
								else if(currentlyParsingTagSpecial && (tagInPhrasalVerbSpecialAndNotFoundCount <= GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_MAX_NUMBER_WORDS))
								{
									//changed 2b2e - 24 May 2017
									GIApreprocessorMultiwordReductionPlainTextWord* currentTagInCollapsedPhrasalVerbSpecialArbitrarySection = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInCollapsedPhrasalVerb->nextTag);
									while(currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->nextTag != NULL)
									{
										currentTagInCollapsedPhrasalVerbSpecialArbitrarySection = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->nextTag);
									}
									currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->nextTag = new GIApreprocessorMultiwordReductionPlainTextWord();
									currentTagInCollapsedPhrasalVerbSpecialArbitrarySection = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->nextTag);
									currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->tagName = currentTagInPlainTextSentenceTemp->tagName;		//arbitrary thing/place/body name
									currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->collapsedPhrasalVerbExactDefinedSection = false;
									//currentTagInCollapsedPhrasalVerbSpecialArbitrarySection->collapsedPhrasalVerbExactDefinedSectionTemp = false;
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
						}

						if(currentTagInPhrasalVerb->tagSpecialArbitraryName)
						{
							if(currentTagInPhrasalVerb->optional)
							{
								//special+optional tag found - just increment the phrasal verb for now [for this round]...
								currentTagInPhrasalVerb = static_cast<GIApreprocessorMultiwordReductionPhrasalVerbWord*>(currentTagInPhrasalVerb->nextTag);
							}
							else
							{
								currentTagInPhrasalVerb = static_cast<GIApreprocessorMultiwordReductionPhrasalVerbWord*>(currentTagInPhrasalVerb->nextTag);
								currentTagInPlainTextSentenceTemp = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp->nextTag);
							}
						}
						else if(currentTagInPhrasalVerbOptionalAndNotFound)
						{
							//optional phrasal verb tag not found - just increment the phrasal verb tag list...
							currentTagInPhrasalVerb = static_cast<GIApreprocessorMultiwordReductionPhrasalVerbWord*>(currentTagInPhrasalVerb->nextTag);
						}
						else if(currentTagInPhrasalVerbSpecialAndNotFound)
						{
							//next phrasal verb tag not found yet - just increment the sentence tag list...
							currentTagInPlainTextSentenceTemp = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp->nextTag);
						}
						else
						{
							currentTagInPhrasalVerb = static_cast<GIApreprocessorMultiwordReductionPhrasalVerbWord*>(currentTagInPhrasalVerb->nextTag);
							currentTagInPlainTextSentenceTemp = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp->nextTag);
						}
					}

					if(stillFoundVerbMatchOfArbitraryTense && foundAtLeastOneMatch)
					{
						if(currentTagInPhrasalVerb->nextTag == NULL)
						{//make sure the entire multiword phrasal verb is matched (in case currentTagInPlainTextSentenceTemp reaches end of sentence without matching the multiword phrasal verb in its entirety)

							if(numberTagSpecialTagsFound <= 1)
							{//do not preprocess phrasal verbs with more than one special tag (ie sth/sb/swh) - as this generally involves more than a verb [verb sth preposition sth1] - added 1p1aTEMP5
								
								//reduce all entities
								
								numberWordsInMultiwordMatched = numberWordsInMultiword;
								
								#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
								//updated 3b4b
								//assume firstTagInCollapsedMultiwordWord->preprocessorUpperLevelWordReference already set
								GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentenceTemp2 = currentTagInPlainTextSentence;
								for(int i=0; i<numberWordsInMultiwordMatched; i++)
								{
									currentTagInPlainTextSentenceTemp2->preprocessorUpperLevelWordReference->preprocessorLowerLevelWordReference = firstTagInCollapsedPhrasalVerb;
									currentTagInPlainTextSentenceTemp2 = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp2->nextTag);	
								}
								firstTagInCollapsedPhrasalVerb->preprocessorUpperLevelWordReference = currentTagInPlainTextSentence->preprocessorUpperLevelWordReference;
								firstTagInCollapsedPhrasalVerb->preprocessorUpperLevelWordReferenceSize = numberWordsInMultiwordMatched;
								#endif							
								
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
			}
			if(foundAtLeastOnePhrasalVerbInSentenceAndCollapsed)
			{
				#ifdef GIA_DEBUG_PREPROCESSOR
				cout << "foundAtLeastOnePhrasalVerbInSentenceAndCollapsed; currentTagInPlainTextSentence = " << currentTagInPlainTextSentence->tagName << endl;
				#endif
				
				//renumberEntityIndiciesInCorrespondenceInfo(firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, currentGIApreprocessorSentenceInList->sentenceIndexOriginal, numberWordsInMultiwordMatched); - this is not required because searchAndReplaceMultiwordWordList is executed after searchAndReplacePhrasalVerbs
				GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
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
						currentCorrespondenceInfo->sentenceIndex = currentGIApreprocessorSentenceInList->sentenceIndexOriginal;
						currentCorrespondenceInfo->entityIndex = newEntityIndex;
						currentCorrespondenceInfo->lemmaWithLRP = currentTagInPlainTextSentenceTemp2->tagNameLemma;
						currentCorrespondenceInfo->wordWithLRP = currentTagInPlainTextSentenceTemp2->tagName;
						currentCorrespondenceInfo->wordWithLRPforNLPonly = generateWordWithLRPforNLPonly(currentTagInPlainTextSentenceTemp2);
						currentCorrespondenceInfo->next = new GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
						currentCorrespondenceInfo = currentCorrespondenceInfo->next;
					}
					currentTagInPlainTextSentenceTemp2->entityIndex = newEntityIndex;
					currentTagInPlainTextSentenceTemp2 = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp2->nextTag);
					newEntityIndex++;
				}
			}
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentence->nextTag);
		}

		currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		
		#ifdef GIA_PREPROCESSOR_DEBUG
		cout << "\t";
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			cout << currentTagInPlainTextSentence->tagName << " ";
			currentTagInPlainTextSentence = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentence->nextTag);
		}
		cout << endl;
		#endif
		
		GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstTagInPlainTextSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsLRP));

		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	return result;
}


bool GIApreprocessorMultiwordReductionClass::loadMultiwordWordListAndSearchAndReplace(const string multiwordWordListFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
{
	bool result = true;
	
	string multiwordWordListFileNameFullPath = lrpDataFolderName + multiwordWordListFileName;
	multimap<string, GIApreprocessorMultiwordReductionBasicSentence*> multiwordWordList;
	if(!this->loadMultiwordWordList(multiwordWordListFileNameFullPath, &multiwordWordList))
	{
		result = false;
	}
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;
	while(currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo->next != NULL)
	{
		currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo = currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo->next;	//added 2j6d (add to end of list)
	}
	if(!this->searchAndReplaceMultiwordWordList(firstGIApreprocessorSentenceInList, &multiwordWordList, currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, wordListType))
	{
		result = false;
	}
	
	return result;
}	
	
bool GIApreprocessorMultiwordReductionClass::loadMultiwordWordList(const string multiwordWordListFileName, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionWord* firstTagInMultiwordWord = new GIApreprocessorMultiwordReductionWord();
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
				currentTagInMultiwordWord = static_cast<GIApreprocessorMultiwordReductionWord*>(currentTagInMultiwordWord->nextTag);
				if(currentToken == CHAR_NEWLINE)
				{
					GIApreprocessorMultiwordReductionBasicSentence* sentence = new GIApreprocessorMultiwordReductionBasicSentence();
					sentence->firstTagInSentence = firstTagInMultiwordWord;
					multiwordWordList->insert(pair<string, GIApreprocessorMultiwordReductionBasicSentence*>(firstTagInMultiwordWord->tagName, sentence));
					firstTagInMultiwordWord = new GIApreprocessorMultiwordReductionWord();
					currentTagInMultiwordWord = firstTagInMultiwordWord;
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

bool GIApreprocessorMultiwordReductionClass::searchAndReplaceMultiwordWordList(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
{
	bool result = true;
	
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo;	//new correspondence info for each found multiword word

	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{		
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		GIApreprocessorMultiwordReductionClassObject.generateFlatSentenceWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), &firstTagInPlainTextSentence);
		GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIApreprocessorMultiwordReductionPlainTextWord* previousTagInPlainTextSentence = NULL;

		int entityIndex = GIA_NLP_START_ENTITY_INDEX;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{						
			bool foundAtLeastOneMultiwordWordInSentenceAndCollapsed = false;
			int numberWordsInMultiwordMatched = 0;
			
			pair<multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>::iterator, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>::iterator> range;
			range = multiwordWordList->equal_range(currentTagInPlainTextSentence->tagName);
			for(multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>::iterator currentTagInMultiwordWordListIterator = range.first; currentTagInMultiwordWordListIterator != range.second; ++currentTagInMultiwordWordListIterator)
			{	
				GIApreprocessorMultiwordReductionBasicSentence* currentTagInMultiwordWordList = currentTagInMultiwordWordListIterator->second;
				//cout << "currentTagInMultiwordWordList->firstTagInSentence->tagName = " << currentTagInMultiwordWordList->firstTagInSentence->tagName << endl;
		
				bool foundAtLeastOneMultiwordWordInSentenceAndCollapsedTemp = false;
				bool stillFoundWordMatch = true;
				int numberWordsInMultiword = 0;
				bool foundAtLeastOneMatch = false;
				GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentenceTemp = currentTagInPlainTextSentence;
				GIApreprocessorMultiwordReductionPlainTextWord* firstTagInCollapsedMultiwordWord = new GIApreprocessorMultiwordReductionPlainTextWord();
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
					currentTagInMultiwordWord = static_cast<GIApreprocessorMultiwordReductionWord*>(currentTagInMultiwordWord->nextTag);
					currentTagInPlainTextSentenceTemp = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp->nextTag);
				}

				if(stillFoundWordMatch && foundAtLeastOneMatch)
				{
					if(currentTagInMultiwordWord->nextTag == NULL)
					{//make sure the entire multiword word is matched (in case currentTagInPlainTextSentenceTemp reaches end of sentence without matching the multiword word in its entirety)
						
						//reduce all entities
						
						numberWordsInMultiwordMatched = numberWordsInMultiword;
						#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
						//updated 3b4b
						//assume firstTagInCollapsedMultiwordWord->preprocessorUpperLevelWordReference already set
						GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentenceTemp2 = currentTagInPlainTextSentence;
						for(int i=0; i<numberWordsInMultiwordMatched; i++)
						{
							currentTagInPlainTextSentenceTemp2->preprocessorUpperLevelWordReference->preprocessorLowerLevelWordReference = firstTagInCollapsedMultiwordWord;
							currentTagInPlainTextSentenceTemp2 = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp2->nextTag);	
						}
						firstTagInCollapsedMultiwordWord->preprocessorUpperLevelWordReference = currentTagInPlainTextSentence->preprocessorUpperLevelWordReference;
						firstTagInCollapsedMultiwordWord->preprocessorUpperLevelWordReferenceSize = numberWordsInMultiwordMatched;
						#endif
						
						firstTagInCollapsedMultiwordWord->nextTag = currentTagInPlainTextSentenceTemp;
						if(previousTagInPlainTextSentence != NULL)
						{
							previousTagInPlainTextSentence->nextTag = firstTagInCollapsedMultiwordWord;
						}
						else
						{
							firstTagInPlainTextSentence = firstTagInCollapsedMultiwordWord;
						}
						foundAtLeastOneMultiwordWordInSentenceAndCollapsed = true;
						foundAtLeastOneMultiwordWordInSentenceAndCollapsedTemp = true;
						
					}
				}
				if(!foundAtLeastOneMultiwordWordInSentenceAndCollapsedTemp)
				{
					delete firstTagInCollapsedMultiwordWord;
				}
			}
			if(foundAtLeastOneMultiwordWordInSentenceAndCollapsed)
			{
				renumberEntityIndiciesInCorrespondenceInfo(firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, currentGIApreprocessorSentenceInList->sentenceIndexOriginal, entityIndex, numberWordsInMultiwordMatched);	//this is required for revertNLPtagNameToOfficialLRPtagName	//this is required because searchAndReplaceMultiwordWordList (zero or more times)/searchAndReplacePhrasalVerbs is executed before searchAndReplaceMultiwordWordList 
				GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentenceTemp2 = firstTagInPlainTextSentence;
				int newEntityIndex = GIA_NLP_START_ENTITY_INDEX;
				//int collapsedMultiwordWordIndex = 0;
				
				while(currentTagInPlainTextSentenceTemp2->nextTag != NULL)
				{
					if(currentTagInPlainTextSentenceTemp2->collapsedMultiwordWordTemp)
					{//create a new correspondenceInfo
						if(newEntityIndex != entityIndex)
						{
							cerr << "newEntityIndex = " << newEntityIndex << endl;
							cerr << "entityIndex = " << entityIndex << endl;
							cerr << "currentTagInPlainTextSentenceTemp2->tagName = " << currentTagInPlainTextSentenceTemp2->tagName << endl;
							cerr << "GIApreprocessorMultiwordReductionClass::searchAndReplaceMultiwordWordList error: (newEntityIndex != entityIndex)" << endl;
							cerr << "generateTextFromPreprocessorSentenceWordList(firstTagInPlainTextSentence) = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromPreprocessorSentenceWordList(firstTagInPlainTextSentence) << endl;
							exit(EXIT_ERROR);
						}
						currentTagInPlainTextSentenceTemp2->collapsedMultiwordWordTemp = false;
						string tagName = currentTagInPlainTextSentenceTemp2->tagName;
						string tagNameWithLastLetterDropped = tagName.substr(0, tagName.length()-1);
						currentTagInPlainTextSentenceTemp2->tagName = tagNameWithLastLetterDropped;	//remove last GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER
						currentCorrespondenceInfo->sentenceIndex = currentGIApreprocessorSentenceInList->sentenceIndexOriginal;
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
					currentTagInPlainTextSentenceTemp2 = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp2->nextTag);
					newEntityIndex++;
				}
				
				entityIndex = entityIndex - (numberWordsInMultiwordMatched-1);	//added 3b2f
			}
			else
			{
				
			}
			entityIndex++;
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentence->nextTag);
		}
		
		GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstTagInPlainTextSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsLRP));

		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
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

bool GIApreprocessorMultiwordReductionClass::writeTagListToFile(const GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPoutput, const bool performLRPforNLPoutput)
{
	bool result = true;
	
	ofstream* plainTextLRPOutput;
	ofstream* plainTextLRPforNLPOutput;
	if(performLRPoutput)
	{
		plainTextLRPOutput = new ofstream(plainTextLRPoutputFileName.c_str());
	}
	if(performLRPforNLPoutput)
	{
		plainTextLRPforNLPOutput = new ofstream(plainTextLRPforNLPoutputFileName.c_str());
	}
	
	bool firstCharacterInFile = true;

	const GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		string sentenceContentsLRP = "";
		string sentenceContentsLRPforNLP = "";
		bool firstCharacterInSentence = true;
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		GIApreprocessorMultiwordReductionClassObject.generateFlatSentenceWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), &firstTagInPlainTextSentence);

		GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{
			string plainTextLRPOutputTag = "";
			if(performLRPoutput)
			{
				plainTextLRPOutputTag = currentTagInPlainTextSentence->tagName;
			}
			string plainTextLRPforNLPOutputTag = "";
			if(performLRPforNLPoutput)
			{
				plainTextLRPforNLPOutputTag = generateWordWithLRPforNLPonly(currentTagInPlainTextSentence);
				if(plainTextLRPforNLPOutputTag != plainTextLRPOutputTag)
				{
					currentTagInPlainTextSentence->tagNameLRPforNLP = plainTextLRPforNLPOutputTag;
				}
				/*
				//Alternate method;
				if(!performLRPoutput)
				{
					if(currentTagInPlainTextSentence->tagNameLRPforNLP != "")
					{
						plainTextLRPforNLPOutputTag = currentTagInPlainTextSentence->tagNameLRPforNLP;
					}				
				}
				*/
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
				if(performLRPoutput)
				{
					plainTextLRPOutputTag = string(STRING_SPACE) + plainTextLRPOutputTag;
				}
				if(performLRPforNLPoutput)
				{
					plainTextLRPforNLPOutputTag = string(STRING_SPACE) + plainTextLRPforNLPOutputTag;
				}
			}
			if(performLRPoutput)
			{
				sentenceContentsLRP = sentenceContentsLRP + plainTextLRPOutputTag;
			}
			if(performLRPforNLPoutput)
			{
				sentenceContentsLRPforNLP = sentenceContentsLRPforNLP + plainTextLRPforNLPOutputTag;
			}
			
			firstCharacterInFile = false;
			firstCharacterInSentence = false;

			currentTagInPlainTextSentence = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentence->nextTag);
		}
		
		if(performLRPoutput)
		{
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE
			sentenceContentsLRP = sentenceContentsLRP + CHAR_NEWLINE;
			#endif
			plainTextLRPOutput->write(sentenceContentsLRP.c_str(), sentenceContentsLRP.length());
		}
		if(performLRPforNLPoutput)
		{
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE
			sentenceContentsLRPforNLP = sentenceContentsLRPforNLP + CHAR_NEWLINE;
			#endif
			plainTextLRPforNLPOutput->write(sentenceContentsLRPforNLP.c_str(), sentenceContentsLRPforNLP.length());
		}
				
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	if(performLRPoutput)
	{
		plainTextLRPOutput->close();
	}
	if(performLRPforNLPoutput)
	{
		plainTextLRPforNLPOutput->close();
	}
	
	return result;
}

string GIApreprocessorMultiwordReductionClass::generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence)
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
						cerr << "error: !foundPrepositionGovernorAndDependentInFeatureList;" << endl;
						cerr << word << "(" << relationGovernor << ", " << relationDependent << ")" << endl;
						exit(EXIT_ERROR);
					}
				}
				else
				{
					if(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex == entityIndexForNonPrepositionsOnly)
					{
						/*
						cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRPforNLPonly << endl;
						cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex << endl;
						cout << "sentenceIndex = " << sentenceIndex << endl;
						cout << "entityIndexForNonPrepositionsOnly = " << entityIndexForNonPrepositionsOnly << endl;
						cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->wordWithLRP << endl;						
						*/

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
	
	if(foundCorrespondingLRPtag)
	{
		*foundOfficialLRPreplacementString = true;
	}
}
#endif



#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
bool GIApreprocessorMultiwordReductionClass::generateVerbCaseStandardList()
{
	//aka parseVerbDataGenerateAllTenseVariants
	
	bool result = true;
	if(!verbListLoaded || !irregularVerbListLoaded)
	{
		cout << "!loadVerbDataAndGenerateAllTenseVariants (GIA with GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE requires -lrpfolder to be set)" << endl;
		result = false;
	}
	else
	{		
		//get global variables
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbList = &verbListGlobal;
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseStandardList = &verbCaseStandardListGlobal;
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList = &verbCaseAdditionalListGlobal;
		unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList = &irregularVerbListGlobal;
		
		for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator currentTagInVerbListIterator = verbList->begin(); currentTagInVerbListIterator != verbList->end(); ++currentTagInVerbListIterator)
		{	
			GIApreprocessorMultiwordReductionWord* currentTagInVerbList = currentTagInVerbListIterator->second;	

			verbCaseStandardList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(currentTagInVerbList->tagName, currentTagInVerbList));
			
			//for each variant of the firstTagInPhrasalVerb, insert the sentence into the list keyed by the variant name
			this->generateStandardTenseVariantsOfVerbBase(currentTagInVerbList, irregularVerbList);
			for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
			{
				for(int j=0; j<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS; j++)
				{
					string generatedTagNameLemma = currentTagInVerbList->grammaticalTenseFormsArray[i][j];
					verbCaseStandardList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(generatedTagNameLemma, currentTagInVerbList));
				}
			}
		}
	}
	return result;
}

//warning: this function is only currently developed for infinitive and continuous case
bool GIApreprocessorMultiwordReductionClass::generateVerbCaseAdditionalList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList)
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

	This code uses a subset of code from generateStandardTenseVariantsOfVerbBase (note generateStandardTenseVariantsOfVerbBase is not used itself as this code is designed to be liberal/robust and detect all possible verbs without being subject to theoretical grammar rules)
	*/
	
	for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator currentTagInVerbListIterator = verbList->begin(); currentTagInVerbListIterator != verbList->end(); ++currentTagInVerbListIterator)
	{	
		GIApreprocessorMultiwordReductionWord* currentTagInVerbList = currentTagInVerbListIterator->second;

		string base = currentTagInVerbList->tagName;

		this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, "", GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP);

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
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
			//continuous rule 1b: running - "run" + "n" [run n] + "ing"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
			//continuous rule 2: changing - "chang" [change e] + "ing"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
			/*
			//continuous rule 3a: N/A !marriing (use marrying)
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP);
			*/

			#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
			//added 2h2a
			//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND _able:
			//potential rule 1a/3b/4: thinkable/changeable
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
			//potential rule 1b: running - "run" + "n" [run n] + "able"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
			/*
			//potential rule 2: N/A !changable (use changeable)
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
			*/
			//potential rule 3a: running - "marr" + "i" + "able"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND_CASE3, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP);
			#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
			//added 2h2c
			//GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP _ive:
			//potential rule 1ai: -> ive eg resistive/adaptive
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
			//potential rule 1aii: -> itive eg additive
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1II, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
			//potential rule 1aiii: -> ative eg affirmative
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1III, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
			/*
			//potential rule 1b: running - "run" + "n" [run n] + "itive"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
			*/
			//potential rule 2i: e -> itive eg competitive/definitive/accomodative
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
			//potential rule 2ii: e -> ment + ive eg judgementive
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
			//potential rule 3a: y -> iment + ive eg supplimentive
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE3, GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP);
			#endif
			#endif

			#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
			//added 2h2a
			//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND _ed:
			//possible state rule 1a/3b/4: visited/opened
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
			//possible state rule 1b: rubbed/stopped/referred/admitted - "rub" + "b" + "ed"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
			//possible state rule 2: smiled/fined - "smil" [change e] + "ed"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
			//possible state rule 3a: studied/married - "marr" + "i" + "ed"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3, GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP);
			#endif

			#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
			//added 2h2d
			//GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP _ment:
			//potential rule 1ai: -> ment eg movement/government/derailment/detainment/enjoyment
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 1aii: -> ament eg disarmament
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1II, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 1aiii: -> lment eg enrollment/installment/fulfillment
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1III, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			/*
			//potential rule 1b: running - "run" + "n" [run n] + "itive"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_INVERSE_APPEND, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			*/
			//potential rule 2i: e -> ment eg judgement/dislodgment
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE2, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			/*
			//potential rule 2ii: e -> ment + ive eg judgementive
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			*/
			//potential rule 3a: y -> iment eg worriment/suppliment/embodiment
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE3, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 5: pt -> pment eg entrapment/equipment
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast2LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE5, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);

			//GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP _ion:
			//potential rule 1ai: -> ion eg absorption/abstraction/adaptation
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 1aii: -> ition eg addition
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1II, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 1aiii: -> ation eg acceptation/affirmation
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1III, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			/*
			//potential rule 1b: running - "run" + "n" [run n] + "itive"
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_INVERSE_APPEND, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			*/
			//potential rule 2i: e -> ion eg relation/acclimatisation/accommodation/activation/accretion
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 2ii: e -> ition + ive eg competition/composition/definition
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2II, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 2iii: e -> ation + ive eg admiration/organisation
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2III, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 3a: ify -> ification eg subjectification/amplification/ammonification/identification/beautification
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE3, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 6i: aim -> amation eg acclamation {acclimation?}
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6I, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 6i: ide -> ision eg division
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6II, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			//potential rule 6iii: ish -> ition eg abolition/demolition
			this->addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6III, GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP);
			#endif
		}
	}

	return foundVerbCase;
}

void GIApreprocessorMultiwordReductionClass::addVerbCaseAdditional(GIApreprocessorMultiwordReductionWord* currentTagInVerbList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList, const string baseTenseFormStart, const string baseTenseFormAppend, const int grammaticalTenseModifierNew)
{
	string hypotheticalVerbVariantCase = baseTenseFormStart + baseTenseFormAppend;

	GIApreprocessorMultiwordReductionWord* newWord = new GIApreprocessorMultiwordReductionWord();
	newWord->grammaticalTenseModifier = grammaticalTenseModifierNew;
	newWord->baseName = currentTagInVerbList->tagName;

	verbCaseAdditionalList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(hypotheticalVerbVariantCase, newWord));
}
#endif

#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
//warning: this function is only currently developed for infinitive and continuous case
bool GIApreprocessorMultiwordReductionClass::determineVerbCaseAdditionalWrapper(const string word, string* baseNameFound, int* grammaticalTenseModifier)
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
		foundVerbCase = this->determineVerbCaseAdditional(word, baseNameFound, grammaticalTenseModifier);

	}
	return foundVerbCase;
}

#endif


#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
//NB determineIfWordIsIrregularVerbContinuousCaseWrapper() can be used instead of determineVerbCaseAdditionalWrapper(), as Stanford only has a problem identifying verbs (pos tag "VBG") when they are irregular varbs
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


//determineIfWordIsIrregularVerbContinuousCase requires updating for new vector list implementation:
bool GIApreprocessorMultiwordReductionClass::determineIfWordIsIrregularVerbContinuousCase(const string word, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList, string* baseNameFound)
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
#endif


#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
bool GIApreprocessorMultiwordReductionClass::loadPrepositionsInverseList(const string prepositionsInverseListFileName, unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* prepositionInverseList)
{
	bool result = true;

	GIApreprocessorMultiwordReductionWord* firstTagInRow = new GIApreprocessorMultiwordReductionWord();
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
				currentTagInRow = static_cast<GIApreprocessorMultiwordReductionWord*>(currentTagInRow->nextTag);	//not required
				
				GIApreprocessorMultiwordReductionBasicSentence* sentence = new GIApreprocessorMultiwordReductionBasicSentence();
				sentence->firstTagInSentence = firstTagInRow;
				prepositionInverseList->insert(pair<string, GIApreprocessorMultiwordReductionBasicSentence*>(firstTagInRow->tagName, sentence));

				firstTagInRow = new GIApreprocessorMultiwordReductionWord();
				currentTagInRow = firstTagInRow;

				currentWord = "";
				lineIndex++;
			}
			else if(currentToken == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_NEXTCOLUMN)
			{

				currentTagInRow->tagName = currentWord;
				currentTagInRow->nextTag = new GIApreprocessorMultiwordReductionWord();
				currentTagInRow = static_cast<GIApreprocessorMultiwordReductionWord*>(currentTagInRow->nextTag);

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
	unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* prepositionInverseList = &prepositionInverseListGlobal;

	*inverseConditionRequired = false;
	*twoWayConditionRequired = false;

	GIApreprocessorMultiwordReductionBasicSentence* sentenceFound = NULL;
	if(findSentenceInSentenceListBasic(prepositionInverseList, conditionName, &sentenceFound))
	{
		//invert condition if necessary
		GIApreprocessorMultiwordReductionWord* firstTagInPrepositionsInverseListSentence = sentenceFound->firstTagInSentence;
		GIApreprocessorMultiwordReductionWord* currentTagInPrepositionsInverseListSentence = firstTagInPrepositionsInverseListSentence;
		bool foundConditionToInvert = false;
		string conditionNameInverted = "";	//= currentTagInPrepositionsInverseList->tagName;
		for(int i=1; i<=GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_NUMBER_OF_TAGS; i++)
		{
			if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_CONDITION)
			{
				if(currentTagInPrepositionsInverseListSentence->tagName != conditionName)
				{
					cerr << "GIApreprocessorMultiwordReductionClass::detectIfInverseOrTwoWayConditionRequired error{}: (currentTagInPrepositionsInverseListSentence->tagName != conditionName)" << endl;
					exit(EXIT_ERROR);
				}
			}
			else if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_REVERSE_CONDITION)
			{
				conditionNameInverted = currentTagInPrepositionsInverseListSentence->tagName;
				if(currentTagInPrepositionsInverseListSentence->tagName == conditionName)
				{
					*twoWayConditionRequired = true;
				}
				else
				{
					foundConditionToInvert = true;
				}
			}
			else if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID)
			{
				if(foundConditionToInvert)
				{
					if(currentTagInPrepositionsInverseListSentence->tagName == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID_VALUE_TRUE)
					{
						*inverseConditionRequired = true;
						*inverseConditionName = conditionNameInverted;
					}
				}
			}
			currentTagInPrepositionsInverseListSentence = static_cast<GIApreprocessorMultiwordReductionWord*>(currentTagInPrepositionsInverseListSentence->nextTag);
		}
	}
}

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DETECT_PREPOSITION_TYPE
bool GIApreprocessorMultiwordReductionClass::identifyConditionType(GIAentityNode* conditionRelationshipEntity)
{
	bool conditionTypeIdentified = false;

	unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* prepositionInverseList = &prepositionInverseListGlobal;

	//identify condition type
	GIApreprocessorMultiwordReductionBasicSentence* sentenceFound = NULL;
	if(findSentenceInSentenceListBasic(prepositionInverseList, conditionRelationshipEntity->entityName, &sentenceFound))
	{
		//the first tag in the sentence object corresponds to the preposition name
		
		GIApreprocessorMultiwordReductionWord* firstTagInPrepositionsInverseListSentence = sentenceFound->firstTagInSentence;
		GIApreprocessorMultiwordReductionWord* currentTagInPrepositionsInverseListSentence = firstTagInPrepositionsInverseListSentence;
		for(int i=1; i<=GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_NUMBER_OF_TAGS; i++)
		{
			if(i == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_TYPE)
			{
				conditionRelationshipEntity->conditionType2 = currentTagInPrepositionsInverseListSentence->tagName;
				conditionTypeIdentified = true;
			}
			currentTagInPrepositionsInverseListSentence = static_cast<GIApreprocessorMultiwordReductionWord*>(currentTagInPrepositionsInverseListSentence->nextTag);
		}
	}

	return conditionTypeIdentified;
}
#endif

#endif



bool GIApreprocessorMultiwordReductionClass::determineIsVerb(GIApreprocessorWord* wordTag, int* grammaticalBaseTenseForm)
{
	bool verbDetected = false;
	if(determineVerbCaseStandardWithAdditional(wordTag->tagName, grammaticalBaseTenseForm))
	{
		verbDetected = true;
	}
	
	GIApreprocessorMultiwordReductionPlainTextWord* wordTagPlaintext = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
	if(wordTagPlaintext->collapsedPhrasalVerbExactDefinedSection)
	{
		*grammaticalBaseTenseForm = wordTagPlaintext->grammaticalTenseFormDetected;	//already determined by GIApreprocessorMultiwordReductionClass::searchAndReplacePhrasalVerbs
		verbDetected = true;
	}
	if(wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_TYPE))
	{
		cout << "GIApreprocessorMultiwordReductionClass::determineIsVerb warning: !(wordTagPlaintext->collapsedPhrasalVerbExactDefinedSection) && (wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_TYPE))" << endl;
		*grammaticalBaseTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT;
		verbDetected = true;
	}
	
	return verbDetected;	
}

bool GIApreprocessorMultiwordReductionClass::determineVerbCaseStandardWithAdditional(const string word, int* grammaticalBaseTenseForm)
{
	bool foundVerbCaseStandardOrAdditional = false;
	
	bool foundVerbCaseStandard = determineVerbCaseStandard(word, grammaticalBaseTenseForm);
	if(foundVerbCaseStandard)
	{
		foundVerbCaseStandardOrAdditional = true;
	}
	
	int determineVerbCaseAdditionalTenseModifier = INT_DEFAULT_VALUE;
	string baseNameFound = "";	//not used
	bool foundVerbCaseAdditional = determineVerbCaseAdditional(word, &baseNameFound, &determineVerbCaseAdditionalTenseModifier);
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

bool GIApreprocessorMultiwordReductionClass::determineVerbCaseStandard(const string word, int* grammaticalBaseTenseForm)
{
	bool foundVerbCase = false;
	
	GIApreprocessorMultiwordReductionWord* wordFound = NULL;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseStandardList = &verbCaseStandardListGlobal;
	if(findWordInWordList(verbCaseStandardList, word, &wordFound))
	{
		foundVerbCase = true;
		*grammaticalBaseTenseForm = wordFound->grammaticalTenseForm;
	}
	
	return foundVerbCase;	
}

bool GIApreprocessorMultiwordReductionClass::determineVerbCaseAdditional(const string word, string* baseNameFound, int* grammaticalTenseModifier)
{
	bool foundVerbCase = false;
	
	GIApreprocessorMultiwordReductionWord* wordFound = NULL;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList = &verbCaseAdditionalListGlobal;
	if(findWordInWordList(verbCaseAdditionalList, word, &wordFound))
	{
		foundVerbCase = true;
		*grammaticalTenseModifier = wordFound->grammaticalTenseModifier;
		*baseNameFound = wordFound->baseName;
	}
	
	return foundVerbCase;	
}

bool GIApreprocessorMultiwordReductionClass::determineIsPreposition(GIApreprocessorWord* wordTag)
{
	bool prepositionDetected = false;
	if(determineIsPreposition(wordTag->tagName))
	{
		prepositionDetected = true;
	}
	GIApreprocessorMultiwordReductionPlainTextWord* wordTagPlaintext = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
	if(wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_TYPE))
	{
		prepositionDetected = true;
	}
	return prepositionDetected;
}
bool GIApreprocessorMultiwordReductionClass::determineIsPreposition(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* prepositionsList = &prepositionListGlobal;
	return findWordInWordList(prepositionsList, word);
}

bool GIApreprocessorMultiwordReductionClass::determineIsAdverb(GIApreprocessorWord* wordTag)
{
	bool adverbDetected = false;
	if(determineIsAdverb(wordTag->tagName))
	{
		adverbDetected = true;
	}
	GIApreprocessorMultiwordReductionPlainTextWord* wordTagPlaintext = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
	if(wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADVERB_TYPE))
	{
		adverbDetected = true;
	}
	return adverbDetected;
}
bool GIApreprocessorMultiwordReductionClass::determineIsAdverb(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* adverbList = &adverbListGlobal;
	return findWordInWordList(adverbList, word);
}

bool GIApreprocessorMultiwordReductionClass::determineIsAdjective(GIApreprocessorWord* wordTag)
{
	bool adjectiveDetected = false;
	if(determineIsAdjective(wordTag->tagName))
	{
		adjectiveDetected = true;
	}
	GIApreprocessorMultiwordReductionPlainTextWord* wordTagPlaintext = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
	if(wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADJECTIVE_TYPE))
	{
		adjectiveDetected = true;
	}
	return adjectiveDetected;
}
bool GIApreprocessorMultiwordReductionClass::determineIsAdjective(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* adjectiveList = &adjectiveListGlobal;
	return findWordInWordList(adjectiveList, word);
}

bool GIApreprocessorMultiwordReductionClass::determineIsNoun(GIApreprocessorWord* wordTag)
{
	bool nounDetected = false;
	if(determineIsNoun(wordTag->tagName))
	{
		nounDetected = true;
	}
	GIApreprocessorMultiwordReductionPlainTextWord* wordTagPlaintext = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
	if(wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_NOUN_TYPE))
	{
		nounDetected = true;
	}
	return nounDetected;
}
bool GIApreprocessorMultiwordReductionClass::determineIsNoun(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounList = &nounListGlobal;
	return findWordInWordList(nounList, word);
}

bool GIApreprocessorMultiwordReductionClass::findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word)
{	
	bool result = false;
	
	GIApreprocessorMultiwordReductionWord* wordFound = NULL;
	if(findWordInWordList(wordList, word, &wordFound))
	{
		result = true;
	}
	
	return result;
}	
bool GIApreprocessorMultiwordReductionClass::findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word, GIApreprocessorMultiwordReductionWord** wordFound)
{	
	bool result = false;
		
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator it;
	it = wordList->find(word);
	if(it != wordList->end())
	{
		result = true;
		*wordFound = it->second;
	}
	
	return result;
}	


bool GIApreprocessorMultiwordReductionClass::findSentenceInSentenceListBasic(unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionBasicSentence** sentenceFound)
{	
	bool result = false;
	
	unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>::iterator it;
	it = sentenceList->find(word);
	if(it != sentenceList->end())
	{
		result = true;
		*sentenceFound = it->second;
	}
	
	return result;
}

bool GIApreprocessorMultiwordReductionClass::findSentenceInSentenceListIrregularVerb(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionIrregularVerbSentence** sentenceFound)
{	
	bool result = false;
	
	unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>::iterator it;
	it = sentenceList->find(word);
	if(it != sentenceList->end())
	{
		result = true;
		*sentenceFound = it->second;
	}
	
	return result;
}




/*
//FUTURE GIA;
#ifdef GIA_NLG
#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
bool GIApreprocessorMultiwordReductionClass::generateVerbCase(const string baseName, string* wordGrammatised, int grammaticalTenseForm, int version)
{
	bool foundVerbCase = false;
	
	GIApreprocessorMultiwordReductionWord* verbList = verbListGlobal;
	GIApreprocessorMultiwordReductionWord* currentTagInVerbList = verbList;
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


