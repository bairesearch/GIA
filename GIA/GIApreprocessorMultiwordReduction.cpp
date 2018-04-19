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
 * File Name: GIApreprocessorMultiwordReduction.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f4a 14-April-2018
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 * /
 *******************************************************************************/


#include "GIApreprocessorMultiwordReduction.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
#include "GIAsynRelTranslatorDefs.hpp"
#endif


bool lrpInitialised = false;
static string lrpDataFolderName;
static bool useLRP;

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*> prepositionInverseListGlobal;
bool prepositionInverseListLoaded;
#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
bool wordListsLoaded;
unordered_map<string, unordered_map<string, GIApreprocessorMultiwordReductionWord*>*> wordListsGlobal;	//string: preprocessorPOStype
unordered_map<string, GIApreprocessorMultiwordReductionWord*> verbCaseStandardListGlobal;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> verbCaseAdditionalListGlobal;
unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*> irregularVerbListGlobal;
bool irregularVerbListLoaded;
#ifdef GIA_PREPROCESSOR_DERIVE_NOUN_VARIANTS
unordered_map<string, GIApreprocessorMultiwordReductionWord*> nounPluralVariantsListGlobal;
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_AUXILIARY_LISTS_TO_VERB_LISTS
unordered_map<string, GIApreprocessorMultiwordReductionWord*> wordListRecordAuxiliaryBeing;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> wordListRecordAuxiliaryHaving;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> wordListRecordAuxiliaryDoing;
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_PREDETERMINER_LIST_TO_DETERMINER_LIST
unordered_map<string, GIApreprocessorMultiwordReductionWord*> wordListRecordAuxiliaryPredeterminer;
#endif
#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
unordered_map<string, GIApreprocessorMultiwordReductionWord*> verbListWithVariantsGlobal;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> nounListWithVariantsGlobal;
#endif
#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*> phrasalVerbListGlobal;
unordered_map<string, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>*> multiwordListsGlobal;	//string: preprocessorPOStype
#endif


		

unordered_map<string, GIApreprocessorMultiwordReductionWord*>* GIApreprocessorMultiwordReductionClass::getWordList(int GIAposType)
{
	bool result = false;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = getWordListIfExistent(GIAposType, &result);
	if(!result)
	{
		if(GIAposType < GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES)
		{
			string GIAposTypeName = GIApreprocessorPOStypeNameArray[GIAposType];
			cerr << "GIApreprocessorMultiwordReductionClass::getWordList: cannot locate word list in wordListsGlobal, GIAposTypeName = " << GIAposTypeName << endl;
		}
		else
		{
			cerr << "GIApreprocessorMultiwordReductionClass::getWordList: cannot locate word list in wordListsGlobal, GIAposType = " << GIAposType << endl;		
		}
		exit(EXIT_ERROR);		
	}
	return wordList;
}
unordered_map<string, GIApreprocessorMultiwordReductionWord*>* GIApreprocessorMultiwordReductionClass::getWordListIfExistent(int GIAposType, bool* result)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList;
	string GIAposTypeName = GIApreprocessorPOStypeNameArray[GIAposType];
	unordered_map<string, unordered_map<string, GIApreprocessorMultiwordReductionWord*>*>::iterator wordListsIter = wordListsGlobal.find(GIAposTypeName); 
	if(wordListsIter != wordListsGlobal.end())
	{
		wordList = wordListsIter->second;
		*result = true;
	}
	else
	{
		*result = false;		
	}
	return wordList;
}
bool GIApreprocessorMultiwordReductionClass::wordListExistent(int GIAposType)
{
	bool result = false;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = getWordListIfExistent(GIAposType, &result);
	return result;
}
bool GIApreprocessorMultiwordReductionClass::transferWordList(int GIAposType1, int GIAposType2, unordered_map<string, GIApreprocessorMultiwordReductionWord*>** wordList1record)
{
	bool result = true;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList1 = getWordList(GIAposType1);
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList2 = getWordList(GIAposType2);
	wordList2->insert(wordList1->begin(), wordList1->end());
	
	string GIAposTypeName = GIApreprocessorPOStypeNameArray[GIAposType1];
	unordered_map<string, unordered_map<string, GIApreprocessorMultiwordReductionWord*>*>::iterator wordListsIter = wordListsGlobal.find(GIAposTypeName); 
	wordListsGlobal.erase(wordListsIter);
	
	*wordList1record = wordList1;
	
	return result;
}





#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
bool wordListAllTypesWithPOSambiguityInfoLoaded;
unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>> wordListAllTypesWithPOSambiguityInfo;		//NB the int corresponds to the POS type ambiguity of the word (binary 11000000 implies that the word may either be a verb or a preposition)	
unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>>* GIApreprocessorMultiwordReductionClass::getWordListAllTypesWithPOSambiguityInfo()
{
	return &wordListAllTypesWithPOSambiguityInfo;
}
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ASSIGN_WEIGHTS_TO_TRAINED_POS_TYPES
vector<double> GIApreprocessorPOStypeWeights;
double GIApreprocessorMultiwordReductionClass::getGIApreprocessorPOStypeWeight(int GIAposType)
{
	return GIApreprocessorPOStypeWeights[GIAposType];
}
#endif
/*
bool GIApreprocessorMultiwordReductionClass::findInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, unsigned long* POSambiguityInfo)
{
	bool result = false;
	
	unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>>::iterator it;
	it = mapWordListAllTypesWithPOSambiguityInfo->find(wordIndex);
	if(it != mapWordListAllTypesWithPOSambiguityInfo->end())
	{
		result = true;
		word = (it->second).first;
		*POSambiguityInfo = (it->second).second;
	}
	
	return result;
}
*/
void GIApreprocessorMultiwordReductionClass::insertInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, const unsigned long POSambiguityInfo)
{
	pair<GIApreprocessorMultiwordReductionWord*, unsigned long> value = make_pair(word, POSambiguityInfo);
	mapWordListAllTypesWithPOSambiguityInfo->insert(pair<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>>(wordIndex, value));
}
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
		string currentFolder = SHAREDvarsClass().getCurrentDirectory(); 
		SHAREDvarsClass().setCurrentDirectory(lrpDataFolderName); 
		
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
		prepositionInverseListLoaded = false;
		if(!loadPrepositionsInverseList(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_FILE_NAME, &prepositionInverseListGlobal))
		{
			cout << "!loadPrepositionsInverseList (GIA with GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS requires -lrpfolder to be set): prepositionsInverseListFileName = " << GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_FILE_NAME << endl;
			result = false;
		}
		else
		{
			prepositionInverseListLoaded = true;
		}
		#endif

		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
		wordListsLoaded = false;
		
		int numberOfWordLists;
		vector<string> wordListFileNames;
		if(!SHAREDvars.getLinesFromFile(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_WORDLISTS_DATABASE_FILE_NAME, &wordListFileNames, &numberOfWordLists))
		{
			result = false;
		}
		for(int i=0; i<numberOfWordLists; i++)
		{
			if(wordListFileNames[i] != "")
			{
				int indexOfSpace = wordListFileNames[i].find(CHAR_SPACE);
				if(indexOfSpace != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					string preprocessorPOStypeName = wordListFileNames[i].substr(0, indexOfSpace);
					string wordListFileName = wordListFileNames[i].substr(indexOfSpace+1);
					//cout << "preprocessorPOStypeName = " << preprocessorPOStypeName << endl;
					//cout << "wordListFileName = " << wordListFileName << endl;

					string dummyWordForWordListPosType = "";
					bool wordListLoaded = false;	//NOTUSED
					unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = new unordered_map<string, GIApreprocessorMultiwordReductionWord*>;
					if(!loadWordListWrapper(&wordListLoaded, wordListFileName, wordList))
					{
						result = false;
					}
					wordListsGlobal.insert(pair<string, unordered_map<string, GIApreprocessorMultiwordReductionWord*>*>(preprocessorPOStypeName, wordList));
					
					wordListsLoaded = true;
				}
				else
				{
					cerr << "GIApreprocessorMultiwordReductionClass::initialiseLRP{} error: cannot find space character in wordListFileNames[i], i = " << i << endl;
					exit(EXIT_ERROR);
				}
			}
			else
			{
				//skip blank lines
			}
		}
		
	
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_AUXILIARY_LISTS_TO_VERB_LISTS
		transferWordList(GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_BEING, GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_VERB, &wordListRecordAuxiliaryBeing);
		transferWordList(GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_HAVING, GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_VERB, &wordListRecordAuxiliaryHaving);
		transferWordList(GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_DOING, GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_VERB, &wordListRecordAuxiliaryDoing);
		#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_PREDETERMINER_LIST_TO_DETERMINER_LIST
		transferWordList(GIA_PREPROCESSOR_POS_TYPE_PREDETERMINER, GIA_PREPROCESSOR_POS_TYPE_DETERMINER, &wordListRecordPredeterminer);
		#endif
		
		irregularVerbListLoaded = false;
		if(!loadIrregularVerbList(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_IRREGULARVERB_DATABASE_FILE_NAME, &irregularVerbListGlobal))
		{
			cout << "!loadIrregularVerbList (GIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE requires -lrpfolder to be set): irregularVerbListFileName = " << GIA_PREPROCESSOR_MULTIWORD_REDUCTION_IRREGULARVERB_DATABASE_FILE_NAME << endl;
			result = false;
		}
		else
		{
			irregularVerbListLoaded = true;
		}


		if(!generateVerbCaseStandardAndAdditionalList(GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE))	//this is required to make verbList usable
		{
			result = false;
		}
		#ifdef GIA_PREPROCESSOR_DERIVE_NOUN_VARIANTS
		if(!generateNounPluralVariantsList())
		{
			result = false;
		}
		#endif
		#endif
		
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
		if(!loadPhrasalVerbDataAndGenerateAllTenseVariants(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_FILE_NAME, &phrasalVerbListGlobal, &irregularVerbListGlobal))
		{
			result = false;
		}	
	
		int numberOfMultiwordLists;
		vector<string> multiwordListFileNames;
		if(!SHAREDvars.getLinesFromFile(GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORDLISTS_DATABASE_FILE_NAME, &multiwordListFileNames, &numberOfMultiwordLists))
		{
			result = false;
		}
		for(int i=0; i<numberOfMultiwordLists; i++)
		{
			if(multiwordListFileNames[i] != "")
			{				
				int indexOfSpace = multiwordListFileNames[i].find(CHAR_SPACE);
				if(indexOfSpace != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					string preprocessorPOStypeName = multiwordListFileNames[i].substr(0, indexOfSpace);
					string multiwordListFileName = multiwordListFileNames[i].substr(indexOfSpace+1);
					//cout << "preprocessorPOStypeName = " << preprocessorPOStypeName << endl;
					//cout << "multiwordListFileName = " << multiwordListFileName << endl;
					
					multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordList = new multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>;
					if(!loadMultiwordWordList(multiwordListFileName, multiwordList))
					{
						result = false;
					}
					multiwordListsGlobal.insert(pair<string, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>*>(preprocessorPOStypeName, multiwordList));	
				}
				else
				{
					cerr << "GIApreprocessorMultiwordReductionClass::initialiseLRP{} error: cannot find space character in multiwordListFileNames[i], i = " << i << endl;
					exit(EXIT_ERROR);
				}
			}
			else
			{
				//skip blank lines
			}
		}
		#endif

		if(!result)
		{
			cout << "lrpDataFolderName = " << lrpDataFolderName << endl;
		}
		
		lrpInitialised = true;
		
		SHAREDvarsClass().setCurrentDirectory(currentFolder); 
	}
	
	return result;
}
bool GIApreprocessorMultiwordReductionClass::getUseLRP()
{
	return useLRP;
}

