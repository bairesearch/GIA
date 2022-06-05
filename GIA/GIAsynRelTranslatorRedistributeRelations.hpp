 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsynRelTranslatorRedistributeRelations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_RELATIONS
#define HEADER_GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_RELATIONS

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
	#include "LRPpreprocessorWordIdentification.hpp"
#include "GIAtranslatorGrammar.hpp"
#include "SHAREDvars.hpp"	//required for convertStringToLowerCase

//should move the following to GIAsynRelTranslatorRedistributeStanfordAndRelations.h/.cpp
class GIAsynRelTranslatorRedistributeRelationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	public: void redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(GIAtranslatorVariablesClass* translatorVariables);
		private: bool correctVerbPOStagAndLemma(GIAentityNode* actionOrSubstanceEntity, GIAfeature* currentFeature);
	#endif
};
		//bool determineVerbCaseAdditional(string* word);


#endif
