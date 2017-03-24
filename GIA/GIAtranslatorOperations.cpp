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
 * File Name: GIAtranslatorOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1u 26-February-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorOperations.hpp"
#ifdef GIA_NLC_INTEGRATION
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
#endif
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
#endif

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
	NLPfeatureParser = GIA_NLP_FEATURE_PARSER_FOR_INPUT_TEXT_DEFAULT;
	NLPdependencyRelationsParser = GIA_NLP_RELATIONS_PARSER_FOR_INPUT_TEXT_DEFAULT;
	NLPdependencyRelationsType = dependencyRelationsTypes[NLPdependencyRelationsParser];
	NLPassumePreCollapsedStanfordRelations = false;
	parseGIA2file = false;
	
	//network variables;
	//entityNodesActiveListNetworkIndexes = NULL;
	//entityNodesActiveListComplete = NULL;
	currentEntityNodeIDInNetworkIndexEntityNodesList = 0;
	currentEntityNodeIDInCompleteList = 0;
	//entityNodesActiveListSentences = NULL;
	//timeConditionNodesActiveList;

	firstParagraphInList = NULL;
	firstSentenceInList = NULL;
	
	//temporary sentence parsing variables (!parsedSentences only):
	saveNetwork = false;
	currentSentenceInList = NULL;
	//GIAentityNodeArrayFilled = NULL;
	//GIAnetworkIndexNodeArray = NULL;
	//GIAfeatureTempEntityNodeArray = NULL;
	//GIAentityNodeArray = NULL;
	
	//sentenceNetworkIndexEntityNodesList = NULL;
	//sentenceTimeConditionNodesList = NULL;
	
	//featureArrayTemp = NULL;
	
	sentenceIndex = INT_DEFAULT_VALUE;

	#ifdef GIA_PREPROCESSOR_SENTENCE
	firstGIApreprocessorSentenceInList = NULL;
	#endif
	
	#ifdef USE_CE	
	firstCodeextensionInHeirachy = NULL;
	//codeextensionsList = NULL;
	useCodeextensionsHeirachy = false;
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



int GIAtranslatorOperationsClass::generateConnectionType(int relationshipEntityType)
{
	int connectionType = entityTypesCrossReferenceEntityVectorConnectionArray[relationshipEntityType];
	if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)
	{
		cout << "generateConnectionType error{}: (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)" << endl;
	}
	return connectionType;
}

int GIAtranslatorOperationsClass::generateConnectionTypeReverse(int relationshipEntityType)
{
	int connectionType = entityTypesCrossReferenceEntityVectorConnectionReverseArray[relationshipEntityType];
	if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)
	{
		cout << "generateConnectionType error{}: (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN)" << endl;
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
			cout << "GIAtranslatorOperationsClass::generateInvertedConnectionType{} error: (connectionTypeInverted == GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN) && !(entityTypesIsRelationshipArray[entity->entityType])" << endl;
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
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType])
	{
		cout << "!GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAtranslatorOperationsClass::getRelationshipObjectEntity error{}: entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType]" << endl;
		exit(EXIT_ERROR);		
	}
	#endif
	
	if(relationshipEntity->relationshipObjectEntity->empty())
	{
		//DEBUG only; note this should never be the case (if a property/definition relationship source is defined, then its target should be defined)
		cout << "GIAtranslatorOperationsClass::getRelationshipObjectEntity error{}: relationshipEntity->relationshipObjectEntity->empty()" << endl;
		exit(EXIT_ERROR);
	}
	GIAentityNode* objectEntity = ((relationshipEntity->relationshipObjectEntity)->back())->entity;
	return objectEntity;
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
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType])
	{
		cout << "!GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAtranslatorOperationsClass::getRelationshipSubjectEntity error{}: entityTypesIsPropertyOrDefinitionRelationshipArray[relationshipEntity->entityType]" << endl;
		exit(EXIT_ERROR);		
	}
	#endif
	
	if(relationshipEntity->relationshipSubjectEntity->empty())
	{
		//DEBUG only; note this should never be the case (if a property/definition relationship source is defined, then its target should be defined)
		cout << "GIAtranslatorOperationsClass::getRelationshipSubjectEntity error{}: relationshipEntity->relationshipSubjectEntity->empty()" << endl;
		exit(EXIT_ERROR);
	}
	GIAentityNode* subjectEntity = ((relationshipEntity->relationshipSubjectEntity)->back())->entity;
	return subjectEntity;
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

