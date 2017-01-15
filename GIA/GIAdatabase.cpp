/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: GIAdatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p3b 14-January-2017
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/


#include "GIAdatabase.h"

#define CHAR_NEWLINE '\n'



#ifdef GIA_DATABASE

static int useDatabase;
static string databaseFolderName;

#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
unordered_map<string, bool>* DBnetworkIndexEntityNodesLoadedList;		//load all references (ids/entity names) whenever a networkIndex node is used
#else
unordered_map<string, GIAnetworkIndexEntityLoaded*>* DBnetworkIndexEntityNodesLoadedList;		//do not load references (ids/entity names) whenever a networkIndex node is used - only load the total number of instances associated with that networkIndex node
#endif

unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBcache;	//represents data loaded into RAM from database
unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactive;	//represents data in entityNodesActiveListComplete

#endif

GIAentityNode* GIAdatabaseClass::findOrAddNetworkIndexEntityNodeByName(vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, string* entityNodeName, bool* found, const long* index, const bool addIfNonexistant, long* currentEntityNodeIDinCompleteList, long* currentEntityNodeIDinNetworkIndexEntityNodesList, const bool saveNetwork)
{
	#ifdef GIA_DATABASE
	GIAentityNode* entityNodeFound = NULL;
	if((useDatabase == GIA_DATABASE_TRUE_READ_INACTIVE) || (useDatabase == GIA_DATABASE_TRUE_READ_ACTIVE))
	{
		entityNodeFound = this->DBfindOrAddNetworkIndexEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, entityNodeName, found, index, addIfNonexistant, currentEntityNodeIDinCompleteList, currentEntityNodeIDinNetworkIndexEntityNodesList, saveNetwork);
	}
	else if(useDatabase == GIA_DATABASE_FALSE)
	{
		entityNodeFound = this->LocalFindOrAddNetworkIndexEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, entityNodeName, found, index, addIfNonexistant, currentEntityNodeIDinCompleteList, currentEntityNodeIDinNetworkIndexEntityNodesList, saveNetwork);
	}
	#else
	entityNodeFound = this->LocalFindOrAddNetworkIndexEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, entityNodeName, found, index, addIfNonexistant, currentEntityNodeIDinCompleteList, currentEntityNodeIDinNetworkIndexEntityNodesList, saveNetwork);
	#endif
	return entityNodeFound;
}

#ifdef GIA_DATABASE
GIAentityNode* GIAdatabaseClass::DBfindOrAddNetworkIndexEntityNodeByName(vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, string* entityNodeName, bool* found, const long* index, const bool addIfNonexistant, long* currentEntityNodeIDinCompleteList, long* currentEntityNodeIDinNetworkIndexEntityNodesList, const bool saveNetwork)
{
	GIAentityNode* entityNodeFound = NULL;

	#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	unordered_map<string, bool>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->find(*entityNodeName);
	#else
	unordered_map<string, GIAnetworkIndexEntityLoaded*>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->find(*entityNodeName);
	#endif

	if(networkIndexEntityNodesLoadedListIterator != DBnetworkIndexEntityNodesLoadedList->end())
	{//networkIndex entity found

		#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
		bool networkIndexEntityNodeLoaded = networkIndexEntityNodesLoadedListIterator->second;
		#else
		GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = networkIndexEntityNodesLoadedListIterator->second;
		bool networkIndexEntityNodeLoaded = networkIndexEntityLoaded->loaded;
		#endif

		GIAentityNode* networkIndexEntityNode;
		if(networkIndexEntityNodeLoaded)
		{
			unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIterator = entityNodesActiveListNetworkIndexes->find(*entityNodeName);
			networkIndexEntityNode = networkIndexEntityNodesListIterator->second;
			#ifdef GIA_DATABASE_DEBUG
			cout << "1. DBfindOrAddNetworkIndexEntityNodeByName: networkIndexEntityNodeLoaded" << endl;
			cout << "   networkIndexEntityNode->entityName = " << networkIndexEntityNode->entityName << endl;
			#endif
		}
		else
		{
			#ifdef GIA_DATABASE_DEBUG
			cout << "2. DBfindOrAddNetworkIndexEntityNodeByName: !networkIndexEntityNodeLoaded" << endl;
			#endif
			//load the networkIndex node from the database into RAM
			networkIndexEntityNode = new GIAentityNode();
			this->DBreadNetworkIndexEntityNode(entityNodeName, networkIndexEntityNode);
			entityNodesActiveListNetworkIndexes->insert(pair<string, GIAentityNode*>(*entityNodeName, networkIndexEntityNode));
			this->addEntityNodesActiveListCompleteFastIndexDBcache(networkIndexEntityNode);	//added 2 Nov 2012	//CHECK THIS

			#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
			networkIndexEntityNodesLoadedListIterator->second = true;
			#else
			networkIndexEntityLoaded->loaded = true;	//added 18 June 2012
			networkIndexEntityNode->networkIndexEntityLoaded = networkIndexEntityLoaded;	//for numberOfInstances
			#endif
			#ifdef GIA_DATABASE_DEBUG
			cout << "   networkIndexEntityNode->entityName = " << networkIndexEntityNode->entityName << endl;
			#endif
		}

		#ifdef GIA_DATABASE_DEBUG
		cout << "\tentity node found; " <<* entityNodeName << endl;
		#endif
		entityNodeFound = networkIndexEntityNode;
		*found = true;
	}
	else
	{//networkIndex entity not found - add it to the map

		if(addIfNonexistant)
		{
			#ifdef GIA_DATABASE_DEBUG
			cout << "adding networkIndex entity node; " <<* entityNodeName << endl;
			#endif
			#ifdef GIA_DATABASE_DEBUG
			cout << "3. DBfindOrAddNetworkIndexEntityNodeByName: " << endl;
			cout << "  * entityNodeName = " <<* entityNodeName << endl;
			#endif

			GIAentityNode* newEntityNode = new GIAentityNode();
			newEntityNode->entityName = *entityNodeName;
			newEntityNode->idInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE;
			newEntityNode->entityType = GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX;	//added 10 May 2012
			newEntityNode->idActiveList = *currentEntityNodeIDinCompleteList;
			#ifdef GIA_DATABASE
			newEntityNode->added = true;
			#endif

			if(saveNetwork)
			{
				entityNodesActiveListComplete->push_back(newEntityNode);						//?not used for database...
				this->addEntityNodesActiveListCompleteFastIndexDBactive(newEntityNode);	//added 2 Nov 2012
			}
			(*currentEntityNodeIDinCompleteList) = (*currentEntityNodeIDinCompleteList) + 1;				//?used to indicate that this new node may need to be updated on the database

			entityNodesActiveListNetworkIndexes->insert(pair<string, GIAentityNode*>(*entityNodeName, newEntityNode));
			(*currentEntityNodeIDinNetworkIndexEntityNodesList) = (*currentEntityNodeIDinNetworkIndexEntityNodesList) + 1;		//?not used for database...

			#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
			bool networkIndexEntityNodeLoaded = true;
			DBnetworkIndexEntityNodesLoadedList->insert(pair<string, bool>(*entityNodeName, networkIndexEntityNodeLoaded));
			#else
			GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = new GIAnetworkIndexEntityLoaded();
			networkIndexEntityLoaded->loaded = true;
			networkIndexEntityLoaded->numberOfInstances = 0;
			DBnetworkIndexEntityNodesLoadedList->insert(pair<string, GIAnetworkIndexEntityLoaded*>(*entityNodeName, networkIndexEntityLoaded));
			newEntityNode->networkIndexEntityLoaded = networkIndexEntityLoaded;
			#endif
			newEntityNode->added = true;											//used to indicate that this new node needs to be added to the database (at the end of the routine, after it is ensured that it has not been disabled)

			/*
			bool networkIndexEntityNodeAdded = true;
			networkIndexEntityNodesModifiedList->insert(pair<string, bool>(*entityNodeName, networkIndexEntityNodeAdded));
			*/

			entityNodeFound = newEntityNode;
		}
	}

	return entityNodeFound;
}
#endif


