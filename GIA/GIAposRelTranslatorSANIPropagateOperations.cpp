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
 * File Name: GIAposRelTranslatorSANIPropagateOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k9b 07-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Operations - generic functions
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorSANIPropagateOperations.hpp"


#ifdef GIA_POS_REL_TRANSLATOR_SANI

static bool parseSentenceReverse;

#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
static bool firstExecution;
#endif

void GIAposRelTranslatorSANIPropagateOperationsClass::setParseSentenceReverseStrict(const bool parseSentenceReverseNew, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	parseSentenceReverse = parseSentenceReverseNew;
	forwardPropogationSentenceData->parseSentenceReverse = parseSentenceReverseNew;
}


void GIAposRelTranslatorSANIPropagateOperationsClass::setParseSentenceReverse(const bool normalDirection, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool parseSentenceReverseNew = false;
	if(normalDirection)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
		parseSentenceReverseNew = true;
		#else
		parseSentenceReverseNew = false;
		#endif
	}
	else
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
		parseSentenceReverseNew = false;
		#else
		parseSentenceReverseNew = true;
		#endif		
	}
	parseSentenceReverse = parseSentenceReverseNew;
	forwardPropogationSentenceData->parseSentenceReverse = parseSentenceReverseNew;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(testComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, lastActiveComponent, &existingActivationFoundNOTUSED);
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(testComponent, components, &existingActivationFoundNOTUSED);
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(testComponent, components, forwardPropogationWordData, repeatDetected, &existingActivationFoundNOTUSED);
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound)
{
	bool componentWordConnectivityTestsNOTUSED = false;
	bool missingStartComponentsFoundNOTUSED = false;
	bool missingOrVariableStartComponentFoundNOTUSED = false;
	int numberOfInactiveComponentsRemaining = 0;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(testComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, lastActiveComponent, existingActivationFound, &missingStartComponentsFoundNOTUSED, componentWordConnectivityTestsNOTUSED, &missingOrVariableStartComponentFoundNOTUSED, &numberOfInactiveComponentsRemaining);
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork** previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork** lastActiveComponent, bool* existingActivationFound, bool* missingStartComponentsFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, int* numberOfInactiveComponentsRemaining)
{	
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	*numberOfInactiveComponentsRemaining = 0;
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
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
		
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (*components)[c];

		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
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
			cout << "component->hasSubComponents = " << GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component) << endl;
			*/
			
			if(component == testComponent)
			{
				//cout << "(component == testComponent)" << endl;

				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
				if(*previousActiveComponent == NULL)
				{
					*firstActiveComponentInGroup = true;
				}
				#endif
				
				stillParsingActiveComponents = false;

				#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
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
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
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
						if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)	//if the component contains repeats, then accept it if it is already active
						{
							//sequential activation found
							sequentialActivationFound = true;
						}
						else
						{

							//component already activated
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
							if(i == 0)	//start component in group
							{
								bool allowComponentReset = true;
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
								if(componentWordConnectivityTests)
								{
								#endif
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
									if(consecutiveSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, components, component))
									{
										if(findValidDualLowerLevelConnection(forwardPropogationSentenceData, forwardPropogationWordData, components, component, true))
										{
											if(repeatedSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, components, component))
											{
												allowComponentReset = false;
											}
										}
									}
									#else
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
									if(consecutiveSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, components, component))
									{
										//cout << "!allowComponentReset: consecutiveSequenceDetected - component->ownerGroup->groupIndex = " << component->ownerGroup->groupIndex << endl;
										//cout << "!allowComponentReset; consecutiveSequenceDetected" << endl;
										allowComponentReset = false;
									}
									#endif
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
									if(findValidDualLowerLevelConnection(forwardPropogationSentenceData, forwardPropogationWordData, components, component, true))
									{
										//cout << "!allowComponentReset; findValidDualLowerLevelConnection" << endl;
										allowComponentReset = false;
									}
									#endif
									#endif
									/*
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
									if(!(component->ownerGroup->neuronComponentConnectionActive))
									{
										if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, forwardPropogationWordData, component, component->ownerGroup))
										{
											allowComponentReset = true;
										}
									}
									#endif
									*/
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
								}
								else
								{
									//always reset, as existing first activated component is not connected (by wordIndices) to prospective activated next component
								}
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
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
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
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
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
				}
				#endif
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				}
				#endif
			}
			else
			{
				//cout << "!(component == testComponent)" << endl;

				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
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
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
					//note it doesn't matter if the start component is active or inactive. Even if it has been activated miscellaneously (ie without maintaining word index connectivity), it can be reset.
					if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
					{
						if(i == 0)
						{
							if(component->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//added GIA3j5aTEMP65
							{
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
								bool validDualLowerLevelConnectionFound = false;
								if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
								{
									//cout << "recordActivatedNeuronWithMaxWordIndexCoverage" << endl;
									validDualLowerLevelConnectionFound = findValidDualLowerLevelConnection(forwardPropogationSentenceData, forwardPropogationWordData, components, component, false);
									if(validDualLowerLevelConnectionFound)
									{
										cout << "validDualLowerLevelConnectionFound" << endl;
									}
								}
								if(!validDualLowerLevelConnectionFound)
								{
								#endif
									*missingOrVariableStartComponentFound = true;
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
								}
								#endif	
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
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
						if(forwardPropogationSignalData->firstIndexInSequence == 0)
						{
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
							if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage) || (i != 0) || (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING))	//ie || !(*missingOrVariableStartComponentFound)
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
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
								if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage) || (i != 0) || (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING))	//ie || !(*missingOrVariableStartComponentFound)
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
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PERFORM_AUTORESET_OF_ACTIVATIONS
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
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
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
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		if(!(*missingStartComponentsFound))
		{
		#endif	
		#endif
			if(*numberOfInactiveComponentsRemaining == 0)
			{
				*activationSequenceCompleted = true;
			}
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS	
		}
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		}
		#endif	
		#endif
		//cout << "numberOfInactiveComponentsRemaining = " << *numberOfInactiveComponentsRemaining << endl;
	}

	return sequentialActivationFound;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
