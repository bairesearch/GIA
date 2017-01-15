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
 * File Name: GIATranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1p11e 24-September-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"

static long currentEntityNodeIDInCompleteList;				//For GIA XML generation only
static long currentEntityNodeIDInConceptEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInSubstanceEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInActionEntityNodesList;			//For GIA XML generation only
static long currentEntityNodeIDInConditionEntityNodesList;		//For GIA XML generation only

static vector<GIAEntityNode*> * entityNodesActiveListComplete;		//For GIA XML generation only
//static vector<GIAEntityNode*> * entityNodesActiveListConcepts;
static vector<GIAEntityNode*> * entityNodesActiveListSubstances;		//For GIA XML generation only
static vector<GIAEntityNode*> * entityNodesActiveListActions;			//For GIA XML generation only
static vector<GIAEntityNode*> * entityNodesActiveListConditions;		//For GIA XML generation only

static bool saveNetwork; 						//For GIA XML generation only / GIA Advanced referencing only
static long currentEntityNodeIDInSentenceCompleteList;			//For GIA Advanced referencing only
static long currentEntityNodeIDInSentenceConceptEntityNodesList;	//For GIA Advanced referencing only

static bool foundComparisonVariable;
static GIAEntityNode* comparisonVariableNode;




void initialiseGIATranslatorForTexualContextOperations()
{
	saveNetwork = true;

	currentEntityNodeIDInCompleteList = 0;
	currentEntityNodeIDInConceptEntityNodesList = 0;
	currentEntityNodeIDInSubstanceEntityNodesList = 0;
	currentEntityNodeIDInActionEntityNodesList = 0;
	currentEntityNodeIDInConditionEntityNodesList = 0;

	/*
	//initialise timeConditionNodesActiveList;		[should be moved elsewhere]
	long firstTimeInNetwork = -14*(10^9)*SECONDS_IN_YEAR;
	string firstTimeNameInNetwork = "beginning";
	GIATimeConditionNode * firstTimeNodeInNetwork = new GIATimeConditionNode();
	firstTimeNodeInNetwork->conditionName = firstTimeNameInNetwork;
	firstTimeNodeInNetwork->totalTimeInSeconds = firstTimeInNetwork;
	timeConditionNodesActiveList->push_back(firstTimeNodeInNetwork);
	timeConditionNumbersActiveList->push_back(firstTimeInNetwork);
	*/

}


bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(Relation * currentRelationInList, GIAEntityNode * GIAEntityNodeArray[], int relationGovernorIndex, int NLPdependencyRelationsType)
{
	bool result = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
		if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD) && (GIAEntityNodeArray[relationGovernorIndex]->entityName == RELATION_ENTITY_BE))
		{//added condition Fri 27 Jan - remove 'be' node artefacts
			disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[relationGovernorIndex]);
			result = false;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "GIAEntityNodeArray[relationGovernorIndex]->disabled = true" << endl;
			#endif
		}
		#endif
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	}
	#endif

	return result;
}

bool isAdjectiveNotConnectedToObjectOrSubject(Sentence * currentSentenceInList, Relation * currentRelationInList, int NLPdependencyRelationsType)
{
	bool passed2 = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
		{
			Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
 			while(currentRelationInList3->next != NULL)
			{
				bool partnerTypeRequiredFound = false;

				for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList3->relationType == relationTypeSubjectNameArray[i])
					{
						partnerTypeRequiredFound = true;
					}
				}
				for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList3->relationType == relationTypeObjectNameArray[i])
					{
						partnerTypeRequiredFound = true;
					}
				}

				if(partnerTypeRequiredFound)
				{
					if(currentRelationInList->relationDependentIndex == currentRelationInList3->relationDependentIndex)
					{//do not add substance, if _advmod argument (eg 'by') is connected to a subject/object
						passed2 = false;
					}
				}
				currentRelationInList3 = currentRelationInList3->next;
			}
		}
		else
		{
			bool passed2 = true;
		}
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	}
	#endif


	return passed2;
}


GIAEntityNode * addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity, bool sameReferenceSet)
{
	//if(thingEntity->isConcept)
	//{
	//	return connectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);
	//}
	//else
	//{
		return addOrConnectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);
	//}
}

//this has been created based upon addOrConnectPropertyToEntity
GIAEntityNode * connectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity, bool sameReferenceSet)
{
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyEntity->disabled))
	{
	if(!(thingEntity->disabled))
	{
	#endif

		thingEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		writeVectorConnection(thingEntity, propertyEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);
		writeVectorConnection(propertyEntity, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return propertyEntity;	//just return original propertyEntity (for compatibility)
}

//changed some instances of addOrConnectPropertyToEntity to addPropertyToEntity
GIAEntityNode * addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingSubstance = propertyEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyEntity->disabled))
	{
	if(!(thingEntity->disabled))
	{
	#endif
		if(!(propertyEntity->isConcept))
		{
			GIAEntityNode * existingSubstance = propertyEntity;

			/*
			if(substanceEntity->hasQualityTemp)
			{
				existingSubstance->isSubstanceQuality = true;
			}
			*/

			thingEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this substance
			writeVectorConnection(thingEntity, existingSubstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(existingSubstance, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);

			newOrExistingSubstance = existingSubstance;

		}
		else
		{			
			GIAEntityNode * newSubstance = addSubstance(propertyEntity);

			/*
			if(substanceEntity->hasQualityTemp)
			{
				newSubstance->isSubstanceQuality = true;
			}
			*/

			writeVectorConnection(newSubstance, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(thingEntity, newSubstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);

			thingEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingSubstance = newSubstance;
		}
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingSubstance;
}


GIAEntityNode * addSubstanceToSubstanceDefinition(GIAEntityNode * substanceEntity)
{
	GIAEntityNode * newOrExistingSubstance = substanceEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(substanceEntity->disabled))
	{
	#endif
		if(substanceEntity->isConcept)
		{
			#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
				/*
				NB GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING is not perfect; it cannot link the blue chicken references together
				A blue chicken is small.
				A red chicken is fat.
				The green chicken ate the pie.
				The blue chicken is late.
				*/
				#ifdef GIA_ENABLE_REFERENCE_LINKING_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
				if(checkEntityHasSubstanceThatWasDeclaredInContext(substanceEntity) && (substanceEntity->grammaticalDefiniteTemp || substanceEntity->grammaticalRelexPersonOrStanfordProperNounTemp))	//NB the grammaticalRelexPersonOrStanfordProperNounTemp condition should only be required here if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS is set to true
				#else
				if(checkEntityHasSubstanceThatWasDeclaredInContext(substanceEntity))
				#endif
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "\tbreak; assigning: substanceEntity->entityName = " << substanceEntity->entityName << endl;
					#endif
					newOrExistingSubstance = getEntitySubstanceThatWasDeclaredInContext(substanceEntity);
					/*
					if(newOrExistingSubstance->isConcept)
					{
						cout << "\tERROR" << endl;
					}
					*/
				}
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "\tbreak; adding: substanceEntity->entityName = " << substanceEntity->entityName << endl;
					#endif
					newOrExistingSubstance = addSubstance(substanceEntity);
				}
			#else
				newOrExistingSubstance = addSubstance(substanceEntity);
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "\tbreak2; substanceEntity->entityName = " << substanceEntity->entityName << endl;
				//cout << "addSubstanceToSubstanceDefinition():" << endl;
				//cout << "\tnewOrExistingSubstance->entityName = " << newOrExistingSubstance->entityName << endl;				
				#endif				
			#endif
		}
		else
		{
			newOrExistingSubstance = substanceEntity;
		}

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	#endif

	return newOrExistingSubstance;
}

void forwardTimeInfoToNewSubstance(GIAEntityNode * entity, GIAEntityNode * newSubstance)
{
	if(entity->hasAssociatedTime)
	{
		newSubstance->hasAssociatedTime = true;
	}
	newSubstance->NormalizedNERTemp = entity->NormalizedNERTemp;	//always required (not just for time info / time condition related)
	newSubstance->NERTemp = entity->NERTemp;

	if(entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)
	{
		newSubstance->hasProgressiveTemp = true;
		//cout << "substance has progressive (eg lying/sitting/being happy)" << endl;
	}

	if(entity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || newSubstance->hasProgressiveTemp)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToSubstance(newSubstance, entity->grammaticalTenseTemp, newSubstance->hasProgressiveTemp);
	}
}

