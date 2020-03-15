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
 * File Name: GIAsynRelTranslatorGeneric.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k3a 10-March-2020
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAsynRelTranslatorGeneric.hpp"






#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION

GIAgenericDepRelInterpretationParameters::GIAgenericDepRelInterpretationParameters(GIAtranslatorVariablesClass* translatorVariablesNew, bool executeOrReassignNew, bool linkPreestablishedReferencesGIAnew)
{
	translatorVariables = *translatorVariablesNew;

	executeOrReassign = executeOrReassignNew;

	linkPreestablishedReferencesGIA = linkPreestablishedReferencesGIAnew;

	//for relation1, relation2, relation3, and relation4 [GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS]:
		//for entity1 (eg substanceEntity), entity2 (eg relationshipObjectEntity, propertyRelationshipObjectEntity), and entity3/intermediaryEntity (eg conditionRelationshipEntity, actionRelationshipEntity) [3]:

		//relations to parse
	numberOfRelations = 1;
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(parseDisabledRelation, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(parseDisabledRelationDuringLink, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);	//not currently used

		//found values
	//NO LONGER INITIALISED: relation = {NULL, NULL, NULL, NULL};
	//relationEntity = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&relationEntityIndex[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(relationEntityPrepFound, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
		//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
	//relationEntityOriginal = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&relationEntityIndexOriginal[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
		//for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation{}:
	//relationFinalResult = {NULL, NULL, NULL, NULL, NULL};
	//relationEntityFinalResult = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&relationEntityIndexFinalResult[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);

		//predefined values tests
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRelationTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	//relationTest = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}};	//internal compiler error: Segmentation fault
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&relationTestIsNegative[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRelationArrayTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//if !useRelationTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY], then useRelationArrayTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY] (as a relation type test must be applied for each parsed relation)
	//NO LONGER INITIALISED: relationArrayTest = {{NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}};
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&relationArrayTestSize[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&relationArrayTestIsNegative[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(expectToFindPrepositionTest, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);

		//entity index match tests
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRelationIndexTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&relationIndexTestRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&relationIndexTestEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
		//NB for relationType tests use relationType as indicies are not available
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&relationIndexTestIsNegative[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
		//special cases
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useSpecialCaseCharacteristicsRelationIndexTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//not used often
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&specialCaseCharacteristicsRelationIndexTestRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);		//not used often
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&specialCaseCharacteristicsRelationIndexTestEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);		//not used often
	//specialCaseCharacteristicsRelationIndexTest initialisation not required

		//for redistribution
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRedistributeRelationEntityIndexReassignment[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);			//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - for reassigning relation entities
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&redistributeRelationEntityIndexReassignmentRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);						//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relation1, relation2, relation3, or relation4 - for reassigning relation entities
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&redistributeRelationEntityIndexReassignmentRelationEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);						//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for reassigning relation entities
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&redistributeRelationEntityIndexReassignmentUseOriginalValues[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRedistributeRelationEntityReassignment[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false); 			//for entity1, entity2, and entity3 - for renaming relation entities
	//redistributeRelationEntityReassignment = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault		//for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for renaming relation entities
		//special cases for redistribution
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSet, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(useRedistributeSpecialCaseRcmodIndicatesSameReferenceSet, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	GIAsynRelTranslatorGenericClass().initialiseIntArray3D(&redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[0][0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, 2, INT_DEFAULT_VALUE);
	GIAsynRelTranslatorGenericClass().initialiseIntArray3D(&redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[0][0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, 2, INT_DEFAULT_VALUE);
	#ifdef GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&redistributeSpecialCaseRelationEntityReassignmentConcatonateType[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	#endif
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRedistributeSpecialCaseRelationEntityNameReassignment[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&redistributeSpecialCaseRelationEntityNameReassignmentRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&redistributeSpecialCaseRelationEntityNameReassignmentUseOriginalValues[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useRedistributeSpecialCaseDisableInstanceAndNetworkIndex[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&useSpecialCaseCharacteristicsRelationEntityIndexReassignment[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false); 	//not used often
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);					//not used often
	GIAsynRelTranslatorGenericClass().initialiseIntArray2D(&specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);				//not used often	//initialisation added 2j7a
	//specialCaseCharacteristicsRelationEntityIndexReassignment initialisation not required

	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);

		//for execution
	defaultSameSetRelationID = INT_DEFAULT_VALUE;
	defaultSameSetReferenceValue = false;
	GIAsynRelTranslatorGenericClass().initialiseIntArray1D(functionEntityRelationID, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION, REL1);				//these dummy values have to be set always to prevent crash //for entity1, entity2, and entity3 - relation1, relation2, relation3, or relation4
	GIAsynRelTranslatorGenericClass().initialiseIntArray1D(functionEntityRelationEntityID, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION, FUNC_ENT1);	//these dummy values have to be set always to prevent crash //for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex
	functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_undefined;
		//special cases
	mustGenerateConditionName = false;
	conditionEntityDefaultName = "";

	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&disableEntity[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&disableEntityUseOriginalValues[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false); 	//for disabling an entity based on its original index
	GIAsynRelTranslatorGenericClass().initialiseBoolArray2D(&enableEntity[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//added GIA 2f12a 13-July-2014
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(disableRelation, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(disableRelationDuringLink, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);

	functionName = "";

	GIAsynRelTranslatorGenericClass().initialiseBoolArray1D(useRedistributeSpecialCaseNonExistantRelationCheck, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);	//non existant relations tests - added GIA 2f12a 13-July-2014
}
GIAgenericDepRelInterpretationParameters::~GIAgenericDepRelInterpretationParameters(void)
{
}

void GIAsynRelTranslatorGenericClass::initialiseBoolArray1D(bool* boolArray, const int size, int value)
{
	for(int i=0; i<size; i++)
	{
		boolArray[i] = value;
	}
}
void GIAsynRelTranslatorGenericClass::initialiseBoolArray2D(bool* boolArray, const int size1, const int size2, int value)
{
	for(int i=0; i<size1; i++)
	{
		for(int j=0; j<size2; j++)
		{
			boolArray[i*size2 + j] = value;
		}
	}
}
void GIAsynRelTranslatorGenericClass::initialiseIntArray1D(int* intArray, const int size, int value)
{
	for(int i=0; i<size; i++)
	{
		intArray[i] = value;
	}
}
void GIAsynRelTranslatorGenericClass::initialiseIntArray2D(int* intArray, const int size1, const int size2, int value)
{
	for(int i=0; i<size1; i++)
	{
		for(int j=0; j<size2; j++)
		{
			intArray[i*size2 + j] = value;
		}
	}
}
void GIAsynRelTranslatorGenericClass::initialiseIntArray3D(int* intArray, const int size1, const int size2, const int size3, int value)
{
	for(int i=0; i<size1; i++)
	{
		for(int j=0; j<size2; j++)
		{
			for(int k=0; k<size3; k++)
			{
				intArray[i*size2*size3 + j*size3 + k] = value;
			}
		}
	}
}

bool GIAsynRelTranslatorGenericClass::genericDependecyRelationInterpretation(GIAgenericDepRelInterpretationParameters* param, int currentRelationID)
{
	bool result = false;

	GIArelation* currentRelationInList = param->translatorVariables.currentSentenceInList->firstRelationInList;
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
			param->relationEntity[currentRelationID][REL_ENT3] = GIAtranslatorOperations.convertPrepositionToRelex(&(param->relationEntity[currentRelationID][REL_ENT3]), &(param->relationEntityPrepFound[currentRelationID]));	//convert stanford prep_x to relex x
			param->relationEntityIndex[currentRelationID][REL_ENT1] = param->relation[currentRelationID]->relationGovernorIndex;
			param->relationEntityIndex[currentRelationID][REL_ENT2] = param->relation[currentRelationID]->relationDependentIndex;
			param->relationEntityIndex[currentRelationID][REL_ENT3] = param->relation[currentRelationID]->relationTypeIndex;

				//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
			param->relationEntityOriginal[currentRelationID][REL_ENT1] = param->relationEntity[currentRelationID][REL_ENT1];
			param->relationEntityOriginal[currentRelationID][REL_ENT2] = param->relationEntity[currentRelationID][REL_ENT2];
			param->relationEntityOriginal[currentRelationID][REL_ENT3] = param->relationEntity[currentRelationID][REL_ENT3];
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT1] = param->relationEntityIndex[currentRelationID][REL_ENT1];
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT2] = param->relationEntityIndex[currentRelationID][REL_ENT2];
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT3] = param->relationEntityIndex[currentRelationID][REL_ENT3];	//added GIA 2c2a
			if(param->expectToFindPrepositionTest[currentRelationID])
			{
				if(!(param->relationEntityPrepFound[currentRelationID]))
				{
					passedRelation = false;
				}
			}
			if(param->useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest[currentRelationID])
			{
				if(param->relation[currentRelationID]->rcmodIndicatesSameReferenceSet)
				{
					passedRelation = false;		//"that"/"which" connection found
				}

			}
			for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
			{
				if(!GIAentityNodeClass.testEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestAndVector[currentRelationID][relationEntityID]), true))
				{
					passedRelation = false;
				}
				if(!GIAentityNodeClass.testEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestOrVector[currentRelationID][relationEntityID]), false))
				{
					passedRelation = false;
				}
				if(!GIAentityNodeClass.testEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestOr2Vector[currentRelationID][relationEntityID]), false))
				{
					passedRelation = false;
				}
				if(!GIAentityNodeClass.testEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestOr3Vector[currentRelationID][relationEntityID]), false))
				{
					passedRelation = false;
				}
				if(param->useRelationTest[currentRelationID][relationEntityID])
				{

					if(passedRelation)
					{

						passedRelation = false;
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
				if(param->useRelationArrayTest[currentRelationID][relationEntityID])
				{
					if(passedRelation)
					{
						passedRelation = false;
						bool foundAnArrayPass = false;
						//int relationArrayTestSize = sizeof((param->relationArrayTest[currentRelationID][relationEntityID]))/sizeof((param->relationArrayTest[currentRelationID][relationEntityID])[0]);

						for(int j=0; j<param->relationArrayTestSize[currentRelationID][relationEntityID]; j++)
						{
							if(param->relationEntity[currentRelationID][relationEntityID] == (param->relationArrayTest[currentRelationID][relationEntityID])[j])
							{
								passedRelation = true;
								foundAnArrayPass = true;
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
				if(currentRelationID < (param->numberOfRelations - 1))
				{
					GIAgenericDepRelInterpretationParameters paramTemp = *param;	//this shouldnt be required anymore with relationFinalResult/relationEntityFinalResult/relationEntityIndexFinalResult... 	//only record parameters (eg relationEntity/relationEntityIndex) if successfully recused - this is required if additional commands are required to be executed based on the successful (result==true) recursion of genericDependecyRelationInterpretation (e.g. in GIAsynRelTranslatorRedistributeRelationsStanford.cpp)
					if(genericDependecyRelationInterpretation(&paramTemp, (currentRelationID+1)))
					{
						result = true;
						*param = paramTemp;	//this shouldnt be required anymore with relationFinalResult/relationEntityFinalResult/relationEntityIndexFinalResult... 	//only record parameters (eg relationEntity/relationEntityIndex) if successfully recused - this is required if additional commands are required to be executed based on the successful (result==true) recursion of genericDependecyRelationInterpretation (e.g. in GIAsynRelTranslatorRedistributeRelationsStanford.cpp)
					}
				}
				else
				{
					bool passedEntityMatchTests = true;
					//entity index match tests

					int minRelationToTest = 0;
					int maxRelationToTest = param->numberOfRelations;
					bool specialCaseRelationCheck = false;
					if(currentRelationID == param->numberOfRelations)
					{
						specialCaseRelationCheck = true;
						minRelationToTest = param->numberOfRelations;
						maxRelationToTest = param->numberOfRelations+1;
					}
					for(int relationID=minRelationToTest; relationID<maxRelationToTest; relationID++)
					{
						for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)	//changed from GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION 25 July 2013
						{
							if(param->useRelationIndexTest[relationID][relationEntityID])
							{
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
							if(param->useSpecialCaseCharacteristicsRelationIndexTest[relationID][relationEntityID])
							{//not used often
								passedEntityMatchTests = false;
								GIAentityNode* currentEntity = (*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]];
								GIAentityNode* targetEntity = (*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[param->specialCaseCharacteristicsRelationIndexTestRelationID[relationID][relationEntityID]][param->specialCaseCharacteristicsRelationIndexTestEntityID[relationID][relationEntityID]]];
								GIAentityCharacteristic* entityCharacteristic = &(param->specialCaseCharacteristicsRelationIndexTest[relationID][relationEntityID]);
								GIAentityNodeClass.getEntityCharacteristic(currentEntity, entityCharacteristic);
								if(GIAentityNodeClass.testEntityCharacteristic(targetEntity, entityCharacteristic))
								{
									passedEntityMatchTests = true;
								}
							}
						}
					}
					if(param->useRedistributeSpecialCaseNonExistantRelationCheck[currentRelationID])
					{//non existant relations tests - added GIA 2f12a 13-July-2014
						GIAgenericDepRelInterpretationParameters paramTemp = *param;
						if(genericDependecyRelationInterpretation(&paramTemp, (currentRelationID+1)))
						{
							passedEntityMatchTests = false;
						}
					}

					if(passedEntityMatchTests)
					{

						result = true;

						//record final values for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation:
						for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
						{
							param->relationFinalResult[relationID] = param->relation[relationID];
							for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
							{
								param->relationEntityFinalResult[relationID][relationEntityID] = param->relationEntity[relationID][relationEntityID];
								param->relationEntityIndexFinalResult[relationID][relationEntityID] = param->relationEntityIndex[relationID][relationEntityID];
							}
						}

						if(!specialCaseRelationCheck)
						{

							//special case reference set checks
							for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
							{
								if(param->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[relationID])
								{
									bool auxiliaryIndicatesDifferentReferenceSet = true;
									bool rcmodIndicatesSameReferenceSet = false;
									GIAgenericDepRelInterpretationParameters paramTemp = *param;
									if(genericDependecyRelationInterpretation(&paramTemp, (currentRelationID+1)))
									{
										rcmodIndicatesSameReferenceSet = true;
									}
									param->relation[relationID]->auxiliaryIndicatesDifferentReferenceSet = auxiliaryIndicatesDifferentReferenceSet;
									param->relation[relationID]->rcmodIndicatesSameReferenceSet = rcmodIndicatesSameReferenceSet;
								}
								if(param->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSet[relationID])
								{
									param->relation[relationID]->auxiliaryIndicatesDifferentReferenceSet = true;
								}
								if(param->useRedistributeSpecialCaseRcmodIndicatesSameReferenceSet[relationID])
								{
									param->relation[relationID]->rcmodIndicatesSameReferenceSet = true;
								}
							}

							if(param->executeOrReassign)
							{//execute
								bool sameReferenceSet;
								//not setting defaultSameSetRelationID (ie leaving it at INT_DEFAULT_VALUE) will enforce the sameReferenceSet value to be the defaultSameSetReferenceValue regardless of auxiliaryIndicatesDifferentReferenceSet and rcmodIndicatesSameReferenceSet
								if(param->defaultSameSetRelationID == INT_DEFAULT_VALUE)
								{
									sameReferenceSet = param->defaultSameSetReferenceValue;
								}
								else
								{
									sameReferenceSet = GIAtranslatorOperations.determineSameReferenceSetValue(param->defaultSameSetReferenceValue, param->relation[param->defaultSameSetRelationID]);
								}

								int functionEntityIndex1 = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT1]][param->functionEntityRelationEntityID[FUNC_ENT1]];
								int functionEntityIndex2 = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT2]][param->functionEntityRelationEntityID[FUNC_ENT2]];
								int functionEntityIndex3 = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];
								int functionEntityIndex4special = param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT4_SPECIAL]][param->functionEntityRelationEntityID[FUNC_ENT4_SPECIAL]];

							

								//if(param->functionToExecuteUponFind is a condition....
								if((param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectConditionToEntity))
								{//if a condition is required to be created
									string conditionEntityName = "";
									bool mustLookupOrGenerateConditionNetworkIndexEntity = false;
									if(param->mustGenerateConditionName)
									{
										conditionEntityName = param->conditionEntityDefaultName;
										functionEntityIndex3 = param->translatorVariables.currentSentenceInList->relationshipEntityArtificialIndexCurrent;
										param->translatorVariables.currentSentenceInList->relationshipEntityArtificialIndexCurrent = param->translatorVariables.currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;
										mustLookupOrGenerateConditionNetworkIndexEntity = true;
									}
									else
									{
										if(param->conditionEntityDefaultName != "")
										{//special case where condition is required to be changed from the preposition name (not often used)
											conditionEntityName = param->conditionEntityDefaultName;
										}
										else
										{
											conditionEntityName = param->relationEntity[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];
										}

										if(param->functionEntityRelationEntityID[FUNC_ENT3] == REL_ENT3)	//condition is being defined as a relation's relationType and has no obvious networkIndex entity. However a relationType is often a preposition - if so it will already have a feature index (determined by determineFeatureIndexOfPreposition()) [of which there might even be a corresponding entity name/index defined depending upon how the dependency relations were generated by the NLP: see GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition() below for information on how this case is handled]
										{
											functionEntityIndex3 = param->relation[param->functionEntityRelationID[FUNC_ENT3]]->relationTypeIndex;	//CHECKTHIS
											if(functionEntityIndex3 == INT_DEFAULT_VALUE)
											{
												functionEntityIndex3 = param->translatorVariables.currentSentenceInList->relationshipEntityArtificialIndexCurrent;
												param->translatorVariables.currentSentenceInList->relationshipEntityArtificialIndexCurrent = param->translatorVariables.currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;
											}
											mustLookupOrGenerateConditionNetworkIndexEntity = true;
										}
										else
										{
											//functionEntityIndex3 will already be set correctly	[param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];]
										}
									}
									if(mustLookupOrGenerateConditionNetworkIndexEntity)
									{
										//NB if networkIndex type entity name has already been defined (GIAentityNodeArrayFilled[functionEntityIndex3]), then findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition will use it instead
										bool entityAlreadyExistant = false;
										GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(functionEntityIndex3, &conditionEntityName, &entityAlreadyExistant, &(param->translatorVariables));
									}
								}
								#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
								else if((param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectPropertyToEntity) ||
								(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntity) ||
								(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAlias))
								{									
									//create intermediary auxiliary 'have'/'be' (property/definition relationship entity) 
									string relationshipEntityName = "";
									GIAentityNode* relationshipEntity = NULL;
									if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectPropertyToEntity)
									{
										relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], &(param->translatorVariables));
									}
									else if((param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntity) || (param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAlias))
									{
										relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], &(param->translatorVariables));
									}
									functionEntityIndex3 = relationshipEntity->entityIndexTemp;
									//GIAentityNode* conditionNetworkIndexEntity = above
								}
								#endif
								

								if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition)
								{
									GIAtranslatorOperations.addInstanceToInstanceDefinition((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], GIA_ENTITY_TYPE_SUBSTANCE, &(param->translatorVariables));
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addActionToActionDefinition)
								{
									GIAtranslatorOperations.addInstanceToInstanceDefinition((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], GIA_ENTITY_TYPE_ACTION, &(param->translatorVariables));
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectPropertyToEntity)
								{
									#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
									GIAtranslatorOperations.connectPropertyToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));
									#else
									GIAtranslatorOperations.connectDirectPropertyToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], sameReferenceSet, &(param->translatorVariables));									
									#endif
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectPropertyToEntityWithAuxiliary)
								{
									#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
									GIAtranslatorOperations.connectPropertyToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));
									#else
									GIAtranslatorOperations.connectDirectPropertyToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));
									//GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3]);
									GIAtranslatorOperations.disableEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3]);
									#endif
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectActionToEntity)
								{
									//cout << "GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectActionToEntity" << endl;
									//bool sameReferenceSetSubject = IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									//bool sameReferenceSetObject = IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									if(param->mustGenerateConditionName)
									{
										//sameReferenceSetSubject = sameReferenceSet;
										//sameReferenceSetObject = sameReferenceSet;
									}
									else
									{
										bool subjectFound = false;
										for(int i=0; i<GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
										{
											if(param->relation[REL1]->relationType == relationTypeSubjectNameArray[i])
											{
												subjectFound = true;
											}
										}
										if(!subjectFound)
										{
											cout << "error: connectActionToEntity && (relation[REL1]->relationType != ~GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_SUBJECT)" << endl;
										}
										bool objectFound = false;
										for(int i=0; i<GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
										{
											if(param->relation[REL2]->relationType == relationTypeObjectNameArray[i])
											{
												objectFound = true;
											}
										}
										if(!objectFound)
										{
											cout << "error: connectActionToEntity && (relation[REL1]->relationType != ~GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_OBJECT)" << endl;
										}
										sameReferenceSet = GIAtranslatorOperations.determineSameReferenceSetValue(param->defaultSameSetReferenceValue, param->relation[REL1]);	//or param->relation[REL2]
									}

									GIAtranslatorOperations.connectActionToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectActionToSubject)
								{
									GIAtranslatorOperations.connectActionToSubject((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], sameReferenceSet, &(param->translatorVariables));
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectActionToObject)
								{
									GIAtranslatorOperations.connectActionToObject((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], sameReferenceSet, &(param->translatorVariables));
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectConditionToEntity)
								{

									GIAtranslatorOperations.connectConditionToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));

									#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED	//CHECKTHIS
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									if(!(currentRelationInList->inverseRelationTwoWay))	//added 2j5e
									{
										#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
										if(currentRelationInList->inverseRelationSingle)	//added 2j5g
										{
											GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex2, currentRelationInList->relationTypeIndexNonInversed, sameReferenceSet);	//use original (non-inversed) values
											GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex1, currentRelationInList->relationTypeIndexNonInversed, sameReferenceSet);	//use original (non-inversed) values
										}
										else
										{
										#endif
											GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
											GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
										#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
										}
										#endif
									}
									#endif
									#endif
									#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
									if(currentRelationInList->relationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectConditionToEntity is called based on a single GIArelation
									{
										(*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3]->conditionTwoWay = true;	//sets conditionTwoWay for condition substance not networkIndex
									}
									#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
									if(currentRelationInList->inverseRelationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectConditionToEntity is called based on a single GIArelation
									{
										(*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3]->inverseConditionTwoWay = true;	//sets inverseConditionTwoWay for condition substance not networkIndex
									}
									#endif
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectBeingDefinitionToEntity)
								{
									#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
									//CHECKTHIS:  (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1] 
									GIAtranslatorOperations.connectBeingDefinitionToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], sameReferenceSet, &(param->translatorVariables));
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
									#else
									cout << "genericDependecyRelationInterpretation{} warning: GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectBeingDefinitionToEntity is not supported by !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS" << endl;
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectHavingPropertyToEntity)
								{
									#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
									//CHECKTHIS:  (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1] 
									GIAtranslatorOperations.connectHavingPropertyToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], sameReferenceSet, &(param->translatorVariables));
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
									#else
									cout << "genericDependecyRelationInterpretation{} warning: GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectHavingPropertyToEntity is not supported by !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS" << endl;
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntity)
								{
									#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
									GIAtranslatorOperations.connectDefinitionToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));
									#else
									GIAtranslatorOperations.connectDirectDefinitionToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], sameReferenceSet, &(param->translatorVariables));									
									#endif
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);	
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityWithAuxiliary)
								{
									#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
									GIAtranslatorOperations.connectDefinitionToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));
									#else
									GIAtranslatorOperations.connectDirectDefinitionToEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2], (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet, &(param->translatorVariables));					
									//GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3]);
									GIAtranslatorOperations.disableEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3]);
									#endif
									#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
									GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);		
									#endif
								}
								#ifdef GIA_ALIASES
								else if((param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAlias) || (param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAliasWithAuxiliary))
								{
									//TODO: verify that param->linkPreestablishedReferencesGIA == translatorVariables->linkPreestablishedReferencesGIA
									if(!GIAtranslatorOperations.connectDefinitionAliasWrapper(&(param->translatorVariables), (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex1], &((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex2]), (*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3], sameReferenceSet))
									{
										result = false;
									}
									
									#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
									GIAtranslatorOperations.disableEntity((*param->translatorVariables.GIAentityNodeArray)[functionEntityIndex3]);
									#endif
									
									if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAlias)
									{
										#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
										GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT_ALIAS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
										#endif
									}
									else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_connectDefinitionToEntityAliasWithAuxiliary)
									{
										#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
										GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_ALIAS, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
										GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(&(param->translatorVariables), GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE_ALIAS, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);		
										#endif
									}
								}
								#endif
								else
								{	
									bool exitProgram = true;
									if(exitProgram)
									{
										cerr << "illegal functionToExecuteUponFind: " << param->functionToExecuteUponFind << endl;
										exit(EXIT_ERROR);
									}
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
												newPrepositionName = newPrepositionName + GIA_SYN_REL_TRANSLATOR_STANFORD_PARSER_PREPOSITION_PREPEND + param->relationEntity[relationID][relationEntityID];
												param->relation[relationID]->relationType = newPrepositionName;
												param->relation[relationID]->relationTypeIndex = param->relationEntityIndex[relationID][relationEntityID];
											}
										}
										if(param->useSpecialCaseCharacteristicsRelationEntityIndexReassignment[relationID][relationEntityID])
										{//not used often
											GIAentityNode* currentEntity = (*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]];
											GIAentityNode* targetEntity = (*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[param->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[relationID][relationEntityID]][param->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[relationID][relationEntityID]]];
											GIAentityCharacteristic* entityCharacteristic = &(param->specialCaseCharacteristicsRelationEntityIndexReassignment[relationID][relationEntityID]);
											GIAentityNodeClass.getEntityCharacteristic(targetEntity, entityCharacteristic);
											GIAentityNodeClass.setEntityCharacteristic(currentEntity, entityCharacteristic);
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
												(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
											}
											else if(relationEntityID == REL_ENT2)
											{
												param->relation[relationID]->relationDependent = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
											}
											else if(relationEntityID == REL_ENT3)
											{
												param->relation[relationID]->relationType = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												//do not overwrite relationTypeIndex: param->relation[relationID]->relationTypeIndex = INT_DEFAULT_VALUE;
												if(param->relationEntityIndex[relationID][relationEntityID] != INT_DEFAULT_VALUE)
												{//preposition entity has been initialised
													(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												}
											}
										}
									}
								}
								//special case reassignments
								for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
								{
									for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
									{
										if(param->useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[relationID][relationEntityID])
										{
											string concatonatedEntityNamePart1 = param->relationEntity[param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[relationID][relationEntityID][0]][param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[relationID][relationEntityID][0]];
											string concatonatedEntityNamePart2 = param->relationEntity[param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[relationID][relationEntityID][1]][param->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[relationID][relationEntityID][1]];
											#ifdef GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES
											string concatonatedEntityName = "";
											/*//OPTIONAL (equivalent code):
											if(param->redistributeSpecialCaseRelationEntityReassignmentConcatonateType[relationID][relationEntityID]] == GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS)
											{
												concatonatedEntityName = createSubClassEntityName(concatonatedEntityNamePart1, concatonatedEntityNamePart2);
											}
											else
											{
											*/
												string concatonatedTypeDelimiter = concatenationTypesArray[param->redistributeSpecialCaseRelationEntityReassignmentConcatonateType[relationID][relationEntityID]];
												concatonatedEntityName = concatonatedEntityNamePart1 + concatonatedTypeDelimiter + concatonatedEntityNamePart2;
											/*
											}
											*/
											#else
											string concatonatedEntityName = concatonatedEntityNamePart1 + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER + concatonatedEntityNamePart2;
											#endif

											if(relationEntityID == REL_ENT1)
											{
												param->relation[relationID]->relationGovernor = concatonatedEntityName;		//probably not required
												(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = concatonatedEntityName;
											}
											else if(relationEntityID == REL_ENT2)
											{
												param->relation[relationID]->relationDependent = concatonatedEntityName;		//probably not required
												(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = concatonatedEntityName;
											}
											else if(relationEntityID == REL_ENT3)
											{//governor/dependent can become preposition
												string newPrepositionName = "";
												newPrepositionName = newPrepositionName + GIA_SYN_REL_TRANSLATOR_STANFORD_PARSER_PREPOSITION_PREPEND + concatonatedEntityName;
												param->relation[relationID]->relationType = newPrepositionName;
												/*
												#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW_ASSIGN_RELATIONTYPEINDEX_FOR_CONCATONATIONS
												relationTypeIndex: param->relation[relationID]->relationTypeIndex = ?;
												#endif
												*/
												if(param->relationEntityIndex[relationID][relationEntityID] != INT_DEFAULT_VALUE)
												{//preposition entity has been initialised
													(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = concatonatedEntityName;
												}
											}
										}
									}
								}
								for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
								{
									for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
									{
										if(param->useRedistributeSpecialCaseRelationEntityNameReassignment[relationID][relationEntityID])
										{
											//reassign entity values
											if(param->redistributeSpecialCaseRelationEntityNameReassignmentUseOriginalValues[relationID][relationEntityID])
											{
												param->relationEntity[relationID][relationEntityID] = param->relationEntityOriginal[param->redistributeSpecialCaseRelationEntityNameReassignmentRelationID[relationID][relationEntityID]][param->redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID[relationID][relationEntityID]];
											}
											else
											{
												param->relationEntity[relationID][relationEntityID] = param->relationEntity[param->redistributeSpecialCaseRelationEntityNameReassignmentRelationID[relationID][relationEntityID]][param->redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID[relationID][relationEntityID]];
											}

											if(relationEntityID == REL_ENT1)
											{
												param->relation[relationID]->relationGovernor = param->relationEntity[relationID][relationEntityID];
												(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->relationEntity[relationID][relationEntityID];
											}
											else if(relationEntityID == REL_ENT2)
											{
												param->relation[relationID]->relationDependent = param->relationEntity[relationID][relationEntityID];
												(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->relationEntity[relationID][relationEntityID];
											}
											else if(relationEntityID == REL_ENT3)
											{//governor/dependent can become preposition
												string newPrepositionName = "";
												newPrepositionName = newPrepositionName + GIA_SYN_REL_TRANSLATOR_STANFORD_PARSER_PREPOSITION_PREPEND + param->relationEntity[relationID][relationEntityID];
												param->relation[relationID]->relationType = newPrepositionName;
												if(param->relationEntityIndex[relationID][relationEntityID] != INT_DEFAULT_VALUE)
												{//preposition entity has been initialised
													(*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->relationEntity[relationID][relationEntityID];
												}
											}
										}
									}
								}
							}

							//special case assignments
							for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
							{
								for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
								{
									if(relationID < param->numberOfRelations)
									{
										GIAentityNodeClass.setEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]], &(param->specialCaseCharacteristicsAssignmentVector[relationID][relationEntityID]));
									}
								}
							}

							//for cleanup
							for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
							{
								for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
								{
									if(param->disableEntity[relationID][relationEntityID])
									{
										GIAentityNode* oldRedundantEntity;
										if(param->relationEntityIndexOriginal[relationID][relationEntityID] != INT_DEFAULT_VALUE)	//check whether attempting to disable a non-preposition (and therefore non-existant) relationType entity
										{
											if(param->disableEntityUseOriginalValues[relationID][relationEntityID])
											{
												oldRedundantEntity = (*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndexOriginal[relationID][relationEntityID]];
											}
											else
											{
												oldRedundantEntity = (*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndex[relationID][relationEntityID]];
											}
											if((param->executeOrReassign) || param->useRedistributeSpecialCaseDisableInstanceAndNetworkIndex[relationID][relationEntityID])
											{
												GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(oldRedundantEntity);
											}
											else
											{
												GIAtranslatorOperations.disableEntity(oldRedundantEntity);
											}
										}
									}
									if(param->enableEntity[relationID][relationEntityID])
									{
										GIAentityNode* entityToRenable;
										if(param->relationEntityIndexOriginal[relationID][relationEntityID] != INT_DEFAULT_VALUE)	//check whether attempting to disable a non-preposition (and therefore non-existant) relationType entity
										{
											entityToRenable = (*param->translatorVariables.GIAentityNodeArray)[param->relationEntityIndexOriginal[relationID][relationEntityID]];
											entityToRenable->disabled = false;
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

	return result;
}

#endif


#ifdef GIA_GENERIC_ENTITY_INTERPRETATION

GIAgenericEntityInterpretationParameters::GIAgenericEntityInterpretationParameters(GIAtranslatorVariablesClass* translatorVariablesNew, bool executeOrReassignNew, bool linkPreestablishedReferencesGIAnew)
{
	translatorVariables = *translatorVariablesNew;

	executeOrReassign = executeOrReassignNew;
	
	linkPreestablishedReferencesGIA = linkPreestablishedReferencesGIAnew;

		//relations to parse
	parseDisabledEntity = false;

		//predefined values tests
	useEntityTest = false;
	entityTest = "";
	entityTestIsNegative = false;
	useEntityArrayTest = false;
	entityArrayTest = NULL;
	entityArrayTestSize = INT_DEFAULT_VALUE;
	entityArrayTestIsNegative = false;

		//for redistribution

		//for execution
	functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_undefined;

	disableEntity = false;

	functionName = "";
}
GIAgenericEntityInterpretationParameters::~GIAgenericEntityInterpretationParameters(void)
{
}

bool GIAsynRelTranslatorGenericClass::genericEntityInterpretation(GIAgenericEntityInterpretationParameters* param)
{
	bool result = false;
	for(int i=0; i<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(param->translatorVariables.currentSentenceInList); i++)
	{
		if((*param->translatorVariables.GIAentityNodeArrayFilled)[i])
		{
			if(param->parseDisabledEntity || !((*param->translatorVariables.GIAentityNodeArray)[i]->disabled))
			{
				bool passedEntity = true;
				if(!GIAentityNodeClass.testEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[i], &(param->specialCaseCharacteristicsTestAndVector), true))
				{
					passedEntity = false;
				}
				if(!GIAentityNodeClass.testEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[i], &(param->specialCaseCharacteristicsTestOrVector), false))
				{
					passedEntity = false;
				}
				if(param->useEntityTest)
				{
					if(passedEntity)
					{
						passedEntity = false;
						if(param->entityTestIsNegative)
						{//negative
							if((*param->translatorVariables.GIAentityNodeArray)[i]->entityName != param->entityTest)
							{
								passedEntity = true;
							}
						}
						else
						{//normal
							if((*param->translatorVariables.GIAentityNodeArray)[i]->entityName == param->entityTest)
							{
								passedEntity = true;
							}
						}
					}
				}
				if(param->useEntityArrayTest)
				{
					if(passedEntity)
					{
						passedEntity = false;
						bool foundAnArrayPass = false;
						for(int j=0; j<param->entityArrayTestSize; j++)
						{
							if((*param->translatorVariables.GIAentityNodeArray)[i]->entityName == (param->entityArrayTest)[j])
							{
								passedEntity = true;
								foundAnArrayPass = true;
							}
						}
						if(param->entityArrayTestIsNegative)
						{
							if(!foundAnArrayPass)
							{
								passedEntity = true;
							}
							else
							{
								passedEntity = false;
							}
						}
					}
				}

				if(passedEntity)
				{
					result = true;


					if(param->executeOrReassign)
					{
						if(param->functionToExecuteUponFind == GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition)
						{
							(*param->translatorVariables.GIAentityNodeArray)[i] = GIAtranslatorOperations.addInstanceToInstanceDefinition((*param->translatorVariables.GIAentityNodeArray)[i], GIA_ENTITY_TYPE_SUBSTANCE, &(param->translatorVariables));
						}
						else if(param->functionToExecuteUponFind == GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addActionToActionDefinition)
						{
							(*param->translatorVariables.GIAentityNodeArray)[i] = GIAtranslatorOperations.addInstanceToInstanceDefinition((*param->translatorVariables.GIAentityNodeArray)[i], GIA_ENTITY_TYPE_ACTION, &(param->translatorVariables));
						}
						else if(param->functionToExecuteUponFind == GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addTenseOnlyTimeConditionToSubstance)
						{
							GIAtranslatorOperations.addTenseOnlyTimeConditionToSubstance((*param->translatorVariables.GIAentityNodeArray)[i], (*param->translatorVariables.GIAentityNodeArray)[i]->grammaticalTenseTemp, (*param->translatorVariables.GIAentityNodeArray)[i]->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE]);
						}
						else
						{
							cout << "illegal functionToExecuteUponFind: " << param->functionToExecuteUponFind << endl;
						}
					}
					/*
					else
					{
						GIAentityNodeClass.setEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[i], &(param->specialCaseCharacteristicsAssignmentVector));
					}
					*/
					GIAentityNodeClass.setEntityCharacteristics((*param->translatorVariables.GIAentityNodeArray)[i], &(param->specialCaseCharacteristicsAssignmentVector));	//this has been moved out in the case reassignment is required along with execution


				}

				//for cleanup
				if(param->disableEntity)
				{
					GIAtranslatorOperations.disableEntity((*param->translatorVariables.GIAentityNodeArray)[i]);
				}
			}
		}
	}
	return result;
}

#endif

bool GIAsynRelTranslatorGenericClass::determineFeatureIndexOfPreposition(GIAsentence* currentSentenceInList, const GIArelation* prepositionRelation, int* indexOfPreposition)
{
	string prepositionName = prepositionRelation->relationType;


	bool prepositionFeatureFound = false;

	//added GIA 2f9b 11-July-2014
	bool centredPrepositionFeatureFound = false;
	int featureIndexOfNonCentredPrepositionFeature = 0;
	int numberOfPrepositionFeaturesFound = 0;


	GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		string singleWordPreposition = prepositionName;
		int indexOfPrepositionDelimiter = prepositionName.rfind(GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER);	//find last occurance
		if(indexOfPrepositionDelimiter != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			int lengthOfSingleWordPreposition = prepositionName.length() - indexOfPrepositionDelimiter - 1;
			singleWordPreposition = prepositionName.substr(indexOfPrepositionDelimiter+1, lengthOfSingleWordPreposition);
			if(indexOfPrepositionDelimiter == prepositionName.length()-1)
			{
				cerr << "determineFeatureIndexOfPreposition: illegal multiword preposition; (indexOfPrepositionDelimiter == prepositionName->length()-1)" << endl;
				cerr << "prepositionName = " << prepositionName << endl;
				exit(EXIT_ERROR);
			}
		}

		if(currentFeatureInList->lemma == singleWordPreposition)
		{
			/*
			cout << "currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			cout << "prepositionFeatureFound: " << prepositionName << " " << currentFeatureInList->entityIndex << endl;
			cout << "prepositionRelation->relationGovernorIndex: " << prepositionRelation->relationGovernorIndex << endl;
			cout << "prepositionRelation->relationDependentIndex: " << prepositionRelation->relationDependentIndex << endl;
			*/

			prepositionFeatureFound = true;
			if(((currentFeatureInList->entityIndex < prepositionRelation->relationGovernorIndex) && (currentFeatureInList->entityIndex > prepositionRelation->relationDependentIndex))
			|| ((currentFeatureInList->entityIndex > prepositionRelation->relationGovernorIndex) && (currentFeatureInList->entityIndex < prepositionRelation->relationDependentIndex)))
			{//added GIA 2f9a 11-July-2014
				centredPrepositionFeatureFound = true;
				*indexOfPreposition = currentFeatureInList->entityIndex;
			}
			else
			{//added GIA 2f9b 11-July-2014
				featureIndexOfNonCentredPrepositionFeature = currentFeatureInList->entityIndex;
			}
			numberOfPrepositionFeaturesFound++;
		}
		currentFeatureInList = currentFeatureInList->next;
	}

	if(prepositionFeatureFound && !centredPrepositionFeatureFound)
	{//added GIA 2f9b 11-July-2014
		if(numberOfPrepositionFeaturesFound > 1)
		{
			#ifdef GIA_ENABLE_WARNINGS
			cout << "determineFeatureIndexOfPreposition{}: !centredPrepositionFeatureFound && (numberOfPrepositionFeaturesFound > 1); cannot guarantee correct preposition feature index has been assigned" << endl;
			#endif
		}
		*indexOfPreposition = featureIndexOfNonCentredPrepositionFeature;
	}
	return prepositionFeatureFound;
}


/*
GIAentityNode* GIAsynRelTranslatorGenericClass::addRelationshipArtificialToEntityAndGenerateSemanticRelations(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables, bool isArtificial, int relationshipSubjectEntityIndex, int relationshipObjectEntityIndex)
{
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addRelationshipArtificialToEntity(relationshipSubjectEntity, relationshipObjectEntity, sameReferenceSet, relationshipEntityType, relationshipEntityName, translatorVariables);

	#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	int featureIndexOfRelationship = relationshipEntity->entityIndexTemp;
	GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables->GIAentityNodeArray, translatorVariables->currentSentenceInList, generateConnectionType(relationshipEntityType), relationshipSubjectEntityIndex, featureIndexOfRelationship, sameReferenceSet);
	GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables->GIAentityNodeArray, translatorVariables->currentSentenceInList, generateConnectionTypeInverse(relationshipEntityType), relationshipObjectEntityIndex, featureIndexOfRelationship, sameReferenceSet);
	#endif
	
	return relationshipEntity;
}
*/