bool GIAposRelTranslatorSANIPropagateOperationsClass::upperNeuronContainsWordIndexOfProspectiveComponent(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorRulesComponentNeuralNetwork* component, GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool result = false;
	
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
		GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
		cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	
		//method2;
		GIAposRelTranslatorRulesComponentNeuralNetwork* previousComponent = NULL;
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
		GIAposRelTranslatorRulesGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
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

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
bool GIAposRelTranslatorSANIPropagateOperationsClass::repeatedSequenceDetected(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorRulesComponentNeuralNetwork* component)
{
	bool result = true;
	
	int componentWordIndexCoverage = component->ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex - component->ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex + 1;

	vector<GIApreprocessorPlainTextWord*> componentWordArray;
	vector<GIApreprocessorPlainTextWord*> componentWordArray2;
	
	int componentWmin = INT_DEFAULT_VALUE;
	int componentWmax = INT_DEFAULT_VALUE;
	int componentWmin2 = INT_DEFAULT_VALUE;
	int componentWmax2 = INT_DEFAULT_VALUE;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		componentWmax = forwardPropogationWordData->w+1+componentWordIndexCoverage;
		componentWmin = forwardPropogationWordData->w+1;
		componentWmax2 = forwardPropogationWordData->w;
		componentWmin2 = forwardPropogationWordData->w - componentWordIndexCoverage;
	}
	else
	{
		componentWmin = forwardPropogationWordData->w-1-componentWordIndexCoverage;
		componentWmax = forwardPropogationWordData->w-1;
		componentWmin2 = forwardPropogationWordData->w;
		componentWmax2 = forwardPropogationWordData->w + componentWordIndexCoverage;
	}
	
	for(int w = componentWmin; w<=componentWmax; w++)
	{
		GIApreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];	
		componentWordArray.push_back(currentWord);
	}
	for(int w = componentWmin2; w<=componentWmax2; w++)
	{
		GIApreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];	
		componentWordArray2.push_back(currentWord);
	}
	for(int w = componentWmin; w<=componentWmax; w++)
	{
		//must sync code with GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkIntro !currentWordAmbiguous exceptions
		
		if(!currentWordAmbiguous(componentWordArray[w]) && !currentWordAmbiguous(componentWordArray2[w]))
		{
			if(componentWordArray[w]->unambiguousPOSindex != componentWordArray2[w]->unambiguousPOSindex)
			{
				result = false;
			}
		}
		else if(currentWordAmbiguous(componentWordArray[w]))
		{
			int wordPOStype = INT_DEFAULT_VALUE;
			bool pass = getWordPOStypeFromAmbiguousWord(componentWordArray[w], &wordPOStype);
			if(pass)
			{
				if(componentWordArray2[w]->unambiguousPOSindex != wordPOStype)
				{
					result = false;
				}
			}
			else
			{
				result = false;
			}
		}		
		else if(currentWordAmbiguous(componentWordArray2[w]))
		{
			int wordPOStype2 = INT_DEFAULT_VALUE;
			bool pass = getWordPOStypeFromAmbiguousWord(componentWordArray2[w], &wordPOStype2);
			if(pass)
			{
				if(componentWordArray2[w]->unambiguousPOSindex != wordPOStype2)
				{
					result = false;
				}
			}
			else
			{
				result = false;
			}
		}
		else
		{
			//both words are ambiguous

			int wordPOStype1 = INT_DEFAULT_VALUE;
			bool pass1 = getWordPOStypeFromAmbiguousWord(componentWordArray[w], &wordPOStype1);
			int wordPOStype2 = INT_DEFAULT_VALUE;
			bool pass2 = getWordPOStypeFromAmbiguousWord(componentWordArray2[w], &wordPOStype2);
			if(pass1 && pass2)
			{
				if(wordPOStype1 != wordPOStype2)
				{
					result = false;
				}
			}
			else
			{
				result = false;
			}
		}
	}	
							
	return result;
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
bool GIAposRelTranslatorSANIPropagateOperationsClass::consecutiveSequenceDetected(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorRulesComponentNeuralNetwork* component)
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
	else
	{
		cout << "GIAposRelTranslatorSANIPropagateOperationsClass::consecutiveSequenceDetected warning: !(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)" << endl;
	}
							
	return result;
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
bool GIAposRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorRulesComponentNeuralNetwork* component, bool assumeFirstComponentActive)
{	
	bool validDualLowerLevelConnectionFound = false;
	bool secondComponentFound = false;
	bool firstComponentFound = false;
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

		GIAposRelTranslatorRulesComponentNeuralNetwork* component2 = (*components)[c2];
		
		if(!firstComponentFound)
		{
			if(assumeFirstComponentActive)
			{
				if(component2->neuronComponentConnectionActive)
				{
					firstComponentFound = true;
				}
			}
			else
			{
				if(!firstComponentFound)
				{
					firstComponentFound = true;
				}	
			}
		}
		else
		{
			if(!secondComponentFound)
			{
				if(!(component2->neuronComponentConnectionActive) || !assumeFirstComponentActive)
				{
					secondComponentFound = true;
					for(int l2=0; l2<component2->ANNbackGroupConnectionList.size(); l2++)
					{
						GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource2 = component2->ANNbackGroupConnectionList[l2];

						for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
						{
							GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];

							if(groupSource2 == groupSource)
							{
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_AND_SATISIFIES_WORD_INDEX
								if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == forwardPropogationWordData->wordReference->translatorSentenceWordIndex-1)	//or component->wordIndex
								{
								#endif
									validDualLowerLevelConnectionFound = true;
									cout << "validDualLowerLevelConnectionFound" << endl;
									//cout << "\n\n\n\n GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: GIAposRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound" << endl;
									//exit(EXIT_ERROR);
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_AND_SATISIFIES_WORD_INDEX
								}
								#endif
							}
							else
							{	
								/*
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
								if(findGroup2InForwardConnectionBranchOfGroup1(groupSource, groupSource2))
								{
									validDualLowerLevelConnectionFound = true;
									cout << "\n\n\n\n warning: GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: GIAposRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound1 - CHECKTHIS" << endl;
									//exit(EXIT_ERROR);
								}
								if(findGroup2InForwardConnectionBranchOfGroup1(groupSource2, groupSource))
								{
									validDualLowerLevelConnectionFound = true;
									cout << "\n\n\n\n warning: GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: GIAposRelTranslatorSANIPropagateOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound2 - CHECKTHIS" << endl;
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
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
bool GIAposRelTranslatorSANIPropagateOperationsClass::findGroup2InForwardConnectionBranchOfGroup1(GIAposRelTranslatorRulesGroupNeuralNetwork* group2, GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroup)
{
	bool result = false;
	
	for(int i=0; i<currentGroup->ANNfrontComponentConnectionList.size(); i++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (currentGroup->ANNfrontComponentConnectionList)[i];
		GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
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

							
bool GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, bool* existingActivationFound)
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
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (*components)[c];
		if(component == testComponent)
		{
			if(component->neuronComponentConnectionActive)
			{
				//component already activated
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
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
bool GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFound)
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
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (*components)[c];
		if(component == testComponent)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			bool pass = true;
			if(testComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
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
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
					//if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == forwardPropogationWordData->wordReference->translatorSentenceWordIndex)
					//{
						*existingActivationFound = true;
					//}
					#endif
					*/
				}
				sequentialActivationFound = true;
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			}
			#endif
		}
		else
		{
			cerr << "GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady{} error: repeat sequences can contain only 1 component" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return sequentialActivationFound;
}



#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
bool GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper(GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = false;

	bool existingActivationFoundNOTUSED = false;

	GIAposRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		int lastActiveComponentInParseTreeIndex = activationPathWordCurrentParseTreeGroupOwner->components.size() - 1;
		GIAposRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = (activationPathWordCurrentParseTreeGroupOwner->components)[lastActiveComponentInParseTreeIndex];
		GIAposRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
		
		GIAposRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED = NULL;
		result = componentWordConnectivityTests(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED, forwardPropogationWordData, existingActivationFoundNOTUSED);
	}
	else
	{
		result = true;
	}
	
	return result;
}
#else
bool GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	return componentWordConnectivityTestsWrapper(ownerGroup->currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFound);
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;

	GIAposRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;

	GIAposRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
	GIAposRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef = NULL;	
	GIAposRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = NULL;
	GIAposRelTranslatorRulesGroupParseTree* ownerGroupParseTreeGroup = currentParseTreeGroupTemp;
	
	/*
	cout << "GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	if(ownerGroup->currentParseTreeGroupTemp == NULL)
	{
		cout << "GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper ownerGroup->currentParseTreeGroupTemp == NULL" << endl;
	}
	cout << "GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTestsWrapper ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
	*/
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
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

			GIAposRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;

			GIAposRelTranslatorRulesComponentParseTree* previousActiveComponentInParseTree = NULL;
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
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	}
	#endif
			
	return result;
}
#endif

