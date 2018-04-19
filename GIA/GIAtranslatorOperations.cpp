/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslatorOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f10c 19-April-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAtranslatorOperations.hpp"


#ifdef GIA_NLC_INTEGRATION
NLCpreprocessorSentence* firstNLCsentenceInListLocalGIA;
#endif



static bool foundComparisonVariable;
static GIAentityNode* comparisonVariableNode;




GIAtranslatorVariablesClass::GIAtranslatorVariablesClass(void)
{
	//parsedSentences = false;	//NOT USED; designed to distingush between when parsing a specific sentence and pre/post processing stage (ie whether 

	//nlp variables;
	isQuery = false;
	NLPrelexCompatibilityMode = false;
	maxNumberSentences = 0;

	//nlp/sentence variables;
	//NLPexeFolderArray = NULL;
	NLPfeatureParser = GIA_NLP_FEATURE_PARSER_FOR_INPUT_TEXT_DEFAULT;
	NLPdependencyRelationsParser = GIA_NLP_RELATIONS_PARSER_FOR_INPUT_TEXT_DEFAULT;
	NLPdependencyRelationsType = dependencyRelationsTypes[NLPdependencyRelationsParser];
	NLPassumePreCollapsedStanfordRelations = false;
	#ifdef GIA_NLP_CLIENT_SERVER
	NLPclient = false;
	#endif
	parseGIA2file = false;
	linkPreestablishedReferencesGIA = false;
	
	//network variables;
	//entityNodesActiveListNetworkIndexes = NULL;
	//entityNodesActiveListComplete = NULL;
	currentEntityNodeIDInNetworkIndexEntityNodesList = 0;
	currentEntityNodeIDInCompleteList = 0;
	//entityNodesActiveListSentences = NULL;
	//timeConditionNodesActiveList;

	firstSentenceInList = NULL;
	
	//temporary sentence parsing variables (!parsedSentences only):
	saveNetwork = false;
	//GIAentityNodeArrayFilled = NULL;
	//GIAnetworkIndexNodeArray = NULL;
	//GIAentityNodeArray = NULL;
	//GIAfeatureTempEntityNodeArray = NULL;
	currentSentenceInList = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
	currentPreprocessorSentenceInList = NULL;
	#endif
	
	//sentenceNetworkIndexEntityNodesList = NULL;
	//sentenceTimeConditionNodesList = NULL;
	
	//featureArrayTemp = NULL;
	
	sentenceIndex = INT_DEFAULT_VALUE;

	#ifdef GIA_PREPROCESSOR
	#ifdef USE_NLC
	firstNLCpreprepreprocessorSentenceInList = NULL;
	#endif
	firstGIApreprocessorSentenceInList = NULL;
	#endif
	#ifdef USE_NLCI
	nlcGeneratedCode = "";	
	#endif
	#ifdef USE_GIAI
	giaQueryAnswer = "";
	#endif	
	
	#ifdef GIA_NEURAL_NETWORK
	firstInputNeuronInNetwork = NULL;
	#endif
}
GIAtranslatorVariablesClass::~GIAtranslatorVariablesClass(void)
{
}


#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
bool GIAtranslatorOperationsClass::connectionIsAlias(const GIAentityConnection* connection)
{
	bool isAlias = false;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(connection->entity->isAlias)
	#else
	if(connection->isAlias)
	#endif
	{
		isAlias = true;
	}
	return isAlias;
}
#endif


bool GIAtranslatorOperationsClass::connectionTargetIsRelationship(GIAentityConnection* connection)
{	
	bool result = false;
	GIAentityNode* entity = connection->entity;
	if(GIAentityNodeClass.entityIsRelationship(entity))
	{
		result = true;
	}
	return result;
}
/*
bool GIAtranslatorOperationsClass::connectionIsRelationship(GIAentityConnection* connection)
{	
	bool result = false;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* entity = connection->entity;
	if(GIAentityNodeClass.entityIsRelationship(entity))
	{
		result = true;
	}
	#else
	if(entityVectorConnectionIsPropertyOrDefinitionArray[connection->connectionType])
	{
		result = true;
	}
	else
	{
		GIAentityNode* entity = connection->entity;
		if(GIAentityNodeClass.entityIsRelationship(entity))
		{
			result = true;
		}
	}	
	#endif
	return result;
}
*/


int GIAtranslatorOperationsClass::generateConnectionType(int relationshipEntityType)
{
	int connectionType = entityTypesCrossReferenceEntityVectorConnectionArray[relationshipEntityType];
	if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)
	{
		cerr << "generateConnectionType error{}: (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)" << endl;
		exit(EXIT_ERROR);
	}
	return connectionType;
}

int GIAtranslatorOperationsClass::generateConnectionTypeReverse(int relationshipEntityType)
{
	int connectionType = entityTypesCrossReferenceEntityVectorConnectionReverseArray[relationshipEntityType];
	if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)
	{
		cerr << "generateConnectionType error{}: (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)" << endl;
		exit(EXIT_ERROR);
	}
	return connectionType;
}

int GIAtranslatorOperationsClass::generateInvertedConnectionType(GIAentityNode* entity, const int connectionType)
{
	int connectionTypeInverted = inverseVectorConnectionsArray[connectionType];
	if(connectionTypeInverted == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)
	{
		if(entityTypesIsRelationshipArray[entity->entityType])
		{
			if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT)
			{
				connectionTypeInverted = entityTypesCrossReferenceEntityVectorConnectionArray[entity->entityType];
			}
			else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT)
			{
				connectionTypeInverted = entityTypesCrossReferenceEntityVectorConnectionReverseArray[entity->entityType];
			}
		}
		else
		{
			cerr << "GIAtranslatorOperationsClass::generateInvertedConnectionType{} error: (connectionTypeInverted == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN) && !(entityTypesIsRelationshipArray[entity->entityType])" << endl;
			exit(EXIT_ERROR);
		}
	}
	return connectionTypeInverted;
}




GIAentityNode* GIAtranslatorOperationsClass::getPropertyRelationshipTargetEntity(GIAentityNode* relationshipEntity)
{
	GIAentityNode* relationshipObjectEntity = NULL;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	relationshipObjectEntity = getRelationshipObjectEntity(relationshipEntity);
	#else
	relationshipObjectEntity = relationshipEntity;
	#endif
	return relationshipObjectEntity;
}
GIAentityNode* GIAtranslatorOperationsClass::getPropertyRelationshipSourceEntity(GIAentityNode* relationshipEntity)
{
	GIAentityNode* relationshipSubjectEntity = NULL;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	relationshipSubjectEntity = getRelationshipSubjectEntity(relationshipEntity);
	#else
	relationshipSubjectEntity = relationshipEntity;
	#endif
	return relationshipSubjectEntity;
}
GIAentityNode* GIAtranslatorOperationsClass::getDefinitionRelationshipTargetEntity(GIAentityNode* relationshipEntity)
{
	GIAentityNode* relationshipObjectEntity = NULL;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	relationshipObjectEntity = getRelationshipObjectEntity(relationshipEntity);
	#else
	relationshipObjectEntity = relationshipEntity;
	#endif
	return relationshipObjectEntity;
}
GIAentityNode* GIAtranslatorOperationsClass::getDefinitionRelationshipSourceEntity(GIAentityNode* relationshipEntity)
{
	GIAentityNode* relationshipSubjectEntity = NULL;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	relationshipSubjectEntity = getRelationshipSubjectEntity(relationshipEntity);
	#else
	relationshipSubjectEntity = relationshipEntity;
	#endif
	return relationshipSubjectEntity;
}



GIAentityNode* GIAtranslatorOperationsClass::getDefinitionRelationshipObjectEntity(GIAentityConnection* relationshipConnection)
{
	return getRelationshipObjectEntity(relationshipConnection);
}
//NB if !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAentityConnection* relationshipConnectionReverse needn't be reverse
GIAentityNode* GIAtranslatorOperationsClass::getDefinitionRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse)
{
	return getRelationshipSubjectEntity(relationshipConnectionReverse);
}
GIAentityNode* GIAtranslatorOperationsClass::getPropertyRelationshipObjectEntity(GIAentityConnection* relationshipConnection)
{
	return getRelationshipObjectEntity(relationshipConnection);
}
//NB if !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAentityConnection* relationshipConnectionReverse needn't be reverse
GIAentityNode* GIAtranslatorOperationsClass::getPropertyRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse)
{
	return getRelationshipSubjectEntity(relationshipConnectionReverse);
}


GIAentityNode* GIAtranslatorOperationsClass::getRelationshipObjectEntity(GIAentityConnection* relationshipConnection)
{
	GIAentityNode* relationshipObjectEntity = NULL;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	relationshipObjectEntity = getRelationshipObjectEntity(relationshipEntity);
	#else
	if(entityVectorConnectionIsRelationshipPropertyOrDefinitionForwardArray[relationshipConnection->connectionType])
	{
		relationshipObjectEntity = relationshipConnection->entity;
	}
	else if(entityVectorConnectionIsRelationshipPropertyOrDefinitionReverseArray[relationshipConnection->connectionType])
	{
		relationshipObjectEntity = relationshipConnection->entityOrigin;
	}	
	else
	{
		GIAentityNode* relationshipEntity = relationshipConnection->entity;
		relationshipObjectEntity = getRelationshipObjectEntity(relationshipEntity);	
	}
	#endif
	return relationshipObjectEntity;
}
GIAentityNode* GIAtranslatorOperationsClass::getRelationshipObjectEntity(GIAentityNode* relationshipEntity)
{
	GIAentityNode* objectEntity = getRelationshipObjectEntityConnection(relationshipEntity)->entity;
	return objectEntity;
}
GIAentityConnection* GIAtranslatorOperationsClass::getRelationshipObjectEntityConnection(GIAentityNode* relationshipEntity)
{
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType])
	{
		cerr << "!GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAtranslatorOperationsClass::getRelationshipObjectEntityConnection error{}: entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType]" << endl;
		exit(EXIT_ERROR);		
	}
	#endif
	
	if(relationshipEntity->relationshipObjectEntity->empty())
	{
		//DEBUG only; note this should never be the case (if a property/definition relationship source is defined, then its target should be defined)
		cerr << "GIAtranslatorOperationsClass::getRelationshipObjectEntityConnection error{}: relationshipEntity->relationshipObjectEntity->empty()" << endl;
		exit(EXIT_ERROR);
	}
	GIAentityConnection* objectEntityConnection = ((relationshipEntity->relationshipObjectEntity)->back());
	return objectEntityConnection;
}

//NB if !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAentityConnection* relationshipConnectionReverse needn't be reverse
GIAentityNode* GIAtranslatorOperationsClass::getRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse)
{
	GIAentityNode* relationshipSubjectEntity = NULL;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnectionReverse->entity;
	relationshipSubjectEntity = getRelationshipSubjectEntity(relationshipEntity);
	#else
	if(entityVectorConnectionIsRelationshipPropertyOrDefinitionReverseArray[relationshipConnectionReverse->connectionType])
	{
		relationshipSubjectEntity = relationshipConnectionReverse->entity;
	}
	else if(entityVectorConnectionIsRelationshipPropertyOrDefinitionForwardArray[relationshipConnectionReverse->connectionType])
	{
		relationshipSubjectEntity = relationshipConnectionReverse->entityOrigin;
	}
	else
	{
		GIAentityNode* relationshipEntity = relationshipConnectionReverse->entity;
		relationshipSubjectEntity = getRelationshipSubjectEntity(relationshipEntity);	
	}
	#endif
	return relationshipSubjectEntity;
}
GIAentityNode* GIAtranslatorOperationsClass::getRelationshipSubjectEntity(GIAentityNode* relationshipEntity)
{
	GIAentityNode* subjectEntity = getRelationshipSubjectEntityConnection(relationshipEntity)->entity;
	return subjectEntity;
}
GIAentityConnection* GIAtranslatorOperationsClass::getRelationshipSubjectEntityConnection(GIAentityNode* relationshipEntity)
{
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType])
	{
		cerr << "!GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAtranslatorOperationsClass::getRelationshipSubjectEntityConnection error{}: entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType]" << endl;
		exit(EXIT_ERROR);		
	}
	#endif
	
	if(relationshipEntity->relationshipSubjectEntity->empty())
	{
		//DEBUG only; note this should never be the case (if a property/definition relationship source is defined, then its target should be defined)
		cerr << "GIAtranslatorOperationsClass::getRelationshipSubjectEntityConnection error{}: relationshipEntity->relationshipSubjectEntity->empty()" << endl;
		exit(EXIT_ERROR);
	}
	GIAentityConnection* subjectEntityConnection = ((relationshipEntity->relationshipSubjectEntity)->back());
	return subjectEntityConnection;
}

















#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS

bool GIAtranslatorOperationsClass::connectPropertyToEntity(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToEntity(propertyRelationshipSubjectEntity, propertyRelationshipObjectEntity, propertyRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_PROPERTY, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectDefinitionToEntity(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToEntity(definitionRelationshipSubjectEntity, definitionRelationshipObjectEntity, definitionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables);
}

#ifdef GIA_ALIASES
#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
bool GIAtranslatorOperationsClass::connectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = connectDefinitionToEntity(definitionRelationshipSubjectEntity, definitionRelationshipObjectEntity, definitionRelationshipEntity, sameReferenceSet, translatorVariables);
	definitionRelationshipEntity->isAlias = true;
	return result;
}
#endif
#endif


bool GIAtranslatorOperationsClass::connectBeingDefinitionToEntity(GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{	
	return connectRelationshipToObject(definitionRelationshipObjectEntity, definitionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectHavingPropertyToEntity(GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToObject(propertyRelationshipObjectEntity, propertyRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables);
}

#else

bool GIAtranslatorOperationsClass::connectDirectPropertyToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* auxHaveEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectDirectRelationshipToEntity(relationshipSubjectEntity, relationshipObjectEntity, auxHaveEntity, sameReferenceSet, GIA_ENTITY_TYPE_PROPERTY, translatorVariables, false);
}

bool GIAtranslatorOperationsClass::connectDirectDefinitionToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* auxHaveEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectDirectRelationshipToEntity(relationshipSubjectEntity, relationshipObjectEntity, auxHaveEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables, false);
}

bool GIAtranslatorOperationsClass::connectDirectPropertyToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectDirectRelationshipToEntity(relationshipSubjectEntity, relationshipObjectEntity, NULL, sameReferenceSet, GIA_ENTITY_TYPE_PROPERTY, translatorVariables, false);
}

bool GIAtranslatorOperationsClass::connectDirectDefinitionToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectDirectRelationshipToEntity(relationshipSubjectEntity, relationshipObjectEntity, NULL, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables, false);
}

#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
bool GIAtranslatorOperationsClass::connectDirectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectDirectRelationshipToEntity(definitionRelationshipSubjectEntity, definitionRelationshipObjectEntity, NULL, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables, true);
}
#endif

bool GIAtranslatorOperationsClass::connectDirectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* auxHaveEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables, bool markConnectionAsAlias)
{
	bool result = true;
	
	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipSubjectEntity->disabled))
	{
	if(!(relationshipObjectEntity->disabled))
	{
	#endif
		
		#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
		setRelationshipObjectToSubstanceIfNecessary(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType);
		#endif
	
		GIAentityConnection* connection1 = NULL;
		GIAentityConnection* connection2 = NULL;
		
		if(relationshipEntityType == GIA_ENTITY_TYPE_PROPERTY)
		{
			connection1 = writeVectorConnection(relationshipSubjectEntity, relationshipObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, sameReferenceSet, translatorVariables);
			connection2 = writeVectorConnection(relationshipObjectEntity, relationshipSubjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, sameReferenceSet, translatorVariables);
		}
		else if(relationshipEntityType == GIA_ENTITY_TYPE_DEFINITION)
		{
			connection1 = writeVectorConnection(relationshipSubjectEntity, relationshipObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, sameReferenceSet, translatorVariables);
			connection2 = writeVectorConnection(relationshipObjectEntity, relationshipSubjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, sameReferenceSet, translatorVariables);
			
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			if(markConnectionAsAlias)
			{
				connection1->isAlias = true;
				connection2->isAlias = true;		
			}
			#endif
		}
		else
		{
			cerr << "GIAtranslatorOperationsClass::connectDirectRelationshipToEntity{} error: illegal relationshipEntityType; relationshipEntityType = " << relationshipEntityType << endl;
			exit(EXIT_ERROR);
		}
		
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
				#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION
				if(relationshipEntityType == GIA_ENTITY_TYPE_PROPERTY)
				{
					connection1->possessionAuxiliaryHave = true;
					connection2->possessionAuxiliaryHave = true;
				}
				#endif
			}		
		}
		#endif

	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif
	
	return result;
}

#endif


	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
