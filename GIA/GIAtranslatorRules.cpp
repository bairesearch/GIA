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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2o8a 25-October-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorRules.h"
#include "GIAtranslatorDefs.h"
#include "XMLrulesClass.h"

#ifdef GIA_TRANSLATOR_XML_INTERPRETATION

bool applyGIATranslatorGenericXMLfunctions(string translatorFileName, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA)
{
	//int tempindex = 1;

	bool result = true;
	XMLparserTag* firstTagInRulesTag = parseTagDownALevel(GIAfirstTagInXMLfile, RULES_XML_TAG_rules, &result);
	if(result)
	{
		XMLparserTag* currentTag = firstTagInRulesTag;
		bool foundTranslatorTag = false;
		XMLparserTag* firstTagInTranslatorTag = NULL;
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
			XMLparserTag* currentFileTag = firstTagInTranslatorTag;
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

						XMLparserTag* firstTagInFileTag = parseTagDownALevel(currentFileTag, RULES_XML_TAG_file, &result);
						if(result)
						{
							XMLparserTag* currentFunctionTag = firstTagInFileTag;
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
									XMLparserTag* firstTagInFunction = currentFunctionTag->firstLowerLevelTag;
									XMLparserTag* currentParamTag = firstTagInFunction;
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
													cout << "applyGIATranslatorGenericXMLfunctions{} error: illegal currentParamTag->firstAttribute->value = " << currentParamTag->firstAttribute->value << endl;
												}
											}
											
											#ifdef GIA_DEBUG
											/*
											if(functionName == "linkSubjectObjectRelationships")
											{
												cout << "" << endl;
												GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
												while(currentRelationInList->next != NULL)
												{
													if(!(currentRelationInList->disabled))
													{
														string relationType = currentRelationInList->relationType;
														GIAentityNode* relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
														GIAentityNode* relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

														cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
													}
													currentRelationInList = currentRelationInList->next;
												}
											}
											*/
																						
											/*
											cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "{}:" << endl;
											for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
											{
												if(GIAentityNodeArrayFilled[w])
												{
													if(GIAentityNodeArray[w]->entityName == "deliver")
													{
														cout << "GIAentityNodeArray[w]->stanfordPOStemp" << GIAentityNodeArray[w]->stanfordPOStemp << endl;
													}
												}
											}
											*/
											
											/*
											cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "{}:" << endl;
											for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
											{
												if(GIAentityNodeArrayFilled[w])
												{
													if(!(GIAentityNodeArray[w]->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX))
													{
														if(!(GIAentityNodeArray[w]->disabled))
														{
															cout << GIAentityNodeArray[w]->entityName << " !networkIndex && !disabled" << endl;
														}
													}
												}
											}
											*/

											/*
											cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "{}:" << endl;
											for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
											{
												if(GIAentityNodeArrayFilled[w])
												{
													if(!(GIAentityNodeArray[w]->disabled))
													{
														cout << "GIAentityNodeArray[" << w << "]->grammaticalPredeterminerTemp = " << GIAentityNodeArray[w]->grammaticalPredeterminerTemp << endl;
													}
												}
											}
											*/

											/*
											if(currentSentenceInList->sentenceIndex == 20)
											{
												cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "{}:" << endl;
												for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIter2 = entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityNodesListIter2 != entityNodesActiveListNetworkIndexes->end(); networkIndexEntityNodesListIter2++)
												{
													GIAentityNode* entityNode = networkIndexEntityNodesListIter2->second;
													cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
												}
											}
											*/

											/*
											GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
											while(currentRelationInList->next != NULL)
											{
												if(!(currentRelationInList->disabled))
												{
													string relationType = currentRelationInList->relationType;
													GIAentityNode* relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
													GIAentityNode* relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

													cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
												}
												currentRelationInList = currentRelationInList->next;
											}
											
											if(functionName == "redistributeStanfordRelationsExpletives")
											{
												cout << "" << endl;
												GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
												while(currentRelationInList->next != NULL)
												{
													if(!(currentRelationInList->disabled))
													{
														string relationType = currentRelationInList->relationType;
														GIAentityNode* relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
														GIAentityNode* relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

														cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
													}
													currentRelationInList = currentRelationInList->next;
												}
											}
											*/

											/*
											if(functionName == "redistributeStanfordRelationsInterpretNamedAsDefinition2")
											{
												GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
												while(currentRelationInList->next != NULL)
												{
													if(!(currentRelationInList->disabled))
													{
														string relationType = currentRelationInList->relationType;
														GIAentityNode* relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
														GIAentityNode* relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

														cout << "relationType = " << currentRelationInList->relationType << endl;
														cout << "relationGoverner = " << currentRelationInList->relationGovernor << endl;
														cout << "relationDependent = " << currentRelationInList->relationDependent << endl;
														cout << "relationGovernerIndex = " << currentRelationInList->relationGovernorIndex << endl;
														cout << "relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
														cout << "GIAentityNodeArray relationGoverner->entityName = " << relationGoverner->entityName << endl;
														cout << "GIAentityNodeArray relationDependent->entityName = " << relationDependent->entityName << endl;

															//cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;

															//cout << "relationGoverner->grammaticalWordTypeTemp = " << relationGoverner->grammaticalWordTypeTemp << endl;
															//cout << "relationGoverner->wasReference = " << relationGoverner->wasReference << endl;
															//cout << "relationGoverner->grammaticalPronounTemp = " << relationGoverner->grammaticalPronounTemp << endl;
															//cout << "relationGoverner->grammaticalNumber = " << relationGoverner->grammaticalNumber << endl;
															//cout << "relationGoverner->grammaticalDefiniteTemp = " << relationGoverner->grammaticalDefiniteTemp << endl;
															//cout << "relationGoverner->grammaticalIndefinitePluralTemp = " << relationGoverner->grammaticalIndefinitePluralTemp << endl;
															//cout << "relationGoverner->isConcept = " << relationGoverner->isConcept << endl;

															//cout << "relationDependent->grammaticalWordTypeTemp = " << relationDependent->grammaticalWordTypeTemp << endl;
															//cout << "relationDependent->wasReference = " << relationDependent->wasReference << endl;
															//cout << "relationDependent->grammaticalPronounTemp = " << relationDependent->grammaticalPronounTemp << endl;
															//cout << "relationDependent->grammaticalNumber = " << relationDependent->grammaticalNumber << endl;
															//cout << "relationDependent->grammaticalDefiniteTemp = " << relationDependent->grammaticalDefiniteTemp << endl;
															//cout << "relationDependent->grammaticalIndefinitePluralTemp = " << relationDependent->grammaticalIndefinitePluralTemp << endl;
															//cout << "relationDependent->isConcept = " << relationDependent->isConcept << endl;

															//cout << "relationGoverner->grammaticalNumber = " << relationGoverner->grammaticalNumber << endl;
															//cout << "relationGoverner->grammaticalDefiniteTemp = " << relationGoverner->grammaticalDefiniteTemp << endl;
															//cout << "relationGoverner->grammaticalIndefinitePluralTemp = " << relationGoverner->grammaticalIndefinitePluralTemp << endl;
															//cout << "relationGoverner->grammaticalProperNounTemp = " << relationGoverner->grammaticalProperNounTemp << endl;
															//cout << "relationGoverner->mustSetIsConceptBasedOnApposRelation = " << relationGoverner->mustSetIsConceptBasedOnApposRelation << endl;
															//cout << "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = " << relationGoverner->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp << endl;

															//cout << "relationDependent->grammaticalNumber = " << relationDependent->grammaticalNumber << endl;
															//cout << "relationDependent->grammaticalDefiniteTemp = " << relationDependent->grammaticalDefiniteTemp << endl;
															//cout << "relationDependent->grammaticalIndefinitePluralTemp = " << relationDependent->grammaticalIndefinitePluralTemp << endl;
															//cout << "relationDependent->grammaticalProperNounTemp = " << relationDependent->grammaticalProperNounTemp << endl;
															//cout << "relationDependent->mustSetIsConceptBasedOnApposRelation = " << relationDependent->mustSetIsConceptBasedOnApposRelation << endl;
															//cout << "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = " << relationDependent->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp << endl;

															//cout << "relationGoverner->isSubstance = " << relationGoverner->isSubstance << endl;
															//cout << "relationGoverner->isNameQuery = " << relationGoverner->isNameQuery << endl;
															//cout << "relationGoverner->isConcept = " << relationGoverner->isConcept << endl;
															//cout << "relationGoverner->hasAssociatedTime = " << relationGoverner->hasAssociatedTime << endl;
															//cout << "relationGoverner->grammaticalProperNounTemp = " << relationGoverner->grammaticalProperNounTemp << endl;
															//cout << "relationGoverner->isNameQuery = " << relationGoverner->isNameQuery << endl;

															//cout << "relationDependent->isSubstance = " << relationDependent->isSubstance << endl;
															//cout << "relationDependent->isNameQuery = " << relationDependent->isNameQuery << endl;
															//cout << "relationDependent->isConcept = " << relationDependent->isConcept << endl;
															//cout << "relationDependent->hasAssociatedTime = " << relationDependent->hasAssociatedTime << endl;
															//cout << "relationDependent->grammaticalProperNounTemp = " << relationDependent->grammaticalProperNounTemp << endl;
															//cout << "relationDependent->isNameQuery = " << relationDependent->isNameQuery << endl;



													}
													currentRelationInList = currentRelationInList->next;
												}
											}
											*/
											#endif
											
											//load options and execute genericDependecyRelationInterpretation/genericEntityInterpretation
											if(!applyGIATranslatorGenericXMLparam(currentParamTag, depRelOrEntity, executeOrReassign, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA, functionName))
											{
											}
											else
											{
												#ifdef GIA_DEBUG
												//cout << "\t\t applyGIATranslatorGenericXMLparam passed: " << functionName << endl;
												#endif
											}

											#ifdef GIA_DEBUG
											/*
											if(functionName == "defineConcepts")
											{
												cout << "\t applyGIATranslatorGenericXMLparam: " << functionName << "{}:" << endl;
												for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
												{
													if(GIAentityNodeArrayFilled[w])
													{
														if(GIAentityNodeArray[w]->entityName == "chess")
														{
															cout << "GIAentityNodeArray[w]->grammaticalWordTypeTemp" << GIAentityNodeArray[w]->grammaticalWordTypeTemp << endl;
															//cout << "GIAentityNodeArray[w]->wasReference" << GIAentityNodeArray[w]->wasReference << endl;
															cout << "GIAentityNodeArray[w]->grammaticalPronounTemp" << GIAentityNodeArray[w]->grammaticalPronounTemp << endl;
															cout << "GIAentityNodeArray[w]->grammaticalNumber" << GIAentityNodeArray[w]->grammaticalNumber << endl;
															cout << "GIAentityNodeArray[w]->grammaticalDefiniteTemp" << GIAentityNodeArray[w]->grammaticalDefiniteTemp << endl;
															cout << "GIAentityNodeArray[w]->grammaticalIndefinitePluralTemp" << GIAentityNodeArray[w]->grammaticalIndefinitePluralTemp << endl;
															cout << "GIAentityNodeArray[w]->grammaticalProperNounTemp" << GIAentityNodeArray[w]->grammaticalProperNounTemp << endl;
															cout << "GIAentityNodeArray[w]->isConcept" << GIAentityNodeArray[w]->isConcept << endl;
														}
													}
												}
											}
											*/

											/*
											if(functionName == "redistributeRelexRelationsInterpretOfAsObjectForContinuousVerbs")
											{
												cout << "\t\t" << tempindex << ": dependency relations: " << endl;
												tempindex++;
												GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
												while(currentRelationInList->next != NULL)
												{
													if(!(currentRelationInList->disabled))
													{
														string relationType = currentRelationInList->relationType;
														GIAentityNode* relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
														GIAentityNode* relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

														cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
													}
													currentRelationInList = currentRelationInList->next;
												}
											}
											*/
											#endif
										}
										currentParamTag = currentParamTag->nextTag;
									}
								}

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

