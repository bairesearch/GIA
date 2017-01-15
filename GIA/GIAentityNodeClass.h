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
 * File Name: GIAentityNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t5c 02-August-2013
 * NB a substance is an instance of an entity, any given entity may contain/comprise/have multiple substances - and substances are unrelated to definitions between entities [they just define what comprises any given entity]
 *
 *******************************************************************************/


#ifndef HEADER_GIA_ENTITY_NODE_CLASS
#define HEADER_GIA_ENTITY_NODE_CLASS



#include "GIAconditionNodeClass.h"
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
#include <utility> // make_pair	//required for GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
using namespace std;

#ifdef GIA_SUPPORT_ALIASES
	#define GIA_ALIASES_CONVERT_TO_STRING_DELIMITER ';'	//has to be different than attribute delimiter used in GIA filesystem database GIA_DATABASE_ATTRIBUTE_DELIMITER
#endif

#define GRAMMATICAL_TENSE_CONCATONATOR_RELEX "_"

//extracted from wn.h (WordNet 3.0 library header)
#define GRAMMATICAL_WORD_TYPE_UNDEFINED (0)
#define GRAMMATICAL_WORD_TYPE_NOUN (1)
#define GRAMMATICAL_WORD_TYPE_VERB (2)
#define GRAMMATICAL_WORD_TYPE_ADJ (3)
#define GRAMMATICAL_WORD_TYPE_ADV (4)
#define GRAMMATICAL_WORD_TYPE_PREP (5)
#define GRAMMATICAL_WORD_TYPE_SATELLITE (6)	/* not really a part of speech */
#define GRAMMATICAL_WORD_TYPE_ADJSAT (SATELLITE)
#define GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES (7)

#define GRAMMATICAL_TENSE_PRESENT 1
#define GRAMMATICAL_TENSE_PAST 2
#define GRAMMATICAL_TENSE_FUTURE 3
#define GRAMMATICAL_TENSE_NUMBER_OF_TYPES (4)

#define GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE 0
#define GRAMMATICAL_TENSE_MODIFIER_PERFECT 1
#define GRAMMATICAL_TENSE_MODIFIER_PASSIVE 2
#define GRAMMATICAL_TENSE_MODIFIER_INFINITIVE 3
#define GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE 4
#define GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES (5)

#define GRAMMATICAL_NUMBER_UNDEFINED 0
#define GRAMMATICAL_NUMBER_UNCOUNTABLE 1
#define GRAMMATICAL_NUMBER_SINGULAR 2
#define GRAMMATICAL_NUMBER_PLURAL 3
#define GRAMMATICAL_NUMBER_NUMBER_OF_TYPES (4)

#define GRAMMATICAL_DEFINITE_UNDEFINED false
#define GRAMMATICAL_DEFINITE true
#define GRAMMATICAL_DEFINITE_NAME "definite"

#define GRAMMATICAL_PERSON_UNDEFINED false
#define GRAMMATICAL_PERSON true
#define GRAMMATICAL_PERSON_NAME "person"

#define GRAMMATICAL_GENDER_UNDEFINED 0
#define GRAMMATICAL_GENDER_PERSON 1
#define GRAMMATICAL_GENDER_MASCULINE 2
#define GRAMMATICAL_GENDER_FEMININE 3
#define GRAMMATICAL_GENDER_NUMBER_OF_TYPES (4)

#define GRAMMATICAL_CATEGORY_UNDEFINED (0)

#define GRAMMATICAL_PRONOUN_UNDEFINED false
#define GRAMMATICAL_PRONOUN true
#define GRAMMATICAL_PRONOUN_NAME "pronoun"

//#define GRAMMATICAL_COUNT_UNDEFINED 0

#define GRAMMATICAL_PROPERNOUN_NAME "propernoun"	//added 28 April 2012 (for cff export)


#define QUANTITY_NUMBER_UNDEFINED (1)
#define QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES (20)
#define QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES (10)
#define QUANTITY_MULTIPLIER_NUMBER_OF_TYPES (7)		//do: work out what these are/can be
#define QUANTITY_MODIFIER_UNDEFINED (-1)		//WARNING: not yet implemented
#define QUANTITY_MODIFIER_NUMBER_OF_TYPES (1)		//WARNING: not yet implemented	//do: work out what these are/can be

#define MEASURE_TYPE_DISTANCE (0)	//see relationTypeMeasureNameArray
#define MEASURE_TYPE_SIZE (1)
#define MEASURE_TYPE_TIME (2)
#define MEASURE_TYPE_PER (3)
#define MEASURE_TYPE_UNKNOWN (4)
#define MEASURE_DEPENDENCY_UNKNOWN (5)
#define MEASURE_TYPE_UNDEFINED (-1)

