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
 * File Name: GIAtranslatorGeneric.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2n5a 30-September-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_GENERIC
#define HEADER_GIA_TRANSLATOR_GENERIC

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
//#include "GIAtranslatorDefs.h"








#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION

#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_undefined -1
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition 1
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addActionToActionDefinition 2
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty 3
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity 4
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToEntity 5
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToSubject 6
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToObject 7
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity 8
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectBeingDefinitionConditionToEntity 9
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectHavingPropertyConditionToEntity 10
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntity 11
#ifdef GIA_SUPPORT_ALIASES
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_mergeEntityNodesAddAlias 12
#endif
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
//#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToSubject 13
//#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToObject 14
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addAuxiliaryToEntity 13
#endif
//#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntityMarkConnectionAsAlias 14
//#endif

#define GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition 1
#define GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addActionToActionDefinition 2
#define GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addTenseOnlyTimeConditionToSubstance 20


#define REL1 (0)	//GIA_GENERIC_DEP_REL_INTERP_REL
#define REL2 (1)
#define REL3 (2)
#define REL4 (3)
#define REL5 (4)
#define GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS (5)
#define REL_ENT1 (0)		//for test/redistribution - governor
#define REL_ENT2 (1)		//for test/redistribution - dependent
#define REL_ENT3 (2)		//for test/redistribution - type
#define GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION (3)
#define GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION (2)
#define FUNC_ENT1 (0)		//for function execution - primary
#define FUNC_ENT2 (1)		//for function execution - secondary
#define FUNC_ENT3 (2)		//for function execution - intermediary
#define FUNC_ENT4_SPECIAL (3)		//for function specific special value determinations
#define GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION (4)
#define FUNCTION_ENTITY_RELATION_ID_NONEXISTANT_MUST_GENERATE (-9999)
#define FUNCTION_ENTITY_RELATION_ENTITY_ID_NONEXISTANT_MUST_GENERATE (-9999)


class GIAgenericDepRelInterpretationParameters
{
public:
	GIAgenericDepRelInterpretationParameters(GIAsentence* newcurrentSentenceInList, bool newGIAentityNodeArrayFilled[], GIAentityNode* newGIAentityNodeArray[], bool executeOrReassign);
	~GIAgenericDepRelInterpretationParameters(void);

		//general parameters
	GIAsentence* currentSentenceInList;
	bool* GIAentityNodeArrayFilled;
	GIAentityNode** GIAentityNodeArray;
	unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes;
	int NLPdependencyRelationsType;

		//execute function based on relations content or redistribute entities within relations
	bool executeOrReassign;

	//for relation1, relation2, relation3, and relation4 [GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS]:
		//for entity1 (eg substanceEntity), entity2 (eg conditionObjectEntity, propertyEntity), and entity3/intermediaryEntity (eg conditionEntity, actionEntity) [3]:

