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
 * File Name: GIATranslatorApplyAdvancedFeatures.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1q4b 14-October-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * ?TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorApplyAdvancedFeatures.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"








void extractDates(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser)
{
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		extractDatesRelex(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		extractDatesStanfordCoreNLP(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);
	}
	#endif
}

#ifdef GIA_USE_STANFORD_CORENLP
void extractDatesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
			if(!(currentEntity->disabled))
			{
				if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
				{
					GIAEntityNode * timeEntity = currentEntity;
					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "currentEntity->entityName = " << currentEntity->entityName << endl;
					#endif
					
					if(timeEntity->timeConditionNode != NULL)
					{
						if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
						{
							/*
							if(!(timeEntity->entityNodeDefiningThisInstance->empty()))
							{//required for anomaly
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "timeEntity->entityNodeDefiningThisInstance->NormalizedNERTemp = " << timeEntity->entityNodeDefiningThisInstance->NormalizedNERTemp << endl;
								#endif
								timeEntity->timeConditionNode->conditionName = (timeEntity->entityNodeDefiningThisInstance->back())->entity->NormalizedNERTemp;
							}
							else
							{
							*/
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "timeEntity->NormalizedNERTemp = " << timeEntity->NormalizedNERTemp << endl;
								#endif
								timeEntity->timeConditionNode->conditionName = timeEntity->NormalizedNERTemp;

								//this case appears to be required for queries... (_%qvar/_%atTime), noting that qVar is not assigned a substance (but remains a concept node)
								/*
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "timeEntity->NormalizedNERTemp = " << timeEntity->NormalizedNERTemp << endl;
								cout << "error: timeEntity->entityNodeDefiningThisInstance != NULL [1b]" << endl;
								#else
								cout << "error: [confidential 1b]" << endl;
								#endif
								exit(0);
								*/
							/*
							}
							*/
						}
					}
					else
					{
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "error: isolated date node found (not declared as a time condition) [1]" << endl;
						#else
						cout << "error: [confidential 1]" << endl;
						#endif
						exit(0);	//remove this later
					}
				}
			}
		}
	}

	//add time condition node to isolated entities with NERTemp == FEATURE_NER_DATE		{or have they already been added?}
}
#endif

#ifdef GIA_USE_RELEX
void extractDatesRelex(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				GIAEntityNode * timeEntity = currentEntity;
				if(timeEntity->timeConditionNode != NULL)
				{
					if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
					{
						string monthString = timeEntity->entityName;
						int monthInt = TIME_MONTH_UNDEFINED;
						for(int i=0; i<TIME_MONTH_NUMBER_OF_TYPES; i++)
						{
							if(monthString == timeMonthStringArray[i])
							{
								monthInt = i+1;
							}
						}
						timeEntity->timeConditionNode->month = monthInt;

						//update/regenerate timeConditionName
						timeEntity->timeConditionNode->conditionName = generateDateTimeConditionName(timeEntity->timeConditionNode->dayOfMonth, timeEntity->timeConditionNode->month, timeEntity->timeConditionNode->year);
							//OLD; timeConditionEntity->entityName
					}
				}
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "error: isolated date node found (not declared as a time condition) [1]" << endl;
					#else
					cout << "error: [confidential 1]" << endl;
					#endif
					exit(0);	//remove this later
				}
			}
		}
	}
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if((currentRelationInList->relationType == RELATION_TYPE_DATE_DAY) || (currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR))
			{
				//now locate and fill corresponding time condition node;
				for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
				{
					if(GIAEntityNodeArrayFilled[i])
					{
						GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
						if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
						{
							GIAEntityNode * timeEntity = currentEntity;
							GIAEntityNode * timeConditionEntity = timeEntity;

							if(timeConditionEntity->entityName == currentRelationInList->relationGovernor)
							{
								if(timeEntity->timeConditionNode != NULL)
								{
									if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
									{
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_DAY)
										{
											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

											//http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
												//The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.	[eg "3rd" -> 3]
											string dayOfMonthString = currentRelationInList->relationDependent;
											char * dayOfMonthStringcharstar = const_cast<char*>(dayOfMonthString.c_str());
											int dayOfMonthInt = atoi(dayOfMonthStringcharstar);
											timeConditionEntity->timeConditionNode->dayOfMonth = dayOfMonthInt;

											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "adding day of month: " << dayOfMonthInt << endl;
											#endif

											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
											//timeConditionEntity->entityName  =
										}
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR)
										{
											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

											string yearString = currentRelationInList->relationDependent;
											char * yearStringcharstar = const_cast<char*>(yearString.c_str());
											int yearInt = atoi(yearStringcharstar);
											timeConditionEntity->timeConditionNode->year = yearInt;

											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "adding year: " << yearInt << endl;
											#endif

											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
											//tempTimeCondition->entityName
										}
									}
								}
								else
								{
									cout << "error: isolated date node found (not declared as a time condition)" << endl;
									exit(0);	//remove this later
								}
							}
						}
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
	#ifdef GIA_USE_TIME_NODE_INDEXING
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout <<"add time condition nodes to index [for fast lookup by time]" << endl;
	#endif
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				if(timeEntity->timeConditionNode != NULL)
				{
					if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
					{
						//replace current entity time condition node with generated time condition node.

						int timeConditionEntityIndex = -1;
						bool argumentEntityAlreadyExistant = false;
						long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
						timeEntity->timeConditionNode = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, conceptEntityNamesList, timeConditionTotalTimeInSeconds, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true, timeEntity->timeConditionNode);
					}
				}
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "error: isolated date node found (not declared as a time condition)" << endl;
					#else
					cout << "error: [confidential 4]" << endl;
					#endif
					exit(0);	//remove this later
				}
			}
		}
	}
	#endif
}
#endif


