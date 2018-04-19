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
 * File Name: GIApreprocessorSentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e12b 12-February-2018
 * Requirements: requires plain text file
 * Description: Logical Condition and Reference Set preprocessor
 *
 *******************************************************************************/


#include "GIApreprocessorSentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"


#ifdef GIA_PREPROCESSOR_SENTENCE

GIApreprocessorSubReferenceSet::GIApreprocessorSubReferenceSet(void)
{
	sentenceIndex = INT_DEFAULT_VALUE;
	//subReferenceSetContents = "";
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
	//subReferenceSetContentsOutputForNLP = "";
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	firstIndexOfReferenceSetText = INT_DEFAULT_VALUE;
	lastIndexOfReferenceSetText = INT_DEFAULT_VALUE;
	dummyNLPtestOffset = 0;
	#endif
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	primaryEntityTemp = NULL;
	#endif
	
	isReferenceSetDelimiter = false;	
	next = NULL;
	
	delimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_UNDEFINED;
	delimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_UNDEFINED;
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	sentenceReference = NULL;
	#endif
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
	definite = false;
	#endif

	isFirstSubreferenceSetInList = false;
}
GIApreprocessorSubReferenceSet::~GIApreprocessorSubReferenceSet(void)
{
}

GIApreprocessorLogicReferenceVariable::GIApreprocessorLogicReferenceVariable(void)
{
	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
	sentenceIndex = INT_DEFAULT_VALUE;
	#endif
	
	//logicReferenceVariableContents = "";
	logicReferenceVariableNameIndex = INT_DEFAULT_VALUE;
	logicReferenceVariableName = "";
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	referenceSetSubject = new GIApreprocessorSubReferenceSet();
	referenceSetObject = new GIApreprocessorSubReferenceSet();
	referenceSetDelimiter = new GIApreprocessorSubReferenceSet();
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	wordIndexSentence = INT_DEFAULT_VALUE;
	#endif
}
GIApreprocessorLogicReferenceVariable::~GIApreprocessorLogicReferenceVariable(void)
{
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	delete referenceSetSubject;
	delete referenceSetObject;
	delete referenceSetDelimiter;
	#endif
}

GIApreprocessorLogicReference::GIApreprocessorLogicReference(void)
{
	//logicReferenceContents = "";
	logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED;		//if necessary (ie is set to GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_UNKNOWN) this will be replaced with GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and/GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or upon the detection of a subsequent and/or
	logicReferenceClassType = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown;
	logicReferenceVariable = new GIApreprocessorLogicReferenceVariable();
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	firstSubLogicReferenceInListGovernor = NULL;
	firstSubLogicReferenceInListDependent = NULL;
	firstSubLogicReferenceInListArray = NULL;
	lastLogicReferenceInUpperLevel = NULL;
	next = NULL;
	previous = NULL;
	hasSubLogicReference = false;
	hasSubLogicReferenceArray = false;
	isSubLogicReferenceGovernor = false;
	isSubLogicReferenceDependent = false;
	isSubLogicReferenceArray = false;
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
	//logicReferenceSetContentsWithVariableNames = "";
	logicReferenceSetContentsWithVariableNamesSentenceIndex = INT_DEFAULT_VALUE;
	#endif
	#endif
}
GIApreprocessorLogicReference::~GIApreprocessorLogicReference(void)
{
	delete logicReferenceVariable;
	if(next != NULL)
	{
		delete next;
	}
}

#endif

GIApreprocessorSentence::GIApreprocessorSentence(void)
{
	sentenceIndexOriginal = 0;	
	sentenceContentsOriginalText = "";
	//#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	//sentenceContentsOriginal = "";
	//sentenceContentsLRP = "";
	//#endif
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	hasLogicReference = false;
	firstLogicReferenceInList = new GIApreprocessorLogicReference();
	//logicReferenceTotal = 0;
	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
	sentenceIndex = INT_DEFAULT_VALUE;
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_EXECUTE_PRELIM_POS_TAGGER
	sentenceReferencePrelim = NULL;
	#endif	
	#else
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	sentenceReference = NULL;
	#endif	
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION
	indentation = 0;
	#endif		
	
	next = NULL;
}
GIApreprocessorSentence::~GIApreprocessorSentence(void)
{
	/*
	//doesn't work because word->next pointers in words will have changed based on lrp
	if(sentenceContentsOriginal.size() > 0)
	{
		delete sentenceContentsOriginal[0];
	}
	*/
	for(vector<GIApreprocessorWord*>::iterator it = sentenceContentsOriginal.begin(); it != sentenceContentsOriginal.end(); it++)
	{
		GIApreprocessorWord* currentWord = *it;
		GIApreprocessorMultiwordReductionPlainTextWord* plaintextWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentWord);	//require to cast back to plaintextword to ensure that all memory is deleted	//CHECKTHIS
		currentWord->nextTag = NULL;	//prevents future words from being deleted
		delete plaintextWord;
	}
	sentenceContentsOriginal.clear();
	sentenceContentsLRP.clear();
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	delete firstLogicReferenceInList;
	#endif
	
	if(next != NULL)
	{
		delete next;
	}
}