		//relations to parse
	int numberOfRelations;
	bool parseDisabledRelation[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	bool parseDisabledRelationDuringLink[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];

		//found values
	GIArelation* relation[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	string relationEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	int relationEntityIndex[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	#else
	int relationEntityIndex[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION];
	#endif
	bool relationEntityPrepFound[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
		//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
	string relationEntityOriginal[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	int relationEntityIndexOriginal[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	#else
	int relationEntityIndexOriginal[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION];
	#endif
		//for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation:
	GIArelation* relationFinalResult[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	string relationEntityFinalResult[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//warning: will contain latest match if multiple matches found
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	int relationEntityIndexFinalResult[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//warning: will contain latest match if multiple matches found
	#else
	int relationEntityIndexFinalResult[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION];
	#endif

		//predefined values tests
	bool useRelationTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	string relationTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool relationTestIsNegative[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool useRelationArrayTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//if !useRelationTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY], then useRelationArrayTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY] (as a relation type test must be applied for each parsed relation)
	string* relationArrayTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	int relationArrayTestSize[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool relationArrayTestIsNegative[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool expectToFindPrepositionTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
		//special cases
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsTestAndVector[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsTestOrVector[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsTestOr2Vector[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//if 2 separate OR statements are required, eg for cases such as ((thingEntity->isSubstance || thingEntity->isNameQuery) && (definitionEntity->isSubstance || definitionEntity->isNameQuery))
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsTestOr3Vector[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//if 3 separate OR statements are required, eg for cases such as (thingEntity->grammaticalProperNounTemp || thingEntity->isNameQuery)

		//entity index match tests
	bool useRelationIndexTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	int relationIndexTestRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	int relationIndexTestEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
		//NB for relationType tests use relationType as indicies are not available
	bool relationIndexTestIsNegative[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
		//special cases
	bool useSpecialCaseCharacteristicsRelationIndexTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];			//not used often (UNTESTED)
	int specialCaseCharacteristicsRelationIndexTestRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often (UNTESTED)
	int specialCaseCharacteristicsRelationIndexTestEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often (UNTESTED)
	GIAentityCharacteristic specialCaseCharacteristicsRelationIndexTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//not used often (UNTESTED)

		//for redistribution
	bool useRedistributeRelationEntityIndexReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];				//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - for reassigning relation entities
	int redistributeRelationEntityIndexReassignmentRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];			//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relation1, relation2, relation3, or relation4 - for reassigning relation entities
	int redistributeRelationEntityIndexReassignmentRelationEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for reassigning relation entities
	bool redistributeRelationEntityIndexReassignmentUseOriginalValues[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - for reassigning relation entities
	bool useRedistributeRelationEntityReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];					//for entity1, entity2, and entity3 - for renaming relation entities
	string redistributeRelationEntityReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];					//for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for renaming relation entities
		//special cases
	bool useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	bool useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSet[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	bool useRedistributeSpecialCaseRcmodIndicatesSameReferenceSet[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	bool useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	int redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION][2];
	int redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION][2];
	#ifdef GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES
	int redistributeSpecialCaseRelationEntityReassignmentConcatonateType[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	#endif
	bool useRedistributeSpecialCaseDisableInstanceAndNetworkIndex[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsAssignmentVector[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool useSpecialCaseCharacteristicsRelationEntityIndexReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];			//not used often (UNTESTED)
	int specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often (UNTESTED)
	int specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often (UNTESTED)
	GIAentityCharacteristic specialCaseCharacteristicsRelationEntityIndexReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//not used often (UNTESTED)

	bool useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];	//added 2j7a

		//for execution
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	int defaultSameSetRelationID;
	bool defaultSameSetReferenceValue;
	#endif
	int functionEntityRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION];		//for entity1, entity2, and entity3 [and entity4 for special tests] - relation1, relation2, relation3, or relation4
	int functionEntityRelationEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION];	//for entity1, entity2, and entity3 [and entity4 for special tests] - relationType, relationGovernorIndex, or relationDependentIndex
	int functionToExecuteUponFind;
		//special cases
	bool mustGenerateConditionName;
	string conditionEntityDefaultName;

		//for cleanup
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	bool disableEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool disableEntityUseOriginalValues[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//for disabling an entity based on its original index
	bool enableEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//added GIA 2f12a/13-July-2014
	#else
	bool disableEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION];		//for entity1 and entity2
	bool disableEntityUseOriginalValues[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION];	//for disabling an entity based on its original index
	bool enableEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION];	//added GIA 2f12a/13-July-2014
	#endif
	bool disableRelation[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	bool disableRelationDuringLink[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];

	string functionName;

	bool useRedistributeSpecialCaseNonExistantRelationCheck[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];	//non existant relations tests - added GIA 2f12a/13-July-2014

	#ifdef GIA_SUPPORT_PREDETERMINERS

	#endif
};

#endif

#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
class GIAgenericEntityInterpretationParameters
{
public:
	GIAgenericEntityInterpretationParameters(GIAsentence* newcurrentSentenceInList, bool newGIAentityNodeArrayFilled[], GIAentityNode* newGIAentityNodeArray[], bool executeOrReassign);
	~GIAgenericEntityInterpretationParameters(void);

		//general parameters
	GIAsentence* currentSentenceInList;
	bool* GIAentityNodeArrayFilled;
	GIAentityNode** GIAentityNodeArray;

		//execute function based on relations content or redistribute entities within relations
	bool executeOrReassign;

		//relations to parse
	bool parseDisabledEntity;

		//predefined values tests
	bool useEntityTest;
	string entityTest;
	bool entityTestIsNegative;
	bool useEntityArrayTest;
	string* entityArrayTest;
	int entityArrayTestSize;
	bool entityArrayTestIsNegative;
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsTestAndVector;
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsTestOrVector;

		//for redistribution
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsAssignmentVector;

		//for execution
	int functionToExecuteUponFind;

		//for cleanup
	bool disableEntity;

	string functionName;
};

#endif


#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION

void initialiseBoolArray1D(bool* boolArray, int size, int value);
void initialiseBoolArray2D(bool* boolArray, int size1, int size2, int value);
void initialiseIntArray1D(int* intArray, int size, int value);
void initialiseIntArray2D(int* intArray, int size1, int size2, int value);
void initialiseIntArray3D(int* intArray, int size1, int size2, int size3, int value);

bool genericDependecyRelationInterpretation(GIAgenericDepRelInterpretationParameters* param, int currentRelationID);

#endif

#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
bool genericEntityInterpretation(GIAgenericEntityInterpretationParameters* param);
#endif

bool determineFeatureIndexOfPreposition(GIAsentence* currentSentenceInList, GIArelation* prepositionRelation, int* indexOfPreposition);

#endif
