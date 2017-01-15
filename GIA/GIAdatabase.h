/*******************************************************************************
 *
 * File Name: GIAdatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l1d 22-May-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/

#ifndef HEADER_GIA_DATABASE
#define HEADER_GIA_DATABASE

#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"
#include "GIAglobalDefs.h"


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#ifdef GIA_USE_DATABASE
	//#define GIA_DATABASE_SAVE_WITH_LEADING_ZEROS_FOR_FUTURE_DIRECT_IO_IMPLEMENTATION
	
	#define GIA_DATABASE_ENTITY_NODE_NAME_MAX_LENGTH (64)	//64 characters per word allowed (NB this must include conjunctions between names, prepositions, etc...)
	#define GIA_DATABASE_INSTANCE_ID_MAX_LENGTH (10)	//10 billion instances allowed
	#define GIA_DATABASE_ENTITY_GRAMMATICAL_NUMBER_MAX_LENGTH (9)	//1 billion - NB if this is increased, then grammaticalNumber must be made a long instead of an int
	
	#define GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME "\$HOME/source/"	//this could be "/mnt/serverNameX/" once configuring appropriate NFS Linux File Sharing in /etc/fstab
	#define GIA_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAKBdatabase")
	#define GIA_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (3) 	//eg e/x/a/example
	#define GIA_DATABASE_INSTANCE_ID_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (3) 	//eg 1/2/3/instance123000000
	#define GIA_DATABASE_INSTANCE_ID_MAX_ORDER 9		//for 9, maximum number of instances becomes 999999999
	#define GIA_DATABASE_INSTANCE_ID_STRING_FORMAT "%0 GIA_DATABASE_INSTANCE_ID_MAX_ORDER ld"		//assumes instance id is in long format
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
	
	#define GIA_DATABASE_FILENAME_TYPE_ENTITY (1)
	#define GIA_DATABASE_FILENAME_TYPE_VECTOR_REFERENCE (1)
	
	#define GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE (0)
	
	#define GIA_DATABASE_GENERATE_FILENAME_CONNECTION_VECTOR (true)
	#define GIA_DATABASE_GENERATE_FILENAME_CONNECTION_BASIC (false)
	#define GIA_DATABASE_GENERATE_FILENAME_CONNECTION_IRRELEVANT (false)
	#define GIA_DATABASE_GENERATE_FILENAME_FILE_ENTITY (1)
	#define GIA_DATABASE_GENERATE_FILENAME_FILE_REFERENCES (2)
	#define GIA_DATABASE_GENERATE_FILENAME_FILE_TIME_CONDITION_NODE (3)
	
	//by default GIA supports a single server, although it is coded to support 1 order of scalability in server name; ie, up to 26 servers. [NB could be programmed to support multiple levels of scalability in server name]
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

//#define GIA_USE_TIME_NODE_INDEXING

#ifdef GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
	GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork);
#else
	GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList);
	#ifdef GIA_USE_TIME_NODE_INDEXING
	GIATimeConditionNode * findOrAddTimeNodeByNumber(vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList, long * timeNodeNumber, bool * found, long * index, bool addIfNonexistant, GIATimeConditionNode * prexistingTimeConditionNode);
	#endif
#endif
GIAEntityNode * findEntityNodeByID(long EntityNodeID, vector<GIAEntityNode*> *entityNodesCompleteList);


#ifdef GIA_USE_DATABASE
static string serverNameArray[ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET] = {GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_A, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_B, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_C, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_D, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_E, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_F, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_G, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_H, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_I, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_J, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_K, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_L, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_M, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_N, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_O, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_P, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Q, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_R, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_S, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_T, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_U, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_V, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_W, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_X, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Y, GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_Z};

GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, bool> *conceptEntityNodesLoadedList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList, bool saveNetwork)

string DBgenerateServerName(string * entityName);
string DBgenerateFileName(string * entityName, long idInstance, int connectionType, bool vectorOrBasic, int fileType);

void DBwriteVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<string> *entityVectorConnectionsNames, vector<long> *entityVectorConnectionsID);	//not yet used (this will need to be used)
void DBwriteBasicConnectionsReferences(string * entityName, long idInstance, int connectionType, string *entityBasicConnectionName, long *entityBasicConnectionID);				//not yet used (this will need to be used)

void DBreadVectorConnections(string * entityName, long idInstance, int connectionType, vector<string> *entityVectorConnectionsNames, vector<long> *entityVectorConnectionsID, vector<bool> *entityVectorConnectionsLoaded, vector<GIAEntityNode*> *entityVectorConnections);
void DBreadBasicConnections(string * entityName, long idInstance, int connectionType, string *entityBasicConnectionName, long *entityBasicConnectionID, bool *entityBasicConnectionLoaded, GIAEntityNode* entityBasicConnection);
	void DBreadVectorConnectionsReferences(string * entityName, long idInstance, int connectionType, vector<string> *entityVectorConnectionsNames, vector<long> *entityVectorConnectionsID);
	void DBreadBasicConnectionsReferences(string * entityName, long idInstance, int connectionType, string *entityBasicConnectionName, long *entityBasicConnectionID);

void DBreadEntityNode(string * entityName, long idInstance, int connectionType, GIAEntityNode ** entityNode);
void DBwriteEntityNodeFile(string * entityFileName, GIAEntityNode* entity);
void DBreadTimeConditionNodeFile(string * timeConditionFileName, GIATimeConditionNode* timeCondition);
void DBwriteTimeConditionNodeFile(string * timeConditionFileName, GIATimeConditionNode* timeCondition);


void setEntityConnectionsLoaded(GIAEntityNode * entityNode, bool loaded)

#endif


long maximumLong(long a, long b);

#endif

