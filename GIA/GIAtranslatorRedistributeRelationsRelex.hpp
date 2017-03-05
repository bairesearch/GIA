/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: GIAtranslatorRedistributeRelationsRelex.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4c 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_RELEX
#define HEADER_GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_RELEX

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
	#include "GIAlrp.hpp"
#include "GIAtranslatorDefineGrammar.hpp"
#include "SHAREDvars.hpp"	//required for convertStringToLowerCase

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
class GIAtranslatorRedistributeRelationsRelexClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorGenericClass GIAtranslatorGeneric;
	public: void redistributeRelexRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser, GIAfeature* featureArrayTemp[]);
		public: void collapseRedundantRelationAndMakeNegativeRelex(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
		private: void redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationAllWithADefinitionFunctionAsPropertyLinks(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	#endif
	#ifdef GIA_ALIASES_RELEX_COMPATIBILITY
		private: void redistributeRelexRelationsCollapseSubjectAndObjectGenerateAppos(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser, GIAfeature* featureArrayTemp[]);
	#endif
	#ifdef GIA_WHO_QUERY_ALIAS_ANSWERS
		private: void redistributeRelexRelationsDetectNameQueries(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAfeature* featureArrayTemp[]);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
		private: void redistributeRelexRelationsInterpretOfAsObjectForContinuousVerbs(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
		private: void redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
		private: void redistributeRelexRelationsDisregardRedundantDefinitionRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
		private: void redistributeRelexRelationsAdverbPlusSubjectRelationAsActionCondition(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], constEffective GIAentityNode* GIAentityNodeArray[]);
	#endif
	#endif
		private: void switchArgumentsAndFunctionsWhereNecessaryRelex(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], constEffective GIAentityNode* GIAentityNodeArray[]);
};
#endif
#endif
