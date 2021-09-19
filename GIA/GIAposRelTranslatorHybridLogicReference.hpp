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
 * File Name: GIAposRelTranslatorHybridLogicReference.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3q2a 19-September-2021
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid Logic Reference
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE
#define HEADER_GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE

#include "GIAglobalDefs.hpp"
#include "GIAposRelTranslatorHybridSentenceClass.hpp"
#include "GIAposRelTranslatorHybridReferenceSet.hpp"
#include "SANIrules.hpp"
#include "LRPpreprocessorWordIdentification.hpp"	//required for isIntrawordPunctuationMark only
#include "XMLparserClass.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID

#define GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_VARIABLE_NAMES_NUM (26)	//max 26 logical condition varibables per sentence; this is a magic number and could be increased
static string GIAposRelTranslatorHybridLogicReferenceVariableNames[GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_VARIABLE_NAMES_NUM] = {"Ax", "Bx", "Cx", "Dx", "Ex", "Fx", "Gx", "Hx", "Ix", "Jx", "Kx", "Lx", "Mx", "Nx", "Ox", "Px", "Qx", "Rx", "Sx", "Tx", "Ux", "Vx", "Wx", "Xx", "Yx", "Zx"};	//minimum 2 characters is currently required for a referenceSet/logicReference subject/object (FUTURE: fix this)

class GIAposRelTranslatorHybridLogicReferenceClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorWordClass LRPpreprocessorWordIdentification;
	private: GIAposRelTranslatorHybridSentenceClassClass GIAposRelTranslatorHybridSentenceClass;
	private: GIAposRelTranslatorHybridReferenceSetClass GIAposRelTranslatorHybridReferenceSet;
	private: SANIrulesClass SANIrules;

	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DEPRECIATED
	public: bool executeLogicReferencePreprocessor(vector<LRPpreprocessorPlainTextWord*>* sentenceContentsWordList, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<XMLparserTag*>* SANIrulesTokenLayers);
		private: bool logicReferenceClassIsPreposition(const GIAposRelTranslatorHybridLogicReference* logicReference);
		private: bool initialiseNextLogicReferenceConjunction(GIAposRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence);
		private: bool initialiseNewSubLogicReferenceConjunctionInferred(GIAposRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence, const bool copySubLogicReferences);
		private: bool initialiseNewSubLogicReferenceConjunction(GIAposRelTranslatorHybridLogicReference** currentLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType, const int wordIndexSentence, const bool copySubLogicReferences);
		private: bool initialiseNewSubLogicReference(GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType, const int wordIndexSentence);
		private: bool fillLogicReferenceVariable(GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*>* logicReferenceVariableWordList, int* logicReferenceVariableNameIndex, bool* expectToFindSubjectAuxObjectLogicReferenceVariable, int wordIndexSentence);
		private: bool identifyImplicitLogicReferenceSets(GIAposRelTranslatorHybridLogicReference* explicitConjunctionLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType);
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
	private: vector<LRPpreprocessorPlainTextWord*> generateLogicReferenceContents(LRPpreprocessorPlainTextWord* logicReferenceWord, const string logicReferenceClassType, const int logicReferenceClass, vector<LRPpreprocessorPlainTextWord*>* logicReferenceVariableWordList);
	#endif
	private: vector<LRPpreprocessorPlainTextWord*> generateLogicReferenceContentsBase(LRPpreprocessorPlainTextWord* logicReferenceWord, const string logicReferenceClassType);
	private: void setLogicReferenceInfo(GIAposRelTranslatorHybridLogicReference* logicReference, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType);
	#endif
	public: bool islogicReferenceExplitOrImplicitConjunctionComponent(const GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList);
	public: bool islogicReferenceConjunctionComponentNotFirst(const GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, const GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList);
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
	public: bool islogicReferenceParentConjunctionComponentNotFirst(const GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList);
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT
	public: bool printLogicReferenceLayer(GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInLayer);
	#endif	
};

#endif

#endif

