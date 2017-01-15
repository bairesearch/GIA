/*******************************************************************************
 *
 * File Name: GIAquery.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 *
 *******************************************************************************/


#include "GIAquery.h"
#include "GIAdatabase.h"

GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, vector<GIAEntityNode*> *conceptEntityNodesListQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * confidence)
{
	double bestConfidence = 0.0;
	double bestConfidenceAssumingFoundAnswer = 0.0;
	
	#ifdef GIA_QUERY_DEBUG
	if(detectComparisonVariable)
	{
		cout << "comparisonVariableNode->entityName = " << comparisonVariableNode->entityName << endl;
	}
	#endif
	
	GIAEntityNode * networkEntityNodeWhenSearchedResultsInBestConfidence = NULL;
	GIAEntityNode * queryEntityNodeWhenSearchedResultsInBestConfidence = NULL;
		
	vector<GIAEntityNode*>::iterator entityIterQuery;
	for(entityIterQuery = conceptEntityNodesListQuery->begin(); entityIterQuery != conceptEntityNodesListQuery->end(); entityIterQuery++) 
	{//for each node in query semantic net;
		
		GIAEntityNode* currentQueryEntityNode = *entityIterQuery;
		
		#ifdef GIA_QUERY_DEBUG
		cout << "currentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
		#endif
		
		bool foundQueryEntityNodeName = false;
		long queryEntityNodeIndex = -1;
		string queryEntityNodeName = currentQueryEntityNode->entityName;
		GIAEntityNode * conceptEntityMatchingCurrentQueryEntity = findOrAddEntityNodeByName(NULL, conceptEntityNodesList, conceptEntityNamesList, &queryEntityNodeName, &foundQueryEntityNodeName, &queryEntityNodeIndex, false, NULL, NULL);
		
		if(foundQueryEntityNodeName)
		{
			//now start matching structure search for all properties of the identical concept node (to current query entity name) in Semantic Network
			
			int numberOfMatchedNodes = 0;	
			bool foundAnswerTemp = false;
			GIAEntityNode* queryAnswerNodeTemp;

			queryAnswerNodeTemp = testEntityNodeForQuery(currentQueryEntityNode, conceptEntityMatchingCurrentQueryEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodes, false);

			double currentConfidence = (double)numberOfMatchedNodes;	//NB confidence value definition for query network structure matching is currently very simple; it is just equal to the number of matched nodes found 

			if(detectComparisonVariable)
			{
				if(foundAnswerTemp)
				{//if comparisonVariable is identified within the query, then an exact answer is required... 
					if(currentConfidence > bestConfidenceAssumingFoundAnswer)
					{
						bestConfidenceAssumingFoundAnswer = currentConfidence;
						queryAnswerNode = queryAnswerNodeTemp;
						*foundAnswer = true;
						*confidence = bestConfidenceAssumingFoundAnswer;
					}
				}
			}

			if(currentConfidence > bestConfidence)
			{
				bestConfidence = currentConfidence;
				networkEntityNodeWhenSearchedResultsInBestConfidence = conceptEntityMatchingCurrentQueryEntity;
				queryEntityNodeWhenSearchedResultsInBestConfidence = currentQueryEntityNode;
			}				

			
		}
	}
	
	cout << "finished query round 1" << endl;

	if(!detectComparisonVariable || !(*foundAnswer))
	{//now set draw parameters for optimium solution...
	
		bool foundAnswerTemp = false;
		GIAEntityNode* queryAnswerNodeTemp;
		int numberOfMatchedNodes = 0;
		
		testEntityNodeForQuery(queryEntityNodeWhenSearchedResultsInBestConfidence, networkEntityNodeWhenSearchedResultsInBestConfidence, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodes, true);
	
		if(!detectComparisonVariable && foundAnswerTemp)
		{
			cout << "asf" << endl;
			*foundAnswer = true;
			queryAnswerNode = queryAnswerNodeTemp;
		}
		*confidence = (double)numberOfMatchedNodes;
	}

	cout << "finished query round 2" << endl;
	return queryAnswerNode;
				
}

