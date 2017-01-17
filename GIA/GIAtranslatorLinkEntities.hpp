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
 * File Name: GIAtranslatorLinkEntities.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4a 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_LINK_ENTITIES
#define HEADER_GIA_TRANSLATOR_LINK_ENTITIES

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#include "GIAsemanticParserOperations.hpp"

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
class GIAtranslatorLinkEntitiesClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	public: void linkEntities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType, const bool linkPreestablishedReferencesGIA);
	//1-3
		private: void linkPropertiesPossessiveRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);								//Stanford Compatible
		private: void linkPropertiesDescriptiveRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], const int NLPdependencyRelationsType);				//Stanford Compatible
		private: void linkEntityDefinitionsAppositiveOfNouns(const GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], const bool linkPreestablishedReferencesGIA);			//Stanford Compatible
		private: void linkDependentActionsType1(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
		private: void linkHavingPropertyConditionsAndBeingDefinitionConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType);
	#endif
		private: void linkSubjectObjectRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType);	//Stanford Compatible
		private: void linkSubjectOrObjectRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], const unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType);	//Stanford Compatible
		private: void linkIndirectObjects(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);										//Stanford Compatible
		private: void linkObjectSubjectOfPreposition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType);		//Stanford Compatible
		private: void linkConjunctionConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);	//Stanford Compatible;
		private: void linkConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType);	//Stanford Compatible
		#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
			private: void createConditionBasedUponPreposition(GIAentityNode* actionOrSubstanceConditionSubjectEntity, GIAentityNode* actionOrSubstanceConditionObjectEntity, const GIArelation* currentRelationInList, const bool negative, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType, bool sameReferenceSet);
				public: void addTimeToSubstance(GIAentityNode* timeConditionEntity);															//Stanford Compatible
				private: GIAentityNode* addTimeConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* timeConditionEntity, GIAentityNode* conditionNetworkIndexEntity, bool sameReferenceSet);	   //Stanford Compatible
				private: GIAentityNode* addLocationConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* locationConditionEntity, GIAentityNode* conditionNetworkIndexEntity, bool sameReferenceSet);  //Stanford Compatible
				private: GIAentityNode* addReasonConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* reasonConditionEntity, GIAentityNode* conditionNetworkIndexEntity, bool sameReferenceSet);	   //Stanford Compatible
				private: GIAentityNode* addGenericConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* substanceConditionEntity, GIAentityNode* conditionNetworkIndexEntity, bool sameReferenceSet);  //Stanford Compatible
		#endif
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
		private: string performPrepositionReduction(string relationType);
	#endif
		private: void linkDependentActionsType2(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);
};
	#endif
#endif
