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
 * File Name: GIAtxtRelTranslatorSANIPropagateHeavyOptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1c 05-March-2020
 * Requirements: 
 * Description: Textual Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Heavy Optimised - ~O(nlogn)
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_HEAVY_OPTIMISED
#define HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_HEAVY_OPTIMISED

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorSANIPropagateOperations.hpp"
#include "GIAtxtRelTranslatorSANIFormation.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
#include "GIAtxtRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED

class GIAtxtRelTranslatorSANIPropagateHeavyOptimisedClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorSANIPropagateOperationsClass GIAtxtRelTranslatorSANIPropagateOperations;
	private: GIAtxtRelTranslatorSANIFormationClass GIAtxtRelTranslatorSANIFormation;
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
	private: GIAtxtRelTranslatorParserOperationsClass GIAtxtRelTranslatorParserOperations;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkWordGroupIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
			private: bool propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
				private: bool propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
		private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
			private: bool connectToPreviousActivationGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup, GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext, GIAtxtRelTranslatorSANIForwardPropogationActivationPointData* forwardPropogationActivationPointData, GIAtxtRelTranslatorSANIForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalDataNext, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordDataNext, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);		
			private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
					private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorDebug* debug);	
						private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorDebug* debug);
							//private: bool findComponentRef(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork* componentRef, GIAtxtRelTranslatorRulesComponentNeuralNetwork** componentFound);
							#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
							private: bool componentWordConnectivityTestsWrapper(GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData);
							private: bool getMinMaxWordIndexInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin);
							#endif
			
	private: bool printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level);

	private: void generateMemoryActiveGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* activationMemoryGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroupTemplate, GIAtxtRelTranslatorRulesGroupActivationMemory** activationMemoryGroupNew, GIAtxtRelTranslatorRulesGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);
	private: void generateMemoryActiveGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* activationMemoryGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroupActivationMemory** activationMemoryGroupNew, GIAtxtRelTranslatorRulesGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);

	private: void printComponents(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components);

};

#endif
#endif

#endif
