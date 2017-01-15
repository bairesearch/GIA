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
 * File Name: GIAtranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t4a 26-July-2013
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIAtimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIAtranslatorOperations.h"
#include "GIAdatabase.h"

static long currentEntityNodeIDinCompleteList;				//For GIA XML generation only
static long currentEntityNodeIDinConceptEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInSubstanceEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInActionEntityNodesList;			//For GIA XML generation only
static long currentEntityNodeIDInConditionEntityNodesList;		//For GIA XML generation only

static vector<GIAentityNode*> * entityNodesActiveListComplete;		//For GIA XML generation only
//static vector<GIAentityNode*> * entityNodesActiveListConcepts;
static vector<GIAentityNode*> * entityNodesActiveListSubstances;		//For GIA XML generation only
static vector<GIAentityNode*> * entityNodesActiveListActions;			//For GIA XML generation only
static vector<GIAentityNode*> * entityNodesActiveListConditions;		//For GIA XML generation only

unordered_map<string, GIAentityNode*> *entityNodesActiveListCompleteFastIndex;

static bool saveNetwork; 						//For GIA XML generation only / GIA Advanced referencing only
static long currentEntityNodeIDInSentenceCompleteList;			//For GIA Advanced referencing only
static long currentEntityNodeIDInSentenceConceptEntityNodesList;	//For GIA Advanced referencing only

static bool foundComparisonVariable;
static GIAentityNode* comparisonVariableNode;




void initialiseGIATranslatorForTexualContextOperations()
{
	saveNetwork = true;

	currentEntityNodeIDinCompleteList = 0;
	currentEntityNodeIDinConceptEntityNodesList = 0;
	currentEntityNodeIDInSubstanceEntityNodesList = 0;
	currentEntityNodeIDInActionEntityNodesList = 0;
	currentEntityNodeIDInConditionEntityNodesList = 0;

	/*
	//initialise timeConditionNodesActiveList;		[should be moved elsewhere]
	long firstTimeInNetwork = -14*(10^9)*SECONDS_IN_YEAR;
	string firstTimeNameInNetwork = "beginning";
	GIAtimeConditionNode * firstTimeNodeInNetwork = new GIAtimeConditionNode();
	firstTimeNodeInNetwork->conditionName = firstTimeNameInNetwork;
	firstTimeNodeInNetwork->totalTimeInSeconds = firstTimeInNetwork;
	timeConditionNodesActiveList->push_back(firstTimeNodeInNetwork);
	timeConditionNumbersActiveList->push_back(firstTimeInNetwork);
	*/

}

#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
//this is no longer required with redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition, etc..
bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(Relation * currentRelationInList, GIAentityNode * GIAentityNodeArray[], int relationGovernorIndex, int NLPdependencyRelationsType)
{
	bool result = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
		if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD) && (GIAentityNodeArray[relationGovernorIndex]->entityName == RELATION_ENTITY_BE))
		{//added condition Fri 27 Jan - remove 'be' node artefacts
			disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[relationGovernorIndex]);
			result = false;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "GIAentityNodeArray[relationGovernorIndex]->disabled = true" << endl;
			#endif
		}
		#endif
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	}
	#endif

	return result;
}

//this is no longer required with redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition, etc..
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
#endif


GIAentityNode * addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(GIAentityNode * thingEntity, GIAentityNode * propertyEntity, bool sameReferenceSet)
{
	if(thingEntity->isConcept)
	{
	      return connectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);
	}
	else
	{
	      return addOrConnectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);
	}
}