GIAEntityNode * addSubstance(GIAEntityNode * entity)
{

	//configure substance node
	GIAEntityNode * newSubstance = new GIAEntityNode();
	#ifdef GIA_USE_DATABASE
	newSubstance->added = true;
	#endif

	if(saveNetwork)
	{
		newSubstance->idActiveList = currentEntityNodeIDInCompleteList;
		newSubstance->idActiveEntityTypeList = currentEntityNodeIDInSubstanceEntityNodesList;

		entityNodesActiveListComplete->push_back(newSubstance);
		currentEntityNodeIDInCompleteList++;
		entityNodesActiveListSubstances->push_back(newSubstance);
		currentEntityNodeIDInSubstanceEntityNodesList++;
	}
	else
	{
		newSubstance->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}

	newSubstance->entityName = entity->entityName;
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	newSubstance->wordOrig = entity->wordOrig;
	#endif
	newSubstance->idInstance = determineNextIdInstance(entity);
	newSubstance->isSubstance = true;

	entity->hasAssociatedInstance = true;
	entity->hasAssociatedInstanceTemp = true;	////temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	newSubstance->wordNetPOS = entity->wordNetPOS;
	newSubstance->grammaticalNumber = entity->grammaticalNumber;
	forwardTimeInfoToNewSubstance(entity, newSubstance);

	writeVectorConnection(newSubstance, entity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(entity, newSubstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	entity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	#endif

	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	if(entity->isQuery)
	{
		entity->isQuery = false;
		newSubstance->isQuery = true;
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(entity->isWhichQuery)
		{
			entity->isWhichQuery = false;
			newSubstance->isWhichQuery = true;
		}
		#endif
	}
	#endif


	return newSubstance;
}


void upgradeSubstanceToAction(GIAEntityNode * substance)
{
	GIAEntityNode * existingAction = substance;

	//CHECK THIS; must remove from substance list, and add to action list
	(existingAction->entityNodeDefiningThisInstance->back())->entity->hasAssociatedInstanceIsAction = true;
	existingAction->isSubstance = false;
	existingAction->isAction = true;

	if(saveNetwork)
	{	
		#ifdef GIA_TRANSLATOR_DEBUG
		//cout << "existingAction->idActiveEntityTypeList = " << existingAction->idActiveEntityTypeList << endl;
		#endif
		int i=0;
		bool substanceEntityNodesListIteratorIsFound = false;
 		vector<GIAEntityNode*>::iterator substanceEntityNodesListIteratorFound;
		for(vector<GIAEntityNode*>::iterator substanceEntityNodesListIterator = entityNodesActiveListSubstances->begin(); substanceEntityNodesListIterator != entityNodesActiveListSubstances->end(); substanceEntityNodesListIterator++)
		{
			if((*substanceEntityNodesListIterator)->idActiveEntityTypeList == existingAction->idActiveEntityTypeList)
			{
				substanceEntityNodesListIteratorFound = substanceEntityNodesListIterator;
				substanceEntityNodesListIteratorIsFound = true;
			}
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "i = " << i << endl;
			//cout << "(*substanceEntityNodesListIterator)->entityName = " << (*substanceEntityNodesListIterator)->entityName << endl;
			#endif
			i++;
		}
		if(!substanceEntityNodesListIteratorIsFound)
		{
			cout << "error: !...EntityNodesListIteratorIsFound" << endl;
			exit(0);
		}
		entityNodesActiveListSubstances->erase(substanceEntityNodesListIteratorFound);

		/*//removed 8 May 2012
		vector<GIAEntityNode*>::iterator substanceEntityNodesListIterator = entityNodesActiveListSubstances->begin();
		advance(substanceEntityNodesListIterator,existingAction->idActiveEntityTypeList);
		entityNodesActiveListSubstances->erase(substanceEntityNodesListIterator);
		currentEntityNodeIDInSubstanceEntityNodesList--;
		*/

		entityNodesActiveListActions->push_back(existingAction);
		currentEntityNodeIDInActionEntityNodesList++;
	}
}

GIAEntityNode * addActionToActionDefinitionDefineSubstances(GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(actionEntity->disabled))
	{
	#endif
		//configure action node
		if(actionEntity->isConcept)
		{
			#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
				#ifdef GIA_ENABLE_REFERENCE_LINKING_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
				if(checkEntityHasSubstanceThatWasDeclaredInContext(actionEntity) && (actionEntity->grammaticalDefiniteTemp || actionEntity->grammaticalRelexPersonOrStanfordProperNounTemp))	//NB the grammaticalRelexPersonOrStanfordProperNounTemp condition should only be required here if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS is set to true
				#else
				if(checkEntityHasSubstanceThatWasDeclaredInContext(actionEntity))
				#endif
				{
					newOrExistingAction = getEntitySubstanceThatWasDeclaredInContext(actionEntity);
				}
				else
				{
					newOrExistingAction = addAction(actionEntity);
				}
			#else
				newOrExistingAction = addAction(actionEntity);
			#endif
		}
		else
		{
			if(actionEntity->isAction == false)
			{//upgrade associated substance to action
				upgradeSubstanceToAction(actionEntity);
				newOrExistingAction = actionEntity;
			}
			else
			{
				#ifdef USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_ENTITY_INDEX_IN_A_GIVEN_SENTENCE
				newOrExistingAction = addAction(actionEntity);
				#else
				newOrExistingAction = actionEntity;
				#endif
			}
		}

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	#endif

	return newOrExistingAction;
}


GIAEntityNode * addActionToActionDefinition(GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(actionEntity->disabled))
	{
	#endif
		//configure action node
		if(actionEntity->isConcept)
		{
			//always add new actions per sentence (never use actions defined from previous sentences)
			newOrExistingAction = addAction(actionEntity);
		}
		else
		{
			if(actionEntity->isAction == false)
			{//upgrade associated substance to action
				upgradeSubstanceToAction(actionEntity);
				newOrExistingAction = actionEntity;
			}
			else
			{
				#ifdef USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_ENTITY_INDEX_IN_A_GIVEN_SENTENCE
				newOrExistingAction = addAction(actionEntity);
				#else
				newOrExistingAction = actionEntity;
				#endif
			}
		}

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	#endif

	return newOrExistingAction;
}


	//conditions required to be added [eg when, where, how, why]
GIAEntityNode * addAction(GIAEntityNode * actionEntity)
{
	GIAEntityNode * newAction = new GIAEntityNode();
	#ifdef GIA_USE_DATABASE
	newAction->added = true;
	#endif

	if(saveNetwork)
	{
		newAction->idActiveList = currentEntityNodeIDInCompleteList;
		newAction->idActiveEntityTypeList = currentEntityNodeIDInActionEntityNodesList;

		entityNodesActiveListComplete->push_back(newAction);
		currentEntityNodeIDInCompleteList++;
		entityNodesActiveListActions->push_back(newAction);
		currentEntityNodeIDInActionEntityNodesList++;
	}
	else
	{
		newAction->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}

	newAction->entityName = actionEntity->entityName;
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	newAction->wordOrig = actionEntity->wordOrig;
	#endif
	newAction->idInstance = determineNextIdInstance(actionEntity);

	writeVectorConnection(newAction, actionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(actionEntity, newAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	actionEntity->hasAssociatedInstance = true;
	actionEntity->hasAssociatedInstanceIsAction = true;
	actionEntity->hasAssociatedInstanceTemp = true;
	newAction->wordNetPOS = actionEntity->wordNetPOS;
	newAction->isAction = true;
	//WHY WOULD THIS EVER BE REQURIED?; newAction->entityNodeContainingThisSubstance = NULL;

	forwardTimeInfoToNewSubstance(actionEntity, newAction);

	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	if(actionEntity->isQuery)
	{
		actionEntity->isQuery = false;
		newAction->isQuery = true;
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(actionEntity->isWhichQuery)
		{
			actionEntity->isWhichQuery = false;
			newAction->isWhichQuery = true;
		}
		#endif
	}
	#endif

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	actionEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	#endif

	return newAction;
}







/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToSubstance(GIAEntityNode * substanceNode, int tense, vector<GIATimeConditionNode*> *timeConditionNodesActiveList, vector<long> *timeConditionNumbersActiveList)
#else
void addTenseOnlyTimeConditionToSubstance(GIAEntityNode * substanceNode, int tense)
#endif
*/
void addTenseOnlyTimeConditionToSubstance(GIAEntityNode * substanceNode, int tense, bool isProgressive)
{
	substanceNode->conditionType = CONDITION_NODE_TYPE_TIME;

	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = 0; //cannot assign absolute time to an event which occurs in the past.... //calculateTotalTimeInSeconds();
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/

	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();

	newTimeCondition->tense = tense;
	newTimeCondition->tenseOnlyTimeCondition = true;
	newTimeCondition->conditionName = grammaticalTenseNameArray[tense];
	if(isProgressive)
	{
		newTimeCondition->isProgressive = true;
	}
	substanceNode->timeConditionNode = newTimeCondition;


}

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity, bool sameReferenceSet)
{
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(thingEntity->disabled))
	{
	if(!(definitionEntity->disabled))
	{
	#endif

		//configure entity node and entity definition node
		writeVectorConnection(thingEntity, definitionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
		writeVectorConnection(definitionEntity, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif
}

	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
GIAEntityNode * addOrConnectActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity, bool sameReferenceSetSubject, bool sameReferenceSetObject)
{
	GIAEntityNode * newOrExistingAction = actionEntity;
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(objectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif

	newOrExistingAction = addActionToActionDefinition(actionEntity);

	connectActionInstanceToSubject(subjectEntity, newOrExistingAction, sameReferenceSetSubject);

	connectActionInstanceToObject(objectEntity, newOrExistingAction, sameReferenceSetObject);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif
	return newOrExistingAction;
}


void connectActionInstanceToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * newOrExistingAction, bool sameReferenceSet)
{
	//configure subject entity node
	writeVectorConnection(subjectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, subjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, sameReferenceSet);

	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}


GIAEntityNode * addOrConnectActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		connectActionInstanceToSubject(subjectEntity, newOrExistingAction, sameReferenceSet);
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}

void connectActionInstanceToObject(GIAEntityNode * objectEntity, GIAEntityNode * newOrExistingAction, bool sameReferenceSet)
{
	//configure object entity node
	writeVectorConnection(objectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, sameReferenceSet);

	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

GIAEntityNode * addOrConnectActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(objectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		connectActionInstanceToObject(objectEntity, newOrExistingAction, sameReferenceSet);
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}


GIAEntityNode * addOrConnectConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, GIAEntityNode * conditionTypeEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionEntityNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif
		if(!(conditionTypeEntity->isConcept))
		{
			GIAEntityNode * existingCondition = conditionTypeEntity;

			//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this substance
			writeVectorConnection(existingCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(existingCondition, conditionEntityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionEntityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, sameReferenceSet);

			newOrExistingCondition = existingCondition;

		}
		else
		{
			GIAEntityNode * newCondition = addCondition(conditionTypeEntity);

			writeVectorConnection(newCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(newCondition, conditionEntityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionEntityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, sameReferenceSet);

			//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingCondition = newCondition;
		}

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#ifdef GIA_TRANSLATOR_DEBUG
	/*
	}
	else
	{
		cout << "(conditionTypeConceptEntity->disabled)" << endl;
	}
	}
	else
	{
		cout << "(conditionEntityNode->disabled)" << endl;
	}
	}
	else
	{
		cout << "(entityNode->disabled)" << endl;
	}
	*/
	#endif
	#endif

	return newOrExistingCondition;
}

GIAEntityNode * addOrConnectBeingDefinitionConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionDefinitionNode, GIAEntityNode * conditionTypeEntity, bool negative, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionDefinitionNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif

		if(!(conditionTypeEntity->isConcept))
		{
			GIAEntityNode * existingCondition = conditionTypeEntity;

			//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this substance
			writeVectorConnection(existingCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(existingCondition, conditionDefinitionNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
			writeVectorConnection(conditionDefinitionNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);

			newOrExistingCondition = existingCondition;

		}
		else
		{
			GIAEntityNode * newCondition = addCondition(conditionTypeEntity);
			newCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node

			writeVectorConnection(newCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(newCondition, conditionDefinitionNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
			writeVectorConnection(conditionDefinitionNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);

			//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingCondition = newCondition;
		}

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAEntityNode * addOrConnectHavingPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionSubstanceNode, GIAEntityNode * conditionTypeEntity, bool negative, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionSubstanceNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif

		if(!(conditionTypeEntity->isConcept))
		{
			GIAEntityNode * existingCondition = conditionTypeEntity;

			//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this substance
			writeVectorConnection(existingCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionSubstanceNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(existingCondition, conditionSubstanceNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);

			newOrExistingCondition = existingCondition;

		}
		else
		{
			GIAEntityNode * newCondition = addCondition(conditionTypeEntity);
			newCondition->negative = negative;	//overwrite negative with correct one from context; ie that from "having" entity node

			writeVectorConnection(newCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionSubstanceNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(newCondition, conditionSubstanceNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);

			//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingCondition = newCondition;
		}

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAEntityNode * addCondition(GIAEntityNode * conditionEntity)
{
	GIAEntityNode * newCondition = new GIAEntityNode();
	#ifdef GIA_USE_DATABASE
	newCondition->added = true;
	#endif

	if(saveNetwork)
	{
		newCondition->idActiveList = currentEntityNodeIDInCompleteList;
		newCondition->idActiveEntityTypeList = currentEntityNodeIDInConditionEntityNodesList;

		entityNodesActiveListComplete->push_back(newCondition);
		currentEntityNodeIDInCompleteList++;
		entityNodesActiveListConditions->push_back(newCondition);
		currentEntityNodeIDInConditionEntityNodesList++;
	}
	else
	{
		newCondition->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}

	newCondition->entityName = conditionEntity->entityName;
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	newCondition->wordOrig = conditionEntity->wordOrig;
	#endif
	newCondition->idInstance = determineNextIdInstance(conditionEntity);

	writeVectorConnection(newCondition, conditionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(conditionEntity, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	conditionEntity->hasAssociatedInstance = true;
	conditionEntity->hasAssociatedInstanceIsCondition = true;
	conditionEntity->hasAssociatedInstanceTemp = true;
	newCondition->isCondition = true;
	newCondition->negative = conditionEntity->negative;

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	conditionEntity->entityAlreadyDeclaredInThisContext = true;	//added 9 May 2012
	#endif

	return newCondition;
}


string convertStanfordPrepositionToRelex(string * preposition, int NLPdependencyRelationsType, bool * stanfordPrepositionFound)
{
	string relexPreposition = *preposition;
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		string stanfordPrepositionPrependFound = "";
		bool multiwordPrepositionIntermediaryRelationTypeBFound = false;
		for(int i=0; i<REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES; i++)
		{
			string currentStanfordPrepositionPrepend = referenceTypeStanfordParserPrepositionPrependNameArray[i];
			int foundStanfordPrepositionPrepend = preposition->find(currentStanfordPrepositionPrepend);

			if(foundStanfordPrepositionPrepend == string::npos)
			{
				/*
				#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
				cout << "convertStanfordPrepositionToRelex(): error - preposition 'prep_...' not found" << endl;
				cout << "the following dependency relation was expected to be a preposition = " << *preposition << endl;
				exit(0);
				#endif
				*/
			}
			else
			{
				int indexOfFirstRealCharacterInPreposition = currentStanfordPrepositionPrepend.length();
				int lengthOfPreposition = preposition->length() - (indexOfFirstRealCharacterInPreposition);
				relexPreposition = preposition->substr(indexOfFirstRealCharacterInPreposition, lengthOfPreposition);
				*stanfordPrepositionFound = true;
			}
		}
	}
	return relexPreposition;
}


void setTranslatorEntityNodesCompleteList(vector<GIAEntityNode*> * newEntityNodesCompleteList)
{
	entityNodesActiveListComplete = newEntityNodesCompleteList;
}
/*
void setTranslatorConceptEntityNodesList(vector<GIAEntityNode*> * newConceptEntityNodesList)
{
	entityNodesActiveListConcepts = newConceptEntityNodesList;
}
*/
void setTranslatorSubstanceEntityNodesList(vector<GIAEntityNode*> * newSubstanceEntityNodesList)
{
	entityNodesActiveListSubstances = newSubstanceEntityNodesList;
}
void setTranslatorActionEntityNodesList(vector<GIAEntityNode*> * newActionEntityNodesList)
{
	entityNodesActiveListActions = newActionEntityNodesList;
}
void setTranslatorConditionEntityNodesList(vector<GIAEntityNode*> * newConditionEntityNodesList)
{
	entityNodesActiveListConditions = newConditionEntityNodesList;
}



vector<GIAEntityNode*> * getTranslatorEntityNodesCompleteList()
{
	return entityNodesActiveListComplete;
}
/*
vector<GIAEntityNode*> * getTranslatorConceptEntityNodesList()
{
	return entityNodesActiveListConcepts;
}
*/
vector<GIAEntityNode*> * getTranslatorSubstanceEntityNodesList()
{
	return entityNodesActiveListSubstances;
}
vector<GIAEntityNode*> * getTranslatorActionEntityNodesList()
{
	return entityNodesActiveListActions;
}
vector<GIAEntityNode*> * getTranslatorConditionEntityNodesList()
{
	return entityNodesActiveListConditions;
}

void setSaveNetwork(bool val)
{
	saveNetwork = val;
}
bool getSaveNetwork()
{
	return saveNetwork;
}


bool getFoundComparisonVariable()
{
	return foundComparisonVariable;
}
GIAEntityNode* getComparisonVariableNode()
{
	return comparisonVariableNode;
}
void setFoundComparisonVariable(bool value)
{
	foundComparisonVariable = value;
}
void setComparisonVariableNode(GIAEntityNode* newComparisonVariableNode)
{
	comparisonVariableNode = newComparisonVariableNode;
}


long * getCurrentEntityNodeIDInCompleteList()
{
	return &currentEntityNodeIDInCompleteList;
}
long * getCurrentEntityNodeIDInConceptEntityNodesList()
{
	return &currentEntityNodeIDInConceptEntityNodesList;
}
long * getCurrentEntityNodeIDInSubstanceEntityNodesList()
{
	return &currentEntityNodeIDInSubstanceEntityNodesList;
}
long * getCurrentEntityNodeIDInActionEntityNodesList()
{
	return &currentEntityNodeIDInActionEntityNodesList;
}
long * getCurrentEntityNodeIDInConditionEntityNodesList()
{
	return &currentEntityNodeIDInConditionEntityNodesList;
}

long * getCurrentEntityNodeIDInSentenceCompleteList()
{
	return &currentEntityNodeIDInSentenceCompleteList;
}
long * getCurrentEntityNodeIDInSentenceConceptEntityNodesList()
{
	return &currentEntityNodeIDInSentenceConceptEntityNodesList;
}




void applyConceptEntityAlreadyExistsFunction(GIAEntityNode * entity, bool entityAlreadyExistant)
{
	if(entityAlreadyExistant)
	{
		entity->disabled = false;
		entity->firstSentenceToAppearInNetwork = false;
	}
	else
	{
		entity->firstSentenceToAppearInNetwork	= true;
	}
}

void disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	if(entity->firstSentenceToAppearInNetwork)
	{
		entity->disabled = true;
	}
	#endif
}

void disableEntity(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	entity->disabled = true;
	#endif
}


void disableConceptEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(entity);	//CHECKTHIS; only disable the concept if it was created in the immediate context (eg sentence)

	if(entity->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the immediate context (eg sentence)
	//if(entity->AssociatedInstanceNodeList.size() >= 1)
	{
		GIAEntityNode * associateInstance = (entity->AssociatedInstanceNodeList->back())->entity;
		associateInstance->disabled = true;	//and disable their associated instances (substance nodes)
	}
	#endif
}

void disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	entity->disabled = true;

	if(!(entity->entityNodeDefiningThisInstance->empty()))
	{
		//CHECKTHIS; only disable the concept if it was created in the immediate context (eg sentence)
		GIAEntityNode * conceptEntity = (entity->entityNodeDefiningThisInstance->back())->entity;

		disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(conceptEntity);
	}
	#endif
}



void recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(!(entityNode->disabled))
		{
			entityNode->permanentConcept = true;
		}
	}
}


//(used for printing/xml write purposes)
void recordConceptNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(!(entityNode->permanentConcept))
		{
			entityNode->disabled = true;
		}
	}
}

//(used for printing/xml write purposes)
void recordConceptNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(entityNode->disabled)
		{
			entityNode->permanentConcept = false;
		}
		else
		{
			entityNode->permanentConcept = true;
		}
	}
}

void convertRelexPOSTypeToWordnetWordType(string * relexPOStype, int * wordNetPOS)
{
	*wordNetPOS = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOSTypeArray[i] == *relexPOStype)
		{
			*wordNetPOS = featureRelexPOSTypeCrossReferenceWordnetWordTypeArray[i];
		}
	}

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "convertRelexPOSTypeToWordnetWordType(): " << endl;
	cout << "relexPOStype = " << *relexPOStype << endl;
	cout << "wordNetPOS = " << *wordNetPOS << endl;
	*/
	#endif
}

void convertStanfordPOSTagToRelexPOSTypeAndWordnetWordType(string * POStag, string * relexPOStype, int * wordNetPOS)
{
	*relexPOStype = FEATURE_RELEX_POS_TYPE_WORD;
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL; i++)
	{
		if(featurePOSTagMinimalArray[i] == *POStag)
		{
			*relexPOStype = featurePOSTagCrossReferenceRelexPOSTypeArray[i];
		}
	}

	convertRelexPOSTypeToWordnetWordType(relexPOStype, wordNetPOS);

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "convertStanfordPOSTagToRelexPOSTypeAndWordnetWordType(): " << endl;
	cout << "POStag = " << *POStag << endl;
	cout << "relexPOStype = " << *relexPOStype << endl;
	cout << "wordNetPOS = " << *wordNetPOS << endl;
	*/
	#endif
	
}

void generateTempFeatureArray(Feature * firstFeatureInList, Feature * featureArrayTemp[])
{
	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		featureArrayTemp[currentFeatureInList->entityIndex] = currentFeatureInList;
		currentFeatureInList = currentFeatureInList->next;
	}

	/*OLD: is not compatible with Relex query nodes (set to new entity index eg "199")
	//generateFeatureArray;
	int featureIndex = 1;
	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		featureArrayTemp[featureIndex] = currentFeatureInList;
		currentFeatureInList = currentFeatureInList->next;
		featureIndex++;
	}
	*/
}

#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING

/*
//NOT REQUIRED: redundant: this unique check is redundant considering if a concept entity has a substance that was declared in the immediate context, ie sentence, then the entity node being queried will be the substance itself (and not its concept)
bool checkEntityHasSubstanceThatWasDeclaredInContextAndIsUnique(GIAEntityNode * entityNode, int entityIndexTemp, int sentenceIndexTemp)
{
	bool result = false;

	if(checkEntityHasSubstanceThatWasDeclaredInContext(entityNode))
	{
		GIAEntityNode * entitySubstanceDeclaredInContext = getEntitySubstanceThatWasDeclaredInContext(entityNode);
		if(!((entitySubstanceDeclaredInContext->entityIndexTemp == entityIndexTemp) && (entitySubstanceDeclaredInContext->sentenceIndexTemp == sentenceIndexTemp)))
		{
			result = true;
		}
	}

	return result;
}
*/

bool checkEntityHasSubstanceThatWasDeclaredInContext(GIAEntityNode * entityNode)
{
	bool result = false;

	if(entityNode->entityAlreadyDeclaredInThisContext)
	{
		result = true;
	}

	return result;
}

GIAEntityNode * getEntitySubstanceThatWasDeclaredInContext(GIAEntityNode * entityNode)
{
	GIAEntityNode * entityNodeAssociatedInstance = entityNode;

	if(entityNode->entityAlreadyDeclaredInThisContext)
	{
		if(!(entityNode->AssociatedInstanceNodeList->empty()))
		{
			entityNodeAssociatedInstance = (entityNode->AssociatedInstanceNodeList->back())->entity;
		}
		else
		{
			cout << "error: entityAlreadyDeclaredInThisContext && (entityNode->AssociatedInstanceNodeList.size() == 0)" << endl;
			cout << "entityNode->entityName = " <<  entityNode->entityName << endl;
			exit(0);
		}
	}

	return entityNodeAssociatedInstance;
}
#endif

#ifdef GIA_USE_ADVANCED_REFERENCING
bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, Relation * relation)
{

	bool auxillaryIndicatesDifferentReferenceSet = relation->auxillaryIndicatesDifferentReferenceSet;
	bool rcmodIndicatesSameReferenceSet = relation->rcmodIndicatesSameReferenceSet;

	bool sameReferenceSet = defaultSameSetValueForRelation;
	if(auxillaryIndicatesDifferentReferenceSet)
	{
		sameReferenceSet = false;
	}
	if(rcmodIndicatesSameReferenceSet)
	{
		sameReferenceSet = true;
	}


	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\ndetermineSameReferenceSetValue():" << endl;
	cout << "\t" << relation->relationType << "(" << relation->relationGovernor << ", " << relation->relationDependent << ")" << endl;
	cout << "\tauxillaryIndicatesDifferentReferenceSet = " << auxillaryIndicatesDifferentReferenceSet << endl;
	cout << "\trcmodIndicatesSameReferenceSet = " << rcmodIndicatesSameReferenceSet << endl;
	cout << "\tdefaultSameSetValueForRelation = " << defaultSameSetValueForRelation << endl;
	cout << "\tsameReferenceSet = " << sameReferenceSet << endl;
	#endif


	return sameReferenceSet;
}
#endif


