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
 * File Name: GIApreprocessorWordReduction.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1n 24-April-2018
 * Requirements: requires plain text file
 * Description: Preprocessor Word Reduction
 * /
 *******************************************************************************/


#include "GIApreprocessorWordReduction.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
#include "GIAsynRelTranslatorDefs.hpp"
#endif


#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION

GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* textGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* queryGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* activeGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;

multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*> phrasalVerbListGlobal;
unordered_map<string, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>*> multiwordListsGlobal;	//string: preprocessorPOStype

/*
multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* GIApreprocessorWordReductionClass::getPhrasalVerbListGlobal()
{
	return phrasalVerbListGlobal;
}
unordered_map<string, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>*>* GIApreprocessorWordReductionClass::getMultiwordListsGlobal()
{
	return multiwordListsGlobal;
}
*/

bool GIApreprocessorWordReductionClass::initialiseLRPreduction(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList)
{
	bool result = true;
	
	if(!loadPhrasalVerbDataAndGenerateAllTenseVariants(GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_FILE_NAME, &phrasalVerbListGlobal, irregularVerbList))
	{
		result = false;
	}	

	int numberOfMultiwordLists;
	vector<string> multiwordListFileNames;
	if(!SHAREDvars.getLinesFromFile(GIA_PREPROCESSOR_WORD_MULTIWORDLISTS_DATABASE_FILE_NAME, &multiwordListFileNames, &numberOfMultiwordLists))
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
				cerr << "GIApreprocessorWordReductionClass::initialiseLRP{} error: cannot find space character in multiwordListFileNames[i], i = " << i << endl;
				exit(EXIT_ERROR);
			}
		}
		else
		{
			//skip blank lines
		}
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
bool GIApreprocessorWordReductionClass::loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList)
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
				if(currentWord == GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_OR)
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
					if(SHAREDvars.textInTextArray(currentWord, lrpPhrasalVerbDatabaseTagArbitraryNameArray, GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES, &i))
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
							for(int i=0; i<GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
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
				if((currentToken == CHAR_NEWLINE) || (currentWord == GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_OR))
				{
					parsingVerbBase = true;
					phrasalVerbBaseNameRecord = "";
				}
				//start parsing new

			}
			else if(currentToken == GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_ALTERNATE)
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
				if(SHAREDvars.textInTextArray(currentWord, lrpPhrasalVerbDatabaseTagArbitraryNameArray, GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES, &i))
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
			else if(currentToken == GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_OPEN_OPTIONAL)
			{
				currentWordOptional = true;
			}
			else if(currentToken == GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_CLOSE_OPTIONAL)
			{//moved 1p1aTEMP5b
				currentWordOptional = false;
			}
			#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
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








GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* GIApreprocessorWordReductionClass::getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo()
{
	return activeGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo;
}
void GIApreprocessorWordReductionClass::setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery)
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
void GIApreprocessorWordReductionClass::initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery)
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
void GIApreprocessorWordReductionClass::deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery)
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