bool GIApreprocessorMultiwordReductionClass::loadWordListWrapper(bool* wordListLoaded, const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList)
{
	bool result = true;
	
	*wordListLoaded = false;
	if(!loadWordList(wordListFileName, wordList))
	{
		cout << "!loadWordList (GIA with wordListFileName requires -lrpfolder to be set): wordListFileName = " << wordListFileName << endl;
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
				//cout << "currentWord = " << currentWord << endl;
				#ifndef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES_SUPPORT_UPPERCASE_PROPERNOUN_WORD_LISTS
				currentWord = SHAREDvars.convertStringToLowerCase(&(currentWord));
				#endif
				currentTagInWordList->tagName = currentWord;
				wordList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(currentWord, currentTagInWordList));
				//*dummyWordForWordListPosType = currentWord;	//OLD: set dummyWordForWordListPosType to last word in list (e.g. z..)
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

bool GIApreprocessorMultiwordReductionClass::loadIrregularVerbList(const string irregularVerbListFileName, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList)
{
	bool result = true;

	//irregular verb list row format: infinitive past[/alt/alt/..] pastParticiple[/alt/alt/..] continuous
	
	GIApreprocessorMultiwordReductionIrregularVerbWord* firstTagInIrregularVerb = new GIApreprocessorMultiwordReductionIrregularVerbWord();
	GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInIrregularVerb = firstTagInIrregularVerb;
	GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				
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
		int alternateWordIndex = 0;
		string currentWord = "";
		while(parseFileObject.get(currentToken))
		{
			if((currentToken == CHAR_NEWLINE) || (currentToken == CHAR_SPACE))
			{
				//create "LRP tag" to store phrasal verb base or past/past participle tense variant
				currentTagInAlternateList->tagName = currentWord;
				currentTagInAlternateList->alternateTag = new GIApreprocessorMultiwordReductionIrregularVerbWord();
				//cout << "currentWord = " << currentWord << endl;
					
				currentTagInIrregularVerb->nextTag = new GIApreprocessorMultiwordReductionIrregularVerbWord();
				currentTagInIrregularVerb = static_cast<GIApreprocessorMultiwordReductionIrregularVerbWord*>(currentTagInIrregularVerb->nextTag);
				currentTagInAlternateList = currentTagInIrregularVerb;

				if(currentToken == CHAR_NEWLINE)
				{
					GIApreprocessorMultiwordReductionIrregularVerbSentence* sentence = new GIApreprocessorMultiwordReductionIrregularVerbSentence();
					sentence->firstTagInSentence = firstTagInIrregularVerb;
					irregularVerbList->insert(pair<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>(firstTagInIrregularVerb->tagName, sentence));
					firstTagInIrregularVerb = new GIApreprocessorMultiwordReductionIrregularVerbWord();
					currentTagInIrregularVerb = firstTagInIrregularVerb;
					currentTagInAlternateList = currentTagInIrregularVerb;
					wordIndex = 0;
				}
				else
				{
					wordIndex++;
				}
				
				alternateWordIndex = 0;
				currentWord = "";

				//start parsing new line
			}
			else if(currentToken == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ALTERNATE)
			{
				currentTagInAlternateList->tagName = currentWord;
				currentTagInAlternateList->alternateTag = new GIApreprocessorMultiwordReductionIrregularVerbWord();
				currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				//cout << "currentWord = " << currentWord << endl;
				
				//wordIndex does not change...
				currentWord = "";
				alternateWordIndex++;
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
						bool irregularVerbFound = generateIrregularTenseVariantsOfVerbBase(currentTagInPhrasalVerb, irregularVerbList, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);
						generateStandardTenseVariantsOfVerbBase(currentTagInPhrasalVerb, irregularVerbFound, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);
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
							for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
							{
								for(int j=0; j<firstTagInPhrasalVerb->grammaticalTenseFormsArray[i].size(); j++)
								{
									string verbVariant = (firstTagInPhrasalVerb->grammaticalTenseFormsArray[i])[j];
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
					bool irregularVerbFound = generateIrregularTenseVariantsOfVerbBase(currentTagInPhrasalVerb, irregularVerbList, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);
					generateStandardTenseVariantsOfVerbBase(currentTagInPhrasalVerb, irregularVerbFound, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);								
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







#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
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

	string currentFolder = SHAREDvars.getCurrentDirectory();

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_NUMERICAL_WORDS_TO_NUMBERS
	if(!replaceAllNumericalWordsWithNumbers(firstGIApreprocessorSentenceInList))
	{
		result = false;
	}
	#endif

	//cout << "searchAndReplacePhrasalVerbs" << endl;
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo = getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	if(!searchAndReplacePhrasalVerbs(firstGIApreprocessorSentenceInList, &phrasalVerbListGlobal, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo))
	{
		result = false;
	}

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_ORIGINAL
	for(unordered_map<string, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>*>::iterator multiwordListsIter = multiwordListsGlobal.begin(); multiwordListsIter != multiwordListsGlobal.end(); multiwordListsIter++)
	{
		string multiwordListName = multiwordListsIter->first;
		multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordList = multiwordListsIter->second;
		int multiwordListType = GIApreprocessorMultiwordReductionClassObject.getPOStypeFromName(multiwordListName);
		if(!searchAndReplaceMultiwordWordList(firstGIApreprocessorSentenceInList, multiwordList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, multiwordListType))
		{
			result = false;
		}
	}
	#endif		

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DYNAMIC
	for(unordered_map<string, unordered_map<string, GIApreprocessorMultiwordReductionWord*>*>::iterator wordListsIter = wordListsGlobal.begin(); wordListsIter != wordListsGlobal.end(); wordListsIter++)
	{
		string wordListName = wordListsIter->first;
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = wordListsIter->second;
		int wordListType = GIApreprocessorMultiwordReductionClassObject.getPOStypeFromName(wordListName);
		if(!searchAndReplaceMultiwordWordListDynamic(firstGIApreprocessorSentenceInList, wordList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, wordListType))
		{
			result = false;
		}			
	}
	#endif
	
	SHAREDvars.setCurrentDirectory(outputFolder);
	if(!writeTagListToFile(firstGIApreprocessorSentenceInList, plainTextLRPoutputFileName, plainTextLRPforNLPoutputFileName, true, true))
	{
		result = false;
	}
	SHAREDvars.setCurrentDirectory(currentFolder);
	
	return result;
}





	

//NB the collapsed phrasal verb contains precisely 2 entities: phrasalVerbCollapsed, entity2: thing/place/body (eg belong_to + sb/Tom) - thing/place/bodies are not currently being differentiated by the LRP as this information is only first generated at NLP/GIA parse stage
bool GIApreprocessorMultiwordReductionClass::searchAndReplacePhrasalVerbs(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo)
{
	bool result = true;

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;	//new correspondence info for each found phrasal verb
	
	//not required because searchAndReplacePhrasalVerbs is called first by parseTextFileAndReduceLanguage:
	while(currentCorrespondenceInfo->next != NULL)
	{
		currentCorrespondenceInfo = currentCorrespondenceInfo->next;	//added 3f1a
	}
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		//cout << "(currentGIApreprocessorSentenceInList->sentenceContentsLRP).size() = " << (currentGIApreprocessorSentenceInList->sentenceContentsLRP).size() << endl;
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
					bool foundPhrasalVerbInSentenceAndCollapsed = false;
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
									for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
									{
										for(int j=0; j<currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[i].size(); j++)
										{
											if((currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[i])[j] == currentTagInPlainTextSentenceTemp->tagName)		//USED TO BE currentTagInCollapsedPhrasalVerb before 6 Sept 2012
											{
												currentTagInCollapsedPhrasalVerb->grammaticalTenseFormDetected = i;								//USED TO BE AND STILL IS currentTagInCollapsedPhrasalVerb before 6 Sept 2012
												generatedTagNameLemma = currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][0];
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
								currentTagInCollapsedPhrasalVerb->collapsedMultiwordWordType = GIA_PREPROCESSOR_POS_TYPE_VERB;	//added 3f1a
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
									if(currentTagInPlainTextSentenceTemp2->preprocessorUpperLevelWordReference != NULL)	//added 3d8a
									{
										currentTagInPlainTextSentenceTemp2->preprocessorUpperLevelWordReference->preprocessorLowerLevelWordReference = firstTagInCollapsedPhrasalVerb;
									}
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
								foundPhrasalVerbInSentenceAndCollapsed = true;
							}
						}
					}
					if(!foundPhrasalVerbInSentenceAndCollapsed)
					{
						delete firstTagInCollapsedPhrasalVerb;
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
						
						currentTagInPlainTextSentence = currentTagInPlainTextSentenceTemp2;	//added 3d8a
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

bool GIApreprocessorMultiwordReductionClass::searchAndReplaceMultiwordWordList(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
{
	bool result = true;
	
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;	//new correspondence info for each found multiword word
	while(currentCorrespondenceInfo->next != NULL)
	{
		currentCorrespondenceInfo = currentCorrespondenceInfo->next;	//added 2j6d (add to end of list)
	}
		
	int sentenceIndexTemp = 0;
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		//cout << "sentenceIndexTemp = " << sentenceIndexTemp << endl;
	
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
		
				bool foundMultiwordWordInSentenceAndCollapsed = false;
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
							if(currentTagInPlainTextSentenceTemp2->preprocessorUpperLevelWordReference != NULL)	//added 3d8a
							{
								currentTagInPlainTextSentenceTemp2->preprocessorUpperLevelWordReference->preprocessorLowerLevelWordReference = firstTagInCollapsedMultiwordWord;
							}
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
						foundMultiwordWordInSentenceAndCollapsed = true;
						
					}
				}
				if(!foundMultiwordWordInSentenceAndCollapsed)
				{
					delete firstTagInCollapsedMultiwordWord;
				}
			}
			if(foundAtLeastOneMultiwordWordInSentenceAndCollapsed)
			{
				createNewCorrespondenceInfo(firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, &currentCorrespondenceInfo, currentGIApreprocessorSentenceInList, firstTagInPlainTextSentence, &currentTagInPlainTextSentence, entityIndex, numberWordsInMultiwordMatched);
			}

			entityIndex++;
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentence->nextTag);
		}
		
		GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstTagInPlainTextSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsLRP));

		sentenceIndexTemp++;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	return result;
}

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DYNAMIC
bool GIApreprocessorMultiwordReductionClass::searchAndReplaceMultiwordWordListDynamic(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
{
	bool result = true;
	
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;	//new correspondence info for each found multiword word
	while(currentCorrespondenceInfo->next != NULL)
	{
		currentCorrespondenceInfo = currentCorrespondenceInfo->next;
	}
		
	int sentenceIndexTemp = 0;
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		//cout << "sentenceIndexTemp = " << sentenceIndexTemp << endl;
	
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		GIApreprocessorMultiwordReductionClassObject.generateFlatSentenceWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), &firstTagInPlainTextSentence);
		GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence = firstTagInPlainTextSentence;
		GIApreprocessorMultiwordReductionPlainTextWord* previousTagInPlainTextSentence = NULL;

		int entityIndex = GIA_NLP_START_ENTITY_INDEX;
		while(currentTagInPlainTextSentence->nextTag != NULL)
		{			
			if(currentTagInPlainTextSentence->nextTag->nextTag != NULL)
			{			
				bool foundConsecutiveWordsOfSameListType = false;
				int numberWordsInMultiwordMatched = 0;

				unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator wordListIter = wordList->find(currentTagInPlainTextSentence->tagName);
				if(wordListIter != wordList->end())
				{
					unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator wordListIter2 = wordList->find(currentTagInPlainTextSentence->nextTag->tagName);
					if(wordListIter2 != wordList->end())
					{
						foundConsecutiveWordsOfSameListType = true;
						numberWordsInMultiwordMatched = 2;
					}
				}
	
				if(foundConsecutiveWordsOfSameListType)
				{							
					GIApreprocessorMultiwordReductionPlainTextWord* firstTagInCollapsedMultiwordWord = currentTagInPlainTextSentence;
										
					firstTagInCollapsedMultiwordWord->collapsedMultiwordWord = true;
					firstTagInCollapsedMultiwordWord->collapsedMultiwordWordTemp = false;
					firstTagInCollapsedMultiwordWord->collapsedMultiwordWordType = wordListType;
					firstTagInCollapsedMultiwordWord->tagName = firstTagInCollapsedMultiwordWord->tagName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER + currentTagInPlainTextSentence->nextTag->tagName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER;

					#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
					firstTagInCollapsedMultiwordWord->preprocessorUpperLevelWordReferenceSize = numberWordsInMultiwordMatched;
					#endif
					
					firstTagInCollapsedMultiwordWord->nextTag =  currentTagInPlainTextSentence->nextTag->nextTag;	//remove currentTagInPlainTextSentence->nextTag from list
					
					createNewCorrespondenceInfo(firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, &currentCorrespondenceInfo, currentGIApreprocessorSentenceInList, firstTagInPlainTextSentence, &currentTagInPlainTextSentence, entityIndex, numberWordsInMultiwordMatched);
				}
			}
			
			entityIndex++;
			previousTagInPlainTextSentence = currentTagInPlainTextSentence;
			currentTagInPlainTextSentence = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentence->nextTag);
		}
		
		GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstTagInPlainTextSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsLRP));

		sentenceIndexTemp++;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	return result;
}
#endif



