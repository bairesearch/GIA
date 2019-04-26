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
 * File Name: GIAneuralNetworkOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3h1a 20-April-2019
 * Description: Neural Network - visual representation of GIA contents in prototype biological neural network
 * /
 *******************************************************************************/


#include "GIAneuralNetworkOperations.hpp"


		

#ifdef GIA_NEURAL_NETWORK

GIAneuralNetworkVariablesClass::GIAneuralNetworkVariablesClass(void)
{
	firstInputNeuronInNetwork = NULL;
	
	neuronIDcounter = 0;
	conceptNeuronOrderIDcounter = 0;
	specificConceptNeuronOrderIDcounter = 0;
	instanceNeuronOrderIDcounter = 0;
	
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	substanceConceptIDcounter = 0;
	referenceSetDelimiterConceptIDcounter = 0;
	#endif
	
	sentenceIndex = 0;
}
GIAneuralNetworkVariablesClass::~GIAneuralNetworkVariablesClass(void)
{

}


bool GIAneuralNetworkOperationsClass::readNeuralNetXMLfile(const string xmlFileName, ANNneuron* firstInputNeuronInNetwork)
{
	bool result = false;

	int64_t numberOfInputNeuronsLoaded;
	int64_t numberOfOutputNeuronsLoaded;
	int64_t numberOfLayersLoaded;

	ANNneuron* firstOutputNeuronInNetwork = ANNxmlConversion.readNetXMLfileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded, &numberOfLayersLoaded);

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
	artificial layer 3:  instance neurons
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
	artificial layer 2bi: substance concept index bit neurons
	artificial layer 2bii: reference set delimiter (e.g. action/condition) concept index bit neurons
	#endif
	*/
	
	ANNneuron* firstConceptNeuron = neuralNetworkVariables->firstInputNeuronInNetwork;
	ANNneuron* firstSpecificConceptNeuron = new ANNneuron();
	ANNneuron* firstInstanceNeuron = new ANNneuron();
	ANNneuron* firstSubstanceConceptIndexBitNeuron = new ANNneuron();
	ANNneuron* firstReferenceSetDelimiterConceptIndexBitNeuron = new ANNneuron();
	
	firstConceptNeuron->hasFrontLayer = true;
	firstConceptNeuron->hasBackLayer = false;
	firstConceptNeuron->firstNeuronInFrontLayer = firstSpecificConceptNeuron;
	firstConceptNeuron->firstNeuronInBackLayer = NULL;
	firstSpecificConceptNeuron->hasFrontLayer = true;
	firstSpecificConceptNeuron->hasBackLayer = true;
	firstSpecificConceptNeuron->firstNeuronInFrontLayer = firstInstanceNeuron;
	firstSpecificConceptNeuron->firstNeuronInBackLayer = firstConceptNeuron;
	firstInstanceNeuron->hasBackLayer = true;
	firstInstanceNeuron->firstNeuronInBackLayer = firstSpecificConceptNeuron;
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
	firstInstanceNeuron->hasFrontLayer = true;
	firstInstanceNeuron->firstNeuronInFrontLayer = firstSubstanceConceptIndexBitNeuron;
	firstSubstanceConceptIndexBitNeuron->hasFrontLayer = true;
	firstSubstanceConceptIndexBitNeuron->hasBackLayer = true;
	firstSubstanceConceptIndexBitNeuron->firstNeuronInFrontLayer = firstReferenceSetDelimiterConceptIndexBitNeuron;
	firstSubstanceConceptIndexBitNeuron->firstNeuronInBackLayer = firstInstanceNeuron;
	firstReferenceSetDelimiterConceptIndexBitNeuron->hasFrontLayer = false;
	firstReferenceSetDelimiterConceptIndexBitNeuron->hasBackLayer = true;
	firstReferenceSetDelimiterConceptIndexBitNeuron->firstNeuronInFrontLayer = NULL;
	firstReferenceSetDelimiterConceptIndexBitNeuron->firstNeuronInBackLayer = firstSubstanceConceptIndexBitNeuron;
	#else
	firstInstanceNeuron->hasFrontLayer = false;
	firstInstanceNeuron->firstNeuronInFrontLayer = NULL;
	#endif
}