GIAEntityNode * findOrAddEntityNodeByNameSimpleWrapperCondition(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int featureIndex, string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	GIAEntityNode * conditionTypeEntity;
	#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
	if(GIAEntityNodeArrayFilled[featureIndex])
	{
		conditionTypeEntity = GIAEntityNodeArray[featureIndex];
	}
	else
	{
		conditionTypeEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListConcepts);
		GIAEntityNodeArrayFilled[featureIndex] = true;
	}
	#else
	conditionTypeEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListConcepts);
	#endif

	return conditionTypeEntity;
}

GIAEntityNode * findOrAddConceptEntityNodeByNameSimpleWrapper(string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	GIAEntityNode * entityNodeFound = NULL;

	/*already available locally...
	long * currentEntityNodeIDInCompleteList;
	long * currentEntityNodeIDInConceptEntityNodesList;
	vector<GIAEntityNode*> * entityNodesActiveListComplete;
	if(saveNetwork)
	{
		entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();
		currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
		currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();
	}
	else
	{
		currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInSentenceCompleteList();
		currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInSentenceConceptEntityNodesList();
	}
	*/

	long entityIndex = -1;
	entityNodeFound = findOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodeName, entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList, saveNetwork);

	applyConceptEntityAlreadyExistsFunction(entityNodeFound, *entityAlreadyExistant);

	return entityNodeFound;
}