bool applyGIATranslatorGenericXMLparam(XMLparserTag* currentParamTag, bool depRelOrEntity, bool executeOrReassign, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA, string functionName)
{
	bool result = false;
	if(currentParamTag->firstLowerLevelTag != NULL)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
		cout << "\tapplyGIATranslatorGenericXMLparam: " << currentParamTag->name << endl;
		#endif

		GIAgenericDepRelInterpretationParameters paramDepRel(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, executeOrReassign);
		paramDepRel.functionName = functionName;
		paramDepRel.entityNodesActiveListNetworkIndexes = entityNodesActiveListNetworkIndexes;
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
		string assertPostProcessingValue = "";
		bool assertassignPluralAfterFinish = false;
		#ifdef GIA_SUPPORT_PREDETERMINERS
		bool assertassignPredeterminerAfterFinish = false;
		#endif

		XMLparserTag* firstConfigurationTag = currentParamTag->firstLowerLevelTag;
		XMLparserTag* currentConfigurationTag = firstConfigurationTag;
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
				REL = convertStringToInt(RELstring) - 1;
				#ifdef GIA_DEBUG
				//cout << "REL = " << REL << endl;
				#endif
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_REL_ENT, &REL_ENTstring))
			{
				REL_ENT = convertStringToInt(REL_ENTstring) - 1;
				#ifdef GIA_DEBUG
				//cout << "REL_ENT = " << REL_ENT << endl;
				#endif
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_FUNC_ENT, &FUNC_ENTstring))
			{
				FUNC_ENT = convertStringToInt(FUNC_ENTstring) - 1;
				#ifdef GIA_DEBUG
				//cout << "FUNC_ENT = " << FUNC_ENT << endl;
				#endif
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_swapIndex, &swapIndexstring))
			{
				swapIndex = convertStringToInt(swapIndexstring) - 1;
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
				GIAentityCharacteristic* entityCharacteristics = new GIAentityCharacteristic();
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
				XMLparserAttribute* assertAttribute = currentConfigurationTag->firstAttribute;
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
								#ifdef GIA_DEBUG
								//cout << "featureQueryWordAcceptedByAlternateMethodNameArray[i] = " << featureQueryWordAcceptedByAlternateMethodNameArray[i] << endl;
								//cout << "featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma = " << featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma << endl;	crashes
								#endif
								if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i])
								{
									#ifdef GIA_DEBUG
									//cout << "whichOrWhatQueryFound" << endl;
									#endif
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
								#ifdef GIA_DEBUG
								//cout << "found who" << endl;
								#endif
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
						#ifdef GIA_DEBUG
						//cout << "disableRelationAfterFinish: asssertPostProcessingREL = " << asssertPostProcessingREL << endl;
						#endif
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
					else if(assertAttribute->value == "assignPluralAfterFinish")
					{
						assertassignPluralAfterFinish = true;
						asssertPostProcessingREL = REL;
						asssertPostProcessingREL_ENT = REL_ENT;
					}
					#ifdef GIA_SUPPORT_PREDETERMINERS
					else if(assertAttribute->value == "assignPredeterminerAfterFinish")
					{
						assertassignPredeterminerAfterFinish = true;
						asssertPostProcessingREL = REL;
						asssertPostProcessingREL_ENT = REL_ENT;
					}
					#endif
				}

				//added assertPostProcessingValue 2i34a:
				assertAttribute = currentConfigurationTag->firstAttribute;
				while(assertAttribute->nextAttribute != NULL)
				{
					if(assertAttribute->name == "assertPostProcessingValue")
					{
						assertPostProcessingValue = assertAttribute->value;
					}
					assertAttribute = assertAttribute->nextAttribute;
				}
			}
			else
			{
				cout << "applyGIATranslatorGenericXMLparam{}: error - illegal param tag:" << currentConfigurationTag->name  << endl;
				result = false;
				//exit(0);
			}

			currentConfigurationTag = currentConfigurationTag->nextTag;
		}

		if(passedAssert)
		{
			if(depRelOrEntity)
			{
				#ifdef GIA_DEBUG
				//cout << "genericDependecyRelationInterpretation{}" << endl;
				#endif
				if(genericDependecyRelationInterpretation(&paramDepRel, REL1))
				{
					result = true;
					#ifdef GIA_DEBUG
					//cout << "\t\t genericDependecyRelationInterpretation passed: " << functionName << endl;
					#endif
					if(asssertsetDefiniteAfterFinish)
					{
						featureArrayTemp[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalIsDefinite = true;
					}
					if(assertdisableRelationAfterFinish)
					{
						paramDepRel.relationFinalResult[asssertPostProcessingREL]->disabled =  true;
					}
					if(assertassignPluralAfterFinish)
					{
						featureArrayTemp[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;
					}
					#ifdef GIA_SUPPORT_PREDETERMINERS
					if(assertassignPredeterminerAfterFinish)
					{
						int arrayIndexOfResultFound = GRAMMATICAL_PREDETERMINER_UNDEFINED;
						if(textInTextArray(assertPostProcessingValue, entityPredeterminerSmallNameArray, GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES, &arrayIndexOfResultFound))
						{
							#ifdef GIA_DEBUG
							//cout << "arrayIndexOfResultFound = " << arrayIndexOfResultFound << endl;
							//cout << featureArrayTemp[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->word << endl;
							#endif
							featureArrayTemp[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalPredeterminer = arrayIndexOfResultFound;
						}
					}
					#endif
				}
				else
				{
					#ifdef GIA_DEBUG
					//cout << "\t\t genericDependecyRelationInterpretation failed: " << functionName << endl;
					#endif
				}
			}
			else
			{
				if(genericEntityInterpretation(&paramEntity))
				{
					result = true;
					#ifdef GIA_DEBUG
					//cout << "\t\t genericEntityInterpretation passed: " << functionName << endl;
					#endif
				}
				else
				{
					#ifdef GIA_DEBUG
					//cout << "\t\t genericEntityInterpretation failed: " << functionName << endl;
					#endif
				}
			}
		}
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
		cout << "\tEND: applyGIATranslatorGenericXMLparam: " << currentParamTag->name << endl;
		#endif
	}
	else
	{
		cout << "applyGIATranslatorGenericXMLparam{}: error - param has no options/special case tags: currentParamTag->name" << currentParamTag->name << endl;
		result = false;
	}
	return result;
}


bool genericInterpretationGenerateSpecialCase(XMLparserTag* xmlTag, GIAentityCharacteristic* entityCharacteristics, string* type)
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
		entityCharacteristics->arrayIndex = convertStringToInt(arrayIndex);
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
		cout << "genericEntityInterpretationApplySpecialCase{} error: !(typeFound && variableFound && valueFound)" << endl;
	}
	return result;
}

