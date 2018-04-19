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
 * File Name: GIApreprocessorMultiwordReductionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f2p 04-April-2018
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction Class
 * /
 *******************************************************************************/


#include "GIApreprocessorMultiwordReductionClass.hpp"

GIApreprocessorWord::GIApreprocessorWord(void)
{
	initialiseGIApreprocessorWord();
}
GIApreprocessorWord::GIApreprocessorWord(string tagNameNew)
{
	initialiseGIApreprocessorWord();
	tagName = tagNameNew;
}
GIApreprocessorWord::~GIApreprocessorWord(void)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	#ifdef GIA_PREPROCESSOR_POS_TAGGER
	if(featureReferencePrelim != NULL)
	{
		delete featureReferencePrelim;
	}
	#else
	//featureReferencePrelim will be deleted when deleting prelim sentence objects created by parsing of prelim extenal nlp pos tagger output
	#endif
	#endif
		
	if(nextTag != NULL)
	{
		delete nextTag;
	}
}
void GIApreprocessorWord::initialiseGIApreprocessorWord()
{
	tagName = "";

	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	featureReferencePrelim = NULL;
	#endif
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	featureReferenceOriginal = NULL;
	featureReference = NULL;
	entityReference = NULL;
	#endif

	plainTextWord = false;	

	#ifdef GIA_NEURAL_NETWORK
	wordShortcutToConceptNeuron = NULL;
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
	neuralNetworkPreprocessorWordType = GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_UNDEFINED;
	tagNameOriginalNonLemma = "";
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_PERFORM_BATCH_PREDICTIONS
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
	//POStaggerExperiencePermutations = NULL;
	#else
	POStaggerExperience = NULL;
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
	centreWordPOSambiguityInfo = 0;
	#endif
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	unambiguousPOSindex = GIA_SHARED_POS_TYPE_UNDEFINED;	//GIA_PREPROCESSOR_POS_TYPE_UNDEFINED is out of scope
	#else
	POSambiguityInfo = 0;
	#endif
	wordPOStypeInferred = GIA_SHARED_POS_TYPE_UNDEFINED;
	alreadyFoundMatch = false;
	translatorSentenceEntityIndex = GIA_ENTITY_INDEX_UNDEFINED;
	translatorEntity = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordVerbVariantGrammaticalTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordNounVariantGrammaticalTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR;
	#endif
	#endif
		
	nextTag = NULL;
}


GIApreprocessorMultiwordReductionWord::GIApreprocessorMultiwordReductionWord(void)
{
	tagNameLemma = "";

	base = false;
	
	grammaticalTenseForm = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE;
	
	grammaticalTenseModifier = GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP;
	baseName = "";

}
GIApreprocessorMultiwordReductionWord::~GIApreprocessorMultiwordReductionWord(void)
{
}

GIApreprocessorMultiwordReductionPhrasalVerbWord::GIApreprocessorMultiwordReductionPhrasalVerbWord(void)
{
	tagSpecialArbitraryName = false;
	tagSpecialArbitraryNameType = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_UNDEFINED;
	optional = false;
	/*
	primary = false;
	primaryPhrasalVerbReplacementString = "";
	primaryPhrasalVerbReplacementStringNLPonly = "";
	*/
	alternateTag = NULL;
}
GIApreprocessorMultiwordReductionPhrasalVerbWord::~GIApreprocessorMultiwordReductionPhrasalVerbWord(void)
{
}

GIApreprocessorMultiwordReductionIrregularVerbWord::GIApreprocessorMultiwordReductionIrregularVerbWord(void)
{
	alternateTag = NULL;
}
GIApreprocessorMultiwordReductionIrregularVerbWord::~GIApreprocessorMultiwordReductionIrregularVerbWord(void)
{
	if(alternateTag != NULL)
	{
		delete alternateTag;	//CHECKTHIS
	}	
}

GIApreprocessorMultiwordReductionPlainTextWord::GIApreprocessorMultiwordReductionPlainTextWord(void)
{	
	entityIndex = false;
	collapsedPhrasalVerbExactDefinedSection = false;
	collapsedMultiwordWord = false;
	collapsedMultiwordWordType = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_UNDEFINED_TYPE;
	collapsedMultiwordWordTemp = false;
	grammaticalTenseFormDetected = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE;
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	preprocessorUpperLevelWordReference = NULL;
	preprocessorUpperLevelWordReferenceSize = INT_DEFAULT_VALUE;
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	tagNameLRPforNLP = "";
	#endif
	
	plainTextWord = true;
}
GIApreprocessorMultiwordReductionPlainTextWord::GIApreprocessorMultiwordReductionPlainTextWord(string tagNameNew)
{	
	tagName = tagNameNew;
	
	entityIndex = false;
	collapsedPhrasalVerbExactDefinedSection = false;
	collapsedMultiwordWord = false;
	collapsedMultiwordWordType = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_UNDEFINED_TYPE;
	collapsedMultiwordWordTemp = false;
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	preprocessorUpperLevelWordReference = NULL;
	preprocessorUpperLevelWordReferenceSize = INT_DEFAULT_VALUE;
	preprocessorLowerLevelWordReference = NULL;
	characterIndexInSentenceContentsOriginalText = INT_DEFAULT_VALUE;
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	tagNameLRPforNLP = "";
	#endif
	
	plainTextWord = true;
}
GIApreprocessorMultiwordReductionPlainTextWord::~GIApreprocessorMultiwordReductionPlainTextWord(void)
{
}