#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
/*
Assume neural net algorithm for language processing is dedicated (ie different to standard backpropogation with given inputs and outputs)
a new neuron is assigned for every new concept, and concepts are binary coded (connected to concept index bit neurons)
a new neuron assigned for each new instance, which is preconnected to the concept index bit neurons, and their synapses are tuned to detect the relevant concept (exhitatory and inhibitory)
reference set delimiter instance neurons are connected to substance instance neurons with bidirectional synapses (electrical synapses).
The subject and object synapses may be weighted differently to identify the direction of the reference set delimiter (e.g. action/condition); e.g. X sees Y instead of Y sees X.

                hair [concept property / feature]
              \  |  /
                dog [concept neurons]
                 |   (e.g. 01111)
               alsation                ride  [reference set delimiter concept neuron]
              // | \\ (e.g. 01011)   // | \\ (e.g. 01011)
             b b b b b               b b b b b  [concept index bit neurons]
             | | | | |               | | | | |
             | | | | |               | | | |
             | | | | |               | | | | |        
             | | | | |               | | | | |                       
             | | | | |               | | | | |
             \ \ |/ /                | | | | |
               dog        \          | | | | |
             | | | | |       \       \ \ |/ / 
             | | | | |                 ride        - - - - - - -> (spatio-temporal index)
             | | | | |               | | | | |
             | | | | |               | | | | |                   
             | | | | |       /       | | | | |           
               bike       /          | | | | |
             | | | | |               | | | | |        
             | | | | |               | | | | |
             v v v v v               v v v v v                          	              
*/
#endif
#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
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
                            /      \                        [GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE]
                           /         . [unique interneuron connection for given instance]        
                          /        /    \                
                        red      rides   bike
		references are identified by activating each of the concepts (ie words) of the sentence	and identifing the subnet that lights up (is activated); this is the subnet which will have the new relation assigned [or is the subnet which is being queried]
