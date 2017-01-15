/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l5f 03-June-2012
 * NB a property is an instance of an entity, any given entity may contain/comprise/have multiple properties - and properties are unrelated to definitions between entities [they just define what comprises any given entity]
 *
 *******************************************************************************/


#ifndef HEADER_GIA_ENTITY_NODE_CLASS
#define HEADER_GIA_ENTITY_NODE_CLASS



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
#include <unordered_map>		//required for GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
#include <utility> // make_pair	//required for GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
using namespace std;

//extracted from wn.h (WordNet 3.0 library header)
#define GRAMMATICAL_WORD_TYPE_UNDEFINED (0)
#define GRAMMATICAL_WORD_TYPE_NOUN (1)
#define GRAMMATICAL_WORD_TYPE_VERB (2)
#define GRAMMATICAL_WORD_TYPE_ADJ (3)
#define GRAMMATICAL_WORD_TYPE_ADV (4)
#define GRAMMATICAL_WORD_TYPE_SATELLITE (5)	/* not really a part of speech */
#define GRAMMATICAL_WORD_TYPE_ADJSAT (SATELLITE)
#define GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES (6)

#define GRAMMATICAL_TENSE_UNDEFINED 0
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

static string grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", "noun", "verb", "adj", "adv", "satellite"};	//must be same as FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME


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
static string entityVectorConnectionContextArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"outgoingAction(s)", "incomingAction(s)", "conditionNode(s)", "incomingConditionNode(s)", "propertyNode(s)", "propertyNode(s)", "entityNodeDefinition(s)", "incomingEntityNodeDefinition(s)", "associatedInstanceNodes(s)", "actionSubjectEntity", "actionObjectEntity", "conditionSubjectEntity", "conditionObjectEntity", "entityNodeDefiningThisInstance"};
static bool entityVectorConnectionThisIsInstanceAndPreviousNodeWasDefinitionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, true, false, false, false, false, false};
//ORIG: static bool entityVectorConnectionIsConditionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, true, false, false, false, false, false, false, false, true, true, false};
static bool entityVectorConnectionIsConditionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, true, false, false, false, false, false, false, false, false, false, false};
static bool entityVectorConnectionIsBasicArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false,false,false,false,false,false,false,false,false,true,true,true,true,true};
static string entityVectorConnectionDrawConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"NA", "NA", "NA", "NA", "NA", "property", "definition", "NA", "NA", "subject", "object", "subject", "object", "instance"};
#define GIA_ENTITY_TIME_CONDITION_NODE_NAME "timeCondition"
#define GIA_ENTITY_NODE_NAME "entity"
/*
#define GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPES (2)
*/


#define VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define GIA_REFERENCE_SET_ID_UNDEFINED (-1)


class GIAEntityConnection;

class GIAEntityNode
{
public:

	GIAEntityNode(void);
	~GIAEntityNode(void);
	
	long idActiveList;
	long idActiveEntityTypeList;
	long idActiveListReorderdIDforXMLsave;	//for CXL output only
	long idInstance; 		//not for concepts (this instance idActiveList of the concept entityName)
	
	string entityName;
	double confidence;
	
	bool isConcept;			//is this entity a concept? [added 10 May 2012]
	bool isProperty;		//is this entity a property?
	bool isAction;			//is this entity an action?
	bool isCondition;		//is this entity a condition?
	bool hasAssociatedInstance;	//this boolean appears to only represent whether this entity defines a child property node [and not whether it contains one]
	bool hasAssociatedInstanceIsAction;
	bool hasAssociatedInstanceIsCondition;
	bool hasAssociatedTime;
	bool hasQuality;		//PRECISE ORIGINAL NAME: isPropertyQualityOrAffection	//eg 'the locked door..' / 'Jim runs quickly' / 'Mr. Smith is late' 	[Not: Tom has an arm'/'Tom's bike']

