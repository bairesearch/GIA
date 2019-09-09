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
 * File Name: GIAtxtRelTranslatorNeuralNetworkParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j2h 10-August-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Parser
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkParser.hpp"
#include "SHAREDvars.hpp"



//TODO: check this code

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSER

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
bool GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, int layer)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC
	forwardPropogationSignalData->parserForwardPropogationSignalData = GIAtxtRelTranslatorParserForwardPropogationSignalData();	//reinitialise parserForwardPropogationSignalData (clear all variables)
	#endif
	
	GIAtxtRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;
	if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, firstParseTreeGroup, &parserForwardPropogationSignalData, layer))
	{
		result = false;
	}

	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC
	forwardPropogationSignalData->parserForwardPropogationSignalData = parserForwardPropogationSignalData;	//store for next generateSemanticRelationsFromTxtRelationsWrapper execution
	#endif
		
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer)
{
	bool result = true;
	
	int numberOfComponentsInGroup = currentParseTreeGroup->components.size();
	vector<GIAtxtRelTranslatorParserForwardPropogationSignalData> parseTreeComponentSignalDataArray(numberOfComponentsInGroup);
	
	/*
	//code used if GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
	if(currentParseTreeGroup->neuronActive)
	{
	*/		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentParseTree* ownerComponent = (currentParseTreeGroup->components).at(i);
			/*
			//code used if GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
			if(ownerComponent->neuronComponentConnectionActive)
			{
			*/
				GIAtxtRelTranslatorRulesComponentParseTree* parseTreeComponent = ownerComponent;
				
				/*
				//code used if GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
				if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponent))
				{
					for(int i1=0; i1<ownerComponent->subComponents.size(); i1++)
					{
						GIAtxtRelTranslatorRulesComponent* subComponent = (ownerComponent->subComponents)[i1];
						if(subComponent->neuronComponentConnectionActive)
						{
							parseTreeComponent = subComponent;	//CHECKTHIS: what if the OR component has multiple activated subcomponents?
						}
					}
				}
				*/

				GIAtxtRelTranslatorParserForwardPropogationSignalData* parseTreeComponentSignalData = &(parseTreeComponentSignalDataArray[i]);

				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				GIAtxtRelTranslatorRules.printComponent(parseTreeComponent, layer);
				#endif

				if(parseTreeComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_BIO
					*parseTreeComponentSignalData = *(parseTreeComponent->parserForwardPropogationSignalData);
					#else
					if(parseTreeComponent->parseTreeGroupRef != NULL)
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC
						if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, parseTreeComponent->parseTreeGroupRef, parseTreeComponentSignalData, layer+1))
						{
							result = false;
						}
						#else
						*parseTreeComponentSignalData = (parseTreeComponent->parseTreeGroupRef->groupOrig->semanticRelationReturnEntityForwardPropogationSignalData).parserForwardPropogationSignalData;
						/*
						//note can't check parseTreeComponent->parseTreeGroupRef->neuronActive as it may have been reset by GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup; resetGroupActivation
							//note it will not reset semanticRelationReturnEntityForwardPropogationSignalData, but deactivate the group+components
						if(!parseTreeComponent->parseTreeGroupRef->neuronActive)
						{
							cerr << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (!parseTreeComponent->parseTreeGroupRef->neuronActive)" << endl;
							exit(EXIT_ERROR);
						}
						*/						
						#endif
					}
					else
					{
						cout << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: (parseTreeComponent->parseTreeGroupRef == NULL)" << endl;
						exit(EXIT_ERROR);
					}
					#endif
					
					if(parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)
					{
						cerr << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)" << endl;
						exit(EXIT_ERROR);
					}
				}
				else if(parseTreeComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					GIApreprocessorPlainTextWord* parseTreeComponentWord = parseTreeComponent->candidateStringMatch;
					parseTreeComponentSignalData->semanticRelationReturnEntity = parseTreeComponentWord->translatorEntity;
					//cout << "parseTreeComponentWord->tagName = " << parseTreeComponentWord->tagName << endl;
					//cout << "parseTreeComponentWord->translatorEntity->entityName = " << parseTreeComponentWord->translatorEntity->entityName << endl;
				}
				else
				{
					cerr << "GIAtxtRelTranslatorParserOperationsClass::generateRulesGroupTreeComponent{} error: parseTreeComponent->componentType illdefined" << endl;
					exit(EXIT_ERROR);
				}
			/*
			//code used if GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
			}
			else
			{
				bool optional = false;
				if(!(ownerComponent->optional))
				{
					cerr << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: !(ownerComponent->optional)" << endl;
					exit(EXIT_ERROR);
				}
			}
			*/
		}

		if(!GIAtxtRelTranslatorParserOperations.generateSemanticRelationsFromTxtRelationsPart2(translatorVariables, currentParseTreeGroup, &parseTreeComponentSignalDataArray, parserForwardPropogationSignalData, layer))
		{
			result = false;
		}

	/*
	}
	else
	{
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC
		cerr << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: !(currentParseTreeGroup->neuronActive)" << endl;
		exit(EXIT_ERROR);
		#endif
	}
	*/
	
	return result;
}
#endif



#endif