//this has been created based upon addOrConnectPropertyToEntity
GIAentityNode * connectPropertyToEntity(GIAentityNode * thingEntity, GIAentityNode * propertyEntity, bool sameReferenceSet)
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
GIAentityNode * addOrConnectPropertyToEntity(GIAentityNode * thingEntity, GIAentityNode * propertyEntity, bool sameReferenceSet)
{
	GIAentityNode * newOrExistingSubstance = propertyEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyEntity->disabled))
	{
	if(!(thingEntity->disabled))
	{
	#endif
		if(!(propertyEntity->isConcept))
		{
			GIAentityNode * existingSubstance = propertyEntity;

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
			GIAentityNode * newSubstance = addSubstance(propertyEntity);

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


GIAentityNode * addSubstanceToSubstanceDefinition(GIAentityNode * substanceEntity)
{
	GIAentityNode * newOrExistingSubstance = substanceEntity;

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

void forwardInfoToNewSubstance(GIAentityNode * entity, GIAentityNode * newSubstance)
{	
	if(entity->hasAssociatedTime)
	{
		newSubstance->hasAssociatedTime = true;
	}
	newSubstance->NormalizedNERtemp = entity->NormalizedNERtemp;	//always required (not just for time info / time condition related)
	newSubstance->NERTemp = entity->NERTemp;

	if(entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)
	{
		newSubstance->hasProgressiveTemp = true;
		//cout << "substance has progressive (eg lying/sitting/being happy)" << endl;
	}

	/*//execution of addTenseOnlyTimeConditionToSubstance has been shifted from forwardInfoToNewSubstance to a separate function - 26 July 2013 
	//cout << "entity = " << entity->entityName << endl;
	//cout << "entity->grammaticalTenseTemp = " << entity->grammaticalTenseTemp << endl;	
	if(entity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || entity->hasProgressiveTemp)	//changed from newSubstance->hasProgressiveTemp to entity->hasProgressiveTemp 26 July 2013
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToSubstance(newSubstance, entity->grammaticalTenseTemp, entity->hasProgressiveTemp);
	}
	*/
	newSubstance->grammaticalTenseTemp = entity->grammaticalTenseTemp;

	#ifdef GIA_SUPPORT_ALIASES
	if(entity->isName)
	{
		newSubstance->isName = true;
	}	
	#endif
	
	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	if(entity->isQuery)
	{
		entity->isQuery = false;
		newSubstance->isQuery = true;
		
		//this propogation was only defined for substance instances [generalised 1t1m 24 July 2013]:
		#ifdef GIA_1S8D_LOW_PRI_RELEX_UPDATE_CHECK_THAT_IT_DOESNT_BREAK_STANFORD_OPTIMISATION_APPLY_FIX_TO_IS_NAME_QUERY_PROPOGATION
		#ifdef GIA_SUPPORT_ALIASES
		if(entity->isNameQuery)
		{
			entity->isNameQuery = false;		
			newSubstance->isNameQuery = true;
		}
		#endif		
		#endif
		
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(entity->isWhichOrEquivalentWhatQuery)
		{
			entity->isWhichOrEquivalentWhatQuery = false;
			newSubstance->isWhichOrEquivalentWhatQuery = true;
		}
		#endif
	}
	#endif

	//this propogation was only defined for substance instances [generalised 1t1m 24 July 2013]:
	if(entity->isToBeComplimentOfActionTemp)
	{	
		newSubstance->isToBeComplimentOfActionTemp = true;
	} 
		
	if(entity->negative)
	{
		newSubstance->negative = true;
	}	
}

GIAentityNode * addSubstance(GIAentityNode * entity)
{

	//configure substance node
	GIAentityNode * newSubstance = new GIAentityNode();
	#ifdef GIA_USE_DATABASE
	newSubstance->added = true;
	#endif
	
	newSubstance->isSubstance = true;
	addInstanceEntityNodeToActiveLists(newSubstance);

	newSubstance->entityName = entity->entityName;
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	newSubstance->wordOrig = entity->wordOrig;
	#endif
	newSubstance->idInstance = determineNextIdInstance(entity);
	
	entity->hasAssociatedInstance = true;
	entity->hasAssociatedInstanceTemp = true;	////temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	newSubstance->wordNetPOS = entity->wordNetPOS;
	newSubstance->grammaticalNumber = entity->grammaticalNumber;
	#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
	newSubstance->isSubstanceConcept = entity->isSubstanceConcept;
	#endif
	forwardInfoToNewSubstance(entity, newSubstance);

	writeVectorConnection(newSubstance, entity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(entity, newSubstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	entity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	#endif

	return newSubstance;
}


void upgradeSubstanceToAction(GIAentityNode * substance)
{
	GIAentityNode * existingAction = substance;

	//CHECK THIS; must remove from substance list, and add to action list
	getPrimaryConceptNodeDefiningInstance(existingAction)->hasAssociatedInstanceIsAction = true;
	existingAction->isSubstance = false;
	existingAction->isAction = true;

	if(saveNetwork)
	{	
		#ifdef GIA_TRANSLATOR_DEBUG
		//cout << "existingAction->idActiveEntityTypeList = " << existingAction->idActiveEntityTypeList << endl;
		#endif
		int i=0;
		bool substanceEntityNodesListIteratorIsFound = false;
 		vector<GIAentityNode*>::iterator substanceEntityNodesListIteratorFound;
		for(vector<GIAentityNode*>::iterator substanceEntityNodesListIterator = entityNodesActiveListSubstances->begin(); substanceEntityNodesListIterator != entityNodesActiveListSubstances->end(); substanceEntityNodesListIterator++)
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
		vector<GIAentityNode*>::iterator substanceEntityNodesListIterator = entityNodesActiveListSubstances->begin();
		advance(substanceEntityNodesListIterator,existingAction->idActiveEntityTypeList);
		entityNodesActiveListSubstances->erase(substanceEntityNodesListIterator);
		currentEntityNodeIDInSubstanceEntityNodesList--;
		*/

		entityNodesActiveListActions->push_back(existingAction);
		currentEntityNodeIDInActionEntityNodesList++;
	}
}

GIAentityNode * addActionToActionDefinitionDefineSubstances(GIAentityNode * actionEntity)
{
	GIAentityNode * newOrExistingAction = actionEntity;

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
				#ifdef GIA_USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_ENTITY_INDEX_IN_A_GIVEN_SENTENCE
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


GIAentityNode * addActionToActionDefinition(GIAentityNode * actionEntity)
{
	GIAentityNode * newOrExistingAction = actionEntity;

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
				#ifdef GIA_USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_ENTITY_INDEX_IN_A_GIVEN_SENTENCE
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
GIAentityNode * addAction(GIAentityNode * actionEntity)
{
	GIAentityNode * newAction = new GIAentityNode();
	#ifdef GIA_USE_DATABASE
	newAction->added = true;
	#endif

	newAction->isAction = true;
	addInstanceEntityNodeToActiveLists(newAction);

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
	//WHY WOULD THIS EVER BE REQURIED?; newAction->entityNodeContainingThisSubstance = NULL;

	forwardInfoToNewSubstance(actionEntity, newAction);

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	actionEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	#endif

	return newAction;
}







/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToSubstance(GIAentityNode * substanceNode, int tense, vector<GIAtimeConditionNode*> *timeConditionNodesActiveList, vector<long> *timeConditionNumbersActiveList)
#else
void addTenseOnlyTimeConditionToSubstance(GIAentityNode * substanceNode, int tense)
#endif
*/
void addTenseOnlyTimeConditionToSubstance(GIAentityNode * substanceNode, int tense, bool isProgressive)
{
	substanceNode->conditionType = CONDITION_NODE_TYPE_TIME;

	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = 0; //cannot assign absolute time to an event which occurs in the past.... //calculateTotalTimeInSeconds();
	GIAtimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIAtimeConditionNode * newTimeCondition = new GIAtimeConditionNode();
	#endif
	*/

	GIAtimeConditionNode * newTimeCondition = new GIAtimeConditionNode();

	newTimeCondition->tense = tense;
	newTimeCondition->tenseOnlyTimeCondition = true;
	newTimeCondition->conditionName = grammaticalTenseNameArray[tense];
	if(isProgressive)
	{
		newTimeCondition->isProgressive = true;
	}
	substanceNode->timeConditionNode = newTimeCondition;


}

void addDefinitionToEntity(GIAentityNode * thingEntity, GIAentityNode * definitionEntity, bool sameReferenceSet)
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
GIAentityNode * addOrConnectActionToEntity(GIAentityNode * subjectEntity, GIAentityNode * objectEntity, GIAentityNode * actionEntity, bool sameReferenceSetSubject, bool sameReferenceSetObject)
{
	GIAentityNode * newOrExistingAction = actionEntity;
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(objectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection * connectionFound = findEntityNodeNameInVector(subjectEntity, &(actionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode * currentActionNodeInList = connectionFound->entity;
			bool foundNode2 = false;
			GIAentityConnection * connectionFound2 = findEntityNodePointerInVector(currentActionNodeInList, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, &foundNode2);
			if(foundNode2)
			{
				if(newOrExistingAction != currentActionNodeInList)
				{
					newOrExistingAction->disabled = true;
					newOrExistingAction = currentActionNodeInList;
				}
			}		
		}
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


void connectActionInstanceToSubject(GIAentityNode * subjectEntity, GIAentityNode * newOrExistingAction, bool sameReferenceSet)
{
	//configure action subject entity node
	writeVectorConnection(subjectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, subjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, sameReferenceSet);

	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}


GIAentityNode * addOrConnectActionToSubject(GIAentityNode * subjectEntity, GIAentityNode * actionEntity, bool sameReferenceSet)
{
	GIAentityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		/*//do not presume single linked actions/conditions are identical
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection * connectionFound = findEntityNodeNameInVector(subjectEntity, &(actionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode * currentActionNodeInList = connectionFound->entity;
			if(newOrExistingAction != currentActionNodeInList)
			{
				newOrExistingAction->disabled = true;
				newOrExistingAction = currentActionNodeInList;
			}		
		}
		#endif
		*/	
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		connectActionInstanceToSubject(subjectEntity, newOrExistingAction, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}

void connectActionInstanceToObject(GIAentityNode * objectEntity, GIAentityNode * newOrExistingAction, bool sameReferenceSet)
{
	//configure action object entity node
	writeVectorConnection(objectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, sameReferenceSet);

	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

GIAentityNode * addOrConnectActionToObject(GIAentityNode * objectEntity, GIAentityNode * actionEntity, bool sameReferenceSet)
{
	GIAentityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(objectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		/*//do not presume single linked actions/conditions are identical
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection * connectionFound = findEntityNodeNameInVector(objectEntity, &(actionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode * currentActionNodeInList = connectionFound->entity;
			if(newOrExistingAction != currentActionNodeInList)
			{
				newOrExistingAction->disabled = true;
				newOrExistingAction = currentActionNodeInList;
			}		
		}
		#endif
		*/
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		connectActionInstanceToObject(objectEntity, newOrExistingAction, sameReferenceSet);
		
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}


GIAentityNode * addOrConnectConditionToEntity(GIAentityNode * entityNode, GIAentityNode * conditionEntityNode, GIAentityNode * conditionTypeEntity, bool sameReferenceSet)
{
	GIAentityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionEntityNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection * connectionFound = findEntityNodeNameInVector(entityNode, &(conditionTypeEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode * currentConditionNodeInList = connectionFound->entity;
			bool foundNode2 = false;
			GIAentityConnection * connectionFound2 = findEntityNodePointerInVector(currentConditionNodeInList, conditionEntityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, &foundNode2);
			if(foundNode2)
			{
				if(newOrExistingCondition != currentConditionNodeInList)
				{
					newOrExistingCondition->disabled = true;
					newOrExistingCondition = currentConditionNodeInList;
				}
			}		
		}
		#endif
		
		newOrExistingCondition = addConditionToConditionDefinition(conditionTypeEntity);

		#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		//required to compensate for defineSubstancesActions() being exectuted before linkDependentActionsType1()
		newOrExistingCondition->isSubstance = false;	//required because defineSubstancesActions() defines substances [not actions]
		newOrExistingCondition->isCondition = true;
		#endif
		//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToSubject(entityNode, newOrExistingCondition, sameReferenceSet);
		connectConditionInstanceToObject(conditionEntityNode, newOrExistingCondition, sameReferenceSet);
				
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

GIAentityNode * addConditionToConditionDefinition(GIAentityNode * conditionTypeEntity)
{
	GIAentityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(conditionTypeEntity->disabled))	//Added 12 October 2012 (for consistency with addActionToActionDefinition)
	{
	#endif
		//configure condition node
		if(conditionTypeEntity->isConcept)
		{
			//always add new conditions per sentence (never use conditions defined from previous sentences)
			newOrExistingCondition = addCondition(conditionTypeEntity);
		}
		else
		{
			#ifdef GIA_USE_SUPPORT_MULTIPLE_CONDITION_INSTANCES_PER_CONDITION_ENTITY_INDEX_IN_A_GIVEN_SENTENCE
			newOrExistingCondition = addCondition(conditionTypeEntity);	//Added 12 October 2012 (for consistency with addActionToActionDefinition)
			#else
			newOrExistingCondition = conditionTypeEntity;
			#endif
		}
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	#endif

	return newOrExistingCondition;
}


void connectConditionInstanceToSubject(GIAentityNode * subjectEntity, GIAentityNode * newOrExistingCondition, bool sameReferenceSet)
{
	//configure condition subject entity node
	writeVectorConnection(subjectEntity, newOrExistingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingCondition, subjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
}

void connectConditionInstanceToObject(GIAentityNode * objectEntity, GIAentityNode * newOrExistingCondition, bool sameReferenceSet)
{
	//configure condition object entity node
	writeVectorConnection(objectEntity, newOrExistingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingCondition, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, sameReferenceSet);
}


GIAentityNode * addOrConnectBeingDefinitionConditionToEntity(GIAentityNode * entityNode, GIAentityNode * conditionDefinitionNode, GIAentityNode * conditionTypeEntity, bool negative, bool sameReferenceSet)
{
	GIAentityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionDefinitionNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif
		newOrExistingCondition = addConditionToConditionDefinition(conditionTypeEntity);

		//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToSubject(entityNode, newOrExistingCondition, sameReferenceSet);
		addDefinitionToEntity(newOrExistingCondition, conditionDefinitionNode, sameReferenceSet);
		
		if(conditionTypeEntity->isConcept)
		{
			newOrExistingCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node
		}
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAentityNode * addOrConnectHavingPropertyConditionToEntity(GIAentityNode * entityNode, GIAentityNode * conditionSubstanceNode, GIAentityNode * conditionTypeEntity, bool negative, bool sameReferenceSet)
{
	GIAentityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionSubstanceNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif
		newOrExistingCondition = addConditionToConditionDefinition(conditionTypeEntity);

		//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToSubject(entityNode, newOrExistingCondition, sameReferenceSet);
		connectPropertyToEntity(newOrExistingCondition, conditionSubstanceNode, sameReferenceSet);

		if(conditionTypeEntity->isConcept)
		{
			newOrExistingCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node
		}
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAentityNode * addCondition(GIAentityNode * conditionEntity)
{
	GIAentityNode * newCondition = new GIAentityNode();
	#ifdef GIA_USE_DATABASE
	newCondition->added = true;
	#endif

	newCondition->isCondition = true;
	addInstanceEntityNodeToActiveLists(newCondition);

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
	newCondition->negative = conditionEntity->negative;	//check forwardInfoToNewSubstance() is not required

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	conditionEntity->entityAlreadyDeclaredInThisContext = true;	//added 9 May 2012
	#endif

	return newCondition;
}

//assumes prepositions have previously been converted to stanford prep_preposition format during preprocessor (for robustness)
string convertPrepositionToRelex(string * preposition, bool * prepositionFound)
{
	*prepositionFound = false;
	string relexPreposition = *preposition;
	for(int i=0; i<REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES; i++)
	{
		string currentStanfordPrepositionPrepend = referenceTypeStanfordParserPrepositionPrependNameArray[i];
		int foundStanfordPrepositionPrepend = preposition->find(currentStanfordPrepositionPrepend);

		if(foundStanfordPrepositionPrepend != string::npos)
		{
			int indexOfFirstRealCharacterInPreposition = currentStanfordPrepositionPrepend.length();
			int lengthOfPreposition = preposition->length() - (indexOfFirstRealCharacterInPreposition);
			relexPreposition = preposition->substr(indexOfFirstRealCharacterInPreposition, lengthOfPreposition);
			*prepositionFound = true;
		}
	}
	
	return relexPreposition;
}


void setTranslatorEntityNodesCompleteList(vector<GIAentityNode*> * newEntityNodesCompleteList)
{
	entityNodesActiveListComplete = newEntityNodesCompleteList;
}
void setTranslatorEntityNodesCompleteListFastIndex(unordered_map<string, GIAentityNode*> * newEntityNodesCompleteListFastIndex)
{
	entityNodesActiveListCompleteFastIndex = newEntityNodesCompleteListFastIndex;
}
/*
void setTranslatorConceptEntityNodesList(vector<GIAentityNode*> * newConceptEntityNodesList)
{
	entityNodesActiveListConcepts = newConceptEntityNodesList;
}
*/
void setTranslatorSubstanceEntityNodesList(vector<GIAentityNode*> * newSubstanceEntityNodesList)
{
	entityNodesActiveListSubstances = newSubstanceEntityNodesList;
}
void setTranslatorActionEntityNodesList(vector<GIAentityNode*> * newActionEntityNodesList)
{
	entityNodesActiveListActions = newActionEntityNodesList;
}
void setTranslatorConditionEntityNodesList(vector<GIAentityNode*> * newConditionEntityNodesList)
{
	entityNodesActiveListConditions = newConditionEntityNodesList;
}



vector<GIAentityNode*> * getTranslatorEntityNodesCompleteList()
{
	return entityNodesActiveListComplete;
}
unordered_map<string, GIAentityNode*> * getTranslatorEntityNodesCompleteListFastIndex()
{
	return entityNodesActiveListCompleteFastIndex;
}
/*
vector<GIAentityNode*> * getTranslatorConceptEntityNodesList()
{
	return entityNodesActiveListConcepts;
}
*/
vector<GIAentityNode*> * getTranslatorSubstanceEntityNodesList()
{
	return entityNodesActiveListSubstances;
}
vector<GIAentityNode*> * getTranslatorActionEntityNodesList()
{
	return entityNodesActiveListActions;
}
vector<GIAentityNode*> * getTranslatorConditionEntityNodesList()
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
GIAentityNode* getComparisonVariableNode()
{
	return comparisonVariableNode;
}
void setFoundComparisonVariable(bool value)
{
	foundComparisonVariable = value;
}
void setComparisonVariableNode(GIAentityNode* newComparisonVariableNode)
{
	comparisonVariableNode = newComparisonVariableNode;
}


long * getCurrentEntityNodeIDinCompleteList()
{
	return &currentEntityNodeIDinCompleteList;
}
long * getCurrentEntityNodeIDinConceptEntityNodesList()
{
	return &currentEntityNodeIDinConceptEntityNodesList;
}
long * getCurrentEntityNodeIDinSubstanceEntityNodesList()
{
	return &currentEntityNodeIDInSubstanceEntityNodesList;
}
long * getCurrentEntityNodeIDinActionEntityNodesList()
{
	return &currentEntityNodeIDInActionEntityNodesList;
}
long * getCurrentEntityNodeIDinConditionEntityNodesList()
{
	return &currentEntityNodeIDInConditionEntityNodesList;
}

long * getCurrentEntityNodeIDinSentenceCompleteList()
{
	return &currentEntityNodeIDInSentenceCompleteList;
}
long * getCurrentEntityNodeIDinSentenceConceptEntityNodesList()
{
	return &currentEntityNodeIDInSentenceConceptEntityNodesList;
}




void applyConceptEntityAlreadyExistsFunction(GIAentityNode * entity, bool entityAlreadyExistant)
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

void disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	if(entity->firstSentenceToAppearInNetwork)
	{
		entity->disabled = true;
	}
	#endif
}

void disableEntity(GIAentityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	entity->disabled = true;
	#endif
}


void disableConceptEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(entity);	//CHECKTHIS; only disable the concept if it was created in the immediate context (eg sentence)

	if(entity->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the immediate context (eg sentence)
	//if(entity->associatedInstanceNodeList.size() >= 1)
	{
		GIAentityNode * associateInstance = (entity->associatedInstanceNodeList->back())->entity;
		associateInstance->disabled = true;	//and disable their associated instances (substance nodes)
	}
	#endif
}

//NB only disables the primary concept node defining the instance
void disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	entity->disabled = true;

	if(!(entity->entityNodeDefiningThisInstance->empty()))
	{
		//CHECKTHIS; only disable the concept if it was created in the immediate context (eg sentence)
		GIAentityNode * conceptEntity = getPrimaryConceptNodeDefiningInstance(entity);

		disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(conceptEntity);
	}
	#endif
}



void recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(!(entityNode->disabled))
		{
			entityNode->permanentConcept = true;
		}
	}
}


//(used for printing/xml write purposes)
void recordConceptNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(!(entityNode->permanentConcept))
		{
			entityNode->disabled = true;
		}
	}
}

//(used for printing/xml write purposes)
void recordConceptNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListMapIter->second;
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

void convertRelexPOStypeToWordnetWordType(string * relexPOStype, int * wordNetPOS)
{
	*wordNetPOS = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOStypeArray[i] == *relexPOStype)
		{
			*wordNetPOS = featureRelexPOStypeCrossReferenceWordnetWordTypeArray[i];
		}
	}

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "convertRelexPOStypeToWordnetWordType(): " << endl;
	cout << "relexPOStype = " << *relexPOStype << endl;
	cout << "wordNetPOS = " << *wordNetPOS << endl;
	*/
	#endif
}

void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(string * POStag, string * relexPOStype, int * wordNetPOS)
{
	*relexPOStype = FEATURE_RELEX_POS_TYPE_WORD;
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL; i++)
	{
		if(featurePOStagMinimalArray[i] == *POStag)
		{
			*relexPOStype = featurePOStagCrossReferenceRelexPOStypeArray[i];
		}
	}

	convertRelexPOStypeToWordnetWordType(relexPOStype, wordNetPOS);

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "convertStanfordPOStagToRelexPOStypeAndWordnetWordType(): " << endl;
	cout << "POStag = " << *POStag << endl;
	cout << "relexPOStype = " << *relexPOStype << endl;
	cout << "wordNetPOS = " << *wordNetPOS << endl;
	*/
	#endif
	
}

void generateTempFeatureArray(Feature * firstFeatureInList, Feature * featureArrayTemp[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		featureArrayTemp[w] = NULL;	//initialise as NULL (required to prevent crashes during array access) - added 14 July 2013
	}
	
	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		//cout << "currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
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
bool checkEntityHasSubstanceThatWasDeclaredInContextAndIsUnique(GIAentityNode * entityNode, int entityIndexTemp, int sentenceIndexTemp)
{
	bool result = false;

	if(checkEntityHasSubstanceThatWasDeclaredInContext(entityNode))
	{
		GIAentityNode * entitySubstanceDeclaredInContext = getEntitySubstanceThatWasDeclaredInContext(entityNode);
		if(!((entitySubstanceDeclaredInContext->entityIndexTemp == entityIndexTemp) && (entitySubstanceDeclaredInContext->sentenceIndexTemp == sentenceIndexTemp)))
		{
			result = true;
		}
	}

	return result;
}
*/

bool checkEntityHasSubstanceThatWasDeclaredInContext(GIAentityNode * entityNode)
{
	bool result = false;

	if(entityNode->entityAlreadyDeclaredInThisContext)
	{
		result = true;
	}

	return result;
}

GIAentityNode * getEntitySubstanceThatWasDeclaredInContext(GIAentityNode * entityNode)
{
	GIAentityNode * entityNodeAssociatedInstance = entityNode;

	if(entityNode->entityAlreadyDeclaredInThisContext)
	{
		if(!(entityNode->associatedInstanceNodeList->empty()))
		{
			entityNodeAssociatedInstance = (entityNode->associatedInstanceNodeList->back())->entity;
		}
		else
		{
			cout << "error: entityAlreadyDeclaredInThisContext && (entityNode->associatedInstanceNodeList.size() == 0)" << endl;
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


GIAentityNode * findOrAddEntityNodeByNameSimpleWrapperCondition(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int featureIndex, string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
{
	GIAentityNode * conditionTypeEntity;
	#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
	if(GIAentityNodeArrayFilled[featureIndex])
	{
		conditionTypeEntity = GIAentityNodeArray[featureIndex];
	}
	else
	{
		conditionTypeEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListConcepts);
		GIAentityNodeArrayFilled[featureIndex] = true;
	}
	#else
	conditionTypeEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListConcepts);
	#endif

	return conditionTypeEntity;
}

GIAentityNode * findOrAddConceptEntityNodeByNameSimpleWrapper(string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
{
	GIAentityNode * entityNodeFound = NULL;

	/*already available locally...
	long * currentEntityNodeIDinCompleteList;
	long * currentEntityNodeIDinConceptEntityNodesList;
	vector<GIAentityNode*> * entityNodesActiveListComplete;
	if(saveNetwork)
	{
		entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();
		currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinCompleteList();
		currentEntityNodeIDinConceptEntityNodesList = getCurrentEntityNodeIDinConceptEntityNodesList();
	}
	else
	{
		currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinSentenceCompleteList();
		currentEntityNodeIDinConceptEntityNodesList = getCurrentEntityNodeIDinSentenceConceptEntityNodesList();
	}
	*/

	long entityIndex = -1;
	entityNodeFound = findOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodeName, entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDinCompleteList, &currentEntityNodeIDinConceptEntityNodesList, saveNetwork);

	applyConceptEntityAlreadyExistsFunction(entityNodeFound, *entityAlreadyExistant);

	return entityNodeFound;
}


		

/*these functions have been added for GIA Database compatibility*/
//this function does write to database, but prepares data structures for write to database (at the end of the user sequence, writeDatabase() is written...)
void writeVectorConnection(GIAentityNode * entityNode, GIAentityNode * entityNodeToAdd, int connectionType, bool sameReferenceSet)
{
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	if(!(entityNode->wasReferenceTemp && entityNodeToAdd->wasReferenceTemp))
	{
	#endif
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_PROPERTIES_AND_DEFINITIONS
		//see if link already exists between the two nodes, and if so increment the confidence
		bool foundNode = false;
		GIAentityConnection * connectionFound = findEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &foundNode);
		if(foundNode)
		{
			connectionFound->confidence = connectionFound->confidence + GIA_ENTITY_CONNECTION_CONFIDENCE_INCREMENT;
		}
		else
		{
		#endif
			vector<GIAentityConnection*> * vectorConnection = &(entityNode->entityVectorConnectionsArray[connectionType]);
			if(entityVectorConnectionIsBasicArray[connectionType])
			{
				#ifdef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				if(connectionType != GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)
				{
				#endif
					vectorConnection->clear();	//clear the vector (basic connections only support 1 node)
				#ifdef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				}
				#endif			
			}
			
			GIAentityConnection * newConnection = new GIAentityConnection();
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
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_PROPERTIES_AND_DEFINITIONS
		}
		#endif
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	}
	#endif
}

#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES

GIAentityConnection * findEntityNodePointerInVector(GIAentityNode * entityNode, GIAentityNode * entityNodeToFind, int connectionType, bool * foundNode)
{
	GIAentityConnection * connectionFound = NULL;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if((*connectionIter)->entity == entityNodeToFind)
		{
			*foundNode = true;
			connectionFound = *connectionIter;
		}
	}
	return connectionFound;
}

GIAentityConnection * findEntityNodeNameInVector(GIAentityNode * entityNode, string * entityNodeNameToFind, int connectionType, bool * foundNode)
{
	GIAentityConnection * connectionFound = NULL;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if((*connectionIter)->entity->entityName == *entityNodeNameToFind)
		{
			*foundNode = true;
			connectionFound = *connectionIter;			
		}
	}
	return connectionFound;
}

#endif


long determineNextIdInstance(GIAentityNode * entity)
{
	long nextIdInstance;
	#ifdef GIA_DATABASE_DEBUG
	cout << "\t\tDEBUG: determineNextIdInstance(); 0. entity->entityName = " << entity->entityName << endl;
	cout << "\t\tDEBUG: determineNextIdInstance(); 0. entity->idInstance = " << entity->idInstance << endl;
	#endif
	GIAentityNode * conceptEntity;
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
			conceptEntity = getPrimaryConceptNodeDefiningInstance(entity);
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

//#if defined(GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS) || !defined(GIA_USE_DATABASE)
//check #elif !defined GIA_USE_DATABASE is OK instead of #elif !defined(GIA_USE_DATABASE)
#ifdef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
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
#elif !defined GIA_USE_DATABASE
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
/*//replaced with optimised function findEntityNodesActiveListCompleteFastIndexDBactive()
bool entityInActiveListComplete(string * entityName, long idInstance)
{
	bool entityAlreadyInActiveListComplete = false;
	for(vector<GIAentityNode*>::iterator entityNodesActiveListCompleteIter = entityNodesActiveListComplete->begin(); entityNodesActiveListCompleteIter != entityNodesActiveListComplete->end(); entityNodesActiveListCompleteIter++)
	{
		GIAentityNode * entityNode = *entityNodesActiveListComplete;
		if((entityNode->entityName == entityName) && (entityNode->idInstance == idInstance))
		{
			entityAlreadyInActiveListComplete = true;
		}
	}
	cout << "entityInActiveListComplete(string * entityName, long idInstance) = " << entityAlreadyInActiveListComplete << endl;
	return entityAlreadyInActiveListComplete;
}

bool entityInActiveListComplete(GIAentityNode * entity)
{
	bool entityAlreadyInActiveListComplete = false;
	for(vector<GIAentityNode*>::iterator entityNodesActiveListCompleteIter = entityNodesActiveListComplete->begin(); entityNodesActiveListCompleteIter != entityNodesActiveListComplete->end(); entityNodesActiveListCompleteIter++)
	{
		GIAentityNode * entityNode = *entityNodesActiveListComplete;
		if(entityNode == entity)
		{
			entityAlreadyInActiveListComplete = true;
		}
	}
	cout << "entityInActiveListComplete(GIAentityNode * entity) = " << entityAlreadyInActiveListComplete << endl;
	return entityAlreadyInActiveListComplete;
}
*/

void addInstanceEntityNodeToActiveLists(GIAentityNode * entity)
{//NB add reference set entity to active list complete + appropriate list (substance/action/condition) [NB the reference set entity will already be added to concept active list entityNodesActiveListConcepts...] - this function enables references to be written to XML

	/*already available locally...
	long * currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinCompleteList();
	long * currentEntityNodeIDinConceptEntityNodesList = getCurrentEntityNodeIDinConceptEntityNodesList();
	long * currentEntityNodeIDInSubstanceEntityNodesList = getCurrentEntityNodeIDinSubstanceEntityNodesList();
	long * currentEntityNodeIDInActionEntityNodesList = getCurrentEntityNodeIDinActionEntityNodesList();
	long * currentEntityNodeIDInConditionEntityNodesList = getCurrentEntityNodeIDinConditionEntityNodesList();
	*/

	if(saveNetwork)
	{
		if(entity->isConcept)
		{
			//addInstanceEntityNodeToActiveLists() does not support concept entities. NB when this function is executed via linkAdvancedReferencesGIA(), the referenceSource concept entity is already added to the concept active list (ie, see GIAentityNode * referenceSourceConceptEntity = findOrAddConceptEntityNodeByNameSimpleWrapper)
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
		
		entity->idActiveList = currentEntityNodeIDinCompleteList;
		entityNodesActiveListComplete->push_back(entity);
		currentEntityNodeIDinCompleteList++;
		
		if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
		{
			addEntityNodesActiveListCompleteFastIndexDBactive(entity);	//added 2 Nov 2012
		}
	}
	else
	{//this case should never be used when this function is executed via linkAdvancedReferencesGIA() 
		entity->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}
}
#endif



#ifdef GIA_SUPPORT_ALIASES
void mergeEntityNodesAddAlias(GIAentityNode * entityNode, GIAentityNode * entityNodeToMerge)
{
	entityNode->aliasList.push_back(entityNodeToMerge->entityName);

	#ifdef GIA_ALIASES_DEBUG
	cout << "\n" << endl;
	cout << "entityNode->entityName = " << entityNode->entityName << endl;
	cout << "entityNodeToMerge->entityName = " << entityNodeToMerge->entityName << endl;
	#endif

	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entityNodeToMerge->entityVectorConnectionsArray[i].begin(); connectionIter != entityNodeToMerge->entityVectorConnectionsArray[i].end(); )
		{
			bool connectionIterErased = false;
			//connect entityNodeToMerge ambient node to entityNode
			GIAentityNode * entityConnectedToEntityToMerge = (*connectionIter)->entity;

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

			if(entityNode != entityConnectedToEntityToMerge)
			{//added 29 November 2012
				
				//disconnect reference sources from each other, as their connection between each other will be redeclared in current context
				int iInverted = inverseVectorConnectionsArray[i];
				for(vector<GIAentityConnection*>::iterator connectionIter2 = entityConnectedToEntityToMerge->entityVectorConnectionsArray[iInverted].begin(); connectionIter2 != entityConnectedToEntityToMerge->entityVectorConnectionsArray[iInverted].end(); )
				{
					bool connectionIter2Erased = false;
					GIAentityNode * entityConnectedToEntityConnectedToEntityToMerge = (*connectionIter2)->entity;

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

					if(entityNodeToMerge == entityConnectedToEntityConnectedToEntityToMerge)	//OR (entityNodeToMerge == entityConnectedToEntityConnectedToEntityToMerge)?
					{
						#ifndef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
						//the commenting out of the below case is required for advanced referencing (eg concept Tom has associated instance Dog) [NB this means that instances can appear to have more than one entityNodeDefiningThisInstance]
						if((entityConnectedToEntityToMerge->isConcept) && (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE))
						{//restored 29 November 2012, and condition (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE) added
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
						#endif
							//connect entityConnectedToEntityConnectedToEntityToMerge back to entityNode (z)
							#ifdef GIA_ALIASES_DEBUG
							cout << "change entityConnectedToEntityConnectedToEntityToMerge (" << entityConnectedToEntityConnectedToEntityToMerge->entityName << ") to entityNode (" << entityNode->entityName << ") (z)" << endl;
							#endif

							(*connectionIter2)->entity = entityNode;
							#ifdef GIA_USE_DATABASE
							(*connectionIter2)->entityName = entityNode->entityName;	//added 29 November 2012
							(*connectionIter2)->idInstance = entityNode->idInstance;	//added 29 November 2012							
							(*connectionIter2)->modified = true;
							#endif
						#ifndef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
						}
						#endif
					}

					if(!connectionIter2Erased)
					{
						connectionIter2++;
					}
				}

				//connect entityNode to entityConnectedToEntityToMerge (x)
				
				#ifndef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				//the commenting out of the below case is required for advanced referencing (eg concept Tom has associated instance Dog) [NB this means that instances can appear to have more than one entityNodeDefiningThisInstance]
				if(!((entityConnectedToEntityToMerge->isConcept) && (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)))
				{
				#endif
					#ifdef GIA_ALIASES_DEBUG
					cout << "connect entityNode (" << entityNode->entityName << ") to entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (x)" << endl;
					#endif
					#ifdef GIA_USE_ADVANCED_REFERENCING
					bool sameReferenceSet = (*connectionIter)->sameReferenceSet;
					#else
					bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					#endif
					writeVectorConnection(entityNode, entityConnectedToEntityToMerge, i, sameReferenceSet);
				#ifndef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				}
				#endif
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

	//disconnect entityNodeToMerge from entityNode
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); )
		{
			bool connectionIterErased = false;
			GIAentityNode * entityConnectedToEntity = (*connectionIter)->entity;
			#ifdef GIA_ALIASES_DEBUG
			cout << "entityConnectedToEntity->entityName = " << entityConnectedToEntity->entityName << endl;
			cout << "entityNodeToMerge->entityName = " << entityNodeToMerge->entityName << endl;
			#endif			
			if(entityConnectedToEntity == entityNodeToMerge)
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
		entityNode->isWhichOrEquivalentWhatQuery = entityNodeToMerge->isWhichOrEquivalentWhatQuery;
		#ifdef GIA_SUPPORT_ALIASES
		entityNode->isNameQuery = entityNodeToMerge->isNameQuery;
		#endif
	}
	#ifdef GIA_SUPPORT_ALIASES
	entityNode->isName = entityNodeToMerge->isName;
	#endif
	
	if(entityNodeToMerge->isToBeComplimentOfActionTemp)
	{
		entityNode->isToBeComplimentOfActionTemp = true;	//should not be required
	}
	
	disableEntity(entityNodeToMerge);
	
	#ifdef GIA_ALIASES_DEBUG
	cout << "finished: mergeEntityNodesAddAlias" << endl;
	#endif

}
#endif


#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION

GIAgenericDepRelInterpretationParameters::GIAgenericDepRelInterpretationParameters(Sentence * newcurrentSentenceInList, bool newGIAentityNodeArrayFilled[], GIAentityNode * newGIAentityNodeArray[], bool newexecuteOrReassign)
{
	currentSentenceInList = newcurrentSentenceInList;
	GIAentityNodeArrayFilled = newGIAentityNodeArrayFilled;
	GIAentityNodeArray = newGIAentityNodeArray;
	unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts;
	
	executeOrReassign = newexecuteOrReassign;
		
	//for relation1, relation2, relation3, and relation4 [GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS]:
		//for entity1 (eg substanceEntity), entity2 (eg conditionEntity, propertyEntity), and entity3/intermediaryEntity (eg conditionTypeEntity, actionEntity) [3]:
		
		//relations to parse
	numberOfRelations = 1;
	parseDisabledRelation = {false, false, false, false, false};
	parseDisabledRelationDuringLink = {false, false, false, false, false};	//not currently used

		//found values
	relation = {NULL, NULL, NULL, NULL};		
	//relationEntity = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	relationEntityIndex = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
	relationEntityPrepFound = {false, false, false, false, false};
		//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
	//relationEntityOriginal = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	relationEntityIndexOriginal = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
		//for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation():
	relationFinalResult = {false, false, false, false, false};
	//relationEntityFinalResult = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	relationEntityIndexFinalResult = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
	
		//predefined values tests
	useRelationTest = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	//relationTest = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}};	//internal compiler error: Segmentation fault
	relationTestIsNegative = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	useRelationArrayTest = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};	//if !useRelationTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY], then useRelationArrayTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY] (as a relation type test must be applied for each parsed relation)
	relationArrayTest = {{NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}};
	relationArrayTestSize = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
	relationArrayTestIsNegative = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	expectToFindPrepositionTest = {false, false, false, false, false};
	relationTestSpecialCaseOfOrPossType = {false, false, false, false, false};
	relationTestSpecialCaseContinousVerb = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};	//special case to check for continuous verbs
	relationTestSpecialCaseNotDefinite = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	relationTestSpecialCasePOStemp = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	relationArrayTestSpecialCasePOStemp = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	relationTestSpecialCaseIsNotAction = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	relationTestSpecialCaseIsNotToBeComplimentOfAction = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	
		//entity index match tests
	useRelationIndexTest = {{false, false, false}, {false, false}, {false, false, false}, {false, false, false}};
	relationIndexTestRelationID = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
	relationIndexTestEntityID = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
		//NB for relationType tests use relationType as indicies are not available
	relationIndexTestIsNegative = {{false, false, false}, {false, false}, {false, false, false}, {false, false, false}};		
				
		//for redistribution
	useRedistributeRelationEntityIndexReassignment = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}}; 			//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - for reassigning relation entities 
	redistributeRelationEntityIndexReassignmentRelationID = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};						//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relation1, relation2, relation3, or relation4 - for reassigning relation entities 
	redistributeRelationEntityIndexReassignmentRelationEntityID = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};						//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for reassigning relation entities	
	redistributeRelationEntityIndexReassignmentUseOriginalValues = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}}; 	//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 
	useRedistributeRelationEntityReassignment = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}}; 			//for entity1, entity2, and entity3 - for renaming relation entities 
	//redistributeRelationEntityReassignment = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault		//for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for renaming relation entities	
		//special cases for redistribution
	useRedistributeSpecialCaseAuxillaryIndicatesDifferentReferenceSetCheck = {false, false, false, false, false};
	useRedistributeSpecialCaseRelationEntityReassignmentConcatonate = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID = {{{-1, -1} , {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}};	    
	redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID = {{{-1, -1} , {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}}};
	useRedistributeSpecialCaseIsNameQueryAssignment = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	useRedistributeSpecialCaseIsNameAssignment = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	useRedistributeSpecialCaseNegativeAssignment = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	useRedistributeSpecialCaseQualityAssignment = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	useRedistributeSpecialCaseDisableInstanceAndConcept = {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}};
	
		//for execution
	#ifdef GIA_USE_ADVANCED_REFERENCING
	defaultSameSetRelationID = -1; 
	defaultSameSetReferenceValue = -1;
	#endif		
	functionEntityRelationID = {REL1, REL1, REL1, REL1};				//these dummy values have to be set always to prevent crash //for entity1, entity2, and entity3 - relation1, relation2, relation3, or relation4
	functionEntityRelationEntityID = {FUNC_ENT1, FUNC_ENT1, FUNC_ENT1, FUNC_ENT1};	//these dummy values have to be set always to prevent crash //for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex	
	functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_undefined;
		//special cases
	mustGenerateConditionTypeName = false;
	conditionTypeEntityDefaultName = "";
	conditionTypeEntityDefaultIndex = -1;
		
	disableEntity = {{false, false}, {false, false}, {false, false}, {false, false}}; 	//for entity1 and entity2 only
	disableEntityUseOriginalValues = {{false, false}, {false, false}, {false, false}, {false, false}}; 	//for disabling an entity based on its original index
	disableRelation = {false, false, false, false, false};
	disableRelationDuringLink = {false, false, false, false, false};
		
}
GIAgenericDepRelInterpretationParameters::~GIAgenericDepRelInterpretationParameters(void)
{
}

bool genericDependecyRelationInterpretation(GIAgenericDepRelInterpretationParameters * param, int currentRelationID)
{
	//cout << "START genericDependecyRelationInterpretation: " << currentRelationID << endl;
	bool result = false;

	Relation * currentRelationInList = param->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		bool relationPreviouslyUsed = false;
		for(int i=0; i<currentRelationID; i++)
		{
			if(currentRelationInList == param->relation[i])
			{
				relationPreviouslyUsed = true;
			}
		}
		/*info:
		!(param->relation[currentRelationID]->disabled) is to prevent parsing of disabled relations (unless param->parseDisabledRelation[currentRelationID] has been explicitly set)
		!relationPreviouslyUsed is to prevent reusing a relation
		*/
		if((param->parseDisabledRelation[currentRelationID] || !(currentRelationInList->disabled)) && (param->parseDisabledRelationDuringLink[currentRelationID] || !(currentRelationInList->disabledDuringLink)) && !relationPreviouslyUsed)
		{			
			param->relation[currentRelationID] = currentRelationInList;
			
			//predefined values tests
			bool passedRelation = true;
			param->relationEntity[currentRelationID][REL_ENT1] = param->relation[currentRelationID]->relationGovernor;
			param->relationEntity[currentRelationID][REL_ENT2] = param->relation[currentRelationID]->relationDependent;
			param->relationEntity[currentRelationID][REL_ENT3] = param->relation[currentRelationID]->relationType;
			param->relationEntity[currentRelationID][REL_ENT3] = convertPrepositionToRelex(&(param->relationEntity[currentRelationID][REL_ENT3]), &(param->relationEntityPrepFound[currentRelationID]));	//convert stanford prep_x to relex x 
			param->relationEntityIndex[currentRelationID][REL_ENT1] = param->relation[currentRelationID]->relationGovernorIndex;
			param->relationEntityIndex[currentRelationID][REL_ENT2] = param->relation[currentRelationID]->relationDependentIndex;
				//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
			param->relationEntityOriginal[currentRelationID][REL_ENT1] = param->relationEntity[currentRelationID][REL_ENT1];
			param->relationEntityOriginal[currentRelationID][REL_ENT2] = param->relationEntity[currentRelationID][REL_ENT2];
			param->relationEntityOriginal[currentRelationID][REL_ENT3] = param->relationEntity[currentRelationID][REL_ENT3];
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT1] = param->relationEntityIndex[currentRelationID][REL_ENT1];
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT2] = param->relationEntityIndex[currentRelationID][REL_ENT2];		
			if(param->expectToFindPrepositionTest[currentRelationID])
			{
				if(!(param->relationEntityPrepFound[currentRelationID]))
				{
					passedRelation = false;
				}
			}
			if(param->relationTestSpecialCaseOfOrPossType[currentRelationID])
			{
				if(passedRelation)
				{
					passedRelation = false;				
					if(param->relationEntity[currentRelationID][REL_ENT3] == RELATION_TYPE_PREPOSITION_OF)
					{
						passedRelation = true;
					}	
					else if(param->relationEntity[currentRelationID][REL_ENT3] == RELATION_TYPE_POSSESSIVE)
					{
						passedRelation = true;
					}
				}			
			}			
			for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
			{
				if(param->relationTestSpecialCaseIsNotAction[currentRelationID][relationEntityID])
				{
					if(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]]->isAction)
					{
						passedRelation = false;	
					}				
				}
				if(param->relationTestSpecialCaseIsNotToBeComplimentOfAction[currentRelationID][relationEntityID])
				{
					if(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]]->isToBeComplimentOfActionTemp)
					{
						passedRelation = false;	
					}				
				}			
				if(param->relationTestSpecialCaseNotDefinite[currentRelationID][relationEntityID])
				{
					if(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]]->grammaticalDefiniteTemp)
					{
						passedRelation = false;	
					}				
				}			
				if(param->relationTestSpecialCaseContinousVerb[currentRelationID][relationEntityID])
				{
					if(!((param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]]->wordNetPOS == GRAMMATICAL_WORD_TYPE_VERB) && 
					     (param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]]->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)))
					{
						passedRelation = false;	
					}				
				}			
				if(param->useRelationTest[currentRelationID][relationEntityID])
				{
					if(passedRelation)
					{
						passedRelation = false;
						if(param->relationTestSpecialCasePOStemp[currentRelationID][relationEntityID])
						{//special case
							if(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]]->stanfordPOStemp == param->relationTest[currentRelationID][relationEntityID])
							{
								passedRelation = true;
							}
						}
						else
						{						
							if(param->relationTestIsNegative[currentRelationID][relationEntityID])
							{//negative
								if(param->relationEntity[currentRelationID][relationEntityID] != param->relationTest[currentRelationID][relationEntityID])
								{
									passedRelation = true;
								}						
							}
							else
							{//normal
								if(param->relationEntity[currentRelationID][relationEntityID] == param->relationTest[currentRelationID][relationEntityID])
								{
									passedRelation = true;
								}
							}
						}
					}
				}
				if(param->useRelationArrayTest[currentRelationID][relationEntityID])
				{
					if(passedRelation)
					{				
						passedRelation = false;
						bool foundAnArrayPass = false;
						//int relationArrayTestSize = sizeof((param->relationArrayTest[currentRelationID][relationEntityID]))/sizeof((param->relationArrayTest[currentRelationID][relationEntityID])[0]);
						//cout << "relationArrayTestSize = " << relationArrayTestSize << endl;
						for(int j=0; j<param->relationArrayTestSize[currentRelationID][relationEntityID]; j++)
						{
							if(param->relationArrayTestSpecialCasePOStemp[currentRelationID][relationEntityID])
							{//special case
								if(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]]->stanfordPOStemp == (param->relationArrayTest[currentRelationID][relationEntityID])[j])
								{
									//cout << "(param->relationArrayTest[currentRelationID][relationEntityID])[j] = " << (param->relationArrayTest[currentRelationID][relationEntityID])[j] << endl;
									passedRelation = true;
									foundAnArrayPass = true;
								}
							}
							else
							{
								if(param->relationEntity[currentRelationID][relationEntityID] == (param->relationArrayTest[currentRelationID][relationEntityID])[j])
								{
									passedRelation = true;
									foundAnArrayPass = true;
								}
							}
						}
						if(param->relationArrayTestIsNegative[currentRelationID][relationEntityID])
						{
							if(!foundAnArrayPass)
							{
								passedRelation = true;
							}
							else
							{
								passedRelation = false;
							}
						}
					}				
				}
			}
			if(passedRelation)
			{
				//cout << "param->numberOfRelations = " << param->numberOfRelations << endl;
				//cout << "currentRelationID = " << currentRelationID << endl;
				if(currentRelationID < (param->numberOfRelations - 1))
				{
					#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
					cout << currentRelationID << ": " << param->relation[currentRelationID]->relationType << "(" << param->relation[currentRelationID]->relationGovernor << ", " << param->relation[currentRelationID]->relationDependent << ")" << endl;
					#endif
					GIAgenericDepRelInterpretationParameters paramTemp = *param;	//this shouldnt be required anymore with relationFinalResult/relationEntityFinalResult/relationEntityIndexFinalResult... 	//only record parameters (eg relationEntity/relationEntityIndex) if successfully recused - this is required if additional commands are required to be executed based on the successful (result==true) recursion of genericDependecyRelationInterpretation (e.g. in GIAtranslatorRedistributeStanfordRelations.cpp)  
					if(genericDependecyRelationInterpretation(&paramTemp, (currentRelationID+1)))
					{
						result = true;
						*param = paramTemp;	//this shouldnt be required anymore with relationFinalResult/relationEntityFinalResult/relationEntityIndexFinalResult... 	//only record parameters (eg relationEntity/relationEntityIndex) if successfully recused - this is required if additional commands are required to be executed based on the successful (result==true) recursion of genericDependecyRelationInterpretation (e.g. in GIAtranslatorRedistributeStanfordRelations.cpp)  
					}
				}
				else
				{
					#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
					cout << currentRelationID << ": " << param->relation[currentRelationID]->relationType << "(" << param->relation[currentRelationID]->relationGovernor << ", " << param->relation[currentRelationID]->relationDependent << ")" << endl;				
					cout << "passedRelations = " << passedRelation << endl;
					#endif
					bool passedEntityMatchTests = true;
					//entity index match tests

					int minRelationToTest = 0;					
					int maxRelationToTest = param->numberOfRelations;
					bool specialCaseAuxillaryIndicatesDifferentReferenceSetCheck = false;
					if(currentRelationID == param->numberOfRelations)
					{
						specialCaseAuxillaryIndicatesDifferentReferenceSetCheck = true;
						minRelationToTest = param->numberOfRelations;
						maxRelationToTest = param->numberOfRelations+1;
					}	
					for(int relationID=minRelationToTest; relationID<maxRelationToTest; relationID++)
					{
						for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)	//changed from GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION 25 July 2013
						{
							if(param->useRelationIndexTest[relationID][relationEntityID])
							{
								//cout << "useRelationIndexTest: " << relationID << ", " << relationEntityID << endl;				
								
								if(passedEntityMatchTests)
								{//NB for relationType tests use relationType as indicies are not available							
									passedEntityMatchTests = false;
									if(param->relationIndexTestIsNegative[relationID][relationEntityID])
									{
										if(relationEntityID == REL_ENT3)
										{
											if(param->relationEntity[relationID][relationEntityID] != param->relationEntity[param->relationIndexTestRelationID[relationID][relationEntityID]][param->relationIndexTestEntityID[relationID][relationEntityID]])
											{
												passedEntityMatchTests = true;
											}
										}
										else
										{
											if(param->relationEntityIndex[relationID][relationEntityID] != param->relationEntityIndex[param->relationIndexTestRelationID[relationID][relationEntityID]][param->relationIndexTestEntityID[relationID][relationEntityID]])
											{
												passedEntityMatchTests = true;
											}
										}
									}
									else
									{
										if(relationEntityID == REL_ENT3)
										{
											if(param->relationEntity[relationID][relationEntityID] == param->relationEntity[param->relationIndexTestRelationID[relationID][relationEntityID]][param->relationIndexTestEntityID[relationID][relationEntityID]])
											{
												passedEntityMatchTests = true;
											}
										}
										else
										{
											if(param->relationEntityIndex[relationID][relationEntityID] == param->relationEntityIndex[param->relationIndexTestRelationID[relationID][relationEntityID]][param->relationIndexTestEntityID[relationID][relationEntityID]])
											{
												passedEntityMatchTests = true;
											}
										}
									
									}
								}
							}
						}
					}
					if(passedEntityMatchTests)
					{
						#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
						cout << "passedEntityMatchTests" << endl;
						#endif
						result = true;
						
						//record final values for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation:
						for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
						{
							param->relationFinalResult[relationID] = param->relation[relationID];
							for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION; relationEntityID++)
							{							
								param->relationEntityFinalResult[relationID][relationEntityID] = param->relationEntity[relationID][relationEntityID];
								param->relationEntityIndexFinalResult[relationID][relationEntityID] = param->relationEntityIndex[relationID][relationEntityID];
							}
						}
						
						if(!specialCaseAuxillaryIndicatesDifferentReferenceSetCheck)
						{
							if(param->executeOrReassign)
							{//execute
								#ifdef GIA_USE_ADVANCED_REFERENCING
								bool sameReferenceSet;
								if(param->defaultSameSetRelationID == -1)
								{
									sameReferenceSet = param->defaultSameSetReferenceValue;
								}
								else
								{
									sameReferenceSet = determineSameReferenceSetValue(param->defaultSameSetReferenceValue, param->relation[param->defaultSameSetRelationID]);
								}
								#else
								bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif

								int functionEntityIndex1 = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT1]][param->functionEntityRelationEntityID[FUNC_ENT1]]; 
								int functionEntityIndex2 = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT2]][param->functionEntityRelationEntityID[FUNC_ENT2]];
								int functionEntityIndex3 = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];						
								int functionEntityIndex4special = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT4_SPECIAL]][param->functionEntityRelationEntityID[FUNC_ENT4_SPECIAL]]; 
																
								//if(param->functionToExecuteUponFind is a condition....
								if((param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity) ||
								  (param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectBeingDefinitionConditionToEntity) || 
								  (param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectHavingPropertyConditionToEntity))
								{//if a condition is required to be created
									string conditionTypeEntityName = "";
									bool mustLookupOrGenerateConditionTypeConceptEntity = false;
									if(param->mustGenerateConditionTypeName)
									{
										conditionTypeEntityName = param->conditionTypeEntityDefaultName;
										functionEntityIndex3 = param->conditionTypeEntityDefaultIndex;	
										mustLookupOrGenerateConditionTypeConceptEntity = true;
									}
									else
									{
										if(param->conditionTypeEntityDefaultName != "")
										{//special case where conditionType is required to be changed from the preposition name (not often used)
											conditionTypeEntityName = param->conditionTypeEntityDefaultName;
										}	
										else
										{
											conditionTypeEntityName = param->relationEntity[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];
										}
										if(param->functionEntityRelationEntityID[FUNC_ENT3] == REL_ENT3)	//conditionType is being defined as a relation's relationType and has no obvious concept entity. However a relationType is often a preposition - if so it will already have a feature index (determined by determineFeatureIndexOfPreposition()) [of which there might even be a corresponding entity name/index defined depending upon how the dependency relations were generated by the NLP: see findOrAddEntityNodeByNameSimpleWrapperCondition() below for information on how this case is handled]
										{
											functionEntityIndex3 = -1;
											bool prepositionFeatureFound = determineFeatureIndexOfPreposition(param->currentSentenceInList, &conditionTypeEntityName, &functionEntityIndex3);
											if(!prepositionFeatureFound)
											{
												functionEntityIndex3 = param->conditionTypeEntityDefaultIndex;
											}
											mustLookupOrGenerateConditionTypeConceptEntity = true;
										}
										else
										{
											//functionEntityIndex3 will already be set correctly	[param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];]
										}
									}
									if(mustLookupOrGenerateConditionTypeConceptEntity)
									{
										//NB if concept type entity name has already been defined (GIAentityNodeArrayFilled[functionEntityIndex3]), then findOrAddEntityNodeByNameSimpleWrapperCondition will use it instead
										bool entityAlreadyExistant = false;
										param->GIAentityNodeArray[functionEntityIndex3] = findOrAddEntityNodeByNameSimpleWrapperCondition(param->GIAentityNodeArrayFilled, param->GIAentityNodeArray, functionEntityIndex3, &conditionTypeEntityName, &entityAlreadyExistant, param->entityNodesActiveListConcepts);
										//GIAentityNode * conditionTypeConceptEntity = above									
									}
								}

								if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition)
								{
									param->GIAentityNodeArray[functionEntityIndex1] = addSubstanceToSubstanceDefinition(param->GIAentityNodeArray[functionEntityIndex1]);
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addActionToActionDefinitionDefineSubstances)
								{
									param->GIAentityNodeArray[functionEntityIndex1] = addActionToActionDefinitionDefineSubstances(param->GIAentityNodeArray[functionEntityIndex1]);
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty)
								{
									param->GIAentityNodeArray[functionEntityIndex2] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
								}							
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity)
								{
									param->GIAentityNodeArray[functionEntityIndex2] =  addOrConnectPropertyToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
								}							
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToEntity)
								{
									#ifdef GIA_USE_ADVANCED_REFERENCING
									bool subjectFound = false;
									for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
									{
										if(param->relation[REL1]->relationType == relationTypeSubjectNameArray[i])
										{
											subjectFound = true;
										}
									}									
									if(!subjectFound)
									{
										cout << "error: addOrConnectActionToEntity && (relation[REL1]->relationType != ~RELATION_TYPE_SUBJECT)" << endl;
									}
									bool objectFound = false;
									for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
									{
										if(param->relation[REL2]->relationType == relationTypeObjectNameArray[i])
										{
											objectFound = true;
										}
									}									
									if(!objectFound)
									{
										cout << "error: addOrConnectActionToEntity && (relation[REL1]->relationType != ~RELATION_TYPE_OBJECT)" << endl;
									}																	
									bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, param->relation[REL1]);
									bool sameReferenceSetObject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, param->relation[REL2]);
									#else
									bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									bool sameReferenceSetObject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									#endif							
									param->GIAentityNodeArray[functionEntityIndex3] =  addOrConnectActionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], sameReferenceSetSubject, sameReferenceSetObject);
								}							
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToSubject)
								{
									param->GIAentityNodeArray[functionEntityIndex2] = addOrConnectActionToSubject(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
								}							
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToObject)
								{
									param->GIAentityNodeArray[functionEntityIndex2] = addOrConnectActionToObject(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
								}							
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity)
								{
									//cout << "param->GIAentityNodeArray[functionEntityIndex1] = " << param->GIAentityNodeArray[functionEntityIndex1]->entityName << endl; 
									//cout << "param->GIAentityNodeArray[functionEntityIndex2] = " << param->GIAentityNodeArray[functionEntityIndex2]->entityName << endl;
									//cout << "param->GIAentityNodeArray[functionEntityIndex3] = " << param->GIAentityNodeArray[functionEntityIndex3]->entityName << endl;
									//cout << "sameReferenceSet = " << sameReferenceSet << endl;
									param->GIAentityNodeArray[functionEntityIndex3] = addOrConnectConditionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], sameReferenceSet);
								}							
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectBeingDefinitionConditionToEntity)
								{
									bool negative = param->GIAentityNodeArray[functionEntityIndex4special]->negative;
									//cout << "negative = " << negative << endl;
									param->GIAentityNodeArray[functionEntityIndex3] = addOrConnectBeingDefinitionConditionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], negative, sameReferenceSet);
								}						
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectHavingPropertyConditionToEntity)
								{	
									bool negative = param->GIAentityNodeArray[functionEntityIndex4special]->negative;						
									//cout << "negative = " << negative << endl;
									param->GIAentityNodeArray[functionEntityIndex3] = addOrConnectHavingPropertyConditionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], negative, sameReferenceSet);
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntity)
								{
									addDefinitionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
								}																												
							}
							else
							{//reassign entities

								for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
								{
									for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
									{							
										if(param->useRedistributeRelationEntityIndexReassignment[relationID][relationEntityID])
										{
											//cout << "relationID = " << relationID << ", relationEntityID = " << relationEntityID << endl;
											//reassign param index + entity values
											if(param->redistributeRelationEntityIndexReassignmentUseOriginalValues[relationID][relationEntityID])
											{
												param->relationEntityIndex[relationID][relationEntityID] = param->relationEntityIndexOriginal[param->redistributeRelationEntityIndexReassignmentRelationID[relationID][relationEntityID]][param->redistributeRelationEntityIndexReassignmentRelationEntityID[relationID][relationEntityID]];
												param->relationEntity[relationID][relationEntityID] = param->relationEntityOriginal[param->redistributeRelationEntityIndexReassignmentRelationID[relationID][relationEntityID]][param->redistributeRelationEntityIndexReassignmentRelationEntityID[relationID][relationEntityID]];											
											}
											else
											{
												param->relationEntityIndex[relationID][relationEntityID] = param->relationEntityIndex[param->redistributeRelationEntityIndexReassignmentRelationID[relationID][relationEntityID]][param->redistributeRelationEntityIndexReassignmentRelationEntityID[relationID][relationEntityID]];
												param->relationEntity[relationID][relationEntityID] = param->relationEntity[param->redistributeRelationEntityIndexReassignmentRelationID[relationID][relationEntityID]][param->redistributeRelationEntityIndexReassignmentRelationEntityID[relationID][relationEntityID]];
												//cout << "param->relationEntityIndex[relationID][relationEntityID] = " << param->relationEntityIndex[relationID][relationEntityID] << endl;
												//cout << "param->relationEntity[relationID][relationEntityID] = " << param->relationEntity[relationID][relationEntityID] << endl;										
											}

											if(relationEntityID == REL_ENT1)
											{
												param->relation[relationID]->relationGovernorIndex = param->relationEntityIndex[relationID][relationEntityID];
												param->relation[relationID]->relationGovernor = param->relationEntity[relationID][relationEntityID];								
											}
											else if(relationEntityID == REL_ENT2)
											{
												param->relation[relationID]->relationDependentIndex = param->relationEntityIndex[relationID][relationEntityID];
												param->relation[relationID]->relationDependent = param->relationEntity[relationID][relationEntityID];											
											}
											else if(relationEntityID == REL_ENT3)
											{//governor/dependent can become preposition
												string newPrepositionName = ""; 
												newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + param->relationEntity[relationID][relationEntityID];
												param->relation[relationID]->relationType = newPrepositionName;
												//cout << "param->relation[relationID]->relationType = " << param->relation[relationID]->relationType << endl;
											}																			
										}									
									}			
								}									
								for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
								{							
									for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
									{							
										if(param->useRedistributeRelationEntityReassignment[relationID][relationEntityID])
										{
											if(relationEntityID == REL_ENT1)
											{
												param->relation[relationID]->relationGovernor = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												/*
												//Alternate implementation;
												if(useRelationEntityIndexUpdated[relationID][relationEntityID])
												{
													param->GIAentityNodeArray[param->useRelationEntityIndexUpdated[relationID][relationEntityID]] = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												}
												*/
											}
											else if(relationEntityID == REL_ENT2)
											{
												param->relation[relationID]->relationDependent = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->redistributeRelationEntityReassignment[relationID][relationEntityID];									
												/*
												//Alternate implementation;
												if(useRelationEntityIndexUpdated[relationID][relationEntityID])
												{
													param->GIAentityNodeArray[param->useRelationEntityIndexUpdated[relationID][relationEntityID]] = param->redistributeRelationEntityReassignment[relationID][relationEntityID];					
												}
												*/										
											}
											else if(relationEntityID == REL_ENT3)
											{
												param->relation[relationID]->relationType = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
											}									
										}
									}	
								}
								//special case reassignments
								for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
								{
									if(param->useRedistributeSpecialCaseAuxillaryIndicatesDifferentReferenceSetCheck[relationID])
									{
										#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
										cout << currentRelationID << ": [SpecialCaseAuxCheck]: " << param->relation[currentRelationID]->relationType << "(" << param->relation[currentRelationID]->relationGovernor << ", " << param->relation[currentRelationID]->relationDependent << ")" << endl;
										#endif
										bool auxillaryIndicatesDifferentReferenceSet = true;
										GIAgenericDepRelInterpretationParameters paramTemp = *param;
										if(genericDependecyRelationInterpretation(&paramTemp, (currentRelationID+1)))
										{
											auxillaryIndicatesDifferentReferenceSet = false;
										}
										#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
										cout << "auxillaryIndicatesDifferentReferenceSet = " << auxillaryIndicatesDifferentReferenceSet << endl;									
										#endif
										param->relation[relationID]->auxillaryIndicatesDifferentReferenceSet = auxillaryIndicatesDifferentReferenceSet;									
									}

									for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
									{	
										if(param->useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[relationID][relationEntityID])
										{

											string concatonatedEntityNamePart1 = param->relationEntity[param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[relationID][relationEntityID][0]][param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[relationID][relationEntityID][0]];
											string concatonatedEntityNamePart2 = param->relationEntity[param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[relationID][relationEntityID][1]][param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[relationID][relationEntityID][1]];
											string concatonatedEntityName = concatonatedEntityNamePart1 + STANFORD_PARSER_PREPOSITION_DELIMITER + concatonatedEntityNamePart2;
											//cout << "concatonatedEntityName = " << concatonatedEntityName << endl;
											//cout << "concatonatedEntityNamePart1 = " << concatonatedEntityNamePart1 << endl;
											//cout << "concatonatedEntityNamePart2 = " << concatonatedEntityNamePart2 << endl;

											if(relationEntityID == REL_ENT1)
											{
												param->relation[relationID]->relationGovernor = concatonatedEntityName;		//probably not required
												param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName = concatonatedEntityName;							
											}
											else if(relationEntityID == REL_ENT2)
											{
												param->relation[relationID]->relationDependent = concatonatedEntityName;		//probably not required
												param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName = concatonatedEntityName;									
											}
											else if(relationEntityID == REL_ENT3)
											{//governor/dependent can become preposition
												string newPrepositionName = ""; 
												newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + concatonatedEntityName;
												param->relation[relationID]->relationType = newPrepositionName;
												//cout << "param->relation[relationID]->relationType = " << param->relation[relationID]->relationType << endl;
											}																			
										}
										if(param->useRedistributeSpecialCaseIsNameQueryAssignment[relationID][relationEntityID])
										{
											param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->isNameQuery = true;							
										}
										if(param->useRedistributeSpecialCaseIsNameAssignment[relationID][relationEntityID])
										{
											param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->isName = true;
										}
										if(param->useRedistributeSpecialCaseNegativeAssignment[relationID][relationEntityID])
										{
											//cout << "neg: " << param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName << endl;
											param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->negative = true;
										}	
										if(param->useRedistributeSpecialCaseQualityAssignment[relationID][relationEntityID])
										{
											param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->isSubstanceQuality = true;
										}																												
									}
								}														
							}

							//for cleanup
							for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
							{
								for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION; relationEntityID++)
								{
									if(param->disableEntity[relationID][relationEntityID])
									{
										GIAentityNode * oldRedundantEntity;
										if(param->disableEntityUseOriginalValues[relationID][relationEntityID])
										{
											oldRedundantEntity = param->GIAentityNodeArray[param->relationEntityIndexOriginal[relationID][relationEntityID]];
										}
										else
										{
											oldRedundantEntity = param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]];
										}
										if((param->executeOrReassign) || param->useRedistributeSpecialCaseDisableInstanceAndConcept[relationID][relationEntityID])
										{
											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(oldRedundantEntity);
										}
										else
										{
											disableEntity(oldRedundantEntity);
										}
									}

								}
								if(param->disableRelation[relationID])
								{
									param->relation[relationID]->disabled =  true;
								}
								if(param->disableRelationDuringLink[relationID])
								{
									param->relation[relationID]->disabledDuringLink =  true;
								}								
							}
						}						
					}					
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
	//cout << "END genericDependecyRelationInterpretation: " << currentRelationID << endl;	
	return result;
}