//uses fast search algorithm
GIAentityNode* GIAdatabaseClass::LocalFindOrAddNetworkIndexEntityNodeByName(vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, string* entityNodeName, bool* found, const long* index, const bool addIfNonexistant, long* currentEntityNodeIDinCompleteList, long* currentEntityNodeIDinNetworkIndexEntityNodesList, const bool saveNetwork)
{
	GIAentityNode* entityNodeFound = NULL;

	unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIterator = entityNodesActiveListNetworkIndexes->find(*entityNodeName);

	if(networkIndexEntityNodesListIterator != entityNodesActiveListNetworkIndexes->end())
	{//networkIndex entity found
		#ifdef GIA_DATABASE_DEBUG
		cout << "\tnetworkIndex entity node found; " <<* entityNodeName << endl;
		#endif
		entityNodeFound = networkIndexEntityNodesListIterator->second;
		*found = true;
	}
	else
	{//networkIndex entity not found - add it to the map

		if(addIfNonexistant)
		{
			#ifdef GIA_DATABASE_DEBUG
			cout << "adding networkIndex entity node; " <<* entityNodeName << endl;
			#endif

			GIAentityNode* newEntityNode = new GIAentityNode();
			newEntityNode->entityName = *entityNodeName;
			newEntityNode->idInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE;
			newEntityNode->entityType = GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX;	//added 10 May 2012
			newEntityNode->idActiveList = *currentEntityNodeIDinCompleteList;
			#ifdef GIA_DATABASE
			newEntityNode->added = true;
			#endif

			if(saveNetwork)
			{
				entityNodesActiveListComplete->push_back(newEntityNode);
				//addEntityNodesActiveListCompleteFastIndexDBactive(newEntityNode);	//added 2 Nov 2012
			}
			(*currentEntityNodeIDinCompleteList) = (*currentEntityNodeIDinCompleteList) + 1;

			//entityNodesActiveListNetworkIndexes[entityNodeName] = newEntityNode;
			entityNodesActiveListNetworkIndexes->insert(pair<string, GIAentityNode*>(*entityNodeName, newEntityNode));
			(*currentEntityNodeIDinNetworkIndexEntityNodesList) = (*currentEntityNodeIDinNetworkIndexEntityNodesList) + 1;

			#ifdef GIA_DATABASE
			#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
			GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = new GIAnetworkIndexEntityLoaded();
			networkIndexEntityLoaded->loaded = true;
			networkIndexEntityLoaded->numberOfInstances = 0;
			newEntityNode->networkIndexEntityLoaded = networkIndexEntityLoaded;
			#endif
			#endif

			entityNodeFound = newEntityNode;
		}
	}

	return entityNodeFound;
}


GIAentityNode* GIAdatabaseClass::findActiveEntityNodeByID(const long EntityNodeID, const vector<GIAentityNode*>* entityNodesActiveListComplete)
{
	GIAentityNode* foundEntityNode = NULL;
	foundEntityNode = entityNodesActiveListComplete->at(EntityNodeID);

	/*
	GIAentityNode* currentEntityNode = entityNodesActiveListComplete;
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



long GIAdatabaseClass::maximumLong(long a, const long b)
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










#ifdef GIA_DATABASE

bool GIAdatabaseClass::DBdirectoryExists(string* folderName)
{
	bool folderExists = SHAREDvars.directoryExists(folderName);
	if(folderExists)
	{
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "\tdirectoryExists: folderName = " <<* folderName << endl;
		#endif
	}

	return folderExists;
}

bool GIAdatabaseClass::DBcreateDirectory(string* folderName)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "\tDBcreateDirectory: folderName = " <<* folderName << endl;
	#endif
	bool result = true;

	SHAREDvars.createDirectory(folderName);
	/*removed debug support for Windows;
	#ifndef LINUX
	if(CreateDirectory(folderName->c_str(), 0) == 0)	//if( _mkdir(folderName->c_str()) != 0)	//
	{
		result = false;
	}
	#endif
	*/

	return result;
}

bool GIAdatabaseClass::DBsetCurrentDirectory(string* folderName)
{
	bool result = true;
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "\tDBsetCurrentDirectory: folderName = " <<* folderName << endl;
	#endif
	SHAREDvars.setCurrentDirectory(folderName);
	/*removed debug support for Windows;
	#ifndef LINUX
	if(SetCurrentDirectory(folderName->c_str()) == 0)
	{
		result = false;
	}
	#endif
	*/
	return result;
}

bool GIAdatabaseClass::checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(string* folderName)
{
	bool result = true;
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "checkIfFolderExistsAndIfNotMakeAndSetAsCurrent: folderName = " <<* folderName << endl;
	#endif
	if(!this->DBdirectoryExists(folderName))
	{
		this->DBcreateDirectory(folderName);
	}
	this->DBsetCurrentDirectory(folderName);

	return result;
}


string GIAdatabaseClass::DBgenerateServerDatabaseName(const string* entityName, const int fileType, const string defaultDatabaseName, string databaseFolderNameUserChoice)
{
	string databaseName;
	if(fileType == GIA_DATABASE_GENERATE_FILENAME_FILE_NETWORK_INDEX_ENTITY_NODES_LIST)
	{
		databaseName = databaseFolderName;
	}
	else
	{
		string serverName;

		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		//cout << "entityName = " <<* entityName << endl;
		#endif

		#ifdef GIA_DATABASE_FILESYSTEM_USE_MULTIPLE_SERVERS
		char entityFirstCharacter = entityName->at(0);
		if((entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z))
		{
			cout << "determineServerName error: (entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z)" << endl;
			cout << "entityName = " <<* entityName << endl;
			exit(0);
		}

		int entityFirstCharacterIndex = entityFirstCharacter - ASCII_TABLE_INDEX_OF_a;
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		//cout << "entityFirstCharacterIndex = " << entityFirstCharacterIndex << endl;
		#endif
		string serverName = serverNameArray[entityFirstCharacterIndex]; 	//this could be a more complex algorithm; eg serverName = (string)"/mnt/" + serverNameArray[entityFirstCharacterIndex]
		databaseName = serverName + defaultDatabaseName;
		#else
		databaseName = databaseFolderNameUserChoice;
		#ifdef GIA_DEBUG
		//cout << "databaseFolderNameUserChoice = " << databaseFolderNameUserChoice << endl;
		#endif
		#endif

	}
	#ifdef GIA_DATABASE_DEBUG
	cout << "databaseName = " << databaseName << endl;
	#endif

	return databaseName;
}

//NB idInstance 0 corresponds to the networkIndex entity (null instance)
string GIAdatabaseClass::DBgenerateFileName(string* entityName, const long idInstance, const int connectionType, const int fileType)
{

	//eg network/server/GIAdatabase/e/x/a/example/1/2/3/instance123000000/{vectorConnectionsReferencesConnectionTypeX}.txt	//OLD: //eg network/server/GIAdatabase/e/x/a/example/1/2/3/{vectorConnectionsReferencesConnectionTypeX}/instance123000000.txt
	string serverName = this->DBgenerateServerDatabaseName(entityName, fileType, GIA_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME, databaseFolderName);
	string fileName = serverName;

	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "1fileName = " << fileName << endl;
	#endif
	this->DBsetCurrentDirectory(&fileName);

	if(fileType == GIA_DATABASE_GENERATE_FILENAME_FILE_NETWORK_INDEX_ENTITY_NODES_LIST)
	{
		fileName = fileName + GIA_DATABASE_NETWORK_INDEX_ENTITY_NODES_FILE_NAME;
	}
	else
	{
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		//cout << "*entityName = " <<* entityName << endl;
		#endif

		int numberOfEntityNameLevels;
		if(entityName->length() < GIA_DATABASE_NETWORK_INDEX_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)
		{
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			//cout << "DBgenerateFileName: (entityName.length() < GIA_DATABASE_NETWORK_INDEX_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)" << endl;
			//cout << "entityName = " <<* entityName << endl;
			#endif
			numberOfEntityNameLevels = entityName->length();
		}
		else
		{
			numberOfEntityNameLevels = GIA_DATABASE_NETWORK_INDEX_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS;
		}
		for(int level=0; level<numberOfEntityNameLevels; level++)
		{
			string folderName = "";
			folderName = folderName + entityName->at(level);
			fileName = fileName + folderName + "/";
			this->checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&folderName);
		}
		fileName = fileName +* entityName + "/";
		this->checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(entityName);

		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "2fileName = " << fileName << endl;
		#endif

		char idInstanceStringCharStar[GIA_DATABASE_INSTANCE_ID_MAX_ORDER+1];	//+1 for char* string null character \0
		sprintf(idInstanceStringCharStar, GIA_DATABASE_INSTANCE_ID_STRING_FORMAT, idInstance);
		string idInstanceString = idInstanceStringCharStar;

		for(int level=0; level<GIA_DATABASE_INSTANCE_ID_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS*2; level=level+2)
		{
			string folderName = "";
			char idLevelChar1 = idInstanceString.at(level);
			int levelOfChar2 = level+1;
			char idLevelChar2 = idInstanceString.at(levelOfChar2);
			folderName = folderName + idLevelChar1 + idLevelChar2;
			fileName = fileName + folderName + "/";
			this->checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&folderName);
		}
		fileName = fileName + idInstanceString + "/";
		this->checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&idInstanceString);

		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "3fileName = " << fileName << endl;
		#endif

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

		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "4fileName = " << fileName << endl;
		#endif
	}

	#ifdef GIA_DATABASE_DEBUG
	cout << "fileName = " << fileName << endl;
	#endif

	return fileName;
}




















