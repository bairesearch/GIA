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
 * File Name: GIApreprocessorReferenceSet.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e12a 12-February-2018
 * Requirements: requires plain text file
 * Description: Reference Set preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_REFERENCE_SET
#define HEADER_GIA_PREPROCESSOR_REFERENCE_SET

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"

class GIApreprocessorReferenceSetClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	public: bool generateSentenceWordList(GIApreprocessorMultiwordReductionWord* sentenceContentsFirstWord, vector<GIApreprocessorWord*>* logicReferenceVariableWordList);
	public: bool executeReferenceSetPreprocessor(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, const bool expectToFindSubjectAuxObjectLogicReferenceVariable, const int wordIndexSentence);
			public: bool detectAuxiliary(const string currentWord);
			public: bool detectModalAuxiliary(const string currentWord);
			public: bool detectRcmodSameReferenceSetDelimiter(const string currentWord);
			#ifdef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
			private: bool determineIsVerbAndAdjective(GIApreprocessorWord* currentWordTag, bool usePOSprelim, bool grammaticallyStrict);
			#endif
			private: void updateIndices(const bool currentWordIsReferenceSetDelimiter, bool* referenceSetDelimiterDetected, bool* parsingReferenceSetDelimiter, const int currentDelimiterSpecialCase, const int wordIndexOfHypotheticalPreceedingThatWhich, const bool currentWordIsReferenceSetDelimiterPreceededByThatWhich, bool* referenceSetDelimiterIndicatesSameReferenceSet, int* firstIndexOfReferenceSetDelimiterText, int* lastIndexOfPreviousReferenceSet);
			private: bool formSubReferenceSetTextFromWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, vector<GIApreprocessorWord*>* subReferenceSetText, const int firstIndexOfSubReferenceSet, const int lastIndexOfSubReferenceSet);
			private: bool addReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, vector<GIApreprocessorWord*>* subReferenceSetText, const bool referenceSetDelimiter, const int wordIndexLogicReference, const int wordIndexSentence, const int delimiterType, const int delimiterSpecialCase);
				private: bool addSubReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, vector<GIApreprocessorWord*>* subReferenceSetText, const bool referenceSetDelimiter, const int wordIndexLogicReference, const int wordIndexSentence, const int delimiterType, const int delimiterSpecialCase);
	public: bool hasReferenceSet(GIApreprocessorSubReferenceSet* referenceSet);
	public: void setReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet, vector<GIApreprocessorWord*>* text);
	public: vector<GIApreprocessorWord*> getReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet);
	public: void printSubReferenceSet(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList);
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
	public: bool verifyIsPrepositionNotProgressiveVerbBasedOnContext(const vector<GIApreprocessorWord*>* sentenceContentsWordList, const int prepositionWordIndex, const bool usePOSprelim);
	#endif
};

#endif