/*
GIApreprocessorMultiwordReductionSentence::GIApreprocessorMultiwordReductionSentence(void)
{
	nextSentence = NULL;
}
GIApreprocessorMultiwordReductionSentence::~GIApreprocessorMultiwordReductionSentence(void)
{
}
*/

GIApreprocessorMultiwordReductionBasicSentence::GIApreprocessorMultiwordReductionBasicSentence(void)
{
	firstTagInSentence = new GIApreprocessorMultiwordReductionWord();
	
	nextSentence = NULL;
}
GIApreprocessorMultiwordReductionBasicSentence::~GIApreprocessorMultiwordReductionBasicSentence(void)
{
}

GIApreprocessorMultiwordReductionPhrasalVerbSentence::GIApreprocessorMultiwordReductionPhrasalVerbSentence(void)
{
	firstTagInSentence = new GIApreprocessorMultiwordReductionPhrasalVerbWord();
	alternateSentence = NULL;
	
	nextSentence = NULL;
}
GIApreprocessorMultiwordReductionPhrasalVerbSentence::~GIApreprocessorMultiwordReductionPhrasalVerbSentence(void)
{
}

GIApreprocessorMultiwordReductionIrregularVerbSentence::GIApreprocessorMultiwordReductionIrregularVerbSentence(void)
{
	firstTagInSentence = new GIApreprocessorMultiwordReductionIrregularVerbWord();
	
	nextSentence = NULL;
}
GIApreprocessorMultiwordReductionIrregularVerbSentence::~GIApreprocessorMultiwordReductionIrregularVerbSentence(void)
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




#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES

string GIApreprocessorMultiwordReductionClassClass::generateTextFromPreprocessorSentenceWordList(const GIApreprocessorWord* firstWordInSentence)
{
	return generateTextFromPreprocessorSentenceWordList(firstWordInSentence, false);
}
string GIApreprocessorMultiwordReductionClassClass::generateTextFromPreprocessorSentenceWordList(const GIApreprocessorWord* firstWordInSentence, const bool LRPforNLP)
{
	string sentenceText = "";
	const GIApreprocessorWord* currentWordInSentence = firstWordInSentence;
	bool isFirstWordInSentence = true;
	while(currentWordInSentence->nextTag != NULL)
	{
		const GIApreprocessorWord* word = currentWordInSentence;
		sentenceText = sentenceText + generateTextFromPreprocessorSentenceWord(word, LRPforNLP, isFirstWordInSentence);
		isFirstWordInSentence = false;
		
		currentWordInSentence = currentWordInSentence->nextTag;
	}
	return sentenceText;
}

string GIApreprocessorMultiwordReductionClassClass::generateTextFromVectorWordList(const vector<GIApreprocessorWord*>* wordList)
{
	return generateTextFromVectorWordList(wordList, false);
}
string GIApreprocessorMultiwordReductionClassClass::generateTextFromVectorWordList(const vector<GIApreprocessorWord*>* wordList, const bool LRPforNLP)
{
	string sentenceText = "";
	bool isFirstWordInSentence = true;
	for(int wordIndex = 0; wordIndex<wordList->size(); wordIndex++)
	{
		GIApreprocessorWord* word = (*wordList)[wordIndex];
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION	
		if(isApostrophePossessionOrOmission(word))
		{
			isFirstWordInSentence = true;	//do not prepend a space
		}
		#endif
		sentenceText = sentenceText + generateTextFromPreprocessorSentenceWord(word, LRPforNLP, isFirstWordInSentence);
		isFirstWordInSentence = false;
	}
	return sentenceText;
}