/*
void DBreadReferencesFile(string* referencesFileName, GIAentityNode* entity)
{
	//read file into c struct

	ifstream parseFileObject(referencesFileName->c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//xml file does not exist in current directory.
		cout << "Error: GIA Entity File does not exist in current directory: " <<* referencesFileName << endl;
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






#ifdef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
void GIAdatabaseClass::DBreadDatabase(vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	#ifdef GIA_DATABASE_DEBUG
	cout << "DBreadDatabase... (GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ)" << endl;
	#endif

	this->initialiseDBentityNodesActiveListCompleteFastIndexDBcache();

	//based on code from DBwriteNetworkIndexEntityNodesLoadedList()
	#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	for(unordered_map<string, bool>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->begin(); networkIndexEntityNodesLoadedListIterator != DBnetworkIndexEntityNodesLoadedList->end(); networkIndexEntityNodesLoadedListIterator++)
	{
		string networkIndexEntityName = networkIndexEntityNodesLoadedListIterator->first;
	}
	#else
	for(unordered_map<string, GIAnetworkIndexEntityLoaded*>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->begin(); networkIndexEntityNodesLoadedListIterator != DBnetworkIndexEntityNodesLoadedList->end(); networkIndexEntityNodesLoadedListIterator++)
	{
		string networkIndexEntityName = networkIndexEntityNodesLoadedListIterator->first;
		GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = networkIndexEntityNodesLoadedListIterator->second;
		long numberOfInstances = networkIndexEntityLoaded->numberOfInstances;
	#endif
		//based on code from DBfindOrAddNetworkIndexEntityNodeByName
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "\n" << endl;
		cout << "\t\tnetworkIndexEntityName = " << networkIndexEntityName << endl;
		cout << "\t\tnetworkIndexEntityLoaded->numberOfInstances = " << networkIndexEntityLoaded->numberOfInstances << endl;
		#endif

		//check to see if entity already loaded into RAM via a different connection (in TempActiveList; ie, DB only)...
		bool entityFoundInActiveListCompleteFastIndexDBcache = false;
		GIAentityNode* entityNodeFoundDBcache = this->findEntityNodesActiveListCompleteFastIndexDBcache(&(networkIndexEntityName), GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE, &entityFoundInActiveListCompleteFastIndexDBcache);

		GIAentityNode* networkIndexEntityNode = NULL;
		if(entityFoundInActiveListCompleteFastIndexDBcache)
		{
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			cout << "\tDBreadDatabase() - networkIndex node already in RAM (cache)" << endl;
			cout << "\tentityNodeFoundDBcache->networkIndexEntityName = " << entityNodeFoundDBcache->entityName << endl;
			#endif
			networkIndexEntityNode = entityNodeFoundDBcache;
		}
		else
		{
			//load the networkIndex node from the database into RAM
			networkIndexEntityNode = new GIAentityNode();
			this->DBreadNetworkIndexEntityNode(&networkIndexEntityName, networkIndexEntityNode);
			this->addEntityNodesActiveListCompleteFastIndexDBcache(networkIndexEntityNode);
		}

		entityNodesActiveListNetworkIndexes->insert(pair<string, GIAentityNode*>(networkIndexEntityName, networkIndexEntityNode));
		entityNodesActiveListComplete->push_back(networkIndexEntityNode);

		#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
		networkIndexEntityNodesLoadedListIterator->second = true;
		#else
		networkIndexEntityLoaded->loaded = true;
		networkIndexEntityNode->networkIndexEntityLoaded = networkIndexEntityLoaded;
		#endif
		#ifdef GIA_DATABASE_DEBUG
		cout << "   networkIndexEntityNode->entityName = " << networkIndexEntityNode->entityName << endl;
		#endif

		//based on code from GIAquery()
		//now load all instances into RAM
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			//read all instances
			this->DBreadVectorConnections(networkIndexEntityNode, i);
		}

		#ifdef GIA_DEBUG
		//cout << "done reading networkIndex connections" << endl;
		#endif

		#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
		networkIndexEntityNode->wasReference = true;	//required for node to be printed
		#endif
		for(vector<GIAentityConnection*>::iterator connectionIter = (networkIndexEntityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES]).begin(); connectionIter != (networkIndexEntityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES]).end(); connectionIter++)
		{
			GIAentityNode* entityNode = (*connectionIter)->entity;

			#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
			if(!(entityNode->entityVectorConnectionsReferenceListLoadedArray[0]))	//test if entity has already had its connections loaded (and therefore has already been added to entityNodesActiveListComplete)
			{
			#endif
				entityNodesActiveListComplete->push_back(entityNode);
				#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
				entityNode->wasReference = true;	//required for node to be printed
				#endif
				for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
				{
					//read all instances
					this->DBreadVectorConnections(entityNode, i);
				}
			#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
			}
			#endif
		}
	}

	this->clearDBentityNodesActiveListCompleteFastIndexDBcache();
}
#endif




void GIAdatabaseClass::initialiseDatabase(const bool readFromDatabase, const string newDatabaseFolderName, const bool useDatabase, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	if(useDatabase)
	{
		databaseFolderName = newDatabaseFolderName;

		this->initialiseDBnetworkIndexEntityNodesLoadedList();

		this->DBreadNetworkIndexEntityNodesLoadedList();

		this->initialiseDBentityNodesActiveListCompleteFastIndexDBactive();

		#ifdef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
		this->DBreadDatabase(entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes);
		#endif

		#ifndef GIA_DATABASE_CLEAR_CACHE_EVERY_SENTENCE
		this->initialiseDBentityNodesActiveListCompleteFastIndexDBcache();
		#endif

		if(readFromDatabase)
		{
			this->setUseDatabase(GIA_DATABASE_TRUE_READ_ACTIVE);
		}
		else
		{
			this->setUseDatabase(GIA_DATABASE_TRUE_READ_INACTIVE);
		}
		/*
		this->setUseDatabase(GIA_DATABASE_TRUE_READ_INACTIVE);	//OLD: at initialisation (/by default), do not activate read (use active list instead)
		*/
	}
	else
	{
		this->setUseDatabase(GIA_DATABASE_FALSE);
	}
}

void GIAdatabaseClass::DBreadNetworkIndexEntityNodesLoadedList()	//unordered_map<string, bool>* DBnetworkIndexEntityNodesLoadedListLocal
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadNetworkIndexEntityNodesLoadedList{}" << endl;
	#endif

	string networkIndexEntityNodesListFileName = this->DBgenerateFileName(NULL, NULL, NULL, GIA_DATABASE_GENERATE_FILENAME_FILE_NETWORK_INDEX_ENTITY_NODES_LIST);

#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	ifstream parseFileObject(networkIndexEntityNodesListFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//file does not exist in current directory.
		cout << "DBreadNetworkIndexEntityNodesLoadedList{} error: GIA NetworkIndex Entity Nodes List File does not exist in current directory: " << networkIndexEntityNodesListFileName << endl;
		exit(0);
	}
	else
	{
		char currentToken;
		char networkIndexEntityNameCharStar[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		while(parseFileObject.getline(networkIndexEntityNameCharStar, GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH))
		{
			string networkIndexEntityName = networkIndexEntityNameCharStar;
			DBnetworkIndexEntityNodesLoadedList->insert(pair<string, bool>(networkIndexEntityName, false));
		}
	}
#else
  	FILE* pFile = fopen(networkIndexEntityNodesListFileName.c_str(),"r");
	if(pFile == NULL)
	{
		cout << "DBreadNetworkIndexEntityNodesLoadedList{} error: GIA NetworkIndex Entity Nodes List File does not exist in current directory: " << networkIndexEntityNodesListFileName << endl;
	}
	else
	{
		int networkIndexEntityIndex = 0;
		while(!feof(pFile))
		{
			char networkIndexEntityNameCharStar[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
			long networkIndexNumberOfInstances;
			int result = fscanf(pFile, GIA_DATABASE_REFERENCES_FILE_FORMAT_READ, networkIndexEntityNameCharStar, &networkIndexNumberOfInstances);
			string networkIndexEntityName = networkIndexEntityNameCharStar;
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			cout << "networkIndexEntityIndex = " << networkIndexEntityIndex << endl;
			cout << "result = " << result << endl;
			cout << "networkIndexEntityNameCharStar = " << networkIndexEntityNameCharStar << endl;
			cout << "networkIndexNumberOfInstances = " << networkIndexNumberOfInstances << endl;
			#endif
			if(result > 0)	//&& (result != EOF)
			{
				#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
				//cout << "networkIndexEntityName = " << networkIndexEntityName << endl;
				#endif
				GIAnetworkIndexEntityLoaded* newNetworkIndexEntityLoaded = new GIAnetworkIndexEntityLoaded();
				newNetworkIndexEntityLoaded->loaded = false;
				newNetworkIndexEntityLoaded->numberOfInstances = networkIndexNumberOfInstances;

				DBnetworkIndexEntityNodesLoadedList->insert(pair<string, GIAnetworkIndexEntityLoaded*>(networkIndexEntityName, newNetworkIndexEntityLoaded));
			}
			networkIndexEntityIndex++;
		}

		fclose(pFile);
	}
#endif

	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	this->DBprintNetworkIndexEntityNodesLoadedList("DBprintNetworkIndexEntityNodesLoadedList{}");
	#endif
}






void GIAdatabaseClass::DBreadVectorConnections(GIAentityNode* entityNode, int connectionType)
{
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	if(!(entityNode->disabled))	//Added 17 August 2012 (is this required? - entities should not have been written to database in first place with GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES)
	{
	#endif
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "\tDBreadVectorConnections{}: entity = " << entityNode->entityName << ", connectionType = " << connectionType << endl;
		#endif

		if(!(entityNode->entityVectorConnectionsReferenceListLoadedArray[connectionType]))
		{
			this->DBreadVectorConnectionsReferences(&(entityNode->entityName), entityNode->idInstance, connectionType, &(entityNode->entityVectorConnectionsArray[connectionType]));
			entityNode->entityVectorConnectionsReferenceListLoadedArray[connectionType] = true;
		}
		else
		{
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			cout << "!DBreadVectorConnectionsReferences (already loaded)" << endl;
			#endif
		}

		this->DBreadVectorConnectionEntities(&(entityNode->entityName), entityNode->idInstance, connectionType, &(entityNode->entityVectorConnectionsArray[connectionType]));
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	}
	#endif
}

//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
//assume vectorConnectionSubstances has not been populated
void GIAdatabaseClass::DBreadVectorConnectionsReferences(string* entityName, const long idInstance, const int connectionType, vector<GIAentityConnection*>* entityVectorConnections)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadVectorConnectionsReferences{}: entityName = " <<* entityName << ", connectionType = " << connectionType << endl;
	#endif

	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	*/

	string referencesFileName = this->DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadVectorConnectionsReferences{}: referencesFileName = " << referencesFileName << endl;
	#endif
	//now read file


 	FILE* pFile = NULL;
	const char* fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile == NULL)
	{
		cout << "DBreadVectorConnectionsReferences{} error: referencesFileName, " << referencesFileName << " does not exist" << endl;
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
			int result = fscanf(pFile, GIA_DATABASE_REFERENCES_FILE_FORMAT_READ, connectionEntityName, &connectionInstanceID);
			if(result > 0)	//&& (result != EOF)
			{
				GIAentityConnection* newConnection = new GIAentityConnection();
				newConnection->entityName = connectionEntityName;
				newConnection->idInstance = connectionInstanceID;
				newConnection->referenceLoaded = true;
				entityVectorConnections->push_back(newConnection);
				#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
				cout << "DBreadVectorConnectionsReferences{}: newConnection->entityName = " << newConnection->entityName << endl;
				cout << "DBreadVectorConnectionsReferences{}: newConnection->idInstance = " << newConnection->idInstance << endl;
				cout << "DBreadVectorConnectionsReferences{}: newConnection->referenceLoaded = " << newConnection->referenceLoaded << endl;
				#endif
			}
		}
		#endif

		fclose(pFile);
	}

}

