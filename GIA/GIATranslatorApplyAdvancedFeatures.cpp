/*******************************************************************************
 *
 * File Name: GIATranslatorApplyAdvancedFeatures.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorApplyAdvancedFeatures.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"




void extractDates(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPparserType)
{	
	#ifdef GIA_USE_RELEX
	if(NLPparserType == GIA_NLP_PARSER_RELEX)
	{
		extractDatesRelex(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	else if(NLPparserType == GIA_USE_STANFORD_CORENLP)
	{
		extractDatesStanfordCoreNLP(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);
	}	
	#endif
}

#ifdef GIA_USE_STANFORD_CORENLP
void extractDatesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eliminate all redundant date relations eg num(December-4, 3rd-5)/num(December-4, 1990-7)/nn(3rd-5, December-4)/appos(3rd-5, 1990-7), where both the governer and the dependent have NER tag set to DATE

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		GIAEntityNode * governerEntity = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
		GIAEntityNode * dependentEntity = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];
		if((governerEntity->NERTemp == FEATURE_NER_DATE) && (dependentEntity->NERTemp == FEATURE_NER_DATE))
		{
			governerEntity->disabled = true;
			dependentEntity->disabled = true;
		}
		
		currentRelationInList = currentRelationInList->next;
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
					cout << "error: isolated date concept node found (ie has no instance)" << endl;
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
						cout << "error: isolated date node found (not declared as a time condition)" << endl;
						#else
						cout << "error: [confidential 1]" << endl;	
						#endif
						exit(0);	//remove this later
					}
				}
				else
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "error: isolated date node found (not declared as a time condition)" << endl;
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
							cout << "error: isolated date concept node found (ie has no instance)" << endl;
							#else
							cout << "error: [confidential 0]" << endl;	
							#endif
						}
											
						GIAEntityNode * timeConditionEntity = timeEntity;

						if(timeConditionEntity->entityName == currentRelationInList->relationFunction)
						{	
							if(timeEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
							{
								if(timeEntity->timeConditionNode != NULL)
								{									
									if(currentRelationInList->relationType == RELATION_TYPE_DATE_DAY)
									{
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
										GIAEntityNodeArray[currentRelationInList->relationArgumentIndex]->disabled = true;
										#endif
				
										//http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
											//The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.	[eg "3rd" -> 3]
										string dayOfMonthString = currentRelationInList->relationArgument;
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
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
										GIAEntityNodeArray[currentRelationInList->relationArgumentIndex]->disabled = true;
										#endif	
																		
										string yearString = currentRelationInList->relationArgument;
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
								cout << "error: isolated date node found (not declared as a time condition)" << endl;
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

void extractQuantities(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
		{
			GIAEntityNode * quantityEntity = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
			if(quantityEntity->AssociatedInstanceNodeList.size() >= 1)
			//if(quantityEntity->AssociatedInstanceNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector				
			{
				GIAEntityNode * quantityProperty = quantityEntity->AssociatedInstanceNodeList.back();
				quantityProperty->hasQuantity = true;
				quantityProperty->quantityNumberString = currentRelationInList->relationArgument;
				#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
				GIAEntityNodeArray[currentRelationInList->relationArgumentIndex]->disabled = true;
				#endif
				int quantityNumberInt = calculateQuantityNumberInt(quantityProperty->quantityNumberString);
				quantityProperty->quantityNumber = quantityNumberInt;

				if(currentRelationInList->relationArgument == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{//update comparison variable (set it to the quantity)	
					quantityProperty->isQuery = true;
					GIAEntityNodeArray[currentRelationInList->relationArgumentIndex]->isQuery = false;
					comparisonVariableNode = quantityProperty;		
				}

				//now locate quantity modifiers and multiplicators
				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					//cout << "here1" << endl;
					//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

					if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
					{	
						if(currentRelationInList2->relationFunction == currentRelationInList->relationFunction)
						{
							//cout << "AAAA" << endl;

							/*
							int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationArgument);
							quantityProperty->quantityModifier = quantityModifierInt;
							*/
							quantityProperty->quantityModifierString = currentRelationInList2->relationArgument;

							//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)	
							GIAEntityNode * entityNode = quantityProperty;
							GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex];
							//GIAEntityNode * conditionTypeConceptEntity = quantityProperty->quantityModifierString;
							
							string conditionTypeName = "quantityModifier";	//quantityProperty->quantityModifierString //CHECKTHIS; 
							long entityIndex = -1;
							bool entityAlreadyExistant = false;
							GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);
							
							addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionTypeConceptEntity);

						}

					}	
					if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
					{
						if(currentRelationInList2->relationFunction == currentRelationInList->relationArgument)
						{
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
							GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex]->disabled = true;
							#endif
							
							int quantityMultiplierInt = calculateQuantityMultiplierInt(currentRelationInList2->relationArgument);
							quantityProperty->quantityNumber = quantityProperty->quantityNumber * quantityMultiplierInt;
							quantityProperty->hasQuantityMultiplier = true;
						}						
					}						

					currentRelationInList2 = currentRelationInList2->next;
				}	


				bool relationTypeQuantityArgumentImplyMeasurePer = false;
				for(int i=0; i<RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList->relationArgument == relationTypeQuantityArgumentImplyMeasurePerNameArray[i])
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
						if(currentRelationInList2->relationArgument == currentRelationInList->relationFunction)
						{		
							entityToConnectMeasurePerEntity = GIAEntityNodeArray[currentRelationInList2->relationFunctionIndex];	//eg row
							foundQuantityOwner = true;
						}	

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
						newQuantityTimesEntity->id = currentEntityNodeIDInCompleteList;
						newQuantityTimesEntity->idSecondary = currentEntityNodeIDInPropertyEntityNodesList;

						entityNodesCompleteList->push_back(newQuantityTimesEntity);
						currentEntityNodeIDInCompleteList++;
						propertyEntityNodesList->push_back(newQuantityTimesEntity);
						currentEntityNodeIDInPropertyEntityNodesList++;

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
						GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);

						//now add measure_per condition node
						addOrConnectPropertyConditionToEntity(newQuantityTimesEntity, measureProperty, conditionTypeConceptEntity);

					}

				}

			}								
		}
		currentRelationInList = currentRelationInList->next;		
	}
}

void extractMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
		bool pass = false;
		int measureTypeIndex = MEASURE_TYPE_UNDEFINED;
		for(int i=0; i<RELATION_TYPE_MEASURE_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == relationTypeMeasureNameArray[i])
			{
				measureTypeIndex = i;
				pass = true;
			}
		}																		
		if(pass)
		{
			int relationQuantityIndex = 0;
			int relationMeasureIndex = 0;
			if(currentRelationInList->relationType == RELATION_TYPE_MEASURE_PER)
			{
				relationQuantityIndex = currentRelationInList->relationFunctionIndex;
				relationMeasureIndex = currentRelationInList->relationArgumentIndex;			
			}
			else
			{

				relationQuantityIndex = currentRelationInList->relationArgumentIndex;
				relationMeasureIndex = currentRelationInList->relationFunctionIndex;										
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
				GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);

				if(currentRelationInList->relationType == RELATION_TYPE_MEASURE_PER)
				{
					addOrConnectPropertyConditionToEntity(quantityEntity, measurePropertyEntity, conditionTypeConceptEntity);
				}
				else
				{
					addOrConnectPropertyConditionToEntity(measurePropertyEntity, quantityEntity, conditionTypeConceptEntity);

				}
			}								
		}
		currentRelationInList = currentRelationInList->next;		
	}
}

void extractQualities(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
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
		//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_1) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_2) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_3))
		if(passed)
		{
			bool passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList);

			if(passed2)
			{			
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];

				if(propertyEntity->AssociatedInstanceNodeList.size() >= 1)
				//if(propertyEntity->AssociatedInstanceNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector
				{
					GIAEntityNode * qualityPropertyEntity = propertyEntity->AssociatedInstanceNodeList.back();
					qualityPropertyEntity->hasQuality = true;	//[eg2 The locked door.. / Jim runs quickly / Mr. Smith is late {_amod/_advmod/_predadj}]	
				}
			}
		}
		currentRelationInList = currentRelationInList->next;		
	}
}


void defineToBeAndToDoProperties(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
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
				GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];

				addOrConnectPropertyToEntity(entityNode, propertyEntity);				
			}
			else if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
			{
				#ifndef GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_PROPERTY_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE 
				#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
				if(GIAEntityNodeArray[currentRelationInList->relationFunctionIndex]->entityName != RELATION_FUNCTION_DEFINITION_1)
				{//this condition is required to support GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK			
				#endif
				#endif

						GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
						GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];
						GIAEntityNode * conditionTypeEntityNode;
						string conditionTypeEntityNodeName = currentRelationInList->relationType;
						long EntityIndex = -1;
						bool EntityAlreadyExistant = false;	
						conditionTypeEntityNode = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeEntityNodeName, &EntityAlreadyExistant, &EntityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);

						addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionTypeEntityNode);				

				#ifndef GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_PROPERTY_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE
				#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
				}
				#endif
				#endif				
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
}

void linkPropertiesParataxis(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(currentRelationInList->relationType == RELATION_TYPE_PARATAXIS)
		{
			//cout << "RELATION_TYPE_PARATAXIS" << endl;

			string propertyName = currentRelationInList->relationFunction; 
			string actionName = currentRelationInList->relationArgument; 
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				

			GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
			GIAEntityNode * actionEntity = GIAEntityNodeArray[relationArgumentIndex];
			//cout << "propertyName = " << propertyEntity->entityName << endl;
			//cout << "actionName = " << actionEntity->entityName << endl;

			addOrConnectPropertyToEntity(actionEntity, propertyEntity);
		}			
		currentRelationInList = currentRelationInList->next;
	}	
}

#ifdef GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
void defineConjunctionConditions(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)
{//NB defineConjunctionConditions() currently performs the same function as defineActionPropertyConditions()

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_STANFORD)
	{
	#endif	
			
		Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[relationFunctionIndex];				
			GIAEntityNode * actionOrPropertyConditionEntity = GIAEntityNodeArray[relationArgumentIndex];

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
				GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);	

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
				cout << "actionOrPropertyConditionEntity->entityName = " << actionOrPropertyConditionEntity->entityName << endl;
				cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl; 			
				#endif

				addPropertyConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);				
			}

			currentRelationInList = currentRelationInList->next;
		}	
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	}
	#endif				
}
#endif

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void defineClausalComplementProperties(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(currentRelationInList->relationType == STANFORD_RELATION_TYPE_CLAUSAL_COMPLEMENT)
		{
			//cout << "STANFORD_RELATION_TYPE_CLAUSAL_COMPLEMENT" << endl;
			//eg ccomp(say, like)	He says that you like to swim

			string actionName = currentRelationInList->relationFunction; 
			string propertyName = currentRelationInList->relationArgument; 
			
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				

			GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];
			GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
			//cout << "actionName = " << actionEntity->entityName << endl;
			//cout << "propertyName = " << propertyEntity->entityName << endl;

			addOrConnectPropertyToEntity(actionEntity, propertyEntity);
		}			
		currentRelationInList = currentRelationInList->next;
	}	
}
#endif
