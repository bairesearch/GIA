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
 * File Name: GIAtranslatorRules.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3c2a 12-June-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorRules.hpp"

#ifdef GIA_TRANSLATOR_XML_INTERPRETATION

bool GIAtranslatorRulesClass::applyGIATranslatorGenericXMLfunctions(const string translatorFileName, GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA)
{
	//int tempindex = 1;

	bool result = true;
	XMLparserTag* firstTagInRulesTag = XMLparserClass.parseTagDownALevel(GIAfirstTagInXMLfile, RULES_XML_TAG_rules, &result);
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
				firstTagInTranslatorTag = XMLparserClass.parseTagDownALevel(currentTag, RULES_XML_TAG_translator, &result);
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
						/*
						if(translatorFileName == "GIAtranslatorLinkEntities")
						{
							cout << "GIAtranslatorLinkEntities" << endl;
						}
						*/

						XMLparserTag* firstTagInFileTag = XMLparserClass.parseTagDownALevel(currentFileTag, RULES_XML_TAG_file, &result);
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
											
											
											//load options and execute genericDependecyRelationInterpretation/genericEntityInterpretation
											if(!this->applyGIATranslatorGenericXMLparam(currentParamTag, depRelOrEntity, executeOrReassign, translatorVariables, linkPreestablishedReferencesGIA, functionName))
											{
											}
											else
											{
											}

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

bool GIAtranslatorRulesClass::applyGIATranslatorGenericXMLparam(XMLparserTag* currentParamTag, const bool depRelOrEntity, bool executeOrReassign, GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, const string functionName)
{
	bool result = false;
	if(currentParamTag->firstLowerLevelTag != NULL)
	{

		GIAgenericDepRelInterpretationParameters paramDepRel(translatorVariables, executeOrReassign);
		paramDepRel.functionName = functionName;
		paramDepRel.translatorVariables.entityNodesActiveListNetworkIndexes = translatorVariables->entityNodesActiveListNetworkIndexes;
		paramDepRel.translatorVariables.NLPdependencyRelationsType = translatorVariables->NLPdependencyRelationsType;
		paramDepRel.executeOrReassign = executeOrReassign;
		GIAgenericEntityInterpretationParameters paramEntity(translatorVariables, executeOrReassign);
		paramEntity.functionName = functionName;
		paramEntity.executeOrReassign = executeOrReassign;
		bool passedAssert = true;
		bool assertdisableRelationAfterFinish = false;
		bool asssertsetDefiniteAfterFinish = false;
		int asssertPostProcessingREL_ENT = INT_DEFAULT_VALUE;
		int asssertPostProcessingREL = INT_DEFAULT_VALUE;
		string assertPostProcessingValue = "";
		bool assertassignPluralAfterFinish = false;
		#ifdef GIA_PREDETERMINERS
		bool assertassignPredeterminerAfterFinish = false;
		#endif

		XMLparserTag* firstConfigurationTag = currentParamTag->firstLowerLevelTag;
		XMLparserTag* currentConfigurationTag = firstConfigurationTag;
		while(currentConfigurationTag->nextTag != NULL)
		{

			string RELstring = "";
			string REL_ENTstring = "";
			string FUNC_ENTstring = "";
			string swapIndexstring = "";
			int REL = INT_DEFAULT_VALUE;
			int REL_ENT = INT_DEFAULT_VALUE;
			int FUNC_ENT = INT_DEFAULT_VALUE;
			int swapIndex = INT_DEFAULT_VALUE;
			if(XMLparserClass.getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_REL, &RELstring))
			{
				REL = SHAREDvars.convertStringToInt(RELstring) - 1;
			}
			if(XMLparserClass.getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_REL_ENT, &REL_ENTstring))
			{
				REL_ENT = SHAREDvars.convertStringToInt(REL_ENTstring) - 1;
			}
			if(XMLparserClass.getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_FUNC_ENT, &FUNC_ENTstring))
			{
				FUNC_ENT = SHAREDvars.convertStringToInt(FUNC_ENTstring) - 1;
			}
			if(XMLparserClass.getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_swapIndex, &swapIndexstring))
			{
				swapIndex = SHAREDvars.convertStringToInt(swapIndexstring) - 1;
			}

			if(currentConfigurationTag->name == RULES_XML_TAG_option)
			{
				if(depRelOrEntity)
				{
					this->genericDepRelInterpretationApplyOptions(&paramDepRel, currentConfigurationTag, REL, REL_ENT, FUNC_ENT, swapIndex);
				}
				else
				{
					this->genericEntityInterpretationApplyOptions(&paramEntity, currentConfigurationTag);
				}
			}
			else if(currentConfigurationTag->name == RULES_XML_TAG_specialCase)
			{
				string entityCharacteristicsType = "";
				GIAentityCharacteristic* entityCharacteristics = new GIAentityCharacteristic();
				if(this->genericInterpretationGenerateSpecialCase(currentConfigurationTag, entityCharacteristics, &entityCharacteristicsType))
				{
					if(depRelOrEntity)
					{
						this->genericDepRelInterpretationApplySpecialCase(entityCharacteristics, &paramDepRel, REL, REL_ENT, entityCharacteristicsType);
					}
					else
					{
						this->genericEntityInterpretationApplySpecialCase(entityCharacteristics, &paramEntity, entityCharacteristicsType);
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
						if(!(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "whichOrWhatQueryFound")
					{
						bool whichOrWhatQueryFound = false;
						//if(GIAentityNodeArrayFilled[GIA_NLP_START_ENTITY_INDEX]	//"what" is not added to a relation and therefore GIAentityNodeArrayFilled will not be filled
						if((*translatorVariables->featureArrayTemp)[GIA_NLP_START_ENTITY_INDEX] != NULL)
						{
							for(int i=0; i<FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES; i++)
							{
								if((*translatorVariables->featureArrayTemp)[GIA_NLP_START_ENTITY_INDEX]->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i])
								{
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
						if((*translatorVariables->featureArrayTemp)[GIA_NLP_START_ENTITY_INDEX] != NULL)
						{
							if((*translatorVariables->featureArrayTemp)[GIA_NLP_START_ENTITY_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
							{
								firstWordOfSentenceIsWho = true;
							}
						}
						#else
						if((*translatorVariables->GIAentityNodeArrayFilled)[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX])
						{
							if((*translatorVariables->featureArrayTemp)[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
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
						if(!(translatorVariables->currentSentenceInList->isQuestion))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "disableRelationAfterFinish")
					{
						assertdisableRelationAfterFinish = true;
						asssertPostProcessingREL = REL;
					}
					else if(assertAttribute->value == "setDefiniteAfterFinish")
					{
						asssertsetDefiniteAfterFinish = true;
						asssertPostProcessingREL = REL;
						asssertPostProcessingREL_ENT = REL_ENT;
					}
					#ifdef GIA_ADVANCED_REFERENCING
					else if(assertAttribute->value == "linkPreestablishedReferencesGIA")
					{
						if(!linkPreestablishedReferencesGIA)
						{
							passedAssert = false;
						}
					}
					#endif
					else if(assertAttribute->value == "NLPdependencyRelationsTypeStanford")
					{
						if(!(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD))
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
					#ifdef GIA_PREDETERMINERS
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
				cerr << "applyGIATranslatorGenericXMLparam{}: error - illegal param tag:" << currentConfigurationTag->name  << endl;
				cerr << "functionName = " << functionName << endl;
				result = false;
				exit(EXIT_ERROR);
			}

			currentConfigurationTag = currentConfigurationTag->nextTag;
		}

		if(passedAssert)
		{
			if(depRelOrEntity)
			{
				if(GIAtranslatorGeneric.genericDependecyRelationInterpretation(&paramDepRel, REL1))
				{
					result = true;
					if(asssertsetDefiniteAfterFinish)
					{
						(*translatorVariables->featureArrayTemp)[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalIsDefinite = true;
					}
					if(assertdisableRelationAfterFinish)
					{
						paramDepRel.relationFinalResult[asssertPostProcessingREL]->disabled =  true;
					}
					if(assertassignPluralAfterFinish)
					{
						(*translatorVariables->featureArrayTemp)[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;
					}
					#ifdef GIA_PREDETERMINERS
					if(assertassignPredeterminerAfterFinish)
					{
						int arrayIndexOfResultFound = GRAMMATICAL_PREDETERMINER_UNDEFINED;
						if(SHAREDvars.textInTextArray(assertPostProcessingValue, entityPredeterminerSmallNameArray, GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES, &arrayIndexOfResultFound))
						{
							(*translatorVariables->featureArrayTemp)[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalPredeterminer = arrayIndexOfResultFound;
						}
					}
					#endif
				}
				else
				{
				}
			}
			else
			{
				if(GIAtranslatorGeneric.genericEntityInterpretation(&paramEntity))
				{
					result = true;
				}
				else
				{
				}
			}
		}
	}
	else
	{
		cout << "applyGIATranslatorGenericXMLparam{}: error - param has no options/special case tags: currentParamTag->name" << currentParamTag->name << endl;
		result = false;
	}
	return result;
}


bool GIAtranslatorRulesClass::genericInterpretationGenerateSpecialCase(XMLparserTag* xmlTag, GIAentityCharacteristic* entityCharacteristics, string* type)
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

	if(XMLparserClass.getAttribute(xmlTag, RULES_XML_ATTRIBUTE_type, type))
	{
		typeFound = true;
	}
	if(XMLparserClass.getAttribute(xmlTag, RULES_XML_ATTRIBUTE_variable, &variable))
	{
		entityCharacteristics->name = variable;
		variableFound = true;
	}
	if(XMLparserClass.getAttribute(xmlTag, RULES_XML_ATTRIBUTE_value, &value))
	{
		entityCharacteristics->value = value;
		valueFound = true;
	}
	if(XMLparserClass.getAttribute(xmlTag, RULES_XML_ATTRIBUTE_arrayIndex, &arrayIndex))
	{
		entityCharacteristics->arrayIndex = SHAREDvars.convertStringToInt(arrayIndex);
	}
	if(XMLparserClass.getAttribute(xmlTag, RULES_XML_ATTRIBUTE_negative, &negative))
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

bool GIAtranslatorRulesClass::genericDepRelInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericDepRelInterpretationParameters* paramDepRel, int REL, int REL_ENT, const string type)
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

bool GIAtranslatorRulesClass::genericDepRelInterpretationApplyOptions(GIAgenericDepRelInterpretationParameters* paramDepRel, const XMLparserTag* xmlTag, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool result = true;
	const XMLparserAttribute* currentAttribute = xmlTag->firstAttribute;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(!this->genericDepRelInterpretationApplyOption(paramDepRel, currentAttribute, REL, REL_ENT, FUNC_ENT, swapIndex))
		{
			result = false;
		}
		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}

bool GIAtranslatorRulesClass::genericDepRelInterpretationApplyOption(GIAgenericDepRelInterpretationParameters* paramDepRel, const XMLparserAttribute* xmlAttribute, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool foundMatch = false;

	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->numberOfRelations), xmlAttribute, "numberOfRelations", &foundMatch, false);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelation[REL]), xmlAttribute, "parseDisabledRelation", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelationDuringLink[REL]), xmlAttribute, "parseDisabledRelationDuringLink", &foundMatch);

	//predefined values tests
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationTest[REL][REL_ENT]), xmlAttribute, "useRelationTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionstring(&(paramDepRel->relationTest[REL][REL_ENT]), xmlAttribute, "relationTest", &foundMatch);
	/*
	if(this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationTest[REL][REL_ENT]), xmlAttribute, "useRelationTest", &foundMatch))
	{
		cout << "relationTest = " << (paramDepRel->useRelationTest[REL][REL_ENT]) << endl;
	}
	if(this->genericEntityInterpretationApplyOptionstring(&(paramDepRel->relationTest[REL][REL_ENT]), xmlAttribute, "relationTest", &foundMatch))
	{
		cout << "relationTest = " << (paramDepRel->relationTest[REL][REL_ENT]) << endl;
	}
	*/
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationTestIsNegative", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationArrayTest[REL][REL_ENT]), xmlAttribute, "useRelationArrayTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionstringarray(&(paramDepRel->relationArrayTest[REL][REL_ENT]), xmlAttribute, "relationArrayTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->relationArrayTestSize[REL][REL_ENT]), xmlAttribute, "relationArrayTestSize", &foundMatch, false);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationArrayTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationArrayTestIsNegative", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->expectToFindPrepositionTest[REL]), xmlAttribute, "expectToFindPrepositionTest", &foundMatch);

	//entity index match tests
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useRelationIndexTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "relationIndexTestRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "relationIndexTestEntityID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationIndexTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationIndexTestIsNegative", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationIndexTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestEntityID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationIndexTest[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationIndexTest", &foundMatch);

	//for redistribution
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityIndexReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityIndexReassignment", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeRelationEntityIndexReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeRelationEntityIndexReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentRelationEntityID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->redistributeRelationEntityIndexReassignmentUseOriginalValues[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentUseOriginalValues", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityReassignment", &foundMatch);
	this->genericEntityInterpretationApplyOptionstring(&(paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityReassignment", &foundMatch);
	/*
	if(this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityReassignment", &foundMatch))
	{
		cout << "useRedistributeRelationEntityReassignment: " << (paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]) << endl;
	}
	if(this->genericEntityInterpretationApplyOptionstring(&(paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityReassignment", &foundMatch))
	{
		cout << "redistributeRelationEntityReassignment: " << (paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]) << endl;
	}
	*/
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL]), xmlAttribute, "useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSet[REL]), xmlAttribute, "useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSet", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRcmodIndicatesSameReferenceSet[REL]), xmlAttribute, "useRedistributeSpecialCaseRcmodIndicatesSameReferenceSet", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseRelationEntityReassignmentConcatonate", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID", &foundMatch, true);
	#ifdef GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityReassignmentConcatonateType[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityReassignmentConcatonateType", &foundMatch, false);
	#endif
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRelationEntityNameReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseRelationEntityNameReassignment", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityNameReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityNameReassignmentRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityNameReassignmentRelationEntityID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->redistributeSpecialCaseRelationEntityNameReassignmentUseOriginalValues[REL][REL_ENT]), xmlAttribute, "redistributeSpecialCaseRelationEntityNameReassignmentUseOriginalValues", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseDisableInstanceAndNetworkIndex[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseDisableInstanceAndNetworkIndex", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);

	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest[REL]), xmlAttribute, "useRedistributeSpecialCaseRcmodIndicatesSameReferenceSetNotTest", &foundMatch);

	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->defaultSameSetRelationID), xmlAttribute, "defaultSameSetRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->defaultSameSetReferenceValue), xmlAttribute, "defaultSameSetReferenceValue", &foundMatch);

	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->functionEntityRelationID[FUNC_ENT]), xmlAttribute, "functionEntityRelationID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->functionEntityRelationEntityID[FUNC_ENT]), xmlAttribute, "functionEntityRelationEntityID", &foundMatch, true);
	this->genericEntityInterpretationApplyOptionint(&(paramDepRel->functionToExecuteUponFind), xmlAttribute, "functionToExecuteUponFind", &foundMatch, false);

	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->mustGenerateConditionName), xmlAttribute, "mustGenerateConditionName", &foundMatch);
	this->genericEntityInterpretationApplyOptionstring(&(paramDepRel->conditionEntityDefaultName), xmlAttribute, "conditionEntityDefaultName", &foundMatch);

	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableEntity[REL][REL_ENT]), xmlAttribute, "disableEntity", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableEntityUseOriginalValues[REL][REL_ENT]), xmlAttribute, "disableEntityUseOriginalValues", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->enableEntity[REL][REL_ENT]), xmlAttribute, "enableEntity", &foundMatch);	//added GIA 2f12a 13-July-2014
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableRelation[REL]), xmlAttribute, "disableRelation", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableRelationDuringLink[REL]), xmlAttribute, "disableRelationDuringLink", &foundMatch);

	this->genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseNonExistantRelationCheck[REL]), xmlAttribute, "useRedistributeSpecialCaseNonExistantRelationCheck", &foundMatch);	//non existant relations tests - added GIA 2f12a 13-July-2014

	return foundMatch;
}