void GIAdatabaseClass::DBreadVectorConnectionsReference(string* entityName, const long idInstance, const int connectionType, string* entityVectorConnectionsName, long* entityVectorConnectionsID, const long referenceIndex)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadVectorConnectionsReference{}: entityName = " <<* entityName << endl;
	#endif

	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance <-- read this
	entityName,idInstance
	*/

	string referencesFileName = this->DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);

	//determine position in file to write...
	int positionOfReference = GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE*referenceIndex;

	//now write file
	char connectionEntityName[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
	long connectionInstanceID;

 	FILE* pFile = NULL;
	const char* fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"r");
	fseek(pFile, positionOfReference, SEEK_SET);
	int result = fscanf(pFile, GIA_DATABASE_REFERENCES_FILE_FORMAT_READ, connectionEntityName, connectionInstanceID);
	if(result > 0)	//&& (result != EOF)
	{
		*entityVectorConnectionsName = connectionEntityName;
		*entityVectorConnectionsID = connectionInstanceID;
	}
	else
	{
		cout << "DBreadVectorConnectionsReference{} error: cannot read location, referenceIndex = " << referenceIndex << endl;
		cout << "entityName = " <<* entityName << endl;
		cout << "idInstance = " << idInstance << endl;
	}

	fclose(pFile);

}


//assumes entityVectorConnections list is not already populated (ie, not already intialised with null pointers / GIA entity nodes)
void GIAdatabaseClass::DBreadVectorConnectionEntities(string* entityName, long idInstance, const int connectionType, vector<GIAentityConnection*>* entityVectorConnections)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadVectorConnectionEntities{}: entityName = " <<* entityName << ", idInstance = " << idInstance << ", connectionType = " << connectionType << endl;
	#endif

	for(vector<GIAentityConnection*>::iterator connectionIter = entityVectorConnections->begin(); connectionIter != entityVectorConnections->end(); connectionIter++)
	{
		GIAentityConnection* connection = *connectionIter;

		if(!(connection->loaded))
		{
			//check to see if entity already loaded into RAM via a different connection (in activeList)...
			bool entityFoundInActiveListCompleteFastIndexDBactive = false;
			GIAentityNode* entityNodeFoundDBactive = this->findEntityNodesActiveListCompleteFastIndexDBactive(&(connection->entityName), connection->idInstance, &entityFoundInActiveListCompleteFastIndexDBactive);

			//check to see if entity already loaded into RAM via a different connection (in TempActiveList; ie, DB only)...
			bool entityFoundInActiveListCompleteFastIndexDBcache = false;
			GIAentityNode* entityNodeFoundDBcache = this->findEntityNodesActiveListCompleteFastIndexDBcache(&(connection->entityName), connection->idInstance, &entityFoundInActiveListCompleteFastIndexDBcache);

			bool alreadyInRAM = false;
			if(entityFoundInActiveListCompleteFastIndexDBactive)
			{
				#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
				cout << "!DBreadVectorConnectionEntities{} - connection already in RAM (active)" << endl;
				cout << "connection->entityName = " << connection->entityName << endl;
				cout << "connection->idInstance = " << connection->idInstance << endl;
				#endif
				connection->entity = entityNodeFoundDBactive;
				alreadyInRAM = true;
			}
			else if(entityFoundInActiveListCompleteFastIndexDBcache)
			{
				#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
				cout << "!DBreadVectorConnectionEntities{} - connection already in RAM (cache)" << endl;
				cout << "connection->entityName = " << connection->entityName << endl;
				cout << "connection->idInstance = " << connection->idInstance << endl;
				#endif
				connection->entity = entityNodeFoundDBcache;
				alreadyInRAM = true;
			}
			else
			{
				#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
				cout << "!DBreadVectorConnectionEntities{} - reading entity from DB" << endl;
				cout << "connection->entityName = " << connection->entityName << endl;
				cout << "connection->idInstance = " << connection->idInstance << endl;
				#endif
				GIAentityNode* entity = new GIAentityNode();
				this->DBreadEntityNode(&(connection->entityName), connection->idInstance, entity);
				connection->entity = entity;

				this->addEntityNodesActiveListCompleteFastIndexDBcache(entity);
			}

			connection->loaded = true;
			connection->modified = false;
			connection->added = false;
		}
		else
		{
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			cout << "!DBreadVectorConnectionEntities{} - connection already loaded" << endl;
			cout << "connection->entityName = " << connection->entityName << endl;
			cout << "connection->idInstance = " << connection->idInstance << endl;
			#endif
		}
	}
}


void GIAdatabaseClass::DBreadNetworkIndexEntityNode(string* entityName, GIAentityNode* networkIndexEntityNode)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadNetworkIndexEntityNode{}" << endl;
	#endif

	int idInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE;
	this->DBreadEntityNode(entityName, idInstance, networkIndexEntityNode);

	#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	//added 18 June 2012 - now networkIndex entity nodes have their reference names/ids loaded upon first access in DB (this is required such that new idInstances can be assigned correctly)
		//will this scale for ~1000000 instances per networkIndex node? [1 connection = 64bits/8bytes{id} + 64bytes{name} per networkIndex node = ~72bytes = 72MB per networkIndex node]. Perhaps not... - may need to devise an alternate system, where by total number of instances (and therefore max instance id) per networkIndex are recorded and loaded upon openDatabase
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{//is this required for all connection types? or perhaps it is only required for associated instances GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES?
		if(!(networkIndexEntityNode->entityVectorConnectionsReferenceListLoadedArray[i]))
		{
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			cout << "networkIndexEntityNode->idInstance = " << networkIndexEntityNode->idInstance << endl;
			#endif
			this->DBreadVectorConnectionsReferences(&(networkIndexEntityNode->entityName), networkIndexEntityNode->idInstance, i, &(networkIndexEntityNode->entityVectorConnectionsArray[i]));
			networkIndexEntityNode->entityVectorConnectionsReferenceListLoadedArray[i] = true;
		}
	}
	#endif
}




void GIAdatabaseClass::DBreadEntityNode(string* entityName, long idInstance, GIAentityNode* entityNode)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadEntityNode{}: entityName = " <<* entityName << ", idInstance = " << idInstance << endl;
	#endif

	string entityFileName = this->DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY);
	this->DBreadEntityNodeFile(&entityFileName, entityNode);
	entityNode->idInstance = idInstance;

	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		entityNode->timeConditionNode = new GIAtimeConditionNode();
		string timeConditionFileName = this->DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE);
		this->DBreadTimeConditionNodeFile(&timeConditionFileName, entityNode->timeConditionNode);
	}

	GIAentityNodeClass.DBsetEntityConnectionsReferenceListsLoaded(entityNode, false);
	//do not read references
}


