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
 * File Name: GIAneuralNetwork.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3d1a 10-July-2017
 * Description: 
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NEURAL_NETWORK
#define HEADER_GIA_NEURAL_NETWORK

#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "GIAtranslatorOperations.hpp"

#define GIA_NEURAL_NETWORK_LAYER_CONCEPT_NEURONS (1)
#define GIA_NEURAL_NETWORK_LAYER_SPECIFIC_CONCEPT_NEURONS (2)
#define GIA_NEURAL_NETWORK_LAYER_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS (3)
#define GIA_NEURAL_NETWORK_SUBNET_COUNTER (0)

class GIAneuralNetworkClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: ANNneuronClassClass ANNneuronClass;
	
	#ifndef GIA_NEURAL_NETWORK_ACTIVE
	public: bool generateNeuralNetFromSemanticNet(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
		private: bool calculateLayerOfSpecificConceptNeuron(GIAentityNode* entity, int layer, int* maxLayer);
		private: bool getSpecificConceptNeuronAndLink(ANNneuron** currentSynapseArtificialInstanceNeuron, GIAentityNode* entity, ANNneuron** specificConceptNeuronFound, long* neuronIDcounter, long* orderIDcounter);
		#endif
		private: bool getConceptNeuron(GIAentityNode* entity, ANNneuron** conceptNeuronFound);
		private: bool generateSubnetFromConnectedInstances(ANNneuron** currentSynapseArtificialInstanceNeuron, ANNneuron* conceptNeuron, GIAentityNode* instanceEntity, bool direction, int artificialLayer, long* neuronIDcounter, long* orderIDcounter);
	#endif
	private: ANNneuron* generateNeuralNetFromSemanticNet(ANNneuron* firstInputNeuronInNetwork, const string conceptEntityName);
	private: ANNneuron* getLastNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork);
	private: bool createANNconnection(ANNneuron* neuron1, ANNneuron* neuron2);


};

#endif
