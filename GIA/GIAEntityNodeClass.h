/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
 * NB a property is an instance of an entity, any given entity may contain/comprise/have multiple properties - and properties are unrelated to definitions between entities [they just define what comprises any given entity]
 *
 *******************************************************************************/


#ifndef HEADER_GIA_ENTITY_NODE_CLASS
#define HEADER_GIA_ENTITY_NODE_CLASS


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


class GIAActionNode;
class GIALocationConditionNode;
class GIAPropertyConditionNode;
class GIAActionConditionNode;
class GIATimeConditionNode;

#define GRAMMATICAL_TENSE_UNDEFINED 0
#define GRAMMATICAL_TENSE_PRESENT 1
#define GRAMMATICAL_TENSE_PAST 2
#define GRAMMATICAL_TENSE_FUTURE 3
#define GRAMMATICAL_TENSE_NUMBER_OF_TYPES (4)
#define GRAMMATICAL_NUMBER_UNDEFINED 0
#define GRAMMATICAL_NUMBER_UNCOUNTABLE 1
#define GRAMMATICAL_NUMBER_SINGULAR 2
#define GRAMMATICAL_NUMBER_PLURAL 3
#define GRAMMATICAL_NUMBER_NUMBER_OF_TYPES (4)
#define GRAMMATICAL_DEFINITE_UNDEFINED false
#define GRAMMATICAL_DEFINITE true
#define GRAMMATICAL_DEFINITE_NAME "definite"

extern string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES];
extern int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES];
extern string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES];
extern int grammaticalNumberNameLengthsArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES];

class GIAEntityNode
{
public:

	GIAEntityNode(void);
	~GIAEntityNode(void);

	int grammaticalTenseTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalNumberTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool definiteTemp; 		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool hasAssociatedPropertyTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool hasAssociatedActionTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	
	int grammaticalNumber; 
	
	bool initialisedForPrinting;
	bool printed;
	int printX;
	int printY;
	int printXIndex;
	int printYIndex;
	int printTextX;
	int printTextY;
	
	string entityName;
	double confidence;
	
	int isProperty;		//is this entity a property?
	bool hasAssociatedProperty;	//this boolean appears to only represent whether this entity defines a child property node [and not whether it contains one]
	bool hasAssociatedAction;
	bool hasAssociatedTime;
	
	enum
	{
		definesAThingNoun, definesAPropertyAdjective, definesAnActionVerb, undefinedEntityType
	}type;	//is the following entity known to be an instance?		
	
	
	/*instances are now arbitrary, every entity is an instance of its parent(s) in some form or another...
	enum
	{
		isinstance, notinstance, undefinedInstance
	}instance;	//is the following entity known to be an instance?		
	*/

	//action connections;
	//where this entity is the subject of the action
	vector<GIAActionNode*> firstActionNodeInList;
	vector<GIAActionNode*>::iterator firstActionNodeInListIterator;
	//where this entity is the object of the action
	vector<GIAActionNode*> firstIncomingActionNodeInList;
	vector<GIAActionNode*>::iterator firstIncomingActionNodeInListIterator;
	
	//property connections;
	//record list of all properties for this entity
	vector<GIAEntityNode*> firstPropertyNodeInList;
	vector<GIAEntityNode*>::iterator firstPropertyNodeInListIterator;
	vector<GIAEntityNode*> firstPropertyNodeInReverseList;				//if property only: 
	vector<GIAEntityNode*>::iterator firstPropertyNodeInReverseListIterator;	//if property only:
	GIAEntityNode * entityNodeContainingThisProperty;				//if property only:	//eg, Tom; OR;  Tom's Assets	//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	GIAEntityNode * entityNodeDefiningThisProperty;					//if property only:					//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	
	//entity connections;										
	//record parent and child entity definition nodes
	vector<GIAEntityNode*> firstEntityNodeInDefinitionList;				//if not property only: 	//this should logically reduce to a single entity, although not required, therefore it is a vector [eg, a dog is a mammal, which is an animal, but a dog is an animal also]
	vector<GIAEntityNode*>::iterator firstDefinitionEntityNodeInListIterator;	//if not property only: 
	vector<GIAEntityNode*> firstEntityNodeInDefinitionReverseList;			//if not property only: 	//more than one entity can be defined by this entity [eg if this entity is "animal", a bird is an animal, a mammal is an animal, etc]
	vector<GIAEntityNode*>::iterator firstDefinitionEntityNodeInReverseListIterator;//if not property only: 
	
	//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]
	vector<GIAActionNode*> firstAssociatedActionNodeInList;				//if not property only: if type == definesAnActionVerb
	vector<GIAActionNode*>::iterator firstAssociatedActionNodeInListIterator;	//if not property only: if type == definesAnActionVerb
	vector<GIAEntityNode*> firstAssociatedPropertyNodeInList;			//if not property only: if type == definesAPropertyAdjective (ie, if this entity is not a property/instance but defines one or more properties/instances)
	vector<GIAEntityNode*>::iterator firstAssociatedPropertyNodeInListIterator;	//if not property only: if type == definesAPropertyAdjective (ie, if this entity is not a property/instance but defines one or more properties/instances)
	
	//conditions connections: conditions and reverse conditions (reason) lookups [condition and reason respectively]
	vector<GIATimeConditionNode*> firstTimeConditionNodeInList;
	vector<GIATimeConditionNode*>::iterator firstTimeConditionNodeInListIterator;		
	vector<GIATimeConditionNode*> firstTimeConditionNodeInReverseList;
	vector<GIATimeConditionNode*>::iterator firstTimeConditionNodeInReverseListIterator;		
	vector<GIALocationConditionNode*> firstLocationConditionNodeInList;	
	vector<GIALocationConditionNode*>::iterator firstLocationConditionNodeInListIterator;		
	vector<GIALocationConditionNode*> firstLocationConditionNodeInReverseList;	
	vector<GIALocationConditionNode*>::iterator firstLocationConditionNodeInReverseListIterator;	
	vector<GIAPropertyConditionNode*> firstPropertyConditionNodeInList;				//this property requires the following properties 
	vector<GIAPropertyConditionNode*>::iterator firstPropertyConditionNodeInListIterator;		//this property requires the following properties	
	vector<GIAPropertyConditionNode*> firstPropertyConditionNodeInReverseList;			//this property is required by the following properties	//aka reason [a property needs this property]
	vector<GIAPropertyConditionNode*>::iterator firstPropertyConditionNodeInReverseListIterator;	//this property is required by the following properties	//aka reason [a property needs this property]
	vector<GIAActionConditionNode*> firstActionConditionNodeInList;					//this property requires the following actions 
	vector<GIAActionConditionNode*>::iterator firstActionConditionNodeInListIterator;		//this property requires the following actions	
	vector<GIAActionConditionNode*> firstActionConditionNodeInReverseList;				//this property is required by the following actions	//aka reason [an action needs this Action]
	vector<GIAActionConditionNode*>::iterator firstActionConditionNodeInReverseListIterator;	//this property is required by the following actions	//aka reason [an action needs this Action]
	
	
	/*flat tree structures are not used			
	//this MAY NEED to be replaced with a vector of entity node pointers
	GIAEntityNode * next;
	*/
};





#endif

