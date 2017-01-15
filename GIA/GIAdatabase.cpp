/*******************************************************************************
 *
 * File Name: GIAdatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1k3d 11-May-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/
 
#include "GIAdatabase.h"

#ifdef GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
//uses fast search algorithm
GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList)
{
	GIAEntityNode * entityNodeFound = NULL;
	
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListIterator;
	conceptEntityNodesListIterator = conceptEntityNodesList->find(*entityNodeName);
	
	
	if(conceptEntityNodesListIterator != conceptEntityNodesList->end())
	{//concept entity found	
		#ifdef GIA_DATABASE_DEBUG
		cout << "\tentity node found; " << *entityNodeName << endl;
		#endif		
		entityNodeFound = conceptEntityNodesListIterator->second;
		*found = true;
	}
	else
	{//concept entity not found - add it to the map
		
		if(addIfNonexistant)
		{
			#ifdef GIA_DATABASE_DEBUG
			cout << "adding entity node; " << *entityNodeName << endl;
			#endif

			entityNodeFound = new GIAEntityNode();
			entityNodeFound->isConcept = true;	//added 10 May 2012
			entityNodeFound->id = *currentEntityNodeIDInCompleteList;
			entityNodeFound->idSecondary = *currentEntityNodeIDInConceptEntityNodesList;

			entityNodesCompleteList->push_back(entityNodeFound);
			(*currentEntityNodeIDInCompleteList) = (*currentEntityNodeIDInCompleteList) + 1;

			//conceptEntityNodesList[entityNodeName] = entityNodeFound;
			conceptEntityNodesList->insert(pair<string, GIAEntityNode*>(*entityNodeName, entityNodeFound));
			(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;

			entityNodeFound->entityName = *entityNodeName;	
		}
	}

	return entityNodeFound;
}
#else 
//uses fast search algorithm
GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList)
{
	GIAEntityNode * entityNodeFound = NULL;
	
	long vectorSize = conceptEntityNamesList->size();

	/*
	cout << "debug entity" << endl;
	vector<GIAEntityNode*>::iterator entityIter;
	for (entityIter = conceptEntityNodesList->begin(); entityIter != conceptEntityNodesList->end(); entityIter++) 
	{
		cout << "entityName = " << (*entityIter)->entityName << endl;
	}
	*/
	
		
	//vector<long>::iterator indexOfEntityNamesIterator;
	if(vectorSize == 0)
	{
		if(addIfNonexistant)
		{
			//cout << "vectorSize = "  << vectorSize << endl;
			//cout << "as" << endl;
			#ifdef GIA_DATABASE_DEBUG
			cout << "adding entity node; " << *entityNodeName << endl;
			#endif
			
			entityNodeFound = new GIAEntityNode();
			entityNodeFound->id = *currentEntityNodeIDInCompleteList;
			entityNodeFound->idSecondary = *currentEntityNodeIDInConceptEntityNodesList;
			
			entityNodesCompleteList->push_back(entityNodeFound);
			(*currentEntityNodeIDInCompleteList) = (*currentEntityNodeIDInCompleteList) + 1;
			conceptEntityNodesList->push_back(entityNodeFound);
			(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;

			entityNodeFound->entityName = *entityNodeName;
				//configure property definition node
			conceptEntityNamesList->push_back(*entityNodeName);	
		}
					
	}
	else
	{

		long findIndex = vectorSize/2;
		long findRange = maximumLong(findIndex/2, 1);	//maximumLong(((findIndex+1)/2), 1);
		long previousFindRange = findRange;
		
		//indexOfEntityNamesIterator = conceptEntityNamesList->begin() + startingPoint;

		//cout << "indexOfEntityNamesIterator = " << indexOfEntityNamesIterator << endl;

		int findRangeAtMinimaTimes = 0;

		bool searchOptionsAvailable = true;
		string previousTempName;
		string nameTemp;
		long previousFindIndex = findIndex;
		
		previousTempName = conceptEntityNamesList->at(findIndex);	//requires start value
		
		bool first = true;
		
		while(searchOptionsAvailable)
		{
			nameTemp = conceptEntityNamesList->at(findIndex);
			
			/*
			if(*entityNodeName == "Jane")
			{
				cout << "vectorSize = "  << vectorSize << endl;
				cout << "findIndex = " << findIndex << endl;
				cout << "findRange = " << findRange << endl;		      
				cout << "nameTemp = " << nameTemp << endl;
				cout << "*entityNodeName = " << *entityNodeName << endl;		      
			}
			*/
			
			
			//cout << "vectorSize = "  << vectorSize << endl;
			//cout << "findIndex = " << findIndex << endl;
			//cout << "findRange = " << findRange << endl;			
			//cout << "nameTemp = " << nameTemp << endl;
			//cout << "*entityNodeName = " << *entityNodeName << endl;
			
			if(nameTemp > *entityNodeName)
			{
				/*
				if(*entityNodeName == "red")
				{
					cout << nameTemp << ">" << *entityNodeName << endl;
					cout << "previousTempName = " << previousTempName << endl;
				}
				*/
								
				if(((previousTempName < *entityNodeName) && (previousFindRange == 1)) || (vectorSize==1))	//&& (!first || (vectorSize==1))
				{//optimum position lies inbetween
					//cout << "as" <<endl;
					searchOptionsAvailable = false;
					*found = false;	
					//findIndex = findIndex [use current findIndex, nb vectors get inserted before the current index]			
				}
				else
				{
					previousFindIndex = findIndex;
					long temp = (findIndex - findRange);
					//cout << "temp = " << temp;
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					if(findIndex < 0)
					{
						//cout << "error: (findIndex < 0)" << endl;
							//cout << "entityNodeName " << *entityNodeName << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
						findIndex = 0; //findIndex++;
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(nameTemp < *entityNodeName)
			{
				/*
				if(*entityNodeName == "red")
				{
					cout << nameTemp << "<" << *entityNodeName << endl;
					cout << "previousTempName = " << previousTempName << endl;
				}
				*/		
				
				if(((previousTempName > *entityNodeName) && (previousFindRange == 1)) || (vectorSize==1))		//& (!first || (vectorSize==1))
				{//optimum position lies inbetween
					searchOptionsAvailable = false;
					*found = false;
					findIndex = findIndex+1;		//Added 9 October 2011
					//cout << "here" << endl;
				}
				else
				{	
					previousFindIndex = findIndex;			
					long temp = (findIndex + findRange);
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					//cout << "findIndex = " << findIndex << endl;
					//cout << "(vectorSize-1) = " << (vectorSize-1) << endl;
					
					if(findIndex > (vectorSize-1))
					{

							//cout << "findIndex = " << findIndex << endl;
							//cout << "*entityNodeName = " << *entityNodeName << endl;
						//cout << "error: (findIndex > (vectorSize-1))" << endl;
							//cout << "entityNodeName " << *entityNodeName << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
						findIndex = (vectorSize);
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(nameTemp == *entityNodeName)
			{
				*index = findIndex;
				entityNodeFound = conceptEntityNodesList->at(findIndex);

				#ifdef GIA_DATABASE_DEBUG
				cout << "\tentity node found; " << *entityNodeName << endl;
				#endif
				//cout << "findIndex = " << findIndex << endl;
				
				searchOptionsAvailable = false;
				*found = true;
			}
			
			if((searchOptionsAvailable == false) && (*found == false) && (addIfNonexistant))
			{
				#ifdef GIA_DATABASE_DEBUG
				cout << "\t\tadding entity node; " << *entityNodeName << endl;
				#endif
				
				//cout << "previousFindIndex = " << previousFindIndex << endl;
				
				entityNodeFound = new GIAEntityNode();
				entityNodeFound->id = *currentEntityNodeIDInCompleteList;
				entityNodeFound->idSecondary = *currentEntityNodeIDInConceptEntityNodesList;
			
				entityNodesCompleteList->push_back(entityNodeFound);
				(*currentEntityNodeIDInCompleteList) = (*currentEntityNodeIDInCompleteList) + 1;
				entityNodeFound->entityName = *entityNodeName;
				
				vector<GIAEntityNode*>::iterator indexOfEntityNodesIterator = conceptEntityNodesList->begin();
				//indexOfEntityNodesIterator = conceptEntityNodesList->at(findIndex);
				advance(indexOfEntityNodesIterator,findIndex);
				conceptEntityNodesList->insert(indexOfEntityNodesIterator, entityNodeFound);

				vector<string>::iterator indexOfEntityNamesIterator = conceptEntityNamesList->begin();
				//indexOfEntityNamesIterator = conceptEntityNamesList->at(findIndex);
				advance(indexOfEntityNamesIterator,findIndex);
				conceptEntityNamesList->insert(indexOfEntityNamesIterator, *entityNodeName);
				
				(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;	
			}
			
			first = false;
			
			previousTempName = nameTemp;
			

		}
	}

	/*
	cout << "debug entity" << endl;
	vector<GIAEntityNode*>::iterator entityIter;
	for (entityIter = conceptEntityNodesList->begin(); entityIter != conceptEntityNodesList->end(); entityIter++) 
	{
		cout << (*entityIter)->entityName << endl;
	}
	*/
	/*
	cout << "debug names" << endl;
	vector<string>::iterator stringIter;
	for (stringIter = conceptEntityNamesList->begin(); stringIter != conceptEntityNamesList->end(); stringIter++) 
	{
		cout << (*stringIter) << endl;
	}	
	*/			
				
	return entityNodeFound;
}
#ifdef GIA_USE_TIME_NODE_INDEXING
//CHECK THIS, it has been updated based upon above code as a template
GIATimeConditionNode * findOrAddTimeNodeByNumber(vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList, long * timeNodeNumber, bool * found, long * index, bool addIfNonexistant, GIATimeConditionNode * prexistingTimeConditionNode)
{
	GIATimeConditionNode * timeNodeFound = NULL;
	
	long vectorSize = timeConditionNumbersList->size();
	
	//vector<long>::iterator indexOfEntityNamesIterator;
	if(vectorSize == 0)
	{
		if(addIfNonexistant)
		{
			//cout << "vectorSize = "  << vectorSize << endl;
			cout << "adding time node; " << *timeNodeNumber << endl;

			if(prexistingTimeConditionNode != NULL)
			{
				timeNodeFound = prexistingTimeConditionNode;
			}
			else
			{
				timeNodeFound = new GIATimeConditionNode();
			}
			timeNodeFound->totalTimeInSeconds = *timeNodeNumber;
				//configure property definition node
			timeConditionNodesList->push_back(timeNodeFound);	
			timeConditionNumbersList->push_back(*timeNodeNumber);	
		}
					
	}
	else
	{
		long findIndex = vectorSize/2;
		long findRange = maximumLong(findIndex/2, 1);	//maximumLong(((findIndex+1)/2), 1);
		long previousFindRange = findRange;
		
		int findRangeAtMinimaTimes = 0;

		bool searchOptionsAvailable = true;
		long previousTempTime;
		long timeTemp;
		long previousFindIndex = findIndex;
		
		previousTempTime = timeConditionNumbersList->at(findIndex);	//requires start value
		
		while(searchOptionsAvailable)
		{
			//cout << "vectorSize = "  << vectorSize << endl;
			//cout << "findIndex = " << findIndex << endl;
			//cout << "findRange = " << findRange << endl;
			timeTemp = timeConditionNumbersList->at(findIndex);
			//cout << "timeTemp = " << timeTemp << endl;

			

			if(timeTemp > *timeNodeNumber)
			{
				if(((previousTempTime < *timeNodeNumber) && (previousFindRange == 1)) || (vectorSize==1))
				{//optimum position lies inbetween
					searchOptionsAvailable = false;
					*found = false;	
					//findIndex = findIndex [use current findIndex, nb vectors get inserted before the current index]			
				}
				else
				{
					previousFindIndex = findIndex;
					long temp = (findIndex - findRange);
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					//cout << timeTemp << ">" << *timeNodeNumber << endl;
					if(findIndex < 0)
					{
						//cout << "error: (findIndex < 0)" << endl;
						//cout << "timeNodeNumber " << *timeNodeNumber << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
						findIndex = 0; //findIndex++;
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(timeTemp < *timeNodeNumber)
			{
				if(((previousTempTime > *timeNodeNumber) && (previousFindRange == 1)) || (vectorSize==1))
				{//optimum position lies inbetween
					searchOptionsAvailable = false;
					*found = false;
					findIndex = previousFindIndex;
				}
				else
				{	
					previousFindIndex = findIndex;			
					long temp = (findIndex + findRange);
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					//cout << timeTemp << "<" << *timeNodeNumber << endl;		
					if(findIndex > (vectorSize-1))
					{

						//cout << "findIndex = " << findIndex << endl;
						//cout << "*timeNodeNumber = " << *timeNodeNumber << endl;
						//cout << "error: (findIndex > (vectorSize-1))" << endl;
						//cout << "timeNodeNumber " << *timeNodeNumber << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(timeTemp == *timeNodeNumber)
			{
				*index = findIndex;
				timeNodeFound = timeConditionNodesList->at(findIndex);

				cout << "time node found; " << *timeNodeNumber << endl;
				//cout << "findIndex = " << findIndex << endl;
				
				searchOptionsAvailable = false;
				*found = true;
			}
			
			if((searchOptionsAvailable == false) && (*found == false) && (addIfNonexistant))
			{
				cout << "adding time node; " << *timeNodeNumber << endl;

				if(prexistingTimeConditionNode != NULL)
				{
					timeNodeFound = prexistingTimeConditionNode;
				}
				else
				{
					timeNodeFound = new GIATimeConditionNode();
				}
				timeNodeFound->totalTimeInSeconds = *timeNodeNumber;
				vector<GIATimeConditionNode*>::iterator indexOfTimeNodesIterator = timeConditionNodesList->begin();
				//indexOfTimeNodesIterator = timeConditionNodesList->at(findIndex);
				advance(indexOfTimeNodesIterator,findIndex);
				timeConditionNodesList->insert(indexOfTimeNodesIterator, timeNodeFound);

				vector<long>::iterator indexOfTimeNumbersIterator = timeConditionNumbersList->begin();
				//indexOfTimeNumbersIterator = timeConditionNumbersList->at(findIndex);
				advance(indexOfTimeNumbersIterator,findIndex);
				timeConditionNumbersList->insert(indexOfTimeNumbersIterator, *timeNodeNumber);
			}
			
			previousTempTime = timeTemp;
			

		}
	}

	/*
	cout << "debug entity" << endl;
	vector<GIAEntityNode*>::iterator entityIter;
	for (entityIter = conceptEntityNodesList->begin(); entityIter != conceptEntityNodesList->end(); entityIter++) 
	{
		cout << (*entityIter)->entityName << endl;
	}
	cout << "debug names" << endl;
	vector<string>::iterator stringIter;
	for (stringIter = conceptEntityNamesList->begin(); stringIter != conceptEntityNamesList->end(); stringIter++) 
	{
		cout << (*stringIter) << endl;
	}	
	*/			
				
	return timeNodeFound;
}
#endif
#endif
GIAEntityNode * findEntityNodeByID(long EntityNodeID, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	GIAEntityNode * foundEntityNode = NULL;
	foundEntityNode = entityNodesCompleteList->at(EntityNodeID);
	
	/*
	GIAEntityNode * currentEntityNode = entityNodesCompleteList;
	while(currentEntityNode->next != NULL)
	{
		if(currentEntityNode->id == EntityNodeID)
		{
			foundEntityNode = currentEntityNode;
		}
		currentEntityNode = currentEntityNode->next;
	}
	*/
	
	return foundEntityNode;
}



long maximumLong(long a, long b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}






