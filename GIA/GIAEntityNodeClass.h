/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b8b 22-Sept-11
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
class GIAConditionNode;

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
#define GRAMMATICAL_PERSON_UNDEFINED false
#define GRAMMATICAL_PERSON true
#define GRAMMATICAL_PERSON_NAME "person"
#define GRAMMATICAL_GENDER_UNDEFINED 0
#define GRAMMATICAL_GENDER_PERSON 1
#define GRAMMATICAL_GENDER_MASCULINE 2
#define GRAMMATICAL_GENDER_FEMININE 3
#define GRAMMATICAL_GENDER_NUMBER_OF_TYPES (4)
#define GRAMMATICAL_CATEGORY_UNDEFINED (0)
//#define GRAMMATICAL_COUNT_UNDEFINED 0


#define QUANTITY_NUMBER_UNDEFINED (1)
#define QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES (20)
#define QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES (10)
#define QUANTITY_MULTIPLIER_NUMBER_OF_TYPES (7)		//do: work out what these are/can be
#define QUANTITY_MODIFIER_UNDEFINED (-1)		//WARNING: not yet implemented
#define QUANTITY_MODIFIER_NUMBER_OF_TYPES (1)		//WARNING: not yet implemented	//do: work out what these are/can be	

#define MEASURE_TYPE_UNDEFINED (-1)

extern string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES];
extern int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES];
extern string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES];
extern int grammaticalNumberNameLengthsArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES];
extern string grammaticalGenderNameArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES];
extern int grammaticalGenderNameLengthsArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES];




class GIAEntityNode
{
public:

	GIAEntityNode(void);
	~GIAEntityNode(void);

	int grammaticalTenseTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalNumberTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalDefiniteTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalPersonTemp;	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed		
	int grammaticalGenderTemp; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	//bool grammaticalCountTemp;	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed		
	bool isSubjectTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool isObjectTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool hasPropertyTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	
	//bool isReferenceEntityInThisSentence;	//temporary: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	bool entityAlreadyDeclaredInThisContext;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed

	bool hasAssociatedPropertyTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool hasAssociatedActionTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	
	int grammaticalNumber;
	
	bool hasQuantity;
	int quantityNumber;
	int quantityModifier;	//not yet implemented
	string quantityModifierString;	//eg "almost"
	bool hasMeasure;
	int measureType;
	
		
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

	//action connections;
	//where this entity is the subject of the action
	vector<GIAActionNode*> ActionNodeList;
	vector<GIAActionNode*>::iterator ActionNodeListIterator;
	//where this entity is the object of the action
	vector<GIAActionNode*> IncomingActionNodeList;
	vector<GIAActionNode*>::iterator IncomingActionNodeListIterator;
	
		//properties only
	//property connections;
	//record list of all properties for this entity
	vector<GIAEntityNode*> PropertyNodeList;
	vector<GIAEntityNode*>::iterator PropertyNodeListIterator;
	/*this has been removed 25 Sept - use entityNodeContainingThisProperty instead
	vector<GIAEntityNode*> PropertyNodeReverseList;				//if property only: 
	vector<GIAEntityNode*>::iterator PropertyNodeReverseListIterator;	//if property only:
	*/
	GIAEntityNode * entityNodeContainingThisProperty;				//if property only:	//eg, Tom; OR;  Tom's Assets	//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	GIAEntityNode * entityNodeDefiningThisProperty;					//if property only:					//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	
		//pure entites only (not properties/"instances" of entities)
	//entity connections;										
	//record parent and child entity definition nodes
	vector<GIAEntityNode*> EntityNodeDefinitionList;				//if not property only: 	//this should logically reduce to a single entity, although not required, therefore it is a vector [eg, a dog is a mammal, which is an animal, but a dog is an animal also]
	vector<GIAEntityNode*>::iterator EntityNodeDefinitionListIterator;	//if not property only: 
	vector<GIAEntityNode*> EntityNodeDefinitionReverseList;			//if not property only: 	//more than one entity can be defined by this entity [eg if this entity is "animal", a bird is an animal, a mammal is an animal, etc]
	vector<GIAEntityNode*>::iterator EntityNodeDefinitionReverseListIterator;	//if not property only: 
	//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]
	vector<GIAActionNode*> AssociatedActionNodeList;				//if not property only: if type == definesAnActionVerb
	vector<GIAActionNode*>::iterator AssociatedActionNodeListIterator;	//if not property only: if type == definesAnActionVerb
	vector<GIAEntityNode*> AssociatedPropertyNodeList;			//if not property only: if type == definesAPropertyAdjective (ie, if this entity is not a property/instance but defines one or more properties/instances)
	vector<GIAEntityNode*>::iterator AssociatedPropertyNodeListIterator;	//if not property only: if type == definesAPropertyAdjective (ie, if this entity is not a property/instance but defines one or more properties/instances)
	
	//conditions connections: conditions and reverse conditions (reason) lookups [condition and reason respectively]
	vector<GIAConditionNode*> ConditionNodeList;		//this property requires the following...
	vector<GIAConditionNode*>::iterator ConditionNodeListIterator;		
	vector<GIAConditionNode*> ConditionNodeReverseList;	//this property is required by the following... //aka reason	[NB these may only be property, location, {and time action condtions}, not action conditions]
	vector<GIAConditionNode*>::iterator ConditionNodeReverseListIterator;		
	
	//flat tree structures are not used - this is only used for the semanticNet xml parse (read) process;		
	GIAEntityNode * next;
	
};

int calculateQuantityNumberInt(string quantityNumberString);
int calculateQuantityModifierInt(string quantityModifierString);
int calculateQuantityMultiplierInt(string quantityMultiplierString);




#endif

