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
 * File Name: GIAtxtRelTranslatorSANIParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1c 05-March-2020
 * Requirements: 
 * Description: Textual Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Parser
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PARSER
#define HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PARSER

#include "GIAglobalDefs.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSER

//#include "GIAtxtRelTranslatorParser.hpp"
#include "GIAtxtRelTranslatorParserOperations.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIApreprocessorWordClass.hpp"

class GIAtxtRelTranslatorSANIParserClass
{
	private: SHAREDvarsClass SHAREDvars;
	//private: GIAtxtRelTranslatorParserClass GIAtxtRelTranslatorParser;
	private: GIAtxtRelTranslatorParserOperationsClass GIAtxtRelTranslatorParserOperations;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	public: bool generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, int layer);
		public: bool generateSemanticRelationsFromTxtRelationsNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer);
	#endif
};

#endif

#endif
