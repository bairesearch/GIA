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
 * File Name: GIAtxtRelTranslatorSANIPropagateOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1a 05-March-2020
 * Requirements: 
 * Description: Textual Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Operations - generic functions
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_OPERATIONS
#define HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_OPERATIONS


#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
//#include "GIAtxtRelTranslatorSANIFormation.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
//#include "GIAtxtRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"

#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
extern bool firstExecution;
#endif

class GIAtxtRelTranslatorSANIPropagateOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	//private: GIAtxtRelTranslatorSANIFormationClass GIAtxtRelTranslatorSANIFormation;
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
	//private: GIAtxtRelTranslatorParserOperationsClass GIAtxtRelTranslatorParserOperations;
	#endif

#ifdef GIA_TXT_REL_TRANSLATOR_SANI

	public: void setParseSentenceReverseStrict(const bool parseSentenceReverseNew, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//set parseSentenceReverse irrespective of GIA_TXT_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
	public: void setParseSentenceReverse(const bool normalDirection, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//determine and set parseSentenceReverse depending on GIA_TXT_REL_TRANSLATOR_SANI_REVERSE_DIRECTION	

	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAtxtRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent);
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components);
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected);
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAtxtRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAtxtRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound, bool* missingStartComponentsFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, int* numberOfInactiveComponentsRemaining);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, bool* existingActivationFound);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFound);
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
	private: bool upperNeuronContainsWordIndexOfProspectiveComponent(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
	#endif	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
	private: bool consecutiveSequenceDetected(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
	private: bool findValidDualLowerLevelConnection(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component);
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
		private: bool findGroup2InForwardConnectionBranchOfGroup1(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group2, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroup);
		#endif
	#endif	

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY
	public: bool componentWordConnectivityTestsWrapper(GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData);
	#else
	public: bool componentWordConnectivityTestsWrapper(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
		public: bool componentWordConnectivityTestsWrapper(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
	#endif
			private: bool componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree, GIAtxtRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
				public: bool getMinMaxWordIndexInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level);
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	public: bool calculatePerformanceWeightOfParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* maxWeight);
		public: bool setPerformanceWeightOfParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int maxWeight);
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE
	public: bool updatePerformance(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
		public: bool updatePerformanceGroup(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
	#endif
		public: bool traceBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents);
			public: bool traceBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight);
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	public: bool isSentenceWordDataFullyConnected(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	public: void isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAtxtRelTranslatorSANIForwardPropogationWordData* currentWordData);
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	public: bool componentTests1(GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	public: bool componentTests2(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
		private: bool getFirstWordInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceWordIndexMin, int* numComponentsFound);

		private: bool findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype);
		private: bool findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype);
			public: bool checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex);
	#endif

	public: bool resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);
	public: bool resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS
	public: bool isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
	#endif

	public: bool resetGroupActivation(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
		public: bool resetComponentsActivation(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components);
	public: bool restoreGroupActivation(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig);
		public: bool restoreComponentsActivation(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* componentsOrig);
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
	public: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupActivationMemory* activationMemoryGroup, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef);
	#else
	public: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	#endif
	#else
	public: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	public: bool restoreGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupOrig);
	#endif
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
	public: bool resetNeuronBackprop(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	public: bool resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, const int groupBoolIndexType);
	#endif	

	public: bool doesRequireResetGroupActivation(GIAtxtRelTranslatorRulesComponentNeuralNetwork* Xcomponent, int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData);
		private: bool findNextAvailableComponent(int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup);

	public: bool currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord);

	public: int convertWordIndexToSentenceEntityIndex(int wordIndex);
	public: int convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex);

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
	public: bool isWordPOStypeNoun(int wordPOStype);
	#endif

#endif
	public: bool deleteGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
		public: bool deleteComponents(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components);
	public: bool deleteGroup(GIAtxtRelTranslatorRulesGroupParseTree* group);
		public: bool deleteComponents(vector<GIAtxtRelTranslatorRulesComponentParseTree*>* components);
	public: bool deleteGroup(GIAtxtRelTranslatorRulesGroupActivationMemory* group);
		
	public: bool printGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, int layer);
		public: bool printComponent(GIAtxtRelTranslatorRulesComponentParseTree* component, int layer);
		public: bool printComponent(GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, int layer);
	public: bool printParseTreeDebugIndentation(int layer);
	
	public: int calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset);

	public: void deleteParseTree(GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToDelete, int level);
	public: GIAtxtRelTranslatorRulesGroupParseTree* replicateParseTree(GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToReplicate, int level);

	public: GIAtxtRelTranslatorRulesGroupParseTree* convertNeuralNetworkGroupToParseTreeGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
	public: GIAtxtRelTranslatorRulesComponentParseTree* convertNeuralNetworkComponentToParseTreeComponent(GIAtxtRelTranslatorRulesComponentNeuralNetwork* component);
	public: GIAtxtRelTranslatorRulesGroupActivationMemory* convertNeuralNetworkGroupToMemoryActivationGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
	public: GIAtxtRelTranslatorRulesGroupParseTree* convertNeuralNetworkGroupToParseTreeGroupNew(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
	public: GIAtxtRelTranslatorRulesComponentParseTree* convertNeuralNetworkComponentToParseTreeComponentNew(GIAtxtRelTranslatorRulesComponentNeuralNetwork* component);
	public: GIAtxtRelTranslatorRulesGroupActivationMemory* convertNeuralNetworkGroupToMemoryActivationGroupNew(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	private: bool resetGroupOptimumPathway(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
	private: bool resetComponentsOptimumPathway(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_FREE_MEMORY
	public: bool initialiseParseTreeGroupList(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIAtxtRelTranslatorRulesGroupParseTree*>* parseTreeGroupList);
	public: bool deinitialiseParseTreeGroupList(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIAtxtRelTranslatorRulesGroupParseTree*>* parseTreeGroupList);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	public: int calculateCoverage(GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
	public: bool traceBackpropNeuralNetwork(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentNeuron, int level, int previousComponentIndex, int previousComponentType);
	public: bool isNeuronString(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentNeuron);
	#endif
};


#endif
