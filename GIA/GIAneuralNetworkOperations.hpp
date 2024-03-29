 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAneuralNetworkOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Description: Neural Network - visual representation of GIA contents in prototype biological neural network
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_NEURAL_NETWORK_OPERATIONS
#define HEADER_GIA_NEURAL_NETWORK_OPERATIONS

#include "GIAglobalDefs.hpp"

#ifdef GIA_NEURAL_NETWORK

#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef SANI_ANN
#include "SANInodes.hpp"
/*
#ifdef GIA_POS_REL_TRANSLATOR_RULES
#include "SANIrules.hpp"
#endif
*/
#include "SANIformation.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
//#include "LRPpreprocessorWordClass.hpp"
#endif
#include "ANNxmlConversion.hpp"
#include "ANNxmlConversion.hpp"
#include "SHAREDvars.hpp"


#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_NEURONS (1)
#define GIA_NEURAL_NETWORK_LAYER_SPECIFIC_CONCEPT_NEURONS (2)
#define GIA_NEURAL_NETWORK_LAYER_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS (3)
#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_SUBSTANCE_NEURONS (4)
#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_REFRENCE_SET_DELIMITER_NEURONS (5)
#endif
#define GIA_NEURAL_NETWORK_SUBNET_COUNTER (0)

#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
static int GIAANNsymbolicCoreConceptIndexBitsTypeCrossReferenceLayerIndex[GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_NUMBER_OF_TYPES] = {GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_SUBSTANCE_NEURONS, GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_REFRENCE_SET_DELIMITER_NEURONS};
#endif

class GIAneuralNetworkVariablesClass
{
public:
	GIAneuralNetworkVariablesClass(void);
	~GIAneuralNetworkVariablesClass(void);
	
	ANNneuron* firstInputNeuronInNetwork;
	
	int64_t neuronIDcounter;
	int64_t conceptNeuronOrderIDcounter;
	int64_t specificConceptNeuronOrderIDcounter;
	int64_t instanceNeuronOrderIDcounter;

	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	int64_t substanceConceptIDcounter;
	int64_t referenceSetDelimiterConceptIDcounter;
	#endif
	
	int yPosRelStart;
		
	int sentenceIndex;
};

class GIAneuralNetworkOperationsClass
{
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef SANI_ANN
	private: SANInodesClass SANInodes;
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_RULES
	private: SANIrulesClass SANIrules;
	#endif
	*/
	private: SANIformationClass SANIformation;
	#endif
	private: ANNneuronClassClass ANNneuronClass;
	private: ANNxmlConversionClass ANNxmlConversion;
	private: SHAREDvarsClass SHAREDvars;

	public: bool readNeuralNetXMLfile(const string xmlFileName, ANNneuron* firstInputNeuronInNetwork);
	public: bool writeNeuralNetXMLfile(string xmlFileName, ANNneuron* firstInputNeuronInNetwork);
			
