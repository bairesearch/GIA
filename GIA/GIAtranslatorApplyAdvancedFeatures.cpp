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
 * Project Version: 2p4d 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorApplyAdvancedFeatures.hpp"
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
#endif



void GIAtranslatorApplyAdvancedFeaturesClass::applyAdvancedFeatures(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType, const int NLPfeatureParser)
{
#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4a; collapseRedundantRelationAndMakeNegativeStanford (eg The chicken has not eaten a pie.: neg(eaten-5, not-4)" << endl;
	#endif
	collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4b; extractMeasures;  eg The boy is 4 feet away. / Take these 4 times a day. / The boy is 4 feet tall. / The birthday boy is 12 years old.	_measure_distance(away, foot) / _measure_per(times, day) / _measure_size(tall, feet) / _measure_time(old, years)" << endl;
	#endif
	this->extractMeasures(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4c/4d; defineToBeAndToDoConditions;" << endl;
	cout << "eg1 The pastry tasted awesome. _to-be(taste[3], awesome[4]) + _subj(taste[3], pastry[2])" << endl;
	cout << "eg2 Jezel likes to draw. _to-do(like[2], draw[4]) + _subj(like[2], Jezel[1])" << endl;
	#endif
	this->defineToBeAndToDoConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4e; extractQualities; eg The broken pencil fell apart. / Giants are red. [Joe is happy.] / Tom runs quickly. _amod(pencil, broken) / _predadj(giants, red) / _advmod(run, quick)" << endl;
	#endif
	this->extractQualities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4f; linkPropertiesParataxis; eg The guy, Akari said, left early in the morning. _parataxis(leave[7], say[5])" << endl;
	#endif
	this->linkPropertiesParataxis(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_STANFORD_CORENLP
		#ifndef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "section B4g; defineClausalComplementProperties; eg He says that you like to swim. ccomp(say, like)" << endl;
		#endif
		this->defineClausalComplementProperties(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
		#endif
		#endif
	}

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4g; defineTenseOnlyTimeConditions" << endl;
	#endif
	this->defineTenseOnlyTimeConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4h; defineActionConcepts1" << endl;
	#endif
	this->defineActionConcepts1(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	#endif
#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout <<"section B4i; extractDates; eg The battle happened on March 11th, 1973. _date_day(December, 3rd) /_date_year(December, 1990)" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
	#endif
	this->extractDates(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4j; extractQuantities; eg He lost three dollars. /   He lost almost three dollars. / He lost three hundred dollars. _quantity(dollar, three) / _quantity_mod(three, almost) / _quantity_mult(hundred, three) " << endl;
	#endif
	this->extractQuantities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, NLPfeatureParser);

	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4k; defineActionConcepts2" << endl;
	#endif
	this->defineActionConcepts2(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4l; updateConceptDesignationBasedPropertyOwnerContext" << endl;
	#endif
	this->updateConceptDesignationBasedPropertyOwnerContext(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
}

void GIAtranslatorApplyAdvancedFeaturesClass::extractDates(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser)
{
	#ifdef GIA_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		/*
		eg The battle happened on March 11th, 1973.  _date_day(December, 3rd) /_date_year(December, 1990)
		*/
		this->extractDatesRelex(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	}
	#endif
	#ifdef GIA_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		this->extractDatesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	}
	#endif
}

#ifdef GIA_STANFORD_CORENLP
void GIAtranslatorApplyAdvancedFeaturesClass::extractDatesStanfordCoreNLP(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* currentEntity = GIAentityNodeArray[i];
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
								//cout << "(getPrimaryNetworkIndexNodeDefiningInstance(timeEntity))->NormalizedNERtemp = " << (getPrimaryNetworkIndexNodeDefiningInstance(timeEntity))->NormalizedNERtemp << endl;
								#endif
								timeEntity->timeConditionNode->conditionName = (getPrimaryNetworkIndexNodeDefiningInstance(timeEntity))->NormalizedNERtemp;
							}
							else
							{
							*/
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "timeEntity->NormalizedNERtemp = " << timeEntity->NormalizedNERtemp << endl;
								#endif
								timeEntity->timeConditionNode->conditionName = timeEntity->NormalizedNERtemp;

								//this case appears to be required for queries... (_%qvar/_%atTime), noting that qVar is not assigned a substance (but remains a networkIndex node)
								/*
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "timeEntity->NormalizedNERtemp = " << timeEntity->NormalizedNERtemp << endl;
								cout << "error: getPrimaryNetworkIndexNodeDefiningInstance(timeEntity) != NULL [1b]" << endl;
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

#ifdef GIA_RELEX
void GIAtranslatorApplyAdvancedFeaturesClass::extractDatesRelex(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* currentEntity = GIAentityNodeArray[i];
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
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
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
						GIAentityNode* currentEntity = GIAentityNodeArray[i];
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
											GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

											//http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
												//The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.	[eg "3rd" -> 3]
											string dayOfMonthString = currentRelationInList->relationDependent;
											int dayOfMonthInt = SHAREDvars.convertStringToInt(dayOfMonthString);
											timeConditionEntity->timeConditionNode->dayOfMonth = dayOfMonthInt;

											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "adding day of month: " << dayOfMonthInt << endl;
											#endif

											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = GIAconditionNodeClass.generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
											//timeConditionEntity->entityName  =
										}
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR)
										{
											GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

											string yearString = currentRelationInList->relationDependent;
											int yearInt = SHAREDvars.convertStringToInt(yearString);
											timeConditionEntity->timeConditionNode->year = yearInt;

											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "adding year: " << yearInt << endl;
											#endif

											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = GIAconditionNodeClass.generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
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
	#ifdef GIA_TIME_NODE_INDEXING
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout <<"add time condition nodes to index [for fast lookup by time]" << endl;
	#endif
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* currentEntity = GIAentityNodeArray[i];
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

#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
void GIAtranslatorApplyAdvancedFeaturesClass::addTimeToSubstance(GIAentityNode* timeConditionEntity)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "addTimeToSubstance timeConditionEntity->entityName = " << timeConditionEntity->entityName << endl;
	#endif
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;

	GIAtimeConditionNode* newTimeCondition = new GIAtimeConditionNode();

	timeConditionEntity->timeConditionNode = newTimeCondition;

}
#endif

void GIAtranslatorApplyAdvancedFeaturesClass::extractQuantities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPfeatureParser)
{
	/*
	eg He lost three dollars. /   He lost almost three dollars. / He lost three hundred dollars.	_quantity(dollar, three) / _quantity_mod(three, almost) / _quantity_mult(hundred, three)
	*/
	#ifdef GIA_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		this->extractQuantitiesRelex(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes);
	}
	#endif
	#ifdef GIA_STANFORD_CORENLP
	//NB GIA could support quantity extraction with Stanford Parser only (ie without Stanford CoreNLP as the NLPfeatureParser) but this has not been implemented
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		this->extractQuantitiesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes);
	}
	#endif
}

#ifdef GIA_STANFORD_CORENLP
void GIAtranslatorApplyAdvancedFeaturesClass::extractQuantitiesStanfordCoreNLP(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;

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

	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "extractQuantitiesStanfordCoreNLP{}" << endl;
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				#endif

				GIAentityNode* quantityEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

				if(quantityEntity->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).
				{
					#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
					GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, currentRelationInList->relationGovernorIndex, currentRelationInList->relationDependentIndex, false);
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
					#ifdef GIA_DEBUG
					//cout << "quantitySubstance->quantityNumber = " << quantitySubstance->quantityNumber << endl;
					#endif
					quantitySubstance->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//added 2a11a [because defineConcepts() does not have access to quantity data]

					GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					}

					//now locate quantity modifiers
					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
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
									GIAentityNode* entityNode = quantitySubstance;
									GIAentityNode* conditionObjectEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
									//GIAentityNode* conditionEntity = quantitySubstance->quantityModifierString;

									string conditionName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

									int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
									currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

									bool entityAlreadyExistant = false;
									GIAentityNode* conditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);

									bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

									#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
									GIAentityNodeArray[featureIndexOfCondition] = GIAtranslatorOperations.addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
									#else
									GIAtranslatorOperations.addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
									#endif

									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, currentRelationInList->relationGovernorIndex, featureIndexOfCondition, sameReferenceSet);
									GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, currentRelationInList2->relationDependentIndex, featureIndexOfCondition, sameReferenceSet);
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
			GIAentityNode* currentEntity = GIAentityNodeArray[i];
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

						GIAentityNode* quantitySubstance = currentEntity;
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

