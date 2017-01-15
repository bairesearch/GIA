/*******************************************************************************
 *
 * File Name: GIATranslatorApplyAdvancedFeatures.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j6c 01-May-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
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
				if(currentEntity->hasAssociatedTime)
				{
					//cout << "currentEntity->entityName = " << currentEntity->entityName << endl;
				
					GIAEntityNode * timeEntity = currentEntity;
					if(timeEntity->hasAssociatedInstanceTemp)	//CHECKTHIS; only use the instance if it was created in the current context (eg sentence)
					//if(timeEntity->AssociatedInstanceNodeList.size() >= 1)
					{
						timeEntity = timeEntity->AssociatedInstanceNodeList.back();
					}
					else
					{
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "error: isolated date concept node found (ie has no instance) [0]" << endl;
						#else
						cout << "error: [confidential 0]" << endl;	
						#endif
					}	

					if(timeEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
					{
						if(timeEntity->timeConditionNode != NULL)
						{
							//cout << "currentEntity->NormalizedNERTemp = " << currentEntity->NormalizedNERTemp << endl;
							timeEntity->timeConditionNode->conditionName = currentEntity->NormalizedNERTemp;
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
					else
					{
						#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5B_RELATIONS_COMPENSATE_FOR_INACCURATE_STANFORD_DATE_TAGGING
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "error: isolated date node found (not declared as a time condition) [2]" << endl;
						#else
						cout << "error: [confidential 2]" << endl;
						#endif
						exit(0);	//remove this later	
						#endif					
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
			if(currentEntity->hasAssociatedTime)
			{
				GIAEntityNode * timeEntity = currentEntity;
				if(timeEntity->hasAssociatedInstanceTemp)	//CHECKTHIS; only use the instance if it was created in the current context (eg sentence)
				//if(timeEntity->AssociatedInstanceNodeList.size() >= 1)
				{
					timeEntity = timeEntity->AssociatedInstanceNodeList.back();
				}
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "error: isolated date concept node found (ie has no instance) [0]" << endl;
					#else
					cout << "error: [confidential 0]" << endl;	
					#endif
				}	
				
				if(timeEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
				{
					if(timeEntity->timeConditionNode != NULL)
					{
						GIAEntityNode * timeConditionEntity = timeEntity;
						//cout << "as1" << endl;

						string monthString = timeConditionEntity->entityName;
						int monthInt = TIME_MONTH_UNDEFINED;
						for(int i=0; i<TIME_MONTH_NUMBER_OF_TYPES; i++)
						{
							if(monthString == timeMonthStringArray[i])
							{
								monthInt = i+1;
							}
						}
						timeConditionEntity->timeConditionNode->month = monthInt;

						//update/regenerate timeConditionName
						timeConditionEntity->timeConditionNode->conditionName = generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
							//OLD; timeConditionEntity->entityName
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
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "error: isolated date node found (not declared as a time condition) [2]" << endl;
					#else
					cout << "error: [confidential 2]" << endl;
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
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if((currentRelationInList->relationType == RELATION_TYPE_DATE_DAY) || (currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR))
			{	
				//now locate and fill corresponding time condition node;	
				for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
				{
					if(GIAEntityNodeArrayFilled[i])
					{
						GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
						if(currentEntity->hasAssociatedTime)
						{	
							GIAEntityNode * timeEntity = currentEntity;			
							if(timeEntity->hasAssociatedInstanceTemp)	//CHECKTHIS; only use the instance if it was created in the current context (eg sentence)
							//if(timeEntity->AssociatedInstanceNodeList.size() >= 1)
							{
								timeEntity = timeEntity->AssociatedInstanceNodeList.back();
							}
							else
							{
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "error: isolated date concept node found (ie has no instance) [0]" << endl;
								#else
								cout << "error: [confidential 0]" << endl;	
								#endif
							}

							GIAEntityNode * timeConditionEntity = timeEntity;

							if(timeConditionEntity->entityName == currentRelationInList->relationGovernor)
							{	
								if(timeEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
								{
									if(timeEntity->timeConditionNode != NULL)
									{									
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_DAY)
										{
											disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

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
											disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

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
									else
									{
										cout << "error: isolated date node found (not declared as a time condition)" << endl;
										exit(0);	//remove this later
									}										
								}
								else
								{
									#ifdef GIA_TRANSLATOR_DEBUG
									cout << "error: isolated date node found (not declared as a time condition) [3]" << endl;
									#else
									cout << "error: [confidential 3]" << endl;
									#endif
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
	//cout <<"4b2 pass; add time condition nodes to index [for fast lookup by time]" << endl;
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
			if(currentEntity->hasAssociatedTime)
			{
				GIAEntityNode * timeEntity = currentEntity;			
				if(timeEntity->hasAssociatedInstanceTemp)	//CHECKTHIS; only use the instance if it was created in the current context (eg sentence)
				//if(timeEntity->AssociatedInstanceNodeList.size() >= 1)
				{
					timeEntity = timeEntity->AssociatedInstanceNodeList.back();
				}
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "error: isolated date concept node found (ie has no instance)" << endl;
					#else
					cout << "error: [confidential 0]" << endl;	
					#endif
				}	
							
				if(timeEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
				{
					if(timeEntity->timeConditionNode != NULL)
					{
						//replace current entity time condition node with generated time condition node.

						int timeConditionEntityIndex = -1;
						bool argumentEntityAlreadyExistant = false;
						long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
						timeEntity->timeConditionNode = findOrAddTimeNodeByNumber(timeConditionNodesList, conceptEntityNamesList, timeConditionTotalTimeInSeconds, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true, timeEntity->timeConditionNode);

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
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "error: isolated date node found (not declared as a time condition)" << endl;
					#else
					cout << "error: [confidential 5]" << endl;
					#endif
					exit(0);	//remove this later						
				}

			}
		}
	}	
	#endif	
}
#endif


void extractQuantities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPfeatureParser)
{	
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		extractQuantitiesRelex(currentSentenceInList, GIAEntityNodeArray, conceptEntityNodesList);
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		extractQuantitiesStanfordCoreNLP(currentSentenceInList, GIAEntityNodeArray, conceptEntityNodesList);
	}	
	#endif
}

#ifdef GIA_USE_STANFORD_CORENLP
void extractQuantitiesStanfordCoreNLP(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				/*
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				*/

				GIAEntityNode * quantityEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];

				if(quantityEntity->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).	//OLD: if(!(quantityEntity->hasAssociatedTime))  [NO because must support times, eg The operation happened at 3:30pm. num(pm-6, 3:30-5)prep_at(happened-3, pm-6)]
				{
					if(quantityEntity->AssociatedInstanceNodeList.size() >= 1)
					//if(quantityEntity->AssociatedInstanceNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector				
					{
						GIAEntityNode * quantityProperty = quantityEntity->AssociatedInstanceNodeList.back();
						quantityProperty->hasQuantity = true;
						if(quantityEntity->NormalizedNERTemp != "")
						{
							quantityProperty->quantityNumberString = quantityEntity->NormalizedNERTemp;					
						}
						else
						{
							quantityProperty->quantityNumberString = currentRelationInList->relationDependent;
						}

						disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

						if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
						{//update comparison variable (set it to the quantity)	
							quantityProperty->isQuery = true;
							GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
							setComparisonVariableNode(quantityProperty);		
						}


						//now locate quantity modifiers
						Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
						while(currentRelationInList2->next != NULL)
						{	
							#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
							if(!(currentRelationInList2->disabled))
							{			
							#endif						
								//cout << "here1" << endl;
								//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

								if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
								{	
									if(currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor)
									{
										//cout << "AAAA" << endl;

										/*
										int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationDependent);
										quantityProperty->quantityModifier = quantityModifierInt;
										*/
										quantityProperty->quantityModifierString = currentRelationInList2->relationDependent;

										//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)	
										GIAEntityNode * entityNode = quantityProperty;
										GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
										//GIAEntityNode * conditionTypeConceptEntity = quantityProperty->quantityModifierString;

										string conditionTypeName = "quantityModifier";	//quantityProperty->quantityModifierString //CHECKTHIS; 
										long entityIndex = -1;
										bool entityAlreadyExistant = false;
										vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
										long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
										long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();
										GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
										if(entityAlreadyExistant)
										{
											applyEntityAlreadyExistsFunction(conditionTypeConceptEntity);
										}

										addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionTypeConceptEntity);

									}

								}
							#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
							}			
							#endif															

							currentRelationInList2 = currentRelationInList2->next;
						}																			
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
				//cout << "asd" << endl;
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

						if(currentEntity->AssociatedInstanceNodeList.size() >= 1)
						//if(quantityEntity->AssociatedInstanceNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector				
						{
							GIAEntityNode * quantityProperty = currentEntity->AssociatedInstanceNodeList.back();
							quantityProperty->hasQuantity = true;
							quantityProperty->quantityNumberString = currentEntity->NormalizedNERTemp;
							cout << "adding quantity: " << quantityProperty << endl; 
							cout << "quantityNumberString: " << currentEntity->NormalizedNERTemp << endl; 
						}
					}		
				}
				//cout << "asd2" << endl;
			}
		}
	}
	*/
}
#endif