bool GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests(GIAposRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree, GIAposRelTranslatorRulesGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;
		
	//cout << "\t 1GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests{}: " << endl;

	int wordIndexMax = -1;
	int wordIndexMin = REALLY_LARGE_INT;
	GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMin;
	GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax;
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
			//cout << "GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests error{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true)" << endl;	
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests pass{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true)" << endl;	
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
			//cout << "GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests error{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false) && " << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAposRelTranslatorSANIPropagateOperationsClass::componentWordConnectivityTests pass{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false) && " << endl;
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMin = SHAREDvars.minInt(wordIndexMin, wordIndexMinMin);

	//#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA9
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
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
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
	else if(existingActivationFound)
	{
	
		//assumes GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION:
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE_BIO
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
bool GIAposRelTranslatorSANIPropagateOperationsClass::getMinMaxWordIndexInParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA6
	if(findMaxOrMinWordIndex)
	{
		printParseTreeDebugIndentation(level);
		cout << "GIAposRelTranslatorSANIPropagateOperationsClass::getMinMaxWordIndexInParseTree: currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
	}
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
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
		GIAposRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA6
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



#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
bool GIAposRelTranslatorSANIPropagateOperationsClass::calculatePerformanceWeightOfParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* maxWeight)
{
	bool result;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
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
	resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	#endif
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::setPerformanceWeightOfParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int maxWeight)
{
	bool result;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
	currentParseTreeGroup->parseTreeTotalWeight = maxWeight;
	#else
	currentParseTreeGroup->parseTreeMaxWeight = maxWeight;
	#endif
	//cout << "maxWeight = " << *maxWeight << endl;
	#else
	cerr << "GIAposRelTranslatorSANIPropagateOperationsClass::setPerformanceWeightOfParseTree requires GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_WEIGHT_BIO" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE

#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE

bool GIAposRelTranslatorSANIPropagateOperationsClass::updatePerformance(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	bool topLevelGroup = GIAposRelTranslatorRulesGroupClassObject.isTopLevelGroupType(currentParseTreeGroup->groupTypeName, currentParseTreeGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
	if(topLevelGroup)
	{
		result = updatePerformanceGroup(currentParseTreeGroup, forwardPropogationSentenceData, layer);
	}
	
	return result;
}

//doesn't perform topLevelGroup check;
bool GIAposRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	//cout << "updatePerformanceGroup" << endl;
			 
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_BIO
	forwardPropogationSentenceData->performance = currentParseTreeGroup->parseTreeMaxWordIndex - currentParseTreeGroup->parseTreeMinWordIndex + 1;
	#else
	int performanceTemp = 0;
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION
	cout << "GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION: GIAposRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup, traceBackpropParseTree:" << endl;
	bool print = true;
	#else
	bool print = false;
	#endif
	bool performancePreprocess = true;
	if(!traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceTemp, forwardPropogationSentenceData->sentenceContents))
	{
		cout << "GIAposRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup fail #1" << endl;
		result = false;
	}
	resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
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
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	if(result)
	{
		if(forwardPropogationSentenceData->performance != forwardPropogationSentenceData->sentenceContents->size())
		{
	#endif
			cout << "GIAposRelTranslatorSANIPropagateOperationsClass::updatePerformanceGroup fail #2" << endl;
			result = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
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

bool GIAposRelTranslatorSANIPropagateOperationsClass::traceBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool calculateMaxWeight = false;
	int maxWeightNOTUSED = 0;
	return traceBackpropParseTree(currentParseTreeGroup, level, print, performancePreprocess, performance, sentenceContents, calculateMaxWeight, &maxWeightNOTUSED);

}
bool GIAposRelTranslatorSANIPropagateOperationsClass::traceBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight)
{
	bool result = true;
	
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif
	
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(calculateMaxWeight)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
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
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
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
			//#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
			printParseTreeDebugIndentation(level);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupRef->groupIndex = " << currentParseTreeGroup->groupRef->groupIndex << endl;
			#else
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupTypeName = " << currentParseTreeGroup->groupTypeName << endl;		//<< ", parse word (providing semanticRelationReturnEntity) = ?"
			#endif
			//#endif
		}
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAposRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
			
			//printComponent(currentComponent, level);
			
			#ifndef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			if(performancePreprocess)
			{	
				//cout << "currentComponent->candidateStringMatch = " << currentComponent->candidateStringMatch << endl;
				if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
				{					
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
					if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//redundant?
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
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
						}
					}
					#endif
				}
			}
			#endif
	
			if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				if(print)
				{
					printComponent(currentComponent, level+1);	//+1 added @GIA3j5aTEMP66
					/*
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
					if(currentComponent->parseTreeGroupRef != NULL)
					{	
						printParseTreeDebugIndentation(level);
						cout << "\tcurrentComponent->parseTreeGroupRef->groupRef->groupIndex = " << currentComponent->parseTreeGroupRef->groupRef->groupIndex << endl;
					}
					#endif
					*/
				}
			}
			#ifndef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{							
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA
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
			#ifndef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}

