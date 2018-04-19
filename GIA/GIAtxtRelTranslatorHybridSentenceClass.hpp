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
 * File Name: GIAtxtRelTranslatorHybridSentenceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f2c 04-April-2018
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid Sentence Class
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_SENTENCE_CLASS
#define HEADER_GIA_TXT_REL_TRANSLATOR_SENTENCE_CLASS

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "GIAentityNodeClass.hpp"	//required for primaryEntityTemp, GIA_PREPROCESSOR_RECORD_REFERENCES
#include "GIAsentenceClass.hpp"	//required for GIA_PREPROCESSOR_RECORD_REFERENCES
#include "GIApreprocessorMultiwordReductionClass.hpp" 




#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID

class GIApreprocessorSubReferenceSet
{
public:
	GIApreprocessorSubReferenceSet(void);
	~GIApreprocessorSubReferenceSet(void);

	int sentenceIndex;		//if GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES this is a temporary sentence index
	vector<GIApreprocessorWord*> subReferenceSetContents;
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
	vector<GIApreprocessorWord*> subReferenceSetContentsOutputForNLP;
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	int firstIndexOfReferenceSetText;
	int lastIndexOfReferenceSetText;
	int dummyNLPtestOffset;
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	GIAentityNode* primaryEntityTemp;	//required for conjunction post processing
	#endif
	
	bool isReferenceSetDelimiter;
	GIApreprocessorSubReferenceSet* next;
	
	//for optimisation purposes;
	int delimiterType;
	int delimiterSpecialCase;
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIAsentence* sentenceReference;
	#endif
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
	int definite;
	#endif
	
	bool isFirstSubreferenceSetInList;
};

class GIAtxtRelTranslatorHybridLogicReferenceVariable
{
public:
	GIAtxtRelTranslatorHybridLogicReferenceVariable(void);
	~GIAtxtRelTranslatorHybridLogicReferenceVariable(void);

	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
	int sentenceIndex;
	#endif
	
	vector<GIApreprocessorWord*> logicReferenceVariableContents;
	int logicReferenceVariableNameIndex;
	string logicReferenceVariableName;	//replacement variable name for logical condition variable
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	GIApreprocessorSubReferenceSet* referenceSetSubject;	//referenceSetSubject
	GIApreprocessorSubReferenceSet* referenceSetObject;	//referenceSetObject
	GIApreprocessorSubReferenceSet* referenceSetDelimiter;	//NB if !GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITERS, only the referenceSetContents string is used of the referenceSetDelimiter
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	int wordIndexSentence;
	#endif
};

class GIAtxtRelTranslatorHybridLogicReference
{
public:
	GIAtxtRelTranslatorHybridLogicReference(void);
	~GIAtxtRelTranslatorHybridLogicReference(void);

	vector<GIApreprocessorWord*> logicReferenceContents;	   //similiar if not identical to GIAtxtRelTranslatorHybridLogicReferenceTypes[logicReferenceClassType];
	int logicReferenceClass;
	string logicReferenceClassType;
	GIAtxtRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	GIAtxtRelTranslatorHybridLogicReference* firstSubLogicReferenceInListGovernor;	//for verbs/propositions, eg said/considering(A, B)
	GIAtxtRelTranslatorHybridLogicReference* firstSubLogicReferenceInListDependent;	//for verbs/propositions, eg said/considering(A, B)
	GIAtxtRelTranslatorHybridLogicReference* firstSubLogicReferenceInListArray;	//for conjunctions eg and(A, B, C)
	GIAtxtRelTranslatorHybridLogicReference* lastLogicReferenceInUpperLevel;	//need to work out how to identify in when returning to an upper level logical condition layer (is this even done in English?); eg Tom said that Mary said A is B, but Jerry said otherwise.	but(said(Tom, said(Mary, A is B)), said(Jerry, otherwise))
	GIAtxtRelTranslatorHybridLogicReference* next;		//for isSubLogicReferenceArray only; conjunctions (and, or, but), eg and(said(A, B), said(A, C), said(A, D))
	GIAtxtRelTranslatorHybridLogicReference* previous;	//for isSubLogicReferenceArray only; conjunctions (and, or, but), eg and(said(A, B), said(A, C), said(A, D))
	bool hasSubLogicReference;
	bool hasSubLogicReferenceArray;
	bool isSubLogicReferenceGovernor;
	bool isSubLogicReferenceDependent;
	bool isSubLogicReferenceArray;
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
	vector<GIApreprocessorWord*> logicReferenceSetContentsWithVariableNames;
	int logicReferenceSetContentsWithVariableNamesSentenceIndex;
	#endif
	#endif
};

#endif

class GIAtxtRelTranslatorHybridSentenceClassClass
{
	
};



#endif
