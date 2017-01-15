/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i9a 11-Apr-2012
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
#define MEASURE_TYPE_UNDEFINED (-1)

extern string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES];
extern string grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];
extern string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES];
extern string grammaticalGenderNameArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES];


class GIAEntityNode
{
public:

	GIAEntityNode(void);
	~GIAEntityNode(void);
	
	long id;
	long idSecondary;
	long reorderdIDforXMLsave;

	string entityName;
	double confidence;
		
	bool isProperty;		//is this entity a property?
	bool isAction;			//is this entity an action?
	bool isCondition;		//is this entity a condition?
	bool hasAssociatedInstance;	//this boolean appears to only represent whether this entity defines a child property node [and not whether it contains one]
	bool hasAssociatedInstanceIsAction;
	bool hasAssociatedInstanceIsCondition;
	bool hasAssociatedTime;
	bool hasProgressiveTemp;	//PRECISE ORIGINALNAME: isActionOrPropertyState		//eg The cat is lying on the bed. / Mark is being happy.
	bool hasQuality;		//PRECISE ORIGINAL NAME: isPropertyQualityOrAffection	//eg 'the locked door..' / 'Jim runs quickly' / 'Mr. Smith is late' 	[Not: Tom has an arm'/'Tom's bike']
		
	/*
	enum
	{
		definesAThingNoun, definesAPropertyAdjective, definesAnActionVerb, undefinedEntityType
	}type;	//is the following entity known to be an instance?		
	*/
	
	/*instances are now arbitrary, every entity is an instance of its parent(s) in some form or another...
	enum
	{
		isinstance, notinstance, undefinedInstance
	}instance;	//is the following entity known to be an instance?		
	*/

		//non-actions only;
	//action connections;
	//where this entity is the subject of the action
	vector<GIAEntityNode*> ActionNodeList;
	vector<GIAEntityNode*>::iterator ActionNodeListIterator;
	//where this entity is the object of the action
	vector<GIAEntityNode*> IncomingActionNodeList;
	vector<GIAEntityNode*>::iterator IncomingActionNodeListIterator;

		//actions only;
	//NB actions can be performed by and on entities, and by and on properties
	//record of entity that is the subject of this action instance
	GIAEntityNode * actionSubjectEntity;
	//record of which entity that is the object of this action instance
	GIAEntityNode * actionObjectEntity;

		//conditions only;
	//conditions connections: conditions and reverse conditions (reason) lookups [condition and reason respectively]
	vector<GIAEntityNode*> ConditionNodeList;		//this property requires the following...
	vector<GIAEntityNode*>::iterator ConditionNodeListIterator;		
	vector<GIAEntityNode*> IncomingConditionNodeList;	//this property is required by the following... //aka reason	[NB these may only be property, location, {and time action condtions}, not action conditions]
	vector<GIAEntityNode*>::iterator IncomingConditionNodeListIterator;		
		
	//NB actions can be performed by and on entities, and by and on properties
	//record of entity that is the subject of this action instance
	GIAEntityNode * conditionSubjectEntity;
	//record of which entity that is the object of this action instance
	GIAEntityNode * conditionObjectEntity;	
	int conditionType;	//added 25 Sept 11	
	GIATimeConditionNode * timeConditionNode;		//if conditionType == CONDITION_NODE_TYPE_TIME

	//property connections;
	//record list of all properties for this entity
	vector<GIAEntityNode*> PropertyNodeList;
	vector<GIAEntityNode*>::iterator PropertyNodeListIterator;
	
		//properties only [is this possible for actions also? - may require upgrade in future]
	//GIAEntityNode * entityNodeContainingThisProperty;		//removed 8 Dec 2011			//OLD: if property/action only:	//OLD: eg, Tom; OR;  Tom's Assets	//OLD: NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	vector<GIAEntityNode*> PropertyNodeReverseList;			//if property/action only:	//eg, Tom; OR;  Tom's Assets	//more than 1 thing can contain any given property [eg "a cat has arms", and "a monkey has arms"]; but note this may only be applicable for concept entities [property entities may possibly only be contained by {ie, be a property of} a single entity]
	vector<GIAEntityNode*>::iterator PropertyNodeReverseListIterator;	
	
		//actions and properties only
	GIAEntityNode * entityNodeDefiningThisInstance;					//if property/action only:					//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	
		//pure entites only (not properties/"instances" of entities)
	//entity connections;										
	//record parent and child entity definition nodes
	vector<GIAEntityNode*> EntityNodeDefinitionList;				//if not property only: 	//this should logically reduce to a single entity, although not required, therefore it is a vector [eg, a dog is a mammal, which is an animal, but a dog is an animal also]
	vector<GIAEntityNode*>::iterator EntityNodeDefinitionListIterator;	//if not property only: 
	vector<GIAEntityNode*> EntityNodeDefinitionReverseList;			//if not property only: 	//more than one entity can be defined by this entity [eg if this entity is "animal", a bird is an animal, a mammal is an animal, etc]
	vector<GIAEntityNode*>::iterator EntityNodeDefinitionReverseListIterator;	//if not property only: 
	//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]
	vector<GIAEntityNode*> AssociatedInstanceNodeList;			//if not property only: if type == definesAPropertyAdjective (ie, if this entity is not a property/instance but defines one or more properties/instances)
	vector<GIAEntityNode*>::iterator AssociatedInstanceNodeListIterator;	//if not property only: if type == definesAPropertyAdjective (ie, if this entity is not a property/instance but defines one or more properties/instances)
	
	
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
	bool grammaticalRelexPersonOrStanfordProperNounTemp;	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed		
	int grammaticalGenderTemp; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	//bool grammaticalCountTemp;	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed		
	bool grammaticalPronounTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool isSubjectTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool isObjectTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool hasPropertyTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	//bool hasQualityTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool isObject;			//used for generating non exact answers during queries
	
	#ifdef GIA_USE_STANFORD_CORENLP
	/*
	int CharacterOffsetBeginTemp;
	int CharacterOffsetEndTemp;
	string POSTemp;
	*/
	string NERTemp;
	string NormalizedNERTemp;
	string TimexTemp;
	#endif
	
	
	//bool isReferenceEntityInThisSentence;	//temporary: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	bool entityAlreadyDeclaredInThisContext;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed

	bool hasAssociatedInstanceTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed

	bool isQuery;
	bool isWhichQuery;
	bool isAnswerToQuery;				
	#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES				
	bool isAnswerContextToQuery;
	#endif
	bool testedForQueryComparison;
	
	
	bool negative;	//for prepositional entities which will be collapsed into conditions only [in the future, this should also be used for properties and actions; but relex does not appear to output this information]
	
	bool disableParsingAsAPrepositionRelationTemp;
	
	bool queryEntityTraced;	//temporary for determining max confifence
	
	bool disabled;	//if true, this entity has been deemed redundant in semantic network generation

	bool CXLdummyNode;
	
};

void disconnectNodeFromAllButDefinitions(GIAEntityNode * entityNode);

/*
class GIAEntityNodeContainer
{
public:

	GIAEntityNodeContainer(void);
	~GIAEntityNodeContainer(void);
	
	//flat tree structures are not used - this is only used for the semanticNet xml parse (read) process;		
	GIAEntityNode * node;
	GIAEntityNodeContainer * next;
	long id;	
	
}
*/

int calculateQuantityNumberInt(string quantityNumberString);
int calculateQuantityModifierInt(string quantityModifierString);
int calculateQuantityMultiplierInt(string quantityMultiplierString);

string printQuantityNumberString(GIAEntityNode * entityNode);



#endif

