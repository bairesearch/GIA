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
 * File Name: GIAposRelTranslatorSANIFormation.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k14d 12-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Formation
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorSANIFormation.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_SANI


GIAposRelTranslatorRulesGroupNeuralNetwork* firstInputGroupInNetwork;
GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionWordPOStype;
#ifdef GIA_POS_REL_TRANSLATOR_SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionWordNounVariantType;
GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionWordVerbVariantType;
#endif
GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionExplicitWord;
GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayer;

int numberOfInputGroupsInSectionWordPOStype;
#ifdef GIA_POS_REL_TRANSLATOR_SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
int numberOfInputGroupsInSectionNounVariantType;
int numberOfInputGroupsInSectionVerbVariantType;
#endif
int numberOfInputGroupsInSectionExplicitWord;
int numberOfInputGroupsInSectionTokensLayer;
unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*> inputLayerSectionExplicitWordMap;
unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*> inputLayerSectionTokensLayerMap;
#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
vector<string> explicitWordListLocal;
#endif

//high level groupType names should be synced with GIAposRelTranslatorClass::generateParseTreeIntro
GIAposRelTranslatorRulesGroupNeuralNetwork* topLevelGroupInOuputLayerSectionStatements;
GIAposRelTranslatorRulesGroupNeuralNetwork* topLevelGroupInOuputLayerSectionQuestions;
GIAposRelTranslatorRulesGroupNeuralNetwork* topLevelGroupInOuputLayerSectionSubjects;

#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
vector<string>* GIAposRelTranslatorSANIFormationClass::getExplicitWordList()
{
	return &explicitWordListLocal;
}
#endif

unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>* GIAposRelTranslatorSANIFormationClass::getInputLayerSectionExplicitWordMap()
{
	return &inputLayerSectionExplicitWordMap;
}

unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>* GIAposRelTranslatorSANIFormationClass::getInputLayerSectionTokensLayerMap()
{
	return &inputLayerSectionTokensLayerMap;
}

GIAposRelTranslatorRulesGroupNeuralNetwork* GIAposRelTranslatorSANIFormationClass::getFirstGroupInInputLayerSectionWordPOStype()
{
	return firstGroupInInputLayerSectionWordPOStype;
}

#ifdef GIA_NEURAL_NETWORK
GIAposRelTranslatorRulesGroupNeuralNetwork* GIAposRelTranslatorSANIFormationClass::getFirstInputGroupInNetwork()
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



bool GIAposRelTranslatorSANIFormationClass::createGIAposRelTranslatorSANI(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	createInputNeuronLayer(GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupTypes);
	
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY		
	createNeuronLayerIntro(GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupTypes);
	#endif
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
	cout << "exiting normally" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}