*/
#endif

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
	ANNneuron* firstInstanceNeuron = getFirstInstanceNeuron(neuralNetworkVariables.firstInputNeuronInNetwork);
	ANNneuron* currentConceptNeuron = firstConceptNeuron;	
	ANNneuron* currentSpecificConceptNeuron = firstSpecificConceptNeuron;
	ANNneuron* currentInstanceNeuron = firstInstanceNeuron;
	
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	ANNneuron* firstSubstanceConceptIndexBitNeuron = getFirstSubstanceConceptIndexBitNeuron(neuralNetworkVariables.firstInputNeuronInNetwork);
	ANNneuron* firstReferenceSetDelimiterConceptIndexBitNeuron = getFirstReferenceSetDelimiterConceptIndexBitNeuron(neuralNetworkVariables.firstInputNeuronInNetwork);
	generateConceptIndexBitNeurons(&neuralNetworkVariables, firstSubstanceConceptIndexBitNeuron, GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_MAX, GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_SUBSTANCE);
	generateConceptIndexBitNeurons(&neuralNetworkVariables, firstReferenceSetDelimiterConceptIndexBitNeuron, GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_MAX, GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_REFERENCE_SET_DELIMITER);
	#endif
			
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

			#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
			int conceptIndexType = GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_SUBSTANCE;
			GIAentityNode* firstInstance = NULL;
			if(!networkIndexEntity->instanceNodeList->empty())
			{
				firstInstance = (networkIndexEntity->instanceNodeList->back())->entity;
				conceptIndexType = getConceptIndexType(firstInstance);
			}
			#else
			int conceptIndexType = INT_DEFAULT_VALUE;
			#endif
			
			ANNneuron* conceptNeuron = createNewConceptNeuron(&neuralNetworkVariables, &currentConceptNeuron, networkIndexEntity->entityName, xPosRel, conceptIndexType);
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
						int layer = 1;
						calculateLayerOfSpecificConceptNeuron(conceptEntity, 1, &layer);
						
						//required to generate the specific concept now such that the connection algortihm works
						ANNneuron* specificConceptNeuron = createNewSpecificConceptNeuron(&neuralNetworkVariables, &currentSpecificConceptNeuron, specificConceptEntity->entityName, xPosRel, layer, conceptIndexType);
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
			if(getSpecificConceptNeuronAndLink(&neuralNetworkVariables, &currentInstanceNeuron, instanceEntity, &specificConceptNeuron))
			{	
				//cout << "getSpecificConceptNeuronAndLink passed" << endl;
			}
				
			if((instanceEntity->entityType != GIA_ENTITY_TYPE_CONCEPT) && (instanceEntity->entityType != GIA_ENTITY_TYPE_DEFINITION))
			{
			#endif
				//generate non-concept entity connections
				GIAentityNode specificConceptEntityArtificial;
				specificConceptEntityArtificial.entityType = GIA_ENTITY_TYPE_CONCEPT;	//CHECKTHIS
	
				bool previousEntityIsConcept = false;
				ANNneuron* previousNeuron = NULL;
				int artificialLayer = GIA_NEURAL_NETWORK_OFFSET_INSTANCE_NEURONS_LAYERS;
				#ifndef GIA_NEURAL_NETWORK_USE_ARTIFICIAL_INSTANCE_NEURON_SYNAPSES
				if(!(instanceEntity->parsedForANNgeneration))
				{	
					//create a new instance neuron, and create a connection between the concept and the instance neuron
					ANNneuron* instanceNeuron = createNewInstanceNeuron(&neuralNetworkVariables, &currentInstanceNeuron, instanceEntity->entityName, instanceEntity->idInstance, specificConceptNeuron, artificialLayer, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE);
					previousEntityIsConcept = false;	//this ensure that GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE connections are now created
					previousNeuron = instanceNeuron;
				}
				#else
				previousEntityIsConcept = true;
				previousNeuron = specificConceptNeuron;			 
				#endif
			
				#ifdef GIA_DEBUG_NEURAL_NETWORK
				cout << "instanceEntity = " << instanceEntity->entityName << endl;
				#endif
				
				if(!generateSubnetFromConnectedInstances(&neuralNetworkVariables, &currentInstanceNeuron, previousEntityIsConcept, previousNeuron, instanceEntity, artificialLayer, false, false))
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

#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
bool GIAneuralNetworkOperationsClass::generateConceptIndexBitNeurons(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* firstConceptIndexBitNeuronInLayer, int conceptIndexMaxSizeBits, int conceptIndexType)
{
	bool result = true;
	
	ANNneuron* currentConceptIndexBitNeuronInLayer = firstConceptIndexBitNeuronInLayer;
	for(int i=0; i<conceptIndexMaxSizeBits; i++)
	{
		int xPosRel = i*GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_OFFSET + (conceptIndexType-1)*GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_OFFSET;
		createNewConceptIndexBitNeuron(neuralNetworkVariables, &currentConceptIndexBitNeuronInLayer, i, conceptIndexType, xPosRel);
	}
	
	return result;
}

int GIAneuralNetworkOperationsClass::getConceptIndexType(const GIAentityNode* entity)
{
	int conceptIndexType = GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_UNKNOWN;
	if(GIAentityNodeClass.entityIsRelationship(entity))
	{
		conceptIndexType = GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_REFERENCE_SET_DELIMITER;
	}
	else
	{
		conceptIndexType = GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_SUBSTANCE; 
	}	
	return conceptIndexType;
}	
#endif

bool GIAneuralNetworkOperationsClass::generateSubnetFromConnectedInstances(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, bool previousEntityIsConcept, ANNneuron* previousNeuron, GIAentityNode* entity, int artificialLayer, bool parsingConcept, bool parsingSpecificConcept)
{
	bool result = true;
	
	#ifndef GIA_NEURAL_NETWORK_USE_ARTIFICIAL_INSTANCE_NEURON_SYNAPSES
	//entity == previousEntity
	#endif
	
	if(!(entity->parsedForANNgeneration))
	{
		entity->parsedForANNgeneration = true;
		
		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			bool pass = true;
			if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
			{
				pass = false;
			}
			if(pass)
			{
				bool direction = entityVectorConnectionDirectionArray[connectionType];
				int artificialLayerNew = 0;
				if(direction)
				{
					artificialLayerNew = artificialLayer + 1;
				}
				else
				{
					artificialLayerNew = artificialLayer - 1;
				}

			
				for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
				{
					GIAentityNode* entity2 = (*connectionIter)->entity;
					#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
					bool createDirectConceptPropertyFeature = false;
					if(entity2->entityType == GIA_ENTITY_TYPE_PROPERTY)
					{
						GIAentityNode* propertyObjectEntity = GIAtranslatorOperations.getRelationshipObjectEntity(*connectionIter);
						if(propertyObjectEntity->entityType == GIA_ENTITY_TYPE_QUALITY)
						{
							bool createDirectConceptProperty = false;
							if(parsingSpecificConcept && ((*connectionIter)->sameReferenceSet))
							{
								createDirectConceptPropertyFeature = true;
								//detected a specific concept name attribute (eg 'red' in 'red dogs are happy'), and this should be added to the concept hierachy 
							}
							else if(parsingConcept && !((*connectionIter)->sameReferenceSet))
							{
								createDirectConceptPropertyFeature = true;
								//detected a specific concept name attribute (eg 'happy' in 'dogs are happy'), and this should be added to the concept hierachy 
							}
							
							if(createDirectConceptPropertyFeature)
							{
								propertyObjectEntity->parsedForANNgeneration = true;
								ANNneuron* propertyObjectNeuron = propertyObjectEntity->entityShortcutToConceptNeuron;
								createANNconnection(propertyObjectNeuron, previousNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT_PROPERTIES);	//CHECKTHIS
							}
						}
					}
					if(!createDirectConceptPropertyFeature)
					{
					#endif
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
								ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE;
							}
							else
							{
								ANNconnectionType2 = GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE;	//CHECKTHIS
								if(getConceptNeuron(entity2, &conceptNeuron2))
								{

								}
							}

							ANNneuron* newNeuron = createNewInstanceNeuron(neuralNetworkVariables, currentInstanceNeuron, entity2->entityName, entity->idInstance, conceptNeuron2, artificialLayerNew, ANNconnectionType2);

							//add a connection between the previous neuron and the newNeuron
							int ANNconnectionType = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
							if(previousEntityIsConcept)	//OLD: if(previousNeuron->GIAisConceptEntity)
							{
								ANNconnectionType = GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE;	//CHECKTHIS
							}
							else
							{
								ANNconnectionType = GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE;
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
							if(!generateSubnetFromConnectedInstances(neuralNetworkVariables, currentInstanceNeuron, false, newNeuron, entity2, artificialLayerNew, false, false))	//OLD: if(!generateSubnetFromConnectedInstances(firstInputNeuronInNetwork, conceptNeuron2, entity2, directionNew, artificialLayerNew)
							{
								result = false;
							}
							//NB if (entity2->entityType == GIA_ENTITY_TYPE_CONCEPT) then  entity2->parsedForANNgeneration will be true so generateSubnetFromConnectedInstances will not execute any code

							#ifndef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
							previousNeuron = newNeuron;
							#endif
						}
					#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
					}
					#endif					
				}
			}
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
			cout << "GIAneuralNetworkOperationsClass::calculateLayerOfSpecificConceptNeuron{} error: (specificConceptEntity->entityType != GIA_ENTITY_TYPE_CONCEPT)" << endl;
			cout << "specificConceptEntity->entityType = " << specificConceptEntity->entityType << endl;
			exit(EXIT_ERROR);
		}
	}
	return result;
}