#ifdef GIA_RELEX
void GIAtranslatorApplyAdvancedFeaturesClass::extractQuantitiesRelex(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
				GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, currentRelationInList->relationGovernorIndex, currentRelationInList->relationDependentIndex, false);
				#endif

				#ifdef GIA_TRANSLATOR_DEBUG
				/*
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				*/
				#endif

				GIAentityNode* quantityEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];


				GIAentityNode* quantitySubstance = quantityEntity;
				quantitySubstance->hasQuantity = true;
				quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

				GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

				int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
				quantitySubstance->quantityNumber = quantityNumberInt;
				quantitySubstance->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;	//added 2a11a [because defineConcepts() does not have access to quantity data]

				if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{//update comparison variable (set it to the quantity)
					quantitySubstance->isQuery = true;
					GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
					GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
				}

				//now locate quantity modifiers and multiplicators
				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
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
								GIAentityNode* entityNode = quantitySubstance;
								GIAentityNode* conditionObjectEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
								//GIAentityNode* conditionEntity = quantitySubstance->quantityModifierString;

								string conditionName = "quantityModifier";	//quantitySubstance->quantityModifierString //CHECKTHIS;

								int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
								currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

								bool entityAlreadyExistant = false;
								GIAentityNode* conditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
								#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
								GIAentityNodeArray[featureIndexOfCondition] = GIAtranslatorOperations.addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
								#else
								GIAtranslatorOperations.addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
								#endif

								#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
								GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, currentRelationInList->relationGovernorIndex, featureIndexOfCondition, sameReferenceSet);
								GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, currentRelationInList2->relationDependentIndex, featureIndexOfCondition, sameReferenceSet);
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
								GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList2->relationDependentIndex]);

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
					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
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
						GIAentityNodeClass.disconnectNodeFromAllButDefinitions(quantitySubstance);

						GIAentityNode* measureSubstance = quantitySubstance;	//convert quantity substance to measure substance
						measureSubstance->hasQuantity = false;
						measureSubstance->hasMeasure = true;
						measureSubstance->measureType = MEASURE_TYPE_PER;

						GIAentityNode* newQuantityTimesEntity = new GIAentityNode();
						#ifdef GIA_DATABASE
						newQuantityTimesEntity->added = true;
						#endif

						if(GIAtranslatorOperations.getSaveNetwork())
						{
							long* currentEntityNodeIDinCompleteList = GIAtranslatorOperations.getCurrentEntityNodeIDinCompleteList();
							newQuantityTimesEntity->idActiveList = *currentEntityNodeIDinCompleteList;

							vector<GIAentityNode*>* entityNodesActiveListComplete = GIAtranslatorOperations.getTranslatorEntityNodesCompleteList();
							entityNodesActiveListComplete->push_back(newQuantityTimesEntity);

							(*currentEntityNodeIDinCompleteList)++;
						}
						else
						{
							long* currentEntityNodeIDinCompleteList = GIAtranslatorOperations.getCurrentEntityNodeIDinSentenceCompleteList();
							newQuantityTimesEntity->idActiveList = *currentEntityNodeIDinCompleteList;

							(*currentEntityNodeIDinCompleteList)++;
						}

						newQuantityTimesEntity->entityName = "times";

						//reconnect refreshed quantity (times) node;
						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...
						GIAtranslatorOperations.addOrConnectPropertyToEntity(entityToConnectMeasurePerEntity, newQuantityTimesEntity, sameReferenceSet);

						if(newQuantityTimesEntity->hasAssociatedInstanceTemp)
						{//assumed true since its substance was just explicitly created
							newQuantityTimesEntity = (newQuantityTimesEntity->associatedInstanceNodeList->back())->entity;
						}
						newQuantityTimesEntity->hasQuantity = true;
						newQuantityTimesEntity->quantityNumber = 1;
						newQuantityTimesEntity->quantityNumberString = "1";

						string conditionName = RELATION_TYPE_MEASURE_PER;

						bool entityAlreadyExistant = false;
						int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
						currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

						GIAentityNode* conditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);

						//now add measure_per condition node
						sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAentityNodeArray[featureIndexOfCondition] = GIAtranslatorOperations.addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionEntity, sameReferenceSet);
						#else
						GIAtranslatorOperations.addOrConnectConditionToEntity(newQuantityTimesEntity, measureSubstance, conditionEntity, sameReferenceSet);
						#endif

						#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
						//GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, FEATURE_INDEX_OF_QUANTITY_TIMES_UNKNOWN, featureIndexOfCondition, sameReferenceSet);	//this (conditionSubject<->condition connection) is not currently been generated correctly by GIA1
						GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, currentRelationInList->relationGovernorIndex, featureIndexOfCondition, sameReferenceSet);
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

