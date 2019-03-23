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
 * File Name: GIAtxtRelTranslatorNeuralNetworkOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g11g 01-March-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Operations - generic functions
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_OPERATIONS
#define HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_OPERATIONS


#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
//#include "GIAtxtRelTranslatorNeuralNetworkFormation.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
//#include "GIAtxtRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"

#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA8
extern bool firstExecution;
#endif

class GIAtxtRelTranslatorNeuralNetworkOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	//private: GIAtxtRelTranslatorNeuralNetworkFormationClass GIAtxtRelTranslatorNeuralNetworkFormation;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//private: GIAtxtRelTranslatorParserOperationsClass GIAtxtRelTranslatorParserOperations;
	#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponent** previousActiveComponent, GIAtxtRelTranslatorRulesComponent** lastActiveComponent);
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components);
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected);
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponent** previousActiveComponent, GIAtxtRelTranslatorRulesComponent** lastActiveComponent, bool* existingActivationFound);
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, bool* existingActivationFound);
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFound);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	public: bool componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData);
	#else
	public: bool componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData);
		public: bool componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
	#endif
		private: bool componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
			private: bool getMinMaxWordIndexInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
	public: bool verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
		private: bool getWordIndicesInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, vector<int>* wordIndexList, int level);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	public: int countNumberWordsInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
	public: bool updatePerformance(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
		public: bool updatePerformanceGroup(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
	#endif
		public: bool traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents);
			public: bool traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight);
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	public: bool isSentenceWordDataFullyConnected(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
	public: void isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* currentWordData);
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	public: bool componentTests1(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	public: bool componentTests2(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
		private: bool getFirstWordInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceEntityIndexMin, int* numComponentsFound);

		private: bool findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype);
		private: bool findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype);
			public: bool checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex);
	#endif

	public: bool resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);
	public: bool resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
	public: bool isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroup* group);
	#endif

	public: bool resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group);
		public: bool resetComponentsActivation(vector<GIAtxtRelTranslatorRulesComponent*>* components);
	public: bool restoreGroupActivation(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* groupOrig);
		public: bool restoreComponentsActivation(vector<GIAtxtRelTranslatorRulesComponent*>* components, vector<GIAtxtRelTranslatorRulesComponent*>* componentsOrig);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
	public: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* activationMemoryGroup, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef);
	#else
	public: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, const bool deleteExistingParseTreeGroupRef);
	#endif
	#else
	public: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, const bool deleteExistingParseTreeGroupRef);
	public: bool restoreGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* parseTreeGroupOrig);
	#endif
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	public: bool resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, const int groupBoolIndexType);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	public: bool resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType);
	#endif	
	
	public: bool deleteGroup(GIAtxtRelTranslatorRulesGroup* group);
		public: bool deleteComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components);

	public: bool doesRequireResetGroupActivation(GIAtxtRelTranslatorRulesComponent* Xcomponent, int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData);
		private: bool findNextAvailableComponent(int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup);

	public: bool currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord);

	public: int convertWordIndexToSentenceEntityIndex(int wordIndex);
	public: int convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
	public: bool isWordPOStypeNoun(int wordPOStype);
	#endif

#endif
	public: bool printGroup(GIAtxtRelTranslatorRulesGroup* group, int layer);
		public: bool printComponent(GIAtxtRelTranslatorRulesComponent* component, int layer);
	public: bool printParseTreeDebugIndentation(int layer);
	
	public: int calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
	public: bool resetAllowed(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
		public: bool existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
			public: bool existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
	#endif
	
	public: GIAtxtRelTranslatorRulesGroup* replicateParseTree(GIAtxtRelTranslatorRulesGroup* parseTreeGroupToReplicate, int level);


};


#endif
