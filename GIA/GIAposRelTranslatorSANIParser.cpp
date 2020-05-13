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
 * File Name: GIAposRelTranslatorSANIParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k14c 12-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Parser
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorSANIParser.hpp"
#include "SHAREDvars.hpp"



//TODO: check this code

#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSER

#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
bool GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupParseTree* firstParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, int layer)
{
	bool result = true;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_BASIC
	forwardPropogationSignalData->parserForwardPropogationSignalData = GIAposRelTranslatorParserForwardPropogationSignalData();	//reinitialise parserForwardPropogationSignalData (clear all variables)
	#endif
	
	GIAposRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;
	if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, firstParseTreeGroup, &parserForwardPropogationSignalData, layer))
	{
		result = false;
	}

	#ifndef GIA_POS_REL_TRANSLATOR_SANI_PARSE_BASIC
	forwardPropogationSignalData->parserForwardPropogationSignalData = parserForwardPropogationSignalData;	//store for next generateSemanticRelationsFromTxtRelationsWrapper execution
	#endif
		
	return result;
}

bool GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer)
{
	bool result = true;
	
	int numberOfComponentsInGroup = currentParseTreeGroup->components.size();
	vector<GIAposRelTranslatorParserForwardPropogationSignalData> parseTreeComponentSignalDataArray(numberOfComponentsInGroup);
	
	/*
	//code used if GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAposRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
	if(currentParseTreeGroup->neuronActive)
	{
	*/		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAposRelTranslatorRulesComponentParseTree* ownerComponent = (currentParseTreeGroup->components).at(i);
			/*
			//code used if GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAposRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
			if(ownerComponent->neuronComponentConnectionActive)
			{
			*/
				GIAposRelTranslatorRulesComponentParseTree* parseTreeComponent = ownerComponent;
				
				/*
				//code used if GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAposRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
				if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponent))
				{
					for(int i1=0; i1<ownerComponent->subComponents.size(); i1++)
					{
						GIAposRelTranslatorRulesComponent* subComponent = (ownerComponent->subComponents)[i1];
						if(subComponent->neuronComponentConnectionActive)
						{
							parseTreeComponent = subComponent;	//CHECKTHIS: what if the OR component has multiple activated subcomponents?
						}
					}
				}
				*/

				GIAposRelTranslatorParserForwardPropogationSignalData* parseTreeComponentSignalData = &(parseTreeComponentSignalDataArray[i]);

				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				GIAposRelTranslatorRules.printComponent(parseTreeComponent, layer);
				#endif

				if(parseTreeComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
				{
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_BIO
					*parseTreeComponentSignalData = *(parseTreeComponent->parserForwardPropogationSignalData);
					#else
					if(parseTreeComponent->parseTreeGroupRef != NULL)
					{
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_BASIC
						if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, parseTreeComponent->parseTreeGroupRef, parseTreeComponentSignalData, layer+1))
						{
							result = false;
						}
						#else
						*parseTreeComponentSignalData = (parseTreeComponent->parseTreeGroupRef->groupOrig->semanticRelationReturnEntityForwardPropogationSignalData).parserForwardPropogationSignalData;
						/*
						//note can't check parseTreeComponent->parseTreeGroupRef->neuronActive as it may have been reset by GIAposRelTranslatorSANIClass::propagateWordThroughNetworkGroup; resetGroupActivation
							//note it will not reset semanticRelationReturnEntityForwardPropogationSignalData, but deactivate the group+components
						if(!parseTreeComponent->parseTreeGroupRef->neuronActive)
						{
							cerr << "GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (!parseTreeComponent->parseTreeGroupRef->neuronActive)" << endl;
							exit(EXIT_ERROR);
						}
						*/						
						#endif
					}
					else
					{
						cout << "GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: (parseTreeComponent->parseTreeGroupRef == NULL)" << endl;
						exit(EXIT_ERROR);
					}
					#endif
					
					if(parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)
					{
						cerr << "GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)" << endl;
						exit(EXIT_ERROR);
					}
				}
				else if(parseTreeComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					GIApreprocessorPlainTextWord* parseTreeComponentWord = parseTreeComponent->candidateStringMatch;
					parseTreeComponentSignalData->semanticRelationReturnEntity = parseTreeComponentWord->translatorEntity;
					//cout << "parseTreeComponentWord->tagName = " << parseTreeComponentWord->tagName << endl;
					//cout << "parseTreeComponentWord->translatorEntity->entityName = " << parseTreeComponentWord->translatorEntity->entityName << endl;
				}
				else
				{
					cerr << "GIAposRelTranslatorParserOperationsClass::generateRulesGroupTreeComponent{} error: parseTreeComponent->componentType illdefined" << endl;
					exit(EXIT_ERROR);
				}
			/*
			//code used if GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup replaced with GIAposRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroup
			}
			else
			{
				bool optional = false;
				if(!(ownerComponent->optional))
				{
					cerr << "GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: !(ownerComponent->optional)" << endl;
					exit(EXIT_ERROR);
				}
			}
			*/
		}

		if(!GIAposRelTranslatorParserOperations.generateSemanticRelationsFromTxtRelationsPart2(translatorVariables, currentParseTreeGroup, &parseTreeComponentSignalDataArray, parserForwardPropogationSignalData, layer))
		{
			result = false;
		}

	/*
	}
	else
	{
		#ifndef GIA_POS_REL_TRANSLATOR_SANI_PARSE_BASIC
		cerr << "GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: !(currentParseTreeGroup->neuronActive)" << endl;
		exit(EXIT_ERROR);
		#endif
	}
	*/
	
	return result;
}
#endif



#endif