bool genericDepRelInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericDepRelInterpretationParameters* paramDepRel, int REL, int REL_ENT, string type)
{
	bool result = true;
	if(type == "specialCaseCharacteristicsTestAndVector")
	{
		paramDepRel->specialCaseCharacteristicsTestAndVector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsTestOrVector")
	{
		paramDepRel->specialCaseCharacteristicsTestOrVector[REL][REL_ENT].push_back(entityCharacteristics);
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
		cout << "genericDepRelInterpretationApplySpecialCase{} error: illegal type" << endl;
		result = false;
	}
	return result;
}


//string* convertDelimitedStringToArray(string str, char delimiter)

bool genericDepRelInterpretationApplyOptions(GIAgenericDepRelInterpretationParameters* paramDepRel, XMLparserTag* xmlTag, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool result = true;
	XMLparserAttribute* currentAttribute = xmlTag->firstAttribute;
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

bool genericDepRelInterpretationApplyOption(GIAgenericDepRelInterpretationParameters* paramDepRel, XMLparserAttribute* xmlAttribute, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool foundMatch = false;

	genericEntityInterpretationApplyOptionint(&(paramDepRel->numberOfRelations), xmlAttribute, "numberOfRelations", &foundMatch, false);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelation[REL]), xmlAttribute, "parseDisabledRelation", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelationDuringLink[REL]), xmlAttribute, "parseDisabledRelationDuringLink", &foundMatch);

	//predefined values tests
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

	//entity index match tests
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useRelationIndexTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "relationIndexTestRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "relationIndexTestEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationIndexTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationIndexTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationIndexTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationIndexTest[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationIndexTest", &foundMatch);

	//for redistribution
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
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSet[REL]), xmlAttribute, "useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSet", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRcmodIndicatesSameReferenceSet[REL]), xmlAttribute, "useRedistributeSpecialCaseRcmodIndicatesSameReferenceSet", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseRelationEntityReassignmentConcatonate", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID", &foundMatch, true);
	#ifdef GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityReassignmentConcatonateType[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityReassignmentConcatonateType", &foundMatch, false);
	#endif
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRelationEntityNameReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseRelationEntityNameReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityNameReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityNameReassignmentRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->redistributeSpecialCaseRelationEntityNameReassignmentUseOriginalValues[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityNameReassignmentUseOriginalValues", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseDisableInstanceAndNetworkIndex[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseDisableInstanceAndNetworkIndex", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID", &foundMatch, true);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID", &foundMatch, true);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest[REL]), xmlAttribute, "useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest", &foundMatch);

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


