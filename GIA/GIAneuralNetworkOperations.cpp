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
 * File Name: GIAneuralNetworkOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3d5d 11-August-2017
 * Description: 
 *
 *******************************************************************************/


#include "GIAneuralNetworkOperations.hpp"


GIAneuralNetworkVariablesClass::GIAneuralNetworkVariablesClass(void)
{
	firstInputNeuronInNetwork = NULL;
	neuronIDcounter = 0;
	conceptNeuronOrderIDcounter = 0;
	specificConceptNeuronOrderIDcounter = 0;
	synapseArtificialInstanceOrderIDcounter = 0;
	
	sentenceIndex = 0;
}
GIAneuralNetworkVariablesClass::~GIAneuralNetworkVariablesClass(void)
{

}


		
bool GIAneuralNetworkOperationsClass::readNeuralNetXMLfile(const string xmlFileName, ANNneuron* firstInputNeuronInNetwork)
{
	bool result = false;

	long numberOfInputNeuronsLoaded;
	long numberOfOutputNeuronsLoaded;

	ANNneuron* firstOutputNeuronInNetwork = ANNxmlConversion.readNetXMLfileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded);

	return result;
}

bool GIAneuralNetworkOperationsClass::writeNeuralNetXMLfile(const string xmlFileName, ANNneuron* firstInputNeuronInNetwork)
{
	bool result = true;

	if(!ANNxmlConversion.writeNetXMLfile(xmlFileName, firstInputNeuronInNetwork))
	{
		result = false;
	}
		
	return result;
}


void GIAneuralNetworkOperationsClass::initiateGIAneuralNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables)
{
	/*
	Neural Net Format (for display only; not for back propogation)
	artificial layer 1:  networkIndex/non-specific concept neurons
	artificial layer 2:  specific concept neurons
	artificial layer 3:  (non-concept) instance neurons
	*/
	
	ANNneuron* firstConceptNeuron = neuralNetworkVariables->firstInputNeuronInNetwork;
	ANNneuron* firstSpecificConceptNeuron = new ANNneuron();
	ANNneuron* firstSynapseArtificialInstanceNeuron = new ANNneuron();
	
	firstConceptNeuron->hasFrontLayer = true;
	firstConceptNeuron->hasBackLayer = false;
	firstConceptNeuron->firstNeuronInFrontLayer = firstSpecificConceptNeuron;
	firstConceptNeuron->firstNeuronInBackLayer = NULL;
	firstSpecificConceptNeuron->hasFrontLayer = true;
	firstSpecificConceptNeuron->hasBackLayer = true;
	firstSpecificConceptNeuron->firstNeuronInFrontLayer = firstSynapseArtificialInstanceNeuron;
	firstSpecificConceptNeuron->firstNeuronInBackLayer = firstConceptNeuron;
	firstSynapseArtificialInstanceNeuron->hasFrontLayer = false;
	firstSynapseArtificialInstanceNeuron->hasBackLayer = true;
	firstSynapseArtificialInstanceNeuron->firstNeuronInFrontLayer = NULL;
	firstSynapseArtificialInstanceNeuron->firstNeuronInBackLayer = firstSpecificConceptNeuron;
}

/*
Assume neural net algorithm for language processing is dedicated (ie different to standard backpropogation with given inputs and outputs)
method A [less realistic biological implementation]: a new neuron assigned for each new instance, and a new connection assigned for each new non-reference set [ie direct GIA semantic network mapping into neural network]
method B* [more realistic biological implementation]: a new neuron is assigned for every new concept, instances are stored as a connection subnet between concepts (ie a new artificial neuron is created for every connection, not for every instance)
	connections out of a neuron refer to a new instance
		e.g. The red dog that rides the bike
			  dog
			     \	
			       . [unique interneuron connection for given instance]
			     /   \
			    /      \	
			   /	     . [unique interneuron connection for given instance]
			  /        /    \
			red      rides   bike
		references are identified by activating each of the concepts (ie words) of the sentence	and identifing the subnet that lights up (is activated); this is the subnet which will have the new relation assigned [or is the subnet which is being queried]
*/