bool GIAposRelTranslatorSANIFormationClass::createInputNeuronLayer(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	/*
	create inputLayer - define all neurons for all;
		wordPOStype
			wordNounVariantType
			wordVerbVariantType
		explicit words in GIAposRelTranslatorRules.xml
		tokens-layer in GIAposRelTranslatorRules.xml
			classes
				types
	*/

	numberOfInputGroupsInSectionWordPOStype = 0;
	numberOfInputGroupsInSectionExplicitWord = 0;
	numberOfInputGroupsInSectionTokensLayer = 0;
	
	GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSection = NULL;
	firstGroupInInputLayerSectionWordPOStype = new GIAposRelTranslatorRulesGroupNeuralNetwork();
	firstInputGroupInNetwork = firstGroupInInputLayerSectionWordPOStype;
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionWordPOStype;
	if(!createInputNeuronLayerSectionWordPOStype(&currentGroupInInputLayerSection, &numberOfInputGroupsInSectionWordPOStype))
	{
		result = false;
	}
	
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	firstGroupInInputLayerSectionExplicitWord = new GIAposRelTranslatorRulesGroupNeuralNetwork();
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionExplicitWord;
	if(!createInputNeuronLayerSectionExplicitWord(&currentGroupInInputLayerSection, &numberOfInputGroupsInSectionExplicitWord, GIAposRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	
	firstGroupInInputLayerSectionTokensLayer = new GIAposRelTranslatorRulesGroupNeuralNetwork();
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionTokensLayer;
	if(!createInputNeuronLayerSectionTokensLayer(GIAposRelTranslatorRulesTokenLayers, &currentGroupInInputLayerSection, &numberOfInputGroupsInSectionTokensLayer))
	{
		result = false;
	}
	#endif

	return result;
}


bool GIAposRelTranslatorSANIFormationClass::createInputNeuronLayerSectionWordPOStype(GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordPOStype, int* numberOfGroupsInSection)
{
	bool result = true;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
	numberOfInputGroupsInSectionNounVariantType = 0;
	numberOfInputGroupsInSectionVerbVariantType = 0;
	#endif
	
	for(int i=0; i<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
		if(i == GIA_PREPROCESSOR_POS_TYPE_NOUN)
		{
			firstGroupInInputLayerSectionWordNounVariantType = new GIAposRelTranslatorRulesGroupNeuralNetwork();
			if(!createInputNeuronLayerSectionNounVariantType(*currentGroupInInputLayerSectionWordPOStype, &firstGroupInInputLayerSectionWordNounVariantType, &numberOfInputGroupsInSectionNounVariantType))
			{
				result = false;
			}
		}
		if(i == GIA_PREPROCESSOR_POS_TYPE_VERB)
		{
			firstGroupInInputLayerSectionWordVerbVariantType = new GIAposRelTranslatorRulesGroupNeuralNetwork();
			if(!createInputNeuronLayerSectionVerbVariantType(*currentGroupInInputLayerSectionWordPOStype, &firstGroupInInputLayerSectionWordVerbVariantType, &numberOfInputGroupsInSectionVerbVariantType))
			{
				result = false;
			}
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
		(*currentGroupInInputLayerSectionWordPOStype)->neuronReference->GIAentityName = GIApreprocessorPOStypeNameArray[i];
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
		(*currentGroupInInputLayerSectionWordPOStype)->groupTypeIsString = true;
		#endif
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_ASSIGN_GROUP_INDICES_TO_INPUT_NEURONS
		int newNeuronIndex = GIAposRelTranslatorRules.assignGroupIndex(*currentGroupInInputLayerSectionWordPOStype);	
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
		(*currentGroupInInputLayerSectionWordPOStype)->initiateANNneuron("groupIndex:" + SHAREDvars.convertIntToString(newNeuronIndex));
		#endif
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordPOStype, numberOfGroupsInSection);
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
		cout << "GIAposRelTranslatorSANIFormationClass::createInputNeuronLayerSectionWordPOStype{} addGroupToLayer(currentGroupInInputLayerSectionWordPOStype, numberOfGroupsInSection);  i = " << i << endl;
		#endif
	}
}

#ifdef GIA_POS_REL_TRANSLATOR_SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
bool GIAposRelTranslatorSANIFormationClass::createInputNeuronLayerSectionNounVariantType(GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordNounVariantType, int* numberOfGroupsInSection)
{	
	bool result = true;
	
	for(int i=0; i<GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES; i++)
	{
		createGroupANNconnectionIO(currentGroupInInputLayerSectionWordPOStype, *currentGroupInInputLayerSectionWordNounVariantType);	//switched GIA3h3a
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
		(*currentGroupInInputLayerSectionWordNounVariantType)->neuronReference->GIAentityName = GIApreprocessorMultiwordReductionNounDatabaseTagBaseTenseFormArray[i];
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordNounVariantType, numberOfGroupsInSection);		
	}
	
	return result;
}

bool GIAposRelTranslatorSANIFormationClass::createInputNeuronLayerSectionVerbVariantType(GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordVerbVariantType, int* numberOfGroupsInSection)
{
	bool result = true;
	
	for(int i=0; i<GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES; i++)
	{
		createGroupANNconnectionIO(currentGroupInInputLayerSectionWordPOStype, *currentGroupInInputLayerSectionWordVerbVariantType);	//switched GIA3h3a
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
		(*currentGroupInInputLayerSectionWordVerbVariantType)->neuronReference->GIAentityName = GIApreprocessorMultiwordReductionVerbDatabaseTagBaseTenseFormArray[i];
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordVerbVariantType, numberOfGroupsInSection);		
	}
	
	return result;
}
#endif


