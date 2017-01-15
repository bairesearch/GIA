/*******************************************************************************
 *
 * File Name: GIATranslatorDefineGrammarAndReferencing.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorDefineGrammarAndReferencing.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"





void fillGrammaticalArrays(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int NLPparserType, int NLPdependencyRelationsType)
{
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
	{
		fillGrammaticalArraysRelex(currentSentenceInList, GIAEntityNodeIsDate, GIAEntityNodeGrammaticalTenseArray, GIAEntityNodeGrammaticalTenseModifierArray, GIAEntityNodeGrammaticalNumberArray, GIAEntityNodeGrammaticalIsDefiniteArray, GIAEntityNodeGrammaticalIsPersonArray, GIAEntityNodeGrammaticalGenderArray, GIAEntityNodeGrammaticalIsPronounArray);
	}
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_STANFORD)
	{
		fillGrammaticalArraysStanford(currentSentenceInList, GIAEntityNodeIsDate, GIAEntityNodeGrammaticalTenseArray, GIAEntityNodeGrammaticalTenseModifierArray, GIAEntityNodeGrammaticalNumberArray, GIAEntityNodeGrammaticalIsDefiniteArray, GIAEntityNodeGrammaticalIsPersonArray, GIAEntityNodeGrammaticalGenderArray, GIAEntityNodeGrammaticalIsPronounArray);
	}
	#endif
	
	#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
	if(NLPparserType == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		fillGrammaticalArraysStanfordCoreNLP(currentSentenceInList, GIAEntityNodeIsDate, GIAEntityNodeGrammaticalTenseArray, GIAEntityNodeGrammaticalTenseModifierArray, GIAEntityNodeGrammaticalNumberArray, GIAEntityNodeGrammaticalIsDefiniteArray, GIAEntityNodeGrammaticalIsPersonArray, GIAEntityNodeGrammaticalGenderArray, GIAEntityNodeGrammaticalIsPronounArray);
	}
	#endif
		
}

#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
void fillGrammaticalArraysStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[])
{

}
#endif

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void fillGrammaticalArraysStanford(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[])
{

}
#endif

#ifdef GIA_USE_RELEX
void fillGrammaticalArraysRelex(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[])
{
	Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		//cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;

		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_TENSE_DATE) != -1)
		{
			GIAEntityNodeIsDate[currentFeatureInList->entityIndex] = true;
			//cout << "isDate currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
		}

		for(int grammaticalTenseIndex = 0; grammaticalTenseIndex < GRAMMATICAL_TENSE_NUMBER_OF_TYPES; grammaticalTenseIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name 
			if((currentFeatureInList->grammar).find(grammaticalTenseNameArray[grammaticalTenseIndex]) != -1) 
			//if((currentFeatureInList->grammar).substr(0, grammaticalTenseNameLengthsArray[grammaticalTenseIndex]) == grammaticalTenseNameArray[grammaticalTenseIndex]) 
			{
				GIAEntityNodeGrammaticalTenseArray[currentFeatureInList->entityIndex] = grammaticalTenseIndex;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseIndex = " << grammaticalTenseNameArray[grammaticalTenseIndex] << endl;
			}			
		}
		for(int grammaticalTenseModifierIndex = 0; grammaticalTenseModifierIndex < GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name 
			if((currentFeatureInList->grammar).find(grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex]) != -1) 
			{
				//cout << "grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex] = " << grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex] << endl;
				GIAEntityNodeGrammaticalTenseModifierArray[currentFeatureInList->entityIndex*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + grammaticalTenseModifierIndex] = true;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseModifierIndex true = " << grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex] << endl;
			}			
		}
		
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_2A_GRAMMAR_TREAT_PRESENT_PERFECT_AS_PAST_TENSE
		//interpret "present_perfect" relex flag as past tense
		if(GIAEntityNodeGrammaticalTenseArray[currentFeatureInList->entityIndex] == GRAMMATICAL_TENSE_PRESENT)
		{
			//cout << "a" << endl;
			if(GIAEntityNodeGrammaticalTenseModifierArray[currentFeatureInList->entityIndex*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + GRAMMATICAL_TENSE_MODIFIER_PERFECT] == true)
			{
				//cout << "b" << endl;
				GIAEntityNodeGrammaticalTenseArray[currentFeatureInList->entityIndex] = GRAMMATICAL_TENSE_PAST;
			}		
		}
	#endif

		for(int grammaticalNumberIndex = 0; grammaticalNumberIndex < GRAMMATICAL_NUMBER_NUMBER_OF_TYPES; grammaticalNumberIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical number type name 
			if((currentFeatureInList->grammar).find(grammaticalNumberNameArray[grammaticalNumberIndex]) != -1) 				
			{
				GIAEntityNodeGrammaticalNumberArray[currentFeatureInList->entityIndex] = grammaticalNumberIndex;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalNumberIndex = " << grammaticalNumberNameArray[grammaticalNumberIndex] << endl;
			}			
		}
		if((currentFeatureInList->grammar).find(GRAMMATICAL_DEFINITE_NAME) != -1)
		{
			GIAEntityNodeGrammaticalIsDefiniteArray[currentFeatureInList->entityIndex] = GRAMMATICAL_DEFINITE;
			//cout << "isDefinite currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;

		}			

		if((currentFeatureInList->grammar).find(GRAMMATICAL_PERSON_NAME) != -1)
		{
			GIAEntityNodeGrammaticalIsPersonArray[currentFeatureInList->entityIndex] = GRAMMATICAL_PERSON;
			//cout << "isPerson currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;

		}
		for(int grammaticalGenderIndex = 0; grammaticalGenderIndex < GRAMMATICAL_GENDER_NUMBER_OF_TYPES; grammaticalGenderIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical Gender type name 
			if((currentFeatureInList->grammar).find(grammaticalGenderNameArray[grammaticalGenderIndex]) != -1) 				
			{
				//NB it will always find "person" in relex grammar string if "person" is existant, but this will be overwritten by "feminine" or "masculine" if this is specified (not possible for bigender names like "joe")
				GIAEntityNodeGrammaticalGenderArray[currentFeatureInList->entityIndex] = grammaticalGenderIndex;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalGenderIndex = " << grammaticalGenderNameArray[grammaticalGenderIndex] << endl;
			}			
		}

		/*
		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_COUNT) != -1)
		{
			GIAEntityNodeGrammaticalHasCountArray[currentFeatureInList->entityIndex] = true;
			//cout << "hasCount currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
		}	
		*/

		if((currentFeatureInList->grammar).find(GRAMMATICAL_PRONOUN_NAME) != -1)
		{
			GIAEntityNodeGrammaticalIsPronounArray[currentFeatureInList->entityIndex] = GRAMMATICAL_PRONOUN;
			//cout << "isDefinite currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;	
		}					

		currentFeatureInList = currentFeatureInList->next;
	}
}
#endif

