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
 * File Name: GIAtranslatorLinkEntities.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2j5e 08-June-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_LINK_ENTITIES
#define HEADER_GIA_TRANSLATOR_LINK_ENTITIES

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorOperations.h"

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
void linkEntities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool linkPreestablishedReferencesGIA);
	//1-3
	void linkPropertiesPossessiveRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);								//Stanford Compatible
	void linkPropertiesDescriptiveRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], int NLPdependencyRelationsType);				//Stanford Compatible
	void linkEntityDefinitionsAppositiveOfNouns(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], bool linkPreestablishedReferencesGIA);			//Stanford Compatible
	void linkDependentActionsType1(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	void linkHavingPropertyConditionsAndBeingDefinitionConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType);
	#endif
	void linkSubjectObjectRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
	void linkSubjectOrObjectRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
	void linkIndirectObjects(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);										//Stanford Compatible
	void linkObjectSubjectOfPreposition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType);		//Stanford Compatible
	void linkConjunctionConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts);	//Stanford Compatible;
	void linkConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
		#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
		void createConditionBasedUponPreposition(GIAentityNode* actionOrSubstanceConditionSubjectEntity, GIAentityNode* actionOrSubstanceConditionObjectEntity, GIArelation* currentRelationInList, bool negative, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool sameReferenceSet);
			void addTimeToSubstance(GIAentityNode* timeConditionEntity);															//Stanford Compatible
			GIAentityNode* addTimeConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* timeConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet);	   //Stanford Compatible
			GIAentityNode* addLocationConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* locationConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet);  //Stanford Compatible
			GIAentityNode* addReasonConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* reasonConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet);	   //Stanford Compatible
			GIAentityNode* addGenericConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* substanceConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet);  //Stanford Compatible
		#endif
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	string performPrepositionReduction(string relationType);
	#endif
	void linkDependentActionsType2(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts);
	#endif
#endif
