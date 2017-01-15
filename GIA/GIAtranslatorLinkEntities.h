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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2e6a 08-June-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_LINK_ENTITIES
#define HEADER_GIA_TRANSLATOR_LINK_ENTITIES

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorOperations.h"

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
void linkEntities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool linkPreestablishedReferencesGIA);
	//1-3
	void linkPropertiesPossessiveRelationships(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);								//Stanford Compatible
	void linkPropertiesDescriptiveRelationships(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[], int NLPdependencyRelationsType);				//Stanford Compatible
	#ifdef GIA_USE_ADVANCED_REFERENCING
	void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[], bool linkPreestablishedReferencesGIA);			//Stanford Compatible
	#else
	void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);
	#endif
	void linkDependentActionsType1(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	void linkHavingPropertyConditionsAndBeingDefinitionConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);
	#endif
	void linkSubjectObjectRelationships(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
	void linkSubjectOrObjectRelationships(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
	void linkIndirectObjects(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);										//Stanford Compatible
	void linkObjectSubjectOfPreposition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);		//Stanford Compatible
	void linkConjunctionConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);	//Stanford Compatible;
	void linkConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
		#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
		void createConditionBasedUponPreposition(GIAentityNode * actionOrSubstanceEntity, GIAentityNode * actionOrSubstanceConditionEntity, string relationType, bool negative, Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool sameReferenceSet);	//Stanford Compatible
			void addTimeToSubstance(GIAentityNode * timeConditionEntity);															//Stanford Compatible
			GIAentityNode * addTimeConditionToEntity(GIAentityNode * substanceNode, GIAentityNode * timeConditionEntity, GIAentityNode * conditionTypeConceptEntity, bool sameReferenceSet);	   //Stanford Compatible
			GIAentityNode * addLocationConditionToEntity(GIAentityNode * substanceNode, GIAentityNode * locationConditionEntity, GIAentityNode * conditionTypeConceptEntity, bool sameReferenceSet);  //Stanford Compatible
			GIAentityNode * addReasonConditionToEntity(GIAentityNode * substanceNode, GIAentityNode * reasonConditionEntity, GIAentityNode * conditionTypeConceptEntity, bool sameReferenceSet);	   //Stanford Compatible
			GIAentityNode * addGenericConditionToEntity(GIAentityNode * substanceNode, GIAentityNode * substanceConditionEntity, GIAentityNode * conditionTypeConceptEntity, bool sameReferenceSet);  //Stanford Compatible
		#endif
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	string performPrepositionReduction(string relationType);
	#endif
	void linkDependentActionsType2(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);
	#endif
#endif