//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void GIAdatabaseClass::DBreadEntityNodeFile(string* entityFileName, GIAentityNode* entity)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadEntityNodeFile{}: entityFileName = " <<* entityFileName << ", entity = " << entity->entityName << endl;
	#endif

	/*
	Format:
	idActiveList,entityName,wordOrig,confidence,entityType,hasAssociatedInstance,hasAssociatedTime,disabled,conditionType,grammaticalNumber,hasQuantity,quantityNumber,quantityNumberString,quantityModifier,quantityModifierString,hasQuantityMultiplier,hasMeasure,measureType
	//format derived from GIA XML file
	*/

	//read file into c struct directly?
	//use fscanf instead? (http://bytes.com/topic/c/answers/882529-read-text-file-into-formatted-struct-array)

 	FILE* pFile = NULL;
	const char* fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile == NULL)
	{
		cout << "DBreadEntityNodeFile{} error: entityFileName, " <<* entityFileName << " does not exist" << endl;
	}
	else
	{
		//these need to be converted back to booleans

		char entityNameCharStarTemp[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		char wordOrigCharStarTemp[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		char aliasesCharStarTemp[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		int entityType;
		int hasAssociatedInstance;
		int hasAssociatedTime;
		int disabled;
		//int conditionType;
		//int grammaticalNumber;
		int hasQuantity;
		char quantityNumberStringCharStarTemp[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		char quantityModifierStringCharStarTemp[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];
		int hasQuantityMultiplier;
		int hasMeasure;

		int result = fscanf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT_READ, &(entity->idActiveList), entityNameCharStarTemp, wordOrigCharStarTemp, aliasesCharStarTemp, &(entity->confidence), &entityType, &hasAssociatedInstance, &hasAssociatedTime, &disabled, &(entity->conditionType), &(entity->grammaticalNumber), &hasQuantity, &(entity->quantityNumber), quantityNumberStringCharStarTemp, &(entity->quantityModifier), quantityModifierStringCharStarTemp, &hasQuantityMultiplier, &hasMeasure, &(entity->measureType));
		if(result > 0)	//&& (result != EOF)
		{
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			/*
			cout << "\tDBreadEntityNodeFile{}: entityType = " << entityType << endl;
			cout << "\tDBreadEntityNodeFile{}: hasAssociatedInstance = " << hasAssociatedInstance << endl;
			cout << "\tDBreadEntityNodeFile{}: hasAssociatedTime = " << hasAssociatedTime << endl;
			cout << "\tDBreadEntityNodeFile{}: disabled = " << disabled << endl;
			cout << "\tDBreadEntityNodeFile{}: hasQuantity = " << hasQuantity << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->hasMeasure = " << hasMeasure << endl;
			*/
			#endif

			entity->entityName = entityNameCharStarTemp;
			#ifdef GIA_WORD_ORIG
			entity->wordOrig = this->DBreplaceBlankString(string(wordOrigCharStarTemp));
			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			//cout << "entityNameCharStarTemp = " << entityNameCharStarTemp << endl;
			//cout << "wordOrigCharStarTemp = " << wordOrigCharStarTemp << endl;
			#endif
			#endif
			#ifdef GIA_ALIASES
			string aliasesString = this->DBreplaceBlankString(string(aliasesCharStarTemp));
			GIAentityNodeClass.convertAliasesStringToAliases(entity, aliasesString);
			#endif
			entity->entityType = entityType;
			entity->hasAssociatedInstance = bool(hasAssociatedInstance);
			entity->hasAssociatedTime = bool(hasAssociatedTime);
			entity->disabled = bool(disabled);
			//entity->conditionType = conditionType;
			//entity->grammaticalNumber = grammaticalNumber;
			entity->hasQuantity = bool(hasQuantity);
			entity->quantityNumberString = this->DBreplaceBlankString(string(quantityNumberStringCharStarTemp));
			entity->quantityModifierString = this->DBreplaceBlankString(string(quantityModifierStringCharStarTemp));
			entity->hasQuantityMultiplier = bool(hasQuantityMultiplier);
			entity->hasMeasure = bool(hasMeasure);

			#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
			/*
			cout << "\tDBreadEntityNodeFile{}: entity->idActiveList = " << entity->idActiveList << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->entityName = " << entity->entityName << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->confidence = " << entity->confidence << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->entityType = " << entity->entityType << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->hasAssociatedInstance = " << int(entity->hasAssociatedInstance) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->hasAssociatedTime = " << int(entity->hasAssociatedTime) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->disabled = " << int(entity->disabled) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->conditionType = " << entity->conditionType << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->grammaticalNumber = " << entity->grammaticalNumber << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->hasQuantity = " << int(entity->hasQuantity) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->quantityNumber = " << int(entity->quantityNumber) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->quantityNumberString = " << entity->quantityNumberString) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->quantityModifier = " << entity->quantityModifier << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->quantityModifierString = " << entity->quantityModifierString << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->hasQuantity = " << int(entity->hasQuantity) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->hasQuantityMultiplier = " << int(entity->hasQuantityMultiplier) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->hasMeasure = " << int(entity->hasMeasure) << endl;
			cout << "\tDBreadEntityNodeFile{}: entity->measureType = " << entity->measureType << endl;
			*/
			#endif
		}
		else
		{
			cout << "DBreadEntityNodeFile{} error: cannot read entity" << endl;
			cout << "entityFileName = " <<* entityFileName << endl;
		}

		fclose(pFile);


	}

	/*
	ifstream parseFileObject(entityFileName->c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//xml file does not exist in current directory.
		cout << "Error: GIA Entity File does not exist in current directory: " <<* entityFileName << endl;
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

	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadEntityNodeFile{}: entityFileName = " <<* entityFileName << ", entity = " << entity->entityName << endl;
	#endif
}

//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void GIAdatabaseClass::DBreadTimeConditionNodeFile(string* timeConditionFileName, GIAtimeConditionNode* timeCondition)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadTimeConditionNodeFile{}: timeConditionFileName = " <<* timeConditionFileName << endl;		//UNTESTED AS YET
	#endif

	/*
	Format:
	conditionName,tense,second,hour,dayOfWeek,month,dayOfMonth,year,period,totalTimeInSeconds,tenseOnlyTimeCondition,isProgressive
	//format derived from GIA XML file
	*/

 	FILE* pFile = NULL;
	const char* fileNameCharStar = timeConditionFileName->c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile == NULL)
	{
		cout << "DBreadTimeConditionNodeFile{} error: timeConditionFileName, " <<* timeConditionFileName << " does not exist" << endl;
	}
	else
	{
		int tenseOnlyTimeCondition;
		int isProgressive;

		char conditionNameCharStarTemp[GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH];

		int result = fscanf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT_READ, &conditionNameCharStarTemp, &(timeCondition->tense), &(timeCondition->second), &(timeCondition->hour), &(timeCondition->dayOfWeek), &(timeCondition->month), &(timeCondition->dayOfMonth), &(timeCondition->year), &(timeCondition->period), &(timeCondition->totalTimeInSeconds), &tenseOnlyTimeCondition, &isProgressive);							//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o)
		//fscanf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT_READ, &conditionNameCharStarTemp, &(timeCondition->tense), &(timeCondition->second), &(timeCondition->hour), &(timeCondition->dayOfWeek), &(timeCondition->month), &(timeCondition->dayOfMonth), &(timeCondition->year), &(timeCondition->period), &(timeCondition->totalTimeInSeconds), &(int(timeCondition->tenseOnlyTimeCondition)), &(int(timeCondition->isProgressive)));	//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o)

		if(result > 0)	//&& (result != EOF)
		{
			timeCondition->conditionName = conditionNameCharStarTemp;

			timeCondition->tenseOnlyTimeCondition = bool(tenseOnlyTimeCondition);
			timeCondition->isProgressive = bool(isProgressive);
		}
		else
		{
			cout << "DBreadTimeConditionNodeFile{} error: cannot read time condition node" << endl;
			cout << "entityFileName = " <<* timeConditionFileName << endl;
		}


		fclose(pFile);


	}

}





void GIAdatabaseClass::writeAndCloseDatabase(vector<GIAentityNode*>* entityNodesActiveListComplete)
{
	#ifdef GIA_DATABASE_DEBUG
	cout << "writeAndCloseDatabase{}" << endl;
	#endif

	this->writeDatabase(entityNodesActiveListComplete);

	this->DBwriteNetworkIndexEntityNodesLoadedList();

	#ifdef GIA_DATABASE_DEBUG
	//cout << "writeAndCloseDatabase() finish" << endl;
	#endif
}

