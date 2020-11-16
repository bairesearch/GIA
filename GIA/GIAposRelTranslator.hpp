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
 * Project Version: 3o2a 08-November-2020
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
#include "SANIrules.hpp"
#ifdef SANI_FORWARD
#include "SANIformation.hpp"
#include "SANIpropagateOperations.hpp"
#endif
#ifdef SANI_SEQUENCE_GRAMMAR
#include "SANIpropagateCompact.hpp"
#endif
#include "SANIposRelTranslator.hpp"
#ifdef LRP_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
#include "LRPpreprocessorWordIdentification.hpp"
#endif
#include "LRPpreprocessorWordClass.hpp"
#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
#include "GIAnlp.hpp"
#endif
#include "GIAtranslator.hpp"
#include "GIAposRelTranslatorParser.hpp"	//required for createSANItranslatorVariablesFromGIAtranslatorVariables
#include "SHAREDvars.hpp"

class GIAposRelTranslatorClass
{
	//private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#endif
	#ifdef SANI_FORWARD
	private: SANIformationClass SANIformation;
	private: SANIpropagateOperationsClass SANIpropagateOperations;	//required for printComponent/printParseTreeDebugIndentation	
	#endif
	private: SANIrulesClass SANIrules;
	private: SANIposRelTranslatorClass SANIposRelTranslator;
	#ifdef LRP_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#endif
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	private: GIAnlpClass GIAnlp;
	#endif
	private: GIAtranslatorClass GIAtranslator;
	private: GIAposRelTranslatorParserClass GIAposRelTranslatorParser;
	private: SHAREDvarsClass SHAREDvars;

	public: bool parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName);

};
#endif




#endif
