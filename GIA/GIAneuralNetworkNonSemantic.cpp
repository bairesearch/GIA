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
 * File Name: GIAneuralNetworkNonSemantic.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k6a 29-March-2020
 * Description: Neural Network - visual representation of GIA contents in prototype biological neural network
 * /
 *******************************************************************************/


#include "GIAneuralNetworkNonSemantic.hpp"
#include "GIAsynRelTranslatorDefs.hpp"
#include "GIAposRelTranslatorHybridReferenceSet.hpp"	//requried for GIAposRelTranslatorHybrid objects

#ifdef GIA_NEURAL_NETWORK

#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE

bool GIAneuralNetworkNonSemanticClass::addTextToNetwork(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	//determine if concepts
	//For each reference set in the sentence preprocessor tree, perform a lookup (find the most activated subnet in neural net if existent; if not create a new one or modify the existing one as per method 2 above). Then create a new connection between the reference set subnets based on the logic reference delimiter (eg have/rides/etc).

	GIAneuralNetworkVariablesClass neuralNetworkVariables;
	neuralNetworkVariables.firstInputNeuronInNetwork = translatorVariables->firstInputNeuronInNetwork;
	GIAneuralNetworkNonSemanticOperations.initiateGIAneuralNetworkNonSemantic(&neuralNetworkVariables);
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		neuralNetworkVariables.sentenceIndex = currentGIApreprocessorSentenceInList->sentenceIndexOriginal;
		addTextToNetworkLogicReference(&neuralNetworkVariables, currentGIApreprocessorSentenceInList->firstLogicReferenceInList, NULL, true);
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	return result;
}

bool GIAneuralNetworkNonSemanticClass::addTextToNetworkLogicReference(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, ANNneuron* higherLogicReferenceArtificialSynapseNeuron, bool higherLogicReferenceArtificialSynapseNeuronDirection)
{
	bool result = true;
	
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			GIApreprocessorPlainTextWord* logicReferenceWord = getLogicReferenceWord(currentLogicReferenceInList);
			ANNneuron* higherLogicReferenceArtificialSynapseNeuron = findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, logicReferenceWord);
			
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(!addTextToNetworkLogicReference(neuralNetworkVariables, currentLogicReferenceInList->firstSubLogicReferenceInListArray, higherLogicReferenceArtificialSynapseNeuron, true))
				{
					result = false;
				}
			}
			else
			{
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!addTextToNetworkLogicReference(neuralNetworkVariables, currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, higherLogicReferenceArtificialSynapseNeuron, false))
					{
						result = false;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(!addTextToNetworkLogicReference(neuralNetworkVariables, currentLogicReferenceInList->firstSubLogicReferenceInListDependent, higherLogicReferenceArtificialSynapseNeuron, true))
				{
					result = false;
				}
			}
		}
		else
		{
		#endif	
			
			#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_REPLACE_WORDS_WITH_LEMMAS
			replaceWordsWithLemmas(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject);
			replaceWordsWithLemmas(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject);
			#endif
			determineReferenceSetDefinite(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject);
			determineReferenceSetDefinite(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject);
			
			bool indefiniteConceptDefinitionFound = false;
			if(detectIndefiniteConceptDefinition(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter))
			{
				indefiniteConceptDefinitionFound = true;
			}
			bool foundConceptSubject = false;
			bool foundConceptObject = false;
			ANNneuron* conceptNeuronSubject = NULL;
			ANNneuron* conceptNeuronObject = NULL;
		
			if(!identifyAndDemarcateConceptsInReferenceSet(neuralNetworkVariables, currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT, indefiniteConceptDefinitionFound, &foundConceptSubject, &conceptNeuronSubject))
			{
				result = false;
			}
			if(!identifyAndDemarcateConceptsInReferenceSet(neuralNetworkVariables, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT, indefiniteConceptDefinitionFound, &foundConceptObject, &conceptNeuronObject))
			{
				result = false;
			}	
			bool conceptDefinitionDetected = false;
			if(foundConceptSubject && foundConceptObject)
			{
				string firstWordInDelimiter = getDelimiterWord(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter)->tagName;
				if(indefiniteConceptDefinitionFound)
				{
					if(firstWordInDelimiter == GIA_TRANSLATOR_ENGLISH_INDEFINITE_CONCEPT_DEFINITION_DELIMITER)
					{
						conceptDefinitionDetected = true;
					}
				}
				else
				{
					if(firstWordInDelimiter == GIA_TRANSLATOR_ENGLISH_PLURAL_CONCEPT_DEFINITION_DELIMITER)
					{
						conceptDefinitionDetected = true;
					}
				}
			}
			
			if(conceptDefinitionDetected)
			{
				//create a connection between the concepts
				GIAneuralNetworkNonSemanticOperations.createANNconnection(conceptNeuronSubject, conceptNeuronObject, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT);
			}
			else
			{
				ANNneuron* referenceSetDelimiterSubnetEntry = NULL;
				ANNneuron* referenceSetSubjectSubnetEntry = NULL;
				ANNneuron* referenceSetObjectSubnetEntry = NULL;
			
				if(!createDelimiterArtificialSynapseNeuron(neuralNetworkVariables, &referenceSetDelimiterSubnetEntry, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter))
				{
					result = false;
				}

				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "addTextToNetworkLogicReference GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT, neuralNetworkVariables->sentenceIndex = " << neuralNetworkVariables->sentenceIndex << endl;
				#endif
				if(!findOrAddReferenceSetInNetwork(neuralNetworkVariables, currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, &referenceSetSubjectSubnetEntry, referenceSetDelimiterSubnetEntry, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT))
				{
					result = false;
				}
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "addTextToNetworkLogicReference GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT, neuralNetworkVariables->sentenceIndex = " << neuralNetworkVariables->sentenceIndex << endl;
				#endif
				if(!findOrAddReferenceSetInNetwork(neuralNetworkVariables, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, &referenceSetObjectSubnetEntry, referenceSetDelimiterSubnetEntry, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT))
				{
					result = false;
				}
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "addTextToNetworkLogicReference GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, neuralNetworkVariables->sentenceIndex = " << neuralNetworkVariables->sentenceIndex << endl;
				#endif
				
				if(!connectReferenceSetsInNetwork(neuralNetworkVariables, referenceSetSubjectSubnetEntry, referenceSetObjectSubnetEntry, &referenceSetDelimiterSubnetEntry, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter))
				{
					result = false;
				}
				#endif
				
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
				if(higherLogicReferenceArtificialSynapseNeuron != NULL)
				{
					if(higherLogicReferenceArtificialSynapseNeuronDirection)
					{
						GIAneuralNetworkNonSemanticOperations.createANNconnection(higherLogicReferenceArtificialSynapseNeuron, referenceSetDelimiterSubnetEntry, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);
					}
					else
					{
						GIAneuralNetworkNonSemanticOperations.createANNconnection(referenceSetDelimiterSubnetEntry, higherLogicReferenceArtificialSynapseNeuron, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);	
					}
				}
				#endif
			}
					
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		}
		#endif
		
		if(currentLogicReferenceInList->next != NULL)
		{
			currentLogicReferenceInList = currentLogicReferenceInList->next;
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}
	}
	
	return result;
}