void locateAndAddAllConceptEntities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], vector<GIAEntityNode*> *sentenceConceptEntityNodesList, int NLPdependencyRelationsType)
{	
	bool expectToFindComparisonVariable = false;
	if(currentSentenceInList->isQuestion)
	{
		expectToFindComparisonVariable = true;
		setFoundComparisonVariable(false);
		//cout << "expectToFindComparisonVariable" << endl;
	}	
		
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{		
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
		currentRelationInList->relationFunction = performPrepositionReduction(currentRelationInList->relationFunction);
		currentRelationInList->relationArgument = performPrepositionReduction(currentRelationInList->relationArgument);
		currentRelationInList->relationType = performPrepositionReduction(currentRelationInList->relationType);
		#endif	
				
		string name[2]; 
		name[0] = currentRelationInList->relationFunction;
		name[1] =  currentRelationInList->relationArgument; 	//argumentName
		
		#ifdef GIA_USE_RELEX
		if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
		{	
			#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_ARGUMENT_INDEX_MINUS_1
			if(currentRelationInList->relationArgumentIndex < 0)
			{//to prevent Relex 1.4.0 error where it uses a relation argument index of '-1' very occasionally
				currentRelationInList->relationArgumentIndex = GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_ARGUMENT_INDEX_MINUS_1_REPLACEMENT_INDEX;
				//cout << "\tGIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_ARGUMENT_INDEX_MINUS_1_REPLACEMENT_INDEX = " << GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_ARGUMENT_INDEX_MINUS_1_REPLACEMENT_INDEX << endl;
			}
			#endif
		}
		#endif
		
				
		int relationIndex[2];
		relationIndex[0] = currentRelationInList->relationFunctionIndex;
		relationIndex[1] = currentRelationInList->relationArgumentIndex;
		
		
		//cout << "relationIndex[0]  = " << relationIndex[0] << endl;
		//cout << "relationIndex[1]  = " << relationIndex[1] << endl;
		//cout << "name[0]  = " << name[0] << endl;
		//cout << "name[1]  = " << name[1] << endl;
		long entityIndex[2];
		entityIndex[0] = -1;
		entityIndex[1] = -1;
		bool entityAlreadyExistant[2];
		entityAlreadyExistant[0] = false;
		entityAlreadyExistant[1] = false;

		bool argumentIsQuery = false;
		if(name[1] == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
		{//modify relation index [to prevent overlapping of comparison variable indicies with other indicies]
			relationIndex[1] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_ARGUMENT_INDEX;
			currentRelationInList->relationArgumentIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_ARGUMENT_INDEX;
			argumentIsQuery = true;
		}

		for(int i=0; i<2; i++)
		{
			if(!GIAEntityNodeArrayFilled[relationIndex[i]])
			{
				vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
				long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
				long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();				
				GIAEntityNode * entity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &(name[i]), &(entityAlreadyExistant[i]), &(entityIndex[i]), true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
				GIAEntityNodeArrayFilled[relationIndex[i]] = true;
				GIAEntityNodeArray[relationIndex[i]] = entity;				
				entity->hasAssociatedTime = GIAEntityNodeIsDate[relationIndex[i]]; 
				//cout << "entity->hasAssociatedTime = " << entity->hasAssociatedTime << endl;	
				//cout << "[relationIndex[i] = " << [relationIndex[i] << endl;

				for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
				{
					GIAEntityNodeArray[relationIndex[i]]->grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = GIAEntityNodeGrammaticalTenseModifierArray[relationIndex[i]*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + grammaticalTenseModifierIndex];				
				}			
				GIAEntityNodeArray[relationIndex[i]]->grammaticalTenseTemp = GIAEntityNodeGrammaticalTenseArray[relationIndex[i]];
				GIAEntityNodeArray[relationIndex[i]]->grammaticalNumberTemp = GIAEntityNodeGrammaticalNumberArray[relationIndex[i]];
				GIAEntityNodeArray[relationIndex[i]]->grammaticalDefiniteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[relationIndex[i]];
				GIAEntityNodeArray[relationIndex[i]]->grammaticalPersonTemp = GIAEntityNodeGrammaticalIsPersonArray[relationIndex[i]];
				GIAEntityNodeArray[relationIndex[i]]->grammaticalGenderTemp = GIAEntityNodeGrammaticalGenderArray[relationIndex[i]];
				GIAEntityNodeArray[relationIndex[i]]->grammaticalPronounTemp = GIAEntityNodeGrammaticalIsPronounArray[relationIndex[i]];

				GIAEntityNodeArray[relationIndex[i]]->hasAssociatedInstanceTemp = false;

				sentenceConceptEntityNodesList->push_back(entity);	//for GIA_USE_CE
				
				if(i == 1)
				{//argument index only
					if(argumentIsQuery)
					{
						GIAEntityNodeArray[relationIndex[i]]->isQuery = true;
						setFoundComparisonVariable(true);
						setComparisonVariableNode(GIAEntityNodeArray[relationIndex[i]]);				
					}
				}			
			}		
		}

		
		currentRelationInList = currentRelationInList->next;
	}
}


void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])	//unordered_map<string, GIAEntityNode*> *conceptEntityNodesList
{	
	bool expectToFindComparisonVariable = false;
	if(currentSentenceInList->isQuestion)
	{
		expectToFindComparisonVariable = true;
		//cout << "expectToFindComparisonVariable" << endl;
	}
	
	if(expectToFindComparisonVariable)
	{
		/* moved back to locateAndAddAllConceptEntities
		foundComparisonVariable = false;
		
		//cout << "here1" << endl;
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAEntityNodeArrayFilled[i])
			{
				GIAEntityNode * entityNode = GIAEntityNodeArray[i];
				if(entityNode->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{
					//cout << "foundComparisonVariable" << endl;
					entityNode->isQuery = true;
					foundComparisonVariable = true;
					comparisonVariableNode = entityNode;	
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "foundComparisonVariable" << endl;				
					#endif
				}
			}			
		}
		*/
		
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
		if(!getFoundComparisonVariable())
		{//define comparison variable; define required answer entity as the next noun after the question word/lemma eg "house/person" 
			
			bool foundComparisonVariableAlternateMethod = false;
			
			bool foundQueryWordAcceptedByAlternateMethod = false;
			bool foundFirstNounAfterQueryWordAcceptedByAlternateMethod = false;
			Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
			while(currentFeatureInList->next != NULL)
			{
				bool passed = false;
				for(int i=0; i<FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES; i++)
				{
					/*
					cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
					cout << "currentFeatureInList->lemma = " << currentFeatureInList->lemma << endl;
					cout << "currentFeatureInList->type = " << currentFeatureInList->type << endl;
					cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;
					*/
					if((currentFeatureInList->word == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->type == FEATURE_TYPE_ADJECTIVE) && (currentFeatureInList->grammar == featureQueryWordAcceptedByAlternateMethodNameArray[i]))
					{//eg 1	which	which	adj	which
						//cout << "foundQueryWordAcceptedByAlternateMethod" << endl;
						foundQueryWordAcceptedByAlternateMethod = true;
					}
				}
				if(foundQueryWordAcceptedByAlternateMethod)
				{
					if((currentFeatureInList->type == FEATURE_TYPE_NOUN) && !foundComparisonVariableAlternateMethod)
					{
						foundComparisonVariableAlternateMethod = true;
						//cout << "foundQueryWordAcceptedByAlternateMethod" << endl;
						
						/*
						long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
						long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();						
						vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
						string queryComparisonVariableName = currentFeatureInList->word; 
						long queryComparisonVariableEntityIndex = -1;
						bool queryComparisonVariableEntityAlreadyExistant = false;						
						GIAEntityNode * queryComparisonVariableEntityNode = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &queryComparisonVariableName, &queryComparisonVariableEntityAlreadyExistant, &queryComparisonVariableEntityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
						*/
						if(GIAEntityNodeArrayFilled[currentFeatureInList->entityIndex])
						{
							GIAEntityNode * queryComparisonVariableEntityNode = GIAEntityNodeArray[currentFeatureInList->entityIndex];
							queryComparisonVariableEntityNode->isQuery = true;
							queryComparisonVariableEntityNode->isWhichQuery = true;
							setFoundComparisonVariable(true);								
							setComparisonVariableNode(queryComparisonVariableEntityNode);
							#ifdef GIA_TRANSLATOR_DEBUG
							cout << "foundComparisonVariable" << endl;
							cout << "queryComparisonVariableEntityNode->entityName = " << queryComparisonVariableEntityNode->entityName << endl;
							#endif
						}
						else
						{
							cout << "error: !GIAEntityNodeArrayFilled[indexOfAlternateComparisonVariableEntityNode]" << endl;
						}						
					}
				}
				
				currentFeatureInList = currentFeatureInList->next;
			}
		}
		#endif
	}
	/*
	else
	{
		if(foundComparisonVariableTemp)
		{
			cout << "error: expectToFindComparisonVariable && foundComparisonVariable" << endl;
			exit(0);
		}
	}
	*/
}