bool GIAposRelTranslatorSANIFormationClass::createInputNeuronLayerSectionExplicitWord(GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSection, int* numberOfGroupsInSection, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	inputLayerSectionExplicitWordMap.clear();	//added GIA3g11aTEMP62
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	explicitWordListLocal.clear();
	#endif
	vector<string> explicitWordList;
	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
			if(!createInputNeuronLayerSectionExplicitWord(&(group->components), false, &explicitWordList))
			{
				result = false;
			}
		}
	}
	for(int i=0; i<explicitWordList.size(); i++)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
		(*currentGroupInInputLayerSection)->neuronReference->GIAentityName = explicitWordList[i];
		#endif
		inputLayerSectionExplicitWordMap.insert(pair<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>(explicitWordList[i], *currentGroupInInputLayerSection));
		addGroupToLayer(currentGroupInInputLayerSection, numberOfGroupsInSection);
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
	explicitWordListLocal = explicitWordList;
	#endif
	#endif
	
	
	return result;
}
bool GIAposRelTranslatorSANIFormationClass::createInputNeuronLayerSectionExplicitWord(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, vector<string>* explicitWordList)
{
	bool result = true;
	
	for(int j=0; j<components->size(); j++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (*components)[j];
		
		if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
			{
				string word = component->word;
				if(!findWordInList(word, explicitWordList))
				{
					explicitWordList->push_back(word);
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
					cout << "GIAposRelTranslatorRulesClass::connectComponentsReferences{} explicitWordList->push_back(word) = " << word << endl;
					#endif
					
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
					if(component->stringTypeExplicitAddToExplicitWordTempPOS)
					{
						explicitWordListLocal.push_back(word);
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
						cout << "GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT: explicitWordListLocal.push_back(word) = " << word << endl;
						#endif 
					}
					#endif
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
			if(!createInputNeuronLayerSectionExplicitWord(&(component->subComponents), true, explicitWordList))
			{
				result = false;
				cerr << "GIAposRelTranslatorRulesClass::connectComponentsReferences{} error: !connectComponentsReferences()" << endl;
				exit(EXIT_ERROR);			
			}
		}
	}
	
	return result;
}


bool GIAposRelTranslatorSANIFormationClass::createInputNeuronLayerSectionTokensLayer(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionTokensLayer, int* numberOfGroupsInSectionTokensLayer)
{
	bool classTagFound = false;
	bool result = false;
	
	inputLayerSectionTokensLayerMap.clear();	//added GIA3g11aTEMP62
	
	for(int i=0; i<GIAposRelTranslatorRulesTokenLayers->size(); i++)
	{	
		XMLparserTag* currentLayerTag = GIAposRelTranslatorRulesTokenLayers->at(i);

		//cout << "i = " << i << endl;
		//cout << "currentLayerTag->name = " << currentLayerTag->name << endl;	//only the first 3 xml rules <tokens> <layer> layers detected

		string layerName = "";
		if(!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName))
		{
			cerr << "GIAposRelTranslatorHybridClass::executeTxtRelTranslatorDepreciated{} error: (!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName)" << endl;
			exit(EXIT_ERROR);
		}
		
		XMLparserTag* firstXMLtagInLayer = XMLparserClass.parseTagDownALevel(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer, &result);
		XMLparserTag* currentXMLtagInLayer = firstXMLtagInLayer;

		GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClass = new GIAposRelTranslatorRulesGroupNeuralNetwork();
		GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClass = firstGroupInInputLayerSectionTokensLayerClass;
		int numberOfGroupsInSectionTokensLayerClass = 0; 
		
		while(currentXMLtagInLayer->nextTag != NULL)
		{
			if(currentXMLtagInLayer->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class)
			{	
				bool sameClass = false;
				string className = "";
				//vector<string>* logicReferenceClassVector = NULL;
				if(currentXMLtagInLayer->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
				{
					className = currentXMLtagInLayer->firstAttribute->value;
				}
				
				XMLparserTag* firstTagInClass = XMLparserClass.parseTagDownALevel(currentXMLtagInLayer, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class, &result);
				XMLparserTag* currentTagInClass = firstTagInClass;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				//cout << "classNameToFind = " << classNameToFind << endl;
				#endif
				
				GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClassType = new GIAposRelTranslatorRulesGroupNeuralNetwork();
				GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassType = firstGroupInInputLayerSectionTokensLayerClassType;
				int numberOfGroupsInSectionTokensLayerClassType = 0; 
		
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
						}

						XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type, &result);
						XMLparserTag* currentTagInType = firstTagInTypeTag;
						
						GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClassTypeInstance = new GIAposRelTranslatorRulesGroupNeuralNetwork();
						GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = firstGroupInInputLayerSectionTokensLayerClassTypeInstance;
						int numberOfGroupsInSectionTokensLayerClassTypeInstance = 0; 
				
						while(currentTagInType->nextTag != NULL)
						{
							if(currentTagInType->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance)
							{
								#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
								//cout << "\t\tGIAposRelTranslatorRules::isClassTag{} debug: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance" << endl;
								#endif
								string instanceName = "";
								if(currentTagInType->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
								{
									instanceName = currentTagInType->firstAttribute->value;
									#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
									cout << "GIAposRelTranslatorRulesClass::createInputNeuronLayerSectionTokensLayer{} instanceName = " << instanceName << endl;
									#endif
								}
								
								currentGroupInInputLayerSectionTokensLayerClassTypeInstance->GIAtokenLayerClassTypeInstanceName = instanceName;
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
								currentGroupInInputLayerSectionTokensLayerClassTypeInstance->neuronReference->GIAentityName = instanceName;
								#endif
								createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClassTypeInstance, currentGroupInInputLayerSectionTokensLayerClassType);
								addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClassTypeInstance, &numberOfGroupsInSectionTokensLayerClassTypeInstance);
							}
							
							currentTagInType = currentTagInType->nextTag;
						}
							
						currentGroupInInputLayerSectionTokensLayerClassType->GIAtokenLayerClassTypeName = classTypeName;
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
						currentGroupInInputLayerSectionTokensLayerClassType->neuronReference->GIAentityName = classTypeName;
						#endif
						//cout << "classTypeName = " << classTypeName << endl;
						createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClassType, currentGroupInInputLayerSectionTokensLayerClass);
						addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClassType, &numberOfGroupsInSectionTokensLayerClassType);
					}
					
					currentTagInClass = currentTagInClass->nextTag;
				}
				
				currentGroupInInputLayerSectionTokensLayerClass->GIAtokenLayerClassName = className;
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
				currentGroupInInputLayerSectionTokensLayerClass->neuronReference->GIAentityName = className;
				#endif
				//cout << "className = " << className << endl;
				createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClass, *currentGroupInInputLayerSectionTokensLayer);
				addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClass, &numberOfGroupsInSectionTokensLayerClass);
			}

			currentXMLtagInLayer = currentXMLtagInLayer->nextTag;
		}
		
		(*currentGroupInInputLayerSectionTokensLayer)->GIAtokenLayerName = layerName;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
		(*currentGroupInInputLayerSectionTokensLayer)->neuronReference->GIAentityName = layerName;
		#endif
		//cout << "layerName = " << layerName << endl;
		inputLayerSectionTokensLayerMap.insert(pair<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>(layerName, *currentGroupInInputLayerSectionTokensLayer));
		addGroupToLayer(currentGroupInInputLayerSectionTokensLayer, numberOfGroupsInSectionTokensLayer);
	}
	
	return classTagFound;
}



