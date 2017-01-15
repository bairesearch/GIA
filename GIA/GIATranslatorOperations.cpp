/*******************************************************************************
 *
 * File Name: GIATranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"


static string featureQueryWordAcceptedByAlternateMethodNameArray[FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES] = {"which"};

static string relationTypePropositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {"in", "on", "after", "ago", "before", "between", "by", "during", "for", "to", "till", "until", "past", "since", "up_to", "within", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
	//http://www.englisch-hilfen.de/en/grammar/preposition_time.htm + is [time is] etc
static string relationTypePropositionLocationNameArray[RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES] = {"in", "on", "at", "by", "near", "nearby", "above", "below", "over", "under", "around", "through", "inside", "inside_of", "outside", "between", "beside", "beyond", "in_front_of", "in_front", "in_back_of", "behind", "next_to", "on_top_of", "within", "beneath", "underneath", "among", "along", "against", "before", "after", "behind", "to", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};		
	//http://www.eslgold.com/grammar/prepositions_location.html + before, after, behind, to, etc
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
//static string relationTypePropositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, "after", "ago", "before", "between", "during", "for", "till", "until", "past", "since", "up_to", "within", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
//static string relationTypePropositionLocationNameArray[RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, "near", "nearby", "above", "below", "over", "under", "around", "through", "inside", "inside_of", "outside", "between", "beside", "beyond", "in_front_of", "in_front", "in_back_of", "behind", "next_to", "on_top_of", "within", "beneath", "underneath", "among", "along", "against", "before", "after", "behind", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};		

static string relationTypePropositionReductionNameArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES][RELATION_TYPE_PREPOSITION_REDUCTION_MAX_NUMBER_VARIATIONS] = {{"at", "in", "to", "on"}, {"from", "of", "by", "", }, {"for", "since", "", ""}};
static int relationTypePropositionReductionNumberVariationsArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_NUMBER_OF_TYPES, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_NUMBER_OF_TYPES, RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_NUMBER_OF_TYPES};
static string relationTypePropositionReductionReplacementNamesArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_REPLACEMENT_STRING};
#endif

static string relationTypePropositionReasonOrCircumstanceNameArray[RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES] = {"because", "on_account_of", "for", "out_of", "when",  REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY};
	//http://vlc.polyu.edu.hk/vlc/GrammarCourse/Lesson2_Preposition/CausePurpose.htm
static string relationContextPropositionTimeNameArray[REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES] = {"time", "period", "era", "millenia", "decade", "day", "month", "year", "hour", "minute", "second", "millisecond", "microsecond", "nanosecond", "picosecond"};
static string relationContextPropositionLocationNameArray[REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES] = {"location", "place", "position"};	//coordinates?
static string relationContextPropositionReasonNameArray[REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES] = {"reason", "basis", "argument"};

static string relationContextNegativeNameArray[RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES][GIA_NUMBER_OF_DEPENDENCY_RELATION_FORMATIONS] = {{RELEX_RELATION_TYPE_NEGATIVE_CONTEXT_1}, {STANFORD_RELATION_TYPE_NEGATIVE_CONTEXT_1}};

#ifdef GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
static string relationTypeConjugationNameArray[RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES] = {RELATION_TYPE_CONJUGATION_AND, RELATION_TYPE_CONJUGATION_OR};
#ifdef GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
static string relationTypeConjugationBasicNameArray[RELATION_TYPE_CONJUGATION_BASIC_NUMBER_OF_TYPES] = {RELATION_TYPE_CONJUGATION_AND_BASIC, RELATION_TYPE_CONJUGATION_OR_BASIC};
#endif
#endif
	
static string relationTypeObjectNameArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT_THAT};
static string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_SUBJECT_EXPLETIVE};
static string relationTypeAdjectiveNameArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_1, RELATION_TYPE_ADJECTIVE_2, RELATION_TYPE_ADJECTIVE_3};
static string relationTypePossessiveNameArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE, RELATION_TYPE_PRENOMIAL_MODIFIER};

static string relationFunctionCompositionNameArray[RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_COMPOSITION_1, RELATION_FUNCTION_COMPOSITION_2, RELATION_FUNCTION_COMPOSITION_3, RELATION_FUNCTION_COMPOSITION_4};
static string relationFunctionDefinitionNameArray[RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_DEFINITION_1};

static string relationTypeObjectSpecialConditionMeasureDistanceNameArray[RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE};
static string relationTypeObjectSpecialConditionToDoPropertyNameArray[RELATION_TYPE_OBJECT_SPECIAL_TO_DO_PROPERTY_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_DO};
static string relationTypeObjectSpecialConditionToBePropertyNameArray[RELATION_TYPE_OBJECT_SPECIAL_TO_BE_PROPERTY_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_BE};

static string relationTypeComplementsNameArray[RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES] = {RELEX_RELATION_TYPE_COMPLIMENT_TO_BE, RELEX_RELATION_TYPE_COMPLIMENT_TO_DO};

static int referenceTypeHasDeterminateCrossReferenceNumberArray[GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_SINGULAR};
static string relationTypeAdjectiveWhichImplyEntityInstanceNameArray[RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_1, RELATION_TYPE_ADJECTIVE_3};
static string relationTypeRequireSwitchingNameArray[RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT_THAT};
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_PROPERTY
static string relationTypeTreatAsPronounIeProperty[RELATION_TYPE_TREAT_AS_PRONOUN_IE_PROPERTY_NUMBER_OF_TYPES] = {"that"};
#endif

static string relationTypeQuantityOrMeasureNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY, RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME};
static string relationTypeQuantityOrMeasureSwitchedNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY_MOD, RELATION_TYPE_MEASURE_PER};
static string relationTypeMeasureNameArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME};
static int relationTypeMeasureNameTypeIndexArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {MEASURE_TYPE_DISTANCE, MEASURE_TYPE_PER, MEASURE_TYPE_SIZE, MEASURE_TYPE_TIME};

static string relationTypeQuantityArgumentImplyMeasurePerNameArray[RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES] = {"every"};

//static int timeMonthIntArray[TIME_MONTH_NUMBER_OF_TYPES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static string timeMonthStringArray[TIME_MONTH_NUMBER_OF_TYPES] = {TIME_MONTH_JANUARY, TIME_MONTH_FEBRUARY, TIME_MONTH_MARCH, TIME_MONTH_APRIL, TIME_MONTH_MAY, TIME_MONTH_JUNE, TIME_MONTH_JULY, TIME_MONTH_AUGUST, TIME_MONTH_SEPTEMBER, TIME_MONTH_OCTOBER, TIME_MONTH_NOVEMBER, TIME_MONTH_DECEMBER};
	

static string referenceTypePossessiveNameArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {"undefined", "his", "her", "them", "its"};
//static int referenceTypePossessiveNameLengthsArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {9, 3, 3, 4, 3};
static string referenceTypePersonNameArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {"undefined", "he", "she", "they", "it"};
//static int referenceTypePersonNameLengthsArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {9, 2, 3, 4, 2};

int referenceTypePersonCrossReferenceNumberArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_UNDEFINED, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_PLURAL, GRAMMATICAL_NUMBER_SINGULAR};
int referenceTypePersonCrossReferenceGenderArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_MASCULINE, GRAMMATICAL_GENDER_FEMININE, GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_UNDEFINED};
bool referenceTypePersonCrossReferenceDefiniteArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {false, true, true, true, true};
bool referenceTypePersonCrossReferencePersonArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON_UNDEFINED};

static long currentEntityNodeIDInCompleteList;
static vector<GIAEntityNode*> * entityNodesCompleteList;
static long currentEntityNodeIDInConceptEntityNodesList;
//static vector<GIAEntityNode*> * conceptEntityNodesList;
static long currentEntityNodeIDInPropertyEntityNodesList;
static vector<GIAEntityNode*> * propertyEntityNodesList;
static long currentEntityNodeIDInActionEntityNodesList;
static vector<GIAEntityNode*> * actionEntityNodesList;
static long currentEntityNodeIDInConditionEntityNodesList;
static vector<GIAEntityNode*> * conditionEntityNodesList;

static bool foundComparisonVariable;
static GIAEntityNode* comparisonVariableNode;



bool isAdjectiveNotAnAdvmodAndRelationFunctionIsNotBe(Relation * currentRelationInList, GIAEntityNode * GIAEntityNodeArray[], int relationFunctionIndex)
{
	bool result = true;
	
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_STANFORD)
	{
	#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
		if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_3) && (GIAEntityNodeArray[relationFunctionIndex]->entityName == RELATION_FUNCTION_DEFINITION_1))
		{//added condition Fri 27 Jan - remove 'be' node artefacts
			#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
			GIAEntityNodeArray[relationFunctionIndex]->disabled = true;
			#endif		
			result = false;
			//cout << "GIAEntityNodeArray[relationFunctionIndex]->disabled = true" << endl;		
		}
		#endif
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_CODE	
	}
	#endif
	
	return result;
}


void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity)
{
	
	if(propertyEntity->entityAlreadyDeclaredInThisContext)
	{
		if(!(propertyEntity->hasAssociatedInstanceTemp))
		{
			propertyEntity->hasAssociatedInstanceTemp = true;
		}	
		//cout << "1. propertyEntity->entityName = " << propertyEntity->entityName << endl; 
	}

	if(thingEntity->entityAlreadyDeclaredInThisContext)
	{
		if(!(thingEntity->hasAssociatedInstanceTemp))
		{
			thingEntity->hasAssociatedInstanceTemp = true;
		}	
	}
			
	if(propertyEntity->hasAssociatedInstanceTemp)
	{
		GIAEntityNode * existingProperty  = propertyEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a

		/*
		if(propertyEntity->hasQualityTemp)
		{
			existingProperty->hasQuality = true;
		}
		*/
			
		if(thingEntity->hasAssociatedInstanceTemp)
		{		
			thingEntity = thingEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
		}

		//configure entity node containing this property
		thingEntity->PropertyNodeList.push_back(existingProperty);	

		thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		existingProperty->PropertyNodeReverseList.push_back(thingEntity);	
		//existingProperty->entityNodeContainingThisProperty = thingEntity;		//added 26 Aug 11a, removed 8 Dec 2011
		
	}
	else
	{
		if(thingEntity->hasAssociatedInstanceTemp)
		{	
			//cout << "thingEntity = " << thingEntity->entityName << endl;
			//cout << "propertyEntity = " << propertyEntity->entityName << endl;
				
			thingEntity = thingEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
		}
		
		//cout << "thingEntity = " << thingEntity->entityName << endl;
		
		GIAEntityNode * newProperty = addProperty(propertyEntity);		

		/*
		if(propertyEntity->hasQualityTemp)
		{
			newProperty->hasQuality = true;
		}
		*/

		newProperty->PropertyNodeReverseList.push_back(thingEntity);
		//newProperty->entityNodeContainingThisProperty = thingEntity;		//added 26 Aug 11a, removed 8 Dec 2011
		
		//configure entity node containing this property
		thingEntity->PropertyNodeList.push_back(newProperty);		

		thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	}	
}


