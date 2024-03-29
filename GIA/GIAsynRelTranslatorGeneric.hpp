 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsynRelTranslatorGeneric.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SYN_REL_TRANSLATOR_GENERIC
#define HEADER_GIA_SYN_REL_TRANSLATOR_GENERIC

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
#include "GIAsemRelTranslatorOperations.hpp"
#endif
//#include "GIAsynRelTranslatorDefs.hpp"








#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION

#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_undefined -1
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition 1
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addActionToActionDefinition 2
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectPropertyToEntity 4
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectActionToEntity 5
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectActionToSubject 6
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectActionToObject 7
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectConditionToEntity 8
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectBeingDefinitionToEntity 9
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectHavingPropertyToEntity 10
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntity 11
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAlias 12
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAliasWithAuxiliary 13

#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityWithAuxiliary 15
#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectPropertyToEntityWithAuxiliary 16

#define GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition 1
#define GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addActionToActionDefinition 2
#define GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addTenseOnlyTimeConditionToSubstance 20

//#define GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectPrenominalModifierToEntity 21	//not currently used; relies on GIArules.xml to be defined to execute the desired functions upon detection of nn syntactical relation


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
	GIAgenericDepRelInterpretationParameters(GIAtranslatorVariablesClass* translatorVariablesNew, bool executeOrReassignNew, bool linkPreestablishedReferencesGIAnew);
	~GIAgenericDepRelInterpretationParameters(void);

		//general parameters
	GIAtranslatorVariablesClass translatorVariables;

		//execute function based on relations content or redistribute entities within relations
	bool executeOrReassign;

	bool linkPreestablishedReferencesGIA;

	//for relation1, relation2, relation3, and relation4 [GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS]:
		//for entity1 (eg substanceEntity), entity2 (eg relationshipObjectEntity, propertyRelationshipObjectEntity), and entity3/intermediaryEntity (eg conditionRelationshipEntity, actionRelationshipEntity) [3]:

		//relations to parse
	int numberOfRelations;
	bool parseDisabledRelation[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	bool parseDisabledRelationDuringLink[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];

		//found values
	GIArelation* relation[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	string relationEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	int relationEntityIndex[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool relationEntityPrepFound[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
		//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
	string relationEntityOriginal[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	int relationEntityIndexOriginal[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
		//for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation:
	GIArelation* relationFinalResult[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	string relationEntityFinalResult[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//warning: will contain latest match if multiple matches found
	int relationEntityIndexFinalResult[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//warning: will contain latest match if multiple matches found

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
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsTestOr2Vector[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//if 2 separate OR statements are required, eg for cases such as ((thingEntity->isSubstance || thingEntity->isNameQuery) && (definitionRelationshipObjectEntity->isSubstance || definitionRelationshipObjectEntity->isNameQuery))
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
	bool useRedistributeSpecialCaseRelationEntityNameReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];				//not used often
	int redistributeSpecialCaseRelationEntityNameReassignmentRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];			//not used often
	int redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often
	bool redistributeSpecialCaseRelationEntityNameReassignmentUseOriginalValues[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often
	bool useRedistributeSpecialCaseDisableInstanceAndNetworkIndex[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	vector<GIAentityCharacteristic*> specialCaseCharacteristicsAssignmentVector[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool useSpecialCaseCharacteristicsRelationEntityIndexReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];			//not used often (UNTESTED)
	int specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often (UNTESTED)
	int specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];		//not used often (UNTESTED)
	GIAentityCharacteristic specialCaseCharacteristicsRelationEntityIndexReassignment[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//not used often (UNTESTED)

	bool useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];	//added 2j7a

		//for execution
	int defaultSameSetRelationID;
	bool defaultSameSetReferenceValue;
	int functionEntityRelationID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION];		//for entity1, entity2, and entity3 [and entity4 for special tests] - relation1, relation2, relation3, or relation4
	int functionEntityRelationEntityID[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION];	//for entity1, entity2, and entity3 [and entity4 for special tests] - relationType, relationGovernorIndex, or relationDependentIndex
	int functionToExecuteUponFind;
		//special cases
	bool mustGenerateConditionName;
	string conditionEntityDefaultName;

		//for cleanup
	bool disableEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];
	bool disableEntityUseOriginalValues[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//for disabling an entity based on its original index
	bool enableEntity[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS][GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION];	//added GIA 2f12a/13-July-2014
	bool disableRelation[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];
	bool disableRelationDuringLink[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];

	string functionName;

	bool useRedistributeSpecialCaseNonExistantRelationCheck[GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS];	//non existant relations tests - added GIA 2f12a/13-July-2014

	#ifdef GIA_PREDETERMINERS

	#endif
};

#endif

#ifdef GIA_GENERIC_ENTITY_INTERPRETATION
class GIAgenericEntityInterpretationParameters
{
public:
	GIAgenericEntityInterpretationParameters(GIAtranslatorVariablesClass* translatorVariablesNew, bool executeOrReassignNew, bool linkPreestablishedReferencesGIAnew);
	~GIAgenericEntityInterpretationParameters(void);

		//general parameters
	GIAtranslatorVariablesClass translatorVariables;

		//execute function based on relations content or redistribute entities within relations
	bool executeOrReassign;

	bool linkPreestablishedReferencesGIA;

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



class GIAsynRelTranslatorGenericClass
{
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	#endif
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION	
	public: void initialiseBoolArray1D(bool* boolArray, const int size, int value);
	public: void initialiseBoolArray2D(bool* boolArray, const int size1, const int size2, int value);
	public: void initialiseIntArray1D(int* intArray, const int size, int value);
	public: void initialiseIntArray2D(int* intArray, const int size1, const int size2, int value);
	public: void initialiseIntArray3D(int* intArray, const int size1, const int size2, const int size3, int value);
	public: bool genericDependecyRelationInterpretation(GIAgenericDepRelInterpretationParameters* param, int currentRelationID);
	#endif
	
	#ifdef GIA_GENERIC_ENTITY_INTERPRETATION
	public: bool genericEntityInterpretation(GIAgenericEntityInterpretationParameters* param);
	#endif

	public: bool determineFeatureIndexOfPreposition(GIAsentence* currentSentenceInList, const GIArelation* prepositionRelation, int* indexOfPreposition);
	
	//GIAentityNode* addRelationshipToEntityAndGenerateSemanticRelations(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables, bool isArtificial, int relationshipSubjectEntityIndex, int relationshipObjectEntityIndex);
};

#endif
