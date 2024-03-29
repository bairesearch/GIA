 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorHybridLogicReference.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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