	public: void initiateGIAneuralNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables);

	#ifdef GIA_NEURAL_NETWORK_PASSIVE
	public: bool generateNeuralNetFromSemanticNet(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
		private: bool calculateLayerOfSpecificConceptNeuron(GIAentityNode* entity, int layer, int* maxLayer);
		private: bool getSpecificConceptNeuronAndLink(const GIAtranslatorVariablesClass* translatorVariables, GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, GIAentityNode* entity, ANNneuron** specificConceptNeuronFound);
		#endif
		private: bool getConceptNeuron(GIAentityNode* entity, ANNneuron** conceptNeuronFound);
		#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
		private: bool generateConceptIndexBitNeurons(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* firstConceptIndexBitNeuronInLayer, const int conceptIndexMaxSizeBits, const int conceptIndexType);
		#endif
		private: bool generateSubnetFromConnectedInstances(const GIAtranslatorVariablesClass* translatorVariables, GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, const bool previousEntityIsConcept, ANNneuron* previousNeuron, GIAentityNode* entity, const int artificialLayer, const bool parsingConcept, const bool parsingSpecificConcept);
	#endif
	private: ANNneuron* getLastNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork);

	public: ANNneuronConnection* createANNconnection(ANNneuron* neuron1, ANNneuron* neuron2, int ANNconnectionType);
	public: ANNneuron* getFirstConceptNeuron(ANNneuron* firstInputNeuronInNetwork);
	public: ANNneuron* getFirstSpecificConceptNeuron(ANNneuron* firstInputNeuronInNetwork);
	public: ANNneuron* getFirstInstanceNeuron(ANNneuron* firstInputNeuronInNetwork);
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	private: ANNneuron* getFirstConceptIndexBitNeuron(const ANNneuron* firstInputNeuronInNetwork, const int conceptIndexType);
	private: ANNneuron* getFirstConceptIndexBitNeuron(const ANNneuron* firstInputNeuronInNetwork, const GIAentityNode* entity);
		private: ANNneuron* getFirstSubstanceConceptIndexBitNeuron(const ANNneuron* firstInputNeuronInNetwork);
		private: ANNneuron* getFirstReferenceSetDelimiterConceptIndexBitNeuron(const ANNneuron* firstInputNeuronInNetwork);
	private: int getConceptIndexType(const GIAentityNode* entity);
	#endif
	
	public: ANNneuron* getLastNeuronInLayer(ANNneuron* firstNeuronInLayer, int* xPosRel);
	public: int calculateNumberOfInstancesOfConceptNeuron(const ANNneuron* conceptNeuron);
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	private: ANNneuron* createNewConceptIndexBitNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptIndexBitNeuron, int conceptIndexBitID, const int conceptIndexType, const int xPosRel);
	#endif
	public: ANNneuron* createNewConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptNeuron, const string entityName, const int xPosRel, int conceptIndexType);
	public: ANNneuron* createNewSpecificConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSpecificConceptNeuron, const string entityName, const int xPosRel, const int layer, int conceptIndexType);
	public: ANNneuron* createNewInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, const string entityName, const int instanceID, ANNneuron* conceptNeuron, const int artificialLayer, int ANNconnectionType, const int sentenceOffset, const int entityOffset);
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	private: int generateInstanceNeuronXposRel(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const ANNneuron* conceptNeuron);
	private: bool connectConceptNeuronToConceptIndexBitNeurons(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron, int conceptIndexType);
		private: int getConceptIndexIDcounter(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const int conceptIndexType);
		private: void incrementConceptIndexIDcounter(GIAneuralNetworkVariablesClass* neuralNetworkVariables, const int conceptIndexType);
	#endif
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	bool createANNconnectionBetweenConceptNeuronAndInstanceNeuron(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const ANNneuron* conceptNeuron, ANNneuron* instanceNeuron);
		double calculateConceptIndexBitConnectionWeight(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const int conceptIndex, const int bitID);
	#else
	bool createANNconnectionBetweenConceptNeuronAndInstanceNeuron(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const ANNneuron* conceptNeuron, ANNneuron* instanceNeuron);
	#endif
	#endif
	
	#ifdef GIA_NEURAL_NETWORK_ACTIVE
	public: ANNneuron* addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorPlainTextWord* wordTag, const bool specificConcept, const vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: ANNneuron* addSpecificConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorPlainTextWord* wordTag, const vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: ANNneuron* addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorPlainTextWord* wordTag);
			public: bool addSpecificConceptPropertiesToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* currentSpecificConceptNeuron, const vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
	public: bool findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorPlainTextWord* wordTag, ANNneuron** conceptNeuronFound, const bool specificConcept, const vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: bool findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorPlainTextWord* wordTag, ANNneuron** conceptNeuronFound);
		public: bool findSpecificConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, LRPpreprocessorPlainTextWord* wordTag, ANNneuron** conceptNeuronFound, const vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
			public: bool verifySpecificConceptProperties(const GIAneuralNetworkVariablesClass* neuralNetworkVariables, const ANNneuron* specificConcept, const vector<LRPpreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
	#endif

	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_SEPARATE_INSTANCES_BY_SENTENCE_INDEX
	int calculateSentenceOffset(const LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, const int sentenceIndex);
	#endif
	
	int64_t getCurrentTime();
};

#endif

#endif

