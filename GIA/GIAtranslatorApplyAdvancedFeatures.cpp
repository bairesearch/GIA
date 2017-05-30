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
 * File Name: GIAtranslatorApplyAdvancedFeatures.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b3i 25-May-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorApplyAdvancedFeatures.hpp"

//see XML for dynamic translation code



void GIAtranslatorApplyAdvancedFeaturesClass::applyAdvancedFeatures(GIAtranslatorVariablesClass* translatorVariables)
{
	this->extractDates(translatorVariables);

	this->extractQuantities(translatorVariables);

	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	this->defineActionConcepts2(translatorVariables);
	#endif

	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	this->updateConceptDesignationBasedPropertyOwnerContext(translatorVariables);
	#endif
}

void GIAtranslatorApplyAdvancedFeaturesClass::extractDates(GIAtranslatorVariablesClass* translatorVariables)
{
	#ifdef GIA_RELEX
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		/*
		eg The battle happened on March 11th, 1973.  _date_day(December, 3rd) /_date_year(December, 1990)
		*/
		this->extractDatesRelex(translatorVariables);
	}
	#endif
	#ifdef GIA_STANFORD_CORENLP
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		this->extractDatesStanfordCoreNLP(translatorVariables);
	}
	#endif
}

#ifdef GIA_STANFORD_CORENLP
void GIAtranslatorApplyAdvancedFeaturesClass::extractDatesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(translatorVariables->GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* currentEntity = translatorVariables->GIAentityNodeArray[i];
			if(!(currentEntity->disabled))
			{
				#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
				if(currentEntity->hasAssociatedTime)
				{
					this->addTimeToSubstance(currentEntity);	//note will overwrite any previous time condition nodes (eg tenseOnlyTimeCondition) - but these should not exist in the case of hasAssociatedTime
				#else
				if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
				{
				#endif
					GIAentityNode* timeEntity = currentEntity;

					if(timeEntity->timeConditionNode != NULL)
					{
						if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
						{
							/*
							if(!(timeEntity->instanceReverseNodeList->empty()))
							{//required for anomaly
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "(getPrimaryNetworkIndexNodeDefiningInstance(timeEntity))->NormalizedNERtemp = " << (getPrimaryNetworkIndexNodeDefiningInstance(timeEntity))->NormalizedNERtemp << endl;
								#endif
								timeEntity->timeConditionNode->conditionName = (getPrimaryNetworkIndexNodeDefiningInstance(timeEntity))->NormalizedNERtemp;
							}
							else
							{
							*/
								timeEntity->timeConditionNode->conditionName = timeEntity->NormalizedNERtemp;

								//this case appears to be required for queries... (_%qvar/_%atTime), noting that qVar is not assigned a substance (but remains a networkIndex node)
								/*
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "timeEntity->NormalizedNERtemp = " << timeEntity->NormalizedNERtemp << endl;
								cout << "error: getPrimaryNetworkIndexNodeDefiningInstance(timeEntity) != NULL [1b]" << endl;
								#else
								cout << "error: [confidential 1b]" << endl;
								#endif
								exit(EXIT_ERROR);
								*/
							/*
							}
							*/
						}
					}
					else
					{
						#ifndef GIA_TRANSLATOR_DEBUG
						cout << "error: [confidential 1]" << endl;
						#endif
						exit(EXIT_ERROR);	//remove this later
					}
				}
			}
		}
	}

	//add time condition node to isolated entities with NERTemp == FEATURE_NER_DATE		{or have they already been added?}
}
#endif

