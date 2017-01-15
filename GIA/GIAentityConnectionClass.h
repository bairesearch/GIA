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
 * File Name: GIAentityConnectionClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2e4f 19-April-2014
 * NB a substance is an instance of an entity, any given entity may contain/comprise/have multiple substances - and substances are unrelated to definitions between entities [they just define what comprises any given entity]
 *
 *******************************************************************************/


#ifndef HEADER_GIA_ENTITY_CONNECTION_CLASS
#define HEADER_GIA_ENTITY_CONNECTION_CLASS



#include "GIAglobalDefs.h"


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>		//required for GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
#include <utility> 			// make_pair	//required for GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
using namespace std;

#define GIA_ENTITY_CONNECTION_CONFIDENCE_BASE (1.0)
#define GIA_ENTITY_CONNECTION_CONFIDENCE_INCREMENT (1.0)

class GIAentityNode;

class GIAentityConnection
{
public:
	GIAentityConnection(void);
	~GIAentityConnection(void);
	GIAentityConnection(string * entityName, long idInstance);

	GIAentityNode * entity;
	double confidence;

	#ifdef GIA_USE_ADVANCED_REFERENCING_SEARCH_CODE
	bool sameReferenceSet;
	#endif

	#ifdef GIA_USE_DATABASE
	bool referenceLoaded;
	string entityName;	//records the vector connection target entity name (to enable loading from db) [of target]
	long idInstance;	//records the vector connection target instance id (to enable loading from db) [of target]
	bool loaded;		//signifies whether the vector connection target node has been loaded and is in RAM (eg from the db)
	bool modified;		//signifies whether the database needs to be updated upon exit with modified reference
	bool added;		//signifies whether the database needs to be updated upon exit with new reference
	//long referenceIndex; 	//OLD [now assume referenceIndex equals the index of the connection in the connection vector] added 24 May 2012 (needs to be populated)
	#endif

	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
	bool initialisedForPrinting;
	#endif

	#ifdef USE_NLC
	bool parsedForNLCcodeBlocks;
	bool NLCconditionLogicalOperations;
	#endif
	/*
	#ifdef GIA_USE_NLG
	bool parsedForLanguageGeneration;
	#endif
	*/
};

#endif