void switchArgumentsAndFunctionsWhereNecessary(Sentence * currentSentenceInList, int NLPdependencyRelationsType)
{
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
	{
	#endif	
		if(GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY)
		{
			cout << "a" << endl;

			Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{	
				//cout << "here1" << endl;
				//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

				bool passed = false;
				for(int i=0; i<RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES; i++)
				{
					//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

					if(currentRelationInList->relationType == relationTypeRequireSwitchingNameArray[i])
					{
						passed = true;
					}
				}
				if(passed)
				{	
					bool passed2 = false;

					if(GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED)
					{						
						//now find the associated object..
 						Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
						while(currentRelationInList2->next != NULL)
						{	

							/*
							bool partnerTypeRequiredFoundObj = false;
							for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
								{
									partnerTypeRequiredFoundObj = true;
								}
							}
							*/
							bool partnerTypeRequiredFoundSubj = false;
							for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeSubjectNameArray[i])
								{
									partnerTypeRequiredFoundSubj = true;
								}
							}								
							if(partnerTypeRequiredFoundSubj)
							{
								if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationArgumentIndex)
								{//found a matching subject-that[obj] relationship that requires function/argument switching

									cout << "found a matching subject-that[obj] relationship that requires function/argument switching" << endl;
									passed2 = true;
									//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								}
							}

							currentRelationInList2 = currentRelationInList2->next;
						}
					}
					else
					{
						passed2 = true;
					}
					if(passed2)
					{
						string tempString = currentRelationInList->relationArgument;
						int tempIndex = currentRelationInList->relationArgumentIndex;
						currentRelationInList->relationArgument = currentRelationInList->relationFunction;				
						currentRelationInList->relationFunction = tempString;
						currentRelationInList->relationArgumentIndex = currentRelationInList->relationFunctionIndex;				
						currentRelationInList->relationFunctionIndex = tempIndex;						
					}
				}
				currentRelationInList = currentRelationInList->next;
			}					
		}
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	}
	#endif		
}

