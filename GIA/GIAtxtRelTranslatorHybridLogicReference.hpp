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
 * File Name: GIAtxtRelTranslatorHybridLogicReference.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1w 22-February-2018
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid Logic Reference
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE
#define HEADER_GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE

#include "GIAglobalDefs.hpp"
#include "GIAtxtRelTranslatorHybridSentenceClass.hpp"
#include "GIAtxtRelTranslatorHybridReferenceSet.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"	//required for isIntrawordPunctuationMark only
#include "XMLparserClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID

#define GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_VARIABLE_NAMES_NUM (26)	//max 26 logical condition varibables per sentence; this is a magic number and could be increased
static string GIAtxtRelTranslatorHybridLogicReferenceVariableNames[GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_VARIABLE_NAMES_NUM] = {"Ax", "Bx", "Cx", "Dx", "Ex", "Fx", "Gx", "Hx", "Ix", "Jx", "Kx", "Lx", "Mx", "Nx", "Ox", "Px", "Qx", "Rx", "Sx", "Tx", "Ux", "Vx", "Wx", "Xx", "Yx", "Zx"};	//minimum 2 characters is currently required for a referenceSet/logicReference subject/object (FUTURE: fix this)

class GIAtxtRelTranslatorHybridLogicReferenceClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIAtxtRelTranslatorHybridSentenceClassClass GIAtxtRelTranslatorHybridSentenceClass;
	private: GIAtxtRelTranslatorHybridReferenceSetClass GIAtxtRelTranslatorHybridReferenceSet;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;

	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_DEPRECIATED
	public: bool executeLogicReferencePreprocessor(const vector<GIApreprocessorWord*>* sentenceContentsWordList, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers);
		private: void addSentenceToText(string* sentenceContentsPreprocessed, GIApreprocessorSubReferenceSet* referenceSet, int* sentenceIndex);
		private: bool logicReferenceClassIsPreposition(GIAtxtRelTranslatorHybridLogicReference* logicReference);
		private: bool initialiseNextLogicReferenceConjunction(GIAtxtRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence);
		private: bool initialiseNewSubLogicReferenceConjunctionInferred(GIAtxtRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence, const bool copySubLogicReferences);
		private: bool initialiseNewSubLogicReferenceConjunction(GIAtxtRelTranslatorHybridLogicReference** currentLogicReferenceInList, const vector<GIApreprocessorWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence, const bool copySubLogicReferences);
		private: bool initialiseNewSubLogicReference(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, const vector<GIApreprocessorWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence);
		private: bool fillLogicReferenceVariable(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, vector<GIApreprocessorWord*>* logicReferenceVariableWordList, int* logicReferenceVariableNameIndex, bool* expectToFindSubjectAuxObjectLogicReferenceVariable, const int wordIndexSentence);
		private: bool identifyImplicitLogicReferenceSets(GIAtxtRelTranslatorHybridLogicReference* explicitConjunctionLogicReferenceInList, const vector<GIApreprocessorWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType);
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
	private: vector<GIApreprocessorWord*> generateLogicReferenceContents(GIApreprocessorWord* logicReferenceWord, const string logicReferenceClassType, int logicReferenceClass, vector<GIApreprocessorWord*>* logicReferenceVariableWordList);
	#endif
	private: vector<GIApreprocessorWord*> generateLogicReferenceContentsBase(GIApreprocessorWord* logicReferenceWord, const string logicReferenceClassType);
	private: void setLogicReferenceInfo(GIAtxtRelTranslatorHybridLogicReference* logicReference, const vector<GIApreprocessorWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType);
	#endif
	public: bool islogicReferenceExplitOrImplicitConjunctionComponent(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList);
	public: bool islogicReferenceConjunctionComponentNotFirst(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList);
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
	public: bool islogicReferenceParentConjunctionComponentNotFirst(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList);
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT
	public: bool printLogicReferenceLayer(GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInLayer);
	#endif	
};

#endif

#endif