#endif

bool determineFeatureIndexOfPreposition(Sentence * currentSentenceInList, string * prepositionName, int * indexOfPreposition)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "*prepositionName = " << *prepositionName << endl;
	#endif
	
	bool prepositionFeatureFound = false;
	Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		string singleWordPreposition = *prepositionName;
		int indexOfPrepositionDelimiter = prepositionName->rfind(STANFORD_PARSER_PREPOSITION_DELIMITER);	//find last occurance
		if(indexOfPrepositionDelimiter != string::npos)
		{
			int lengthOfSingleWordPreposition = prepositionName->length() - indexOfPrepositionDelimiter - 1;
			singleWordPreposition = prepositionName->substr(indexOfPrepositionDelimiter+1, lengthOfSingleWordPreposition);
			if(indexOfPrepositionDelimiter == prepositionName->length()-1)
			{
				cout << "determineFeatureIndexOfPreposition: illegal multiword preposition; (indexOfPrepositionDelimiter == prepositionName->length()-1)" << endl;
				cout << "prepositionName = " << *prepositionName << endl;
				exit(0);
			}
		}

		/*OLD: find first occurance; not possible as the first word in multiword prepositions has often already been assigned an entity array index, eg He rode the carriage that is near to the horse. nsubj(near-7, carriage-4) / prep_to(near-7, horse-10) -> prep_near_to(carriage-4, horse-10)
		string singleWordPreposition = *prepositionName;
		int indexOfPrepositionDelimiter = prepositionName->find(STANFORD_PARSER_PREPOSITION_DELIMITER);
		if(indexOfPrepositionDelimiter != string::npos)
		{
			int lengthOfSingleWordPreposition = indexOfPrepositionDelimiter;
			singleWordPreposition = prepositionName->substr(0, lengthOfSingleWordPreposition);
			if(indexOfPrepositionDelimiter == 0)
			{
				cout << "determineFeatureIndexOfPreposition: illegal multiword preposition; indexOfPrepositionDelimiter == 0" << endl;
				cout << "prepositionName = " << *prepositionName << endl;
				exit(0);
			}
		}
		*/

		if(currentFeatureInList->lemma == singleWordPreposition)
		{
			prepositionFeatureFound = true;
			*indexOfPreposition = currentFeatureInList->entityIndex;
		}
		currentFeatureInList = currentFeatureInList->next;
	}
	return prepositionFeatureFound;
}
