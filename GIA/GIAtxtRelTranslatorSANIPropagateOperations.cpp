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
 * File Name: GIAtxtRelTranslatorSANIPropagateOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1b 05-March-2020
 * Requirements: 
 * Description: Textual Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Operations - generic functions
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorSANIPropagateOperations.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_SANI

static bool parseSentenceReverse;

#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
static bool firstExecution;
#endif

void GIAtxtRelTranslatorSANIPropagateOperationsClass::setParseSentenceReverseStrict(const bool parseSentenceReverseNew, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	parseSentenceReverse = parseSentenceReverseNew;
	forwardPropogationSentenceData->parseSentenceReverse = parseSentenceReverseNew;
}


void GIAtxtRelTranslatorSANIPropagateOperationsClass::setParseSentenceReverse(const bool normalDirection, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool parseSentenceReverseNew = false;
	if(normalDirection)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
		parseSentenceReverseNew = true;
		#else
		parseSentenceReverseNew = false;
		#endif
	}
	else
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
		parseSentenceReverseNew = false;
		#else
		parseSentenceReverseNew = true;
		#endif		
	}
	parseSentenceReverse = parseSentenceReverseNew;
	forwardPropogationSentenceData->parseSentenceReverse = parseSentenceReverseNew;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAtxtRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(testComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, lastActiveComponent, &existingActivationFoundNOTUSED);
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(testComponent, components, &existingActivationFoundNOTUSED);
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(testComponent, components, forwardPropogationWordData, repeatDetected, &existingActivationFoundNOTUSED);
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAtxtRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound)
{
	bool componentWordConnectivityTestsNOTUSED = false;
	bool missingStartComponentsFoundNOTUSED = false;
	bool missingOrVariableStartComponentFoundNOTUSED = false;
	int numberOfInactiveComponentsRemaining = 0;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(testComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, lastActiveComponent, existingActivationFound, &missingStartComponentsFoundNOTUSED, componentWordConnectivityTestsNOTUSED, &missingOrVariableStartComponentFoundNOTUSED, &numberOfInactiveComponentsRemaining);
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAtxtRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound, bool* missingStartComponentsFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, int* numberOfInactiveComponentsRemaining)
{	
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	*numberOfInactiveComponentsRemaining = 0;
		
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
	*previousActiveComponent = NULL;
	#endif
		
	for(int i=0; i<components->size(); i++)
	{
		int c;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}
		
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (*components)[c];

		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
		if(component->neuronComponentConnectionActive)
		{
			*lastActiveComponent = component;
		}
		#endif
			
		//cout << "component->componentIndex = " << component->componentIndex << endl;
			
		if(stillParsingActiveComponents)
		{
			//cout << "component->neuronComponentConnectionActive = " << component->neuronComponentConnectionActive << endl;
			/*
			cout << "component->neuronComponentConnectionActive = " << component->neuronComponentConnectionActive << endl;
			cout << "component->optional = " << component->optional << endl;
			cout << "component->hasSubComponents = " << GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component) << endl;
			*/
			
			if(component == testComponent)
			{
				//cout << "(component == testComponent)" << endl;

				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
				if(*previousActiveComponent == NULL)
				{
					*firstActiveComponentInGroup = true;
				}
				#endif
				
				stillParsingActiveComponents = false;

				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				bool previousComponentWasActivatedBySameWordDifferentPOSpropagation = false;
				if(*previousActiveComponent != NULL)
				{
					if((*previousActiveComponent)->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference)
					{
						previousComponentWasActivatedBySameWordDifferentPOSpropagation = true;
						//cout << "upperNeuronContainsWordIndexOfProspectiveComponentTest" << endl;
					}
				}
				if(!previousComponentWasActivatedBySameWordDifferentPOSpropagation)
				{
				#endif		
				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
				bool upperNeuronContainsWordIndexOfProspectiveComponentTest = false;
				if(!(component->ownerGroup->neuronActive))
				{
					if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, forwardPropogationWordData, component, component->ownerGroup))
					{
						upperNeuronContainsWordIndexOfProspectiveComponentTest = true;
						//cout << "upperNeuronContainsWordIndexOfProspectiveComponentTest" << endl;
					}
				}
				if(!upperNeuronContainsWordIndexOfProspectiveComponentTest)
				{
				#endif			
					if(component->neuronComponentConnectionActive)
					{
						//cout << "(component->neuronComponentConnectionActive)" << endl;
						if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)	//if the component contains repeats, then accept it if it is already active
						{
							//sequential activation found
							sequentialActivationFound = true;
						}
						else
						{

							//component already activated
							#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
							if(i == 0)	//start component in group
							{
								bool allowComponentReset = true;
								#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
								if(componentWordConnectivityTests)
								{
								#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
									if(consecutiveSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, components, component))
									{
										//cout << "!allowComponentReset: consecutiveSequenceDetected - component->ownerGroup->groupIndex = " << component->ownerGroup->groupIndex << endl;
										//cout << "!allowComponentReset; consecutiveSequenceDetected" << endl;
										allowComponentReset = false;
									}
									#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
									if(findValidDualLowerLevelConnection(forwardPropogationSentenceData, components, component))
									{
										//cout << "!allowComponentReset; findValidDualLowerLevelConnection" << endl;
										allowComponentReset = false;
									}
									#endif
									/*
									#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
									if(!(component->ownerGroup->neuronComponentConnectionActive))
									{
										if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, forwardPropogationWordData, component, component->ownerGroup))
										{
											allowComponentReset = true;
										}
									}
									#endif
									*/
								#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
								}
								else
								{
									//always reset, as existing first activated component is not connected (by wordIndices) to prospective activated next component
								}
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
								if(existingActivationFound)
								{
									if(!(forwardPropogationSignalData->firstPOSval))
									{
										if(component->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference)
										{
											//component has been activated by previous POS propagation
											allowComponentReset = false;
										}
									}
								}
								#endif
								if(allowComponentReset)
								{
									sequentialActivationFound = true;
									*existingActivationFound = true;
								}
								else
								{
									sequentialActivationFound = false;
								}
							}
							#else
							#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
							sequentialActivationFound = true;
							*existingActivationFound = true;	
							#endif
							#endif
						}
					}
					else
					{
						sequentialActivationFound = true;
						//cout << "!(component->neuronComponentConnectionActive)" << endl;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
				}
				#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				}
				#endif
			}
			else
			{
				//cout << "!(component == testComponent)" << endl;

				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
				if(component->neuronComponentConnectionActive)
				{
					//cout << "previousActiveComponent = component" << endl;
					*previousActiveComponent = component;
				}
				#endif

				if(component->optional)
				{
					//ignore optional components
				}
				else
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
					//note it doesn't matter if the start component is active or inactive. Even if it has been activated miscellaneously (ie without maintaining word index connectivity), it can be reset.
					if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
					{
						if(i == 0)
						{
							if(component->componentType != GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//added GIA3j5aTEMP65
							{
								*missingOrVariableStartComponentFound = true;
							}	
						}
						else
						{
							*missingOrVariableStartComponentFound = false;	//removed @GIA3j5aTEMP18
						}
					}
					#endif
						
					if(!(component->neuronComponentConnectionActive))
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
						if(forwardPropogationSignalData->firstIndexInSequence == 0)
						{
							#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
							if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage) || (i != 0) || (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING))	//ie || !(*missingOrVariableStartComponentFound)
							{
								stillParsingActiveComponents = false;
							}
							#else
							stillParsingActiveComponents = false;	
							#endif
						}
						else
						{
							if(*previousActiveComponent == NULL)
							{
								//support activation of group components with missing start components
								*missingStartComponentsFound = true;
							}
							else
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
								if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage) || (i != 0) || (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING))	//ie || !(*missingOrVariableStartComponentFound)
								{
									stillParsingActiveComponents = false;
								}
								#else
								stillParsingActiveComponents = false;	//found an active component then a missing component (fail)
								#endif
							}
						}
						#else
						stillParsingActiveComponents = false;	
						#endif
					}						
				}
			}
		}
		else
		{
			if(sequentialActivationFound)
			{
				if(!(component->optional))
				{
					if(!(component->neuronComponentConnectionActive))
					{
						*numberOfInactiveComponentsRemaining = *numberOfInactiveComponentsRemaining + 1;	
					}
					else
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PERFORM_AUTORESET_OF_ACTIVATIONS
						bool foundFollowingActiveComponent = true;
						//perform autoreset of activations
						if(*existingActivationFound)
						{
							//cout << "*existingActivationFound" << endl;
							*numberOfInactiveComponentsRemaining = *numberOfInactiveComponentsRemaining + 1;
						}
						else
						{
							//cout << "group not suitable for activation, following component is active" << endl;
							sequentialActivationFound = false;
						}
						/*OLD:
						component->neuronComponentConnectionActive = false;
						*existingActivationFound = true;
						*/
						#else
						//cout << "group not suitable for activation, following component is active" << endl;
						sequentialActivationFound = false;
						#endif
					}
				}
			}
		}
	}
	
	if(sequentialActivationFound)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		bool passMissingStartComponentTests = false;
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			if(!((*missingStartComponentsFound) && !(*missingOrVariableStartComponentFound)))
			{
				passMissingStartComponentTests = true;
			}
		}
		else
		{
			if(!(*missingStartComponentsFound))
			{
				passMissingStartComponentTests = true;
			}
		}
		//if(!((*missingStartComponentsFound) && !(*missingOrVariableStartComponentFound)))	//equivalent
		if(passMissingStartComponentTests)
		{
		#else
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		if(!(*missingStartComponentsFound))
		{
		#endif	
		#endif
			if(*numberOfInactiveComponentsRemaining == 0)
			{
				*activationSequenceCompleted = true;
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS	
		}
		#else
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		}
		#endif	
		#endif
		//cout << "numberOfInactiveComponentsRemaining = " << *numberOfInactiveComponentsRemaining << endl;
	}

	return sequentialActivationFound;
}


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::upperNeuronContainsWordIndexOfProspectiveComponent(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool result = false;
	
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
		cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	
		//method2;
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* previousComponent = NULL;
		int c = currentComponent->componentIndex;
		int cPrevious;
		bool foundPreviousComponent = false;
		if(parseSentenceReverse)
		{
			cPrevious = c+1;
			if(cPrevious < ownerGroup->components.size())
			{
				foundPreviousComponent = true;
			}
		}
		else
		{
			cPrevious = c-1;
			if(cPrevious >= 0)
			{
				foundPreviousComponent = true;
			}
		}
		if(foundPreviousComponent)
		{
			previousComponent = (ownerGroup->components)[cPrevious];
			if(previousComponent->neuronComponentConnectionActive)
			{
				if(previousComponent->neuronComponentConnectionActiveWordRecord == forwardPropogationWordData->wordReference)
				{
					result = true;
					cout << "upperNeuronContainsWordIndexOfProspectiveComponent - forwardPropogationWordData has already been propagated to an upper group (prevent usage duplication)" << endl;
					cout << "previousComponent->neuronComponentConnectionActiveWordRecord->w = " << previousComponent->neuronComponentConnectionActiveWordRecord->w << endl;
					cout << "forwardPropogationWordData->wordReference->w = " << forwardPropogationWordData->wordReference->w << endl;
					//exit(EXIT_ERROR);
				}
			}
		}
			
		/*
		//method1;
		GIAtxtRelTranslatorRulesGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			if(ownerGroupParseTree->currentParseTreeGroupTemp->parseTreeMinWordIndex == forwardPropogationWordData->w)	//<=	//component->neuronComponentConnectionActiveWordRecord->w
			{
				result = true;
				cout << "upperNeuronContainsWordIndexOfProspectiveComponent 1 - forwardPropogationWordData has already been propagated to an upper group (prevent usage duplication)" << endl;
			}
		}
		else
		{
			if(ownerGroupParseTree->currentParseTreeGroupTemp->parseTreeMaxWordIndex == forwardPropogationWordData->w)	//>=
			{	
				result = true;
				cout << "upperNeuronContainsWordIndexOfProspectiveComponent 2 - forwardPropogationWordData has already been propagated to an upper group (prevent usage duplication)" << endl;
			}
		}
		*/

		if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, forwardPropogationWordData, component, ownerGroup))
		{
			result = true;
		}	

	}
							
	return result;
}
#endif

							
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::consecutiveSequenceDetected(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component)
{
	bool result = false;
	
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		//if(component->ownerGroup->neuronActive)
		//if(component->neuronComponentConnectionActiveWordRecord == forwardPropogationWordData->wordReference)
		//{
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				if(component->ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex == forwardPropogationWordData->w+1)
				{
					//group is being repeated:
					result = true;
					//cout << "consecutiveSequenceDetected 1" << endl;
				}
			}
			else
			{
				if(component->ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex == forwardPropogationWordData->w-1)
				{	
					//group is being repeated:
					result = true;
					//cout << "consecutiveSequenceDetected 2" << endl;
				}
			}
		//}
	}
							
	return result;
}
#endif


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component)
{	
	bool validDualLowerLevelConnectionFound = false;
	bool firstInactiveComponentFound = false;
	bool firstActiveComponentFound = false;
	for(int i2=0; i2<components->size(); i2++)
	{
		int c2;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c2 = components->size()-1-i2;
		}
		else
		{
			c2 = i2;
		}

		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component2 = (*components)[c2];
		if(component2->neuronComponentConnectionActive)
		{
			firstActiveComponentFound = true;
		}
		else if(firstActiveComponentFound)
		{
			if(!firstInactiveComponentFound)
			{
				if(!(component2->neuronComponentConnectionActive))
				{
					firstInactiveComponentFound = true;
					for(int l=0; l<component2->ANNbackGroupConnectionList.size(); l++)
					{
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupSource2 = component2->ANNbackGroupConnectionList[l];

						for(int l2=0; l2<component->ANNbackGroupConnectionList.size(); l2++)
						{
							GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l2];

							if(groupSource2 == groupSource)
							{
								validDualLowerLevelConnectionFound = true;
								//cout << "\n\n\n\n GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: GIAtxtRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound" << endl;
								//exit(EXIT_ERROR);
							}
							else
							{	
								/*
								#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
								if(findGroup2InForwardConnectionBranchOfGroup1(groupSource, groupSource2))
								{
									validDualLowerLevelConnectionFound = true;
									cout << "\n\n\n\n warning: GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: GIAtxtRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound1 - CHECKTHIS" << endl;
									//exit(EXIT_ERROR);
								}
								if(findGroup2InForwardConnectionBranchOfGroup1(groupSource2, groupSource))
								{
									validDualLowerLevelConnectionFound = true;
									cout << "\n\n\n\n warning: GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: GIAtxtRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound2 - CHECKTHIS" << endl;
									//exit(EXIT_ERROR);
								}
								#endif
								*/
							}
						}
					}
				}
			}
		}
	}
	return validDualLowerLevelConnectionFound;
}
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::findGroup2InForwardConnectionBranchOfGroup1(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group2, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroup)
{
	bool result = false;
	
	for(int i=0; i<currentGroup->ANNfrontComponentConnectionList.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (currentGroup->ANNfrontComponentConnectionList)[i];
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
		if(group2 == ownerGroup)
		{
			result = true;
		}
		else
		{
			if(findGroup2InForwardConnectionBranchOfGroup1(group2, ownerGroup))
			{
				result = true;
			}
		}
	}
	
	return result;
}
#endif
#endif

							
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, bool* existingActivationFound)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		int c;
		if(parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (*components)[c];
		if(component == testComponent)
		{
			if(component->neuronComponentConnectionActive)
			{
				//component already activated
				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
				sequentialActivationFound = true;
				*existingActivationFound = true;
				#endif
			}	
			else
			{
				//sequential activation found
				sequentialActivationFound = true;
			}
		}
	}
	
	return sequentialActivationFound;
}

