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
 * File Name: GIAtxtRelTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j1a 03-August-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR
#define HEADER_GIA_TXT_REL_TRANSLATOR

#include "GIAglobalDefs.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3

//#include "SHAREDvars.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
#include "GIAtxtRelTranslatorHybrid.hpp"
#endif
#include "GIAtxtRelTranslatorRules.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
#include "GIAtxtRelTranslatorNeuralNetworkFormation.hpp"
#endif
#include "GIAtxtRelTranslatorNeuralNetworkOperations.hpp"
//#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
#include "GIAtxtRelTranslatorPermutations.hpp"
//#endif
#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
#include "GIApreprocessorWordIdentification.hpp"
#endif
#include "GIApreprocessorWordClass.hpp"
#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
#include "GIAnlp.hpp"
#endif
#include "GIAtranslator.hpp"
#include "SHAREDvars.hpp"

class GIAtxtRelTranslatorClass
{
	//private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
	private: GIAtxtRelTranslatorHybridClass GIAtxtRelTranslatorHybrid;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	private: GIAtxtRelTranslatorNeuralNetworkFormationClass GIAtxtRelTranslatorNeuralNetworkFormation;
	private: GIAtxtRelTranslatorNeuralNetworkOperationsClass GIAtxtRelTranslatorNeuralNetworkOperations;	//required for printComponent/printParseTreeDebugIndentation	
	#endif
	//#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	private: GIAtxtRelTranslatorPermutationsClass GIAtxtRelTranslatorPermutations;
	//#endif
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	#endif
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	private: GIAnlpClass GIAnlp;
	#endif
	private: GIAtranslatorClass GIAtranslator;
	private: SHAREDvarsClass SHAREDvars;

	public: bool parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName);

};
#endif




#endif
