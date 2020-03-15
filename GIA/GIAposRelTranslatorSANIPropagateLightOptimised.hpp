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
 * File Name: GIAposRelTranslatorSANIPropagateLightOptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k2a 10-March-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Light Optimised - ~O(n)
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_LIGHT_OPTIMISED
#define HEADER_GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_LIGHT_OPTIMISED

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
#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
#include "GIAposRelTranslatorSANIParser.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED


class GIAposRelTranslatorSANIPropagateLightOptimisedClass
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
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	private: GIAposRelTranslatorSANIParserClass GIAposRelTranslatorSANIParser;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAposRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
			private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
						private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup);
							private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool preprocessSearchForNonResetActivations);
								private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool measureSequentialActivationOnly, bool* sequentialActivationFound, bool* existingActivationFound, bool* resetUnallowed, GIAposRelTranslatorDebug* debug);	
									private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool existingActivationFound, GIAposRelTranslatorDebug* debug);
										private: bool propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
										#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
										#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
										private: bool solidifyNeuralNetBackpropGroupLastComponent(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level);
										#endif
										#endif
										
										#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
										private: bool componentTests2(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted);
											private: bool findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int previousWordPOStype);
											private: bool findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int existsPreceedingWordPOStype);
										#endif
										
										#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
										private: bool resetAllowed(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent);
											private: bool existingActivatedComponentCapturesLastWordInSentence(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup);
												private: bool existingActivatedComponentCapturesLastWordInSentence(GIAposRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
										#endif

										#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
										private: bool verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
											private: bool getWordIndicesInParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, vector<int>* wordIndexList, int level);
										#endif
	
										#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
										private: bool existingActivationVerifyMaxNumberWordsInParseTree(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool existingActivationFound);
											private: int countNumberWordsInParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
										#endif
										
										#ifdef GIA_POS_REL_TRANSLATOR_SANI_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
										private: bool verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, int layer);
											private: bool parseTreeContainsGroup(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToFind, int level);
										#endif

										private: void updateParseTreeMaxMinWordIndexOfParent(GIAposRelTranslatorRulesGroupParseTree* parentGroup, GIAposRelTranslatorRulesComponentParseTree* parentComponent, GIAposRelTranslatorRulesGroupParseTree* childGroup);
										
	private: bool printBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* group, int level);
	private: bool groupActivationCompleted(GIAposRelTranslatorRulesGroupNeuralNetwork* group);
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	public: bool demarkateOptimumPathwayBackprop(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup);
	private: bool isWordInGroupOptimumPathwayWordList(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData);
	#endif



};

#endif

#endif
