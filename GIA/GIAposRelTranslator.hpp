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
 * File Name: GIAposRelTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k11c 09-May-2020
 * Requirements: requires plain text file
 * Description: Part-of-speech Relation Translator
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR
#define HEADER_GIA_POS_REL_TRANSLATOR

#include "GIAglobalDefs.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3

//#include "SHAREDvars.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
#include "GIAposRelTranslatorHybrid.hpp"
#endif
#include "GIAposRelTranslatorRules.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_SANI
#include "GIAposRelTranslatorSANIFormation.hpp"
#include "GIAposRelTranslatorSANIPropagateOperations.hpp"
#endif
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
#include "GIAposRelTranslatorSANIPropagateCompact.hpp"
#endif
//#ifndef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
#include "GIAposRelTranslatorPermutations.hpp"
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

class GIAposRelTranslatorClass
{
	//private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI
	private: GIAposRelTranslatorSANIFormationClass GIAposRelTranslatorSANIFormation;
	private: GIAposRelTranslatorSANIPropagateOperationsClass GIAposRelTranslatorSANIPropagateOperations;	//required for printComponent/printParseTreeDebugIndentation	
	#endif
	//#ifndef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	private: GIAposRelTranslatorPermutationsClass GIAposRelTranslatorPermutations;
	//#endif
	private: GIAposRelTranslatorRulesClass GIAposRelTranslatorRules;
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