void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity)
{	
	//cout << "\t\t addPropertyToPropertyDefinition; propertyEntity->entityName = " << propertyEntity->entityName << endl;
	
	GIAEntityNode * newOrExistingProperty;
	
	if(propertyEntity->entityAlreadyDeclaredInThisContext)
	{
		if(!(propertyEntity->hasAssociatedInstanceTemp))
		{
			propertyEntity->hasAssociatedInstanceTemp = true;
		}	
	}
	
	if(propertyEntity->hasAssociatedInstanceTemp)
	{	
		//cout << "break; propertyEntity->entityName = " << propertyEntity->entityName << endl;
		GIAEntityNode * newOrExistingProperty = propertyEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
		
	}
	else
	{	
		GIAEntityNode * newProperty = addProperty(propertyEntity);	
	}	
}


GIAEntityNode * addProperty(GIAEntityNode * propertyEntity)
{	
	//configure property node
	GIAEntityNode * newProperty = new GIAEntityNode();
	newProperty->id = currentEntityNodeIDInCompleteList;
	newProperty->idSecondary = currentEntityNodeIDInPropertyEntityNodesList;

	entityNodesCompleteList->push_back(newProperty);
	currentEntityNodeIDInCompleteList++;
	propertyEntityNodesList->push_back(newProperty);
	currentEntityNodeIDInPropertyEntityNodesList++;

	newProperty->entityName = propertyEntity->entityName;
	newProperty->isProperty = true;
	//newProperty->entityNodeContainingThisProperty = NULL;
	newProperty->entityNodeDefiningThisInstance = propertyEntity;
	propertyEntity->hasAssociatedInstance = true;
	propertyEntity->hasAssociatedInstanceTemp = true;	////temporary: used for GIA translator only - overwritten every time a new sentence is parsed

	//if(propertyEntity->grammaticalNumberTemp > GRAMMATICAL_NUMBER_SINGULAR)
	//{
	newProperty->grammaticalNumber = propertyEntity->grammaticalNumberTemp;
	//}

	if(propertyEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)
	{
		newProperty->hasProgressiveTemp = true;
		//cout << "property has progressive (eg lying/sitting/being happy)" << endl;
	}
		
	if(propertyEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || newProperty->hasProgressiveTemp)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToProperty(newProperty, propertyEntity->grammaticalTenseTemp, newProperty->hasProgressiveTemp);
	}

	//configure property definition node
	propertyEntity->AssociatedInstanceNodeList.push_back(newProperty);

	propertyEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	
	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	//cout << "propertyEntity->entityName = " << propertyEntity->entityName << endl;
	if(propertyEntity->isQuery)
	{
		propertyEntity->isQuery = false;
		newProperty->isQuery = true;
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(propertyEntity->isWhichQuery)
		{
			propertyEntity->isWhichQuery = false;
			newProperty->isWhichQuery = true;			
		}
		#endif
	}
	#endif	
				
	return newProperty;	
}