void extractQuantities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPfeatureParser)
{
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		extractQuantitiesRelex(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts);
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		extractQuantitiesStanfordCoreNLP(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts);
	}
	#endif
}

#ifdef GIA_USE_STANFORD_CORENLP
void extractQuantitiesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				/*
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				*/
				#endif

				GIAEntityNode * quantityEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];

				if(quantityEntity->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).
				{
					GIAEntityNode * quantitySubstance = quantityEntity;
					quantitySubstance->hasQuantity = true;
					if((quantitySubstance->NormalizedNERTemp != "") && (quantitySubstance->NormalizedNERTemp != "0.0"))		//added 0.0 for a stanford anomaly 11 May 2012
					{
						quantitySubstance->quantityNumberString = quantitySubstance->NormalizedNERTemp;
					}
					else
					{
						quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;
					}

					int quantityNumberInt = calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;

					disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						setComparisonVariableNode(quantitySubstance);
					}


					//now locate quantity modifiers
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
							{
								if(currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor)
								{
									#ifdef GIA_TRANSLATOR_DEBUG
									//cout << "add quantityModifier" << endl;
									#endif
									/*
									int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationDependent);
									quantitySubstance->quantityModifier = quantityModifierInt;
									*/
									quantitySubstance->quantityModifierString = currentRelationInList2->relationDependent;

									//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)
									GIAEntityNode * entityNode = quantitySubstance;
									GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
									//GIAEntityNode * conditionTypeEntity = quantitySubstance->quantityModifierString;

									string conditionTypeName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

									bool entityAlreadyExistant = false;
									GIAEntityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

									bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

									#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
									GIAEntityNodeArray[FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
									#else
									addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
									#endif
								}

							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						#endif

						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}



	/*
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
			if(!(currentEntity->disabled))
			{
				bool featureNERindicatesNormalisedNERavailable = false;
				for(int i=0; i<FEATURE_NER_INDICATES_NORMALISED_NER_AVAILABLE_NUMBER_TYPES; i++)
				{
					if(currentEntity->NERTemp == featureNERindicatesNormalisedNERavailableTypeArray[i])
					{
						if(currentEntity->NERTemp != FEATURE_NER_DATE)
						{//do not allow dates here (as these are parsed specifically elsewhere
							featureNERindicatesNormalisedNERavailable = true;
						}
					}
				}

				if(featureNERindicatesNormalisedNERavailable)
				{
					if(!(currentEntity->hasAssociatedTime))
					{//do not assume quantity entities when dealing with Stanford Dates, eg num(March-5, 11th-6)  / num(March-5, 1973-8)

						GIAEntityNode * quantitySubstance = currentEntity;
						quantitySubstance->hasQuantity = true;
						quantitySubstance->quantityNumberString = currentEntity->NormalizedNERTemp;
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "adding quantity: " << quantitySubstance << endl;
						cout << "quantityNumberString: " << currentEntity->NormalizedNERTemp << endl;
						#endif
					}
				}
			}
		}
	}
	*/
}
#endif