bool GIAposRelTranslatorSANIFormationClass::createNeuronLayerIntro(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);

		//parse from highest level groups first 
		//high level groupType names should be synced with GIAposRelTranslatorClass::generateParseTreeIntro
		bool passGroupTests = false;
		GIAposRelTranslatorRulesGroupNeuralNetwork* topLevelGroup = NULL;
		
		//bool passGroupTests = GIAposRelTranslatorRulesGroupClassObject.isTopLevelGroupType(groupType->groupTypeName, groupType->referenceSetType, isQuery, parseIsolatedSubreferenceSets);	
		if(groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE)
		{
			if(groupType->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionStatements != NULL)
				{
					cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionStatements != NULL): more than one (groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionStatements = new GIAposRelTranslatorRulesGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionStatements;
			}
			else if(groupType->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionQuestions != NULL)
				{
					cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionQuestions != NULL): more than one (groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionQuestions = new GIAposRelTranslatorRulesGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionQuestions;
			}
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			else if(groupType->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionSubjects != NULL)
				{
					cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionSubjects != NULL): more than one (groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionSubjects = new GIAposRelTranslatorRulesGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionSubjects;
			}
			#endif
		}
		
		if(passGroupTests)
		{
			//top level group found
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
			ANNneuron* currentGroupNeuronInLayer = NULL;
			ANNneuron* previousGroupNeuronInLayer = NULL;
			#endif	
			for(int j=0; j<(groupType->groups).size(); j++)
			{
				GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
				
				//createGroupANNconnectionIO(group, topLevelGroup);
				createGroupANNconnectionIObasic(group, topLevelGroup);
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
				currentGroupNeuronInLayer = group->neuronReference;
				if(j != 0)
				{
					previousGroupNeuronInLayer->nextNeuron = currentGroupNeuronInLayer;
				}
				#endif
				
				if(!createNeuronLayerGroup(GIAposRelTranslatorRulesTokenLayers, group))
				{
					result = false;
				}
				
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
				previousGroupNeuronInLayer = currentGroupNeuronInLayer;
				#endif
			}
		}
	}
	
	return result;
}