void GIAdatabaseClass::writeDatabase(vector<GIAentityNode*>* entityNodesActiveListComplete)
{
	#ifdef GIA_DATABASE_DEBUG
	cout << "writeDatabase{}" << endl;
	#endif

	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesActiveListComplete->begin(); entityNodesActiveCompleteListIterator != entityNodesActiveListComplete->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* entityNode = *entityNodesActiveCompleteListIterator;

		#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		if(!(entityNode->disabled))	//added 17 August 2012
		{
		#endif
			if(entityNode->added || entityNode->modified)
			{
				//(re-)write node itself
				this->DBwriteEntityNode(&(entityNode->entityName), entityNode->idInstance, entityNode);
			}

			#ifdef GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES
			if(!(entityNode->disabled))	//added 29 November 2012
			{
			#endif
				//only write specific connections
				for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
				{
					if(entityNode->entityVectorConnectionsReferenceListLoadedArray[i])
					{//they should all be loaded

						if(entityNode->added || entityNode->entityVectorConnectionsRemovedArray[i])
						{
							#ifdef GIA_DATABASE_DEBUG
							//cout << "DBwriteVectorConnectionsReferences" << endl;
							#endif
							//write all vector connection references
							this->DBwriteVectorConnectionsReferences(&(entityNode->entityName), entityNode->idInstance, i, &(entityNode->entityVectorConnectionsArray[i]));
						}
						else
						{
							int referenceIndex = 0;
							for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
							{
								GIAentityConnection* connection = *connectionIter;
								#ifdef GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES
								if(!(connection->entity->disabled))
								{
								#endif
									if(connection->modified)
									{//modified; overwrite vector connection reference
										#ifdef GIA_DATABASE_DEBUG
										//cout << "DBmodifyVectorConnectionsReference" << endl;
										#endif
										this->DBmodifyVectorConnectionsReference(&(entityNode->entityName), entityNode->idInstance, i, &(connection->entityName), connection->idInstance, referenceIndex);
									}
									else if(connection->added)
									{//added; append vector connection reference
										#ifdef GIA_DATABASE_DEBUG
										//cout << "DBappendVectorConnectionsReference" << endl;
										#endif
										this->DBappendVectorConnectionsReference(&(entityNode->entityName), entityNode->idInstance, i, &(connection->entityName), connection->idInstance);
									}
								#ifdef GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES
								}
								#endif
								referenceIndex++;
							}
						}
					}
					else
					{
						//this may be the case when joining reference sets - so do not throw error
						#ifdef GIA_DATABASE_DEBUG
						/*
						cout << "writeDatabase{}: entityVectorConnectionsReferenceListLoadedArray[i] != true" << endl;
						cout << "entityNode = " << entityNode->entityName << endl;
						*/
						#endif
					}
				}
			#ifdef GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES
			}
			#endif
		#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		}
		#endif
	}

	#ifdef GIA_DATABASE_DEBUG
	//cout << "writeDatabase() finish" << endl;
	#endif

	/* OLD: this is done in DBwriteNetworkIndexEntityNodesLoadedList()
	string networkIndexEntityName = networkIndexEntityNodesLoadedListIterator->first;
	const char* networkIndexEntityNameCharStar = networkIndexEntityName.c_str();
	writeFileObject.write(networkIndexEntityNameCharStar, networkIndexEntityName.length());
	//for(int i=0; i<networkIndexEntityName.length(); i++)
	//{
	//	writeFileObject.put(networkIndexEntityName[i]);
	//}
	writeFileObject.put(CHAR_NEWLINE);
	*/

}



void GIAdatabaseClass::DBwriteEntityNode(string* entityName, const long idInstance, GIAentityNode* entityNode)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBwriteEntityNode{}: entityName = " <<* entityName << endl;
	#endif
	string entityFileName = this->DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY);
	this->DBwriteEntityNodeFile(&entityFileName, entityNode);

	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		string timeConditionFileName = this->DBgenerateFileName(entityName, idInstance, GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT, GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE);
		this->DBwriteTimeConditionNodeFile(&timeConditionFileName, entityNode->timeConditionNode);
	}
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	//cout << "DBwriteEntityNode finish" << endl;
	#endif
}


//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void GIAdatabaseClass::DBwriteEntityNodeFile(string* entityFileName, GIAentityNode* entity)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBwriteEntityNodeFile{}: entityFileName = " <<* entityFileName << ", entity = " << entity->entityName << endl;
	#endif
	/*
	Format:
	idActiveList,entityName,confidence,entityType,hasAssociatedInstance,hasAssociatedTime,disabled,conditionType,grammaticalNumber,hasQuantity,quantityNumber,quantityNumberString,quantityModifier,quantityModifierString,hasQuantityMultiplier,hasMeasure,measureType
	//format derived from GIA XML file
	*/

 	FILE* pFile = NULL;
	const char* fileNameCharStar = entityFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBwriteEntityNodeFile{} error: entityFileName, " <<* entityFileName << " does not exist" << endl;
	}
	else
	{
		string wordOrig = "";
		wordOrig = this->DBaddBlankString(entity->wordOrig);
		string aliasesString = "";
		#ifdef GIA_ALIASES
		GIAentityNodeClass.convertAliasesToAliasesString(entity, &aliasesString);
		#endif
		aliasesString = this->DBaddBlankString(aliasesString);
		string quantityNumberString = "";
		quantityNumberString = this->DBaddBlankString(entity->quantityNumberString);
		string quantityModifierString = "";
		quantityModifierString = this->DBaddBlankString(entity->quantityModifierString);
		string entityName = (entity->entityName);

		if(entityName.length() > GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH-1)
		{
			cout << "error: entityName.length() > GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH-1" << endl;
			entityName = entityName.substr(0, GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH-1);
		}
		if(wordOrig.length() > GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH-1)
		{
			cout << "error: wordOrig.length() > GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH-1" << endl;
			wordOrig = wordOrig.substr(0, GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH-1);
		}
		if(aliasesString.length() > GIA_DATABASE_ENTITY_NODE_ALIASES_STRING_MAX_LENGTH-1)
		{
			cout << "error: aliasesString.length() > GIA_DATABASE_ENTITY_NODE_ALIASES_STRING_MAX_LENGTH-1" << endl;
			aliasesString = aliasesString.substr(0, GIA_DATABASE_ENTITY_NODE_ALIASES_STRING_MAX_LENGTH-1);
		}
		if(quantityNumberString.length() > GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_STRING_MAX_LENGTH-1)
		{
			cout << "error: quantityNumberString.length() > GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_STRING_MAX_LENGTH-1" << endl;
			quantityNumberString = quantityNumberString.substr(0, GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_STRING_MAX_LENGTH-1);
		}
		if(quantityModifierString.length() > GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_STRING_MAX_LENGTH-1)
		{
			cout << "error: quantityModifierString.length() > GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_STRING_MAX_LENGTH-1" << endl;
			quantityModifierString = quantityModifierString.substr(0, GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_STRING_MAX_LENGTH-1);
		}

		fprintf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT_WRITE, entity->idActiveList, entityName.c_str(), wordOrig.c_str(), aliasesString.c_str(), entity->confidence, entity->entityType, int(entity->hasAssociatedInstance), int(entity->hasAssociatedTime), int(entity->disabled), entity->conditionType, entity->grammaticalNumber, int(entity->hasQuantity), entity->quantityNumber, quantityNumberString.c_str(), entity->quantityModifier, quantityModifierString.c_str(), int(entity->hasQuantityMultiplier), int(entity->hasMeasure), entity->measureType);
		//fprintf(pFile, GIA_DATABASE_ENTITY_NODE_FILE_FORMAT_WRITE, entity->idActiveList, (entity->entityName).c_str());
		fclose(pFile);
	}
}

//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void GIAdatabaseClass::DBwriteTimeConditionNodeFile(string* timeConditionFileName, const GIAtimeConditionNode* timeCondition)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBwriteTimeConditionNodeFile{}: timeConditionFileName = " <<* timeConditionFileName << endl;
	#endif

	/*
	Format:
	conditionName,tense,second,hour,dayOfWeek,month,dayOfMonth,year,period,totalTimeInSeconds,tenseOnlyTimeCondition,isProgressive
	//format derived from GIA XML file
	*/

 	FILE* pFile = NULL;
	const char* fileNameCharStar = timeConditionFileName->c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBwriteTimeConditionNodeFile{} error: timeConditionFileName, " <<* timeConditionFileName << " does not exist" << endl;
	}
	else
	{
		fprintf(pFile, GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT_WRITE, (timeCondition->conditionName).c_str(), (timeCondition->tense), (timeCondition->second), (timeCondition->hour), (timeCondition->dayOfWeek), (timeCondition->month), (timeCondition->dayOfMonth), (timeCondition->year), (timeCondition->period), (timeCondition->totalTimeInSeconds), (int(timeCondition->tenseOnlyTimeCondition)), (int(timeCondition->isProgressive)));	//tenseOnlyTimeCondition + isProgressive are only in DB at the moment (not yet in XML i/o)
		fclose(pFile);
	}
}




//this could be made more efficient, as it is known each row is of fixed column width (assuming entity names can be padded)
void GIAdatabaseClass::DBwriteVectorConnectionsReferences(string* entityName, const long idInstance, const int connectionType, vector<GIAentityConnection*>* entityVectorConnections)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBwriteVectorConnectionsReferences{}: entityName = " <<* entityName << endl;
	#endif

	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	*/

	string referencesFileName = this->DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	//now write file


 	FILE* pFile = NULL;
	const char* fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"w");
	if(pFile == NULL)
	{
		cout << "DBwriteVectorConnectionsReferences{} error: referencesFileName, " << referencesFileName << " could not be created" << endl;
	}
	else
	{
		#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
		cout << "error: GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION not yet coded to take advantage of fixed length file columns" << endl;
		#else

		for(vector<GIAentityConnection*>::iterator connectionIter = entityVectorConnections->begin(); connectionIter != entityVectorConnections->end(); connectionIter++)
		{
			GIAentityConnection* connection = *connectionIter;

			#ifdef GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES
			if(!(connection->entity->disabled))
			{
			#endif
				#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
				cout << "\n" << endl;
				cout << "\t\tconnectionType = " << connectionType << endl;
				cout << "\t\tconnection->entityName = " << connection->entityName << endl;
				cout << "\t\tconnection->idInstance = " << connection->idInstance << endl;
				#endif

				char* connectionEntityName = const_cast<char*>(connection->entityName.c_str()); 	//NB NOT connection->entity->entityName (as DBwriteVectorConnectionsReferences() does not assume connection->loaded == true)
				long connectionInstanceID = connection->idInstance; 					//NB NOT connection->entity->idInstance (as DBwriteVectorConnectionsReferences() does not assume connection->loaded == true)
				fprintf(pFile, GIA_DATABASE_REFERENCES_FILE_FORMAT_WRITE, connectionEntityName, connectionInstanceID);
			#ifdef GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES
			}
			#endif
		}
		#endif

		fclose(pFile);
	}
}