#ifdef GIA_USE_RELEX
void extractQuantitiesRelex(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				/*
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				*/
				#endif

				GIAEntityNode * quantityEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];


				GIAEntityNode * quantitySubstance = quantityEntity;
				quantitySubstance->hasQuantity = true;
				quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

				int quantityNumberInt = calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
				quantitySubstance->quantityNumber = quantityNumberInt;

				if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{//update comparison variable (set it to the quantity)
					quantitySubstance->isQuery = true;
					GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
					setComparisonVariableNode(quantitySubstance);
				}

				//now locate quantity modifiers and multiplicators
				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
						{
							if(currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor)
							{
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "add quantityModifier" << endl;
								#endif
								/*
								int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationDependent);
								quantitySubstance->quantityModifier = quantityModifierInt;
								*/
								quantitySubstance->quantityModifierString = currentRelationInList2->relationDependent;

								//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)
								GIAEntityNode * entityNode = quantitySubstance;
								GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
								//GIAEntityNode * conditionTypeEntity = quantitySubstance->quantityModifierString;

								string conditionTypeName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

								bool entityAlreadyExistant = false;
								GIAEntityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
								#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
								GIAEntityNodeArray[FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
								#else
								addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
								#endif
							}

						}
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
						{
							if(currentRelationInList2->relationGovernor == currentRelationInList->relationDependent)
							{
								disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]);

								int quantityMultiplierInt = calculateQuantityMultiplierInt(currentRelationInList2->relationDependent);
								quantitySubstance->quantityNumber = quantitySubstance->quantityNumber * quantityMultiplierInt;
								quantitySubstance->hasQuantityMultiplier = true;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}


				bool relationTypeQuantityArgumentImplyMeasurePer = false;
				for(int i=0; i<RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList->relationDependent == relationTypeQuantityArgumentImplyMeasurePerNameArray[i])
					{
						relationTypeQuantityArgumentImplyMeasurePer = true;
					}
				}
				if(relationTypeQuantityArgumentImplyMeasurePer)
				{//eg "every hour" or "every day" - convert to measure_per system

					GIAEntityNode * entityToConnectMeasurePerEntity;
					bool foundQuantityOwner = false;
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationDependent == currentRelationInList->relationGovernor)
							{
								entityToConnectMeasurePerEntity = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex];	//eg row
								foundQuantityOwner = true;
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						#endif

						currentRelationInList2 = currentRelationInList2->next;
					}

					if(foundQuantityOwner)
					{
						//disconnect quantity node from existing connections (not including definitions) - NOT YET CODED.
						disconnectNodeFromAllButDefinitions(quantitySubstance);

						GIAEntityNode * measureSubstance = quantitySubstance;	//convert quantity substance to measure substance
						measureSubstance->hasQuantity = false;
						measureSubstance->hasMeasure = true;
						measureSubstance->measureType = MEASURE_TYPE_PER;

						GIAEntityNode * newQuantityTimesEntity = new GIAEntityNode();
						#ifdef GIA_USE_DATABASE
						newQuantityTimesEntity->added = true;
						#endif

						if(getSaveNetwork())
						{
							long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
							long * currentEntityNodeIDInSubstanceEntityNodesList = getCurrentEntityNodeIDInSubstanceEntityNodesList();
							newQuantityTimesEntity->idActiveList = *currentEntityNodeIDInCompleteList;
							newQuantityTimesEntity->idActiveEntityTypeList = *currentEntityNodeIDInSubstanceEntityNodesList;

							vector<GIAEntityNode*> *entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();
							entityNodesActiveListComplete->push_back(newQuantityTimesEntity);
							(*currentEntityNodeIDInCompleteList)++;
							vector<GIAEntityNode*> * entityNodesActiveListSubstances = getTranslatorSubstanceEntityNodesList();
							entityNodesActiveListSubstances->push_back(newQuantityTimesEntity);
							(*currentEntityNodeIDInSubstanceEntityNodesList)++;
						}
						else
						{
							long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInSentenceCompleteList();
							newQuantityTimesEntity->idActiveList = *currentEntityNodeIDInCompleteList;
							(*currentEntityNodeIDInCompleteList)++;
						}

						newQuantityTimesEntity->entityName = "times";

						//reconnect refreshed quantity (times) node;
						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
						addOrConnectPropertyToEntity(entityToConnectMeasurePerEntity, newQuantityTimesEntity, sameReferenceSet);

						if(newQuantityTimesEntity->hasAssociatedInstanceTemp)
						{//assumed true since its substance was just explicitly created
							newQuantityTimesEntity = (newQuantityTimesEntity->AssociatedInstanceNodeList->back())->entity;
						}
						newQuantityTimesEntity->hasQuantity = true;
						newQuantityTimesEntity->quantityNumber = 1;
						newQuantityTimesEntity->quantityNumberString = "1";

						string conditionTypeName = RELATION_TYPE_MEASURE_PER;

						bool entityAlreadyExistant = false;
						GIAEntityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

						//now add measure_per condition node
						sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAEntityNodeArray[FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN] = addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionTypeEntity, sameReferenceSet);
						#else
						addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionTypeEntity, sameReferenceSet);
						#endif
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