void GIApreprocessorMultiwordReductionClass::createNewCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo** currentCorrespondenceInfo, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence, GIApreprocessorMultiwordReductionPlainTextWord** currentTagInPlainTextSentence, int entityIndex, int numberWordsInMultiwordMatched)
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
			(*currentCorrespondenceInfo)->sentenceIndex = currentGIApreprocessorSentenceInList->sentenceIndexOriginal;
			(*currentCorrespondenceInfo)->entityIndex = newEntityIndex;	//this is not currently used for LRP collapsed multiword word
			//#ifdef GIA_SEM_REL_TRANSLATOR
			(*currentCorrespondenceInfo)->lemmaWithLRP = currentTagInPlainTextSentenceTemp2->tagName;	//added 2j6c	//required for GIA2 only?
			//#endif
			(*currentCorrespondenceInfo)->wordWithLRP = currentTagInPlainTextSentenceTemp2->tagName;
			(*currentCorrespondenceInfo)->wordWithLRPforNLPonly = generateWordWithLRPforNLPonly(currentTagInPlainTextSentenceTemp2); //lrpDummyCollapsedMultiwordWordLemmaNameForNLPArray[collapsedMultiwordWordIndex];
			//collapsedMultiwordWordIndex++;

			(*currentCorrespondenceInfo)->next = new GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
			(*currentCorrespondenceInfo) = (*currentCorrespondenceInfo)->next;

			*currentTagInPlainTextSentence = currentTagInPlainTextSentenceTemp2;	//added 3d8a
		}
		currentTagInPlainTextSentenceTemp2->entityIndex = newEntityIndex;
		currentTagInPlainTextSentenceTemp2 = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentTagInPlainTextSentenceTemp2->nextTag);
		newEntityIndex++;
	}
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
				//cout << "currentTagInPlainTextSentence->collapsedMultiwordWord = " << currentTagInPlainTextSentence->collapsedMultiwordWord << endl;
				plainTextLRPforNLPOutputTag = generateWordWithLRPforNLPonly(currentTagInPlainTextSentence);
				/*
				if((currentTagInPlainTextSentence->collapsedPhrasalVerbExactDefinedSection) || (currentTagInPlainTextSentence->collapsedMultiwordWord))
				{
					plainTextLRPforNLPOutputTag = generateWordWithLRPforNLPonly(currentTagInPlainTextSentence);
				}
				else
				{
					plainTextLRPforNLPOutputTag = currentTagInPlainTextSentence->tagName;
				}
				*/
				
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
		wordWithLRPforNLPonly = GIApreprocessorPOStypeMultiwordReplacementArray[currentTagInPlainTextSentence->collapsedMultiwordWordType];
	}
	/*
	else
	{
		cout << "GIApreprocessorMultiwordReductionClass::generateWordWithLRPforNLPonly{} warning: !(currentTagInPlainTextSentence->collapsedPhrasalVerbExactDefinedSection) && !(currentTagInPlainTextSentence->collapsedMultiwordWord)" << endl;
	}
	*/
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

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo = getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();

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
#endif



#ifdef GIA_PREPROCESSOR_DERIVE_NOUN_VARIANTS
bool GIApreprocessorMultiwordReductionClass::generateNounPluralVariantsList()
{
	//aka parseVerbDataGenerateAllTenseVariants
	
	bool result = true;
	if(!wordListsLoaded)
	{
		cerr << "GIApreprocessorMultiwordReductionClass::generateNounPluralVariantsList: (!nounListLoaded)" << endl;
		exit(EXIT_ERROR);
	}
	else
	{		
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounList = getWordList(GIA_PREPROCESSOR_POS_TYPE_NOUN);
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounPluralVariantsList = &nounPluralVariantsListGlobal;
		for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator currentTagInNounListIterator = nounList->begin(); currentTagInNounListIterator != nounList->end(); ++currentTagInNounListIterator)
		{	
			GIApreprocessorMultiwordReductionWord* currentTagInNounList = currentTagInNounListIterator->second;	
			generateNounPluralVariants(currentTagInNounList, nounPluralVariantsList);
		}
	}
	return result;
}