/*these functions have been added for GIA Database compatibility*/
//this function does write to database, but prepares data structures for write to database (at the end of the user sequence, writeDatabase() is written...)
void writeVectorConnection(GIAEntityNode * entityNode, GIAEntityNode * entityNodeToAdd, int connectionType, bool sameReferenceSet)
{
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	if(!(entityNode->wasReferenceTemp && entityNodeToAdd->wasReferenceTemp))
	{
	#endif

		vector<GIAEntityConnection*> * vectorConnection = &(entityNode->entityVectorConnectionsArray[connectionType]);
		if(entityVectorConnectionIsBasicArray[connectionType])
		{
			vectorConnection->clear();	//clear the vector (basic connections only support 1 node)

		}

		GIAEntityConnection * newConnection = new GIAEntityConnection();
		newConnection->entity = entityNodeToAdd;
		vectorConnection->push_back(newConnection);

		#ifdef GIA_USE_ADVANCED_REFERENCING
		newConnection->sameReferenceSet = sameReferenceSet;
		/*
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "writeVectorConnection: newConnection->sameReferenceSet = " << sameReferenceSet << endl;
		#endif
		*/
		#endif

		#ifdef GIA_USE_DATABASE
		if((getUseDatabase() == GIA_USE_DATABASE_TRUE_READ_ACTIVE) || (getUseDatabase() == GIA_USE_DATABASE_TRUE_READ_INACTIVE))	//NB even if not accessing the database for new information (read), still prepare nodes for database write
		{
			//#ifdef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS		//why is this preprocessor check not required???
			//required for database syncronisation with RAM
			if(!(entityNode->entityVectorConnectionsReferenceListLoadedArray[connectionType]))
			{
				cout << "error: writeVectorConnection called, but entityVectorConnectionsReferenceListLoadedArray set to false" << endl;
				cout << "entityNode = " << entityNode->entityName << ", entityNodeToAdd = " << entityNodeToAdd->entityName << ", connectionType = " << connectionType << endl;
				exit(0);
			}
			//#endif

			newConnection->entityName = entityNodeToAdd->entityName;
			newConnection->idInstance = entityNodeToAdd->idInstance;
			newConnection->loaded = true;
			newConnection->modified = false;
			newConnection->added = true;		//this allows for fast update of the DB (append reference connections)
		}
		#endif

	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	}
	#endif
}