void extractMeasures(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool measureFound = false;
			int measureTypeIndex = MEASURE_TYPE_UNDEFINED;
			for(int i=0; i<RELATION_TYPE_MEASURE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeMeasureNameArray[i])
				{
					measureTypeIndex = i;
					measureFound = true;
				}
			}
			if(measureFound)
			{
				bool measureDependencyFound = false;
				for(int i=0; i<RELATION_TYPE_MEASURE_DEPENDENCY_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList->relationType == relationTypeMeasureDependencyNameArray[i])
					{
						measureDependencyFound = true;
					}
				}

				int relationQuantityIndex = 0;
				int relationMeasureIndex = 0;
				if(measureDependencyFound)
				{
					relationQuantityIndex = currentRelationInList->relationGovernorIndex;
					relationMeasureIndex = currentRelationInList->relationDependentIndex;
				}
				else
				{
					relationQuantityIndex = currentRelationInList->relationDependentIndex;
					relationMeasureIndex = currentRelationInList->relationGovernorIndex;
				}

				GIAEntityNode * measureEntity = GIAEntityNodeArray[relationMeasureIndex];
				GIAEntityNode * quantityEntity = GIAEntityNodeArray[relationQuantityIndex];

				GIAEntityNode * measureSubstanceEntity = measureEntity;
				measureSubstanceEntity->hasMeasure = true;
				measureSubstanceEntity->measureType = measureTypeIndex;

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "measureSubstanceName = " << measureSubstanceEntity->entityName << endl;
				cout << "quantityEntityName = " << quantityEntity->entityName << endl;
				#endif

				string conditionTypeName = relationTypeMeasureNameArray[measureTypeIndex];
				bool entityAlreadyExistant = false;
				GIAEntityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_MEASURE_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

				#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
				if(measureDependencyFound)
				{
					GIAEntityNodeArray[FEATURE_INDEX_OF_MEASURE_UNKNOWN] = addOrConnectConditionToEntity(quantityEntity, measureSubstanceEntity, conditionTypeEntity, sameReferenceSet);
				}
				else
				{
					GIAEntityNodeArray[FEATURE_INDEX_OF_MEASURE_UNKNOWN] = addOrConnectConditionToEntity(measureSubstanceEntity, quantityEntity, conditionTypeEntity, sameReferenceSet);
				}
				#else
				if(measureDependencyFound)
				{
					addOrConnectConditionToEntity(quantityEntity, measureSubstanceEntity, conditionTypeEntity, sameReferenceSet);
				}
				else
				{
					addOrConnectConditionToEntity(measureSubstanceEntity, quantityEntity, conditionTypeEntity, sameReferenceSet);
				}
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void extractQualities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeAdjectiveNameArray[i])
				{
					passed = true;
				}
			}
			//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_AMOD) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_PREDADJ) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD))
			if(passed)
			{
				bool passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);

				if(passed2)
				{
					int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
					int relationDependentIndex = currentRelationInList->relationDependentIndex;
					GIAEntityNode * thingEntity = GIAEntityNodeArray[relationGovernorIndex];
					GIAEntityNode * substanceEntity = GIAEntityNodeArray[relationDependentIndex];

					substanceEntity->isSubstanceQuality = true;	//[eg2 The locked door.. / Jim runs quickly / Mr. Smith is late {_amod/_advmod/_predadj}]
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


void defineToBeAndToDoPropertiesAndConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool pass = false;
			for(int i=0; i<RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeComplementsNameArray[i])
				{
					pass = true;
				}
			}
			if(pass)
			{
				if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
				{
					int entityIndex = currentRelationInList->relationGovernorIndex;
					int substanceIndex = currentRelationInList->relationDependentIndex;

					GIAEntityNode * entityNode = GIAEntityNodeArray[entityIndex];
					GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

					bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES; 	//eg The rose smelled sweet. / The chicken ate the pie that smelled sweet.
					GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(entityNode, substanceEntity, sameReferenceSet);
				}
				else if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
				{
					#ifndef GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_SUBSTANCE_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
					if(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->entityName != RELATION_ENTITY_BE)
					{//this condition is required to support GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
					#endif
					#endif

						GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
						GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
						string conditionTypeEntityNodeName = currentRelationInList->relationType;

						bool entityAlreadyExistant = false;
						GIAEntityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_TODO_UNKNOWN, &conditionTypeEntityNodeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//eg Linas likes to row. / The chicken ate the pie that likes to row.

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAEntityNodeArray[FEATURE_INDEX_OF_TODO_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
						#else
						addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
						#endif

					#ifndef GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_SUBSTANCE_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
					}
					#endif
					#endif
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void linkPropertiesParataxis(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_PARATAXIS)
			{
				string substanceName = currentRelationInList->relationGovernor;
				string actionName = currentRelationInList->relationDependent;
				int substanceIndex = currentRelationInList->relationGovernorIndex;
				int actionIndex = currentRelationInList->relationDependentIndex;

				GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];
				GIAEntityNode * actionEntity = GIAEntityNodeArray[actionIndex];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "linkPropertiesParataxis: found RELATION_TYPE_PARATAXIS" << endl;				
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				//cout << "actionName = " << actionEntity->entityName << endl;
				#endif
				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PARATAXIS; 		//eg The guy, John said, left early in the morning.	[NB The guy, that John said was blue, left early in the morning. / He says that you like to swim.  does not generate parataxis, so these cases needn't be considered here...]
				GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(actionEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void linkConjunctionConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{//NB linkConjunctionConditions() currently performs the same function as linkConditions()

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * actionOrSubstanceEntity = GIAEntityNodeArray[relationGovernorIndex];
			GIAEntityNode * actionOrSubstanceConditionEntity = GIAEntityNodeArray[relationDependentIndex];

			bool passed = false;
			for(int i=0; i<RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES; i++)
			{
				if(relationType == relationTypeConjugationNameArray[i])
				{
					passed = true;
					#ifdef GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
					relationType = relationTypeConjugationBasicNameArray[i];
					#endif
				}
			}

			if(passed)
			{
				string conditionTypeName = relationType;

				bool entityAlreadyExistant = false;
				GIAEntityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_CONJUNCTION_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
				cout << "actionOrSubstanceConditionEntity->entityName = " << actionOrSubstanceConditionEntity->entityName << endl;
				cout << "conditionTypeEntity->entityName = " << conditionTypeEntity->entityName << endl;
				#endif

				#ifdef GIA_USE_ADVANCED_REFERENCING
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE, currentRelationInList);	//eg "and that has a house" versus "and has a house" ??? [untested]
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif

				#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
				GIAEntityNodeArray[FEATURE_INDEX_OF_CONJUNCTION_UNKNOWN] = addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeEntity, sameReferenceSet);
				#else
				addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeEntity, sameReferenceSet);
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void defineClausalComplementProperties(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_CLAUSAL_COMPLEMENT)
			{
				//eg ccomp(say, like)	He says that you like to swim

				string actionName = currentRelationInList->relationGovernor;
				string substanceName = currentRelationInList->relationDependent;

				int actionIndex = currentRelationInList->relationGovernorIndex;
				int substanceIndex = currentRelationInList->relationDependentIndex;

				GIAEntityNode * actionEntity = GIAEntityNodeArray[actionIndex];
				GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "defineClausalComplementProperties: found RELATION_TYPE_CLAUSAL_COMPLEMENT" << endl;				
				//cout << "actionName = " << actionEntity->entityName << endl;
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				#endif

				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CCCOMP; 	//eg The guy, that John said was blue, left early in the morning. / He says that you like to swim.
				GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(actionEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif


