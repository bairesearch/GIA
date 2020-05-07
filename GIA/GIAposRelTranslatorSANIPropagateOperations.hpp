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
 * File Name: GIAposRelTranslatorSANIPropagateOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k8c 06-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Operations - generic functions
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_OPERATIONS
#define HEADER_GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_OPERATIONS


#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAposRelTranslatorRules.hpp"
#include "GIAposRelTranslatorRulesGroupClass.hpp"
#include "GIAposRelTranslatorRulesComponentClass.hpp"
//#include "GIAposRelTranslatorSANIFormation.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
//#include "GIAposRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"

#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
extern bool firstExecution;
#endif

class GIAposRelTranslatorSANIPropagateOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAposRelTranslatorRulesClass GIAposRelTranslatorRules;
	private: GIAposRelTranslatorRulesGroupClass GIAposRelTranslatorRulesGroupClassObject;
	private: GIAposRelTranslatorRulesComponentClass GIAposRelTranslatorRulesComponentClassObject;
	//private: GIAposRelTranslatorSANIFormationClass GIAposRelTranslatorSANIFormation;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	//private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI

	public: void setParseSentenceReverseStrict(const bool parseSentenceReverseNew, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//set parseSentenceReverse irrespective of GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
	public: void setParseSentenceReverse(const bool normalDirection, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//determine and set parseSentenceReverse depending on GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION	

	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent);
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components);
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected);
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound, bool* missingStartComponentsFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, int* numberOfInactiveComponentsRemaining);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, bool* existingActivationFound);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFound);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
	private: bool upperNeuronContainsWordIndexOfProspectiveComponent(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorRulesComponentNeuralNetwork* component, GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	#endif	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
	private: bool repeatedSequenceDetected(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorRulesComponentNeuralNetwork* component);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
	private: bool consecutiveSequenceDetected(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorRulesComponentNeuralNetwork* component);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
	private: bool findValidDualLowerLevelConnection(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorRulesComponentNeuralNetwork* component, bool assumeFirstComponentActive);
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
		private: bool findGroup2InForwardConnectionBranchOfGroup1(GIAposRelTranslatorRulesGroupNeuralNetwork* group2, GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroup);
		#endif
	#endif	

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	public: bool componentWordConnectivityTestsWrapper(GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData);
	#else
	public: bool componentWordConnectivityTestsWrapper(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
		public: bool componentWordConnectivityTestsWrapper(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
	#endif
			private: bool componentWordConnectivityTests(GIAposRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree, GIAposRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
				public: bool getMinMaxWordIndexInParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level);
	#endif
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	public: bool calculatePerformanceWeightOfParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* maxWeight);
		public: bool setPerformanceWeightOfParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int maxWeight);
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE
	public: bool updatePerformance(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
		public: bool updatePerformanceGroup(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
	#endif
		public: bool traceBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents);
			public: bool traceBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight);
	#endif
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	public: bool isSentenceWordDataFullyConnected(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	public: void isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAposRelTranslatorSANIForwardPropogationWordData* currentWordData);
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	public: bool componentTests1(GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	public: bool componentTests2(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
		private: bool getFirstWordInParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceWordIndexMin, int* numComponentsFound);

		private: bool findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype);
		private: bool findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype);
			public: bool checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex);
	#endif

	public: bool resetAllNeurons(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, const int groupBoolIndexType);
	public: bool resetAllNeuronComponents(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, const int groupBoolIndexType);

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS
	public: bool isLastComponentFuzzy(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	#endif

	public: bool resetGroupActivation(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
		public: bool resetComponentsActivation(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components);
	public: bool restoreGroupActivation(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig);
		public: bool restoreComponentsActivation(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsOrig);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
	public: bool resetGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupActivationMemory* activationMemoryGroup, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef);
	#else
	public: bool resetGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	#endif
	#else
	public: bool resetGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	public: bool restoreGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupOrig);
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	public: bool resetNeuronBackprop(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	public: bool resetNeuronForwardProp(GIAposRelTranslatorRulesGroupNeuralNetwork* group, const int groupBoolIndexType);
	#endif	

	public: bool doesRequireResetGroupActivation(GIAposRelTranslatorRulesComponentNeuralNetwork* Xcomponent, int indexOfXcomponentInGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData);
		private: bool findNextAvailableComponent(int indexOfXcomponentInGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup);

	public: bool currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord);

	public: int convertWordIndexToSentenceEntityIndex(int wordIndex);
	public: int convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex);

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
	public: bool isWordPOStypeNoun(int wordPOStype);
	#endif
	
	public: int countParseTreeLeafSize(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_MATCHING_DEPTH
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	public: bool adjustNetworkDepth(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	public: bool adjustNetworkDepthReset(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	#endif
	public: bool getNeuralNetworkDepth(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron, int* maxDepth);
		public: bool getNeuralNetworkDepth(GIAposRelTranslatorRulesComponentNeuralNetwork* component, int* maxDepth);
	#endif
	public: bool countNeuralNetworkMaxLeafSizeAndDepth(GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroup, int* maxLeafSize, int* maxDepth);
		public: bool countNeuralNetworkMaxLeafSizeAndDepth(GIAposRelTranslatorRulesComponentNeuralNetwork* component, int* maxLeafSize, int* maxDepth);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	public: bool countNeuralNetworkMaxLeafSizeAndDepthReset(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron);
		public: bool countNeuralNetworkMaxLeafSizeAndDepthReset(GIAposRelTranslatorRulesComponentNeuralNetwork* component);
	#endif
	#endif

	public: GIAposRelTranslatorRulesComponentNeuralNetwork* getFirstComponent(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron, bool fromStart);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_MATCHING_DEPTH	
	public: int calculateDepthFromBinaryTreeLeafSize(int numberOfLeafNodesInBinaryTree);
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE
	public: double generateRandomNumber();
	#endif
#endif
	public: bool deleteGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
		public: bool deleteComponents(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components);
	public: bool deleteGroup(GIAposRelTranslatorRulesGroupParseTree* group);
		public: bool deleteComponents(vector<GIAposRelTranslatorRulesComponentParseTree*>* components);
	public: bool deleteGroup(GIAposRelTranslatorRulesGroupActivationMemory* group);
		
	public: bool printGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group, int layer);
		public: bool printComponent(GIAposRelTranslatorRulesComponentParseTree* component, int layer);
		public: bool printComponent(GIAposRelTranslatorRulesComponentNeuralNetwork* component, int layer);
	public: bool printParseTreeDebugIndentation(int layer);
	
	public: int calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset);

	public: void deleteParseTree(GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToDelete, int level);
	public: GIAposRelTranslatorRulesGroupParseTree* replicateParseTree(GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToReplicate, int level);

	public: GIAposRelTranslatorRulesGroupParseTree* convertNeuralNetworkGroupToParseTreeGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	public: GIAposRelTranslatorRulesComponentParseTree* convertNeuralNetworkComponentToParseTreeComponent(GIAposRelTranslatorRulesComponentNeuralNetwork* component);
	public: GIAposRelTranslatorRulesGroupActivationMemory* convertNeuralNetworkGroupToMemoryActivationGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	public: GIAposRelTranslatorRulesGroupParseTree* convertNeuralNetworkGroupToParseTreeGroupNew(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	public: GIAposRelTranslatorRulesComponentParseTree* convertNeuralNetworkComponentToParseTreeComponentNew(GIAposRelTranslatorRulesComponentNeuralNetwork* component);
	public: GIAposRelTranslatorRulesGroupActivationMemory* convertNeuralNetworkGroupToMemoryActivationGroupNew(GIAposRelTranslatorRulesGroupNeuralNetwork* group);

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	private: bool resetGroupOptimumPathway(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	private: bool resetComponentsOptimumPathway(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
	public: bool initialiseParseTreeGroupList(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIAposRelTranslatorRulesGroupParseTree*>* parseTreeGroupList);
	public: bool deinitialiseParseTreeGroupList(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIAposRelTranslatorRulesGroupParseTree*>* parseTreeGroupList);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	public: int calculateCoverage(GIAposRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
	public: bool traceBackpropNeuralNetwork(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron, int level, int previousComponentIndex, int previousComponentType);
	public: bool isNeuronString(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron);
	public: bool getWordPOStypeFromAmbiguousWord(GIApreprocessorPlainTextWord* currentWord, int* wordPOStype);
	#endif
	
	public: string printParseTreeGroupIndices(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup);


};


#endif
