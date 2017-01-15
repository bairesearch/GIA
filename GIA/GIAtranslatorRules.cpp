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
 * File Name: GIAtranslatorRules.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h2a 18-November-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/



#include "GIAtranslatorRules.h"
#include "GIAtranslatorDefs.h"
#include "XMLrulesClass.h"

#ifdef GIA_TRANSLATOR_XML_INTERPRETATION

bool applyGIATranslatorGenericXMLfunctions(string translatorFileName, Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA)
{
	//int tempindex = 1;

	bool result = true;
	XMLparserTag * firstTagInRulesTag = parseTagDownALevel(GIAfirstTagInXMLfile, RULES_XML_TAG_rules, &result);
	if(result)
	{
		XMLparserTag * currentTag = firstTagInRulesTag;
		bool foundTranslatorTag = false;
		XMLparserTag * firstTagInTranslatorTag = NULL;
		while((currentTag->nextTag != NULL) && !foundTranslatorTag)
		{
			if(currentTag->name == RULES_XML_TAG_translator)
			{
				foundTranslatorTag = true;
				firstTagInTranslatorTag = parseTagDownALevel(currentTag, RULES_XML_TAG_translator, &result);
			}
			currentTag = currentTag->nextTag;
		}
		if(foundTranslatorTag)
		{
			XMLparserTag * currentFileTag = firstTagInTranslatorTag;
			while(currentFileTag->nextTag != NULL)
			{
				if(currentFileTag->firstAttribute->name == RULES_XML_ATTRIBUTE_name)
				{
					if(currentFileTag->firstAttribute->value == translatorFileName)
					{
						#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
						cout << "applyGIATranslatorGenericXMLfunctions translatorFileName: " << translatorFileName << endl;
						#endif
						/*
						if(translatorFileName == "GIAtranslatorLinkEntities")
						{
							cout << "GIAtranslatorLinkEntities" << endl;
						}
						*/

						XMLparserTag * firstTagInFileTag = parseTagDownALevel(currentFileTag, RULES_XML_TAG_file, &result);
						if(result)
						{
							XMLparserTag * currentFunctionTag = firstTagInFileTag;
							while(currentFunctionTag->nextTag != NULL)
							{
								string functionName = "";
								if(currentFunctionTag->firstAttribute->name == RULES_XML_ATTRIBUTE_name)
								{
									functionName = currentFunctionTag->firstAttribute->value;
								}

								#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
								cout << "applyGIATranslatorGenericXMLfunctions functionName: " << functionName << endl;
								#endif

								if(currentFunctionTag->firstLowerLevelTag != NULL)
								{
									XMLparserTag * firstTagInFunction = currentFunctionTag->firstLowerLevelTag;
									XMLparserTag * currentParamTag = firstTagInFunction;
									while(currentParamTag->nextTag != NULL)
									{
										if((currentParamTag->name == RULES_XML_TAG_paramDepRel) || (currentParamTag->name == RULES_XML_TAG_paramEntity))
										{
											bool depRelOrEntity = false;
											if(currentParamTag->name == RULES_XML_TAG_paramDepRel)
											{
												depRelOrEntity = true;
											}
											bool executeOrReassign = false;
											if(currentParamTag->firstAttribute->name == RULES_XML_ATTRIBUTE_mode)
											{
												if(currentParamTag->firstAttribute->value == "execute")
												{
													executeOrReassign = true;
												}
												else if(currentParamTag->firstAttribute->value == "redistribute")
												{
													executeOrReassign = false;
												}
												else
												{
													cout << "applyGIATranslatorGenericXMLfunctions() error: illegal currentParamTag->firstAttribute->value = " << currentParamTag->firstAttribute->value << endl;
												}
											}

											/*
											cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "():" << endl;
											for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
											{
												if(GIAentityNodeArrayFilled[w])
												{
													if(!(GIAentityNodeArray[w]->isConcept))
													{
														if(!(GIAentityNodeArray[w]->disabled))
														{
															cout << GIAentityNodeArray[w]->entityName << " !concept && !disabled" << endl;
														}
													}
												}
											}
											*/

											/*
											cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "():" << endl;
											for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
											{
												if(GIAentityNodeArrayFilled[w])
												{
													if(!(GIAentityNodeArray[w]->disabled))
													{
														cout << GIAentityNodeArray[w]->entityName << " !disabled" << endl;
													}
												}
											}
											*/
											
											/*
											if(currentSentenceInList->sentenceIndex == 20)
											{
												cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "():" << endl;
												unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter2;
												for(conceptEntityNodesListIter2 = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter2 != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter2++)
												{
													GIAentityNode * entityNode = conceptEntityNodesListIter2->second;
													cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
												}
											}
											*/
											
											/*
											Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
											while(currentRelationInList->next != NULL)
											{
												if(!(currentRelationInList->disabled))
												{
													string relationType = currentRelationInList->relationType;
													GIAentityNode * relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
													GIAentityNode * relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

													cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
												}
												currentRelationInList = currentRelationInList->next;
											}
											*/

											/*
											if(functionName == "defineSubstanceConcepts")	//defineSubstancesBasedOnDeterminatesOfDefinitionEntities //linkEntityDefinitionsAppositiveOfNouns
											{
												Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
												while(currentRelationInList->next != NULL)
												{
													if(!(currentRelationInList->disabled))
													{
														string relationType = currentRelationInList->relationType;
														if(relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
														{
															GIAentityNode * relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
															GIAentityNode * relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

															cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;

															cout << "relationGoverner->grammaticalWordTypeTemp = " << relationGoverner->grammaticalWordTypeTemp << endl;
															//cout << "relationGoverner->wasReference = " << relationGoverner->wasReference << endl;
															cout << "relationGoverner->grammaticalPronounTemp = " << relationGoverner->grammaticalPronounTemp << endl;
															cout << "relationGoverner->grammaticalNumber = " << relationGoverner->grammaticalNumber << endl;
															cout << "relationGoverner->grammaticalDefiniteTemp = " << relationGoverner->grammaticalDefiniteTemp << endl;
															cout << "relationGoverner->grammaticalIndefinitePluralTemp = " << relationGoverner->grammaticalIndefinitePluralTemp << endl;
															cout << "relationGoverner->isSubstanceConcept = " << relationGoverner->isSubstanceConcept << endl;

															cout << "relationDependent->grammaticalWordTypeTemp = " << relationDependent->grammaticalWordTypeTemp << endl;
															//cout << "relationDependent->wasReference = " << relationDependent->wasReference << endl;
															cout << "relationDependent->grammaticalPronounTemp = " << relationDependent->grammaticalPronounTemp << endl;
															cout << "relationDependent->grammaticalNumber = " << relationDependent->grammaticalNumber << endl;
															cout << "relationDependent->grammaticalDefiniteTemp = " << relationDependent->grammaticalDefiniteTemp << endl;
															cout << "relationDependent->grammaticalIndefinitePluralTemp = " << relationDependent->grammaticalIndefinitePluralTemp << endl;
															cout << "relationDependent->isSubstanceConcept = " << relationDependent->isSubstanceConcept << endl;
															
															//cout << "relationGoverner->grammaticalNumber = " << relationGoverner->grammaticalNumber << endl;
															//cout << "relationGoverner->grammaticalDefiniteTemp = " << relationGoverner->grammaticalDefiniteTemp << endl;
															//cout << "relationGoverner->grammaticalIndefinitePluralTemp = " << relationGoverner->grammaticalIndefinitePluralTemp << endl;
															//cout << "relationGoverner->grammaticalProperNounTemp = " << relationGoverner->grammaticalProperNounTemp << endl;
															//cout << "relationGoverner->mustSetIsSubstanceConceptBasedOnApposRelation = " << relationGoverner->mustSetIsSubstanceConceptBasedOnApposRelation << endl;
															//cout << "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = " << relationGoverner->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp << endl;

															//cout << "relationDependent->grammaticalNumber = " << relationDependent->grammaticalNumber << endl;
															//cout << "relationDependent->grammaticalDefiniteTemp = " << relationDependent->grammaticalDefiniteTemp << endl;
															//cout << "relationDependent->grammaticalIndefinitePluralTemp = " << relationDependent->grammaticalIndefinitePluralTemp << endl;
															//cout << "relationDependent->grammaticalProperNounTemp = " << relationDependent->grammaticalProperNounTemp << endl;
															//cout << "relationDependent->mustSetIsSubstanceConceptBasedOnApposRelation = " << relationDependent->mustSetIsSubstanceConceptBasedOnApposRelation << endl;
															//cout << "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = " << relationDependent->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp << endl;
															
															//cout << "relationGoverner->isSubstance = " << relationGoverner->isSubstance << endl;
															//cout << "relationGoverner->isNameQuery = " << relationGoverner->isNameQuery << endl;
															//cout << "relationGoverner->isSubstanceConcept = " << relationGoverner->isSubstanceConcept << endl;
															//cout << "relationGoverner->hasAssociatedTime = " << relationGoverner->hasAssociatedTime << endl;
															//cout << "relationGoverner->grammaticalProperNounTemp = " << relationGoverner->grammaticalProperNounTemp << endl;
															//cout << "relationGoverner->isNameQuery = " << relationGoverner->isNameQuery << endl;

															//cout << "relationDependent->isSubstance = " << relationDependent->isSubstance << endl;
															//cout << "relationDependent->isNameQuery = " << relationDependent->isNameQuery << endl;
															//cout << "relationDependent->isSubstanceConcept = " << relationDependent->isSubstanceConcept << endl;
															//cout << "relationDependent->hasAssociatedTime = " << relationDependent->hasAssociatedTime << endl;
															//cout << "relationDependent->grammaticalProperNounTemp = " << relationDependent->grammaticalProperNounTemp << endl;
															//cout << "relationDependent->isNameQuery = " << relationDependent->isNameQuery << endl;	
															
														
														}
													}
													currentRelationInList = currentRelationInList->next;
												}
											}
											*/
										
											//load options and execute genericDependecyRelationInterpretation/genericEntityInterpretation
											if(!applyGIATranslatorGenericXMLparam(currentParamTag, depRelOrEntity, executeOrReassign, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA, functionName))
											{
												result = false;
											}

											/*
											if(functionName == "redistributeRelexRelationsInterpretOfAsObjectForContinuousVerbs")
											{
												cout << "\t\t" << tempindex << ": dependency relations: " << endl;
												tempindex++;
												Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
												while(currentRelationInList->next != NULL)
												{
													if(!(currentRelationInList->disabled))
													{
														string relationType = currentRelationInList->relationType;
														GIAentityNode * relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
														GIAentityNode * relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

														cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
													}
													currentRelationInList = currentRelationInList->next;
												}
											}
											*/
										}
										currentParamTag = currentParamTag->nextTag;
									}
								}

								/*
								if(functionName == "defineSubstanceConcepts")
								{
									exit(0);
								}
								*/

								currentFunctionTag = currentFunctionTag->nextTag;
							}
						}
					}
				}
				currentFileTag = currentFileTag->nextTag;
			}
		}
	}
	return result;
}