bool genericEntityInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericEntityInterpretationParameters* paramEntity, string type)
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
		cout << "genericEntityInterpretationApplySpecialCase{} error: illegal type: " << type << endl;
		result = false;
	}
	return result;
}

bool genericEntityInterpretationApplyOptions(GIAgenericEntityInterpretationParameters* paramEntity, XMLparserTag* xmlTag)
{
	bool result = true;
	XMLparserAttribute* currentAttribute = xmlTag->firstAttribute;
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
bool genericEntityInterpretationApplyOption(GIAgenericEntityInterpretationParameters* paramEntity, XMLparserAttribute* xmlAttribute)
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


bool genericEntityInterpretationApplyOptionbool(bool* paramVal, XMLparserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch)
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
			cout << "genericEntityInterpretationApplyOptionbool{} error: illegal paramOptionSetValue for " << iterationVariable << ": " << xmlAttribute->value << endl;
		}
		//bool paramOptionSetValue = convertStringToInt(xmlAttribute->value);		//if GIA Translator XML file booleans are defined as '1'/'0' instead of 'true'/'false'
		*paramVal = paramOptionSetValue;

		#ifdef GIA_DEBUG
		//cout << "genericEntityInterpretationApplyOptionbool{}: " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		#endif
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionint(int* paramVal, XMLparserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch, bool subtractOne)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		int paramOptionSetValue = convertStringToInt(xmlAttribute->value);
		if(subtractOne)
		{
			paramOptionSetValue = paramOptionSetValue - 1;
		}
		*paramVal = paramOptionSetValue;

		#ifdef GIA_DEBUG
		//cout << "genericEntityInterpretationApplyOptionint{}: " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		#endif
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionstring(string* paramVal, XMLparserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;
		*paramVal = paramOptionSetValue;

		#ifdef GIA_DEBUG
		//cout << "testEntityCharacteristicIterationstring{}: " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		#endif
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionstringarray(string** paramVal, XMLparserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;

		*paramVal = convertDelimitedStringToArray(paramOptionSetValue, GIA_TRANSLATOR_XML_INTERPRETATION_ARRAY_DELIMITER);

		#ifdef GIA_DEBUG
		//cout << "testEntityCharacteristicIterationstringarray{}: " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		#endif
		*foundMatch = true;
		result = true;
	}
	return result;
}


#endif