bool GIAposRelTranslatorSANIFormationClass::createNeuronLayerGroupType(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent, GIAposRelTranslatorRulesGroupType* groupType)
{
	bool result = true;
	
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
	ANNneuron* currentGroupNeuronInLayer = NULL;
	ANNneuron* previousGroupNeuronInLayer = NULL;
	#endif	
	*/
	for(int j=0; j<(groupType->groups).size(); j++)
	{
		GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
		
		createGroupANNconnection(group, higherLevelComponent);
		/*
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
		currentGroupNeuronInLayer = group->neuronReference;
		if(j != 0)
		{
			previousGroupNeuronInLayer->nextNeuron = currentGroupNeuronInLayer;
		}
		#endif
		*/
		
		if(!createNeuronLayerGroup(GIAposRelTranslatorRulesTokenLayers, group))
		{
			result = false;
		}
		
		/*
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
		previousGroupNeuronInLayer = currentGroupNeuronInLayer;
		#endif
		*/
	}
	return result;
}


bool GIAposRelTranslatorSANIFormationClass::createNeuronLayerGroup(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool result = true;
	
	if(!group->neuronGenerated)	//prevent circular loops
	{
		group->neuronGenerated = true;
		if(!createNeuronLayerComponents(GIAposRelTranslatorRulesTokenLayers, group, &(group->components), false, NULL))
		{
			result = false;
		}
	}
	
	return result;
}