string GIApreprocessorMultiwordReductionClassClass::generateTextFromPreprocessorSentenceWord(const GIApreprocessorWord* word, const bool LRPforNLP, const bool isFirstWordInSentence)
{
	string wordText = word->tagName;
	if(LRPforNLP)
	{
		if(word->plainTextWord)
		{	
			//cout << "word->plainTextWord: " << word->tagName << endl;
			const GIApreprocessorMultiwordReductionPlainTextWord* plaintextWord = static_cast<const GIApreprocessorMultiwordReductionPlainTextWord*>(word);
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
			if(plaintextWord->tagNameLRPforNLP != "")
			{
				wordText = plaintextWord->tagNameLRPforNLP;
			}
			#endif
		}
	}
	bool punctuationMarkFound = false;
	for(int i=0; i<GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS; i++)
	{
		string nlpPunctionMarkCharacterString = "";
		nlpPunctionMarkCharacterString = nlpPunctionMarkCharacterString + nlpPunctionMarkCharacterArray[i];
		if(wordText == nlpPunctionMarkCharacterString)
		{
			punctuationMarkFound = true;
		}
	}
	if(!punctuationMarkFound && !isFirstWordInSentence)
	{
		wordText = string(STRING_SPACE) + wordText;
	}
	return wordText;
}

int GIApreprocessorMultiwordReductionClassClass::calculateLengthOfGeneratedVectorWordListText(vector<GIApreprocessorWord*>* wordList)
{
	string sentenceText = generateTextFromVectorWordList(wordList);
	return sentenceText.length();
}

bool GIApreprocessorMultiwordReductionClassClass::generateSentenceWordList(GIApreprocessorWord* sentenceContentsFirstWord, vector<GIApreprocessorWord*>* wordList)
{
	return generateSentenceWordList(sentenceContentsFirstWord, wordList, GIA_PREPROCESSOR_WORD_LIST_INDEX_NO_REQUIREMENT, GIA_PREPROCESSOR_WORD_LIST_INDEX_NO_REQUIREMENT);
}
bool GIApreprocessorMultiwordReductionClassClass::generateSentenceWordList(GIApreprocessorWord* sentenceContentsFirstWord, vector<GIApreprocessorWord*>* wordList, int startIndex, int endIndex)
{
	bool result = true;
	
	wordList->clear();	//added 3b3i
	
	int index = 0;
	GIApreprocessorWord* currentWordInSentence = sentenceContentsFirstWord;
	while(currentWordInSentence->nextTag != NULL)
	{
		if((startIndex == GIA_PREPROCESSOR_WORD_LIST_INDEX_NO_REQUIREMENT) || (index >= startIndex))
		{
			if((endIndex == GIA_PREPROCESSOR_WORD_LIST_INDEX_NO_REQUIREMENT) || (index < endIndex))
			{
				wordList->push_back(currentWordInSentence);
			}
		}
		
		currentWordInSentence = currentWordInSentence->nextTag;
		index++;
	}
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "GIAtxtRelTranslatorHybridReferenceSetClass::generateSentenceWordList{}: " << endl;
	for(int i=0; i<wordList->size(); i++)
	{
		cout << ((*wordList)[i])->tagName << STRING_SPACE;
	}
	cout << endl;
	#endif
	
	return result;
}

//precondition: words in wordList are unique across all logicReferenceVariableWordLists in memory
bool GIApreprocessorMultiwordReductionClassClass::generateFlatSentenceWordList(const vector<GIApreprocessorWord*>* wordList, GIApreprocessorMultiwordReductionPlainTextWord** sentenceContentsFirstWord)
{
	bool result = true;
	
	if(wordList->size() > 0)
	{
		*sentenceContentsFirstWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((*wordList)[0]);
		GIApreprocessorWord* currentWordInSentence = *sentenceContentsFirstWord;
		for(int i=1; i<wordList->size(); i++)
		{
			GIApreprocessorWord* word = (*wordList)[i];
			if(!(word->plainTextWord))
			{
				cout << "GIApreprocessorMultiwordReductionClassClass::generateFlatSentenceWordList{} warning: (!(word->plainTextWord))" << endl;
			}
			currentWordInSentence->nextTag = word;
			currentWordInSentence = currentWordInSentence->nextTag;
		}
		if(currentWordInSentence->nextTag == NULL)
		{
			//cout << "GIApreprocessorMultiwordReductionClassClass::generateFlatSentenceWordList{}: final (currentWordInSentence->nextTag == NULL)" << endl;
			//cout << "currentWordInSentence->tagName = " << currentWordInSentence->tagName << endl;
			currentWordInSentence->nextTag = new GIApreprocessorMultiwordReductionPlainTextWord();	//add a blank word to end of list (as per standard object list specification)
		}
	}
	
	return result;
}