void GIAtranslatorApplyAdvancedFeaturesClass::extractMeasures(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	/*
	eg The boy is 4 feet away. / Take these 4 times a day. / The boy is 4 feet tall. / The birthday boy is 12 years old.	_measure_distance(away, foot) / _measure_per(times, day) / _measure_size(tall, feet) / _measure_time(old, years)
	*/
#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.entityNodesActiveListNetworkIndexes = entityNodesActiveListNetworkIndexes;
	param.numberOfRelations = 1;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;	//coincidentially this condition holds for both cases
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;	//coincidentially this condition holds for both 3 cases
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE;
	#endif

	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationArrayTest[REL1][REL_ENT3] = true; paramA.relationArrayTest[REL1][REL_ENT3] = relationTypeMeasureDependencyNameArray; paramA.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_MEASURE_DEPENDENCY_NUMBER_OF_TYPES;
	paramA.mustGenerateConditionName = true; paramA.conditionEntityDefaultName = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN;	//NB GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK does not support the full array of Relex measure/dependency types
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&paramA, REL1);

	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypeMeasureNotDependencyNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_MEASURE_NOT_DEPENDENCY_NUMBER_OF_TYPES;
	paramB.mustGenerateConditionName = true; paramB.conditionEntityDefaultName = RELATION_TYPE_MEASURE_UNKNOWN;	//NB GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK does not support the full array of Relex measure/dependency types
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&paramB, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
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

				GIAentityNode* measureEntity = GIAentityNodeArray[relationMeasureIndex];
				GIAentityNode* quantityEntity = GIAentityNodeArray[relationQuantityIndex];

				GIAentityNode* measureSubstanceEntity = measureEntity;
				measureSubstanceEntity->hasMeasure = true;
				measureSubstanceEntity->measureType = measureTypeIndex;

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "measureSubstanceName = " << measureSubstanceEntity->entityName << endl;
				cout << "quantityEntityName = " << quantityEntity->entityName << endl;
				#endif

				string conditionName = relationTypeMeasureNameArray[measureTypeIndex];
				bool entityAlreadyExistant = false;
				int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
				currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

				GIAentityNode* conditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);

				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//CHECK; sameReferenceSet value...

				#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
				if(measureDependencyFound)
				{
					GIAentityNodeArray[featureIndexOfCondition] = GIAtranslatorOperations.addOrConnectConditionToEntity(quantityEntity, measureSubstanceEntity, conditionEntity, sameReferenceSet);
				}
				else
				{
					GIAentityNodeArray[featureIndexOfCondition] = GIAtranslatorOperations.addOrConnectConditionToEntity(measureSubstanceEntity, quantityEntity, conditionEntity, sameReferenceSet);
				}
				#else
				if(measureDependencyFound)
				{
					GIAtranslatorOperations.addOrConnectConditionToEntity(quantityEntity, measureSubstanceEntity, conditionEntity, sameReferenceSet);
				}
				else
				{
					GIAtranslatorOperations.addOrConnectConditionToEntity(measureSubstanceEntity, quantityEntity, conditionEntity, sameReferenceSet);
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


void GIAtranslatorApplyAdvancedFeaturesClass::defineToBeAndToDoConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
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
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;	//old before 2i8e
	paramA.defaultSameSetRelationID = REL1; paramA.defaultSameSetReferenceValue = false;	//determine reference set; eg The pastry tasted awesome./The chicken ate the pie that tasted awesome.
	paramA.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
	paramA.useRelationIndexTest[REL2][REL_ENT2] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; paramA.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	#endif
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&paramA, REL1);
	#else
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_BE;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	paramA.functionEntityRelationID[FUNC_ENT3] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//old before 2i8e
	paramA.defaultSameSetRelationID = REL1; paramA.defaultSameSetReferenceValue = false;	//determine reference set; eg The pastry tasted awesome./The chicken ate the pie that tasted awesome.
	paramA.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
	paramA.useRelationIndexTest[REL2][REL_ENT2] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; paramA.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	#endif
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&paramA, REL1);
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
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;	//old before 2i8e
	paramA.defaultSameSetRelationID = REL1; paramA.defaultSameSetReferenceValue = false;	//determine same reference set; eg Jezel likes to draw./The chicken ate the pie that likes to draw.
	paramA.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
	paramA.useRelationIndexTest[REL2][REL_ENT2] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; paramA.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	#endif
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&paramB, REL1);
	#else
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	paramB.functionEntityRelationID[FUNC_ENT3] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//old before 2i8e
	paramA.defaultSameSetRelationID = REL1; paramA.defaultSameSetReferenceValue = false;	//determine same reference set; eg Jezel likes to draw./The chicken ate the pie that likes to draw.
	paramA.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
	paramA.useRelationIndexTest[REL2][REL_ENT2] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; paramA.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	#endif
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&paramB, REL1);
	#endif