//precondition: repeat sequences can contain only 1 component
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFound)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		int c;
		if(parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (*components)[c];
		if(component == testComponent)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			bool pass = true;
			if(testComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				if(component->neuronComponentConnectionActive)
				{
					pass = false;
					int translatorSentenceWordIndexComparisonInt;
					if(parseSentenceReverse)
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex + 1;
					}
					else
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex - 1;
					}
					if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == translatorSentenceWordIndexComparisonInt)
					{
						pass = true;
						//cout << "propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady pass" << endl;
						//exit(EXIT_ERROR);
					}
				}
			}
			if(pass)
			{
			#endif
				//sequential activation found
				if(component->neuronComponentConnectionActive)
				{
					*repeatDetected = true;
					/*
					//do not treat repeats as existingActivationFound
					#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
					//if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == forwardPropogationWordData->wordReference->translatorSentenceWordIndex)
					//{
						*existingActivationFound = true;
					//}
					#endif
					*/
				}
				sequentialActivationFound = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			}
			#endif
		}
		else
		{
			cerr << "GIAtxtRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady{} error: repeat sequences can contain only 1 component" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return sequentialActivationFound;
}



#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper(GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = false;

	bool existingActivationFoundNOTUSED = false;

	GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		int lastActiveComponentInParseTreeIndex = activationPathWordCurrentParseTreeGroupOwner->components.size() - 1;
		GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = (activationPathWordCurrentParseTreeGroupOwner->components)[lastActiveComponentInParseTreeIndex];
		GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
		
		GIAtxtRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED = NULL;
		result = componentWordConnectivityTests(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED, forwardPropogationWordData, existingActivationFoundNOTUSED);
	}
	else
	{
		result = true;
	}
	
	return result;
}
#else
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	return componentWordConnectivityTestsWrapper(ownerGroup->currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFound);
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;

	GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;

	GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
	GIAtxtRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef = NULL;	
	GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = NULL;
	GIAtxtRelTranslatorRulesGroupParseTree* ownerGroupParseTreeGroup = currentParseTreeGroupTemp;
	
	/*
	cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	if(ownerGroup->currentParseTreeGroupTemp == NULL)
	{
		cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper ownerGroup->currentParseTreeGroupTemp == NULL" << endl;
	}
	cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
	*/
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	if(existingActivationFound)
	{
		//group activations will be reset so assume real ownerGroupParseTreeGroup->components.size() == 0 
		result = true;
	}
	else
	{
	#endif
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{
			int lastActiveComponentInParseTreeIndex;
			int previousActiveComponentInParseTreeIndex;
			if(parseSentenceReverse)
			{
				lastActiveComponentInParseTreeIndex = 0;
				previousActiveComponentInParseTreeIndex = 1;		
			}
			else
			{
				lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				previousActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 2;		
			}

			GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;

			GIAtxtRelTranslatorRulesComponentParseTree* previousActiveComponentInParseTree = NULL;
			if(ownerGroupParseTreeGroup->components.size() > 1)
			{
				previousActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[previousActiveComponentInParseTreeIndex];
				previousActiveComponentInParseTreeParseTreeGroupRef = previousActiveComponentInParseTree->parseTreeGroupRef;
			}

			if(componentWordConnectivityTests(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRef, forwardPropogationWordData, existingActivationFound))
			{
				result = true;
			}		
		}
		else
		{
			result = true;
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	}
	#endif
			
	return result;
}
#endif

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree, GIAtxtRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;
		
	//cout << "\t 1GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests{}: " << endl;

	int wordIndexMax = -1;
	int wordIndexMin = REALLY_LARGE_INT;
	GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMin;
	GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax;
	int wordIndexMinMin;
	int wordIndexMaxMax;
	if(parseSentenceReverse)
	{
		parseTreeGroupToFindWordIndexMin = lastActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindWordIndexMax = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		//cout << "lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex = " << lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex << endl;
		wordIndexMinMin = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;	//CHECKTHIS
		wordIndexMaxMax = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//CHECKTHIS
		//cout << "wordIndexMaxMax1: forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
		//cout << " forwardPropogationWordData->wordReference = " << forwardPropogationWordData->wordReference->tagName << endl;
	}
	else
	{
		parseTreeGroupToFindWordIndexMin = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindWordIndexMax = lastActiveComponentInParseTreeParseTreeGroupRef;
		wordIndexMinMin = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		wordIndexMaxMax = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
	}

	if(parseTreeGroupToFindWordIndexMax != NULL)
	{
		//cout << "wordIndexMax1: parseTreeGroupToFindWordIndexMax->parseTreeMaxWordIndex = " << parseTreeGroupToFindWordIndexMax->parseTreeMaxWordIndex << endl;
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true, &wordIndexMax, 0))
		{
			//cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests error{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true)" << endl;	
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests pass{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true)" << endl;	
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMax = SHAREDvars.maxInt(wordIndexMax, wordIndexMaxMax);

	if(parseTreeGroupToFindWordIndexMin != NULL)
	{
		//cout << "parseTreeGroupToFindWordIndexMin->groupRef->groupIndex = " << parseTreeGroupToFindWordIndexMin->groupRef->groupIndex << endl;
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false, &wordIndexMin, 0))
		{
			//newParseComponent->neuronComponentConnectionActiveWordRecord has not yet been defined
			//cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests error{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false) && " << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests pass{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false) && " << endl;
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMin = SHAREDvars.minInt(wordIndexMin, wordIndexMinMin);

	//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA9
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
	cout << "wordIndexMax = " << wordIndexMax << endl;
	cout << "wordIndexMin = " << wordIndexMin << endl;
	cout << "wordIndexMaxMax = " << wordIndexMaxMax << endl;
	cout << "wordIndexMinMin = " << wordIndexMinMin << endl;
	cout << "existingActivationFound = " << existingActivationFound << endl;
	#endif
	
	if(wordIndexMin-wordIndexMax == 1)
	{
		result = true;
	}
	#ifndef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
	else if(existingActivationFound)
	{
	
		//assumes GIA_TXT_REL_TRANSLATOR_SANI_REVERSE_DIRECTION:
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE_BIO
		if(previousActiveComponentInParseTreeParseTreeGroupRef != NULL)
		{
			int wordIndexMinPrevious = previousActiveComponentInParseTreeParseTreeGroupRef->parseTreeMinWordIndex;
			if(wordIndexMinPrevious-wordIndexMax == 1)
			{
				result = true;
			}
		}
		else
		{
			if(wordIndexMin <= wordIndexMax)
			{
				result = true;
			}
		}
		#else
		if(wordIndexMin <= wordIndexMax)
		{
			result = true;
		}
		#endif
	}
	#endif
	#endif

	return result;
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::getMinMaxWordIndexInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA6
	if(findMaxOrMinWordIndex)
	{
		printParseTreeDebugIndentation(level);
		cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::getMinMaxWordIndexInParseTree: currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
	}
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroup->components.size() > 0)
	{
		if(findMaxOrMinWordIndex)
		{
			*wordIndexMaxOrMin = currentParseTreeGroup->parseTreeMaxWordIndex;
			//cout << "currentParseTreeGroup->parseTreeMaxWordIndex = " << currentParseTreeGroup->parseTreeMaxWordIndex << endl;
			result = true;
		}
		else
		{
			*wordIndexMaxOrMin = currentParseTreeGroup->parseTreeMinWordIndex;
			//cout << "currentParseTreeGroup->parseTreeMinWordIndex = " << currentParseTreeGroup->parseTreeMinWordIndex << endl;
			result = true;
		}
	}
	#else
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA6
		if(!findMaxOrMinWordIndex)
		{
			printParseTreeDebugIndentation(level);
			cout << "currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex = " << currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex  << endl;
		}
		#endif
		
		if(findMaxOrMinWordIndex)
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex > *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
				result = true;
			}
		}
		else
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex < *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
				result = true;
			}
		}
						
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(getMinMaxWordIndexInParseTree(currentComponent->parseTreeGroupRef, findMaxOrMinWordIndex, wordIndexMaxOrMin, level+1))
			{
				result = true;
			}
		}
	}
	#endif
	
	return result;	
}