void GIAdatabaseClass::DBmodifyVectorConnectionsReference(string* entityName, const long idInstance, const int connectionType, const string* entityVectorConnectionsName, const long entityVectorConnectionsID, const long referenceIndex)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBmodifyVectorConnectionsReference{}: entityName = " <<* entityName << endl;
	#endif

	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance <--- modify
	entityName,idInstance
	*/

	string referencesFileName = this->DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	const char* fileNameCharStar = referencesFileName.c_str();

	//determine position in file to write...
	int positionOfReference = GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE*referenceIndex;

	//now write file
	char* connectionEntityName = const_cast<char*>(entityVectorConnectionsName->c_str());
	long connectionInstanceID = entityVectorConnectionsID;

	FILE* pFile = fopen(fileNameCharStar, "r+b"); // r+ if you need char mode
	if(pFile == NULL)
	{
		cout << "DBmodifyVectorConnectionsReference{} error: referencesFileName, " << referencesFileName << " could not be created" << endl;
	}
	else
	{
		fseek(pFile, positionOfReference, SEEK_SET);
		fprintf(pFile, GIA_DATABASE_REFERENCES_FILE_FORMAT_WRITE, connectionEntityName, connectionInstanceID);
		/*
		char reference[GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE];
		printf(reference, GIA_DATABASE_REFERENCES_FILE_FORMAT_WRITE, connectionEntityName, connectionInstanceID);
		fwrite(reference, sizeof(char), GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE, pFile);
		*/

		fclose(pFile);
	}
}

void GIAdatabaseClass::DBappendVectorConnectionsReference(string* entityName, const long idInstance, const int connectionType, const string* entityVectorConnectionsName, const long entityVectorConnectionsID)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBappendVectorConnectionsReference{}: entityName = " <<* entityName << endl;
	#endif

	/*
	Format:
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance
	entityName,idInstance <--- append
	*/

	string referencesFileName = this->DBgenerateFileName(entityName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);
	const char* fileNameCharStar = referencesFileName.c_str();

	//now write file
	char* connectionEntityName = const_cast<char*>(entityVectorConnectionsName->c_str());
	long connectionInstanceID = entityVectorConnectionsID;

	FILE* pFile = fopen(fileNameCharStar, "a");
	if(pFile == NULL)
	{
		cout << "DBappendVectorConnectionsReference{} error: referencesFileName, " << referencesFileName << " could not be created" << endl;
	}
	else
	{
		fprintf(pFile, GIA_DATABASE_REFERENCES_FILE_FORMAT_WRITE, connectionEntityName, connectionInstanceID);
		/*
		char reference[GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE];
		printf(reference, GIA_DATABASE_REFERENCES_FILE_FORMAT_WRITE, connectionEntityName, connectionInstanceID);
		fwrite(reference, sizeof(char), GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE, pFile);
		*/

		fclose(pFile);
	}
}


void GIAdatabaseClass::DBwriteNetworkIndexEntityNodesLoadedList()	//unordered_map<string, bool>* DBnetworkIndexEntityNodesLoadedList
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBwriteNetworkIndexEntityNodesLoadedList{}" << endl;
	#endif

	string networkIndexEntityNodesListFileName = this->DBgenerateFileName(NULL, NULL, NULL, GIA_DATABASE_GENERATE_FILENAME_FILE_NETWORK_INDEX_ENTITY_NODES_LIST);
	#ifdef GIA_DEBUG
	//cout << "networkIndexEntityNodesListFileName = " << networkIndexEntityNodesListFileName << endl;
	#endif

#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS

	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	cout << "error: GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS cannot be used with GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES" << endl;
	exit(0);
	#endif

	ofstream writeFileObject(networkIndexEntityNodesListFileName.c_str());
	for(unordered_map<string, bool>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->begin(); networkIndexEntityNodesLoadedListIterator != DBnetworkIndexEntityNodesLoadedList->end(); networkIndexEntityNodesLoadedListIterator++)
	{
		string networkIndexEntityName = networkIndexEntityNodesLoadedListIterator->first;
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		//cout << "networkIndexEntityName = " << networkIndexEntityName << endl;
		#endif
		const char* networkIndexEntityNameCharStar = networkIndexEntityName.c_str();
		writeFileObject.write(networkIndexEntityNameCharStar, networkIndexEntityName.length());
		/*
		for(int i=0; i<networkIndexEntityName.length(); i++)
		{
			writeFileObject.put(networkIndexEntityName[i]);
		}
		*/
		writeFileObject.put(CHAR_NEWLINE);
	}
	writeFileObject.close();
#else
  	FILE* pFile = fopen(networkIndexEntityNodesListFileName.c_str(),"w");
	if(pFile == NULL)
	{
		cout << "DBwriteNetworkIndexEntityNodesLoadedList{} error: networkIndexEntityNodesListFileName, " << networkIndexEntityNodesListFileName << " could not be created" << endl;
	}
	else
	{
		for(unordered_map<string, GIAnetworkIndexEntityLoaded*>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->begin(); networkIndexEntityNodesLoadedListIterator != DBnetworkIndexEntityNodesLoadedList->end(); networkIndexEntityNodesLoadedListIterator++)
		{
			string networkIndexEntityName = networkIndexEntityNodesLoadedListIterator->first;
			GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = networkIndexEntityNodesLoadedListIterator->second;

			#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
			if(!(networkIndexEntityLoaded->disabled))
			{
			#endif

				#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
				cout << "\n" << endl;
				cout << "\t\tnetworkIndexEntityName = " << networkIndexEntityName << endl;
				cout << "\t\tnetworkIndexEntityLoaded->numberOfInstances = " << networkIndexEntityLoaded->numberOfInstances << endl;
				#endif

				char* connectionEntityName = const_cast<char*>(networkIndexEntityName.c_str());
				long numberOfInstances = networkIndexEntityLoaded->numberOfInstances;
				fprintf(pFile, GIA_DATABASE_REFERENCES_FILE_FORMAT_WRITE, connectionEntityName, numberOfInstances);
			#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
			}
			#endif
		}
		fclose(pFile);
	}
#endif
}

void GIAdatabaseClass::closeDatabase()
{
	//no action
}








void GIAdatabaseClass::DBprintNetworkIndexEntityNodesLoadedList(const string executionStage)
{
	cout << "DBprintNetworkIndexEntityNodesLoadedList{}:" << executionStage << endl;

#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	for(unordered_map<string, bool>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->begin(); networkIndexEntityNodesLoadedListIterator != DBnetworkIndexEntityNodesLoadedList->end(); networkIndexEntityNodesLoadedListIterator++)
	{
		string networkIndexEntityName = networkIndexEntityNodesLoadedListIterator->first;
		bool networkIndexEntityNodeLoaded = networkIndexEntityNodesLoadedListIterator->second;
		cout << "\n" << endl;
		cout << "\t\tnetworkIndexEntityName = " << networkIndexEntityName << endl;
		cout << "\t\tnetworkIndexEntityNodeLoaded = " << int(networkIndexEntityNodeLoaded) << endl;
	}
#else
	for(unordered_map<string, GIAnetworkIndexEntityLoaded*>::iterator networkIndexEntityNodesLoadedListIterator = DBnetworkIndexEntityNodesLoadedList->begin(); networkIndexEntityNodesLoadedListIterator != DBnetworkIndexEntityNodesLoadedList->end(); networkIndexEntityNodesLoadedListIterator++)
	{
		string networkIndexEntityName = networkIndexEntityNodesLoadedListIterator->first;
		GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded = networkIndexEntityNodesLoadedListIterator->second;
		cout << "\n" << endl;
		cout << "\t\tnetworkIndexEntityName = " << networkIndexEntityName << endl;
		cout << "\t\tnetworkIndexEntityLoaded->loaded = " << networkIndexEntityLoaded->loaded << endl;
		cout << "\t\tnetworkIndexEntityLoaded->numberOfInstances = " << networkIndexEntityLoaded->numberOfInstances << endl;
	}
#endif
}


long GIAdatabaseClass::DBreadNetworkIndexEntityNumberOfInstances(string* entityNodeName)		//OLD: GIAentityNode* entityNodeName
{
	int idInstance = GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE;
	int connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES;
	long numberOfReferences = this->DBreadNumberOfReferencesInList(entityNodeName, idInstance, connectionType);
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "DBreadNetworkIndexEntityNumberOfInstances{}: numberOfReferences = " << numberOfReferences << endl;
	#endif
	return numberOfReferences;
}