#ifndef GIA_NEURAL_NETWORK_ACTIVE
//NB neural connections are defined in same direction as GIA connections. Define front [direction=true] neural connections from concept to instance (synapse artificial neuron) and vice versa. Define front [direction=true] neural connections from concept to specific concept, to more specific concept etc.
bool GIAneuralNetworkOperationsClass::generateNeuralNetFromSemanticNet(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	//generate neurons
	int xPosRel = 0;
	
	//NB if the neural net is already populated then must search for existing references in neural net when adding instances/semantic relations (ie GIA_NEURAL_NETWORK_ACTIVE): cannot use generateNeuralNetFromSemanticNet. 
		//NOT POSSIBLE: ANNneuron* currentConceptNeuron = getLastNeuronInNeuralNet(neuralNetworkVariables->firstInputNeuronInNetwork);	//in case the neural net is already populated

	GIAneuralNetworkVariablesClass neuralNetworkVariables;
	neuralNetworkVariables.firstInputNeuronInNetwork = translatorVariables->firstInputNeuronInNetwork;
	initiateGIAneuralNetwork(&neuralNetworkVariables);
		
	ANNneuron* firstConceptNeuron = getFirstConceptNeuron(neuralNetworkVariables.firstInputNeuronInNetwork);
	ANNneuron* firstSpecificConceptNeuron = getFirstSpecificConceptNeuron(neuralNetworkVariables.firstInputNeuronInNetwork);
	ANNneuron* firstSynapseArtificialInstanceNeuron = getFirstSynapseArtificialInstanceNeuron(neuralNetworkVariables.firstInputNeuronInNetwork);
	ANNneuron* currentConceptNeuron = firstConceptNeuron;	
	ANNneuron* currentSpecificConceptNeuron = firstSpecificConceptNeuron;
	ANNneuron* currentSynapseArtificialInstanceNeuron = firstSynapseArtificialInstanceNeuron;
			
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityIter = translatorVariables->entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityIter != translatorVariables->entityNodesActiveListNetworkIndexes->end(); networkIndexEntityIter++)	
	{
		GIAentityNode* networkIndexEntity = networkIndexEntityIter->second;
		networkIndexEntity->parsedForANNgeneration = true;
		
		#ifdef GIA_NEURAL_NETWORK_ACTIVE
		//this is only required if the neural network is already formed;
		if(networkIndexEntity->entityShortcutToConceptNeuron == NULL)
		//ANNneuron* foundNeuron = NULL;
		//if(!findConceptNeuronInNeuralNet(firstInputNeuronInNetwork, networkIndexEntity->entityName, &foundNeuron))	
		{
		#endif
			ANNneuron* conceptNeuron = createNewConceptNeuron(&neuralNetworkVariables, &currentConceptNeuron, networkIndexEntity->entityName, xPosRel);
			networkIndexEntity->entityShortcutToConceptNeuron = conceptNeuron;
							
			#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
			//must generate the specific concept neurons now such that the connection algortihm works
			for(vector<GIAentityConnection*>::iterator instanceConnectionIter = networkIndexEntity->instanceNodeList->begin(); instanceConnectionIter != networkIndexEntity->instanceNodeList->end(); instanceConnectionIter++)
			{
				GIAentityNode* instanceEntity = (*instanceConnectionIter)->entity;
				if(instanceEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)
				{
					GIAentityNode* conceptEntity = instanceEntity;
					if(networkIndexEntity->shortcutToNonspecificConceptEntity != conceptEntity)
					{
						GIAentityNode* specificConceptEntity = conceptEntity;
						int maxLayer = 1;
						calculateLayerOfSpecificConceptNeuron(conceptEntity, 1, &maxLayer);
						
						//required to generate the specific concept now such that the connection algortihm works
						ANNneuron* specificConceptNeuron = createNewSpecificConceptNeuron(&neuralNetworkVariables, &currentSpecificConceptNeuron, specificConceptEntity->entityName, xPosRel, maxLayer);
						specificConceptEntity->entityShortcutToConceptNeuron = specificConceptNeuron;
					}
					else
					{
						conceptEntity->entityShortcutToConceptNeuron = conceptNeuron;	//NB non-specific concept neuron = networkIndex concept neuron
					}
				}
			}
			#endif
			
			xPosRel++;
		#ifdef GIA_NEURAL_NETWORK_ACTIVE
		}
		#endif
	}
	
	//generate connections
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityIter = translatorVariables->entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityIter != translatorVariables->entityNodesActiveListNetworkIndexes->end(); networkIndexEntityIter++)	
	{
		GIAentityNode* networkIndexEntity = networkIndexEntityIter->second;
		ANNneuron* conceptNeuron = networkIndexEntity->entityShortcutToConceptNeuron;
	
		for(vector<GIAentityConnection*>::iterator instanceConnectionIter = networkIndexEntity->instanceNodeList->begin(); instanceConnectionIter != networkIndexEntity->instanceNodeList->end(); instanceConnectionIter++)
		{
			GIAentityNode* instanceEntity = (*instanceConnectionIter)->entity;
			#ifdef GIA_DEBUG_NEURAL_NETWORK
			cout << "instanceEntity = " << instanceEntity->entityName << endl;
			#endif
				
			ANNneuron* specificConceptNeuron = conceptNeuron;
			#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
			//generate concept entity connections
			if(getSpecificConceptNeuronAndLink(&neuralNetworkVariables, &currentSynapseArtificialInstanceNeuron, instanceEntity, &specificConceptNeuron))
			{	
				//cout << "getSpecificConceptNeuronAndLink passed" << endl;
			}
				
			if((instanceEntity->entityType != GIA_ENTITY_TYPE_CONCEPT) && (instanceEntity->entityType != GIA_ENTITY_TYPE_DEFINITION))
			{
			#endif
				//generate non-concept entity connections
				GIAentityNode specificConceptEntityArtificial;
				specificConceptEntityArtificial.entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//CHECKTHIS
	
				if(!generateSubnetFromConnectedInstances(&neuralNetworkVariables, &currentSynapseArtificialInstanceNeuron, &specificConceptEntityArtificial, specificConceptNeuron, instanceEntity, true, GIA_NEURAL_NETWORK_OFFSET_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS_LAYERS))
				{
					result = false;
				}
			#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
			}
			else
			{
			
			}
			#endif
		}
	}
	
	return result;
}