long determineNextIdInstance(GIAEntityNode * entity)
{
	long nextIdInstance;
	#ifdef GIA_DATABASE_DEBUG
	cout << "\t\tDEBUG: determineNextIdInstance(); 0. entity->entityName = " << entity->entityName << endl;
	cout << "\t\tDEBUG: determineNextIdInstance(); 0. entity->idInstance = " << entity->idInstance << endl;
	#endif
	GIAEntityNode * conceptEntity;
	#ifdef GIA_APPLY_BUG_WORKAROUND_WHERE_A_CONCEPT_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE
	if(entity->idInstance == GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE)
	{
		//the current entity is a concept entity
		conceptEntity = entity;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance(); 1b. conceptEntity->entityName = " << conceptEntity->entityName << endl;
		cout << "\t\tDEBUG: determineNextIdInstance(); 1b. conceptEntity->idInstance = " << conceptEntity->idInstance << endl;
		#endif	
	}
	else
	{
	#endif
		if(!(entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE].empty()))
		{
			//the current entity is a substance of a concept entity
			conceptEntity = entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE].back()->entity;
			#ifdef GIA_DATABASE_DEBUG
			cout << "\t\tDEBUG: determineNextIdInstance(); 1a. conceptEntity->entityName = " << conceptEntity->entityName << endl;
			cout << "\t\tDEBUG: determineNextIdInstance(); 1a. conceptEntity->idInstance = " << conceptEntity->idInstance << endl;
			#endif
		}
		else
		{
			//the current entity is a concept entity
			conceptEntity = entity;
			#ifdef GIA_DATABASE_DEBUG
			cout << "\t\tDEBUG: determineNextIdInstance(); 1b. conceptEntity->entityName = " << conceptEntity->entityName << endl;
			cout << "\t\tDEBUG: determineNextIdInstance(); 1b. conceptEntity->idInstance = " << conceptEntity->idInstance << endl;
			#endif
		}
	#ifdef GIA_APPLY_BUG_WORKAROUND_WHERE_A_CONCEPT_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE
	}
	#endif