bool GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupNeuralNetwork* group, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent)
{
	bool result = true;
	
	for(int j=0; j<components->size(); j++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (*components)[j];
		
		//#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		//forwardNounVerbVariantRequirementsGroupToComponent(currentParseTreeGroup, newParseComponent);
		//#endif
		
		if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* stringGroup = NULL;
			if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
			{
				string wordPOStypeName = component->wordPOStype;
				int wordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(wordPOStypeName);
				
				stringGroup = getInputGroupLayerSection(firstGroupInInputLayerSectionWordPOStype, wordPOStype);
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
				cout << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerGroupType{}: getInputGroupLayerSection): wordPOStypeName = " << wordPOStypeName << endl;
				#endif
			}
			else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
			{
				string word = component->word;
				unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>::iterator iter = inputLayerSectionExplicitWordMap.find(word);
				if(iter != inputLayerSectionExplicitWordMap.end())
				{
					stringGroup = iter->second;
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
					cout << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerGroupType{}: (iter != inputLayerSectionExplicitWordMap.end()): word = " << word << endl;
					#endif	
				}
				else
				{
					cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents{} error: cannot find word in inputLayerSectionExplicitWordMap: word = " << word << endl;
					exit(EXIT_ERROR);
				}
			}
			else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
			{
				if(findTokensLayerClassType(component->tokenLayer, component->tokenClass, component->tokenType, &stringGroup))
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
					cout << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerGroupType{}: findTokensLayerClassType: component->tokenClass = " << component->tokenClass << ", component->tokenType = " << component->tokenType << endl;
					#endif				
				}
				else
				{
					cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents{} error: !findTokensLayerClassType(): component->tokenClass = " << component->tokenClass << ", component->tokenType = " << component->tokenType << endl;
					exit(EXIT_ERROR);
				}
			}
			
			createGroupANNconnection(stringGroup, component);
		}
		else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
		{
			if(component->groupRef != NULL)
			{
				createGroupANNconnection(component->groupRef, component);
				
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
				cout << "createGroupANNconnection: component->groupRef->groupName = " << component->groupRef->groupName << ", higher level component groupOwner group->groupName = " << group->groupName << endl;
				#endif

				//#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				//forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				//#endif
				
				if(createNeuronLayerGroup(GIAposRelTranslatorRulesTokenLayers, component->groupRef))
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
					cout << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents{}: createNeuronLayerGroup" << endl;
					#endif
					//foundWordMatch = true;
				}
			}
			else if(component->groupTypeRef != NULL)
			{
				//#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				//forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				//#endif
				
				//cout << "group->groupName = " << group->groupName << endl;
				//cout << "\tcomponent->groupTypeRef = " << component->groupTypeRef->groupTypeName << endl;
				
				if(createNeuronLayerGroupType(GIAposRelTranslatorRulesTokenLayers, component, component->groupTypeRef))
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_CREATE
					cout << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents{}: createNeuronLayerGroupType" << endl;
					#endif
					//foundWordMatch = true;
				}
			}
			else
			{
				cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents{} error: (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && (component->groupTypeRef/groupRef == NULL)" << endl;
				exit(EXIT_ERROR);
			}
			
		}
		else if((component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
		{
			if(subcomponents)
			{
				result = false;
				cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents{} error: (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR/REPEAT) && subcomponents" << endl;
				exit(EXIT_ERROR);
			}
			if(!createNeuronLayerComponents(GIAposRelTranslatorRulesTokenLayers, group, &(component->subComponents), true, component))	//CHECKTHIS (NB repeat/optional subcomponents are currently added directly to the groupNeuron like the other neurons)
			{
				result = false;
				cerr << "GIAposRelTranslatorSANIFormationClass::createNeuronLayerComponents{} error: !connectComponentsReferences()" << endl;
				exit(EXIT_ERROR);		
			}
		}
	}
	
	return result;
}





