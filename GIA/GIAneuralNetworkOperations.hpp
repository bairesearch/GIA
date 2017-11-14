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
 * Project Version: 3d5b 11-August-2017
 * Description: 
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NEURAL_NETWORK_XML_CONVERSION
#define HEADER_GIA_NEURAL_NETWORK_XML_CONVERSION



#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "ANNxmlConversion.hpp"

#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_NEURONS (1)
#define GIA_NEURAL_NETWORK_LAYER_SPECIFIC_CONCEPT_NEURONS (2)
#define GIA_NEURAL_NETWORK_LAYER_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS (3)
#define GIA_NEURAL_NETWORK_SUBNET_COUNTER (0)


class GIAneuralNetworkVariablesClass
{
public:
	GIAneuralNetworkVariablesClass(void);
	~GIAneuralNetworkVariablesClass(void);
	
	ANNneuron* firstInputNeuronInNetwork;
	long neuronIDcounter;
	long conceptNeuronOrderIDcounter;
	long specificConceptNeuronOrderIDcounter;
	long synapseArtificialInstanceOrderIDcounter;
	
	int sentenceIndex;
};

class GIAneuralNetworkOperationsClass
{
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
		private: bool generateSubnetFromConnectedInstances(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSynapseArtificialInstanceNeuron, GIAentityNode* previousEntity, ANNneuron* previousNeuron, GIAentityNode* instanceEntity, bool direction, int artificialLayer);
	#endif
	private: ANNneuron* getLastNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork);

	public: ANNneuronConnection* createANNconnection(ANNneuron* neuron1, ANNneuron* neuron2, int ANNconnectionType);
	public: ANNneuron* getFirstConceptNeuron(ANNneuron* firstInputNeuronInNetwork);
	public: ANNneuron* getFirstSpecificConceptNeuron(ANNneuron* firstInputNeuronInNetwork);
	public: ANNneuron* getFirstSynapseArtificialInstanceNeuron(ANNneuron* firstInputNeuronInNetwork);
	public: ANNneuron* getLastNeuronInLayer(ANNneuron* firstNeuronInLayer, int* xPosRel);
	public: int calculateNumberOfInstancesOfConceptNeuron(ANNneuron* conceptNeuron);
	public: ANNneuron* createNewConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptNeuron, const string entityName, int xPosRel);
	public: ANNneuron* createNewSpecificConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSpecificConceptNeuron, const string entityName, int xPosRel, const int maxLayer);
	public: ANNneuron* createNewSynapseArtificialInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSynapseArtificialInstanceNeuron, const string entityName, const int instanceID, ANNneuron* conceptNeuron2, int artificialLayer, int ANNconnectionType);
	public: ANNneuron* addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, const bool specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: ANNneuron* addSpecificConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: ANNneuron* addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag);
			public: bool addSpecificConceptPropertiesToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* currentSpecificConceptNeuron, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
	public: bool findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound, const bool specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
		public: bool findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound);
		public: bool findSpecificConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
			public: bool verifySpecificConceptProperties(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept);
	long getCurrentTime();
};

#endif
