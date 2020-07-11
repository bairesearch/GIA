/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorRules.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l7c 06-July-2020
 * Requirements: requires plain text file
 * Description: Part-of-speech Relation Translator Rules
 * /
 *******************************************************************************/



#include "GIAposRelTranslatorRules.hpp"
#include "XMLrulesClass.hpp"


#ifdef GIA_POS_REL_TRANSLATOR_RULES

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
static int newNeuronIndex;
#endif

vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypesGlobal;
vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayersGlobal;

vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesClass::getGIAposRelTranslatorRulesGroupTypesGlobal()
{
	return GIAposRelTranslatorRulesGroupTypesGlobal;
}
vector<XMLparserTag*>* GIAposRelTranslatorRulesClass::getGIAposRelTranslatorRulesTokenLayersGlobal()
{
	return GIAposRelTranslatorRulesTokenLayersGlobal;
}

bool GIAposRelTranslatorRulesClass::extractGIAposRelTranslatorRules(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers)
{
	bool result = true;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR		
	//FUTURE GIA - upgrade GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR to read/write GIAposRelTranslatorRulesGenerated.xml
	GIAposRelTranslatorRulesGroupType* groupType = new GIAposRelTranslatorRulesGroupType();
	groupType->groupTypeName = GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_NAME;
	groupType->referenceSetType = GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_REFERENCE_SET_TYPE;
	GIAposRelTranslatorRulesGroupTypes->push_back(groupType);
	initialiseNewGroupIndex(GIAposRelTranslatorRulesGroupTypes);
	GIAposRelTranslatorRulesGroupTypesGlobal = GIAposRelTranslatorRulesGroupTypes;
	#else
	//parse extractGIAposRelTranslatorRules
	if(!extractGIAposRelTranslatorRulesGroups(GIAposRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	if(!extractGIAposRelTranslatorRulesTokens(GIAposRelTranslatorRulesTokenLayers))
	{
		result = false;
	}
	#endif
	
	return result;
}

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
void GIAposRelTranslatorRulesClass::initialiseNewGroupIndex(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	GIAposRelTranslatorRulesGroupType* groupType = getSequenceGrammarGroupTypeDefault(GIAposRelTranslatorRulesGroupTypes);
	newNeuronIndex = groupType->groups.size();	//0
}
int* GIAposRelTranslatorRulesClass::getNewGroupIndex()
{
	return &newNeuronIndex;
}
GIAposRelTranslatorRulesGroupType* GIAposRelTranslatorRulesClass::getSequenceGrammarGroupTypeDefault(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	GIAposRelTranslatorRulesGroupType* groupType = NULL;
	if(!findGroupType(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_NAME, &groupType))
	{
		cerr << "GIAposRelTranslatorRulesClass::getSequenceGrammarGroupTypeDefault error: !findGroupType" << endl;
		exit(EXIT_ERROR);
	}
	return groupType;
}
int GIAposRelTranslatorRulesClass::assignGroupIndex(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{	
	int* newNeuronIndex = getNewGroupIndex();
	group->groupIndex = *newNeuronIndex;
	*newNeuronIndex = *newNeuronIndex + 1;
	return *newNeuronIndex;
}
#endif


bool GIAposRelTranslatorRulesClass::extractGIAposRelTranslatorRulesGroups(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
			delete group->neuronReference;
		}
	}
	#endif

	XMLparserTag* firstTagInRulesTag = XMLparserClass.parseTagDownALevel(GIAposRelTranslatorFirstTagInXMLfile, RULES_XML_TAG_rules, &result);
	if(result)
	{
		XMLparserTag* currentTag = firstTagInRulesTag;
		bool foundTxtRelTranslatorTag = false;
		XMLparserTag* firstTagInTxtRelTranslatorTag = NULL;
		while((currentTag->nextTag != NULL) && !foundTxtRelTranslatorTag)
		{
			if(currentTag->name == GIA_POS_REL_TRANSLATOR_RULES_TAG_txtRelTranslator)
			{
				foundTxtRelTranslatorTag = true;
				firstTagInTxtRelTranslatorTag = XMLparserClass.parseTagDownALevel(currentTag, GIA_POS_REL_TRANSLATOR_RULES_TAG_txtRelTranslator, &result);
			}
			currentTag = currentTag->nextTag;
		}
				
		if(foundTxtRelTranslatorTag)
		{
			XMLparserTag* currentTagInTxtRelTranslator = firstTagInTxtRelTranslatorTag;
			
			bool foundTxtRelTranslatorGroupsTag = false;
			XMLparserTag* firstTagInTxtRelTranslatorGroupsTag = NULL;
			while((currentTagInTxtRelTranslator->nextTag != NULL) && !foundTxtRelTranslatorGroupsTag)
			{
				if(currentTagInTxtRelTranslator->name == GIA_POS_REL_TRANSLATOR_RULES_TAG_groups)
				{
					foundTxtRelTranslatorGroupsTag = true;
					firstTagInTxtRelTranslatorGroupsTag = XMLparserClass.parseTagDownALevel(currentTagInTxtRelTranslator, GIA_POS_REL_TRANSLATOR_RULES_TAG_groups, &result);
				}
				currentTagInTxtRelTranslator = currentTagInTxtRelTranslator->nextTag;
			}
						
			if(foundTxtRelTranslatorGroupsTag)
			{
				XMLparserTag* currentTagInGroupsTag = firstTagInTxtRelTranslatorGroupsTag;
				bool foundTxtRelTranslatorGroupTypeTag = false;	//NOTUSED
				while(currentTagInGroupsTag->nextTag != NULL)
				{
					if(currentTagInGroupsTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TAG_groupType)
					{
						foundTxtRelTranslatorGroupTypeTag = true;
												
						string groupTypeName = "";
						string referenceSetType = GIAposRelTranslatorRulesGroupsComponentReferenceSetTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN];
						if(!XMLparserClass.getAttribute(currentTagInGroupsTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_ATTRIBUTE_groupTypeName, &groupTypeName))
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInGroupsTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_ATTRIBUTE_groupTypeName, &groupTypeName)" << endl;
							exit(EXIT_ERROR);
						}
						if(!XMLparserClass.getAttribute(currentTagInGroupsTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_ATTRIBUTE_referenceSetType, &referenceSetType))
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInGroupsTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_ATTRIBUTE_referenceSetType, &referenceSetType)" << endl;
							exit(EXIT_ERROR);
						}
					
						int referenceSetTypeInt = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN;
						if(!SHAREDvars.textInTextArray(referenceSetType, GIAposRelTranslatorRulesGroupsComponentReferenceSetTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES, &referenceSetTypeInt))
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: componentType unknown; referenceSetType = " << referenceSetType << endl;
							exit(EXIT_ERROR);
						}
								
						GIAposRelTranslatorRulesGroupType* groupType = new GIAposRelTranslatorRulesGroupType();
						groupType->groupTypeName = groupTypeName;
						groupType->referenceSetType = referenceSetTypeInt;
						GIAposRelTranslatorRulesGroupTypes->push_back(groupType);
						//cout << "groupType->groupTypeName = " << groupType->groupTypeName << endl;
						
						XMLparserTag* firstTagInTxtRelTranslatorGroupTypeTag = XMLparserClass.parseTagDownALevel(currentTagInGroupsTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TAG_groupType, &result);

						XMLparserTag* currentTagInTxtRelTranslatorGroupTypeTag = firstTagInTxtRelTranslatorGroupTypeTag;
						bool foundTxtRelTranslatorGroupTag = false;	//NOTUSED
						while(currentTagInTxtRelTranslatorGroupTypeTag->nextTag != NULL)
						{
							if(currentTagInTxtRelTranslatorGroupTypeTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_TAG_group)
							{																
								vector<string> semanticRelationFunctionName(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
								#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
								string semanticRelationFunctionConditionNewName = "";
								#endif
								string groupName = "";
								#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
								string previousWordPOStype = "";
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_EXISTS_PRECEEDING_WORD_POS_TYPE
								string existsPreceedingWordPOStype = "";
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
								string groupWeight = "";
								int groupWeightInt = 0;
								#endif
								if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionName, &(semanticRelationFunctionName[0])))
								{
									//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionName, &(semanticRelationFunctionName[2]))" << endl;
								}
								if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionName2, &(semanticRelationFunctionName[1])))
								{
									//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionName2, &(semanticRelationFunctionName2[1]))" << endl;
								}
								#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
								if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionConditionNewName, &semanticRelationFunctionConditionNewName))
								{
									//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionConditionNewName, &(semanticRelationFunctionConditionNewName[2]))" << endl;
								}
								#endif
								if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_groupName, &groupName))
								{
									//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_groupName, &groupName)" << endl;
								}
								#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
								if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_previousWordPOStype, &previousWordPOStype))
								{
									//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_previousWordPOStype, &previousWordPOStype)" << endl;
								}
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_EXISTS_PRECEEDING_WORD_POS_TYPE
								if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_existsPreceedingWordPOStype, &existsPreceedingWordPOStype))
								{
									//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_existsPreceedingWordPOStype, &existsPreceedingWordPOStype)" << endl;
								}
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
								if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_groupWeight, &groupWeight))
								{
									//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_groupWeight, &groupWeight)" << endl;
								}
								else
								{
									groupWeightInt = SHAREDvars.convertStringToInt(groupWeight);
								}
								#endif
								
								bool passGroupNameRequirements = true;
								bool GIAwithNoExternalProcessingFunctionFound = false;
								bool GIAwithExternalProcessingFunctionFound = false;
								if(SHAREDvars.textInTextArray(groupName, GIAtxtRelWithNoExternalProcessingGroupNameArray, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_WITH_NO_EXTERNAL_PROCESSING_NUMBER_OF_TYPES))
								{
									GIAwithNoExternalProcessingFunctionFound = true;
								}	
								if(SHAREDvars.textInTextArray(groupName, GIAtxtRelWithExternalProcessingGroupNameArray, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_WITH_EXTERNAL_PROCESSING_NUMBER_OF_TYPES))
								{
									GIAwithExternalProcessingFunctionFound = true;
								}
								#ifdef GIA_EXTERNAL_PROCESSING
								if(GIAwithNoExternalProcessingFunctionFound)
								{
									passGroupNameRequirements = false;
								}
								#else
								if(GIAwithExternalProcessingFunctionFound)
								{
									passGroupNameRequirements = false;
								}
								#endif
								if(passGroupNameRequirements)
								{
									GIAposRelTranslatorRulesGroupNeuralNetwork* group = new GIAposRelTranslatorRulesGroupNeuralNetwork();	
									group->semanticRelationFunctionName = semanticRelationFunctionName;
									#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
									group->semanticRelationFunctionConditionNewName = semanticRelationFunctionConditionNewName;
									//cout << "semanticRelationFunctionConditionNewName = " << semanticRelationFunctionConditionNewName << endl;
									#endif
									group->groupName = groupName;
									#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
									group->previousWordPOStype = previousWordPOStype;
									#endif
									#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_EXISTS_PRECEEDING_WORD_POS_TYPE
									group->existsPreceedingWordPOStype = existsPreceedingWordPOStype;
									#endif
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
									group->groupWeight = groupWeightInt;
									#endif
									
									#ifdef GIA_POS_REL_TRANSLATOR_RULES_DEFINE_GROUP_TYPE_BACKUP_AT_START
									group->groupTypeName = groupType->groupTypeName;
									group->groupTypeReferenceSetType = groupType->referenceSetType;
									#endif
									
									/*
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_NEW
									group->currentParseTreeGroupTemp = new GIAposRelTranslatorRulesGroupNeuralNetwork(*group);
									#endif
									*/
									
									groupType->groups.push_back(group);

									XMLparserTag* firstTagInTxtRelTranslatorGroupTag = XMLparserClass.parseTagDownALevel(currentTagInTxtRelTranslatorGroupTypeTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_TAG_group, &result);

									if(!parseComponents(firstTagInTxtRelTranslatorGroupTag, group, &(group->components), false, NULL))
									{
										cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !parseComponents(firstTagInTxtRelTranslatorGroupTag, &(group->components), false))" << endl;
										exit(EXIT_ERROR);			
									}
								}
							}
							else
							{
								cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !(currentTagInGroups->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPSTYPE_TAG_group)" << endl;
								exit(EXIT_ERROR);					
							}

							currentTagInTxtRelTranslatorGroupTypeTag = currentTagInTxtRelTranslatorGroupTypeTag->nextTag;
						}
					}
					else
					{
						cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !(currentTagInGroups->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPS_TAG_groupType)" << endl;
						exit(EXIT_ERROR);					
					}
					
					currentTagInGroupsTag = currentTagInGroupsTag->nextTag;
				}
			}
		}
	}

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
	if(!removeLastOptionalComponents(GIAposRelTranslatorRulesGroupTypes))
	{
		result = false;
	}	
	#endif
		
	if(!connectGroupsReferences(GIAposRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	
	GIAposRelTranslatorRulesGroupTypesGlobal = GIAposRelTranslatorRulesGroupTypes;
	
	return result;
}


bool GIAposRelTranslatorRulesClass::parseComponents(XMLparserTag* firstTxtRelTranslatorRulesFirstComponentTag, GIAposRelTranslatorRulesGroupNeuralNetwork* groupOwner, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsList, const bool parseSubcomponent, GIAposRelTranslatorRulesComponentNeuralNetwork* subComponentOwner)
{
	bool result = true;
	
	XMLparserTag* currentTagInTxtRelTranslatorGroupTag = firstTxtRelTranslatorRulesFirstComponentTag;
	bool foundTxtRelTranslatorGroupTag = false;	//NOTUSED
	int componentIndex = GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST;
	while(currentTagInTxtRelTranslatorGroupTag->nextTag != NULL)
	{
		if(currentTagInTxtRelTranslatorGroupTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_example)
		{
			//ignore
		}
		else if(currentTagInTxtRelTranslatorGroupTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component)
		{			
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
			string referenceSetTypeHybrid = GIAposRelTranslatorRulesGroupsComponentReferenceSetTypeHybridTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_UNKNOWN];
			#endif
			string componentType = GIAposRelTranslatorRulesGroupsComponentTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_UNKNOWN];
			int componentTypeInt = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_UNKNOWN;
			string groupTypeRefName = "";
			string groupRefName = "";
			string stringType = GIAposRelTranslatorRulesGroupsComponentStringTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_UNKNOWN];
			int stringTypeInt = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_UNKNOWN;
			string word = "";
			string wordPOStype = "";
			string tokenLayer = "";
			string tokenClass = "";
			string tokenType = "";
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
			string missing = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
			string addToExplicitWordTempPOS = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#endif
			vector<string> semanticRelationIndexType(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
			vector<int> semanticRelationIndexTypeInt(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
			for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
			{
				semanticRelationIndexType[i] = GIAposRelTranslatorRulesGroupsComponentSemanticRelationIndexTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN];
			}
			string semanticRelationReturnEntity = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
			string semanticRelationReturnConnectionDynamic = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#endif
			string semanticRelationReturnFunctionName = "";
			string semanticRelationReturnFunctionNameIndexType = GIAposRelTranslatorRulesGroupsComponentSemanticRelationIndexTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN];
			int semanticRelationReturnFunctionNameIndexTypeInt = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
			string semanticRelationRecord = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			string semanticRelationConnect = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			string optional = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
			string queryComparisonVariable = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			string isAuxiliaryQuery = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			string isWhichOrEquivalentWhatQuery = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
			string isNameQuery = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			string isName = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
			string isNumberOf = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
			string wordVerbVariantType = GIApreprocessorMultiwordReductionVerbDatabaseTagBaseTenseFormArray[GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN];
			int wordVerbVariantTypeInt = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
			string wordNounVariantType = GIApreprocessorMultiwordReductionNounDatabaseTagBaseTenseFormArray[GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN];
			int wordNounVariantTypeInt = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
			string isExpletive = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			#endif

			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
			int referenceSetTypeHybridInt = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_UNKNOWN;
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_referenceSetTypeHybrid, &referenceSetTypeHybrid))
			{
				cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_referenceSetTypeHybrid, &referenceSetTypeHybrid)" << endl;
				exit(EXIT_ERROR);
			}
			if(!SHAREDvars.textInTextArray(referenceSetTypeHybrid, GIAposRelTranslatorRulesGroupsComponentReferenceSetTypeHybridTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_NUMBER_OF_TYPES, &referenceSetTypeHybridInt))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: referenceSetTypeHybrid unknown; componentType = " << referenceSetTypeHybrid << endl;
			}			
			#endif
			
			if(XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType, &componentType))
			{
				if(!SHAREDvars.textInTextArray(componentType, GIAposRelTranslatorRulesGroupsComponentTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_NUMBER_OF_TYPES, &componentTypeInt))
				{
					cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: componentType unknown; componentType = " << componentType << endl;
					exit(EXIT_ERROR);
				}
				if(componentTypeInt == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
				{
					if(XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef, &groupTypeRefName))
					{

					}
					else 
					{
						cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: (componentTypeInt == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && groupTypeName not specified" << endl;
						exit(EXIT_ERROR);
					}
					if(XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef, &groupRefName))
					{

					}
				}
				else if(componentTypeInt == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType, &stringType))
					{
						cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType, &groupRefName)" << endl;
						exit(EXIT_ERROR);
					}
					if(!SHAREDvars.textInTextArray(stringType, GIAposRelTranslatorRulesGroupsComponentStringTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_NUMBER_OF_TYPES, &stringTypeInt))
					{
						cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: stringType unknown; stringType = " << stringType << endl;
						cout << "currentTagInTxtRelTranslatorGroupTag->lineCount = " << currentTagInTxtRelTranslatorGroupTag->lineCount << endl;
						exit(EXIT_ERROR);
					}
					if(stringTypeInt == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
					{
						if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype, &wordPOStype))
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype, &groupRefName)" << endl;
							exit(EXIT_ERROR);
						}
					}
					else if(stringTypeInt == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
					{
						if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word, &word))
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word, &groupRefName)" << endl;
							exit(EXIT_ERROR);
						}	
					}
					else if(stringTypeInt == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
					{
						if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer, &tokenLayer))
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer, &groupRefName)" << endl;
							exit(EXIT_ERROR);
						}		
						if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass, &tokenClass))
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass, &groupRefName)" << endl;
							exit(EXIT_ERROR);
						}
						if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType, &tokenType))
						{
							//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType, &groupRefName)" << endl;
						}
					}
					else
					{
						cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: stringType unknown; stringType = " << stringType << ", stringTypeInt = " << stringTypeInt << endl;
						exit(EXIT_ERROR);	
					}
				}
				else
				{
					cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: componentType unknown, componentTypeInt = " << componentTypeInt << endl;
					exit(EXIT_ERROR);	
				}
			}
			else
			{
				cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType, &componentType)" << endl;
				cout << "currentTagInTxtRelTranslatorGroupTag->lineCount = " << currentTagInTxtRelTranslatorGroupTag->lineCount << endl;
				exit(EXIT_ERROR);			
			}

			
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_missing, &missing))
			{
				cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_missing, &groupRefName)" << endl;
				exit(EXIT_ERROR);
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_addToExplicitWordTempPOS, &addToExplicitWordTempPOS))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_addToExplicitWordTempPOS, &groupRefName)" << endl;
			}
			#endif
			
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationIndexType, &(semanticRelationIndexType[0])))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationIndexType, &(semanticRelationIndexType[0]))" << endl;
			}
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationIndexType2, &(semanticRelationIndexType[1])))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationIndexType2, &(semanticRelationIndexType[1]))" << endl;
			}
			for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
			{
				if(!SHAREDvars.textInTextArray(semanticRelationIndexType[i], GIAposRelTranslatorRulesGroupsComponentSemanticRelationIndexTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_NUMBER_OF_TYPES, &(semanticRelationIndexTypeInt[i])))
				{
					cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: semanticRelationIndexType unknown; semanticRelationIndexType[i] = " << semanticRelationIndexType[i] << endl;
					exit(EXIT_ERROR);
				}
			}	
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnEntity, &semanticRelationReturnEntity))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnEntity, &semanticRelationReturnEntity)" << endl;
			}
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnConnectionDynamic, &semanticRelationReturnConnectionDynamic))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnConnectionDynamic, &semanticRelationReturnConnectionDynamic)" << endl;
			}
			#endif
			if(XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnFunctionName, &semanticRelationReturnFunctionName))
			{
				if(XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnFunctionNameIndexType, &semanticRelationReturnFunctionNameIndexType))
				{
					if(!SHAREDvars.textInTextArray(semanticRelationReturnFunctionNameIndexType, GIAposRelTranslatorRulesGroupsComponentSemanticRelationIndexTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_NUMBER_OF_TYPES, &semanticRelationReturnFunctionNameIndexTypeInt))
					{
						cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: semanticRelationReturnFunctionNameIndexType unknown; semanticRelationReturnFunctionNameIndexType = " << semanticRelationReturnFunctionNameIndexType << endl;
						exit(EXIT_ERROR);
					}
				}
				else
				{
					cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnFunctionNameIndexType, &semanticRelationReturnFunctionNameIndexType)" << endl;
				}
			}
			else
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnFunctionName, &semanticRelationReturnFunctionName)" << endl;
			}

			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationRecord, &semanticRelationRecord))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationRecord, &semanticRelationRecord)" << endl;
			}
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationConnect, &semanticRelationConnect))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationConnect, &semanticRelationConnect)" << endl;
			}
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_optional, &optional))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} warning: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_optional, &optional)" << endl;
			}
			
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_queryComparisonVariable, &queryComparisonVariable))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_queryComparisonVariable, &queryComparisonVariable)" << endl;
			}
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isAuxiliaryQuery, &isAuxiliaryQuery))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isAuxiliaryQuery, &isAuxiliaryQuery)" << endl;
			}
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isWhichOrEquivalentWhatQuery, &isWhichOrEquivalentWhatQuery))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isWhichOrEquivalentWhatQuery, &isWhichOrEquivalentWhatQuery)" << endl;
			}
			#endif	
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isNameQuery, &isNameQuery))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isNameQuery, &isNameQuery)" << endl;
			}
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isName, &isName))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isName, &isName)" << endl;
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isNumberOf, &isNumberOf))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isNumberOf, &isNumberOf)" << endl;
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
			if(XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordVerbVariantType, &wordVerbVariantType))
			{
				if(!SHAREDvars.textInTextArray(wordVerbVariantType, GIApreprocessorMultiwordReductionVerbDatabaseTagBaseTenseFormArray, GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES, &wordVerbVariantTypeInt))
				{
					cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: semanticRelationReturnFunctionNameIndexType unknown; wordVerbVariantType = " << wordVerbVariantType << endl;
					exit(EXIT_ERROR);
				}
			}
			if(XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordNounVariantType, &wordNounVariantType))
			{
				if(!SHAREDvars.textInTextArray(wordNounVariantType, GIApreprocessorMultiwordReductionNounDatabaseTagBaseTenseFormArray, GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES, &wordNounVariantTypeInt))
				{
					cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: semanticRelationReturnFunctionNameIndexType unknown; wordNounVariantType = " << wordNounVariantType << endl;
					exit(EXIT_ERROR);
				}		
			}						
			#endif			
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isExpletive, &isExpletive))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isExpletive, &isExpletive)" << endl;
			}
			#endif
			
			GIAposRelTranslatorRulesComponentNeuralNetwork* component = new GIAposRelTranslatorRulesComponentNeuralNetwork();
			component->componentIndex = componentIndex;
			#ifdef GIA_POS_REL_TRANSLATOR_SANI
			component->ownerGroup = groupOwner;	//enables reverse lookup for ANN
			if(parseSubcomponent)
			{
				component->isSubcomponent = true;
				component->ownerComponent = subComponentOwner;
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
			component->referenceSetTypeHybrid = referenceSetTypeHybridInt;
			#endif
			component->componentType = componentTypeInt;
			component->groupTypeRefName = groupTypeRefName;
			component->groupRefName = groupRefName;
			component->stringType = stringTypeInt;
			component->word = word;
			component->wordPOStype = wordPOStype;
			component->tokenLayer = tokenLayer;
			component->tokenClass = tokenClass;
			component->tokenType = tokenType;
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
			component->missing = SHAREDvars.convertStringToBool(missing);
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
			component->stringTypeExplicitAddToExplicitWordTempPOS = SHAREDvars.convertStringToBool(addToExplicitWordTempPOS);
			#endif
			component->semanticRelationIndexType = semanticRelationIndexTypeInt;
			component->semanticRelationReturnEntity = SHAREDvars.convertStringToBool(semanticRelationReturnEntity);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
			component->semanticRelationReturnConnectionDynamic = SHAREDvars.convertStringToBool(semanticRelationReturnConnectionDynamic);
			#endif
			component->semanticRelationReturnFunctionName = semanticRelationReturnFunctionName;
			component->semanticRelationReturnFunctionNameIndexType = semanticRelationReturnFunctionNameIndexTypeInt;
			component->semanticRelationRecord = SHAREDvars.convertStringToBool(semanticRelationRecord);
			component->semanticRelationConnect = SHAREDvars.convertStringToBool(semanticRelationConnect);
			component->optional = SHAREDvars.convertStringToBool(optional);
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
			component->queryComparisonVariable = SHAREDvars.convertStringToBool(queryComparisonVariable);
			component->isAuxiliaryQuery = SHAREDvars.convertStringToBool(isAuxiliaryQuery);
			component->isWhichOrEquivalentWhatQuery = SHAREDvars.convertStringToBool(isWhichOrEquivalentWhatQuery);
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
			component->isNameQuery = SHAREDvars.convertStringToBool(isNameQuery);
			component->isName = SHAREDvars.convertStringToBool(isName);
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
			component->isNumberOf = SHAREDvars.convertStringToBool(isNumberOf);
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
			component->wordVerbVariantType = wordVerbVariantTypeInt;
			component->wordNounVariantType = wordNounVariantTypeInt;
			//cout << "wordNounVariantTypeInt = " << wordNounVariantTypeInt << endl;
			#endif				
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
			component->isExpletive = SHAREDvars.convertStringToBool(isExpletive);
			#endif
			
			componentsList->push_back(component);
			
			componentIndex++;
			
			//XMLparserTag* firstTagInTxtRelTranslatorComponentTag = XMLparserClass.parseTagDownALevel(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component, &result);
		}
		else if(currentTagInTxtRelTranslatorGroupTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_or)
		{
			if(parseSubcomponent)
			{
				cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error:(currentTagInTxtRelTranslatorGroupTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_or) && parseSubcomponent" << endl;
				exit(EXIT_ERROR);
			}
			
			GIAposRelTranslatorRulesComponentNeuralNetwork* component = new GIAposRelTranslatorRulesComponentNeuralNetwork();
			component->componentIndex = componentIndex;
			#ifdef GIA_POS_REL_TRANSLATOR_SANI
			component->ownerGroup = groupOwner;	//enables reverse lookup for ANN
			#endif
			component->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR;
			componentsList->push_back(component);

			XMLparserTag* firstTagInTxtRelTranslatorOrTag = XMLparserClass.parseTagDownALevel(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_or, &result);

			if(!parseComponents(firstTagInTxtRelTranslatorOrTag, groupOwner, &(component->subComponents), true, component))
			{
				cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !parseComponents(firstTagInTxtRelTranslatorOrTag, &(component->components))" << endl;
				exit(EXIT_ERROR);			
			}
			
			componentIndex++;
		}
		else if(!parseSubcomponent && currentTagInTxtRelTranslatorGroupTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat)
		{	
			if(parseSubcomponent)
			{
				cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error:(currentTagInTxtRelTranslatorGroupTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat) && parseSubcomponent" << endl;
				exit(EXIT_ERROR);
			}
					
			string optional = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false;
			if(!XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_optional, &optional))
			{
				//cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !XMLparserClass.getAttribute(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_optional, &optional)" << endl;
			}
			
			GIAposRelTranslatorRulesComponentNeuralNetwork* component = new GIAposRelTranslatorRulesComponentNeuralNetwork();
			component->componentIndex = componentIndex;
			#ifdef GIA_POS_REL_TRANSLATOR_SANI
			component->ownerGroup = groupOwner;	//enables reverse lookup for ANN
			#endif
			component->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT;
			component->optional = SHAREDvars.convertStringToBool(optional);
			componentsList->push_back(component);

			XMLparserTag* firstTagInTxtRelTranslatorRepeatTag = XMLparserClass.parseTagDownALevel(currentTagInTxtRelTranslatorGroupTag, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat, &result);

			if(!parseComponents(firstTagInTxtRelTranslatorRepeatTag, groupOwner, &(component->subComponents), true, component))
			{
				cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !parseComponents(firstTagInTxtRelTranslatorRepeatTag, &(component->components))" << endl;
				exit(EXIT_ERROR);			
			}
			
			componentIndex++;
		}
		else
		{
			cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: !(currentTagInTxtRelTranslatorGroupTag->name == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component); currentTagInTxtRelTranslatorGroupTag->name = " << currentTagInTxtRelTranslatorGroupTag->name << endl;
			exit(EXIT_ERROR);					
		}

		currentTagInTxtRelTranslatorGroupTag = currentTagInTxtRelTranslatorGroupTag->nextTag;
	}
	
	return result;
}


