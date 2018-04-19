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
 * File Name: GIAbot.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f3n 10-April-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
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
	
	public: void botSwitchFirstAndSecondPersonSyntactic(GIAtranslatorVariablesClass* translatorVariables);
	public: void botSwitchFirstAndSecondPersonSemantic(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
	#ifndef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	public: void botSwitchFirstAndSecondPersonTxt(GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif
};
#endif


#endif
