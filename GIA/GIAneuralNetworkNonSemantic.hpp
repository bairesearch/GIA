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
 * File Name: GIAneuralNetworkNonSemantic.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l8e 15-July-2020
 * Description: Neural Network - visual representation of GIA contents in prototype biological neural network
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_NEURAL_NETWORK_NON_SEMANTIC
#define HEADER_GIA_NEURAL_NETWORK_NON_SEMANTIC

#ifdef GIA_NEURAL_NETWORK

#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAneuralNetworkOperations.hpp"
#include "SHAREDvars.hpp"
#include "GIApreprocessorWordIdentification.hpp"

//move to GIApreprocessorWordClass.hpp:
 
#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_CONCEPT_DEFINITION_DELIMITER "is"	//e.g. a dog is an animal
#define GIA_TRANSLATOR_ENGLISH_PLURAL_CONCEPT_DEFINITION_DELIMITER "are"	//e.g. dogs are animals

#define GIA_TRANSLATOR_ENGLISH_ARTICLES_NUMBER_OF_TYPES (3)
static string translatorEnglishArticlesArray[GIA_TRANSLATOR_ENGLISH_ARTICLES_NUMBER_OF_TYPES] = {"a", "an", "the"};

#define GIA_TRANSLATOR_ENGLISH_POSSESSIVE_DETERMINERS_NUMBER_OF_TYPES (8)
static string translatorEnglishPossessiveDeterminersArray[GIA_TRANSLATOR_ENGLISH_POSSESSIVE_DETERMINERS_NUMBER_OF_TYPES] = {"my", "your", "his", "her", "its", "our", "their", "'s"};	//one's

#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_NUMBER_OF_TYPES (5)
static string translatorEnglishIndefinitePronounsPluralArray[GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_NUMBER_OF_TYPES] = {"both", "few", "fewer", "many", "several"};

#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_OR_SINGULAR_NUMBER_OF_TYPES (6)
static string translatorEnglishIndefinitePronounsPluralOrSingularArray[GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_OR_SINGULAR_NUMBER_OF_TYPES] = {"all", "any", "more", "most", "some", "such"};
//#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_SINGULAR_NUMBER_OF_TYPES (4)
//static string translatorEnglishIndefinitePronounsSingular[GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_SINGULAR_NUMBER_OF_TYPES] = {"another", "each", "either", "every"};	//difference words: another, other

#define GIA_TRANSLATOR_ENGLISH_DEMONSTRATIVE_PRONOUNS_PLURAL_NUMBER_OF_TYPES (2)
static string translatorEnglishDemonstrativePronounsPluralArray[GIA_TRANSLATOR_ENGLISH_DEMONSTRATIVE_PRONOUNS_PLURAL_NUMBER_OF_TYPES] = {"these", "those"};	//such?
//#define GIA_TRANSLATOR_ENGLISH_DEMONSTRATIVE_PRONOUNS_SINGULAR_NUMBER_OF_TYPES (2)
//static string translatorEnglishDemonstrativePronounsSingular[GIA_TRANSLATOR_ENGLISH_DEMONSTRATIVE_PRONOUNS_SINGULAR_NUMBER_OF_TYPES] = {"this", "that"};

#define GIA_TRANSLATOR_ENGLISH_DETERMINER_DEFINITE_NUMBER_OF_TYPES (3)
static string translatorEnglishDeterminerDefiniteArray[GIA_TRANSLATOR_ENGLISH_DETERMINER_DEFINITE_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_DEFINITE, GRAMMATICAL_DETERMINER_DEFINITE_EACH, GRAMMATICAL_DETERMINER_DEFINITE_EVERY};
#define GIA_TRANSLATOR_ENGLISH_DETERMINER_INDEFINITE_NUMBER_OF_TYPES (2)
static string translatorEnglishDeterminerIndefiniteArray[GIA_TRANSLATOR_ENGLISH_DETERMINER_INDEFINITE_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR};





class GIAneuralNetworkNonSemanticClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: ANNneuronClassClass ANNneuronClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAneuralNetworkOperationsClass GIAneuralNetworkOperations;
	private: GIApreprocessorWordClass GIApreprocessorWordIdentification;
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE
	public: bool addTextToNetwork(GIAtranslatorVariablesClass* translatorVariables);
		private: bool addTextToNetworkLogicReference(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, ANNneuron* higherLogicReferenceArtificialSynapseNeuron, bool higherLogicReferenceArtificialSynapseNeuronDirection);
			#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_REPLACE_WORDS_WITH_LEMMAS
			private: bool replaceWordsWithLemmas(GIApreprocessorSubReferenceSet* referenceSet);
			#endif
			private: bool detectIndefiniteConceptDefinition(GIApreprocessorSubReferenceSet* referenceSetSubject, GIApreprocessorSubReferenceSet* referenceSetObject, GIApreprocessorSubReferenceSet* referenceSetDelimiter);
			private: bool identifyAndDemarcateConceptsInReferenceSet(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, int referenceSetType, bool conceptDefinitionDetectedInSentence, bool* foundConcept, ANNneuron** conceptNeuronFound);				
			private: bool detectIfWordIsConcept(const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, int wordIndex, bool* specificConceptDetected, int* indexOfStartOfSpecificConcept, bool indefiniteConceptDefinitionFound);	
				private: bool detectIfWordIsDeterminer(const string word);
				private: bool findIndexOfStartOfSpecificConcept(const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int startIndexToSearch, int* indexOfStartOfSpecificConcept);
				private: bool detectIfWordIsPluralNoun(const GIApreprocessorPlainTextWord* word);
			private: bool findOrAddReferenceSetInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, int referenceSetType);
				private: void calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, int* numberOfActiveConceptNeuronsInSubnet, int64_t* activationAgeOfSubnetSynapsesTotal);
				private: void calculateNumberActiveConceptNeuronsInSubnetReset(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet);
				private: void calculateNumberActiveConceptNeuronsInSubnetUpdateActivationAge(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet);
					private: void calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, bool direction, int* numberOfActiveConceptNeuronsInSubnet, int64_t* activationAgeOfSubnetSynapsesTotal, bool reset, bool updateActivationAge);
				private: bool addReferenceSetInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, int referenceSetType);
			private: bool createDelimiterArtificialSynapseNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** referenceSetDelimiterSubnetEntry, GIApreprocessorSubReferenceSet* referenceSetDelimiter);
			private: bool connectReferenceSetsInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* referenceSetSubjectSubnetEntry, ANNneuron* referenceSetObjectSubnetEntry, ANNneuron** referenceSetDelimiterSubnetEntry, GIApreprocessorSubReferenceSet* referenceSetDelimiter);
				private: ANNneuron* findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag);
	public: bool performQuery(GIAtranslatorVariablesClass* translatorVariables, GIAtranslatorVariablesClass* translatorVariablesQuery);
	public: bool determineReferenceSetDefinite(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList);
	private: GIApreprocessorPlainTextWord* getDelimiterWord(GIApreprocessorSubReferenceSet* referenceSetDelimiter);
	private: GIApreprocessorPlainTextWord* getLogicReferenceWord(GIAposRelTranslatorHybridLogicReference* logicReference);
	private: int generateArtificialLayer(GIAneuralNetworkVariablesClass* neuralNetworkVariables);
	#endif

};

#endif

#endif