bool GIAtranslatorOperationsClass::connectActionToEntity(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToEntity(actionRelationshipSubjectEntity, actionRelationshipObjectEntity, actionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_ACTION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectConditionToEntity(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = connectRelationshipToEntity(conditionRelationshipSubjectEntity, conditionRelationshipObjectEntity, conditionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
	
	#ifdef GIA_PREPROCESSOR_WORD_DETECT_PREPOSITION_TYPE
	GIApreprocessorWordIdentification.identifyConditionType(conditionRelationshipEntity);
	#endif
	
	return result;
}


bool GIAtranslatorOperationsClass::connectActionToSubject(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToSubject(actionRelationshipSubjectEntity, actionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_ACTION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectActionToObject(GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToObject(actionRelationshipObjectEntity, actionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_ACTION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectConditionToSubject(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToSubject(conditionRelationshipSubjectEntity, conditionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectConditionToObject(GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToObject(conditionRelationshipObjectEntity, conditionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectDefinitionToSubject(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToSubject(definitionRelationshipSubjectEntity, definitionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectDefinitionToObject(GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToObject(definitionRelationshipObjectEntity, definitionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectPropertyToSubject(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToSubject(propertyRelationshipSubjectEntity, propertyRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_PROPERTY, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectPropertyToObject(GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToObject(propertyRelationshipObjectEntity, propertyRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_PROPERTY, translatorVariables);
}






bool GIAtranslatorOperationsClass::connectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipSubjectEntity->disabled))
	{
	if(!(relationshipObjectEntity->disabled))
	{
	if(!(relationshipEntity->disabled))
	{
	#endif
		
		relationshipEntity->entityType = relationshipEntityType;
		
		#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
		setRelationshipObjectToSubstanceIfNecessary(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType);
		#endif
	
		//configure entity node containing this substance
		connectRelationshipInstanceToSubject(relationshipSubjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);
		connectRelationshipInstanceToObject(relationshipObjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);


	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	}
	#endif
	
	return result;
}

bool GIAtranslatorOperationsClass::connectRelationshipToSubject(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipSubjectEntity->disabled))
	{
	if(!(relationshipEntity->disabled))
	{
	#endif
		relationshipEntity->entityType = relationshipEntityType;
		
		connectRelationshipInstanceToSubject(relationshipSubjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);

	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif
	
	return result;
}

bool GIAtranslatorOperationsClass::connectRelationshipToObject(GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipObjectEntity->disabled))
	{
	if(!(relationshipEntity->disabled))
	{
	#endif
		relationshipEntity->entityType = relationshipEntityType;
		
		connectRelationshipInstanceToObject(relationshipObjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);

	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif
	
	return result;
}


GIAentityNode* GIAtranslatorOperationsClass::useExistingRelationshipIfExistant(GIAentityNode* newOrExistingRelationship, GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, const int relationshipEntityType)
{
	if((relationshipEntityType == GIA_ENTITY_TYPE_ACTION) || (relationshipEntityType == GIA_ENTITY_TYPE_CONDITION))
	{
		int connectionType = generateConnectionType(relationshipEntityType);
		
		//see if relevant link already exists between the two nodes, and if so use that
		GIAentityConnection* connectionFound = NULL;
		if(findEntityNodeNameInVector(relationshipSubjectEntity, &(relationshipEntity->entityName), connectionType, &connectionFound))
		{
			GIAentityNode* currentRelationshipNodeInList = connectionFound->entity;
			GIAentityConnection* connectionFound2 = NULL;
			if(findEntityNodePointerInVector(currentRelationshipNodeInList, relationshipObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, &connectionFound2))
			{
				if(newOrExistingRelationship != currentRelationshipNodeInList)
				{
					newOrExistingRelationship->disabled = true;
					newOrExistingRelationship = currentRelationshipNodeInList;
				}
			}
		}
	}
	
	return newOrExistingRelationship;
}

#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
void GIAtranslatorOperationsClass::setRelationshipObjectToSubstanceIfNecessary(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType)
{
	if((relationshipEntityType == GIA_ENTITY_TYPE_PROPERTY) || (relationshipEntityType == GIA_ENTITY_TYPE_CONDITION))
	{
		if(relationshipObjectEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)
		{
			if(!(relationshipSubjectEntity->entityType == GIA_ENTITY_TYPE_CONCEPT))
			{
				//cout << "connectConditionToEntity{} warning: condition object was declared concept while condition subject was not declared concept" << endl;
				relationshipObjectEntity->entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//CHECKTHIS: will this ever be a quality?
			}
		}
	}
}
#endif

void GIAtranslatorOperationsClass::connectRelationshipInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingRelationship, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	//configure relationship subject entity node
	int connectionType = generateConnectionType(relationshipEntityType);
	connectEntities(subjectEntity, newOrExistingRelationship, connectionType, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, sameReferenceSet, translatorVariables);
}

void GIAtranslatorOperationsClass::connectRelationshipInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingRelationship, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	//configure relationship object entity node
	int connectionTypeReverse = generateConnectionTypeReverse(relationshipEntityType);
	connectEntities(objectEntity, newOrExistingRelationship, connectionTypeReverse, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, sameReferenceSet, translatorVariables);
}

void GIAtranslatorOperationsClass::connectEntities(GIAentityNode* entity1, GIAentityNode* entity2, const int connectionType, const int connectionTypeInverse, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	writeVectorConnection(entity1, entity2, connectionType, sameReferenceSet, translatorVariables);
	writeVectorConnection(entity2, entity1, connectionTypeInverse, sameReferenceSet, translatorVariables);
}








#ifndef GIA_SYN_REL_TRANSLATOR_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
//this is no longer required with redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition, etc..
bool GIAtranslatorOperationsClass::isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(const GIArelation* currentRelationInList, GIAentityNode* GIAentityNodeArray[], int relationGovernorIndex, const int NLPdependencyRelationsType)
{
	bool result = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES_ADVANCED
		if((currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_ADJECTIVE_ADVMOD) && (GIAentityNodeArray[relationGovernorIndex]->entityName == RELATION_ENTITY_BE))
		{//added condition Fri 27 Jan - remove 'be' node artefacts
			disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[relationGovernorIndex]);
			result = false;
		}
		#endif
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	}
	#endif

	return result;
}

//this is no longer required with redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition, etc..
bool GIAtranslatorOperationsClass::isAdjectiveNotConnectedToObjectOrSubject(const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInList, const int NLPdependencyRelationsType)
{
	bool passed2 = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_ADJECTIVE_ADVMOD)
		{
			const GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
 			while(currentRelationInList3->next != NULL)
			{
				bool partnerTypeRequiredFound = false;

				for(int i=0; i<GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList3->relationType == relationTypeSubjectNameArray[i])
					{
						partnerTypeRequiredFound = true;
					}
				}
				for(int i=0; i<GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
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



GIAentityNode* GIAtranslatorOperationsClass::addInstanceToInstanceDefinition(GIAentityNode* entity, int instanceType, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* newOrExistingInstance = entity;

	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(entity->disabled))
	{
	#endif
		if(entity->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
		{
			newOrExistingInstance = addInstance(entity, instanceType, translatorVariables);
		}
		else
		{
			newOrExistingInstance = entity;
			if(entityTypesAutomaticallyUpgradeUponInstanceSelectionArray[instanceType])
			{
				//upgrade substance to action/condition/concept etc
				newOrExistingInstance->entityType = instanceType;	
			}
		}

	#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	#endif

	return newOrExistingInstance;
}

GIAentityNode* GIAtranslatorOperationsClass::addInstance(GIAentityNode* entity, const int instanceType, GIAtranslatorVariablesClass* translatorVariables)
{
	//configure substance node
	GIAentityNode* newInstance = new GIAentityNode();
	#ifdef GIA_DATABASE
	newInstance->added = true;
	#endif

	newInstance->entityType = instanceType;

	addInstanceEntityNodeToActiveLists(newInstance, translatorVariables);
	
	newInstance->entityName = entity->entityName;
	#ifdef GIA_WORD_ORIG
	newInstance->wordOrig = entity->wordOrig;
	#endif
	newInstance->idInstance = determineNextIdInstance(entity);

	entity->hasAssociatedInstance = true;
	entity->hasAssociatedInstanceTemp = true;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	/*//original code from addSubstance: what was this used for?
	#ifdef GIA_SPECIFIC_CONCEPTS
	if(entity->entityType == GIA_ENTITY_TYPE_CONCEPT)
	{
		newInstance->entityType = GIA_ENTITY_TYPE_CONCEPT;
		//entityType->entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//?
	}
	#endif
	*/

	forwardInfoToNewSubstance(entity, newInstance);
	
	writeVectorConnection(newInstance, entity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN, translatorVariables);
	writeVectorConnection(entity, newInstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN, translatorVariables);

	return newInstance;
}




void GIAtranslatorOperationsClass::forwardInfoToNewSubstance(GIAentityNode* entity, GIAentityNode* newSubstance)
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
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_GENERIC_ENTITY_INTERPRETATION
	newSubstance->grammaticalDefiniteTemp = entity->grammaticalDefiniteTemp;	//must forward grammatical info for GIAsynRelTranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalIndefinitePluralTemp = entity->grammaticalIndefinitePluralTemp;	//must forward grammatical info for GIAsynRelTranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalProperNounTemp = entity->grammaticalProperNounTemp;	//must forward grammatical info for GIAsynRelTranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalGenderTemp = entity->grammaticalGenderTemp;		//not currently used
	newSubstance->grammaticalPronounTemp = entity->grammaticalPronounTemp;		//must forward grammatical info for GIAsynRelTranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->mustSetIsConceptBasedOnApposRelation = entity->mustSetIsConceptBasedOnApposRelation;
	#endif
	#ifdef GIA_PREDETERMINERS
	newSubstance->grammaticalPredeterminerTemp = entity->grammaticalPredeterminerTemp;
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
	newSubstance->grammaticalPredeterminerTempSentenceArray.insert(make_pair(entity->sentenceIndexTemp, entity->grammaticalPredeterminerTemp));	//FUTURE GIA: this should copy entity->grammaticalPredeterminerTempSentenceArray to newSubstance->grammaticalPredeterminerTempSentenceArray
	#endif
	#endif
	#ifdef GIA_STANFORD_CORENLP
	newSubstance->NormalizedNERtemp = entity->NormalizedNERtemp;	//always required (not just for time info / time condition related)
	#endif
	newSubstance->NERTemp = entity->NERTemp;

	#ifdef GIA_ALIASES
	if(entity->isName)
	{
		entity->isName = false;	//added 2n5a
		newSubstance->isName = true;
	}
	#endif

	#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	if(entity->isQuery)
	{
		entity->isQuery = false;
		newSubstance->isQuery = true;

		//this propogation was only defined for substance instances [generalised 1t1m 24 July 2013]:
		#ifdef GIA_1S8D_LOW_PRI_RELEX_UPDATE_CHECK_THAT_IT_DOESNT_BREAK_STANFORD_OPTIMISATION_APPLY_FIX_TO_IS_NAME_QUERY_PROPOGATION
		#ifdef GIA_ALIASES
		if(entity->isNameQuery)
		{
			entity->isNameQuery = false;
			newSubstance->isNameQuery = true;
		}
		#endif
		#endif

		#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
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

	newSubstance->entityIndexTemp = entity->entityIndexTemp;
	newSubstance->sentenceIndexTemp = entity->sentenceIndexTemp;

	#ifdef GIA_TRANSLATOR_NUMBER_OF
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
	
	#ifdef GIA_TRANSLATOR_EXPLETIVES
	if(entity->isExpletive)
	{
		entity->isExpletive = false;
		newSubstance->isExpletive = true;
	}	
	#endif

	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(entity->isArtificialAuxiliary)
	{
		entity->isArtificialAuxiliary = false;
		newSubstance->isArtificialAuxiliary = true;
	}
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
	newSubstance->semanticRelationEntityIsReferenced = entity->semanticRelationEntityIsReferenced;	//shouldnt be required
	newSubstance->semanticRelationPreprocessorEntityIndex = entity->semanticRelationPreprocessorEntityIndex;
	#ifndef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	newSubstance->semanticRelationWordPOStypeInferred = entity->semanticRelationWordPOStypeInferred;
	#endif
	newSubstance->semanticRelationEntityIsReferenced = entity->semanticRelationEntityIsReferenced;	
	
	#endif

}


/*
#ifdef GIA_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, int tense, vector<GIAtimeConditionNode*>* timeConditionNodesActiveList, vector<long>* timeConditionNumbersActiveList)
#else
void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, int tense)
#endif
*/
void GIAtranslatorOperationsClass::addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, const int tense, const bool isProgressive)
{
	substanceNode->conditionType = CONDITION_NODE_TYPE_TIME;

	/*
	#ifdef GIA_TIME_NODE_INDEXING
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













//assumes prepositions have previously been converted to stanford prep_preposition format during preprocessor (for robustness)
string GIAtranslatorOperationsClass::convertPrepositionToRelex(const string* preposition, bool* prepositionFound)
{
	*prepositionFound = false;
	string relexPreposition = *preposition;
	for(int i=0; i<GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_GIA_SYN_REL_TRANSLATOR_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES; i++)
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




bool GIAtranslatorOperationsClass::getFoundComparisonVariable()
{
	return foundComparisonVariable;
}
GIAentityNode* GIAtranslatorOperationsClass::getComparisonVariableNode()
{
	return comparisonVariableNode;
}
void GIAtranslatorOperationsClass::setFoundComparisonVariable(const bool value)
{
	foundComparisonVariable = value;
}
void GIAtranslatorOperationsClass::setComparisonVariableNode(GIAentityNode* newComparisonVariableNode)
{
	comparisonVariableNode = newComparisonVariableNode;
}




void GIAtranslatorOperationsClass::applyNetworkIndexEntityAlreadyExistsFunction(GIAentityNode* entity, const bool entityAlreadyExistant, const bool tempEntityEnabled)
{
	if(entityAlreadyExistant)
	{
		if(tempEntityEnabled)
		{
			entity->disabled = false;
		}
	}
}

void GIAtranslatorOperationsClass::disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	if(!(entity->permanentNetworkIndex))
	{
		entity->disabled = true;
	}
	#endif
}

void GIAtranslatorOperationsClass::disableEntity(GIAentityNode* entity)
{
	#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	entity->disabled = true;
	#endif
}


void GIAtranslatorOperationsClass::disableNetworkIndexEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entity);	//CHECKTHIS; only disable the networkIndex if it was created in the immediate context (eg sentence)

	if(entity->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the immediate context (eg sentence)
	//if(entity->instanceNodeList.size() >= 1)
	{
		GIAentityNode* associateInstance = (entity->instanceNodeList->back())->entity;
		associateInstance->disabled = true;	//and disable their associated instances (substance nodes)
	}
	#endif
}

//NB only disables the primary networkIndex node defining the instance
void GIAtranslatorOperationsClass::disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	entity->disabled = true;

	if(!(entity->instanceReverseNodeList->empty()))
	{
		//CHECKTHIS; only disable the networkIndex if it was created in the immediate context (eg sentence)
		GIAentityNode* networkIndexEntity = getPrimaryNetworkIndexNodeDefiningInstance(entity);

		disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(networkIndexEntity);
	}
	#endif
}

//NB only disables the primary networkIndex node defining the instance
void GIAtranslatorOperationsClass::disableInstanceAndNetworkIndexEntity(GIAentityNode* entity)
{
	entity->disabled = true;
	if(!(entity->instanceReverseNodeList->empty()))
	{
		GIAentityNode* networkIndexEntity = getPrimaryNetworkIndexNodeDefiningInstance(entity);
		networkIndexEntity->disabled = true;
	}
}



void GIAtranslatorOperationsClass::recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap)
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
void GIAtranslatorOperationsClass::recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap)
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
void GIAtranslatorOperationsClass::recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap)
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

void GIAtranslatorOperationsClass::convertRelexPOStypeToWordnetWordType(const string* relexPOStype, int* grammaticalWordTypeTemp, const bool grammaticallyStrict)
{
	*grammaticalWordTypeTemp = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOStypeArray[i] == *relexPOStype)
		{
			*grammaticalWordTypeTemp = featureRelexPOStypeCrossReferenceWordnetWordTypeArray[i];
		}
	}
}

void GIAtranslatorOperationsClass::convertStanfordPOStagToRelexPOStypeAndWordnetWordType(const string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp, const bool grammaticallyStrict)
{
	*relexPOStype = FEATURE_RELEX_POS_TYPE_WORD;
	
	string* featurePOStagMinimalArrayRef; 
	string* featurePOStagCrossReferenceRelexPOStypeArrayRef; 
	int featurePOStagMinimalArrayRefSize;
	if(grammaticallyStrict)
	{
		featurePOStagMinimalArrayRef = featurePOStagMinimalArrayGrammaticallyStrict;
		featurePOStagMinimalArrayRefSize = FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL_GRAMMATICALLY_STRICT;
		featurePOStagCrossReferenceRelexPOStypeArrayRef = featurePOStagCrossReferenceRelexPOStypeArrayGrammaticallyStrict;
	}
	else
	{
		featurePOStagMinimalArrayRef = featurePOStagMinimalArray;
		featurePOStagMinimalArrayRefSize = FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL;
		featurePOStagCrossReferenceRelexPOStypeArrayRef = featurePOStagCrossReferenceRelexPOStypeArray;
	}
	for(int i=0; i<featurePOStagMinimalArrayRefSize; i++)
	{
		if(featurePOStagMinimalArrayRef[i] == *POStag)
		{
			*relexPOStype = featurePOStagCrossReferenceRelexPOStypeArrayRef[i];
		}
	}

	convertRelexPOStypeToWordnetWordType(relexPOStype, grammaticalWordTypeTemp, grammaticallyStrict);


}

void GIAtranslatorOperationsClass::generateTempFeatureArray(GIAtranslatorVariablesClass* translatorVariables, vector<GIAfeature*>* featureArrayTemp)
{
	#ifndef GIA_SYN_REL_TRANSLATOR_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
	//required for locateAndAddAllFeatureTempEntities:generateTempFeatureArray
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		(*featureArrayTemp)[w] = NULL;	//initialise as NULL (required to prevent crashes during array access) - added 14 July 2013
	}
	#endif

	GIAfeature* currentFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		(*featureArrayTemp)[currentFeatureInList->entityIndex] = currentFeatureInList;
		currentFeatureInList = currentFeatureInList->next;
	}
}

bool GIAtranslatorOperationsClass::determineSameReferenceSetValue(bool defaultSameSetValueForRelation, const GIArelation* relation)
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


	return sameReferenceSet;
}


GIAentityNode* GIAtranslatorOperationsClass::addPropertyRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return addRelationshipArtificialToEntity(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, GIA_ENTITY_TYPE_PROPERTY, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES, translatorVariables);
}
GIAentityNode* GIAtranslatorOperationsClass::addDefinitionRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return addRelationshipArtificialToEntity(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS, translatorVariables);
}
GIAentityNode* GIAtranslatorOperationsClass::addConditionRelationshipArtificialToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const string conditionName, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return addRelationshipArtificialToEntity(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, conditionName, translatorVariables);
}
GIAentityNode* GIAtranslatorOperationsClass::addRelationshipArtificialToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
	GIAentityNode* relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType, relationshipEntityName, translatorVariables);
	#else
	GIAentityNode* relationshipEntity = NULL;
	#endif
	
	if(relationshipEntityType == GIA_ENTITY_TYPE_DEFINITION)
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		result = connectDefinitionToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariables);
		#else
		result = connectDirectDefinitionToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariables);	
		//NB function will return NULL	
		#endif
	}
	else if(relationshipEntityType == GIA_ENTITY_TYPE_PROPERTY)
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		result = connectPropertyToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariables);
		#else
		result = connectDirectPropertyToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariables);	
		//NB function will return NULL	
		#endif
	}
	else if(relationshipEntityType == GIA_ENTITY_TYPE_CONDITION)
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		result = connectConditionToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariables);
		#else
		relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType, relationshipEntityName, translatorVariables);
		result = connectConditionToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariables);		
		#endif
	}
	else
	{
		cerr << "GIAtranslatorOperationsClass::addRelationshipArtificialToEntity{} error: unsupported relationshipEntityType; relationshipEntityType = " << relationshipEntityType << endl;
		exit(EXIT_ERROR);
	}
	
	return relationshipEntity;
}


#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipObjectEntity, GIAtranslatorVariablesClass* translatorVariables)
{
	return findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(propertyRelationshipSubjectEntity, propertyRelationshipObjectEntity, GIA_ENTITY_TYPE_PROPERTY, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES, translatorVariables);
}
GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAtranslatorVariablesClass* translatorVariables)
{
	return findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(definitionRelationshipSubjectEntity, definitionRelationshipObjectEntity, GIA_ENTITY_TYPE_DEFINITION, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS, translatorVariables);
}


GIAentityNode* GIAtranslatorOperationsClass::addEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(GIAtranslatorVariablesClass* translatorVariables)	//not used
{
	return addEntityNodeByNameSimpleWrapperRelationshipArtificial(GIA_ENTITY_TYPE_PROPERTY, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES, translatorVariables);
}
GIAentityNode* GIAtranslatorOperationsClass::addEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(GIAtranslatorVariablesClass* translatorVariables)	//not used
{
	return addEntityNodeByNameSimpleWrapperRelationshipArtificial(GIA_ENTITY_TYPE_DEFINITION, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS, translatorVariables);
}
#endif

//CHECKTHIS;
GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialCondition(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipObjectEntity, string conditionName, GIAtranslatorVariablesClass* translatorVariables)
{
	return findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(propertyRelationshipSubjectEntity, propertyRelationshipObjectEntity, GIA_ENTITY_TYPE_CONDITION, conditionName, translatorVariables);
}


GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* relationshipEntity = NULL;
	if(!findExistingRelationshipInSentenceEntityArray(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType, relationshipEntityName, &relationshipEntity, translatorVariables))
	{
		relationshipEntity = addEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipEntityType, relationshipEntityName, translatorVariables);
	}

	/*
	#ifdef GIA_SEM_REL_TRANSLATOR
	#ifdef GIA_SEM_REL_TRANSLATOR_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex] = new GIAfeature();
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->word = inverseConditionName;
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
	#endif
	#endif
	*/
	
	return relationshipEntity;	
}

bool GIAtranslatorOperationsClass::findExistingRelationshipInSentenceEntityArray(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, GIAentityNode** relationshipEntity, GIAtranslatorVariablesClass* translatorVariables)
{
	//if !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS; this function can only be used for relationshipEntityType == GIA_ENTITY_TYPE_CONDITION
		
	//NB an artificial or non-artificial relationship may have been created in a previous sentence
	
	bool foundExistingRelationship = false;
	int connectionType = generateConnectionType(relationshipEntityType);
	int connectionTypeReverse = generateConnectionTypeReverse(relationshipEntityType);
	for(vector<GIAentityConnection*>::iterator connectionIter = relationshipSubjectEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != relationshipSubjectEntity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		GIAentityNode* relationshipEntityTemp = (*connectionIter)->entity; 
		
		if(getRelationshipObjectEntity((*connectionIter)) == relationshipObjectEntity)
		{
			#ifdef GIA_PREPROCESSOR_FIND_EXISTING_RELATIONSHIP_IN_SENTENCE_ENFORCE_SAME_SENTENCE_CHECKS
			if(relationshipEntityTemp->entityName == relationshipEntityName)	//probably redundant
			{	
				int featureIndexTemp = relationshipEntityTemp->entityIndexTemp;
				if((*translatorVariables->GIAentityNodeArrayFilled)[featureIndexTemp])
				{ 
			#endif
					*relationshipEntity = relationshipEntityTemp;
					foundExistingRelationship = true;
			#ifdef GIA_PREPROCESSOR_FIND_EXISTING_RELATIONSHIP_IN_SENTENCE_ENFORCE_SAME_SENTENCE_CHECKS
				}
			}
			#endif
		} 
	}
	/*
	//redundant
	for(vector<GIAentityConnection*>::iterator connectionIter = relationshipObjectEntity->entityVectorConnectionsArray[connectionTypeReverse].begin(); connectionIter != relationshipObjectEntity->entityVectorConnectionsArray[connectionTypeReverse].end(); connectionIter++)
	{	
		if(getRelationshipSubjectEntity((*connectionIter)) == relationshipSubjectEntity)
		{
			*relationshipEntity = (*connectionIter)->entity;
			foundExistingRelationship = true;
		}		
	}
	*/
			
	return foundExistingRelationship;
}

GIAentityNode* GIAtranslatorOperationsClass::addEntityNodeByNameSimpleWrapperRelationshipArtificial(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables)
{
	int relationshipEntityIndex = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;

	GIAentityNode* relationshipEntity = findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship(relationshipEntityIndex, &relationshipEntityName, translatorVariables);
	
	//added 3a1j;
	relationshipEntity = addInstanceToInstanceDefinition(relationshipEntity, relationshipEntityType, translatorVariables);
	(*translatorVariables->GIAentityNodeArray)[relationshipEntityIndex] = relationshipEntity;
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	relationshipEntity->isArtificialAuxiliary = true;
	#endif
	
	return relationshipEntity;	
}

/*
GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipConditionArtificial(GIAtranslatorVariablesClass* translatorVariables, string conditionName)
{
	//CHECKTHIS;
	
	int conditionEntityIndex = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;

	bool entityAlreadyExistant = false;
	GIAentityNode* conditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(conditionEntityIndex, &conditionName, &entityAlreadyExistant, translatorVariables);
	conditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	conditionEntity->entityIndexTemp = inverseConditionEntityIndex;
	//why not set inverseConditionEntity->sentenceIndexTemp?

	//#ifdef GIA_SEM_REL_TRANSLATOR_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	//(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex] = new GIAfeature();
	//(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->word = conditionName;
	//(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->lemma = conditionName;	//is this necessary?
	//(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
	//#endif
}
*/

GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(int featureIndex, const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* conditionRelationshipEntity;
	#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
	if((*translatorVariables->GIAentityNodeArrayFilled)[featureIndex])
	{
		conditionRelationshipEntity = (*translatorVariables->GIAentityNodeArray)[featureIndex];
	}
	else
	{
		conditionRelationshipEntity = findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship(featureIndex, entityNodeName, translatorVariables);
	}
	#else
	conditionRelationshipEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, translatorVariables);	
	#endif
	
	//added 3a1j;
	#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
	conditionRelationshipEntity->entityIndexTemp = featureIndex;	//added 3a1n (for ((*translatorVariables->GIAentityNodeArrayFilled)[featureIndex]) and/or !GIA_ADVANCED_REFERENCING_CONDITIONS cases) 
	conditionRelationshipEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	#endif
	conditionRelationshipEntity = addInstanceToInstanceDefinition(conditionRelationshipEntity, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
	(*translatorVariables->GIAentityNodeArray)[featureIndex] = conditionRelationshipEntity;
	
	return conditionRelationshipEntity;
}

GIAentityNode* GIAtranslatorOperationsClass::findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship(int featureIndex, const string* entityNodeName, GIAtranslatorVariablesClass* translatorVariables)
{
	bool entityAlreadyExistant = false;
	GIAentityNode* relationshipEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, &entityAlreadyExistant, translatorVariables);
	(*translatorVariables->GIAentityNodeArrayFilled)[featureIndex] = true;
	(*translatorVariables->GIAentityNodeArray)[featureIndex] = relationshipEntity;
	relationshipEntity->entityIndexTemp = featureIndex;
	relationshipEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	
	return relationshipEntity;
}





//"2" functions executed after parsing sentences (ie no sentence entity arrays):
GIAentityNode* GIAtranslatorOperationsClass::addPropertyRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed)
{
	return addRelationshipArtificialToEntity2(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, GIA_ENTITY_TYPE_PROPERTY, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES, translatorVariablesSentencesParsed);
}
GIAentityNode* GIAtranslatorOperationsClass::addDefinitionRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed)
{
	return addRelationshipArtificialToEntity2(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, GIA_ENTITY_TYPE_DEFINITION, RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS, translatorVariablesSentencesParsed);
}
GIAentityNode* GIAtranslatorOperationsClass::addRelationshipArtificialToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed)
{
	bool result = true;
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial2(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType, relationshipEntityName, translatorVariablesSentencesParsed);
	#else
	GIAentityNode* relationshipEntity = NULL;
	#endif
	
	if(relationshipEntityType == GIA_ENTITY_TYPE_DEFINITION)
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
		result = connectDefinitionToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariablesSentencesParsed);
		#else
		result = connectDirectDefinitionToEntity(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, translatorVariablesSentencesParsed);	
		//NB function will return NULL
		#endif
	}
	else if(relationshipEntityType == GIA_ENTITY_TYPE_PROPERTY)
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
		result = connectPropertyToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariablesSentencesParsed);
		#else
		result = connectDirectPropertyToEntity(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, translatorVariablesSentencesParsed);	
		//NB function will return NULL	
		#endif
	}
	/*
	//FUTURE:
	else if(relationshipEntityType == GIA_ENTITY_TYPE_CONDITION)
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
		result = connectConditionToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariablesSentencesParsed);
		#else
		bool entityAlreadyExistant = false;
		relationshipEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&conditionRelationshipEntityName, &entityAlreadyExistant, translatorVariablesSentencesParsed);
		result = connectConditionToEntity(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntity, sameReferenceSet, translatorVariablesSentencesParsed);		
		#endif
	}
	*/
	else
	{
		cerr << "GIAtranslatorOperationsClass::addRelationshipToEntity2{} error: unsupported relationshipEntityType; relationshipEntityType = " << relationshipEntityType << endl;
		exit(EXIT_ERROR);
	}
	
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
	if(relationshipEntityType == GIA_ENTITY_TYPE_CONDITION)
	{
	#endif
		translatorVariablesSentencesParsed->entityNodesActiveListComplete->push_back(relationshipEntity);
		//now add the relationshipEntity to entityNodesActiveListSentences such that it can be found quickly in the future (e.g. by a high level application)
		map<int, vector<GIAentityNode*>*>::iterator entityNodesActiveListSentenceIter = translatorVariablesSentencesParsed->entityNodesActiveListSentences->find(translatorVariablesSentencesParsed->sentenceIndex);	
		if(entityNodesActiveListSentenceIter != translatorVariablesSentencesParsed->entityNodesActiveListSentences->end())
		{
			vector<GIAentityNode*>* entityNodesActiveListSentence = entityNodesActiveListSentenceIter->second;
			entityNodesActiveListSentence->push_back(relationshipEntity);	
		}
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
	}
	#endif
	
	return relationshipEntity;
}
#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed)
{
	GIAentityNode* relationshipEntity = NULL;
	#ifndef GIA_PREPROCESSOR_FIND_EXISTING_RELATIONSHIP_IN_SENTENCE_ENFORCE_SAME_SENTENCE_CHECKS
	if(!findExistingRelationshipInSentenceEntityArray(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType, relationshipEntityName, &relationshipEntity, translatorVariablesSentencesParsed))	//added 3a2b (see <=3a1j for equivalent change);
	{
	#endif
		relationshipEntity = addEntityNodeByNameSimpleWrapperRelationshipArtificial2(relationshipEntityType, relationshipEntityName, translatorVariablesSentencesParsed);
	#ifndef GIA_PREPROCESSOR_FIND_EXISTING_RELATIONSHIP_IN_SENTENCE_ENFORCE_SAME_SENTENCE_CHECKS	
	}
	#endif
	
	return relationshipEntity;	
}
GIAentityNode* GIAtranslatorOperationsClass::addEntityNodeByNameSimpleWrapperRelationshipArtificial2(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed)
{
	GIAentityNode* relationshipEntity = findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship2(relationshipEntityName, translatorVariablesSentencesParsed);
	
	//added 3a2b (see 3a1j for equivalent change);
	relationshipEntity = addInstanceToInstanceDefinition(relationshipEntity, relationshipEntityType, translatorVariablesSentencesParsed);
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	relationshipEntity->isArtificialAuxiliary = true;
	#endif
	
	return relationshipEntity;	
}
GIAentityNode* GIAtranslatorOperationsClass::findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship2(const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed)
{
	bool entityAlreadyExistant = false;
	GIAentityNode* relationshipEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&relationshipEntityName, &entityAlreadyExistant, translatorVariablesSentencesParsed);
	
	return relationshipEntity;
}
#endif





GIAentityNode* GIAtranslatorOperationsClass::findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables)
{
	return findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, translatorVariables, true);
}

GIAentityNode* GIAtranslatorOperationsClass::findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables, const bool tempEntityEnabled)
{
	GIAentityNode* entityNodeFound = NULL;

	entityNodeFound = GIAdatabase.findOrAddNetworkIndexEntityNodeByName(translatorVariables->entityNodesActiveListComplete, translatorVariables->entityNodesActiveListNetworkIndexes, entityNodeName, entityAlreadyExistant, true, &(translatorVariables->currentEntityNodeIDInCompleteList), &(translatorVariables->currentEntityNodeIDInNetworkIndexEntityNodesList), translatorVariables->saveNetwork);
	
	applyNetworkIndexEntityAlreadyExistsFunction(entityNodeFound, *entityAlreadyExistant, tempEntityEnabled);

	return entityNodeFound;
}




/*these functions have been added for GIA Database compatibility*/
//this function does write to database, but prepares data structures for write to database (at the end of the user sequence, writeDatabase{} is written...)
GIAentityConnection* GIAtranslatorOperationsClass::writeVectorConnection(GIAentityNode* entityNode, GIAentityNode* entityNodeToAdd, const int connectionType, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityConnection* newConnection = NULL;
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	if(!(entityNode->wasReferenceTemp && entityNodeToAdd->wasReferenceTemp))
	{
	#endif
		#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE
		GIAentityConnection* connectionFound2 = NULL;
		if(!findSameSentenceEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &connectionFound2, translatorVariables->sentenceIndex))
		{
		#endif
			vector<GIAentityConnection*>* vectorConnection = &(entityNode->entityVectorConnectionsArray[connectionType]);
			#ifndef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
			if(entityVectorConnectionIsBasicArray[connectionType])
			{
				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				if(connectionType != GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
				{
				#endif
					vectorConnection->clear();	//clear the vector (basic connections only support 1 node)
				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				}
				#endif
			}
			#endif

			newConnection = new GIAentityConnection();
			newConnection->entity = entityNodeToAdd;
			#ifdef GIA_ENTITY_CONNECTION_RECORD_ENTITY_ORIGIN
			newConnection->entityOrigin = entityNode;
			#ifdef GIA_ENTITY_CONNECTION_RECORD_RELATIONSHIP_TYPE
			newConnection->connectionType = connectionType;
			#endif
			#endif
			#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
			GIAentityConnection* connectionFound3 = NULL;
			if(findEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &connectionFound3))
			{
				newConnection->isReference = true;
			}
			#endif
			vectorConnection->push_back(newConnection);

			newConnection->sameReferenceSet = sameReferenceSet;
			/*
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			cout << "writeVectorConnection: newConnection->sameReferenceSet = " << sameReferenceSet << endl;
			#endif
			*/

			#ifdef GIA_DATABASE
			if((GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE) || (GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_INACTIVE))	//NB even if not accessing the database for new information (read), still prepare nodes for database write
			{
				//#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS		//why is this preprocessor check not required???
				//required for database syncronisation with RAM
				if(!(entityNode->entityVectorConnectionsReferenceListLoadedArray[connectionType]))
				{
					cerr << "error: writeVectorConnection called, but entityVectorConnectionsReferenceListLoadedArray set to false" << endl;
					cerr << "entityNode = " << entityNode->entityName << ", entityNodeToAdd = " << entityNodeToAdd->entityName << ", connectionType = " << connectionType << endl;
					exit(EXIT_ERROR);
				}
				//#endif

				newConnection->entityName = entityNodeToAdd->entityName;
				newConnection->idInstance = entityNodeToAdd->idInstance;
				newConnection->loaded = true;
				newConnection->modified = false;
				newConnection->added = true;		//this allows for fast update of the DB (append reference connections)
			}
			#endif

			newConnection->sentenceIndexTemp = translatorVariables->sentenceIndex;
		#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE
		}
		#endif
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	}
	#endif
	return newConnection;
}


bool GIAtranslatorOperationsClass::findEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, GIAentityConnection** connectionFound)
{
	bool foundNode = false;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if((*connectionIter)->entity == entityNodeToFind)
		{
			foundNode = true;
			*connectionFound = *connectionIter;
		}
	}
	return foundNode;
}