bool applyGIATranslatorGenericXMLparam(XMLparserTag * currentParamTag, bool depRelOrEntity, bool executeOrReassign, Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA, string functionName)
{
	bool result = true;
	if(currentParamTag->firstLowerLevelTag != NULL)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
		cout << "\tapplyGIATranslatorGenericXMLparam: " << currentParamTag->name << endl;
		#endif

		GIAgenericDepRelInterpretationParameters paramDepRel(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, executeOrReassign);
		paramDepRel.functionName = functionName;
		paramDepRel.entityNodesActiveListConcepts = entityNodesActiveListConcepts;
		paramDepRel.NLPdependencyRelationsType = NLPdependencyRelationsType;
		paramDepRel.executeOrReassign = executeOrReassign;
		GIAgenericEntityInterpretationParameters paramEntity(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, executeOrReassign);
		paramEntity.functionName = functionName;
		paramEntity.executeOrReassign = executeOrReassign;
		bool passedAssert = true;
		bool assertdisableRelationAfterFinish = false;
		bool asssertsetDefiniteAfterFinish = false;
		int asssertPostProcessingREL_ENT = INT_DEFAULT_VALUE;
		int asssertPostProcessingREL = INT_DEFAULT_VALUE;

		XMLparserTag * firstConfigurationTag = currentParamTag->firstLowerLevelTag;
		XMLparserTag * currentConfigurationTag = firstConfigurationTag;
		while(currentConfigurationTag->nextTag != NULL)
		{
			#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
			cout << "currentConfigurationTag->name: " << currentConfigurationTag->name << endl;
			cout << "currentConfigurationTag->firstAttribute->name: " << currentConfigurationTag->firstAttribute->name << endl;
			#endif

			string RELstring = "";
			string REL_ENTstring = "";
			string FUNC_ENTstring = "";
			string swapIndexstring = "";
			int REL = INT_DEFAULT_VALUE;
			int REL_ENT = INT_DEFAULT_VALUE;
			int FUNC_ENT = INT_DEFAULT_VALUE;
			int swapIndex = INT_DEFAULT_VALUE;
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_REL, &RELstring))
			{
				REL = atoi(RELstring.c_str()) - 1;
				//cout << "REL = " << REL << endl;
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_REL_ENT, &REL_ENTstring))
			{
				REL_ENT = atoi(REL_ENTstring.c_str()) - 1;
				//cout << "REL_ENT = " << REL_ENT << endl;
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_FUNC_ENT, &FUNC_ENTstring))
			{
				FUNC_ENT = atoi(FUNC_ENTstring.c_str()) - 1;
				//cout << "FUNC_ENT = " << FUNC_ENT << endl;
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_swapIndex, &swapIndexstring))
			{
				swapIndex = atoi(swapIndexstring.c_str()) - 1;
			}

			if(currentConfigurationTag->name == RULES_XML_TAG_option)
			{
				if(depRelOrEntity)
				{
					genericDepRelInterpretationApplyOptions(&paramDepRel, currentConfigurationTag, REL, REL_ENT, FUNC_ENT, swapIndex);
				}
				else
				{
					genericEntityInterpretationApplyOptions(&paramEntity, currentConfigurationTag);
				}
			}
			else if(currentConfigurationTag->name == RULES_XML_TAG_specialCase)
			{
				string entityCharacteristicsType = "";
				EntityCharacteristic * entityCharacteristics = new EntityCharacteristic();
				if(genericInterpretationGenerateSpecialCase(currentConfigurationTag, entityCharacteristics, &entityCharacteristicsType))
				{
					if(depRelOrEntity)
					{
						genericDepRelInterpretationApplySpecialCase(entityCharacteristics, &paramDepRel, REL, REL_ENT, entityCharacteristicsType);
					}
					else
					{
						genericEntityInterpretationApplySpecialCase(entityCharacteristics, &paramEntity, entityCharacteristicsType);
					}
				}
			}
			else if(currentConfigurationTag->name == RULES_XML_TAG_assert)
			{//store especially defined conditions for pre/post processing of DepRel/Entity interpretation:
				XMLParserAttribute * assertAttribute = currentConfigurationTag->firstAttribute;
				if(assertAttribute->name == RULES_XML_ATTRIBUTE_variable)
				{
					if(assertAttribute->value == "NLPfeatureParserStanfordCoreNLP")
					{
						if(!(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "whichOrWhatQueryFound")
					{
						bool whichOrWhatQueryFound = false;
						//if(GIAentityNodeArrayFilled[GIA_NLP_START_ENTITY_INDEX]	//"what" is not added to a relation and therefore GIAentityNodeArrayFilled will not be filled
						if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX] != NULL)
						{
							for(int i=0; i<FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES; i++)
							{
								//cout << "featureQueryWordAcceptedByAlternateMethodNameArray[i] = " << featureQueryWordAcceptedByAlternateMethodNameArray[i] << endl;
								//cout << "featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma = " << featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma << endl;	crashes
								if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i])
								{
									//cout << "whichOrWhatQueryFound" << endl;
									whichOrWhatQueryFound = true;
								}
							}
						}
						if(!(whichOrWhatQueryFound))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "firstWordOfSentenceIsWho")
					{
						bool firstWordOfSentenceIsWho = false;
						#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
						//if(GIAentityNodeArrayFilled[GIA_NLP_START_ENTITY_INDEX])	//approximately the reason: "who" is not added to a relation and therefore GIAentityNodeArrayFilled will not be filled
						if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX] != NULL)
						{
							if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
							{
								//cout << "found who" << endl;
								firstWordOfSentenceIsWho = true;
							}
						}
						#else
						if(GIAentityNodeArrayFilled[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX])
						{
							if(featureArrayTemp[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
							{
								firstWordOfSentenceIsWho = true;
							}
						}
						#endif
						if(!firstWordOfSentenceIsWho)
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "isQuestion")
					{
						if(!(currentSentenceInList->isQuestion))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "disableRelationAfterFinish")
					{
						assertdisableRelationAfterFinish = true;
						asssertPostProcessingREL = REL;
						//cout << "disableRelationAfterFinish: asssertPostProcessingREL = " << asssertPostProcessingREL << endl;
					}
					else if(assertAttribute->value == "setDefiniteAfterFinish")
					{
						asssertsetDefiniteAfterFinish = true;
						asssertPostProcessingREL = REL;
						asssertPostProcessingREL_ENT = REL_ENT;
					}
					else if(assertAttribute->value == "linkPreestablishedReferencesGIA")
					{
						if(!linkPreestablishedReferencesGIA)
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "NLPdependencyRelationsTypeStanford")
					{
						if(!(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD))
						{
							passedAssert = false;
						}
					}
				}
			}
			else
			{
				cout << "applyGIATranslatorGenericXMLparam(): error - illegal param tag:" << currentConfigurationTag->name  << endl;
				result = false;
				//exit(0);
			}

			currentConfigurationTag = currentConfigurationTag->nextTag;
		}

		if(passedAssert)
		{
			if(depRelOrEntity)
			{
				//cout << "genericDependecyRelationInterpretation()" << endl;
				if(genericDependecyRelationInterpretation(&paramDepRel, REL1))
				{
					//cout << "\t\t genericDependecyRelationInterpretation passed: " << functionName << endl;	
					if(asssertsetDefiniteAfterFinish)
					{
						featureArrayTemp[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalIsDefinite = true;
					}
					if(assertdisableRelationAfterFinish)
					{
						//cout << "A1" << endl;
						paramDepRel.relationFinalResult[asssertPostProcessingREL]->disabled =  true;
						//cout << "A2" << endl;
					}
				}
				else
				{
					//cout << "\t\t genericDependecyRelationInterpretation failed: " << functionName << endl;
				}
			}
			else
			{
				if(genericEntityInterpretation(&paramEntity))
				{
					//cout << "\t\t genericEntityInterpretation passed: " << functionName << endl;				
				}
				else
				{
					//cout << "\t\t genericEntityInterpretation failed: " << functionName << endl;
				}
			}
		}
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
		cout << "\tEND: applyGIATranslatorGenericXMLparam: " << currentParamTag->name << endl;
		#endif
	}
	else
	{
		cout << "applyGIATranslatorGenericXMLparam(): error - param has no options/special case tags: currentParamTag->name" << currentParamTag->name << endl;
		result = false;
		//exit(0);
	}
	return result;
}