bool GIApreprocessorWordReductionClass::parseTextFileAndReduceLanguage(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName)
{
	bool result = true;

	string currentFolder = SHAREDvars.getCurrentDirectory();

	#ifdef GIA_PREPROCESSOR_WORD_COLLAPSE_NUMERICAL_WORDS_TO_NUMBERS
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

	#ifdef GIA_PREPROCESSOR_WORD_ORIGINAL
	for(unordered_map<string, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>*>::iterator multiwordListsIter = multiwordListsGlobal.begin(); multiwordListsIter != multiwordListsGlobal.end(); multiwordListsIter++)
	{
		string multiwordListName = multiwordListsIter->first;
		multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordList = multiwordListsIter->second;
		int multiwordListType = GIApreprocessorWordClassObject.getPOStypeFromName(multiwordListName);
		if(!searchAndReplaceMultiwordWordList(firstGIApreprocessorSentenceInList, multiwordList, firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, multiwordListType))
		{
			result = false;
		}
	}
	#endif		

	#ifdef GIA_PREPROCESSOR_WORD_DYNAMIC
	for(unordered_map<string, unordered_map<string, GIApreprocessorMultiwordReductionWord*>*>::iterator wordListsIter = wordListsGlobal.begin(); wordListsIter != wordListsGlobal.end(); wordListsIter++)
	{
		string wordListName = wordListsIter->first;
		unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList = wordListsIter->second;
		int wordListType = GIApreprocessorWordClassObject.getPOStypeFromName(wordListName);
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
bool GIApreprocessorWordReductionClass::searchAndReplacePhrasalVerbs(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo)
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
		//cout << "GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)->size() = " << GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)->size() << endl;
		GIApreprocessorWordClassObject.generateFlatSentenceWordList(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList), &firstTagInPlainTextSentence);
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
									for(int i=0; i<GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
									{
										for(int j=0; j<currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[i].size(); j++)
										{
											if((currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[i])[j] == currentTagInPlainTextSentenceTemp->tagName)		//USED TO BE currentTagInCollapsedPhrasalVerb before 6 Sept 2012
											{
												currentTagInCollapsedPhrasalVerb->grammaticalTenseFormDetected = i;								//USED TO BE AND STILL IS currentTagInCollapsedPhrasalVerb before 6 Sept 2012
												generatedTagNameLemma = currentTagInPhrasalVerbNormOrAlternate->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE][0];
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
								cout << "generateTextFromPreprocessorSentenceWordList(firstTagInPhrasalVerb) = " << GIApreprocessorWordClassObject.generateTextFromPreprocessorSentenceWordList(firstTagInPhrasalVerb) << endl;
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
								else if(currentlyParsingTagSpecial && (tagInPhrasalVerbSpecialAndNotFoundCount <= GIA_PREPROCESSOR_WORD_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_MAX_NUMBER_WORDS))
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
		
		GIApreprocessorWordClassObject.generateSentenceWordList(firstTagInPlainTextSentence, GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList));

		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	return result;
}

	
bool GIApreprocessorWordReductionClass::loadMultiwordWordList(const string multiwordWordListFileName, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList)
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

bool GIApreprocessorWordReductionClass::searchAndReplaceMultiwordWordList(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
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
		GIApreprocessorWordClassObject.generateFlatSentenceWordList(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList), &firstTagInPlainTextSentence);
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
		
		GIApreprocessorWordClassObject.generateSentenceWordList(firstTagInPlainTextSentence, GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList));

		sentenceIndexTemp++;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	return result;
}

#ifdef GIA_PREPROCESSOR_WORD_DYNAMIC
bool GIApreprocessorWordReductionClass::searchAndReplaceMultiwordWordListDynamic(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType)
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
		GIApreprocessorWordClassObject.generateFlatSentenceWordList(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList), &firstTagInPlainTextSentence);
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
		
		GIApreprocessorWordClassObject.generateSentenceWordList(firstTagInPlainTextSentence, GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList));

		sentenceIndexTemp++;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	return result;
}
#endif



void GIApreprocessorWordReductionClass::createNewCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo** currentCorrespondenceInfo, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence, GIApreprocessorMultiwordReductionPlainTextWord** currentTagInPlainTextSentence, int entityIndex, int numberWordsInMultiwordMatched)
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
				cerr << "GIApreprocessorWordReductionClass::searchAndReplaceMultiwordWordList error: (newEntityIndex != entityIndex)" << endl;
				cerr << "generateTextFromPreprocessorSentenceWordList(firstTagInPlainTextSentence) = " << GIApreprocessorWordClassObject.generateTextFromPreprocessorSentenceWordList(firstTagInPlainTextSentence) << endl;
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
				
void GIApreprocessorWordReductionClass::renumberEntityIndiciesInCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, int sentenceIndex, int entityIndex, int numberWordsInMultiwordMatched)
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

