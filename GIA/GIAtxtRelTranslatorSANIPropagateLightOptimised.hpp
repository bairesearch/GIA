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
 * File Name: GIAtxtRelTranslatorSANIPropagateLightOptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1b 05-March-2020
 * Requirements: 
 * Description: Textual Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Light Optimised - ~O(n)
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_LIGHT_OPTIMISED
#define HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_LIGHT_OPTIMISED

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
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
#include "GIAtxtRelTranslatorSANIParser.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED


class GIAtxtRelTranslatorSANIPropagateLightOptimisedClass
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
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	private: GIAtxtRelTranslatorSANIParserClass GIAtxtRelTranslatorSANIParser;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
			private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup);
						private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup);
							private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool preprocessSearchForNonResetActivations);
								private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool measureSequentialActivationOnly, bool* sequentialActivationFound, bool* existingActivationFound, bool* resetUnallowed, GIAtxtRelTranslatorDebug* debug);	
									private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool existingActivationFound, GIAtxtRelTranslatorDebug* debug);
										private: bool propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
										#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
										#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
										private: bool solidifyNeuralNetBackpropGroupLastComponent(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level);
										#endif
										#endif
										
										#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
										private: bool componentTests2(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted);
											private: bool findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int previousWordPOStype);
											private: bool findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int existsPreceedingWordPOStype);
										#endif
										
										#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
										private: bool resetAllowed(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent);
											private: bool existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup);
												private: bool existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
										#endif

										#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
										private: bool verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound);
											private: bool getWordIndicesInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, vector<int>* wordIndexList, int level);
										#endif
	
										#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
										private: bool existingActivationVerifyMaxNumberWordsInParseTree(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool existingActivationFound);
											private: int countNumberWordsInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
										#endif
										
										#ifdef GIA_TXT_REL_TRANSLATOR_SANI_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
										private: bool verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, int layer);
											private: bool parseTreeContainsGroup(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFind, int level);
										#endif

										private: void updateParseTreeMaxMinWordIndexOfParent(GIAtxtRelTranslatorRulesGroupParseTree* parentGroup, GIAtxtRelTranslatorRulesComponentParseTree* parentComponent, GIAtxtRelTranslatorRulesGroupParseTree* childGroup);
										
	private: bool printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level);
	private: bool groupActivationCompleted(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	public: bool demarkateOptimumPathwayBackprop(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup);
	private: bool isWordInGroupOptimumPathwayWordList(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData);
	#endif



};

#endif

#endif