bool GIAtranslatorRulesClass::genericEntityInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericEntityInterpretationParameters* paramEntity, const string type)
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

bool GIAtranslatorRulesClass::genericEntityInterpretationApplyOptions(GIAgenericEntityInterpretationParameters* paramEntity, const XMLparserTag* xmlTag)
{
	bool result = true;
	const XMLparserAttribute* currentAttribute = xmlTag->firstAttribute;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(!this->genericEntityInterpretationApplyOption(paramEntity, currentAttribute))
		{
			result = false;
		}
		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}
bool GIAtranslatorRulesClass::genericEntityInterpretationApplyOption(GIAgenericEntityInterpretationParameters* paramEntity, const XMLparserAttribute* xmlAttribute)
{
	bool foundMatch = false;

	this->genericEntityInterpretationApplyOptionbool(&(paramEntity->parseDisabledEntity), xmlAttribute, "parseDisabledEntity", &foundMatch);

	this->genericEntityInterpretationApplyOptionbool(&(paramEntity->useEntityTest), xmlAttribute, "useEntityTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionstring(&(paramEntity->entityTest), xmlAttribute, "entityTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramEntity->entityTestIsNegative), xmlAttribute, "entityTestIsNegative", &foundMatch);
	this->genericEntityInterpretationApplyOptionbool(&(paramEntity->useEntityArrayTest), xmlAttribute, "useEntityArrayTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionstringarray(&(paramEntity->entityArrayTest), xmlAttribute, "entityArrayTest", &foundMatch);
	this->genericEntityInterpretationApplyOptionint(&(paramEntity->entityArrayTestSize), xmlAttribute, "entityArrayTestSize", &foundMatch, false);
	this->genericEntityInterpretationApplyOptionbool(&(paramEntity->entityArrayTestIsNegative), xmlAttribute, "entityArrayTestIsNegative", &foundMatch);

	this->genericEntityInterpretationApplyOptionint(&(paramEntity->functionToExecuteUponFind), xmlAttribute, "functionToExecuteUponFind", &foundMatch, false);

	this->genericEntityInterpretationApplyOptionbool(&(paramEntity->disableEntity), xmlAttribute, "disableEntity", &foundMatch);

	return foundMatch;
}


bool GIAtranslatorRulesClass::genericEntityInterpretationApplyOptionbool(bool* paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch)
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

		*foundMatch = true;
		result = true;
	}
	return result;
}
bool GIAtranslatorRulesClass::genericEntityInterpretationApplyOptionint(int* paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch, const bool subtractOne)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		int paramOptionSetValue = SHAREDvars.convertStringToInt(xmlAttribute->value);
		if(subtractOne)
		{
			paramOptionSetValue = paramOptionSetValue - 1;
		}
		*paramVal = paramOptionSetValue;

		*foundMatch = true;
		result = true;
	}
	return result;
}
bool GIAtranslatorRulesClass::genericEntityInterpretationApplyOptionstring(string* paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;
		*paramVal = paramOptionSetValue;

		*foundMatch = true;
		result = true;
	}
	return result;
}
bool GIAtranslatorRulesClass::genericEntityInterpretationApplyOptionstringarray(string** paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;

		*paramVal = GIAentityNodeClass.convertDelimitedStringToArray(paramOptionSetValue, GIA_TRANSLATOR_XML_INTERPRETATION_ARRAY_DELIMITER);

		*foundMatch = true;
		result = true;
	}
	return result;
}


#endif