#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS

bool GIAneuralNetworkOperationsClass::calculateLayerOfSpecificConceptNeuron(GIAentityNode* entity, int layer, int* maxLayer)
{
	bool result = true;
	for(vector<GIAentityConnection*>::iterator definitionConnectionIter = entity->definitionNodeList->begin(); definitionConnectionIter != entity->definitionNodeList->end(); definitionConnectionIter++)
	{
		GIAentityNode* specificConceptEntity = GIAtranslatorOperations.getDefinitionRelationshipObjectEntity(*definitionConnectionIter);
		
		if(specificConceptEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)
		{
			if(layer > *maxLayer)
			{
				*maxLayer = layer;
			}
			if(!calculateLayerOfSpecificConceptNeuron(specificConceptEntity, layer+1, maxLayer))
			{
				result = false;
			}
		}
		else
		{
			cout << "GIAneuralNetworkClass::calculateLayerOfSpecificConceptNeuron{} error: (specificConceptEntity->entityType != GIA_ENTITY_TYPE_CONCEPT)" << endl;
			cout << "specificConceptEntity->entityType = " << specificConceptEntity->entityType << endl;
			exit(EXIT_ERROR);
		}
	}
	return result;
}

bool GIAneuralNetworkOperationsClass::getSpecificConceptNeuronAndLink(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSynapseArtificialInstanceNeuron, GIAentityNode* entity, ANNneuron** specificConceptNeuronFound)
{
	bool result = false;
	
	GIAentityNode* networkIndexEntity = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(entity);
		
	for(vector<GIAentityConnection*>::iterator definitionConnectionIter = entity->definitionNodeList->begin(); definitionConnectionIter != entity->definitionNodeList->end(); definitionConnectionIter++)
	{
		GIAentityNode* conceptEntity = GIAtranslatorOperations.getDefinitionRelationshipObjectEntity(*definitionConnectionIter);

		if(conceptEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)
		{
			if(conceptEntity->entityShortcutToConceptNeuron == NULL)
			{
				cout << "GIAneuralNetworkClass::getSpecificConceptNeuronAndLink{} error: (conceptEntity->entityShortcutToConceptNeuron == NULL)" << endl;
				exit(EXIT_ERROR);
			}	
			ANNneuron* conceptNeuron = conceptEntity->entityShortcutToConceptNeuron;
			
			//link concept to previous specific concept in net if available;
			if(entity->entityShortcutToConceptNeuron != NULL)
			{
				//cout << "conceptEntity->entityName = " << conceptEntity->entityName << endl;
				//cout << "entity->entityName = " << entity->entityName << endl;
				ANNneuron* previousSpecificConceptNeuron = entity->entityShortcutToConceptNeuron;
				createANNconnection(conceptNeuron, previousSpecificConceptNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_DEFINITION);
			}
			
			if(networkIndexEntity->shortcutToNonspecificConceptEntity != conceptEntity)	//ie networkIndexEntity->entityShortcutToConceptNeuron != conceptEntity->entityShortcutToConceptNeuron [ie conceptNeuron]
			{
				//for specific concepts only;
				
				GIAentityNode* specificConceptEntity = conceptEntity;
				ANNneuron* specificConceptNeuron = conceptNeuron;
				
				if(*specificConceptNeuronFound == NULL)
				{
					*specificConceptNeuronFound = specificConceptNeuron;
					result = true;
				}
			
				if(!(specificConceptEntity->parsedForANNgeneration))
				{	
					//create specific concept neural subnet if not already created (e.g. it may already be created if there exists a diamond shaped connection structure)
					specificConceptEntity->parsedForANNgeneration = true;
					if(!generateSubnetFromConnectedInstances(neuralNetworkVariables, currentSynapseArtificialInstanceNeuron, specificConceptEntity, specificConceptNeuron, specificConceptEntity, true, GIA_NEURAL_NETWORK_OFFSET_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS_LAYERS))
					{
						
					}
				}
				
				#ifdef GIA_NEURAL_NETWORK_GENERATE_SEPARATE_CONCEPT_NETWORKS_RECURSE
				ANNneuron* specificConceptNeuronFoundTemp = NULL;	//disgard higher level specific concepts
				if(!getSpecificConceptNeuronAndLink(neuralNetworkVariables, currentSynapseArtificialInstanceNeuron, specificConceptEntity, &specificConceptNeuronFoundTemp))
				{
					
				}
				#endif
			}
			else
			{
				if(!(conceptEntity->parsedForANNgeneration))
				{
					conceptEntity->parsedForANNgeneration = true;
				}
			}
		}
		else
		{
			/*
			cout << "GIAneuralNetworkClass::getSpecificConceptNeuronAndLink{} warning: (conceptEntity->entityType != GIA_ENTITY_TYPE_CONCEPT)" << endl;
			cout << "conceptEntity->entityType = " << conceptEntity->entityType << endl;
			*/
		}
	}
	
	return result;
}
#endif