#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_REPLACE_WORDS_WITH_LEMMAS
bool GIAneuralNetworkNonSemanticClass::replaceWordsWithLemmas(GIApreprocessorSubReferenceSet* referenceSet)
{
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = referenceSet;
	while(currentSubReferenceSetInList->next != NULL)
	{
		for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
		{
			GIApreprocessorPlainTextWord* word = (currentSubReferenceSetInList->subReferenceSetContents)[i];

			string baseName = "";
			int grammaticalBaseForm = INT_DEFAULT_VALUE;
			
			word->tagNameOriginalNonLemma = word->tagName;
			
			word->tagName = SHAREDvars.convertStringToLowerCase(&word->tagName);	//convert to lower case
			if(GIApreprocessorWordIdentification.determineIsVerb(word, &baseName, &grammaticalBaseForm))
			{
				word->tagName = baseName;
			}
			else if(GIApreprocessorWordIdentification.determineIsNoun(word, &baseName, &grammaticalBaseForm))
			{
				word->tagName = baseName;
			}
		}
	
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;
	}
}
#endif

bool GIAneuralNetworkNonSemanticClass::detectIndefiniteConceptDefinition(GIApreprocessorSubReferenceSet* referenceSetSubject, GIApreprocessorSubReferenceSet* referenceSetObject, GIApreprocessorSubReferenceSet* referenceSetDelimiter)
{
	//assume there is only 1 subreference set in both referenceSetSubject and referenceSetObject
	
	bool indefiniteConceptDefinitionFound = false;
	if((referenceSetSubject->subReferenceSetContents.size() > 0) && (referenceSetObject->subReferenceSetContents.size() > 0) && (referenceSetDelimiter->subReferenceSetContents.size() > 0))
	{
		indefiniteConceptDefinitionFound = true;	//eg "a dog is an animal"
		
		string firstWordInSubject = ((referenceSetSubject->subReferenceSetContents)[0])->tagName;
		if(!SHAREDvars.textInTextArray(firstWordInSubject, grammaticalDeterminerIndefiniteArray, GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES))
		{
			indefiniteConceptDefinitionFound = false;
		}
			
		string firstWordInDelimiter = ((referenceSetDelimiter->subReferenceSetContents)[0])->tagName;
		if(firstWordInDelimiter != GIA_TRANSLATOR_ENGLISH_INDEFINITE_CONCEPT_DEFINITION_DELIMITER)
		{
			indefiniteConceptDefinitionFound = false;
		}

		string firstWordInObject = ((referenceSetObject->subReferenceSetContents)[0])->tagName;
		if(!SHAREDvars.textInTextArray(firstWordInObject, grammaticalDeterminerIndefiniteArray, GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES))
		{
			indefiniteConceptDefinitionFound = false;
		}
	}
	
	return indefiniteConceptDefinitionFound;
}	
		
bool GIAneuralNetworkNonSemanticClass::identifyAndDemarcateConceptsInReferenceSet(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, int referenceSetType, bool indefiniteConceptDefinitionFound, bool* foundConcept, ANNneuron** conceptNeuronFound)
{
	bool result = true;
	
	for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
	{
		GIApreprocessorPlainTextWord* word = (currentSubReferenceSetInList->subReferenceSetContents)[i];

		bool specificConceptDetected = false;
		int indexOfStartOfSpecificConcept = false;
		bool conceptDefinitionDetected = false;
		bool isConcept = detectIfWordIsConcept(&(currentSubReferenceSetInList->subReferenceSetContents), i, &specificConceptDetected, &indexOfStartOfSpecificConcept, indefiniteConceptDefinitionFound);

		//verify that a concept has been created for every word (regardless of whether the word itself is a concept)
		ANNneuron* conceptNeuron = NULL;
		if(!GIAneuralNetworkNonSemanticOperations.findConceptInNetwork(neuralNetworkVariables, word, &conceptNeuron, specificConceptDetected, &(currentSubReferenceSetInList->subReferenceSetContents), indexOfStartOfSpecificConcept, i))
		{
			conceptNeuron = GIAneuralNetworkNonSemanticOperations.addConceptToNetwork(neuralNetworkVariables, word, specificConceptDetected, &(currentSubReferenceSetInList->subReferenceSetContents), indexOfStartOfSpecificConcept, i);
		}
		
		if(isConcept)
		{
			if(specificConceptDetected)
			{
				word->neuralNetworkPreprocessorWordType = GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_SPECIFIC_CONCEPT;
			}
			else
			{
				word->neuralNetworkPreprocessorWordType = GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_CONCEPT;
			}
			
			*foundConcept = true;
			*conceptNeuronFound = conceptNeuron;
		}
	}	
	
	return result;
}