#endif





								
								
#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
bool GIAposRelTranslatorSANIPropagateOperationsClass::isSentenceWordDataFullyConnected(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//forwardPropogationWordData->previousWordConnections.push_back(wPrevious);
	bool result = true;
	
	int numberOfWords = forwardPropogationSentenceData->forwardPropogationWordDataArray.size();
	vector<bool> previousWordConnectionsFound(numberOfWords, false);
	previousWordConnectionsFound[numberOfWords-1] = true;	//set last word as found 
	GIAposRelTranslatorSANIForwardPropogationWordData* lastWordData = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[numberOfWords-1]);
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

void GIAposRelTranslatorSANIPropagateOperationsClass::isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAposRelTranslatorSANIForwardPropogationWordData* currentWordData)
{
	for(int i = 0; i<currentWordData->previousWordConnections.size(); i++)
	{
		GIAposRelTranslatorSANIForwardPropogationWordData* previousWordData = (currentWordData->previousWordConnections)[i];
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
		cout << "previousWordConnectionsFound == true: previousWordData->w = " << previousWordData->w << endl;
		#endif
		(*previousWordConnectionsFound)[previousWordData->w] = true;
		isSentenceWordDataFullyConnected(previousWordConnectionsFound, previousWordData);
	}
}
#endif



#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool GIAposRelTranslatorSANIPropagateOperationsClass::componentTests1(GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
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
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
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
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_INTERPRET_PAST_AND_PAST_PARTICIPLE_THE_SAME
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

#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAposRelTranslatorSANIPropagateOperationsClass::componentTests2(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
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

bool GIAposRelTranslatorSANIPropagateOperationsClass::getFirstWordInParseTree(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceWordIndexMin, int* numComponentsFound)
{
	bool result = false;
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		//if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
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

bool GIAposRelTranslatorSANIPropagateOperationsClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype)
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
bool GIAposRelTranslatorSANIPropagateOperationsClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype)
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

bool GIAposRelTranslatorSANIPropagateOperationsClass::checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex)
{
	bool result = false;	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
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




bool GIAposRelTranslatorSANIPropagateOperationsClass::resetAllNeurons(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];

			bool* boolPointer = NULL;
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED)
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

bool GIAposRelTranslatorSANIPropagateOperationsClass::resetAllNeuronComponents(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	//cout << "GIAposRelTranslatorRulesGroupTypes->size() = " << GIAposRelTranslatorRulesGroupTypes->size() << endl;
	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];

			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE)
			{
				//cout << "resetGroupActivation = " << group->groupName << endl; 
				resetGroupActivation(group);
			}
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
			
			#else
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF)
			{
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
				cout << "group = " << group->groupName << endl;
				printGroup(group, 1);
				#endif
				
				resetGroupParseTreeGroupRef(group, true);
			}
			#endif
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED)
			{
				//cout << "group->neuronPropagated = false" << endl;
				group->neuronPropagated = false;
			}
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE)
			{
				group->neuronPropagatedSave = group->neuronPropagated;
			}
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET)
			{
				group->neuronPropagated = group->neuronPropagatedSave;
			}
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED)
			{
				group->neuronProcessed = false;
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
			{
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_BIO
				group->solidified = false;
				#else
				group->currentParseTreeGroupTemp->solidified = false;
				#endif
			}
			#endif
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET)
			{
				resetGroupOptimumPathway(group);
			}
			#endif
	
		}
	}
	
	return result;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS
bool GIAposRelTranslatorSANIPropagateOperationsClass::isLastComponentFuzzy(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool lastComponentIsFuzzy = false;

	int indexOfLastComponentInGroup = group->components.size()-1;
	GIAposRelTranslatorRulesComponentNeuralNetwork* lastComponent = (group->components)[indexOfLastComponentInGroup];
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_TREAT_OR_AS_FUZZY
	if(lastComponent->optional || GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(lastComponent))
	#else
	if(lastComponent->optional || (lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
	#endif
	{
		lastComponentIsFuzzy = true;
	}
		
	return lastComponentIsFuzzy;	
}
#endif

bool GIAposRelTranslatorSANIPropagateOperationsClass::resetGroupActivation(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool result = true;

	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	resetGroupParseTreeGroupRef(group, false);	//leave existing parseTreeGroupRef in memory
	#endif
	*/
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA2
	//cout << "resetGroupActivation: group->groupName = " << group->groupName << ", group->groupTypeName = " << group->groupTypeName << endl;
	#endif
	
	group->neuronActive = false;	//not used
	resetComponentsActivation(&(group->components));
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::resetComponentsActivation(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i1];
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_RESET_COMPONENT_NEURONCOMPONENTCONNECTIONACTIVEWORDRECORD
		currentComponent->neuronComponentConnectionActiveWordRecord = NULL;
		#endif
		currentComponent->neuronComponentConnectionActive = false;
		
		if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			resetComponentsActivation(&(currentComponent->subComponents));
		}
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::restoreGroupActivation(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig)
{	
	bool result = true;

	group->neuronActive = groupOrig->neuronActive;
	restoreComponentsActivation(&(group->components), &(groupOrig->components));
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::restoreComponentsActivation(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* componentsOrig)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i1];
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponentOrig = (*componentsOrig)[i1];
		currentComponent->neuronComponentConnectionActive = currentComponentOrig->neuronComponentConnectionActive;
		if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			restoreComponentsActivation(&(currentComponent->subComponents), &(currentComponentOrig->subComponents));
		}
	}
	
	return result;
}




