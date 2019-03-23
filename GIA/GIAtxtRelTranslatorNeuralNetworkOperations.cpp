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
 * File Name: GIAtxtRelTranslatorNeuralNetworkOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g11h 01-March-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Operations - generic functions
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkOperations.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA8
bool firstExecution;
#endif

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponent** previousActiveComponent, GIAtxtRelTranslatorRulesComponent** lastActiveComponent)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(testComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, lastActiveComponent, &existingActivationFoundNOTUSED);
}
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(testComponent, components, &existingActivationFoundNOTUSED);
}
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(testComponent, components, forwardPropogationWordData, repeatDetected, &existingActivationFoundNOTUSED);
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, GIAtxtRelTranslatorRulesComponent** previousActiveComponent, GIAtxtRelTranslatorRulesComponent** lastActiveComponent, bool* existingActivationFound)
{	
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	int numberOfInactiveComponentsRemaining = 0;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	*previousActiveComponent = NULL;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	for(int i=components->size()-1; i>=0; i--)
	#else
	for(int i=0; i<components->size(); i++)
	#endif
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		if(component->neuronComponentConnectionActive)
		{
			*lastActiveComponent = component;
		}
		#endif
				
		if(stillParsingActiveComponents)
		{
			/*
			cout << "component->neuronComponentConnectionActive = " << component->neuronComponentConnectionActive << endl;
			cout << "component->optional = " << component->optional << endl;
			cout << "component->hasSubComponents = " << GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component) << endl;
			*/
			if(component == testComponent)
			{
				//cout << "(component == testComponent)" << endl;

				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
				if(*previousActiveComponent == NULL)
				{
					*firstActiveComponentInGroup = true;
				}
				#endif
				
				stillParsingActiveComponents = false;

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
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
						sequentialActivationFound = true;
						*existingActivationFound = true;		
						#endif
					}
				}
				else
				{
					sequentialActivationFound = true;
					//cout << "!(component->neuronComponentConnectionActive)" << endl;
				}
			}
			else
			{
				//cout << "!(component == testComponent)" << endl;

				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
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
					if(!(component->neuronComponentConnectionActive))
					{
						stillParsingActiveComponents = false;
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
						numberOfInactiveComponentsRemaining++;	
					}
					else
					{
						//cout << "group not suitable for activation, following component is active" << endl;
						bool foundFollowingActiveComponent = true;
						sequentialActivationFound = false;
					}
				}
			}
		}
	}
	
	if(sequentialActivationFound)
	{
		if(numberOfInactiveComponentsRemaining == 0)
		{
			//cout << "*activationSequenceCompleted" << endl;
			
			*activationSequenceCompleted = true;
		}
		//cout << "numberOfInactiveComponentsRemaining = " << numberOfInactiveComponentsRemaining << endl;
	}

	return sequentialActivationFound;
}


bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, bool* existingActivationFound)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	for(int i=components->size()-1; i>=0; i--)
	#else
	for(int i=0; i<components->size(); i++)
	#endif
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];
		if(component == testComponent)
		{
			if(component->neuronComponentConnectionActive)
			{
				//component already activated
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
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
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFound)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	for(int i=components->size()-1; i>=0; i--)
	#else
	for(int i=0; i<components->size(); i++)
	#endif
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];
		if(component == testComponent)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			bool pass = true;
			if(testComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				if(component->neuronComponentConnectionActive)
				{
					pass = false;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
					if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->wordReference->translatorSentenceEntityIndex + 1)
					#else
					if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->wordReference->translatorSentenceEntityIndex - 1)
					#endif
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
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
					//if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->wordReference->translatorSentenceEntityIndex)
					//{
						*existingActivationFound = true;
					//}
					#endif
					*/
				}
				sequentialActivationFound = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			}
			#endif
		}
		else
		{
			cerr << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady{} error: repeat sequences can contain only 1 component" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return sequentialActivationFound;
}



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = false;

	bool existingActivationFoundNOTUSED = false;

	GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		int lastActiveComponentInParseTreeIndex = activationPathWordCurrentParseTreeGroupOwner->components.size() - 1;
		GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree = (activationPathWordCurrentParseTreeGroupOwner->components)[lastActiveComponentInParseTreeIndex];
		GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
		
		result = componentWordConnectivityTests(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, forwardPropogationWordData, existingActivationFoundNOTUSED);
	}
	else
	{
		result = true;
	}
	
	return result;
}
#else
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData)
{
	bool existingActivationFoundNOTUSED = false;
	return componentWordConnectivityTests(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFoundNOTUSED);
}
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;

	GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;

	GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
	GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree = NULL;
	GIAtxtRelTranslatorRulesGroup* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_FIX_BUG1
	GIAtxtRelTranslatorRulesGroup lastActiveComponentInParseTreeParseTreeGroupRefTemp;	//in case is NULL
	GIAtxtRelTranslatorRulesGroup prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRefTemp;	//in case is NULL
	GIAtxtRelTranslatorRulesComponent prospectiveNewlyActiveComponentInParseTreeParseTreeTemp;	//in case is NULL
	#endif
	
	if(ownerGroupParseTreeGroup->components.size() > 0)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
		int lastActiveComponentInParseTreeIndex = 0;
		#else
		int lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
		#endif
		GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_FIX_BUG
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_FIX_BUG2
		lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
		#else
		if(lastActiveComponentInParseTree->parseTreeGroupRef != NULL)
		{
			lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
		}
		else
		{
			lastActiveComponentInParseTreeParseTreeGroupRefTemp.components.push_back(lastActiveComponentInParseTree);
			lastActiveComponentInParseTreeParseTreeGroupRef = &lastActiveComponentInParseTreeParseTreeGroupRefTemp;
		}
		
		if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef == NULL)
		{
			prospectiveNewlyActiveComponentInParseTreeParseTreeTemp.neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
			prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRefTemp.components.push_back(&prospectiveNewlyActiveComponentInParseTreeParseTreeTemp);
			prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = &prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRefTemp;
		}
		#endif
		
		if(componentWordConnectivityTests(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, forwardPropogationWordData, existingActivationFound))
		{
			result = true;
		}		
				
		#else
		if(activationPathWordCurrentParseTreeGroup != NULL)
		{			
			if(lastActiveComponentInParseTree->parseTreeGroupRef != NULL)
			{
				GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
				if(componentWordConnectivityTests(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, forwardPropogationWordData, existingActivationFound))
				{
					result = true;
				}
			}
			else
			{
				result = true;
			}
		}
		else
		{
			result = true;
		}
		#endif
	}
	else
	{
		result = true;
	}
		
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;
		
	int wordIndexMax = -1;
	int wordIndexMin = REALLY_LARGE_INT;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMin = lastActiveComponentInParseTreeParseTreeGroupRef;
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMax = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
	int wordIndexMinMin = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;	//CHECKTHIS
	int wordIndexMaxMax = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;	//CHECKTHIS
	#else
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMin = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMax = lastActiveComponentInParseTreeParseTreeGroupRef;
	int wordIndexMinMin = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;
	int wordIndexMaxMax = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_FIX_BUG2
	if(parseTreeGroupToFindWordIndexMax != NULL)
	{
	#endif
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true, &wordIndexMax, 0))
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests error{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true)" << endl;	
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests pass{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true)" << endl;	
			//exit(EXIT_ERROR);
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_FIX_BUG2
	}
	#endif
	wordIndexMax = SHAREDvars.maxInt(wordIndexMax, wordIndexMaxMax);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_FIX_BUG2
	if(parseTreeGroupToFindWordIndexMin != NULL)
	{
	#endif
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false, &wordIndexMin, 0))
		{
			//newParseComponent->neuronComponentConnectionActiveWordRecord has not yet been defined
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests error{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false) && " << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentWordConnectivityTests pass{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false) && " << endl;
			//exit(EXIT_ERROR);
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_FIX_BUG2
	}
	#endif
	wordIndexMin = SHAREDvars.minInt(wordIndexMin, wordIndexMinMin);

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
	cout << "wordIndexMax = " << wordIndexMax << endl;
	cout << "wordIndexMin = " << wordIndexMin << endl;
	cout << "wordIndexMinMin = " << wordIndexMinMin << endl;
	cout << "existingActivationFound = " << existingActivationFound << endl;
	//cout << "wordIndexMinMin = " << wordIndexMinMin << endl;
	#endif
	
	if(wordIndexMin-wordIndexMax == 1)
	{
		result = true;
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
	else if(existingActivationFound)
	{
		if(wordIndexMin <= wordIndexMax)
		{
			result = true;
		}
	}
	#endif

	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::getMinMaxWordIndexInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
	if(findMaxOrMinWordIndex)
	{
		printParseTreeDebugIndentation(level);
		cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::getMinMaxWordIndexInParseTree: currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeNameBackup << ":" << currentParseTreeGroup->groupName << endl;
	}
	#endif
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i];
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
		if(!findMaxOrMinWordIndex)
		{
			printParseTreeDebugIndentation(level);
			cout << "currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex = " << currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex  << endl;
		}
		#endif
		
		if(findMaxOrMinWordIndex)
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex > *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
				result = true;
			}
		}
		else
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex < *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
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
	
	return result;	
}

