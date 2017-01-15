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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h11d 20-January-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorGeneric.h"
#include "GIAtranslatorOperations.h"






#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION

GIAgenericDepRelInterpretationParameters::GIAgenericDepRelInterpretationParameters(Sentence * newcurrentSentenceInList, bool newGIAentityNodeArrayFilled[], GIAentityNode * newGIAentityNodeArray[], bool newexecuteOrReassign)
{
	currentSentenceInList = newcurrentSentenceInList;
	GIAentityNodeArrayFilled = newGIAentityNodeArrayFilled;
	GIAentityNodeArray = newGIAentityNodeArray;
	unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts;

	executeOrReassign = newexecuteOrReassign;

	//for relation1, relation2, relation3, and relation4 [GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS]:
		//for entity1 (eg substanceEntity), entity2 (eg conditionObjectEntity, propertyEntity), and entity3/intermediaryEntity (eg conditionEntity, actionEntity) [3]:

		//relations to parse
	numberOfRelations = 1;
	initialiseBoolArray1D(parseDisabledRelation, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	initialiseBoolArray1D(parseDisabledRelationDuringLink, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);	//not currently used

		//found values
	//NO LONGER INITIALISED: relation = {NULL, NULL, NULL, NULL};
	//relationEntity = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	initialiseIntArray2D(&relationEntityIndex[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	#else
	initialiseIntArray2D(&relationEntityIndex[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	#endif
	initialiseBoolArray1D(relationEntityPrepFound, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
		//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
	//relationEntityOriginal = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	initialiseIntArray2D(&relationEntityIndexOriginal[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	#else
	initialiseIntArray2D(&relationEntityIndexOriginal[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	#endif
		//for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation():
	//relationFinalResult = {NULL, NULL, NULL, NULL, NULL};
	//relationEntityFinalResult = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	initialiseIntArray2D(&relationEntityIndexFinalResult[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	#else
	initialiseIntArray2D(&relationEntityIndexFinalResult[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	#endif

		//predefined values tests
	initialiseBoolArray2D(&useRelationTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	//relationTest = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}};	//internal compiler error: Segmentation fault
	initialiseBoolArray2D(&relationTestIsNegative[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	initialiseBoolArray2D(&useRelationArrayTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//if !useRelationTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY], then useRelationArrayTest[x][REL_ENT0_TYPE_OR_INTERMEDIARY] (as a relation type test must be applied for each parsed relation)
	//NO LONGER INITIALISED: relationArrayTest = {{NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}};
	initialiseIntArray2D(&relationArrayTestSize[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	initialiseBoolArray2D(&relationArrayTestIsNegative[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	initialiseBoolArray1D(expectToFindPrepositionTest, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);

		//entity index match tests
	initialiseBoolArray2D(&useRelationIndexTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	initialiseIntArray2D(&relationIndexTestRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
	initialiseIntArray2D(&relationIndexTestEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);
		//NB for relationType tests use relationType as indicies are not available
	initialiseBoolArray2D(&relationIndexTestIsNegative[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
		//special cases
	initialiseBoolArray2D(&useSpecialCaseCharacteristicsRelationIndexTest[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//not used often
	initialiseIntArray2D(&specialCaseCharacteristicsRelationIndexTestRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);		//not used often
	initialiseIntArray2D(&specialCaseCharacteristicsRelationIndexTestEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);		//not used often

		//for redistribution
	initialiseBoolArray2D(&useRedistributeRelationEntityIndexReassignment[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);			//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - for reassigning relation entities
	initialiseIntArray2D(&redistributeRelationEntityIndexReassignmentRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);						//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relation1, relation2, relation3, or relation4 - for reassigning relation entities
	initialiseIntArray2D(&redistributeRelationEntityIndexReassignmentRelationEntityID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);						//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for reassigning relation entities
	initialiseBoolArray2D(&redistributeRelationEntityIndexReassignmentUseOriginalValues[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//for relation1, relation2, relation3, and relation4; for entity1, entity2, and entity3
	initialiseBoolArray2D(&useRedistributeRelationEntityReassignment[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false); 			//for entity1, entity2, and entity3 - for renaming relation entities
	//redistributeRelationEntityReassignment = {{"", "", ""}, {"", "", ""}, {"", "", ""}, {"", "", ""}}; 	//internal compiler error: Segmentation fault		//for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex - for renaming relation entities
		//special cases for redistribution
	initialiseBoolArray1D(useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	initialiseBoolArray2D(&useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	initialiseIntArray3D(&redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[0][0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, 2, INT_DEFAULT_VALUE);
	initialiseIntArray3D(&redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[0][0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, 2, INT_DEFAULT_VALUE);
	initialiseBoolArray2D(&useRedistributeSpecialCaseDisableInstanceAndConcept[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	initialiseBoolArray2D(&useSpecialCaseCharacteristicsRelationEntityIndexReassignment[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false); 	//not used often
	initialiseIntArray2D(&specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, INT_DEFAULT_VALUE);						//not used often

		//for execution
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	defaultSameSetRelationID = INT_DEFAULT_VALUE;
	defaultSameSetReferenceValue = false;
	#endif
	initialiseIntArray1D(functionEntityRelationID, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION, REL1);				//these dummy values have to be set always to prevent crash //for entity1, entity2, and entity3 - relation1, relation2, relation3, or relation4
	initialiseIntArray1D(functionEntityRelationEntityID, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_FUNCTION, FUNC_ENT1);	//these dummy values have to be set always to prevent crash //for entity1, entity2, and entity3 - relationType, relationGovernorIndex, or relationDependentIndex
	functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_undefined;
		//special cases
	mustGenerateConditionName = false;
	conditionEntityDefaultName = "";

	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	initialiseBoolArray2D(&disableEntity[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);
	initialiseBoolArray2D(&disableEntityUseOriginalValues[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false); 	//for disabling an entity based on its original index
	initialiseBoolArray2D(&enableEntity[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION, false);	//added GIA 2f12a 13-July-2014
	#else
	initialiseBoolArray2D(&disableEntity[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION, false); 	//for entity1 and entity2 only
	initialiseBoolArray2D(&disableEntityUseOriginalValues[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION, false); 	//for disabling an entity based on its original index
	initialiseBoolArray2D(&enableEntity[0][0], GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION, false);	//added GIA 2f12a 13-July-2014
	#endif
	initialiseBoolArray1D(disableRelation, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);
	initialiseBoolArray1D(disableRelationDuringLink, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);

	functionName = "";

	initialiseBoolArray1D(useRedistributeSpecialCaseNonExistantRelationCheck, GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS, false);	//non existant relations tests - added GIA 2f12a 13-July-2014
}
GIAgenericDepRelInterpretationParameters::~GIAgenericDepRelInterpretationParameters(void)
{
}

void initialiseBoolArray1D(bool * boolArray, int size, int value)
{
	for(int i=0; i<size; i++)
	{
		boolArray[i] = value;
	}
}
void initialiseBoolArray2D(bool * boolArray, int size1, int size2, int value)
{
	for(int i=0; i<size1; i++)
	{
		for(int j=0; j<size2; j++)
		{
			boolArray[i*size2 + j] = value;
		}
	}
}
void initialiseIntArray1D(int * intArray, int size, int value)
{
	for(int i=0; i<size; i++)
	{
		intArray[i] = value;
	}
}
void initialiseIntArray2D(int * intArray, int size1, int size2, int value)
{
	for(int i=0; i<size1; i++)
	{
		for(int j=0; j<size2; j++)
		{
			intArray[i*size2 + j] = value;
		}
	}
}
void initialiseIntArray3D(int * intArray, int size1, int size2, int size3, int value)
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

bool genericDependecyRelationInterpretation(GIAgenericDepRelInterpretationParameters * param, int currentRelationID)
{
	//cout << "START genericDependecyRelationInterpretation: " << currentRelationID << endl;
	bool result = false;

	#ifdef GIA_CREATE_INDEPENDENT_CONJUNCTION_ENTITIES
	int conjunctionRelationIndex = 0;
	#endif

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
			//cout << "passed disabled rel" << endl;
			param->relation[currentRelationID] = currentRelationInList;

			//predefined values tests
			bool passedRelation = true;
			param->relationEntity[currentRelationID][REL_ENT1] = param->relation[currentRelationID]->relationGovernor;
			param->relationEntity[currentRelationID][REL_ENT2] = param->relation[currentRelationID]->relationDependent;
			param->relationEntity[currentRelationID][REL_ENT3] = param->relation[currentRelationID]->relationType;
			param->relationEntity[currentRelationID][REL_ENT3] = convertPrepositionToRelex(&(param->relationEntity[currentRelationID][REL_ENT3]), &(param->relationEntityPrepFound[currentRelationID]));	//convert stanford prep_x to relex x
			param->relationEntityIndex[currentRelationID][REL_ENT1] = param->relation[currentRelationID]->relationGovernorIndex;
			param->relationEntityIndex[currentRelationID][REL_ENT2] = param->relation[currentRelationID]->relationDependentIndex;
			#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
			param->relationEntityIndex[currentRelationID][REL_ENT3] = param->relation[currentRelationID]->relationTypeIndex;
			#else
			#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
			//added GIA 2c2a for redistributeStanfordRelationsMultiwordPreposition();
			bool prepositionFound = false;
			string prepositionName = convertPrepositionToRelex(&(param->relation[currentRelationID]->relationType), &prepositionFound);
			if(prepositionFound)
			{
				//cout << "prepositionName = " << prepositionName << endl;
				param->relationEntityIndex[currentRelationID][REL_ENT3] = INT_DEFAULT_VALUE;	//added GIA 2f9a
				bool prepositionFeatureFound = determineFeatureIndexOfPreposition(param->currentSentenceInList, param->relation[currentRelationID], &(param->relationEntityIndex[currentRelationID][REL_ENT3]));
				if(prepositionFeatureFound)
				{
					//cout << "currentRelationID = " << currentRelationID << endl;
					//cout << "REL_ENT3 = " << REL_ENT3 << endl;
					//cout << "(param->relationEntityIndex[currentRelationID][REL_ENT3]) = " << (param->relationEntityIndex[currentRelationID][REL_ENT3]) << endl;
				}
			}
			else
			{//added GIA 2f2b
				param->relationEntityIndex[currentRelationID][REL_ENT3] = INT_DEFAULT_VALUE;
			}
			#endif
			#endif

				//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
			param->relationEntityOriginal[currentRelationID][REL_ENT1] = param->relationEntity[currentRelationID][REL_ENT1];
			param->relationEntityOriginal[currentRelationID][REL_ENT2] = param->relationEntity[currentRelationID][REL_ENT2];
			param->relationEntityOriginal[currentRelationID][REL_ENT3] = param->relationEntity[currentRelationID][REL_ENT3];
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT1] = param->relationEntityIndex[currentRelationID][REL_ENT1];
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT2] = param->relationEntityIndex[currentRelationID][REL_ENT2];
			#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
			param->relationEntityIndexOriginal[currentRelationID][REL_ENT3] = param->relationEntityIndex[currentRelationID][REL_ENT3];	//added GIA 2c2a
			#endif
			if(param->expectToFindPrepositionTest[currentRelationID])
			{
				if(!(param->relationEntityPrepFound[currentRelationID]))
				{
					passedRelation = false;
				}
			}
			for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
			{
				if(!testEntityCharacteristics(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestAndVector[currentRelationID][relationEntityID]), true))
				{
					passedRelation = false;
				}
				if(!testEntityCharacteristics(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestOrVector[currentRelationID][relationEntityID]), false))
				{
					//cout << "failed specialCaseCharacteristicsTestOrVector" << endl;
					passedRelation = false;
				}
				if(!testEntityCharacteristics(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestOr2Vector[currentRelationID][relationEntityID]), false))
				{
					passedRelation = false;
				}
				if(!testEntityCharacteristics(param->GIAentityNodeArray[param->relationEntityIndex[currentRelationID][relationEntityID]], &(param->specialCaseCharacteristicsTestOr3Vector[currentRelationID][relationEntityID]), false))
				{
					passedRelation = false;
				}
				if(param->useRelationTest[currentRelationID][relationEntityID])
				{
					//cout << "\t useRelationTest: " << currentRelationID << ", " << relationEntityID << " = " << param->relationTest[currentRelationID][relationEntityID] << endl;

					if(passedRelation)
					{
						//cout << "\t passedRelation useRelationTest: " << currentRelationID << ", " << relationEntityID << " = " << param->relationTest[currentRelationID][relationEntityID] << endl;

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
						//cout << "relationArrayTestSize = " << relationArrayTestSize << endl;
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
					bool specialCaseRelationCheck = false;
					if(currentRelationID == param->numberOfRelations)
					{
						//cout << "currentRelationID == param->numberOfRelations: " << param->numberOfRelations << endl;
						specialCaseRelationCheck = true;
						minRelationToTest = param->numberOfRelations;
						maxRelationToTest = param->numberOfRelations+1;
					}
					for(int relationID=minRelationToTest; relationID<maxRelationToTest; relationID++)
					{
						//cout << "relationID = " << relationID << endl;
						for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)	//changed from GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION 25 July 2013
						{
							//cout << "param->useRelationIndexTest[relationID][relationEntityID] = " << param->useRelationIndexTest[relationID][relationEntityID] << endl;
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
							if(param->useSpecialCaseCharacteristicsRelationIndexTest[relationID][relationEntityID])
							{//not used often
								//cout << "useSpecialCaseCharacteristicsRelationIndexTest" << endl;
								passedEntityMatchTests = false;
								GIAentityNode * currentEntity = param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]];
								GIAentityNode * targetEntity = param->GIAentityNodeArray[param->relationEntityIndex[param->specialCaseCharacteristicsRelationIndexTestRelationID[relationID][relationEntityID]][param->specialCaseCharacteristicsRelationIndexTestEntityID[relationID][relationEntityID]]];
								EntityCharacteristic * entityCharacteristic = &(param->specialCaseCharacteristicsRelationIndexTest[relationID][relationEntityID]);
								getEntityCharacteristic(currentEntity, entityCharacteristic);
								if(testEntityCharacteristic(targetEntity, entityCharacteristic))
								{
									passedEntityMatchTests = true;
								}
							}
						}
					}
					if(param->useRedistributeSpecialCaseNonExistantRelationCheck[currentRelationID])
					{//non existant relations tests - added GIA 2f12a 13-July-2014
						#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
						cout << "trying to find non allowed relation:" << endl;
						cout << currentRelationID << ": [SpecialCaseNonExistantRelationCheck]: " << param->relation[currentRelationID]->relationType << "(" << param->relation[currentRelationID]->relationGovernor << ", " << param->relation[currentRelationID]->relationDependent << ")" << endl;
						#endif
						GIAgenericDepRelInterpretationParameters paramTemp = *param;
						if(genericDependecyRelationInterpretation(&paramTemp, (currentRelationID+1)))
						{
							passedEntityMatchTests = false;
							#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
							cout << "found disallowed relation" << endl;
							#endif
						}
					}

					if(passedEntityMatchTests)
					{
						#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
						cout << "passedEntityMatchTests" << endl;
						#endif
						//cout << "currentRelationID = " << currentRelationID << endl;

						result = true;

						//cout << "\t\t\genericDependecyRelationInterpretation() passed: function = " << param->functionName << endl;

						//record final values for further manipulation of variables after successful (match found) recursive execution of genericDependecyRelationInterpretation:
						for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
						{
							param->relationFinalResult[relationID] = param->relation[relationID];
							#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
							for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
							#else
							for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION; relationEntityID++)
							#endif
							{
								param->relationEntityFinalResult[relationID][relationEntityID] = param->relationEntity[relationID][relationEntityID];
								param->relationEntityIndexFinalResult[relationID][relationEntityID] = param->relationEntityIndex[relationID][relationEntityID];
							}
						}

						if(!specialCaseRelationCheck)
						{
							if(param->executeOrReassign)
							{//execute
								#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
								bool sameReferenceSet;
								if(param->defaultSameSetRelationID == INT_DEFAULT_VALUE)
								{
									sameReferenceSet = param->defaultSameSetReferenceValue;
								}
								else
								{
									//cout << "param->defaultSameSetReferenceValue = " << param->defaultSameSetReferenceValue << endl;
									//cout << "sameReferenceSet = " << sameReferenceSet << endl;
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
								if((param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity)
								#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
								|| (param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectBeingDefinitionConditionToEntity)
								|| (param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectHavingPropertyConditionToEntity)
								#endif
								)
								{//if a condition is required to be created
									string conditionEntityName = "";
									bool mustLookupOrGenerateConditionConceptEntity = false;
									if(param->mustGenerateConditionName)
									{
										conditionEntityName = param->conditionEntityDefaultName;
										functionEntityIndex3 = param->currentSentenceInList->conditionEntityArtificialIndexCurrent;
										param->currentSentenceInList->conditionEntityArtificialIndexCurrent = param->currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
										mustLookupOrGenerateConditionConceptEntity = true;
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

										#ifdef GIA_CREATE_INDEPENDENT_CONJUNCTION_ENTITIES
										int conjunctionType = INT_DEFAULT_VALUE;	//not used
										string conditionEntityNameOrig = param->relationEntity[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];
										//cout << "conditionEntityNameOrig = " << conditionEntityNameOrig << endl;
										bool conjunctionConditionFound = textInTextArray(currentRelationInList->relationType, relationTypeConjugationNameArray, ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES, &conjunctionType);
										if(conjunctionConditionFound)
										{
											//cout << "conjunctionRelationIndex = " << conjunctionRelationIndex << endl;
											int conjunctionFeatureIndex = FEATURE_INDEX_OF_CONJUNCTION_1-conjunctionRelationIndex;
											functionEntityIndex3 = conjunctionFeatureIndex;
											mustLookupOrGenerateConditionConceptEntity = true;
											conjunctionRelationIndex++;
										}
										else
										{
										#endif
											if(param->functionEntityRelationEntityID[FUNC_ENT3] == REL_ENT3)	//condition is being defined as a relation's relationType and has no obvious concept entity. However a relationType is often a preposition - if so it will already have a feature index (determined by determineFeatureIndexOfPreposition()) [of which there might even be a corresponding entity name/index defined depending upon how the dependency relations were generated by the NLP: see findOrAddEntityNodeByNameSimpleWrapperCondition() below for information on how this case is handled]
											{
												#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
												functionEntityIndex3 = param->relation[param->functionEntityRelationID[FUNC_ENT3]]->relationTypeIndex;	//CHECKTHIS
												if(functionEntityIndex3 == INT_DEFAULT_VALUE)
												{
													functionEntityIndex3 = param->currentSentenceInList->conditionEntityArtificialIndexCurrent;
													param->currentSentenceInList->conditionEntityArtificialIndexCurrent = param->currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
												}
												#else
												functionEntityIndex3 = INT_DEFAULT_VALUE;
												bool prepositionFeatureFound = determineFeatureIndexOfPreposition(param->currentSentenceInList, param->relation[param->functionEntityRelationID[FUNC_ENT3]], &functionEntityIndex3);
												if(!prepositionFeatureFound)
												{
													functionEntityIndex3 = param->currentSentenceInList->conditionEntityArtificialIndexCurrent;
													param->currentSentenceInList->conditionEntityArtificialIndexCurrent = param->currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
												}
												#endif
												mustLookupOrGenerateConditionConceptEntity = true;
											}
											else
											{
												//functionEntityIndex3 will already be set correctly	[param->relationEntityIndex[param->functionEntityRelationID[FUNC_ENT3]][param->functionEntityRelationEntityID[FUNC_ENT3]];]
											}
										#ifdef GIA_CREATE_INDEPENDENT_CONJUNCTION_ENTITIES
										}
										#endif
									}
									if(mustLookupOrGenerateConditionConceptEntity)
									{
										//NB if concept type entity name has already been defined (GIAentityNodeArrayFilled[functionEntityIndex3]), then findOrAddEntityNodeByNameSimpleWrapperCondition will use it instead
										bool entityAlreadyExistant = false;
										param->GIAentityNodeArray[functionEntityIndex3] = findOrAddEntityNodeByNameSimpleWrapperCondition(param->GIAentityNodeArrayFilled, param->GIAentityNodeArray, functionEntityIndex3, &conditionEntityName, &entityAlreadyExistant, param->entityNodesActiveListConcepts);
										//GIAentityNode * conditionConceptEntity = above
									}
								}
								#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToEntity)
								{
									if(param->mustGenerateConditionName)
									{
										//create intermediary action 'have'
										string conditionEntityName = param->conditionEntityDefaultName;
										functionEntityIndex3 = param->currentSentenceInList->conditionEntityArtificialIndexCurrent;
										param->currentSentenceInList->conditionEntityArtificialIndexCurrent = param->currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
										bool entityAlreadyExistant = false;
										param->GIAentityNodeArray[functionEntityIndex3] = findOrAddEntityNodeByNameSimpleWrapperCondition(param->GIAentityNodeArrayFilled, param->GIAentityNodeArray, functionEntityIndex3, &conditionEntityName, &entityAlreadyExistant, param->entityNodesActiveListConcepts);
										//GIAentityNode * conditionConceptEntity = above
									}
								}
								#endif

								//cout << "\t\tgenericDependecyRelationInterpretation() passed: function = " << param->functionName << endl;
								//cout << "\t\tgenericEntityArrayInterpretation: " << param->GIAentityNodeArray[functionEntityIndex1]->entityName << endl;
								//cout << "\t\tisSubstanceConcept: " << param->GIAentityNodeArray[functionEntityIndex1]->isSubstanceConcept << endl;

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
									//cout << "entityName1 = " << param->GIAentityNodeArray[functionEntityIndex1]->entityName << endl;
									//cout << "entityName2 = " << param->GIAentityNodeArray[functionEntityIndex2]->entityName << endl;
									//cout << "sameReferenceSet = " << sameReferenceSet << endl;
									param->GIAentityNodeArray[functionEntityIndex2] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity)
								{
									//cout << "param->GIAentityNodeArray[functionEntityIndex1]->entityName = " << param->GIAentityNodeArray[functionEntityIndex1]->entityName << endl;
									//cout << "param->GIAentityNodeArray[functionEntityIndex2]->entityName = " << param->GIAentityNodeArray[functionEntityIndex2]->entityName << endl;
									param->GIAentityNodeArray[functionEntityIndex2] = addOrConnectPropertyToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToEntity)
								{
									bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									bool sameReferenceSetObject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
									if(param->mustGenerateConditionName)
									{
										sameReferenceSetSubject = sameReferenceSet;
										sameReferenceSetObject = sameReferenceSet;
									}
									else
									{
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
										sameReferenceSetSubject = determineSameReferenceSetValue(param->defaultSameSetReferenceValue, param->relation[REL1]);	//changed from DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS 2c1a to support !GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
										sameReferenceSetObject = determineSameReferenceSetValue(param->defaultSameSetReferenceValue, param->relation[REL2]);	//changed from DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS 2c1a to support !GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC

									}
									#endif

									param->GIAentityNodeArray[functionEntityIndex3] = addOrConnectActionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], sameReferenceSetSubject, sameReferenceSetObject);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, functionEntityIndex1, functionEntityIndex3, sameReferenceSetSubject);
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, functionEntityIndex2, functionEntityIndex3, sameReferenceSetObject);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToSubject)
								{
									param->GIAentityNodeArray[functionEntityIndex2] = addOrConnectActionToSubject(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToObject)
								{
									param->GIAentityNodeArray[functionEntityIndex2] = addOrConnectActionToObject(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity)
								{
									//cout << "param->GIAentityNodeArray[functionEntityIndex1] = " << param->GIAentityNodeArray[functionEntityIndex1]->entityName << endl;
									//cout << "param->GIAentityNodeArray[functionEntityIndex2] = " << param->GIAentityNodeArray[functionEntityIndex2]->entityName << endl;
									//cout << "param->GIAentityNodeArray[functionEntityIndex3] = " << param->GIAentityNodeArray[functionEntityIndex3]->entityName << endl;
									//cout << "sameReferenceSet = " << sameReferenceSet << endl;									
									param->GIAentityNodeArray[functionEntityIndex3] = addOrConnectConditionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
									#endif
									#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
									if(currentRelationInList->relationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity is called based on a single Relation
									{	
										#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
										cout << "currentRelationInList->relationTwoWay detected" << endl;
										#endif
										param->GIAentityNodeArray[functionEntityIndex3]->conditionTwoWay = true;	//sets conditionTwoWay for condition substance not concept 
									}
									#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
									if(currentRelationInList->inverseRelationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity is called based on a single Relation
									{	
										#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
										cout << "currentRelationInList->inverseRelationTwoWay detected" << endl;
										#endif
										param->GIAentityNodeArray[functionEntityIndex3]->inverseConditionTwoWay = true;	//sets inverseConditionTwoWay for condition substance not concept 
									}
									#endif
									#endif
								}
								#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectBeingDefinitionConditionToEntity)
								{
									bool negative = param->GIAentityNodeArray[functionEntityIndex4special]->negative;
									//cout << "negative = " << negative << endl;
									param->GIAentityNodeArray[functionEntityIndex3] = addOrConnectBeingDefinitionConditionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], negative, sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, functionEntityIndex3, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectHavingPropertyConditionToEntity)
								{
									bool negative = param->GIAentityNodeArray[functionEntityIndex4special]->negative;
									//cout << "negative = " << negative << endl;
									param->GIAentityNodeArray[functionEntityIndex3] = addOrConnectHavingPropertyConditionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], param->GIAentityNodeArray[functionEntityIndex3], negative, sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, functionEntityIndex3, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								#endif
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntity)
								{
									addDefinitionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								#ifdef GIA_SUPPORT_ALIASES
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_mergeEntityNodesAddAlias)
								{
									#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
									cout << "GIArules.xml warning: functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_mergeEntityNodesAddAlias (12) and GIA has been compiled with GIA_DISABLE_ALIAS_ENTITY_MERGING: executing GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntityMarkConnectionAsAlias (14) instead" << endl; 
									addDefinitionToEntityMarkConnectionAsAlias(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_MARK_CONNECTION_AS_ALIAS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
									#else
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_ENTITY_NODES_ADD_ALIAS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#else
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
									#endif
									mergeEntityNodesAddAlias(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2]);
									param->GIAentityNodeArray[functionEntityIndex2] = param->GIAentityNodeArray[functionEntityIndex1];
									#endif
								}
								#endif
								#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
								#ifdef GIA_USE_CORPUS_DATABASE
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addAuxiliaryToEntity)
								{
									addDefinitionToEntity(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_COMPOSITION_AUXILIARY, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
								}
								#endif
								#endif
								else if(param->functionToExecuteUponFind == GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntityMarkConnectionAsAlias)
								{
									#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
									addDefinitionToEntityMarkConnectionAsAlias(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2], sameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_MARK_CONNECTION_AS_ALIAS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
									#else
									cout << "GIArules.xml warning: functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntityMarkConnectionAsAlias (14) and GIA has been compiled without GIA_DISABLE_ALIAS_ENTITY_MERGING: executing GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_mergeEntityNodesAddAlias (12) instead" << endl; 
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_ENTITY_NODES_ADD_ALIAS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#else
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(param->GIAentityNodeArray, param->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
									#endif
									#endif
									mergeEntityNodesAddAlias(param->GIAentityNodeArray[functionEntityIndex1], param->GIAentityNodeArray[functionEntityIndex2]);
									param->GIAentityNodeArray[functionEntityIndex2] = param->GIAentityNodeArray[functionEntityIndex1];
									#endif
								}
								
								else
								{
									cout << "illegal functionToExecuteUponFind: " << param->functionToExecuteUponFind << endl;
									exit(0);
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
												#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
												param->relation[relationID]->relationTypeIndex = param->relationEntityIndex[relationID][relationEntityID];
												#endif
												//cout << "param->relation[relationID]->relationType = " << param->relation[relationID]->relationType << endl;
											}
										}
										if(param->useSpecialCaseCharacteristicsRelationEntityIndexReassignment[relationID][relationEntityID])
										{//not used often
											GIAentityNode * currentEntity = param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]];
											GIAentityNode * targetEntity = param->GIAentityNodeArray[param->relationEntityIndex[param->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[relationID][relationEntityID]][param->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[relationID][relationEntityID]]];
											EntityCharacteristic * entityCharacteristic = &(param->specialCaseCharacteristicsRelationEntityIndexReassignment[relationID][relationEntityID]);
											getEntityCharacteristic(targetEntity, entityCharacteristic);
											setEntityCharacteristic(currentEntity, entityCharacteristic);
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
											}
											else if(relationEntityID == REL_ENT2)
											{
												param->relation[relationID]->relationDependent = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
											}
											else if(relationEntityID == REL_ENT3)
											{
												param->relation[relationID]->relationType = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
												//do not overwrite relationTypeIndex: param->relation[relationID]->relationTypeIndex = INT_DEFAULT_VALUE;
												#endif
												#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
												if(param->relationEntityIndex[relationID][relationEntityID] != INT_DEFAULT_VALUE)
												{//preposition entity has been initialised
													param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName = param->redistributeRelationEntityReassignment[relationID][relationEntityID];
												}
												#endif
											}
										}
									}
								}
								//special case reassignments
								for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
								{
									#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
									if(param->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[relationID])
									{
										#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
										cout << currentRelationID << ": [SpecialCaseAuxCheck]: " << param->relation[currentRelationID]->relationType << "(" << param->relation[currentRelationID]->relationGovernor << ", " << param->relation[currentRelationID]->relationDependent << ")" << endl;
										#endif
										bool auxiliaryIndicatesDifferentReferenceSet = true;
										GIAgenericDepRelInterpretationParameters paramTemp = *param;
										if(genericDependecyRelationInterpretation(&paramTemp, (currentRelationID+1)))
										{
											auxiliaryIndicatesDifferentReferenceSet = false;
										}
										#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
										cout << "auxiliaryIndicatesDifferentReferenceSet = " << auxiliaryIndicatesDifferentReferenceSet << endl;
										#endif
										param->relation[relationID]->auxiliaryIndicatesDifferentReferenceSet = auxiliaryIndicatesDifferentReferenceSet;
									}
									#endif

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
												/*
												#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW_ASSIGN_RELATIONTYPEINDEX_FOR_CONCATONATIONS
												relationTypeIndex: param->relation[relationID]->relationTypeIndex = ?;
												#endif
												*/
												//cout << "param->relation[relationID]->relationType = " << param->relation[relationID]->relationType << endl;
												#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
												if(param->relationEntityIndex[relationID][relationEntityID] != INT_DEFAULT_VALUE)
												{//preposition entity has been initialised
													param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]]->entityName = concatonatedEntityName;
												}
												#endif
											}
										}

										if(relationID < param->numberOfRelations)
										{
											setEntityCharacteristics(param->GIAentityNodeArray[param->relationEntityIndex[relationID][relationEntityID]], &(param->specialCaseCharacteristicsAssignmentVector[relationID][relationEntityID]));
										}
									}
								}
							}

							//for cleanup
							for(int relationID=0; relationID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_RELATIONS; relationID++)
							{
								#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
								for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_ENTITIES_PER_RELATION; relationEntityID++)
								#else
								for(int relationEntityID=0; relationEntityID<GIA_GENERIC_DEP_REL_INTERP_MAX_NUM_GOVDEP_ENTITIES_PER_RELATION; relationEntityID++)
								#endif
								{
									if(param->disableEntity[relationID][relationEntityID])
									{
										GIAentityNode * oldRedundantEntity;
										#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
										if(param->relationEntityIndexOriginal[relationID][relationEntityID] != INT_DEFAULT_VALUE)	//check whether attempting to disable a non-preposition (and therefore non-existant) relationType entity
										{
										#endif
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
										#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
										}
										#endif
									}
									if(param->enableEntity[relationID][relationEntityID])
									{
										GIAentityNode * entityToRenable;
										#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
										if(param->relationEntityIndexOriginal[relationID][relationEntityID] != INT_DEFAULT_VALUE)	//check whether attempting to disable a non-preposition (and therefore non-existant) relationType entity
										{
										#endif
											entityToRenable = param->GIAentityNodeArray[param->relationEntityIndexOriginal[relationID][relationEntityID]];
											//cout << "entityToRenable->entityName" << entityToRenable->entityName << endl;
											entityToRenable->disabled = false;
										#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
										}
										#endif
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

#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION

GIAgenericEntityInterpretationParameters::GIAgenericEntityInterpretationParameters(Sentence * newcurrentSentenceInList, bool newGIAentityNodeArrayFilled[], GIAentityNode * newGIAentityNodeArray[], bool newexecuteOrReassign)
{
	currentSentenceInList = newcurrentSentenceInList;
	GIAentityNodeArrayFilled = newGIAentityNodeArrayFilled;
	GIAentityNodeArray = newGIAentityNodeArray;

	executeOrReassign = newexecuteOrReassign;

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

bool genericEntityInterpretation(GIAgenericEntityInterpretationParameters * param)
{
	bool result = false;
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(param->GIAentityNodeArrayFilled[i])
		{
			if(param->parseDisabledEntity || !(param->GIAentityNodeArray[i]->disabled))
			{
				bool passedEntity = true;
				if(!testEntityCharacteristics(param->GIAentityNodeArray[i], &(param->specialCaseCharacteristicsTestAndVector), true))
				{
					passedEntity = false;
				}
				if(!testEntityCharacteristics(param->GIAentityNodeArray[i], &(param->specialCaseCharacteristicsTestOrVector), false))
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
							if(param->GIAentityNodeArray[i]->entityName != param->entityTest)
							{
								passedEntity = true;
							}
						}
						else
						{//normal
							if(param->GIAentityNodeArray[i]->entityName == param->entityTest)
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
						//int entityArrayTestSize = sizeof((param->entityArrayTest))/sizeof((param->entityArrayTest)[0]);
						//cout << "entityArrayTestSize = " << entityArrayTestSize << endl;
						//cout << "genericEntityInterpretation() passed: function = " << functionName << endl;
						for(int j=0; j<param->entityArrayTestSize; j++)
						{
							if(param->GIAentityNodeArray[i]->entityName == (param->entityArrayTest)[j])
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

					#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
					cout << "\t\t\tgenericEntityInterpretation() passed: function = " << param->functionName << endl;
					cout << "\t\tgenericEntityArrayInterpretation: " << param->GIAentityNodeArray[i]->entityName << endl;
					cout << "\t\tisSubstanceConcept: " << param->GIAentityNodeArray[i]->isSubstanceConcept << endl;
					#endif

					if(param->executeOrReassign)
					{
						if(param->functionToExecuteUponFind == GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition)
						{
							param->GIAentityNodeArray[i] = addSubstanceToSubstanceDefinition(param->GIAentityNodeArray[i]);
						}
						else if(param->functionToExecuteUponFind == GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addActionToActionDefinitionDefineSubstances)
						{
							param->GIAentityNodeArray[i] = addActionToActionDefinitionDefineSubstances(param->GIAentityNodeArray[i]);
						}
						else if(param->functionToExecuteUponFind == GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addTenseOnlyTimeConditionToSubstance)
						{
							addTenseOnlyTimeConditionToSubstance(param->GIAentityNodeArray[i], param->GIAentityNodeArray[i]->grammaticalTenseTemp, param->GIAentityNodeArray[i]->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE]);
						}
						else
						{
							cout << "illegal functionToExecuteUponFind: " << param->functionToExecuteUponFind << endl;
						}
					}
					/*
					else
					{
						setEntityCharacteristics(param->GIAentityNodeArray[i], &(param->specialCaseCharacteristicsAssignmentVector));
					}
					*/
					setEntityCharacteristics(param->GIAentityNodeArray[i], &(param->specialCaseCharacteristicsAssignmentVector));	//this has been moved out in the case reassignment is required along with execution

					//cout << "\t\tisSubstanceConcept after: " << param->GIAentityNodeArray[i]->isSubstanceConcept << endl;
				}

				//for cleanup
				if(param->disableEntity)
				{
					disableEntity(param->GIAentityNodeArray[i]);
				}
			}
		}
	}
	return result;
}

#endif

bool determineFeatureIndexOfPreposition(Sentence * currentSentenceInList, Relation * prepositionRelation, int * indexOfPreposition)
{
	string prepositionName = prepositionRelation->relationType;

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "*prepositionName = " << *prepositionName << endl;
	#endif

	bool prepositionFeatureFound = false;

	//added GIA 2f9b 11-July-2014
	bool centredPrepositionFeatureFound = false;
	int featureIndexOfNonCentredPrepositionFeature = 0;
	int numberOfPrepositionFeaturesFound = 0;


	Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		string singleWordPreposition = prepositionName;
		int indexOfPrepositionDelimiter = prepositionName.rfind(STANFORD_PARSER_PREPOSITION_DELIMITER);	//find last occurance
		if(indexOfPrepositionDelimiter != string::npos)
		{
			int lengthOfSingleWordPreposition = prepositionName.length() - indexOfPrepositionDelimiter - 1;
			singleWordPreposition = prepositionName.substr(indexOfPrepositionDelimiter+1, lengthOfSingleWordPreposition);
			if(indexOfPrepositionDelimiter == prepositionName.length()-1)
			{
				cout << "determineFeatureIndexOfPreposition: illegal multiword preposition; (indexOfPrepositionDelimiter == prepositionName->length()-1)" << endl;
				cout << "prepositionName = " << prepositionName << endl;
				exit(0);
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
				//cout << "centred prepositionFeatureFound: " << prepositionName << " " << currentFeatureInList->entityIndex << endl;
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
			cout << "determineFeatureIndexOfPreposition(): !centredPrepositionFeatureFound && (numberOfPrepositionFeaturesFound > 1); cannot guarantee correct preposition feature index has been assigned" << endl;
			#endif
		}
		*indexOfPreposition = featureIndexOfNonCentredPrepositionFeature;
	}
	return prepositionFeatureFound;
}