bool GIApreprocessorMultiwordReductionClassClass::addWordListToWordList(vector<GIApreprocessorWord*>* wordList, vector<GIApreprocessorWord*>* wordListToAdd)
{
	for(int i=0; i<wordListToAdd->size(); i++)
	{
		wordList->push_back((*wordListToAdd)[i]);
	}
}
bool GIApreprocessorMultiwordReductionClassClass::addStringArrayToWordList(vector<GIApreprocessorWord*>* wordList, const string* stringArrayToAdd, const int arraySize)
{
	for(int i=0; i<arraySize; i++)
	{
		GIApreprocessorWord* newWord = new GIApreprocessorMultiwordReductionPlainTextWord(stringArrayToAdd[i]);
		wordList->push_back(newWord);
	}
}
bool GIApreprocessorMultiwordReductionClassClass::addStringToWordList(vector<GIApreprocessorWord*>* wordList, const string stringToAdd)
{
	GIApreprocessorWord* newWord = new GIApreprocessorMultiwordReductionPlainTextWord(stringToAdd);
	wordList->push_back(newWord);
}


#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
bool GIApreprocessorMultiwordReductionClassClass::isIntrawordPunctuationMark(const int indexOfCurrentToken, const string* lineContents)
{
	bool intrawordPunctuationMark = false;
	char currentToken = (*lineContents)[indexOfCurrentToken];
	if((currentToken == CHAR_FULLSTOP) || (currentToken == CHAR_COLON))	//updated 2j6e (added CHAR_COLON for times, eg 06:45)
	{
		if(indexOfCurrentToken < lineContents->length()-1)	//ensure fullstop is not immediately succeded by an alphabetical character, which indicates that the fullstop is part of a filename, eg "people.xml"
		{
			char characterImmediatelySucceedingPunctuationMark = (*lineContents)[indexOfCurrentToken+1];
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			bool isPunctuationMarkImmediatelySucceededByAlphanumericCharacter = SHAREDvars.charInCharArray(characterImmediatelySucceedingPunctuationMark, GIApreprocessorMultiwordReductionNLPparsableCharacters, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES);
			if(isPunctuationMarkImmediatelySucceededByAlphanumericCharacter)
			{
				//e.g. "thefile.exe ..."
				intrawordPunctuationMark = true;
			}
			#endif
			
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_ABBREVIATIONS
			if(currentToken == CHAR_FULLSTOP)
			{
				if(indexOfCurrentToken < lineContents->length()-2)
				{
					if(characterImmediatelySucceedingPunctuationMark == CHAR_SPACE)
					{
						bool characterPreceededByTitlePrefixAbbreviation = false;
						for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_ABBREVIATIONS_TITLE_PREFIXES_NUMBER_OF_TYPES; i++)
						{
							int lengthOfPrefix = preprocessorSupportAbbreviationsTitlePrefixesArray[i].length();
							if(indexOfCurrentToken >= lengthOfPrefix-1)	//-1 to take into account fact that prefixes are defined with a succeeding full stop character
							{
								if(lineContents->substr(indexOfCurrentToken-lengthOfPrefix+1, lengthOfPrefix) == preprocessorSupportAbbreviationsTitlePrefixesArray[i])	//+1 to take into account fact that prefixes are defined with a succeeding full stop character
								{
									characterPreceededByTitlePrefixAbbreviation = true;
									//cout << "characterPreceededByTitlePrefixAbbreviation: preprocessorSupportAbbreviationsTitlePrefixesArray[i] = " << preprocessorSupportAbbreviationsTitlePrefixesArray[i] << endl;
								}
							}
						}
						if(characterPreceededByTitlePrefixAbbreviation)
						{
							//e.g. "Ms. House ..."
							intrawordPunctuationMark = true;
						}

						char characterImmediatelySucceedingSpace = (*lineContents)[indexOfCurrentToken+2];
						bool isPunctuationMarkSucceededBySpaceAndUncapitalisedCharacter = SHAREDvars.charInCharArray(characterImmediatelySucceedingSpace, preprocessorLowerCaseLettersArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_CASE_LETTERS_NUMBER_OF_TYPES);
						if(isPunctuationMarkSucceededBySpaceAndUncapitalisedCharacter)
						{
							//e.g. "e.g. the house ..."
							intrawordPunctuationMark = true;
						}
					}
				}
			}
			#endif
		}
	}
	return intrawordPunctuationMark;
}
#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
bool GIApreprocessorMultiwordReductionClassClass::isApostrophePossessionOrOmission(const int indexOfCurrentToken, const string* lineContents)
{
	bool apostrophePossessionOrOmissionFound = false;
	
	char currentToken = (*lineContents)[indexOfCurrentToken];
	if(currentToken == CHAR_APOSTROPHE)
	{
		if((indexOfCurrentToken >= 1) && (indexOfCurrentToken < lineContents->length()-1))
		{
			char characterImmediatelyPreceedingApostrophe = (*lineContents)[indexOfCurrentToken-1];
			char characterImmediatelySucceedingApostrophe = (*lineContents)[indexOfCurrentToken+1];
			bool characterImmediatelyPreceedingApostropheIsLetter = SHAREDvars.charInCharArray(characterImmediatelyPreceedingApostrophe, preprocessorLowerOrUpperCaseLettersArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_OR_UPPER_CASE_LETTERS_NUMBER_OF_TYPES);
			bool characterImmediatelySucceedingApostropheIsLetter = SHAREDvars.charInCharArray(characterImmediatelySucceedingApostrophe, preprocessorLowerOrUpperCaseLettersArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_OR_UPPER_CASE_LETTERS_NUMBER_OF_TYPES);			
			if(characterImmediatelyPreceedingApostropheIsLetter && characterImmediatelySucceedingApostropheIsLetter)
			{
				apostrophePossessionOrOmissionFound = true;
			}
		}
	}
	
	return apostrophePossessionOrOmissionFound;	
}
bool GIApreprocessorMultiwordReductionClassClass::isApostrophePossessionOrOmission(const GIApreprocessorWord* word)
{
	bool apostrophePossessionOrOmissionFound = false;
	
	const string wordText = word->tagName;
	if(wordText.length() > 0)
	{
		if((wordText[0] == CHAR_APOSTROPHE) && (wordText[wordText.length()-1] != CHAR_APOSTROPHE))
		{
			apostrophePossessionOrOmissionFound = true;
		}
	}
	
	return apostrophePossessionOrOmissionFound;	
}
#endif			
		
		