bool genericInterpretationGenerateSpecialCase(XMLparserTag * xmlTag, EntityCharacteristic * entityCharacteristics, string * type)
{
	bool result = false;

	bool typeFound = false;
	bool variableFound = false;
	bool valueFound = false;

	*type = "";
	string variable = "";
	string value = "";
	string arrayIndex = "";
	string negative = "";

	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_type, type))
	{
		typeFound = true;
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_variable, &variable))
	{
		entityCharacteristics->name = variable;
		variableFound = true;
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_value, &value))
	{
		entityCharacteristics->value = value;
		valueFound = true;
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_arrayIndex, &arrayIndex))
	{
		entityCharacteristics->arrayIndex = atoi(arrayIndex.c_str());
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_negative, &negative))
	{
		if(negative == "true")
		{
			entityCharacteristics->isNegative = true;
		}
	}
	if(typeFound && variableFound && valueFound)
	{
		result = true;
	}
	else
	{
		cout << "genericEntityInterpretationApplySpecialCase() error: !(typeFound && variableFound && valueFound)" << endl;
	}
	return result;
}

bool genericDepRelInterpretationApplySpecialCase(EntityCharacteristic * entityCharacteristics, GIAgenericDepRelInterpretationParameters * paramDepRel, int REL, int REL_ENT, string type)
{
	bool result = true;
	if(type == "specialCaseCharacteristicsTestAndVector")
	{
		paramDepRel->specialCaseCharacteristicsTestAndVector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsTestOrVector")
	{
		paramDepRel->specialCaseCharacteristicsTestOrVector[REL][REL_ENT].push_back(entityCharacteristics);
		//cout << "\t*** genericDepRelInterpretationApplySpecialCase: REL = " << REL << ", REL_ENT = " << REL_ENT << "entityCharacteristics->name = " << entityCharacteristics->name << ", entityCharacteristics->value = " << entityCharacteristics->value << endl;
	}
	else if(type == "specialCaseCharacteristicsTestOr2Vector")
	{
		paramDepRel->specialCaseCharacteristicsTestOr2Vector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsTestOr3Vector")
	{
		paramDepRel->specialCaseCharacteristicsTestOr3Vector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsAssignmentVector")
	{
		paramDepRel->specialCaseCharacteristicsAssignmentVector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else
	{
		cout << "genericDepRelInterpretationApplySpecialCase() error: illegal type" << endl;
		result = false;
	}
	return result;
}


//string * convertDelimitedStringToArray(string str, char delimiter)

bool genericDepRelInterpretationApplyOptions(GIAgenericDepRelInterpretationParameters * paramDepRel, XMLparserTag * xmlTag, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool result = true;
	XMLParserAttribute * currentAttribute = xmlTag->firstAttribute;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(!genericDepRelInterpretationApplyOption(paramDepRel, currentAttribute, REL, REL_ENT, FUNC_ENT, swapIndex))
		{
			result = false;
		}
		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}

bool genericDepRelInterpretationApplyOption(GIAgenericDepRelInterpretationParameters * paramDepRel, XMLParserAttribute * xmlAttribute, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool foundMatch = false;

	genericEntityInterpretationApplyOptionint(&(paramDepRel->numberOfRelations), xmlAttribute, "numberOfRelations", &foundMatch, false);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelation[REL]), xmlAttribute, "parseDisabledRelation", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelationDuringLink[REL]), xmlAttribute, "parseDisabledRelationDuringLink", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationTest[REL][REL_ENT]), xmlAttribute, "useRelationTest", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->relationTest[REL][REL_ENT]), xmlAttribute, "relationTest", &foundMatch);
	/*
	if(genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationTest[REL][REL_ENT]), xmlAttribute, "useRelationTest", &foundMatch))
	{
		cout << "relationTest = " << (paramDepRel->useRelationTest[REL][REL_ENT]) << endl;
	}
	if(genericEntityInterpretationApplyOptionstring(&(paramDepRel->relationTest[REL][REL_ENT]), xmlAttribute, "relationTest", &foundMatch))
	{
		cout << "relationTest = " << (paramDepRel->relationTest[REL][REL_ENT]) << endl;
	}
	*/
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationArrayTest[REL][REL_ENT]), xmlAttribute, "useRelationArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionstringarray(&(paramDepRel->relationArrayTest[REL][REL_ENT]), xmlAttribute, "relationArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationArrayTestSize[REL][REL_ENT]), xmlAttribute, "relationArrayTestSize", &foundMatch, false);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationArrayTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationArrayTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->expectToFindPrepositionTest[REL]), xmlAttribute, "expectToFindPrepositionTest", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useRelationIndexTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "relationIndexTestRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "relationIndexTestEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationIndexTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationIndexTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationIndexTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationIndexTest[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationIndexTest", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityIndexReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityIndexReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeRelationEntityIndexReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeRelationEntityIndexReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentRelationEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->redistributeRelationEntityIndexReassignmentUseOriginalValues[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentUseOriginalValues", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityReassignment", &foundMatch);
	/*
	if(genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityReassignment", &foundMatch))
	{
		cout << "useRedistributeRelationEntityReassignment: " << (paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]) << endl;
	}
	if(genericEntityInterpretationApplyOptionstring(&(paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityReassignment", &foundMatch))
	{
		cout << "redistributeRelationEntityReassignment: " << (paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]) << endl;
	}
	*/

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL]), xmlAttribute, "useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseRelationEntityReassignmentConcatonate", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseDisableInstanceAndConcept[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseDisableInstanceAndConcept", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	genericEntityInterpretationApplyOptionint(&(paramDepRel->defaultSameSetRelationID), xmlAttribute, "defaultSameSetRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->defaultSameSetReferenceValue), xmlAttribute, "defaultSameSetReferenceValue", &foundMatch);
	#endif

	genericEntityInterpretationApplyOptionint(&(paramDepRel->functionEntityRelationID[FUNC_ENT]), xmlAttribute, "functionEntityRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->functionEntityRelationEntityID[FUNC_ENT]), xmlAttribute, "functionEntityRelationEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->functionToExecuteUponFind), xmlAttribute, "functionToExecuteUponFind", &foundMatch, false);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->mustGenerateConditionName), xmlAttribute, "mustGenerateConditionName", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->conditionEntityDefaultName), xmlAttribute, "conditionEntityDefaultName", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableEntity[REL][REL_ENT]), xmlAttribute, "disableEntity", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableEntityUseOriginalValues[REL][REL_ENT]), xmlAttribute, "disableEntityUseOriginalValues", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->enableEntity[REL][REL_ENT]), xmlAttribute, "enableEntity", &foundMatch);	//added GIA 2f12a 13-July-2014
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableRelation[REL]), xmlAttribute, "disableRelation", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableRelationDuringLink[REL]), xmlAttribute, "disableRelationDuringLink", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseNonExistantRelationCheck[REL]), xmlAttribute, "useRedistributeSpecialCaseNonExistantRelationCheck", &foundMatch);	//non existant relations tests - added GIA 2f12a 13-July-2014

	return foundMatch;
}