#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
bool GIAtranslatorOperationsClass::connectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = connectDefinitionToEntity(definitionRelationshipSubjectEntity, definitionRelationshipObjectEntity, definitionRelationshipEntity, sameReferenceSet, translatorVariables);
	definitionRelationshipEntity->isAlias = true;
	return result;
}
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
	
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipSubjectEntity->disabled))
	{
	if(!(relationshipObjectEntity->disabled))
	{
	#endif
		
		#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
		this->setRelationshipObjectToSubstanceIfNecessary(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType);
		#endif
	
		GIAentityConnection* connection1 = NULL;
		GIAentityConnection* connection2 = NULL;
		
		if(relationshipEntityType == GIA_ENTITY_TYPE_PROPERTY)
		{
			connection1 = this->writeVectorConnection(relationshipSubjectEntity, relationshipObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, sameReferenceSet, translatorVariables);
			connection2 = this->writeVectorConnection(relationshipObjectEntity, relationshipSubjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, sameReferenceSet, translatorVariables);
		}
		else if(relationshipEntityType == GIA_ENTITY_TYPE_DEFINITION)
		{
			connection1 = this->writeVectorConnection(relationshipSubjectEntity, relationshipObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, sameReferenceSet, translatorVariables);
			connection2 = this->writeVectorConnection(relationshipObjectEntity, relationshipSubjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, sameReferenceSet, translatorVariables);
			
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
			cout << "GIAtranslatorOperationsClass::connectDirectRelationshipToEntity{} error: illegal relationshipEntityType; relationshipEntityType = " << relationshipEntityType << endl;
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

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
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

bool GIAtranslatorOperationsClass::connectActionToSubject(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToSubject(actionRelationshipSubjectEntity, actionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_ACTION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectActionToObject(GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToObject(actionRelationshipObjectEntity, actionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_ACTION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectConditionToEntity(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = connectRelationshipToEntity(conditionRelationshipSubjectEntity, conditionRelationshipObjectEntity, conditionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DETECT_PREPOSITION_TYPE
	GIApreprocessorMultiwordReduction.identifyConditionType(conditionRelationshipEntity);
	#endif
	
	return result;
}

bool GIAtranslatorOperationsClass::connectConditionToSubject(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToSubject(conditionRelationshipSubjectEntity, conditionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
}

bool GIAtranslatorOperationsClass::connectConditionToObject(GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	return connectRelationshipToObject(conditionRelationshipObjectEntity, conditionRelationshipEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
}









bool GIAtranslatorOperationsClass::connectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipSubjectEntity->disabled))
	{
	if(!(relationshipObjectEntity->disabled))
	{
	if(!(relationshipEntity->disabled))
	{
	#endif
		
		relationshipEntity->entityType = relationshipEntityType;
		
		#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
		this->setRelationshipObjectToSubstanceIfNecessary(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType);
		#endif
	
		//configure entity node containing this substance
		this->connectRelationshipInstanceToSubject(relationshipSubjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);
		this->connectRelationshipInstanceToObject(relationshipObjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);


	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	}
	#endif
	
	return result;
}

bool GIAtranslatorOperationsClass::connectRelationshipToSubject(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipSubjectEntity->disabled))
	{
	if(!(relationshipEntity->disabled))
	{
	#endif
		relationshipEntity->entityType = relationshipEntityType;
		
		this->connectRelationshipInstanceToSubject(relationshipSubjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	}
	}
	#endif
	
	return result;
}

bool GIAtranslatorOperationsClass::connectRelationshipToObject(GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(relationshipObjectEntity->disabled))
	{
	if(!(relationshipEntity->disabled))
	{
	#endif
		relationshipEntity->entityType = relationshipEntityType;
		
		this->connectRelationshipInstanceToObject(relationshipObjectEntity, relationshipEntity, sameReferenceSet, relationshipEntityType, translatorVariables);

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
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
		if(this->findEntityNodeNameInVector(relationshipSubjectEntity, &(relationshipEntity->entityName), connectionType, &connectionFound))
		{
			GIAentityNode* currentRelationshipNodeInList = connectionFound->entity;
			GIAentityConnection* connectionFound2 = NULL;
			if(this->findEntityNodePointerInVector(currentRelationshipNodeInList, relationshipObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, &connectionFound2))
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
	this->connectEntities(subjectEntity, newOrExistingRelationship, connectionType, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, sameReferenceSet, translatorVariables);
}

void GIAtranslatorOperationsClass::connectRelationshipInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingRelationship, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	//configure relationship object entity node
	int connectionTypeReverse = generateConnectionTypeReverse(relationshipEntityType);
	this->connectEntities(objectEntity, newOrExistingRelationship, connectionTypeReverse, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, sameReferenceSet, translatorVariables);
}

void GIAtranslatorOperationsClass::connectEntities(GIAentityNode* entity1, GIAentityNode* entity2, const int connectionType, const int connectionTypeInverse, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	this->writeVectorConnection(entity1, entity2, connectionType, sameReferenceSet, translatorVariables);
	this->writeVectorConnection(entity2, entity1, connectionTypeInverse, sameReferenceSet, translatorVariables);
}








#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
//this is no longer required with redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition, etc..
bool GIAtranslatorOperationsClass::isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(const GIArelation* currentRelationInList, GIAentityNode* GIAentityNodeArray[], int relationGovernorIndex, const int NLPdependencyRelationsType)
{
	bool result = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES_ADVANCED
		if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD) && (GIAentityNodeArray[relationGovernorIndex]->entityName == RELATION_ENTITY_BE))
		{//added condition Fri 27 Jan - remove 'be' node artefacts
			this->disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[relationGovernorIndex]);
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
bool GIAtranslatorOperationsClass::isAdjectiveNotConnectedToObjectOrSubject(const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInList, const int NLPdependencyRelationsType)
{
	bool passed2 = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
		{
			const GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
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



GIAentityNode* GIAtranslatorOperationsClass::addInstanceToInstanceDefinition(GIAentityNode* entity, int instanceType, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* newOrExistingInstance = entity;

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
	if(!(entity->disabled))
	{
	#endif
		if(entity->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
		{
			newOrExistingInstance = this->addInstance(entity, instanceType, translatorVariables);
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

	#ifdef GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES
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

	this->addInstanceEntityNodeToActiveLists(newInstance, translatorVariables);
	
	newInstance->entityName = entity->entityName;
	#ifdef GIA_WORD_ORIG
	newInstance->wordOrig = entity->wordOrig;
	#endif
	newInstance->idInstance = this->determineNextIdInstance(entity);

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

	this->forwardInfoToNewSubstance(entity, newInstance);
	
	this->writeVectorConnection(newInstance, entity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN, translatorVariables);
	this->writeVectorConnection(entity, newInstance, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN, translatorVariables);

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
	newSubstance->grammaticalDefiniteTemp = entity->grammaticalDefiniteTemp;	//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalIndefinitePluralTemp = entity->grammaticalIndefinitePluralTemp;	//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalProperNounTemp = entity->grammaticalProperNounTemp;	//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->grammaticalGenderTemp = entity->grammaticalGenderTemp;		//not currently used
	newSubstance->grammaticalPronounTemp = entity->grammaticalPronounTemp;		//must forward grammatical info for GIAtranslatorDefineSubstances.cpp post substance declaration modifications (ie defineConcepts)
	newSubstance->mustSetIsConceptBasedOnApposRelation = entity->mustSetIsConceptBasedOnApposRelation;
	#endif
	#ifdef GIA_PREDETERMINERS
	newSubstance->grammaticalPredeterminerTemp = entity->grammaticalPredeterminerTemp;
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
	newSubstance->grammaticalPredeterminerTempSentenceArray.insert(make_pair(entity->sentenceIndexTemp, entity->grammaticalPredeterminerTemp));
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
	#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
	cout << "\nnewSubstance->entityName = " << newSubstance->entityName << endl;
	cout << "newSubstance->entityIndexTemp = " << newSubstance->entityIndexTemp << endl;
	//cout << "newSubstance->sentenceIndexTemp = " << newSubstance->sentenceIndexTemp << endl;
	#endif

	#ifdef GIA_NUMBER_OF
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
	
	#ifdef GIA_EXPLETIVES
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
			entity->firstSentenceToAppearInNetwork = false;
		}
	}
	else
	{
		entity->firstSentenceToAppearInNetwork	= true;
	}
}

void GIAtranslatorOperationsClass::disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	if(entity->firstSentenceToAppearInNetwork)
	{
		entity->disabled = true;
	}
	#endif
}

void GIAtranslatorOperationsClass::disableEntity(GIAentityNode* entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	entity->disabled = true;
	#endif
}


void GIAtranslatorOperationsClass::disableNetworkIndexEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	this->disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entity);	//CHECKTHIS; only disable the networkIndex if it was created in the immediate context (eg sentence)

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
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
	entity->disabled = true;

	if(!(entity->instanceReverseNodeList->empty()))
	{
		//CHECKTHIS; only disable the networkIndex if it was created in the immediate context (eg sentence)
		GIAentityNode* networkIndexEntity = this->getPrimaryNetworkIndexNodeDefiningInstance(entity);

		this->disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(networkIndexEntity);
	}
	#endif
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

void GIAtranslatorOperationsClass::convertRelexPOStypeToWordnetWordType(const string* relexPOStype, int* grammaticalWordTypeTemp)
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

void GIAtranslatorOperationsClass::convertStanfordPOStagToRelexPOStypeAndWordnetWordType(const string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp)
{
	*relexPOStype = FEATURE_RELEX_POS_TYPE_WORD;
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL; i++)
	{
		if(featurePOStagMinimalArray[i] == *POStag)
		{
			*relexPOStype = featurePOStagCrossReferenceRelexPOStypeArray[i];
		}
	}

	this->convertRelexPOStypeToWordnetWordType(relexPOStype, grammaticalWordTypeTemp);

	#ifdef GIA_WORDNET_DEBUG
	/*
	cout << "convertStanfordPOStagToRelexPOStypeAndWordnetWordType{}: " << endl;
	cout << "POStag = " <<* POStag << endl;
	cout << "relexPOStype = " <<* relexPOStype << endl;
	cout << "grammaticalWordTypeTemp = " <<* grammaticalWordTypeTemp << endl;
	*/
	#endif

}

void GIAtranslatorOperationsClass::generateTempFeatureArray(GIAfeature* firstFeatureInList, GIAfeature* featureArrayTemp[])
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
		cout << "GIAtranslatorOperationsClass::addRelationshipArtificialToEntity{} error: unsupported relationshipEntityType; relationshipEntityType = " << relationshipEntityType << endl;
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

GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* relationshipEntity = NULL;
	if(!findExistingRelationshipInSentenceEntityArray(relationshipSubjectEntity, relationshipObjectEntity, relationshipEntityType, &relationshipEntity, translatorVariables))
	{
		relationshipEntity = addEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipEntityType, relationshipEntityName, translatorVariables);
	}
	
	return relationshipEntity;	
}

bool GIAtranslatorOperationsClass::findExistingRelationshipInSentenceEntityArray(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, GIAentityNode** relationshipEntity, GIAtranslatorVariablesClass* translatorVariables)
{
	//if !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS; this function can only be used for relationshipEntityType == GIA_ENTITY_TYPE_CONDITION
		
	//NB an artificial or non-artificial relationship may have been created in a previous sentence
	
	bool foundExistingRelationship = false;
	int connectionType = generateConnectionType(relationshipEntityType);
	int connectionTypeReverse = generateConnectionTypeReverse(relationshipEntityType);
	for(vector<GIAentityConnection*>::iterator connectionIter = relationshipSubjectEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != relationshipSubjectEntity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if(getRelationshipObjectEntity((*connectionIter)) == relationshipObjectEntity)
		{
			*relationshipEntity = (*connectionIter)->entity;
			foundExistingRelationship = true;
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
	translatorVariables->GIAentityNodeArray[relationshipEntityIndex] = relationshipEntity;
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	relationshipEntity->isArtificialAuxiliary = true;
	#endif
	
	return relationshipEntity;	
}



GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(int featureIndex, const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* conditionRelationshipEntity;
	#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
	if(translatorVariables->GIAentityNodeArrayFilled[featureIndex])
	{
		conditionRelationshipEntity = translatorVariables->GIAentityNodeArray[featureIndex];
	}
	else
	{
		conditionRelationshipEntity = this->findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship(featureIndex, entityNodeName, translatorVariables);
	}
	#else
	conditionRelationshipEntity = this->findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, translatorVariables);	
	#endif
	
	//added 3a1j;
	#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
	conditionRelationshipEntity->entityIndexTemp = featureIndex;	//added 3a1n (for (translatorVariables->GIAentityNodeArrayFilled[featureIndex]) and/or !GIA_ADVANCED_REFERENCING_CONDITIONS cases) 
	conditionRelationshipEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	#endif
	conditionRelationshipEntity = addInstanceToInstanceDefinition(conditionRelationshipEntity, GIA_ENTITY_TYPE_CONDITION, translatorVariables);
	translatorVariables->GIAentityNodeArray[featureIndex] = conditionRelationshipEntity;
	
	return conditionRelationshipEntity;
}

GIAentityNode* GIAtranslatorOperationsClass::findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship(int featureIndex, const string* entityNodeName, GIAtranslatorVariablesClass* translatorVariables)
{
	bool entityAlreadyExistant = false;
	GIAentityNode* relationshipEntity = this->findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, &entityAlreadyExistant, translatorVariables);
	translatorVariables->GIAentityNodeArrayFilled[featureIndex] = true;
	translatorVariables->GIAentityNodeArray[featureIndex] = relationshipEntity;
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
	GIAentityNode* relationshipEntity = findOrAddEntityNodeByNameSimpleWrapperRelationship2(relationshipEntityName, translatorVariablesSentencesParsed, true);
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
		cout << "GIAtranslatorOperationsClass::addRelationshipToEntity2{} error: unsupported relationshipEntityType; relationshipEntityType = " << relationshipEntityType << endl;
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

GIAentityNode* GIAtranslatorOperationsClass::findOrAddEntityNodeByNameSimpleWrapperRelationship2(const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed, bool isArtificial)
{
	bool entityAlreadyExistant = false;
	GIAentityNode* relationshipEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&relationshipEntityName, &entityAlreadyExistant, translatorVariablesSentencesParsed);
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(isArtificial)
	{
		relationshipEntity->isArtificialAuxiliary = true;	
	}
	#endif
	
	return relationshipEntity;
}





GIAentityNode* GIAtranslatorOperationsClass::findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables)
{
	return this->findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, translatorVariables, true);
}

GIAentityNode* GIAtranslatorOperationsClass::findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables, const bool tempEntityEnabled)
{
	GIAentityNode* entityNodeFound = NULL;

	entityNodeFound = GIAdatabase.findOrAddNetworkIndexEntityNodeByName(translatorVariables->entityNodesActiveListComplete, translatorVariables->entityNodesActiveListNetworkIndexes, entityNodeName, entityAlreadyExistant, true, &(translatorVariables->currentEntityNodeIDInCompleteList), &(translatorVariables->currentEntityNodeIDInNetworkIndexEntityNodesList), translatorVariables->saveNetwork);
	
	this->applyNetworkIndexEntityAlreadyExistsFunction(entityNodeFound, *entityAlreadyExistant, tempEntityEnabled);

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
		if(!this->findSameSentenceEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &connectionFound2, translatorVariables->sentenceIndex))
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
			if(this->findEntityNodePointerInVector(entityNode, entityNodeToAdd, connectionType, &connectionFound3))
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
					cout << "error: writeVectorConnection called, but entityVectorConnectionsReferenceListLoadedArray set to false" << endl;
					cout << "entityNode = " << entityNode->entityName << ", entityNodeToAdd = " << entityNodeToAdd->entityName << ", connectionType = " << connectionType << endl;
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
		if(!(entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE].empty()))
		{
			//the current entity is a substance of a networkIndex entity
			networkIndexEntity = this->getPrimaryNetworkIndexNodeDefiningInstance(entity);
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

//#if defined(GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS) || !defined(GIA_DATABASE)
//check #elif !defined GIA_DATABASE is OK instead of #elif !defined(GIA_DATABASE)
#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].empty()))
	{
		long previousIdInstance = networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].back()->entity->idInstance;
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
	long numberOfInstances =  networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].size();
	nextIdInstance = numberOfInstances;
	#endif
#elif !defined GIA_DATABASE
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].empty()))
	{
		long previousIdInstance = networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].back()->entity->idInstance;
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
	long numberOfInstances =  networkIndexEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].size();
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
bool GIAtranslatorOperationsClass::mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	if(entityNode == entityNodeToMerge)	//before 3a1o: (entityNode->idActiveList == entityNodeToMerge->idActiveList)
	{
		#ifdef GIA_ALIASES_DEBUG
		cout << "treatDefinitionAsEquality: already merged" << endl;
		#endif
		result = false;
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

		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNodeToMerge->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNodeToMerge->entityVectorConnectionsArray[connectionType].end(); )
			{
				bool connectionIterErased = false;
				//connect entityNodeToMerge ambient node to entityNode
				GIAentityNode* entityConnectedToEntityToMerge = (*connectionIter)->entity;

				#ifdef GIA_ALIASES_DEBUG
				if(entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
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
					int connectionTypeInverted = generateInvertedConnectionType(entityConnectedToEntityToMerge, connectionType);
					for(vector<GIAentityConnection*>::iterator connectionIter2 = entityConnectedToEntityToMerge->entityVectorConnectionsArray[connectionTypeInverted].begin(); connectionIter2 != entityConnectedToEntityToMerge->entityVectorConnectionsArray[connectionTypeInverted].end(); )
					{
						bool connectionIter2Erased = false;
						GIAentityNode* entityConnectedToEntityConnectedToEntityToMerge = (*connectionIter2)->entity;

						#ifdef GIA_ALIASES_DEBUG
						if(entityConnectedToEntityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
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
							#ifndef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
							//the commenting out of the below case is required for advanced referencing (eg networkIndex Tom has associated instance Dog) [NB this means that instances can appear to have more than one instanceReverseNodeList]
							if((entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE))
							{//restored 29 November 2012, and condition (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE) added
								//disconnect entityConnectedToEntityConnectedToEntityToMerge from entityConnectedToEntityToMerge (networkIndex) (z2)
								#ifdef GIA_ALIASES_DEBUG
								cout << "disconnect entityConnectedToEntityConnectedToEntityToMerge (" << entityConnectedToEntityConnectedToEntityToMerge->entityName << ") from entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (networkIndex) (z2)" << endl;
								#endif
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
								#ifdef GIA_ALIASES_DEBUG
								cout << "change entityConnectedToEntityConnectedToEntityToMerge (" << entityConnectedToEntityConnectedToEntityToMerge->entityName << ") to entityNode (" << entityNode->entityName << ") (z)" << endl;
								#endif

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
						#ifdef GIA_ALIASES_DEBUG
						cout << "connect entityNode (" << entityNode->entityName << ") to entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (x)" << endl;
						#endif
						bool sameReferenceSet = (*connectionIter)->sameReferenceSet;
						this->writeVectorConnection(entityNode, entityConnectedToEntityToMerge, connectionType, sameReferenceSet, translatorVariables);
					#ifndef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
					}
					#endif
				}

				//disconnect entityConnectedToEntityToMerge from entityNodeToMerge
				#ifdef GIA_ALIASES_DEBUG
				if(entityConnectedToEntityToMerge->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
				{
					cout << "disconnect entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") (networkIndex) from entityNodeToMerge (" << entityNodeToMerge->entityName << ") (x2)" << endl;
				}
				else
				{
					cout << "disconnect entityConnectedToEntityToMerge (" << entityConnectedToEntityToMerge->entityName << ") from entityNodeToMerge (" << entityNodeToMerge->entityName << ") (x2)" << endl;
				}
				#endif
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
			#ifdef GIA_DEBUG
			//cout << "entityNodeToMerge->isQuery = " << entityNodeToMerge->isQuery << endl;
			#endif
			entityNode->isQuery = entityNodeToMerge->isQuery;
			entityNode->isWhichOrEquivalentWhatQuery = entityNodeToMerge->isWhichOrEquivalentWhatQuery;
			#ifdef GIA_ALIASES
			entityNode->isNameQuery = entityNodeToMerge->isNameQuery;
			#endif
		}
		#ifdef GIA_ALIASES
		entityNode->isName = entityNodeToMerge->isName;
		#endif

		if(entityNodeToMerge->isToBeComplimentOfActionTemp)
		{
			entityNode->isToBeComplimentOfActionTemp = true;	//should not be required
		}

		this->disableEntity(entityNodeToMerge);

		//entityNode->entityIndexTemp = entityNodeToMerge->entityIndexTemp;	//added 3a1o

		#ifdef GIA_ALIASES_DEBUG
		cout << "finished: mergeEntityNodesAddAlias" << endl;
		cout << "entityNode->entityName = " << entityNode->entityName << endl;
		cout << "entityNode->aliasList[0] = " << entityNode->aliasList[0] << endl;
		cout << "entityNode->entityIndexTemp = " << entityNode->entityIndexTemp << endl;
		cout << "entityNodeToMerge->entityIndexTemp = " << entityNodeToMerge->entityIndexTemp << endl;
		#endif
	}
	return result;
}
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
			#ifdef GIA_DEBUG
			//cout << "primaryNetworkIndexNodeDefiningInstance = networkIndexEntityNode" << endl;
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
		NLCpreprocessorSentence* currentNLCsentenceInList = this->getFirstNLCsentenceInListGIA();
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

			#ifdef GIA_DEBUG
			cout << "definiteEntity = " << definiteEntity->entityName << endl;
			cout << "indefiniteEntity = " << indefiniteEntity->entityName << endl;
			#endif
			
			if(foundDefiniteEntitySentence)
			{
				if(minimumIndentationBetweenIndefiniteAndIndefiniteEntitySentence < indefiniteEntityNLCsentenceInList->indentation)
				{
				       #ifdef GIA_DEBUG
				       //cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA{}: no reference found" << endl;
				       #endif
				}
				else
				{
				       #ifdef GIA_DEBUG
				       //cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA{}: entity declared in this function" << endl;
				       #endif
				       foundIndefiniteEntity = true;
				       *indentationDifferenceFound = definiteEntityNLCsentenceInList->indentation - indefiniteEntityNLCsentenceInList->indentation;
				}
			}
			else
			{
			       #ifdef GIA_DEBUG
			       //cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA{} error: !foundDefiniteEntitySentence" << endl;
			       #endif
			}
		}
		else
		{
			#ifdef GIA_DEBUG
			//cout << "checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA{} error: !foundIndefiniteEntitySentence" << endl;
			#endif
		}
	}

	return foundIndefiniteEntity;
}
#endif
#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
bool GIAtranslatorOperationsClass::checkIfSentenceIsMathTextParsablePhrase(const GIAsentence* currentSentenceInList)
{
	bool sentenceIsMathTextParsablePhrase = false;
	NLCpreprocessorSentence* firstNLCsentenceInList = this->getFirstNLCsentenceInListGIA();
	NLCpreprocessorSentence* sentence = NULL;
	if(NLCpreprocessorSentenceClass.getSentenceInSentenceList(currentSentenceInList->sentenceIndex, firstNLCsentenceInList, &sentence)) 
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
	GIAentityNode* nonspecificConcept = this->addInstanceToInstanceDefinition(networkIndexEntity, GIA_ENTITY_TYPE_CONCEPT, translatorVariables);
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
		if(this->isNonspecificConceptEntity(conceptEntity))
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
		if(this->isNonspecificConceptEntity(conceptEntity))
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

