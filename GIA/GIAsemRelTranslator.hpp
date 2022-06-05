 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemRelTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SEM_REL_TRANSLATOR
#define HEADER_GIA_SEM_REL_TRANSLATOR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAsemRelTranslatorOperations.hpp"
#include "GIAsemRelTranslatorDatabase.hpp"
#include "GIAnlp.hpp"
#include "GIAtranslator.hpp"
#include "SHAREDvars.hpp"	//file io

#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
class GIAsemRelTranslatorClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAnlpClass GIAnlp;
	private: GIAtranslatorClass GIAtranslator;
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	private: GIAsemRelTranslatorDatabaseClass GIAsemRelTranslatorDatabase;

	public: bool performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, string outputCFFfileName);
		private: bool lookupSemanticParserFiles(const GIAsentence* firstSentenceInList, const int NLPfeatureParser);
			private: bool lookupSemanticParserFiles(const GIAsentence* firstSentenceInList, const int NLPfeatureParser);
};
#endif

#endif
