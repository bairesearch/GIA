 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAbot.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: 
 * Description: Bot
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_BOT
#define HEADER_GIA_BOT


#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"

#define FEATURE_FIRST_PERSON_NUMBER_OF_TYPES (5)
#define FEATURE_SECOND_PERSON_NUMBER_OF_TYPES (5)
static string featureSecondPersonNameArray[FEATURE_SECOND_PERSON_NUMBER_OF_TYPES] = {"you","you","your","yours","yourself"};
static string featureFirstPersonNameArray[FEATURE_FIRST_PERSON_NUMBER_OF_TYPES] = {"i","me","my","mine","myself"};

#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
class GIAbotClass
{
	private: GIAsentenceClassClass GIAsentenceClass;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;

	public: void botSwitchFirstAndSecondPersonSyntactic(GIAtranslatorVariablesClass* translatorVariables);
	public: void botSwitchFirstAndSecondPersonSemantic(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	#ifndef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	public: void botSwitchFirstAndSecondPersonTxt(const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif
};
#endif


#endif
