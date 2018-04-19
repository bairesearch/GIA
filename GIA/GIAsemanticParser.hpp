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
 * File Name: GIAsemanticParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e11a 21-January-2018
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER
#define HEADER_GIA_SEMANTIC_PARSER

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAsemanticParserOperations.hpp"
#include "GIAsemanticParserDatabase.hpp"
#include "GIAnlp.hpp"
#include "GIAtranslator.hpp"
#include "SHAREDvars.hpp"	//file io

#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
class GIAsemanticParserClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAnlpClass GIAnlp;
	private: GIAtranslatorClass GIAtranslator;
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	public: bool performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputPlainTXTFileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName, const string NLPexeFolderArray[]);
		private: bool lookupSemanticParserFiles(GIAparagraph* firstParagraphInList, const int NLPfeatureParser);
			private: bool lookupSemanticParserFiles(GIAsentence* firstSentenceInList, const int NLPfeatureParser);
};
#endif

#endif