#endif



#ifdef GIA_TXT_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::calculatePerformanceWeightOfParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* maxWeight)
{
	bool result;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
	*maxWeight = currentParseTreeGroup->parseTreeTotalWeight;
	#else
	*maxWeight = currentParseTreeGroup->parseTreeMaxWeight;
	#endif
	//cout << "maxWeight = " << *maxWeight << endl;
	#else
	bool print = false;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	bool calculateMaxWeight = true;
	*maxWeight = 0;
	traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceNOTUSED, forwardPropogationSentenceData->sentenceContents, calculateMaxWeight, maxWeight);
	resetNeuronBackprop(currentParseTreeGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::setPerformanceWeightOfParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int maxWeight)
{
	bool result;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
	currentParseTreeGroup->parseTreeTotalWeight = maxWeight;
	#else
	currentParseTreeGroup->parseTreeMaxWeight = maxWeight;
	#endif
	//cout << "maxWeight = " << *maxWeight << endl;
	#else
	cerr << "GIAtxtRelTranslatorSANIPropagateOperationsClass::setPerformanceWeightOfParseTree requires GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_WEIGHT_BIO" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}
#endif


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE

#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::updatePerformance(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(currentParseTreeGroup->groupTypeName, currentParseTreeGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
	if(topLevelGroup)
	{
		result = updatePerformanceGroup(currentParseTreeGroup, forwardPropogationSentenceData, layer);
	}
	
	return result;
}