bool GIAneuralNetworkOperationsClass::getConceptNeuron(GIAentityNode* entity, ANNneuron** conceptNeuronFound)
{
	bool result = true;
	
	GIAentityNode* networkIndexEntity = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(entity);
							
	#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
	for(vector<GIAentityConnection*>::iterator definitionConnectionIter = entity->definitionNodeList->begin(); definitionConnectionIter != entity->definitionNodeList->end(); definitionConnectionIter++)
	{
		GIAentityNode* conceptEntity = GIAtranslatorOperations.getDefinitionRelationshipObjectEntity(*definitionConnectionIter);
		
		if(conceptEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)
		{
			if(conceptEntity->entityShortcutToConceptNeuron == NULL)
			{
				cout << "GIAneuralNetworkClass::getSpecificConceptNeuronAndLink{} error: (conceptEntity->entityShortcutToConceptNeuron == NULL)" << endl;
				exit(EXIT_ERROR);
			}	
			ANNneuron* conceptNeuron = conceptEntity->entityShortcutToConceptNeuron;
			
			if(networkIndexEntity->shortcutToNonspecificConceptEntity != conceptEntity)	//ie networkIndexEntity->entityShortcutToConceptNeuron != conceptEntity->entityShortcutToConceptNeuron [ie conceptNeuron]
			{
				//for specific concepts only;
				
				GIAentityNode* specificConceptEntity = conceptEntity;
				ANNneuron* specificConceptNeuron = conceptNeuron;
				
				if(*conceptNeuronFound == NULL)
				{
					*conceptNeuronFound = specificConceptNeuron;
				}
			}
		}
	}
	#endif

	if(*conceptNeuronFound == NULL)
	{		
		*conceptNeuronFound = networkIndexEntity->entityShortcutToConceptNeuron;
	}

	return result;
}
						
				
bool GIAneuralNetworkOperationsClass::generateSubnetFromConnectedInstances(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSynapseArtificialInstanceNeuron, GIAentityNode* previousEntity, ANNneuron* previousNeuron, GIAentityNode* instanceEntity, bool direction, int artificialLayer)
{
	bool result = true;
	
	if(!(instanceEntity->parsedForANNgeneration))
	{
		instanceEntity->parsedForANNgeneration = true;
		
		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			bool pass = true;
			if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
			{
				pass = false;
			}
			if(pass)
			{
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != instanceEntity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
				{
					GIAentityNode* entity2 = (*connectionIter)->entity;
					if(!(entity2->parsedForANNgeneration) || (entity2->entityType == GIA_ENTITY_TYPE_CONCEPT))	//CHECKTHIS: || (entity2->entityType == GIA_ENTITY_TYPE_CONCEPT)
					{	
						#ifdef GIA_DEBUG_NEURAL_NETWORK
						cout << "entity2 = " << entity2->entityName << endl;
						#endif
					
						ANNneuron* conceptNeuron2 = NULL;
						int ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
						if(entity2->entityType == GIA_ENTITY_TYPE_CONCEPT)
						{
							conceptNeuron2 = entity2->entityShortcutToConceptNeuron;	//create a direct connection between concept neurons
							ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_CONCEPT_DEFINITION_ARTIFICIAL_INSTANCE;
						}
						else
						{
							ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_ARTIFICIAL_INSTANCE;
							if(getConceptNeuron(entity2, &conceptNeuron2))
							{

							}
						}
						
						//create a new synapseArtificialInstanceNeuron
						ANNneuron* newNeuron = createNewSynapseArtificialInstanceNeuron(neuralNetworkVariables, currentSynapseArtificialInstanceNeuron, entity2->entityName, instanceEntity->idInstance, conceptNeuron2, artificialLayer, ANNconnectionType2);


						//add a connection between the previous neuron and the newNeuron
						int ANNconnectionType = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
						if(previousEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)	//OLD: if(previousNeuron->GIAisConceptEntity)
						{
							ANNconnectionType = GIA_ANN_CONNECTION_TYPE_CONCEPT_DEFINITION_ARTIFICIAL_INSTANCE;	//CHECKTHIS
						}
						else
						{
							ANNconnectionType = GIA_ANN_CONNECTION_TYPE_ARTIFICIAL_INSTANCE;
						}
						
						if(direction)
						{
							createANNconnection(previousNeuron, newNeuron, ANNconnectionType);
						}
						else
						{
							createANNconnection(newNeuron, previousNeuron, ANNconnectionType);	
						}

						//recurse
						bool directionNew = entityVectorConnectionDirectionArray[connectionType];
						int artificialLayerNew = 0;
						if(directionNew)
						{
							artificialLayerNew = artificialLayer + 1;
						}
						else
						{
							artificialLayerNew = artificialLayer - 1;
						}
						if(!generateSubnetFromConnectedInstances(neuralNetworkVariables, currentSynapseArtificialInstanceNeuron, instanceEntity, newNeuron, entity2, directionNew, artificialLayerNew))	//OLD: if(!generateSubnetFromConnectedInstances(firstInputNeuronInNetwork, conceptNeuron2, entity2, directionNew, artificialLayerNew)
						{
							result = false;
						}
						
						previousNeuron = newNeuron;
					}
				}
			}
		}
	}
	
	return result;
}
#endif

