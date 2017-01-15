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
 * File Name: GIAtranslatorApplyAdvancedFeatures.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2f19d 23-July-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorApplyAdvancedFeatures.h"
#include "GIAdatabase.h"
#ifdef GIA_USE_CORPUS_DATABASE
#include "GIAcorpusOperations.h"
#endif
#include "GIAtranslatorGeneric.h"



void applyAdvancedFeatures(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType, int NLPfeatureParser)
{
#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4a pass; collapseRedundantRelationAndMakeNegativeStanford (eg The chicken has not eaten a pie.: neg(eaten-5, not-4)" << endl;
	#endif
	collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4b pass; extract measures and link properties (measure-quantity relationships);  eg The boy is 4 feet away. / Take these 4 times a day. / The boy is 4 feet tall. / The birthday boy is 12 years old.	_measure_distance(away, foot) / _measure_per(times, day) / _measure_size(tall, feet) / _measure_time(old, years)" << endl;
	#endif
	extractMeasures(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4c/4d pass; define to_be/to_do conditions;" << endl;
	cout << "eg1 The pastry tasted awesome. _to-be(taste[3], awesome[4]) + _subj(taste[3], pastry[2])" << endl;
	cout << "eg2 Jezel likes to draw. _to-do(like[2], draw[4]) + _subj(like[2], Jezel[1])" << endl;
	#endif
	defineToBeAndToDoConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4e pass; extract qualities; eg The broken pencil fell apart. / Giants are red. [Joe is happy.] / Tom runs quickly. _amod(pencil, broken) / _predadj(giants, red) / _advmod(run, quick)" << endl;
	#endif
	extractQualities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4f pass; link properties (parataxis); eg The guy, Akari said, left early in the morning. _parataxis(leave[7], say[5])" << endl;
	#endif
	linkPropertiesParataxis(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_USE_STANFORD_CORENLP
		#ifndef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "4g pass; define Clausal Complement Properties (ccomp); eg He says that you like to swim. ccomp(say, like)" << endl;
		#endif
		defineClausalComplementProperties(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
		#endif
		#endif
	}

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4h pass; define tense only time conditions" << endl;
	#endif
	defineTenseOnlyTimeConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4i pass; define action concepts1 (ie specific action concepts)" << endl;
	#endif
	defineActionConcepts1(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	#endif
#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout <<"4j pass; extract dates; eg The battle happened on March 11th, 1973. _date_day(December, 3rd) /_date_year(December, 1990)" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
	#endif
	extractDates(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4k pass; extract quantities; eg He lost three dollars. /   He lost almost three dollars. / He lost three hundred dollars. _quantity(dollar, three) / _quantity_mod(three, almost) / _quantity_mult(hundred, three) " << endl;
	#endif
	extractQuantities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPfeatureParser);

	#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4l pass; define action concepts (ie specific action concepts)" << endl;
	#endif
	defineActionConcepts2(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifdef GIA_CREATE_NEW_SUBSTANCE_CONCEPT_FOR_EVERY_REFERENCE_TO_A_SUBSTANCE_CONCEPT
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4m pass; remove substance concept tag for all entities with a property owner that is a substance (a non-substance concept)" << endl;
	#endif
	updateSubstanceConceptDesignationBasedPropertyOwnerContext(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
}

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
				#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
				if(currentEntity->hasAssociatedTime)
				{
					addTimeToSubstance(currentEntity);	//note will overwrite any previous time condition nodes (eg tenseOnlyTimeCondition) - but these should not exist in the case of hasAssociatedTime
				#else
				if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
				{
				#endif
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
			#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
			if(currentEntity->hasAssociatedTime)
			{
				addTimeToSubstance(currentEntity);	//note will overwrite any previous time condition nodes (eg tenseOnlyTimeCondition) - but these should not exist in the case of hasAssociatedTime
			#else
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
			#endif
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

						int timeConditionEntityIndex = INT_DEFAULT_VALUE;
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

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
void addTimeToSubstance(GIAentityNode * timeConditionEntity)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "addTimeToSubstance timeConditionEntity->entityName = " << timeConditionEntity->entityName << endl;
	#endif
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;

	GIAtimeConditionNode * newTimeCondition = new GIAtimeConditionNode();

	timeConditionEntity->timeConditionNode = newTimeCondition;

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
	//NB GIA could support quantity extraction with Stanford Parser only (ie without Stanford CoreNLP as the NLPfeatureParser) but this has not been implemented
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
				cout << "extractQuantitiesStanfordCoreNLP()" << endl;
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				#endif

				GIAentityNode * quantityEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

				if(quantityEntity->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).
				{
					#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, currentRelationInList->relationGovernorIndex, currentRelationInList->relationDependentIndex, false);
					#endif

					GIAentityNode * quantitySubstance = quantityEntity;
					quantitySubstance->hasQuantity = true;
					if((quantitySubstance->NormalizedNERtemp != "") && (quantitySubstance->NormalizedNERtemp != "0.0") && (quantitySubstance->NERTemp != FEATURE_NER_DURATION))		//added (quantitySubstance->NERTemp != FEATURE_NER_DURATION) for Stanford Parser 2014-01-04 //added (quantitySubstance->NormalizedNERtemp != "0.0") for a stanford anomaly 11 May 2012
					{
						quantitySubstance->quantityNumberString = quantitySubstance->NormalizedNERtemp;
					}
					else
					{
						//cout << "here2" << endl;
						quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;
					}

					int quantityNumberInt = calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;
					//cout << "quantitySubstance->quantityNumber = " << quantitySubstance->quantityNumber << endl;
					quantitySubstance->isSubstanceConcept = false;	//added 2a11a [because defineSubstanceConcepts() does not have access to quantity data]

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
								if((currentRelationInList2->relationGovernor == currentRelationInList->relationDependent) || (currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor))	//OLD before GIA 2b7d/2c2b: if(currentRelationInList2->relationGovernor ==currentRelationInList->relationGovernor)
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
									GIAentityNode * conditionObjectEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
									//GIAentityNode * conditionEntity = quantitySubstance->quantityModifierString;

									string conditionName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

									bool entityAlreadyExistant = false;
									GIAentityNode * conditionEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

									bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

									#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
									GIAentityNodeArray[FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
									#else
									addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
									#endif

									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, currentRelationInList->relationGovernorIndex, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, sameReferenceSet);
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, currentRelationInList2->relationDependentIndex, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, sameReferenceSet);
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
				for(int i=0; i<FEATURE_NER_INDICATES_NORMALISED_NER_AVAILABLE_NUMBER_OF_TYPES; i++)
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
				#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
				GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, currentRelationInList->relationGovernorIndex, currentRelationInList->relationDependentIndex, false);
				#endif

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
				quantitySubstance->isSubstanceConcept = false;	//added 2a11a [because defineSubstanceConcepts() does not have access to quantity data]

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
							if((currentRelationInList2->relationGovernor == currentRelationInList->relationDependent) || (currentRelationInList2->relationGovernor == currentRelationInList->relationGovernor))	//OLD before GIA 2c2b: if(currentRelationInList2->relationGovernor ==currentRelationInList->relationGovernor)
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
								GIAentityNode * conditionObjectEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
								//GIAentityNode * conditionEntity = quantitySubstance->quantityModifierString;

								string conditionName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

								bool entityAlreadyExistant = false;
								GIAentityNode * conditionEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
								#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
								GIAentityNodeArray[FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN] = addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
								#else
								addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
								#endif

								#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
								GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, currentRelationInList->relationGovernorIndex, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, sameReferenceSet);
								GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, currentRelationInList2->relationDependentIndex, FEATURE_INDEX_OF_QUANTITY_MODIFIER_UNKNOWN, sameReferenceSet);
								#endif
							}

						}
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
						{
							if(currentRelationInList2->relationGovernor == currentRelationInList->relationDependent)
							{
								#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
								//quantity multipliers not currently supported by GIA2 [NB Stanford CoreNLP use them, only Relex]
								#endif
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

						string conditionName = RELATION_TYPE_MEASURE_PER;

						bool entityAlreadyExistant = false;
						GIAentityNode * conditionEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

						//now add measure_per condition node
						sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAentityNodeArray[FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN] = addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionEntity, sameReferenceSet);
						#else
						addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionEntity, sameReferenceSet);
						#endif

						#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
						//GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, FEATURE_INDEX_OF_QUANTITY_TIMES_UNKNOWN, FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN, sameReferenceSet);	//this (conditionSubject<->condition connection) is not currently been generated correctly by GIA1
						GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, currentRelationInList->relationGovernorIndex, FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN, sameReferenceSet);
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

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION

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
	paramA.mustGenerateConditionName = true; paramA.conditionEntityDefaultName = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN; paramA.conditionEntityDefaultIndex = FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN; //NB GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK does not support the full array of Relex measure/dependency types, and FEATURE_INDEX_OF_MEASURE_PER_UNKNOWN is now used instead of FEATURE_INDEX_OF_MEASURE_UNKNOWN to allow both measure and measure per entities to be used in same sentence
	genericDependecyRelationInterpretation(&paramA, REL1);

	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypeMeasureNotDependencyNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_MEASURE_NOT_DEPENDENCY_NUMBER_OF_TYPES;
	paramB.mustGenerateConditionName = true; paramB.conditionEntityDefaultName = RELATION_TYPE_MEASURE_UNKNOWN; paramB.conditionEntityDefaultIndex = FEATURE_INDEX_OF_MEASURE_UNKNOWN;	//NB GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK does not support the full array of Relex measure/dependency types
	genericDependecyRelationInterpretation(&paramB, REL1);
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

				string conditionName = relationTypeMeasureNameArray[measureTypeIndex];
				bool entityAlreadyExistant = false;
				GIAentityNode * conditionEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, FEATURE_INDEX_OF_MEASURE_UNKNOWN, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

				#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
				if(measureDependencyFound)
				{
					GIAentityNodeArray[FEATURE_INDEX_OF_MEASURE_UNKNOWN] = addOrConnectConditionToEntity(quantityEntity, measureSubstanceEntity, conditionEntity, sameReferenceSet);
				}
				else
				{
					GIAentityNodeArray[FEATURE_INDEX_OF_MEASURE_UNKNOWN] = addOrConnectConditionToEntity(measureSubstanceEntity, quantityEntity, conditionEntity, sameReferenceSet);
				}
				#else
				if(measureDependencyFound)
				{
					addOrConnectConditionToEntity(quantityEntity, measureSubstanceEntity, conditionEntity, sameReferenceSet);
				}
				else
				{
					addOrConnectConditionToEntity(measureSubstanceEntity, quantityEntity, conditionEntity, sameReferenceSet);
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


void defineToBeAndToDoConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts)
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
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_OR_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_BE;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;	//eg The chicken ate the pie that likes to draw.
	#endif
	genericDependecyRelationInterpretation(&paramA, REL1);
	#else
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_BE;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	paramA.functionEntityRelationID[FUNC_ENT3] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;
	paramA.conditionEntityDefaultIndex = FEATURE_INDEX_OF_TOBE_UNKNOWN;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//eg The chicken ate the pie that likes to draw.
	#endif
	genericDependecyRelationInterpretation(&paramA, REL1);
	#endif

	/*
	eg Jezel likes to draw. _to-do(like[2], draw[4]) + _subj(like[2], Jezel[1])
	_to-do(like[2], draw[4])
	_subj(like[2], Jezel[1])	//will have been already interpreted as an action by linkSubjectOrObjectRelationships()
	*/
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_OR_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;	//eg The chicken ate the pie that likes to draw.
	#endif
	genericDependecyRelationInterpretation(&paramB, REL1);
	#else
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	paramB.functionEntityRelationID[FUNC_ENT3] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;
	paramB.conditionEntityDefaultIndex = FEATURE_INDEX_OF_TODO_UNKNOWN;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//eg The chicken ate the pie that likes to draw.
	#endif
	genericDependecyRelationInterpretation(&paramB, REL1);
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
				if((currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE) || (currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO))
				{
					//eg Jezel likes to draw. / //eg  The pastry tasted awesome.
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_OR_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
					if(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]->entityName != RELATION_ENTITY_BE)
					{//this condition is required to support GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_OR_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK		//is this still required??
						int entityIndex = currentRelationInList->relationGovernorIndex;
						int substanceIndex = currentRelationInList->relationDependentIndex;

						GIAentityNode * entityNode = GIAentityNodeArray[entityIndex];
						GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES; 	//eg The chicken ate the pie that likes to draw.
						GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(entityNode, substanceEntity, sameReferenceSet);
					}
					#else
						int featureIndexNewCondition = 0;
						if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
						{
							featureIndexNewCondition = FEATURE_INDEX_OF_TOBE_UNKNOWN;
						}
						else if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
						{
							featureIndexNewCondition = FEATURE_INDEX_OF_TODO_UNKNOWN;
						}

						GIAentityNode * entityNode = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
						GIAentityNode * conditionObjectEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
						string conditionEntityNodeName = currentRelationInList->relationType;

						bool entityAlreadyExistant = false;
						GIAentityNode * conditionEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexNewCondition, &conditionEntityNodeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//eg The chicken ate the pie that likes to draw.

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAentityNodeArray[featureIndexNewCondition] = addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
						#else
						addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
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

void extractQualities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
	/*
	eg The broken pencil fell apart. / Giants are red. [Joe is happy.] / Tom runs quickly.	_amod(pencil, broken) / _predadj(giants, red) / _advmod(run, quick)
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeAdjectiveNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES;
	//param.useRedistributeSpecialCaseQualityAssignment[REL1][REL_ENT2] = true;
	EntityCharacteristic useRedistributeSpecialCaseQualityAssignment("isSubstanceQuality", "true");
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT2].push_back(&useRedistributeSpecialCaseQualityAssignment);
	genericDependecyRelationInterpretation(&param, REL1);
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
	genericDependecyRelationInterpretation(&param, REL1);
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
	genericDependecyRelationInterpretation(&param, REL1);
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
#endif

void defineTenseOnlyTimeConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	EntityCharacteristic entityCharacteristicsTest1("conditionType", CONDITION_NODE_TYPE_TIME_STRING, true);
	EntityCharacteristic entityCharacteristicsTest2("grammaticalTenseTemp", GRAMMATICAL_TENSE_PAST_STRING);
	EntityCharacteristic entityCharacteristicsTest3("grammaticalTenseTemp", GRAMMATICAL_TENSE_FUTURE_STRING);
	EntityCharacteristic entityCharacteristicsTest4("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest2);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest3);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest4);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addTenseOnlyTimeConditionToSubstance;
	genericEntityInterpretation(&param);

#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * entity = GIAentityNodeArray[i];
			//cout << "entity = " << entity->entityName << endl;
			//cout << "entity->grammaticalTenseTemp = " << entity->grammaticalTenseTemp << endl;
			if(entity->conditionType != CONDITION_NODE_TYPE_TIME)		//why can't this be used: if(entity->timeConditionNode != NULL)
			{//do not overwrite non-tense only time conditions
				if(entity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE])
				{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
					addTenseOnlyTimeConditionToSubstance(entity, entity->grammaticalTenseTemp, entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE]);
				}
			}
		}
	}
#endif
}

#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
void defineActionConcepts1(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
//#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.parseDisabledRelation[REL1] = true;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_MODAL_AUX;
	param.useRelationTest[REL1][REL_ENT2] = true; param.relationTest[REL1][REL_ENT2] = RELATION_ENTITY_CAN;
	EntityCharacteristic entityCharacteristicsTest("isAction", "true");
	param.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest);
	EntityCharacteristic useRedistributeSpecialCaseAssignment("isActionConcept", "true");
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT1].push_back(&useRedistributeSpecialCaseAssignment);
	genericDependecyRelationInterpretation(&param, REL1);
//#else
	//not coded as this function was developed after GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
//#endif
}
#endif

void defineActionConcepts2(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	/*
	eg Writing is good exercise. / To eat a pie requires strength.
	*/

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * entity = GIAentityNodeArray[i];
			//cout << "entity = " << entity->entityName << endl;
			//cout << "entity->grammaticalTenseTemp = " << entity->grammaticalTenseTemp << endl;

			//if(entity->isAction)	//do not check for isAction; because action concepts are assigned for nodes which have not been defined as actions by GIA; eg "eating is fun"
			//Condition A.
			if((entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && ((entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true) || (entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] == true)))
			{
				//cout << "A" << endl;
				//Condition B1. no subject (cannot have subject)
				if(entity->actionSubjectEntity->empty())
				{
					//cout << "B1" << endl;
					//Condition B2 action can have condition/property, but cannot be used as input condition/property in sentence
					if(entity->incomingConditionNodeList->empty() && entity->propertyNodeReverseList->empty())
					{
						//cout << "B2" << endl;
						bool foundActionConcept = true;
						//Condition 3. object can have condition/property, but it cannot be used as an input condition/property in sentence
						if(!(entity->actionObjectEntity->empty()))
						{
							foundActionConcept = false;
							GIAentityNode * actionObjectentity = entity->actionObjectEntity->back()->entity;
							if(actionObjectentity->incomingConditionNodeList->empty() && actionObjectentity->propertyNodeReverseList->empty())
							{
								foundActionConcept = true;
							}
						}

						if(foundActionConcept)
						{
							//cout << "B3" << endl;
							//GIAentityNodeArray[i] = addActionToActionDefinition(entity);	//is this required?
							GIAentityNodeArray[i]->isActionConcept = true;
						}
					}
				}
			}
		}
	}


}
#endif

#ifdef GIA_CREATE_NEW_SUBSTANCE_CONCEPT_FOR_EVERY_REFERENCE_TO_A_SUBSTANCE_CONCEPT
void updateSubstanceConceptDesignationBasedPropertyOwnerContext(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	/*
	updateSubstanceConceptDesignationBasedPropertyOwnerContext() is designed to distinguish between;
		knights substance (plural): Africa has a castle with knights.
			and;
		knights substance concept: Knights are tall.
	*/

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * entity = GIAentityNodeArray[i];
			if(entity->isSubstanceConcept)
			{
				if(!(entity->propertyNodeReverseList->empty()))
				{
					GIAentityNode * propertyOwner = (entity->propertyNodeReverseList->back())->entity;
					if(!(propertyOwner->isSubstanceConcept))
					{
						entity->isSubstanceConcept = false;
					}
				}
			}
		}
	}
}
#endif