bool GIAneuralNetworkNonSemanticClass::detectIfWordIsConcept(const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, int wordIndex, bool* specificConceptDetected, int* indexOfStartOfSpecificConcept, bool indefiniteConceptDefinitionFound)
{
	bool isConcept = false;
	
	GIApreprocessorPlainTextWord* wordTag = ((*subReferenceSetContents)[wordIndex]);
	string word = wordTag->tagName;

	//this method required refinement;
	bool indefiniteConceptFound = false;
	if((wordIndex > 0) && (wordIndex < subReferenceSetContents->size()-2))
	{		
		string preceedingWord = ((*subReferenceSetContents)[wordIndex-1])->tagName;
		if(SHAREDvars.textInTextArray(preceedingWord, grammaticalDeterminerIndefiniteArray, GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES))
		{
			if(indefiniteConceptDefinitionFound)
			{
				indefiniteConceptFound = true;	//eg "a dog is an animal"
			}
		}
	}

	bool pluralConceptFound = false;
	if(wordIndex > 0)
	{
		pluralConceptFound = true;
		
		string preceedingWord = ((*subReferenceSetContents)[wordIndex-1])->tagName;
		if(detectIfWordIsDeterminer(preceedingWord))
		{
			pluralConceptFound = false;
		}

		GIApreprocessorMultiwordReductionWord* nounBaseFormFoundTemp;
		if(!GIApreprocessorWordIdentification.determineNounPluralVariant(wordTag->tagName, &nounBaseFormFoundTemp))
		{
			pluralConceptFound = false;
		}
	}
	
	if(indefiniteConceptFound || pluralConceptFound)
	{
		#ifdef GIA_DEBUG_NEURAL_NETWORK_ACTIVE
		cout << "isConcept, word = " << word << endl;
		#endif
		isConcept = true;
		
		if(wordIndex > 0)
		{
			if(findIndexOfStartOfSpecificConcept(subReferenceSetContents, wordIndex-1, indexOfStartOfSpecificConcept))
			{
				*specificConceptDetected = true;
			}
		}
	}
	
	return isConcept;
}


bool GIAneuralNetworkNonSemanticClass::detectIfWordIsDeterminer(const string word)
{
	bool isDeterminer = false;
	
	if(SHAREDvars.textInTextArray(word, translatorEnglishArticlesArray, GIA_TRANSLATOR_ENGLISH_ARTICLES_NUMBER_OF_TYPES))
	{
		isDeterminer = true;
	}	
	if(SHAREDvars.textInTextArray(word, translatorEnglishPossessiveDeterminersArray, GIA_TRANSLATOR_ENGLISH_POSSESSIVE_DETERMINERS_NUMBER_OF_TYPES))
	{
		isDeterminer = true;
	}
	if(SHAREDvars.textInTextArray(word, translatorEnglishNumbersArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMBER_OF_TYPES))
	{
		isDeterminer = true;
	}
	if(SHAREDvars.textInTextArray(word, translatorEnglishIndefinitePronounsPluralArray, GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_NUMBER_OF_TYPES))
	{
		isDeterminer = true;
	}
	if(SHAREDvars.textInTextArray(word, translatorEnglishIndefinitePronounsPluralOrSingularArray, GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_OR_SINGULAR_NUMBER_OF_TYPES))
	{
		isDeterminer = true;
	}
	if(SHAREDvars.textInTextArray(word, translatorEnglishDemonstrativePronounsPluralArray, GIA_TRANSLATOR_ENGLISH_DEMONSTRATIVE_PRONOUNS_PLURAL_NUMBER_OF_TYPES))
	{
		isDeterminer = true;
	}
		
	return isDeterminer;
}

bool GIAneuralNetworkNonSemanticClass::findIndexOfStartOfSpecificConcept(const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int startIndexToSearch, int* indexOfStartOfSpecificConcept)
{
	bool foundSpecificConcept = false;
	int indexOfSearch = startIndexToSearch;
	bool stillSearching = true;
	bool foundPreceedingAdjectiveOrNoun = false;
	while(stillSearching)
	{
		GIApreprocessorPlainTextWord* currentWordTag = (*subReferenceSetContents)[indexOfSearch];
		GIApreprocessorMultiwordReductionWord* nounBaseFormFound = NULL;
		if(GIApreprocessorWordIdentification.determineIsAdjective(currentWordTag))
		{
			//stillSearching = true;	//e.g. blue dog(s)
			foundPreceedingAdjectiveOrNoun = true;
		}
		else if(GIApreprocessorWordIdentification.determineIsNoun(currentWordTag))
		{
			//stillSearching = true;	//e.g. house dog(s)
			foundPreceedingAdjectiveOrNoun = true;
		}
		else
		{
			stillSearching = false;
			if(foundPreceedingAdjectiveOrNoun)
			{
				foundSpecificConcept = true;
				*indexOfStartOfSpecificConcept = indexOfSearch+1;
			}
		}
		
		if(indexOfSearch = 0)
		{
			//start of sentence (/subsentence)
			stillSearching = false;
		}
		
		indexOfSearch = indexOfSearch - 1;
	}
	
	return foundSpecificConcept;
}


	
	
