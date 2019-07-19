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
 * File Name: GIAtxtRelTranslatorNeuralNetworkFormation.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3i2b 27-May-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Formation
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkFormation.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK


GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstInputGroupInNetwork;
GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionWordPOStype;
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionWordNounVariantType;
GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionWordVerbVariantType;
#endif
GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionExplicitWord;
GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayer;

int numberOfInputGroupsInSectionWordPOStype;
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
int numberOfInputGroupsInSectionNounVariantType;
int numberOfInputGroupsInSectionVerbVariantType;
#endif
int numberOfInputGroupsInSectionExplicitWord;
int numberOfInputGroupsInSectionTokensLayer;
unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*> inputLayerSectionExplicitWordMap;
unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*> inputLayerSectionTokensLayerMap;
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
vector<string> explicitWordListLocal;
#endif

//high level groupType names should be synced with GIAtxtRelTranslatorClass::generateParseTreeIntro
GIAtxtRelTranslatorRulesGroupNeuralNetwork* topLevelGroupInOuputLayerSectionStatements;
GIAtxtRelTranslatorRulesGroupNeuralNetwork* topLevelGroupInOuputLayerSectionQuestions;
GIAtxtRelTranslatorRulesGroupNeuralNetwork* topLevelGroupInOuputLayerSectionSubjects;

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
vector<string>* GIAtxtRelTranslatorNeuralNetworkFormationClass::getExplicitWordList()
{
	return &explicitWordListLocal;
}
#endif

unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* GIAtxtRelTranslatorNeuralNetworkFormationClass::getInputLayerSectionExplicitWordMap()
{
	return &inputLayerSectionExplicitWordMap;
}

unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* GIAtxtRelTranslatorNeuralNetworkFormationClass::getInputLayerSectionTokensLayerMap()
{
	return &inputLayerSectionTokensLayerMap;
}

GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkFormationClass::getFirstGroupInInputLayerSectionWordPOStype()
{
	return firstGroupInInputLayerSectionWordPOStype;
}

#ifdef GIA_NEURAL_NETWORK
GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkFormationClass::getFirstInputGroupInNetwork()
{
	return firstInputGroupInNetwork;
}
#endif



/*
[front of network]

input layer:                                        				                                                                                 			                                                              InputLayerSectionTokensLayer
                                                                                                                                                                                                                               /                          |                                      \
pre-input layer:        InputLayerSectionWordNounVariantType    InputLayerSectionWordVerbVariantType                                                                                                   InputLayerSectionTokensLayerClass                   InputLayerSectionTokensLayerClass               InputLayerSectionTokensLayerClass        
                                            \                                     /                                                                                                  			             /                        |                      \                                                             /               |              \
pre-input layer:                                     InputLayerSectionWordPOStype                                             InputLayerSectionExplicitWord                        InputLayerSectionTokensLayerClassType    InputLayerSectionTokensLayerClassType       InputLayerSectionTokensLayerClassType        			       

[back of network]
*/



bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createGIAtxtRelTranslatorNeuralNetwork(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	createInputNeuronLayer(GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes);
			
	createNeuronLayerIntro(GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes);
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
	cout << "exiting normally" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayer(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	/*
	create inputLayer - define all neurons for all;
		wordPOStype
			wordNounVariantType
			wordVerbVariantType
		explicit words in GIAtxtRelTranslatorRules.xml
		tokens-layer in GIAtxtRelTranslatorRules.xml
			classes
				types
	*/

	numberOfInputGroupsInSectionWordPOStype = 0;
	numberOfInputGroupsInSectionExplicitWord = 0;
	numberOfInputGroupsInSectionTokensLayer = 0;
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSection = NULL;
	firstGroupInInputLayerSectionWordPOStype = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
	firstInputGroupInNetwork = firstGroupInInputLayerSectionWordPOStype;
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionWordPOStype;
	if(!createInputNeuronLayerSectionWordPOStype(&currentGroupInInputLayerSection, &numberOfInputGroupsInSectionWordPOStype))
	{
		result = false;
	}
	firstGroupInInputLayerSectionExplicitWord = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionExplicitWord;
	if(!createInputNeuronLayerSectionExplicitWord(&currentGroupInInputLayerSection, &numberOfInputGroupsInSectionExplicitWord, GIAtxtRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	firstGroupInInputLayerSectionTokensLayer = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionTokensLayer;
	if(!createInputNeuronLayerSectionTokensLayer(GIAtxtRelTranslatorRulesTokenLayers, &currentGroupInInputLayerSection, &numberOfInputGroupsInSectionTokensLayer))
	{
		result = false;
	}

	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayerSectionWordPOStype(GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordPOStype, int* numberOfGroupsInSection)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
	numberOfInputGroupsInSectionNounVariantType = 0;
	numberOfInputGroupsInSectionVerbVariantType = 0;
	#endif
	
	for(int i=0; i<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
		if(i == GIA_PREPROCESSOR_POS_TYPE_NOUN)
		{
			firstGroupInInputLayerSectionWordNounVariantType = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
			if(!createInputNeuronLayerSectionNounVariantType(*currentGroupInInputLayerSectionWordPOStype, &firstGroupInInputLayerSectionWordNounVariantType, &numberOfInputGroupsInSectionNounVariantType))
			{
				result = false;
			}
		}
		if(i == GIA_PREPROCESSOR_POS_TYPE_VERB)
		{
			firstGroupInInputLayerSectionWordVerbVariantType = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
			if(!createInputNeuronLayerSectionVerbVariantType(*currentGroupInInputLayerSectionWordPOStype, &firstGroupInInputLayerSectionWordVerbVariantType, &numberOfInputGroupsInSectionVerbVariantType))
			{
				result = false;
			}
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
		(*currentGroupInInputLayerSectionWordPOStype)->neuronReference->GIAentityName = GIApreprocessorPOStypeNameArray[i];
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordPOStype, numberOfGroupsInSection);
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
		cout << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayerSectionWordPOStype{} addGroupToLayer(currentGroupInInputLayerSectionWordPOStype, numberOfGroupsInSection);  i = " << i << endl;
		#endif
	}
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayerSectionNounVariantType(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordNounVariantType, int* numberOfGroupsInSection)
{	
	bool result = true;
	
	for(int i=0; i<GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES; i++)
	{
		createGroupANNconnectionIO(currentGroupInInputLayerSectionWordPOStype, *currentGroupInInputLayerSectionWordNounVariantType);	//switched GIA3h3a
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
		(*currentGroupInInputLayerSectionWordNounVariantType)->neuronReference->GIAentityName = GIApreprocessorMultiwordReductionNounDatabaseTagBaseTenseFormArray[i];
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordNounVariantType, numberOfGroupsInSection);		
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayerSectionVerbVariantType(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordVerbVariantType, int* numberOfGroupsInSection)
{
	bool result = true;
	
	for(int i=0; i<GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES; i++)
	{
		createGroupANNconnectionIO(currentGroupInInputLayerSectionWordPOStype, *currentGroupInInputLayerSectionWordVerbVariantType);	//switched GIA3h3a
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
		(*currentGroupInInputLayerSectionWordVerbVariantType)->neuronReference->GIAentityName = GIApreprocessorMultiwordReductionVerbDatabaseTagBaseTenseFormArray[i];
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordVerbVariantType, numberOfGroupsInSection);		
	}
	
	return result;
}
#endif


bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayerSectionExplicitWord(GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSection, int* numberOfGroupsInSection, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	inputLayerSectionExplicitWordMap.clear();	//added GIA3g11aTEMP62
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	explicitWordListLocal.clear();
	#endif
	vector<string> explicitWordList;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
			if(!createInputNeuronLayerSectionExplicitWord(&(group->components), false, &explicitWordList))
			{
				result = false;
			}
		}
	}
	for(int i=0; i<explicitWordList.size(); i++)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
		(*currentGroupInInputLayerSection)->neuronReference->GIAentityName = explicitWordList[i];
		#endif
		inputLayerSectionExplicitWordMap.insert(pair<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>(explicitWordList[i], *currentGroupInInputLayerSection));
		addGroupToLayer(currentGroupInInputLayerSection, numberOfGroupsInSection);
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
	explicitWordListLocal = explicitWordList;
	#endif
	#endif
	
	
	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayerSectionExplicitWord(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, vector<string>* explicitWordList)
{
	bool result = true;
	
	for(int j=0; j<components->size(); j++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (*components)[j];
		
		if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
			{
				string word = component->word;
				if(!findWordInList(word, explicitWordList))
				{
					explicitWordList->push_back(word);
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
					cout << "GIAtxtRelTranslatorRulesClass::connectComponentsReferences{} explicitWordList->push_back(word) = " << word << endl;
					#endif
					
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
					if(component->stringTypeExplicitAddToExplicitWordTempPOS)
					{
						explicitWordListLocal.push_back(word);
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
						cout << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT: explicitWordListLocal.push_back(word) = " << word << endl;
						#endif 
					}
					#endif
				}
			}
		}
		else if((component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
		{
			if(subcomponents)
			{
				result = false;
				cerr << "GIAtxtRelTranslatorRulesClass::connectComponentsReferences{} error: (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR/REPEAT) && subcomponents" << endl;
				exit(EXIT_ERROR);	
			}
			if(!createInputNeuronLayerSectionExplicitWord(&(component->subComponents), true, explicitWordList))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorRulesClass::connectComponentsReferences{} error: !connectComponentsReferences()" << endl;
				exit(EXIT_ERROR);			
			}
		}
	}
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createInputNeuronLayerSectionTokensLayer(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionTokensLayer, int* numberOfGroupsInSectionTokensLayer)
{
	bool classTagFound = false;
	bool result = false;
	
	inputLayerSectionTokensLayerMap.clear();	//added GIA3g11aTEMP62
	
	for(int i=0; i<GIAtxtRelTranslatorRulesTokenLayers->size(); i++)
	{	
		XMLparserTag* currentLayerTag = GIAtxtRelTranslatorRulesTokenLayers->at(i);

		//cout << "i = " << i << endl;
		//cout << "currentLayerTag->name = " << currentLayerTag->name << endl;	//only the first 3 xml rules <tokens> <layer> layers detected

		string layerName = "";
		if(!XMLparserClass.getAttribute(currentLayerTag, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName))
		{
			cerr << "GIAtxtRelTranslatorHybridClass::executeTxtRelTranslatorDepreciated{} error: (!XMLparserClass.getAttribute(currentLayerTag, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName)" << endl;
			exit(EXIT_ERROR);
		}
		
		XMLparserTag* firstXMLtagInLayer = XMLparserClass.parseTagDownALevel(currentLayerTag, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_layer, &result);
		XMLparserTag* currentXMLtagInLayer = firstXMLtagInLayer;

		GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClass = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClass = firstGroupInInputLayerSectionTokensLayerClass;
		int numberOfGroupsInSectionTokensLayerClass = 0; 
		
		while(currentXMLtagInLayer->nextTag != NULL)
		{
			if(currentXMLtagInLayer->name == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_class)
			{	
				bool sameClass = false;
				string className = "";
				//vector<string>* logicReferenceClassVector = NULL;
				if(currentXMLtagInLayer->firstAttribute->name == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
				{
					className = currentXMLtagInLayer->firstAttribute->value;
				}
				
				XMLparserTag* firstTagInClass = XMLparserClass.parseTagDownALevel(currentXMLtagInLayer, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_class, &result);
				XMLparserTag* currentTagInClass = firstTagInClass;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				//cout << "classNameToFind = " << classNameToFind << endl;
				#endif
				
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClassType = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassType = firstGroupInInputLayerSectionTokensLayerClassType;
				int numberOfGroupsInSectionTokensLayerClassType = 0; 
		
				while(currentTagInClass->nextTag != NULL)
				{
					if(currentTagInClass->name == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_type)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
						//cout << "\tGIAtxtRelTranslatorRules::isClassTag{} debug: GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_type" << endl;
						#endif
						string classTypeName = "";
						bool sameType = false;
						if(currentTagInClass->firstAttribute->name == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
						{
							classTypeName = currentTagInClass->firstAttribute->value;
						}

						XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_type, &result);
						XMLparserTag* currentTagInType = firstTagInTypeTag;
						
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClassTypeInstance = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = firstGroupInInputLayerSectionTokensLayerClassTypeInstance;
						int numberOfGroupsInSectionTokensLayerClassTypeInstance = 0; 
				
						while(currentTagInType->nextTag != NULL)
						{
							if(currentTagInType->name == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_instance)
							{
								#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
								//cout << "\t\tGIAtxtRelTranslatorRules::isClassTag{} debug: GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_TAG_instance" << endl;
								#endif
								string instanceName = "";
								if(currentTagInType->firstAttribute->name == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
								{
									instanceName = currentTagInType->firstAttribute->value;
									#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
									cout << "GIAtxtRelTranslatorRulesClass::createInputNeuronLayerSectionTokensLayer{} instanceName = " << instanceName << endl;
									#endif
								}
								
								currentGroupInInputLayerSectionTokensLayerClassTypeInstance->GIAtokenLayerClassTypeInstanceName = instanceName;
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
								currentGroupInInputLayerSectionTokensLayerClassTypeInstance->neuronReference->GIAentityName = instanceName;
								#endif
								createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClassTypeInstance, currentGroupInInputLayerSectionTokensLayerClassType);
								addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClassTypeInstance, &numberOfGroupsInSectionTokensLayerClassTypeInstance);
							}
							
							currentTagInType = currentTagInType->nextTag;
						}
							
						currentGroupInInputLayerSectionTokensLayerClassType->GIAtokenLayerClassTypeName = classTypeName;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
						currentGroupInInputLayerSectionTokensLayerClassType->neuronReference->GIAentityName = classTypeName;
						#endif
						//cout << "classTypeName = " << classTypeName << endl;
						createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClassType, currentGroupInInputLayerSectionTokensLayerClass);
						addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClassType, &numberOfGroupsInSectionTokensLayerClassType);
					}
					
					currentTagInClass = currentTagInClass->nextTag;
				}
				
				currentGroupInInputLayerSectionTokensLayerClass->GIAtokenLayerClassName = className;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
				currentGroupInInputLayerSectionTokensLayerClass->neuronReference->GIAentityName = className;
				#endif
				//cout << "className = " << className << endl;
				createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClass, *currentGroupInInputLayerSectionTokensLayer);
				addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClass, &numberOfGroupsInSectionTokensLayerClass);
			}

			currentXMLtagInLayer = currentXMLtagInLayer->nextTag;
		}
		
		(*currentGroupInInputLayerSectionTokensLayer)->GIAtokenLayerName = layerName;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
		(*currentGroupInInputLayerSectionTokensLayer)->neuronReference->GIAentityName = layerName;
		#endif
		//cout << "layerName = " << layerName << endl;
		inputLayerSectionTokensLayerMap.insert(pair<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>(layerName, *currentGroupInInputLayerSectionTokensLayer));
		addGroupToLayer(currentGroupInInputLayerSectionTokensLayer, numberOfGroupsInSectionTokensLayer);
	}
	
	return classTagFound;
}



bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerIntro(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);

		//parse from highest level groups first 
		//high level groupType names should be synced with GIAtxtRelTranslatorClass::generateParseTreeIntro
		bool passGroupTests = false;
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* topLevelGroup = NULL;
		
		//bool passGroupTests = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(groupType->groupTypeName, groupType->referenceSetType, isQuery, parseIsolatedSubreferenceSets);	
		if(groupType->referenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE)
		{
			if(groupType->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionStatements != NULL)
				{
					cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionStatements != NULL): more than one (groupType->referenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionStatements = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionStatements;
			}
			else if(groupType->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionQuestions != NULL)
				{
					cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionQuestions != NULL): more than one (groupType->referenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionQuestions = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionQuestions;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			else if(groupType->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionSubjects != NULL)
				{
					cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionSubjects != NULL): more than one (groupType->referenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionSubjects = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionSubjects;
			}
			#endif
		}
		
		if(passGroupTests)
		{
			//top level group found
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
			ANNneuron* currentGroupNeuronInLayer = NULL;
			ANNneuron* previousGroupNeuronInLayer = NULL;
			#endif	
			for(int j=0; j<(groupType->groups).size(); j++)
			{
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
				
				//createGroupANNconnectionIO(group, topLevelGroup);
				createGroupANNconnectionIObasic(group, topLevelGroup);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
				currentGroupNeuronInLayer = group->neuronReference;
				if(j != 0)
				{
					previousGroupNeuronInLayer->nextNeuron = currentGroupNeuronInLayer;
				}
				#endif
				
				if(!createNeuronLayerGroup(GIAtxtRelTranslatorRulesTokenLayers, group))
				{
					result = false;
				}
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
				previousGroupNeuronInLayer = currentGroupNeuronInLayer;
				#endif
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerGroupType(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent, GIAtxtRelTranslatorRulesGroupType* groupType)
{
	bool result = true;
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
	ANNneuron* currentGroupNeuronInLayer = NULL;
	ANNneuron* previousGroupNeuronInLayer = NULL;
	#endif	
	*/
	for(int j=0; j<(groupType->groups).size(); j++)
	{
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
		
		createGroupANNconnection(group, higherLevelComponent);
		/*
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
		currentGroupNeuronInLayer = group->neuronReference;
		if(j != 0)
		{
			previousGroupNeuronInLayer->nextNeuron = currentGroupNeuronInLayer;
		}
		#endif
		*/
		
		if(!createNeuronLayerGroup(GIAtxtRelTranslatorRulesTokenLayers, group))
		{
			result = false;
		}
		
		/*
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
		previousGroupNeuronInLayer = currentGroupNeuronInLayer;
		#endif
		*/
	}
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerGroup(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool result = true;
	
	if(!group->neuronGenerated)	//prevent circular loops
	{
		group->neuronGenerated = true;
		if(!createNeuronLayerComponents(GIAtxtRelTranslatorRulesTokenLayers, group, &(group->components), false, NULL))
		{
			result = false;
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent)
{
	bool result = true;
	
	for(int j=0; j<components->size(); j++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (*components)[j];
		
		//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		//forwardNounVerbVariantRequirementsGroupToComponent(currentParseTreeGroup, newParseComponent);
		//#endif
		
		if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* stringGroup = NULL;
			if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
			{
				string wordPOStypeName = component->wordPOStype;
				int wordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(wordPOStypeName);
				
				stringGroup = getInputGroupLayerSection(firstGroupInInputLayerSectionWordPOStype, wordPOStype);
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
				cout << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerGroupType{}: getInputGroupLayerSection): wordPOStypeName = " << wordPOStypeName << endl;
				#endif
			}
			else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
			{
				string word = component->word;
				unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>::iterator iter = inputLayerSectionExplicitWordMap.find(word);
				if(iter != inputLayerSectionExplicitWordMap.end())
				{
					stringGroup = iter->second;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
					cout << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerGroupType{}: (iter != inputLayerSectionExplicitWordMap.end()): word = " << word << endl;
					#endif	
				}
				else
				{
					cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents{} error: cannot find word in inputLayerSectionExplicitWordMap: word = " << word << endl;
					exit(EXIT_ERROR);
				}
			}
			else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
			{
				if(findTokensLayerClassType(component->tokenLayer, component->tokenClass, component->tokenType, &stringGroup))
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
					cout << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerGroupType{}: findTokensLayerClassType: component->tokenClass = " << component->tokenClass << ", component->tokenType = " << component->tokenType << endl;
					#endif				
				}
				else
				{
					cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents{} error: !findTokensLayerClassType(): component->tokenClass = " << component->tokenClass << ", component->tokenType = " << component->tokenType << endl;
					exit(EXIT_ERROR);
				}
			}
			
			createGroupANNconnection(stringGroup, component);
		}
		else if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
		{
			if(component->groupRef != NULL)
			{
				createGroupANNconnection(component->groupRef, component);
				
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA8
				cout << "createGroupANNconnection: component->groupRef->groupName = " << component->groupRef->groupName << ", higher level component groupOwner group->groupName = " << group->groupName << endl;
				#endif

				//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				//forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				//#endif
				
				if(createNeuronLayerGroup(GIAtxtRelTranslatorRulesTokenLayers, component->groupRef))
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
					cout << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents{}: createNeuronLayerGroup" << endl;
					#endif
					//foundWordMatch = true;
				}
			}
			else if(component->groupTypeRef != NULL)
			{
				//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				//forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				//#endif
				
				//cout << "group->groupName = " << group->groupName << endl;
				//cout << "\tcomponent->groupTypeRef = " << component->groupTypeRef->groupTypeName << endl;
				
				if(createNeuronLayerGroupType(GIAtxtRelTranslatorRulesTokenLayers, component, component->groupTypeRef))
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_CREATE
					cout << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents{}: createNeuronLayerGroupType" << endl;
					#endif
					//foundWordMatch = true;
				}
			}
			else
			{
				cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents{} error: (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && (component->groupTypeRef/groupRef == NULL)" << endl;
				exit(EXIT_ERROR);
			}
			
		}
		else if((component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
		{
			if(subcomponents)
			{
				result = false;
				cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents{} error: (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR/REPEAT) && subcomponents" << endl;
				exit(EXIT_ERROR);
			}
			if(!createNeuronLayerComponents(GIAtxtRelTranslatorRulesTokenLayers, group, &(component->subComponents), true, component))	//CHECKTHIS (NB repeat/optional subcomponents are currently added directly to the groupNeuron like the other neurons)
			{
				result = false;
				cerr << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createNeuronLayerComponents{} error: !connectComponentsReferences()" << endl;
				exit(EXIT_ERROR);		
			}
		}
	}
	
	return result;
}





bool GIAtxtRelTranslatorNeuralNetworkFormationClass::findTokensLayerClassType(string layerName, string layerClassName, string layerClassTypeName, GIAtxtRelTranslatorRulesGroupNeuralNetwork** groupFound)
{	
	bool result = false;
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* layerClassGroupFound = NULL;
	if(findTokensLayerClass(layerName, layerClassName, &layerClassGroupFound))
	{
		if(layerClassTypeName != "")
		{
			for(int i=0; i<layerClassGroupFound->ANNbackGroupConnectionList.size(); i++)
			{
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* layerClassTypeGroup = ((layerClassGroupFound->ANNbackGroupConnectionList)[i]);
				if(layerClassTypeGroup->GIAtokenLayerClassTypeName == layerClassTypeName)
				{
					result = true;
					*groupFound = layerClassTypeGroup;
				}
			}
		}
		else
		{
			*groupFound = layerClassGroupFound;
		}		
	}
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::findTokensLayerClass(string layerName, string layerClassName, GIAtxtRelTranslatorRulesGroupNeuralNetwork** layerClassGroupFound)
{
	bool result = false;	
	for(unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>::iterator iter = inputLayerSectionTokensLayerMap.begin(); iter != inputLayerSectionTokensLayerMap.end(); iter++)
	{
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* layerGroup = iter->second;
		//cout << "layerGroup->GIAtokenLayerName = " << layerGroup->GIAtokenLayerName << endl;
		//cout << "layerName = " << layerName << endl;
		if(layerGroup->GIAtokenLayerName == layerName)
		{
			for(int i=0; i<layerGroup->ANNbackGroupConnectionList.size(); i++)
			{
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* layerClassGroup = ((layerGroup->ANNbackGroupConnectionList)[i]);
				//cout << "layerClassGroup->GIAtokenLayerClassName = " << layerClassGroup->GIAtokenLayerClassName << endl;
				//cout << "layerClassName = " << layerClassName << endl;
				if(layerClassGroup->GIAtokenLayerClassName == layerClassName)
				{
					result = true;
					*layerClassGroupFound = layerClassGroup;
				}
			}
		}
	}
	return result;
}


			
bool GIAtxtRelTranslatorNeuralNetworkFormationClass::findWordInList(const string word, vector<string>* explicitWordList)
{
	bool result = false;
	vector<string>::iterator it = find(explicitWordList->begin(), explicitWordList->end(), word);
	if(it != explicitWordList->end())
	{
		result = true;
	}
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::findWordInGroupMap(const string word, unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* wordMap, GIAtxtRelTranslatorRulesGroupNeuralNetwork** groupFound)
{
	bool result = false;
	unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>::iterator it = wordMap->find(word);
	if(it != wordMap->end())
	{
		result = true;
		*groupFound = it->second;
	}
	return result;
}


				
GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkFormationClass::getInputGroupLayerSection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSection, int groupIndexInSection)
{
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSection = firstGroupInInputLayerSection;
	for(int i=0; i<groupIndexInSection; i++)
	{
		currentGroupInInputLayerSection = currentGroupInInputLayerSection->next;
	}
	return currentGroupInInputLayerSection;
}



void GIAtxtRelTranslatorNeuralNetworkFormationClass::addGroupToLayer(GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInLayer, int* numberOfGroupsInSection)
{
	(*currentGroupInLayer)->next = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
	*currentGroupInLayer = (*currentGroupInLayer)->next;
	*numberOfGroupsInSection = *numberOfGroupsInSection + 1;
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createGroupANNconnectionIO(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup)
{
	//this creates an artificial group connection for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK only (not used by GIAtxtRelTranslatorParser)
	group->ANNfrontGroupConnectionList.push_back(higherLevelGroup);
	higherLevelGroup->ANNbackGroupConnectionList.push_back(group);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	createANNconnection(group, higherLevelGroup);
	#endif
}

bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createGroupANNconnectionIObasic(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup)
{
	//this creates an artificial group connection for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK only (not used by GIAtxtRelTranslatorParser)
	group->ANNfrontGroupConnectionList.push_back(higherLevelGroup);
	higherLevelGroup->ANNbackGroupConnectionList.push_back(group);
}



bool GIAtxtRelTranslatorNeuralNetworkFormationClass::createGroupANNconnection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent)
{
	//cout << "GIAtxtRelTranslatorNeuralNetworkFormationClass::createGroupANNconnection" << endl;
	
	group->ANNfrontComponentConnectionList.push_back(higherLevelComponent);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	createANNconnection(group, higherLevelComponent);
	#endif
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
ANNneuronConnection* GIAtxtRelTranslatorNeuralNetworkFormationClass::createANNconnection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup)
{
	ANNneuron* neuron1 = group->neuronReference;
	ANNneuron* neuron2 = higherLevelGroup->neuronReference;
	
	ANNneuronConnection* newANNneuronConnection = new ANNneuronConnection();
	newANNneuronConnection->backNeuron = neuron1;
	newANNneuronConnection->frontNeuron = neuron2;
	(neuron1->frontANNneuronConnectionList).push_back(newANNneuronConnection);
	(neuron2->backANNneuronConnectionList).push_back(newANNneuronConnection);
	
	return newANNneuronConnection;
}

ANNneuronConnection* GIAtxtRelTranslatorNeuralNetworkFormationClass::createANNconnection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent)
{
	vector<ANNneuronConnection*>* ANNbackNeuronConnectionList = &(higherLevelComponent->ANNbackNeuronConnectionList);
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup = higherLevelComponent->ownerGroup;

	ANNneuronConnection* newANNneuronConnection = createANNconnection(group, higherLevelGroup);
	
	ANNbackNeuronConnectionList->push_back(newANNneuronConnection);
	
	return newANNneuronConnection;
}
#endif


#endif