bool GIApreprocessorWordReductionClass::writeTagListToFile(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPoutput, const bool performLRPforNLPoutput)
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

	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		string sentenceContentsLRP = "";
		string sentenceContentsLRPforNLP = "";
		bool firstCharacterInSentence = true;
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		GIApreprocessorWordClassObject.generateFlatSentenceWordList(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList), &firstTagInPlainTextSentence);

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

			#ifdef GIA_PREPROCESSOR_WORD_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE
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
			#ifdef GIA_PREPROCESSOR_WORD_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE
			sentenceContentsLRP = sentenceContentsLRP + CHAR_NEWLINE;
			#endif
			plainTextLRPOutput->write(sentenceContentsLRP.c_str(), sentenceContentsLRP.length());
		}
		if(performLRPforNLPoutput)
		{
			#ifdef GIA_PREPROCESSOR_WORD_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE
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

string GIApreprocessorWordReductionClass::generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence)
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
		cout << "GIApreprocessorWordReductionClass::generateWordWithLRPforNLPonly{} warning: !(currentTagInPlainTextSentence->collapsedPhrasalVerbExactDefinedSection) && !(currentTagInPlainTextSentence->collapsedMultiwordWord)" << endl;
	}
	*/
	return wordWithLRPforNLPonly;
}
			
			
#ifdef GIA_PREPROCESSOR_WORD_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY

//NB preposition reversion routine will not work for RelEx as RelEx defines dependency relations based on lemmas not words...
void GIApreprocessorWordReductionClass::revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInListForPrepositionsOnly, const bool isPreposition, bool* foundOfficialLRPreplacementString)
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


bool GIApreprocessorWordReductionClass::generateIrregularTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList, bool grammaticallyStrict)
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

			#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
			if(verbTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "1 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}	
			}
			else if(verbTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "2 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}	
			} else
			#endif
			if(verbTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "3 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}	
			}
			else if(verbTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE)
			{
				GIApreprocessorMultiwordReductionIrregularVerbWord* currentTagInAlternateList = currentTagInIrregularVerb;
				while(currentTagInAlternateList->alternateTag != NULL)
				{
					//cout << "4 currentTagInIrregularVerb->tagName = " << currentTagInIrregularVerb->tagName << endl;
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(currentTagInIrregularVerb->tagName);
					currentTagInAlternateList = currentTagInAlternateList->alternateTag;
				}
			}

			irregularVerbTagIndex++;
			currentTagInIrregularVerb = static_cast<GIApreprocessorMultiwordReductionIrregularVerbWord*>(currentTagInIrregularVerb->nextTag);
		}
	}
	
	return irregularVerbFound;
}

