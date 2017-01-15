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
 * File Name: GIAtranslatorApplyAdvancedFeatures.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t2l 24-July-2013
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * ?TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIAtranslatorApplyAdvancedFeatures.h"
#include "GIAdatabase.h"








void extractDates(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser)
{
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		/*
		eg The battle happened on March 11th, 1973.  _date_day(December, 3rd) /_date_year(December, 1990) 
		*/
		extractDatesRelex(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		extractDatesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	}
	#endif
}

#ifdef GIA_USE_STANFORD_CORENLP
void extractDatesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * currentEntity = GIAentityNodeArray[i];
			if(!(currentEntity->disabled))
			{
				if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
				{
					GIAentityNode * timeEntity = currentEntity;
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
								//cout << "(getPrimaryConceptNodeDefiningInstance(timeEntity))->NormalizedNERtemp = " << (getPrimaryConceptNodeDefiningInstance(timeEntity))->NormalizedNERtemp << endl;
								#endif
								timeEntity->timeConditionNode->conditionName = (getPrimaryConceptNodeDefiningInstance(timeEntity))->NormalizedNERtemp;
							}
							else
							{
							*/
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "timeEntity->NormalizedNERtemp = " << timeEntity->NormalizedNERtemp << endl;
								#endif
								timeEntity->timeConditionNode->conditionName = timeEntity->NormalizedNERtemp;

								//this case appears to be required for queries... (_%qvar/_%atTime), noting that qVar is not assigned a substance (but remains a concept node)
								/*
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "timeEntity->NormalizedNERtemp = " << timeEntity->NormalizedNERtemp << endl;
								cout << "error: getPrimaryConceptNodeDefiningInstance(timeEntity) != NULL [1b]" << endl;
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
void extractDatesRelex(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * currentEntity = GIAentityNodeArray[i];
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				GIAentityNode * timeEntity = currentEntity;
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
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if((currentRelationInList->relationType == RELATION_TYPE_DATE_DAY) || (currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR))
			{
				//now locate and fill corresponding time condition node;
				for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
				{
					if(GIAentityNodeArrayFilled[i])
					{
						GIAentityNode * currentEntity = GIAentityNodeArray[i];
						if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
						{
							GIAentityNode * timeEntity = currentEntity;
							GIAentityNode * timeConditionEntity = timeEntity;

							if(timeConditionEntity->entityName == currentRelationInList->relationGovernor)
							{
								if(timeEntity->timeConditionNode != NULL)
								{
									if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
									{
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_DAY)
										{
											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

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
											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

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
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * currentEntity = GIAentityNodeArray[i];
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


void extractQuantities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPfeatureParser)
{
	/*
	eg He lost three dollars. /   He lost almost three dollars. / He lost three hundred dollars.	_quantity(dollar, three) / _quantity_mod(three, almost) / _quantity_mult(hundred, three) 
	*/
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		extractQuantitiesRelex(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		extractQuantitiesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);
	}
	#endif
}