void identifyEntityTypes(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
{	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//possessive of properties [NB plural/singular indicates definite noun - eg a robin, the robin, the robins - which is therefore a property (entity instance):

		//possessive of properties:

		bool passed = false;
		for(int i=0; i<RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == relationTypePossessiveNameArray[i])
			{
				passed = true;
			}
		}			
		//if(currentRelationInList->relationType == RELATION_TYPE_POSSESSIVE)
		if(passed)
		{
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
			GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
			GIAEntityNode * ownerEntity = GIAEntityNodeArray[relationArgumentIndex];
			ownerEntity->hasPropertyTemp = true;
		}

		//possessive of properties:

		passed = false;
		for(int i=0; i<RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == relationTypeAdjectiveNameArray[i])
			{
				passed = true;
			}
		}						
		//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_1) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_2) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_3))
		if(passed)
		{
			bool passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);

			if(passed2)
			{			
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				thingEntity->hasPropertyTemp = true;
				
				//propertyEntity->hasQualityTemp = true;	//[eg2 The locked door.. / Jim runs quickly / Mr. Smith is late {_amod/_advmod/_predadj}]				
			}
		}

		//has subject:

		passed = false;
		for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
			{
				passed = true;
			}
		}														
		//if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT || (currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE))
		if(passed)
		{
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;	
			GIAEntityNode * subjectEntity = GIAEntityNodeArray[relationArgumentIndex];
			subjectEntity->isSubjectTemp = true;
		}

		//has object:

		passed = false;
		for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
			{
				passed = true;
			}
		}
		//if((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO))
		if(passed)	
		{
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
			GIAEntityNode * objectEntity = GIAEntityNodeArray[relationArgumentIndex];
			objectEntity->isObjectTemp = true; 
		}

		currentRelationInList = currentRelationInList->next;
	}
}