GIAEntityNode * addActionToActionDefinition(GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction;
	
	if(actionEntity->entityAlreadyDeclaredInThisContext)
	{//CHECK THIS; need to convert to action node here also? ie hasAssociatedInstanceIsAction->true? [must look at generated semanticNet.xml and see if any propertyNodeContainers contain action nodes..., or if any actionNodeContainers do not contain property nodes...]
		if(!(actionEntity->hasAssociatedInstanceTemp))
		{	
			//cout << "actionEntity->entityAlreadyDeclaredInThisContext as a property:" << actionEntity->entityName << endl;
			actionEntity->hasAssociatedInstanceTemp = true;
		}
	}
			
	//configure action node	
	if(actionEntity->hasAssociatedInstanceTemp)
	{
		newOrExistingAction = actionEntity->AssociatedInstanceNodeList.back();	
		
		if(actionEntity->hasAssociatedInstanceIsAction == false)
		{//upgrade associated property to action
			
			//CHECK THIS; must remove from property list, and add to action list 
			actionEntity->hasAssociatedInstanceIsAction = true;
			newOrExistingAction->isProperty = false;
			newOrExistingAction->isAction = true;
			
			//cout << "newOrExistingAction->idSecondary = " << newOrExistingAction->idSecondary << endl;
			
			vector<GIAEntityNode*>::iterator propertyEntityNodesListIterator = propertyEntityNodesList->begin();
			advance(propertyEntityNodesListIterator,newOrExistingAction->idSecondary);
			propertyEntityNodesList->erase(propertyEntityNodesListIterator);
			currentEntityNodeIDInPropertyEntityNodesList--;
			
			actionEntityNodesList->push_back(newOrExistingAction);
			currentEntityNodeIDInActionEntityNodesList++;
		}
		#ifdef USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_CONCEPT_ENTITY_IN_A_GIVEN_SENTENCE
		else
		{
			newOrExistingAction = addAction(actionEntity);
		}
		#endif
	}
	else
	{
		newOrExistingAction = addAction(actionEntity);
	}
	
	return newOrExistingAction;
}


	//conditions required to be added [eg when, where, how, why]