#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
bool GIAposRelTranslatorSANIPropagateOperationsClass::resetGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupActivationMemory* activationMemoryGroup, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;	
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP			
	for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
	{
		GIAposRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray[i];
		GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroupActivationPointTemp = currentParseTreeGroup->activationPathWordFirstParseTreeGroupActivationPointArray[i];
		
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
bool GIAposRelTranslatorSANIPropagateOperationsClass::resetGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		for(int i=0; i<group->currentParseTreeGroupArray.size(); i++)
		{
			GIAposRelTranslatorRulesGroupParseTree* tempParseTreeGroup = (group->currentParseTreeGroupArray)[i];
			if(!(tempParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
				delete tempParseTreeGroup;
			}
		}
	}
	group->currentParseTreeGroupArray.clear();

	//CHECKTHIS;
	GIAposRelTranslatorRulesGroupParseTree* tempParseTreeGroup = convertNeuralNetworkGroupToParseTreeGroupNew(group);	//new GIAposRelTranslatorRulesGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	/*
	NO, as dont wish to copy subcomponents into tempParseTreeGroup;
	GIAposRelTranslatorRulesGroupParseTree* tempParseTreeGroup = copyGroup(group);
	deleteComponents(&(tempParseTreeGroup->components));
	*/
	tempParseTreeGroup->components.clear();
	group->currentParseTreeGroupArray.push_back(tempParseTreeGroup);		
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	for(int i=0; i<group->activationMemoryGroupArray.size(); i++)
	{
		GIAposRelTranslatorRulesGroupActivationMemory* tempMemoryGroup = (group->activationMemoryGroupArray)[i];
		delete tempMemoryGroup;
	}
	group->activationMemoryGroupArray.clear();

	GIAposRelTranslatorRulesGroupActivationMemory* tempMemoryGroup = convertNeuralNetworkGroupToMemoryActivationGroupNew(group); //GIAposRelTranslatorRules.copyGroup(convertNeuralNetworkGroupToMemoryActivationGroup(group));
	group->activationMemoryGroupArray.push_back(tempMemoryGroup);
	#endif
	
	return result;
}
#endif
#else
bool GIAposRelTranslatorSANIPropagateOperationsClass::resetGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		if(group->currentParseTreeGroupTemp != NULL)
		{
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
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
			
			#ifndef GIA_POS_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED
			if(!(group->currentParseTreeGroupTemp->neuronTraced))	//fixed GIA3g11aTEMP14
			//if(!(group->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
			#endif
				//delete group->currentParseTreeGroupTemp;	//OLD: don't delete components
				deleteGroup(group->currentParseTreeGroupTemp);	//added GIA3g11aTEMP27
			#ifndef GIA_POS_REL_TRANSLATOR_SANI_DONT_SET_NEURON_TRACED
			}
			#endif
		}
	}
	group->currentParseTreeGroupTemp = convertNeuralNetworkGroupToParseTreeGroupNew(group); 	//new GIAposRelTranslatorRulesGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	group->currentParseTreeGroupTemp->components.clear();	//added 3g1h
	
	return result;
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::restoreGroupParseTreeGroupRef(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupOrig)
{
	bool result = true;
	
	deleteGroup(group->currentParseTreeGroupTemp);	//CHECKTHIS - delete newly created currentParseTreeGroupTemp 
	group->currentParseTreeGroupTemp = parseTreeGroupOrig;
	
	return result;
}


#endif
#endif






#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE

bool GIAposRelTranslatorSANIPropagateOperationsClass::resetNeuronBackprop(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED)
	{
		boolPointer = &(currentParseTreeGroup->neuronTraced);
	}
	/*
	else if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	{
		boolPointer = &(currentParseTreeGroup->neuronSolidified);
	}
	*/
				
	if(*boolPointer)
	{
		*boolPointer = false;
		
		for(int i1=0; i1<currentParseTreeGroup->components.size(); i1++)
		{
			GIAposRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i1];
			if(currentComponent->parseTreeGroupRef != NULL)
			{
				resetNeuronBackprop(currentComponent->parseTreeGroupRef, groupBoolIndexType);
			}
		}
	}
	
	return result;
}

#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
bool GIAposRelTranslatorSANIPropagateOperationsClass::resetNeuronForwardProp(GIAposRelTranslatorRulesGroupNeuralNetwork* group, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;
	if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED)
	{
		boolPointer = &(group->neuronPropagated);
	}
	else if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED)
	{
		boolPointer = &(group->neuronPreviousWordPOStypeTested);
	}
			
	if(*boolPointer)
	{
		//cout << "resetNeuronForwardProp: group = " << group->groupName << endl;
		*boolPointer = false;
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = ownerComponent->ownerGroup;
			resetNeuronForwardProp(ownerGroup, groupBoolIndexType);
		}
	}
	
	return result;
}

#endif


			