bool GIApreprocessorMultiwordReductionClassClass::findAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind,  const string stringSimpleReplacement)
{
	return findAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(wordList, &stringSimpleToFind, &stringSimpleReplacement);
}
bool GIApreprocessorMultiwordReductionClassClass::findAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string* stringSimpleToFind,  const string* stringSimpleReplacement)
{
	bool result = false;
	
	//cout << "findAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring{}: wordList before: " << generateTextFromVectorWordList(wordList) << ", stringSimpleToFind = " << *stringSimpleToFind << ", stringSimpleReplacement = " << *stringSimpleReplacement  << endl;

	vector<GIApreprocessorWord*> wordListToFind;
	generateSentenceWordListFromStringSimple(&wordListToFind, stringSimpleToFind);
	vector<GIApreprocessorWord*> wordListReplacement;
	generateSentenceWordListFromStringSimple(&wordListReplacement, stringSimpleReplacement);
	
	for(int w = 0; w < wordList->size(); w++)
	{
		if(findAndReplaceWordListInWordListAtIndexWithWordList(wordList, &wordListToFind, w, &wordListReplacement))
		{
			w = w + wordListReplacement.size()-1;
			result = true;
		}
	}
		
	return result;
}
bool GIApreprocessorMultiwordReductionClassClass::findAndReplaceSimpleSubstringInWordListAtIndexWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind, const int indexToPerformFind, const string stringSimpleReplacement)
{
	bool result = false;
	
	vector<GIApreprocessorWord*> wordListToFind;
	generateSentenceWordListFromStringSimple(&wordListToFind, &stringSimpleToFind);
	vector<GIApreprocessorWord*> wordListReplacement;
	generateSentenceWordListFromStringSimple(&wordListReplacement, &stringSimpleReplacement);
	
	if(findAndReplaceWordListInWordListAtIndexWithWordList(wordList, &wordListToFind, indexToPerformFind, &wordListReplacement))
	{
		result = true;
	}
		
	return result;
}
bool GIApreprocessorMultiwordReductionClassClass::findAndReplaceWordListInWordListAtIndexWithWordList(vector<GIApreprocessorWord*>* wordList, vector<GIApreprocessorWord*>* wordListToFind, const int indexToPerformFind, vector<GIApreprocessorWord*>* wordListReplacement)
{
	bool result = false;
	
	//cout << "findAndReplaceWordListInWordListAtIndexWithWordList{}: wordList before: " << generateTextFromVectorWordList(wordList) << ", wordListToFind = " << generateTextFromVectorWordList(wordListToFind) << ", wordListReplacement = " << generateTextFromVectorWordList(wordListReplacement)  << endl;
	
	bool caseInsensitive = false;
	if(findSubWordListInWordListAtIndex(wordList, wordListToFind, indexToPerformFind, caseInsensitive))
	{
		removeWordsFromWordList(wordList, indexToPerformFind, wordListToFind->size());
		insertWordListIntoWordList(wordList, wordListReplacement, indexToPerformFind);
		result = true;
	}
	
	//cout << "findAndReplaceWordListInWordListAtIndexWithWordList{}: wordList after: " << generateTextFromVectorWordList(wordList) << endl;
	
	return result;
}