#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;

	if(existingActivationFound)
	{
		GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = NULL;
		if(activationPathWordCurrentParseTreeGroup != NULL)
		{
			prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
		}
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
		cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree: existingActivationFound" << endl;
		#endif
		
		GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
		GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree = NULL;
		GIAtxtRelTranslatorRulesGroup* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
			int lastActiveComponentInParseTreeIndex = 0;
			#else
			int lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
			#endif
			lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			if(lastActiveComponentInParseTree->parseTreeGroupRef != NULL)
			{
				lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
			}
		
			
			vector<int> prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList;
			vector<int> lastActiveComponentInParseTreeParseTreeWordIndexList;
			if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				getWordIndicesInParseTree(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, &prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList, 0);
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex);
				//prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(convertWordIndexToSentenceEntityIndex(forwardPropogationWordData->w));
			}
			else
			{
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex);
			}
			if(lastActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				getWordIndicesInParseTree(lastActiveComponentInParseTreeParseTreeGroupRef, &lastActiveComponentInParseTreeParseTreeWordIndexList, 0);	
			}
			else
			{
				lastActiveComponentInParseTreeParseTreeWordIndexList.push_back(lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex);
			}

			result = true;
			for(int i=0; i<lastActiveComponentInParseTreeParseTreeWordIndexList.size(); i++)
			{
				bool foundLastActiveComponentParseTreeWord = false;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
				cout << "lastActiveComponentInParseTreeParseTreeWordIndexList[i] = " << lastActiveComponentInParseTreeParseTreeWordIndexList[i] << endl;
				#endif
				
				for(int j=0; j<prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.size(); j++)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
					cout << "prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
					#endif
					
					if(prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] == lastActiveComponentInParseTreeParseTreeWordIndexList[i])
					{
						foundLastActiveComponentParseTreeWord = true;
						//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree debug: foundLastActiveComponentParseTreeWord; prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
					}
				}
				if(!foundLastActiveComponentParseTreeWord)
				{
					result = false;	
				}
			}
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}	
		
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::getWordIndicesInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, vector<int>* wordIndexList, int level)
{
	bool result = true;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
	printParseTreeDebugIndentation(level);
	cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::getWordIndicesInParseTree: " <<  currentParseTreeGroup->groupTypeNameBackup << ":" << currentParseTreeGroup->groupName << endl;	
	#endif
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i];
		
		wordIndexList->push_back(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex); 
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
		printComponent(currentComponent, level);
		#endif
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(getWordIndicesInParseTree(currentComponent->parseTreeGroupRef, wordIndexList, level+1))
			{
				result = true;
			}
		}
	}
	
	return result;	
}
#endif






#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE

int GIAtxtRelTranslatorNeuralNetworkOperationsClass::countNumberWordsInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{	
	int performanceOrig = forwardPropogationSentenceData->performance;
	
	updatePerformanceGroup(currentParseTreeGroup, forwardPropogationSentenceData, layer);
	int performance = forwardPropogationSentenceData->performance;

	forwardPropogationSentenceData->performance = performanceOrig;
	 
	return performance;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::updatePerformance(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(currentParseTreeGroup->groupTypeNameBackup, currentParseTreeGroup->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
	if(topLevelGroup)
	{
		result = updatePerformanceGroup(currentParseTreeGroup, forwardPropogationSentenceData, layer);
	}
	
	return result;
}

//doesn't perform topLevelGroup check;
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::updatePerformanceGroup(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
			 
	int performanceTemp = 0;
	bool print = false;
	bool performancePreprocess = true;
	if(!traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceTemp, forwardPropogationSentenceData->sentenceContents))
	{
		result = false;
	}
	resetNeuronBackprop(currentParseTreeGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
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
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	if(result)
	{
		if(forwardPropogationSentenceData->performance != forwardPropogationSentenceData->sentenceContents->size())
		{
	#endif
			result = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
		}
	}
	#endif
				
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool calculateMaxWeight = false;
	int maxWeightNOTUSED = 0;
	return traceBackpropParseTree(currentParseTreeGroup, level, print, performancePreprocess, performance, sentenceContents, calculateMaxWeight, &maxWeightNOTUSED);

}
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight)
{
	bool result = true;
	
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(calculateMaxWeight)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
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
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
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
			//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
			printParseTreeDebugIndentation(level);
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupTypeNameBackup = " << currentParseTreeGroup->groupTypeNameBackup << endl;		//<< ", parse word (providing semanticRelationReturnEntity) = ?"
			//#endif
		}

		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i];
			
			//GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, level);
			
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			if(performancePreprocess)
			{
				if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
					if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//redundant?
					{
						if(currentComponent->candidateStringMatch->alreadyFoundMatch)
						{
							result = false;
							//duplicate instance of word detected in parsetree - fail to parse sentence
						}
						else
						{
					#endif
						currentComponent->candidateStringMatch->alreadyFoundMatch = true;
						//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
							//"the" and "a" will use identical neural groups and so will only count to +1 performance total
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
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
					printComponent(currentComponent, level);
				}
			}
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{							
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
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
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
			}
			#endif
		}	
	}
	
	return result;
}

#endif





								
								
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::isSentenceWordDataFullyConnected(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//forwardPropogationWordData->previousWordConnections.push_back(wPrevious);
	bool result = true;
	
	int numberOfWords = forwardPropogationSentenceData->forwardPropogationWordDataArray.size();
	vector<bool> previousWordConnectionsFound(numberOfWords, false);
	previousWordConnectionsFound[numberOfWords-1] = true;	//set last word as found 
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* lastWordData = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[numberOfWords-1]);
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

void GIAtxtRelTranslatorNeuralNetworkOperationsClass::isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* currentWordData)
{
	for(int i = 0; i<currentWordData->previousWordConnections.size(); i++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* previousWordData = (currentWordData->previousWordConnections)[i];
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
		cout << "previousWordConnectionsFound == true: previousWordData->w = " << previousWordData->w << endl;
		#endif
		(*previousWordConnectionsFound)[previousWordData->w] = true;
		isSentenceWordDataFullyConnected(previousWordConnectionsFound, previousWordData);
	}
}
#endif