bool GIAposRelTranslatorSANIPropagateOperationsClass::doesRequireResetGroupActivation(GIAposRelTranslatorRulesComponentNeuralNetwork* Xcomponent, int indexOfXcomponentInGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool requireResetGroupActivation = false;
	
	if(Xcomponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		if(Xcomponent->subComponents.size() > 0)
		{
			if(Xcomponent->neuronComponentConnectionActive)
			{
				GIAposRelTranslatorRulesComponentNeuralNetwork* subComponent = (Xcomponent->subComponents)[0];
				if(subComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
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
			cerr << "GIAposRelTranslatorSANIPropagateOperationsClass::doesRequireResetGroupActivation{} error: Xcomponent->subcomponents.size() == 0" << endl;
		}
	}
	else
	{
		requireResetGroupActivation = true;
	}
	
	return requireResetGroupActivation;
}
						

bool GIAposRelTranslatorSANIPropagateOperationsClass::findNextAvailableComponent(int indexOfXcomponentInGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup)
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
				GIAposRelTranslatorRulesComponentNeuralNetwork* component = (ownerGroup->components)[i2];
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


bool GIAposRelTranslatorSANIPropagateOperationsClass::currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord)
{
	bool result = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
	#else
	if(currentWord->POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
	#endif
	{
		result = true;
	}
	
	return result;
}


int GIAposRelTranslatorSANIPropagateOperationsClass::convertWordIndexToSentenceEntityIndex(int wordIndex)
{
	int sentenceEntityIndex = wordIndex + GIA_POS_REL_TRANSLATOR_SANI_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sentenceEntityIndex;
}
int GIAposRelTranslatorSANIPropagateOperationsClass::convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex)
{
	int wordIndex = sentenceEntityIndex - GIA_POS_REL_TRANSLATOR_SANI_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return wordIndex;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
bool GIAposRelTranslatorSANIPropagateOperationsClass::isWordPOStypeNoun(int wordPOStype)
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

int GIAposRelTranslatorSANIPropagateOperationsClass::countParseTreeLeafSize(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup)
{
	int size = 0;
	
	size = currentParseTreeGroup->parseTreeMaxWordIndex - currentParseTreeGroup->parseTreeMinWordIndex + 1;
	
	/*
	if(currentParseTreeGroup != NULL)
	{			
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAposRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];

			size++;
						
			size = size + countParseTreeLeafSize(currentComponent->parseTreeGroupRef);
		}
	}
	*/
	
	return size;
}
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_MATCHING_DEPTH
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH

bool GIAposRelTranslatorSANIPropagateOperationsClass::adjustNetworkDepth(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool result = true;
	
	int maxDepth = group->networkDepth;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!(group->counted))
	{
		group->counted = true;
	#endif
		/*
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
		countNeuralNetworkMaxLeafSizeAndDepth(ownerGroup, &maxLeafSize, &maxDepth);
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
		countNeuralNetworkMaxLeafSizeAndDepthReset(ownerGroup);
		#endif
		*/

		for(int l=0; l<group->ANNfrontComponentConnectionList.size(); l++)
		{
			GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[l];
			GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			ownerGroup->networkDepth = max(ownerGroup->networkDepth, maxDepth);

			adjustNetworkDepth(ownerGroup);

		}
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool GIAposRelTranslatorSANIPropagateOperationsClass::adjustNetworkDepthReset(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	bool result = true;
	
	if(group->counted)
	{
		group->counted = false;

		for(int l=0; l<group->ANNfrontComponentConnectionList.size(); l++)
		{
			GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[l];
			GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			adjustNetworkDepthReset(ownerGroup);
		}
	}
	
	return result;
}

#endif

	
		
bool GIAposRelTranslatorSANIPropagateOperationsClass::getNeuralNetworkDepth(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron, int* maxDepth)
{
	bool result = true;	

	*maxDepth = currentNeuron->networkDepth;
	
	return result;
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::getNeuralNetworkDepth(GIAposRelTranslatorRulesComponentNeuralNetwork* component, int* maxDepth)
{
	bool result = true;
				
	int componentMaxDepth = 0;	
	
	for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
	{
		GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
		componentMaxDepth = max(componentMaxDepth, groupSource->networkDepth);
	}
	*maxDepth = componentMaxDepth;
	
	return result;
}
#endif
bool GIAposRelTranslatorSANIPropagateOperationsClass::countNeuralNetworkMaxLeafSizeAndDepth(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron, int* maxLeafSize, int* maxDepth)
{
	bool result = true;	

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!(currentNeuron->counted))
	{
		currentNeuron->counted = true;
	#endif
	
		int maxDepthStart = *maxDepth + 1;
		*maxDepth = maxDepthStart;
	
		if(currentNeuron->inputLayerNeuron)
		{
			*maxLeafSize = *maxLeafSize + 1;
		}
		else
		{
			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				int c = i;

				GIAposRelTranslatorRulesComponentNeuralNetwork* component = (currentNeuron->components)[c];

				int leafSize = 0;
				int depth = maxDepthStart;
				countNeuralNetworkMaxLeafSizeAndDepth(component, &leafSize, &depth);
				*maxLeafSize = *maxLeafSize + leafSize;
				*maxDepth = max(*maxDepth, depth);
			}
		}
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::countNeuralNetworkMaxLeafSizeAndDepth(GIAposRelTranslatorRulesComponentNeuralNetwork* component, int* maxLeafSize, int* maxDepth)
{
	bool result = true;
				
	int componentMaxLeafSize = 0;
	int componentMaxDepth = *maxDepth;	
	int maxDepthStart = *maxDepth;
		
	for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
	{
		GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
		int leafSize = 0;
		int depth = maxDepthStart; 
		countNeuralNetworkMaxLeafSizeAndDepth(groupSource, &leafSize, &depth);
		componentMaxLeafSize = max(componentMaxLeafSize, leafSize);
		componentMaxDepth = max(componentMaxDepth, depth);
	}
	*maxLeafSize = componentMaxLeafSize;
	*maxDepth = componentMaxDepth;
	
	return result;
}
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool GIAposRelTranslatorSANIPropagateOperationsClass::countNeuralNetworkMaxLeafSizeAndDepthReset(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron)
{
	bool result = true;	

	if(currentNeuron->counted)
	{
		currentNeuron->counted = false;
	
		if(currentNeuron->inputLayerNeuron)
		{
		}
		else
		{
			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				int c = i;
				GIAposRelTranslatorRulesComponentNeuralNetwork* component = (currentNeuron->components)[c];
				
				countNeuralNetworkMaxLeafSizeAndDepthReset(component);
			}
		}
	}
	
	return result;
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::countNeuralNetworkMaxLeafSizeAndDepthReset(GIAposRelTranslatorRulesComponentNeuralNetwork* component)
{
	bool result = true;
				
	for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
	{
		GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
		countNeuralNetworkMaxLeafSizeAndDepthReset(groupSource);
	}
	
	return result;
}
#endif
#endif

GIAposRelTranslatorRulesComponentNeuralNetwork* GIAposRelTranslatorSANIPropagateOperationsClass::getFirstComponent(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron, bool fromStart)
{
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		fromStart = !fromStart;
	}
	int componentIndex;
	if(fromStart)
	{
		componentIndex = GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST;
	}
	else
	{
		componentIndex = GIA_POS_REL_TRANSLATOR_SANI_COMPONENT_INDEX_FIRST+currentNeuron->components.size()-1;
	}
	GIAposRelTranslatorRulesComponentNeuralNetwork* firstComponent = currentNeuron->components[componentIndex];
	
	return firstComponent;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_MATCHING_DEPTH	
int GIAposRelTranslatorSANIPropagateOperationsClass::calculateDepthFromBinaryTreeLeafSize(int numberOfLeafNodesInBinaryTree)
{
	//see http://courses.cs.vt.edu/~cs3114/Fall09/wmcquain/Notes/T03a.BinaryTreeTheorems.pdf
	
	/*
	//assume full and complete tree:
	
	//numberOfLeafNodesInBinaryTree = (n+1)/2
	int nodesInBinaryTree = numberOfLeafNodesInBinaryTree*2 - 1;
	
	//nodesInBinaryTree = 2^(h+1)-1.
	int maxDepth = floor(log2(nodesInBinaryTree));
	*/
	
	//assume full but incomplete tree (ie elongated tree):
	int maxDepth = numberOfLeafNodesInBinaryTree - 1;
	
	/*
	cout << "numberOfLeafNodesInBinaryTree = " << numberOfLeafNodesInBinaryTree << endl;
	cout << "nodesInBinaryTree = " << nodesInBinaryTree << endl;
	cout << "maxDepth = " << maxDepth << endl;
	*/
	return maxDepth;
}
#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE
double GIAposRelTranslatorSANIPropagateOperationsClass::generateRandomNumber()
{
	double randomNumberBetween0And1 = rand()/double(RAND_MAX);
}
#endif

#endif


bool GIAposRelTranslatorSANIPropagateOperationsClass::deleteGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::deleteComponents(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i];
		if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			deleteComponents(&(currentComponent->subComponents));
		}
		delete currentComponent;
	}
	
	return result;
}	

bool GIAposRelTranslatorSANIPropagateOperationsClass::deleteGroup(GIAposRelTranslatorRulesGroupParseTree* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool GIAposRelTranslatorSANIPropagateOperationsClass::deleteComponents(vector<GIAposRelTranslatorRulesComponentParseTree*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* currentComponent = (*components)[i];
		delete currentComponent;
	}
	
	return result;
}	

bool GIAposRelTranslatorSANIPropagateOperationsClass::deleteGroup(GIAposRelTranslatorRulesGroupActivationMemory* group)
{
	deleteComponents(&(group->components));
	delete group;
}	



			
bool GIAposRelTranslatorSANIPropagateOperationsClass::printGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group, int layer)
{
	for(int i=0; i<group->components.size(); i++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (group->components)[i];
		printComponent(component, layer);
	}	
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::printComponent(GIAposRelTranslatorRulesComponentParseTree* component, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SHAREDvars.convertIntToString(component->wordPOStypeInferred) + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->word + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenLayer + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenClass + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenType + CHAR_TAG_CLOSE;
		}
		
		if(component->candidateStringMatch != NULL)
		{
			componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
		}
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupTypeRefName + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupRefName + STRING_SPACE + CHAR_TAG_CLOSE;	
		cout << componentTagRegenerated << endl;
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::printComponent(GIAposRelTranslatorRulesComponentNeuralNetwork* component, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->wordPOStype + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->word + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenLayer + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenClass + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenType + CHAR_TAG_CLOSE;
		}
		
		if(component->candidateStringMatch != NULL)
		{
			componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
		}
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + GIAposRelTranslatorRulesGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupTypeRefName + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupRefName + STRING_SPACE + CHAR_TAG_CLOSE;	
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_or + STRING_SPACE + CHAR_TAG_CLOSE;
		cout << componentTagRegenerated << endl;
		for(int i=0; i<component->subComponents.size(); i++)
		{
			GIAposRelTranslatorRulesComponentNeuralNetwork* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat + STRING_SPACE + CHAR_TAG_CLOSE;
		cout << componentTagRegenerated << endl;
		for(int i=0; i<component->subComponents.size(); i++)
		{
			GIAposRelTranslatorRulesComponentNeuralNetwork* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::printParseTreeDebugIndentation(int layer)
{
	bool result = true;
	
	for(int i=0; i<layer; i++)
	{
		cout << "\t";
	}
	
	return result;
}

int GIAposRelTranslatorSANIPropagateOperationsClass::calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset)
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


void GIAposRelTranslatorSANIPropagateOperationsClass::deleteParseTree(GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToDelete, int level)
{
	if(parseTreeGroupToDelete != NULL)
	{
		for(int i=0; i<parseTreeGroupToDelete->components.size(); i++)
		{
			GIAposRelTranslatorRulesComponentParseTree* currentComponent = (parseTreeGroupToDelete->components)[i];

			if(currentComponent->parseTreeGroupRef != NULL)
			{
				deleteParseTree(currentComponent->parseTreeGroupRef, level+1);
			}
		}

		deleteGroup(parseTreeGroupToDelete);
	}
}

GIAposRelTranslatorRulesGroupParseTree* GIAposRelTranslatorSANIPropagateOperationsClass::replicateParseTree(GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToReplicate, int level)
{
	bool result = false;

	GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupNew = GIAposRelTranslatorRules.copyGroup(parseTreeGroupToReplicate);
	//parseTreeGroupNew->components.clear();
	
	for(int i=0; i<parseTreeGroupToReplicate->components.size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* currentComponent = (parseTreeGroupToReplicate->components)[i];
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{
			GIAposRelTranslatorRulesGroupParseTree* componentParseTreeGroupNew = replicateParseTree(currentComponent->parseTreeGroupRef, level+1);
			(parseTreeGroupNew->components)[i]->parseTreeGroupRef = componentParseTreeGroupNew;
			//parseTreeGroupNew->components.push_back(componentParseTreeGroupNew);
		}
	}
	
	return parseTreeGroupNew;
}

GIAposRelTranslatorRulesGroupParseTree* GIAposRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToParseTreeGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	GIAposRelTranslatorRulesGroup* groupBase = group;
	return (static_cast<GIAposRelTranslatorRulesGroupParseTree*>(groupBase));
}
GIAposRelTranslatorRulesComponentParseTree* GIAposRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkComponentToParseTreeComponent(GIAposRelTranslatorRulesComponentNeuralNetwork* component)
{
	GIAposRelTranslatorRulesComponent* componentBase = component;
	return (static_cast<GIAposRelTranslatorRulesComponentParseTree*>(componentBase));
}
GIAposRelTranslatorRulesGroupActivationMemory* GIAposRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToMemoryActivationGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	return (static_cast<GIAposRelTranslatorRulesGroupActivationMemory*>(group));
}