//doesn't perform topLevelGroup check;
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	//cout << "updatePerformanceGroup" << endl;
			 
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_BIO
	forwardPropogationSentenceData->performance = currentParseTreeGroup->parseTreeMaxWordIndex - currentParseTreeGroup->parseTreeMinWordIndex + 1;
	#else
	int performanceTemp = 0;
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_DEBUG_WORDCONNECTIVITY_VERIFICATION
	cout << "GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION: GIAtxtRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup, traceBackpropParseTree:" << endl;
	bool print = true;
	#else
	bool print = false;
	#endif
	bool performancePreprocess = true;
	if(!traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceTemp, forwardPropogationSentenceData->sentenceContents))
	{
		cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup fail #1" << endl;
		result = false;
	}
	resetNeuronBackprop(currentParseTreeGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
	forwardPropogationSentenceData->performance = performanceTemp;
	#else
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
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	if(result)
	{
		if(forwardPropogationSentenceData->performance != forwardPropogationSentenceData->sentenceContents->size())
		{
	#endif
			cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup fail #2" << endl;
			result = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
		}
	}
	#endif

	/*
	cout << "forwardPropogationSentenceData->performance = " << forwardPropogationSentenceData->performance << endl;
	cout << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
	cout << "result = " << result << endl;
	*/
			
	return result;
}
#endif

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool calculateMaxWeight = false;
	int maxWeightNOTUSED = 0;
	return traceBackpropParseTree(currentParseTreeGroup, level, print, performancePreprocess, performance, sentenceContents, calculateMaxWeight, &maxWeightNOTUSED);

}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight)
{
	bool result = true;
	
	#ifndef GIA_TXT_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif
	
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(calculateMaxWeight)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
			*maxWeight = *maxWeight + currentParseTreeGroup->groupWeight;
			//cout << "*maxWeight = " << *maxWeight << endl;
			#else
			if(currentParseTreeGroup->groupWeight > *maxWeight)
			{
				*maxWeight = currentParseTreeGroup->groupWeight;
			}
			#endif
		}
		#endif

		if(performancePreprocess)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			*performance = *performance + 1;
			//won't calculate performance perfectly because some groups are reused
			#else
			/*
			if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentParseTreeGroup->semanticRelationReturnEntityForwardPropogationSignalData->wordReference))
			{
				//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
					//"dog" and "apple" will use identical neural groups and so will only count to +1 performance total
			}
			*/
			#endif
		}
				
		if(print)
		{
			//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE
			printParseTreeDebugIndentation(level);
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupRef->groupIndex = " << currentParseTreeGroup->groupRef->groupIndex << endl;
			#else
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupTypeName = " << currentParseTreeGroup->groupTypeName << endl;		//<< ", parse word (providing semanticRelationReturnEntity) = ?"
			#endif
			//#endif
		}
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
			
			//printComponent(currentComponent, level);
			
			#ifndef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			if(performancePreprocess)
			{	
				//cout << "currentComponent->candidateStringMatch = " << currentComponent->candidateStringMatch << endl;
				if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
				{					
					#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
					if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//redundant?
					{
						if(currentComponent->candidateStringMatch->alreadyFoundMatch)
						{
							result = false;
							//cout << "duplicate instance of word detected in parsetree - fail to parse sentence" << endl;
							//duplicate instance of word detected in parsetree - fail to parse sentence
						}
						else
						{
					#endif
							//cout << "alreadyFoundMatch" << endl;
							
							currentComponent->candidateStringMatch->alreadyFoundMatch = true;
							//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
								//"the" and "a" will use identical neural groups and so will only count to +1 performance total
					#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
						}
					}
					#endif
				}
			}
			#endif
	
			if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				if(print)
				{
					printComponent(currentComponent, level+1);	//+1 added @GIA3j5aTEMP66
					/*
					#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
					if(currentComponent->parseTreeGroupRef != NULL)
					{	
						printParseTreeDebugIndentation(level);
						cout << "\tcurrentComponent->parseTreeGroupRef->groupRef->groupIndex = " << currentComponent->parseTreeGroupRef->groupRef->groupIndex << endl;
					}
					#endif
					*/
				}
			}
			#ifndef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{							
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA
					if(print)
					{
						printParseTreeDebugIndentation(level);
						cout << "(currentComponent->parseTreeGroupRef != NULL): currentComponent->parseTreeGroupRef->groupName = " << currentComponent->parseTreeGroupRef->groupName << endl;
					}
					#endif
					
					if(!traceBackpropParseTree(currentComponent->parseTreeGroupRef, level+1, print, performancePreprocess, performance, sentenceContents, calculateMaxWeight, maxWeight))
					{
						result = false;
					}
				}
			#ifndef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef GIA_TXT_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}