bool GIApreprocessorMultiwordReductionClass::generateNounPluralVariants(GIApreprocessorMultiwordReductionWord* wordTag, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounPluralVariantsList)
{
	//vector<string>* nounPluralVariants = wordTag->nounPluralVariants;
	string word = wordTag->tagName;
	
	for(int i=0; i<GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_ADDITION_DEFAULT_NUMBER_OF_TYPES; i++)
	{
		string stringToAddOrReplace = translatorEnglishNounPluralModifierAdditionDefaultArray[i];
		string nounPluralVariant = word + stringToAddOrReplace;
		//nounPluralVariants->push_back(nounPluralVariant);
		nounPluralVariantsList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(nounPluralVariant, wordTag));
		//cout << "nounPluralVariant = " << nounPluralVariant << endl;
	}
			
	for(int i=0; i<GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_ADDITION_NUMBER_OF_TYPES; i++)
	{
		string stringToFind = translatorEnglishNounPluralModifierAdditionArray[i][0];
		//find string at end of word
		if(word.find(stringToFind, word.length()-stringToFind.length()) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			string stringToAddOrReplace = translatorEnglishNounPluralModifierAdditionArray[i][1];
			string nounPluralVariant = word + stringToAddOrReplace;
			//nounPluralVariants->push_back(nounPluralVariant);
			nounPluralVariantsList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(nounPluralVariant, wordTag));
			//cout << "nounPluralVariant = " << nounPluralVariant << endl;
		}
	}

	for(int i=0; i<GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_REPLACEMENT_SUCCEEDING_CONSONANT_NUMBER_OF_TYPES; i++)
	{
		string stringToFind = translatorEnglishNounPluralModifierReplacementSucceedingConsonantArray[i][0];
		if(word.length()-stringToFind.length() >= 1)
		{
			char hypotheticalConsonantChar = word.length()-stringToFind.length()-1;
			if(SHAREDvars.charInCharArray(hypotheticalConsonantChar, translatorEnglishConsonants, GIA_TRANSLATOR_ENGLISH_CONSONANTS_NUMBER_OF_TYPES))
			{
				//find string at end of word
				if(word.find(stringToFind, word.length()-stringToFind.length()) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					string stringToAddOrReplace = translatorEnglishNounPluralModifierReplacementSucceedingConsonantArray[i][1];
					string nounPluralVariant = word.substr(0, word.length()-stringToFind.length());
					nounPluralVariant = nounPluralVariant + stringToAddOrReplace;
					//nounPluralVariants->push_back(nounPluralVariant);
					nounPluralVariantsList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(nounPluralVariant, wordTag));
					//cout << "nounPluralVariant = " << nounPluralVariant << endl;
				}
			}
		}
	}
	
	for(int i=0; i<GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_REPLACEMENT_NUMBER_OF_TYPES; i++)
	{
		string stringToFind = translatorEnglishNounPluralModifierReplacementArray[i][0];
		//find string at end of word
		if(word.find(stringToFind, word.length()-stringToFind.length()) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			string stringToAddOrReplace = translatorEnglishNounPluralModifierReplacementArray[i][1];
			string nounPluralVariant = word.substr(0, word.length()-stringToFind.length());
			nounPluralVariant = nounPluralVariant + stringToAddOrReplace;
			//nounPluralVariant->push_back(nounPluralVariant);
			nounPluralVariantsList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(nounPluralVariant, wordTag));
			//cout << "nounPluralVariant = " << nounPluralVariant << endl;
		}
	}
}

#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
//grammaticallyStrict is by default set to false: this function was developed for special verb cases e.g. continuous (these verb cases are formally nouns; but are interpreted semantically as actions)
bool GIApreprocessorMultiwordReductionClass::generateVerbCaseStandardAndAdditionalList(bool grammaticallyStrict)
{
	//aka parseVerbDataGenerateAllTenseVariants
	
	bool result = true;
	if(!wordListsLoaded || !irregularVerbListLoaded)
	{
		cout << "GIApreprocessorMultiwordReductionClass::generateVerbCaseStandardAndAdditionalList: (!wordListsLoaded || !irregularVerbListLoaded)" << endl;
		exit(EXIT_ERROR);
	}
	else
	{		
		//get global variables
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbList = getWordList(GIA_PREPROCESSOR_POS_TYPE_VERB);
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseStandardList = &verbCaseStandardListGlobal;
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList = &verbCaseAdditionalListGlobal;
		unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList = &irregularVerbListGlobal;
		
		for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator currentTagInVerbListIterator = verbList->begin(); currentTagInVerbListIterator != verbList->end(); ++currentTagInVerbListIterator)
		{	
			GIApreprocessorMultiwordReductionWord* currentTagInVerbList = currentTagInVerbListIterator->second;	

			/*
			currentTagInVerbList->grammaticalTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE;	//added 3d8a
			verbCaseStandardList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(currentTagInVerbList->tagName, currentTagInVerbList));
			*/
			
			bool irregularVerbFound = generateIrregularTenseVariantsOfVerbBase(currentTagInVerbList, irregularVerbList, grammaticallyStrict);

			//for each variant of the firstTagInPhrasalVerb, insert the sentence into the list keyed by the variant name
			generateStandardTenseVariantsOfVerbBase(currentTagInVerbList, irregularVerbFound, grammaticallyStrict);

			for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
			{
				for(int j=0; j<currentTagInVerbList->grammaticalTenseFormsArray[i].size(); j++)
				{

					//added 3d8a;
					GIApreprocessorMultiwordReductionWord* newWord = new GIApreprocessorMultiwordReductionWord();
					newWord->grammaticalTenseForm = i;
					newWord->baseName = currentTagInVerbList->tagName;
	
					string generatedTagNameLemma = (currentTagInVerbList->grammaticalTenseFormsArray[i])[j];
					
					verbCaseStandardList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(generatedTagNameLemma, newWord));
				}
			}
			
			generateAdditionalTenseVariantsOfVerbBase(verbCaseAdditionalList, currentTagInVerbList, irregularVerbFound, grammaticallyStrict);
		}
	}
	return result;
}

#endif	

bool GIApreprocessorMultiwordReductionClass::generateIrregularTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList, bool grammaticallyStrict)
{
	bool irregularVerbFound = false;
	
	string base = SHAREDvars.convertStringToLowerCase(&(baseTag->tagName));

	GIApreprocessorMultiwordReductionIrregularVerbSentence* sentenceFound = NULL;
	if(findSentenceInSentenceListIrregularVerb(irregularVerbList, base, &sentenceFound))
	{
		irregularVerbFound = true;
		
		//cout << "irregularVerbFound" << endl;
		//cout << "base = " << base << endl;
					
		GIApreprocessorMultiwordReductionIrregularVerbWord* firstTagInIrregularVerb = sentenceFound->firstTagInSentence;
		GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInIrregularVerb = static_cast<GIApreprocessorMultiwordReductionIrregularVerbWord*>(firstTagInIrregularVerb);
		int irregularVerbTagIndex = 0;
		while(currentTagInIrregularVerb->nextTag != NULL)
		{
			//second index is past, third is past participle
			int verbTenseForm = GIApreprocessorMultiwordReductionIrregularVerbColumnCrossReferenceVerbDatabaseBaseTenseForm[irregularVerbTagIndex];

			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
			if(verbTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "1 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}	
			}
			else if(verbTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "2 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}	
			} else
			#endif
			if(verbTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "3 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}	
			}
			else if(verbTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "4 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}
			}

			irregularVerbTagIndex++;
			currentTagInIrregularVerb = static_cast<GIApreprocessorMultiwordReductionIrregularVerbWord*>(currentTagInIrregularVerb->nextTag);
		}
	}
	
	return irregularVerbFound;
}

bool GIApreprocessorMultiwordReductionClass::generateStandardTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, bool irregularVerbFound, bool grammaticallyStrict)
{
	bool result = true;

	//NB grammaticallyStrict: do not interpret special cases (e.g. continuous) as verbs; as these are stricly nouns 
	
	string base = SHAREDvars.convertStringToLowerCase(&(baseTag->tagName));

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

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
	if(!irregularVerbFound)
	{
	#endif
		//generate (ie record) infinitive tense form [for consistency; even though this is the same as tagName...]
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE].push_back(base);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
	}
	#endif
		
	//Rule 1: Words ending with a Consonant-Vowel-Consonant Pattern
	if(rule1)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);
		if(rule1b)
		{
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);
		}

		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			if(!grammaticallyStrict)
			{
			#endif
				//b. generate continuous tense form
				//happening/entering
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
				if(rule1b)
				{
					//hopping/sitting
					//beginning/permitting
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + lastCharacterInBase + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);	//double consonant
				}
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//visited/opened
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
			if(rule1b)
			{
				//rubbed/stopped
				//referred/admitted
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(base + lastCharacterInBase + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);	//double consonant
			}

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
			if(rule1b)
			{
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(base + lastCharacterInBase + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);	//double consonant
			}
		}
	}
	//Rule 2: Words ending in E
	else if(rule2)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);

		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			if(!grammaticallyStrict)
			{
			#endif
				//b. generate continuous tense form
				//dancing/skating
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//smiled/fined
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
		}
	}
	//Rule 3: Words ending in Y
	else if(rule3a)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_CASE3);

		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			if(!grammaticallyStrict)
			{
			#endif
				//b. generate continuous tense form
				//carrying/replying
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//studied/married
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3);

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3);
		}
	}
	//Rule 4: Other words...
	else if(rule3b || rule4)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);

		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			if(!grammaticallyStrict)
			{
			#endif
				//b. generate continuous tense form
				//enjoying/straying
				//needing/beeping
				//needing/laughing
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//played/stayed
			//dreamed/rained
			//parked/earned
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(base + GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
		}
	}

	return result;
}

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS

//grammaticallyStrict is by default set to false: this function was developed for special verb cases e.g. continuous (these verb cases are formally nouns; but are interpreted semantically as actions)
bool GIApreprocessorMultiwordReductionClass::generateAdditionalTenseVariantsOfVerbBase(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList, GIApreprocessorMultiwordReductionWord* baseTag, bool irregularVerbFound, bool grammaticallyStrict)
{
	bool result = true;
		
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

	GIApreprocessorMultiwordReductionWord* currentTagInVerbList = baseTag;

	string base = SHAREDvars.convertStringToLowerCase(&(currentTagInVerbList->tagName));	//OLD: currentTagInVerbList->tagName

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
	if(!irregularVerbFound)
	{
	#endif
		addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, "", GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
	}
	#endif

	if(base.length() >= 3)	//match to baseWithLast3LettersDropped
	{
		int baseStringLength = base.length();
		int indexOfLastCharacterInBase = baseStringLength-1;
		char lastCharacterInBase = base[indexOfLastCharacterInBase];
		string baseWithLast1LettersDropped = base.substr(0, baseStringLength-1);
		string baseWithLast2LettersDropped = base.substr(0, baseStringLength-2);
		string baseWithLast3LettersDropped = base.substr(0, baseStringLength-3);


		#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
		if(!grammaticallyStrict)
		{
		#endif
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
			if(!irregularVerbFound)
			{
			#endif
				//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND/GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS _ing:
				//continuous rule 1a/3b/4: thinking/happening/entering
				addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS);
				//continuous rule 1b: running - "run" + "n" [run n] + "ing"
				addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS);
				//continuous rule 2: changing - "chang" [change e] + "ing"
				addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS);
				/*
				//continuous rule 3a: N/A !marriing (use marrying)
				addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS);
				*/
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
			}
			#endif
		#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
		}
		#endif
		if(!grammaticallyStrict)
		{
			#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
			//added 2h2a
			//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND/GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL _able:
			//potential rule 1a/3b/4: thinkable/changeable
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL);
			//potential rule 1b: running - "run" + "n" [run n] + "able"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL);
			/*
			//potential rule 2: N/A !changable (use changeable)
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL);
			*/
			//potential rule 3a: running - "marr" + "i" + "able"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND_CASE3, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL);
			#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
			//added 2h2c
			//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND/GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE _ive:
			//potential rule 1ai: -> ive eg resistive/adaptive
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE);
			//potential rule 1aii: -> itive eg additive
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1II, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE);
			//potential rule 1aiii: -> ative eg affirmative
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1III, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE);
			/*
			//potential rule 1b: running - "run" + "n" [run n] + "itive"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE);
			*/
			//potential rule 2i: e -> itive eg competitive/definitive/accomodative
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE);
			//potential rule 2ii: e -> ment + ive eg judgementive
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE);
			//potential rule 3a: y -> iment + ive eg supplimentive
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE3, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE);
			#endif
			#endif
		}

		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
			//added 2h2a
			//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND/GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST _ed:
			//possible state rule 1a/3b/4: visited/opened
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST);
			//possible state rule 1b: rubbed/stopped/referred/admitted - "rub" + "b" + "ed"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST);
			//possible state rule 2: smiled/fined - "smil" [change e] + "ed"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST);
			//possible state rule 3a: studied/married - "marr" + "i" + "ed"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST);
			#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif

		if(!grammaticallyStrict)
		{
			#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
			//added 2h2d
			//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND/GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION _ment:
			//potential rule 1ai: -> ment eg movement/government/derailment/detainment/enjoyment
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 1aii: -> ament eg disarmament
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1II, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 1aiii: -> lment eg enrollment/installment/fulfillment
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1III, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			/*
			//potential rule 1b: running - "run" + "n" [run n] + "itive"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_INVERSE_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			*/
			//potential rule 2i: e -> ment eg judgement/dislodgment
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE2, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			/*
			//potential rule 2ii: e -> ment + ive eg judgementive
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			*/
			//potential rule 3a: y -> iment eg worriment/suppliment/embodiment
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE3, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 5: pt -> pment eg entrapment/equipment
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast2LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE5, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);

			//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND/GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION _ion:
			//potential rule 1ai: -> ion eg absorption/abstraction/adaptation
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 1aii: -> ition eg addition
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1II, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 1aiii: -> ation eg acceptation/affirmation
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1III, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			/*
			//potential rule 1b: running - "run" + "n" [run n] + "itive"
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, base+lastCharacterInBase, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_INVERSE_APPEND, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			*/
			//potential rule 2i: e -> ion eg relation/acclimatisation/accommodation/activation/accretion
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 2ii: e -> ition + ive eg competition/composition/definition
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2II, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 2iii: e -> ation + ive eg admiration/organisation
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2III, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 3a: ify -> ification eg subjectification/amplification/ammonification/identification/beautification
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast1LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE3, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 6i: aim -> amation eg acclamation {acclimation?}
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6I, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 6i: ide -> ision eg division
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6II, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			//potential rule 6iii: ish -> ition eg abolition/demolition
			addVerbCaseAdditional(currentTagInVerbList, verbCaseAdditionalList, baseWithLast3LettersDropped, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6III, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION);
			#endif
		}
	}
	
	return result;
}

void GIApreprocessorMultiwordReductionClass::addVerbCaseAdditional(GIApreprocessorMultiwordReductionWord* currentTagInVerbList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList, const string baseTenseFormStart, const string baseTenseFormAppend, const int grammaticalTenseFormNew)
{
	string hypotheticalVerbVariantCase = baseTenseFormStart + baseTenseFormAppend;

	GIApreprocessorMultiwordReductionWord* newWord = new GIApreprocessorMultiwordReductionWord();
	newWord->grammaticalTenseForm = grammaticalTenseFormNew;
	newWord->baseName = currentTagInVerbList->tagName;

	verbCaseAdditionalList->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(hypotheticalVerbVariantCase, newWord));
}
#endif

