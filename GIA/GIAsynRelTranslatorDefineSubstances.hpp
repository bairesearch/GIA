 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsynRelTranslatorDefineSubstances.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SYN_REL_TRANSLATOR_DEFINE_SUBSTANCES
#define HEADER_GIA_SYN_REL_TRANSLATOR_DEFINE_SUBSTANCES

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#include "GIAsynRelTranslatorGeneric.hpp"

class GIAsynRelTranslatorDefineSubstancesClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsynRelTranslatorGenericClass GIAsynRelTranslatorGeneric;

	//these had to be separated out from the above because they are used by GIAsemRelTranslatorParser.cpp;
	public: void defineSubstancesAllNodes(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	public: void defineSubstancesActionNetworkIndexes(GIAtranslatorVariablesClass* translatorVariables);
	#endif
};


#endif
