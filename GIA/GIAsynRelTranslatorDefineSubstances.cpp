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
 * File Name: GIAsynRelTranslatorDefineSubstances.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g9a 03-January-2019
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAsynRelTranslatorDefineSubstances.hpp"

//see XML for dynamic translation code



void GIAsynRelTranslatorDefineSubstancesClass::defineSubstancesAllNodes(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAgenericEntityInterpretationParameters param(translatorVariables, true, translatorVariables->linkPreestablishedReferencesGIA);
	param.useEntityTest = true; param.entityTest = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE; param.entityTestIsNegative = true;
	GIAentityCharacteristic entityCharacteristicsTest1("entityType", GIA_ENTITY_TYPE_NETWORK_INDEX_STRING);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	GIAsynRelTranslatorGeneric.genericEntityInterpretation(&param);
}

#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
void GIAsynRelTranslatorDefineSubstancesClass::defineSubstancesActionNetworkIndexes(GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	eg 'swim' in 'To swim to the beach requires strength.'
	*/
	GIAgenericEntityInterpretationParameters param(translatorVariables, true, translatorVariables->linkPreestablishedReferencesGIA);
	GIAentityCharacteristic entityCharacteristicsTest1("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_VERB_STRING);
	GIAentityCharacteristic entityCharacteristicsTest2("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE);
	GIAentityCharacteristic entityCharacteristicsTest3("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_INFINITIVE);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest2);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest3);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	GIAsynRelTranslatorGeneric.genericEntityInterpretation(&param);
}
#endif