GIAEntityNode * addAction(GIAEntityNode * actionEntity)
{				
	//cout << "as4" << endl;

	GIAEntityNode * newAction = new GIAEntityNode();
	newAction->id = currentEntityNodeIDInCompleteList;
	newAction->idSecondary = currentEntityNodeIDInActionEntityNodesList;

	entityNodesCompleteList->push_back(newAction);
	currentEntityNodeIDInCompleteList++;
	actionEntityNodesList->push_back(newAction);
	currentEntityNodeIDInActionEntityNodesList++;

	newAction->entityName = actionEntity->entityName;		
	newAction->entityNodeDefiningThisInstance = actionEntity;

	//cout << "as4b" << endl;

	actionEntity->AssociatedInstanceNodeList.push_back(newAction);
	actionEntity->hasAssociatedInstance = true;
	actionEntity->hasAssociatedInstanceIsAction = true;
	actionEntity->hasAssociatedInstanceTemp = true;
	newAction->isAction = true;
	//WHY WOULD THIS EVER BE REQURIED?; newAction->entityNodeContainingThisProperty = NULL;

	if(actionEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)
	{
		newAction->hasProgressiveTemp = true;
		//cout << "property has progressive (eg lying/sitting/being happy)" << endl;			
	}

	//cout << "actionEntity->grammaticalTenseTemp = " << actionEntity->grammaticalTenseTemp << endl;
	//cout << "actionEntity->entityName = " << actionEntity->entityName << endl;

	if(actionEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || newAction->hasProgressiveTemp)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		//cout << "hello" << endl;
		//exit(0);
		addTenseOnlyTimeConditionToProperty(newAction, actionEntity->grammaticalTenseTemp, newAction->hasProgressiveTemp);
	}	

	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	if(actionEntity->isQuery)
	{
		actionEntity->isQuery = false;
		newAction->isQuery = true;
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(actionEntity->isWhichQuery)
		{
			actionEntity->isWhichQuery = false;
			newAction->isWhichQuery = true;			
		}
		#endif
	}
	#endif	

	actionEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed

	//cout << "as5" << endl;	

	return newAction;
}