bool GIAtranslatorOperationsClass::findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex)
{
	bool foundNode = false;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if((*connectionIter)->entity == entityNodeToFind)
		{
			if((*connectionIter)->sentenceIndexTemp == sentenceIndex)
			{
				foundNode = true;
				*connectionFound = *connectionIter;
			}
		}
	}
	return foundNode;
}

bool GIAtranslatorOperationsClass::findEntityNodeNameInVector(GIAentityNode* entityNode, const string* entityNodeNameToFind, const int connectionType, GIAentityConnection** connectionFound)
{
	bool foundNode = false;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if((*connectionIter)->entity->entityName == *entityNodeNameToFind)
		{
			foundNode = true;
			*connectionFound = *connectionIter;
		}
	}
	return foundNode;
}



long GIAtranslatorOperationsClass::determineNextIdInstance(GIAentityNode* entity)
{
	long nextIdInstance;
	GIAentityNode* networkIndexEntity;
	#ifdef GIA_APPLY_BUG_WORKAROUND_WHERE_A_NETWORK_INDEX_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE
	if(entity->idInstance == GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE)
	{
		//the current entity is a networkIndex entity
		networkIndexEntity = entity;
	}
	else
	{
	#endif
		if(!(entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE].empty()))
		{
			//the current entity is a substance of a networkIndex entity
			networkIndexEntity = getPrimaryNetworkIndexNodeDefiningInstance(entity);
		}
		else
		{
			//the current entity is a networkIndex entity
			networkIndexEntity = entity;
		}
	#ifdef GIA_APPLY_BUG_WORKAROUND_WHERE_A_NETWORK_INDEX_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE
	}
	#endif