#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
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

						GIAentityNode* entityNode = GIAentityNodeArray[entityIndex];
						GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];

						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES; 	//eg The chicken ate the pie that likes to draw.
						GIAentityNodeArray[substanceIndex] = GIAtranslatorOperations.addOrConnectPropertyToEntity(entityNode, substanceEntity, sameReferenceSet);
					}
					#else
						int featureIndexNewCondition = 0;
						if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
						{
							featureIndexNewCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
							currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
						}
						else if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
						{
							featureIndexNewCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
							currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
						}

						GIAentityNode* entityNode = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
						GIAentityNode* conditionObjectEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
						string conditionEntityNodeName = currentRelationInList->relationType;

						bool entityAlreadyExistant = false;
						GIAentityNode* conditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexNewCondition, &conditionEntityNodeName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);

						bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//eg The chicken ate the pie that likes to draw.

						#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
						GIAentityNodeArray[featureIndexNewCondition] = GIAtranslatorOperations.addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
						#else
						GIAtranslatorOperations.addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionEntity, sameReferenceSet);
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

void GIAtranslatorApplyAdvancedFeaturesClass::extractQualities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType)
{
	/*
	eg The broken pencil fell apart. / Giants are red. [Joe is happy.] / Tom runs quickly.	_amod(pencil, broken) / _predadj(giants, red) / _advmod(run, quick)
	*/
#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeQualityNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_QUALITY_NUMBER_OF_TYPES;
	//param.useRedistributeSpecialCaseQualityAssignment[REL1][REL_ENT2] = true;
	GIAentityCharacteristic useRedistributeSpecialCaseQualityAssignment("isSubstanceQuality", "true");
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT2].push_back(&useRedistributeSpecialCaseQualityAssignment);
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeQualityNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				bool passed2 = true;
				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
				{
				#endif
					passed2 = GIAtranslatorOperations.isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);
				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				}
				#endif

				if(passed2)
				{
				#endif
					int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
					int relationDependentIndex = currentRelationInList->relationDependentIndex;
					GIAentityNode* thingEntity = GIAentityNodeArray[relationGovernorIndex];
					GIAentityNode* substanceEntity = GIAentityNodeArray[relationDependentIndex];

					substanceEntity->entityType = GIA_ENTITY_TYPE_TYPE_QUALITY;	//[eg2 The locked door.. / Jim runs quickly / Mr. Smith is late {_amod/_advmod/_predadj}]
				#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
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


void GIAtranslatorApplyAdvancedFeaturesClass::linkPropertiesParataxis(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PARATAXIS;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PARATAXIS;
	#endif
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
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

				GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];
				GIAentityNode* actionEntity = GIAentityNodeArray[actionIndex];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "linkPropertiesParataxis: found RELATION_TYPE_PARATAXIS" << endl;
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				//cout << "actionName = " << actionEntity->entityName << endl;
				#endif
				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PARATAXIS; 		//eg The guy, Akari said, left early in the morning.	[NB The guy, that Akari said was blue, left early in the morning. / He says that you like to swim.  does not generate parataxis, so these cases needn't be considered here...]
				GIAentityNodeArray[substanceIndex] = GIAtranslatorOperations.addOrConnectPropertyToEntity(actionEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

//currently disabled;
#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
#ifndef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
void GIAtranslatorApplyAdvancedFeaturesClass::defineClausalComplementProperties(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*
	eg He says that you like to swim. ccomp(say, like)
	*/
#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CLAUSAL_COMPLEMENT;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CCCOMP;	//eg The guy, that Akari said was blue, left early in the morning. / He says that you like to swim.
	#endif
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
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

				GIAentityNode* actionEntity = GIAentityNodeArray[actionIndex];
				GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "defineClausalComplementProperties: found RELATION_TYPE_CLAUSAL_COMPLEMENT" << endl;
				//cout << "actionName = " << actionEntity->entityName << endl;
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				#endif

				bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CCCOMP; 	//eg The guy, that Akari said was blue, left early in the morning. / He says that you like to swim.
				GIAentityNodeArray[substanceIndex] = GIAtranslatorOperations.addOrConnectPropertyToEntity(actionEntity, substanceEntity, sameReferenceSet);
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

void GIAtranslatorApplyAdvancedFeaturesClass::defineTenseOnlyTimeConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
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

#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* entity = GIAentityNodeArray[i];
			#ifdef GIA_DEBUG
			//cout << "entity = " << entity->entityName << endl;
			//cout << "entity->grammaticalTenseTemp = " << entity->grammaticalTenseTemp << endl;
			#endif
			if(entity->conditionType != CONDITION_NODE_TYPE_TIME)		//why can't this be used: if(entity->timeConditionNode != NULL)
			{//do not overwrite non-tense only time conditions
				if(entity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE])
				{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
					GIAtranslatorOperations.addTenseOnlyTimeConditionToSubstance(entity, entity->grammaticalTenseTemp, entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE]);
				}
			}
		}
	}