bool GIAposRelTranslatorRulesClass::connectGroupsReferences(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];

			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
			group->initiateANNneuron(group->groupTypeName + ":" + group->groupName);
			#endif
									
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
			cout << "GIAposRelTranslatorRulesClass::connectGroupsReferences{}: group->groupName = " << group->groupName << endl;
			#endif
			//cout << "GIAposRelTranslatorRulesClass::connectGroupsReferences{}: group->groupName = " << group->groupName << endl;
			if(!connectComponentsReferences(GIAposRelTranslatorRulesGroupTypes, &(group->components), false))
			{
				result = false;
			}
		}
	}
	return result;
}
bool GIAposRelTranslatorRulesClass::connectComponentsReferences(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents)
{
	bool result = true;
	for(int j=0; j<components->size(); j++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (*components)[j];
		if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
		{
			if(component->groupTypeRefName != "")
			{
				if(component->groupRefName != "")
				{	
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
					cout << "\tGIAposRelTranslatorRulesClass::connectComponentsReferences{}: component->groupTypeRefName = " << component->groupTypeRefName << ", component->groupRefName = " << component->groupRefName <<endl;
					#endif
					if(!findGroup(GIAposRelTranslatorRulesGroupTypes, component->groupTypeRefName, component->groupRefName, &(component->groupTypeRef), &(component->groupRef)))
					{
						result = false;
						cerr << "GIAposRelTranslatorRulesClass::connectComponentsReferences{} error: !findGroup(), component->groupTypeRefName = " << component->groupTypeRefName << ", component->groupRefName = " << component->groupRefName <<endl;
						exit(EXIT_ERROR);
					}
				}
				else
				{
					if(!findGroupType(GIAposRelTranslatorRulesGroupTypes, component->groupTypeRefName, &(component->groupTypeRef)))
					{
						result = false;
						cerr << "GIAposRelTranslatorRulesClass::connectComponentsReferences{} error: !findGroupType(), component->groupTypeRefName = " << component->groupTypeRefName << endl;
						exit(EXIT_ERROR);
					}
					//cout << "\tcomponent->groupTypeRef->groupTypeName = " << component->groupTypeRef->groupTypeName << endl;					
				}
			}
		}
		else if((component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
		{
			if(subcomponents)
			{
				result = false;
				cerr << "GIAposRelTranslatorRulesClass::connectComponentsReferences{} error: (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR/REPEAT) && subcomponents" << endl;
				exit(EXIT_ERROR);	
			}
			if(!connectComponentsReferences(GIAposRelTranslatorRulesGroupTypes, &(component->subComponents), true))
			{
				result = false;
				cerr << "GIAposRelTranslatorRulesClass::connectComponentsReferences{} error: !connectComponentsReferences()" << endl;
				exit(EXIT_ERROR);			
			}
		}
	}
	return result;
}
bool GIAposRelTranslatorRulesClass::findGroupType(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, const string groupTypeName, GIAposRelTranslatorRulesGroupType** groupTypeFound)
{
	bool result = false;
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		//cout << "groupType->groupTypeName = " << groupType->groupTypeName << endl;
		if(groupType->groupTypeName == groupTypeName)
		{
			*groupTypeFound = groupType;
			result = true;
		}
	}
	return result;
}
bool GIAposRelTranslatorRulesClass::findGroup(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, const string groupTypeName, const string groupName, GIAposRelTranslatorRulesGroupType** groupTypeFound, GIAposRelTranslatorRulesGroupNeuralNetwork** groupFound)
{
	bool result = false;
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		if(groupType->groupTypeName == groupTypeName)
		{
			*groupTypeFound = groupType;
			for(int j=0; j<groupType->groups.size(); j++)
			{
				GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
				if(group->groupName == groupName)
				{
					*groupFound = group;
					result = true;
				}
			}
		}
	}
	return result;
}

