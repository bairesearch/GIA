/*******************************************************************************
 *
 * File Name: GIAdatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l5e 03-June-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/
 
#include "GIAdatabase.h"

#define CHAR_NEW_LINE '\n'

#ifdef GIA_USE_DATABASE

unordered_map<string, bool> *DBconceptEntityNodesLoadedList;
bool useDatabase;

GIAEntityNode * findOrAddConceptEntityNodeByName(vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
{
	GIAEntityNode * entityNodeFound = NULL;
	if(useDatabase)
	{
		entityNodeFound = DBfindOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodeName, found, index, addIfNonexistant, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList, saveNetwork);
	}
	else
	{
		entityNodeFound = LocalFindOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodeName, found, index, addIfNonexistant, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList, saveNetwork);
	}
	return entityNodeFound;
}

GIAEntityNode * DBfindOrAddConceptEntityNodeByName(vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
{
	GIAEntityNode * entityNodeFound = NULL;
	
	unordered_map<string, bool>::iterator conceptEntityNodesLoadedListIterator = DBconceptEntityNodesLoadedList->find(*entityNodeName);
	
	if(conceptEntityNodesLoadedListIterator != DBconceptEntityNodesLoadedList->end())
	{//concept entity found	
			
		bool conceptEntityNodeLoaded = conceptEntityNodesLoadedListIterator->second;
		
		GIAEntityNode * conceptEntityNode;
		if(conceptEntityNodeLoaded)
		{
			unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListIterator;
			conceptEntityNodesListIterator = entityNodesActiveListConcepts->find(*entityNodeName);		
			conceptEntityNode = conceptEntityNodesListIterator->second;
		}
		else
		{
			//load the concept node from the database into RAM
			conceptEntityNode = new GIAEntityNode();
			DBreadConceptEntityNode(entityNodeName, conceptEntityNode);
			entityNodesActiveListConcepts->insert(pair<string, GIAEntityNode*>(*entityNodeName, conceptEntityNode));
			//readConceptEntityNumberOfInstances(entityNodeName);
		}
			
		#ifdef GIA_DATABASE_DEBUG
		cout << "\tentity node found; " << *entityNodeName << endl;
		#endif		
		entityNodeFound = conceptEntityNode;
		*found = true;
	}
	else
	{//concept entity not found - add it to the map
		
		if(addIfNonexistant)
		{
			#ifdef GIA_DATABASE_DEBUG
			cout << "adding entity node; " << *entityNodeName << endl;
			#endif

			GIAEntityNode * newEntityNode = new GIAEntityNode();
			newEntityNode->isConcept = true;	//added 10 May 2012
			newEntityNode->idActiveList = *currentEntityNodeIDInCompleteList;
			newEntityNode->idActiveEntityTypeList = *currentEntityNodeIDInConceptEntityNodesList;
			#ifdef GIA_USE_DATABASE
			newEntityNode->added = true;
			#endif
			
			if(saveNetwork)
			{
				entityNodesActiveListComplete->push_back(newEntityNode);							//not used for database...
			}
			(*currentEntityNodeIDInCompleteList) = (*currentEntityNodeIDInCompleteList) + 1;				//used to indicate that this new node may need to be updated on the database

			entityNodesActiveListConcepts->insert(pair<string, GIAEntityNode*>(*entityNodeName, newEntityNode));
			(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;		//not used for database...

			bool conceptEntityNodeLoaded = true;
			DBconceptEntityNodesLoadedList->insert(pair<string, bool>(*entityNodeName, conceptEntityNodeLoaded));		
			newEntityNode->added = true;											//used to indicate that this new node needs to be added to the database (at the end of the routine, after it is ensured that it has not been disabled)
			/*
			bool conceptEntityNodeAdded = true;
			conceptEntityNodesModifiedList->insert(pair<string, bool>(*entityNodeName, conceptEntityNodeAdded));		
			*/
						
			newEntityNode->entityName = *entityNodeName;	
		
			entityNodeFound = newEntityNode;
		}
	}

	return entityNodeFound;
}
#endif