#ifdef GIA_USE_STANFORD_CORENLP
void linkReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList)	//Stanford Compatible [NOT YET CODED]
{
	cout << "linkReferencesStanfordCoreNLP(): error - function not yet coded" << endl;
	exit(0); 
}
#endif
			
void linkReferences(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[])
{		
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{	
		//cout << "w = " << w << endl;

		if(GIAEntityNodeArrayFilled[w])
		{
			GIAEntityNode * currentGIAEntityNode = GIAEntityNodeArray[w];
			//cout << "currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;

			for(int i=0; i< REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES; i++)
			{
				//cout << "i = " << i << endl;

				if(((currentGIAEntityNode->entityName == referenceTypePossessiveNameArray[i]) || (currentGIAEntityNode->entityName == referenceTypePersonNameArray[i])) && (currentGIAEntityNode->grammaticalPronounTemp == GRAMMATICAL_PRONOUN))
				{//pronoun required for currentGIAEntityNode
					//cout << "currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;
					//now go for a search in tree for given / this sentence + previous sentence until find candidate reference source

					GIAEntityNode * referenceSource = NULL;
					bool referenceSourceHasBeenFound = false;
					bool stillSentencesToSearch = true;
					Sentence * currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInList;
					int s2 = 0;
					while(!referenceSourceHasBeenFound && stillSentencesToSearch) 
					{
						//cout << "s2 = " << s2 << endl;

						Relation * currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->firstRelationInList;
						int maxWordLimit = 999999;
						if(s2 == 0)
						{
							maxWordLimit = w;
						}
						int w2 = 0;
						while((currentRelationInWhichReferenceSourceIsBeingSearchedFor->next != NULL) && (w2 < maxWordLimit))
						{
							//cout << "w2 = " << w2 << endl;

							long entityIndex = -1;
							bool entityAlreadyExistant = false;

							string entityName = currentRelationInWhichReferenceSourceIsBeingSearchedFor->relationArgument;

							//cout << "currentRelationInWhichReferenceSourceIsBeingSearchedFor = " << entityName << endl;

							if(entityName != "")
							{
	
								vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
								long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
								long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();								
								GIAEntityNode * currentEntityInWhichReferenceSourceIsBeingSearchedFor = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &entityName, &entityAlreadyExistant, &entityIndex, false, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);

								if(entityAlreadyExistant)
								{
									//cout << "candidateReferenceSourceEntityName = " << entityName << endl;

									bool entityPassesGrammaticalTestsForReference = true;



									//if(referenceTypePersonCrossReferenceNumberArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
									//if(!((referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp != referenceTypePersonCrossReferenceNumberArray[i])
										{
											//cout << "a1" << endl;
											entityPassesGrammaticalTestsForReference = false;
										}
									//}
									//if(referenceTypePersonCrossReferenceGenderArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
									//if(!((referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp != referenceTypePersonCrossReferenceGenderArray[i])
										{
											//cout << "a2" << endl;
											entityPassesGrammaticalTestsForReference = false;
										}
									//}
									//if(referenceTypePersonCrossReferenceDefiniteArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
									//if(!((referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp != referenceTypePersonCrossReferenceDefiniteArray[i])
										{
											//cout << "a3" << endl;
											entityPassesGrammaticalTestsForReference = false;
										}
									//}
									//if(referenceTypePersonCrossReferencePersonArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
									//if(!((referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp != referenceTypePersonCrossReferencePersonArray[i])
										{
											//cout << "a4" << endl;
											entityPassesGrammaticalTestsForReference = false;
										}


										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPronounTemp == GRAMMATICAL_PRONOUN)
										{
											//cout << "a5" << endl;
											entityPassesGrammaticalTestsForReference = false;
										}



									//}
									if(entityPassesGrammaticalTestsForReference)
									{	
										#ifdef GIA_TRANSLATOR_DEBUG
										cout << "entityPassesGrammaticalTestsForReference" << endl;
										#endif

										//cout << "referenceTypePersonCrossReferenceNumberArray[i] = " << referenceTypePersonCrossReferenceNumberArray[i] << endl;
										//cout << "referenceTypePersonCrossReferenceGenderArray[i] = " << referenceTypePersonCrossReferenceGenderArray[i] << endl;
										//cout << "referenceTypePersonCrossReferenceDefiniteArray[i] = " << referenceTypePersonCrossReferenceDefiniteArray[i] << endl;
										//cout << "referenceTypePersonCrossReferencePersonArray[i] = " << referenceTypePersonCrossReferencePersonArray[i] << endl;
										//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp << endl;
										//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp << endl;
										//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp << endl;
										//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp << endl;


										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp)
										{
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp << endl;
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
										else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp) && (s2 > 0))
										{
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp << endl;
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
										else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp) && (s2 > 0))
										{
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp << endl;
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
									}

								}
								else
								{
									//cout << "error: !entityAlreadyExistant" << endl;	//will be non-existant in the case of intermediary words like "the"
									//exit(0);
								}
							}
							w2++;

							currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentRelationInWhichReferenceSourceIsBeingSearchedFor->next;
						}

						if(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous != NULL)
						{	
							currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous;
						}
						else
						{
							stillSentencesToSearch = false;
						}
						s2++;

					}

					if(referenceSourceHasBeenFound)
					{//remap entity; eg He to John 
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "referenceSourceHasBeenFound: assigning " << GIAEntityNodeArray[w]->entityName << " to " << referenceSource->entityName << "." << endl;
						#endif
						//referenceSource->isReferenceEntityInThisSentence = true;
						#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
						GIAEntityNodeArray[w]->disabled = true;
						#endif
						GIAEntityNodeArray[w] =	referenceSource;
						GIAEntityNodeIsAReference[w] = true;
					}			
				}
			}
		}
	}
}









