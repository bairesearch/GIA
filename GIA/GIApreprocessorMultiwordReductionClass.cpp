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
 * File Name: GIApreprocessorMultiwordReductionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3b3i 25-May-2017
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 *
 *******************************************************************************/


#include "GIApreprocessorMultiwordReductionClass.hpp"

GIApreprocessorWord::GIApreprocessorWord(void)
{
	tagName = "";

	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	featureReferenceOriginal = NULL;
	featureReference = NULL;
	entityReference = NULL;
	#endif
			
	nextTag = NULL;
	
	plainTextWord = false;
}
GIApreprocessorWord::GIApreprocessorWord(string tagNameNew)
{
	tagName = tagNameNew;

	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	featureReferenceOriginal = NULL;
	featureReference = NULL;
	entityReference = NULL;
	#endif
	
	plainTextWord = false;
		
	nextTag = NULL;
}
GIApreprocessorWord::~GIApreprocessorWord(void)
{
}




GIApreprocessorMultiwordReductionWord::GIApreprocessorMultiwordReductionWord(void)
{
	tagNameLemma = "";

	base = false;
	for(int i=0; i<GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS; i++)
	{
		grammaticalTenseFormsArray[i][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD] = "";
		grammaticalTenseFormsArray[i][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE] = "";
	}
	grammaticalTenseFormDetected = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE;
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
}

GIApreprocessorMultiwordReductionPlainTextWord::GIApreprocessorMultiwordReductionPlainTextWord(void)
{	
	entityIndex = false;
	collapsedPhrasalVerbExactDefinedSection = false;
	collapsedMultiwordWord = false;
	collapsedMultiwordWordType = GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_UNDEFINED_TYPE;
	collapsedMultiwordWordTemp = false;
	
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

string GIApreprocessorMultiwordReductionClassClass::generateTextFromVectorWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList)
{
	return generateTextFromVectorWordList(logicReferenceVariableWordList, false);
}
string GIApreprocessorMultiwordReductionClassClass::generateTextFromVectorWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, const bool LRPforNLP)
{
	string sentenceText = "";
	bool isFirstWordInSentence = true;
	for(int wordIndex = 0; wordIndex<logicReferenceVariableWordList->size(); wordIndex++)
	{
		GIApreprocessorWord* word = (*logicReferenceVariableWordList)[wordIndex];
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
			if(plaintextWord->tagNameLRPforNLP != "")
			{
				wordText = plaintextWord->tagNameLRPforNLP;
			}
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

int GIApreprocessorMultiwordReductionClassClass::calculateLengthOfGeneratedVectorWordListText(vector<GIApreprocessorWord*>* logicReferenceVariableWordList)
{
	string sentenceText = generateTextFromVectorWordList(logicReferenceVariableWordList);
	return sentenceText.length();
}

bool GIApreprocessorMultiwordReductionClassClass::generateSentenceWordList(GIApreprocessorWord* sentenceContentsFirstWord, vector<GIApreprocessorWord*>* logicReferenceVariableWordList)
{
	bool result = true;
	
	logicReferenceVariableWordList->clear();	//added 3b3i
	
	GIApreprocessorWord* currentWordInSentence = sentenceContentsFirstWord;
	while(currentWordInSentence->nextTag != NULL)
	{
		logicReferenceVariableWordList->push_back(currentWordInSentence);
		currentWordInSentence = currentWordInSentence->nextTag;
	}
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "GIApreprocessorReferenceSetClass::generateSentenceWordList{}: " << endl;
	for(int i=0; i<logicReferenceVariableWordList->size(); i++)
	{
		cout << ((*logicReferenceVariableWordList)[i])->tagName << STRING_SPACE;
	}
	cout << endl;
	#endif
	
	return result;
}

//precondition: words in logicReferenceVariableWordList are unique across all logicReferenceVariableWordLists in memory
bool GIApreprocessorMultiwordReductionClassClass::generateFlatSentenceWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, GIApreprocessorMultiwordReductionPlainTextWord** sentenceContentsFirstWord)
{
	bool result = true;
	
	if(logicReferenceVariableWordList->size() > 0)
	{
		*sentenceContentsFirstWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((*logicReferenceVariableWordList)[0]);
		GIApreprocessorWord* currentWordInSentence = *sentenceContentsFirstWord;
		for(int i=1; i<logicReferenceVariableWordList->size(); i++)
		{
			GIApreprocessorWord* word = (*logicReferenceVariableWordList)[i];
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
		cout << "GIApreprocessorMultiwordReductionClassClass::findSubWordListInWordListAtIndex error{}: !(indexToPerformFind+wordListToFind->size() <= wordList->size())" << endl;
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
		cout << "GIApreprocessorMultiwordReductionClassClass::findStringInWordList error{}: !(startIndexToPerformFind < wordList->size())" << endl;
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
		cout << "GIApreprocessorMultiwordReductionClassClass::findSubstringAtStartOfWordInWordList error{}: !(startIndexToPerformFind < wordList->size())" << endl;
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
		cout << "GIApreprocessorMultiwordReductionClassClass::extractSubWordListInWordList error{}: !(startIndexToExtract+numberOfWordsToExtract <= wordList->size())" << endl;
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
		cout << "GIApreprocessorMultiwordReductionClassClass::insertWordIntoWordList error{}: !(indexToInsert <= wordList->size())" << endl;
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
		cout << "GIApreprocessorMultiwordReductionClassClass::insertWordIntoWordList error{}: !(indexToInsert <= wordList->size())" << endl;
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
		cout << "GIApreprocessorMultiwordReductionClassClass::removeWordsFromWordList error{}: !(indexToRemove+numberElementsToRemove <= wordList->size())" << endl;
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
		cout << "GIApreprocessorMultiwordReductionClassClass::replaceWordListAtIndexWithSimpleSubstring error{}: !(indexToPerformReplacement+wordListReplacement.size() <= wordList.size())" << endl;
		exit(EXIT_ERROR);
	}
	*/
		
	return result;
}


#endif			