//uses fast search algorithm
#ifdef GIA_USE_DATABASE
GIAEntityNode * LocalFindOrAddConceptEntityNodeByName(vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
#else
GIAEntityNode * findOrAddConceptEntityNodeByName(vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
#endif
{
	GIAEntityNode * entityNodeFound = NULL;
	
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListIterator;
	conceptEntityNodesListIterator = entityNodesActiveListConcepts->find(*entityNodeName);
	
	
	if(conceptEntityNodesListIterator != entityNodesActiveListConcepts->end())
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
			entityNodeFound->idActiveList = *currentEntityNodeIDInCompleteList;
			entityNodeFound->idActiveEntityTypeList = *currentEntityNodeIDInConceptEntityNodesList;
			#ifdef GIA_USE_DATABASE
			entityNodeFound->added = true;
			#endif
			
			if(saveNetwork)
			{
				entityNodesActiveListComplete->push_back(entityNodeFound);
			}
			(*currentEntityNodeIDInCompleteList) = (*currentEntityNodeIDInCompleteList) + 1;

			//entityNodesActiveListConcepts[entityNodeName] = entityNodeFound;
			entityNodesActiveListConcepts->insert(pair<string, GIAEntityNode*>(*entityNodeName, entityNodeFound));
			(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;

			entityNodeFound->entityName = *entityNodeName;	
		}
	}

	return entityNodeFound;
}


GIAEntityNode * findActiveEntityNodeByID(long EntityNodeID, vector<GIAEntityNode*> *entityNodesActiveListComplete)
{
	GIAEntityNode * foundEntityNode = NULL;
	foundEntityNode = entityNodesActiveListComplete->at(EntityNodeID);
	
	/*
	GIAEntityNode * currentEntityNode = entityNodesActiveListComplete;
	while(currentEntityNode->next != NULL)
	{
		if(currentEntityNode->idActiveList == EntityNodeID)
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










#ifdef GIA_USE_DATABASE


string DBgenerateServerName(string * entityName)
{
	char entityFirstCharacter = entityName->at(0);
	if((entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z))
	{
		cout << "determineServerName error: (entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z)" << endl;
		exit(0);
	}

	int entityFirstCharacterIndex = entityFirstCharacter - ASCII_TABLE_INDEX_OF_a;
	string serverName = serverNameArray[entityFirstCharacterIndex]; 	//this could be a more complex algorithm; eg serverName = (string)"/mnt/" + serverNameArray[entityFirstCharacterIndex]
	#ifdef GIA_DATABASE_DEBUG
	cout << "serverName = " << serverName << endl;
	#endif
	return serverName;
}

//NB idInstance 0 corresponds to the concept entity (null instance)
string DBgenerateFileName(string * entityName, long idInstance, int connectionType, int fileType)
{
	//eg network/server/GIAdatabase/e/x/a/example/1/2/3/instance123000000/{vectorConnectionsReferencesConnectionTypeX}.txt	//OLD: //eg network/server/GIAdatabase/e/x/a/example/1/2/3/{vectorConnectionsReferencesConnectionTypeX}/instance123000000.txt
	string serverName = DBgenerateServerName(entityName);
	string databaseName = GIA_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	string fileName = serverName + databaseName;
	
	if(fileType >= GIA_DATABASE_GENERATE_FILENAME_FILE_CONCEPT_ENTITY_NODES_LIST)
	{
		fileName = fileName + GIA_DATABASE_CONCEPT_ENTITY_NODES_FILE_NAME;
	}
	else
	{
		if(entityName->length() < GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)
		{
			cout << "DBgenerateFileName error: (entityName.length() < GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)" << endl;
		}
		for(int level=0; level<GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS; level++)
		{
			fileName = fileName + entityName[level] + "/";
		}
		fileName = fileName + *entityName + "/";

		char idInstanceStringCharStar[GIA_DATABASE_INSTANCE_ID_MAX_ORDER+1];	//+1 for char * string null character \0
		sprintf(idInstanceStringCharStar, GIA_DATABASE_INSTANCE_ID_STRING_FORMAT, idInstance);
		string idInstanceString = idInstanceStringCharStar;

		for(int level=0; level<GIA_DATABASE_INSTANCE_ID_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS; level++)
		{
			fileName = fileName + idInstanceString[level] + "/";
		}	
		fileName = fileName + idInstanceString + "/";

		if(fileType == GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY)
		{
			fileName = fileName + GIA_ENTITY_NODE_NAME;

			fileName = fileName + GIA_DATABASE_ENTITY_FILE_NAME_EXTENSION;
		}
		else if(fileType == GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE)
		{
			fileName = fileName + GIA_ENTITY_TIME_CONDITION_NODE_NAME;

			fileName = fileName + GIA_DATABASE_ENTITY_FILE_NAME_EXTENSION;	
		}
		else if(fileType == GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES)
		{
			fileName = fileName + entityVectorConnectionNameArray[connectionType];
			fileName = fileName + GIA_DATABASE_REFERENCES_FILE_NAME_EXTENSION;
		}
		else
		{
			cout << "error: illegal fileType" << endl;
			exit(0);
		}
	}
	
	#ifdef GIA_DATABASE_DEBUG
	cout << "fileName = " << fileName << endl;
	#endif
	
	return fileName;
}

	


















/*
void DBreadReferencesFile(string * referencesFileName, GIAEntityNode* entity)
{
	//read file into c struct
	
	ifstream parseFileObject(referencesFileName->c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//xml file does not exist in current directory.
		cout << "Error: GIA Entity File does not exist in current directory: " << *referencesFileName << endl;
		result = false;
	}
	else
	{
		char currentToken;
		while(parseFileObject->get(currentToken))
		{
			
		}
	}
}
*/











void openDatabase()
{
	DBreadConceptEntityNodesLoadedList();
	
	setUseDatabase(false);	//at initialisation (/by default), do not use database - use active list instead			
}

void DBreadConceptEntityNodesLoadedList()	//unordered_map<string, bool> *DBconceptEntityNodesLoadedListLocal
{
	string conceptEntityNodesListFileName = DBgenerateFileName(NULL, NULL, NULL, GIA_DATABASE_GENERATE_FILENAME_FILE_CONCEPT_ENTITY_NODES_LIST);

	ifstream parseFileObject(conceptEntityNodesListFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//file does not exist in current directory.
		cout << "DBreadConceptEntityNodesLoadedList() error: GIA Concept Entity Nodes List File does not exist in current directory: " << conceptEntityNodesListFileName << endl;
		exit(0);
	}
	else
	{
		char currentToken;
		char conceptEntityNameCharStar[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		while(parseFileObject.getline(conceptEntityNameCharStar, GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH))
		{
			string conceptEntityName = conceptEntityNameCharStar;
			DBconceptEntityNodesLoadedList->insert(pair<string, bool>(conceptEntityName, false));			
		}
	}
}


void DBreadVectorConnections(GIAEntityNode * entityNode, int connectionType)
{	
	if(!(entityNode->entityVectorConnectionsReferenceListLoadedArray[connectionType]))
	{
		DBreadVectorConnectionsReferences(&(entityNode->entityName), entityNode->idInstance, connectionType, &(entityNode->entityVectorConnectionsArray[connectionType]));
		entityNode->entityVectorConnectionsReferenceListLoadedArray[connectionType] = true;
	}

	DBreadVectorConnectionEntities(&(entityNode->entityName), entityNode->idInstance, connectionType, &(entityNode->entityVectorConnectionsArray[connectionType]));	
}

//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
//assume vectorConnectionProperties has not been populated
void DBreadVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<GIAEntityConnection*> *entityVectorConnections)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	//now read file
	

 	FILE * pFile = NULL;
	const char * fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile == NULL)
	{
		cout << "DBreadVectorConnectionsReferences() error: referencesFileName, " << referencesFileName << " does not exist" << endl;
	}
	else
	{
		#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
		cout << "error: GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION not yet coded to take advantage of fixed length file columns" << endl;
		#else	
		while(!feof(pFile))
		{
			char connectionEntityName[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH]; 
			long connectionInstanceID; 
			fscanf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, &connectionEntityName, &connectionInstanceID); 		
			
			GIAEntityConnection* newConnection = new GIAEntityConnection();
			newConnection->entityName = connectionEntityName;
			newConnection->idInstance = connectionInstanceID;
			newConnection->referenceLoaded = true;
			entityVectorConnections->push_back(newConnection);
		}
		#endif
		
		fclose(pFile);
	}
		
}

void DBreadVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long *entityVectorConnectionsID, long referenceIndex)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance <-- read this
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
		
	//determine position in file to write...
	int positionOfReference = GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE*referenceIndex;
	
	//now write file
	char connectionEntityName[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH]; 
	long connectionInstanceID; 	
	
 	FILE * pFile = NULL;
	const char * fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"r");
	fseek(pFile, positionOfReference, SEEK_SET);	
	fscanf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
	
	*entityVectorConnectionsName = connectionEntityName;
	*entityVectorConnectionsID = connectionInstanceID;
	
	fclose(pFile);
		
}


//assumes entityVectorConnections list is not already populated (ie, not already intialised with null pointers / GIA entity nodes)	
void DBreadVectorConnectionEntities(string * entityName, long idInstance, int connectionType, vector<GIAEntityConnection*> *entityVectorConnections)
{	
	for(vector<GIAEntityConnection*>::iterator connectionIter = entityVectorConnections->begin(); connectionIter != entityVectorConnections->end(); connectionIter++) 
	{
		GIAEntityConnection* connection = *connectionIter;
		
		if(!(connection->loaded))
		{
			GIAEntityNode * entity = new GIAEntityNode();
			DBreadEntityNode(&(connection->entityName), connection->idInstance, entity);

			connection->entity = entity;
			connection->loaded = true;
			connection->modified = false;
			connection->added = false;
		}
	}
}