#endif





								
								
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::isSentenceWordDataFullyConnected(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//forwardPropogationWordData->previousWordConnections.push_back(wPrevious);
	bool result = true;
	
	int numberOfWords = forwardPropogationSentenceData->forwardPropogationWordDataArray.size();
	vector<bool> previousWordConnectionsFound(numberOfWords, false);
	previousWordConnectionsFound[numberOfWords-1] = true;	//set last word as found 
	GIAtxtRelTranslatorSANIForwardPropogationWordData* lastWordData = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[numberOfWords-1]);
	isSentenceWordDataFullyConnected(&previousWordConnectionsFound, lastWordData);
	for(int w = 0; w<previousWordConnectionsFound.size(); w++)
	{
		if(!previousWordConnectionsFound[w])
		{
			result = false;
		}
	}
	
	return result;
}

void GIAtxtRelTranslatorSANIPropagateOperationsClass::isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAtxtRelTranslatorSANIForwardPropogationWordData* currentWordData)
{
	for(int i = 0; i<currentWordData->previousWordConnections.size(); i++)
	{
		GIAtxtRelTranslatorSANIForwardPropogationWordData* previousWordData = (currentWordData->previousWordConnections)[i];
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
		cout << "previousWordConnectionsFound == true: previousWordData->w = " << previousWordData->w << endl;
		#endif
		(*previousWordConnectionsFound)[previousWordData->w] = true;
		isSentenceWordDataFullyConnected(previousWordConnectionsFound, previousWordData);
	}
}
#endif



#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::componentTests1(GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	if(currentComponent->wordNounVariantType != GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		
		/*
		cout << "componentTests; (currentComponent->wordNounVariantType != GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)" << endl;
		cout << "forwardPropogationSignalData->wordNounVariantType = " << forwardPropogationSignalData->wordNounVariantType << endl;
		cout << "currentComponent->wordNounVariantType = " << currentComponent->wordNounVariantType << endl;
		*/
		
		if(forwardPropogationSignalData->wordNounVariantType == currentComponent->wordNounVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
		else if(forwardPropogationSignalData->wordNounVariantType == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR_OR_PLURAL)	
		{
			componentTests = true;
		}
		#endif
	}
	if(currentComponent->wordVerbVariantType != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordVerbVariantType == currentComponent->wordVerbVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_INTERPRET_PAST_AND_PAST_PARTICIPLE_THE_SAME
		else if((forwardPropogationSignalData->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST) && (currentComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE))
		{
			componentTests = true;
		}
		else if((forwardPropogationSignalData->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE) && (currentComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST))
		{
			componentTests = true;
		}
		#endif
	}
	
	return componentTests;
}
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::componentTests2(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
	Algorithm:
	#else
	Algorithm:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous word POStype that was succesfully parsed by a higher order neuron/group.
		Question: so how will the program know this until the higher order neuron group has been parsed?
		Conclusion: will need to;
			a) record the wordPos type for every activated component in a group
			b) read both;
				i) previous components in current group and 
				ii) read up the tree to see if this condition is met (ie there exists a previous word POStype that was succesfully parsed by a higher order neuron/group)
	#endif
	*/
	if(group->previousWordPOStype != "")
	{
		componentTests = false;
		int previousWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);
		int translatorSentenceWordIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		bool pass = false;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceWordIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationWordData->wordReference;
		}
		else
		{
			pass = true;
			//cout << "firstWordInCurrentParseTreeGroupParseTree->translatorSentenceWordIndex = " << firstWordInCurrentParseTreeGroupParseTree->translatorSentenceWordIndex << endl;
			//exit(EXIT_ERROR);
		}

		/*
		if(pass)
		{
			cout << "group->previousWordPOStype = " << group->previousWordPOStype << ", getFirstWordInParseTree pass, firstWordInCurrentParseTreeGroupParseTree->translatorSentenceWordIndex = " << firstWordInCurrentParseTreeGroupParseTree->translatorSentenceWordIndex << ", numComponentsFound = " << numComponentsFound << endl;
		}
		else
		{
			cout << "group->previousWordPOStype = " << group->previousWordPOStype << ", getFirstWordInParseTree !pass" << endl;
		}
		*/
		
		if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, previousWordPOStype))		
		{
			//cout << "componentTests = true" << endl;
			componentTests = true;
		}
	}
	if(group->existsPreceedingWordPOStype != "")
	{
		componentTests = false;
		int existsPreceedingWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);
		int translatorSentenceWordIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceWordIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationWordData->wordReference;
		}
		else
		{
		
		}
		
		if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, existsPreceedingWordPOStype))		
		{
			componentTests = true;
		}
	}
	
	return componentTests;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::getFirstWordInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceWordIndexMin, int* numComponentsFound)
{
	bool result = false;
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		//if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		//{
			*numComponentsFound = *numComponentsFound + 1;
		//}
		
		int translatorSentenceWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		if(translatorSentenceWordIndex < *translatorSentenceWordIndexMin)
		{
			*translatorSentenceWordIndexMin = translatorSentenceWordIndex;
			*firstWordInCurrentParseTreeGroupParseTree = currentComponent->neuronComponentConnectionActiveWordRecord;
			result = true;
		}
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{							
			if(getFirstWordInParseTree(currentComponent->parseTreeGroupRef, firstWordInCurrentParseTreeGroupParseTree, translatorSentenceWordIndexMin, numComponentsFound))
			{
				result = true;
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	GIApreprocessorPlainTextWord* previousComponentWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(contextWord == currentWord)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				previousComponentWord = contextWord;
			}
		}
	}
	if(previousComponentWord != NULL)
	{
		if(checkWordPOStype(previousComponentWord, previousWordPOStype))
		{
			result = true;
		}
	}
			
	return result;
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(contextWord == currentWord)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				if(checkWordPOStype(contextWord, existsPreceedingWordPOStype))
				{
					result = true;
				}
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex)
{
	bool result = false;	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == unambiguousPOSindex)
	{
		result = true;
	}
	#else
	if(!currentWordAmbiguous(currentWord))
	{
		if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, unambiguousPOSindex))
		{
			result = true;
		}
	}
	#endif
	return result;
}
#endif




bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];

			bool* boolPointer = NULL;
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED)
			{
				boolPointer = &(group->neuronGenerated);
			}

			if(*boolPointer)
			{
				*boolPointer = false;
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	//cout << "GIAtxtRelTranslatorRulesGroupTypes->size() = " << GIAtxtRelTranslatorRulesGroupTypes->size() << endl;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];

			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE)
			{
				//cout << "resetGroupActivation = " << group->groupName << endl; 
				resetGroupActivation(group);
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
			
			#else
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF)
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
				cout << "group = " << group->groupName << endl;
				printGroup(group, 1);
				#endif
				
				resetGroupParseTreeGroupRef(group, true);
			}
			#endif
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED)
			{
				//cout << "group->neuronPropagated = false" << endl;
				group->neuronPropagated = false;
			}
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE)
			{
				group->neuronPropagatedSave = group->neuronPropagated;
			}
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET)
			{
				group->neuronPropagated = group->neuronPropagatedSave;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED)
			{
				group->neuronProcessed = false;
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SOLIDIFY_BIO
				group->solidified = false;
				#else
				group->currentParseTreeGroupTemp->solidified = false;
				#endif
			}
			#endif
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET)
			{
				resetGroupOptimumPathway(group);
			}
			#endif
	
		}
	}
	
	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool lastComponentIsFuzzy = false;

	int indexOfLastComponentInGroup = group->components.size()-1;
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastComponent = (group->components)[indexOfLastComponentInGroup];
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_TREAT_OR_AS_FUZZY
	if(lastComponent->optional || GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(lastComponent))
	#else
	if(lastComponent->optional || (lastComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
	#endif
	{
		lastComponentIsFuzzy = true;
	}
		
	return lastComponentIsFuzzy;	
}
#endif

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetGroupActivation(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool result = true;

	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
	resetGroupParseTreeGroupRef(group, false);	//leave existing parseTreeGroupRef in memory
	#endif
	*/
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA2
	//cout << "resetGroupActivation: group->groupName = " << group->groupName << ", group->groupTypeName = " << group->groupTypeName << endl;
	#endif
	
	group->neuronActive = false;	//not used
	resetComponentsActivation(&(group->components));
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetComponentsActivation(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i1];
		
		#ifdef GIA_TXT_REL_TRANSLATOR_SANI_RESET_COMPONENT_NEURONCOMPONENTCONNECTIONACTIVEWORDRECORD
		currentComponent->neuronComponentConnectionActiveWordRecord = NULL;
		#endif
		currentComponent->neuronComponentConnectionActive = false;
		
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			resetComponentsActivation(&(currentComponent->subComponents));
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::restoreGroupActivation(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig)
{	
	bool result = true;

	group->neuronActive = groupOrig->neuronActive;
	restoreComponentsActivation(&(group->components), &(groupOrig->components));
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::restoreComponentsActivation(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* componentsOrig)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i1];
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponentOrig = (*componentsOrig)[i1];
		currentComponent->neuronComponentConnectionActive = currentComponentOrig->neuronComponentConnectionActive;
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			restoreComponentsActivation(&(currentComponent->subComponents), &(currentComponentOrig->subComponents));
		}
	}
	
	return result;
}




#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY
#ifdef GIA_TXT_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupActivationMemory* activationMemoryGroup, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;	
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP			
	for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
	{
		GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray[i];
		GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroupActivationPointTemp = currentParseTreeGroup->activationPathWordFirstParseTreeGroupActivationPointArray[i];
		
		if(!(activationPathWordFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded))
		{
			deleteGroup(activationPathWordFirstActivationMemoryGroupActivationPointTemp);	
			if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
			{
				deleteGroup(activationPathWordFirstParseTreeGroupActivationPointTemp);
			}
		}
	}
	#endif

	if(deleteExistingParseTreeGroupRef)
	{
		if(!(currentParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
		{
			deleteGroup(currentParseTreeGroup);
		}
	}
		
	deleteGroup(activationMemoryGroup);
	
	return result;
}
#else
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		for(int i=0; i<group->currentParseTreeGroupArray.size(); i++)
		{
			GIAtxtRelTranslatorRulesGroupParseTree* tempParseTreeGroup = (group->currentParseTreeGroupArray)[i];
			if(!(tempParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
				delete tempParseTreeGroup;
			}
		}
	}
	group->currentParseTreeGroupArray.clear();

	//CHECKTHIS;
	GIAtxtRelTranslatorRulesGroupParseTree* tempParseTreeGroup = convertNeuralNetworkGroupToParseTreeGroupNew(group);	//new GIAtxtRelTranslatorRulesGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	/*
	NO, as dont wish to copy subcomponents into tempParseTreeGroup;
	GIAtxtRelTranslatorRulesGroupParseTree* tempParseTreeGroup = copyGroup(group);
	deleteComponents(&(tempParseTreeGroup->components));
	*/
	tempParseTreeGroup->components.clear();
	group->currentParseTreeGroupArray.push_back(tempParseTreeGroup);		
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	for(int i=0; i<group->activationMemoryGroupArray.size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupActivationMemory* tempMemoryGroup = (group->activationMemoryGroupArray)[i];
		delete tempMemoryGroup;
	}
	group->activationMemoryGroupArray.clear();

	GIAtxtRelTranslatorRulesGroupActivationMemory* tempMemoryGroup = convertNeuralNetworkGroupToMemoryActivationGroupNew(group); //GIAtxtRelTranslatorRules.copyGroup(convertNeuralNetworkGroupToMemoryActivationGroup(group));
	group->activationMemoryGroupArray.push_back(tempMemoryGroup);
	#endif
	
	return result;
}
#endif
#else
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		if(group->currentParseTreeGroupTemp != NULL)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
			cout << "group->currentParseTreeGroupTemp = " << group->currentParseTreeGroupTemp->groupName << endl;
			if(!firstExecution)
			{
				if(group->currentParseTreeGroupTemp->groupName != group->groupName)
				{
					cerr << "3 (group->currentParseTreeGroupTemp->groupName != group->groupName)" << endl;
					exit(EXIT_ERROR);
				}
			}
			#endif
			
			#ifndef GIA_TXT_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED
			if(!(group->currentParseTreeGroupTemp->neuronTraced))	//fixed GIA3g11aTEMP14
			//if(!(group->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
			#endif
				//delete group->currentParseTreeGroupTemp;	//OLD: don't delete components
				deleteGroup(group->currentParseTreeGroupTemp);	//added GIA3g11aTEMP27
			#ifndef GIA_TXT_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED
			}
			#endif
		}
	}
	group->currentParseTreeGroupTemp = convertNeuralNetworkGroupToParseTreeGroupNew(group); 	//new GIAtxtRelTranslatorRulesGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	group->currentParseTreeGroupTemp->components.clear();	//added 3g1h
	
	return result;
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::restoreGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupOrig)
{
	bool result = true;
	
	deleteGroup(group->currentParseTreeGroupTemp);	//CHECKTHIS - delete newly created currentParseTreeGroupTemp 
	group->currentParseTreeGroupTemp = parseTreeGroupOrig;
	
	return result;
}