#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg	The accident happened as the night was falling. 	advcl(happen, fall) / mark(fall, as)
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == STANFORD_RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
				bool partnerTypeRequiredFound = false;					
				if(currentRelationInList2->relationType == STANFORD_RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER)
				{
					partnerTypeRequiredFound = true;
				}

				if(partnerTypeRequiredFound)
				{		
					if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationArgumentIndex)
					{//found a matching object-subject relationship
						
						currentRelationInList2->disabled = true;
						currentRelationInList->relationType = (GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex])->entityName;
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}				
#endif				


#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg	What she said makes sense. 	csubj (make, say) / dobj ( said-3 , What-1 )
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_OBJECT)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
				bool partnerTypeRequiredFound = false;					
				if(currentRelationInList2->relationType == STANFORD_RELATION_TYPE_CLAUSAL_SUBJECT)
				{
					partnerTypeRequiredFound = true;
				}

				if(partnerTypeRequiredFound)
				{		
					if(currentRelationInList2->relationArgumentIndex == currentRelationInList->relationFunctionIndex)
					{//found a matching object-subject relationship
						
						currentRelationInList2->relationType = RELATION_TYPE_SUBJECT;
						currentRelationInList2->relationArgumentIndex = currentRelationInList->relationArgumentIndex;
						currentRelationInList2->relationArgument = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex]->entityName;
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}				
#endif	

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(currentRelationInList->relationType == STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE)
		{
			//cout << "STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE" << endl;
			//eg They shut down the station. 	prt(shut, down) 			

			GIAEntityNode * governerEntity = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
			GIAEntityNode * dependencyEntity = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];
			governerEntity->entityName = governerEntity->entityName + "_" + dependencyEntity->entityName;
			//cout << "governerEntity->entityName = " <<governerEntity->entityName << endl;

			dependencyEntity->disabled = true;
		}			
		currentRelationInList = currentRelationInList->next;
	}
}

#endif


#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsNSubjAndPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//look for nsubj/prep combination, eg nsubj(next-4, garage-2) / prep_to(next-4, house-7)
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
				bool stanfordPrepositionFound = false;
				string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList2->relationType), GIA_DEPENDENCY_RELATION_FORMATION_STANFORD, &stanfordPrepositionFound);

				if(stanfordPrepositionFound)
				{		
					if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationFunctionIndex)
					{//found a matching preposition of object-subject relationship
					
																	
						string newPrepositionName = GIAEntityNodeArray[currentRelationInList2->relationFunctionIndex]->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;
											
						Relation * subjectOfPrepositionRelation = currentRelationInList;
						Relation * objectOfPrepositionRelation = currentRelationInList2;
						subjectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION;
						objectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
						GIAEntityNodeArray[currentRelationInList2->relationFunctionIndex]->entityName = newPrepositionName;
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}
#endif