void DBreadConceptEntityNode(string * entityName, GIAEntityNode * conceptEntityNode)
{
	int idInstance = GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE;
	DBreadEntityNode(entityName, idInstance, conceptEntityNode);
}




void DBreadEntityNode(string * entityName, long idInstance, GIAEntityNode * entityNode)
{
	string entityFileName = DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY);
	DBreadEntityNodeFile(&entityFileName, entityNode);
	
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		entityNode->timeConditionNode = new GIATimeConditionNode();
		string timeConditionFileName = DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE);
		DBreadTimeConditionNodeFile(&timeConditionFileName, entityNode->timeConditionNode);
	}
	
	DBsetEntityConnectionsLoaded(entityNode, false);
	//do not read references
}	


//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBreadEntityNodeFile(string * entityFileName, GIAEntityNode* entity)
{
	/*
	Format:
	idActiveList,entityName,confidence,isConcept,isProperty,isAction,isCondition,hasAssociatedInstance,hasAssociatedInstanceIsAction,hasAssociatedPropertyIsCondition,hasAssociatedTime,hasQuality,disabled,conditionType,grammaticalNumber,hasQuantity,hasMeasure
	//format derived from GIA XML file
	*/
	
	//read file into c struct directly?
	//use fscanf instead? (http://bytes.com/topic/c/answers/882529-read-text-file-into-formatted-struct-array)

 	FILE * pFile = NULL;
	const char * fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile == NULL)
	{
		cout << "DBreadEntityNodeFile() error: entityFileName, " << *entityFileName << " does not exist" << endl;
	}
	else
	{
		//these need to be converted back to booleans
		
		int isConcept;
		int isProperty;
		int isAction;
		int isCondition;
		int hasAssociatedInstance;
		int hasAssociatedInstanceIsAction;
		int hasAssociatedInstanceIsCondition;
		int hasAssociatedTime;
		int hasQuality;
		int disabled;
		//int conditionType;
		//int grammaticalNumber;
		int hasQuantity;
		int hasMeasure; 	
	
		fscanf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT, &(entity->idActiveList), &(entity->entityName), &(entity->confidence), &isConcept, &isProperty, &isAction, &isCondition, &hasAssociatedInstance, &hasAssociatedInstanceIsAction, &hasAssociatedInstanceIsCondition, &hasAssociatedTime, &hasQuality, &disabled, &(entity->conditionType), &(entity->grammaticalNumber), &hasQuantity, &hasMeasure); 
		//fscanf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT, &(entity->idActiveList), &(entity->entityName), &(entity->confidence), &(int(entity->isConcept)), &(int(entity->isProperty)), &(int(entity->isAction)), &(int(entity->isCondition)), &(int(entity->hasAssociatedInstance)), &(int(entity->hasAssociatedInstanceIsAction)), &(int(entity->hasAssociatedInstanceIsCondition)), &(int(entity->hasAssociatedTime)), &(int(entity->hasQuality)), &(int(entity->disabled)), &(entity->conditionType), &(entity->grammaticalNumber), &(int(entity->hasQuantity)), &(int(entity->hasMeasure))); 
		fclose(pFile);
	
		entity->isConcept = bool(isConcept);
		entity->isProperty = bool(isProperty);
		entity->isAction = bool(isAction);
		entity->isCondition = bool(isCondition);
		entity->hasAssociatedInstance = bool(hasAssociatedInstance);
		entity->hasAssociatedInstanceIsAction = bool(hasAssociatedInstanceIsAction);
		entity->hasAssociatedInstanceIsCondition = bool(hasAssociatedInstanceIsCondition);
		entity->hasAssociatedTime = bool(hasAssociatedTime);
		entity->hasQuality = bool(hasQuality);
		entity->disabled = bool(disabled);
		//entity->conditionType = conditionType;
		//entity->grammaticalNumber = grammaticalNumber;
		entity->hasQuantity = bool(hasQuantity);
		entity->hasMeasure = bool(hasMeasure); 
			
	}
  	
	/*
	ifstream parseFileObject(entityFileName->c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//xml file does not exist in current directory.
		cout << "Error: GIA Entity File does not exist in current directory: " << *entityFileName << endl;
		result = false;
	}
	else
	{
		char currentToken;
		while(parseFileObject->get(currentToken))
		{
			
		}
	}
	*/

}

