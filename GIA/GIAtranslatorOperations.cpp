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
 * File Name: GIAtranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2o7d 23-October-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorOperations.h"
#include "GIAdatabase.h"
#ifdef GIA_SUPPORT_NLC_INTEGRATION
#include "NLCpreprocessorSentenceClass.h"
#endif
#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
#include "GIAlrp.h"
#endif
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
#include "GIAsemanticParserOperations.h"
#endif

#ifdef GIA_SUPPORT_NLC_INTEGRATION
NLCsentence* firstNLCsentenceInListLocal;
#endif

static long currentEntityNodeIDinCompleteList;				//For GIA XML generation only
static long currentEntityNodeIDinNetworkIndexEntityNodesList;		//For GIA XML generation only

static vector<GIAentityNode*>* entityNodesActiveListComplete;		//For GIA XML generation only
//static vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes;

unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndex;

#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
static bool linkPreestablishedReferencesGIA;
#endif
static bool saveNetwork; 						//For GIA XML generation only / GIA Advanced referencing only
static long currentEntityNodeIDInSentenceCompleteList;			//For GIA Advanced referencing only
static long currentEntityNodeIDInSentenceNetworkIndexEntityNodesList;	//For GIA Advanced referencing only

static bool foundComparisonVariable;
static GIAentityNode* comparisonVariableNode;

#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
static int currentSentenceIndex;
#endif





void initialiseGIATranslatorForTexualContextOperations()
{
	saveNetwork = true;

	currentEntityNodeIDinCompleteList = 0;
	currentEntityNodeIDinNetworkIndexEntityNodesList = 0;

	/*
	//initialise timeConditionNodesActiveList;		[should be moved elsewhere]
	long firstTimeInNetwork = -14*(10^9)*SECONDS_IN_YEAR;
	string firstTimeNameInNetwork = "beginning";
	GIAtimeConditionNode* firstTimeNodeInNetwork = new GIAtimeConditionNode();
	firstTimeNodeInNetwork->conditionName = firstTimeNameInNetwork;
	firstTimeNodeInNetwork->totalTimeInSeconds = firstTimeInNetwork;
	timeConditionNodesActiveList->push_back(firstTimeNodeInNetwork);
	timeConditionNumbersActiveList->push_back(firstTimeInNetwork);
	*/

}

#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
//this is no longer required with redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition, etc..
bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(GIArelation* currentRelationInList, GIAentityNode* GIAentityNodeArray[], int relationGovernorIndex, int NLPdependencyRelationsType)
{
	bool result = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES_ADVANCED
		if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD) && (GIAentityNodeArray[relationGovernorIndex]->entityName == RELATION_ENTITY_BE))
		{//added condition Fri 27 Jan - remove 'be' node artefacts
			disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[relationGovernorIndex]);
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
bool isAdjectiveNotConnectedToObjectOrSubject(GIAsentence* currentSentenceInList, GIArelation* currentRelationInList, int NLPdependencyRelationsType)
{
	bool passed2 = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
		{
			GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
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
			passed2 = true;
		}
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	}
	#endif


	return passed2;
}
#endif


GIAentityNode* addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet)
{
	if(thingEntity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
	{
	      return connectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);
	}
	else
	{
	      return addOrConnectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);
	}
}

GIAentityNode* connectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet)
{
	return connectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet, NULL);
}