bool GIAposRelTranslatorSANIFormationClass::findTokensLayerClassType(string layerName, string layerClassName, string layerClassTypeName, GIAposRelTranslatorRulesGroupNeuralNetwork** groupFound)
{	
	bool result = false;
	GIAposRelTranslatorRulesGroupNeuralNetwork* layerClassGroupFound = NULL;
	if(findTokensLayerClass(layerName, layerClassName, &layerClassGroupFound))
	{
		if(layerClassTypeName != "")
		{
			for(int i=0; i<layerClassGroupFound->ANNbackGroupConnectionList.size(); i++)
			{
				GIAposRelTranslatorRulesGroupNeuralNetwork* layerClassTypeGroup = ((layerClassGroupFound->ANNbackGroupConnectionList)[i]);
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

bool GIAposRelTranslatorSANIFormationClass::findTokensLayerClass(string layerName, string layerClassName, GIAposRelTranslatorRulesGroupNeuralNetwork** layerClassGroupFound)
{
	bool result = false;	
	for(unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>::iterator iter = inputLayerSectionTokensLayerMap.begin(); iter != inputLayerSectionTokensLayerMap.end(); iter++)
	{
		GIAposRelTranslatorRulesGroupNeuralNetwork* layerGroup = iter->second;
		//cout << "layerGroup->GIAtokenLayerName = " << layerGroup->GIAtokenLayerName << endl;
		//cout << "layerName = " << layerName << endl;
		if(layerGroup->GIAtokenLayerName == layerName)
		{
			for(int i=0; i<layerGroup->ANNbackGroupConnectionList.size(); i++)
			{
				GIAposRelTranslatorRulesGroupNeuralNetwork* layerClassGroup = ((layerGroup->ANNbackGroupConnectionList)[i]);
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


			
bool GIAposRelTranslatorSANIFormationClass::findWordInList(const string word, vector<string>* explicitWordList)
{
	bool result = false;
	vector<string>::iterator it = find(explicitWordList->begin(), explicitWordList->end(), word);
	if(it != explicitWordList->end())
	{
		result = true;
	}
	return result;
}

bool GIAposRelTranslatorSANIFormationClass::findWordInGroupMap(const string word, unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>* wordMap, GIAposRelTranslatorRulesGroupNeuralNetwork** groupFound)
{
	bool result = false;
	unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>::iterator it = wordMap->find(word);
	if(it != wordMap->end())
	{
		result = true;
		*groupFound = it->second;
	}
	return result;
}


				
GIAposRelTranslatorRulesGroupNeuralNetwork* GIAposRelTranslatorSANIFormationClass::getInputGroupLayerSection(GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSection, int groupIndexInSection)
{
	GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSection = firstGroupInInputLayerSection;
	for(int i=0; i<groupIndexInSection; i++)
	{
		currentGroupInInputLayerSection = currentGroupInInputLayerSection->next;
	}
	return currentGroupInInputLayerSection;
}



void GIAposRelTranslatorSANIFormationClass::addGroupToLayer(GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInLayer, int* numberOfGroupsInSection)
{
	(*currentGroupInLayer)->next = new GIAposRelTranslatorRulesGroupNeuralNetwork();
	*currentGroupInLayer = (*currentGroupInLayer)->next;
	*numberOfGroupsInSection = *numberOfGroupsInSection + 1;
}

bool GIAposRelTranslatorSANIFormationClass::createGroupANNconnectionIO(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup)
{
	//this creates an artificial group connection for GIA_POS_REL_TRANSLATOR_SANI only (not used by GIAposRelTranslatorParser)
	group->ANNfrontGroupConnectionList.push_back(higherLevelGroup);
	higherLevelGroup->ANNbackGroupConnectionList.push_back(group);
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	createANNconnection(group, higherLevelGroup);
	#endif
}

bool GIAposRelTranslatorSANIFormationClass::createGroupANNconnectionIObasic(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup)
{
	//this creates an artificial group connection for GIA_POS_REL_TRANSLATOR_SANI only (not used by GIAposRelTranslatorParser)
	group->ANNfrontGroupConnectionList.push_back(higherLevelGroup);
	higherLevelGroup->ANNbackGroupConnectionList.push_back(group);
}



bool GIAposRelTranslatorSANIFormationClass::createGroupANNconnection(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent)
{
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_FORMATION
	cout << "GIAposRelTranslatorSANIFormationClass::createGroupANNconnection, group->groupIndex = " << group->groupIndex << ", higherLevelComponent->ownerGroup = " << higherLevelComponent->ownerGroup->groupName << endl;
	#endif
	
	group->ANNfrontComponentConnectionList.push_back(higherLevelComponent);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	higherLevelComponent->ANNbackGroupConnectionList.push_back(group);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	createANNconnection(group, higherLevelComponent);
	#endif
	#endif
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
ANNneuronConnection* GIAposRelTranslatorSANIFormationClass::createANNconnection(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent)
{
	vector<ANNneuronConnection*>* ANNbackNeuronConnectionList = &(higherLevelComponent->ANNbackNeuronConnectionList);
	GIAposRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup = higherLevelComponent->ownerGroup;

	ANNneuronConnection* newANNneuronConnection = createANNconnection(group, higherLevelGroup);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WEIGHTS
	newANNneuronConnection->GIAconnectionStrength = higherLevelComponent->componentStrength;
	#endif
				
	ANNbackNeuronConnectionList->push_back(newANNneuronConnection);
	
	return newANNneuronConnection;
}

ANNneuronConnection* GIAposRelTranslatorSANIFormationClass::createANNconnection(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup)
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


#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
bool GIAposRelTranslatorSANIFormationClass::createANNconnectivity(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[j];
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WEIGHTS
			group->neuronReference->GIAneuronStrength = group->groupStrength;
			#endif
			for(int k=0; k<group->components.size(); k++)
			{
				GIAposRelTranslatorRulesComponentNeuralNetwork* component = (group->components).at(k);
				for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
				{
					GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
					createANNconnection(groupSource, component);
				}
				/*
				GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource = component->groupRef;
				createANNconnection(groupSource, component);
				*/
			}
		}
	}

}
#endif

#endif


#endif