//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBreadTimeConditionNodeFile(string * timeConditionFileName, GIATimeConditionNode* timeCondition)
{
	/*
	Format:
	conditionName,tense,second,hour,dayOfWeek,month,dayOfMonth,year,period,totalTimeInSeconds,tenseOnlyTimeCondition,isProgressive
	//format derived from GIA XML file
	*/
	
 	FILE * pFile = NULL;
	const char * fileNameCharStar = timeConditionFileName->c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile == NULL)
	{
		cout << "DBreadTimeConditionNodeFile() error: timeConditionFileName, " << *timeConditionFileName << " does not exist" << endl;
	}
	else
	{
		int tenseOnlyTimeCondition;
		int isProgressive;
			
		fscanf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT, &(timeCondition->conditionName), &(timeCondition->tense), &(timeCondition->second), &(timeCondition->hour), &(timeCondition->dayOfWeek), &(timeCondition->month), &(timeCondition->dayOfMonth), &(timeCondition->year), &(timeCondition->period), &(timeCondition->totalTimeInSeconds), &tenseOnlyTimeCondition, &isProgressive);							//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o) 			
		//fscanf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT, &(timeCondition->conditionName), &(timeCondition->tense), &(timeCondition->second), &(timeCondition->hour), &(timeCondition->dayOfWeek), &(timeCondition->month), &(timeCondition->dayOfMonth), &(timeCondition->year), &(timeCondition->period), &(timeCondition->totalTimeInSeconds), &(int(timeCondition->tenseOnlyTimeCondition)), &(int(timeCondition->isProgressive)));	//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o) 
		fclose(pFile);
		
		timeCondition->tenseOnlyTimeCondition = bool(tenseOnlyTimeCondition);
		timeCondition->isProgressive = bool(isProgressive);
	}
  
}