#define FEATURE_NER_UNDEFINED (0)
#define FEATURE_NER_DATE (1)
#define FEATURE_NER_TIME (2)
#define FEATURE_NER_MONEY (3)
#define FEATURE_NER_NUMBER (4)
#define FEATURE_NER_PERSON (5)
#define FEATURE_NER_LOCATION (6)
#define FEATURE_NER_ORGANIZATION (7)
#define FEATURE_NER_MISC (8)
#define FEATURE_NER_DURATION (9)
#define FEATURE_NER_NUMBER_TYPES (10)
#define FEATURE_NER_EXPLICIT_NUMBER_TYPES (9)

static string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {"undefined", "present", "past", "future"};
	//OPENCOG RECOMMENDED; "future", "future_progressive", "imperative", "infinitive", "past", "past_infinitive", "past_progressive", "perfect", "present", "present_progressive", "progressive"
	//MINIMAL; "undefined", "future", "progressive", "imperative", "infinitive", "past", "perfect", "present"
	//ABSTRACTED; "present", "present_perfect", "present_passive", "present_infinitive", "present_progressive", "present_progressive_passive", "past", "past_passive", "past_infinitive", "past_progressive", "future", "future_progressive", "imperative", "progressive"
static string grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES] = {"progressive", "perfect", "passive", "infinitive", "imperative"};
static string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {"undefined", "uncountable", "singular", "plural"};
static string grammaticalGenderNameArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES] = {"undefined", "person", "masculine", "feminine"};

static string grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", "noun", "verb", "adj", "adv", "prep", "satellite"};	//must be same as FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME


#define GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES (14)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS (0)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS (1)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS (2)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS (3)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES (4)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES (5)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS (6)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS (7)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES (8)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT (9)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT (10)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT (11)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT (12)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE (13)	//GIA_ENTITY_BASIC_CONNECTION_TYPE_INSTANCE_DEFINITION
static string entityVectorConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"actions", "incomingActions", "conditions", "incomingConditions", "properties", "reverseProperties", "definitions", "reverseDefinitions", "associatedInstances","actionSubject", "actionObject", "conditionSubject", "conditionObject", "nodeDefiningInstance"};	//instanceDefinition
static string entityVectorConnectionSourceContextArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"is ", "", "", "", "has ", "possessed by ", "is ", "defines ", "","is done by ", "", "", "", ""};
static string entityVectorConnectionContextArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"outgoingAction(s)", "incomingAction(s)", "conditionNode(s)", "incomingConditionNode(s)", "propertyNode(s)", "reversePropertyNode(s)", "entityNodeDefinition(s)", "reverseEntityNodeDefinition(s)", "associatedInstanceNodes(s)", "actionSubjectEntity", "actionObjectEntity", "conditionSubjectEntity", "conditionObjectEntity", "entityNodeDefiningThisInstance"};
static bool entityVectorConnectionThisIsInstanceAndPreviousNodeWasDefinitionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, true, false, false, false, false, false};
//ORIG: static bool entityVectorConnectionIsConditionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, true, false, false, false, false, false, false, false, true, true, false};
static bool entityVectorConnectionIsConditionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, true, false, false, false, false, false, false, false, false, false, false};
static bool entityVectorConnectionIsBasicArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false,false,false,false,false,false,false,false,false,true,true,true,true,true};
static string entityVectorConnectionDrawConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"NA", "NA", "NA", "NA", "NA", "property", "NA", "definition", "NA", "subject", "object", "subject", "object", "instance"};
#define GIA_ENTITY_TIME_CONDITION_NODE_NAME "timeCondition"
#define GIA_ENTITY_NODE_NAME "entity"
/*
#define GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPES (2)
*/

static int inverseVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES};


#define GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES_EQUALITIES (2)
static int entityVectorConnectionEqualitiesArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES_EQUALITIES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS};


#define VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define GIA_REFERENCE_SET_ID_UNDEFINED (-1)
#define GIA_ENTITY_INDEX_UNDEFINED (-1)
#define GIA_SENTENCE_INDEX_UNDEFINED (-1)

class GIAentityConnection;

#ifdef GIA_USE_DATABASE
#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
class GIAconceptEntityLoaded
{
public:
	GIAconceptEntityLoaded(void);
	~GIAconceptEntityLoaded(void);

	bool loaded;
	long numberOfInstances;
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	bool disabled; 	//added 29 November 2012 for GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES support
	#endif
};
#endif
#endif

class GIAentityNode
{
public:

	GIAentityNode(void);
	~GIAentityNode(void);

	long idActiveList;
	long idActiveEntityTypeList;
	long idActiveListReorderdIDforXMLsave;	//for CXL output only
	long idInstance; 		//not for concepts (this instance idActiveList of the concept entityName)