#endif
#endif






#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetNeuronBackprop(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED)
	{
		boolPointer = &(currentParseTreeGroup->neuronTraced);
	}
	/*
	else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	{
		boolPointer = &(currentParseTreeGroup->neuronSolidified);
	}
	*/
				
	if(*boolPointer)
	{
		*boolPointer = false;
		
		for(int i1=0; i1<currentParseTreeGroup->components.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i1];
			if(currentComponent->parseTreeGroupRef != NULL)
			{
				resetNeuronBackprop(currentComponent->parseTreeGroupRef, groupBoolIndexType);
			}
		}
	}
	
	return result;
}

#endif

#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, const int groupBoolIndexType)
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
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = ownerComponent->ownerGroup;
			resetNeuronForwardProp(ownerGroup, groupBoolIndexType);
		}
	}
	
	return result;
}

#endif


			


bool GIAtxtRelTranslatorSANIPropagateOperationsClass::doesRequireResetGroupActivation(GIAtxtRelTranslatorRulesComponentNeuralNetwork* Xcomponent, int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool requireResetGroupActivation = false;
	
	if(Xcomponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		if(Xcomponent->subComponents.size() > 0)
		{
			if(Xcomponent->neuronComponentConnectionActive)
			{
				GIAtxtRelTranslatorRulesComponentNeuralNetwork* subComponent = (Xcomponent->subComponents)[0];
				if(subComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					//expect repeated strings to occur consecutively
					int translatorSentenceWordIndexComparisonInt;
					if(parseSentenceReverse)
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex + 1;
					}
					else
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex - 1;
					}
					
					if(subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex != translatorSentenceWordIndexComparisonInt)
					{
						requireResetGroupActivation = true;
					}
				}
				else
				{
					if(findNextAvailableComponent(indexOfXcomponentInGroup, ownerGroup))
					{
						requireResetGroupActivation = true;
					}
					else
					{
						//FUTURE: what if still need to reset however; what if it was activated incorrectly, or already activated from a previous successful parse (e.g. <group groupName="nounMultiword" semanticRelationFunctionName="multiwordNoun">)?
					}
				}
			}
			else
			{

			}
		}
		else
		{
			cerr << "GIAtxtRelTranslatorSANIPropagateOperationsClass::doesRequireResetGroupActivation{} error: Xcomponent->subcomponents.size() == 0" << endl;
		}
	}
	else
	{
		requireResetGroupActivation = true;
	}
	
	return requireResetGroupActivation;
}
						

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::findNextAvailableComponent(int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup)
{
	bool foundNextAvailableComponent = false;
	bool findingNextAvailableComponent = true;
	
	for(int i=indexOfXcomponentInGroup; i<ownerGroup->components.size(); i++)
	{
		int i2;
		if(parseSentenceReverse)
		{
			i2 = ownerGroup->components.size()-1-i;
		}
		else
		{
			i2 = i;
		}
		
		if(findingNextAvailableComponent)
		{
			bool i2Test = false;
			if(parseSentenceReverse)
			{
				if(i2 < indexOfXcomponentInGroup)
				{
					i2Test = true;
				}
			}
			else
			{
				if(i2 > indexOfXcomponentInGroup)
				{
					i2Test = true;
				}
			}
			
			if(i2Test)
			{
				GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (ownerGroup->components)[i2];
				if(!(component->neuronComponentConnectionActive))
				{
					if(!component->optional)
					{
						findingNextAvailableComponent = false;
						foundNextAvailableComponent = true;
					}
				}
				else
				{
					findingNextAvailableComponent = false;
				}
			}
		}
	}
	
	return foundNextAvailableComponent;
}


bool GIAtxtRelTranslatorSANIPropagateOperationsClass::currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
	#else
	if(currentWord->POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
	#endif
	{
		result = true;
	}
	
	return result;
}


int GIAtxtRelTranslatorSANIPropagateOperationsClass::convertWordIndexToSentenceEntityIndex(int wordIndex)
{
	int sentenceEntityIndex = wordIndex + GIA_TXT_REL_TRANSLATOR_SANI_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sentenceEntityIndex;
}
int GIAtxtRelTranslatorSANIPropagateOperationsClass::convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex)
{
	int wordIndex = sentenceEntityIndex - GIA_TXT_REL_TRANSLATOR_SANI_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return wordIndex;
}


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::isWordPOStypeNoun(int wordPOStype)
{
	bool result = false;
	
	if(wordPOStype == GIA_SHARED_POS_TYPE_NOUN)	//what about GIA_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE etc from GIAsemRelTranslatorDefs.hpp?
	{
		result = true;
	}
	//sync with (ie based on) <group groupName="nounMultiwordPropernoun"> [includes: <group groupName="nounMultiwordName">]
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FIRST_MALE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FIRST_FEMALE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FAMILY)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_PLACE)
	{
		result = true;
	}	
	//sync with (ie based on) <group groupName="pronoun">		
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_INDEFINITE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_REFLEXIVE)
	{
		result = true;
	}	
	
	else if(wordPOStype == GIA_SHARED_POS_TYPE_VERB)
	{
		result = true;
	}
	
	return result;
}
#endif

#endif


bool GIAtxtRelTranslatorSANIPropagateOperationsClass::deleteGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::deleteComponents(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i];
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			deleteComponents(&(currentComponent->subComponents));
		}
		delete currentComponent;
	}
	
	return result;
}	

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::deleteGroup(GIAtxtRelTranslatorRulesGroupParseTree* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::deleteComponents(vector<GIAtxtRelTranslatorRulesComponentParseTree*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (*components)[i];
		delete currentComponent;
	}
	
	return result;
}	

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::deleteGroup(GIAtxtRelTranslatorRulesGroupActivationMemory* group)
{
	deleteComponents(&(group->components));
	delete group;
}	



			
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::printGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, int layer)
{
	for(int i=0; i<group->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (group->components)[i];
		printComponent(component, layer);
	}	
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::printComponent(GIAtxtRelTranslatorRulesComponentParseTree* component, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
	if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SHAREDvars.convertIntToString(component->wordPOStypeInferred) + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->word + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenLayer + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenClass + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenType + CHAR_TAG_CLOSE;
		}
		
		if(component->candidateStringMatch != NULL)
		{
			componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
		}
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupTypeRefName + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupRefName + STRING_SPACE + CHAR_TAG_CLOSE;	
		cout << componentTagRegenerated << endl;
	}
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::printComponent(GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
	if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->wordPOStype + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->word + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenLayer + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenClass + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenType + CHAR_TAG_CLOSE;
		}
		
		if(component->candidateStringMatch != NULL)
		{
			componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
		}
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAtxtRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupTypeRefName + STRING_SPACE + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupRefName + STRING_SPACE + CHAR_TAG_CLOSE;	
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_or + STRING_SPACE + CHAR_TAG_CLOSE;
		cout << componentTagRegenerated << endl;
		for(int i=0; i<component->subComponents.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	else if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat + STRING_SPACE + CHAR_TAG_CLOSE;
		cout << componentTagRegenerated << endl;
		for(int i=0; i<component->subComponents.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::printParseTreeDebugIndentation(int layer)
{
	bool result = true;
	
	for(int i=0; i<layer; i++)
	{
		cout << "\t";
	}
	
	return result;
}

int GIAtxtRelTranslatorSANIPropagateOperationsClass::calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset)
{	
	int minIndexOfMatchesFound = -1;
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(w);
		if(currentWord->alreadyFoundMatch)
		{	
			minIndexOfMatchesFound = w;
		}
	}
	
	return minIndexOfMatchesFound;
}


