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
 * File Name: GIApreprocessorLogicReference.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e10a 15-January-2018
 * Requirements: requires plain text file
 * Description: Logic Reference preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_LOGIC_REFERENCE
#define HEADER_GIA_PREPROCESSOR_LOGIC_REFERENCE

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorReferenceSet.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"	//required for isIntrawordPunctuationMark only
#include "XMLparserClass.hpp"

class GIApreprocessorLogicReferenceClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIApreprocessorReferenceSetClass GIApreprocessorReferenceSet;

	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	public: bool extractGIApreprocessorLogicReferenceClasses(XMLparserTag** firstLogicReferenceClassTag, vector<string>* logicReferenceInstanceList);
	public: bool executeLogicReferencePreprocessor(const vector<GIApreprocessorWord*>* sentenceContentsWordList, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, XMLparserTag* firstLogicReferenceClassTag);
		private: void addSentenceToText(string* sentenceContentsPreprocessed, GIApreprocessorSubReferenceSet* referenceSet, int* sentenceIndex);
		private: bool logicReferenceClassIsPreposition(GIApreprocessorLogicReference* logicReference);
		private: bool initialiseNextLogicReferenceConjunction(GIApreprocessorLogicReference** currentLogicReferenceInList, const int wordIndexSentence);
		private: bool initialiseNewSubLogicReferenceConjunctionInferred(GIApreprocessorLogicReference** currentLogicReferenceInList, const int wordIndexSentence, const bool copySubLogicReferences);
		private: bool initialiseNewSubLogicReferenceConjunction(GIApreprocessorLogicReference** currentLogicReferenceInList, const vector<GIApreprocessorWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence, const bool copySubLogicReferences);
		private: bool initialiseNewSubLogicReference(GIApreprocessorLogicReference* currentLogicReferenceInList, const vector<GIApreprocessorWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence);
		private: bool fillLogicReferenceVariable(GIApreprocessorLogicReference* currentLogicReferenceInList, vector<GIApreprocessorWord*>* logicReferenceVariableWordList, int* logicReferenceVariableNameIndex, bool* expectToFindSubjectAuxObjectLogicReferenceVariable, const int wordIndexSentence);
		private: bool isClassTag(string word, string classNameToFind, string* classTypeNameFound, XMLparserTag* firstLogicReferenceClassTag);
		private: bool identifyImplicitLogicReferenceSets(GIApreprocessorLogicReference* explicitConjunctionLogicReferenceInList, const vector<GIApreprocessorWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType);
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
	private: vector<GIApreprocessorWord*> generateLogicReferenceContents(GIApreprocessorWord* logicReferenceWord, const string logicReferenceClassType, int logicReferenceClass, vector<GIApreprocessorWord*>* logicReferenceVariableWordList);
	#endif
	private: vector<GIApreprocessorWord*> generateLogicReferenceContentsBase(GIApreprocessorWord* logicReferenceWord, const string logicReferenceClassType);
	private: void setLogicReferenceInfo(GIApreprocessorLogicReference* logicReference, const vector<GIApreprocessorWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType);
	public: bool islogicReferenceExplitOrImplicitConjunctionComponent(GIApreprocessorLogicReference* currentLogicReferenceInList);
	public: bool islogicReferenceConjunctionComponentNotFirst(GIApreprocessorLogicReference* currentLogicReferenceInList, GIApreprocessorLogicReference* firstLogicReferenceInList);
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
	public: bool islogicReferenceParentConjunctionComponentNotFirst(GIApreprocessorLogicReference* currentLogicReferenceInList, GIApreprocessorLogicReference* firstLogicReferenceInList);
	#endif
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_PRINT_OUTPUT
	public: bool printLogicReferenceLayer(GIApreprocessorLogicReference* firstLogicReferenceInLayer);
	#endif	
};

#endif