/*
//slow, use networkIndexEntity->entityShortcutToConceptNeuron instead;
ANNneuron* GIAneuralNetworkOperationsClass::findConceptNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork, const string conceptEntityName)
{
	bool result = false;
	ANNneuron* foundNeuron = NULL;
	ANNneuron* currentConceptNeuron = firstInputNeuronInNetwork;
	while(currentConceptNeuron->nextNeuron != NULL)
	{
		if(currentConceptNeuron->GIAisConceptEntity)
		{
			if(currentConceptNeuron->GIAentityName == conceptEntityName)
			{
				foundNeuron = currentConceptNeuron;
				result = true;
			}
		}
		currentConceptNeuron = currentConceptNeuron->nextNeuron;	
	}
	
	return foundNeuron;
}
*/

ANNneuron* GIAneuralNetworkOperationsClass::getLastNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* currentConceptNeuron = firstInputNeuronInNetwork;
	while(currentConceptNeuron->nextNeuron != NULL)
	{
		currentConceptNeuron = currentConceptNeuron->nextNeuron;	
	}
	return currentConceptNeuron;
}

ANNneuronConnection* GIAneuralNetworkOperationsClass::createANNconnection(ANNneuron* neuron1, ANNneuron* neuron2, int ANNconnectionType)
{
	ANNneuronConnection* newANNneuronConnection = new ANNneuronConnection();
	newANNneuronConnection->GIAconnectionType = ANNconnectionType;
	newANNneuronConnection->backNeuron = neuron1;
	newANNneuronConnection->frontNeuron = neuron2;
	(neuron1->frontANNneuronConnectionList).push_back(newANNneuronConnection);
	(neuron2->backANNneuronConnectionList).push_back(newANNneuronConnection);
	
	return newANNneuronConnection;
}

ANNneuron* GIAneuralNetworkOperationsClass::getFirstConceptNeuron(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* firstConceptNeuron = firstInputNeuronInNetwork;
	return firstConceptNeuron;	
}

ANNneuron* GIAneuralNetworkOperationsClass::getFirstSpecificConceptNeuron(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* firstSpecificConceptNeuron = firstInputNeuronInNetwork->firstNeuronInFrontLayer;	
	return firstSpecificConceptNeuron;
}

ANNneuron* GIAneuralNetworkOperationsClass::getFirstSynapseArtificialInstanceNeuron(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* firstSynapseArtificialInstanceNeuron = firstInputNeuronInNetwork->firstNeuronInFrontLayer->firstNeuronInFrontLayer;
	return firstSynapseArtificialInstanceNeuron;	
}

ANNneuron* GIAneuralNetworkOperationsClass::getLastNeuronInLayer(ANNneuron* firstNeuronInLayer, int* xPosRel)
{
	*xPosRel = 0;
	ANNneuron* currentNeuronInLayer = firstNeuronInLayer;
	while(currentNeuronInLayer->nextNeuron != NULL)
	{
		*xPosRel = currentNeuronInLayer->xPosRel;
		*xPosRel = *xPosRel + 1;
		currentNeuronInLayer = currentNeuronInLayer->nextNeuron;
	}
	return currentNeuronInLayer;
}

int GIAneuralNetworkOperationsClass::calculateNumberOfInstancesOfConceptNeuron(ANNneuron* conceptNeuron)
{
	return (conceptNeuron->frontANNneuronConnectionList).size();	//CHECKTHIS
}


