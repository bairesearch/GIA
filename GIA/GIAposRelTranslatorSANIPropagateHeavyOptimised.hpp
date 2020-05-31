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
 * File Name: GIAposRelTranslatorSANIPropagateHeavyOptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l1c 28-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Heavy Optimised - ~O(nlogn)
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_HEAVY_OPTIMISED
#define HEADER_GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_HEAVY_OPTIMISED

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAposRelTranslatorRules.hpp"
#include "GIAposRelTranslatorRulesGroupClass.hpp"
#include "GIAposRelTranslatorRulesComponentClass.hpp"
#include "GIAposRelTranslatorSANIPropagateOperations.hpp"
#include "GIAposRelTranslatorSANIFormation.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
#include "GIAposRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED

class GIAposRelTranslatorSANIPropagateHeavyOptimisedClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAposRelTranslatorRulesClass GIAposRelTranslatorRules;
	private: GIAposRelTranslatorRulesGroupClass GIAposRelTranslatorRulesGroupClassObject;
	private: GIAposRelTranslatorRulesComponentClass GIAposRelTranslatorRulesComponentClassObject;
	private: GIAposRelTranslatorSANIPropagateOperationsClass GIAposRelTranslatorSANIPropagateOperations;
	private: GIAposRelTranslatorSANIFormationClass GIAposRelTranslatorSANIFormation;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAposRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkWordGroupIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
			private: bool propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
				private: bool propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
		private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
			private: bool connectToPreviousActivationGroup(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup, GIAposRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext, GIAposRelTranslatorSANIForwardPropogationActivationPointData* forwardPropogationActivationPointData, GIAposRelTranslatorSANIForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalDataNext, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordDataNext, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);		
			private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
					private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAposRelTranslatorDebug* debug);	
						private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAposRelTranslatorDebug* debug);
			
	private: bool printBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* group, int level);

	private: void generateMemoryActiveGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* activationMemoryGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig, GIAposRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroupTemplate, GIAposRelTranslatorRulesGroupActivationMemory** activationMemoryGroupNew, GIAposRelTranslatorRulesGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);
	private: void generateMemoryActiveGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* activationMemoryGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorRulesGroupActivationMemory** activationMemoryGroupNew, GIAposRelTranslatorRulesGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);

	private: void printComponents(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components);

};

#endif
#endif

#endif