#ifdef GIA_RELEX
void GIAtranslatorApplyAdvancedFeaturesClass::extractDatesRelex(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(translatorVariables->GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* currentEntity = translatorVariables->GIAentityNodeArray[i];
			#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
			if(currentEntity->hasAssociatedTime)
			{
				this->addTimeToSubstance(currentEntity);	//note will overwrite any previous time condition nodes (eg tenseOnlyTimeCondition) - but these should not exist in the case of hasAssociatedTime
			#else
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
			#endif
				GIAentityNode* timeEntity = currentEntity;
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
						timeEntity->timeConditionNode->conditionName = GIAconditionNodeClass.generateDateTimeConditionName(timeEntity->timeConditionNode->dayOfMonth, timeEntity->timeConditionNode->month, timeEntity->timeConditionNode->year);
							//OLD; timeConditionEntity->entityName
					}
				}
				else
				{
					#ifndef GIA_TRANSLATOR_DEBUG
					cout << "error: [confidential 1]" << endl;
					#endif
					exit(EXIT_ERROR);	//remove this later
				}
			}
		}
	}
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
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
					if(translatorVariables->GIAentityNodeArrayFilled[i])
					{
						GIAentityNode* currentEntity = translatorVariables->GIAentityNodeArray[i];
						if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
						{
							GIAentityNode* timeEntity = currentEntity;
							GIAentityNode* timeConditionEntity = timeEntity;

							if(timeConditionEntity->entityName == currentRelationInList->relationGovernor)
							{
								if(timeEntity->timeConditionNode != NULL)
								{
									if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
									{
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_DAY)
										{
											GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

											//http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
												//The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.	[eg "3rd" -> 3]
											string dayOfMonthString = currentRelationInList->relationDependent;
											int dayOfMonthInt = SHAREDvars.convertStringToInt(dayOfMonthString);
											timeConditionEntity->timeConditionNode->dayOfMonth = dayOfMonthInt;


											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = GIAconditionNodeClass.generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
											//timeConditionEntity->entityName  =
										}
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR)
										{
											GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

											string yearString = currentRelationInList->relationDependent;
											int yearInt = SHAREDvars.convertStringToInt(yearString);
											timeConditionEntity->timeConditionNode->year = yearInt;


											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = GIAconditionNodeClass.generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
											//tempTimeCondition->entityName
										}
									}
								}
								else
								{
									cout << "error: isolated date node found (not declared as a time condition)" << endl;
									exit(EXIT_ERROR);	//remove this later
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
	#ifdef GIA_TIME_NODE_INDEXING
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(translatorVariables->GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* currentEntity = translatorVariables->GIAentityNodeArray[i];
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				if(timeEntity->timeConditionNode != NULL)
				{
					if(!(timeEntity->timeConditionNode->tenseOnlyTimeCondition))
					{
						//replace current entity time condition node with generated time condition node.

						int timeConditionEntityIndex = INT_DEFAULT_VALUE;
						bool argumentEntityAlreadyExistant = false;
						long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
						timeEntity->timeConditionNode = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, networkIndexEntityNamesList, timeConditionTotalTimeInSeconds, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true, timeEntity->timeConditionNode);
					}
				}
				else
				{
					#ifndef GIA_TRANSLATOR_DEBUG
					cout << "error: [confidential 4]" << endl;
					#endif
					exit(EXIT_ERROR);	//remove this later
				}
			}
		}
	}
	#endif
}
#endif

void GIAtranslatorApplyAdvancedFeaturesClass::addTimeToSubstance(GIAentityNode* timeConditionEntity)
{
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;

	GIAtimeConditionNode* newTimeCondition = new GIAtimeConditionNode();

	timeConditionEntity->timeConditionNode = newTimeCondition;

}

void GIAtranslatorApplyAdvancedFeaturesClass::extractQuantities(GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	eg He lost three dollars. /   He lost almost three dollars. / He lost three hundred dollars.	_quantity(dollar, three) / _quantity_mod(three, almost) / _quantity_mult(hundred, three)
	*/
	#ifdef GIA_RELEX
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		this->extractQuantitiesRelex(translatorVariables);
	}
	#endif
	#ifdef GIA_STANFORD_CORENLP
	//NB GIA could support quantity extraction with Stanford Parser only (ie without Stanford CoreNLP as the NLPfeatureParser) but this has not been implemented
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		this->extractQuantitiesStanfordCoreNLP(translatorVariables);
	}
	#endif
}