bool GIAneuralNetworkOperationsClass::getSpecificConceptNeuronAndLink(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, GIAentityNode* entity, ANNneuron** specificConceptNeuronFound)
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
				cout << "GIAneuralNetworkOperationsClass::getSpecificConceptNeuronAndLink{} error: (conceptEntity->entityShortcutToConceptNeuron == NULL)" << endl;
				exit(EXIT_ERROR);
			}	
			ANNneuron* conceptNeuron = conceptEntity->entityShortcutToConceptNeuron;
			
			//link concept to previous specific concept in net if available;
			if(entity->entityShortcutToConceptNeuron != NULL)
			{
				//cout << "conceptEntity->entityName = " << conceptEntity->entityName << endl;
				//cout << "entity->entityName = " << entity->entityName << endl;
				ANNneuron* previousSpecificConceptNeuron = entity->entityShortcutToConceptNeuron;
				createANNconnection(conceptNeuron, previousSpecificConceptNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT);	//CHECKTHIS
			}
			
			bool specificConceptDetected = false;
			if(networkIndexEntity->shortcutToNonspecificConceptEntity != conceptEntity)	//ie networkIndexEntity->entityShortcutToConceptNeuron != conceptEntity->entityShortcutToConceptNeuron [ie conceptNeuron]
			{
				specificConceptDetected = true;
				
				GIAentityNode* specificConceptEntity = conceptEntity;
				ANNneuron* specificConceptNeuron = conceptNeuron;
				
				if(*specificConceptNeuronFound == NULL)
				{
					*specificConceptNeuronFound = specificConceptNeuron;
					result = true;
				}
			}

			if(!(conceptEntity->parsedForANNgeneration))
			{	
				//create specific concept neural subnet if not already created (e.g. it may already be created if there exists a diamond shaped connection structure)
				conceptEntity->parsedForANNgeneration = true;
				int artificialLayer = GIA_NEURAL_NETWORK_OFFSET_INSTANCE_NEURONS_LAYERS;
				if(!generateSubnetFromConnectedInstances(neuralNetworkVariables, currentInstanceNeuron, true, conceptNeuron, conceptEntity, artificialLayer, true, specificConceptDetected))
				{

				}
			}
			
			#ifdef GIA_NEURAL_NETWORK_GENERATE_SEPARATE_CONCEPT_NETWORKS_RECURSE
			if(specificConceptDetected)
			{	
				ANNneuron* specificConceptNeuronFoundTemp = NULL;	//disgard higher level specific concepts
				if(!getSpecificConceptNeuronAndLink(neuralNetworkVariables, currentInstanceNeuron, conceptEntity, &specificConceptNeuronFoundTemp))
				{
					
				}
			}
			#endif
		}
		else
		{
			/*
			cout << "GIAneuralNetworkOperationsClass::getSpecificConceptNeuronAndLink{} warning: (conceptEntity->entityType != GIA_ENTITY_TYPE_CONCEPT)" << endl;
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
				cout << "GIAneuralNetworkOperationsClass::getSpecificConceptNeuronAndLink{} error: (conceptEntity->entityShortcutToConceptNeuron == NULL)" << endl;
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
ANNneuron* GIAneuralNetworkOperationsClass::getFirstInstanceNeuron(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* firstInstanceNeuron = firstInputNeuronInNetwork->firstNeuronInFrontLayer->firstNeuronInFrontLayer;
	return firstInstanceNeuron;	
}

#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_MAX
ANNneuron* GIAneuralNetworkOperationsClass::getFirstConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork, const int conceptIndexType)
{
	ANNneuron* firstSpecificConceptNeuron = NULL;
	if(conceptIndexType == GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_SUBSTANCE)
	{
		firstSpecificConceptNeuron = getFirstSubstanceConceptIndexBitNeuron(firstInputNeuronInNetwork);
	}
	else if(conceptIndexType == GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_REFERENCE_SET_DELIMITER)
	{
		firstSpecificConceptNeuron = getFirstReferenceSetDelimiterConceptIndexBitNeuron(firstInputNeuronInNetwork);
	}
	else
	{
		cerr << "GIAneuralNetworkOperationsClass::getFirstConceptIndexBitNeuron{} error: illegal conceptIndexType, conceptIndexType = " << conceptIndexType << endl;
		exit(EXIT_ERROR);
	}
	return firstSpecificConceptNeuron;
}
ANNneuron* GIAneuralNetworkOperationsClass::getFirstConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork, const GIAentityNode* entity)
{	
	int conceptIndexType = getConceptIndexType(entity);
	ANNneuron* firstSpecificConceptNeuron = getFirstConceptIndexBitNeuron(firstInputNeuronInNetwork, conceptIndexType);
	return firstSpecificConceptNeuron;
}
ANNneuron* GIAneuralNetworkOperationsClass::getFirstSubstanceConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* firstSubstanceConceptIndexBitNeuron = firstInputNeuronInNetwork->firstNeuronInFrontLayer->firstNeuronInFrontLayer->firstNeuronInFrontLayer;	
	return firstSubstanceConceptIndexBitNeuron;
}
ANNneuron* GIAneuralNetworkOperationsClass::getFirstReferenceSetDelimiterConceptIndexBitNeuron(ANNneuron* firstInputNeuronInNetwork)
{
	ANNneuron* firstReferenceSetDelimiterConceptIndexBitNeuron = firstInputNeuronInNetwork->firstNeuronInFrontLayer->firstNeuronInFrontLayer->firstNeuronInFrontLayer->firstNeuronInFrontLayer;	
	return firstReferenceSetDelimiterConceptIndexBitNeuron;
}
#endif


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

#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
ANNneuron* GIAneuralNetworkOperationsClass::createNewConceptIndexBitNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptIndexBitNeuron, const int conceptIndexBitID, const int conceptIndexType, const int xPosRel)
{
	(*currentConceptIndexBitNeuron)->GIAconceptIndexBitID = conceptIndexBitID;
	(*currentConceptIndexBitNeuron)->GIAisConceptEntity = false;	//CHECKTHIS
	(*currentConceptIndexBitNeuron)->spatialCoordinatesSet3D = true;
	(*currentConceptIndexBitNeuron)->xPosRel = xPosRel;
	(*currentConceptIndexBitNeuron)->yPosRel = GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_LAYER;	//ie networkIndexEntity->idInstance;
	(*currentConceptIndexBitNeuron)->zPosRel = 0;

	int layer = GIAANNsymbolicCoreConceptIndexBitsTypeCrossReferenceLayerIndex[conceptIndexType];
	ANNneuronClass.fillInNeuronIDProperties((*currentConceptIndexBitNeuron), neuralNetworkVariables->neuronIDcounter, conceptIndexBitID, layer, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
	neuralNetworkVariables->neuronIDcounter = neuralNetworkVariables->neuronIDcounter + 1;
	(*currentConceptIndexBitNeuron)->hasBackLayer = true;	//required for ANNdraw	
	if(conceptIndexType == GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_NUMBER_OF_TYPES)
	{
		(*currentConceptIndexBitNeuron)->hasFrontLayer = false;	//required for ANNdraw
	}
	else
	{
		(*currentConceptIndexBitNeuron)->hasFrontLayer = true;	//required for ANNdraw
	}
	
	ANNneuron* newNeuron = (*currentConceptIndexBitNeuron);

	(*currentConceptIndexBitNeuron)->nextNeuron = new ANNneuron();
	(*currentConceptIndexBitNeuron) = (*currentConceptIndexBitNeuron)->nextNeuron;
			
	return newNeuron;
}
#endif

ANNneuron* GIAneuralNetworkOperationsClass::createNewConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentConceptNeuron, const string entityName, const int xPosRel, const int conceptIndexType)
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
	
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	connectConceptNeuronToConceptIndexBitNeurons(neuralNetworkVariables, (*currentConceptNeuron), conceptIndexType);
	#endif
	
	ANNneuron* newNeuron = (*currentConceptNeuron);

	(*currentConceptNeuron)->nextNeuron = new ANNneuron();
	(*currentConceptNeuron) = (*currentConceptNeuron)->nextNeuron;
			
	return newNeuron;
}


ANNneuron* GIAneuralNetworkOperationsClass::createNewSpecificConceptNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentSpecificConceptNeuron, const string entityName, const int xPosRel, const int layer, const int conceptIndexType)
{
	//cout << "createNewSpecificConceptNeuron entityName = " << entityName << endl;

	(*currentSpecificConceptNeuron)->GIAentityName = entityName;	//networkIndexEntity->entityName
	(*currentSpecificConceptNeuron)->GIAisConceptEntity = true;
	(*currentSpecificConceptNeuron)->spatialCoordinatesSet3D = true;
	(*currentSpecificConceptNeuron)->xPosRel = xPosRel;	//currentConceptNeuron->xPosRel
	#ifdef GIA_NEURAL_NETWORK_OFFSET_INSTANCE_NEURONS
	(*currentSpecificConceptNeuron)->yPosRel = layer;
	(*currentSpecificConceptNeuron)->zPosRel = 0;				
	#else
	(*currentSpecificConceptNeuron)->yPosRel = 0;
	(*currentSpecificConceptNeuron)->zPosRel = layer;	
	#endif

	ANNneuronClass.fillInNeuronIDProperties((*currentSpecificConceptNeuron), neuralNetworkVariables->neuronIDcounter, neuralNetworkVariables->specificConceptNeuronOrderIDcounter, GIA_NEURAL_NETWORK_LAYER_SPECIFIC_CONCEPT_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
	neuralNetworkVariables->neuronIDcounter = neuralNetworkVariables->neuronIDcounter + 1;
	neuralNetworkVariables->specificConceptNeuronOrderIDcounter = neuralNetworkVariables->specificConceptNeuronOrderIDcounter + 1;
	(*currentSpecificConceptNeuron)->hasFrontLayer = true;	//required for ANNdraw
	(*currentSpecificConceptNeuron)->hasBackLayer = true;	//required for ANNdraw

	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	connectConceptNeuronToConceptIndexBitNeurons(neuralNetworkVariables, (*currentSpecificConceptNeuron), conceptIndexType);
	#endif
	
	ANNneuron* newNeuron = (*currentSpecificConceptNeuron);
	
	(*currentSpecificConceptNeuron)->nextNeuron = new ANNneuron();
	(*currentSpecificConceptNeuron) = (*currentSpecificConceptNeuron)->nextNeuron; 
	
	return newNeuron;
}

ANNneuron* GIAneuralNetworkOperationsClass::createNewInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron** currentInstanceNeuron, const string entityName, const int instanceID, ANNneuron* conceptNeuron, const int artificialLayer, const int ANNconnectionType)
{
	#ifdef GIA_DEBUG_NEURAL_NETWORK
	cout << "InstanceNeuron: conceptNeuron->GIAentityName = " << conceptNeuron->GIAentityName << endl;
	#endif

	(*currentInstanceNeuron)->GIAentityName = entityName;
	(*currentInstanceNeuron)->GIAisConceptEntity = false;
	(*currentInstanceNeuron)->spatialCoordinatesSet3D = true;
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	(*currentInstanceNeuron)->xPosRel = generateInstanceNeuronXposRel(neuralNetworkVariables, conceptNeuron);
	#else
	(*currentInstanceNeuron)->xPosRel = conceptNeuron->xPosRel;
	#endif
	(*currentInstanceNeuron)->yPosRel = artificialLayer;
	(*currentInstanceNeuron)->zPosRel = instanceID;
	(*currentInstanceNeuron)->GIAactivationAge = getCurrentTime();

	ANNneuronClass.fillInNeuronIDProperties((*currentInstanceNeuron), neuralNetworkVariables->neuronIDcounter, neuralNetworkVariables->instanceNeuronOrderIDcounter, GIA_NEURAL_NETWORK_LAYER_SYNAPSE_ARTIFICIAL_INSTANCE_NEURONS, GIA_NEURAL_NETWORK_SUBNET_COUNTER);
	neuralNetworkVariables->neuronIDcounter = neuralNetworkVariables->neuronIDcounter + 1;
	neuralNetworkVariables->instanceNeuronOrderIDcounter = neuralNetworkVariables->instanceNeuronOrderIDcounter + 1;
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	(*currentInstanceNeuron)->hasFrontLayer = true;	//required for ANNdraw	
	#else
	(*currentInstanceNeuron)->hasFrontLayer = false;	//required for ANNdraw
	#endif
	(*currentInstanceNeuron)->hasBackLayer = true;		//required for ANNdraw

	//add a connection between the concept entity and the InstanceNeuron
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
	createANNconnection(conceptNeuron, *currentInstanceNeuron, ANNconnectionType);
	#endif
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
	//createANNconnection(conceptNeuron, instanceNeuron, ANNconnectionType);		//temp backward compatibility check only
	createANNconnectionBetweenConceptNeuronAndInstanceNeuron(neuralNetworkVariables, conceptNeuron, *currentInstanceNeuron);
	#endif
	
	ANNneuron* newNeuron = (*currentInstanceNeuron);

	ANNneuron* instanceNeuronNew = new ANNneuron();
	(*currentInstanceNeuron)->nextNeuron = instanceNeuronNew;	//redundant
	(*currentInstanceNeuron) = (*currentInstanceNeuron)->nextNeuron;
	
	return newNeuron;
}	

#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS

int GIAneuralNetworkOperationsClass::generateInstanceNeuronXposRel(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron)
{
	int xPosRel = 0;
	int conceptIndexType = conceptNeuron->GIAconceptIndexType;
	
	xPosRel = (conceptIndexType-1)*GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_OFFSET;		//conceptNeuron->xPosRel
	
	return xPosRel;
}

bool GIAneuralNetworkOperationsClass::connectConceptNeuronToConceptIndexBitNeurons(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron, int conceptIndexType)
{
	bool result = true;

	int conceptIndex = getConceptIndexIDcounter(neuralNetworkVariables, conceptIndexType); 
	conceptNeuron->GIAconceptIndexType = conceptIndexType;
	conceptNeuron->GIAconceptIndex = conceptIndex;
	
	int i = 0;
	ANNneuron* currentConceptIndexBitNeuronInLayer = getFirstConceptIndexBitNeuron(neuralNetworkVariables->firstInputNeuronInNetwork, conceptIndexType);
	while(currentConceptIndexBitNeuronInLayer->nextNeuron != NULL)
	{
		int GIAconceptIndexBitID = currentConceptIndexBitNeuronInLayer->GIAconceptIndexBitID;	//i
		ANNneuronConnection* connection = createANNconnection(conceptNeuron, currentConceptIndexBitNeuronInLayer, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT_INDEX_BIT);
		connection->weight = calculateConceptIndexBitConnectionWeight(neuralNetworkVariables, conceptIndex, i);	//GIAconceptIndexBitID
		i++;
		currentConceptIndexBitNeuronInLayer = currentConceptIndexBitNeuronInLayer->nextNeuron;
	}
	
	incrementConceptIndexIDcounter(neuralNetworkVariables, conceptIndexType);
	
	return result;
}

void GIAneuralNetworkOperationsClass::incrementConceptIndexIDcounter(GIAneuralNetworkVariablesClass* neuralNetworkVariables, const int conceptIndexType)
{
	if(conceptIndexType == GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_SUBSTANCE)
	{
		neuralNetworkVariables->substanceConceptIDcounter = neuralNetworkVariables->substanceConceptIDcounter + 1;
	}
	else if(conceptIndexType == GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_REFERENCE_SET_DELIMITER)
	{
		neuralNetworkVariables->referenceSetDelimiterConceptIDcounter = neuralNetworkVariables->referenceSetDelimiterConceptIDcounter + 1;
	}
}	

int GIAneuralNetworkOperationsClass::getConceptIndexIDcounter(GIAneuralNetworkVariablesClass* neuralNetworkVariables, const int conceptIndexType)
{
	int conceptIndexIDcounter = INT_DEFAULT_VALUE;
	if(conceptIndexType == GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_SUBSTANCE)
	{
		conceptIndexIDcounter = neuralNetworkVariables->substanceConceptIDcounter;
	}
	else if(conceptIndexType == GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_REFERENCE_SET_DELIMITER)
	{
		conceptIndexIDcounter = neuralNetworkVariables->referenceSetDelimiterConceptIDcounter;
	}
	return conceptIndexIDcounter;
}	
#endif				

#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE
#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
bool GIAneuralNetworkOperationsClass::createANNconnectionBetweenConceptNeuronAndInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron, ANNneuron* instanceNeuron)
{

	int conceptIndex = conceptNeuron->GIAconceptIndex;  
	int conceptIndexType = conceptNeuron->GIAconceptIndexType;
	
	ANNneuron* currentConceptIndexBitNeuronInLayer = getFirstConceptIndexBitNeuron(neuralNetworkVariables->firstInputNeuronInNetwork, conceptIndexType);
	int i = 0;
	while(currentConceptIndexBitNeuronInLayer->nextNeuron != NULL)
	{
		int GIAconceptIndexBitID = currentConceptIndexBitNeuronInLayer->GIAconceptIndexBitID;	//i
		ANNneuronConnection* connection = createANNconnection(currentConceptIndexBitNeuronInLayer, instanceNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_INDEX_BIT_TO_INSTANCE);
		connection->weight = calculateConceptIndexBitConnectionWeight(neuralNetworkVariables, conceptIndex, i);
		i++;
		currentConceptIndexBitNeuronInLayer = currentConceptIndexBitNeuronInLayer->nextNeuron;
	}	
}
double GIAneuralNetworkOperationsClass::calculateConceptIndexBitConnectionWeight(GIAneuralNetworkVariablesClass* neuralNetworkVariables, int conceptIndex, int bitID)
{
	bool bit = SHAREDvars.getBitValue(conceptIndex, bitID);	//GIAconceptIndexBitID
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_NEGATIVE_WEIGHTS
	int weight = 0;
	if(bit)
	{
		weight = 1;
	}
	else
	{
		weight = -1;
	}
	#else
	int weight = int(bit);
	#endif
	return double(weight);
}
#else
bool GIAneuralNetworkOperationsClass::createANNconnectionBetweenConceptNeuronAndInstanceNeuron(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* conceptNeuron, ANNneuron* instanceNeuron)
{
	ANNneuronConnection* connection = createANNconnection(conceptNeuron, instanceNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE);

}
#endif
#endif

#ifdef GIA_NEURAL_NETWORK_ACTIVE
ANNneuron* GIAneuralNetworkOperationsClass::addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag, const bool specificConcept, const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
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

ANNneuron* GIAneuralNetworkOperationsClass::addSpecificConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag, const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{	
	bool result = true;
	
	string word = wordTag->tagName;
	ANNneuron* newNeuron = NULL;
	
	ANNneuron* conceptNeuron = NULL;
	if(!findConceptInNetwork(neuralNetworkVariables, wordTag, &conceptNeuron))
	{
		conceptNeuron = addConceptToNetwork(neuralNetworkVariables, wordTag);
	}

	int layer = 1;
	//calculateLayerOfSpecificConceptNeuron(conceptEntity, 1, &layer);	//TODO

	int xPosRel = 0;	
	ANNneuron* firstSpecificConceptNeuron = getFirstSpecificConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentSpecificConceptNeuron = getLastNeuronInLayer(firstSpecificConceptNeuron, &xPosRel);
	newNeuron = createNewSpecificConceptNeuron(neuralNetworkVariables, &currentSpecificConceptNeuron, word, xPosRel, layer, GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_UNKNOWN);
	wordTag->wordShortcutToConceptNeuron = newNeuron;

	//connect specific concept to concept 
	createANNconnection(conceptNeuron, newNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT);
	
	if(!addSpecificConceptPropertiesToNetwork(neuralNetworkVariables, currentSpecificConceptNeuron, subReferenceSetContents, indexOfStartOfSpecificConcept, indexOfSpecificConcept))
	{
		result = false;
	}

	return newNeuron;
}

ANNneuron* GIAneuralNetworkOperationsClass::addConceptToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag)
{
	int xPosRel = 0;
	ANNneuron* firstConceptNeuron = getFirstConceptNeuron(neuralNetworkVariables->firstInputNeuronInNetwork);
	ANNneuron* currentConceptNeuron = getLastNeuronInLayer(firstConceptNeuron, &xPosRel);	
	ANNneuron* newNeuron = createNewConceptNeuron(neuralNetworkVariables, &currentConceptNeuron, wordTag->tagName, xPosRel, GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_UNKNOWN);
	wordTag->wordShortcutToConceptNeuron = newNeuron;
	
	return newNeuron;
}

bool GIAneuralNetworkOperationsClass::addSpecificConceptPropertiesToNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* currentSpecificConceptNeuron, const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{
	for(int i=indexOfStartOfSpecificConcept; i<indexOfSpecificConcept; i++)
	{
		GIApreprocessorPlainTextWord* wordTag = (*subReferenceSetContents)[i];
		#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
		wordTag->neuralNetworkPreprocessorWordType = GIA_NEURAL_NETWORK_NON_SEMANTIC_PREPROCESSOR_WORD_TYPE_IGNORE;
		#endif
		
		ANNneuron* conceptNeuron = NULL;
		if(!findConceptInNetwork(neuralNetworkVariables, wordTag, &conceptNeuron))
		{
			cout << "GIAneuralNetworkOperationsClass::addSpecificConceptPropertiesToNetwork error: !findConceptInNetwork" << endl;
			exit(EXIT_ERROR);
		}
	
		createANNconnection(currentSpecificConceptNeuron, conceptNeuron, GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT_PROPERTIES);
	}		
}

		
bool GIAneuralNetworkOperationsClass::findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag, ANNneuron** conceptNeuronFound, const bool specificConcept, const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
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

bool GIAneuralNetworkOperationsClass::findConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag, ANNneuron** conceptNeuronFound)
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


bool GIAneuralNetworkOperationsClass::findSpecificConceptInNetwork(GIAneuralNetworkVariablesClass* neuralNetworkVariables, GIApreprocessorPlainTextWord* wordTag, ANNneuron** conceptNeuronFound, const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
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

bool GIAneuralNetworkOperationsClass::verifySpecificConceptProperties(GIAneuralNetworkVariablesClass* neuralNetworkVariables, ANNneuron* specificConcept, const vector<GIApreprocessorPlainTextWord*>* subReferenceSetContents, const int indexOfStartOfSpecificConcept, const int indexOfSpecificConcept)
{
	bool result = false;
	
	bool everyWordHasANeuronProperty = true;
	bool everyNeuronPropertyHasAWord = true;
	
	for(int i=indexOfStartOfSpecificConcept; i<indexOfSpecificConcept; i++)
	{
		GIApreprocessorPlainTextWord* word = (*subReferenceSetContents)[i];
		for(int i=0; i<specificConcept->frontANNneuronConnectionList.size(); i++)
		{
			ANNneuronConnection* connection = (specificConcept->frontANNneuronConnectionList)[i];
			bool foundMatch = false;
			if(connection->GIAconnectionType == GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT_PROPERTIES)
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
		if(connection->GIAconnectionType == GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT_PROPERTIES)
		{
			ANNneuron* neuron = connection->frontNeuron;
			bool foundMatch = false;
			for(int i=indexOfStartOfSpecificConcept; i<indexOfSpecificConcept; i++)
			{
				GIApreprocessorPlainTextWord* word = (*subReferenceSetContents)[i];
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

#endif

int64_t GIAneuralNetworkOperationsClass::getCurrentTime()
{
	return static_cast<int64_t int>(time(NULL));
}

#endif

