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
 * Project Version: 3b3a 25-May-2017
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

bool GIApreprocessorMultiwordReductionClassClass::generateSentenceWordList(GIApreprocessorMultiwordReductionWord* sentenceContentsFirstWord, vector<GIApreprocessorWord*>* logicReferenceVariableWordList)
{
	bool result = true;
	
	GIApreprocessorMultiwordReductionWord* currentWordInSentence = sentenceContentsFirstWord;
	while(currentWordInSentence->nextTag != NULL)
	{
		logicReferenceVariableWordList->push_back(currentWordInSentence);

		currentWordInSentence = static_cast<GIApreprocessorMultiwordReductionWord*>(currentWordInSentence->nextTag);
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
		GIApreprocessorWord* newWord = new GIApreprocessorWord(stringArrayToAdd[i]); 
		wordList->push_back(newWord);
	}
}
bool GIApreprocessorMultiwordReductionClassClass::addStringToWordList(vector<GIApreprocessorWord*>* wordList, const string stringToAdd)
{
	GIApreprocessorWord* newWord = new GIApreprocessorWord(stringToAdd); 
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
#endif			
		
		
#endif


bool GIApreprocessorMultiwordReductionClassClass::wordListFindAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind,  const string stringSimpleReplacement)
{
	return wordListFindAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(wordList, &stringSimpleToFind, &stringSimpleReplacement);
}
bool GIApreprocessorMultiwordReductionClassClass::wordListFindAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string* stringSimpleToFind,  const string* stringSimpleReplacement)
{
	bool result = false;
	
	//cout << "wordListFindAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring{}: wordList before: " << generateTextFromVectorWordList(wordList) << ", stringSimpleToFind = " << *stringSimpleToFind << ", stringSimpleReplacement = " << *stringSimpleReplacement  << endl;

	vector<GIApreprocessorWord*> wordListToFind;
	generateSentenceWordListFromStringSimple(&wordListToFind, stringSimpleToFind);
	vector<GIApreprocessorWord*> wordListReplacement;
	generateSentenceWordListFromStringSimple(&wordListReplacement, stringSimpleReplacement);
	
	for(int w = 0; w < wordList->size(); w++)
	{
		if(wordListFindAndReplaceWordListInWordListWithWordList(wordList, &wordListToFind, w, &wordListReplacement))
		{
			w = w + wordListReplacement.size()-1;
			result = true;
		}
	}
		
	return result;
}


bool GIApreprocessorMultiwordReductionClassClass::wordListFindAndReplaceSimpleSubstringInWordListWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind, const int startIndexToPerformFind, const string stringSimpleReplacement)
{
	bool result = false;
	
	vector<GIApreprocessorWord*> wordListToFind;
	generateSentenceWordListFromStringSimple(&wordListToFind, &stringSimpleToFind);
	vector<GIApreprocessorWord*> wordListReplacement;
	generateSentenceWordListFromStringSimple(&wordListReplacement, &stringSimpleReplacement);
	
	if(wordListFindAndReplaceWordListInWordListWithWordList(wordList, &wordListToFind, startIndexToPerformFind, &wordListReplacement))
	{
		result = true;
	}
		
	return result;
}

//limiations: sequence must be found at startIndexToPerformFind (not after it)
bool GIApreprocessorMultiwordReductionClassClass::wordListFindAndReplaceWordListInWordListWithWordList(vector<GIApreprocessorWord*>* wordList, vector<GIApreprocessorWord*>* wordListToFind, const int startIndexToPerformFind, vector<GIApreprocessorWord*>* wordListReplacement)
{
	bool result = false;
	
	//cout << "wordListFindAndReplaceWordListInWordListWithWordList{}: wordList before: " << generateTextFromVectorWordList(wordList) << ", wordListToFind = " << generateTextFromVectorWordList(wordListToFind) << ", wordListReplacement = " << generateTextFromVectorWordList(wordListReplacement)  << endl;
	
	if(wordListFindSubWordListInWordList(wordList, wordListToFind, startIndexToPerformFind))
	{
		removeWordsFromWordList(wordList, startIndexToPerformFind, wordListToFind->size());
		insertWordListIntoWordList(wordList, wordListReplacement, startIndexToPerformFind);
		result = true;
	}
	
	//cout << "wordListFindAndReplaceWordListInWordListWithWordList{}: wordList after: " << generateTextFromVectorWordList(wordList) << endl;
	
	return result;
}

//limiations: sequence must be found at startIndexToPerformFind (not after it)
bool GIApreprocessorMultiwordReductionClassClass::wordListFindSubWordListInWordList(const vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToFind, int startIndexToPerformFind)
{
	bool result = false;
	
	if(startIndexToPerformFind+wordListToFind->size() <= wordList->size())
	{
		bool stillFindingWordMatches = true;
		int i = 0;
		while((i < wordListToFind->size()) && stillFindingWordMatches)
		{
			if((*wordList)[i+startIndexToPerformFind]->tagName != (*wordListToFind)[i]->tagName)
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
		
	return indexOfWordFound;
}

bool GIApreprocessorMultiwordReductionClassClass::findSubstringInWordList(const vector<GIApreprocessorWord*>* wordList, const string substringToFind)
{
	const int startIndexToPerformFind = 0;
	return findSubstringInWordList(wordList, substringToFind, startIndexToPerformFind);
}
bool GIApreprocessorMultiwordReductionClassClass::findSubstringInWordList(const vector<GIApreprocessorWord*>* wordList, const string substringToFind, const int startIndexToPerformFind)
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
				if(currentWord.find(substringToFind) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					result = true;
					//indexOfWordFound = i;
				}
			}
		}
	}
		
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
		
	return wordListSub;
}

bool GIApreprocessorMultiwordReductionClassClass::insertWordListIntoWordList(vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToInsert, const int indexToInsert)
{
	bool result = false;
		
	if(indexToInsert+wordListToInsert->size() <= wordList->size())
	{
		result = true;
		for(int i=0; i<wordListToInsert->size(); i++)
		{
			vector<GIApreprocessorWord*>::iterator iter = wordList->begin() + indexToInsert + i;
			wordList->insert(iter, (*wordListToInsert)[i]);				
		}
	}
		
	return result;
}

bool GIApreprocessorMultiwordReductionClassClass::insertStringIntoWordList(vector<GIApreprocessorWord*>* wordList, const string stringToInsert, const int indexToInsert)
{
	bool result = true;
	GIApreprocessorWord* wordListToInsert = new GIApreprocessorWord(stringToInsert);
	if(!insertWordIntoWordList(wordList, wordListToInsert, indexToInsert))
	{
		result = false;
	}
		
	return result;
}
//why can't this be declared const?; GIApreprocessorWord* wordListToInsert
bool GIApreprocessorMultiwordReductionClassClass::insertWordIntoWordList(vector<GIApreprocessorWord*>* wordList, GIApreprocessorWord* wordListToInsert, const int indexToInsert)
{
	bool result = false;
	
	if(indexToInsert < wordList->size())
	{
		result = true;
		vector<GIApreprocessorWord*>::iterator iter = wordList->begin() + indexToInsert;
		wordList->insert(iter, wordListToInsert);	
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
		
	return result;
}





				