GIAEntityNode * testReferencedEntityNodeForNameMatch(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode * comparisonVariableNode,  bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, bool isSuitableNodeTypeForInexactAnswer, bool isCondition, string * conditionType)
{
	bool foundMatch = false;
	
	if(queryEntityNode->entityName == entityNode->entityName)
	{
		if(detectComparisonVariable && comparisonVariableNode->hasQuantity && queryEntityNode->hasQuantity && entityNode->hasQuantity)
		{//exact match found [NB if a quantity, the queryEntityNode's entityName will not have the comparisonVariable name (_$qVar) specified, and therefore a matched entity node entityName is required]
			foundMatch = true;
			//cout << "entityNode->quantityNumber = " << entityNode->quantityNumber << endl;
		}
		else
		{
			queryAnswerNode = testEntityNodeForQuery(queryEntityNode, entityNode, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters);
			
			if(isSuitableNodeTypeForInexactAnswer)
			{
				if(findBestInexactAnswerAndSetDrawParameters)
				{
					foundMatch = true;
					cout << "foundBestInexactAnswerAndSetDrawParameters:" << comparisonVariableNode->entityName << endl;
					//set queryAnswerNode if entityNode is an object;
					/*eg;
					Which house does did Jane buy?
					Which house is that?
					What day is it?
					What house did Jane buy?
					*/

				}			
			}
		}
	}
	else if(detectComparisonVariable)
	{
		if(queryEntityNode->entityName == comparisonVariableNode->entityName)
		{//exact match found
			foundMatch = true;		
		}
	}
	
	if(foundMatch)
	{
		if((!findBestInexactAnswerAndSetDrawParameters && !(entityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
		{
			entityNode->isAnswerToQuery = true;
			*foundAnswer = true; 
			queryAnswerNode = entityNode;
			cout << "foundAnswer:" << comparisonVariableNode->entityName << endl;
			//CHECKTHIS; need to take into account vector of answers (not just a single answer)
			
			if(findBestInexactAnswerAndSetDrawParameters)
			{
				entityNode->isAnswerContextToQuery = true;			 
			}
			else
			{
				entityNode->testedForQueryComparison = true;		//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]				
			}
			
			if(isCondition)
			{
				entityNode->queryIsCondition = true;
				entityNode->queryConditionType = *conditionType;
			}			
		}	
	}
	
	return queryAnswerNode;
	
	
}
				
//current queryEntityNode, currentEntityNode
GIAEntityNode * testEntityNodeForQuery(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters)
{
	*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		
	/*
	bool searchingForExactMatchAtThisQueryEntityNode = false;
	//test for exact match
	if(detectComparisonVariable)
	{
		if(comparisonVariableNode->entityName == queryEntityNode->entityName)
		{
			cout << "comparisonVariableNode->entityName = " << comparisonVariableNode->entityName << endl;	//should equal something like _$qVar
			searchingForExactMatchAtThisQueryEntityNode = true;
		}
	}
	*/
						
	if((!findBestInexactAnswerAndSetDrawParameters && !(entityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
	{//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]
	
		if(findBestInexactAnswerAndSetDrawParameters)
		{
			entityNode->isAnswerContextToQuery = true;			 
		}
		else
		{
			entityNode->testedForQueryComparison = true;		//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]				
		}
		
		#ifdef GIA_QUERY_DEBUG
		if(entityNode->isProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}
		else if(entityNode->isAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}		
		else if(entityNode->hasAssociatedProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property)" << endl;	
		}
		else if(entityNode->hasAssociatedPropertyIsAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		} 
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << endl;
		}
		#endif
		
		/*
		cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		cout << "\tentityNode->isProperty = " << entityNode->isProperty << endl;
		cout << "\tentityNode->hasAssociatedProperty = " << entityNode->hasAssociatedProperty << endl;
		cout << "\tentityNode->hasAssociatedPropertyIsAction = " << entityNode->hasAssociatedPropertyIsAction << endl;
		*/
		
			
		vector<GIAEntityNode*>::iterator actionIterQuery;
		vector<GIAEntityNode*>::iterator actionIter;
		for(actionIterQuery = queryEntityNode->IncomingActionNodeList.begin(); actionIterQuery != queryEntityNode->IncomingActionNodeList.end(); actionIterQuery++) 
		{
			for(actionIter = entityNode->IncomingActionNodeList.begin(); actionIter != entityNode->IncomingActionNodeList.end(); actionIter++) 
			{
				//cout << "A" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*actionIterQuery, *actionIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);
			
				//cout << "AA" << endl;
			}
		}			
		for(actionIterQuery = queryEntityNode->ActionNodeList.begin(); actionIterQuery != queryEntityNode->ActionNodeList.end(); actionIterQuery++) 
		{
			for(actionIter = entityNode->ActionNodeList.begin(); actionIter != entityNode->ActionNodeList.end(); actionIter++) 
			{
				//cout << "AAA" << endl;	
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*actionIterQuery, *actionIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);				
				//cout << "AAAA" << endl;
			}
		}	



		
		if(queryEntityNode->actionSubjectEntity != NULL)
		{		
			if(entityNode->actionSubjectEntity != NULL)
			{		
				//cout << "b5" << endl;
				//cout << "entityNode->actionSubjectEntity->testedForQueryComparison = " << entityNode->actionSubjectEntity->testedForQueryComparison << endl;
				//cout << "entityNode->actionSubjectEntity->entityName = " << entityNode->actionSubjectEntity->entityName << endl;
				//cout << "entityNode->actionObjectEntity->entityName = " << entityNode->actionObjectEntity->entityName << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->actionSubjectEntity, entityNode->actionSubjectEntity, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);
							
				//cout << "b6" << endl;		
			}	
		}			
		if(queryEntityNode->actionObjectEntity != NULL)
		{		
			if(entityNode->actionObjectEntity != NULL)
			{		
				//cout << "b7" << endl;

				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->actionObjectEntity, entityNode->actionObjectEntity, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, true, false, NULL);
				
				//cout << "b8" << endl;		
			}	
		}
					
		
		//cout << "a2" << endl;
		
		//conditions connections
		vector<GIAEntityNode*>::iterator ConditionIterQuery;
		vector<GIAEntityNode*>::iterator ConditionIter;
		vector<string>::iterator ConditionNodeTypeListIteratorQuery;
		vector<string>::iterator ConditionNodeTypeListIterator;
		
		//CHECK THIS; need to take into account condition types for match
		ConditionNodeTypeListIteratorQuery = queryEntityNode->ConditionNodeTypeList.begin();
		for(ConditionIterQuery = queryEntityNode->ConditionNodeList.begin(); ConditionIterQuery != queryEntityNode->ConditionNodeList.end(); ConditionIterQuery++) 
		{	
			ConditionNodeTypeListIterator = entityNode->ConditionNodeTypeList.begin();		
			for(ConditionIter = entityNode->ConditionNodeList.begin(); ConditionIter != entityNode->ConditionNodeList.end(); ConditionIter++) 
			{	
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIterQuery, *ConditionIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, true, &(*ConditionNodeTypeListIterator));
				
				ConditionNodeTypeListIterator++;			
			}			
			ConditionNodeTypeListIteratorQuery++;			
		}				
		if(queryEntityNode->timeConditionNode != NULL)
		{	
			if(queryEntityNode->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				if(entityNode->timeConditionNode != NULL)
				{	
					if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
					{
						if(queryEntityNode->timeConditionNode->conditionName == entityNode->timeConditionNode->conditionName)
						{
							*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
							//CHECK THIS; need to parse timeConditionNodes and test exact matching?
						}
						/*CHECKTHIS; cannot currently match timeConditionNodes*/						
					}		
				}			
			}		
		}
		
		//CHECK THIS; need to take into account condition types for match		
		//go reverse also...
		ConditionNodeTypeListIteratorQuery = queryEntityNode->ConditionNodeTypeReverseList.begin();
		for(ConditionIterQuery = queryEntityNode->ConditionNodeReverseList.begin(); ConditionIterQuery != queryEntityNode->ConditionNodeReverseList.end(); ConditionIterQuery++) 
		{	
			ConditionNodeTypeListIterator = entityNode->ConditionNodeTypeReverseList.begin();
			for(ConditionIter = entityNode->ConditionNodeReverseList.begin(); ConditionIter != entityNode->ConditionNodeReverseList.end(); ConditionIter++) 
			{	
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIterQuery, *ConditionIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, true, &(*ConditionNodeTypeListIterator));
				ConditionNodeTypeListIterator++;		
			}	
			ConditionNodeTypeListIteratorQuery++;		
		}
				
		
		
		//cout << "a3" << endl;
		
		//property connections	
		vector<GIAEntityNode*>::iterator entityIterQuery;
		vector<GIAEntityNode*>::iterator entityIter;	
		for(entityIterQuery = queryEntityNode->PropertyNodeList.begin(); entityIterQuery != queryEntityNode->PropertyNodeList.end(); entityIterQuery++) 
		{//DRAW SHOULD NOT BE REQUIRED	
			for(entityIter = entityNode->PropertyNodeList.begin(); entityIter != entityNode->PropertyNodeList.end(); entityIter++) 
			{//DRAW SHOULD NOT BE REQUIRED	
				//cout << "a31" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);
			}		
		}
		/*this has been removed 25 Sept - use entityNodeContainingThisProperty instead
		//go reverse also...		
		for(entityIter = entityNode->PropertyNodeReverseList.begin(); entityIter != entityNode->PropertyNodeReverseList.end(); entityIter++) 
		{//DRAW SHOULD NOT BE REQUIRED
			//cout << "a32" << endl;	
			currentReferenceInPrintList = testEntityNodeForQuery((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
		}
		*/	
		//cout << "a3b" << endl;
		//go upwards also...	
		if(queryEntityNode->entityNodeDefiningThisPropertyOrAction != NULL)
		{
			if(entityNode->entityNodeDefiningThisPropertyOrAction != NULL)
			{
				//cout << "a33" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->entityNodeDefiningThisPropertyOrAction, entityNode->entityNodeDefiningThisPropertyOrAction, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);
			}		
		}
		//cout << "a3c" << endl;
		if(queryEntityNode->entityNodeContainingThisProperty != NULL)
		{
			if(entityNode->entityNodeContainingThisProperty != NULL)
			{
				//cout << "a34" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->entityNodeContainingThisProperty, entityNode->entityNodeContainingThisProperty, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);
			}
		}
		//cout << "a4" << endl;
		

		
		//cout << "a5" << endl;				
		for(entityIterQuery = queryEntityNode->EntityNodeDefinitionList.begin(); entityIterQuery != queryEntityNode->EntityNodeDefinitionList.end(); entityIterQuery++) 
		{
			for(entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter != entityNode->EntityNodeDefinitionList.end(); entityIter++) 
			{
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);				
			}
		}
		//go reverse also...	
		for(entityIterQuery = queryEntityNode->EntityNodeDefinitionReverseList.begin(); entityIterQuery != queryEntityNode->EntityNodeDefinitionReverseList.end(); entityIterQuery++) 
		{//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
			for(entityIter = entityNode->EntityNodeDefinitionReverseList.begin(); entityIter != entityNode->EntityNodeDefinitionReverseList.end(); entityIter++) 
			{//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);						
			}
		}
		//cout << "a6" << endl;
		
				
		//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
		//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]	
		for(entityIterQuery = queryEntityNode->AssociatedPropertyNodeList.begin(); entityIterQuery != queryEntityNode->AssociatedPropertyNodeList.end(); entityIterQuery++) 
		{
			for(entityIter = entityNode->AssociatedPropertyNodeList.begin(); entityIter != entityNode->AssociatedPropertyNodeList.end(); entityIter++) 
			{
				//cout << "as0" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, false, false, NULL);						
				//cout << "as1" << endl;
			}
		}	
		
		//cout << "a7" << endl;
		
		/*
		if(initialiseOrPrint == DRAW_PRINT)
		{	
			//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
			
			int boxThickness = GIA_DRAW_THICKNESS_NORMAL;
			
			int entityColour;
			if(entityNode->hasAssociatedPropertyIsAction)
			{
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_ACTION_DEFINITION_NODE_COLOUR;	//clearly identify the definition of the action
				}
			}			
			else if(entityNode->isProperty)
			{
				if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
				{
					boxThickness = GIA_DRAW_THICKNESS_THICK;
				}
				
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else if(entityNode->hasQuantity)
				{
					entityColour = GIA_DRAW_PROPERTY_QUANTITY_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_PROPERTY_NODE_COLOUR;
				}
			}
			else if(entityNode->isAction)
			{
				entityColour = GIA_DRAW_ACTION_NODE_COLOUR;	
			}			
			else if(entityNode->hasAssociatedTime)
			{
				entityColour = GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR;	//clear identify a time node
			}
			//else if(entityNode->hasConditionTime)
			//{
			//	entityColour = GIA_DRAW_CONDITION_DEFINITION_NODE_COLOUR;	//clear identify a time node
			//}		
			else if(entityNode->hasAssociatedProperty)
			{//the original spec seemed to imply that entities that have associated properties (ie, that define properties) are special but they don't appear to be
				if(!(entityNode->isProperty))
				{		
					//added 2 May 11a (highlight entities which define property nodes)
					entityColour = GIA_DRAW_PROPERTY_DEFINITION_NODE_COLOUR;	//OLD: no colour modifier, just use basic entity colour; GIA_DRAW_BASICENTITY_NODE_COLOUR;
				}
			}					
			else
			{	
				entityColour = GIA_DRAW_BASICENTITY_NODE_COLOUR;
			}
										
			//first, print this action node.
			string nameOfBox = "";
			if(entityNode->hasQuantity)
			{
				char quantityNumberStringcharstar[20];
				if(entityNode->isQuery)
				{
					strcpy(quantityNumberStringcharstar, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_TEMP_FOR_DISPLAY_ONLY);
				}
				else
				{
					
					sprintf(quantityNumberStringcharstar, "%d", entityNode->quantityNumber);
					
				}
				nameOfBox = nameOfBox + quantityNumberStringcharstar + " " + entityNode->entityName;
				
			}
			else
			{
				nameOfBox = entityNode->entityName;
			}
			currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_ACTION_NODE_WIDTH, GIA_DRAW_ACTION_NODE_HEIGHT, entityColour, &nameOfBox, writeFileObject, boxThickness);

		}
		*/
		
		//cout << "a8" << endl;	
		#ifdef GIA_QUERY_DEBUG
		if(entityNode->isProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}
		else if(entityNode->isAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}		
		else if(entityNode->hasAssociatedProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property)" << endl;	
		}
		else if(entityNode->hasAssociatedPropertyIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		} 
		else if(entityNode->hasAssociatedTime)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << endl;
		}
		#endif
							
	}
	//cout << "a0c" << endl;
	
	return queryAnswerNode;
		
}