	string entityName;
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	string wordOrig;		//this needs to be added to XML i/o + file system database i/o [used for NLG2 bug]
	#endif
	double confidence;		//not currently used
	#ifdef GIA_SUPPORT_ALIASES
	vector<string> aliasList;
	#endif

	bool isConcept;			//is this entity a concept? [added 10 May 2012]
	bool isSubstance;		//is this entity a substance?
	bool isAction;			//is this entity an action?
	bool isCondition;		//is this entity a condition?
	bool hasAssociatedInstance;	//this boolean appears to only represent whether this entity defines a child substance node [and not whether it contains one]
	bool hasAssociatedInstanceIsAction;
	bool hasAssociatedInstanceIsCondition;
	bool hasAssociatedTime;
	bool isSubstanceQuality;		//PRECISE ORIGINAL NAME: isSubstanceQualityOrAffection	//eg 'the locked door..' / 'Jim runs quickly' / 'Mr. Smith is late' 	[Not: Tom has an arm'/'Tom's bike']
	bool isSubstanceConcept;		//added 1q4a to take into account specific concepts eg 'red bears' as opposed to 'bears' //eg Red dogs are bad animals. / A blue chicken is a happy bird.
	bool isActionConcept;			//added 1t5a to take into account specific actions eg 'eating pies', 'to eat a pie'
	
	bool hasProgressiveTemp;	//PRECISE ORIGINALNAME: isActionOrSubstanceQualityState		//eg The cat is lying on the bed. / Mark is being happy.