void GIAtxtRelTranslatorSANIPropagateOperationsClass::deleteParseTree(GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToDelete, int level)
{
	if(parseTreeGroupToDelete != NULL)
	{
		for(int i=0; i<parseTreeGroupToDelete->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (parseTreeGroupToDelete->components)[i];

			if(currentComponent->parseTreeGroupRef != NULL)
			{
				deleteParseTree(currentComponent->parseTreeGroupRef, level+1);
			}
		}

		deleteGroup(parseTreeGroupToDelete);
	}
}

GIAtxtRelTranslatorRulesGroupParseTree* GIAtxtRelTranslatorSANIPropagateOperationsClass::replicateParseTree(GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToReplicate, int level)
{
	bool result = false;

	GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNew = GIAtxtRelTranslatorRules.copyGroup(parseTreeGroupToReplicate);
	//parseTreeGroupNew->components.clear();
	
	for(int i=0; i<parseTreeGroupToReplicate->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (parseTreeGroupToReplicate->components)[i];
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{
			GIAtxtRelTranslatorRulesGroupParseTree* componentParseTreeGroupNew = replicateParseTree(currentComponent->parseTreeGroupRef, level+1);
			(parseTreeGroupNew->components)[i]->parseTreeGroupRef = componentParseTreeGroupNew;
			//parseTreeGroupNew->components.push_back(componentParseTreeGroupNew);
		}
	}
	
	return parseTreeGroupNew;
}

GIAtxtRelTranslatorRulesGroupParseTree* GIAtxtRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToParseTreeGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	GIAtxtRelTranslatorRulesGroup* groupBase = group;
	return (static_cast<GIAtxtRelTranslatorRulesGroupParseTree*>(groupBase));
}
GIAtxtRelTranslatorRulesComponentParseTree* GIAtxtRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkComponentToParseTreeComponent(GIAtxtRelTranslatorRulesComponentNeuralNetwork* component)
{
	GIAtxtRelTranslatorRulesComponent* componentBase = component;
	return (static_cast<GIAtxtRelTranslatorRulesComponentParseTree*>(componentBase));
}
GIAtxtRelTranslatorRulesGroupActivationMemory* GIAtxtRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToMemoryActivationGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	return (static_cast<GIAtxtRelTranslatorRulesGroupActivationMemory*>(group));
}


GIAtxtRelTranslatorRulesGroupParseTree* GIAtxtRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToParseTreeGroupNew(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	GIAtxtRelTranslatorRulesGroup* groupBase = group;
	GIAtxtRelTranslatorRulesGroupParseTree* groupNew = new GIAtxtRelTranslatorRulesGroupParseTree();
	GIAtxtRelTranslatorRulesGroup* groupNewBase = groupNew;
	*groupNewBase = *groupBase;
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	groupNew->groupRef = group;
	#endif
	return groupNew;
}
GIAtxtRelTranslatorRulesComponentParseTree* GIAtxtRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkComponentToParseTreeComponentNew(GIAtxtRelTranslatorRulesComponentNeuralNetwork* component)	//why isnt this required?
{
	GIAtxtRelTranslatorRulesComponent* componentBase = component;
	GIAtxtRelTranslatorRulesComponentParseTree* componentNew = new GIAtxtRelTranslatorRulesComponentParseTree();
	GIAtxtRelTranslatorRulesComponent* componentNewBase = componentNew;
	*componentNewBase = *componentBase;
	return componentNew;
}
GIAtxtRelTranslatorRulesGroupActivationMemory* GIAtxtRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToMemoryActivationGroupNew(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{
	GIAtxtRelTranslatorRulesGroupActivationMemory* groupNew = new GIAtxtRelTranslatorRulesGroupActivationMemory();
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupNewBase = groupNew;
	*groupNewBase = *group;
	groupNew->components.clear();
	GIAtxtRelTranslatorRules.copyComponents(&(group->components), &(groupNew->components));
	return groupNew;
}


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetGroupOptimumPathway(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool result = true;
	
	resetComponentsOptimumPathway(&(group->components));
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::resetComponentsOptimumPathway(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i1];
		
		currentComponent->optimumPathwayWordList.clear();
		
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			resetComponentsOptimumPathway(&(currentComponent->subComponents));
		}
	}
	
	return result;
}
#endif


#ifdef GIA_TXT_REL_TRANSLATOR_SANI_FREE_MEMORY
bool GIAtxtRelTranslatorSANIPropagateOperationsClass::initialiseParseTreeGroupList(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIAtxtRelTranslatorRulesGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];
			parseTreeGroupList->push_back(group->currentParseTreeGroupTemp);
			//cout << "initialiseParseTreeGroupList" << endl;
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::deinitialiseParseTreeGroupList(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIAtxtRelTranslatorRulesGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i2=0; i2<parseTreeGroupList->size(); i2++)
	{
		GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupTemp = (*parseTreeGroupList)[i2];
		deleteGroup(parseTreeGroupTemp);
	}
	parseTreeGroupList->clear();
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];
			group->currentParseTreeGroupTemp = NULL;
		}
	}
	
	return result;
}
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
int GIAtxtRelTranslatorSANIPropagateOperationsClass::calculateCoverage(GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	int activatedNeuronWordIndexCoverage = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex - activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex + 1;
	return activatedNeuronWordIndexCoverage;
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::traceBackpropNeuralNetwork(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentNeuron, int level, int previousComponentIndex, int previousComponentType)
{
	printParseTreeDebugIndentation(level);
	if(isNeuronString(currentNeuron))
	{
		cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->wordPOStype = " << currentNeuron->wordPOStype << endl;
	}
	else
	{
		cout << "GIAtxtRelTranslatorSANIPropagateOperationsClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;
	}
	
	for(int i=0; i<currentNeuron->components.size(); i++)
	{
		int c = i;
		/*
		int c;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c = currentNeuron->components.size()-1-i;
		}
		else
		{
			c = i;
		}
		*/
		
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = (currentNeuron->components)[c];
					
		for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
			traceBackpropNeuralNetwork(groupSource, level+1, i, component->componentType);
		}	
	}
		
}

bool GIAtxtRelTranslatorSANIPropagateOperationsClass::isNeuronString(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentNeuron)
{
	bool result = false;
	if(currentNeuron->groupTypeIsString)
	{
		//groupType==string detected (FUTURE: set this)
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}
	
#endif