bool GIAneuralNetworkNonSemanticClass::findOrAddReferenceSetInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, int referenceSetType)
{
	bool result = true;
	
	int xPosRel = 0;
	ANNneuron* firstConceptNeuron = GIAneuralNetworkNonSemanticOperations.getFirstConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* firstSpecificConceptNeuron = GIAneuralNetworkNonSemanticOperations.getFirstSpecificConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* firstSynapseArtificialInstanceNeuron = GIAneuralNetworkNonSemanticOperations.getFirstInstanceNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentSynapseArtificialInstanceNeuron = GIAneuralNetworkNonSemanticOperations.getLastNeuronInLayer(firstSynapseArtificialInstanceNeuron, &xPosRel);
	
	if(firstSubReferenceSetInList->definite)
	{
		//activate subnet concept neurons
		int idealNumberOfActiveConceptNeuronsInSubnet = 0;
		GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
		while(currentSubReferenceSetInList->next != NULL)
		{
			for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
			{
				GIApreprocessorPlainTextWord* word = (currentSubReferenceSetInList->subReferenceSetContents)[i];
				if(word->neuralNetworkPreprocessorWordType != GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_IGNORE)
				{
					ANNneuron* conceptNeuron = word->wordShortcutToConceptNeuron;		//ANNneuron* conceptNeuron = findConceptInNetwork(wordTag);
					conceptNeuron->GIAactiveForSubnetIdentification = true;
					#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_BYPASS_AUXILIARIES
					if(!(currentSubReferenceSetInList->isReferenceSetDelimiter) || (currentSubReferenceSetInList->delimiterType != GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY))
					{
					#endif
						idealNumberOfActiveConceptNeuronsInSubnet++;
					#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_BYPASS_AUXILIARIES
					}
					#endif
				}
			}
			currentSubReferenceSetInList = currentSubReferenceSetInList->next;
		}

		//identify the closest matching subnet in network
		bool foundExistingSubnet = false;
		int maxNumberOfActiveConceptNeuronsInSubnet = 0;
		int64_t activationAgeOfExistingSubnet = 0;
		ANNneuronConnection* connectionEntryToSubnetWithMaximumNumberActiveConceptNeuronsConnected = NULL;
		currentSubReferenceSetInList = firstSubReferenceSetInList;
		while(currentSubReferenceSetInList->next != NULL)
		{
			for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
			{
				GIApreprocessorPlainTextWord* word = (currentSubReferenceSetInList->subReferenceSetContents)[i];
				ANNneuron* conceptNeuron = word->wordShortcutToConceptNeuron;		//ANNneuron* conceptNeuron = findConceptInNetwork(wordTag);
				for(int i=0; i<conceptNeuron->frontANNneuronConnectionList.size(); i++)
				{
					ANNneuronConnection* connection = (conceptNeuron->frontANNneuronConnectionList)[i];
					if(connection->GIAconnectionType == GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE)
					{
						int numberOfActiveConceptNeuronsInSubnet = 1;
						int64_t activationAgeOfExistingSubnetTemp = 0;
						calculateNumberActiveConceptNeuronsInSubnet(connection, &numberOfActiveConceptNeuronsInSubnet, &activationAgeOfExistingSubnetTemp);
						calculateNumberActiveConceptNeuronsInSubnetReset(connection);
						if((numberOfActiveConceptNeuronsInSubnet > maxNumberOfActiveConceptNeuronsInSubnet) || ((numberOfActiveConceptNeuronsInSubnet == maxNumberOfActiveConceptNeuronsInSubnet) && (activationAgeOfExistingSubnetTemp < activationAgeOfExistingSubnet)))
						{
							maxNumberOfActiveConceptNeuronsInSubnet = numberOfActiveConceptNeuronsInSubnet;
							connectionEntryToSubnetWithMaximumNumberActiveConceptNeuronsConnected = connection;
							activationAgeOfExistingSubnet = activationAgeOfExistingSubnetTemp;
							foundExistingSubnet = true;
						}
					}
				}
			}
			currentSubReferenceSetInList = currentSubReferenceSetInList->next;
		}

		if(foundExistingSubnet && (idealNumberOfActiveConceptNeuronsInSubnet-maxNumberOfActiveConceptNeuronsInSubnet <= GIA_NEURAL_NETWORK_NON_SEMANTIC_REFERENCE_SET_IDENTIFICATION_MAX_ERROR))
		{
			if(maxNumberOfActiveConceptNeuronsInSubnet >= idealNumberOfActiveConceptNeuronsInSubnet)
			{
				//use existing subnet
				cout << "use existing subnet" << endl;
				*referenceSetSubnetEntry = connectionEntryToSubnetWithMaximumNumberActiveConceptNeuronsConnected->frontNeuron;
			}
			else
			{
				//adapt existing subnet (add or bypass connections)
				cout << "adapt existing subnet" << endl;

				int numberOfActiveConceptNeuronsInSubnet = 1;
				int64_t activationAgeOfExistingSubnetTemp = 0;
				calculateNumberActiveConceptNeuronsInSubnet(connectionEntryToSubnetWithMaximumNumberActiveConceptNeuronsConnected, &numberOfActiveConceptNeuronsInSubnet, &activationAgeOfExistingSubnetTemp);	//this will mark which active concept neurons are used by best case subnet

				ANNneuron* conceptNeuronPrevious = NULL;
				ANNneuron* previousConceptNeuronOrConnectedSynapse = NULL;
				GIApreprocessorPlainTextWord* previousWord = NULL; 
				
				bool firstValidWordInReferenceSet = true;
				
				GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
				while(currentSubReferenceSetInList->next != NULL)
				{
					for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
					{
						GIApreprocessorPlainTextWord* word = (currentSubReferenceSetInList->subReferenceSetContents)[i];
						if(word->neuralNetworkPreprocessorWordType != GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_IGNORE)
						{
							bool currentWordIsConcept = false;
							if((word->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_CONCEPT) || (word->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_SPECIFIC_CONCEPT))
							{
								currentWordIsConcept = true;
							}

							bool directConnectionAlreadyMade = false;
							ANNneuron* artificialInstanceNeuronWithDirectConnection = NULL;

							ANNneuron* conceptNeuron = word->wordShortcutToConceptNeuron;		//ANNneuron* conceptNeuron = findConceptInNetwork(wordTag);
							if(conceptNeuron->GIAalreadyParsed)
							{
								ANNneuronConnection* conceptConnectionFound = NULL; 
								for(int i=0; i<conceptNeuron->frontANNneuronConnectionList.size(); i++)
								{
									ANNneuronConnection* conceptConnection = (conceptNeuron->frontANNneuronConnectionList)[i];
									if(conceptConnection->GIAconnectionType == GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE) //|| GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE?	//CHECKTHIS (currently do not reference existing specific concept definitions; eg "red dogs that eat pies..."
									{
										if(conceptConnection->GIAalreadyParsed)
										{
											ANNneuron* artificialInstanceNeuron = conceptConnection->frontNeuron;
											for(int i=0; i<artificialInstanceNeuron->backANNneuronConnectionList.size(); i++)
											{	
												ANNneuronConnection* artificialInstanceNeuronConnectionReverse = (artificialInstanceNeuron->backANNneuronConnectionList)[i];
												if(artificialInstanceNeuronConnectionReverse != conceptConnection)
												{
													if(artificialInstanceNeuronConnectionReverse->GIAalreadyParsed)	//redundant
													{
														ANNneuron* artificialInstanceNeuronPrevious = artificialInstanceNeuronConnectionReverse->backNeuron;
														if(artificialInstanceNeuronPrevious->GIAisConceptEntity)
														{
															if(artificialInstanceNeuronPrevious == conceptNeuronPrevious)	//CHECKTHIS what if conceptNeuronPrevious not yet defined?
															{
																directConnectionAlreadyMade = true;	
																artificialInstanceNeuronWithDirectConnection = artificialInstanceNeuron;
															}
														}
														
														for(int i=0; i<artificialInstanceNeuronPrevious->backANNneuronConnectionList.size(); i++)
														{	
															ANNneuronConnection* artificialInstanceNeuronPreviousConnectionReverse = (artificialInstanceNeuronPrevious->backANNneuronConnectionList)[i];
															if(artificialInstanceNeuronPreviousConnectionReverse->GIAalreadyParsed)	//redundant
															{
																ANNneuron* conceptNeuronPreviousTemp2 = artificialInstanceNeuronPreviousConnectionReverse->backNeuron;
																if(conceptNeuronPreviousTemp2 == conceptNeuronPrevious)	//CHECKTHIS what if conceptNeuronPrevious not yet defined?
																{
																	directConnectionAlreadyMade = true;	
																	artificialInstanceNeuronWithDirectConnection = artificialInstanceNeuron;
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}

							if(directConnectionAlreadyMade)	//subnet already contains a direct connection between current word and previous word
							{
								previousConceptNeuronOrConnectedSynapse = artificialInstanceNeuronWithDirectConnection;
								firstValidWordInReferenceSet = false;
							}
							else
							{
								//connect previousConceptNeuronOrConnectedSynapse to new concept/synapse	//CHECKTHIS
								if(firstValidWordInReferenceSet)
								{
									previousConceptNeuronOrConnectedSynapse = conceptNeuron;
									firstValidWordInReferenceSet = false;
								}
								else
								{
									if(previousConceptNeuronOrConnectedSynapse == NULL)
									{
										cout << "error: (previousConceptNeuronOrConnectedSynapse == NULL)" << endl;
										exit(EXIT_ERROR);
									}

									bool ANNconnectionType1 = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
									if((previousWord->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_CONCEPT) || (previousWord->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_SPECIFIC_CONCEPT))	//OLD: if(previousConceptNeuronOrConnectedSynapse->GIAisConceptEntity)
									{
										ANNconnectionType1 = GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE;	//CHECKTHIS
									}
									else
									{
										ANNconnectionType1 = GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE;
									}


									bool ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
									if(currentWordIsConcept)
									{
										ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE;
									}
									else
									{
										ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE;
									}

									int artificialLayer = generateArtificialLayer(neuralNetworkVariables);
									ANNneuron* newNeuron = GIAneuralNetworkNonSemanticOperations.createNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, &currentSynapseArtificialInstanceNeuron, word->tagName, GIAneuralNetworkNonSemanticOperations.calculateNumberOfInstancesOfConceptNeuron(conceptNeuron), conceptNeuron, artificialLayer, ANNconnectionType2);
									GIAneuralNetworkNonSemanticOperations.createANNconnection(previousConceptNeuronOrConnectedSynapse, newNeuron, ANNconnectionType1);
									previousConceptNeuronOrConnectedSynapse = newNeuron;
								}
							}
							conceptNeuronPrevious = conceptNeuron;
						}
					}
					currentSubReferenceSetInList = currentSubReferenceSetInList->next;
				}

				calculateNumberActiveConceptNeuronsInSubnetReset(connectionEntryToSubnetWithMaximumNumberActiveConceptNeuronsConnected);

				//update the activation age of every synapse in the modified subnet;
				calculateNumberActiveConceptNeuronsInSubnetUpdateActivationAge(connectionEntryToSubnetWithMaximumNumberActiveConceptNeuronsConnected);
				calculateNumberActiveConceptNeuronsInSubnetReset(connectionEntryToSubnetWithMaximumNumberActiveConceptNeuronsConnected);
			}
		}
		else
		{
			//create a new subnet
			cout << "create a new subnet" << endl;
			if(!addReferenceSetInNetwork(neuralNetworkVariables, firstSubReferenceSetInList, referenceSetSubnetEntry, referenceSetDelimiterSubnetEntry, referenceSetType))
			{
				result = false;
			}
		}

		//deactivate subnet concept neurons
		currentSubReferenceSetInList = firstSubReferenceSetInList;
		while(currentSubReferenceSetInList->next != NULL)
		{
			for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
			{
				GIApreprocessorPlainTextWord* word = (currentSubReferenceSetInList->subReferenceSetContents)[i];
				if(word->neuralNetworkPreprocessorWordType != GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_IGNORE)
				{
					ANNneuron* conceptNeuron = word->wordShortcutToConceptNeuron;	//ANNneuron* conceptNeuron = findConceptInNetwork(wordTag);
					conceptNeuron->GIAactiveForSubnetIdentification = false;
				}
			}
			currentSubReferenceSetInList = currentSubReferenceSetInList->next;
		}
	}
	else
	{
		//CHECKTHIS: duplicate the subnet for new instances of an object
		//create a new subnet
		if(!addReferenceSetInNetwork(neuralNetworkVariables, firstSubReferenceSetInList, referenceSetSubnetEntry, referenceSetDelimiterSubnetEntry, referenceSetType))
		{
			result = false;
		}		
	}

	return result;
}

void GIAneuralNetworkNonSemanticClass::calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, int* numberOfActiveConceptNeuronsInSubnet, int64_t* activationAgeOfSubnetSynapsesTotal)
{
	bool direction = true;
	bool reset = false;
	bool updateActivationAge = false;
	calculateNumberActiveConceptNeuronsInSubnet(currentNeuronConnectionInInstanceSubnet, direction, numberOfActiveConceptNeuronsInSubnet, activationAgeOfSubnetSynapsesTotal, reset, updateActivationAge);
}

void GIAneuralNetworkNonSemanticClass::calculateNumberActiveConceptNeuronsInSubnetReset(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet)
{
	bool direction = true;
	int numberOfActiveConceptNeuronsInSubnet = 1;
	int64_t activationAgeOfSubnetSynapsesTotal = 0;
	bool reset = true;
	bool updateActivationAge = false;
	calculateNumberActiveConceptNeuronsInSubnet(currentNeuronConnectionInInstanceSubnet, direction, &numberOfActiveConceptNeuronsInSubnet, &activationAgeOfSubnetSynapsesTotal, reset, updateActivationAge);
}

void GIAneuralNetworkNonSemanticClass::calculateNumberActiveConceptNeuronsInSubnetUpdateActivationAge(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet)
{
	bool direction = true;
	int numberOfActiveConceptNeuronsInSubnet = 1;
	int64_t activationAgeOfSubnetSynapsesTotal = 0;
	bool reset = false;
	bool updateActivationAge = true;
	calculateNumberActiveConceptNeuronsInSubnet(currentNeuronConnectionInInstanceSubnet, direction, &numberOfActiveConceptNeuronsInSubnet, &activationAgeOfSubnetSynapsesTotal, reset, updateActivationAge);
}


void GIAneuralNetworkNonSemanticClass::calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, bool direction, int* numberOfActiveConceptNeuronsInSubnet, int64_t* activationAgeOfSubnetSynapsesTotal, bool reset, bool updateActivationAge)
{
	ANNneuron* artificialInstanceNeuron = NULL;
	if(direction)
	{
		artificialInstanceNeuron = currentNeuronConnectionInInstanceSubnet->frontNeuron;
	}
	else
	{
		artificialInstanceNeuron = currentNeuronConnectionInInstanceSubnet->backNeuron;
	}
		
	bool parse = false;
	if(!reset && !(currentNeuronConnectionInInstanceSubnet->GIAalreadyParsed))
	{
		artificialInstanceNeuron->GIAalreadyParsed = true;
		currentNeuronConnectionInInstanceSubnet->GIAalreadyParsed = true; 
		parse = true;
	}
	else if(reset && (currentNeuronConnectionInInstanceSubnet->GIAalreadyParsed))
	{
		artificialInstanceNeuron->GIAalreadyParsed = false;
		currentNeuronConnectionInInstanceSubnet->GIAalreadyParsed = false;
		parse = true;
	}
	
	if(parse)
	{
		if(!reset)
		{
			if(updateActivationAge)
			{
				artificialInstanceNeuron->GIAactivationAge = GIAneuralNetworkNonSemanticOperations.getCurrentTime();
			}
			else
			{
				*activationAgeOfSubnetSynapsesTotal = *activationAgeOfSubnetSynapsesTotal + artificialInstanceNeuron->GIAactivationAge;
			}
		}

		if(artificialInstanceNeuron->GIAisConceptEntity)
		{
			if(!reset)
			{
				if(artificialInstanceNeuron->GIAactiveForSubnetIdentification)
				{
					*numberOfActiveConceptNeuronsInSubnet = *numberOfActiveConceptNeuronsInSubnet + 1;
				}
			}
		}
		else
		{
			for(int i=0; i<artificialInstanceNeuron->frontANNneuronConnectionList.size(); i++)
			{
				ANNneuronConnection* connection = (artificialInstanceNeuron->frontANNneuronConnectionList)[i];
				calculateNumberActiveConceptNeuronsInSubnet(connection, true, numberOfActiveConceptNeuronsInSubnet, activationAgeOfSubnetSynapsesTotal, reset, updateActivationAge);
			}
			for(int i=0; i<artificialInstanceNeuron->backANNneuronConnectionList.size(); i++)
			{
				ANNneuronConnection* connection = (artificialInstanceNeuron->backANNneuronConnectionList)[i];
				calculateNumberActiveConceptNeuronsInSubnet(connection, false, numberOfActiveConceptNeuronsInSubnet, activationAgeOfSubnetSynapsesTotal, reset, updateActivationAge);
			}
		}
	}
}




bool GIAneuralNetworkNonSemanticClass::addReferenceSetInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, int referenceSetType)
{
	bool result = true;
	
	int xPosRel = 0;
	ANNneuron* firstSynapseArtificialInstanceNeuron = GIAneuralNetworkNonSemanticOperations.getFirstInstanceNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentSynapseArtificialInstanceNeuron = GIAneuralNetworkNonSemanticOperations.getLastNeuronInLayer(firstSynapseArtificialInstanceNeuron, &xPosRel);
	
	bool referenceSetSubnetEntryCreated = false;
	GIApreprocessorPlainTextWord* firstLegalWordInSubnet = NULL;
	bool firstWordInReferenceSet = true;
	bool firstInstanceNeuronInReferenceSet = false;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
	ANNneuron* lastSubreferenceSetSubnetEntry = NULL;
	ANNneuron* lastVerbSubreferenceSetSubnetEntry = NULL;
	#endif
	
	ANNneuron* previousConceptNeuronOrConnectedSynapse = NULL;

	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_CREATE_DIRECT_CONNECTION_BETWEEN_DELIMITER_AND_OBJECT
	if(*referenceSetSubnetEntry != NULL)
	{//object of referenceset full (referenceSetSubnetEntry = delimiter)
		cout << "GIAneuralNetworkNonSemanticClass::addReferenceSetInNetwork : (*referenceSetSubnetEntry != NULL); using referenceSetSubnetEntry as previousConceptNeuronOrConnectedSynapse" << endl;
		referenceSetSubnetEntryCreated = true;
		firstWordInReferenceSet = false;
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
		lastSubreferenceSetSubnetEntry = *referenceSetSubnetEntry;
		#endif
		previousConceptNeuronOrConnectedSynapse = *referenceSetSubnetEntry
	}
	#endif
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_BYPASS_AUXILIARIES
	bool createBypassAuxiliaryConnection = false;
	ANNneuron* lastSubreferenceSetSubnetEntryNonDelimiter = NULL;
	#endif
	
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
		if(currentSubReferenceSetInList->isReferenceSetDelimiter)
		{
			if(referenceSetSubnetEntryCreated)
			{
				GIApreprocessorPlainTextWord* delimiterWordTag = getDelimiterWord(currentSubReferenceSetInList);
				string delimiterWord = delimiterWordTag->tagName;
		
				ANNneuron* newArtificialSynapseNeuron = findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, delimiterWordTag);
				
				//CHECKTHIS; GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB
						
				if(currentSubReferenceSetInList->delimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB)
				{
					if(lastVerbSubreferenceSetSubnetEntry != NULL)
					{
						GIAneuralNetworkNonSemanticOperations.createANNconnection(lastVerbSubreferenceSetSubnetEntry, newArtificialSynapseNeuron, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);
						previousConceptNeuronOrConnectedSynapse = lastVerbSubreferenceSetSubnetEntry;
					}
					else
					{
						//assume verb was stored in the delimiter of the full reference set
						GIAneuralNetworkNonSemanticOperations.createANNconnection(referenceSetDelimiterSubnetEntry, newArtificialSynapseNeuron, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);
						previousConceptNeuronOrConnectedSynapse = referenceSetDelimiterSubnetEntry;
					}
				}
				else
				{
					GIAneuralNetworkNonSemanticOperations.createANNconnection(lastSubreferenceSetSubnetEntry, newArtificialSynapseNeuron, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);
					previousConceptNeuronOrConnectedSynapse = lastSubreferenceSetSubnetEntry;
				}
				
				#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_BYPASS_AUXILIARIES
				if(currentSubReferenceSetInList->delimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)
				{
					createBypassAuxiliaryConnection = true;
					lastSubreferenceSetSubnetEntryNonDelimiter = lastSubreferenceSetSubnetEntry;
				}
				#endif
		
				if(currentSubReferenceSetInList->delimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB)
				{
					lastVerbSubreferenceSetSubnetEntry = newArtificialSynapseNeuron;
				}
				lastSubreferenceSetSubnetEntry = newArtificialSynapseNeuron;
			}
			else
			{
				cout << "GIAneuralNetworkNonSemanticClass::addReferenceSetInNetwork error: (currentSubReferenceSetInList->isReferenceSetDelimiter) && !referenceSetSubnetEntryCreated" << endl;
				exit(EXIT_ERROR);
			}
		}
		else
		{
		#endif
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
			bool firstInstanceNeuronInSubReferenceSet = true;
			#endif
			
			GIApreprocessorPlainTextWord* previousWord = NULL;
			for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
			{
				GIApreprocessorPlainTextWord* word = (currentSubReferenceSetInList->subReferenceSetContents)[i];
				if(word->neuralNetworkPreprocessorWordType != GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_IGNORE)
				{
					//cout << "word = " << word->tagName << endl;
					//cout << "word->neuralNetworkPreprocessorWordType = " << word->neuralNetworkPreprocessorWordType << endl;

					bool currentWordIsConcept = false;
					if((word->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_CONCEPT) || (word->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_SPECIFIC_CONCEPT))
					{
						currentWordIsConcept = true;
					}

					ANNneuron* conceptNeuron = word->wordShortcutToConceptNeuron;

					if(firstWordInReferenceSet)
					{
						previousConceptNeuronOrConnectedSynapse = conceptNeuron;
						firstWordInReferenceSet = false;
						firstInstanceNeuronInReferenceSet = true;
					}
					else
					{
						//connect previousConceptNeuronOrConnectedSynapse to new concept/synapse	//CHECKTHIS

						bool ANNconnectionType1 = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
						if((previousWord->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_CONCEPT) || (previousWord->neuralNetworkPreprocessorWordType == GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_SPECIFIC_CONCEPT))	//OLD: if(previousConceptNeuronOrConnectedSynapse->GIAisConceptEntity)
						{
							ANNconnectionType1 = GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE;	//CHECKTHIS
						}
						else
						{
							ANNconnectionType1 = GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE;
						}


						bool ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
						if(currentWordIsConcept)
						{
							ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE;
						}
						else
						{
							ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE;
						}

						int artificialLayer = generateArtificialLayer(neuralNetworkVariables);
						ANNneuron* newNeuron = GIAneuralNetworkNonSemanticOperations.createNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, &currentSynapseArtificialInstanceNeuron, word->tagName, GIAneuralNetworkNonSemanticOperations.calculateNumberOfInstancesOfConceptNeuron(conceptNeuron), conceptNeuron, artificialLayer, ANNconnectionType2);
						ANNneuronConnection* newConnection = GIAneuralNetworkNonSemanticOperations.createANNconnection(previousConceptNeuronOrConnectedSynapse, newNeuron, ANNconnectionType1);
						if(firstInstanceNeuronInReferenceSet)
						{
							*referenceSetSubnetEntry = newNeuron;
							referenceSetSubnetEntryCreated = true;
							firstInstanceNeuronInReferenceSet = false;
						}
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
						if(firstInstanceNeuronInSubReferenceSet)
						{
							firstInstanceNeuronInSubReferenceSet = false;
							lastSubreferenceSetSubnetEntry = newNeuron;
							
							#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_BYPASS_AUXILIARIES
							if(createBypassAuxiliaryConnection)
							{
								createBypassAuxiliaryConnection = false;
								GIAneuralNetworkNonSemanticOperations.createANNconnection(lastSubreferenceSetSubnetEntryNonDelimiter, newNeuron, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);
							}
							#endif
						}
						#endif
						previousConceptNeuronOrConnectedSynapse = newNeuron;
					}

					firstLegalWordInSubnet = word;
				}

				previousWord = word;
			}
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
		}
		#endif
		
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
	}
	
	if(!referenceSetSubnetEntryCreated)
	{
		*referenceSetSubnetEntry = findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, firstLegalWordInSubnet);
	}
	
	return result;
}



	
bool GIAneuralNetworkNonSemanticClass::createDelimiterArtificialSynapseNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** referenceSetDelimiterSubnetEntry, GIApreprocessorSubReferenceSet* referenceSetDelimiter)
{
	bool result = true;
	
	if(!(referenceSetDelimiter->subReferenceSetContents).empty())
	{
		//FUTURE: connect the subject and object of each reference set (rather than the delimiters if existent)
		
		GIApreprocessorPlainTextWord* delimiterWordTag = getDelimiterWord(referenceSetDelimiter);
		string delimiterWord = delimiterWordTag->tagName;
			
		ANNneuron* delimiterArtificialSynapseNeuron = findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, delimiterWordTag);
		
		*referenceSetDelimiterSubnetEntry = delimiterArtificialSynapseNeuron;
	}
	else
	{
		cout << "connectReferenceSetsInNetwork, (referenceSetDelimiter->subReferenceSetContents).empty(), neuralNetworkVariables->sentenceIndex  = " << neuralNetworkVariables->sentenceIndex << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}

