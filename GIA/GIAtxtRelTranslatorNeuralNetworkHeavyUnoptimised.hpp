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
 * File Name: GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g11f 01-March-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Heavy Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
#define HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkOperations.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkFormation.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
#include "GIAtxtRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED

class GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorNeuralNetworkOperationsClass GIAtxtRelTranslatorNeuralNetworkOperations;
	private: GIAtxtRelTranslatorNeuralNetworkFormationClass GIAtxtRelTranslatorNeuralNetworkFormation;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	private: GIAtxtRelTranslatorParserOperationsClass GIAtxtRelTranslatorParserOperations;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace, const bool passThrough);
			private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool passThrough);
				private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroup* inputLayerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool passThrough);
					private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool passThrough);
						private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, const bool passThrough);
							private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, const bool passThrough);
								private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAtxtRelTranslatorDebug* debug);	
									private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAtxtRelTranslatorDebug* debug);
										#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
										#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
										private: bool solidifyNeuralNetBackpropGroupLastComponent(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level);
										#endif
										#endif
										//private: bool findComponentRef(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorRulesComponent* componentRef, GIAtxtRelTranslatorRulesComponent** componentFound);
										private: bool propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough);


	private: bool printBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level);
	private: bool groupActivationCompleted(GIAtxtRelTranslatorRulesGroup* group);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_DYNAMIC_RESET_INCLUDE_PARTIALLY_OR_FULLY_ACTIVATED_GROUPS
	private: bool groupPartiallyOrFullyActivated(GIAtxtRelTranslatorRulesGroup* group);
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	private: GIAtxtRelTranslatorRulesGroup* copyGroup(GIAtxtRelTranslatorRulesGroup* group);
	#endif

};

#endif
#endif

#endif
