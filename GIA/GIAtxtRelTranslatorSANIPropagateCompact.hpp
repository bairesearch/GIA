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
 * File Name: GIAtxtRelTranslatorSANIPropagateCompact.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1c 05-March-2020
 * Requirements: 
 * Description: Textual Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Propagate Compact - ~O(n)
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_COMPACT
#define HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_COMPACT

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorSANIPropagateOperations.hpp"
#include "GIAtxtRelTranslatorSANIFormation.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
//#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_SANI_COMPACT


class GIAtxtRelTranslatorSANIPropagateCompactClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	//private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorSANIPropagateOperationsClass GIAtxtRelTranslatorSANIPropagateOperations;
	private: GIAtxtRelTranslatorSANIFormationClass GIAtxtRelTranslatorSANIFormation;

	private: ANNdisplayClass ANNdisplay;

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_FREE_MEMORY
	public: vector<GIAtxtRelTranslatorRulesGroupParseTree*>* getParseTreeGroupList();
	#endif

	public: bool defineFirstLayer(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents);
		public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
			public: bool executeTxtRelTranslatorNeuralNetworkPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
			public: bool executeTxtRelTranslatorNeuralNetworkPart2(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
				public: bool performPropagationTest(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);				
				public: bool performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence);
				public: bool performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
					public: bool performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
				private: bool propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer);
					private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer);
				private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup);
						private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFound);
							private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFound, const bool missingStartComponentsFound, const bool missingOrVariableStartComponentFound, const bool variableStartComponentFound);
				private: bool sequentialActivationConnectivityTests(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFound, const bool variableStartComponentFound);
				private: void updateParseTreeMaxMinWordIndexOfParent(GIAtxtRelTranslatorRulesGroupParseTree* parentGroup, GIAtxtRelTranslatorRulesComponentParseTree* parentComponent, GIAtxtRelTranslatorRulesGroupParseTree* childGroup);
					private: void updateParseTreeMaxMinWordIndexOfParentBasic(GIAtxtRelTranslatorRulesGroupParseTree* parentGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorRulesGroupParseTree* childGroup);
				private: bool printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level);
			
	//public: bool verifyActivatedNeuronWithMaxWordIndexCoverage(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronBaseline, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool candidateCoverageVariable);
		public: bool verifyActivatedNeuronWithMaxWordIndexCoverage(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronBaseline, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool candidateCoverageVariable);
			public: bool verifyActivatedNeuronWithMaxWordIndexAllowed(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition);
	public: bool verifyActivatedNeuronsAtLeastOne(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup1);
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
	public: bool verifyNeverSplitGroupBetweenTwoIdenticalComponents(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const bool candidateCoveragePartial);		
		private: bool neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
	#endif
		public: bool verifyActivatedNeuronsAtLeastOneBasic(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);

	public: bool identifyComponentIndexLastActivatedIndex(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex);
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	public: bool identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex);
		public: bool identifyComponentIndexFirstAndLastActivatedIndexUnordered(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex);
	#else
	public: bool identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex);
		public: bool identifyComponentIndexLastActivatedIndexUnordered(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex);	
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	public: bool upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFound);
		public: bool upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int wordIndexMinToFindAbove, const int wordIndexMaxToFindAbove, const int wordIndexLastAboveEffectiveAfterGroupReset, int level, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, bool wordIndexMinFound, bool wordIndexMaxFound);
	#endif
};

#endif

#endif
