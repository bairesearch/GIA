 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAneuralNetworkNonSemantic.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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
#include "LRPpreprocessorWordIdentification.hpp"

//move to LRPpreprocessorWordClass.hpp:
 
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
	private: LRPpreprocessorWordClass LRPpreprocessorWordIdentification;
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE
	public: bool addTextToNetwork(GIAtranslatorVariablesClass* translatorVariables);
		private: bool addTextToNetworkLogicReference(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, ANNneuron* higherLogicReferenceArtificialSynapseNeuron, const bool higherLogicReferenceArtificialSynapseNeuronDirection);
			#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_REPLACE_WORDS_WITH_LEMMAS
			private: bool replaceWordsWithLemmas(const LRPpreprocessorSubReferenceSet* referenceSet);
			#endif
			private: bool detectIndefiniteConceptDefinition(LRPpreprocessorSubReferenceSet* referenceSetSubject, LRPpreprocessorSubReferenceSet* referenceSetObject, LRPpreprocessorSubReferenceSet* referenceSetDelimiter);
			private: bool identifyAndDemarcateConceptsInReferenceSet(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList, const int referenceSetType, const bool indefiniteConceptDefinitionFound, bool* foundConcept, ANNneuron** conceptNeuronFound);
			private: bool detectIfWordIsConcept(vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, int wordIndex, bool* specificConceptDetected, int* indexOfStartOfSpecificConcept, const bool indefiniteConceptDefinitionFound);	
				private: bool detectIfWordIsDeterminer(const string word);
				private: bool findIndexOfStartOfSpecificConcept(const vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, const int startIndexToSearch, int* indexOfStartOfSpecificConcept);
			private: bool findOrAddReferenceSetInNetwork(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, const int referenceSetType);
				private: void calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, int* numberOfActiveConceptNeuronsInSubnet, int64_t* activationAgeOfSubnetSynapsesTotal);
				private: void calculateNumberActiveConceptNeuronsInSubnetReset(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet);
				private: void calculateNumberActiveConceptNeuronsInSubnetUpdateActivationAge(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet);
					private: void calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, const bool direction, int* numberOfActiveConceptNeuronsInSubnet, int64_t* activationAgeOfSubnetSynapsesTotal, const bool reset, const bool updateActivationAge);
				private: bool addReferenceSetInNetwork(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, const int referenceSetType);
			private: bool createDelimiterArtificialSynapseNeuron(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** referenceSetDelimiterSubnetEntry, LRPpreprocessorSubReferenceSet* referenceSetDelimiter);
			private: bool connectReferenceSetsInNetwork(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* referenceSetSubjectSubnetEntry, ANNneuron* referenceSetObjectSubnetEntry, ANNneuron** referenceSetDelimiterSubnetEntry, LRPpreprocessorSubReferenceSet* referenceSetDelimiter);
				private: ANNneuron* findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const LRPpreprocessorPlainTextWord* wordTag);
	public: bool performQuery(const GIAtranslatorVariablesClass* translatorVariables, const GIAtranslatorVariablesClass* translatorVariablesQuery);
	public: bool determineReferenceSetDefinite(LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList);
	private: LRPpreprocessorPlainTextWord* getDelimiterWord(const LRPpreprocessorSubReferenceSet* referenceSetDelimiter);
	private: LRPpreprocessorPlainTextWord* getLogicReferenceWord(const GIAposRelTranslatorHybridLogicReference* logicReference);
	private: int generateArtificialLayer(const GIAneuralNetworkVariablesClass* neuralNetworkVariables);
	#endif

};

#endif

#endif