#ifdef GIA_USE_RELEX
void extractQuantitiesRelex(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				/*
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				*/

				GIAEntityNode * quantityEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];

				/*no longer required as there is now a stanford specific function for parsing quantities
				if(!(quantityEntity->hasAssociatedTime))
				{//do not assume quantity entities when dealing with Stanford Dates, eg num(March-5, 11th-6)  / num(March-5, 1973-8)
				*/
					if(quantityEntity->AssociatedInstanceNodeList.size() >= 1)
					//if(quantityEntity->AssociatedInstanceNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector				
					{
						GIAEntityNode * quantityProperty = quantityEntity->AssociatedInstanceNodeList.back();
						quantityProperty->hasQuantity = true;
						quantityProperty->quantityNumberString = currentRelationInList->relationDependent;

						disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);

						int quantityNumberInt = calculateQuantityNumberInt(quantityProperty->quantityNumberString);
						quantityProperty->quantityNumber = quantityNumberInt;

						if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
						{//update comparison variable (set it to the quantity)	
							quantityProperty->isQuery = true;
							GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
							setComparisonVariableNode(quantityProperty);		
						}

						//now locate quantity modifiers and multiplicators
						Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
						while(currentRelationInList2->next != NULL)
						{	
							#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
							if(!(currentRelationInList2->disabled))
							{			
							#endif						
								//cout << "here1" << endl;
								//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

								if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
								{	
									if(currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor)
									{
										//cout << "AAAA" << endl;

										/*
										int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationDependent);
										quantityProperty->quantityModifier = quantityModifierInt;
										*/
										quantityProperty->quantityModifierString = currentRelationInList2->relationDependent;

										//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)	
										GIAEntityNode * entityNode = quantityProperty;
										GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
										//GIAEntityNode * conditionTypeConceptEntity = quantityProperty->quantityModifierString;

										string conditionTypeName = "quantityModifier";	//quantityProperty->quantityModifierString //CHECKTHIS; 
										long entityIndex = -1;
										bool entityAlreadyExistant = false;
										vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
										long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
										long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();
										GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
										if(entityAlreadyExistant)
										{
											applyEntityAlreadyExistsFunction(conditionTypeConceptEntity);
										}

										addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionTypeConceptEntity);

									}

								}	
								if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
								{
									if(currentRelationInList2->relationGovernor == currentRelationInList->relationDependent)
									{
										disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]);

										int quantityMultiplierInt = calculateQuantityMultiplierInt(currentRelationInList2->relationDependent);
										quantityProperty->quantityNumber = quantityProperty->quantityNumber * quantityMultiplierInt;
										quantityProperty->hasQuantityMultiplier = true;
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
								disconnectNodeFromAllButDefinitions(quantityProperty);

								GIAEntityNode * measureProperty = quantityProperty;	//convert quantity property to measure property
								measureProperty->hasQuantity = false;
								measureProperty->hasMeasure = true;
								measureProperty->measureType = MEASURE_TYPE_PER;						

								GIAEntityNode * newQuantityTimesEntity = new GIAEntityNode();
								long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
								long * currentEntityNodeIDInPropertyEntityNodesList = getCurrentEntityNodeIDInPropertyEntityNodesList();
								newQuantityTimesEntity->id = *currentEntityNodeIDInCompleteList;
								newQuantityTimesEntity->idSecondary = *currentEntityNodeIDInPropertyEntityNodesList;

								vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
								entityNodesCompleteList->push_back(newQuantityTimesEntity);
								(*currentEntityNodeIDInCompleteList)++;
								vector<GIAEntityNode*> * propertyEntityNodesList = getTranslatorPropertyEntityNodesList();
								propertyEntityNodesList->push_back(newQuantityTimesEntity);
								(*currentEntityNodeIDInPropertyEntityNodesList)++;

								newQuantityTimesEntity->entityName = "times";

								//reconnect refreshed quanity (times) node;
								addOrConnectPropertyToEntity(entityToConnectMeasurePerEntity, newQuantityTimesEntity);

								if(newQuantityTimesEntity->hasAssociatedInstanceTemp)
								{//assumed true since its property was just explicitly created
									newQuantityTimesEntity = newQuantityTimesEntity->AssociatedInstanceNodeList.back();
								}
								newQuantityTimesEntity->hasQuantity = true;
								newQuantityTimesEntity->quantityNumber = 1;
								newQuantityTimesEntity->quantityNumberString = "1";

								string conditionTypeName = RELATION_TYPE_MEASURE_PER;
								long entityIndex = -1;
								bool entityAlreadyExistant = false;
								long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();						
								GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
								if(entityAlreadyExistant)
								{
									applyEntityAlreadyExistsFunction(conditionTypeConceptEntity);
								}

								//now add measure_per condition node
								addOrConnectPropertyConditionToEntity(newQuantityTimesEntity, measureProperty, conditionTypeConceptEntity);

							}

						}
					/*
					}
					*/
				}								
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		currentRelationInList = currentRelationInList->next;		
	}
}
#endif

void extractMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
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
				if(measureEntity->AssociatedInstanceNodeList.size() >= 1)
				//if(measureEntity->AssociatedInstanceNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector
				{
					GIAEntityNode * measurePropertyEntity = measureEntity->AssociatedInstanceNodeList.back();
					measurePropertyEntity->hasMeasure = true;
					measurePropertyEntity->measureType = measureTypeIndex;

					#ifdef GIA_TRANSLATOR_DEBUG									
					cout << "measurePropertyName = " << measurePropertyEntity->entityName << endl;
					cout << "quantityEntityName = " << quantityEntity->entityName << endl;
					#endif

					string conditionTypeName = relationTypeMeasureNameArray[measureTypeIndex];
					long entityIndex = -1;
					bool entityAlreadyExistant = false;
					vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
					long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
					long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();				
					GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
					if(entityAlreadyExistant)
					{
						applyEntityAlreadyExistsFunction(conditionTypeConceptEntity);
					}

					if(measureDependencyFound)
					{
						addOrConnectPropertyConditionToEntity(quantityEntity, measurePropertyEntity, conditionTypeConceptEntity);
					}
					else
					{
						addOrConnectPropertyConditionToEntity(measurePropertyEntity, quantityEntity, conditionTypeConceptEntity);

					}
				}								
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		currentRelationInList = currentRelationInList->next;		
	}
}