#endif
}

#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES

#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
void GIAtranslatorApplyAdvancedFeaturesClass::defineActionConcepts1(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
//#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.parseDisabledRelation[REL1] = true;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_MODAL_AUX;
	param.useRelationTest[REL1][REL_ENT2] = true; param.relationTest[REL1][REL_ENT2] = RELATION_ENTITY_CAN;
	GIAentityCharacteristic entityCharacteristicsTest("entityType", GIA_ENTITY_TYPE_TYPE_ACTION_STRING);
	param.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest);
	GIAentityCharacteristic useRedistributeSpecialCaseAssignment2("isActionConcept", "true");
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT1].push_back(&useRedistributeSpecialCaseAssignment2);
	GIAtranslatorGeneric.genericDependecyRelationInterpretation(&param, REL1);
//#else
	//not coded as this function was developed after GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
//#endif
}
#endif

void GIAtranslatorApplyAdvancedFeaturesClass::defineActionConcepts2(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*
	eg Writing is good exercise. / To eat a pie requires strength.
	*/

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* entity = GIAentityNodeArray[i];
			#ifdef GIA_DEBUG
			//cout << "entity = " << entity->entityName << endl;
			//cout << "entity->grammaticalTenseTemp = " << entity->grammaticalTenseTemp << endl;
			#endif

			//if(entity->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)	//do not check for isAction; because action concepts are assigned for nodes which have not been defined as actions by GIA; eg "eating is fun" [CHECKTHIS; this is not the same as the XML implementation]
			//Condition A.
			if((entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && ((entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true) || (entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] == true)))
			{
				//Condition B1. no subject (cannot have subject)
				if(entity->actionSubjectEntity->empty())
				{
					//Condition B2 action can have condition/property, but cannot be used as input condition/property in sentence
					if(entity->incomingConditionNodeList->empty() && entity->propertyNodeReverseList->empty())
					{
						bool foundActionNetworkIndex = true;
						//Condition 3. object can have condition/property, but it cannot be used as an input condition/property in sentence
						if(!(entity->actionObjectEntity->empty()))
						{
							foundActionNetworkIndex = false;
							GIAentityNode* actionObjectentity = entity->actionObjectEntity->back()->entity;
							if(actionObjectentity->incomingConditionNodeList->empty() && actionObjectentity->propertyNodeReverseList->empty())
							{
								foundActionNetworkIndex = true;
							}
						}

						if(foundActionNetworkIndex)
						{
							//GIAentityNodeArray[i] = addInstanceToInstanceDefinition(entity, GIA_ENTITY_TYPE_TYPE_ACTION);	//is this required?
							GIAentityNodeArray[i]->entityType = GIA_ENTITY_TYPE_TYPE_ACTION;
							GIAentityNodeArray[i]->isActionConcept = true;
						}
					}
				}
			}
		}
	}


}
#endif

#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
void GIAtranslatorApplyAdvancedFeaturesClass::updateConceptDesignationBasedPropertyOwnerContext(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*
	updateConceptDesignationBasedPropertyOwnerContext() is designed to distinguish between;
		knights substance (plural): Africa has a castle with knights.
			and;
		knights concept: Knights are tall.
	*/

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* entity = GIAentityNodeArray[i];
			if(entity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
			{
				if(!(entity->propertyNodeReverseList->empty()))
				{
					GIAentityNode* propertyOwner = (entity->propertyNodeReverseList->back())->entity;
					if(!(propertyOwner->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
					{
						entity->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;
					}
				}
			}
		}
	}
}
#endif