#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentTests1(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
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

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::componentTests2(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
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
		int translatorSentenceEntityIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		bool pass = false;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceEntityIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationWordData->wordReference;
		}
		else
		{
			pass = true;
			//cout << "firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex = " << firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex << endl;
			//exit(EXIT_ERROR);
		}

		/*
		if(pass)
		{
			cout << "group->previousWordPOStype = " << group->previousWordPOStype << ", getFirstWordInParseTree pass, firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex = " << firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex << ", numComponentsFound = " << numComponentsFound << endl;
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
		int translatorSentenceEntityIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceEntityIndexMin, &numComponentsFound))
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

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::getFirstWordInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceEntityIndexMin, int* numComponentsFound)
{
	bool result = false;
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i];
		
		//if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		//{
			*numComponentsFound = *numComponentsFound + 1;
		//}
		
		int translatorSentenceEntityIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
		if(translatorSentenceEntityIndex < *translatorSentenceEntityIndexMin)
		{
			*translatorSentenceEntityIndexMin = translatorSentenceEntityIndex;
			*firstWordInCurrentParseTreeGroupParseTree = currentComponent->neuronComponentConnectionActiveWordRecord;
			result = true;
		}
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{							
			if(getFirstWordInParseTree(currentComponent->parseTreeGroupRef, firstWordInCurrentParseTreeGroupParseTree, translatorSentenceEntityIndexMin, numComponentsFound))
			{
				result = true;
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype)
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
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype)
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

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex)
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




bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroup* group = (groupType->groups)[i2];

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

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroup* group = (groupType->groups)[i2];

			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE)
			{
				//cout << "resetGroupActivation = " << group->groupName << endl; 
				resetGroupActivation(group);
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
			
			#else
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF)
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA8
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
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED)
			{
				group->neuronProcessed = false;
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
			{
				group->currentParseTreeGroupTemp->solidified = false;
			}
			#endif
	
		}
	}
	
	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroup* group)
{
	bool lastComponentIsFuzzy = false;

	int indexOfLastComponentInGroup = group->components.size()-1;
	GIAtxtRelTranslatorRulesComponent* lastComponent = (group->components)[indexOfLastComponentInGroup];
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_TREAT_OR_AS_FUZZY
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

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group)
{	
	bool result = true;

	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	resetGroupParseTreeGroupRef(group, false);	//leave existing parseTreeGroupRef in memory
	#endif
	*/
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
	//cout << "resetGroupActivation: group->groupName = " << group->groupName << ", group->groupTypeNameBackup = " << group->groupTypeNameBackup << endl;
	#endif
	
	group->neuronActive = false;	//not used
	resetComponentsActivation(&(group->components));
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetComponentsActivation(vector<GIAtxtRelTranslatorRulesComponent*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i1];
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_COMPONENT_NEURONCOMPONENTCONNECTIONACTIVEWORDRECORD
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

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::restoreGroupActivation(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* groupOrig)
{	
	bool result = true;

	group->neuronActive = groupOrig->neuronActive;
	restoreComponentsActivation(&(group->components), &(groupOrig->components));
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::restoreComponentsActivation(vector<GIAtxtRelTranslatorRulesComponent*>* components, vector<GIAtxtRelTranslatorRulesComponent*>* componentsOrig)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i1];
		GIAtxtRelTranslatorRulesComponent* currentComponentOrig = (*componentsOrig)[i1];
		currentComponent->neuronComponentConnectionActive = currentComponentOrig->neuronComponentConnectionActive;
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			restoreComponentsActivation(&(currentComponent->subComponents), &(currentComponentOrig->subComponents));
		}
	}
	
	return result;
}




#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* activationMemoryGroup, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;	
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
				
	for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
	{
		GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray[i];
		GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroupActivationPointTemp = currentParseTreeGroup->activationPathWordFirstParseTreeGroupActivationPointArray[i];
		
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
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		for(int i=0; i<group->currentParseTreeGroupArray.size(); i++)
		{
			GIAtxtRelTranslatorRulesGroup* tempParseTreeGroup = (group->currentParseTreeGroupArray)[i];
			if(!(tempParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
				delete tempParseTreeGroup;
			}
		}
	}
	group->currentParseTreeGroupArray.clear();

	//CHECKTHIS;
	GIAtxtRelTranslatorRulesGroup* tempParseTreeGroup = new GIAtxtRelTranslatorRulesGroup(*group);
	/*
	NO, as dont wish to copy subcomponents into tempParseTreeGroup;
	GIAtxtRelTranslatorRulesGroup* tempParseTreeGroup = copyGroup(group);
	deleteComponents(&(tempParseTreeGroup->components));
	*/
	tempParseTreeGroup->components.clear();
	group->currentParseTreeGroupArray.push_back(tempParseTreeGroup);		
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	for(int i=0; i<group->activationMemoryGroupArray.size(); i++)
	{
		GIAtxtRelTranslatorRulesGroup* tempMemoryGroup = (group->activationMemoryGroupArray)[i];
		delete tempMemoryGroup;
	}
	group->activationMemoryGroupArray.clear();

	GIAtxtRelTranslatorRulesGroup* tempMemoryGroup = GIAtxtRelTranslatorRules.copyGroup(group);
	group->activationMemoryGroupArray.push_back(tempMemoryGroup);
	#endif
	
	return result;
}
#endif
#else
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		if(group->currentParseTreeGroupTemp != NULL)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA8
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
			
			if(!(group->currentParseTreeGroupTemp->neuronTraced))	//fixed GIA3g11aTEMP14
			//if(!(group->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
				//delete group->currentParseTreeGroupTemp;	//OLD: don't delete components
				deleteGroup(group->currentParseTreeGroupTemp);	//added GIA3g11aTEMP27
			}
		}
	}
	group->currentParseTreeGroupTemp = new GIAtxtRelTranslatorRulesGroup(*group);
	group->currentParseTreeGroupTemp->components.clear();	//added 3g1h
	
	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::restoreGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* parseTreeGroupOrig)
{
	bool result = true;
	
	deleteGroup(group->currentParseTreeGroupTemp);	//CHECKTHIS - delete newly created currentParseTreeGroupTemp 
	group->currentParseTreeGroupTemp = parseTreeGroupOrig;
	
	return result;
}


#endif
#endif






#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, const int groupBoolIndexType)
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
			GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i1];
			if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
			{
				for(int i2=0; i2<currentComponent->subComponents.size(); i2++)
				{
					GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i2];
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

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType)
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

#endif



bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::deleteGroup(GIAtxtRelTranslatorRulesGroup* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::deleteComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i];
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			deleteComponents(&(currentComponent->subComponents));
		}
		delete currentComponent;
	}
	
	return result;
}							


bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::doesRequireResetGroupActivation(GIAtxtRelTranslatorRulesComponent* Xcomponent, int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData)
{
	bool requireResetGroupActivation = false;
	
	if(Xcomponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		if(Xcomponent->subComponents.size() > 0)
		{
			if(Xcomponent->neuronComponentConnectionActive)
			{
				GIAtxtRelTranslatorRulesComponent* subComponent = (Xcomponent->subComponents)[0];
				if(subComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					//expect repeated strings to occur consecutively
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
					if(subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex != forwardPropogationWordData->wordReference->translatorSentenceEntityIndex + 1)
					#else
					if(subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex != forwardPropogationWordData->wordReference->translatorSentenceEntityIndex - 1)
					#endif
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
			cerr << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::doesRequireResetGroupActivation{} error: Xcomponent->subcomponents.size() == 0" << endl;
		}
	}
	else
	{
		requireResetGroupActivation = true;
	}
	
	return requireResetGroupActivation;
}
						

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::findNextAvailableComponent(int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup)
{
	bool foundNextAvailableComponent = false;
	bool findingNextAvailableComponent = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	for(int i2=indexOfXcomponentInGroup; i2>=0; i2--)
	#else
	for(int i2=indexOfXcomponentInGroup; i2<ownerGroup->components.size(); i2++)
	#endif
	{
		if(findingNextAvailableComponent)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
			if(i2 < indexOfXcomponentInGroup)
			#else
			if(i2 > indexOfXcomponentInGroup)
			#endif
			{
				GIAtxtRelTranslatorRulesComponent* component = (ownerGroup->components)[i2];
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


bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord)
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


int GIAtxtRelTranslatorNeuralNetworkOperationsClass::convertWordIndexToSentenceEntityIndex(int wordIndex)
{
	int sentenceEntityIndex = wordIndex + GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sentenceEntityIndex;
}
int GIAtxtRelTranslatorNeuralNetworkOperationsClass::convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex)
{
	int wordIndex = sentenceEntityIndex - GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return wordIndex;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::isWordPOStypeNoun(int wordPOStype)
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


bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::printGroup(GIAtxtRelTranslatorRulesGroup* group, int layer)
{
	for(int i=0; i<group->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* component = (group->components)[i];
		printComponent(component, layer);
	}	
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::printComponent(GIAtxtRelTranslatorRulesComponent* component, int layer)
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
			GIAtxtRelTranslatorRulesComponent* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	else if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat + STRING_SPACE + CHAR_TAG_CLOSE;
		cout << componentTagRegenerated << endl;
		for(int i=0; i<component->subComponents.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	
	
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::printParseTreeDebugIndentation(int layer)
{
	bool result = true;
	
	for(int i=0; i<layer; i++)
	{
		cout << "\t";
	}
	
	return result;
}

int GIAtxtRelTranslatorNeuralNetworkOperationsClass::calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset)
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

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
//determines if reset is allowed
bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::resetAllowed(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;
	
	if((ownerGroup->groupName == "general") && (ownerGroup->groupTypeNameBackup == "referenceSets"))	//CHECKTHIS magic strings
	{
		if(existingActivatedComponentCapturesLastWordInSentence(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFound, forwardPropogationSentenceData))
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;

	GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;

	GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
	GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree = NULL;
	GIAtxtRelTranslatorRulesGroup* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	
	if(existingActivationFound)
	{
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
			int lastActiveComponentInParseTreeIndex = 0;
			#else
			int lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
			#endif
			GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];

			lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;

			if(existingActivatedComponentCapturesLastWordInSentence(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, forwardPropogationWordData, existingActivationFound, forwardPropogationSentenceData))
			{
				result = true;
			}
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	bool calculateMaxOrMin = true;
	int wordIndexMax1 = -1;
	int wordIndexMax2 = -1;
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMax1 = lastActiveComponentInParseTreeParseTreeGroupRef;
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMax2 = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
	int wordIndexMaxMin1 = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;	//CHECKTHIS
	int wordIndexMaxMin2 = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;	//CHECKTHIS
	#else
	bool calculateMaxOrMin = false;
	int wordIndexMax1 = REALLY_LARGE_INT;
	int wordIndexMax2 = REALLY_LARGE_INT;
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMax1 = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupToFindWordIndexMax2 = lastActiveComponentInParseTreeParseTreeGroupRef;
	int wordIndexMaxMin1 = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;
	int wordIndexMaxMin2 = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
	#endif

	if(parseTreeGroupToFindWordIndexMax1 != NULL)
	{
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, calculateMaxOrMin, &wordIndexMax1, 0))
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence error{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, )" << endl;	
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence pass{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, )" << endl;	
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMax1 = SHAREDvars.maxInt(wordIndexMax1, wordIndexMaxMin1);
	
	if(parseTreeGroupToFindWordIndexMax2 != NULL)
	{
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, calculateMaxOrMin, &wordIndexMax2, 0))
		{
			//newParseComponent->neuronComponentConnectionActiveWordRecord has not yet been defined
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence error{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, ) && " << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence pass{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, ) && " << endl;
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMax2 = SHAREDvars.maxInt(wordIndexMax2, wordIndexMaxMin2);

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
	cout << "wordIndexMax1 = " << wordIndexMax1 << endl;
	cout << "wordIndexMax2 = " << wordIndexMax2 << endl;
	//cout << "wordIndexMinMin = " << wordIndexMinMin << endl;
	cout << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	if(wordIndexMax1 == forwardPropogationSentenceData->sentenceContents->size()-1)	//ignore full stop
	#else
	if(wordIndexMax1 == 0)
	#endif
	{
		//existing activated component's parseTree contains last word in sentence 
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
		if(wordIndexMax2 == forwardPropogationSentenceData->sentenceContents->size()-1)	//ignore full stop
		#else
		if(wordIndexMax2 == 0)
		#endif
		{
			//overwritten/newly activated component's parseTree contains last word in sentence 
			result = true;
		}
	}
	else
	{
		result = true;
	}

	return result;
}
#endif


GIAtxtRelTranslatorRulesGroup* GIAtxtRelTranslatorNeuralNetworkOperationsClass::replicateParseTree(GIAtxtRelTranslatorRulesGroup* parseTreeGroupToReplicate, int level)
{
	bool result = false;

	GIAtxtRelTranslatorRulesGroup* parseTreeGroupNew = GIAtxtRelTranslatorRules.copyGroup(parseTreeGroupToReplicate);
	//parseTreeGroupNew->components.clear();
	
	for(int i=0; i<parseTreeGroupToReplicate->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (parseTreeGroupToReplicate->components)[i];
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{
			GIAtxtRelTranslatorRulesGroup* componentParseTreeGroupNew = replicateParseTree(currentComponent->parseTreeGroupRef, level+1);
			(parseTreeGroupNew->components)[i]->parseTreeGroupRef = componentParseTreeGroupNew;
			//parseTreeGroupNew->components.push_back(componentParseTreeGroupNew);
		}
	}
	
	return parseTreeGroupNew;
}