void writeAndCloseDatabase(vector<GIAEntityNode*> *entityNodesActiveListComplete)
{
	writeDatabase(entityNodesActiveListComplete);
	
	DBwriteConceptEntityNodesLoadedList();
}

void writeDatabase(vector<GIAEntityNode*> *entityNodesActiveListComplete)
{
	for(vector<GIAEntityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesActiveListComplete->begin(); entityNodesActiveCompleteListIterator != entityNodesActiveListComplete->end(); entityNodesActiveCompleteListIterator++) 
	{
		GIAEntityNode* entityNode = *entityNodesActiveCompleteListIterator;
		
		if(entityNode->added || entityNode->modified)
		{
			//(re-)write node itself
			DBwriteEntityNode(&(entityNode->entityName), entityNode->idInstance, entityNode);
		}
		
		//only write specific connections
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			if(entityNode->entityVectorConnectionsReferenceListLoadedArray[i])
			{//they should all be loaded			

				if(entityNode->added || entityNode->entityVectorConnectionsRemovedArray[i])
				{	
					//write all vector connection references
					DBwriteVectorConnectionsReferences(&(entityNode->entityName), entityNode->idInstance, i, &(entityNode->entityVectorConnectionsArray[i]));					
				}
				else
				{
					int referenceIndex = 0;			
					for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++) 
					{
						GIAEntityConnection * connection = *connectionIter;
						if(connection->modified)
						{//modified; overwrite vector connection reference
							DBmodifyVectorConnectionsReference(&(entityNode->entityName), entityNode->idInstance, i, &(connection->entityName), connection->idInstance, referenceIndex);
						}
						else if(connection->added)
						{//added; append vector connection reference
							DBappendVectorConnectionsReference(&(entityNode->entityName), entityNode->idInstance, i, &(connection->entityName), connection->idInstance);
						}
						referenceIndex++;
					}
				}
			}
			else
			{
				cout << "writeDatabase() error: entityVectorConnectionsReferenceListLoadedArray[i] != true" << endl;
				cout << "entityNode = " << entityNode->entityName << endl;
			}
		}				

	}
	
	/* this is done in DBwriteConceptEntityNodesLoadedList()
	string conceptEntityName = conceptEntityNodesLoadedListIterator->first;
	const char * conceptEntityNameCharStar = conceptEntityName.c_str();
	writeFileObject.write(conceptEntityNameCharStar, conceptEntityName.length());
	//for(int i=0; i<conceptEntityName.length(); i++)
	//{
	//	writeFileObject.put(conceptEntityName[i]);
	//}
	writeFileObject.put(CHAR_NEW_LINE);
	*/
			
}



void DBwriteEntityNode(string * entityName, long idInstance, GIAEntityNode * entityNode)
{
	string entityFileName = DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY);
	DBwriteEntityNodeFile(&entityFileName, entityNode);
	
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		string timeConditionFileName = DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE);
		DBwriteTimeConditionNodeFile(&timeConditionFileName, entityNode->timeConditionNode);
	}
}	


