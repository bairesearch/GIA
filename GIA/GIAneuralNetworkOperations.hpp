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
 * File Name: GIAneuralNetworkOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e2b 10-December-2017
 * Description: 
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NEURAL_NETWORK_OPERATIONS
#define HEADER_GIA_NEURAL_NETWORK_OPERATIONS



#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "ANNxmlConversion.hpp"

#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_NEURONS (1)
#define GIA_NEURAL_NETWORK_LAYER_SPECIFIC_CONCEPT_NEURONS (2)
#define GIA_NEURAL_NETWORK_LAYER_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS (3)
#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_SUBSTANCE_NEURONS (4)
#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_REFRENCE_SET_DELIMITER_NEURONS (5)
#endif
#define GIA_NEURAL_NETWORK_SUBNET_COUNTER (0)

static int GIAANNsymbolicCoreConceptIndexBitsTypeCrossReferenceLayerIndex[GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_NUMBER_OF_TYPES] = {GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_SUBSTANCE_NEURONS, GIA_NEURAL_NETWORK_LAYER_CONCEPT_INDEX_BIT_REFRENCE_SET_DELIMITER_NEURONS};


class GIAneuralNetworkVariablesClass
{
public:
	GIAneuralNetworkVariablesClass(void);
	~GIAneuralNetworkVariablesClass(void);
	
	ANNneuron* firstInputNeuronInNetwork;
	
	long neuronIDcounter;
	long conceptNeuronOrderIDcounter;
	long specificConceptNeuronOrderIDcounter;
	long instanceNeuronOrderIDcounter;

	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	long substanceConceptIDcounter;
	long referenceSetDelimiterConceptIDcounter;
	#endif
		
	int sentenceIndex;
};

class GIAneuralNetworkOperationsClass
{
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: ANNneuronClassClass ANNneuronClass;
	private: ANNxmlConversionClass ANNxmlConversion;

	public: bool readNeuralNetXMLfile(const string xmlFileName, ANNneuron* firstInputNeuronInNetwork);
	public: bool writeNeuralNetXMLfile(const string xmlFileName, ANNneuron* ANNfirstInputNeuronInNetwork);
			
	public: void initiateGIAneuralNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables);
	#ifndef GIA_NEURAL_NETWORK_ACTIVE
	public: bool generateNeuralNetFromSemanticNet(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
		private: bool calculateLayerOfSpecificConceptNeuron(GIAentityNode* entity, int layer, int* maxLayer);
		private: bool getSpecificConceptNeuronAndLink(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSynapseArtificialInstanceNeuron, GIAentityNode* entity, ANNneuron** specificConceptNeuronFound);
		#endif
		private: bool getConceptNeuron(GIAentityNode* entity, ANNneuron** conceptNeuronFound);
		#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
		private: bool generateConceptIndexBitNeurons(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* firstConceptIndexBitNeuronInLayer, int conceptIndexMaxSizeBits, int conceptIndexType);
		private: int getConceptIndexType(const GIAentityNode* entity);
		#endif
		private: bool generateSubnetFromConnectedInstances(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, bool previousEntityIsConcept, ANNneuron* previousNeuron, GIAentityNode* entity, int artificialLayer, bool parsingConcept, bool parsingSpecificConcept);
	#endif
	private: ANNneuron* getLastNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork);

	public: ANNneuronConnection* createANNconnection(ANNneuron* neuron1, ANNneuron* neuron2, int ANNconnectionType);
	public: ANNneuron* getFirstConceptNeuron(ANNneuron* firstInputNeuronInNetwork);
	public: ANNneuron* getFirstSpecificConceptNeuron(ANNneuron* firstInputNeuronInNetwork);
	public: ANNneuron* getFirstInstanceNeuron(ANNneuron* firstInputNeuronInNetwork);
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_MAX
	private: ANNneuron* getFirstConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork, const int conceptIndexType);
	private: ANNneuron* getFirstConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork, const GIAentityNode* entity);
		private: ANNneuron* getFirstSubstanceConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork);
		private: ANNneuron* getFirstReferenceSetDelimiterConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork);
	#endif
	
	public: ANNneuron* getLastNeuronInLayer(ANNneuron* firstNeuronInLayer, int* xPosRel);
	public: int calculateNumberOfInstancesOfConceptNeuron(ANNneuron* conceptNeuron);
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	private: ANNneuron* createNewConceptIndexBitNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptIndexBitNeuron, const int GIAconceptIndexBitID, const int conceptIndexType, const int xPosRel);
	#endif
	public: ANNneuron* createNewConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptNeuron, const string entityName, const int xPosRel, const int conceptIndexType);
	public: ANNneuron* createNewSpecificConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSpecificConceptNeuron, const string entityName, const int xPosRel, const int layer, const int conceptIndexType);
	public: ANNneuron* createNewInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, const string entityName, const int instanceID, ANNneuron* conceptNeuron, const int artificialLayer, const int ANNconnectionType);
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	private: int generateInstanceNeuronXposRel(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron);
	private: bool connectConceptNeuronToConceptIndexBitNeurons(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron, int conceptIndexType);
		private: bool getBitValue(int value, int x);
		private: int getConceptIndexIDcounter(GIAneuralNetworkVariablesClass* neuralNetworkVariables, const int conceptIndexType);
		private: void incrementConceptIndexIDcounter(GIAneuralNetworkVariablesClass* neuralNetworkVariables, const int conceptIndexType);
	#endif
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	bool createANNconnectionBetweenConceptNeuronAndInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron, ANNneuron* instanceNeuron);
		double calculateConceptIndexBitConnectionWeight(GIAneuralNetworkVariablesClass* neuralNetworkVariables, int conceptIndex, int bitID);
	#else
	bool createANNconnectionBetweenConceptNeuronAndInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron, ANNneuron* instanceNeuron);
	#endif
	#endif
	
	#ifdef GIA_NEURAL_NETWORK_ACTIVE
	public: ANNneuron* addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, const bool specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: ANNneuron* addSpecificConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: ANNneuron* addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag);
			public: bool addSpecificConceptPropertiesToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* currentSpecificConceptNeuron, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
	public: bool findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound, const bool specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: bool findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound);
		public: bool findSpecificConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
			public: bool verifySpecificConceptProperties(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
	#endif

	long getCurrentTime();
};

#endif

