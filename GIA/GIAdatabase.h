/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t2i 23-July-2013
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/

#ifndef HEADER_GIA_DATABASE
#define HEADER_GIA_DATABASE

#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAglobalDefs.h"


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <unordered_map>		//required for GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
#include <utility> 			// make_pair	//required for GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
using namespace std;


#define GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE (0)

#ifdef GIA_USE_DATABASE

	#define GIA_DATABASE_BLANK_STRING "-"

	#define GIA_USE_DATABASE_FALSE (0)
	#define GIA_USE_DATABASE_TRUE_READ_ACTIVE (1)		//get data from database
	#define GIA_USE_DATABASE_TRUE_READ_INACTIVE (2)		//do not get data from database (still prepare/operate for [in the event] database write though)

	#define GIA_DATABASE_FILESYSTEM_LINUX_PERMISSIONS 0755
	//#define GIA_DATABASE_USE_NETWORKED_FILE_SERVERS

	#define GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH (65)	//x2 to prevent buffer overflow... //64 characters per word allowed (NB this must include conjunctions between names, prepositions, etc...): +1 for cstr compatiblity (final char = '\0')
	#define GIA_DATABASE_ENTITY_NODE_MAX_NUMBER_ALIASES (3)
	#define GIA_DATABASE_ENTITY_NODE_ALIASES_STRING_MAX_LENGTH (GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH*GIA_DATABASE_ENTITY_NODE_MAX_NUMBER_ALIASES)
	#define GIA_DATABASE_INSTANCE_ID_MAX_LENGTH 10		//10 billion instances allowed
	#define GIA_DATABASE_ENTITY_GRAMMATICAL_NUMBER_MAX_LENGTH (9)	//1 billion - NB if this is increased, then grammaticalNumber must be made a long instead of an int
	#define GIA_ACTIVE_LIST_ID_MAX_LENGTH 19		//~billion billion ids allowed in active list (allows reading/writing database to XML - which is highly unrealistic)
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_MAX_LENGTH (9)
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_MAX_LENGTH (9)	
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_STRING_MAX_LENGTH (10)		//important: GIA database can only support quantity number strings <= 10 digits
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_STRING_MAX_LENGTH (10)	//important: GIA database can only support quantity modifer strings <= 10 digits
	#define GIA_DATABASE_ENTITY_NODE_MEASURE_TYPE_MAX_LENGTH (2)
	/*
	#define GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH_STRING "GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH"
	#define GIA_DATABASE_INSTANCE_ID_MAX_LENGTH_STRING "GIA_DATABASE_INSTANCE_ID_MAX_LENGTH"
	#define GIA_DATABASE_ENTITY_GRAMMATICAL_NUMBER_MAX_LENGTH_STRING "GIA_DATABASE_ENTITY_GRAMMATICAL_NUMBER_MAX_LENGTH"
	#define GIA_ACTIVE_LIST_ID_MAX_LENGTH_STRING "GIA_ACTIVE_LIST_ID_MAX_LENGTH"
	*/
	#define GIA_DATABASE_ENTITY_NODE_ALIASES_STRING_MAX_LENGTH_STRING "195"
	#define GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH_STRING "64"
	#define GIA_DATABASE_INSTANCE_ID_MAX_LENGTH_STRING "10"
	#define GIA_DATABASE_ENTITY_GRAMMATICAL_NUMBER_MAX_LENGTH_STRING "9"
	#define GIA_ACTIVE_LIST_ID_MAX_LENGTH_STRING "19"
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_MAX_LENGTH_STRING "9"
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_MAX_LENGTH_STRING "9"	
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_STRING_MAX_LENGTH_STRING "9"
	#define GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_STRING_MAX_LENGTH_STRING "9"
	#define GIA_DATABASE_ENTITY_NODE_MEASURE_TYPE_MAX_LENGTH_STRING "2"

	#define GIA_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAKBdatabase/")
	#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME "/home/systemusername/source/"	 //this could be "/mnt/serverNameX/" once configuring appropriate NFS Linux File Sharing in /etc/fstab	
	#define GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (3) 	//eg e/x/a/example
	#define GIA_DATABASE_INSTANCE_ID_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (3) 	//eg 1/2/3/instance123000000
	#define GIA_DATABASE_INSTANCE_ID_MAX_ORDER (GIA_DATABASE_INSTANCE_ID_MAX_LENGTH-1)		//for 9, maximum number of instances becomes 999999999
	#define GIA_DATABASE_INSTANCE_ID_MAX_ORDER_STRING "9"
	#define GIA_DATABASE_INSTANCE_ID_STRING_FORMAT "%0" GIA_DATABASE_INSTANCE_ID_MAX_ORDER_STRING "ld"	//assumes instance id is in long format
	/*
	#define GIA_DATABASE_INSTANCE_ID_STRING_FORMAT_PART_A "%0
	#define GIA_DATABASE_INSTANCE_ID_STRING_FORMAT_PART_B GIA_DATABASE_INSTANCE_ID_MAX_ORDER
	#define GIA_DATABASE_INSTANCE_ID_STRING_FORMAT_PART_C ld"
	#define GIA_DATABASE_INSTANCE_ID_STRING_FORMAT GIA_DATABASE_INSTANCE_ID_STRING_FORMAT_PART_A GIA_DATABASE_INSTANCE_ID_STRING_FORMAT_PART_B GIA_DATABASE_INSTANCE_ID_STRING_FORMAT_PART_C
	*/
	#define ASCII_TABLE_INDEX_OF_a (97)
	#define ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET (26)
	#define ASCII_TABLE_INDEX_OF_z (ASCII_TABLE_INDEX_OF_a + ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET)

	#define GIA_DATABASE_ENTITY_FILE_NAME_EXTENSION ".ent"
	#define GIA_DATABASE_REFERENCES_FILE_NAME_EXTENSION ".ref"
	#define GIA_DATABASE_CONCEPT_ENTITY_NODES_FILE_NAME "conceptEntityNodesLoadedList.txt"

	#define GIA_DATABASE_FILENAME_TYPE_ENTITY (1)
	#define GIA_DATABASE_FILENAME_TYPE_VECTOR_REFERENCE (1)

	/*
	#define GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR (true)
	#define GIA_DATABASE_GENERATE_FILENAME_CONNECTION_BASIC (false)
	#define GIA_DATABASE_GENERATE_FILENAME_CONNECTION_IRRELEVANT (false)
	*/
	#define GIA_DATABASE_GENERATE_FILENAME_TYPE_IRRELEVANT (-1)
	#define GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY (1)
	#define GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES (2)
	#define GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE (3)
	#define GIA_DATABASE_GENERATE_FILENAME_FILE_CONCEPT_ENTITY_NODES_LIST (4)

	//#define GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_TAKE_ADVANTAGE_FOR_DIRECT_IO_IMPLEMENTATION
	#define GIA_DATABASE_SAVE_WITH_LEADING_ZEROS	//required for readNumberOfReferencesInList()
	#define GIA_DATABASE_ATTRIBUTE_DELIMITER '|'
	#ifdef GIA_DATABASE_SAVE_WITH_LEADING_ZEROS

		#define GIA_DATABASE_ENTITY_NODE_FILE_FORMAT_READ "%ld|%s |%s |%s |%lf|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d |%s |%d |%s |%d|%d|%d\n"
		#define GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT_READ "%s |%d|%lf|%d|%d|%d|%d|%ld|%lf|%ld|%d|%d\n"	//used to be %1i %1i at end
		#define GIA_DATABASE_REFERENCES_FILE_FORMAT_READ "%s |%ld\n"

		#define GIA_DATABASE_ENTITY_NODE_FILE_FORMAT_WRITE "%0" GIA_ACTIVE_LIST_ID_MAX_LENGTH_STRING "ld|%" GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH_STRING "s |%" GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH_STRING "s |%" GIA_DATABASE_ENTITY_NODE_ALIASES_STRING_MAX_LENGTH_STRING "s |%0.6lf|%01d|%01d|%01d|%01d|%01d|%01d|%01d|%01d|%01d|%01d|%01d|%01d|%0" GIA_DATABASE_ENTITY_GRAMMATICAL_NUMBER_MAX_LENGTH_STRING "d|%01d|%0" GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_MAX_LENGTH_STRING "d|%" GIA_DATABASE_ENTITY_NODE_QUANTITY_NUMBER_STRING_MAX_LENGTH_STRING "s |%0" GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_MAX_LENGTH_STRING "d|%" GIA_DATABASE_ENTITY_NODE_QUANTITY_MODIFIER_STRING_MAX_LENGTH_STRING "s |%01d|%01d|%0" GIA_DATABASE_ENTITY_NODE_MEASURE_TYPE_MAX_LENGTH_STRING "d\n"
		#define GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT_WRITE "%" GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH_STRING "s |%02d|%0.6lf|%02d|%01d|%02d|%02d|%020ld|%0.6lf|%020ld|%01d|%01d\n"	//used to be %1i %1i at end
		#define GIA_DATABASE_REFERENCES_FILE_FORMAT_WRITE "%" GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH_STRING "s |%0" GIA_DATABASE_INSTANCE_ID_MAX_LENGTH_STRING "ld\n"

		#define GIA_DATABASE_REFERENCES_FILE_NUMBER_CHARACTERS_PER_LINE ((GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH-1) + 2 + GIA_DATABASE_INSTANCE_ID_MAX_LENGTH + 1)		//includes new line character

	#else
		#define GIA_DATABASE_ENTITY_NODE_FILE_FORMAT "%ld %s %s %0.6lf %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %s %d %s %d %d %d\n"
		#define GIA_DATABASE_TIME_CONDITION_NODE_FILE_FORMAT "%s %d %0.6lf %d %d %d %d %ld %0.6lf %ld %i %i\n"
		#define GIA_DATABASE_REFERENCES_FILE_FORMAT "%s %ld\n"
	#endif



	//by default GIA supports a single server, although it is coded to support 1 order of scalability in server name; ie, up to 26 servers. [NB could be programmed to support multiple levels of scalability in server name]
	#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_BASE (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
	#ifdef GIA_DATABASE_FILESYSTEM_USE_MULTIPLE_SERVERS
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_A (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_B (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_C (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_D (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_E (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_F (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_G (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_H (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_I (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_J (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_K (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_L (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_M (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_N (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_O (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_P (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Q (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_R (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_S (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_T (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_U (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_V (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_W (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_X (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Y (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
		#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Z (GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME)
	#endif
#endif

//#define GIA_USE_TIME_NODE_INDEXING

#ifdef GIA_USE_DATABASE
GIAentityNode * findOrAddConceptEntityNodeByName(vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDinCompleteList, long * currentEntityNodeIDinConceptEntityNodesList, bool saveNetwork);
GIAentityNode * DBfindOrAddConceptEntityNodeByName(vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDinCompleteList, long * currentEntityNodeIDinConceptEntityNodesList, bool saveNetwork);
GIAentityNode * LocalFindOrAddConceptEntityNodeByName(vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDinCompleteList, long * currentEntityNodeIDinConceptEntityNodesList, bool saveNetwork);
#else
GIAentityNode * findOrAddConceptEntityNodeByName(vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDinCompleteList, long * currentEntityNodeIDinConceptEntityNodesList, bool saveNetwork);
#endif


GIAentityNode * findActiveEntityNodeByID(long EntityNodeID, vector<GIAentityNode*> *entityNodesActiveListComplete);


#ifdef GIA_USE_DATABASE

#ifdef GIA_DATABASE_FILESYSTEM_USE_MULTIPLE_SERVERS
static string serverNameArray[ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET] = {GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_A, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_B, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_C, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_D, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_E, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_F, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_G, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_H, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_I, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_J, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_K, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_L, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_M, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_N, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_O, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_P, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Q, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_R, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_S, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_T, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_U, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_V, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_W, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_X, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Y, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Z};
#endif

bool directoryExists(string * folderName);
bool makeDirectory(string * folderName);
bool setCurrentDirectory(string * folderName);
bool checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(string * folderName);



string DBgenerateServerDatabaseName(string * entityName, int fileType);
string DBgenerateFileName(string * entityName, long idInstance, int connectionType, int fileType);

void initialiseDatabase(bool readFromDatabase, string newDatabaseFolderName, bool useDatabase, vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> * entityNodesActiveListConcepts);
	void DBreadConceptEntityNodesLoadedList();						//unordered_map<string, bool> *DBconceptEntityNodesLoadedList
	#ifdef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
	void DBreadDatabase(vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> * entityNodesActiveListConcepts);
	#endif
	
void DBreadVectorConnections(GIAentityNode * entityNode, int connectionType);
	void DBreadVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<GIAentityConnection*> *entityVectorConnections);
		void DBreadVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long *entityVectorConnectionsID, long referenceIndex);
	void DBreadVectorConnectionEntities(string * entityName, long idInstance, int connectionType, vector<GIAentityConnection*> *entityVectorConnections);
	void DBreadConceptEntityNode(string * entityName, GIAentityNode * conceptEntityNode);
		void DBreadEntityNode(string * entityName, long idInstance, GIAentityNode * entityNode);
			void DBreadEntityNodeFile(string * entityFileName, GIAentityNode* entity);
			void DBreadTimeConditionNodeFile(string * timeConditionFileName, GIAtimeConditionNode* timeCondition);

void writeAndCloseDatabase(vector<GIAentityNode*> *entityNodesActiveListComplete);
	void writeDatabase(vector<GIAentityNode*> *entityNodesActiveListComplete);
		void DBwriteEntityNode(string * entityName, long idInstance, GIAentityNode * entityNode);
			void DBwriteEntityNodeFile(string * entityFileName, GIAentityNode* entity);
			void DBwriteTimeConditionNodeFile(string * timeConditionFileName, GIAtimeConditionNode* timeCondition);
		void DBwriteVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<GIAentityConnection*> *entityVectorConnections);	//not yet used (this will need to be used)
			void DBmodifyVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long entityVectorConnectionsID, long referenceIndex);
			void DBappendVectorConnectionsReference(string * entityName, long idInstance, int connectionType, string *entityVectorConnectionsName, long entityVectorConnectionsID);
	void DBwriteConceptEntityNodesLoadedList();	//unordered_map<string, bool> *DBconceptEntityNodesLoadedList
void closeDatabase();

void DBprintConceptEntityNodesLoadedList(string executionStage);

long DBreadConceptEntityNumberOfInstances(string * entityNodeName);	//is this ever used? (shouldn't GIA need to read the number of references for all vector connection lists in the entityVectorConnectionsArray + entityBasicConnectionsArray, not just the instances vector?),
	long DBreadNumberOfReferencesInList(string * entityNodeName, long idInstance, int connectionType);

void initialiseDBconceptEntityNodesLoadedList();
void setUseDatabase(int value);
int getUseDatabase();

GIAentityNode * findEntityInActiveConceptList(string * entityName, long idInstance, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, bool * alreadyInRAM);		//to store reference set candidates in RAM [NOT USED]

GIAentityNode * findEntityNodesActiveListCompleteFastIndexDBcache(string * entityName, long idInstance, bool * foundNode);		//used to cache DB matched reference sets in RAM
GIAentityNode * findEntityNodesActiveListCompleteFastIndexDBactive(string * entityName, long idInstance, bool * foundNode);
	GIAentityNode * findEntityNodesActiveListCompleteFastIndex(string * entityName, long idInstance, bool * foundNode, unordered_map<string, GIAentityNode*> *entityNodesActiveListCompleteFastIndex);
void addEntityNodesActiveListCompleteFastIndexDBcache(GIAentityNode * entityNode);		//used to cache DB matched reference sets in RAM
void addEntityNodesActiveListCompleteFastIndexDBactive(GIAentityNode * entityNode);
	void addEntityNodesActiveListCompleteFastIndex(GIAentityNode * entityNode, unordered_map<string, GIAentityNode*> *entityNodesActiveListCompleteFastIndex);
	string createEntityNodesActiveListCompleteFastIndexIndex(string * entityName, long idInstance);	//used to cache DB matched reference sets in RAM
void initialiseDBentityNodesActiveListCompleteFastIndexDBcache();							//used to cache DB matched reference sets in RAM
void initialiseDBentityNodesActiveListCompleteFastIndexDBactive();
void clearDBentityNodesActiveListCompleteFastIndexDBcache();							//used to cache DB matched reference sets in RAM
void clearDBentityNodesActiveListCompleteFastIndexDBactive();
void setDBentityNodesActiveListCompleteFastIndexDBactive(unordered_map<string, GIAentityNode*> *newEntityNodesActiveListCompleteFastIndexDBactive);
unordered_map<string, GIAentityNode*> * getDBentityNodesActiveListCompleteFastIndexDBactive();

string DBreplaceBlankString(string word);
string DBaddBlankString(string word);

#endif

GIAentityNode * getPrimaryConceptNodeDefiningInstance(GIAentityNode * instanceEntity);

long maximumLong(long a, long b);





#endif

