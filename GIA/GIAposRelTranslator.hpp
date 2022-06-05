 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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

	public: bool parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, string outputCFFfileName);

};
#endif




#endif
