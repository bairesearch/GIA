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
 * Project Version: 3g1a 24-April-2018
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
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

class GIAtxtRelTranslatorNeuralNetworkClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorNeuralNetworkFormationClass GIAtxtRelTranslatorNeuralNetworkFormation;
	private: ANNdisplayClass ANNdisplay;
	//private: GIAtxtRelTranslatorNeuralNetworkClass GIAtxtRelTranslatorNeuralNetworkClassObject;

	public: bool executeTxtRelTranslatorNeuralNetwork(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool parseIsolatedSubreferenceSets);
		private: bool propagateWordThroughNetworkIntro(GIApreprocessorPlainTextWord* currentWord, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool parseIsolatedSubreferenceSets);
			//private: bool propagateWordThroughNetworkInputLayerGroup(GIAtxtRelTranslatorRulesGroup* inputLayerGroup);
			private: bool propagateWordThroughNetworkGroupIntro(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalDataPackage* forwardPropogationSignalDataPackage);
				private: bool propagateWordThroughNetworkGroup(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalDataPackage* forwardPropogationSignalDataPackage, int layer);
					private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalDataPackage* forwardPropogationSignalDataPackage, int layer);	
						private: bool propagateWordThroughNetworkGroupComponent(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalDataPackage* forwardPropogationSignalDataPackage, const bool activationSequenceCompleted, int layer);
							private: bool componentTests(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalDataPackage* forwardPropogationSignalDataPackage);
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
								private: bool findPreviousActivatedComponentInGroupWrapper(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int wordPOStype, int previousWordPOStype);
								private: bool findPreceedingActivatedComponentInGroupWrapper(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int wordPOStype, int existsPreceedingWordPOStype);
								#else
								bool findPreviousOrPreceedingActivatedComponentInHigherLayerNetwork(GIAtxtRelTranslatorRulesGroup* group, int wordPOStype, bool findFirstComponent);
									bool findPreviousActivatedComponentInGroupWrapper(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int previousWordPOStype);
										bool findPreviousActivatedComponentInGroup(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int previousWordPOStype, bool* foundPreviousActivatedWord);
									bool findPreceedingActivatedComponentInGroupWrapper(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int existsPreceedingWordPOStype);
										bool findPreceedingActivatedComponentInGroup(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int existsPreceedingWordPOStype);
								#endif
								#endif
					private: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, bool* activationSequenceCompleted);
					private: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components);
					private: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	private: bool resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType);
	private: bool resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType);
	#endif
	private: bool resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);
	private: bool resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType);
	private: bool printBackpropActivationPath(GIAtxtRelTranslatorRulesGroup* group, int level);
	private: bool groupActivationCompleted(GIAtxtRelTranslatorRulesGroup* group);
	private: bool resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group);
		private: bool resetComponentActivation(GIAtxtRelTranslatorRulesComponent* currentComponent);
	private: bool resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group);
	private: bool setParseTreeGroupRefOfAllComponents(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* parseTreeGroupRef);
	private: bool componentHasSubcomponents(GIAtxtRelTranslatorRulesComponent* component);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET
	private: bool solidifyNeuralNetIntro(GIAtxtRelTranslatorRulesGroup* group, int level);
		private: bool solidifyNeuralNetGroupLastComponent(GIAtxtRelTranslatorRulesGroup* group, int level);
		private: bool solidifyNeuralNetGroup(GIAtxtRelTranslatorRulesGroup* group, int level);
			private: bool solidifyNeuralNetComponent(GIAtxtRelTranslatorRulesComponent* currentComponent, int level, const bool lastComponent, const bool solidifyComponent);
	#endif
		private: bool isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroup* group);
	#endif

};

#endif

#endif