ANNneuron* GIAneuralNetworkOperationsClass::createNewConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptNeuron, const string entityName, int xPosRel)
{
	(*currentConceptNeuron)->GIAentityName = entityName;
	(*currentConceptNeuron)->GIAisConceptEntity = true;
	(*currentConceptNeuron)->spatialCoordinatesSet3D = true;
	(*currentConceptNeuron)->xPosRel = xPosRel;
	(*currentConceptNeuron)->yPosRel = 0;	//ie networkIndexEntity->idInstance;
	(*currentConceptNeuron)->zPosRel = 0;

	ANNneuronClass.fillInNeuronIDProperties((*currentConceptNeuron), neuralNetworkVariables->neuronIDcounter, neuralNetworkVariables->conceptNeuronOrderIDcounter, GIA_NEURAL_NETWORK_LAYER_CONCEPT_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
	neuralNetworkVariables->neuronIDcounter = neuralNetworkVariables->neuronIDcounter + 1;
	neuralNetworkVariables->conceptNeuronOrderIDcounter = neuralNetworkVariables->conceptNeuronOrderIDcounter + 1;
	(*currentConceptNeuron)->hasFrontLayer = true;	//required for ANNdraw
	(*currentConceptNeuron)->hasBackLayer = false;	//required for ANNdraw
	
	ANNneuron* newNeuron = (*currentConceptNeuron);

	(*currentConceptNeuron)->nextNeuron = new ANNneuron();
	(*currentConceptNeuron) = (*currentConceptNeuron)->nextNeuron;
			
	return newNeuron;
}


ANNneuron* GIAneuralNetworkOperationsClass::createNewSpecificConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSpecificConceptNeuron, const string entityName, int xPosRel, const int maxLayer)
{
	//cout << "createNewSpecificConceptNeuron entityName = " << entityName << endl;

	(*currentSpecificConceptNeuron)->GIAentityName = entityName;	//networkIndexEntity->entityName
	(*currentSpecificConceptNeuron)->GIAisConceptEntity = true;
	(*currentSpecificConceptNeuron)->spatialCoordinatesSet3D = true;
	(*currentSpecificConceptNeuron)->xPosRel = xPosRel;	//currentConceptNeuron->xPosRel
	#ifdef GIA_NEURAL_NETWORK_OFFSET_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS
	(*currentSpecificConceptNeuron)->yPosRel = maxLayer;
	(*currentSpecificConceptNeuron)->zPosRel = 0;				
	#else
	(*currentSpecificConceptNeuron)->yPosRel = 0;
	(*currentSpecificConceptNeuron)->zPosRel = maxLayer;	
	#endif

	ANNneuronClass.fillInNeuronIDProperties((*currentSpecificConceptNeuron), neuralNetworkVariables->neuronIDcounter, neuralNetworkVariables->specificConceptNeuronOrderIDcounter, GIA_NEURAL_NETWORK_LAYER_SPECIFIC_CONCEPT_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
	neuralNetworkVariables->neuronIDcounter = neuralNetworkVariables->neuronIDcounter + 1;
	neuralNetworkVariables->specificConceptNeuronOrderIDcounter = neuralNetworkVariables->specificConceptNeuronOrderIDcounter + 1;
	(*currentSpecificConceptNeuron)->hasFrontLayer = true;	//required for ANNdraw
	(*currentSpecificConceptNeuron)->hasBackLayer = true;	//required for ANNdraw
	
	ANNneuron* newNeuron = (*currentSpecificConceptNeuron);
	
	(*currentSpecificConceptNeuron)->nextNeuron = new ANNneuron();
	(*currentSpecificConceptNeuron) = (*currentSpecificConceptNeuron)->nextNeuron; 
	
	return newNeuron;
}

ANNneuron* GIAneuralNetworkOperationsClass::createNewSynapseArtificialInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSynapseArtificialInstanceNeuron, const string entityName, const int instanceID, ANNneuron* conceptNeuron2, int artificialLayer, int ANNconnectionType)
{
	#ifdef GIA_DEBUG_NEURAL_NETWORK
	cout << "synapseArtificialInstanceNeuron: conceptNeuron2->GIAentityName = " << conceptNeuron2->GIAentityName << endl;
	#endif

	(*currentSynapseArtificialInstanceNeuron)->GIAentityName = entityName;
	(*currentSynapseArtificialInstanceNeuron)->GIAisConceptEntity = false;
	(*currentSynapseArtificialInstanceNeuron)->spatialCoordinatesSet3D = true;
	(*currentSynapseArtificialInstanceNeuron)->xPosRel = conceptNeuron2->xPosRel;
	(*currentSynapseArtificialInstanceNeuron)->yPosRel = artificialLayer;
	(*currentSynapseArtificialInstanceNeuron)->zPosRel = instanceID;
	(*currentSynapseArtificialInstanceNeuron)->GIAactivationAge = getCurrentTime();

	ANNneuronClass.fillInNeuronIDProperties((*currentSynapseArtificialInstanceNeuron), neuralNetworkVariables->neuronIDcounter, neuralNetworkVariables->synapseArtificialInstanceOrderIDcounter, GIA_NEURAL_NETWORK_LAYER_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
	neuralNetworkVariables->neuronIDcounter = neuralNetworkVariables->neuronIDcounter + 1;
	neuralNetworkVariables->synapseArtificialInstanceOrderIDcounter = neuralNetworkVariables->synapseArtificialInstanceOrderIDcounter + 1;
	(*currentSynapseArtificialInstanceNeuron)->hasFrontLayer = false;	//required for ANNdraw
	(*currentSynapseArtificialInstanceNeuron)->hasBackLayer = true;		//required for ANNdraw

	//add a connection between the concept entity and the synapseArtificialInstanceNeuron
	createANNconnection(conceptNeuron2, *currentSynapseArtificialInstanceNeuron, ANNconnectionType);

	ANNneuron* newNeuron = (*currentSynapseArtificialInstanceNeuron);

	ANNneuron* synapseArtificialInstanceNeuronNew = new ANNneuron();
	(*currentSynapseArtificialInstanceNeuron)->nextNeuron = synapseArtificialInstanceNeuronNew;	//redundant
	(*currentSynapseArtificialInstanceNeuron) = (*currentSynapseArtificialInstanceNeuron)->nextNeuron;
	
	return newNeuron;
}					
	
ANNneuron* GIAneuralNetworkOperationsClass::addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, const bool specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{	
	bool result = true;
	
	string word = wordTag->tagName;
	ANNneuron* newNeuron = NULL;
	
	if(specificConcept)
	{
		newNeuron = addSpecificConceptToNetwork(neuralNetworkVariables, wordTag, subReferenceSetContents, indexOfStartOfSpecificConcept, indexOfSpecificConcept);
	}
	else
	{
		newNeuron = addConceptToNetwork(neuralNetworkVariables, wordTag);
	}
	
	return newNeuron;
}

ANNneuron* GIAneuralNetworkOperationsClass::addSpecificConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{	
	bool result = true;
	
	string word = wordTag->tagName;
	ANNneuron* newNeuron = NULL;
	
	ANNneuron* conceptNeuron = NULL;
	if(!findConceptInNetwork(neuralNetworkVariables, wordTag, &conceptNeuron))
	{
		conceptNeuron = addConceptToNetwork(neuralNetworkVariables, wordTag);
	}

	int maxLayer = 1;
	//calculateLayerOfSpecificConceptNeuron(conceptEntity, 1, &maxLayer);	//TODO

	int xPosRel = 0;	
	ANNneuron* firstSpecificConceptNeuron = getFirstSpecificConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentSpecificConceptNeuron = getLastNeuronInLayer(firstSpecificConceptNeuron, &xPosRel);
	newNeuron = createNewSpecificConceptNeuron(neuralNetworkVariables, &currentSpecificConceptNeuron, word, xPosRel, maxLayer);
	wordTag->wordShortcutToConceptNeuron = newNeuron;

	//connect specific concept to concept 
	createANNconnection(conceptNeuron, newNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_DEFINITION);
	
	if(!addSpecificConceptPropertiesToNetwork(neuralNetworkVariables, currentSpecificConceptNeuron, subReferenceSetContents, indexOfStartOfSpecificConcept, indexOfSpecificConcept))
	{
		result = false;
	}

	return newNeuron;
}

ANNneuron* GIAneuralNetworkOperationsClass::addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag)
{
	int xPosRel = 0;
	ANNneuron* firstConceptNeuron = getFirstConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentConceptNeuron = getLastNeuronInLayer(firstConceptNeuron, &xPosRel);	
	ANNneuron* newNeuron = createNewConceptNeuron(neuralNetworkVariables, &currentConceptNeuron, wordTag->tagName, xPosRel);
	wordTag->wordShortcutToConceptNeuron = newNeuron;
	
	return newNeuron;
}