GIAposRelTranslatorRulesGroupParseTree* GIAposRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToParseTreeGroupNew(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	GIAposRelTranslatorRulesGroup* groupBase = group;
	GIAposRelTranslatorRulesGroupParseTree* groupNew = new GIAposRelTranslatorRulesGroupParseTree();
	GIAposRelTranslatorRulesGroup* groupNewBase = groupNew;
	*groupNewBase = *groupBase;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	groupNew->groupRef = group;
	#endif
	return groupNew;
}
GIAposRelTranslatorRulesComponentParseTree* GIAposRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkComponentToParseTreeComponentNew(GIAposRelTranslatorRulesComponentNeuralNetwork* component)	//why isnt this required?
{
	GIAposRelTranslatorRulesComponent* componentBase = component;
	GIAposRelTranslatorRulesComponentParseTree* componentNew = new GIAposRelTranslatorRulesComponentParseTree();
	GIAposRelTranslatorRulesComponent* componentNewBase = componentNew;
	*componentNewBase = *componentBase;
	return componentNew;
}
GIAposRelTranslatorRulesGroupActivationMemory* GIAposRelTranslatorSANIPropagateOperationsClass::convertNeuralNetworkGroupToMemoryActivationGroupNew(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{
	GIAposRelTranslatorRulesGroupActivationMemory* groupNew = new GIAposRelTranslatorRulesGroupActivationMemory();
	GIAposRelTranslatorRulesGroupNeuralNetwork* groupNewBase = groupNew;
	*groupNewBase = *group;
	groupNew->components.clear();
	GIAposRelTranslatorRules.copyComponents(&(group->components), &(groupNew->components));
	return groupNew;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
bool GIAposRelTranslatorSANIPropagateOperationsClass::resetGroupOptimumPathway(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool result = true;
	
	resetComponentsOptimumPathway(&(group->components));
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::resetComponentsOptimumPathway(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i1];
		
		currentComponent->optimumPathwayWordList.clear();
		
		if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			resetComponentsOptimumPathway(&(currentComponent->subComponents));
		}
	}
	
	return result;
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
bool GIAposRelTranslatorSANIPropagateOperationsClass::initialiseParseTreeGroupList(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIAposRelTranslatorRulesGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];
			parseTreeGroupList->push_back(group->currentParseTreeGroupTemp);
			//cout << "initialiseParseTreeGroupList" << endl;
		}
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::deinitialiseParseTreeGroupList(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIAposRelTranslatorRulesGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i2=0; i2<parseTreeGroupList->size(); i2++)
	{
		GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupTemp = (*parseTreeGroupList)[i2];
		deleteGroup(parseTreeGroupTemp);
	}
	parseTreeGroupList->clear();
	for(int i=0; i<GIAposRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAposRelTranslatorRulesGroupType* groupType = GIAposRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];
			group->currentParseTreeGroupTemp = NULL;
		}
	}
	
	return result;
}
#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
int GIAposRelTranslatorSANIPropagateOperationsClass::calculateCoverage(GIAposRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	int activatedNeuronWordIndexCoverage = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex - activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex + 1;
	return activatedNeuronWordIndexCoverage;
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::traceBackpropNeuralNetwork(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron, int level, int previousComponentIndex, int previousComponentType)
{
	printParseTreeDebugIndentation(level);
	if(isNeuronString(currentNeuron))
	{
		cout << "GIAposRelTranslatorSANIPropagateOperationsClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->wordPOStype = " << currentNeuron->wordPOStype << endl;
	}
	else
	{
		cout << "GIAposRelTranslatorSANIPropagateOperationsClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;
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
		
		GIAposRelTranslatorRulesComponentNeuralNetwork* component = (currentNeuron->components)[c];
					
		for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
			traceBackpropNeuralNetwork(groupSource, level+1, i, component->componentType);
		}	
	}
		
}