//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBwriteEntityNodeFile(string * entityFileName, GIAEntityNode* entity)
{
	/*
	Format:
	idActiveList,entityName,confidence,isConcept,isProperty,isAction,isCondition,hasAssociatedInstance,hasAssociatedInstanceIsAction,hasAssociatedPropertyIsCondition,hasAssociatedTime,hasQuality,disabled,conditionType,grammaticalNumber,hasQuantity,hasMeasure
	//format derived from GIA XML file
	*/
	
 	FILE * pFile = NULL;
	const char * fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBwriteEntityNodeFile() error: entityFileName, " << *entityFileName << " does not exist" << endl;
	}
	else
	{
		fprintf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT, entity->idActiveList, entity->entityName, entity->confidence, int(entity->isConcept), int(entity->isProperty), int(entity->isAction), int(entity->isCondition), int(entity->hasAssociatedInstance), int(entity->hasAssociatedInstanceIsAction), int(entity->hasAssociatedInstanceIsCondition), int(entity->hasAssociatedTime), int(entity->hasQuality), int(entity->disabled), entity->conditionType, entity->grammaticalNumber, int(entity->hasQuantity), int(entity->hasMeasure)); 
		fclose(pFile);
	}
	
	/*
	ofstream writeFileObject(entityFileName->c_str());

	writeFileObject.put(CHAR_NEW_LINE);

	writeFileObject.close();
	*/
}

//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBwriteTimeConditionNodeFile(string * timeConditionFileName, GIATimeConditionNode* timeCondition)
{
	/*
	Format:
	conditionName,tense,second,hour,dayOfWeek,month,dayOfMonth,year,period,totalTimeInSeconds,tenseOnlyTimeCondition,isProgressive
	//format derived from GIA XML file
	*/
	
 	FILE * pFile = NULL;
	const char * fileNameCharStar = timeConditionFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBwriteTimeConditionNodeFile() error: timeConditionFileName, " << *timeConditionFileName << " does not exist" << endl;
	}
	else
	{
		fprintf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT, (timeCondition->conditionName), (timeCondition->tense), (timeCondition->second), (timeCondition->hour), (timeCondition->dayOfWeek), (timeCondition->month), (timeCondition->dayOfMonth), (timeCondition->year), (timeCondition->period), (timeCondition->totalTimeInSeconds), (int(timeCondition->tenseOnlyTimeCondition)), (int(timeCondition->isProgressive)));	//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o) 
		fclose(pFile);
	}
}




//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBwriteVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<GIAEntityConnection*> *entityVectorConnections)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	//now write file
	

 	FILE * pFile = NULL;
	const char * fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBwriteVectorConnectionsReferences() error: referencesFileName, " << referencesFileName << " does not exist" << endl;
	}
	else
	{
		#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
		cout << "error: GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION not yet coded to take advantage of fixed length file columns" << endl;
		#else	
		
		for(vector<GIAEntityConnection*>::iterator connectionIter = entityVectorConnections->begin(); connectionIter != entityVectorConnections->end(); connectionIter++) 
		{
			GIAEntityConnection * connection = *connectionIter;
			
			char * connectionEntityName = const_cast<char*>(connection->entityName.c_str()); 	//NB NOT connection->entity->entityName (as DBwriteVectorConnectionsReferences() does not assume connection->loaded == true) 
			long connectionInstanceID = connection->idInstance; 					//NB NOT connection->entity->idInstance (as DBwriteVectorConnectionsReferences() does not assume connection->loaded == true) 
			fprintf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
		}
		#endif
		
		fclose(pFile);
	}
}

void DBmodifyVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long entityVectorConnectionsID, long referenceIndex)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance <--- modify
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	const char * fileNameCharStar = referencesFileName.c_str();
		
	//determine position in file to write...
	int positionOfReference = GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE*referenceIndex;
	
	//now write file
	char * connectionEntityName = const_cast<char*>(entityVectorConnectionsName->c_str()); 
	long connectionInstanceID = entityVectorConnectionsID; 	
		
	FILE* pFile = fopen(fileNameCharStar, "r+b"); // r+ if you need char mode
	fseek(pFile, positionOfReference, SEEK_SET);	
	fprintf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
	/*
	char reference[GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE];
	printf(reference, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 		
	fwrite(reference, sizeof(char), GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE, pFile);
	*/
	
	fclose(pFile);	
}

void DBappendVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long entityVectorConnectionsID)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance 
	entityName,idInstance <--- append
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	const char * fileNameCharStar = referencesFileName.c_str();
	
	//now write file
	char * connectionEntityName = const_cast<char*>(entityVectorConnectionsName->c_str()); 
	long connectionInstanceID = entityVectorConnectionsID; 	
		
	FILE* pFile = fopen(fileNameCharStar, "a");
	fprintf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
	/*
	char reference[GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE];
	printf(reference, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 		
	fwrite(reference, sizeof(char), GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE, pFile);
	*/
	
	fclose(pFile);	
}


void DBwriteConceptEntityNodesLoadedList()	//unordered_map<string, bool> *DBconceptEntityNodesLoadedList 
{
	string conceptEntityNodesListFileName = DBgenerateFileName(NULL, NULL, NULL, GIA_DATABASE_GENERATE_FILENAME_FILE_CONCEPT_ENTITY_NODES_LIST);

	ofstream writeFileObject(conceptEntityNodesListFileName.c_str());

	for(unordered_map<string, bool>::iterator conceptEntityNodesLoadedListIterator = DBconceptEntityNodesLoadedList->begin(); conceptEntityNodesLoadedListIterator != DBconceptEntityNodesLoadedList->end(); conceptEntityNodesLoadedListIterator++) 
	{
		string conceptEntityName = conceptEntityNodesLoadedListIterator->first;
		const char * conceptEntityNameCharStar = conceptEntityName.c_str();
		writeFileObject.write(conceptEntityNameCharStar, conceptEntityName.length());
		/*
		for(int i=0; i<conceptEntityName.length(); i++)
		{
			writeFileObject.put(conceptEntityName[i]);
		}
		*/
		writeFileObject.put(CHAR_NEW_LINE);
	}
	
	writeFileObject.close();
}


void closeDatabase()
{
	//no action
}










long DBreadConceptEntityNumberOfInstances(string * entityNodeName)		//OLD: GIAEntityNode * entityNodeName
{
	int idInstance = GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE;
	int connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES;
	long numberOfReferences = DBreadNumberOfReferencesInList(entityNodeName, idInstance, connectionType);
}

//given GIA_DATABASE_INSTANCE_ID_MAX_ORDER (9), this implies there are up to than 9.999 billion instances, and as such a 32 bit signed int is insufficient [max +2147483648 ~= 2 billion, so a 64bit signed long is required) 
long DBreadNumberOfReferencesInList(string * entityNodeName, long idInstance, int connectionType)	//OLD: GIAEntityNode * entityNodeName
{
	string referencesFileName = DBgenerateFileName(entityNodeName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);		

	#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS
 	FILE * pFile = NULL;
	const char * fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"rb");	//pFile = fopen(fileNameCharStar,"rb");
	fseek(pFile, 0, SEEK_END);	//seek to the end of the file
	long fileSize = ftell(pFile);	//get the file size in bytes
	
	cout << "fileSize = " << fileSize << endl;
	long numberOfReferences = fileSize/GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE;
	cout << "numberOfReferences = " << numberOfReferences << endl;
	
	#else
	cout << "error: readNumberOfReferencesInList() has only been coded with GIA_DATABASE_SAVE_WITH_LEADING_ZEROS" << endl;
	#endif
	return 	numberOfReferences;
}


void setUseDatabase(bool value)
{
	useDatabase = value;
}
bool getUseDatabase()
{
	return useDatabase;
}

	
#endif