void extractQualities(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
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
					GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationDependentIndex];

					if(propertyEntity->AssociatedInstanceNodeList.size() >= 1)
					//if(propertyEntity->AssociatedInstanceNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector
					{
						GIAEntityNode * qualityPropertyEntity = propertyEntity->AssociatedInstanceNodeList.back();
						qualityPropertyEntity->hasQuality = true;	//[eg2 The locked door.. / Jim runs quickly / Mr. Smith is late {_amod/_advmod/_predadj}]	
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		currentRelationInList = currentRelationInList->next;		
	}
}


void defineToBeAndToDoProperties(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

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
					GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
					GIAEntityNode * propertyEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];

					addOrConnectPropertyToEntity(entityNode, propertyEntity);				
				}
				else if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
				{
					#ifndef GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_PROPERTY_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE 
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
					if(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->entityName != RELATION_ENTITY_BE)
					{//this condition is required to support GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK			
					#endif
					#endif

						GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
						GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
						GIAEntityNode * conditionTypeEntityNode;
						string conditionTypeEntityNodeName = currentRelationInList->relationType;
						long EntityIndex = -1;
						bool EntityAlreadyExistant = false;	
						vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
						long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
						long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();						
						conditionTypeEntityNode = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeEntityNodeName, &EntityAlreadyExistant, &EntityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
						if(EntityAlreadyExistant)
						{
							applyEntityAlreadyExistsFunction(conditionTypeEntityNode);
						}

						addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntityNode);				

					#ifndef GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_PROPERTY_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
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

void linkPropertiesParataxis(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
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
				//cout << "RELATION_TYPE_PARATAXIS" << endl;

				string propertyName = currentRelationInList->relationGovernor; 
				string actionName = currentRelationInList->relationDependent; 
				int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
				int relationDependentIndex = currentRelationInList->relationDependentIndex;				

				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationGovernorIndex];
				GIAEntityNode * actionEntity = GIAEntityNodeArray[relationDependentIndex];
				//cout << "propertyName = " << propertyEntity->entityName << endl;
				//cout << "actionName = " << actionEntity->entityName << endl;

				addOrConnectPropertyToEntity(actionEntity, propertyEntity);
			}	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif		
		currentRelationInList = currentRelationInList->next;
	}	
}