bool GIAposRelTranslatorSANIPropagateOperationsClass::isNeuronString(GIAposRelTranslatorRulesGroupNeuralNetwork* currentNeuron)
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

bool GIAposRelTranslatorSANIPropagateOperationsClass::getWordPOStypeFromAmbiguousWord(GIApreprocessorPlainTextWord* currentWord, int* wordPOStype)
{
	bool result = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
	if(GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
	{	
		*wordPOStype = GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
		result = true;
	}
	else
	{
	#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		*wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
		result = true;		
		#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
	}
	#endif
	
	return result;
}

#endif	


string GIAposRelTranslatorSANIPropagateOperationsClass::printParseTreeGroupIndices(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup)
{
	string groupIndicesString = "";
	
	groupIndicesString = groupIndicesString + SHAREDvars.convertIntToString(currentParseTreeGroup->groupRef->groupIndex) + " ";
	//cout << currentParseTreeGroup->groupRef->groupIndex << " ";
	//cout << currentParseTreeGroup->groupRef->neuronReference->GIAentityName << " ";
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];

		if(currentComponent->parseTreeGroupRef != NULL)
		{
			groupIndicesString = groupIndicesString + printParseTreeGroupIndices(currentComponent->parseTreeGroupRef);
		}
	}

	return groupIndicesString;
}