bool GIApreprocessorMultiwordReductionClassClass::findSimpleSubstringInWordList(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind)
{
	const int startIndexToPerformFind = 0;
	return findSimpleSubstringInWordList(wordList, stringSimpleToFind, startIndexToPerformFind);
}
bool GIApreprocessorMultiwordReductionClassClass::findSimpleSubstringInWordList(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind, const int startIndexToPerformFind)
{
	bool result = false;
	
	vector<GIApreprocessorWord*> wordListToFind;
	generateSentenceWordListFromStringSimple(&wordListToFind, &stringSimpleToFind);
	
	if(findSubWordListInWordList(wordList, &wordListToFind, startIndexToPerformFind))
	{
		result = true;
	}
		
	return result;
}
bool GIApreprocessorMultiwordReductionClassClass::findSubWordListInWordList(vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToFind, const int startIndexToPerformFind)
{
	bool result = false;
	
	for(int i=startIndexToPerformFind; i<wordList->size(); i++)
	{
		bool caseInsensitive = false;
		if(findSubWordListInWordListAtIndex(wordList, wordListToFind, i, caseInsensitive))
		{
			result = true;
		}
	}
		
	return result;
}
bool GIApreprocessorMultiwordReductionClassClass::findSimpleSubstringInWordListAtIndex(const vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind, const int indexToPerformFind, const bool caseInsensitive)
{
	bool result = false;
	
	vector<GIApreprocessorWord*> wordListToFind;
	generateSentenceWordListFromStringSimple(&wordListToFind, &stringSimpleToFind);
	
	if(findSubWordListInWordListAtIndex(wordList, &wordListToFind, indexToPerformFind, caseInsensitive))
	{
		result = true;
	}
		
	return result;
}
bool GIApreprocessorMultiwordReductionClassClass::findSubWordListInWordListAtIndex(const vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToFind, const int indexToPerformFind, const bool caseInsensitive)
{
	bool result = false;
	
	if(indexToPerformFind+wordListToFind->size() <= wordList->size())
	{
		bool stillFindingWordMatches = true;
		int i = 0;
		while((i < wordListToFind->size()) && stillFindingWordMatches)
		{
			string string1 = (*wordList)[i+indexToPerformFind]->tagName;
			string string2 = (*wordListToFind)[i]->tagName;
			if(caseInsensitive)
			{
				string1 = SHAREDvars.convertStringToLowerCase(&string1);
				string2 = SHAREDvars.convertStringToLowerCase(&string2);
			}
			if(string1 != string2)
			{
				stillFindingWordMatches = false;
			}
			i++;
		}
		if(stillFindingWordMatches)
		{
			result = true;
		}
	}
	/*
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::findSubWordListInWordListAtIndex error{}: !(indexToPerformFind+wordListToFind->size() <= wordList->size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
			
	return result;
}

bool GIApreprocessorMultiwordReductionClassClass::generateSentenceWordListFromStringSimple(vector<GIApreprocessorWord*>* wordList, const string* stringSimple)
{
	bool result = true;
	
	string currentWord = "";
	for(int i=0; i < stringSimple->length(); i++)
	{
		char c = (*stringSimple)[i];
		if((c == CHAR_SPACE) || (i == stringSimple->length()-1))
		{
			if(i == stringSimple->length()-1)
			{
				currentWord = currentWord + c;
			}
			addStringToWordList(wordList, currentWord);
			//cout << "generateSentenceWordListFromStringSimple: currentWord = " << currentWord << endl;
			currentWord = "";
		}
		else
		{
			currentWord = currentWord + c;
		}
	}
	
	return result;
}


int GIApreprocessorMultiwordReductionClassClass::findStringInWordList(const vector<GIApreprocessorWord*>* wordList, const string stringToFind)
{
	const int startIndexToPerformFind = 0;
	return findStringInWordList(wordList, stringToFind, startIndexToPerformFind);
}
int GIApreprocessorMultiwordReductionClassClass::findStringInWordList(const vector<GIApreprocessorWord*>* wordList, const string stringToFind, const int startIndexToPerformFind)
{
	bool result  = false;
	int indexOfWordFound = CPP_STRING_FIND_RESULT_FAIL_VALUE;
	
	if(startIndexToPerformFind < wordList->size())
	{
		for(int i=startIndexToPerformFind; i<wordList->size(); i++)
		{
			if(!result)
			{//return first instance
				string currentWord = (*wordList)[i]->tagName;
				if(currentWord == stringToFind)
				{
					result = true;
					indexOfWordFound = i;
				}
			}
		}
	}
	/*
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::findStringInWordList error{}: !(startIndexToPerformFind < wordList->size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
			
	return indexOfWordFound;
}

bool GIApreprocessorMultiwordReductionClassClass::findSubstringAtStartOfWordInWordList(const vector<GIApreprocessorWord*>* wordList, const string substringToFind)
{
	const int startIndexToPerformFind = 0;
	return findSubstringAtStartOfWordInWordList(wordList, substringToFind, startIndexToPerformFind);
}
bool GIApreprocessorMultiwordReductionClassClass::findSubstringAtStartOfWordInWordList(const vector<GIApreprocessorWord*>* wordList, const string substringToFind, const int startIndexToPerformFind)
{
	bool result  = false;
	//int indexOfWordFound = CPP_STRING_FIND_RESULT_FAIL_VALUE;
	
	if(startIndexToPerformFind < wordList->size())
	{
		for(int i=startIndexToPerformFind; i<wordList->size(); i++)
		{
			if(!result)
			{//return first instance
				string currentWord = (*wordList)[i]->tagName;
				if(currentWord.find(substringToFind) == 0)
				{
					result = true;
					//indexOfWordFound = i;
				}
			}
		}
	}
	/*
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::findSubstringAtStartOfWordInWordList error{}: !(startIndexToPerformFind < wordList->size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
			
	return result;
}



vector<GIApreprocessorWord*> GIApreprocessorMultiwordReductionClassClass::extractSubWordListInWordList(const vector<GIApreprocessorWord*>* wordList, const int startIndexToExtract)
{
	int numberOfWordsToExtract = wordList->size() - startIndexToExtract;
	return extractSubWordListInWordList(wordList, startIndexToExtract, numberOfWordsToExtract);
}
vector<GIApreprocessorWord*> GIApreprocessorMultiwordReductionClassClass::extractSubWordListInWordList(const vector<GIApreprocessorWord*>* wordList, const int startIndexToExtract, const int numberOfWordsToExtract)
{
	bool result = false;
	vector<GIApreprocessorWord*> wordListSub;
	
	if(startIndexToExtract+numberOfWordsToExtract <= wordList->size())
	{
		result = true;
		for(int i=startIndexToExtract; i<startIndexToExtract+numberOfWordsToExtract; i++)
		{
			wordListSub.push_back((*wordList)[i]);
		}
	}
	/*
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::extractSubWordListInWordList error{}: !(startIndexToExtract+numberOfWordsToExtract <= wordList->size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
			
	return wordListSub;
}

bool GIApreprocessorMultiwordReductionClassClass::insertWordListIntoWordList(vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToInsert, const int indexToInsert)
{
	bool result = false;
		
	if(indexToInsert <= wordList->size())
	{
		result = true;
		for(int i=0; i<wordListToInsert->size(); i++)
		{
			vector<GIApreprocessorWord*>::iterator iter = wordList->begin() + indexToInsert + i;
			wordList->insert(iter, (*wordListToInsert)[i]);				
		}
	}
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::insertWordIntoWordList error{}: !(indexToInsert <= wordList->size())" << endl;
		exit(EXIT_ERROR);
	}
		
	return result;
}
bool GIApreprocessorMultiwordReductionClassClass::insertStringIntoWordList(vector<GIApreprocessorWord*>* wordList, const string stringToInsert, const int indexToInsert)
{
	bool result = true;
	GIApreprocessorWord* wordToInsert = new GIApreprocessorMultiwordReductionPlainTextWord(stringToInsert);
	if(!insertWordIntoWordList(wordList, wordToInsert, indexToInsert))
	{
		result = false;
	}
		
	return result;
}
//why can't this be declared const?; GIApreprocessorWord* wordToInsert
bool GIApreprocessorMultiwordReductionClassClass::insertWordIntoWordList(vector<GIApreprocessorWord*>* wordList, GIApreprocessorWord* wordToInsert, const int indexToInsert)
{
	bool result = false;
	
	if(indexToInsert <= wordList->size())
	{
		result = true;
		vector<GIApreprocessorWord*>::iterator iter = wordList->begin() + indexToInsert;
		wordList->insert(iter, wordToInsert);	
	}
	/*
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::insertWordIntoWordList error{}: !(indexToInsert <= wordList->size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
		
	return result;
}

bool GIApreprocessorMultiwordReductionClassClass::wordListFindAndRemoveAllOccurancesSimpleSubstringInWordList(vector<GIApreprocessorWord*>* wordList, const string stringToFind)
{
	bool result = false;
	
	bool stillFinding = true;
	for(int i=0; i<wordList->size(); i++)
	{
		string currentWord = (*wordList)[i]->tagName;
		if(currentWord == stringToFind)
		{
			removeWordFromWordList(wordList, i);
			i = i - 1;
		}
	}
	return result;
}
bool GIApreprocessorMultiwordReductionClassClass::removeWordFromWordList(vector<GIApreprocessorWord*>* wordList, const int indexToRemove)
{
	return removeWordsFromWordList(wordList, indexToRemove, 1);
}
bool GIApreprocessorMultiwordReductionClassClass::removeWordsFromWordList(vector<GIApreprocessorWord*>* wordList, const int indexToRemove, const int numberElementsToRemove)
{
	bool result = false;

	if(indexToRemove+numberElementsToRemove <= wordList->size())
	{
		result = true;
		wordList->erase((wordList->begin() + indexToRemove), (wordList->begin() + indexToRemove+numberElementsToRemove));
	}
	/*
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::removeWordsFromWordList error{}: !(indexToRemove+numberElementsToRemove <= wordList->size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
		
	return result;
}


bool GIApreprocessorMultiwordReductionClassClass::replaceWordListAtIndexWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const int indexToPerformReplacement, const string stringSimpleReplacement)
{
	bool result = true;
	
	vector<GIApreprocessorWord*> wordListReplacement;
	generateSentenceWordListFromStringSimple(&wordListReplacement, &stringSimpleReplacement);
	
	if(!replaceWordListAtIndexWithSimpleSubstring(wordList, indexToPerformReplacement, &wordListReplacement))
	{
		result = false;
	}
	
	return result;
}

bool GIApreprocessorMultiwordReductionClassClass::replaceWordListAtIndexWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const int indexToPerformReplacement, vector<GIApreprocessorWord*>* wordListReplacement)
{
	bool result = false;
	
	if(indexToPerformReplacement+wordListReplacement->size() <= wordList->size())
	{
		result = true;
		for(int i=0; i<wordListReplacement->size(); i++)
		{
			(*wordList)[indexToPerformReplacement+i]->tagName = (*wordListReplacement)[i]->tagName;
		}
	}
	/*
	else
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::replaceWordListAtIndexWithSimpleSubstring error{}: !(indexToPerformReplacement+wordListReplacement.size() <= wordList.size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
		
	return result;
}


bool GIApreprocessorMultiwordReductionClassClass::printWordList(const vector<GIApreprocessorWord*>* wordList)
{
	cerr << printWordListString(wordList) << endl;
}
string GIApreprocessorMultiwordReductionClassClass::printWordListString(const vector<GIApreprocessorWord*>* wordList)
{
	string output = "";
	for(int i=0; i<wordList->size(); i++)
	{
		output = output + ((*wordList)[i])->tagName + STRING_SPACE;
	}
	return output;
}

#endif			


void GIApreprocessorMultiwordReductionClassClass::preprocessorFillCurrentWord(GIApreprocessorMultiwordReductionPlainTextWord** currentWordInSentence, string* currentWord, int* entityIndex, int lastCharacterIndexOfWordInSentence)
{
	(*currentWordInSentence)->tagName = *currentWord;
	(*currentWordInSentence)->entityIndex = *entityIndex;
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	(*currentWordInSentence)->characterIndexInSentenceContentsOriginalText = lastCharacterIndexOfWordInSentence - currentWord->size() - 1;
	#endif
	(*currentWordInSentence)->nextTag = new GIApreprocessorMultiwordReductionPlainTextWord();
	(*currentWordInSentence) = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((*currentWordInSentence)->nextTag);
	(*entityIndex) = (*entityIndex) + 1;
	*currentWord = "";
}

int GIApreprocessorMultiwordReductionClassClass::getPOStypeFromName(const string wordPOStypeName)
{	
	int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
	if(!SHAREDvars.textInTextArray(wordPOStypeName, GIApreprocessorPOStypeNameArray, GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype))
	{
		cerr << "GIApreprocessorMultiwordReductionClassClass::getPOStypeFromName{} error: GIA LRP wordPOStypeName not found in GIApreprocessorPOStypeNameArray, wordPOStypeName = " << wordPOStypeName << endl;
		exit(EXIT_ERROR);
	}
	return wordPOStype;
}

//based on NLCpreprocessorSentenceClassClass::isStringNumberPreprocessorMath(string phrase)
bool GIApreprocessorMultiwordReductionClassClass::isStringNumber(const string phrase)
{
	bool stringIsNumber = false;
	if(phrase == STRING_FULLSTOP)
	{
		stringIsNumber = false;
	}
	else
	{
		stringIsNumber = true;
		for(int i=0; i<phrase.length(); i++)
		{
			char c = phrase[i];
			bool numberFound = SHAREDvars.charInCharArray(c, translatorEnglishNumbersNumericalFractionalArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMERICAL_FRACTIONAL_NUMBER_OF_TYPES);
			if(!numberFound)
			{
				stringIsNumber = false;
			}
		}
	}
	
	return stringIsNumber;
}