/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList)
#else
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense)
#endif
*/
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, bool isProgressive)
{
	propertyNode->conditionType = CONDITION_NODE_TYPE_TIME;
	
	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = 0; //cannot assign absolute time to an event which occurs in the past.... //calculateTotalTimeInSeconds();
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/
	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	
	newTimeCondition->tense = tense;
	newTimeCondition->conditionName = grammaticalTenseNameArray[tense];
	if(isProgressive)
	{
		newTimeCondition->isProgressive = true;
	}	
	propertyNode->timeConditionNode = newTimeCondition;
	

}

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity)
{
	if(thingEntity->hasAssociatedInstanceTemp)
	{
		//cout << "thingEntity = thingEntity->AssociatedInstanceNodeList.back()" << endl;
		thingEntity = thingEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
	}
	
	if(definitionEntity->hasAssociatedInstanceTemp)
	{
		//cout << "definitionEntity = definitionEntity->AssociatedInstanceNodeList.back()" << endl;
		definitionEntity = definitionEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
	}
	
	//configure entity node
	thingEntity->EntityNodeDefinitionList.push_back(definitionEntity);
	
	//configure entity definition node
	definitionEntity->EntityNodeDefinitionReverseList.push_back(thingEntity);		
}

	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction;
	newOrExistingAction = addActionToActionDefinition(actionEntity);
	
	addActionInstanceToSubject(subjectEntity, newOrExistingAction);

	addActionInstanceToObject(objectEntity, newOrExistingAction);
}


void addActionInstanceToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * newOrExistingAction)
{	
	if(subjectEntity->hasAssociatedInstanceTemp)
	{
		subjectEntity = subjectEntity->AssociatedInstanceNodeList.back();
	}
	
	//configure subject entity node
	subjectEntity->ActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionSubjectEntity = subjectEntity;
	//cout << "BUG SHOULD BE JOE; subjectEntity->entityName = " << subjectEntity->entityName << endl;
	
	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}


void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity)
{	
	GIAEntityNode * newOrExistingAction;
	newOrExistingAction = addActionToActionDefinition(actionEntity);
	addActionInstanceToSubject(subjectEntity, newOrExistingAction);	
}

void addActionInstanceToObject(GIAEntityNode * objectEntity, GIAEntityNode * newOrExistingAction)
{		
	if(objectEntity->hasAssociatedInstanceTemp)
	{
		objectEntity = objectEntity->AssociatedInstanceNodeList.back();
	}
		
	//configure object entity node
	objectEntity->IncomingActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionObjectEntity = objectEntity;
	
	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction;
	newOrExistingAction = addActionToActionDefinition(actionEntity);
	addActionInstanceToObject(objectEntity, newOrExistingAction);	
}


void addOrConnectPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, GIAEntityNode * conditionTypeConceptEntity)
{
	if(entityNode->hasAssociatedInstanceTemp)
	{
		entityNode = entityNode->AssociatedInstanceNodeList.back();
	}
	if(conditionEntityNode->hasAssociatedInstanceTemp)
	{
		conditionEntityNode = conditionEntityNode->AssociatedInstanceNodeList.back();
	}										
	addConditionToProperty(entityNode, conditionEntityNode, conditionTypeConceptEntity);
}

void addOrConnectBeingDefinitionConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionDefinitionNode, GIAEntityNode * conditionTypeConceptEntity, bool negative)
{
	if(entityNode->hasAssociatedInstanceTemp)
	{
		entityNode = entityNode->AssociatedInstanceNodeList.back();
	}
	if(conditionDefinitionNode->hasAssociatedInstanceTemp)
	{
		conditionDefinitionNode = conditionDefinitionNode->AssociatedInstanceNodeList.back();
	}
	
	GIAEntityNode * newCondition = addCondition(conditionTypeConceptEntity);
	newCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node
	//cout << "negative = " << negative;
	
	newCondition->conditionSubjectEntity = entityNode;	
	entityNode->ConditionNodeList.push_back(newCondition);
	
	newCondition->EntityNodeDefinitionList.push_back(conditionDefinitionNode);
	conditionDefinitionNode->EntityNodeDefinitionReverseList.push_back(newCondition);		
}

void addOrConnectHavingPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionPropertyNode, GIAEntityNode * conditionTypeConceptEntity, bool negative)
{
	if(entityNode->hasAssociatedInstanceTemp)
	{
		entityNode = entityNode->AssociatedInstanceNodeList.back();
	}
	if(conditionPropertyNode->hasAssociatedInstanceTemp)
	{
		conditionPropertyNode = conditionPropertyNode->AssociatedInstanceNodeList.back();
	}	
	
	GIAEntityNode * newCondition = addCondition(conditionTypeConceptEntity);
	newCondition->negative = negative;	//overwrite negative with correct one from context; ie that from "having" entity node
	
	newCondition->conditionSubjectEntity = entityNode;
	entityNode->ConditionNodeList.push_back(newCondition);
	
	conditionPropertyNode->PropertyNodeReverseList.push_back(newCondition);
	//conditionPropertyNode->entityNodeContainingThisProperty = newCondition;	//added 26 Aug 11a, removed 8 Dec 2011
	
	newCondition->PropertyNodeList.push_back(conditionPropertyNode);	
}


void addConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
{	
	GIAEntityNode * newCondition = addCondition(conditionTypeConceptEntity);
	
	newCondition->conditionSubjectEntity = propertyNode;
	newCondition->conditionObjectEntity = propertyConditionEntity;
		
	propertyNode->ConditionNodeList.push_back(newCondition);
	propertyConditionEntity->IncomingConditionNodeList.push_back(newCondition);
}

GIAEntityNode * addCondition(GIAEntityNode * conditionEntity)
{
	GIAEntityNode * newCondition = new GIAEntityNode();
	newCondition->id = currentEntityNodeIDInCompleteList;
	newCondition->idSecondary = currentEntityNodeIDInConditionEntityNodesList;

	entityNodesCompleteList->push_back(newCondition);
	currentEntityNodeIDInCompleteList++;
	conditionEntityNodesList->push_back(newCondition);
	currentEntityNodeIDInConditionEntityNodesList++;

	newCondition->entityName = conditionEntity->entityName;		
	newCondition->entityNodeDefiningThisInstance = conditionEntity;

	conditionEntity->AssociatedInstanceNodeList.push_back(newCondition);
	conditionEntity->hasAssociatedInstance = true;
	conditionEntity->hasAssociatedInstanceIsCondition = true;
	conditionEntity->hasAssociatedInstanceTemp = true;
	newCondition->isCondition = true;
	newCondition->negative = conditionEntity->negative;
	
	return newCondition;	
}



void setTranslatorEntityNodesCompleteList(vector<GIAEntityNode*> * newEntityNodesCompleteList)
{
	entityNodesCompleteList = newEntityNodesCompleteList;
}
/*
void setTranslatorConceptEntityNodesList(vector<GIAEntityNode*> * newConceptEntityNodesList)
{
	conceptEntityNodesList = newConceptEntityNodesList;
}
*/
void setTranslatorPropertyEntityNodesList(vector<GIAEntityNode*> * newPropertyEntityNodesList)
{
	propertyEntityNodesList = newPropertyEntityNodesList;
}
void setTranslatorActionEntityNodesList(vector<GIAEntityNode*> * newActionEntityNodesList)
{
	actionEntityNodesList = newActionEntityNodesList;
}
void setTranslatorConditionEntityNodesList(vector<GIAEntityNode*> * newConditionEntityNodesList)
{
	conditionEntityNodesList = newConditionEntityNodesList;
}

	
bool getFoundComparisonVariable()
{
	return foundComparisonVariable;
}
GIAEntityNode* getComparisonVariableNode()
{
	return comparisonVariableNode;
}