//#if defined(GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS) || !defined(GIA_DATABASE)
//check #elif !defined GIA_DATABASE is OK instead of #elif !defined(GIA_DATABASE)
#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].empty()))
	{
		long previousIdInstance = networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].back()->entity->idInstance;
		nextIdInstance = previousIdInstance + 1;
	}
	else
	{
		nextIdInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE + 1;
	}
	#else
	long numberOfInstances =  networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].size();
	nextIdInstance = numberOfInstances;
	#endif
#elif !defined GIA_DATABASE
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].empty()))
	{
		long previousIdInstance = networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].back()->entity->idInstance;
		nextIdInstance = previousIdInstance + 1;
	}
	else
	{
		nextIdInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE + 1;
	}
	#else
	long numberOfInstances =  networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].size();
	nextIdInstance = numberOfInstances;
	#endif
#else
	if(networkIndexEntity->networkIndexEntityLoaded == NULL)
	{
		cout <<"ERROR: (networkIndexEntity->networkIndexEntityLoaded == NULL)" << endl;
	}
	GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = networkIndexEntity->networkIndexEntityLoaded;
	networkIndexEntityLoaded->numberOfInstances = networkIndexEntityLoaded->numberOfInstances + 1;
	nextIdInstance = networkIndexEntityLoaded->numberOfInstances;
#endif

	return nextIdInstance;
}


#ifdef GIA_DATABASE

void GIAtranslatorOperationsClass::addInstanceEntityNodeToActiveLists(GIAentityNode* entity, GIAtranslatorVariablesClass* translatorVariables)
{//NB add reference set entity to active list complete + appropriate list (substance/action/condition) [NB the reference set entity will already be added to networkIndex active list entityNodesActiveListNetworkIndexes...] - this function enables references to be written to XML

	if(translatorVariables->saveNetwork)
	{
		if(entity->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
		{
			//addInstanceEntityNodeToActiveLists{} does not support networkIndex entities. NB when this function is executed via linkAdvancedReferencesGIA{}, the referenceSource networkIndex entity is already added to the networkIndex active list (ie, see GIAentityNode* referenceSourceNetworkIndexEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper)
		}

		entity->idActiveList = translatorVariables->currentEntityNodeIDInCompleteList;
		(translatorVariables->entityNodesActiveListComplete)->push_back(entity);
		(translatorVariables->currentEntityNodeIDInCompleteList) = (translatorVariables->currentEntityNodeIDInCompleteList) + 1;

		if(GIAdatabase.getUseDatabase() != GIA_DATABASE_FALSE)
		{
			GIAdatabase.addEntityNodesActiveListCompleteFastIndexDBactive(entity);	//added 2 Nov 2012
		}
	}
	else
	{//this case should never be used when this function is executed via linkAdvancedReferencesGIA{}

	}
}
#endif

#ifdef GIA_ALIASES
#ifndef GIA_DISABLE_ALIAS_ENTITY_MERGING
bool GIAtranslatorOperationsClass::mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	if(entityNode == entityNodeToMerge)	//before 3a1o: (entityNode->idActiveList == entityNodeToMerge->idActiveList)
	{
		result = false;
	}
	else
	{
		if(entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT)
		{
			entityNode->entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//added 3c4a
		}
		
		entityNode->aliasList.push_back(entityNodeToMerge->entityName);

		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNodeToMerge->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNodeToMerge->entityVectorConnectionsArray[connectionType].end(); )
			{
				bool connectionIterErased = false;
				//connect entityNodeToMerge ambient node to entityNode
				GIAentityNode* entityConnectedToEntityToMerge = (*connectionIter)->entity;


				if(entityNode != entityConnectedToEntityToMerge)
				{//added 29 November 2012

					//disconnect reference sources from each other, as their connection between each other will be redeclared in current context
					int connectionTypeInverted = generateInvertedConnectionType(entityConnectedToEntityToMerge, connectionType);
					for(vector<GIAentityConnection*>::iterator connectionIter2 = entityConnectedToEntityToMerge->entityVectorConnectionsArray[connectionTypeInverted].begin(); connectionIter2 != entityConnectedToEntityToMerge->entityVectorConnectionsArray[connectionTypeInverted].end(); )
					{
						bool connectionIter2Erased = false;
						GIAentityNode* entityConnectedToEntityConnectedToEntityToMerge = (*connectionIter2)->entity;


						if(entityNodeToMerge == entityConnectedToEntityConnectedToEntityToMerge)	//OR (entityNodeToMerge == entityConnectedToEntityConnectedToEntityToMerge)?
						{
							#ifndef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
							//the commenting out of the below case is required for advanced referencing (eg networkIndex Tom has associated instance Dog) [NB this means that instances can appear to have more than one instanceReverseNodeList]
							if((entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE))
							{//restored 29 November 2012, and condition (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE) added
								//disconnect entityConnectedToEntityConnectedToEntityToMerge from entityConnectedToEntityToMerge (networkIndex) (z2)
								//delete* connectionIter2;	//delete connection
								connectionIter2 = entityConnectedToEntityToMerge->entityVectorConnectionsArray[connectionTypeInverted].erase(connectionIter2);		//(*connectionIter2)->entity = NULL;	//need a better delete routine
								connectionIter2Erased = true;
								#ifdef GIA_DATABASE
								//(*connectionIter2)->modified = true;
								entityConnectedToEntityToMerge->entityVectorConnectionsRemovedArray[connectionTypeInverted] = true;	//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
								#endif
							}
							else
							{
							#endif
								//connect entityConnectedToEntityConnectedToEntityToMerge back to entityNode (z)

								(*connectionIter2)->entity = entityNode;
								#ifdef GIA_DATABASE
								(*connectionIter2)->entityName = entityNode->entityName;	//added 29 November 2012
								(*connectionIter2)->idInstance = entityNode->idInstance;	//added 29 November 2012
								(*connectionIter2)->modified = true;
								#endif
							#ifndef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
							}
							#endif
						}

						if(!connectionIter2Erased)
						{
							connectionIter2++;
						}
					}

					//connect entityNode to entityConnectedToEntityToMerge (x)

					#ifndef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
					//the commenting out of the below case is required for advanced referencing (eg networkIndex Tom has associated instance Dog) [NB this means that instances can appear to have more than one instanceReverseNodeList]
					if(!((entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)))
					{
					#endif
						bool sameReferenceSet = (*connectionIter)->sameReferenceSet;
						writeVectorConnection(entityNode, entityConnectedToEntityToMerge, connectionType, sameReferenceSet, translatorVariables);
					#ifndef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
					}
					#endif
				}

				//disconnect entityConnectedToEntityToMerge from entityNodeToMerge
				//delete* connectionIter;	//delete connection
				connectionIter = entityNodeToMerge->entityVectorConnectionsArray[connectionType].erase(connectionIter);		//(*connectionIter)->entity = NULL;	//need a better delete routine
				connectionIterErased = true;
				#ifdef GIA_DATABASE
				//(*connectionIter)->modified = true;
				entityNodeToMerge->entityVectorConnectionsRemovedArray[connectionType] = true;	//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
				#endif

				if(!connectionIterErased)
				{
					connectionIter++;
				}
			}
		}

		//disconnect entityNodeToMerge from entityNode
		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); )
			{
				bool connectionIterErased = false;
				GIAentityNode* entityConnectedToEntity = (*connectionIter)->entity;
				if(entityConnectedToEntity == entityNodeToMerge)
				{
					//disconnect entityNodeToMerge from entityNode (y)
					connectionIter = entityNode->entityVectorConnectionsArray[connectionType].erase(connectionIter);		//(*connectionIter)->entity = NULL;	//need a better delete routine
					connectionIterErased = true;
					#ifdef GIA_DATABASE
					//(*connectionIter)->modified = true;
					entityNode->entityVectorConnectionsRemovedArray[connectionType] = true;	//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
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
			#ifdef GIA_ALIASES
			entityNode->isNameQuery = entityNodeToMerge->isNameQuery;
			#endif
			if(getComparisonVariableNode() == entityNodeToMerge)
			{
				setComparisonVariableNode(entityNode);
			}
		}
		#ifdef GIA_ALIASES
		entityNode->isName = entityNodeToMerge->isName;
		#endif

		if(entityNodeToMerge->isToBeComplimentOfActionTemp)
		{
			entityNode->isToBeComplimentOfActionTemp = true;	//should not be required
		}
		
		disableEntity(entityNodeToMerge);
		


		//entityNode->entityIndexTemp = entityNodeToMerge->entityIndexTemp;	//added 3a1o

	}
	return result;
}
#endif
#endif


GIAentityNode* GIAtranslatorOperationsClass::getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNode* instanceEntity)
{
	GIAentityNode* primaryNetworkIndexNodeDefiningInstance = NULL;

	#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
	for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->instanceReverseNodeList->begin(); connectionIter != instanceEntity->instanceReverseNodeList->end(); connectionIter++)
	{
		GIAentityNode* networkIndexEntityNode = (*connectionIter)->entity;
		#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
		#ifndef USE_NLC
		if(instanceEntity->entityName == networkIndexEntityNode->entityName)	//this check is incompatible with NLC_ADVANCED_REFERENCING_SUPPORT_ALIASES (in which an entity name may be changed by NLC; entity2->entityName = aliasClassName;)
		{
		#endif
		#endif
			primaryNetworkIndexNodeDefiningInstance = networkIndexEntityNode;
		#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
		#ifndef USE_NLC
		}
		#endif
		#endif
	}
	#else
	if(!(instanceEntity->instanceReverseNodeList->empty()))
	{
		primaryNetworkIndexNodeDefiningInstance = (instanceEntity->instanceReverseNodeList->back())->entity;
	}
	#endif

	return primaryNetworkIndexNodeDefiningInstance;
}