#ifdef GIA_USE_STANFORD_CORENLP
void extractQuantitiesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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

				GIAentityNode * quantityEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

				if(quantityEntity->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).
				{
					GIAentityNode * quantitySubstance = quantityEntity;
					quantitySubstance->hasQuantity = true;
					if((quantitySubstance->NormalizedNERtemp != "") && (quantitySubstance->NormalizedNERtemp != "0.0"))		//added 0.0 for a stanford anomaly 11 May 2012
					{
						quantitySubstance->quantityNumberString = quantitySubstance->NormalizedNERtemp;
					}
					else
					{
						quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;
					}

					int quantityNumberInt = calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;

					disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						setComparisonVariableNode(quantitySubstance);
					}


					//now locate quantity modifiers
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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
									GIAentityNode * entityNode = quantitySubstance;
									GIAentityNode * conditionEntityNode = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
									//GIAentityNode * conditionTypeEntity = quantitySubstance->quantityModifierString;

									string conditionTypeName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

									bool entityAlreadyExistant = false;
									GIAentityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

									bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

									#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
									GIAentityNodeArray[FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
									#else
									addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
									#endif
								}

							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						}
						#endif

						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}



	/*
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * currentEntity = GIAentityNodeArray[i];
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

						GIAentityNode * quantitySubstance = currentEntity;
						quantitySubstance->hasQuantity = true;
						quantitySubstance->quantityNumberString = currentEntity->NormalizedNERtemp;
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "adding quantity: " << quantitySubstance << endl;
						cout << "quantityNumberString: " << currentEntity->NormalizedNERtemp << endl;
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
void extractQuantitiesRelex(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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

				GIAentityNode * quantityEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];


				GIAentityNode * quantitySubstance = quantityEntity;
				quantitySubstance->hasQuantity = true;
				quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

				int quantityNumberInt = calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
				quantitySubstance->quantityNumber = quantityNumberInt;

				if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{//update comparison variable (set it to the quantity)
					quantitySubstance->isQuery = true;
					GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
					setComparisonVariableNode(quantitySubstance);
				}

				//now locate quantity modifiers and multiplicators
				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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
								GIAentityNode * entityNode = quantitySubstance;
								GIAentityNode * conditionEntityNode = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
								//GIAentityNode * conditionTypeEntity = quantitySubstance->quantityModifierString;

								string conditionTypeName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

								bool entityAlreadyExistant = false;
								GIAentityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
								#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
								GIAentityNodeArray[FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
								#else
								addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
								#endif
							}

						}
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
						{
							if(currentRelationInList2->relationGovernor == currentRelationInList->relationDependent)
							{
								disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList2->relationDependentIndex]);

								int quantityMultiplierInt = calculateQuantityMultiplierInt(currentRelationInList2->relationDependent);
								quantitySubstance->quantityNumber = quantitySubstance->quantityNumber * quantityMultiplierInt;
								quantitySubstance->hasQuantityMultiplier = true;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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

					GIAentityNode * entityToConnectMeasurePerEntity;
					bool foundQuantityOwner = false;
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationDependent == currentRelationInList->relationGovernor)
							{
								entityToConnectMeasurePerEntity = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex];	//eg row
								foundQuantityOwner = true;
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						}
						#endif

						currentRelationInList2 = currentRelationInList2->next;
					}

					if(foundQuantityOwner)
					{
						//disconnect quantity node from existing connections (not including definitions) - NOT YET CODED.
						disconnectNodeFromAllButDefinitions(quantitySubstance);

						GIAentityNode * measureSubstance = quantitySubstance;	//convert quantity substance to measure substance
						measureSubstance->hasQuantity = false;
						measureSubstance->hasMeasure = true;
						measureSubstance->measureType = MEASURE_TYPE_PER;

						GIAentityNode * newQuantityTimesEntity = new GIAentityNode();
						#ifdef GIA_USE_DATABASE
						newQuantityTimesEntity->added = true;
						#endif

						if(getSaveNetwork())
						{
							long * currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinCompleteList();
							long * currentEntityNodeIDInSubstanceEntityNodesList = getCurrentEntityNodeIDinSubstanceEntityNodesList();
							newQuantityTimesEntity->idActiveList = *currentEntityNodeIDinCompleteList;
							newQuantityTimesEntity->idActiveEntityTypeList = *currentEntityNodeIDInSubstanceEntityNodesList;

							vector<GIAentityNode*> *entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();
							entityNodesActiveListComplete->push_back(newQuantityTimesEntity);
							(*currentEntityNodeIDinCompleteList)++;
							vector<GIAentityNode*> * entityNodesActiveListSubstances = getTranslatorSubstanceEntityNodesList();
							entityNodesActiveListSubstances->push_back(newQuantityTimesEntity);
							(*currentEntityNodeIDInSubstanceEntityNodesList)++;
						}
						else
						{
							long * currentEntityNodeIDinCompleteList = getCurrentEntityNodeIDinSentenceCompleteList();
							newQuantityTimesEntity->idActiveList = *currentEntityNodeIDinCompleteList;
							(*currentEntityNodeIDinCompleteList)++;
						}

						newQuantityTimesEntity->entityName = "times";

						//reconnect refreshed quantity (times) node;
						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
						addOrConnectPropertyToEntity(entityToConnectMeasurePerEntity, newQuantityTimesEntity, sameReferenceSet);

						if(newQuantityTimesEntity->hasAssociatedInstanceTemp)
						{//assumed true since its substance was just explicitly created
							newQuantityTimesEntity = (newQuantityTimesEntity->associatedInstanceNodeList->back())->entity;
						}
						newQuantityTimesEntity->hasQuantity = true;
						newQuantityTimesEntity->quantityNumber = 1;
						newQuantityTimesEntity->quantityNumberString = "1";

						string conditionTypeName = RELATION_TYPE_MEASURE_PER;

						bool entityAlreadyExistant = false;
						GIAentityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

						//now add measure_per condition node
						sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAentityNodeArray[FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN] = addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionTypeEntity, sameReferenceSet);
						#else
						addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionTypeEntity, sameReferenceSet);
						#endif
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

void extractMeasures(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
{
	/*
	eg The boy is 4 feet away. / Take these 4 times a day. / The boy is 4 feet tall. / The birthday boy is 12 years old.	_measure_distance(away, foot) / _measure_per(times, day) / _measure_size(tall, feet) / _measure_time(old, years) 
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);	
	param.entityNodesActiveListConcepts = entityNodesActiveListConcepts;
	param.numberOfRelations = 1;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;	//coincidentially this condition holds for both cases
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;	//coincidentially this condition holds for both 3 cases
	#ifdef GIA_USE_ADVANCED_REFERENCING
	param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE;
	#endif	
		
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationArrayTest[REL1][REL_ENT3] = true; paramA.relationArrayTest[REL1][REL_ENT3] = relationTypeMeasureDependencyNameArray; paramA.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_MEASURE_DEPENDENCY_NUMBER_OF_TYPES;	
	paramA.mustGenerateConditionTypeName = true; paramA.conditionTypeEntityDefaultName = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN; paramA.conditionTypeEntityDefaultIndex = FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN; //NB GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK does not support the full array of Relex measure/dependency types, and FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN is now used instead of FEATURE_INDEX_OF_MEASURE_UNKNOWN to allow both measure and measure per entities to be used in same sentence
	genericDependecyRelationInterpretation(&paramA, 1);
		
	GIAgenericDepRelInterpretationParameters paramB = param;	
	paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypeMeasureNotDependencyNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_MEASURE_NOT_DEPENDENCY_NUMBER_OF_TYPES;	
	paramB.mustGenerateConditionTypeName = true; paramB.conditionTypeEntityDefaultName = RELATION_TYPE_MEASURE_UNKNOWN; paramB.conditionTypeEntityDefaultIndex = FEATURE_INDEX_OF_MEASURE_UNKNOWN;	//NB GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK does not support the full array of Relex measure/dependency types
	genericDependecyRelationInterpretation(&paramB, 1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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

				GIAentityNode * measureEntity = GIAentityNodeArray[relationMeasureIndex];
				GIAentityNode * quantityEntity = GIAentityNodeArray[relationQuantityIndex];

				GIAentityNode * measureSubstanceEntity = measureEntity;
				measureSubstanceEntity->hasMeasure = true;
				measureSubstanceEntity->measureType = measureTypeIndex;

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "measureSubstanceName = " << measureSubstanceEntity->entityName << endl;
				cout << "quantityEntityName = " << quantityEntity->entityName << endl;
				#endif

				string conditionTypeName = relationTypeMeasureNameArray[measureTypeIndex];
				bool entityAlreadyExistant = false;
				GIAentityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_MEASURE_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

				#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
				if(measureDependencyFound)
				{
					GIAentityNodeArray[FEATURE_INDEX_OF_MEASURE_UNKNOWN] = addOrConnectConditionToEntity(quantityEntity, measureSubstanceEntity, conditionTypeEntity, sameReferenceSet);
				}
				else
				{
					GIAentityNodeArray[FEATURE_INDEX_OF_MEASURE_UNKNOWN] = addOrConnectConditionToEntity(measureSubstanceEntity, quantityEntity, conditionTypeEntity, sameReferenceSet);
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
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void extractQualities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
	/* 
	eg The broken pencil fell apart. / Giants are red. [Joe is happy.] / Tom runs quickly.	_amod(pencil, broken) / _predadj(giants, red) / _advmod(run, quick)
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeAdjectiveNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES;
	param.useRedistributeSpecialCaseQualityAssignment[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&param, 1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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
				#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				bool passed2 = true;
				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
				{
				#endif
					passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);
				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				}
				#endif
				
				if(passed2)
				{
				#endif
					int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
					int relationDependentIndex = currentRelationInList->relationDependentIndex;
					GIAentityNode * thingEntity = GIAentityNodeArray[relationGovernorIndex];
					GIAentityNode * substanceEntity = GIAentityNodeArray[relationDependentIndex];

					substanceEntity->isSubstanceQuality = true;	//[eg2 The locked door.. / Jim runs quickly / Mr. Smith is late {_amod/_advmod/_predadj}]
				#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				}
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif	
}


void defineToBeAndToDoPropertiesAndConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);	
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeComplementsNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES;	//redundant test
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
		
	/*
	eg The pastry tasted awesome. _to-be(taste[3], awesome[4]) + _subj(taste[3], pastry[2])
	_to-be(taste[3], awesome[4])
	_subj(taste[3], pastry[2])	//will have been already interpreted as an action by linkSubjectOrObjectRelationships()	
	*/
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_BE;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;	//eg The chicken ate the pie that tasted awesome.
	#endif	
	genericDependecyRelationInterpretation(&paramA, 1);
	
	/*
	eg Jezel likes to draw. _to-do(like[2], draw[4]) + _subj(like[2], Jezel[1])
	_to-do(like[2], draw[4])
	_subj(like[2], Jezel[1])	//will have been already interpreted as an action by linkSubjectOrObjectRelationships()
	*/
	#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;	//eg The chicken ate the pie that likes to draw.
	#endif		
	genericDependecyRelationInterpretation(&paramB, 1);
	#else
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	paramB.functionEntityRelationID[FUNC_ENT3] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;	
	paramB.conditionTypeEntityDefaultIndex = FEATURE_INDEX_OF_TODO_UNKNOWN;	
	#ifdef GIA_USE_ADVANCED_REFERENCING
	paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//eg The chicken ate the pie that likes to draw.
	#endif	
	genericDependecyRelationInterpretation(&paramB, 1);
	#endif
	
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
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
					//eg  The pastry tasted awesome. 
					int entityIndex = currentRelationInList->relationGovernorIndex;
					int substanceIndex = currentRelationInList->relationDependentIndex;

					GIAentityNode * entityNode = GIAentityNodeArray[entityIndex];
					GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

					bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES; 	//eg The chicken ate the pie that tasted awesome.
					GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(entityNode, substanceEntity, sameReferenceSet);
				}
				else if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
				{
					//eg Jezel likes to draw. 
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
					if(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]->entityName != RELATION_ENTITY_BE)
					{//this condition is required to support GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
						//ORIGINAL + NEW - interpret as substance link
						int entityIndex = currentRelationInList->relationGovernorIndex;
						int substanceIndex = currentRelationInList->relationDependentIndex;

						GIAentityNode * entityNode = GIAentityNodeArray[entityIndex];
						GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES; 	//eg The chicken ate the pie that likes to draw.
						GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(entityNode, substanceEntity, sameReferenceSet);					
					}
					#else
						//INTERMEDIATE - interpret as condition link
						GIAentityNode * entityNode = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
						GIAentityNode * conditionEntityNode = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
						string conditionTypeEntityNodeName = currentRelationInList->relationType;

						bool entityAlreadyExistant = false;
						GIAentityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_TODO_UNKNOWN, &conditionTypeEntityNodeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//eg The chicken ate the pie that likes to draw.

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAentityNodeArray[FEATURE_INDEX_OF_TODO_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
						#else
						addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntity, sameReferenceSet);
						#endif					
					#endif
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void linkPropertiesParataxis(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);	
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PARATAXIS;	
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PARATAXIS;
	#endif	
	genericDependecyRelationInterpretation(&param, 1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_PARATAXIS)
			{
				string substanceName = currentRelationInList->relationGovernor;
				string actionName = currentRelationInList->relationDependent;
				int substanceIndex = currentRelationInList->relationGovernorIndex;
				int actionIndex = currentRelationInList->relationDependentIndex;

				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];
				GIAentityNode * actionEntity = GIAentityNodeArray[actionIndex];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "linkPropertiesParataxis: found RELATION_TYPE_PARATAXIS" << endl;				
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				//cout << "actionName = " << actionEntity->entityName << endl;
				#endif
				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PARATAXIS; 		//eg The guy, Akari said, left early in the morning.	[NB The guy, that Akari said was blue, left early in the morning. / He says that you like to swim.  does not generate parataxis, so these cases needn't be considered here...]
				GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(actionEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif	
}

void linkConjunctionConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	/*
	Tom and/or Max eat the cake.	conj_and(Tom[1], Max[3]) / conj_or(Tom[2], Max[4])
	*/
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);	
	param.entityNodesActiveListConcepts = entityNodesActiveListConcepts;
	param.numberOfRelations = 1;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	param.conditionTypeEntityDefaultIndex = FEATURE_INDEX_OF_CONJUNCTION_UNKNOWN;	//shouldn't be required as conditionType is converted from "conj_and"/"conj_or" to "and"/"or" (which should be detected as features within the sentence) 
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT3] = REL1; param.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE;
	#endif	
	
	GIAgenericDepRelInterpretationParameters paramA = param;	
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_AND;	
	paramA.conditionTypeEntityDefaultName = RELATION_TYPE_CONJUGATION_AND_BASIC; //change the conditionType name (_conj_and -> and)
	genericDependecyRelationInterpretation(&paramA, 1);
	
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_OR;
	paramB.conditionTypeEntityDefaultName = RELATION_TYPE_CONJUGATION_OR_BASIC; //change the conditionType name (_conj_or -> or)
	genericDependecyRelationInterpretation(&paramB, 1);
#else	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAentityNode * actionOrSubstanceEntity = GIAentityNodeArray[relationGovernorIndex];
			GIAentityNode * actionOrSubstanceConditionEntity = GIAentityNodeArray[relationDependentIndex];

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
				GIAentityNode * conditionTypeEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_CONJUNCTION_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

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
				GIAentityNodeArray[FEATURE_INDEX_OF_CONJUNCTION_UNKNOWN] = addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeEntity, sameReferenceSet);
				#else
				addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeEntity, sameReferenceSet);
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif	
}

//currently disabled;
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
#ifndef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
void defineClausalComplementProperties(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	/*
	eg He says that you like to swim. ccomp(say, like)
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);	
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CLAUSAL_COMPLEMENT;	
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CCCOMP;	//eg The guy, that Akari said was blue, left early in the morning. / He says that you like to swim.
	#endif	
	genericDependecyRelationInterpretation(&param, 1);
#else	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_CLAUSAL_COMPLEMENT)
			{
				//eg He says that you like to swim. ccomp(say, like)	

				string actionName = currentRelationInList->relationGovernor;
				string substanceName = currentRelationInList->relationDependent;

				int actionIndex = currentRelationInList->relationGovernorIndex;
				int substanceIndex = currentRelationInList->relationDependentIndex;

				GIAentityNode * actionEntity = GIAentityNodeArray[actionIndex];
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "defineClausalComplementProperties: found RELATION_TYPE_CLAUSAL_COMPLEMENT" << endl;				
				//cout << "actionName = " << actionEntity->entityName << endl;
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				#endif

				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CCCOMP; 	//eg The guy, that Akari said was blue, left early in the morning. / He says that you like to swim.
				GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(actionEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif	
}
#endif
#endif
