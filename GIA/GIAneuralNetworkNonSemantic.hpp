/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAneuralNetworkNonSemantic.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3d6a 12-November-2017
 * Description: 
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NEURAL_NETWORK_NON_SEMANTIC
#define HEADER_GIA_NEURAL_NETWORK_NON_SEMANTIC

#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAneuralNetworkOperations.hpp"
#include "SHAREDvars.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"

#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_CONCEPT_DEFINITION_DELIMITER "is"	//e.g. a dog is an animal
#define GIA_TRANSLATOR_ENGLISH_PLURAL_CONCEPT_DEFINITION_DELIMITER "are"	//e.g. dogs are animals

#define GIA_TRANSLATOR_ENGLISH_ARTICLES_NUMBER_OF_TYPES (3)
static string translatorEnglishArticlesArray[GIA_TRANSLATOR_ENGLISH_ARTICLES_NUMBER_OF_TYPES] = {"a", "an", "the"};

#define GIA_TRANSLATOR_ENGLISH_POSSESSIVE_DETERMINERS_NUMBER_OF_TYPES (8)
static string translatorEnglishPossessiveDeterminersArray[GIA_TRANSLATOR_ENGLISH_POSSESSIVE_DETERMINERS_NUMBER_OF_TYPES] = {"my", "your", "his", "her", "its", "our", "their", "'s"};	//one's

#define GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMBER_OF_TYPES (28)
static string translatorEnglishNumbersArray[GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMBER_OF_TYPES] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

#define GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMERICAL_NUMBER_OF_TYPES (10)
static char translatorEnglishNumbersNumericalArray[GIA_TRANSLATOR_ENGLISH_NUMBERS_NUMERICAL_NUMBER_OF_TYPES] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_NUMBER_OF_TYPES (5)
static string translatorEnglishIndefinitePronounsPluralArray[GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_NUMBER_OF_TYPES] = {"both", "few", "fewer", "many", "several"};

#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_OR_SINGULAR_NUMBER_OF_TYPES (6)
static string translatorEnglishIndefinitePronounsPluralOrSingularArray[GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_PLURAL_OR_SINGULAR_NUMBER_OF_TYPES] = {"all", "any", "more", "most", "some", "such"};
//#define GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_SINGULAR_NUMBER_OF_TYPES (4)
//static string translatorEnglishIndefinitePronounsSingular[GIA_TRANSLATOR_ENGLISH_INDEFINITE_PRONOUNS_SINGULAR_NUMBER_OF_TYPES] = {"another", "each", "either", "every"};

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
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE
	public: bool addTextToNetwork(GIAtranslatorVariablesClass* translatorVariables);
		private: bool addTextToNetworkLogicReference(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorLogicReference* firstLogicReferenceInList, ANNneuron* higherLogicReferenceArtificialSynapseNeuron, bool higherLogicReferenceArtificialSynapseNeuronDirection);
			#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_REPLACE_WORDS_WITH_LEMMAS
			private: bool replaceWordsWithLemmas(GIApreprocessorSubReferenceSet* referenceSet);
			#endif
			private: bool detectIndefiniteConceptDefinition(GIApreprocessorSubReferenceSet* referenceSetSubject, GIApreprocessorSubReferenceSet* referenceSetObject, GIApreprocessorSubReferenceSet* referenceSetDelimiter);
			private: bool identifyAndDemarcateConceptsInReferenceSet(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, int referenceSetType, bool conceptDefinitionDetectedInSentence, bool* foundConcept, ANNneuron** conceptNeuronFound);				
			private: bool detectIfWordIsConcept(const vector<GIApreprocessorWord*>* subReferenceSetContents, int wordIndex, bool* specificConceptDetected, int* indexOfStartOfSpecificConcept, bool indefiniteConceptDefinitionFound);	
				private: bool detectIfWordIsDeterminer(const string word);
				private: bool findIndexOfStartOfSpecificConcept(const vector<GIApreprocessorWord*>* subReferenceSetContents, const int startIndexToSearch, int* indexOfStartOfSpecificConcept);
				private: bool detectIfWordIsPluralNoun(const GIApreprocessorWord* word);
			private: bool findOrAddReferenceSetInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, int referenceSetType);
				private: void calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, int* numberOfActiveConceptNeuronsInSubnet, long* activationAgeOfSubnetSynapsesTotal);
				private: void calculateNumberActiveConceptNeuronsInSubnetReset(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet);
				private: void calculateNumberActiveConceptNeuronsInSubnetUpdateActivationAge(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet);
					private: void calculateNumberActiveConceptNeuronsInSubnet(ANNneuronConnection* currentNeuronConnectionInInstanceSubnet, bool direction, int* numberOfActiveConceptNeuronsInSubnet, long* activationAgeOfSubnetSynapsesTotal, bool reset, bool updateActivationAge);
				private: bool addReferenceSetInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, ANNneuron** referenceSetSubnetEntry, ANNneuron* referenceSetDelimiterSubnetEntry, int referenceSetType);
			private: bool createDelimiterArtificialSynapseNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** referenceSetDelimiterSubnetEntry, GIApreprocessorSubReferenceSet* referenceSetDelimiter);
			private: bool connectReferenceSetsInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* referenceSetSubjectSubnetEntry, ANNneuron* referenceSetObjectSubnetEntry, ANNneuron** referenceSetDelimiterSubnetEntry, GIApreprocessorSubReferenceSet* referenceSetDelimiter);
				private: ANNneuron* findOrAddConceptAndConnectNewSynapseArtificialInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag);
	public: bool performQuery(GIAtranslatorVariablesClass* translatorVariables, GIAtranslatorVariablesClass* translatorVariablesQuery);
	public: bool determineReferenceSetDefinite(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList);
	private: GIApreprocessorWord* getDelimiterWord(GIApreprocessorSubReferenceSet* referenceSetDelimiter);
	private: GIApreprocessorWord* getLogicReferenceWord(GIApreprocessorLogicReference* logicReference);
	private: int generateArtificialLayer(GIAneuralNetworkVariablesClass* neuralNetworkVariables);
	#endif

};

#endif
