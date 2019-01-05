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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1e 24-April-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Parser
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkParser.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SEPARATE
bool GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup, int layer)
{
	bool result = true;
	
	(firstParseTreeGroup->semanticRelationReturnEntityForwardPropogationSignalData).parserForwardPropogationSignalData = GIAtxtRelTranslatorParserForwardPropogationSignalData();	//reinitialise parserForwardPropogationSignalData (clear all variables)
	
	GIAtxtRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;
	if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, firstParseTreeGroup, &parserForwardPropogationSignalData, layer))
	{
		result = false;
	}

	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC
	(firstParseTreeGroup->semanticRelationReturnEntityForwardPropogationSignalData).parserForwardPropogationSignalData = parserForwardPropogationSignalData;	//store for next generateSemanticRelationsFromTxtRelationsWrapper execution
	#endif
		
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer)
{
	bool result = true;
	
	int numberOfComponentsInGroup = currentParseTreeGroup->components.size();
	vector<GIAtxtRelTranslatorParserForwardPropogationSignalData> parseTreeComponentSignalDataArray(numberOfComponentsInGroup);
		
	if(currentParseTreeGroup->neuronActive)
	{
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* ownerComponent = (currentParseTreeGroup->components).at(i);
			if(ownerComponent->neuronComponentConnectionActive)
			{
				GIAtxtRelTranslatorRulesComponent* parseTreeComponent = ownerComponent;
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

				GIAtxtRelTranslatorParserForwardPropogationSignalData* parseTreeComponentSignalData = &(parseTreeComponentSignalDataArray[i]);

				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				GIAtxtRelTranslatorRules.printComponent(parseTreeComponent, layer);
				#endif

				if(parseTreeComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
				{
					if(parseTreeComponent->parseTreeGroupRef != NULL)
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC
						if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, parseTreeComponent->parseTreeGroupRef, parseTreeComponentSignalData, layer+1))
						{
							result = false;
						}
						#else
						*parseTreeComponentSignalData = (parseTreeComponent->parseTreeGroupRef->semanticRelationReturnEntityForwardPropogationSignalData).parserForwardPropogationSignalData;
						/*
						//note can't check parseTreeComponent->parseTreeGroupRef->neuronActive as it may have been reset by GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup; resetGroupActivation
							//note it will not reset semanticRelationReturnEntityForwardPropogationSignalData, but deactivate the group+components
						if(!parseTreeComponent->parseTreeGroupRef->neuronActive)
						{
							cerr << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (!parseTreeComponent->parseTreeGroupRef->neuronActive)" << endl;
							exit(EXIT_ERROR);
						}
						*/
						if(parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)
						{
							cerr << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)" << endl;
							exit(EXIT_ERROR);
						}
						#endif
					}
					else
					{
						cout << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: (parseTreeComponent->parseTreeGroupRef == NULL)" << endl;
						exit(EXIT_ERROR);
					}
				}
				else if(parseTreeComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					GIApreprocessorPlainTextWord* parseTreeComponentWord = parseTreeComponent->candidateStringMatch;
					parseTreeComponentSignalData->semanticRelationReturnEntity = parseTreeComponentWord->translatorEntity;
				}
				else
				{
					cerr << "GIAtxtRelTranslatorParserOperationsClass::generateRulesGroupTreeComponent{} error: parseTreeComponent->componentType illdefined" << endl;
					exit(EXIT_ERROR);
				}
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
		}
		
		if(!GIAtxtRelTranslatorParserOperations.generateSemanticRelationsFromTxtRelationsPart2(translatorVariables, currentParseTreeGroup, &parseTreeComponentSignalDataArray, parserForwardPropogationSignalData, layer))
		{
			result = false;
		}
	}
	else
	{
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC
		cerr << "GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: !(currentParseTreeGroup->neuronActive)" << endl;
		exit(EXIT_ERROR);
		#endif
	}
	
	return result;
}
#endif
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
bool GIAtxtRelTranslatorNeuralNetworkParserClass::updatePerformance(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	//cout << "GIAtxtRelTranslatorNeuralNetworkParserClass::updatePerformance" << endl;
		 
	bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(currentParseTreeGroup->groupTypeNameBackup, currentParseTreeGroup->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
	if(topLevelGroup)
	{
		int performanceTemp = 0;
		traceBackpropActivationPath(currentParseTreeGroup, 1, false, &performanceTemp, forwardPropogationSentenceData->sentenceContents);
		resetNeuronBackprop(currentParseTreeGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_PRINTED);

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		forwardPropogationSentenceData->performance = performanceTemp;
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
		forwardPropogationSentenceData->performance = 0;
		for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
		{
			GIApreprocessorPlainTextWord* currentWord = (forwardPropogationSentenceData->sentenceContents)->at(i);
			if(currentWord->alreadyFoundMatch)
			{
				forwardPropogationSentenceData->performance = forwardPropogationSentenceData->performance + 1;
				currentWord->alreadyFoundMatch = false;
			}
		}
		#endif
		#endif

	}
			
	return result;
}
#endif



bool GIAtxtRelTranslatorNeuralNetworkParserClass::traceBackpropActivationPath(GIAtxtRelTranslatorRulesGroup* group, int level, bool print, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool result = true;
	
	if(!group->neuronPrinted)
	{
		if(!print)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			*performance = *performance + 1;
			//won't calculate performance perfectly because some groups are reused
			#else
			/*
			if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, group->semanticRelationReturnEntityForwardPropogationSignalData->wordReference))
			{
				//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
					//"dog" and "apple" will use identical neural groups and so will only count to +1 performance total
			}
			*/
			#endif
		}
		
		group->neuronPrinted = true;
		
		if(group->allComponentsActivatedAtLeastOnce)
		{			
			if(print)
			{
				//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(level);
				cout << "traceBackpropActivationPath: group->groupName = " << group->groupName << ", group->groupTypeNameBackup = " << group->groupTypeNameBackup << endl;		//<< ", parse word (providing semanticRelationReturnEntity) = ?"
				//#endif
			}

			for(int i=0; i<group->components.size(); i++)
			{
				GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i];
				
				//GIAtxtRelTranslatorRules.printComponent(currentComponent, level);
				
				if(!print)
				{
					#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
					if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
					{
						currentComponent->candidateStringMatch->alreadyFoundMatch = true;
						//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
							//"the" and "a" will use identical neural groups and so will only count to +1 performance total
					}
					#endif
				}
		
				if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					if(print)
					{
						GIAtxtRelTranslatorRules.printComponent(currentComponent, level);
					}
				}
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
				else if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
				{					
					for(int i2=0; i2<currentComponent->subComponents.size(); i2++)
					{
						GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i2];
						if(print)
						{
							if(subComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
							{
								GIAtxtRelTranslatorRules.printComponent(subComponent, level);
							}
							else
							{
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
								GIAtxtRelTranslatorRules.printComponent(subComponent, level);
								#endif
							}
						}
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_PARSETREE_GROUP_REFS
						for(int j=0; j<subComponent->parseTreeGroupRefs.size(); j++)
						{
							traceBackpropActivationPath(subComponent->parseTreeGroupRefs[j], level+1, print, performance, sentenceContents);
						}
						#else
						if(subComponent->parseTreeGroupRef != NULL)
						{
							traceBackpropActivationPath(subComponent->parseTreeGroupRef, level+1, print, performance, sentenceContents);
						}
						#endif
					}
				}
				#endif
				else
				{					
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_PARSETREE_GROUP_REFS
					for(int j=0; j<currentComponent->parseTreeGroupRefs.size(); j++)
					{
						traceBackpropActivationPath(currentComponent->parseTreeGroupRefs[j], level+1, print, performance, sentenceContents);
					}
					#else
					if(currentComponent->parseTreeGroupRef != NULL)
					{							
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
						if(print)
						{
							GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(level);
							cout << "(currentComponent->parseTreeGroupRef != NULL): currentComponent->parseTreeGroupRef->groupName = " << currentComponent->parseTreeGroupRef->groupName << endl;
						}
						#endif
						
						traceBackpropActivationPath(currentComponent->parseTreeGroupRef, level+1, print, performance, sentenceContents);
					}
					#endif
				}
			}	
		}
	}
	
	return result;
}



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
bool GIAtxtRelTranslatorNeuralNetworkParserClass::resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED)
	{
		boolPointer = &(group->neuronPropagated);
	}
	else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED)
	{
		boolPointer = &(group->neuronPreviousWordPOStypeTested);
	}
			
	if(*boolPointer)
	{
		//cout << "resetNeuronForwardProp: group = " << group->groupName << endl;
		*boolPointer = false;
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			GIAtxtRelTranslatorRulesGroup* ownerGroup = ownerComponent->ownerGroup;
			resetNeuronForwardProp(ownerGroup, groupBoolIndexType);
		}
	}
	
	return result;
}


//consider upgrading for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS/parseTreeGroupRefs support
bool GIAtxtRelTranslatorNeuralNetworkParserClass::resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_PRINTED)
	{
		boolPointer = &(group->neuronPrinted);
	}
	/*
	else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	{
		boolPointer = &(group->neuronSolidified);
	}
	*/
				
	if(*boolPointer)
	{
		*boolPointer = false;
		
		for(int i1=0; i1<group->components.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
			if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
			{
				for(int i1=0; i1<currentComponent->subComponents.size(); i1++)
				{
					GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i1];
					if(subComponent->parseTreeGroupRef != NULL)
					{
						resetNeuronBackprop(subComponent->parseTreeGroupRef, groupBoolIndexType);
					}
				}
			}
			else
			{
				if(currentComponent->parseTreeGroupRef != NULL)
				{
					resetNeuronBackprop(currentComponent->parseTreeGroupRef, groupBoolIndexType);
				}	
			}
		}
	}
	
	return result;
}

#endif



#endif