	vector<GIAentityConnection*> entityVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];		//allows for generic coding
	#ifdef GIA_USE_ADVANCED_REFERENCING
	GIAentityNode* entityCorrespondingBestMatch;	 //best match entity node corresponding to this assumed query entity node	//does not take into account multiple diversions/answers [assumes single matches only]
	#endif

	#ifdef GIA_USE_DATABASE
	//designed for a high scale database (eg 200,000 references per instance, 200,000 instances per concept)
	bool entityVectorConnectionsReferenceListLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];			//signifies whether all the vector connections in the reference list has been loaded from file and entityConnections entityNames+idInstance have therefore been populated. This is the first step required to enable loading of connections into RAM (see entityVectorConnectionsLoadedArray)
	//bool entityVectorConnectionsLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];				//not used - vector connections are loaded into RAM on an individual basis. //signifies whether all the vector connection nodes have been loaded (eg from the db)
	bool entityVectorConnectionsRemovedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];					//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
	#endif

	//action connections;
		//non-actions only;
	vector<GIAentityConnection*> * actionNodeList;	//where this entity is the subject of the action
	vector<GIAentityConnection*> * incomingActionNodeList;	//where this entity is the object of the action

		//actions only;
	//NB actions can be performed by and on concepts, and by and on substances?
	vector<GIAentityConnection*> * actionSubjectEntity;	//record of entity that is the subject of this action instance
	vector<GIAentityConnection*> * actionObjectEntity;	//record of which entity that is the object of this action instance

	//condition connections;
		//non-conditions only (?);
	//conditions connections: conditions and reverse conditions (reason) lookups [condition and reason respectively]
	vector<GIAentityConnection*> * conditionNodeList;		//this substance requires the following...
	vector<GIAentityConnection*> * incomingConditionNodeList;	//this substance is required by the following... //aka reason

		//conditions only;
	//NB conditions can be performed by and on concepts, and by and on substances?
	vector<GIAentityConnection*> * conditionSubjectEntity;		//record of entity that is the subject of this action instance
	vector<GIAentityConnection*> * conditionObjectEntity;		//record of which entity that is the object of this action instance

	//time condition connections;
	int conditionType;	//added 25 Sept 11
	GIAtimeConditionNode * timeConditionNode;		//if conditionType == CONDITION_NODE_TYPE_TIME

	//substance connections;
	//record list of all substances for this entity
	vector<GIAentityConnection*> * propertyNodeList;
	vector<GIAentityConnection*> * propertyNodeReverseList;			//if substance/action only:	//eg, Tom; OR;  Tom's Assets	//more than 1 thing can contain any given substance [eg "a cat has arms", and "a monkey has arms"]; but note this may only be applicable for concept entities [substance entities may possibly only be contained by {ie, be a substance of} a single entity]

		//actions, substances, and conditions only
	vector<GIAentityConnection*> * entityNodeDefiningThisInstance;					//if substance/action/condition only:					//NB by definition, only 1 thing can contain any given substance [considering a substance is an instance of an entity] - therefore this is not a vector


		//concepts only (not substances/"instances" of entities);
	//entity connections;
	//record parent and child entity definition nodes
	vector<GIAentityConnection*> * entityNodeDefinitionList;			//this should logically reduce to a single entity, although not required, therefore it is a vector [eg, a dog is a mammal, which is an animal, but a dog is an animal also]
	vector<GIAentityConnection*> * entityNodeDefinitionReverseList;			//more than one entity can be defined by this entity [eg if this entity is "animal", a bird is an animal, a mammal is an animal, etc]
	//associated actions and substances [ie does this entity also define an action/verb or a substance/adjective? [ie, it is not just a thing/noun]]
	vector<GIAentityConnection*> * associatedInstanceNodeList;			//if this entity is not a substance/instance but defines one or more substances/instances

	bool hasQuantity;
	int quantityNumber;		//eg 6
	string quantityNumberString;	//eg "6:45"
	int quantityModifier;	//not yet implemented
	string quantityModifierString;	//eg "almost"
	bool hasQuantityMultiplier;
	bool hasMeasure;
	int measureType;

	bool initialisedForPrinting;
	//bool printed;
	int printX;
	int printY;
	int printXIndex;
	int printYIndex;
	int printTextX;
	int printTextY;
	bool printCoordsAlreadyDefined;

	bool grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalTenseTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalNumber;
	bool grammaticalDefiniteTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalRelexPersonOrStanfordProperNounTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalGenderTemp; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	//bool grammaticalCountTemp;	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool grammaticalPronounTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	#ifdef GIA_USE_ADVANCED_REFERENCING
	int grammaticalDefiniteIndexOfDeterminerTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	#endif
	bool foundPossibleInfinitiveVerbTemp;	//added 28 July 2013 to help support action concepts

	bool isSubjectTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool isObjectTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool hasSubstanceTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	//bool hasQualityTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool isActionTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	int entityIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)
	int sentenceIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)

	#ifdef GIA_USE_STANFORD_CORENLP
	/*
	int CharacterOffsetBeginTemp;
	int CharacterOffsetEndTemp;
	*/
	string stanfordPOStemp;
	string NormalizedNERtemp;
	string TimexTemp;
	#endif
	int NERTemp;

	//bool isReferenceEntityInThisSentence;	//temporary: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	bool entityAlreadyDeclaredInThisContext;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed

	bool hasAssociatedInstanceTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed

	bool isQuery;
	bool isWhichOrEquivalentWhatQuery;
	#ifdef GIA_SUPPORT_ALIASES
	bool isName;
	bool isNameQuery;
	#endif
	bool isAnswerToQuery;
	bool testedForQueryComparison;
	bool testedForQueryComparisonTemp; //added 17 May 2012 - support better trace routine
	bool queryAnswerContext;

	bool negative;	//for prepositional entities which will be collapsed into conditions only [in the future, this should also be used for substances and actions; but relex does not appear to output this information]

	bool disableParsingAsPrepositionRelationTemp;

	bool queryEntityTraced;	//temporary for determining max confidence

	bool disabled;	//temporary for concept entities: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool permanentConcept;	//concept entity is to be drawn/saved to XML (if false, this entity has been deemed redundant in semantic network generation)

	bool CXLdummyNode;

	bool firstSentenceToAppearInNetwork;

	int wordNetPOS;		//grammaticalWordTypeTemp	//added 26 April 2012 (used for compatibility with wordnet)

	#ifdef GIA_USE_ADVANCED_REFERENCING
	int referenceSetID;
	int minimumEntityIndexOfReferenceSet;
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	bool wasReferenceTemp;
	#endif
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	bool wasReference;
	#endif	
	#endif

	#ifdef GIA_USE_DATABASE
	bool added;	//implies database Update is Required
	bool modified;	//implies database Update is Required

	#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
	GIAconceptEntityLoaded * conceptEntityLoaded;
	#endif
	#endif

	#ifdef GIA_USE_NLG
	bool parsedForLanguageGeneration;
	bool sourceAddedInLanguageGeneration;
	bool sourceReferencedInLanguageGeneration;
	#endif

	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	bool isName;
	#endif
	
	bool isToBeComplimentOfActionTemp;	//required for Relex (linkConditions()/defineSubstancesOfPossessivePrepositions())
};



#ifdef GIA_USE_DATABASE
void DBsetEntityConnectionsReferenceListsLoaded(GIAentityNode * entityNode, bool loaded);
#endif

void disconnectNodeFromAllButDefinitions(GIAentityNode * entityNode);


int calculateQuantityNumberInt(string quantityNumberString);
int calculateQuantityModifierInt(string quantityModifierString);
int calculateQuantityMultiplierInt(string quantityMultiplierString);

string printQuantityNumberString(GIAentityNode * entityNode);


#ifdef GIA_SUPPORT_ALIASES
void convertAliasesStringToAliases(GIAentityNode * entityNode, string aliasesString);
void convertAliasesToAliasesString(GIAentityNode * entityNode, string * aliasesString);
vector<string> explode(const string& str, const char& ch);
#endif

void deleteEntitiesInEntityNodeList(vector<GIAentityNode*> * entityNodesActiveListComplete);

#endif


