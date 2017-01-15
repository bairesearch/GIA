/*******************************************************************************
 *
 * File Name: GIAdatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l1e 23-May-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/
 
#include "GIAdatabase.h"

#define CHAR_NEW_LINE '\n'

#ifdef GIA_USE_DATABASE

unordered_map<string, bool> *DBconceptEntityNodesLoadedList;
bool useDatabase;

GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
{
	if(useDatabase)
	{
		GIAEntityNode * entityNodeFound = DBfindOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, entityNodeName, found, index, addIfNonexistant, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList, saveNetwork);
	}
	else
	{
		GIAEntityNode * entityNodeFound = LocalFindOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, entityNodeName, found, index, addIfNonexistant, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList, saveNetwork);
	}
	return entityNodeFound;
}

GIAEntityNode * DBfindOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
{
	GIAEntityNode * entityNodeFound = NULL;
	
	unordered_map<string, bool> ::iterator conceptEntityNodesLoadedListIterator;
	conceptEntityNodesLoadedListIterator = conceptEntityNodesList->find(*entityNodeName);
	
	if(conceptEntityNodesLoadedListIterator != conceptEntityNodesLoadedListIterator->end())
	{//concept entity found	
	
		bool conceptEntityNodeLoaded = false;
		if(*conceptEntityNodesLoadedListIterator) 
		{
			conceptEntityNodeLoaded = true;
		}
		
		GIAEntityNode * conceptEntityNode;
		if(conceptEntityNodeLoaded)
		{
			unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListIterator;
			conceptEntityNodesListIterator = conceptEntityNodesList->find(*entityNodeName);		
			conceptEntityNode = conceptEntityNodesListIterator->second;
		}
		else
		{
			//load the concept node from the database into RAM
			conceptEntityNode = new GIAEntityNode();
			DBreadConceptEntityNode(entityNodeName, conceptEntityNode);
			conceptEntityNodesList->insert(pair<string, GIAEntityNode*>(*entityNodeName, conceptEntityNode));
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

			newEntityNode = new GIAEntityNode();
			newEntityNode->isConcept = true;	//added 10 May 2012
			newEntityNode->idActiveList = *currentEntityNodeIDInCompleteList;
			newEntityNode->idActiveEntityTypeList = *currentEntityNodeIDInConceptEntityNodesList;
			#ifdef GIA_USE_DATABASE
			newEntityNode->added = true;
			#endif
			
			if(saveNetwork)
			{
				entityNodesCompleteList->push_back(newEntityNode);							//not used for database...
			}
			(*currentEntityNodeIDInCompleteList) = (*currentEntityNodeIDInCompleteList) + 1;				//used to indicate that this new node may need to be updated on the database

			conceptEntityNodesList->insert(pair<string, GIAEntityNode*>(*entityNodeName, newEntityNode));
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
GIAEntityNode * LocalFindOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
#else
GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)
#endif
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
			entityNodeFound->idActiveList = *currentEntityNodeIDInCompleteList;
			entityNodeFound->idActiveEntityTypeList = *currentEntityNodeIDInConceptEntityNodesList;
			#ifdef GIA_USE_DATABASE
			entityNodeFound->added = true;
			#endif
			
			if(saveNetwork)
			{
				entityNodesCompleteList->push_back(entityNodeFound);
			}
			(*currentEntityNodeIDInCompleteList) = (*currentEntityNodeIDInCompleteList) + 1;

			//conceptEntityNodesList[entityNodeName] = entityNodeFound;
			conceptEntityNodesList->insert(pair<string, GIAEntityNode*>(*entityNodeName, entityNodeFound));
			(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;

			entityNodeFound->entityName = *entityNodeName;	
		}
	}

	return entityNodeFound;
}


GIAEntityNode * findEntityNodeByID(long EntityNodeID, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	GIAEntityNode * foundEntityNode = NULL;
	foundEntityNode = entityNodesCompleteList->at(EntityNodeID);
	
	/*
	GIAEntityNode * currentEntityNode = entityNodesCompleteList;
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


string DBgenerateServerName(string * entityName);
{
	char entityFirstCharacter = entityName[0];
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
string DBgenerateFileName(string * entityName, long idInstance, int connectionType, bool vectorOrBasic, int fileType);
{
	//eg network/server/GIAdatabase/e/x/a/example/1/2/3/instance123000000/{vectorConnectionsReferencesConnectionTypeX}.txt	//OLD: //eg network/server/GIAdatabase/e/x/a/example/1/2/3/{vectorConnectionsReferencesConnectionTypeX}/instance123000000.txt
	string serverName = generateServerName(entityName);
	string databaseName = GIA_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	string fileName = serverName + databaseName;
	
	if(fileType >= GIA_DATABASE_GENERATE_FILENAME_FILE_CONCEPT_ENTITY_NODES_LIST)
	{
		fileName = fileName + GIA_DATABASE_CONCEPT_ENTITY_NODES_FILE_NAME;
	}
	else
	{
		if(entityName.length() < GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)
		{
			cout << "DBgenerateFileName error: (entityName.length() < GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)" << endl;
		}
		for(int level=0; level<GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS; level++)
		{
			fileName = fileName + entityName[level] + "/";
		}
		fileName = fileName + entityName + "/";

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
			if(connectionTypeIsVector)
			{
				fileName = fileName + entityVectorConnectionNameArray[connectionType];
			}
			else
			{//connectionTypeIsBasic
				fileName = fileName + entityBasicConnectionNameArray[connectionType];
			}
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

	

//assumes entityVectorConnections list is not already populated (ie, not already intialised with null pointers / GIA entity nodes)	
void DBreadVectorConnections(string * entityName, long idInstance, int connectionType, vector<string> *entityVectorConnectionsName, vector<long> *entityVectorConnectionsID, vector<bool> *entityVectorConnectionsLoaded, vector<GIAEntityNode*> *entityVectorConnections)
{
	vector<long*>::iterator entityVectorConnectionsNamesIterator = entityVectorConnectionsName->begin();
	vector<bool>::iterator entityVectorConnectionsIDIterator = entityVectorConnectionsID->begin();
	for(entityVectorConnectionsNamesIterator = entityVectorConnectionsName->begin(); entityVectorConnectionsNamesIterator != entityVectorConnectionsName->end(); entityVectorConnectionsNamesIterator++) 
	{
		string entityFileName = DBgenerateFileName(*entityVectorConnectionsNamesIterator, *entityVectorConnectionsIDIterator, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR, GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY);
		
		GIAEntityNode * entity = new GIAEntityNode();
		DBreadEntityFile(entityFileName, entity);
		entityVectorConnections->push_back(entity);		
		
		entityVectorConnectionsIDIterator++;
	}
	
}

//assumes entityBasicConnections array is not already intialised with GIA entity nodes	
void DBreadBasicConnections(string * entityName, long idInstance, int connectionType, string *entityBasicConnectionsName, long *entityBasicConnectionsID, bool *entityBasicConnectionLoaded, GIAEntityNode** entityBasicConnection)
{
	string entityFileName = DBgenerateFileName(entityBasicConnectionsName, *entityBasicConnectionsID, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_BASIC, GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY);
	
	GIAEntityNode * entity = new GIAEntityNode();
	DBreadEntityFile(connectionsReferencesFileName, entity);
	
	*entityBasicConnection = entity;
}

//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBreadVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<string> *entityVectorConnectionsName, vector<long> *entityVectorConnectionsID)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	//now read file
	

 	FILE * pFile = NULL;
	const char * fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBreadVectorConnectionsReferences() error: referencesFileName, " << referencesFileName << " does not exist" << endl;
	}
	else
	{
		#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
		cout << "error: GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION not yet coded to take advantage of fixed length file columns" << endl;
		#else	
		while (!feof(file))
		{
			char connectionEntityName[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH]; 
			long connectionInstanceID; 
			fscanf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, &connectionEntityName, &connectionInstanceID); 		
			entityVectorConnectionsName->push_back(string(connectionEntityName));
			entityVectorConnectionsID->push_back(connectionInstanceID);
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
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	const char * fileNameCharStar = referencesFileName.c_str();
		
	//determine position in file to write...
	int positionOfReference = GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE*referenceIndex;
	
	//now write file
	char connectionEntityName[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH]; 
	long connectionInstanceID; 	
	
 	FILE * pFile = NULL;
	const char * fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"r");
	fseek(pFile, positionOfReference, SEEK_SET);	
	fscanf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
	
	*entityVectorConnectionsName = connectionEntityName;
	*entityVectorConnectionsID = connectionInstanceID;
	
	fclose(pFile);
		
}



//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBreadBasicConnectionsReferences(string * entityName, long idInstance, int connectionType, string *entityBasicConnectionsName, long *entityBasicConnectionsID)
{
	/*
	Format:
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_BASIC, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	//now read file
	

 	FILE * pFile = NULL;
	const char * fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBreadVectorConnectionsReferences() error: referencesFileName, " << referencesFileName << " does not exist" << endl;
	}
	else
	{
		#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
		cout << "error: GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION not yet coded to take advantage of fixed length file columns" << endl;
		#else	
		
		char connectionEntityName[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH]; 
		long connectionInstanceID; 
		fscanf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, &connectionEntityName, &connectionInstanceID); 		
		*entityBasicConnectionsName = connectionEntityName;
		*entityBasicConnectionsID = connectionInstanceID;
		#endif
		
		fclose(pFile);
	}
}



//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void DBwriteVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<string> *entityVectorConnectionsName, vector<long> *entityVectorConnectionsID)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	//now write file
	

 	FILE * pFile = NULL;
	const char * fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBreadEntityNodeFile() error: entityFileName, " << *entityFileName << " does not exist" << endl;
	}
	else
	{
		#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
		cout << "error: GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION not yet coded to take advantage of fixed length file columns" << endl;
		#else	
		
		vector<bool>::iterator entityVectorConnectionsIDIterator = entityVectorConnectionsID->begin();
		for(entityVectorConnectionsNamesIterator = entityVectorConnectionsName->begin(); entityVectorConnectionsNamesIterator != entityVectorConnectionsName->end(); entityVectorConnectionsNamesIterator++) 
		{
			char * connectionEntityName = const_cast<char*>((*entityVectorConnectionsNamesIterator).c_str()); 
			long connectionInstanceID = *entityVectorConnectionsIDIterator; 		
			fprintf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
			entityVectorConnectionsIDIterator++;
		}
		#endif
		
		fclose(pFile);
	}
}

void DBmodifyVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long *entityVectorConnectionsID, long referenceIndex)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance <--- modify
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	const char * fileNameCharStar = referencesFileName.c_str();
		
	//determine position in file to write...
	int positionOfReference = GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE*referenceIndex;
	
	//now write file
	char * connectionEntityName = const_cast<char*>(entityVectorConnectionsName->c_str()); 
	long connectionInstanceID = *entityVectorConnectionsID; 	
		
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

void DBappendVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long *entityVectorConnectionsID)
{
	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance 
	entityName,idInstance <--- append
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	const char * fileNameCharStar = referencesFileName.c_str();
	
	//now write file
	char * connectionEntityName = const_cast<char*>(entityVectorConnectionsName->c_str()); 
	long connectionInstanceID = *entityVectorConnectionsID; 	
		
	FILE* pFile = fopen(fileNameCharStar, "a");
	fprintf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
	/*
	char reference[GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE];
	printf(reference, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 		
	fwrite(reference, sizeof(char), GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE, pFile);
	*/
	
	fclose(pFile);	
}



