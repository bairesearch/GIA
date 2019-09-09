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
 * File Name: GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j2c 10-August-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Propagate Compact - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_COMPACT_GENERATE
#define HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_COMPACT_GENERATE

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkPropagateOperations.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkPropagateCompact.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkFormation.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR

class GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass GIAtxtRelTranslatorNeuralNetworkPropagateOperations;
	private: GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass GIAtxtRelTranslatorNeuralNetworkPropagateCompact;
	private: GIAtxtRelTranslatorNeuralNetworkFormationClass GIAtxtRelTranslatorNeuralNetworkFormation;

	private: ANNdisplayClass ANNdisplay;


	
	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer);
		public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer);
			private: bool findAndReconcileVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer);
				private: bool findAndReconcileIncrementalVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer);
					private: bool createOrAppendFirstLevelHiddenLayerGroup(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, GIAtxtRelTranslatorRulesGroupNeuralNetwork** newNeuronSequenceGroup, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* listOfHighLevelNeurons);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_OPTIONAL_AND_MULTIPLE_INPUTS
				private: bool findAndReconcileCentralVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer);
					private: bool performPropagationDirection(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool parseSentenceReverse, bool* neuronWithMaxWordIndexCoverageisPartiallyActivated, GIAtxtRelTranslatorRulesGroupNeuralNetwork** activatedNeuronWithMaxWordIndexCoverage, int* partiallyActivatedNeuronWithMaxWordIndexCoveragelastActivatedIndex, int* partiallyActivatedNeuronWithMaxWordIndexCoveragelastWordIndex, int* partiallyActivatedNeuronWithMaxWordIndexCoveragenumUnactivatedComponents);		
					private: bool splitGroupsConnectedByOptionalOrDifferentComponent(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverage1, GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverage2, const int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool direction, const bool optionalComponentFound);
						private: bool findAndConnectDifferentOrMissingSection(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool direction, GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponent);
							private: bool findAndConnectDifferentOrMissingSectionSub(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool direction, GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponent, const bool isFullyActivatedNeuronWithMaxWordIndexCoverage, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
				#endif

	private: bool addComponentToFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron, GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup);
	private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* createNewHiddenLayerGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
	//private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* createNewFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron);
	private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* splitGroupAtLastActivatedComponent(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* neuronToSplit, int indexToSplitVector);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* splitGroupAtLastActivatedComponentUnordered(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* neuronToSplit, int indexToSplitVector1, int indexToSplitVector2);
	#endif	

	private: bool addComponentToGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelComponentGroupOwner, int componentType, bool insertAtStart);
	private: bool addComponentToGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);
		private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* createNewGroup();

	private: bool findGroupDirectlyAbove(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group1, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group2);

	private: bool verifyLastWordIndex(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroup, int lastWordIndex, bool* adjacent);

};

#endif

#endif
