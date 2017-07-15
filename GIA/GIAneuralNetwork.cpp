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
 * File Name: GIAneuralNetwork.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3d2d 14-July-2017
 * Description: 
 *
 *******************************************************************************/


#include "GIAneuralNetwork.hpp"

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
bool GIAneuralNetworkClass::generateNeuralNetFromSemanticNet(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	//generate neurons
	int xPosRel = 0;
	ANNneuron* firstInputNeuronInNetwork = translatorVariables->firstInputNeuronInNetwork;
	//NB if the neural net is already populated then must search for existing references in neural net when adding instances/semantic relations (ie GIA_NEURAL_NETWORK_ACTIVE): cannot use generateNeuralNetFromSemanticNet. 
		//NOT POSSIBLE: ANNneuron* currentConceptNeuron = getLastNeuronInNeuralNet(firstInputNeuronInNetwork);	//in case the neural net is already populated

	/*
	Neural Net Format (for display only; not for back propogation)
	artificial layer 1:  networkIndex/non-specific concept neurons
	artificial layer 2:  specific concept neurons
	artificial layer 3:  (non-concept) instance neurons
	*/
		
	ANNneuron* firstConceptNeuron = firstInputNeuronInNetwork;
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

	ANNneuron* currentConceptNeuron = firstConceptNeuron;	
	ANNneuron* currentSpecificConceptNeuron = firstSpecificConceptNeuron;
	ANNneuron* currentSynapseArtificialInstanceNeuron = firstSynapseArtificialInstanceNeuron;
	
	long neuronIDcounter = 0;
	long conceptNeuronOrderIDcounter = 0;
	long specificConceptNeuronOrderIDcounter = 0;
	long synapseArtificialInstanceOrderIDcounter = 0;
			
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityIter = translatorVariables->entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityIter != translatorVariables->entityNodesActiveListNetworkIndexes->end(); networkIndexEntityIter++)	
	{
		GIAentityNode* networkIndexEntity = networkIndexEntityIter->second;
		networkIndexEntity->parsedForANNgeneration = true;
		
		#ifdef GIA_NEURAL_NETWORK_ACTIVE
		//this is only required if the neural network is already formed;
		if(networkIndexEntity->shortcutToConceptNeuron == NULL)
		//ANNneuron* foundNeuron = NULL;
		//if(!findConceptNeuronInNeuralNet(firstInputNeuronInNetwork, networkIndexEntity->entityName, &foundNeuron))	
		{
		#endif
			currentConceptNeuron->entityName = networkIndexEntity->entityName;
			currentConceptNeuron->isConceptEntity = true;
			currentConceptNeuron->spatialCoordinatesSet3D = true;
			currentConceptNeuron->xPosRel = xPosRel;
			currentConceptNeuron->yPosRel = 0;	//ie networkIndexEntity->idInstance;
			currentConceptNeuron->zPosRel = 0;
			networkIndexEntity->shortcutToConceptNeuron = currentConceptNeuron;
			
			ANNneuronClass.fillInNeuronIDProperties(currentConceptNeuron, neuronIDcounter, conceptNeuronOrderIDcounter, GIA_NEURAL_NETWORK_LAYER_CONCEPT_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
			neuronIDcounter = neuronIDcounter + 1;
			conceptNeuronOrderIDcounter = conceptNeuronOrderIDcounter + 1;
			currentConceptNeuron->hasFrontLayer = true;	//required for ANNdraw
			currentConceptNeuron->hasBackLayer = false;	//required for ANNdraw
											
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
						
						//cout << "specificConceptEntity->entityName = " << specificConceptEntity->entityName << endl;
						
						currentSpecificConceptNeuron->entityName = specificConceptEntity->entityName;	//networkIndexEntity->entityName
						currentSpecificConceptNeuron->isConceptEntity = true;
						currentSpecificConceptNeuron->spatialCoordinatesSet3D = true;
						currentSpecificConceptNeuron->xPosRel = xPosRel;	//currentConceptNeuron->xPosRel
						#ifdef GIA_NEURAL_NETWORK_OFFSET_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS
						currentSpecificConceptNeuron->yPosRel = maxLayer;
						currentSpecificConceptNeuron->zPosRel = 0;				
						#else
						currentSpecificConceptNeuron->yPosRel = 0;
						currentSpecificConceptNeuron->zPosRel = maxLayer;	
						#endif
						specificConceptEntity->shortcutToConceptNeuron = currentSpecificConceptNeuron;
						
						ANNneuronClass.fillInNeuronIDProperties(currentSpecificConceptNeuron, neuronIDcounter, specificConceptNeuronOrderIDcounter, GIA_NEURAL_NETWORK_LAYER_SPECIFIC_CONCEPT_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
						neuronIDcounter = neuronIDcounter + 1;
						specificConceptNeuronOrderIDcounter = specificConceptNeuronOrderIDcounter + 1;
						currentSpecificConceptNeuron->hasFrontLayer = true;	//required for ANNdraw
						currentSpecificConceptNeuron->hasBackLayer = true;	//required for ANNdraw
								
						ANNneuron* specificConceptNeuronNew = new ANNneuron();	//required to generate the specific concept now such that the connection algortihm works
						currentSpecificConceptNeuron->nextNeuron = specificConceptNeuronNew;
						currentSpecificConceptNeuron = currentSpecificConceptNeuron->nextNeuron; 
					}
					else
					{
						conceptEntity->shortcutToConceptNeuron = currentConceptNeuron;	//NB non-specific concept neuron = networkIndex concept neuron
					}
				}
			}
			#endif
			
			currentConceptNeuron->nextNeuron = new ANNneuron();
			currentConceptNeuron = currentConceptNeuron->nextNeuron;
			
			xPosRel++;
		#ifdef GIA_NEURAL_NETWORK_ACTIVE
		}
		#endif
	}
	
	//generate connections
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityIter = translatorVariables->entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityIter != translatorVariables->entityNodesActiveListNetworkIndexes->end(); networkIndexEntityIter++)	
	{
		GIAentityNode* networkIndexEntity = networkIndexEntityIter->second;
		ANNneuron* conceptNeuron = networkIndexEntity->shortcutToConceptNeuron;
	
		for(vector<GIAentityConnection*>::iterator instanceConnectionIter = networkIndexEntity->instanceNodeList->begin(); instanceConnectionIter != networkIndexEntity->instanceNodeList->end(); instanceConnectionIter++)
		{
			GIAentityNode* instanceEntity = (*instanceConnectionIter)->entity;
			#ifdef GIA_DEBUG_NEURAL_NETWORK
			cout << "instanceEntity = " << instanceEntity->entityName << endl;
			#endif
				
			ANNneuron* specificConceptNeuron = conceptNeuron;
			#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
			//generate concept entity connections
			if(getSpecificConceptNeuronAndLink(&currentSynapseArtificialInstanceNeuron, instanceEntity, &specificConceptNeuron, &neuronIDcounter, &synapseArtificialInstanceOrderIDcounter))
			{	
				//cout << "getSpecificConceptNeuronAndLink passed" << endl;
			}
				
			if((instanceEntity->entityType != GIA_ENTITY_TYPE_CONCEPT) && (instanceEntity->entityType != GIA_ENTITY_TYPE_DEFINITION))
			{
			#endif
				//generate non-concept entity connections
				if(!generateSubnetFromConnectedInstances(&currentSynapseArtificialInstanceNeuron, specificConceptNeuron, instanceEntity, true, GIA_NEURAL_NETWORK_OFFSET_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS_LAYERS, &neuronIDcounter, &synapseArtificialInstanceOrderIDcounter))
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

bool GIAneuralNetworkClass::calculateLayerOfSpecificConceptNeuron(GIAentityNode* entity, int layer, int* maxLayer)
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

bool GIAneuralNetworkClass::getSpecificConceptNeuronAndLink(ANNneuron** currentSynapseArtificialInstanceNeuron, GIAentityNode* entity, ANNneuron** specificConceptNeuronFound, long* neuronIDcounter, long* orderIDcounter)
{
	bool result = false;
	
	GIAentityNode* networkIndexEntity = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(entity);
		
	for(vector<GIAentityConnection*>::iterator definitionConnectionIter = entity->definitionNodeList->begin(); definitionConnectionIter != entity->definitionNodeList->end(); definitionConnectionIter++)
	{
		GIAentityNode* conceptEntity = GIAtranslatorOperations.getDefinitionRelationshipObjectEntity(*definitionConnectionIter);

		if(conceptEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)
		{
			if(conceptEntity->shortcutToConceptNeuron == NULL)
			{
				cout << "GIAneuralNetworkClass::getSpecificConceptNeuronAndLink{} error: (conceptEntity->shortcutToConceptNeuron == NULL)" << endl;
				exit(EXIT_ERROR);
			}	
			ANNneuron* conceptNeuron = conceptEntity->shortcutToConceptNeuron;
			
			//link concept to previous specific concept in net if available;
			if(entity->shortcutToConceptNeuron != NULL)
			{
				//cout << "conceptEntity->entityName = " << conceptEntity->entityName << endl;
				//cout << "entity->entityName = " << entity->entityName << endl;
				ANNneuron* previousSpecificConceptNeuron = entity->shortcutToConceptNeuron;
				createANNconnection(conceptNeuron, previousSpecificConceptNeuron);
			}
			
			if(networkIndexEntity->shortcutToNonspecificConceptEntity != conceptEntity)	//ie networkIndexEntity->shortcutToConceptNeuron != conceptEntity->shortcutToConceptNeuron [ie conceptNeuron]
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
					if(!generateSubnetFromConnectedInstances(currentSynapseArtificialInstanceNeuron, specificConceptNeuron, specificConceptEntity, true, GIA_NEURAL_NETWORK_OFFSET_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS_LAYERS, neuronIDcounter, orderIDcounter))
					{
						
					}
				}
				
				#ifdef GIA_NEURAL_NETWORK_GENERATE_SEPARATE_CONCEPT_NETWORKS_RECURSE
				ANNneuron* specificConceptNeuronFoundTemp = NULL;	//disgard higher level specific concepts
				if(!getSpecificConceptNeuronAndLink(currentSynapseArtificialInstanceNeuron, specificConceptEntity, &specificConceptNeuronFoundTemp, neuronIDcounter, orderIDcounter))
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

bool GIAneuralNetworkClass::getConceptNeuron(GIAentityNode* entity, ANNneuron** conceptNeuronFound)
{
	bool result = true;
	
	GIAentityNode* networkIndexEntity = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(entity);
							
	#ifdef GIA_NEURAL_NETWORK_GENERATE_SPECIFIC_CONCEPT_NETWORKS
	for(vector<GIAentityConnection*>::iterator definitionConnectionIter = entity->definitionNodeList->begin(); definitionConnectionIter != entity->definitionNodeList->end(); definitionConnectionIter++)
	{
		GIAentityNode* conceptEntity = GIAtranslatorOperations.getDefinitionRelationshipObjectEntity(*definitionConnectionIter);
		
		if(conceptEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)
		{
			if(conceptEntity->shortcutToConceptNeuron == NULL)
			{
				cout << "GIAneuralNetworkClass::getSpecificConceptNeuronAndLink{} error: (conceptEntity->shortcutToConceptNeuron == NULL)" << endl;
				exit(EXIT_ERROR);
			}	
			ANNneuron* conceptNeuron = conceptEntity->shortcutToConceptNeuron;
			
			if(networkIndexEntity->shortcutToNonspecificConceptEntity != conceptEntity)	//ie networkIndexEntity->shortcutToConceptNeuron != conceptEntity->shortcutToConceptNeuron [ie conceptNeuron]
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
		*conceptNeuronFound = networkIndexEntity->shortcutToConceptNeuron;
	}

	return result;
}
						
				
bool GIAneuralNetworkClass::generateSubnetFromConnectedInstances(ANNneuron** currentSynapseArtificialInstanceNeuron, ANNneuron* previousNeuron, GIAentityNode* instanceEntity, bool direction, int artificialLayer, long* neuronIDcounter, long* orderIDcounter)
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
					if(!(entity2->parsedForANNgeneration) || (entity2->entityType == GIA_ENTITY_TYPE_CONCEPT))	//NB: can remove this test entirely
					{	
						#ifdef GIA_DEBUG_NEURAL_NETWORK
						cout << "entity2 = " << entity2->entityName << endl;
						#endif
					
						ANNneuron* newNeuron = NULL;
						if(entity2->entityType == GIA_ENTITY_TYPE_CONCEPT)
						{
							newNeuron = entity2->shortcutToConceptNeuron;	//create a direct connection between concept neurons
						}
						else
						{
							//create a new synapseArtificialInstanceNeuron
							
							ANNneuron* conceptNeuron2 = NULL;
							if(getConceptNeuron(entity2, &conceptNeuron2))
							{

							}
							
							#ifdef GIA_DEBUG_NEURAL_NETWORK
							cout << "synapseArtificialInstanceNeuron: conceptNeuron2->entityName = " << conceptNeuron2->entityName << endl;
							#endif
							
							(*currentSynapseArtificialInstanceNeuron)->entityName = entity2->entityName;
							(*currentSynapseArtificialInstanceNeuron)->isConceptEntity = false;
							(*currentSynapseArtificialInstanceNeuron)->spatialCoordinatesSet3D = true;
							(*currentSynapseArtificialInstanceNeuron)->xPosRel = conceptNeuron2->xPosRel;
							(*currentSynapseArtificialInstanceNeuron)->yPosRel = artificialLayer;
							(*currentSynapseArtificialInstanceNeuron)->zPosRel = instanceEntity->idInstance;
					
							ANNneuronClass.fillInNeuronIDProperties((*currentSynapseArtificialInstanceNeuron), *neuronIDcounter, *orderIDcounter, GIA_NEURAL_NETWORK_LAYER_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
							*neuronIDcounter = *neuronIDcounter + 1;
							*orderIDcounter = *orderIDcounter + 1;
							(*currentSynapseArtificialInstanceNeuron)->hasFrontLayer = false;	//required for ANNdraw
							(*currentSynapseArtificialInstanceNeuron)->hasBackLayer = true;		//required for ANNdraw
							
							//add a connection between the concept entity and the synapseArtificialInstanceNeuron
							createANNconnection(conceptNeuron2, *currentSynapseArtificialInstanceNeuron);

							newNeuron = *currentSynapseArtificialInstanceNeuron;
							
							ANNneuron* synapseArtificialInstanceNeuronNew = new ANNneuron();
							(*currentSynapseArtificialInstanceNeuron)->nextNeuron = synapseArtificialInstanceNeuronNew;	//redundant
							(*currentSynapseArtificialInstanceNeuron) = (*currentSynapseArtificialInstanceNeuron)->nextNeuron;
						}

						//add a connection between the previous neuron and the newNeuron
						if(direction)
						{
							createANNconnection(previousNeuron, newNeuron);
						}
						else
						{
							createANNconnection(newNeuron, previousNeuron);	
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
						if(!generateSubnetFromConnectedInstances(currentSynapseArtificialInstanceNeuron, newNeuron, entity2, directionNew, artificialLayerNew, neuronIDcounter, orderIDcounter))	//OLD: if(!generateSubnetFromConnectedInstances(firstInputNeuronInNetwork, conceptNeuron2, entity2, directionNew, artificialLayerNew)
						{
							result = false;
						}
					}
				}
			}
		}
	}
	
	return result;
}
#endif

/*
//slow, use networkIndexEntity->shortcutToConceptNeuron instead;
ANNneuron* GIAneuralNetworkClass::findConceptNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork, const string conceptEntityName)
{
	bool result = false;
	ANNneuron* foundNeuron = NULL;
	ANNneuron* currentConceptNeuron = firstInputNeuronInNetwork;
	while(currentConceptNeuron->nextNeuron != NULL)
	{
		if(currentConceptNeuron->isConceptEntity)
		{
			if(currentConceptNeuron->entityName == conceptEntityName)
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

ANNneuron* GIAneuralNetworkClass::getLastNeuronInNeuralNet(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* currentConceptNeuron = firstInputNeuronInNetwork;
	while(currentConceptNeuron->nextNeuron != NULL)
	{
		currentConceptNeuron = currentConceptNeuron->nextNeuron;	
	}
	return currentConceptNeuron;
}

bool GIAneuralNetworkClass::createANNconnection(ANNneuron* neuron1, ANNneuron* neuron2)
{
	ANNneuronConnection* newANNneuronConnection = new ANNneuronConnection();
	newANNneuronConnection->backNeuron = neuron1;
	newANNneuronConnection->frontNeuron = neuron2;
	(neuron1->frontANNneuronConnectionList).push_back(newANNneuronConnection);
	(neuron2->backANNneuronConnectionList).push_back(newANNneuronConnection);	
}