void DBwriteBasicConnectionsReferences(string * entityName, long idInstance, int connectionType, string *entityBasicConnectionsName, long *entityBasicConnectionsID)
{
	/*
	Format:
	entityName,idInstance
	*/
	
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_BASIC, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	//now write file
	

 	FILE * pFile = NULL;
	const char * fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBreadEntityNodeFile() error: entityFileName, " << *entityFileName << " does not exist" << endl;
	}
	else
	{
		#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
		cout << "error: GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION not yet coded to take advantage of fixed length file columns" << endl;
		#else	
		
		char * connectionEntityName = const_cast<char*>(entityBasicConnectionsName->c_str()); 
		long connectionInstanceID = *entityBasicConnectionsID; 		
		fprintf(pFile, GIA_DATABASE_REFRENCES_FILE_FORMAT, connectionEntityName, connectionInstanceID); 	
		#endif
		
		fclose(pFile);
	}
}




void DBreadConceptEntityNode(string * entityName, GIAEntityNode * conceptEntityNode)
{
	int idInstance = GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE;
	DBreadEntityNode(entityNodeName, idInstance, conceptEntityNode);
}

void DBreadEntityNode(string * entityName, long idInstance, GIAEntityNode * entityNode)
{
	string entityFileName = DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY);
	DBreadEntityNodeFile(entityFileName, entityNode);
	
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		entityNode->timeConditionNode = new GIATimeConditionNode();
		string timeConditionFileName = DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_CONNECTION_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE);
		DBreadTimeConditionNodeFile(timeConditionFileName, entityNode->timeConditionNode);
	}
	
	DBsetEntityConnectionsLoaded(false);
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
		fscanf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT, &(entity->idActiveList), &(entity->entityName), &(entity->confidence), &(int(entity->isConcept)), &(int(entity->isProperty)), &(int(entity->isAction)), &(int(entity->isCondition)), &(int(entity->hasAssociatedInstance)), &(int(entity->hasAssociatedInstanceIsAction)), &(int(entity->hasAssociatedPropertyIsCondition)), &(int(entity->hasAssociatedTime)), &(int(entity->hasQuality)), &(int(entity->disabled)), &(entity->conditionType), &(entity->grammaticalNumber), &(int(entity->hasQuantity)), &(int(entity->hasMeasure))); 
		fclose(pFile);
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
		cout << "DBreadEntityNodeFile() error: entityFileName, " << *entityFileName << " does not exist" << endl;
	}
	else
	{
		fprintf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT, entity->idActiveList, entity->entityName, entity->confidence, int(entity->isConcept), int(entity->isProperty), int(entity->isAction), int(entity->isCondition), int(entity->hasAssociatedInstance), int(entity->hasAssociatedInstanceIsAction), int(entity->hasAssociatedPropertyIsCondition), int(entity->hasAssociatedTime), int(entity->hasQuality), int(entity->disabled), entity->conditionType, entity->grammaticalNumber, int(entity->hasQuantity), int(entity->hasMeasure)); 
		fclose(pFile);
	}
	
	/*
	ofstream writeFileObject(entityFileName->c_str());

	writeFileObject.put(CHAR_NEW_LINE);

	writeFileObject.close();
	*/
	
	return result;
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
		fscanf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT, &(timeCondition->conditionName), &(timeCondition->tense), &(timeCondition->second), &(timeCondition->hour), &(timeCondition->dayOfWeek), &(timeCondition->month), &(timeCondition->dayOfMonth), &(timeCondition->year), &(timeCondition->period), &(timeCondition->totalTimeInSeconds), &(int(timeCondition->tenseOnlyTimeCondition)), &(int(timeCondition->isProgressive)));	//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o) 
		fclose(pFile);
	}
  
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
		cout << "DBreadTimeConditionNodeFile() error: timeConditionFileName, " << *timeConditionFileName << " does not exist" << endl;
	}
	else
	{
		fprintf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT, (timeCondition->conditionName), (timeCondition->tense), (timeCondition->second), (timeCondition->hour), (timeCondition->dayOfWeek), (timeCondition->month), (timeCondition->dayOfMonth), (timeCondition->year), (timeCondition->period), (timeCondition->totalTimeInSeconds), (int(timeCondition->tenseOnlyTimeCondition)), (int(timeCondition->isProgressive)));	//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o) 
		fclose(pFile);
	}
	
	return result;
}


	
void DBreadReferencesFile(string * referencesFileName, GIAEntityNode* entity)
{
	//read file into c struct
	
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
}