bool GIAneuralNetworkOperationsClass::addSpecificConceptPropertiesToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* currentSpecificConceptNeuron, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{
	for(int i=indexOfStartOfSpecificConcept; i<indexOfSpecificConcept; i++)
	{
		GIApreprocessorWord* wordTag = (*subReferenceSetContents)[i];
		wordTag->neuralNetworkPreprocessorWordType = GIA_NEURAL_NETWORK_PREPROCESSOR_WORD_TYPE_IGNORE;
		
		ANNneuron* conceptNeuron = NULL;
		if(!findConceptInNetwork(neuralNetworkVariables, wordTag, &conceptNeuron))
		{
			cout << "GIAneuralNetworkOperationsClass::addSpecificConceptPropertiesToNetwork error: !findConceptInNetwork" << endl;
			exit(EXIT_ERROR);
		}
	
		createANNconnection(currentSpecificConceptNeuron, conceptNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_PROPERTIES);
	}		
}
		
bool GIAneuralNetworkOperationsClass::findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound, const bool specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{
	bool foundConcept = false;
	
	if(specificConcept)
	{		
		foundConcept = findSpecificConceptInNetwork(neuralNetworkVariables, wordTag, conceptNeuronFound, subReferenceSetContents, indexOfStartOfSpecificConcept, indexOfSpecificConcept);
	}
	else
	{
		foundConcept = findConceptInNetwork(neuralNetworkVariables, wordTag, conceptNeuronFound);
	}
	
	return foundConcept;
}

