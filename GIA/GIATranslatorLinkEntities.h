/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: GIATranslatorLinkEntities.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1p1a 08-September-2012
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
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAEntityConnectionClass.h"
#include "GIAConditionNodeClass.h"

//1-3
void linkPropertiesPossessiveRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);								//Stanford Compatible
void linkPropertiesDescriptiveRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType);				//Stanford Compatible
#ifdef GIA_USE_ADVANCED_REFERENCING
void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], bool linkPreestablishedReferencesGIA);			//Stanford Compatible
#else
void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
#endif
void linkSubjectObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
void linkSubjectOrObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
void linkIndirectObjects(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);										//Stanford Compatible
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void linkHavingPropertyConditionsAndBeingDefinitionConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts);
#endif
void linkObjectSubjectOfPreposition(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);		//Stanford Compatible
void linkConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);	//Stanford Compatible
	void createConditionBasedUponPreposition(GIAEntityNode * actionOrSubstanceEntity, GIAEntityNode * actionOrSubstanceConditionEntity, string relationType, bool negative, Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool sameReferenceSet);	//Stanford Compatible
		void addTimeToSubstance(GIAEntityNode * timeConditionEntity);															//Stanford Compatible
		GIAEntityNode * addTimeConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet);	   //Stanford Compatible
		GIAEntityNode * addLocationConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * locationConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet);  //Stanford Compatible
		GIAEntityNode * addReasonConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * reasonConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet);	   //Stanford Compatible
		GIAEntityNode * addGenericConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * substanceConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet);  //Stanford Compatible

bool determineFeatureIndexOfPreposition(Sentence * currentSentenceInList, string * prepositionName, int * indexOfPreposition);

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
string performPrepositionReduction(string relationType);
#endif


#endif