bool GIApreprocessorWordReductionClass::generateStandardTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, bool irregularVerbFound, bool grammaticallyStrict)
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
	if(SHAREDvars.charInCharArray(lastCharacterInBase, englishVowelArray, GIA_PREPROCESSOR_WORD_NUMBER_OF_VOWELS))
	{
		lastCharacterIsVowel = true;
	}
	if(SHAREDvars.charInCharArray(secondLastCharacterInBase, englishVowelArray, GIA_PREPROCESSOR_WORD_NUMBER_OF_VOWELS))
	{
		secondLastCharacterIsVowel = true;
	}
	if(SHAREDvars.charInCharArray(thirdLastCharacterInBase, englishVowelArray, GIA_PREPROCESSOR_WORD_NUMBER_OF_VOWELS))
	{
		thirdLastCharacterIsVowel = true;
	}
	/*
	for(int i = 0; i<GIA_PREPROCESSOR_WORD_VERB_DOUBLE_CONSONANT_RULE1_LAST_LETTER_EXCEPTIONS_NUMBER_OF_TYPES; i++)
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

	#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
	if(!irregularVerbFound)
	{
	#endif
		//generate (ie record) infinitive tense form [for consistency; even though this is the same as tagName...]
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE].push_back(base);
	#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
	}
	#endif
		
	//Rule 1: Words ending with a Consonant-Vowel-Consonant Pattern
	if(rule1)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);
		if(rule1b)
		{
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);
		}

		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			if(!grammaticallyStrict)
			{
			#endif
				//b. generate continuous tense form
				//happening/entering
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
				if(rule1b)
				{
					//hopping/sitting
					//beginning/permitting
					baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + lastCharacterInBase + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);	//double consonant
				}
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//visited/opened
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
			if(rule1b)
			{
				//rubbed/stopped
				//referred/admitted
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(base + lastCharacterInBase + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);	//double consonant
			}

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
			if(rule1b)
			{
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(base + lastCharacterInBase + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);	//double consonant
			}
		}
	}
	//Rule 2: Words ending in E
	else if(rule2)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);

		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			if(!grammaticallyStrict)
			{
			#endif
				//b. generate continuous tense form
				//dancing/skating
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//smiled/fined
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
		}
	}
	//Rule 3: Words ending in Y
	else if(rule3a)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_CASE3);

		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		if(!irregularVerbFound)
		{
		#endif
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			if(!grammaticallyStrict)
			{
			#endif
				//b. generate continuous tense form
				//carrying/replying
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//studied/married
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3);

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(baseWithLastLetterDropped + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3);
		}
	}
	//Rule 4: Other words...
	else if(rule3b || rule4)
	{

		//a. generate present tense form
		baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND);

		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
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
				baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND);
			#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_BUT_TREAT_CONTINUOUS_AS_VERB
			}
			#endif
		#ifdef GIA_PREPROCESSOR_WORD_VERB_VARIANTS_PRORITISE_IRREGULAR_VERB_INFO
		}
		#endif
		if(!irregularVerbFound)
		{
			//c. generate past tense form
			//played/stayed
			//dreamed/rained
			//parked/earned
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);

			//d. generate past participle form (assume same as past form)
			baseTag->grammaticalTenseFormsArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE].push_back(base + GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND);
		}
	}

	return result;
}

bool GIApreprocessorWordReductionClass::findSentenceInSentenceListIrregularVerb(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionIrregularVerbSentence** sentenceFound)
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


#ifdef GIA_PREPROCESSOR_WORD_COLLAPSE_NUMERICAL_WORDS_TO_NUMBERS

bool GIApreprocessorWordReductionClass::replaceAllNumericalWordsWithNumbers(GIApreprocessorSentence* firstGIApreprocessorSentenceInList)
{
	bool result = true;
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence = NULL;
		GIApreprocessorWordClassObject.generateFlatSentenceWordList(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList), &firstTagInPlainTextSentence);
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
						GIApreprocessorWordClassObject.generateSentenceWordList(firstTagInNumericalWord, &text);
						
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

						
						int64_t number = convertWordToNumber(&text);
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
			
		GIApreprocessorWordClassObject.generateSentenceWordList(firstTagInPlainTextSentence, GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList));
	
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
		
	return result;		
}

//based on https://www.roseindia.net/answers/viewqa/Java-Interview-Questions/13787-Java-program---convert-words-into-numbers.html

int64_t GIApreprocessorWordReductionClass::convertWordToNumber(vector<GIApreprocessorPlainTextWord*>* numericalWordList)
{
		
	int64_t totalValue = 0;
	bool processed = false;
	for(int n = 0; n < GIA_TRANSLATOR_ENGLISH_NUMBERS_MAGITUDES_NUMBER_OF_TYPES; n++) 
	{
		int index = GIApreprocessorWordClassObject.findStringInWordList(numericalWordList, translatorEnglishNumbersMagnitudesTextArray[n]);
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

int64_t GIApreprocessorWordReductionClass::parseNumerals(vector<GIApreprocessorPlainTextWord*>* numericalWordListSubset)
{
	int64_t value = 0;
		
	for(int i=0; i<numericalWordListSubset->size(); i++) 
	{
		string wordText = (*numericalWordListSubset)[i]->tagName;
		
		if(!SHAREDvars.textInTextArray(wordText, translatorEnglishNumbersNumeralsTextArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMBER_OF_TYPES))
		{
			cerr << "GIApreprocessorWordIdentificationClass::parseNumerals{} error: unknown numerical word: " << wordText << endl;
			exit(EXIT_ERROR);
		}

		int64_t subval = getValueOf(wordText);
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

int64_t GIApreprocessorWordReductionClass::getValueOf(const string wordText) 
{
	int index = 0;
	if(!SHAREDvars.textInTextArray(wordText, translatorEnglishNumbersNumeralsTextArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMERALS_NUMBER_OF_TYPES, &index))
	{
		cerr << "GIApreprocessorWordIdentificationClass::getValueOf{} error: wordText not in translatorEnglishNumbersNumeralsTextArray: " << wordText << endl;
		exit(EXIT_ERROR);
	}
	int64_t number = translatorEnglishNumbersNumeralsNumberArray[index];
	return number;
}
			
#endif





