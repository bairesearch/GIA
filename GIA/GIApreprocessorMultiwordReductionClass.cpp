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
 * Project Version: 3b2b 21-May-2017
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 *
 *******************************************************************************/


#include "GIApreprocessorMultiwordReductionClass.hpp"


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

	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIApreprocessorMultiwordReductionWord* preprocessorUpperLevelWordReference;
	int preprocessorUpperLevelWordReferenceSize;	//number of words in preprocessor upper level phrase corresponding to preprocessor word
	#endif
		
	alternateTag = NULL;
	nextTag = NULL;
}
GIApreprocessorMultiwordReductionWord::~GIApreprocessorMultiwordReductionWord(void)
{
}


#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
string GIApreprocessorMultiwordReductionClassClass::generateTextFromPreprocessorSentenceWordList(GIApreprocessorMultiwordReductionWord* firstWordInSentence)
{
	string sentenceText = "";
	GIApreprocessorMultiwordReductionWord* currentWordInSentence = firstWordInSentence;
	bool firstCharacterInSentence = true;
	while(currentWordInSentence->nextTag != NULL)
	{
		string wordText = currentWordInSentence->tagName;
		
		bool punctuationMarkFound = false;
		for(int i=0; i<GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS; i++)
		{
			string nlpPunctionMarkCharacterString = "";
			nlpPunctionMarkCharacterString = nlpPunctionMarkCharacterString + nlpPunctionMarkCharacterArray[i];
			if(currentWordInSentence->tagName == nlpPunctionMarkCharacterString)
			{
				punctuationMarkFound = true;
			}
		}
		if(!punctuationMarkFound && !firstCharacterInSentence)
		{
			wordText = string(STRING_SPACE) + wordText;
		}
		sentenceText = sentenceText + wordText;
			
		currentWordInSentence = currentWordInSentence->nextTag;
	}
	return sentenceText;
}
#endif