void DBsetEntityConnectionsLoaded(GIAEntityNode * entityNode, bool loaded)
{
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		entityNode->entityVectorConnectionsReferenceListLoadedArray = loaded;
	}
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_BASIC_CONNECTION_TYPES; i++)
	{
		entityNode->entityVectorBasicReferenceListLoadedArray = loaded;
	}
}

long DBreadConceptEntityNumberOfInstances(GIAEntityNode * entityNodeName)
{
	int idInstance = GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE;
	int connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES;
	bool vectorOrBasic = GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR;
	long numberOfReferences = DBreadNumberOfReferencesInList(entityNodeName, idInstance, connectionType);
}

//given GIA_DATABASE_INSTANCE_ID_MAX_ORDER (9), this implies there are up to than 9.999 billion instances, and as such a 32 bit signed int is insufficient [max +2147483648 ~= 2 billion, so a 64bit signed long is required) 
long DBreadNumberOfReferencesInList(GIAEntityNode * entityNodeName, long idInstance, int connectionType, bool vectorOrBasic)
{
	string referencesFileName = DBgenerateFileName(entityName, idInstance, connectionType, vectorOrBasic, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);		

	#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS
 	FILE * pFile = NULL;
	const char * fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"rb");	//pFile = fopen(fileNameCharStar,"rb");
	fseek(pFile, 0, SEEK_END);	//seek to the end of the file
	long fileSize = ftell(pFile)	//get the file size in bytes
	
	cout << "fileSize = " << fileSize << endl;
	long numberOfReferences = fileSize/GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE;
	cout << "numberOfReferences = " << numberOfReferences << endl;
	
	#else
	cout << "error: readNumberOfReferencesInList() has only been coded with GIA_DATABASE_SAVE_WITH_LEADING_ZEROS" << endl;
	#endif
	return 	numberOfReferences;
}