bool genericEntityInterpretationApplySpecialCase(EntityCharacteristic * entityCharacteristics, GIAgenericEntityInterpretationParameters * paramEntity, string type)
{
	bool result = true;
	if(type == "specialCaseCharacteristicsTestAndVector")
	{
		paramEntity->specialCaseCharacteristicsTestAndVector.push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsTestOrVector")
	{
		paramEntity->specialCaseCharacteristicsTestOrVector.push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsAssignmentVector")
	{
		paramEntity->specialCaseCharacteristicsAssignmentVector.push_back(entityCharacteristics);
	}
	else
	{
		cout << "genericEntityInterpretationApplySpecialCase() error: illegal type: " << type << endl;
		result = false;
	}
	return result;
}

bool genericEntityInterpretationApplyOptions(GIAgenericEntityInterpretationParameters * paramEntity, XMLparserTag * xmlTag)
{
	bool result = true;
	XMLParserAttribute * currentAttribute = xmlTag->firstAttribute;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(!genericEntityInterpretationApplyOption(paramEntity, currentAttribute))
		{
			result = false;
		}
		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}
bool genericEntityInterpretationApplyOption(GIAgenericEntityInterpretationParameters * paramEntity, XMLParserAttribute * xmlAttribute)
{
	bool foundMatch = false;

	genericEntityInterpretationApplyOptionbool(&(paramEntity->parseDisabledEntity), xmlAttribute, "parseDisabledEntity", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramEntity->useEntityTest), xmlAttribute, "useEntityTest", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramEntity->entityTest), xmlAttribute, "entityTest", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramEntity->entityTestIsNegative), xmlAttribute, "entityTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramEntity->useEntityArrayTest), xmlAttribute, "useEntityArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionstringarray(&(paramEntity->entityArrayTest), xmlAttribute, "entityArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramEntity->entityArrayTestSize), xmlAttribute, "entityArrayTestSize", &foundMatch, false);
	genericEntityInterpretationApplyOptionbool(&(paramEntity->entityArrayTestIsNegative), xmlAttribute, "entityArrayTestIsNegative", &foundMatch);

	genericEntityInterpretationApplyOptionint(&(paramEntity->functionToExecuteUponFind), xmlAttribute, "functionToExecuteUponFind", &foundMatch, false);

	genericEntityInterpretationApplyOptionbool(&(paramEntity->disableEntity), xmlAttribute, "disableEntity", &foundMatch);

	return foundMatch;
}


bool genericEntityInterpretationApplyOptionbool(bool * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		bool paramOptionSetValue;
		if(xmlAttribute->value == "true")
		{
			paramOptionSetValue = true;
		}
		else if(xmlAttribute->value == "false")
		{
			paramOptionSetValue = false;
		}
		else
		{
			cout << "genericEntityInterpretationApplyOptionbool() error: illegal paramOptionSetValue for " << iterationVariable << ": " << xmlAttribute->value << endl;
		}
		//bool paramOptionSetValue = atoi(xmlAttribute->value.c_str());		//if GIA Translator XML file booleans are defined as '1'/'0' instead of 'true'/'false'
		*paramVal = paramOptionSetValue;

		//cout << "genericEntityInterpretationApplyOptionbool(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionint(int * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch, bool subtractOne)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		int paramOptionSetValue = atoi(xmlAttribute->value.c_str());
		if(subtractOne)
		{
			paramOptionSetValue = paramOptionSetValue - 1;
		}
		*paramVal = paramOptionSetValue;

		//cout << "genericEntityInterpretationApplyOptionint(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionstring(string * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;
		*paramVal = paramOptionSetValue;

		//cout << "testEntityCharacteristicIterationstring(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionstringarray(string ** paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;

		*paramVal = convertDelimitedStringToArray(paramOptionSetValue, GIA_TRANSLATOR_XML_INTERPRETATION_ARRAY_DELIMITER);

		//cout << "testEntityCharacteristicIterationstringarray(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}


#endif