//this has been created based upon addOrConnectPropertyToEntity
GIAentityNode* connectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet, GIAentityNode* auxHaveEntity)
{
	/*//disabled 2o6a due to *
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	#ifdef GIA_TRANSLATOR_DO_NOT_CREATE_CONCEPT_PROPERTIES_FOR_NON_CONCEPT_PARENTS
	if(!(thingEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
	{
		propertyEntity->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//CHECKTHIS: will this ever be a quality?*
	}
	#endif
	#endif
	*/

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(propertyEntity->disabled))
	{
	if(!(thingEntity->disabled))
	{
	#endif

		thingEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		GIAentityConnection* connection1 = writeVectorConnection(thingEntity, propertyEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);
		GIAentityConnection* connection2 = writeVectorConnection(propertyEntity, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
		#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO	
		if(auxHaveEntity != NULL)
		{
			connection1->negative = auxHaveEntity->negative;
			connection2->negative = auxHaveEntity->negative;
			copy(auxHaveEntity->grammaticalTenseModifierArrayTemp, auxHaveEntity->grammaticalTenseModifierArrayTemp+GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES, connection1->grammaticalTenseModifierArrayTemp);
			copy(auxHaveEntity->grammaticalTenseModifierArrayTemp, auxHaveEntity->grammaticalTenseModifierArrayTemp+GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES, connection1->grammaticalTenseModifierArrayTemp);
			connection1->grammaticalTenseTemp = auxHaveEntity->grammaticalTenseTemp;
			connection2->grammaticalTenseTemp = auxHaveEntity->grammaticalTenseTemp;
		}
		#endif
		
		#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
		if(propertyEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
		{
			if(!(thingEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
			{
				//cout << "addOrConnectPropertyToEntity() warning: property was declared concept while parent was not declared concept" << endl;
				propertyEntity->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//CHECKTHIS: will this ever be a quality?
			}
		}
		#endif

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif

	return propertyEntity;	//just return original propertyEntity (for compatibility)
}

GIAentityNode* addOrConnectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet)
{
	return addOrConnectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet, NULL);
}

//changed some instances of addOrConnectPropertyToEntity to addPropertyToEntity
GIAentityNode* addOrConnectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet, GIAentityNode* auxHaveEntity)
{
	/*//disabled 2o6a due to *
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	#ifdef GIA_TRANSLATOR_DO_NOT_CREATE_CONCEPT_PROPERTIES_FOR_NON_CONCEPT_PARENTS
	if(!(thingEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
	{
		propertyEntity->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//CHECKTHIS: will this ever be a quality?
	}
	#endif
	#endif
	*/

	GIAentityNode* propertyEntitySubstance = propertyEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(propertyEntity->disabled))
	{
	if(!(thingEntity->disabled))
	{
	#endif
		if(!(propertyEntity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX))
		{
			propertyEntitySubstance = propertyEntity;
		}
		else
		{
			propertyEntitySubstance = addInstance(propertyEntity, GIA_ENTITY_TYPE_TYPE_SUBSTANCE);
		}

		GIAentityConnection* connection1 = writeVectorConnection(thingEntity, propertyEntitySubstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);
		GIAentityConnection* connection2 = writeVectorConnection(propertyEntitySubstance, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
		#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO		
		if(auxHaveEntity != NULL)
		{
			if(auxHaveEntity->negative)
			{
			connection1->negative = auxHaveEntity->negative;
			connection2->negative = auxHaveEntity->negative;
			copy(auxHaveEntity->grammaticalTenseModifierArrayTemp, auxHaveEntity->grammaticalTenseModifierArrayTemp+GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES, connection1->grammaticalTenseModifierArrayTemp);
			copy(auxHaveEntity->grammaticalTenseModifierArrayTemp, auxHaveEntity->grammaticalTenseModifierArrayTemp+GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES, connection1->grammaticalTenseModifierArrayTemp);
			connection1->grammaticalTenseTemp = auxHaveEntity->grammaticalTenseTemp;
			connection2->grammaticalTenseTemp = auxHaveEntity->grammaticalTenseTemp;
			}		
		}
		#endif
		
		thingEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
		if(propertyEntitySubstance->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
		{
			if(!(thingEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
			{
				//cout << "addOrConnectPropertyToEntity{} warning: property was declared concept while parent was not declared concept" << endl;
				propertyEntitySubstance->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//CHECKTHIS: will this ever be a quality?
			}
		}
		#endif

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif

	return propertyEntitySubstance;
}

GIAentityNode* addInstanceToInstanceDefinition(GIAentityNode* entity, int instanceType)
{
	GIAentityNode* newOrExistingInstance = entity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(entity->disabled))
	{
	#endif
		if(entity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
		{
			#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
			//NB GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING is not perfect; it cannot link the blue chicken references together
			//A blue chicken is small.
			//A red chicken is fat.
			//The green chicken ate the pie.
			//The blue chicken is late.
			*/
			#ifdef GIA_ENABLE_REFERENCE_LINKING_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
			if((instanceType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE) && checkEntityHasSubstanceThatWasDeclaredInContext(entity) && (entity->grammaticalDefiniteTemp || entity->grammaticalProperNounTemp || entity->grammaticalIndefinitePluralTemp))	//NB the grammaticalProperNounTemp condition should only be required here if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS is set to true
			#else
			if((instanceType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE) && checkEntityHasSubstanceThatWasDeclaredInContext(entity))
			#endif
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "\tbreak; assigning: entity->entityName = " << entity->entityName << endl;
				#endif
				newOrExistingInstance = getEntitySubstanceThatWasDeclaredInContext(entity);
			}
			else
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "\tbreak; adding: entity->entityName = " << entity->entityName << endl;
				#endif
				newOrExistingInstance = addInstance(entity, instanceType);
			}
			#else
			newOrExistingInstance = addInstance(entity, instanceType);
			#endif
		}
		else
		{
			//if(entity->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)	//CHECKTHIS added 2n1a
			//{
				entity->entityType = instanceType;	//upgrade substance to action/condition/concept etc
			//}

			newOrExistingInstance = entity;
		}

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	#endif

	return newOrExistingInstance;
}

GIAentityNode* addInstance(GIAentityNode* entity, int instanceType)
{
	//configure substance node
	GIAentityNode* newInstance = new GIAentityNode();
	#ifdef GIA_USE_DATABASE
	newInstance->added = true;
	#endif

	if(instanceType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
	{
		newInstance->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;
	}
	else if(instanceType == GIA_ENTITY_TYPE_TYPE_ACTION)
	{
		newInstance->entityType = GIA_ENTITY_TYPE_TYPE_ACTION;
	}
	else if(instanceType == GIA_ENTITY_TYPE_TYPE_CONDITION)
	{
		newInstance->entityType = GIA_ENTITY_TYPE_TYPE_CONDITION;
	}
	else if(instanceType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
	{
		newInstance->entityType = GIA_ENTITY_TYPE_TYPE_CONCEPT;
	}
	else
	{
		cout << "addInstance{} error: illegal instanceType, instanceType = " << instanceType << endl;
	}

	addInstanceEntityNodeToActiveLists(newInstance);

	newInstance->entityName = entity->entityName;
	#ifdef GIA_USE_WORD_ORIG
	newInstance->wordOrig = entity->wordOrig;
	#endif
	newInstance->idInstance = determineNextIdInstance(entity);

	entity->hasAssociatedInstance = true;
	entity->hasAssociatedInstanceTemp = true;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	/*//original code from addSubstance: what was this used for?
	#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
	if(entity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
	{
		newInstance->entityType = GIA_ENTITY_TYPE_TYPE_CONCEPT;
		//entityType->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//?
	}
	#endif
	*/

	if(instanceType == GIA_ENTITY_TYPE_TYPE_CONDITION)
	{
		newInstance->negative = entity->negative;	//check forwardInfoToNewSubstance{} is not required
	}
	else
	{
		forwardInfoToNewSubstance(entity, newInstance);
	}

	writeVectorConnection(newInstance, entity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(entity, newInstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	entity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	#endif

	return newInstance;
}




void forwardInfoToNewSubstance(GIAentityNode* entity, GIAentityNode* newSubstance)
{
	newSubstance->hasAssociatedTime = entity->hasAssociatedTime;

	newSubstance->grammaticalNumber = entity->grammaticalNumber;
	newSubstance->grammaticalWordTypeTemp = entity->grammaticalWordTypeTemp;

	/*//execution of addTenseOnlyTimeConditionToSubstance has been shifted from forwardInfoToNewSubstance to a separate function - 26 July 2013
	#ifdef GIA_DEBUG
	//cout << "entity = " << entity->entityName << endl;
	//cout << "entity->grammaticalTenseTemp = " << entity->grammaticalTenseTemp << endl;
	#endif
	if(entity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE])	//changed from newSubstance->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] to entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] 26 July 2013
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToSubstance(newSubstance, entity->grammaticalTenseTemp, entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE]);
	}
	*/

	for(int i=0; i<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; i++)
	{
		newSubstance->grammaticalTenseModifierArrayTemp[i] = entity->grammaticalTenseModifierArrayTemp[i];	//including GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE eg "substance has progressive (eg lying/sitting/being happy)"
	}
	newSubstance->grammaticalTenseTemp = entity->grammaticalTenseTemp;
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_USE_GENERIC_ENTITY_INTERPRETATION
	newSubstance->grammaticalDefiniteTemp = entity->grammaticalDefiniteTemp;	//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalIndefinitePluralTemp = entity->grammaticalIndefinitePluralTemp;	//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalProperNounTemp = entity->grammaticalProperNounTemp;	//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalGenderTemp = entity->grammaticalGenderTemp;		//not currently used
	newSubstance->grammaticalPronounTemp = entity->grammaticalPronounTemp;		//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->mustSetIsConceptBasedOnApposRelation = entity->mustSetIsConceptBasedOnApposRelation;
	#endif
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	newSubstance->grammaticalIndexOfDeterminerTemp = entity->grammaticalIndexOfDeterminerTemp;
	#endif
	#ifdef GIA_SUPPORT_PREDETERMINERS
	newSubstance->grammaticalPredeterminerTemp = entity->grammaticalPredeterminerTemp;
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
	newSubstance->grammaticalPredeterminerTempSentenceArray.insert(make_pair(entity->sentenceIndexTemp, entity->grammaticalPredeterminerTemp));
	#endif
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	newSubstance->NormalizedNERtemp = entity->NormalizedNERtemp;	//always required (not just for time info / time condition related)
	#endif
	newSubstance->NERTemp = entity->NERTemp;

	#ifdef GIA_SUPPORT_ALIASES
	if(entity->isName)
	{
		entity->isName = false;	//added 2n5a
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
		entity->isToBeComplimentOfActionTemp = false;	//added 2n5a
		newSubstance->isToBeComplimentOfActionTemp = true;
	}

	if(entity->negative)
	{
		entity->negative = false;	//added 2n5a
		newSubstance->negative = true;
	}

	#ifdef GIA_SET_ENTITY_ENTITY_AND_SENTENCE_INDICIES_NORMALLY
	newSubstance->entityIndexTemp = entity->entityIndexTemp;
	newSubstance->sentenceIndexTemp = entity->sentenceIndexTemp;
	#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
	cout << "\nnewSubstance->entityName = " << newSubstance->entityName << endl;
	cout << "newSubstance->entityIndexTemp = " << newSubstance->entityIndexTemp << endl;
	//cout << "newSubstance->sentenceIndexTemp = " << newSubstance->sentenceIndexTemp << endl;
	#endif
	#endif

	#ifdef GIA_SUPPORT_NUMBER_OF
	if(entity->isNumberOf)
	{
		entity->isNumberOf = false;	//added 2n5a
		newSubstance->isNumberOf = true;
	}
	#endif

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	if(entity->convertToSubClass)
	{
		entity->convertToSubClass = false;	//added 2n5a
		newSubstance->convertToSubClass = true;
	}
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	if(entity->addSubClass)
	{
		entity->addSubClass = false;	//added 2n5a
		newSubstance->addSubClass = true;
	}
	#endif
	#endif
	
	#ifdef GIA_SUPPORT_EXPLETIVES
	if(entity->isExpletive)
	{
		entity->isExpletive = false;
		newSubstance->isExpletive = true;
	}	
	#endif
	
}


/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, int tense, vector<GIAtimeConditionNode*>* timeConditionNodesActiveList, vector<long>* timeConditionNumbersActiveList)
#else
void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, int tense)
#endif
*/
void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, int tense, bool isProgressive)
{
	substanceNode->conditionType = CONDITION_NODE_TYPE_TIME;

	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = INT_DEFAULT_VALUE;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = 0; //cannot assign absolute time to an event which occurs in the past.... //calculateTotalTimeInSeconds{};
	GIAtimeConditionNode* newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, networkIndexEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIAtimeConditionNode* newTimeCondition = new GIAtimeConditionNode();
	#endif
	*/

	GIAtimeConditionNode* newTimeCondition = new GIAtimeConditionNode();

	newTimeCondition->tense = tense;
	newTimeCondition->tenseOnlyTimeCondition = true;
	newTimeCondition->conditionName = grammaticalTenseNameArray[tense];
	if(isProgressive)
	{
		newTimeCondition->isProgressive = true;
	}
	substanceNode->timeConditionNode = newTimeCondition;


}

void addDefinitionToEntity(GIAentityNode* thingEntity, GIAentityNode* definitionEntity, bool sameReferenceSet)
{
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(thingEntity->disabled))
	{
	if(!(definitionEntity->disabled))
	{
	#endif

		//configure entity node and entity definition node
		writeVectorConnection(thingEntity, definitionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
		writeVectorConnection(definitionEntity, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif
}

#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
void addDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* thingEntity, GIAentityNode* definitionEntity, bool sameReferenceSet)
{
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(thingEntity->disabled))
	{
	if(!(definitionEntity->disabled))
	{
	#endif
		//configure entity node and entity definition node
		GIAentityConnection* connection = writeVectorConnection(thingEntity, definitionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
		connection->isAlias = true;
		GIAentityConnection* connectionReverse = writeVectorConnection(definitionEntity, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);
		connectionReverse->isAlias = true;
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif
}
#endif





	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
GIAentityNode* addOrConnectActionToEntity(GIAentityNode* subjectEntity, GIAentityNode* objectEntity, GIAentityNode* actionEntity, bool sameReferenceSet)
{
	/*//disabled 2o6a due to *
	#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	#ifdef GIA_TRANSLATOR_DO_NOT_CREATE_CONCEPT_PROPERTIES_FOR_NON_CONCEPT_PARENTS
	if(isActionSpecialPossessive(actionEntity))
	{
		if(!(subjectEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
		{
			objectEntity->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//CHECKTHIS: will this ever be a quality?
		}
	}
	#endif
	#endif
	*/

	GIAentityNode* newOrExistingAction = actionEntity;
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
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
		GIAentityConnection* connectionFound = findEntityNodeNameInVector(subjectEntity, &(actionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode* currentActionNodeInList = connectionFound->entity;
			bool foundNode2 = false;
			GIAentityConnection* connectionFound2 = findEntityNodePointerInVector(currentActionNodeInList, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, &foundNode2);
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

		newOrExistingAction = addInstanceToInstanceDefinition(actionEntity, GIA_ENTITY_TYPE_TYPE_ACTION);

		connectActionInstanceToSubject(subjectEntity, newOrExistingAction, sameReferenceSet);

		connectActionInstanceToObject(objectEntity, newOrExistingAction, sameReferenceSet);
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	}
	#endif
	return newOrExistingAction;
}

GIAentityNode* addOrConnectActionToSubject(GIAentityNode* subjectEntity, GIAentityNode* actionEntity, bool sameReferenceSet)
{
	GIAentityNode* newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		/*//do not presume single linked actions/conditions are identical
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection* connectionFound = findEntityNodeNameInVector(subjectEntity, &(actionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode* currentActionNodeInList = connectionFound->entity;
			if(newOrExistingAction != currentActionNodeInList)
			{
				newOrExistingAction->disabled = true;
				newOrExistingAction = currentActionNodeInList;
			}
		}
		#endif
		*/
		newOrExistingAction = addInstanceToInstanceDefinition(actionEntity, GIA_ENTITY_TYPE_TYPE_ACTION);
		connectActionInstanceToSubject(subjectEntity, newOrExistingAction, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}

GIAentityNode* addOrConnectActionToObject(GIAentityNode* objectEntity, GIAentityNode* actionEntity, bool sameReferenceSet)
{
	GIAentityNode* newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(objectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		/*//do not presume single linked actions/conditions are identical
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection* connectionFound = findEntityNodeNameInVector(objectEntity, &(actionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode* currentActionNodeInList = connectionFound->entity;
			if(newOrExistingAction != currentActionNodeInList)
			{
				newOrExistingAction->disabled = true;
				newOrExistingAction = currentActionNodeInList;
			}
		}
		#endif
		*/
		newOrExistingAction = addInstanceToInstanceDefinition(actionEntity, GIA_ENTITY_TYPE_TYPE_ACTION);
		connectActionInstanceToObject(objectEntity, newOrExistingAction, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}


void connectActionInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingAction, bool sameReferenceSet)
{
	//configure action subject entity node
	writeVectorConnection(subjectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, subjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, sameReferenceSet);

	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

void connectActionInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingAction, bool sameReferenceSet)
{
	//configure action object entity node
	writeVectorConnection(objectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, sameReferenceSet);

	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}






GIAentityNode* addOrConnectConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionObjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet)
{
	GIAentityNode* newOrExistingCondition = conditionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(conditionSubjectEntity->disabled))
	{
	if(!(conditionObjectEntity->disabled))
	{
	if(!(conditionEntity->disabled))
	{
	#endif

		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection* connectionFound = findEntityNodeNameInVector(conditionSubjectEntity, &(conditionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode* currentConditionNodeInList = connectionFound->entity;
			bool foundNode2 = false;
			GIAentityConnection* connectionFound2 = findEntityNodePointerInVector(currentConditionNodeInList, conditionObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, &foundNode2);
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

		newOrExistingCondition = addInstanceToInstanceDefinition(conditionEntity, GIA_ENTITY_TYPE_TYPE_CONDITION);

		#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
		if(conditionObjectEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
		{
			if(!(conditionSubjectEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
			{
				//cout << "addOrConnectConditionToEntity{} warning: condition object was declared concept while condition subject was not declared concept" << endl;
				conditionObjectEntity->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//CHECKTHIS: will this ever be a quality?
			}
		}
		#endif

		#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		//required to compensate for defineSubstancesActions{} being exectuted before linkDependentActionsType1{}
		newOrExistingCondition->entityType = GIA_ENTITY_TYPE_TYPE_CONDITION;
		#endif
		//conditionSubjectEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToSubject(conditionSubjectEntity, newOrExistingCondition, sameReferenceSet);
		connectConditionInstanceToObject(conditionObjectEntity, newOrExistingCondition, sameReferenceSet);

		#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
		identifyConditionType(newOrExistingCondition);
		#endif

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAentityNode* addOrConnectConditionToSubject(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet)
{
	GIAentityNode* newOrExistingCondition = conditionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(conditionSubjectEntity->disabled))
	{
	if(!(conditionEntity->disabled))
	{
	#endif
		/*//do not presume single linked actions/conditions are identical
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection* connectionFound = findEntityNodeNameInVector(conditionSubjectEntity, &(conditionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode* currentConditionNodeInList = connectionFound->entity;
			if(newOrExistingCondition != currentConditionNodeInList)
			{
				newOrExistingCondition->disabled = true;
				newOrExistingCondition = currentConditionNodeInList;
			}
		}
		#endif
		*/

		newOrExistingCondition = addInstanceToInstanceDefinition(conditionEntity, GIA_ENTITY_TYPE_TYPE_CONDITION);

		#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		//required to compensate for defineSubstancesActions{} being exectuted before linkDependentActionsType1{}
		newOrExistingCondition->entityType = GIA_ENTITY_TYPE_TYPE_CONDITION;
		#endif
		//conditionSubjectEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToSubject(conditionSubjectEntity, newOrExistingCondition, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAentityNode* addOrConnectConditionToObject(GIAentityNode* conditionObjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet)
{
	GIAentityNode* newOrExistingCondition = conditionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(conditionObjectEntity->disabled))
	{
	if(!(conditionEntity->disabled))
	{
	#endif
		/*//do not presume single linked actions/conditions are identical
		#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
		//see if relevant link already exists between the two nodes, and if so use that
		bool foundNode1 = false;
		GIAentityConnection* connectionFound = findEntityNodeNameInVector(conditionObjectEntity, &(conditionEntity->entityName), GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, &foundNode1);
		if(foundNode1)
		{
			GIAentityNode* currentConditionNodeInList = connectionFound->entity;
			if(newOrExistingCondition != currentConditionNodeInList)
			{
				newOrExistingCondition->disabled = true;
				newOrExistingCondition = currentConditionNodeInList;
			}
		}
		#endif
		*/

		newOrExistingCondition = addInstanceToInstanceDefinition(conditionEntity, GIA_ENTITY_TYPE_TYPE_CONDITION);

		#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		//required to compensate for defineSubstancesActions{} being exectuted before linkDependentActionsType1{}
		newOrExistingCondition->entityType = GIA_ENTITY_TYPE_TYPE_CONDITION;
		#endif
		//entityNode->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToObject(conditionObjectEntity, newOrExistingCondition, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif

	return newOrExistingCondition;
}

void connectConditionInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingCondition, bool sameReferenceSet)
{
	//configure condition subject entity node
	writeVectorConnection(subjectEntity, newOrExistingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingCondition, subjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
}

void connectConditionInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingCondition, bool sameReferenceSet)
{
	//configure condition object entity node
	writeVectorConnection(objectEntity, newOrExistingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingCondition, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, sameReferenceSet);
}


#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
GIAentityNode* addOrConnectBeingDefinitionConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionDefinitionNode, GIAentityNode* conditionEntity, bool negative, bool sameReferenceSet)
{
	GIAentityNode* newOrExistingCondition = conditionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(conditionSubjectEntity->disabled))
	{
	if(!(conditionDefinitionNode->disabled))
	{
	if(!(conditionEntity->disabled))
	{
	#endif
		newOrExistingCondition = addInstanceToInstanceDefinition(conditionEntity, GIA_ENTITY_TYPE_TYPE_CONDITION);

		//conditionSubjectEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToSubject(conditionSubjectEntity, newOrExistingCondition, sameReferenceSet);
		addDefinitionToEntity(newOrExistingCondition, conditionDefinitionNode, sameReferenceSet);

		if(conditionEntity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
		{
			newOrExistingCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node
		}
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAentityNode* addOrConnectHavingPropertyConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionSubstanceNode, GIAentityNode* conditionEntity, bool negative, bool sameReferenceSet)
{
	GIAentityNode* newOrExistingCondition = conditionEntity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(conditionSubjectEntity->disabled))
	{
	if(!(conditionSubstanceNode->disabled))
	{
	if(!(conditionEntity->disabled))
	{
	#endif
		newOrExistingCondition = addInstanceToInstanceDefinition(conditionEntity, GIA_ENTITY_TYPE_TYPE_CONDITION);

		//conditionSubjectEntity->hasSubstanceTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		//configure entity node containing this substance
		connectConditionInstanceToSubject(conditionSubjectEntity, newOrExistingCondition, sameReferenceSet);
		connectPropertyToEntity(newOrExistingCondition, conditionSubstanceNode, sameReferenceSet);

		if(conditionEntity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
		{
			newOrExistingCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node
		}
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}
#endif


//assumes prepositions have previously been converted to stanford prep_preposition format during preprocessor (for robustness)
string convertPrepositionToRelex(string* preposition, bool* prepositionFound)
{
	*prepositionFound = false;
	string relexPreposition = *preposition;
	for(int i=0; i<REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES; i++)
	{
		string currentStanfordPrepositionPrepend = referenceTypeStanfordParserPrepositionPrependNameArray[i];
		int foundStanfordPrepositionPrepend = preposition->find(currentStanfordPrepositionPrepend);

		if(foundStanfordPrepositionPrepend != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			int indexOfFirstRealCharacterInPreposition = currentStanfordPrepositionPrepend.length();
			int lengthOfPreposition = preposition->length() - (indexOfFirstRealCharacterInPreposition);
			relexPreposition = preposition->substr(indexOfFirstRealCharacterInPreposition, lengthOfPreposition);
			*prepositionFound = true;
		}
	}

	return relexPreposition;
}


void setTranslatorEntityNodesCompleteList(vector<GIAentityNode*>* newEntityNodesCompleteList)
{
	entityNodesActiveListComplete = newEntityNodesCompleteList;
}
void setTranslatorEntityNodesCompleteListFastIndex(unordered_map<string, GIAentityNode*>* newEntityNodesCompleteListFastIndex)
{
	entityNodesActiveListCompleteFastIndex = newEntityNodesCompleteListFastIndex;
}
/*
void setTranslatorNetworkIndexEntityNodesList(vector<GIAentityNode*>* newNetworkIndexEntityNodesList)
{
	entityNodesActiveListNetworkIndexes = newNetworkIndexEntityNodesList;
}
*/

vector<GIAentityNode*>* getTranslatorEntityNodesCompleteList()
{
	return entityNodesActiveListComplete;
}
unordered_map<string, GIAentityNode*>* getTranslatorEntityNodesCompleteListFastIndex()
{
	return entityNodesActiveListCompleteFastIndex;
}
/*
vector<GIAentityNode*>* getTranslatorNetworkIndexEntityNodesList()
{
	return entityNodesActiveListNetworkIndexes;
}
*/

void setSaveNetwork(bool val)
{
	saveNetwork = val;
}
bool getSaveNetwork()
{
	return saveNetwork;
}
#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
void setLinkPreestablishedReferencesGIA(bool val)
{
	linkPreestablishedReferencesGIA = val;
}
bool getLinkPreestablishedReferencesGIA()
{
	return linkPreestablishedReferencesGIA;
}
#endif



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


long* getCurrentEntityNodeIDinCompleteList()
{
	return &currentEntityNodeIDinCompleteList;
}
long* getCurrentEntityNodeIDinNetworkIndexEntityNodesList()
{
	return &currentEntityNodeIDinNetworkIndexEntityNodesList;
}


long* getCurrentEntityNodeIDinSentenceCompleteList()
{
	return &currentEntityNodeIDInSentenceCompleteList;
}
long* getCurrentEntityNodeIDinSentenceNetworkIndexEntityNodesList()
{
	return &currentEntityNodeIDInSentenceNetworkIndexEntityNodesList;
}

#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
int getCurrentSentenceIndex()
{
	return currentSentenceIndex;
}
void setCurrentSentenceIndex(int value)
{
	currentSentenceIndex = value;
}
#endif


void applyNetworkIndexEntityAlreadyExistsFunction(GIAentityNode* entity, bool entityAlreadyExistant, bool tempEntityEnabled)
{
	if(entityAlreadyExistant)
	{
		if(tempEntityEnabled)
		{
			entity->disabled = false;
			entity->firstSentenceToAppearInNetwork = false;
		}
	}
	else
	{
		entity->firstSentenceToAppearInNetwork	= true;
	}
}

void disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	if(entity->firstSentenceToAppearInNetwork)
	{
		entity->disabled = true;
	}
	#endif
}

void disableEntity(GIAentityNode* entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	entity->disabled = true;
	#endif
}


void disableNetworkIndexEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entity);	//CHECKTHIS; only disable the networkIndex if it was created in the immediate context (eg sentence)

	if(entity->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the immediate context (eg sentence)
	//if(entity->associatedInstanceNodeList.size() >= 1)
	{
		GIAentityNode* associateInstance = (entity->associatedInstanceNodeList->back())->entity;
		associateInstance->disabled = true;	//and disable their associated instances (substance nodes)
	}
	#endif
}

//NB only disables the primary networkIndex node defining the instance
void disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	entity->disabled = true;

	if(!(entity->entityNodeDefiningThisInstance->empty()))
	{
		//CHECKTHIS; only disable the networkIndex if it was created in the immediate context (eg sentence)
		GIAentityNode* networkIndexEntity = getPrimaryNetworkIndexNodeDefiningInstance(entity);

		disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(networkIndexEntity);
	}
	#endif
}



void recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap)
{
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListMapIter = networkIndexEntityNodesListMap->begin(); networkIndexEntityNodesListMapIter != networkIndexEntityNodesListMap->end(); networkIndexEntityNodesListMapIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListMapIter->second;
		if(!(entityNode->disabled))
		{
			entityNode->permanentNetworkIndex = true;
		}
	}
}


//(used for printing/xml write purposes)
void recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap)
{
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListMapIter = networkIndexEntityNodesListMap->begin(); networkIndexEntityNodesListMapIter != networkIndexEntityNodesListMap->end(); networkIndexEntityNodesListMapIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListMapIter->second;
		if(!(entityNode->permanentNetworkIndex))
		{
			entityNode->disabled = true;
		}
	}
}

//(used for printing/xml write purposes)
void recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap)
{
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListMapIter = networkIndexEntityNodesListMap->begin(); networkIndexEntityNodesListMapIter != networkIndexEntityNodesListMap->end(); networkIndexEntityNodesListMapIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListMapIter->second;
		if(entityNode->disabled)
		{
			entityNode->permanentNetworkIndex = false;
		}
		else
		{
			entityNode->permanentNetworkIndex = true;
		}
	}
}

void convertRelexPOStypeToWordnetWordType(string* relexPOStype, int* grammaticalWordTypeTemp)
{
	*grammaticalWordTypeTemp = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOStypeArray[i] == *relexPOStype)
		{
			*grammaticalWordTypeTemp = featureRelexPOStypeCrossReferenceWordnetWordTypeArray[i];
		}
	}

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "convertRelexPOStypeToWordnetWordType{}: " << endl;
	cout << "relexPOStype = " <<* relexPOStype << endl;
	cout << "grammaticalWordTypeTemp = " <<* grammaticalWordTypeTemp << endl;
	*/
	#endif
}

void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp)
{
	*relexPOStype = FEATURE_RELEX_POS_TYPE_WORD;
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL; i++)
	{
		if(featurePOStagMinimalArray[i] == *POStag)
		{
			*relexPOStype = featurePOStagCrossReferenceRelexPOStypeArray[i];
		}
	}

	convertRelexPOStypeToWordnetWordType(relexPOStype, grammaticalWordTypeTemp);

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "convertStanfordPOStagToRelexPOStypeAndWordnetWordType{}: " << endl;
	cout << "POStag = " <<* POStag << endl;
	cout << "relexPOStype = " <<* relexPOStype << endl;
	cout << "grammaticalWordTypeTemp = " <<* grammaticalWordTypeTemp << endl;
	*/
	#endif

}

void generateTempFeatureArray(GIAfeature* firstFeatureInList, GIAfeature* featureArrayTemp[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		featureArrayTemp[w] = NULL;	//initialise as NULL (required to prevent crashes during array access) - added 14 July 2013
	}

	GIAfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		#ifdef GIA_DEBUG
		//cout << "currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
		#endif
		featureArrayTemp[currentFeatureInList->entityIndex] = currentFeatureInList;
		currentFeatureInList = currentFeatureInList->next;
	}

	/*OLD: is not compatible with Relex query nodes (set to new entity index eg "199")
	//generateFeatureArray;
	int featureIndex = 1;
	GIAfeature* currentFeatureInList = firstFeatureInList;
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
//NOT REQUIRED: redundant: this unique check is redundant considering if a networkIndex entity has a substance that was declared in the immediate context, ie sentence, then the entity node being queried will be the substance itself (and not its networkIndex)
bool checkEntityHasSubstanceThatWasDeclaredInContextAndIsUnique(GIAentityNode* entityNode, int entityIndexTemp, int sentenceIndexTemp)
{
	bool result = false;

	if(checkEntityHasSubstanceThatWasDeclaredInContext(entityNode))
	{
		GIAentityNode* entitySubstanceDeclaredInContext = getEntitySubstanceThatWasDeclaredInContext(entityNode);
		if(!((entitySubstanceDeclaredInContext->entityIndexTemp == entityIndexTemp) && (entitySubstanceDeclaredInContext->sentenceIndexTemp == sentenceIndexTemp)))
		{
			result = true;
		}
	}

	return result;
}
*/

bool checkEntityHasSubstanceThatWasDeclaredInContext(GIAentityNode* entityNode)
{
	bool result = false;

	if(entityNode->entityAlreadyDeclaredInThisContext)
	{
		result = true;
	}

	return result;
}

GIAentityNode* getEntitySubstanceThatWasDeclaredInContext(GIAentityNode* entityNode)
{
	GIAentityNode* entityNodeAssociatedInstance = entityNode;

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

#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, GIArelation* relation)
{
	bool auxiliaryIndicatesDifferentReferenceSet = relation->auxiliaryIndicatesDifferentReferenceSet;
	bool rcmodIndicatesSameReferenceSet = relation->rcmodIndicatesSameReferenceSet;

	bool sameReferenceSet = defaultSameSetValueForRelation;
	if(auxiliaryIndicatesDifferentReferenceSet)
	{
		sameReferenceSet = false;
	}
	if(rcmodIndicatesSameReferenceSet)
	{
		sameReferenceSet = true;
	}

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\ndetermineSameReferenceSetValue:" << endl;
	cout << "\t" << relation->relationType << "(" << relation->relationGovernor << ", " << relation->relationDependent << ")" << endl;
	cout << "\tauxiliaryIndicatesDifferentReferenceSet = " << auxiliaryIndicatesDifferentReferenceSet << endl;
	cout << "\trcmodIndicatesSameReferenceSet = " << rcmodIndicatesSameReferenceSet << endl;
	cout << "\tdefaultSameSetValueForRelation = " << defaultSameSetValueForRelation << endl;
	cout << "\tsameReferenceSet = " << sameReferenceSet << endl;
	#endif

	return sameReferenceSet;
}
#endif


GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperCondition(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int featureIndex, string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	GIAentityNode* conditionEntity;
	#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_BAD1
	if(GIAentityNodeArrayFilled[featureIndex] && (GIAentityNodeArray[featureIndex]->entityName == *entityNodeName))
	#else
	if(GIAentityNodeArrayFilled[featureIndex])
	#endif
	{
		conditionEntity = GIAentityNodeArray[featureIndex];
	}
	else
	{
		#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_BAD1
		if(GIAentityNodeArray[featureIndex]->entityName != *entityNodeName)
		{
			//must recreate a networkIndex entity for the preposition as its name has changed based on the redistribution process
			disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[featureIndex]);
		}
		#endif
		conditionEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListNetworkIndexes);
		GIAentityNodeArrayFilled[featureIndex] = true;
	}
	#else
	conditionEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListNetworkIndexes);
	#endif

	return conditionEntity;
}

GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	return findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListNetworkIndexes, true);
}

GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, bool tempEntityEnabled)
{
	GIAentityNode* entityNodeFound = NULL;

	/*already available locally...
	long* currentEntityNodeIDinCompleteList;
	long* currentEntityNodeIDinNetworkIndexEntityNodesList;
	vector<GIAentityNode*>* entityNodesActiveListComplete;
	if(saveNetwork)
	{
		entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();
		currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinCompleteList();
		currentEntityNodeIDinNetworkIndexEntityNodesList = getCurrentEntityNodeIDinNetworkIndexEntityNodesList();
	}
	else
	{
		currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinSentenceCompleteList();
		currentEntityNodeIDinNetworkIndexEntityNodesList = getCurrentEntityNodeIDinSentenceNetworkIndexEntityNodesList();
	}
	*/

	long entityIndex = INT_DEFAULT_VALUE;
	entityNodeFound = findOrAddNetworkIndexEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, entityNodeName, entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDinCompleteList, &currentEntityNodeIDinNetworkIndexEntityNodesList, saveNetwork);

	applyNetworkIndexEntityAlreadyExistsFunction(entityNodeFound,* entityAlreadyExistant, tempEntityEnabled);

	return entityNodeFound;
}




/*these functions have been added for GIA Database compatibility*/
//this function does write to database, but prepares data structures for write to database (at the end of the user sequence, writeDatabase{} is written...)
GIAentityConnection* writeVectorConnection(GIAentityNode* entityNode, GIAentityNode* entityNodeToAdd, int connectionType, bool sameReferenceSet)
{
	GIAentityConnection* newConnection = NULL;
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	if(!(entityNode->wasReferenceTemp && entityNodeToAdd->wasReferenceTemp))
	{
	#endif
		#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE
		bool foundNode2 = false;
		if(!findSameSentenceEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &foundNode2, getCurrentSentenceIndex()))
		{
		#endif
			#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_PROPERTIES_AND_DEFINITIONS
			//see if link already exists between the two nodes, and if so increment the confidence
			bool foundNode1 = false;
			GIAentityConnection* connectionFound1 = findEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &foundNode1);
			if(foundNode1)
			{
				connectionFound1->confidence = connectionFound->confidence + GIA_ENTITY_CONNECTION_CONFIDENCE_INCREMENT;
			}
			else
			{
			#endif
				vector<GIAentityConnection*>* vectorConnection = &(entityNode->entityVectorConnectionsArray[connectionType]);
				#ifndef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
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
				#endif

				newConnection = new GIAentityConnection();
				newConnection->entity = entityNodeToAdd;
				#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
				bool foundNode3 = false;
				findEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &foundNode3);
				if(foundNode3)
				{
					newConnection->isReference = true;
				}
				#endif
				vectorConnection->push_back(newConnection);

				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
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
					//#ifdef GIA_USE_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS		//why is this preprocessor check not required???
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

				#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
				newConnection->sentenceIndexTemp = getCurrentSentenceIndex();
				#endif
			#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_PROPERTIES_AND_DEFINITIONS
			}
			#endif
		#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE
		}
		#endif
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	}
	#endif
	return newConnection;
}


GIAentityConnection* findEntityNodePointerInVector(GIAentityNode* entityNode, GIAentityNode* entityNodeToFind, int connectionType, bool* foundNode)
{
	GIAentityConnection* connectionFound = NULL;
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

#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE
GIAentityConnection* findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, GIAentityNode* entityNodeToFind, int connectionType, bool* foundNode, int sentenceIndex)
{
	GIAentityConnection* connectionFound = NULL;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if((*connectionIter)->entity == entityNodeToFind)
		{
			if((*connectionIter)->sentenceIndexTemp == sentenceIndex)
			{
				*foundNode = true;
				connectionFound = *connectionIter;
			}
		}
	}
	return connectionFound;
}
#endif

GIAentityConnection* findEntityNodeNameInVector(GIAentityNode* entityNode, string* entityNodeNameToFind, int connectionType, bool* foundNode)
{
	GIAentityConnection* connectionFound = NULL;
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



long determineNextIdInstance(GIAentityNode* entity)
{
	long nextIdInstance;
	#ifdef GIA_DATABASE_DEBUG
	cout << "\t\tDEBUG: determineNextIdInstance{}; 0. entity->entityName = " << entity->entityName << endl;
	cout << "\t\tDEBUG: determineNextIdInstance{}; 0. entity->idInstance = " << entity->idInstance << endl;
	#endif
	GIAentityNode* networkIndexEntity;
	#ifdef GIA_APPLY_BUG_WORKAROUND_WHERE_A_NETWORK_INDEX_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE
	if(entity->idInstance == GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE)
	{
		//the current entity is a networkIndex entity
		networkIndexEntity = entity;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance{}; 1b. networkIndexEntity->entityName = " << networkIndexEntity->entityName << endl;
		cout << "\t\tDEBUG: determineNextIdInstance{}; 1b. networkIndexEntity->idInstance = " << networkIndexEntity->idInstance << endl;
		#endif
	}
	else
	{
	#endif
		if(!(entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE].empty()))
		{
			//the current entity is a substance of a networkIndex entity
			networkIndexEntity = getPrimaryNetworkIndexNodeDefiningInstance(entity);
			#ifdef GIA_DATABASE_DEBUG
			cout << "\t\tDEBUG: determineNextIdInstance{}; 1a. networkIndexEntity->entityName = " << networkIndexEntity->entityName << endl;
			cout << "\t\tDEBUG: determineNextIdInstance{}; 1a. networkIndexEntity->idInstance = " << networkIndexEntity->idInstance << endl;
			#endif
		}
		else
		{
			//the current entity is a networkIndex entity
			networkIndexEntity = entity;
			#ifdef GIA_DATABASE_DEBUG
			cout << "\t\tDEBUG: determineNextIdInstance{}; 1b. networkIndexEntity->entityName = " << networkIndexEntity->entityName << endl;
			cout << "\t\tDEBUG: determineNextIdInstance{}; 1b. networkIndexEntity->idInstance = " << networkIndexEntity->idInstance << endl;
			#endif
		}
	#ifdef GIA_APPLY_BUG_WORKAROUND_WHERE_A_NETWORK_INDEX_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE
	}
	#endif

//#if defined(GIA_USE_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS) || !defined(GIA_USE_DATABASE)
//check #elif !defined GIA_USE_DATABASE is OK instead of #elif !defined(GIA_USE_DATABASE)
#ifdef GIA_USE_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].empty()))
	{
		long previousIdInstance = networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].back()->entity->idInstance;
		nextIdInstance = previousIdInstance + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance{}; 2a. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	else
	{
		nextIdInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance{}; 2b. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	#else
	long numberOfInstances =  networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].size();
	nextIdInstance = numberOfInstances;
	#endif
#elif !defined GIA_USE_DATABASE
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].empty()))
	{
		long previousIdInstance = networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].back()->entity->idInstance;
		nextIdInstance = previousIdInstance + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance{}; 2a. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	else
	{
		nextIdInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance{}; 2b. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	#else
	long numberOfInstances =  networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].size();
	nextIdInstance = numberOfInstances;
	#endif
#else
	if(networkIndexEntity->networkIndexEntityLoaded == NULL)
	{
		cout <<"ERROR: (networkIndexEntity->networkIndexEntityLoaded == NULL)" << endl;
	}
	GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = networkIndexEntity->networkIndexEntityLoaded;
	#ifdef GIA_DATABASE_DEBUG
	cout << "networkIndexEntityLoaded->numberOfInstances = " << networkIndexEntityLoaded->numberOfInstances << endl;
	#endif
	networkIndexEntityLoaded->numberOfInstances = networkIndexEntityLoaded->numberOfInstances + 1;
	nextIdInstance = networkIndexEntityLoaded->numberOfInstances;
#endif

	return nextIdInstance;
}


#ifdef GIA_USE_DATABASE
/*//replaced with optimised function findEntityNodesActiveListCompleteFastIndexDBactive{}
bool entityInActiveListComplete(string* entityName, long idInstance)
{
	bool entityAlreadyInActiveListComplete = false;
	for(vector<GIAentityNode*>::iterator entityNodesActiveListCompleteIter = entityNodesActiveListComplete->begin(); entityNodesActiveListCompleteIter != entityNodesActiveListComplete->end(); entityNodesActiveListCompleteIter++)
	{
		GIAentityNode* entityNode = *entityNodesActiveListComplete;
		if((entityNode->entityName == entityName) && (entityNode->idInstance == idInstance))
		{
			entityAlreadyInActiveListComplete = true;
		}
	}
	cout << "entityInActiveListComplete(string* entityName, long idInstance) = " << entityAlreadyInActiveListComplete << endl;
	return entityAlreadyInActiveListComplete;
}

bool entityInActiveListComplete(GIAentityNode* entity)
{
	bool entityAlreadyInActiveListComplete = false;
	for(vector<GIAentityNode*>::iterator entityNodesActiveListCompleteIter = entityNodesActiveListComplete->begin(); entityNodesActiveListCompleteIter != entityNodesActiveListComplete->end(); entityNodesActiveListCompleteIter++)
	{
		GIAentityNode* entityNode = *entityNodesActiveListComplete;
		if(entityNode == entity)
		{
			entityAlreadyInActiveListComplete = true;
		}
	}
	cout << "entityInActiveListComplete(GIAentityNode* entity) = " << entityAlreadyInActiveListComplete << endl;
	return entityAlreadyInActiveListComplete;
}
*/

void addInstanceEntityNodeToActiveLists(GIAentityNode* entity)
{//NB add reference set entity to active list complete + appropriate list (substance/action/condition) [NB the reference set entity will already be added to networkIndex active list entityNodesActiveListNetworkIndexes...] - this function enables references to be written to XML

	/*already available locally...
	long* currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinCompleteList();
	long* currentEntityNodeIDinNetworkIndexEntityNodesList = getCurrentEntityNodeIDinNetworkIndexEntityNodesList();
	*/

	if(saveNetwork)
	{
		if(entity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
		{
			//addInstanceEntityNodeToActiveLists{} does not support networkIndex entities. NB when this function is executed via linkAdvancedReferencesGIA{}, the referenceSource networkIndex entity is already added to the networkIndex active list (ie, see GIAentityNode* referenceSourceNetworkIndexEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper)
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
	{//this case should never be used when this function is executed via linkAdvancedReferencesGIA{}
		entity->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}
}
#endif



#ifdef GIA_SUPPORT_ALIASES
void mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge)
{
	if(entityNode->idActiveList == entityNodeToMerge->idActiveList)
	{
		#ifdef GIA_ALIASES_DEBUG
		cout << "treatDefinitionAsEquality: already merged" << endl;
		#endif
	}
	else
	{
		entityNode->aliasList.push_back(entityNodeToMerge->entityName);

		#ifdef GIA_ALIASES_DEBUG
		cout << "\n" << endl;
		cout << "entityNode->entityName = " << entityNode->entityName << endl;
		cout << "entityNodeToMerge->entityName = " << entityNodeToMerge->entityName << endl;
		cout << "entityNode->entityType = " << entityNode->entityType << endl;
		cout << "entityNodeToMerge->entityType = " << entityNodeToMerge->entityType << endl;
		#endif

		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNodeToMerge->entityVectorConnectionsArray[i].begin(); connectionIter != entityNodeToMerge->entityVectorConnectionsArray[i].end(); )
			{
				bool connectionIterErased = false;
				//connect entityNodeToMerge ambient node to entityNode
				GIAentityNode* entityConnectedToEntityToMerge = (*connectionIter)->entity;

				#ifdef GIA_ALIASES_DEBUG
				if(entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
				{
					cout << "entityConnectedToEntityToMerge->entityName = " << entityConnectedToEntityToMerge->entityName << " (networkIndex)" << endl;
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
						GIAentityNode* entityConnectedToEntityConnectedToEntityToMerge = (*connectionIter2)->entity;

						#ifdef GIA_ALIASES_DEBUG
						if(entityConnectedToEntityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
						{
							cout << "entityConnectedToEntityConnectedToEntityToMerge->entityName = " << entityConnectedToEntityConnectedToEntityToMerge->entityName << " (networkIndex)" << endl;
						}
						else
						{
							cout << "entityConnectedToEntityConnectedToEntityToMerge->entityName = " << entityConnectedToEntityConnectedToEntityToMerge->entityName << endl;
						}
						#endif

						if(entityNodeToMerge == entityConnectedToEntityConnectedToEntityToMerge)	//OR (entityNodeToMerge == entityConnectedToEntityConnectedToEntityToMerge)?
						{
							#ifndef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
							//the commenting out of the below case is required for advanced referencing (eg networkIndex Tom has associated instance Dog) [NB this means that instances can appear to have more than one entityNodeDefiningThisInstance]
							if((entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX) && (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE))
							{//restored 29 November 2012, and condition (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE) added
								//disconnect entityConnectedToEntityConnectedToEntityToMerge from entityConnectedToEntityToMerge (networkIndex) (z2)
								#ifdef GIA_ALIASES_DEBUG
								cout << "disconnect entityConnectedToEntityConnectedToEntityToMerge (" << entityConnectedToEntityConnectedToEntityToMerge->entityName << ") from entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (networkIndex) (z2)" << endl;
								#endif
								//delete* connectionIter2;	//delete connection
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
					//the commenting out of the below case is required for advanced referencing (eg networkIndex Tom has associated instance Dog) [NB this means that instances can appear to have more than one entityNodeDefiningThisInstance]
					if(!((entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX) && (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)))
					{
					#endif
						#ifdef GIA_ALIASES_DEBUG
						cout << "connect entityNode (" << entityNode->entityName << ") to entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (x)" << endl;
						#endif
						#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
						bool sameReferenceSet = (*connectionIter)->sameReferenceSet;
						#else
						bool sameReferenceSet = IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
						#endif
						writeVectorConnection(entityNode, entityConnectedToEntityToMerge, i, sameReferenceSet);
					#ifndef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
					}
					#endif
				}

				//disconnect entityConnectedToEntityToMerge from entityNodeToMerge
				#ifdef GIA_ALIASES_DEBUG
				if(entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
				{
					cout << "disconnect entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (networkIndex) from entityNodeToMerge (" << entityNodeToMerge->entityName << ") (x2)" << endl;
				}
				else
				{
					cout << "disconnect entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") from entityNodeToMerge (" << entityNodeToMerge->entityName << ") (x2)" << endl;
				}
				#endif
				//delete* connectionIter;	//delete connection
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
				GIAentityNode* entityConnectedToEntity = (*connectionIter)->entity;
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
			#ifdef GIA_DEBUG
			//cout << "entityNodeToMerge->isQuery = " << entityNodeToMerge->isQuery << endl;
			#endif
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
		cout << "entityNode->entityName = " << entityNode->entityName << endl;
		cout << "entityNode->aliasList[0] = " << entityNode->aliasList[0] << endl;
		cout << "entityNode->entityIndexTemp = " << entityNode->entityIndexTemp << endl;
		#endif
	}
}
#endif

GIAentityNode* getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNode* instanceEntity)
{
	GIAentityNode* primaryNetworkIndexNodeDefiningInstance = NULL;

	if(!(instanceEntity->entityNodeDefiningThisInstance->empty()))
	{
		primaryNetworkIndexNodeDefiningInstance = (instanceEntity->entityNodeDefiningThisInstance->back())->entity;

		#ifdef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
		for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->entityNodeDefiningThisInstance->begin(); connectionIter != instanceEntity->entityNodeDefiningThisInstance->end(); connectionIter++)
		{
			GIAentityNode* networkIndexEntityNode = (*connectionIter)->entity;
			if(instanceEntity->entityName == networkIndexEntityNode->entityName)
			{
				#ifdef GIA_DEBUG
				//cout << "primaryNetworkIndexNodeDefiningInstance = networkIndexEntityNode" << endl;
				#endif
				primaryNetworkIndexNodeDefiningInstance = networkIndexEntityNode;
			}
		}
		#endif
	}

	return primaryNetworkIndexNodeDefiningInstance;
}


#ifdef GIA_SUPPORT_NLC_INTEGRATION
NLCsentence* getFirstNLCsentenceInList()
{
	return firstNLCsentenceInListLocal;
}
void setFirstNLCsentenceInList(NLCsentence* firstNLCsentenceInListNew)
{
	firstNLCsentenceInListLocal = firstNLCsentenceInListNew;
}
bool checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContext(GIAentityNode* indefiniteEntity, GIAentityNode* definiteEntity, int* indentationDifferenceFound)
{
	bool foundIndefiniteEntity = false;

	if(indefiniteEntity->sentenceIndexTemp < definiteEntity->sentenceIndexTemp)
	{
		NLCsentence* currentNLCsentenceInList = firstNLCsentenceInListLocal;
		bool foundIndefiniteEntitySentence = false;
		while((currentNLCsentenceInList->next != NULL) && !foundIndefiniteEntitySentence)
		{
			if(currentNLCsentenceInList->sentenceIndex == indefiniteEntity->sentenceIndexTemp)
			{
			       foundIndefiniteEntitySentence = true;
			}
			else
			{
			       currentNLCsentenceInList = currentNLCsentenceInList->next;
			}
		}
		NLCsentence* indefiniteEntityNLCsentenceInList = currentNLCsentenceInList;
		
		if(foundIndefiniteEntitySentence)
		{
			bool foundDefiniteEntitySentence = false;
			int minimumIndentationBetweenIndefiniteAndIndefiniteEntitySentence = currentNLCsentenceInList->indentation;
			while((currentNLCsentenceInList->next != NULL) && !foundDefiniteEntitySentence)
			{
				if(currentNLCsentenceInList->indentation < minimumIndentationBetweenIndefiniteAndIndefiniteEntitySentence)
				{
				       minimumIndentationBetweenIndefiniteAndIndefiniteEntitySentence = currentNLCsentenceInList->indentation;
				}

				if(currentNLCsentenceInList->sentenceIndex == definiteEntity->sentenceIndexTemp)
				{
				       foundDefiniteEntitySentence = true;
				}
				else
				{
				       currentNLCsentenceInList = currentNLCsentenceInList->next;
				}
			}
			NLCsentence* definiteEntityNLCsentenceInList = currentNLCsentenceInList;

			#ifdef GIA_DEBUG
			cout << "definiteEntity = " << definiteEntity->entityName << endl;
			cout << "indefiniteEntity = " << indefiniteEntity->entityName << endl;
			#endif
			
			if(foundDefiniteEntitySentence)
			{
				if(minimumIndentationBetweenIndefiniteAndIndefiniteEntitySentence < indefiniteEntityNLCsentenceInList->indentation)
				{
				       #ifdef GIA_DEBUG
				       //cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContext{}: no reference found" << endl;
				       #endif
				}
				else
				{
				       #ifdef GIA_DEBUG
				       //cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContext{}: entity declared in this function" << endl;
				       #endif
				       foundIndefiniteEntity = true;
				       *indentationDifferenceFound = definiteEntityNLCsentenceInList->indentation - indefiniteEntityNLCsentenceInList->indentation;
				}
			}
			else
			{
			       #ifdef GIA_DEBUG
			       //cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContext{} error: !foundDefiniteEntitySentence" << endl;
			       #endif
			}
		}
		else
		{
			#ifdef GIA_DEBUG
			//cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContext{} error: !foundIndefiniteEntitySentence" << endl;
			#endif
		}
	}

	return foundIndefiniteEntity;
}
#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
bool checkIfSentenceIsMathTextParsablePhrase(GIAsentence* currentSentenceInList)
{
	bool sentenceIsMathTextParsablePhrase = false;
	NLCsentence* currentNLCsentenceInList = firstNLCsentenceInListLocal;
	while(currentNLCsentenceInList->next != NULL)
	{
		if(currentNLCsentenceInList->sentenceIndex == currentSentenceInList->sentenceIndex)
		{
			if(currentNLCsentenceInList->mathTextNLPparsablePhraseIndex != INT_DEFAULT_VALUE)
			{
				sentenceIsMathTextParsablePhrase = true;
			}
		}
		currentNLCsentenceInList = currentNLCsentenceInList->next;
	}
	return sentenceIsMathTextParsablePhrase;
}
#endif
#endif


#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
#ifdef GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES

GIAentityNode* createNewNonspecificConcept(GIAentityNode* networkIndexEntity)
{
	GIAentityNode* nonspecificConcept = addInstanceToInstanceDefinition(networkIndexEntity, GIA_ENTITY_TYPE_TYPE_CONCEPT);		//or addInstance{}
	#ifdef GIA_SET_ENTITY_ENTITY_AND_SENTENCE_INDICIES_NORMALLY
	//this enables GIA drawing of concept
	nonspecificConcept->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added concept
	nonspecificConcept->sentenceIndexTemp = networkIndexEntity->sentenceIndexTemp;
	#endif
	return nonspecificConcept;
}

GIAentityNode* getNonspecificConceptEntityFromNetworkIndex(GIAentityNode* networkIndexEntity)
{
	GIAentityNode* nonspecificConceptEntity = NULL;
	for(vector<GIAentityConnection*>::iterator iter = networkIndexEntity->associatedInstanceNodeList->begin(); iter < networkIndexEntity->associatedInstanceNodeList->end(); iter++)
	{
		GIAentityNode* conceptEntity = (*iter)->entity;
		if(isNonspecificConceptEntity(conceptEntity))
		{
			nonspecificConceptEntity = conceptEntity;
		}
	}

	return nonspecificConceptEntity;
}

//preconditions: dream mode has already been excuted (all instances have been connected to their non-specific concept entity)
GIAentityNode* getNonspecificConceptEntityFromInstance(GIAentityNode* instanceEntity)
{
	GIAentityNode* nonspecificConceptEntity = NULL;
	for(vector<GIAentityConnection*>::iterator iter = instanceEntity->entityNodeDefinitionList->begin(); iter < instanceEntity->entityNodeDefinitionList->end(); iter++)
	{
		GIAentityNode* conceptEntity = (*iter)->entity;
		if(isNonspecificConceptEntity(conceptEntity))
		{
			nonspecificConceptEntity = conceptEntity;
		}
	}

	return nonspecificConceptEntity;
}

bool isNonspecificConceptEntity(GIAentityNode* entity)
{
	bool nonspecificConcept = false;
	if(entity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
	{
		//cout << "concept found" << endl;

		//now verify that the concept is a specific networkIndex; ie, it has no sameReferenceSet properties/conditions
		//eg accept: Dogs are fat. Dogs are happy.
		//eg reject: Blue Dogs are fat. Blue Dogs are happy.
		nonspecificConcept = true;
		for(vector<GIAentityConnection*>::iterator propertyNodeListIterator = entity->propertyNodeList->begin(); propertyNodeListIterator < entity->propertyNodeList->end(); propertyNodeListIterator++)
		{
			GIAentityConnection* propertyConnection = (*propertyNodeListIterator);
			if(propertyConnection->sameReferenceSet)
			{
				nonspecificConcept = false;
			}
		}
		for(vector<GIAentityConnection*>::iterator conditionNodeListIterator = entity->conditionNodeList->begin(); conditionNodeListIterator < entity->conditionNodeList->end(); conditionNodeListIterator++)
		{
			GIAentityConnection* conditionConnection = (*conditionNodeListIterator);
			if(conditionConnection->sameReferenceSet)
			{
				nonspecificConcept = false;
			}
		}
		if(nonspecificConcept)
		{
			//cout << "nonspecificConcept found" << endl;
		}
	}
	return nonspecificConcept;
}

#endif
#endif