#ifdef GIA_STANFORD_CORENLP
void GIAtranslatorApplyAdvancedFeaturesClass::extractQuantitiesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;

	#ifdef GIA_TRANSLATOR_CONVERT_AMOD_WITH_NUMBERS_TO_QUANTITY_RELATION
	//convert amod(piece, 1) to num(piece, 1)
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_AMOD)
			{
				string s = currentRelationInList->relationDependent;
				int value = SHAREDvars.convertStringToInt(s);
				if(value || (s == "0"))
				{
					currentRelationInList->relationType = RELATION_TYPE_QUANTITY;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
	#endif

	currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{

				GIAentityNode* quantityEntity = translatorVariables->GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

				if(quantityEntity->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).
				{
					#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
					GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, currentRelationInList->relationGovernorIndex, currentRelationInList->relationDependentIndex, false);
					#endif

					GIAentityNode* quantitySubstance = quantityEntity;
					quantitySubstance->hasQuantity = true;
					if((quantitySubstance->NormalizedNERtemp != "") && (quantitySubstance->NormalizedNERtemp != "0.0") && (quantitySubstance->NERTemp != FEATURE_NER_DURATION))		//added (quantitySubstance->NERTemp != FEATURE_NER_DURATION) for Stanford Parser 2014-01-04 //added (quantitySubstance->NormalizedNERtemp != "0.0") for a stanford anomaly 11 May 2012
					{
						quantitySubstance->quantityNumberString = quantitySubstance->NormalizedNERtemp;
					}
					else
					{
						quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;
					}

					int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;
					quantitySubstance->entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//added 2a11a [because defineConcepts() does not have access to quantity data]

					GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					}

					//now locate quantity modifiers
					GIArelation* currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
							{
								if((currentRelationInList2->relationGovernor == currentRelationInList->relationDependent) || (currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor))	//OLD before GIA 2b7d/2c2b: if(currentRelationInList2->relationGovernor ==currentRelationInList->relationGovernor)
								{
									/*
									int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationDependent);
									quantitySubstance->quantityModifier = quantityModifierInt;
									*/
									quantitySubstance->quantityModifierString = currentRelationInList2->relationDependent;

									//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)
									GIAentityNode* entityNode = quantitySubstance;
									GIAentityNode* relationshipObjectEntity = translatorVariables->GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
									//GIAentityNode* conditionRelationshipEntity = quantitySubstance->quantityModifierString;

									string conditionName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

									bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
									//GIAtranslatorGeneric.addRelationshipArtificialToEntityAndGenerateSemanticRelations(entityNode, relationshipObjectEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, conditionName, translatorVariables, false, currentRelationInList->relationGovernorIndex, currentRelationInList2->relationDependentIndex);	//GIA semantic parser output is unsupported for artifical conditions
									GIAtranslatorOperations.addConditionRelationshipArtificialToEntity(entityNode, relationshipObjectEntity, conditionName, sameReferenceSet, translatorVariables);
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
}
#endif

#ifdef GIA_RELEX
void GIAtranslatorApplyAdvancedFeaturesClass::extractQuantitiesRelex(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
				GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, currentRelationInList->relationGovernorIndex, currentRelationInList->relationDependentIndex, false);
				#endif


				GIAentityNode* quantityEntity = translatorVariables->GIAentityNodeArray[currentRelationInList->relationGovernorIndex];


				GIAentityNode* quantitySubstance = quantityEntity;
				quantitySubstance->hasQuantity = true;
				quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

				GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

				int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
				quantitySubstance->quantityNumber = quantityNumberInt;
				quantitySubstance->entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//added 2a11a [because defineConcepts() does not have access to quantity data]

				if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{//update comparison variable (set it to the quantity)
					quantitySubstance->isQuery = true;
					translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
					GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
				}

				//now locate quantity modifiers and multiplicators
				GIArelation* currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
						{
							if((currentRelationInList2->relationGovernor == currentRelationInList->relationDependent) || (currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor))	//OLD before GIA 2c2b: if(currentRelationInList2->relationGovernor ==currentRelationInList->relationGovernor)
							{
								/*
								int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationDependent);
								quantitySubstance->quantityModifier = quantityModifierInt;
								*/
								quantitySubstance->quantityModifierString = currentRelationInList2->relationDependent;

								//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)
								GIAentityNode* entityNode = quantitySubstance;
								GIAentityNode* relationshipObjectEntity = translatorVariables->GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
								//GIAentityNode* conditionRelationshipEntity = quantitySubstance->quantityModifierString;
								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

								string conditionName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;
								//GIAtranslatorGeneric.addRelationshipArtificialToEntityAndGenerateSemanticRelations(entityNode, relationshipObjectEntity, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, conditionName, translatorVariables, false, currentRelationInList->relationGovernorIndex, currentRelationInList2->relationDependentIndex);	//GIA semantic parser output is unsupported for artifical conditions
								GIAtranslatorOperations.addConditionRelationshipArtificialToEntity(entityNode, relationshipObjectEntity, conditionName, sameReferenceSet, translatorVariables);
							}

						}
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
						{
							if(currentRelationInList2->relationGovernor == currentRelationInList->relationDependent)
							{
								#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
								//quantity multipliers not currently supported by GIA2 [NB Stanford CoreNLP use them, only Relex]
								#endif
								GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(translatorVariables->GIAentityNodeArray[currentRelationInList2->relationDependentIndex]);

								int quantityMultiplierInt = GIAentityNodeClass.calculateQuantityMultiplierInt(currentRelationInList2->relationDependent);
								quantitySubstance->quantityNumber = quantitySubstance->quantityNumber* quantityMultiplierInt;
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

					GIAentityNode* entityToConnectMeasurePerEntity;
					bool foundQuantityOwner = false;
					GIArelation* currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationDependent == currentRelationInList->relationGovernor)
							{
								entityToConnectMeasurePerEntity = translatorVariables->GIAentityNodeArray[currentRelationInList2->relationGovernorIndex];	//eg row
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
						GIAentityNodeClass.disconnectNodeFromAllButDefinitions(quantitySubstance);

						GIAentityNode* measureSubstance = quantitySubstance;	//convert quantity substance to measure substance
						measureSubstance->hasQuantity = false;
						measureSubstance->hasMeasure = true;
						measureSubstance->measureType = MEASURE_TYPE_PER;

						GIAentityNode* newQuantityTimesEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial(GIA_ENTITY_TYPE_SUBSTANCE, "times", translatorVariables);	//CHECKTHIS (this is not an artificial relationship entity being created, it is an artificial substance entity); probably need to rename addEntityNodeByNameSimpleWrapperRelationshipArtificial to addEntityNodeByNameSimpleWrapperArtificial
						//newQuantityTimesEntity->isArtificialAuxiliary = false;	//?
						#ifdef GIA_DATABASE
						newQuantityTimesEntity->added = true;
						#endif

						//reconnect refreshed quantity (times) node;
						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
						GIAtranslatorOperations.addPropertyRelationshipToEntity(entityToConnectMeasurePerEntity, newQuantityTimesEntity, sameReferenceSet, translatorVariables);	//GIA semantic parser output is unsupported by extractQuantitiesRelex

						if(newQuantityTimesEntity->hasAssociatedInstanceTemp)
						{//assumed true since its substance was just explicitly created
							newQuantityTimesEntity = (newQuantityTimesEntity->instanceNodeList->back())->entity;
						}
						newQuantityTimesEntity->hasQuantity = true;
						newQuantityTimesEntity->quantityNumber = 1;
						newQuantityTimesEntity->quantityNumberString = "1";

						string conditionName = RELATION_TYPE_MEASURE_PER;
						//GIAtranslatorGeneric.addRelationshipArtificialToEntityAndGenerateSemanticRelations(newQuantityTimesEntity, measureSubstance, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, conditionName, translatorVariables, false, FEATURE_INDEX_OF_QUANTITY_TIMES_UNKNOWN, currentRelationInList->relationGovernorIndex);	//this (conditionRelationshipSubjectEntity<->condition connection) is not currently been generated correctly by GIA1	//GIA semantic parser output is unsupported for artifical conditions
						GIAtranslatorOperations.addConditionRelationshipArtificialToEntity(newQuantityTimesEntity, measureSubstance, conditionName, sameReferenceSet, translatorVariables);
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


void GIAtranslatorApplyAdvancedFeaturesClass::defineTenseOnlyTimeConditions(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAgenericEntityInterpretationParameters param(translatorVariables, true);
	GIAentityCharacteristic entityCharacteristicsTest1("conditionType", CONDITION_NODE_TYPE_TIME_STRING, true);
	GIAentityCharacteristic entityCharacteristicsTest2("grammaticalTenseTemp", GRAMMATICAL_TENSE_PAST_STRING);
	GIAentityCharacteristic entityCharacteristicsTest3("grammaticalTenseTemp", GRAMMATICAL_TENSE_FUTURE_STRING);
	GIAentityCharacteristic entityCharacteristicsTest4("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest2);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest3);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest4);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addTenseOnlyTimeConditionToSubstance;
	GIAtranslatorGeneric.genericEntityInterpretation(&param);
}

#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES

void GIAtranslatorApplyAdvancedFeaturesClass::defineActionConcepts1(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAgenericDepRelInterpretationParameters param(translatorVariables, false);
	param.numberOfRelations = 1;
	param.parseDisabledRelation[REL1] = true;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_MODAL_AUX;
	param.useRelationTest[REL1][REL_ENT2] = true; param.relationTest[REL1][REL_ENT2] = RELATION_ENTITY_CAN;
	GIAentityCharacteristic entityCharacteristicsTest("entityType", GIA_ENTITY_TYPE_ACTION_STRING);
	param.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest);
	GIAentityCharacteristic useRedistributeSpecialCaseAssignment2("isActionConcept", "true");
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT1].push_back(&useRedistributeSpecialCaseAssignment2);
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&param, REL1);

}

void GIAtranslatorApplyAdvancedFeaturesClass::defineActionConcepts2(GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	eg Writing is good exercise. / To eat a pie requires strength.
	*/

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(translatorVariables->GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* entity = translatorVariables->GIAentityNodeArray[i];

			//if(entity->entityType == GIA_ENTITY_TYPE_ACTION)	//do not check for isAction; because action concepts are assigned for nodes which have not been defined as actions by GIA; eg "eating is fun" [CHECKTHIS; this is not the same as the XML implementation]
			//Condition A.
			if((entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && ((entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true) || (entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] == true)))
			{
				//Condition B1. no subject (cannot have subject)
				if(entity->relationshipSubjectEntity->empty())
				{
					//Condition B2 action can have condition/property, but cannot be used as input condition/property in sentence
					if(entity->conditionReverseNodeList->empty() && entity->propertyReverseNodeList->empty())
					{
						bool foundActionNetworkIndex = true;
						//Condition 3. object can have condition/property, but it cannot be used as an input condition/property in sentence
						if(!(entity->relationshipObjectEntity->empty()))
						{
							foundActionNetworkIndex = false;
							GIAentityNode* actionObjectentity = entity->relationshipObjectEntity->back()->entity;
							if(actionObjectentity->conditionReverseNodeList->empty() && actionObjectentity->propertyReverseNodeList->empty())
							{
								foundActionNetworkIndex = true;
							}
						}

						if(foundActionNetworkIndex)
						{
							//GIAentityNodeArray[i] = addInstanceToInstanceDefinition(entity, GIA_ENTITY_TYPE_ACTION);	//is this required?
							translatorVariables->GIAentityNodeArray[i]->entityType = GIA_ENTITY_TYPE_ACTION;
							translatorVariables->GIAentityNodeArray[i]->isActionConcept = true;
						}
					}
				}
			}
		}
	}


}
#endif

#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
void GIAtranslatorApplyAdvancedFeaturesClass::updateConceptDesignationBasedPropertyOwnerContext(GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	updateConceptDesignationBasedPropertyOwnerContext() is designed to distinguish between;
		knights substance (plural): Africa has a castle with knights.
			and;
		knights concept: Knights are tall.
	*/

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(translatorVariables->GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* entity = translatorVariables->GIAentityNodeArray[i];
			if(entity->entityType == GIA_ENTITY_TYPE_CONCEPT)
			{
				if(!(entity->propertyReverseNodeList->empty()))
				{
					GIAentityNode* propertyOwner = GIAtranslatorOperations.getPropertyRelationshipSubjectEntity(entity->propertyReverseNodeList->back());
					if(!(propertyOwner->entityType == GIA_ENTITY_TYPE_CONCEPT))
					{
						entity->entityType = GIA_ENTITY_TYPE_SUBSTANCE;
					}
				}
			}
		}
	}
}
#endif