void defineConjunctionConditions(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{//NB defineConjunctionConditions() currently performs the same function as defineActionPropertyConditions()
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[relationGovernorIndex];				
			GIAEntityNode * actionOrPropertyConditionEntity = GIAEntityNodeArray[relationDependentIndex];

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
				//cout << "as1" << endl;

				//CHECK THIS; check order - either select action or property first; NB there should not be both an associated action and an associated property in a given "Temp" context
				if(actionOrPropertyEntity->hasAssociatedInstanceTemp)
				{
					actionOrPropertyEntity = actionOrPropertyEntity->AssociatedInstanceNodeList.back();	
				}		

				//cout << "as2" << endl;		

				//CHECK THIS; check order - either select action or property first; NB there should not be both an associated action and an associated property in a given "Temp" context
				if(actionOrPropertyConditionEntity->hasAssociatedInstanceTemp)
				{
					//cout << "actionOrPropertyConditionEntity->hasAssociatedInstanceTemp" << endl;
					actionOrPropertyConditionEntity = actionOrPropertyConditionEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
					//cout << "actionOrPropertyConditionEntity->entityName = " << actionOrPropertyConditionEntity->entityName << endl; 
				}

				//cout << "as3" << endl;


				string conditionTypeName = relationType;
				long entityIndex = -1;
				bool entityAlreadyExistant = false;
				//cout << "relationType = " << relationType << endl;
				vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
				long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
				long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();				
				GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);	
				if(entityAlreadyExistant)
				{
					applyEntityAlreadyExistsFunction(conditionTypeConceptEntity);
				}

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
				cout << "actionOrPropertyConditionEntity->entityName = " << actionOrPropertyConditionEntity->entityName << endl;
				cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl; 			
				#endif

				addConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);				
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
				//cout << "RELATION_TYPE_CLAUSAL_COMPLEMENT" << endl;
				//eg ccomp(say, like)	He says that you like to swim

				string actionName = currentRelationInList->relationGovernor; 
				string propertyName = currentRelationInList->relationDependent; 

				int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
				int relationDependentIndex = currentRelationInList->relationDependentIndex;				

				GIAEntityNode * actionEntity = GIAEntityNodeArray[relationGovernorIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationDependentIndex];
				//cout << "actionName = " << actionEntity->entityName << endl;
				//cout << "propertyName = " << propertyEntity->entityName << endl;

				addOrConnectPropertyToEntity(actionEntity, propertyEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif			
		currentRelationInList = currentRelationInList->next;
	}	
}
#endif