#if defined(GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS) || !defined(GIA_USE_DATABASE)
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(conceptEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].empty()))
	{
		long previousIdInstance = conceptEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].back()->entity->idInstance;
		nextIdInstance = previousIdInstance + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance(); 2a. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	else
	{
		nextIdInstance = GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance(); 2b. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	#else
	long numberOfInstances =  conceptEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].size();
	nextIdInstance = numberOfInstances;
	#endif
#else
	if(conceptEntity->conceptEntityLoaded == NULL)
	{
		cout <<"ERROR: (conceptEntity->conceptEntityLoaded == NULL)" << endl;
	}
	GIAconceptEntityLoaded * conceptEntityLoaded = conceptEntity->conceptEntityLoaded;
	#ifdef GIA_DATABASE_DEBUG
	cout << "conceptEntityLoaded->numberOfInstances = " << conceptEntityLoaded->numberOfInstances << endl;
	#endif
	conceptEntityLoaded->numberOfInstances = conceptEntityLoaded->numberOfInstances + 1;
	nextIdInstance = conceptEntityLoaded->numberOfInstances;
#endif

	return nextIdInstance;
}


#ifdef GIA_USE_DATABASE
void addEntityNodeToActiveLists(GIAEntityNode * entity, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{//NB add reference set entity to active list complete + appropriate list (substance/action/condition) [NB the reference set entity will already be added to concept active list entityNodesActiveListConcepts...] - this function enables references to be written to XML

	/*already available locally...
	long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
	long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();
	long * currentEntityNodeIDInSubstanceEntityNodesList = getCurrentEntityNodeIDInSubstanceEntityNodesList();
	long * currentEntityNodeIDInActionEntityNodesList = getCurrentEntityNodeIDInActionEntityNodesList();
	long * currentEntityNodeIDInConditionEntityNodesList = getCurrentEntityNodeIDInConditionEntityNodesList();
	*/

	if(saveNetwork)
	{
		if(entity->isConcept)
		{
			//do nothing - assume reference set entity already added to concept active list (in createGIACoreferenceInListBasedUponIdentifiedReferenceSets())
		}
		else if(entity->isAction)
		{
			entity->idActiveEntityTypeList = currentEntityNodeIDInActionEntityNodesList;
			entityNodesActiveListActions->push_back(entity);
			currentEntityNodeIDInActionEntityNodesList++;
		}
		else if(entity->isCondition)
		{
			entity->idActiveEntityTypeList = currentEntityNodeIDInConditionEntityNodesList;
			entityNodesActiveListConditions->push_back(entity);
			currentEntityNodeIDInConditionEntityNodesList++;
		}
		else if(entity->isSubstance)
		{
			entity->idActiveEntityTypeList = currentEntityNodeIDInSubstanceEntityNodesList;
			entityNodesActiveListSubstances->push_back(entity);
			currentEntityNodeIDInSubstanceEntityNodesList++;
		}

		entity->idActiveList = currentEntityNodeIDInCompleteList;
		entityNodesActiveListComplete->push_back(entity);
		currentEntityNodeIDInCompleteList++;
	}
	else
	{//this case should never be used...
		entity->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}
}
#endif



#ifdef GIA_SUPPORT_ALIASES
void mergeEntityNodesAddAlias(GIAEntityNode * entityNode, GIAEntityNode * entityNodeToMerge)
{
	entityNode->aliasList.push_back(entityNodeToMerge->entityName);

	#ifdef GIA_ALIASES_DEBUG
	cout << "\n" << endl;
	cout << "entityNode->entityName = " << entityNode->entityName << endl;
	cout << "entityNodeToMerge->entityName = " << entityNodeToMerge->entityName << endl;
	#endif

	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		for(vector<GIAEntityConnection*>::iterator connectionIter = entityNodeToMerge->entityVectorConnectionsArray[i].begin(); connectionIter != entityNodeToMerge->entityVectorConnectionsArray[i].end(); )
		{
			bool connectionIterErased = false;
			//connect entityNodeToMerge ambient node to entityNode
			GIAEntityNode * entityConnectedToEntityToMerge = (*connectionIter)->entity;

			#ifdef GIA_ALIASES_DEBUG
			if(entityConnectedToEntityToMerge->isConcept)
			{
				cout << "entityConnectedToEntityToMerge->entityName = " << entityConnectedToEntityToMerge->entityName << " (concept)" << endl;
			}
			else
			{
				cout << "entityConnectedToEntityToMerge->entityName = " << entityConnectedToEntityToMerge->entityName << endl;
			}
			#endif

			//disconnect reference sources from each other, as their connection between each other will be redeclared in current context
			int iInverted = inverseVectorConnectionsArray[i];
			for(vector<GIAEntityConnection*>::iterator connectionIter2 = entityConnectedToEntityToMerge->entityVectorConnectionsArray[iInverted].begin(); connectionIter2 != entityConnectedToEntityToMerge->entityVectorConnectionsArray[iInverted].end(); )
			{
				bool connectionIter2Erased = false;
				GIAEntityNode * entityConnectedToEntityConnectedToEntityToMerge = (*connectionIter2)->entity;

				#ifdef GIA_ALIASES_DEBUG
				if(entityConnectedToEntityConnectedToEntityToMerge->isConcept)
				{
					cout << "entityConnectedToEntityConnectedToEntityToMerge->entityName = " << entityConnectedToEntityConnectedToEntityToMerge->entityName << " (concept)" << endl;
				}
				else
				{
					cout << "entityConnectedToEntityConnectedToEntityToMerge->entityName = " << entityConnectedToEntityConnectedToEntityToMerge->entityName << endl;
				}
				#endif				

				if(entityNodeToMerge->idActiveList == entityConnectedToEntityConnectedToEntityToMerge->idActiveList)	//OR (entityNodeToMerge == entityConnectedToEntityConnectedToEntityToMerge)?
				{
					/*required for advanced referencing (eg concept Tom has associated instance Dog) [NB this means that instances can appear to have more than one entityNodeDefiningThisInstance]
					if(entityConnectedToEntityToMerge->isConcept)
					{
						//disconnect entityConnectedToEntityConnectedToEntityToMerge from entityConnectedToEntityToMerge (concept) (z2)
						#ifdef GIA_ALIASES_DEBUG
						cout << "disconnect entityConnectedToEntityConnectedToEntityToMerge (" << entityConnectedToEntityConnectedToEntityToMerge->entityName << ") from entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (concept) (z2)" << endl;
						#endif
						//delete *connectionIter2;	//delete connection
						connectionIter2 = entityConnectedToEntityToMerge->entityVectorConnectionsArray[iInverted].erase(connectionIter2);		//(*connectionIter2)->entity = NULL;	//need a better delete routine
						connectionIter2Erased = true;
						#ifdef GIA_USE_DATABASE
						//(*connectionIter2)->modified = true;
						entityConnectedToEntityToMerge->entityVectorConnectionsRemovedArray[iInverted] = true;	//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
						#endif
					}
					else
					{
					*/
						//connect entityConnectedToEntityConnectedToEntityToMerge back to entityNode (z)
						#ifdef GIA_ALIASES_DEBUG
						cout << "change entityConnectedToEntityConnectedToEntityToMerge (" << entityConnectedToEntityConnectedToEntityToMerge->entityName << ") to entityNode (" << entityNode->entityName << ") (z)" << endl;
						#endif

						(*connectionIter2)->entity = entityNode;
						#ifdef GIA_USE_DATABASE
						(*connectionIter2)->modified = true;
						#endif
					/*
					}
					*/
				}

				if(!connectionIter2Erased)
				{
					connectionIter2++;
				}
			}

			//connect entityNode to entityConnectedToEntityToMerge (x)
			if(entityNode->idActiveList != entityConnectedToEntityToMerge->idActiveList)
			{
				/*required for advanced referencing (eg concept Tom has associated instance Dog) [NB this means that instances can appear to have more than one entityNodeDefiningThisInstance]
				if(!(entityConnectedToEntityToMerge->isConcept))
				{
				*/
					#ifdef GIA_ALIASES_DEBUG
					cout << "connect entityNode (" << entityNode->entityName << ") to entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (x)" << endl;
					#endif
					#ifdef GIA_USE_ADVANCED_REFERENCING
					bool sameReferenceSet = (*connectionIter)->sameReferenceSet;
					#else
					bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					#endif
					writeVectorConnection(entityNode, entityConnectedToEntityToMerge, i, sameReferenceSet);
				/*
				}
				*/
			}

			//disconnect entityConnectedToEntityToMerge from entityNodeToMerge
			#ifdef GIA_ALIASES_DEBUG
			if(entityConnectedToEntityToMerge->isConcept)
			{
				cout << "disconnect entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (concept) from entityNodeToMerge (" << entityNodeToMerge->entityName << ") (x2)" << endl;
			}
			else
			{
				cout << "disconnect entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") from entityNodeToMerge (" << entityNodeToMerge->entityName << ") (x2)" << endl;
			}
			#endif
			//delete *connectionIter;	//delete connection
			connectionIter = entityNodeToMerge->entityVectorConnectionsArray[i].erase(connectionIter);		//(*connectionIter)->entity = NULL;	//need a better delete routine
			connectionIterErased = true;
			#ifdef GIA_USE_DATABASE
			//(*connectionIter)->modified = true;
			entityNodeToMerge->entityVectorConnectionsRemovedArray[i] = true;	//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
			#endif

			if(!connectionIterErased)
			{
				connectionIter++;
			}
		}
	}

	//disconnect entityNode from entityNodeToMerge
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); )
		{
			bool connectionIterErased = false;
			GIAEntityNode * entityConnectedToEntity = (*connectionIter)->entity;
			#ifdef GIA_ALIASES_DEBUG
			cout << "entityConnectedToEntity->entityName = " << entityConnectedToEntity->entityName << endl;
			cout << "entityNodeToMerge->entityName = " << entityNodeToMerge->entityName << endl;
			#endif			
			if(entityConnectedToEntity->idActiveList == entityNodeToMerge->idActiveList)
			{
				//disconnect entityNodeToMerge from entityNode (y)
				#ifdef GIA_ALIASES_DEBUG
				cout << "disconnect entityNodeToMerge (" << entityNodeToMerge->entityName << ") from entityNode (" << entityNode->entityName << ") (y)" << endl;
				#endif
				connectionIter = entityNode->entityVectorConnectionsArray[i].erase(connectionIter);		//(*connectionIter)->entity = NULL;	//need a better delete routine
				connectionIterErased = true;
				#ifdef GIA_USE_DATABASE
				//(*connectionIter)->modified = true;
				entityNode->entityVectorConnectionsRemovedArray[i] = true;	//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
				#endif
			}
			if(!connectionIterErased)
			{
				connectionIter++;
			}
		}
	}

	//copy secondary node properties
	if(entityNodeToMerge->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		entityNode->conditionType = entityNodeToMerge->conditionType;
		entityNode->timeConditionNode = entityNodeToMerge->timeConditionNode;
	}
	if(entityNodeToMerge->hasQuantity)
	{
		entityNode->hasQuantity = entityNodeToMerge->hasQuantity;
		entityNode->quantityNumber = entityNodeToMerge->quantityNumber;
		entityNode->hasQuantity = entityNodeToMerge->hasQuantity;
		entityNode->quantityModifier = entityNodeToMerge->quantityModifier;
		entityNode->quantityModifierString = entityNodeToMerge->quantityModifierString;
		entityNode->hasQuantityMultiplier = entityNodeToMerge->hasQuantityMultiplier;
	}
	if(entityNodeToMerge->hasMeasure)
	{
		entityNode->hasMeasure = entityNodeToMerge->hasMeasure;
		entityNode->measureType = entityNodeToMerge->measureType;
	}
	if(entityNodeToMerge->isQuery)
	{
		entityNode->isQuery = entityNodeToMerge->isQuery;
		entityNode->isWhichQuery = entityNodeToMerge->isWhichQuery;
		entityNode->isNameQuery = entityNodeToMerge->isNameQuery;
	}

	disableEntity(entityNodeToMerge);
	
	#ifdef GIA_ALIASES_DEBUG
	cout << "finished: mergeEntityNodesAddAlias" << endl;
	#endif

}
#endif

