 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorHybridSentenceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid Sentence Class
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_SENTENCE_CLASS
#define HEADER_GIA_POS_REL_TRANSLATOR_SENTENCE_CLASS

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "GIAentityNodeClass.hpp"	//required for primaryEntityTemp, LRP_PREPROCESSOR_RECORD_REFERENCES
#include "GIAsentenceClass.hpp"	//required for LRP_PREPROCESSOR_RECORD_REFERENCES
#include "LRPpreprocessorWordClass.hpp" 




#ifdef GIA_POS_REL_TRANSLATOR_HYBRID

class LRPpreprocessorSubReferenceSet
{
public:
	LRPpreprocessorSubReferenceSet(void);
	~LRPpreprocessorSubReferenceSet(void);

	int sentenceIndex;		//if LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES this is a temporary sentence index
	vector<LRPpreprocessorPlainTextWord*> subReferenceSetContents;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
	vector<LRPpreprocessorPlainTextWord*> subReferenceSetContentsOutputForNLP;
	#endif
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	int firstIndexOfReferenceSetText;
	int lastIndexOfReferenceSetText;
	int dummyNLPtestOffset;
	#endif
		
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	GIAentityNode* primaryEntityTemp;	//required for conjunction post processing
	#endif
	
	bool isReferenceSetDelimiter;
	LRPpreprocessorSubReferenceSet* next;
	
	//for optimisation purposes;
	int delimiterType;
	int delimiterSpecialCase;
	
	#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
	GIAsentence* sentenceReference;
	#endif
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
	int definite;
	#endif
	
	bool isFirstSubreferenceSetInList;
};

class GIAposRelTranslatorHybridLogicReferenceVariable
{
public:
	GIAposRelTranslatorHybridLogicReferenceVariable(void);
	~GIAposRelTranslatorHybridLogicReferenceVariable(void);

	#ifdef LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
	int sentenceIndex;
	#endif
	
	vector<LRPpreprocessorPlainTextWord*> logicReferenceVariableContents;
	int logicReferenceVariableNameIndex;
	string logicReferenceVariableName;	//replacement variable name for logical condition variable
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	LRPpreprocessorSubReferenceSet* referenceSetSubject;	//referenceSetSubject
	LRPpreprocessorSubReferenceSet* referenceSetObject;	//referenceSetObject
	LRPpreprocessorSubReferenceSet* referenceSetDelimiter;	//NB if !GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITERS, only the referenceSetContents string is used of the referenceSetDelimiter
	#endif
	
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	int wordIndexSentence;
	#endif
};

class GIAposRelTranslatorHybridLogicReference
{
public:
	GIAposRelTranslatorHybridLogicReference(void);
	~GIAposRelTranslatorHybridLogicReference(void);

	vector<LRPpreprocessorPlainTextWord*> logicReferenceContents;	   //similiar if not identical to GIAposRelTranslatorHybridLogicReferenceTypes[logicReferenceClassType];
	int logicReferenceClass;
	string logicReferenceClassType;
	GIAposRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable;
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	GIAposRelTranslatorHybridLogicReference* firstSubLogicReferenceInListGovernor;	//for verbs/propositions, eg said/considering(A, B)
	GIAposRelTranslatorHybridLogicReference* firstSubLogicReferenceInListDependent;	//for verbs/propositions, eg said/considering(A, B)
	GIAposRelTranslatorHybridLogicReference* firstSubLogicReferenceInListArray;	//for conjunctions eg and(A, B, C)
	GIAposRelTranslatorHybridLogicReference* lastLogicReferenceInUpperLevel;	//need to work out how to identify in when returning to an upper level logical condition layer (is this even done in English?); eg Tom said that Mary said A is B, but Jerry said otherwise.	but(said(Tom, said(Mary, A is B)), said(Jerry, otherwise))
	GIAposRelTranslatorHybridLogicReference* next;		//for isSubLogicReferenceArray only; conjunctions (and, or, but), eg and(said(A, B), said(A, C), said(A, D))
	GIAposRelTranslatorHybridLogicReference* previous;	//for isSubLogicReferenceArray only; conjunctions (and, or, but), eg and(said(A, B), said(A, C), said(A, D))
	bool hasSubLogicReference;
	bool hasSubLogicReferenceArray;
	bool isSubLogicReferenceGovernor;
	bool isSubLogicReferenceDependent;
	bool isSubLogicReferenceArray;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
	vector<LRPpreprocessorPlainTextWord*> logicReferenceSetContentsWithVariableNames;
	int logicReferenceSetContentsWithVariableNamesSentenceIndex;
	#endif
	#endif
};

#endif

class GIAposRelTranslatorHybridSentenceClassClass
{
	
};



#endif