//given GIA_DATABASE_INSTANCE_ID_MAX_ORDER (9), this implies there are up to than 9.999 billion instances, and as such a 32 bit signed int is insufficient [max +2147483648 ~= 2 billion, so a 64bit signed long is required)
long GIAdatabaseClass::DBreadNumberOfReferencesInList(string* entityNodeName, const long idInstance, const int connectionType)	//OLD: GIAentityNode* entityNodeName
{
	string referencesFileName = this->DBgenerateFileName(entityNodeName, idInstance, connectionType, GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES);

	#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS
 	FILE* pFile = NULL;
	const char* fileNameCharStar = referencesFileName.c_str();
  	pFile = fopen(fileNameCharStar,"rb");	//pFile = fopen(fileNameCharStar,"rb");
	fseek(pFile, 0, SEEK_END);	//seek to the end of the file
	long fileSize = ftell(pFile);	//get the file size in bytes

	long numberOfReferences = fileSize/GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE;

	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "fileSize = " << fileSize << endl;
	cout << "numberOfReferences = " << numberOfReferences << endl;
	#endif

	#else
	cout << "error: readNumberOfReferencesInList() has only been coded with GIA_DATABASE_SAVE_WITH_LEADING_ZEROS" << endl;
	#endif
	return 	numberOfReferences;
}


void GIAdatabaseClass::initialiseDBnetworkIndexEntityNodesLoadedList()
{
	#ifdef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	DBnetworkIndexEntityNodesLoadedList = new unordered_map<string, bool>;
	#else
	DBnetworkIndexEntityNodesLoadedList = new unordered_map<string, GIAnetworkIndexEntityLoaded*>;
	#endif
}

void GIAdatabaseClass::setUseDatabase(const int value)
{
	useDatabase = value;
}
int GIAdatabaseClass::getUseDatabase()
{
	return useDatabase;
}

GIAentityNode* GIAdatabaseClass::findEntityInActiveNetworkIndexList(string* entityName, const long idInstance, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, bool* alreadyInRAM)
{
	*alreadyInRAM = false;
	GIAentityNode* entityNodeFoundInRAM = NULL;
	unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIterator = entityNodesActiveListNetworkIndexes->find(*entityName);
	if(networkIndexEntityNodesListIterator != entityNodesActiveListNetworkIndexes->end())
	{//networkIndex entity found
		#ifdef GIA_DATABASE_DEBUG
		cout << "\tfindEntityInActiveNetworkIndexList() networkIndex node found; " <<* entityName << endl;
		#endif
		GIAentityNode* entityNode = networkIndexEntityNodesListIterator->second;
		if(idInstance == GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE)
		{
			*alreadyInRAM = true;
			entityNodeFoundInRAM = entityNode;
		}
		else
		{
			if(entityNode->entityVectorConnectionsReferenceListLoadedArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES])
			{
				vector<GIAentityConnection*>* entityVectorConnections = &(entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES]);
				for(vector<GIAentityConnection*>::iterator connectionIter = entityVectorConnections->begin(); connectionIter != entityVectorConnections->end(); connectionIter++)
				{
					GIAentityConnection* currentConnection = *connectionIter;
					if(currentConnection->loaded)
					{
						GIAentityNode* currentInstance = currentConnection->entity;
						if(currentConnection->idInstance == idInstance)		//OR currentInstance->idInstance
						{
							#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
							cout << "findEntityInActiveNetworkIndexList{}: found alreadyInRAM" << endl;
							cout << "currentInstance->entityName = " << currentInstance->entityName << endl;
							cout << "currentInstance->idInstance = " << currentInstance->idInstance << endl;
							#endif

							*alreadyInRAM = true;
							entityNodeFoundInRAM = currentInstance;
						}
					}
				}
			}
		}
	}
	return entityNodeFoundInRAM;
}

GIAentityNode* GIAdatabaseClass::findEntityNodesActiveListCompleteFastIndexDBcache(const string* entityName, const long idInstance, bool* foundNode)
{
	return this->findEntityNodesActiveListCompleteFastIndex(entityName, idInstance, foundNode, entityNodesActiveListCompleteFastIndexDBcache);
}

GIAentityNode* GIAdatabaseClass::findEntityNodesActiveListCompleteFastIndexDBactive(const string* entityName, const long idInstance, bool* foundNode)
{
	return this->findEntityNodesActiveListCompleteFastIndex(entityName, idInstance, foundNode, entityNodesActiveListCompleteFastIndexDBactive);
}

GIAentityNode* GIAdatabaseClass::findEntityNodesActiveListCompleteFastIndex(const string* entityName, const long idInstance, bool* foundNode, const unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndex)
{
	*foundNode = false;
	GIAentityNode* foundEntityNode = NULL;
	string entityNodesTempActiveListCompleteIndex = this->createEntityNodesActiveListCompleteFastIndexIndex(entityName, idInstance);

	unordered_map<string, GIAentityNode*>::iterator entityNodesTempActiveListCompleteIterator = entityNodesActiveListCompleteFastIndexDBcache->find(entityNodesTempActiveListCompleteIndex);

	if(entityNodesTempActiveListCompleteIterator != entityNodesActiveListCompleteFastIndexDBcache->end())
	{//entity found
		*foundNode = true;
		foundEntityNode = entityNodesTempActiveListCompleteIterator->second;
	}

	return foundEntityNode;
}

string GIAdatabaseClass::createEntityNodesActiveListCompleteFastIndexIndex(const string* entityName, const long idInstance)
{

	string idInstanceString = SHAREDvars.convertLongToString(idInstance);
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "idInstanceString = " << idInstanceString << endl;
	#endif
	string entityNodesTempActiveListCompleteIndex = "";
	entityNodesTempActiveListCompleteIndex = entityNodesTempActiveListCompleteIndex +* entityName + idInstanceString;

	#ifdef GIA_DATABASE_DEBUG
	//cout << "*entityName = " <<* entityName << endl;
	//cout << "idInstance = " << idInstance << endl;
	//cout << "entityNodesTempActiveListCompleteIndex = " << entityNodesTempActiveListCompleteIndex << endl;
	#endif

	return entityNodesTempActiveListCompleteIndex;
}

void GIAdatabaseClass::addEntityNodesActiveListCompleteFastIndexDBcache(GIAentityNode* entityNode)
{
	this->addEntityNodesActiveListCompleteFastIndex(entityNode, entityNodesActiveListCompleteFastIndexDBcache);
}

void GIAdatabaseClass::addEntityNodesActiveListCompleteFastIndexDBactive(GIAentityNode* entityNode)
{
	this->addEntityNodesActiveListCompleteFastIndex(entityNode, entityNodesActiveListCompleteFastIndexDBactive);
}

void GIAdatabaseClass::addEntityNodesActiveListCompleteFastIndex(GIAentityNode* entityNode, unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndex)
{
	string entityName = entityNode->entityName;
	long idInstance = entityNode->idInstance;
	string entityNodesTempActiveListCompleteIndex = this->createEntityNodesActiveListCompleteFastIndexIndex(&entityName, idInstance);

	entityNodesActiveListCompleteFastIndex->insert(pair<string, GIAentityNode*>(entityNodesTempActiveListCompleteIndex, entityNode));
}

void GIAdatabaseClass::initialiseDBentityNodesActiveListCompleteFastIndexDBcache()
{
	entityNodesActiveListCompleteFastIndexDBcache = new unordered_map<string, GIAentityNode*>;
}

void GIAdatabaseClass::initialiseDBentityNodesActiveListCompleteFastIndexDBactive()
{
	entityNodesActiveListCompleteFastIndexDBactive = new unordered_map<string, GIAentityNode*>;
}

void GIAdatabaseClass::clearDBentityNodesActiveListCompleteFastIndexDBcache()
{
	entityNodesActiveListCompleteFastIndexDBcache->clear();
}

void GIAdatabaseClass::clearDBentityNodesActiveListCompleteFastIndexDBactive()
{
	entityNodesActiveListCompleteFastIndexDBactive->clear();
}

void GIAdatabaseClass::setDBentityNodesActiveListCompleteFastIndexDBactive(unordered_map<string, GIAentityNode*>* newEntityNodesActiveListCompleteFastIndexDBactive)
{
	entityNodesActiveListCompleteFastIndexDBactive = newEntityNodesActiveListCompleteFastIndexDBactive;
}

unordered_map<string, GIAentityNode*>* GIAdatabaseClass::getDBentityNodesActiveListCompleteFastIndexDBactive()
{
	return entityNodesActiveListCompleteFastIndexDBactive;
}

string GIAdatabaseClass::DBreplaceBlankString(string word)
{
	string wordWithBlankStringReplaced = word;
	if(word == GIA_DATABASE_BLANK_STRING)
	{
		wordWithBlankStringReplaced = "";
	}
	return wordWithBlankStringReplaced;
}

string GIAdatabaseClass::DBaddBlankString(string word)
{
	string wordWithBlankStringAdded = word;
	if(word == "")
	{
		wordWithBlankStringAdded = GIA_DATABASE_BLANK_STRING;
	}
	return wordWithBlankStringAdded;
}



#endif