#ifdef GIA_NLC_INTEGRATION
NLCpreprocessorSentence* GIAtranslatorOperationsClass::getFirstNLCsentenceInListGIA()
{
	return firstNLCsentenceInListLocalGIA;
}
void GIAtranslatorOperationsClass::setFirstNLCsentenceInListGIA(NLCpreprocessorSentence* firstNLCsentenceInListNew)
{
	firstNLCsentenceInListLocalGIA = firstNLCsentenceInListNew;
}
#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
bool GIAtranslatorOperationsClass::checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA(const GIAentityNode* indefiniteEntity, const GIAentityNode* definiteEntity, int* indentationDifferenceFound)
{
	bool foundIndefiniteEntity = false;

	if(indefiniteEntity->sentenceIndexTemp < definiteEntity->sentenceIndexTemp)
	{
		NLCpreprocessorSentence* currentNLCsentenceInList = getFirstNLCsentenceInListGIA();
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
		NLCpreprocessorSentence* indefiniteEntityNLCsentenceInList = currentNLCsentenceInList;
		
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
			NLCpreprocessorSentence* definiteEntityNLCsentenceInList = currentNLCsentenceInList;

			
			if(foundDefiniteEntitySentence)
			{
				if(minimumIndentationBetweenIndefiniteAndIndefiniteEntitySentence < indefiniteEntityNLCsentenceInList->indentation)
				{
				}
				else
				{
				       foundIndefiniteEntity = true;
				       *indentationDifferenceFound = definiteEntityNLCsentenceInList->indentation - indefiniteEntityNLCsentenceInList->indentation;
				}
			}
			else
			{
			}
		}
		else
		{
		}
	}

	return foundIndefiniteEntity;
}
#endif
#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
bool GIAtranslatorOperationsClass::checkIfSentenceIsMathTextParsablePhrase(const int sentenceIndex)
{
	bool sentenceIsMathTextParsablePhrase = false;
	NLCpreprocessorSentence* firstNLCsentenceInList = getFirstNLCsentenceInListGIA();
	NLCpreprocessorSentence* sentence = NULL;
	if(NLCpreprocessorSentenceClass.getSentenceInSentenceList(sentenceIndex, firstNLCsentenceInList, &sentence)) 
	{
		if(NLCpreprocessorSentenceClass.sentencePertainsToLogicalCondition(sentence))	//OLD (CHECKTHIS change): if(sentence->mathTextNLPparsablePhraseIndex != INT_DEFAULT_VALUE)
		{
			sentenceIsMathTextParsablePhrase = true;
		}	
	}
	return sentenceIsMathTextParsablePhrase;
}
#endif
#endif


#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES

GIAentityNode* GIAtranslatorOperationsClass::createNewNonspecificConcept(GIAentityNode* networkIndexEntity, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* nonspecificConcept = addInstanceToInstanceDefinition(networkIndexEntity, GIA_ENTITY_TYPE_CONCEPT, translatorVariables);
	//this enables GIA drawing of concept
	nonspecificConcept->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added concept
	nonspecificConcept->sentenceIndexTemp = networkIndexEntity->sentenceIndexTemp;
	return nonspecificConcept;
}

GIAentityNode* GIAtranslatorOperationsClass::getNonspecificConceptEntityFromNetworkIndex(GIAentityNode* networkIndexEntity)
{
	GIAentityNode* nonspecificConceptEntity = NULL;
	for(vector<GIAentityConnection*>::iterator iter = networkIndexEntity->instanceNodeList->begin(); iter < networkIndexEntity->instanceNodeList->end(); iter++)
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
GIAentityNode* GIAtranslatorOperationsClass::getNonspecificConceptEntityFromInstance(GIAentityNode* instanceEntity)
{
	GIAentityNode* nonspecificConceptEntity = NULL;
	for(vector<GIAentityConnection*>::iterator iter = instanceEntity->definitionNodeList->begin(); iter < instanceEntity->definitionNodeList->end(); iter++)
	{
		GIAentityNode* conceptEntity = (*iter)->entity;
		if(isNonspecificConceptEntity(conceptEntity))
		{
			nonspecificConceptEntity = conceptEntity;
		}
	}

	return nonspecificConceptEntity;
}

bool GIAtranslatorOperationsClass::isNonspecificConceptEntity(GIAentityNode* entity)
{
	bool nonspecificConcept = false;
	if(entity->entityType == GIA_ENTITY_TYPE_CONCEPT)
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


void GIAtranslatorOperationsClass::printEntity(GIAentityNode* entity)
{
	cout << "entity = " << entity->entityName << endl;
	for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
		{
			bool connectionIterErased = false;
			GIAentityNode* entityConnectedToEntity = (*connectionIter)->entity;
			if(!(entityConnectedToEntity->disabled))
			{
				cout << "\tconnection = " << connectionType << ", target = " << entityConnectedToEntity->entityName << endl;
			}
		}
	}
}





#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
void GIAtranslatorOperationsClass::createParentsOfSubclassEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* subclassNetworkIndexEntity = (*translatorVariables->GIAnetworkIndexNodeArray)[w];
			if(subclassNetworkIndexEntity->convertToSubClass)
			{
				#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
				subclassNetworkIndexEntity->isSubClass = true;
				#endif

				string parentClassName = GIAentityNodeClass.getParentClassEntityNameFromSubClassEntityName(subclassNetworkIndexEntity->entityName);


				bool parentNetworkIndexEntityAlreadyExistant = false;
				GIAentityNode* parentClassNetworkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&parentClassName, &parentNetworkIndexEntityAlreadyExistant, translatorVariables);
				if(!parentNetworkIndexEntityAlreadyExistant)
				{
					//cout << "creating parentClassNetworkIndexEntity: " << parentClassName << endl;
					translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(parentClassNetworkIndexEntity);

					parentClassNetworkIndexEntity->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added subclass parent networkIndex
					parentClassNetworkIndexEntity->sentenceIndexTemp = subclassNetworkIndexEntity->sentenceIndexTemp;

					linkSubclassEntitiesWithParentClassEntities(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, false, false, translatorVariables);
				}
				else
				{
					linkSubclassEntitiesWithParentClassEntities(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, false, true, translatorVariables);
				}
			}
		}
	}
}

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
void GIAtranslatorOperationsClass::createAdditionalSubclassEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	//eg The red bat is a xenomorph. -> create xenomorph_bat, alias link xenomorph concept with xenomorph_bat concept
		//OLD: eg The red bat is a xenomorph. -> create xenomorph_bat, definition link xenomorph_bat concept with xenomorph concept, and definitition link red bat to xenomorph_bat concept

	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			if(entity->addSubClass)
			{
				if(!(entity->entityType == GIA_ENTITY_TYPE_CONCEPT))	//assume true
				{
					GIAentityNode* definitionRelationshipObjectEntity = NULL;
					bool foundDefinitionEntity = false;
					for(vector<GIAentityConnection*>::iterator definitionNodeListIterator = entity->definitionNodeList->begin(); definitionNodeListIterator < entity->definitionNodeList->end(); definitionNodeListIterator++)
					{
						GIAentityNode* definitionEntityTemp = (*definitionNodeListIterator)->entity;
						if(definitionEntityTemp->entityType == GIA_ENTITY_TYPE_CONCEPT)	//assume true
						{
							#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
							if((*definitionNodeListIterator)->sentenceIndexTemp = translatorVariables->sentenceIndex)
							{//in the case that a substance has been assigned multiple definitions in the text, eg "The animal is a dog. The animal is an alsatian", in which case "The animal" entity node will be connected to both dog and alsation (due to GIA advanced referencing).
							#else
							//NB dream mode has not yet been executed so all concepts connected to this entity will have been defined in the same sentence
							#endif
								foundDefinitionEntity = true;
								definitionRelationshipObjectEntity = definitionEntityTemp;	//eg xenomorph
							#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
							}
							#endif
						}
					}

					if(foundDefinitionEntity)
					{

						string subClassEntityName = GIAentityNodeClass.createSubClassEntityName(definitionRelationshipObjectEntity->entityName, entity->entityName);	//eg apple_fruit / xenomorph_bat
						//cout << "subClassEntityName = " << subClassEntityName << endl;
						bool subClassNetworkIndexEntityAlreadyExistant = false;
						GIAentityNode* subClassNetworkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&subClassEntityName, &subClassNetworkIndexEntityAlreadyExistant, translatorVariables);
						if(!subClassNetworkIndexEntityAlreadyExistant)
						{
							#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
							subClassNetworkIndexEntity->isSubClass = true;
							#endif

							translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(subClassNetworkIndexEntity);

							subClassNetworkIndexEntity->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added subclass parent networkIndex
							subClassNetworkIndexEntity->sentenceIndexTemp = entity->sentenceIndexTemp;

							GIAentityNode* definitionNetworkIndexEntity = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(definitionRelationshipObjectEntity);
							linkSubclassEntitiesWithParentClassEntities(definitionNetworkIndexEntity, subClassNetworkIndexEntity, false, false, translatorVariables);	//links xenomorph concept with xenomorph_bat concept

							/* //changed 2m7a
							bool sameReferenceSet = false;	//this is required for dreamModeLinkSpecificConceptsAndActions
							#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
							//now create a definition link between the entity and the subClass concept (since this will not be created by GIA dream mode)
							GIAentityNode* subclassNonspecificConcept = getNonspecificConceptEntityFromNetworkIndex(subClassNetworkIndexEntity);
							connectDefinitionToEntity(entity, subclassNonspecificConcept, sameReferenceSet);	//links red bat to xenomorph_bat concept
							//#ifdef GIA_DEBUG
							cout << "createAdditionalSubclassEntities{}: linking entity: " << entity->entityName << " to " << subclassNonspecificConcept->entityName << endl;
							//#endif
							#endif
							*/
						}
					}
				}
			}
		}
	}
}

#endif

void GIAtranslatorOperationsClass::linkSubclassEntitiesWithParentClassEntities(GIAentityNode* subclassNetworkIndexEntity, GIAentityNode* parentClassNetworkIndexEntity, const bool linkAsAlias, const bool onlyLinkIfSubclassOrParentConceptNotFound, GIAtranslatorVariablesClass* translatorVariables)
{
	string parentClassName = parentClassNetworkIndexEntity->entityName;

	bool subclassToParentEntityConnectionAlreadyMade = false;
	GIAentityNode* subclassNonspecificConcept = GIAtranslatorOperations.getNonspecificConceptEntityFromNetworkIndex(subclassNetworkIndexEntity);
	bool subclassConceptFound = false;
	if(subclassNonspecificConcept != NULL)
	{
		//cout << "subclassNonspecificConcept found" << endl;
		subclassConceptFound = true;
		for(vector<GIAentityConnection*>::iterator definitionNodeListIterator = subclassNonspecificConcept->definitionNodeList->begin(); definitionNodeListIterator < subclassNonspecificConcept->definitionNodeList->end(); definitionNodeListIterator++)
		{
			GIAentityNode* definitionRelationshipObjectEntity = (*definitionNodeListIterator)->entity;
			if(definitionRelationshipObjectEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)	//assume true
			{
				if(definitionRelationshipObjectEntity->entityName == parentClassName)
				{
					subclassToParentEntityConnectionAlreadyMade = true;
				}
			}
		}
	}
	else
	{
		subclassNonspecificConcept = GIAtranslatorOperations.createNewNonspecificConcept(subclassNetworkIndexEntity, translatorVariables);
		#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
		subclassNonspecificConcept->isSubClass = true;
		#endif
	}

	if(!subclassToParentEntityConnectionAlreadyMade)
	{
		//should a new subclassNonspecificConcept be defined here regardless; ie GIAentityNode* subclassNonspecificConcept = GIAtranslatorOperations.createNewNonspecificConcept(subclassNetworkIndexEntity)? (because new concepts are created every sentence, even for non-specific concepts; eg "Pies are apples. Pies are berries.")

		bool performLink = false;

		GIAentityNode* parentClassNonspecificConcept = GIAtranslatorOperations.getNonspecificConceptEntityFromNetworkIndex(parentClassNetworkIndexEntity);
		if(parentClassNonspecificConcept == NULL)
		{
			performLink = true;
			parentClassNonspecificConcept = GIAtranslatorOperations.createNewNonspecificConcept(parentClassNetworkIndexEntity, translatorVariables);
		}
		else
		{
			if(!subclassConceptFound)
			{
				performLink = true;
			}
		}

		if(performLink)
		{
			bool sameReferenceSet = false;	//this is required for dreamModeLinkSpecificConceptsAndActions
			
			GIAentityNode* definitionRelationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationship2(RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS, translatorVariables, true);
			//GIAentityNode* definitionRelationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(translatorVariables);	//use findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition2 (ie findOrAddEntityNodeByNameSimpleWrapperRelationship2) instead of findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition [as GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES doesn't fill in GIAentityNodeArrays at this stage of development, and would need to update GIAfeatureTempEntityNodeArray otherwise; not just GIAentityNodeArray]
			
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			if(linkAsAlias)
			{
				GIAtranslatorOperations.connectDefinitionToEntityMarkConnectionAsAlias(subclassNonspecificConcept, parentClassNonspecificConcept, definitionRelationshipEntity, sameReferenceSet, translatorVariables);
			}
			else
			{
			#endif
				GIAtranslatorOperations.connectDefinitionToEntity(subclassNonspecificConcept, parentClassNonspecificConcept, definitionRelationshipEntity, sameReferenceSet, translatorVariables);
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			}
			#endif
		}
	}
}

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
void GIAtranslatorOperationsClass::detectUserDeclaredSubclassEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			if(entity->entityType == GIA_ENTITY_TYPE_SUBSTANCE)	//ie !isAction && !isCondition (prevent multiword prepositions [and actions, in case multiword actions are implemented in the future])
			{
				if((entity->entityName).find(GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					entity->isSubClass = true;
				}
			}
		}
	}
}
#endif

#endif



