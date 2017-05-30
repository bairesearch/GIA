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
 * Project Version: 3b2g 21-May-2017
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

string GIApreprocessorMultiwordReductionClassClass::generateTextFromVectorWordList(vector<GIApreprocessorWord*>* logicReferenceVariableWordList)
{
	return generateTextFromVectorWordList(logicReferenceVariableWordList, false);
}
string GIApreprocessorMultiwordReductionClassClass::generateTextFromVectorWordList(vector<GIApreprocessorWord*>* logicReferenceVariableWordList, const bool LRPforNLP)
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

#endif




