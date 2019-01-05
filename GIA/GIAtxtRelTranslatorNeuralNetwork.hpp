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
 * File Name: GIAtxtRelTranslatorNeuralNetwork.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g2c 21-May-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
#define HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkFormation.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
#include "GIAtxtRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

class GIAactivationPathMostRecentContribution
{
public:
	GIAactivationPathMostRecentContribution(void);
	~GIAactivationPathMostRecentContribution(void);
	
	int mostRecentContributionIndex;
	int mostRecentContributionWordIndex;
	bool mostRecentContributionRequireResetGroupActivation;
};

class GIAtxtRelTranslatorNeuralNetworkClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorNeuralNetworkFormationClass GIAtxtRelTranslatorNeuralNetworkFormation;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	private: GIAtxtRelTranslatorParserOperationsClass GIAtxtRelTranslatorParserOperations;
	#endif
	private: ANNdisplayClass ANNdisplay;
	//private: GIAtxtRelTranslatorNeuralNetworkClass GIAtxtRelTranslatorNeuralNetworkClassObject;

	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace);
			private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup);
				private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED);
					private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED);
						private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED, const bool lastComponentFuzzyAndCompatible);	
							private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED, const bool lastComponentFuzzyAndCompatible);
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
								private: bool isSentenceWordDataFullyConnected(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
								private: void isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* currentWordData);
								#endif
								private: bool propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED);
								private: bool componentTests(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData);
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
									private: bool findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype);
									private: bool findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype);
										private: bool checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex);
									#endif
							private: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted);
							private: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components);
							private: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected);

	private: bool resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);
	private: bool resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);
	private: bool printBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level);
	private: bool groupActivationCompleted(GIAtxtRelTranslatorRulesGroup* group);
	private: bool isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroup* group);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_DYNAMIC_RESET_INCLUDE_PARTIALLY_OR_FULLY_ACTIVATED_GROUPS
	private: bool groupPartiallyOrFullyActivated(GIAtxtRelTranslatorRulesGroup* group);
	#endif
	private: bool resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group);
		private: bool resetComponentActivation(GIAtxtRelTranslatorRulesComponent* currentComponent);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	private: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, const bool deleteExistingParseTreeGroupRef);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
	public: bool updatePerformance(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
	#endif
		public: bool traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level, bool print, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents);
		public: bool resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	public: bool resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType);
	#endif

};

#endif

#endif