bool GIAneuralNetworkNonSemanticClass::connectReferenceSetsInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* referenceSetSubjectSubnetEntry, ANNneuron* referenceSetObjectSubnetEntry, ANNneuron** referenceSetDelimiterSubnetEntry, GIApreprocessorSubReferenceSet* referenceSetDelimiter)
{
	bool result = true;
	
	if(!(referenceSetDelimiter->subReferenceSetContents).empty())
	{
		//FUTURE: connect the subject and object of each reference set (rather than the delimiters if existent)
		
		if(*referenceSetDelimiterSubnetEntry == NULL)
		{
			if(!createDelimiterArtificialSynapseNeuron(neuralNetworkVariables, referenceSetDelimiterSubnetEntry, referenceSetDelimiter))
			{
				result = false;
			}
		}
		
		GIAneuralNetworkNonSemanticOperations.createANNconnection(referenceSetSubjectSubnetEntry, *referenceSetDelimiterSubnetEntry, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);
		GIAneuralNetworkNonSemanticOperations.createANNconnection(*referenceSetDelimiterSubnetEntry, referenceSetObjectSubnetEntry, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);	
	
		#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_BYPASS_AUXILIARIES
		if(referenceSetDelimiter->delimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)
		{
			GIAneuralNetworkNonSemanticOperations.createANNconnection(referenceSetSubjectSubnetEntry, referenceSetObjectSubnetEntry, GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE);
		}
		#endif
	}
	else
	{
		cout << "connectReferenceSetsInNetwork, (referenceSetDelimiter->subReferenceSetContents).empty(), neuralNetworkVariables->sentenceIndex  = " << neuralNetworkVariables->sentenceIndex << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}

ANNneuron* GIAneuralNetworkNonSemanticClass::findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag)
{
	ANNneuron* newNeuron = NULL;
	
	int xPosRel = 0;
	
	ANNneuron* firstSynapseArtificialInstanceNeuron = GIAneuralNetworkNonSemanticOperations.getFirstInstanceNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentSynapseArtificialInstanceNeuron = GIAneuralNetworkNonSemanticOperations.getLastNeuronInLayer(firstSynapseArtificialInstanceNeuron, &xPosRel);

	ANNneuron* conceptNeuron = NULL;
	if(!GIAneuralNetworkNonSemanticOperations.findConceptInNetwork(neuralNetworkVariables, wordTag, &conceptNeuron))
	{
		conceptNeuron = GIAneuralNetworkNonSemanticOperations.addConceptToNetwork(neuralNetworkVariables, wordTag);
	}

	int ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE;

	//create a new artificial instance neuron
	int artificialLayer = generateArtificialLayer(neuralNetworkVariables);
	newNeuron = GIAneuralNetworkNonSemanticOperations.createNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, &currentSynapseArtificialInstanceNeuron, wordTag->tagName, GIAneuralNetworkNonSemanticOperations.calculateNumberOfInstancesOfConceptNeuron(conceptNeuron), conceptNeuron, artificialLayer, ANNconnectionType2);

	return newNeuron;
}


bool GIAneuralNetworkNonSemanticClass::performQuery(GIAtranslatorVariablesClass* translatorVariables, GIAtranslatorVariablesClass* translatorVariablesQuery)
{
	bool result = true;
	
	return result;

}


bool GIAneuralNetworkNonSemanticClass::determineReferenceSetDefinite(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList)
{
	bool result = true;
	
	/*
	Assume definite, unless an indefinite determiner (including some, a, numbers) is found in the reference set (preceding any definite determiners).
	
	NB consider these cases;
	The chicken that rides a bike..
	Tom's chicken that rides a bike.. -illegal (therefore propernouns can't precede an indefinite determiner in the same reference set)
	*/
	
	firstSubReferenceSetInList->definite = true;
	
	bool definiteDeterminerFound = false;
	bool indefiniteDeterminerFound = false;
	
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		for(int i=0; i<currentSubReferenceSetInList->subReferenceSetContents.size(); i++)
		{
			GIApreprocessorPlainTextWord* wordTag = (currentSubReferenceSetInList->subReferenceSetContents)[i];
			string word = wordTag->tagName;
			if(SHAREDvars.textInTextArray(word, translatorEnglishDeterminerDefiniteArray, GIA_TRANSLATOR_ENGLISH_DETERMINER_DEFINITE_NUMBER_OF_TYPES))
			{
				definiteDeterminerFound = true;
				wordTag->neuralNetworkPreprocessorWordType = GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_IGNORE;
			}
			
			bool indefiniteDeterminerFoundTemp = false;
			if(SHAREDvars.textInTextArray(word, translatorEnglishDeterminerIndefiniteArray, GIA_TRANSLATOR_ENGLISH_DETERMINER_INDEFINITE_NUMBER_OF_TYPES))
			{
				indefiniteDeterminerFoundTemp = true;
				wordTag->neuralNetworkPreprocessorWordType = GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_IGNORE;
			}
			if(SHAREDvars.textInTextArray(word, translatorEnglishNumbersArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMBER_OF_TYPES))
			{
				indefiniteDeterminerFoundTemp = true;
			}
			char firstCharacterInWord = word[0];
			if(SHAREDvars.charInCharArray(firstCharacterInWord, translatorEnglishNumbersNumericalArray, GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMERICAL_NUMBER_OF_TYPES))
			{
				indefiniteDeterminerFoundTemp = true;
			}			
			
			if(indefiniteDeterminerFoundTemp)
			{
				indefiniteDeterminerFound = true;
				if(!definiteDeterminerFound)
				{
					firstSubReferenceSetInList->definite = false;
				}
			}
		}
		
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;
	}
	
	return result;
}