#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
//warning: this function is only currently developed for infinitive and continuous case
bool GIApreprocessorMultiwordReductionClass::determineVerbCaseAdditionalWrapper(const string word, string* baseNameFound, int* grammaticalBaseTenseForm)
{
	bool result = true;
	bool foundVerbCase = false;
	if(!wordListsLoaded)
	{
		cout << "!wordListsLoaded (GIA with GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL requires -lrpfolder to be set)" << endl;
		result = false;
	}
	else
	{
		if(determineVerbCaseAdditional(word, baseNameFound, grammaticalBaseTenseForm))
		{
			foundVerbCase = true;
		}
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
		foundIrregularVerbContinuousCase = determineIfWordIsIrregularVerbContinuousCase(word, firstTagInIrregularVerbListGlobal, baseNameFound);
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

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
bool GIApreprocessorMultiwordReductionClass::checkGrammaticalWordTypeFeaturePrelim(GIApreprocessorPlainTextWord* wordTag, const int GIAposType)
{
	bool result = false;
	
	//use feature POS type (instead of GIApreprocessor word type/wordNet lists) if available
	if(wordTag->featureReferencePrelim != NULL)
	{
		/*
		cout << "wordTag->tagName = " << wordTag->tagName << endl;
		cout << "GIAposType = " << GIAposType << endl;
		cout << "wordTag->featureReferencePrelim->GIAposType = " << wordTag->featureReferencePrelim->GIAposType << endl;
		*/
		if(wordTag->featureReferencePrelim->GIAposType == GIAposType)
		{
			result = true;
		}
	}
	
	return result;
}
#endif

bool GIApreprocessorMultiwordReductionClass::determineIsWordType(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, const int GIAposType)
{
	string baseNameFound = "";
	int grammaticalBaseTenseForm = INT_DEFAULT_VALUE;
	return determineIsWordType(wordTag, usePOSprelim, grammaticallyStrict, GIAposType, &baseNameFound, &grammaticalBaseTenseForm);
}
//preconditions: if usePOSprelim, then grammaticallyStrict is assumed true
bool GIApreprocessorMultiwordReductionClass::determineIsWordType(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, const int GIAposType, string* baseNameFound, int* grammaticalBaseTenseForm)
{
	bool wordTypeDetected = false;
	
	string wordLowerCase = SHAREDvars.convertStringToLowerCase(&(wordTag->tagName));
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	if(usePOSprelim)
	{
		if(checkGrammaticalWordTypeFeaturePrelim(wordTag, GIAposType))		
		{
			wordTypeDetected = true;		//usePOSprelim assumes grammaticallyStrict so no check is required
			if(GIAposType == GIA_PREPROCESSOR_POS_TYPE_VERB)
			{
				if(!determineVerbCaseStandardWithAdditional(wordLowerCase, baseNameFound, grammaticalBaseTenseForm))	//added 3e8a
				{
					GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentenceTemp = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
					if(!(currentTagInPlainTextSentenceTemp->collapsedMultiwordWord || currentTagInPlainTextSentenceTemp->collapsedPhrasalVerbExactDefinedSection))
					{
						cout << "GIApreprocessorMultiwordReductionClass::determineIsWordType{} warning: GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS: usePOSprelim && checkGrammaticalWordTypeFeaturePrelim && (GIAposType == GIA_PREPROCESSOR_POS_TYPE_VERB) && !determineVerbCaseStandardWithAdditional" << endl;		
					}
					
					*grammaticalBaseTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE;
				}
			}
		}
	}
	else 
	{
	#endif
		if(GIAposType == GIA_PREPROCESSOR_POS_TYPE_VERB)
		{
			if(determineVerbCaseStandardWithAdditional(wordLowerCase, baseNameFound, grammaticalBaseTenseForm))
			{
				if(!grammaticallyStrict || verbCaseDetectGrammaticallyStrictVariant(*grammaticalBaseTenseForm))
				{
					wordTypeDetected = true;
				}
			}
		}
		else
		{
			unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordTypeList = getWordList(GIAposType);	
			if(determineIsWordType(wordLowerCase, wordTypeList))
			{
				wordTypeDetected = true;
			}
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	}
	#endif
	
	if(GIAposType == GIA_PREPROCESSOR_POS_TYPE_NOUN)
	{
		if(wordTypeDetected)
		{
			*baseNameFound = wordLowerCase;
			*grammaticalBaseTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_TAG_BASE_FORM_SINGULAR;	
		}

		GIApreprocessorMultiwordReductionWord* nounBaseFormFound = NULL;	
		if(determineNounPluralVariant(wordLowerCase, &nounBaseFormFound))
		{
			wordTypeDetected = true;
			*baseNameFound = nounBaseFormFound->tagName;
			*grammaticalBaseTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_TAG_BASE_FORM_PLURAL;
		}
	}
	
	if(GIAposType == GIA_PREPROCESSOR_POS_TYPE_VERB)
	{
		GIApreprocessorMultiwordReductionPlainTextWord* wordTagPlaintext = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
		if(wordTagPlaintext->collapsedPhrasalVerbExactDefinedSection)
		{
			*grammaticalBaseTenseForm = wordTagPlaintext->grammaticalTenseFormDetected;	//already determined by GIApreprocessorMultiwordReductionClass::searchAndReplacePhrasalVerbs
			//cout << "*grammaticalBaseTenseForm = " << *grammaticalBaseTenseForm << endl;
			wordTypeDetected = true;
		}	
	}
	
	GIApreprocessorMultiwordReductionPlainTextWord* wordTagPlaintext = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(wordTag);
	if(wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIAposType))
	{
		if(GIAposType == GIA_PREPROCESSOR_POS_TYPE_VERB)
		{
			cout << "GIApreprocessorMultiwordReductionClass::determineIsWordType warning: !(wordTagPlaintext->collapsedPhrasalVerbExactDefinedSection) && (wordTagPlaintext->collapsedMultiwordWord && (wordTagPlaintext->collapsedMultiwordWordType == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_TYPE))" << endl;
		}
		wordTypeDetected = true;
	}

	return wordTypeDetected;
}
bool GIApreprocessorMultiwordReductionClass::determineIsWordType(const string word, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordTypeList)
{
	return findWordInWordList(wordTypeList, word);
}



bool GIApreprocessorMultiwordReductionClass::determineIsVerb(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict)
{
	string baseNameFound = "";
	int grammaticalBaseTenseForm = INT_DEFAULT_VALUE;
	return determineIsVerb(wordTag, usePOSprelim, grammaticallyStrict, &baseNameFound, &grammaticalBaseTenseForm);
}
bool GIApreprocessorMultiwordReductionClass::determineIsVerb(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, string* baseNameFound, int* grammaticalBaseTenseForm)
{
	bool wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, grammaticallyStrict, GIA_PREPROCESSOR_POS_TYPE_VERB, baseNameFound, grammaticalBaseTenseForm);
	return wordTypeDetected;	
}
bool GIApreprocessorMultiwordReductionClass::determineVerbCaseStandardWithAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm)
{
	bool foundVerbCaseStandardOrAdditional = false;
	
	bool foundVerbCaseStandard = determineVerbCaseStandard(word, baseNameFound, grammaticalBaseTenseForm);
	if(foundVerbCaseStandard)
	{
		//cout << "determineVerbCaseStandard, word = " << word << endl;
		//cout << "determineVerbCaseStandard, baseNameFound = " << *baseNameFound << endl;
		foundVerbCaseStandardOrAdditional = true;
	}
	
	bool foundVerbCaseAdditional = determineVerbCaseAdditional(word, baseNameFound, grammaticalBaseTenseForm);
	if(foundVerbCaseAdditional)
	{
		//cout << "determineVerbCaseAdditional, word = " << word << endl;
		//cout << "determineVerbCaseAdditional, baseNameFound = " << *baseNameFound << endl;
		foundVerbCaseStandardOrAdditional = true;
	}
	
	return foundVerbCaseStandardOrAdditional;
}
/*
bool GIApreprocessorMultiwordReductionClass::convertVerbCaseGrammaticalTenseFormToTenseModifier(const int grammaticalTenseForm, int* grammaticalTenseModifier)
{
	bool result = false;
	
	*grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP;
	
	if(grammaticalTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS)
	{
		*grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP;	//corrected @GIA3f1a //OLD: GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE
		result = true;
	}
	else if(grammaticalTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST)
	{
		*grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP;
		result = true;
	}
	#ifdef GIA_PREPROCESSOR_DETERMINE_VERB_CASE_ADDITIONAL_EXTENDED
	else if(grammaticalTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION)
	{
		*grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP;
		result = true;
	}
	else if(grammaticalTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL)
	{
		*grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP;
		result = true;
	}
	else if(grammaticalTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE)
	{
		*grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP;
		result = true;
	}
	#endif
	
	return result;
}
*/
bool GIApreprocessorMultiwordReductionClass::verbCaseDetectGrammaticallyStrictVariant(const int grammaticalTenseForm)
{
	bool result = false;
	if(GIApreprocessorMultiwordReductionVerbDatabaseTagBaseTenseFormGrammaticallyStrictArray[grammaticalTenseForm])
	{
		result = true;
	}
	return result;
}
bool GIApreprocessorMultiwordReductionClass::determineVerbCaseStandard(const string word, string* baseNameFound, int* grammaticalBaseTenseForm)
{
	bool foundVerbCase = false;
	
	GIApreprocessorMultiwordReductionWord* wordFound = NULL;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseStandardList = &verbCaseStandardListGlobal;
	if(findWordInWordList(verbCaseStandardList, word, &wordFound))
	{
		foundVerbCase = true;
		*grammaticalBaseTenseForm = wordFound->grammaticalTenseForm;
		*baseNameFound = wordFound->baseName;
	}
	
	return foundVerbCase;	
}
bool GIApreprocessorMultiwordReductionClass::determineVerbCaseAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm)
{
	bool foundVerbCase = false;
	
	GIApreprocessorMultiwordReductionWord* wordFound = NULL;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList = &verbCaseAdditionalListGlobal;
	if(findWordInWordList(verbCaseAdditionalList, word, &wordFound))
	{
		foundVerbCase = true;
		*grammaticalBaseTenseForm = wordFound->grammaticalTenseForm;
		*baseNameFound = wordFound->baseName;
	}
	
	return foundVerbCase;	
}


bool GIApreprocessorMultiwordReductionClass::determineIsPreposition(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_PREPOSITION);
	return wordTypeDetected;
}
/*
bool GIApreprocessorMultiwordReductionClass::determineIsPreposition(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* prepositionsList = getWordList(GIA_PREPROCESSOR_POS_TYPE_PREPOSITION);
	return findWordInWordList(prepositionsList, word);
}
*/


bool GIApreprocessorMultiwordReductionClass::determineIsAdverb(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_ADVERB);
	return wordTypeDetected;
}
/*
bool GIApreprocessorMultiwordReductionClass::determineIsAdverb(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* adverbList = getWordList(GIA_PREPROCESSOR_POS_TYPE_ADVERB);
	return findWordInWordList(adverbList, word);
}
*/


bool GIApreprocessorMultiwordReductionClass::determineIsAdjective(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_ADJECTIVE);
	return wordTypeDetected;
}
/*
bool GIApreprocessorMultiwordReductionClass::determineIsAdjective(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* adjectiveList = getWordList(GIA_PREPROCESSOR_POS_TYPE_ADJECTIVE);
	return findWordInWordList(adjectiveList, word);
}
*/


bool GIApreprocessorMultiwordReductionClass::determineIsNoun(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	string baseNameFound = "";
	int grammaticalBaseForm = INT_DEFAULT_VALUE;
	return determineIsNoun(wordTag, usePOSprelim, &baseNameFound, &grammaticalBaseForm);
}
bool GIApreprocessorMultiwordReductionClass::determineIsNoun(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, string* baseNameFound, int* grammaticalBaseForm)
{
	bool wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_NOUN, baseNameFound, grammaticalBaseForm);
	return wordTypeDetected;
}
/*
bool GIApreprocessorMultiwordReductionClass::determineIsNoun(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounList = getWordList(GIA_PREPROCESSOR_POS_TYPE_NOUN);
	return findWordInWordList(nounList, word);
}
*/
bool GIApreprocessorMultiwordReductionClass::determineNounPluralVariant(const string word, GIApreprocessorMultiwordReductionWord** nounBaseFormFound)
{
	bool foundNounPluralVariant = false;
	
	GIApreprocessorMultiwordReductionWord* wordFound = NULL;
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounPluralVariantsList = &nounPluralVariantsListGlobal;
	if(findWordInWordList(nounPluralVariantsList, word, &wordFound))
	{
		foundNounPluralVariant = true;
		*nounBaseFormFound = wordFound;
	}
	
	return foundNounPluralVariant;	
}

bool GIApreprocessorMultiwordReductionClass::determineIsConjunction(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_CONJUNCTION_COORDINATING);	//OLD: GIA_PREPROCESSOR_POS_TYPE_CONJUNCTION
	return wordTypeDetected;
}
/*
bool GIApreprocessorMultiwordReductionClass::determineIsConjunction(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* conjunctionsList = getWordList(GIA_PREPROCESSOR_POS_TYPE_CONJUNCTION_COORDINATING);	//OLD: GIA_PREPROCESSOR_POS_TYPE_CONJUNCTION
	return findWordInWordList(conjunctionsList, word);
}
*/