#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
void GIAtranslatorOperationsClass::createShortcutsToConceptEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			if((*translatorVariables->GIAentityNodeArray)[w]->entityType == GIA_ENTITY_TYPE_CONCEPT)
			{
				GIAentityNode* conceptNode = (*translatorVariables->GIAentityNodeArray)[w];
				bool specificConcept = false;
				if(hasSameReferenceSetConnections(conceptNode, translatorVariables))
				{
					specificConcept = true;
					//cout << "specificConcept: conceptNode = " << conceptNode->entityName << endl;
				}
				
				GIAentityNode* networkIndexNode = getPrimaryNetworkIndexNodeDefiningInstance((*translatorVariables->GIAentityNodeArray)[w]);
				if(specificConcept)
				{
					bool alreadyAddedToSpecificConceptList = false;
					for(vector<GIAentityNode*>::iterator iter2 = networkIndexNode->shortcutsToSpecificConceptEntities.begin(); iter2 != networkIndexNode->shortcutsToSpecificConceptEntities.end(); iter2++)
					{
						GIAentityNode* specificConcept2 = (*iter2);
						if(specificConcept2 == conceptNode)
						{
							alreadyAddedToSpecificConceptList = true;
						}
					}
					if(!alreadyAddedToSpecificConceptList)
					{
						//cout << "!alreadyAddedToSpecificConceptList" << endl;
						networkIndexNode->shortcutsToSpecificConceptEntities.push_back(conceptNode);
					}
				}
				else
				{
					bool alreadyAddedToNonspecificConceptList = false;
					if(networkIndexNode->shortcutToNonspecificConceptEntity == conceptNode)
					{
						alreadyAddedToNonspecificConceptList = true;
					}
					if(!alreadyAddedToNonspecificConceptList)
					{
						networkIndexNode->shortcutToNonspecificConceptEntity = conceptNode;
					}
				}
			}
		}
	}
}

bool GIAtranslatorOperationsClass::hasSameReferenceSetConnections(GIAentityNode* entity, GIAtranslatorVariablesClass* translatorVariables)
{
	bool sameReferenceSetConnection = false;
	for(int connectionType = 0; connectionType < GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		bool pass = true;
		if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
		{
			pass = false;
		}
		if(pass)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
			{
				GIAentityConnection* connection = (*connectionIter);
				if(connection->sentenceIndexTemp == entity->sentenceIndexTemp)	//REDUNDANT
				{		
					if(connection->sameReferenceSet)
					{
						sameReferenceSetConnection = true;
					}
				}
			}
		}
	}
	return sameReferenceSetConnection;
}			
#endif






bool GIAtranslatorOperationsClass::connectDefinitionAliasWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode** entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet)
{
	bool result = true;
	
#ifndef GIA_DISABLE_ALIAS_ENTITY_MERGING
	#ifdef GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
	if(!(translatorVariables->linkPreestablishedReferencesGIA))	//OLD: if(!(param->linkPreestablishedReferencesGIA))
	{
		//disgard definition link, as 1. artificial definition entity will not be created, and 2. the definition link is redundant for aliases (names/queryName/propernoun appos) with !linkPreestablishedReferencesGIA
		/*
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		connectDefinitionToEntity(entitySemanticRelationFunction1, *entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		connectDirectDefinitionToEntity(entitySemanticRelationFunction1, *entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);								      
		#endif
		*/
	}
	else
	{
	#endif
		bool notAlreadyMerged = mergeEntityNodesAddAlias(entitySemanticRelationFunction1, *entitySemanticRelationFunction2, translatorVariables);
		if(notAlreadyMerged)
		{
			*entitySemanticRelationFunction2 = entitySemanticRelationFunction1;
		}
	#ifdef GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
	}
	#endif
#else
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	connectDefinitionToEntityMarkConnectionAsAlias(entitySemanticRelationFunction1, *entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
	#else
	connectDirectDefinitionToEntityMarkConnectionAsAlias(entitySemanticRelationFunction1, *entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	#endif
#endif

	return result;
}

bool GIAtranslatorOperationsClass::connectPrenominalModifierWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	bool result = true;
	
	bool definitionAliasDetected = false;
	if(SHAREDvars.isCapitalised(entitySemanticRelationFunction2->entityName) || entitySemanticRelationFunction2->grammaticalProperNounTemp)	//FUTURE GIA - check if grammaticalProperNounTemp has been assigned
	{
		//e.g. "planet Mars" (noun propernoun)
		//FUTURE GIA - consider creating a GIA LRP list for proper nouns to distinguish n-n and n-pn relations in GIArules.xml
		definitionAliasDetected = true;
		GIAentityNode* entitySemanticRelationFunction3 = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
		if(!connectDefinitionAliasWrapper(translatorVariables, entitySemanticRelationFunction1, &entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet))
		{
			result = false;
		}
	}
	if(!definitionAliasDetected)
	{
		//e.g. "goal line"	
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
		cerr << "GIAgenericEntityInterpretationParameters::connectPrenominalModifierWrapper{} case GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER error: !GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES not currently supported" << endl;
		exit(EXIT_ERROR);
		///GIAsemRelTranslatorParser.createSubclass(entitySemanticRelationFunction1, entitySemanticRelationFunction2);	//create subclass
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;	
	#else
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAentityNode* entitySemanticRelationFunction3 = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
		connectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		/*
		cout << "GIAtranslatorOperationsClass::connectPrenominalModifierWrapper" << endl;
		cout << "entitySemanticRelationFunction1->semanticRelationEntityIsReferenced = " << entitySemanticRelationFunction1->semanticRelationEntityIsReferenced << endl;
		cout << "entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = " << entitySemanticRelationFunction2->semanticRelationEntityIsReferenced << endl;
		cout << "entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = " << entitySemanticRelationFunction3->semanticRelationEntityIsReferenced << endl;
		*/
		#else
		connectDirectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);									
		#endif
	#endif
	}

	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
bool GIAtranslatorOperationsClass::connectMultiwordAuxiliaryWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	entitySemanticRelationFunction1->multiwordAuxiliaryList.push_back(entitySemanticRelationFunction2->entityName);
}
#endif

bool GIAtranslatorOperationsClass::connectMultiwordPrepositionWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	cout << "GIAtranslatorOperationsClass::connectMultiwordPrepositionWrapper{} warning; GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION (multiword prepositions should have been collapsed by GIA preprocessor) - check LRPdata/multiwordlists.txt for presence of 'preposition multiwordlistPreposition.txt'" << endl;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_COLLAPSE_ALIASES_ONLY
	cout << "GIA_TXT_REL_TRANSLATOR_RULES_GIA3_COLLAPSE_ALIASES_ONLY: GIAtranslatorOperationsClass::connectMultiwordPrepositionWrapper{} error; GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION (multiword prepositions should have been collapsed by GIA preprocessor) - check LRPdata/multiwordlists.txt for presence of 'preposition multiwordlistPreposition.txt'" << endl;	
	exit(EXIT_ERROR);
	#endif
	return connectMultiwordCollapse(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
}

bool GIAtranslatorOperationsClass::connectMultiwordAliasWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	return connectMultiwordCollapseReverse(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
}

bool GIAtranslatorOperationsClass::connectMultiwordCollapse(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	bool result = true;
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if(!(entitySemanticRelationFunction1->wasReference))
	{
	#endif
		string multiwordCollapsedName = entitySemanticRelationFunction1->entityName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER + entitySemanticRelationFunction2->entityName;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_COLLAPSE_ALIASES_ONLY
		string* alias = getFirstAlias(entitySemanticRelationFunction1);
		*alias = multiwordCollapsedName;
		#else
		entitySemanticRelationFunction1->entityName = multiwordCollapsedName;
		#endif
		passMultiwordGrammaticalParameters(entitySemanticRelationFunction1, entitySemanticRelationFunction2);

		//disableEntity(entitySemanticRelationFunction2);	
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	}
	else
	{
		//entity name should already have been collapsed
	}
	#endif
	return result;
}
bool GIAtranslatorOperationsClass::connectMultiwordCollapseReverse(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	bool result = true;
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if(!(entitySemanticRelationFunction1->wasReference))
	{
	#endif
		string multiwordCollapsedName = entitySemanticRelationFunction2->entityName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER + entitySemanticRelationFunction1->entityName;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_COLLAPSE_ALIASES_ONLY
		string* alias = getFirstAlias(entitySemanticRelationFunction1);
		*alias = multiwordCollapsedName;
		#else
		entitySemanticRelationFunction1->entityName = multiwordCollapsedName;
		#endif
		passMultiwordGrammaticalParameters(entitySemanticRelationFunction1, entitySemanticRelationFunction2);
		
		//disableEntity(entitySemanticRelationFunction2);
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	}
	else
	{
		//entity name should already have been collapsed
	}
	#endif
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_COLLAPSE_ALIASES_ONLY
string* GIAtranslatorOperationsClass::getFirstAlias(GIAentityNode* entity)
{
	string* alias = NULL;
	if(entity->aliasList.size() == 0)
	{
		string aliasTemp = entity->entityName;
		entity->aliasList.push_back(aliasTemp);
	}
	alias = &(entity->aliasList[0]);
	return alias;
}
#endif

void GIAtranslatorOperationsClass::passMultiwordGrammaticalParameters(GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2)
{
	if(entitySemanticRelationFunction2->grammaticalProperNounTemp)
	{
		entitySemanticRelationFunction1->grammaticalProperNounTemp = true;
	}
	if(entitySemanticRelationFunction2->grammaticalGenderTemp != GRAMMATICAL_GENDER_UNDEFINED)
	{
		entitySemanticRelationFunction1->grammaticalGenderTemp = entitySemanticRelationFunction2->grammaticalGenderTemp;
	}
}



bool GIAtranslatorOperationsClass::connectMultiwordDate(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	bool result = true;
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if(!(entitySemanticRelationFunction1->wasReference))
	{
	#endif
		//cout << "GIAtranslatorOperationsClass::connectMultiwordDate: " << endl;
		
		if(entitySemanticRelationFunction1->timeConditionNode == NULL)
		{
			entitySemanticRelationFunction1->timeConditionNode = new GIAtimeConditionNode();
			entitySemanticRelationFunction1->conditionType = CONDITION_NODE_TYPE_TIME;
			addTimeConditionProperty(entitySemanticRelationFunction1->timeConditionNode, entitySemanticRelationFunction1);
		}

		addTimeConditionProperty(entitySemanticRelationFunction1->timeConditionNode, entitySemanticRelationFunction2);

	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	}
	else
	{
		//time condition should already have been added [CHECKTHIS]
	}
	#endif
	return result;
}

bool GIAtranslatorOperationsClass::addTimeConditionProperty(GIAtimeConditionNode* timeConditionNode, const GIAentityNode* entity)
{
	bool result = false;
	string entityName = entity->entityName;
	
	if(entity->semanticRelationWordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_NUMBER)
	{
		string yearName = entityName;
		int year = SHAREDvars.convertStringToInt(yearName);
		if(year < 10000)
		{
			timeConditionNode->year = year;
			//cout << "GIAtranslatorOperationsClass::addTimeConditionProperty{} pass: year = " << year << endl;	
			result = true;
		}
		else
		{
			cerr << "GIAtxtRelTranslatorParserClass::createSemanticRelationMultiwordDate{} error: yearNameInt >= 10000" << endl;
			exit(EXIT_ERROR);
		}
	}
	else if(entity->semanticRelationWordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_DATE)
	{
		int index = INT_DEFAULT_VALUE;
		#ifndef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES_SUPPORT_UPPERCASE_PROPERNOUN_WORD_LISTS
		entityName = SHAREDvars.convertStringToFirstUpperCase(&entityName);
		#endif
		string lastTwoCharactersOfEntityName = entityName.substr(entityName.length()-TIME_DAY_OF_MONTH_APPEND_LENGTH, TIME_DAY_OF_MONTH_APPEND_LENGTH);
		if(SHAREDvars.textInTextArray(entityName, GIAtimeConditionMonthNameArray, TIME_MONTH_NUMBER_OF_TYPES, &index))
		{
			int month = index;
			timeConditionNode->month = month;
			//cout << "GIAtranslatorOperationsClass::addTimeConditionProperty{} pass: month = " << month << endl;
			result = true;
		}
		else if(SHAREDvars.textInTextArray(entityName, GIAtimeConditionDayOfWeekNameArray, TIME_DAY_OF_WEEK_NUMBER_OF_TYPES, &index))
		{
			int dayOfWeek = index;
			timeConditionNode->dayOfWeek = dayOfWeek;
			//cout << "GIAtranslatorOperationsClass::addTimeConditionProperty{} pass: dayOfWeek = " << dayOfWeek << endl;
			result = true;
		}
		else if(SHAREDvars.textInTextArray(lastTwoCharactersOfEntityName, GIAtimeConditionDayOfMonthAppendNameArray, TIME_DAY_OF_MONTH_APPEND_NUMBER_OF_TYPES, &index))
		{
			string dayOfMonthString = entityName.substr(0, entityName.length()-TIME_DAY_OF_MONTH_APPEND_LENGTH);
			int dayOfMonth = SHAREDvars.convertStringToInt(dayOfMonthString);	//CHECKTHIS; perform error checking (exploit)
			timeConditionNode->dayOfMonth = dayOfMonth;
			//cout << "GIAtranslatorOperationsClass::addTimeConditionProperty{} pass: dayOfMonth = " << dayOfMonth << endl;
			result = true;
		}	
		else
		{
			cerr << "GIAtranslatorOperationsClass::addTimeConditionProperty{} error: (entity->semanticRelationWordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_DATE) && entity name contains unknown time data: " << entityName << endl;
			exit(EXIT_ERROR);
		}
	}
	else
	{
		cerr << "GIAtranslatorOperationsClass::addTimeConditionProperty{} error: entity->semanticRelationWordPOStypeInferred unknown: " << entity->semanticRelationWordPOStypeInferred << endl;
		cerr << "GIApreprocessorPOStypeNameArray[entity->semanticRelationWordPOStypeInferred] = " << GIApreprocessorPOStypeNameArray[entity->semanticRelationWordPOStypeInferred] << endl;
		exit(EXIT_ERROR);
	}
	
	timeConditionNode->conditionName = GIAconditionNodeClass.generateDateTimeConditionName(timeConditionNode->dayOfMonth, timeConditionNode->month, timeConditionNode->year);	//will only partially fill in this info (will improve generation every time addTimeConditionProperty is executed)
	
	return result;
}
			

bool GIAtranslatorOperationsClass::connectQuantityToEntity(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	bool result = true;
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if(!(entitySemanticRelationFunction1->wasReference))
	{
	#endif
			
		if(entitySemanticRelationFunction2->semanticRelationWordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_NUMBER)
		{		
			string quantityString = entitySemanticRelationFunction2->entityName;
			int quantity = SHAREDvars.convertStringToInt(quantityString);
		 	//cout << "quantity = " << quantity << endl;
			entitySemanticRelationFunction1->hasQuantity = true;
			entitySemanticRelationFunction1->quantityNumber = quantity;
			entitySemanticRelationFunction1->quantityNumberString = quantityString;
		}
		else
		{
			cerr << "GIAtranslatorOperationsClass::connectQuantityToEntity{} error: (entitySemanticRelationFunction2->semanticRelationWordPOStypeInferred != GIA_PREPROCESSOR_POS_TYPE_NUMBER)" << endl;
			exit(EXIT_ERROR);
		}
		
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	}
	else
	{
		//time condition should already have been added [CHECKTHIS]
	}
	#endif
	return result;
}

bool GIAtranslatorOperationsClass::connectMeasureToEntity(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	bool result = true;
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if(!(entitySemanticRelationFunction1->wasReference))
	{
	#endif	
		/*
		if(entitySemanticRelationFunction2->hasQuantity)
		{	
		*/	
			entitySemanticRelationFunction2->hasMeasure = true;
			entitySemanticRelationFunction2->measureType = MEASURE_TYPE_UNKNOWN;	//FUTURE infer MEASURE_TYPE_DISTANCE/MEASURE_TYPE_SIZE/MEASURE_TYPE_TIME/MEASURE_TYPE_PER from wordlistUnitDistance/wordlistUnitSize/wordlistUnitTime or from entitySemanticRelationFunction1->entityName (e.g. "tall", "away", etc)
			
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_CONNECT_MEAURES_AS_CONDITIONS_BACKWARDS_COMPATIBILITY
			string conditionName = relationTypeMeasureNameArray[entitySemanticRelationFunction2->measureType];
			GIAentityNode* relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialCondition(entitySemanticRelationFunction1, entitySemanticRelationFunction2, conditionName, translatorVariables);
			connectConditionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, relationshipEntity, sameReferenceSet, translatorVariables);	
			#else
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
			GIAentityNode* relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
			connectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, relationshipEntity, sameReferenceSet, translatorVariables);
			#else
			connectDirectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);		
			#endif
			#endif
		/*
		}
		else
		{
			cerr << "GIAtranslatorOperationsClass::connectMeasureToEntity{} error: (!entitySemanticRelationFunction2->hasQuantity)" << endl;
			exit(EXIT_ERROR);
		}
		*/
		
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	}
	else
	{
		//time condition should already have been added [CHECKTHIS]
	}
	#endif
	return result;
}

