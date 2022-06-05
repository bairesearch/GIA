 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsynRelTranslatorDefineSubstances.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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