bool GIApreprocessorMultiwordReductionClass::determineIsDeterminer(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_DETERMINER);
	return wordTypeDetected;
}
/*
bool GIApreprocessorMultiwordReductionClass::determineIsDeterminer(const string word)
{
	unordered_map<string, GIApreprocessorMultiwordReductionWord*>* determinersList = getWordList(GIA_PREPROCESSOR_POS_TYPE_DETERMINER);
	return findWordInWordList(determinersList, word);
}
*/


bool GIApreprocessorMultiwordReductionClass::determineIsAuxiliaryBeing(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = false;
	#ifndef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_AUXILIARY_LISTS_TO_VERB_LISTS
	wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_BEING);
	#else
	wordTypeDetected = determineIsWordType(wordTag->tagName, wordListRecordAuxiliaryBeing);
	/*
	if(SHAREDvars.textInTextArray(*relationshipName, entityAuxiliaryBeingArray, ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES))		
	{
		wordTypeDetected = true;
	}
	*/
	#endif
	return wordTypeDetected;
}
bool GIApreprocessorMultiwordReductionClass::determineIsAuxiliaryHaving(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = false;
	#ifndef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_AUXILIARY_LISTS_TO_VERB_LISTS
	wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_HAVING);
	#else
	wordTypeDetected = determineIsWordType(wordTag->tagName, wordListRecordAuxiliaryHaving);
	/*
	if(SHAREDvars.textInTextArray(*relationshipName, entityAuxiliaryHavingArray, ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES))
	{
		wordTypeDetected = true;
	}
	*/
	#endif
	return wordTypeDetected;
}
bool GIApreprocessorMultiwordReductionClass::determineIsAuxiliaryDoing(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = false;
	#ifndef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_AUXILIARY_LISTS_TO_VERB_LISTS
	wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_AUXILIARY_DOING);
	#else
	wordTypeDetected = determineIsWordType(wordTag->tagName, wordListRecordAuxiliaryDoing);
	/*
	if(SHAREDvars.textInTextArray(*relationshipName, entityAuxiliaryDoingArray, ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES))
	{
		wordTypeDetected = true;
	}
	*/
	#endif
	return wordTypeDetected;
}

bool GIApreprocessorMultiwordReductionClass::detectAuxiliary(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = false;
	#ifndef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_AUXILIARY_LISTS_TO_VERB_LISTS
	wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_AUXILIARY);
	#else
	if(determineIsWordType(wordTag->tagName, wordListRecordAuxiliaryBeing))
	{
		wordTypeDetected = true;
	}
	if(determineIsWordType(wordTag->tagName, wordListRecordAuxiliaryHaving))
	{
		wordTypeDetected = true;
	}
	if(determineIsWordType(wordTag->tagName, wordListRecordAuxiliaryDoing))
	{
		wordTypeDetected = true;
	}	
	/*
	if(SHAREDvars.textInTextArray(currentWord, entityAuxiliaryBeingArray, ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES))
	{
		wordTypeDetected = true;
	}
	if(SHAREDvars.textInTextArray(currentWord, entityAuxiliaryHavingArray, ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES))
	{
		wordTypeDetected = true;
	}
	if(SHAREDvars.textInTextArray(currentWord, entityAuxiliaryDoingArray, ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES))
	{
		wordTypeDetected = true;
	}
	*/
	#endif
	return wordTypeDetected;
}

bool GIApreprocessorMultiwordReductionClass::detectModalAuxiliary(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = false;
	wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_MODALAUXILIARY);
	/*
	if(SHAREDvars.textInTextArray(currentWord, entityModalAuxiliaryArray, ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES))
	{
		wordTypeDetected = true;
	}
	*/
	return wordTypeDetected;
}

bool GIApreprocessorMultiwordReductionClass::detectRcmodSameReferenceSetDelimiter(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim)
{
	bool wordTypeDetected = false;
	wordTypeDetected = determineIsWordType(wordTag, usePOSprelim, GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT, GIA_PREPROCESSOR_POS_TYPE_RCMOD);
	/*
	if(SHAREDvars.textInTextArray(currentWord, entityRcmodSameReferenceSetDelimiter, ENTITY_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET_DELIMITER_NUMBER_OF_TYPES))
	{
		rcmodSameReferenceSetDelimiterFound = true;
	}
	*/
	return wordTypeDetected;
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


#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
bool GIApreprocessorMultiwordReductionClass::createWordIndexListFromLRPfiles()
{
	bool result = true;
	
	if(!wordListAllTypesWithPOSambiguityInfoLoaded)
	{
		//GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS: assume that all word lists have been loaded by GIApreprocessorMultiwordReductionClass::initialiseLRP
		if(wordListsLoaded)
		{		
			if(verbListWithVariantsGlobal.size() == 0)
			{	
				//cout << "createWordIndexListFromLRPfiles" << endl;
							
				//generate verbListWithVariantsGlobal and nounListWithVariantsGlobal;
				
				//verbListWithVariantsGlobal = *(getWordList(GIA_PREPROCESSOR_POS_TYPE_VERB));
				//nounListWithVariantsGlobal = *(getWordList(GIA_PREPROCESSOR_POS_TYPE_NOUN));
				unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbListGlobal = getWordList(GIA_PREPROCESSOR_POS_TYPE_VERB); 
				unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounListGlobal = getWordList(GIA_PREPROCESSOR_POS_TYPE_NOUN); 

				verbListWithVariantsGlobal.insert(verbListGlobal->begin(), verbListGlobal->end());
				#ifdef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
				verbListWithVariantsGlobal.insert(verbCaseStandardListGlobal.begin(), verbCaseStandardListGlobal.end());
				verbListWithVariantsGlobal.insert(verbCaseAdditionalListGlobal.begin(), verbCaseAdditionalListGlobal.end());
				#else
				//GIA_PREPROCESSOR_POS_TAGGER requires grammatically strict verb variants (regardless of whether grammatically strict verb variants are used by GIA preprocessor for higher level semantic processing of actions; by default they are not)
				
				for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator iter = verbCaseStandardListGlobal.begin(); iter != verbCaseStandardListGlobal.end(); iter++)
				{
					string index = iter->first;
					GIApreprocessorMultiwordReductionWord* word = iter->second;
					#ifdef GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
					if(verbCaseDetectGrammaticallyStrictVariant(word->grammaticalTenseForm))	//ensure that the word is a grammatically strict verb
					{
					#endif
						verbListWithVariantsGlobal.insert(pair<string, GIApreprocessorMultiwordReductionWord*>(index, word));
					#ifdef GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
					}
					#endif
				}
				for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator iter = verbCaseAdditionalListGlobal.begin(); iter != verbCaseAdditionalListGlobal.end(); iter++)
				{
					string index = iter->first;
					GIApreprocessorMultiwordReductionWord* word = iter->second;
					#ifdef GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
					if(verbCaseDetectGrammaticallyStrictVariant(word->grammaticalTenseForm))	//ensure that the word is a grammatically strict verb
					{
					#endif
						verbListWithVariantsGlobal.insert(pair<string, GIApreprocessorMultiwordReductionWord*>(index, word));
					#ifdef GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
					}
					#endif
				}
				#endif
				nounListWithVariantsGlobal.insert(nounListGlobal->begin(), nounListGlobal->end());
				nounListWithVariantsGlobal.insert(nounPluralVariantsListGlobal.begin(), nounPluralVariantsListGlobal.end());
				
				#ifdef GIA_PREPROCESSOR_REPLACE_VERB_AND_NOUN_LISTS_WITH_VERB_AND_NOUN_LISTS_WITH_VARIANTS
				verbListGlobal->clear();
				nounListGlobal->clear();
				verbListGlobal->insert(verbListWithVariantsGlobal.begin(), verbListWithVariantsGlobal.end());
				nounListGlobal->insert(nounListWithVariantsGlobal.begin(), nounListWithVariantsGlobal.end());
				#endif
				
				#ifdef GIA_PREPROCESSOR_REMOVE_VERB_VARIANT_CONTINUOUS_FROM_NOUN_LISTS
				for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator iter = verbListWithVariantsGlobal.begin(); iter != verbListWithVariantsGlobal.end(); iter++)
				{
					string index = iter->first;
					GIApreprocessorMultiwordReductionWord* word = iter->second;
					if(word->grammaticalTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS)
					{
						unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator it=nounListGlobal.find(index);
						if(it != nounListGlobal.end())
						{
							nounListGlobal.delete(it);
						}
					}
				}
				#endif
			}
	
	
			//see GIA_PREPROCESSOR_MULTIWORD_REDUCTION:loadWordList - expect multiwords within the word lists to be concatenated with delimiter _ (because all tags in input text with multiwords have had their space delimiters replaced with _ delimiters)
			unordered_map<string, GIApreprocessorMultiwordReductionWord*> wordListAllTypes;
			for(unordered_map<string, unordered_map<string, GIApreprocessorMultiwordReductionWord*>*>::iterator iter = wordListsGlobal.begin(); iter != wordListsGlobal.end(); iter++)
			{
				unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = iter->second;
				wordListAllTypes.insert(wordList->begin(), wordList->end());
			}
		
			for(unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator iter = wordListAllTypes.begin(); iter != wordListAllTypes.end(); iter++)
			{
				string wordIndex = iter->first;
				GIApreprocessorMultiwordReductionWord* word = iter->second;
				unsigned long POStypeAmbiguity = 0;
				for(int GIAposType=0; GIAposType<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; GIAposType++)
				{		
					string POStypeName = GIApreprocessorPOStypeNameArray[GIAposType];
					bool foundWordList = false;
					unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = getWordListIfExistent(GIAposType, &foundWordList);
					if(foundWordList)
					{
						unordered_map<string, GIApreprocessorMultiwordReductionWord*>::iterator it;
						it = wordList->find(wordIndex);
						if(it != wordList->end())
						{
							POStypeAmbiguity = (SHAREDvars.setBitValue(POStypeAmbiguity, GIAposType, true));
							//cout << "POStypeAmbiguity = " << POStypeAmbiguity << endl;
							//cout << "(1 << GIAposType) = " << (1 << GIAposType) << endl;
						}
					}
					else
					{
						//ignore posTypes with missing wordLists
						//cerr << "GIApreprocessorMultiwordReductionClass::createWordIndexListFromLRPfiles{} warning: cannot find wordlist, POStypeName = " << POStypeName << endl;
					}
				}
				
				insertInstanceInMapWordListAllTypesWithPOSambiguityInfo(&wordListAllTypesWithPOSambiguityInfo, wordIndex, word, POStypeAmbiguity);
				//cout << "insertInstanceInMapWordListAllTypesWithPOSambiguityInfo: wordIndex = " << wordIndex << ", word->tagName = " << word->tagName << ", POStypeAmbiguity = " << POStypeAmbiguity << endl;
			}
			
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ASSIGN_WEIGHTS_TO_TRAINED_POS_TYPES
			GIApreprocessorPOStypeWeights.resize(GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES);
			for(int GIAposType=0; GIAposType<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; GIAposType++)
			{
				string POStypeName = GIApreprocessorPOStypeNameArray[i];
				unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = getWordListIfExistent(GIAposType, &foundWordList); 
				if(foundWordList)
				{
					int wordListSize = wordList->size();
					double wordListWeight = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ASSIGN_WEIGHTS_TO_TRAINED_POS_TYPES_WEIGHT_STANDARD;
					if(wordListSize < 100)
					{
						wordListWeight = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ASSIGN_WEIGHTS_TO_TRAINED_POS_TYPES_WEIGHT_HIGH;
					}
					GIApreprocessorPOStypeWeights[GIAposType] = wordListWeight;
				}
				else
				{
					//ignore posTypes with missing wordLists
					//cerr << "GIApreprocessorMultiwordReductionClass::createWordIndexListFromLRPfiles{} warning: cannot find wordlist, POStypeName = " << POStypeName << endl;
				}
			}
			#endif	
					
			wordListAllTypesWithPOSambiguityInfoLoaded = true;
		}
		else
		{
			cerr << "GIApreprocessorPOStaggerClass::createWordIndexListFromLRPfiles{} error: !(wordListsLoaded)" << endl;
			exit(EXIT_ERROR);
		}
	}
		
	return result;
}
#endif