bool GIAtranslatorOperationsClass::connectMeasurePerToEntity(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet)
{
	bool result = true;
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if(!(entitySemanticRelationFunction1->wasReference))
	{
	#endif	
		/*
		//may not yet be defined:			
		if(entitySemanticRelationFunction2->hasQuantity)
		{	
		*/
			entitySemanticRelationFunction2->hasMeasure = true;
			entitySemanticRelationFunction2->measureType = MEASURE_TYPE_PER;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_CONNECT_MEAURES_AS_CONDITIONS_BACKWARDS_COMPATIBILITY
			string conditionName = relationTypeMeasureNameArray[entitySemanticRelationFunction2->measureType];
			GIAentityNode* relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialCondition(entitySemanticRelationFunction1, entitySemanticRelationFunction2, conditionName, translatorVariables);
			connectConditionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, relationshipEntity, sameReferenceSet, translatorVariables);			
			#else
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
			GIAentityNode* relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
			connectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, relationshipEntity, sameReferenceSet, translatorVariables);
			#else
			connectDirectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);		
			#endif
			#endif
		/*
		}
		else
		{
			//cout << "entitySemanticRelationFunction1->hasQuantity = " << entitySemanticRelationFunction1->hasQuantity << endl;
			cerr << "GIAtranslatorOperationsClass::connectMeasurePerToEntity{} error: (!entitySemanticRelationFunction2->hasQuantity)" << endl;
			exit(EXIT_ERROR);
		}
		*/
		
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	}
	else
	{
		//time condition should already have been added [CHECKTHIS]
	}
	#endif
	return result;
}



#ifdef GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE
bool GIAtranslatorOperationsClass::connectLogicReferenceConjunction(GIAtranslatorVariablesClass* translatorVariables, const string logicReferenceClassType, GIAentityNode* targetEntity, GIAentityNode* logicReferenceEntity, const bool sameReferenceSet)
{
	return connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_CONJUNCTION, logicReferenceClassType, NULL, targetEntity, logicReferenceEntity, sameReferenceSet);
}
bool GIAtranslatorOperationsClass::connectLogicReferenceConclusion(GIAtranslatorVariablesClass* translatorVariables, const string logicReferenceClassType, GIAentityNode* targetEntity, GIAentityNode* logicReferenceEntity, const bool sameReferenceSet)
{
	return connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_CONCLUSION, logicReferenceClassType,  NULL, targetEntity, logicReferenceEntity, sameReferenceSet);
}
bool GIAtranslatorOperationsClass::connectLogicReference(GIAtranslatorVariablesClass* translatorVariables, const int logicReferenceClass, const string logicReferenceClassType, GIAentityNode* sourceEntity, GIAentityNode* targetEntity, GIAentityNode* logicReferenceEntity, const bool sameReferenceSet)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	cout << "GIAtxtRelTranslatorHybridClass::createLogicReference:" << endl;
	cout << "logicReferenceEntity = " << logicReferenceEntity->entityName << endl;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_DYNAMIC_RELATIONSHIP_ENTITY_TYPES
	logicReferenceEntity->entityType = GIAtxtRelTranslatorLogicReferenceClassesCrossReferenceEntityType[logicReferenceClass];	
	#else
	logicReferenceEntity->entityType = GIA_ENTITY_TYPE_LOGIC_REFERENCE;
	#endif
	logicReferenceEntity->isLogicReferenceEntity = true;
	logicReferenceEntity->logicReferenceClass = logicReferenceClass;
	logicReferenceEntity->logicReferenceClassType = logicReferenceClassType; 
	
	if(sourceEntity != NULL)
	{
		connectLogicReferenceRelationshipToSource(logicReferenceEntity, sourceEntity, sameReferenceSet, translatorVariables);
	}
	if(targetEntity != NULL)
	{
		connectLogicReferenceRelationshipToTarget(logicReferenceEntity, targetEntity, sameReferenceSet, translatorVariables);
	}
	if((sourceEntity == NULL) && (targetEntity == NULL))
	{
		cerr << "GIAtxtRelTranslatorHybridClass::connectLogicReference{} error: (sourceEntity == NULL) && (targetEntity == NULL)" << endl;
		exit(EXIT_ERROR);
	}

	return result;
}
			
void GIAtranslatorOperationsClass::connectLogicReferenceRelationshipToTarget(GIAentityNode* relationship, GIAentityNode* targetEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	//cout << "connectLogicReferenceRelationshipToTarget: relationship = " << relationship->entityName << ", targetEntity = " << targetEntity->entityName << endl; 
	int connectionTypeTargetToRelationship = generateConnectionTypeTargetToLogicReferenceRelationship(relationship);
	connectEntities(relationship, targetEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, connectionTypeTargetToRelationship, sameReferenceSet, translatorVariables);
}
void GIAtranslatorOperationsClass::connectLogicReferenceRelationshipToSource(GIAentityNode* relationship, GIAentityNode* sourceEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	//cout << "connectLogicReferenceRelationshipToSource: relationship = " << relationship->entityName << ", sourceEntity = " << sourceEntity->entityName << endl;
	int connectionTypeSourceToRelationship = generateConnectionTypeSourceToLogicReferenceRelationship(relationship);
	connectEntities(relationship, sourceEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, connectionTypeSourceToRelationship, sameReferenceSet, translatorVariables);
}

int GIAtranslatorOperationsClass::generateConnectionTypeTargetToLogicReferenceRelationship(GIAentityNode* relationship)
{
	int connectionTypeTargetToRelationship = generateConnectionTypeReverse(relationship->entityType);
	return connectionTypeTargetToRelationship;
}

int GIAtranslatorOperationsClass::generateConnectionTypeSourceToLogicReferenceRelationship(GIAentityNode* relationship)
{
	int connectionTypeTargetToRelationship = generateConnectionType(relationship->entityType);
	return connectionTypeTargetToRelationship;
}

#endif


		
/*
//based on createNewInverseConditionEntitySemantic{}
GIAentityNode* GIAsemRelTranslatorParserClass::createNewRelationshipEntitySemantic(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* relationshipEntity = addEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipEntityType, relationshipEntityName, translatorVariables);

	#ifdef GIA_SEM_REL_TRANSLATOR_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex] = new GIAfeature();
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex]->word = relationshipEntityName;
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex]->lemma = relationshipEntityName;	//is this necessary?
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex]->entityIndex = relationshipEntity->entityIndexTemp;
	#endif

	return relationshipEntity;
}
*/


#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
int GIAtranslatorOperationsClass::getEntityArrayMaxIndex(GIAtranslatorVariablesClass* translatorVariables)
{
	int numberOfWordsInSentence = translatorVariables->currentPreprocessorSentenceInList->sentenceContentsLRP.size();
	return GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(numberOfWordsInSentence);	
}
int GIAtranslatorOperationsClass::convertSentenceContentsIndexToEntityIndex(const int sentenceContentsIndex)
{
	int entityIndex = sentenceContentsIndex+GIA_NLP_START_ENTITY_INDEX;
	return entityIndex;
}
int GIAtranslatorOperationsClass::convertEntityIndexToSentenceContentsIndex(const int entityIndex)
{
	int sentenceContentsIndex = entityIndex-GIA_NLP_START_ENTITY_INDEX;
	return sentenceContentsIndex;
}
#else
int GIAtranslatorOperationsClass::getEntityArrayMaxIndex(GIAtranslatorVariablesClass* translatorVariables)
{
	return GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList);	
}
#endif

GIAentityConnection* GIAtranslatorOperationsClass::getConnection(GIAentityNode* entity, GIAentityNode* relationshipEntity)
{
	GIAentityConnection* connectionFound = NULL;
	for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
		{
			GIAentityConnection* relationship = *connectionIter;
			if(relationship->entity == relationshipEntity)
			{
				//reverse connection found
				connectionFound = relationship;
			}
		}
	}
	if(!connectionFound)
	{
		cerr << "GIAtranslatorOperationsClass::getConnection; !connectionFound " << endl;
		exit(EXIT_ERROR);
	}
	return connectionFound;
}


int GIAtranslatorOperationsClass::getCurrentSentenceIndex(GIAtranslatorVariablesClass* translatorVariables)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
	int sentenceIndex = translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal;
	#else
	int sentenceIndex = translatorVariables->currentSentenceInList->sentenceIndex;	
	#endif
	
	return sentenceIndex;
}

void GIAtranslatorOperationsClass::updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, const string auxiliaryOrCopulaString)
{
	if(SHAREDvars.textInTextArray(auxiliaryOrCopulaString, GIAtranslatorEntityAuxiliaryNotArray, GIA_TRANSLATOR_ENTITY_AUXILIARY_NOT_NUMBER_OF_TYPES))
	{
		entity->negative = true;	//assume GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS (ie !GIA_ENTITY_CONNECTION_NEGATIVE_DEFINED)
	}
	
	for(int i=0; i<GIA_SEM_REL_TRANSLATOR_RELATION_AUXILIARY_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(auxiliaryOrCopulaString == relationAuxiliaryPastTenseNameArray[i])
		{
			entity->grammaticalTenseTemp = GRAMMATICAL_TENSE_PAST;
		}
	}
	for(int i=0; i<GIA_SEM_REL_TRANSLATOR_RELATION_AUXILIARY_FUTURE_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(auxiliaryOrCopulaString == relationAuxiliaryFutureTenseNameArray[i])
		{
			entity->grammaticalTenseTemp = GRAMMATICAL_TENSE_FUTURE;
		}
	}
}