GIApreprocessorPlainTextWord* GIAneuralNetworkNonSemanticClass::getDelimiterWord(GIApreprocessorSubReferenceSet* referenceSetDelimiter)
{
	GIApreprocessorPlainTextWord* delimiterWordTag = ((referenceSetDelimiter->subReferenceSetContents)[0]);	//CHECKTHIS; or last word in subReferenceSetContents (if 'that' words are recorded in referenceSetDelimiter->subReferenceSetContents); (referenceSetDelimiter->subReferenceSetContents)[referenceSetDelimiter->subReferenceSetContents.size()-1]
	return delimiterWordTag;
}

GIApreprocessorPlainTextWord* GIAneuralNetworkNonSemanticClass::getLogicReferenceWord(GIAposRelTranslatorHybridLogicReference* logicReference)
{
	GIApreprocessorPlainTextWord* logicReferenceWordTag = (logicReference->logicReferenceContents)[0];	//CHECKTHIS
	return logicReferenceWordTag;
}

int GIAneuralNetworkNonSemanticClass::generateArtificialLayer(GIAneuralNetworkVariablesClass* neuralNetworkVariables)
{
	int artificialLayer = GIA_NEURAL_NETWORK_OFFSET_INSTANCE_NEURONS_LAYERS + neuralNetworkVariables->sentenceIndex;	//CHECKTHIS; this needs to be set dynamically
	return artificialLayer;
}

#endif

#endif