bool GIAneuralNetworkOperationsClass::findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound)
{
	bool foundConcept = false;
	
	string word = wordTag->tagName;
	ANNneuron* firstConceptNeuron = getFirstConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentConceptNeuron = firstConceptNeuron;	
	while(currentConceptNeuron->nextNeuron != NULL)
	{
		if(currentConceptNeuron->GIAentityName == word)
		{
			foundConcept = true;
			*conceptNeuronFound = currentConceptNeuron;
			wordTag->wordShortcutToConceptNeuron = currentConceptNeuron;
		}
		currentConceptNeuron = currentConceptNeuron->nextNeuron;
	}
	
	return foundConcept;
}


bool GIAneuralNetworkOperationsClass::findSpecificConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorWord* wordTag, ANNneuron** conceptNeuronFound, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{
	bool foundConcept = false;
	
	string word = wordTag->tagName;		
	
	//alternatively, search through concepts, and then search specific concepts of the concepts;
	//FUTURE: store the ANN concept and specific concept neurons in a map
	ANNneuron* firstSpecificConceptNeuron = getFirstSpecificConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentSpecificConceptNeuron = firstSpecificConceptNeuron;
	while(currentSpecificConceptNeuron->nextNeuron != NULL)
	{
		if(currentSpecificConceptNeuron->GIAentityName == word)
		{
			//locate the nearest/closest specific concept
			if(verifySpecificConceptProperties(neuralNetworkVariables, currentSpecificConceptNeuron, subReferenceSetContents, indexOfStartOfSpecificConcept, indexOfSpecificConcept))
			{
				foundConcept = true;
				*conceptNeuronFound = currentSpecificConceptNeuron;
				wordTag->wordShortcutToConceptNeuron = currentSpecificConceptNeuron;
			}
		}
		currentSpecificConceptNeuron = currentSpecificConceptNeuron->nextNeuron;
	}

	return foundConcept;
}

bool GIAneuralNetworkOperationsClass::verifySpecificConceptProperties(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* specificConcept, const vector<GIApreprocessorWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{
	bool result = false;
	
	bool everyWordHasANeuronProperty = true;
	bool everyNeuronPropertyHasAWord = true;
	
	for(int i=indexOfStartOfSpecificConcept; i<indexOfSpecificConcept; i++)
	{
		GIApreprocessorWord* word = (*subReferenceSetContents)[i];
		for(int i=0; i<specificConcept->frontANNneuronConnectionList.size(); i++)
		{
			ANNneuronConnection* connection = (specificConcept->frontANNneuronConnectionList)[i];
			bool foundMatch = false;
			if(connection->GIAconnectionType == GIA_ANN_CONNECTION_TYPE_CONCEPT_PROPERTIES)
			{
				ANNneuron* neuron = connection->frontNeuron;
				if(neuron->GIAentityName == word->tagName)
				{
					foundMatch = true;
				}
			}
			if(!foundMatch)
			{
				everyWordHasANeuronProperty = false;
			}
		}
	}
	
	for(int i=0; i<specificConcept->frontANNneuronConnectionList.size(); i++)
	{
		ANNneuronConnection* connection = (specificConcept->frontANNneuronConnectionList)[i];
		if(connection->GIAconnectionType == GIA_ANN_CONNECTION_TYPE_CONCEPT_PROPERTIES)
		{
			ANNneuron* neuron = connection->frontNeuron;
			bool foundMatch = false;
			for(int i=indexOfStartOfSpecificConcept; i<indexOfSpecificConcept; i++)
			{
				GIApreprocessorWord* word = (*subReferenceSetContents)[i];
				if(neuron->GIAentityName == word->tagName)
				{
					foundMatch = true;
				}
			}
			if(!foundMatch)
			{
				everyNeuronPropertyHasAWord = false;
			}
		}
	}
	
	if(everyWordHasANeuronProperty && everyNeuronPropertyHasAWord)
	{
		result = true;
	}
		
	return result;
}

long GIAneuralNetworkOperationsClass::getCurrentTime()
{
	return static_cast<long int>(time(NULL));
}