void DBreadConceptEntityNodesLoadedList()	//unordered_map<string, bool> *DBconceptEntityNodesLoadedListLocal
{
	string conceptEntityNodesListFileName = DBgenerateFileName(NULL, NULL, NULL, NULL, GIA_DATABASE_GENERATE_FILENAME_FILE_CONCEPT_ENTITY_NODES_LIST);

	ifstream parseFileObject(conceptEntityNodesListFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//file does not exist in current directory.
		cout << "Error: GIA Concept Entity Nodes List File does not exist in current directory: " << conceptEntityNodesListFileName << endl;
		result = false;
	}
	else
	{
		char currentToken;
		char conceptEntityNameCharStar[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		string conceptEntityName = conceptEntityNameCharStar;
		while(parseFileObject.getline(conceptEntityNameCharStar, GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH))
		{
			DBconceptEntityNodesLoadedList->insert(pair<string, bool>(conceptEntityName, false));			
		}
	}
}

void DBwriteConceptEntityNodesLoadedList(string * conceptEntityNodesListFileName)	//unordered_map<string, bool> *DBconceptEntityNodesLoadedList 
{
	string conceptEntityNodesListFileName = DBgenerateFileName(NULL, NULL, NULL, NULL, GIA_DATABASE_GENERATE_FILENAME_FILE_CONCEPT_ENTITY_NODES_LIST);

	ofstream writeFileObject(conceptEntityNodesListFileName->c_str());

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

void setUseDatabase(bool value)
{
	useDatabase = value;
}
bool getUseDatabase()
{
	return useDatabase;
}

void writeDatabase(vector<GIAEntityNode*> *entityNodesCompleteList)
{
	for(vector<GIAEntityNode*>::iterator entityNodesCompleteListIterator = entityNodesCompleteList->begin(); entityNodesCompleteListIterator != entityNodesCompleteList->end(); entityNodesCompleteListIterator++) 
	{
		GIAEntityNode* entityNode = *entityNodesCompleteListIterator;
		
		if(entityNode->added || entityNode->modified)
		{
			//write node itself
		}
		
		if(entityNode->added)
		{
			//write all vector connections
		}
		else
		{
			//only write specific connections
			for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
			{
				for(vector<bool>::iterator entityVectorConnectionsModifiedArrayIterator = entityNode->entityVectorConnectionsModifiedArray[i]->begin(); entityVectorConnectionsModifiedArrayIterator != entityNode->entityVectorConnectionsModifiedArray[i]->end(); entityVectorConnectionsModifiedArrayIterator++) 
				{
					if(*entityVectorConnectionsModifiedArrayIterator)
					{//modified (or new), so must be written
						write
					}
				}
			}
			for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
			{
				for(vector<bool>::iterator entityVectorConnectionsModifiedArrayIterator = entityNode->entityVectorConnectionsModifiedArray[i]->begin(); entityVectorConnectionsModifiedArrayIterator != entityNode->entityVectorConnectionsModifiedArray[i]->end(); entityVectorConnectionsModifiedArrayIterator++) 
				{
					if(*entityVectorConnectionsModifiedArrayIterator)
					{//modified (or new), so must be written
						write
					}
				}
			}		
			for(int i=0; i<GIA_ENTITY_NUMBER_OF_BASIC_CONNECTION_TYPES; i++)
			{
				if(entityNode->entityBasicConnectionsModifiedArray[i])
				{

				}		
			}
		}

		
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
}

	
#endif













































/*
//uses fast search algorithm
GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList)
{
	GIAEntityNode * entityNodeFound = NULL;
	
	long vectorSize = conceptEntityNamesList->size();

	
	//cout << "debug entity" << endl;
	//vector<GIAEntityNode*>::iterator entityIter;
	//for (entityIter = conceptEntityNodesList->begin(); entityIter != conceptEntityNodesList->end(); entityIter++) 
	//{
	//	cout << "entityName = " << (*entityIter)->entityName << endl;
	//}
	
		
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
			entityNodeFound->idActiveList = *currentEntityNodeIDInCompleteList;
			entityNodeFound->idActiveEntityTypeList = *currentEntityNodeIDInConceptEntityNodesList;
			
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
			
			
			//if(*entityNodeName == "Jane")
			//{
			//	cout << "vectorSize = "  << vectorSize << endl;
			//	cout << "findIndex = " << findIndex << endl;
			//	cout << "findRange = " << findRange << endl;		      
			//	cout << "nameTemp = " << nameTemp << endl;
			//	cout << "*entityNodeName = " << *entityNodeName << endl;		      
			//}
			
			
			
			//cout << "vectorSize = "  << vectorSize << endl;
			//cout << "findIndex = " << findIndex << endl;
			//cout << "findRange = " << findRange << endl;			
			//cout << "nameTemp = " << nameTemp << endl;
			//cout << "*entityNodeName = " << *entityNodeName << endl;
			
			if(nameTemp > *entityNodeName)
			{
				
				//if(*entityNodeName == "red")
				//{
				//	cout << nameTemp << ">" << *entityNodeName << endl;
				//	cout << "previousTempName = " << previousTempName << endl;
				//}
				
								
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
				
				//if(*entityNodeName == "red")
				//{
				//	cout << nameTemp << "<" << *entityNodeName << endl;
				//	cout << "previousTempName = " << previousTempName << endl;
				//}
						
				
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
				entityNodeFound->idActiveList = *currentEntityNodeIDInCompleteList;
				entityNodeFound->idActiveEntityTypeList = *currentEntityNodeIDInConceptEntityNodesList;
			
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


	//cout << "debug entity" << endl;
	//vector<GIAEntityNode*>::iterator entityIter;
	//for (entityIter = conceptEntityNodesList->begin(); entityIter != conceptEntityNodesList->end(); entityIter++) 
	//{
	//	cout << (*entityIter)->entityName << endl;
	//}
	//
	//cout << "debug names" << endl;
	//vector<string>::iterator stringIter;
	//for (stringIter = conceptEntityNamesList->begin(); stringIter != conceptEntityNamesList->end(); stringIter++) 
	//{
	//	cout << (*stringIter) << endl;
	//}	
		
				
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

	
	//cout << "debug entity" << endl;
	//vector<GIAEntityNode*>::iterator entityIter;
	//for (entityIter = conceptEntityNodesList->begin(); entityIter != conceptEntityNodesList->end(); entityIter++) 
	//{
	//	cout << (*entityIter)->entityName << endl;
	//}
	//cout << "debug names" << endl;
	//vector<string>::iterator stringIter;
	//for (stringIter = conceptEntityNamesList->begin(); stringIter != conceptEntityNamesList->end(); stringIter++) 
	//{
	//	cout << (*stringIter) << endl;
	//}	
				
				
	return timeNodeFound;
}
#endif
*/