bool GIAposRelTranslatorRulesClass::extractGIAposRelTranslatorRulesTokens(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers)
{
	bool result = true;
	vector<string> logicReferenceInstanceList;

	XMLparserTag* firstTagInRulesTag = XMLparserClass.parseTagDownALevel(GIAposRelTranslatorFirstTagInXMLfile, RULES_XML_TAG_rules, &result);
	if(result)
	{
		XMLparserTag* currentTag = firstTagInRulesTag;
		bool foundTxtRelTranslatorTag = false;
		XMLparserTag* firstTagInTxtRelTranslatorTag = NULL;
		while((currentTag->nextTag != NULL) && !foundTxtRelTranslatorTag)
		{
			if(currentTag->name == GIA_POS_REL_TRANSLATOR_RULES_TAG_txtRelTranslator)
			{
				foundTxtRelTranslatorTag = true;
				firstTagInTxtRelTranslatorTag = XMLparserClass.parseTagDownALevel(currentTag, GIA_POS_REL_TRANSLATOR_RULES_TAG_txtRelTranslator, &result);
			}
			currentTag = currentTag->nextTag;
		}
		if(foundTxtRelTranslatorTag)
		{
			XMLparserTag* currentTagInTxtRelTranslator = firstTagInTxtRelTranslatorTag;
			
			bool foundTxtRelTranslatorTokensTag = false;
			XMLparserTag* firstTagInTxtRelTranslatorTokensTag = NULL;
			while((currentTagInTxtRelTranslator->nextTag != NULL) && !foundTxtRelTranslatorTokensTag)
			{
				if(currentTagInTxtRelTranslator->name == GIA_POS_REL_TRANSLATOR_RULES_TAG_tokens)
				{
					foundTxtRelTranslatorTokensTag = true;
					firstTagInTxtRelTranslatorTokensTag = XMLparserClass.parseTagDownALevel(currentTagInTxtRelTranslator, GIA_POS_REL_TRANSLATOR_RULES_TAG_tokens, &result);
				}
				currentTagInTxtRelTranslator = currentTagInTxtRelTranslator->nextTag;
			}
			
			if(foundTxtRelTranslatorTokensTag)
			{
				XMLparserTag* currentTagInTokens = firstTagInTxtRelTranslatorTokensTag;
				bool foundTxtRelTranslatorLogicReferenceTag = false;
				XMLparserTag* firstTagInTxtRelTranslatorLogicReferenceTag = NULL;
				while((currentTagInTokens->nextTag != NULL) && !foundTxtRelTranslatorLogicReferenceTag)
				{
					if(currentTagInTokens->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer)
					{
						GIAposRelTranslatorRulesTokenLayers->push_back(currentTagInTokens);
						
						//error checking only:
						if(currentTagInTokens->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
						{
							string layerName = currentTagInTokens->firstAttribute->value;
							if(layerName == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME)
							{
								//cout << "DEBUG: (layerName == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer_logicReference)" << endl;
								foundTxtRelTranslatorLogicReferenceTag = true;	//only record the first 3 <layer> tags
								firstTagInTxtRelTranslatorLogicReferenceTag = XMLparserClass.parseTagDownALevel(currentTagInTokens, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer, &result);
								//*firstTxtRelTranslatorRulesTokenFirstLogicReferenceTag = firstTagInTxtRelTranslatorLogicReferenceTag;
							}
						}
					}
					currentTagInTokens = currentTagInTokens->nextTag;
				}

				//error checking only:
				if(foundTxtRelTranslatorLogicReferenceTag)
				{
					XMLparserTag* currentTagInTxtRelTranslatorLogicReference = firstTagInTxtRelTranslatorLogicReferenceTag;
					while(currentTagInTxtRelTranslatorLogicReference->nextTag != NULL)
					{
						if(currentTagInTxtRelTranslatorLogicReference->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class)
						{	
							bool foundClass = true;
							int logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED;
							if(currentTagInTxtRelTranslatorLogicReference->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
							{
								string className = currentTagInTxtRelTranslatorLogicReference->firstAttribute->value;
								if(className == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_undefined)
								{
									logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED;
								}
								else if(className == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_conjunction)
								{
									logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION;
								}
								else if(className == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_verb)
								{
									logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB;
								}
								else if(className == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_preposition)
								{
									logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION;
								}
								else
								{
									cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: illegal class name = currentTagInTxtRelTranslatorLogicReference->firstAttribute->value = " << currentTagInTxtRelTranslatorLogicReference->firstAttribute->value << endl;
									exit(EXIT_ERROR);
								}
							}
							else
							{
								cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: illegal class name = currentTagInTxtRelTranslatorLogicReference->firstAttribute->name = " << currentTagInTxtRelTranslatorLogicReference->firstAttribute->name << ", expected: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name" << endl;
								exit(EXIT_ERROR);
							}

							if(foundClass)
							{
								XMLparserTag* firstTagInClassTag = XMLparserClass.parseTagDownALevel(currentTagInTxtRelTranslatorLogicReference, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class, &result);	
								XMLparserTag* currentTagInClass = firstTagInClassTag;
								while(currentTagInClass->nextTag != NULL)
								{
									if(currentTagInClass->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type)
									{

										if(currentTagInClass->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
										{

										}
										else
										{
											cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: illegal type name = currentTagInClass->firstAttribute->name = " << currentTagInClass->firstAttribute->name << ", expected: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name" << endl;
											exit(EXIT_ERROR);
										}

										XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type, &result);
										XMLparserTag* currentTagInType = firstTagInTypeTag;
										while(currentTagInType->nextTag != NULL)
										{
											if(currentTagInType->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance)
											{

												if(currentTagInType->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
												{
													string instanceName = currentTagInType->firstAttribute->value;
													//logicReferenceInstanceList->push_back(instanceName);
												}
												else
												{
													cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: illegal instance name = currentTagInType->firstAttribute->name = " << currentTagInType->firstAttribute->name << ", expected: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name" << endl;
													exit(EXIT_ERROR);
												}
											}
											else
											{
												cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: illegal currentTagInType->name = " << currentTagInType->name << ", expected: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance" << endl;
												exit(EXIT_ERROR);
											}
											currentTagInType = currentTagInType->nextTag;
										}
									}
									else
									{
										cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: illegal currentTagInClass->name = " << currentTagInClass->name << ", expected: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type" << endl;
										exit(EXIT_ERROR);
									}
									currentTagInClass = currentTagInClass->nextTag;
								}
							}
						}
						else
						{
							cerr << "GIAposRelTranslatorRules::extractGIAposRelTranslatorRulesGroups{} error: illegal currentTagInTxtRelTranslatorLogicReference->name = " << currentTagInTxtRelTranslatorLogicReference->name << ", expected: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class" << endl;
							exit(EXIT_ERROR);
						}
						currentTagInTxtRelTranslatorLogicReference = currentTagInTxtRelTranslatorLogicReference->nextTag;
					}
				}
			}
		}
	}
	
	GIAposRelTranslatorRulesTokenLayersGlobal = GIAposRelTranslatorRulesTokenLayers;

	return result;
}

bool GIAposRelTranslatorRulesClass::isClassTagWrapper(string word, string layerNameToFind, string classNameToFind, string classTypeNameToFind, vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers)
{
	if(classTypeNameToFind == "")
	{
		string classTypeNameFound = "";
		return isClassTag(word, layerNameToFind, classNameToFind, &classTypeNameFound, GIAposRelTranslatorRulesTokenLayers);
	}
	else
	{
		return isClassTag(word, layerNameToFind, classNameToFind, classTypeNameToFind, GIAposRelTranslatorRulesTokenLayers);	
	}
}

bool GIAposRelTranslatorRulesClass::isClassTag(string word, string layerNameToFind, string classNameToFind, string* classTypeNameFound, vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers)
{
	bool classTagFound = false;
	bool result = false;

	for(int i=0; i<GIAposRelTranslatorRulesTokenLayers->size(); i++)
	{
		XMLparserTag* currentLayerTag = GIAposRelTranslatorRulesTokenLayers->at(i);
		string layerName;
		if(!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName))
		{
			cerr << "GIAposRelTranslatorHybridClass::executeTxtRelTranslatorDepreciated{} error: (!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName)" << endl;
			exit(EXIT_ERROR);
		}
		if(layerName == layerNameToFind)
		{
			XMLparserTag* firstXMLtagInLayer = XMLparserClass.parseTagDownALevel(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer, &result);
			XMLparserTag* firstLogicReferenceClassTag = firstXMLtagInLayer;
			
			XMLparserTag* currentTagInLogicReferencePreprocessor = firstLogicReferenceClassTag;
			string wordLowerCase = SHAREDvars.convertStringToLowerCase(&word);

			while(currentTagInLogicReferencePreprocessor->nextTag != NULL)
			{
				if(currentTagInLogicReferencePreprocessor->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class)
				{	
					bool sameClass = false;
					//vector<string>* logicReferenceClassVector = NULL;
					if(currentTagInLogicReferencePreprocessor->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
					{
						string className = currentTagInLogicReferencePreprocessor->firstAttribute->value;
						if(className == classNameToFind)
						{
							sameClass = true;	
						}
					}

					if(sameClass)
					{				
						XMLparserTag* firstTagInClass = XMLparserClass.parseTagDownALevel(currentTagInLogicReferencePreprocessor, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class, &result);
						XMLparserTag* currentTagInClass = firstTagInClass;
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
						//cout << "classNameToFind = " << classNameToFind << endl;
						#endif
						while(currentTagInClass->nextTag != NULL)
						{
							if(currentTagInClass->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type)
							{
								#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
								//cout << "\tGIAposRelTranslatorRules::isClassTag{} debug: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type" << endl;
								#endif
								string classTypeName = "";
								if(currentTagInClass->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
								{
									classTypeName = currentTagInClass->firstAttribute->value;
								}


								XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type, &result);
								XMLparserTag* currentTagInType = firstTagInTypeTag;
								while(currentTagInType->nextTag != NULL)
								{
									if(currentTagInType->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance)
									{
										#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
										//cout << "\t\tGIAposRelTranslatorRules::isClassTag{} debug: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance" << endl;
										#endif
										if(currentTagInType->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
										{
											string instanceName = currentTagInType->firstAttribute->value;

											if(instanceName == wordLowerCase)
											{
												classTagFound = true;
												*classTypeNameFound = classTypeName;
											}
										}
									}
									currentTagInType = currentTagInType->nextTag;
								}
							}

							currentTagInClass = currentTagInClass->nextTag;
						}
					}
				}

				currentTagInLogicReferencePreprocessor = currentTagInLogicReferencePreprocessor->nextTag;
			}
		}
	}

	return classTagFound;
}

bool GIAposRelTranslatorRulesClass::isClassTag(string word, string layerNameToFind, string classNameToFind, string classTypeNameToFind, vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers)
{
	bool classTagFound = false;
	bool result = false;
	
	string wordLowerCase = SHAREDvars.convertStringToLowerCase(&word);

	for(int i=0; i<GIAposRelTranslatorRulesTokenLayers->size(); i++)
	{
		XMLparserTag* currentLayerTag = GIAposRelTranslatorRulesTokenLayers->at(i);
		string layerName;
		if(!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName))
		{
			cerr << "GIAposRelTranslatorHybridClass::executeTxtRelTranslatorDepreciated{} error: (!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName)" << endl;
			exit(EXIT_ERROR);
		}
		if(layerName == layerNameToFind)
		{
			XMLparserTag* firstXMLtagInLayer = XMLparserClass.parseTagDownALevel(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer, &result);
			XMLparserTag* currentXMLtagInLayer = firstXMLtagInLayer;
			
			while(currentXMLtagInLayer->nextTag != NULL)
			{
				if(currentXMLtagInLayer->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class)
				{	
					bool sameClass = false;
					//vector<string>* logicReferenceClassVector = NULL;
					if(currentXMLtagInLayer->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
					{
						string className = currentXMLtagInLayer->firstAttribute->value;
						if(className == classNameToFind)
						{
							sameClass = true;	
						}
					}

					if(sameClass)
					{				
						XMLparserTag* firstTagInClass = XMLparserClass.parseTagDownALevel(currentXMLtagInLayer, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class, &result);
						XMLparserTag* currentTagInClass = firstTagInClass;
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
						//cout << "classNameToFind = " << classNameToFind << endl;
						#endif
						while(currentTagInClass->nextTag != NULL)
						{
							if(currentTagInClass->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type)
							{
								#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
								//cout << "\tGIAposRelTranslatorRules::isClassTag{} debug: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type" << endl;
								#endif
								string classTypeName = "";
								bool sameType = false;
								if(currentTagInClass->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
								{
									classTypeName = currentTagInClass->firstAttribute->value;
									if(classTypeNameToFind != "")
									{
										if(classTypeName == classTypeNameToFind)
										{
											sameType = true;	
										}
									}
									else
									{
										sameType = true;
									}
								}

								if(sameType)
								{
									XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type, &result);
									XMLparserTag* currentTagInType = firstTagInTypeTag;
									while(currentTagInType->nextTag != NULL)
									{
										if(currentTagInType->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance)
										{
											#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
											//cout << "\t\tGIAposRelTranslatorRules::isClassTag{} debug: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance" << endl;
											#endif
											if(currentTagInType->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
											{
												string instanceName = currentTagInType->firstAttribute->value;

												if(instanceName == wordLowerCase)
												{
													classTagFound = true;
												}
											}
										}
										currentTagInType = currentTagInType->nextTag;
									}
								}
							}

							currentTagInClass = currentTagInClass->nextTag;
						}
					}
				}

				currentXMLtagInLayer = currentXMLtagInLayer->nextTag;
			}
		}
	}
	
	return classTagFound;
}



#ifdef GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
bool GIAposRelTranslatorRulesClass::removeLastOptionalComponents(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		
		int groupTypeGroupsSizeOrig = (groupType->groups).size();
		for(int groupIndex=0; groupIndex<(groupType->groups).size(); groupIndex++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[groupIndex];
			vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components = &(group->components);
					
			if(components->size() > 0)
			{
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
				int lastComponentOptionalComponentIndex = 0;
				#else
				int lastComponentOptionalComponentIndex = components->size()-1;
				#endif
				GIAposRelTranslatorRulesComponentNeuralNetwork* lastComponent = (*components)[lastComponentOptionalComponentIndex];
				
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_OR_OPTIONAL_REPEAT
				bool hasOptionalRepeat = false;
				int repeatOptionalComponentIndex = INT_DEFAULT_VALUE;
				for(int q=0; q<group->components.size(); q++)
				{	
					GIAposRelTranslatorRulesComponentNeuralNetwork* component = group->components[q];
					if((component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT) && (component->optional))
					{
						hasOptionalRepeat = true;
						repeatOptionalComponentIndex = q;
					}
				}
				#endif
		
				if(lastComponent->optional)
				{
					if(components->size() == 1 && lastComponent->optional)
					{
						//group only contains one component and it is optional
						cerr << "GIAposRelTranslatorRulesClass::removeLastOptionalComponents{} warning: (components->size() == 1 && lastComponent->optional) - group only contains one component and it is optional" << endl;
					}
					removeOptionalComponent(groupType, &groupIndex, groupTypeGroupsSizeOrig, group, lastComponentOptionalComponentIndex);
				}
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_OR_OPTIONAL_REPEAT
				else if(hasOptionalRepeat)
				{
					removeOptionalComponent(groupType, &groupIndex, groupTypeGroupsSizeOrig, group, repeatOptionalComponentIndex);
				}
				#endif
			}
			else
			{
				cerr << "GIAposRelTranslatorRulesClass::removeLastOptionalComponents{} error: " << (components->size() == 0) << endl;
				exit(EXIT_ERROR);
			}
		}
	}
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7
	cout << "GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7 exit" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}	

bool GIAposRelTranslatorRulesClass::removeOptionalComponent(GIAposRelTranslatorRulesGroupType* groupType, int* groupIndex, const int groupTypeGroupsSizeOrig, GIAposRelTranslatorRulesGroupNeuralNetwork* group, const int optionalComponentIndex)
{
	bool result = true;
	
	vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components = &(group->components);
	
	GIAposRelTranslatorRulesGroupNeuralNetwork* optionalComponentsWrapperGroup = NULL;
	bool wrapperGroupAlreadyDefined = false;
	if(*groupIndex >= groupTypeGroupsSizeOrig)
	{
		wrapperGroupAlreadyDefined = true;
		optionalComponentsWrapperGroup = group->optionalComponentsWrapperGroup;
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7
		cout << "passed test: (*groupIndex >= groupTypeGroupsSizeOrig)" << endl;
		#endif
	}
			
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7
	cout << "GIAposRelTranslatorRulesClass::removeOptionalComponent: remove optional component from groupNew1" << endl;
	cout << "GIAposRelTranslatorRulesClass::removeOptionalComponent: set optional component from groupNew1 to optional==false" << endl;
	#endif

	//create groupNew1
	GIAposRelTranslatorRulesGroupNeuralNetwork* groupNew1 = copyGroup(group);
	groupNew1->groupName = groupNew1->groupName + GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND1;
	groupNew1->optionalComponentsWrapperGroup = group;
	//remove optional component from groupNew1;
	vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsNew1 = &(groupNew1->components);
	componentsNew1->erase(componentsNew1->begin()+optionalComponentIndex);	//CHECKTHIS
	updateComponentsOwnerGroupAndIndexes(groupNew1, &(groupNew1->components), false, NULL);
	(groupType->groups).push_back(groupNew1);

	//create groupNew2
	GIAposRelTranslatorRulesGroupNeuralNetwork* groupNew2 = copyGroup(group);
	groupNew2->groupName = groupNew2->groupName + GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND2;
	groupNew2->optionalComponentsWrapperGroup = group;
	//set optional component to !optional;
	vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsNew2 = &(groupNew2->components);
	GIAposRelTranslatorRulesComponentNeuralNetwork* optionalComponentNew2 = (*componentsNew2)[optionalComponentIndex];
	optionalComponentNew2->optional = false;
	updateComponentsOwnerGroupAndIndexes(groupNew2, &(groupNew2->components), false, NULL);
	(groupType->groups).push_back(groupNew2);

	GIAposRelTranslatorRulesComponentNeuralNetwork* artificialGroupOrComponent = NULL;
	if(wrapperGroupAlreadyDefined)
	{
		artificialGroupOrComponent = optionalComponentsWrapperGroup->components[0];
		
		//delete artifical group (reference) from groupTypes and artificialGroupOrComponent->components;
		(groupType->groups).erase((groupType->groups).begin() + *groupIndex);
		(*groupIndex) = (*groupIndex) - 1;
		for(int q=0; q<artificialGroupOrComponent->subComponents.size(); q++)
		{	
			GIAposRelTranslatorRulesComponentNeuralNetwork* component = artificialGroupOrComponent->subComponents[q];
			if(component->groupRefName == group->groupName)
			{
				(artificialGroupOrComponent->subComponents).erase((artificialGroupOrComponent->subComponents).begin() + q);
				q--;
			}
		}
	}
	else
	{
		//turn group into a wrapper group with an or function pointing to both groups
		optionalComponentsWrapperGroup = group;
		artificialGroupOrComponent = new GIAposRelTranslatorRulesComponentNeuralNetwork();
		artificialGroupOrComponent->componentIndex = GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI
		artificialGroupOrComponent->ownerGroup = group;	//enables reverse lookup for ANN
		#endif
		artificialGroupOrComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR;
		components->clear();
		components->push_back(artificialGroupOrComponent);
	}

	//add newly created artificial groups to wrapper group
	GIAposRelTranslatorRulesComponentNeuralNetwork* artificialGroupComponentNew1 = new GIAposRelTranslatorRulesComponentNeuralNetwork();
	artificialGroupComponentNew1->groupRefName = groupNew1->groupName;
	artificialGroupComponentNew1->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
	artificialGroupComponentNew1->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
	artificialGroupComponentNew1->componentIndex = artificialGroupOrComponent->subComponents.size();
	artificialGroupComponentNew1->ownerGroup = optionalComponentsWrapperGroup;
	artificialGroupComponentNew1->ownerComponent = artificialGroupOrComponent;
	artificialGroupComponentNew1->isSubcomponent = true;
	artificialGroupComponentNew1->semanticRelationReturnEntity = true;
	artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew1);
	GIAposRelTranslatorRulesComponentNeuralNetwork* artificialGroupComponentNew2 = new GIAposRelTranslatorRulesComponentNeuralNetwork();
	artificialGroupComponentNew2->groupRefName = groupNew2->groupName;
	artificialGroupComponentNew2->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
	artificialGroupComponentNew2->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
	artificialGroupComponentNew2->componentIndex = artificialGroupOrComponent->subComponents.size();
	artificialGroupComponentNew2->ownerGroup = optionalComponentsWrapperGroup;
	artificialGroupComponentNew2->ownerComponent = artificialGroupOrComponent;
	artificialGroupComponentNew2->isSubcomponent = true;
	artificialGroupComponentNew2->semanticRelationReturnEntity = true;
	artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew2);
	for(int c=GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST; c<artificialGroupOrComponent->subComponents.size(); c++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* subcomponent = artificialGroupOrComponent->subComponents[c];
		subcomponent->componentIndex = c;
	}
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
	cout << "artificialGroupComponentNew1->groupRefName = " << artificialGroupComponentNew1->groupRefName << endl;
	cout << "artificialGroupComponentNew2->groupRefName = " << artificialGroupComponentNew2->groupRefName << endl;
	#endif
	
	return result;
}

/*
bool GIAposRelTranslatorRulesClass::removeLastOptionalComponents(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		
		int groupTypeGroupsSizeOrig = (groupType->groups).size();
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
			vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components = &(group->components);
			
			GIAposRelTranslatorRulesGroupNeuralNetwork* optionalComponentsWrapperGroup = NULL;
			bool wrapperGroupAlreadyDefined = false;
			if(j >= groupTypeGroupsSizeOrig)
			{
				wrapperGroupAlreadyDefined = true;
				optionalComponentsWrapperGroup = group->optionalComponentsWrapperGroup;
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7
				cout << "passed test: (j >= groupTypeGroupsSizeOrig)" << endl;
				#endif
			}
							
			if(components->size() > 0)
			{
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
				GIAposRelTranslatorRulesComponentNeuralNetwork* lastComponent = (*components)[0];
				#else
				GIAposRelTranslatorRulesComponentNeuralNetwork* lastComponent = (*components)[components->size()-1];
				#endif
				if(components->size() == 1 && lastComponent->optional)
				{
					//group only contains one component and it is optional
					cerr << "GIAposRelTranslatorRulesClass::removeLastOptionalComponents{} warning: (components->size() == 1 && lastComponent->optional) - group only contains one component and it is optional" << endl;
				}
				if(lastComponent->optional)
				{

					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7
					cout << "GIAposRelTranslatorRulesClass::removeLastOptionalComponents: remove last component from groupNew1" << endl;
					cout << "GIAposRelTranslatorRulesClass::removeLastOptionalComponents: set last component from groupNew1 to optional==false" << endl;
					#endif

					//create groupNew1
					GIAposRelTranslatorRulesGroupNeuralNetwork* groupNew1 = copyGroup(group);
					groupNew1->groupName = groupNew1->groupName + GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND1;
					groupNew1->optionalComponentsWrapperGroup = group;
					//remove last component from groupNew1;
					vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsNew1 = &(groupNew1->components);
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
					//componentsNew1.pop_front();
					componentsNew1->erase(componentsNew1->begin());
					#else
					componentsNew1->pop_back();
					#endif
					updateComponentsOwnerGroupAndIndexes(groupNew1, &(groupNew1->components), false, NULL);
					(groupType->groups).push_back(groupNew1);

					//create groupNew2
					GIAposRelTranslatorRulesGroupNeuralNetwork* groupNew2 = copyGroup(group);
					groupNew2->groupName = groupNew2->groupName + GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND2;
					groupNew2->optionalComponentsWrapperGroup = group;
					//set last component to !optional;
					vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsNew2 = &(groupNew2->components);
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
					GIAposRelTranslatorRulesComponentNeuralNetwork* lastComponentNew2 = (*componentsNew2)[0];
					#else
					GIAposRelTranslatorRulesComponentNeuralNetwork* lastComponentNew2 = (*componentsNew2)[components->size()-1];
					#endif
					lastComponentNew2->optional = false;
					updateComponentsOwnerGroupAndIndexes(groupNew2, &(groupNew2->components), false, NULL);
					(groupType->groups).push_back(groupNew2);

					GIAposRelTranslatorRulesComponentNeuralNetwork* artificialGroupOrComponent = NULL;
					if(wrapperGroupAlreadyDefined)
					{
						artificialGroupOrComponent = optionalComponentsWrapperGroup->components[0];
						
						//delete artifical group (reference) from groupTypes and artificialGroupOrComponent->components;
						(groupType->groups).erase((groupType->groups).begin() + j);
						j--;
						for(int q=0; q<artificialGroupOrComponent->subComponents.size(); q++)
						{	
							GIAposRelTranslatorRulesComponentNeuralNetwork* component = artificialGroupOrComponent->subComponents[q];
							if(component->groupRefName == group->groupName)
							{
								(artificialGroupOrComponent->subComponents).erase((artificialGroupOrComponent->subComponents).begin() + q);
								q--;
							}
						}
					}
					else
					{
						//turn group into a wrapper group with an or function pointing to both groups
						optionalComponentsWrapperGroup = group;
						artificialGroupOrComponent = new GIAposRelTranslatorRulesComponentNeuralNetwork();
						artificialGroupOrComponent->componentIndex = GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST;
						#ifdef GIA_POS_REL_TRANSLATOR_SANI
						artificialGroupOrComponent->ownerGroup = group;	//enables reverse lookup for ANN
						#endif
						artificialGroupOrComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR;
						components->clear();
						components->push_back(artificialGroupOrComponent);
					}

					//add newly created artificial groups to wrapper group
					GIAposRelTranslatorRulesComponentNeuralNetwork* artificialGroupComponentNew1 = new GIAposRelTranslatorRulesComponentNeuralNetwork();
					artificialGroupComponentNew1->groupRefName = groupNew1->groupName;
					artificialGroupComponentNew1->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
					artificialGroupComponentNew1->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
					artificialGroupComponentNew1->componentIndex = artificialGroupOrComponent->subComponents.size();
					artificialGroupComponentNew1->ownerGroup = optionalComponentsWrapperGroup;
					artificialGroupComponentNew1->ownerComponent = artificialGroupOrComponent;
					artificialGroupComponentNew1->isSubcomponent = true;
					artificialGroupComponentNew1->semanticRelationReturnEntity = true;
					artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew1);
					GIAposRelTranslatorRulesComponentNeuralNetwork* artificialGroupComponentNew2 = new GIAposRelTranslatorRulesComponentNeuralNetwork();
					artificialGroupComponentNew2->groupRefName = groupNew2->groupName;
					artificialGroupComponentNew2->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
					artificialGroupComponentNew2->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
					artificialGroupComponentNew2->componentIndex = artificialGroupOrComponent->subComponents.size();
					artificialGroupComponentNew2->ownerGroup = optionalComponentsWrapperGroup;
					artificialGroupComponentNew2->ownerComponent = artificialGroupOrComponent;
					artificialGroupComponentNew2->isSubcomponent = true;
					artificialGroupComponentNew2->semanticRelationReturnEntity = true;
					artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew2);
					for(int c=GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST; c<artificialGroupOrComponent->subComponents.size(); c++)
					{
						GIAposRelTranslatorRulesComponentNeuralNetwork* subcomponent = artificialGroupOrComponent->subComponents[c];
						subcomponent->componentIndex = c;
					}
					
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
					cout << "artificialGroupComponentNew1->groupRefName = " << artificialGroupComponentNew1->groupRefName << endl;
					cout << "artificialGroupComponentNew2->groupRefName = " << artificialGroupComponentNew2->groupRefName << endl;
					#endif
				}
			}
			else
			{
				cerr << "GIAposRelTranslatorRulesClass::removeLastOptionalComponents{} error: " << (components->size() == 0) << endl;
				exit(EXIT_ERROR);
			}
		}
	}
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7
	cout << "GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA7 exit" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}
*/

#endif
	
GIAposRelTranslatorRulesGroupNeuralNetwork* GIAposRelTranslatorRulesClass::copyGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{		
	GIAposRelTranslatorRulesGroupNeuralNetwork* newGroup = new GIAposRelTranslatorRulesGroupNeuralNetwork(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}
bool GIAposRelTranslatorRulesClass::copyComponents(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsNew)
{	
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i];
		copyComponent(currentComponent, componentsNew);
		/*
		GIAposRelTranslatorRulesComponentNeuralNetwork* newComponent = new GIAposRelTranslatorRulesComponentNeuralNetwork(*currentComponent);
		componentsNew->push_back(newComponent);
		if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			newComponent->subComponents.clear();
			copyComponents(&(currentComponent->subComponents), &(newComponent->subComponents)); 
		}
		*/
	}
	
	return result;
}

bool GIAposRelTranslatorRulesClass::copyComponent(GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsNew)
{	
	bool result = true;
	
	GIAposRelTranslatorRulesComponentNeuralNetwork* newComponent = new GIAposRelTranslatorRulesComponentNeuralNetwork(*currentComponent);
	componentsNew->push_back(newComponent);
	if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
	{
		newComponent->subComponents.clear();
		copyComponents(&(currentComponent->subComponents), &(newComponent->subComponents)); 
	}
	
	return result;
}

GIAposRelTranslatorRulesGroupParseTree* GIAposRelTranslatorRulesClass::copyGroup(GIAposRelTranslatorRulesGroupParseTree* group)
{		
	GIAposRelTranslatorRulesGroupParseTree* newGroup = new GIAposRelTranslatorRulesGroupParseTree(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}
bool GIAposRelTranslatorRulesClass::copyComponents(vector<GIAposRelTranslatorRulesComponentParseTree*>* components, vector<GIAposRelTranslatorRulesComponentParseTree*>* componentsNew)
{	
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* currentComponent = (*components)[i];
		GIAposRelTranslatorRulesComponentParseTree* newComponent = new GIAposRelTranslatorRulesComponentParseTree(*currentComponent);
		componentsNew->push_back(newComponent);
	}
	
	return result;
}


GIAposRelTranslatorRulesGroupActivationMemory* GIAposRelTranslatorRulesClass::copyGroup(GIAposRelTranslatorRulesGroupActivationMemory* group)
{		
	GIAposRelTranslatorRulesGroupActivationMemory* newGroup = new GIAposRelTranslatorRulesGroupActivationMemory(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}



#ifdef GIA_POS_REL_TRANSLATOR_SANI
bool GIAposRelTranslatorRulesClass::updateComponentsOwnerGroupAndIndexes(GIAposRelTranslatorRulesGroupNeuralNetwork* group, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, const bool isSubcomponent, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent)
{
	bool result = true;
	
	for(int c=GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST; c<components->size(); c++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (*components)[c];
		component->componentIndex = c;
		component->ownerGroup = group;
		if(isSubcomponent)
		{
			component->ownerComponent = ownerComponent;	//added GIA3i3c
		}
		if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component))
		{
			updateComponentsOwnerGroupAndIndexes(group, &(component->subComponents), true, component);
		}
	}
	
	return result;
}
#endif		

#endif