	bool hasProgressiveTemp;	//PRECISE ORIGINALNAME: isActionOrPropertyState		//eg The cat is lying on the bed. / Mark is being happy.
	
	/*instances are now arbitrary, every entity is an instance of its parent(s) in some form or another...
	enum
	{
		isinstance, notinstance, undefinedInstance
	}instance;	//is the following entity known to be an instance?		
	*/

	vector<GIAEntityConnection*> entityVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];		//allows for generic coding
	#ifdef GIA_USE_ADVANCED_REFERENCING
	GIAEntityNode* entityCorrespondingBestMatch;	 //best match entity node corresponding to this assumed query entity node	//does not take into account multiple diversions/answers [assumes single matches only]	
	#endif
	
	#ifdef GIA_USE_DATABASE
	//designed for a high scale database (eg 200,000 references per instance, 200,000 instances per concept)	
	bool entityVectorConnectionsReferenceListLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];			//signifies whether all the vector connections in the reference list has been loaded from file and entityConnectionsNameArray/entityConnectionsIDArray have therefore been populated. This is the first step required to enable loading of connections into RAM (see entityVectorConnectionsLoadedArray)
	//bool entityVectorConnectionsLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];					//not used - vector connections are loaded into RAM on an individual basis. //signifies whether all the vector connection nodes have been loaded (eg from the db)	
	bool entityVectorConnectionsRemovedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];					//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
	#endif

	//action connections;
		//non-actions only;
	vector<GIAEntityConnection*> * ActionNodeList;	//where this entity is the subject of the action
	vector<GIAEntityConnection*> * IncomingActionNodeList;	//where this entity is the object of the action
	
		//actions only;
	//NB actions can be performed by and on concepts, and by and on properties?
	vector<GIAEntityConnection*> * actionSubjectEntity;	//record of entity that is the subject of this action instance
	vector<GIAEntityConnection*> * actionObjectEntity;	//record of which entity that is the object of this action instance
	
	//condition connections;
		//non-conditions only (?);
	//conditions connections: conditions and reverse conditions (reason) lookups [condition and reason respectively]
	vector<GIAEntityConnection*> * ConditionNodeList;		//this property requires the following...
	vector<GIAEntityConnection*> * IncomingConditionNodeList;	//this property is required by the following... //aka reason	[NB these may only be property, location, {and time action condtions}, not action conditions]

		//conditions only;
	//NB conditions can be performed by and on concepts, and by and on properties?
	vector<GIAEntityConnection*> * conditionSubjectEntity;		//record of entity that is the subject of this action instance
	vector<GIAEntityConnection*> * conditionObjectEntity;		//record of which entity that is the object of this action instance

	//time condition connections;
	int conditionType;	//added 25 Sept 11	
	GIATimeConditionNode * timeConditionNode;		//if conditionType == CONDITION_NODE_TYPE_TIME

	//property connections;
	//record list of all properties for this entity
	vector<GIAEntityConnection*> * PropertyNodeList;
		//properties only [is this possible for actions also? - may require upgrade in future]
	//GIAEntityConnection * entityNodeContainingThisProperty;		//removed 8 Dec 2011			//OLD: if property/action only:	//OLD: eg, Tom; OR;  Tom's Assets	//OLD: NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	vector<GIAEntityConnection*> * PropertyNodeReverseList;			//if property/action only:	//eg, Tom; OR;  Tom's Assets	//more than 1 thing can contain any given property [eg "a cat has arms", and "a monkey has arms"]; but note this may only be applicable for concept entities [property entities may possibly only be contained by {ie, be a property of} a single entity]

		//actions, properties, and conditions only
	vector<GIAEntityConnection*> * entityNodeDefiningThisInstance;					//if property/action/condition only:					//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector

	
		//concepts only (not properties/"instances" of entities);
	//entity connections;										
	//record parent and child entity definition nodes
	vector<GIAEntityConnection*> * EntityNodeDefinitionList;			//if not property only: 	//this should logically reduce to a single entity, although not required, therefore it is a vector [eg, a dog is a mammal, which is an animal, but a dog is an animal also]
	vector<GIAEntityConnection*> * EntityNodeDefinitionReverseList;			//if not property only: 	//more than one entity can be defined by this entity [eg if this entity is "animal", a bird is an animal, a mammal is an animal, etc]
	//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]
	vector<GIAEntityConnection*> * AssociatedInstanceNodeList;			//if not property only: if type == definesAPropertyAdjective (ie, if this entity is not a property/instance but defines one or more properties/instances)
	
	//CHECKTHIS; what is the difference between EntityNodeDefinitionList and entityNodeDefiningThisInstance? - it appears to achieve a similar purpose; ANSWER - one is direct definition [definition of instance] the other is not
	//CHECKTHIS; what is the difference between EntityNodeDefinitionReverseList and AssociatedInstanceNodeList? - it appears to achieve a similar purpose; ANSWER - one is direct definition [definition of instance] the other is not
	
	int grammaticalNumber;
	
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

	bool grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalTenseTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalNumberTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalDefiniteTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalRelexPersonOrStanfordProperNounTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed		
	int grammaticalGenderTemp; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	//bool grammaticalCountTemp;	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed		
	bool grammaticalPronounTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	
	bool isSubjectTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool isObjectTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool hasPropertyTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	//bool hasQualityTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool isSubjectTemp2;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Relex referencing only)
	bool isObjectTemp2;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Relex referencing only)
	bool hasPropertyTemp2;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Relex referencing only)
	int entityIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)
	int sentenceIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)
	
	#ifdef GIA_USE_STANFORD_CORENLP
	/*
	int CharacterOffsetBeginTemp;
	int CharacterOffsetEndTemp;
	*/
	string stanfordPOSTemp;
	string NormalizedNERTemp;
	string TimexTemp;
	#endif
	int NERTemp;
	
	
	//bool isReferenceEntityInThisSentence;	//temporary: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	bool entityAlreadyDeclaredInThisContext;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed

	bool hasAssociatedInstanceTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed

	bool isQuery;
	bool isWhichQuery;
	bool isAnswerToQuery;				
	bool testedForQueryComparison;
	bool testedForQueryComparisonTemp; //added 17 May 2012 - support better trace routine
	
	
	bool negative;	//for prepositional entities which will be collapsed into conditions only [in the future, this should also be used for properties and actions; but relex does not appear to output this information]
	
	bool disableParsingAsAPrepositionRelationTemp;
	
	bool queryEntityTraced;	//temporary for determining max confidence
	
	bool disabled;	//temporary for concept entities: used for GIA translator reference paser only - overwritten every time a new sentence is parsed 
	bool permanentConcept;	//concept entity is to be drawn/saved to XML (if false, this entity has been deemed redundant in semantic network generation)
	
	bool CXLdummyNode;
	
	//bool firstSentenceToAppearInNetwork;
	
	int wordNetPOS;			//added 26 April 2012 (used for compatibility with wordnet)	
	/*
	enum
	{
		definesAThingNoun, definesAPropertyAdjective, definesAnActionVerb, undefinedEntityType
	}type;	//is the following entity known to be an instance?		
	*/

	#ifdef GIA_USE_ADVANCED_REFERENCING
	int referenceSetID;
	#endif
	
	#ifdef GIA_USE_DATABASE
	bool added;	//implies database Update is Required
	bool modified;	//implies database Update is Required
	#endif
};

#ifdef GIA_USE_DATABASE
void DBsetEntityConnectionsLoaded(GIAEntityNode * entityNode, bool loaded);
#endif

void disconnectNodeFromAllButDefinitions(GIAEntityNode * entityNode);


int calculateQuantityNumberInt(string quantityNumberString);
int calculateQuantityModifierInt(string quantityModifierString);
int calculateQuantityMultiplierInt(string quantityMultiplierString);

string printQuantityNumberString(GIAEntityNode * entityNode);


#endif