#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_NUMERICAL_WORDS_TO_NUMBERS

bool GIApreprocessorMultiwordReductionClass::replaceAllNumericalWordsWithNumbers(GIApreprocessorSentence* firstGIApreprocessorSentenceInList)
{
	bool result = true;
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		GIApreprocessorMultiwordReductionClassObject.generateFlatSentenceWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), &firstTagInPlainTextSentence);
		GIApreprocessorPlainTextWord* currentTag = firstTagInPlainTextSentence;
		GIApreprocessorPlainTextWord* firstTagInNumericalWord = NULL;
		GIApreprocessorPlainTextWord* previousTag = NULL;
		bool parsingNumericalWord = false;
		while(currentTag->nextTag != NULL)
		{
			bool foundDelimiter = false;
			
			string currentWordText = SHAREDvars.convertStringToLowerCase(&(currentTag->tagName));
			if(SHAREDvars.textInTextArray(currentWordText, translatorEnglishNumbersArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMBER_OF_TYPES))
			{
				//cout << "currentWordText = " << currentWordText << endl;
				if(parsingNumericalWord)
				{
					//continue parsing
				}
				else
				{
					firstTagInNumericalWord = currentTag;
					parsingNumericalWord = true;
				}
			}
			else
			{
				if(parsingNumericalWord)
				{
					if(currentWordText == GIA_TRANSLATOR_ENGLISH_NUMBERS_DELIMITER)
					{
						//continue parsing
						foundDelimiter = true;
					}
					else
					{
						parsingNumericalWord = false;
						GIApreprocessorPlainTextWord* tagSucceedingNumericalWordTagList = NULL;
						if(previousTag->nextTag->tagName == GIA_TRANSLATOR_ENGLISH_NUMBERS_DELIMITER)
						{
							tagSucceedingNumericalWordTagList = static_cast<GIApreprocessorPlainTextWord*>(previousTag->nextTag);
						}
						else
						{
							tagSucceedingNumericalWordTagList = currentTag;
						} 
						//cout << "tagSucceedingNumericalWordTagList = " << tagSucceedingNumericalWordTagList->tagName << endl;
						//cout << "previousTag = " << previousTag->tagName << endl;
						previousTag->nextTag = new GIApreprocessorPlainTextWord();	//isolate the numerical word tag list
						vector<GIApreprocessorPlainTextWord*> text;
						GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstTagInNumericalWord, &text);
						
						//convert numerical word list to lower case and remove all references to 'and':
						for(vector<GIApreprocessorPlainTextWord*>::iterator iter = text.begin(); iter != text.end(); )
						{
							string wordText = (*iter)->tagName;
							//cout << "wordText = " << wordText << endl;
							(*iter)->tagName = SHAREDvars.convertStringToLowerCase(&wordText);
							
							if(wordText == GIA_TRANSLATOR_ENGLISH_NUMBERS_DELIMITER)
							{
								iter = text.erase(iter);
							}
							else
							{
								iter++;
							}
						}

						
						long number = convertWordToNumber(&text);
						//cout << "number = " << number << endl;
						firstTagInNumericalWord->tagName = SHAREDvars.convertLongToString(number);
						firstTagInNumericalWord->nextTag = tagSucceedingNumericalWordTagList;
					}
				}
			}
			if(!foundDelimiter)
			{
				previousTag = currentTag;	//don't include any final "and" words within the numerical word list (e.g "the number of dogs was three and the number of chickens was five")
			}
			
			currentTag = static_cast<GIApreprocessorPlainTextWord*>(currentTag->nextTag);
		}
			
		GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstTagInPlainTextSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsLRP));
	
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
		
	return result;		
}

//based on https://www.roseindia.net/answers/viewqa/Java-Interview-Questions/13787-Java-program---convert-words-into-numbers.html

long GIApreprocessorMultiwordReductionClass::convertWordToNumber(vector<GIApreprocessorPlainTextWord*>* numericalWordList)
{
		
	long totalValue = 0;
	bool processed = false;
	for(int n = 0; n < GIA_TRANSLATOR_ENGLISH_NUMBERS_MAGITUDES_NUMBER_OF_TYPES; n++) 
	{
		int index = GIApreprocessorMultiwordReductionClassObject.findStringInWordList(numericalWordList, translatorEnglishNumbersMagnitudesTextArray[n]);
		if(index != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			vector<GIApreprocessorPlainTextWord*>::const_iterator first1 = numericalWordList->begin();
			vector<GIApreprocessorPlainTextWord*>::const_iterator last1 = numericalWordList->begin() + index;
			vector<GIApreprocessorPlainTextWord*> text1(first1, last1);

			vector<GIApreprocessorPlainTextWord*>::const_iterator first2 = numericalWordList->begin() + index + translatorEnglishNumbersMagnitudesTextArray[n].length();
			vector<GIApreprocessorPlainTextWord*>::const_iterator last2 = numericalWordList->end();
			vector<GIApreprocessorPlainTextWord*> text2(first2, last2);

			if(text1.empty())
			{	
				GIApreprocessorMultiwordReductionPlainTextWord* newWord = new GIApreprocessorMultiwordReductionPlainTextWord();
				newWord->tagName = "one";
				text1.push_back(newWord);
			}
			if(text2.empty())
			{
				GIApreprocessorMultiwordReductionPlainTextWord* newWord = new GIApreprocessorMultiwordReductionPlainTextWord();
				newWord->tagName = "zero";
				text2.push_back(newWord);
			}
			
			totalValue = parseNumerals(&text1) * translatorEnglishNumbersMagnitudesNumberArray[n] + convertWordToNumber(&text2);
			processed = true;
			break;
		}
	}

	if(processed)
	{
		return totalValue;
	}
	else
	{
		return parseNumerals(numericalWordList);
	}
}

long GIApreprocessorMultiwordReductionClass::parseNumerals(vector<GIApreprocessorPlainTextWord*>* numericalWordListSubset)
{
	long value = 0;
		
	for(int i=0; i<numericalWordListSubset->size(); i++) 
	{
		string wordText = (*numericalWordListSubset)[i]->tagName;
		
		if(!SHAREDvars.textInTextArray(wordText, translatorEnglishNumbersNumeralsTextArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMBER_OF_TYPES))
		{
			cerr << "GIApreprocessorMultiwordReductionClass::parseNumerals{} error: unknown numerical word: " << wordText << endl;
			exit(EXIT_ERROR);
		}

		long subval = getValueOf(wordText);
		if(subval == 100) 
		{
			if(value == 0)
			{
				value = 100;
			}
			else
			{
				value *= 100;
			}
		} 
		else
		{
			value += subval;
		}
	}

	return value;
}

long GIApreprocessorMultiwordReductionClass::getValueOf(const string wordText) 
{
	int index = 0;
	if(!SHAREDvars.textInTextArray(wordText, translatorEnglishNumbersNumeralsTextArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMERALS_NUMBER_OF_TYPES, &index))
	{
		cerr << "GIApreprocessorMultiwordReductionClass::getValueOf{} error: wordText not in translatorEnglishNumbersNumeralsTextArray: " << wordText << endl;
		exit(EXIT_ERROR);
	}
	long number = translatorEnglishNumbersNumeralsNumberArray[index];
	return number;
}
			
#endif


