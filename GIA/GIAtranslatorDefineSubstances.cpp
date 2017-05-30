/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslatorDefineSubstances.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b3f 25-May-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorDefineSubstances.hpp"

//see XML for dynamic translation code



void GIAtranslatorDefineSubstancesClass::defineSubstancesAllNodes(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAgenericEntityInterpretationParameters param(translatorVariables, true);
	param.useEntityTest = true; param.entityTest = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE; param.entityTestIsNegative = true;
	GIAentityCharacteristic entityCharacteristicsTest1("entityType", GIA_ENTITY_TYPE_NETWORK_INDEX_STRING);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	GIAtranslatorGeneric.genericEntityInterpretation(&param);
}

#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
void GIAtranslatorDefineSubstancesClass::defineSubstancesActionNetworkIndexes(GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	eg 'swim' in 'To swim to the beach requires strength.'
	*/
	GIAgenericEntityInterpretationParameters param(translatorVariables, true);
	GIAentityCharacteristic entityCharacteristicsTest1("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_VERB_STRING);
	GIAentityCharacteristic entityCharacteristicsTest2("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE);
	GIAentityCharacteristic entityCharacteristicsTest3("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_INFINITIVE);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest2);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest3);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	GIAtranslatorGeneric.genericEntityInterpretation(&param);
}
#endif






