/*******************************************************************************
 *
 * File Name: GIATranslatorDefineGrammarAndReferencing.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1a 20-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorDefineGrammarAndReferencing.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"



void locateAndAddAllConceptEntities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *sentenceConceptEntityNodesList, int NLPdependencyRelationsType)
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
		currentRelationInList->relationGovernor = performPrepositionReduction(currentRelationInList->relationGovernor);
		currentRelationInList->relationDependent = performPrepositionReduction(currentRelationInList->relationDependent);
		currentRelationInList->relationType = performPrepositionReduction(currentRelationInList->relationType);
		#endif	
				
		string name[2]; 
		name[0] = currentRelationInList->relationGovernor;
		name[1] =  currentRelationInList->relationDependent; 	//argumentName
		
		#ifdef GIA_USE_RELEX
		if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{	
			#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1
			if(currentRelationInList->relationDependentIndex < 0)
			{//to prevent Relex 1.4.0 error where it uses a relation argument index of '-1' very occasionally
				currentRelationInList->relationDependentIndex = GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1_REPLACEMENT_INDEX;
				//cout << "\tGIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1_REPLACEMENT_INDEX = " << GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1_REPLACEMENT_INDEX << endl;
			}
			#endif
		}
		#endif
		
				
		int relationIndex[2];
		relationIndex[0] = currentRelationInList->relationGovernorIndex;
		relationIndex[1] = currentRelationInList->relationDependentIndex;
		
		
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
			relationIndex[1] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
			currentRelationInList->relationDependentIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
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
				if(entityAlreadyExistant[i])
				{
					applyEntityAlreadyExistsFunction(entity);					
				}
				GIAEntityNodeArrayFilled[relationIndex[i]] = true;
				GIAEntityNodeArray[relationIndex[i]] = entity;

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
	
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{	
		if(GIAEntityNodeArrayFilled[w])
		{
			GIAEntityNode * entity = GIAEntityNodeArray[w];
			cout << "entity->entityName = " << entity->entityName << endl;		
		}
	}
		
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		string relationType = currentRelationInList->relationType;
		GIAEntityNode * relationGoverner = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];				
		GIAEntityNode * relationDependent = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
		
		cout << "relationType = " << currentRelationInList->relationType << endl;	      
		cout << "relationGoverner = " << relationGoverner->entityName << endl;
		cout << "relationDependent = " << relationDependent->entityName << endl;		
		
		currentRelationInList = currentRelationInList->next;		
	}
	
	//exit(0);
	
	#endif
		
}



#ifdef GIA_USE_RELEX
void fillGrammaticalArraysRelex(Sentence * currentSentenceInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], int GIAEntityNodeWordTypeArray[])
{
	Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		//cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;

		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_TENSE_DATE) != -1)
		{
			GIAEntityNodeIsDateOrStanfordTime[currentFeatureInList->entityIndex] = true;
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
	
		//fill wordType array for wordnet - added 26 April 2012
		int wordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;
		convertRelexPOSTypeToWordnetWordType(&(currentFeatureInList->type), &wordType);
		GIAEntityNodeWordTypeArray[currentFeatureInList->entityIndex] = wordType;
				
		#ifdef FILL_NER_ARRAY_AFTER_RELEX_PARSE_FOR_STANFORD_EQUIVALENT_PROPER_NOUN_DETECTION
		//fill NER array after Relex Parse for Stanford equivalent proper noun detection - added 26 April 2012
		for(int featureRelexFlagIndex = 0; featureRelexFlagIndex < FEATURE_RELEX_FLAG_NUMBER_TYPES; featureRelexFlagIndex++)
		{
			if((currentFeatureInList->grammar).find(featureRelexFlagTypeArray[featureRelexFlagIndex]) != -1) 				
			{
				GIAEntityNodeNERArray[currentFeatureInList->entityIndex] = featureRelexFlagIndex;
				/*
				#ifdef GIA_WORDNET_DEBUG
				cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex " << featureRelexFlagIndex << " = " << featureRelexFlagTypeArray[featureRelexFlagIndex] << endl;
				#endif
				*/
			}			
		}
		for(int i=0; i<FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_TYPES; i++)
		{
			if(GIAEntityNodeNERArray[currentFeatureInList->entityIndex] == featureNERindicatesProperNounTypeArray[i])
			{
				GIAEntityNodeGrammaticalIsProperNounArray[currentFeatureInList->entityIndex] = true;
				/*
				#ifdef GIA_WORDNET_DEBUG
				cout << "isProperNoun currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;			
				#endif
				*/
			}
		}
		#else
		if((currentFeatureInList->grammar).find(GRAMMATICAL_PERSON_NAME) != -1)
		{
			GIAEntityNodeGrammaticalIsProperNounArray[currentFeatureInList->entityIndex] = GRAMMATICAL_PERSON;
			//cout << "isPerson currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;

		}		
		#endif
				
		currentFeatureInList = currentFeatureInList->next;
	}
}
#endif


#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
void extractPastTenseFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[])
{
	bool pastTenseDetected = false;
	
	//do not write if present tense found: NB copulas take precedence over auxillaries in formation of past tense (eg he has been sick;     nsubj ( sick-4 , he-1 ) / aux ( sick-4 , has-2 ) / cop ( sick-4 , been-3 )
	for(int i=0; i<FEATURE_POS_TAG_VERB_PAST_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagVerbPastArray[i])
		{
			pastTenseDetected = true;
		}
	}
	if(pastTenseDetected)
	{
		GIAEntityNodeGrammaticalTenseArray[GRAMMATICAL_TENSE_PAST] = true;
	}
}

//Preconditions: extractGrammaticalInformationStanford()/extractGrammaticalInformationFromPOStag() must be executed before relations (eg aux/cop) are processed, as they may [possibly] overwrite the tenses here established
void extractGrammaticalInformationFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], bool GIAEntityNodeGrammaticalIsPronounArray[])
{
	//past tense extraction;
	//this is required for past tense verbs without auxillaries; eg He ran fast.     nsubj ( ran-2 , He-1 ), advmod ( ran-2 , fast-3 ) . 
	extractPastTenseFromPOStag(POStag, entityIndex, GIAEntityNodeGrammaticalTenseArray);		


	//progressives tense extraction;
	bool progressiveDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagVerbProgressiveArray[i])
		{
			progressiveDetected = true;
		}
	}
	if(progressiveDetected)
	{
		GIAEntityNodeGrammaticalTenseModifierArray[entityIndex*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] = true;
	}
	
	//singular/plural detection;
	bool pluralDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagPluralNounArray[i])
		{
			pluralDetected = true;
		}
	}
	if(pluralDetected)
	{
		GIAEntityNodeGrammaticalNumberArray[entityIndex] = GRAMMATICAL_NUMBER_PLURAL;
	}
	bool singularDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagSingularNounArray[i])
		{
			singularDetected = true;
		}
	}
	if(singularDetected)
	{
		GIAEntityNodeGrammaticalNumberArray[entityIndex] = GRAMMATICAL_NUMBER_SINGULAR;
	}
	
	//proper noun detection;
	bool properNounDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagProperNounArray[i])
		{
			properNounDetected = true;
		}
	}
	if(properNounDetected)
	{
		GIAEntityNodeGrammaticalIsProperNounArray[entityIndex] = true;
	}
	
	//pronoun detection;
	bool pronounDetected = false;
	//use POS information to extract pronoun information - NB alternatively, could use referenceTypePersonNameArray and referenceTypePossessiveNameArray (as there is only a limited set of pronouns in english)
	for(int i=0; i<FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_TYPES; i++)
	{
		if(*POStag == featurePOSindicatesPronounTypeArray[i])
		{
			GIAEntityNodeGrammaticalIsPronounArray[entityIndex] = true;
		}
	}
					
					
}


void extractGrammaticalInformationStanford(Feature * firstFeatureInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], string GIAEntityNodePOSArray[], int GIAEntityNodeWordTypeArray[], int NLPfeatureParser)
{
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		Feature * currentFeatureInList = firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{	
			int currentFeatureIndex = currentFeatureInList->entityIndex;
			extractGrammaticalInformationFromPOStag(&(currentFeatureInList->POS), currentFeatureIndex, GIAEntityNodeGrammaticalTenseArray, GIAEntityNodeGrammaticalTenseModifierArray, GIAEntityNodeGrammaticalNumberArray, GIAEntityNodeGrammaticalIsProperNounArray, GIAEntityNodeGrammaticalIsPronounArray);
				
			int currentNER = FEATURE_NER_UNDEFINED;
			for(int i=0; i<FEATURE_NER_NUMBER_TYPES; i++)
			{
				if(currentFeatureInList->NER == featureNERTypeArray[i])
				{
					currentNER = i;
					/*
					#ifdef GIA_WORDNET_DEBUG
					cout << "nerType currentFeatureIndex = " << currentNER << endl;			
					#endif
					*/
				}
			}	
			
			GIAEntityNodeNERArray[currentFeatureIndex] = currentNER;
			GIAEntityNodeNormalizedNERArray[currentFeatureIndex] = currentFeatureInList->NormalizedNER;
			GIAEntityNodeTimexArray[currentFeatureIndex] = currentFeatureInList->Timex;
			GIAEntityNodePOSArray[currentFeatureIndex] = currentFeatureInList->POS;
			int wordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;
			convertStanfordPOSTagToRelexPOSTypeAndWordnetWordType(&(currentFeatureInList->POS), &(currentFeatureInList->type), &wordType);
			GIAEntityNodeWordTypeArray[currentFeatureIndex] = wordType;
			
			if((currentNER == FEATURE_NER_DATE) || (currentNER == FEATURE_NER_TIME))
			{
				GIAEntityNodeIsDateOrStanfordTime[currentFeatureIndex] = true;
			}
			/*//NB the GIAEntityNodeGrammaticalIsProperNounArray array for stanford core nlp does not relate to persons (only proper nouns)
			if(currentNER == FEATURE_NER_PERSON)
			{
				GIAEntityNodeGrammaticalIsProperNounArray[currentFeatureIndex] = true;		
			}
			*/
					
			currentFeatureInList = currentFeatureInList->next;
		}	
	}
}

void extractPastTense(int entityIndex, int entityIndexContainingTenseIndication, Feature * firstFeatureInList, int GIAEntityNodeGrammaticalTenseArray[], int NLPfeatureParser)
{
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		//use the copular to set the tense of the noun

		Feature * currentFeatureInList = firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{	
			if(currentFeatureInList->entityIndex == entityIndexContainingTenseIndication)
			{	
				extractPastTenseFromPOStag(&(currentFeatureInList->POS), entityIndex, GIAEntityNodeGrammaticalTenseArray);
			}
			currentFeatureInList = currentFeatureInList->next;
		}
	}
}
#endif

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford() 
void fillGrammaticalArraysStanford(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], string GIAEntityNodePOSArray[], int GIAEntityNodeWordTypeArray[], int NLPfeatureParser)
{
	//uses Stanford specific relations (grammar related)

	//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
	extractGrammaticalInformationStanford(currentSentenceInList->firstFeatureInList, GIAEntityNodeIsDateOrStanfordTime, GIAEntityNodeGrammaticalTenseArray, GIAEntityNodeGrammaticalTenseModifierArray, GIAEntityNodeGrammaticalNumberArray, GIAEntityNodeGrammaticalIsProperNounArray, GIAEntityNodeGrammaticalIsPronounArray, GIAEntityNodeNERArray, GIAEntityNodeNormalizedNERArray, GIAEntityNodeTimexArray, GIAEntityNodePOSArray, GIAEntityNodeWordTypeArray, NLPfeatureParser);					

	 
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{	
		#endif
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			//perfect tense extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_MODAL_AUX)
			{
				//eg aux (died, has) 	Reagan has died.	[addtogrammar: perfect?]
				int entityIndexOfAuxillary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;				
				GIAEntityNodeGrammaticalTenseModifierArray[entityIndexOfVerb*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + GRAMMATICAL_TENSE_MODIFIER_PERFECT] = true;
				GIAEntityNodeArray[entityIndexOfAuxillary]->disabled = true;		

				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB
				extractPastTense(entityIndexOfVerb, entityIndexOfAuxillary, currentSentenceInList->firstFeatureInList, GIAEntityNodeGrammaticalTenseArray, NLPfeatureParser);		
				#endif	

			}

			//passive tense extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
			{
				//eg auxpass(killed, been) Kennedy has been killed. 	[addtogrammar: passive]	
				int entityIndexOfAuxillary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;
				GIAEntityNodeGrammaticalTenseModifierArray[entityIndexOfVerb*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + GRAMMATICAL_TENSE_MODIFIER_PASSIVE] = true;
				GIAEntityNodeArray[entityIndexOfAuxillary]->disabled = true;				

				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB
				extractPastTense(entityIndexOfVerb, entityIndexOfAuxillary, currentSentenceInList->firstFeatureInList, GIAEntityNodeGrammaticalTenseArray, NLPfeatureParser);		
				#endif			

			}

			//past tense extraction:
			//NB copulas take precedence over auxillaries in formation of past tense (eg he has been sick;     nsubj ( sick-4 , he-1 ) / aux ( sick-4 , has-2 ) / cop ( sick-4 , been-3 ) )
			if(currentRelationInList->relationType == RELATION_TYPE_COPULA)
			{
				//eg cop(smelled, sweet) 	The rose smelled sweet. [THIS APPEARS INCORRECT: stanford currently gives; acomp ( smelled-3 , sweet-4 )]
				//eg cop(black-5, was-4) 	Alice's cookie was black.

				int entityIndexOfCopula = currentRelationInList->relationDependentIndex;
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;
				GIAEntityNodeArray[entityIndexOfCopula]->disabled = true;	

				extractPastTense(entityIndexOfNoun, entityIndexOfCopula, currentSentenceInList->firstFeatureInList, GIAEntityNodeGrammaticalTenseArray, NLPfeatureParser);

			}

			//future tense extraction:
			//overwrite current tense derivations with GRAMMATICAL_TENSE_FUTURE if there is an auxillary containing 'will'
			if(currentRelationInList->relationType == RELATION_TYPE_MODAL_AUX)	//|| (currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
			{
				int auxillaryDependencyIndex = currentRelationInList->relationGovernorIndex;
				string auxillaryGovernerEntity = currentRelationInList->relationDependent;
				for(int i=0; i<RELATION_TYPE_AUXILLARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES; i++)
				{
					if(relationAuxillaryGovernerIndicatesFutureTenseArray[i] == auxillaryGovernerEntity)
					{
						GIAEntityNodeGrammaticalTenseArray[auxillaryDependencyIndex] = GRAMMATICAL_TENSE_FUTURE;
					}
				}
			}


			//definite/indefinite extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_DETERMINER)
			{

				//cout << "RELATION_TYPE_DETERMINER = " << RELATION_TYPE_DETERMINER << endl;
				//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

				//eg det(cookie, the) 	the cookie. 
				string determiner = currentRelationInList->relationDependent;
				//cout << "determiner = " << determiner << endl;
				if(determiner == GRAMMATICAL_DETERMINER_DEFINITE)
				{

					int entityIndexOfDeterminier = currentRelationInList->relationDependentIndex;
					int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;					
					GIAEntityNodeArray[entityIndexOfDeterminier]->disabled = true;		
					GIAEntityNodeGrammaticalIsDefiniteArray[entityIndexOfNoun] = true;

					//cout << "GIAEntityNodeArray[entityIndexOfDeterminier]->entityName = " << GIAEntityNodeArray[entityIndexOfDeterminier]->entityName << endl;
					//cout << "GIAEntityNodeArray[entityIndexOfNoun]->entityName = " << GIAEntityNodeArray[entityIndexOfNoun]->entityName << endl;				
				}
				/*
				else if(determiner == GRAMMATICAL_DETERMINER_INDEFINITE)
				{
					//no marking, in accordance with RelEx; 'doesn't mark [a] "book" at all'
				}
				*/

			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS		
		}
		#endif
		
		currentRelationInList = currentRelationInList->next;
	}
	
	
	

}
#endif



void applyGrammaticalInfoToAllConceptEntities(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], string GIAEntityNodePOSArray[], int GIAEntityNodeWordTypeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{	
		//cout << "w = " << w << endl;

		if(GIAEntityNodeArrayFilled[w])
		{
			GIAEntityNode * entity = GIAEntityNodeArray[w];
			//cout << "entity->entityName = " << entity->entityName << endl;
			
			entity->hasAssociatedTime = GIAEntityNodeIsDateOrStanfordTime[w]; 
			//cout << "entity->hasAssociatedTime = " << entity->hasAssociatedTime << endl;	

			for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
			{
				entity->grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = GIAEntityNodeGrammaticalTenseModifierArray[w*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + grammaticalTenseModifierIndex];				
			}			
			entity->grammaticalTenseTemp = GIAEntityNodeGrammaticalTenseArray[w];
			entity->grammaticalNumberTemp = GIAEntityNodeGrammaticalNumberArray[w];
			entity->grammaticalDefiniteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[w];
			entity->grammaticalRelexPersonOrStanfordProperNounTemp = GIAEntityNodeGrammaticalIsProperNounArray[w];
			entity->grammaticalGenderTemp = GIAEntityNodeGrammaticalGenderArray[w];
			entity->grammaticalPronounTemp = GIAEntityNodeGrammaticalIsPronounArray[w];	
			
			entity->POSTemp = GIAEntityNodePOSArray[w];
			entity->NERTemp = GIAEntityNodeNERArray[w];
			entity->NormalizedNERTemp = GIAEntityNodeNormalizedNERArray[w];
			entity->TimexTemp = GIAEntityNodeTimexArray[w];					
		}
	}
}


void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser)	//unordered_map<string, GIAEntityNode*> *conceptEntityNodesList
{	
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
	#endif

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
						if((currentFeatureInList->word == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->type == FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME) && (currentFeatureInList->grammar == featureQueryWordAcceptedByAlternateMethodNameArray[i]))
						{//eg 1	which	which	adj	which
							//cout << "foundQueryWordAcceptedByAlternateMethod" << endl;
							foundQueryWordAcceptedByAlternateMethod = true;
						}
					}
					if(foundQueryWordAcceptedByAlternateMethod)
					{
						if((currentFeatureInList->type == FEATURE_RELEX_POS_TYPE_NOUN_NAME) && !foundComparisonVariableAlternateMethod)
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
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE	
	}
	#endif
}




void switchArgumentsAndFunctionsWhereNecessary(Sentence * currentSentenceInList, int NLPdependencyRelationsType)
{
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif	
		if(GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY)
		{
			cout << "a" << endl;

			Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{	
				#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
				if(!(currentRelationInList->disabled))
				{			
				#endif
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

						if(GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED)
						{						
							//now find the associated object..
 							Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList2->next != NULL)
							{	
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
								if(!(currentRelationInList2->disabled))
								{			
								#endif
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
										if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
										{//found a matching subject-that[obj] relationship that requires function/argument switching

											cout << "found a matching subject-that[obj] relationship that requires function/argument switching" << endl;
											passed2 = true;
											//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
										}
									}
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
								}			
								#endif									

								currentRelationInList2 = currentRelationInList2->next;
							}
						}
						else
						{
							passed2 = true;
						}
						if(passed2)
						{
							string tempString = currentRelationInList->relationDependent;
							int tempIndex = currentRelationInList->relationDependentIndex;
							currentRelationInList->relationDependent = currentRelationInList->relationGovernor;				
							currentRelationInList->relationGovernor = tempString;
							currentRelationInList->relationDependentIndex = currentRelationInList->relationGovernorIndex;				
							currentRelationInList->relationGovernorIndex = tempIndex;						
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS	
					}
					#endif
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
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif
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
				int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
				int relationDependentIndex = currentRelationInList->relationDependentIndex;				
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationGovernorIndex];
				GIAEntityNode * ownerEntity = GIAEntityNodeArray[relationDependentIndex];
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
			//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_AMOD) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_PREDADJ) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD))
			if(passed)
			{
				bool passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);

				if(passed2)
				{			
					int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
					int relationDependentIndex = currentRelationInList->relationDependentIndex;				
					GIAEntityNode * thingEntity = GIAEntityNodeArray[relationGovernorIndex];
					GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationDependentIndex];
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
			//if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			if(passed)
			{
				int relationDependentIndex = currentRelationInList->relationDependentIndex;	
				GIAEntityNode * subjectEntity = GIAEntityNodeArray[relationDependentIndex];
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
				int relationDependentIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * objectEntity = GIAEntityNodeArray[relationDependentIndex];
				objectEntity->isObjectTemp = true; 
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


#ifdef GIA_USE_STANFORD_CORENLP
void linkReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, StanfordCoreNLPCoreference * firstCoreferenceInList, bool GIAEntityNodeIsAReference[])
{
	//cout << "linkReferencesStanfordCoreNLP(): error - function not yet coded" << endl;
	//exit(0); 
	StanfordCoreNLPCoreference * currentCoreferenceInList = firstCoreferenceInList;
	while(currentCoreferenceInList->next != NULL)
	{	
		StanfordCoreNLPMention * firstMentionInList = currentCoreferenceInList->firstMentionInList;
		StanfordCoreNLPMention * currentMentionInList = firstMentionInList;
		GIAEntityNode * referenceSource = NULL;
		bool foundReferenceSource = false;
		while(currentMentionInList->next != NULL)
		{
			if(!foundReferenceSource)
			{
				if(currentMentionInList->representative)
				{
					int referenceSourceSentenceIndex = currentMentionInList->sentence;			
					int referenceSourceEntityNodeIndex = currentMentionInList->head;
					
					//cout << "referenceSourceSentenceIndex = " << referenceSourceSentenceIndex << endl;
					//cout << "\treferenceSourceEntityNodeIndex = " << referenceSourceEntityNodeIndex << endl;
					
					if(referenceSourceSentenceIndex <= currentSentenceInList->sentenceIndex)
					{
						//cout << "ASD" << endl;
						Sentence * currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInList;
						bool stillExistsPreviousSentence = true;
						while(stillExistsPreviousSentence)
						{
							//cout << "currentSentenceInWhichReferenceSourceIsBeingSearchedFor->sentenceIndex = " <<  currentSentenceInWhichReferenceSourceIsBeingSearchedFor->sentenceIndex << endl;
							if(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->sentenceIndex == referenceSourceSentenceIndex)
							{
								Feature * currentFeatureInList = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->firstFeatureInList;
								while(currentFeatureInList->next != NULL)
								{
									//cout << "currentFeatureInList->entityIndex = " <<  currentFeatureInList->entityIndex << endl;
									
									if(currentFeatureInList->entityIndex == referenceSourceEntityNodeIndex)
									{	
										long entityIndex = -1;
										bool entityAlreadyExistant = false;
										string entityName = currentFeatureInList->lemma;	//CHECK THIS; assumes [at least one instance of] the reference source will always occur as a relation argument/dependent (ie, will not find the reference source if it only occurs as a relation function/governer)					
										vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
										long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
										long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();								
										GIAEntityNode * currentEntityInWhichReferenceSourceIsBeingSearchedFor = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &entityName, &entityAlreadyExistant, &entityIndex, false, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);

										//cout << "entityName = " << entityName << endl;
										//cout << "referenceSourceSentenceIndex = " << referenceSourceSentenceIndex << endl;
										//cout << "referenceSourceEntityNodeIndex = " << referenceSourceEntityNodeIndex << endl;

										if(entityAlreadyExistant)
										{
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;

											//cout << "referenceSource->entityName = " << referenceSource->entityName << endl;										
											foundReferenceSource = true;
										}

										//now search for this entity

									}
									currentFeatureInList = currentFeatureInList->next;
								}
							}
							
							if(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous != NULL)
							{
								currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous;
							}
							else
							{
								stillExistsPreviousSentence = false;
							}
						}
					}
				}
			}
			
			if(foundReferenceSource)
			{
				if(!(currentMentionInList->representative))
				{//continue only for references, eg pronoun (not for their source, eg name)	
					
					if(currentMentionInList->sentence == currentSentenceInList->sentenceIndex)
					{
						int currentSentenceEntityNodeIndex = currentMentionInList->head;
						if(GIAEntityNodeArrayFilled[currentSentenceEntityNodeIndex])
						{
							GIAEntityNode * reference = GIAEntityNodeArray[currentSentenceEntityNodeIndex];
							
							bool passedCoreferenceTypeRequirements = false;
							#ifdef GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION
							//use POS information to ensure that the reference is a pronoun - NB alternatively, could use referenceTypePersonNameArray and referenceTypePossessiveNameArray (as there is only a limited set of pronouns in english)
							for(int i=0; i<FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_TYPES; i++)
							{
								if(reference->POSTemp == featurePOSindicatesPronounTypeArray[i])
								{
									passedCoreferenceTypeRequirements = true;
								}
							}
							#else
							passedCoreferenceTypeRequirements = true;
							#endif
							
							if(passedCoreferenceTypeRequirements)
							{
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "referenceSourceHasBeenFound: assigning " << GIAEntityNodeArray[currentSentenceEntityNodeIndex]->entityName << " to " << referenceSource->entityName << "." << endl;
								#endif
								//referenceSource->isReferenceEntityInThisSentence = true;
								disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentSentenceEntityNodeIndex]);

								GIAEntityNodeArray[currentSentenceEntityNodeIndex] = referenceSource;
								GIAEntityNodeIsAReference[currentSentenceEntityNodeIndex] = true;

								applyEntityAlreadyExistsFunction(referenceSource);
							}									
						}
					}
				}
			}
			
			currentMentionInList = currentMentionInList->next;
		}
		currentCoreferenceInList = currentCoreferenceInList->next;
	}
}
#endif
			
void linkReferences(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[])
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

							string entityName = currentRelationInWhichReferenceSourceIsBeingSearchedFor->relationDependent;

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
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalRelexPersonOrStanfordProperNounTemp != referenceTypePersonCrossReferencePersonArray[i])
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
										//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalRelexPersonOrStanfordProperNounTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalRelexPersonOrStanfordProperNounTemp << endl;


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
						disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[w]);

						GIAEntityNodeArray[w] =	referenceSource;
						GIAEntityNodeIsAReference[w] = true;
						
						applyEntityAlreadyExistsFunction(referenceSource);
					}			
				}
			}
		}
	}
}





#ifdef GIA_USE_STANFORD_CORENLP
void disableRedundantNodesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eliminate all redundant date relations eg num(December-4, 3rd-5)/num(December-4, 1990-7)/nn(3rd-5, December-4)/appos(3rd-5, 1990-7), where both the governer and the dependent have NER tag set to DATE

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif	
			GIAEntityNode * governerEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
			GIAEntityNode * dependentEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];

			bool governerAndDependentBothHaveSameNERvalue = false;
			for(int i=0; i<FEATURE_NER_EXPLICIT_NUMBER_TYPES; i++)
			{
				if((governerEntity->NERTemp == featureNERexplicitTypeArray[i]) && (dependentEntity->NERTemp == featureNERexplicitTypeArray[i]))
				{
					governerAndDependentBothHaveSameNERvalue = true;
				}
			}

			//if(((governerEntity->NERTemp == FEATURE_NER_DATE) && (dependentEntity->NERTemp == FEATURE_NER_DATE)) || ((governerEntity->NERTemp == FEATURE_NER_MONEY) && (dependentEntity->NERTemp == FEATURE_NER_MONEY)) || ((governerEntity->NERTemp == FEATURE_NER_NUMBER) && (dependentEntity->NERTemp == FEATURE_NER_NUMBER)) || ((governerEntity->NERTemp == FEATURE_NER_TIME) && (dependentEntity->NERTemp == FEATURE_NER_TIME)))
			if(governerAndDependentBothHaveSameNERvalue)
			{
				//cout << "governerEntity->NERTemp = " << governerEntity->NERTemp << endl;
				//cout << "dependentEntity->NERTemp = " << dependentEntity->NERTemp << endl;

				currentRelationInList->disabled = true;			

				disableEntityBasedUponFirstSentenceToAppearInNetwork(dependentEntity);
				//disableEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(dependentEntity);	//OLD: before moving disableRedundantNodesStanfordCoreNLP() forward in execution heirachy (GIATranslatorDefineGrammarAndReferencing.cpp)

				bool featureNERindicatesNameConcatenationRequired = false;
				for(int i=0; i<FEATURE_NER_INDICATES_NAME_CONCATENATION_REQUIRED_NUMBER_TYPES; i++)
				{
					if(governerEntity->NERTemp == featureNERindicatesNameConcatenationRequiredTypeArray[i])
					{
						featureNERindicatesNameConcatenationRequired = true;
					}
				}

				//if((governerEntity->NETTemp == FEATURE_NER_PERSON) || (governerEntity->NETTemp == FEATURE_NER_LOCATION) || (governerEntity->NETTemp == FEATURE_NER_ORGANIZATION) || (governerEntity->NETTemp == FEATURE_NER_MISC))
				if(featureNERindicatesNameConcatenationRequired)
				{
					governerEntity->entityName = dependentEntity->entityName + FEATURE_NER_NAME_CONCATENATION_TOKEN + governerEntity->entityName;	//join names together

					/*//OLD: before moving disableRedundantNodesStanfordCoreNLP() forward in execution heirachy (GIATranslatorDefineGrammarAndReferencing.cpp)
					if(governerEntity->hasAssociatedInstanceTemp)
					{//disable its property also
						(governerEntity->AssociatedInstanceNodeList.back())->entityName = governerEntity->entityName;	//join names together
					}	
					*/				
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		
		currentRelationInList = currentRelationInList->next;
	}
}
#endif




#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS


void redistributeStanfordRelationsMultiwordPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
		
#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION

	/* 
	need to consider this case for the following example text; 
	The patent claims are on the cart frame, the wheels and the golf bag connection mechanism.

	det(claims-3, The-1)
	nn(claims-3, patent-2)
	nsubj(are-4, claims-3)
	root(ROOT-0, are-4)
	det(frame-8, the-6)
	nn(frame-8, cart-7)
	prep_on(are-4, frame-8)
	det(wheels-11, the-10)
	prep_on(are-4, wheels-11)
	conj_and(frame-8, wheels-11)
	det(mechanism-17, the-13)
	nn(mechanism-17, golf-14)
	nn(mechanism-17, bag-15)
	nn(mechanism-17, connection-16)
	prep_on(are-4, mechanism-17)
	conj_and(frame-8, mechanism-17)

	*/

	//look for nsubj/prep combination, eg nsubj(are-4, claims-3) / prep_on(are-4, frame-8) => prep_on(claims-3, frame-8)
	//OLD: look for nsubj/prep combination, eg nsubj(next-4, garage-2) / prep_to(next-4, house-7)	=> prep_subj(next_to, house) / prep_subj(next_to, garage) 
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{	
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				
				while(currentRelationInList2->next != NULL)
				{					
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif	
						bool stanfordPrepositionFound = false;
						string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList2->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);

						if(stanfordPrepositionFound)
						{		
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching preposition of object-subject relationship

								#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION_OLD
								if(!(currentRelationInList2->prepositionCombinationAlreadyCreatedTemp))
								{																	
									string newPrepositionName = "";
									newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

									//cout << "newPrepositionName = " << newPrepositionName << endl;

									Relation * subjectOfPrepositionRelation = currentRelationInList;
									Relation * objectOfPrepositionRelation = currentRelationInList2;
									subjectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION;
									objectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
									GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName = newPrepositionName;
									currentRelationInList2->prepositionCombinationAlreadyCreatedTemp = true;
								}
								#else
									if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
									{

										currentRelationInList->disabled = true;
										currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationGovernor = currentRelationInList->relationDependent;

										disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]);
									}
								#endif
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}			
					#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}		
		#endif
			//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
#endif

	/*
	//stanford parser prepositition reduction check (based upon http://en.wikipedia.org/wiki/List_of_English_prepositions);
	Y The woman is white according to John.
	Y The job was complete apart from the axel.
	Y As for Tim, he knew no other way.
	Y As per physics, the ball fell.
	N As regards the apple.
	Y Aside from that, all was good.
	N They went back to the mall.
	Y It grew tall because of the water.
	N He is close to the house.
	N The computer broke due to the fire.
	Y All is fine except for the carrot.
	Y The house is far from the beach.
	Y The man fell into the boat.
	Y The kitten is inside of the box.
	Y The kettle is noisy instead of quite.
	N The house is left of the bank.
	N The carriage is near to the horse.
	N The farmer is next to the plank.
	N The chicken goes on to the plank.
	N The chicken when out of the box.
	N The man is outside of the house.
	N Owing to the weather, he bought the paint.
	Y Prior to the fall, he ate a pie.

	Y Regardless of the time, it will be done.
	N Right of the house, is the beach.
	N Subsequent to the holidays, it will be done.
	N Thanks to his results, he will watch tv.
	N is that of Tom's doing?
	N He reached up to the sky.
	N He eats chocolate, where as he is skinny.	[Only possible for stanford core nlp - not for stanford parser]

	N As far as they are concerned, nothing could be better. [Only possible for stanford core nlp - not for stanford parser]
	Y He bought a pie as well as the cake.

	collapse these prepositions;

	nsubj(broke-3, computer-2)
	acomp(broke-3, due-4)
	prep_to(due-4, fire-7)

	nsubj(went-2, They-1)
	advmod(went-2, back-3)
	prep_to(went-2, mall-6)

	nsubj(close-3, He-1)
	cop(close-3, is-2)
	prep_to(close-3, house-6)

	nsubjpass(left-4, house-2)
	auxpass(left-4, is-3)
	prep_of(left-4, bank-7)
	
		[DONE required to be removed based upon the following; In addition to fast assembly, time is saved by not having to connect and disconnect the golf cart and bag at the beginning and end of a game.]
		nsubjpass(saved-9, time-7)
		auxpass(saved-9, is-8)	
		prep_in_addition_to(saved-9, assembly-5)
		
	nsubj(near-4, carriage-2)
	cop(near-4, is-3)
	prep_to(near-4, horse-7)

	nsubj(next-4, farmer-2)
	cop(next-4, is-3)
	prep_to(next-4, plank-7

	nsubj(goes-3, chicken-2)
	prt(goes-3, on-4)
	prep_to(goes-3, plank-7)

	nsubj(outside-4, man-2)
	cop(outside-4, is-3)
	prep_of(outside-4, house-7)

	dobj(bought-7, paint-9)	
	partmod(bought-7, Owing-1)	//NB currently interpreted as obj	
	prep_to(Owing-1, weather-4)

	nsubj(beach-8, Right-1)
	cop(beach-8, is-6)
	prep_of(Right-1, house-4)

	nsubjpass(done-9, it-6)
	partmod(done-9, Subsequent-1)
	prep_to(Subsequent-1, holidays-4)

	partmod(watch-8, Thanks-1)
	prep_to(Thanks-1, results-4)
	nsubj(watch-8, he-6)

	nsubj(doing-6, that-2)		
	aux(doing-6, is-1)
	prep_of(that-2, Tom-4)
		
		[Not required to be removed based upon the following; Space is saved by not having a bulky cart to store at home and in the car. , because "to-10" is not "be-10"]
		nsubj(store-11, cart-9)
		aux(store-11, to-10)		
		prep_at(store-11, home-13)
		prep_in(store-11', car-17)
		
	
	nsubj(reached-2, He-1)
	prt(reached-2, up-3)
	prep_to(reached-2, sky-6)


	summary of multiword preposition contraction process;
	3. obj/subj (a, b)  
	2. aux, auxpass, cop, acomp, partmod, prt ) (a, z)  	    
	1. prep_x (a, c)
	->
	3. obj/subj
	1. prep_z_c(a, c)
	*/	
		
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			bool stanfordPrepositionFound = false;
			string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);

			if(stanfordPrepositionFound)
			{		
				//cout << "redistributeStanfordRelationsMultiwordPreposition(): stanfordPrepositionFound relexPreposition = " << relexPreposition << endl;

 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif
					//NB this assumes "cop/aux" etc relations cannot be disabled in fillGrammaticalArraysStanford
				
						bool multiwordPrepositionIntermediaryRelationTypeAFound = false;
						for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEA; i++)
						{
							if(currentRelationInList2->relationType == redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeA[i])
							{
								if(currentRelationInList2->relationDependent == RELATION_ENTITY_BE)
								{
									multiwordPrepositionIntermediaryRelationTypeAFound = true;
								}
							}
						}

						bool multiwordPrepositionIntermediaryRelationTypeBFound = false;
						for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEB; i++)
						{
							if(currentRelationInList2->relationType == redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeB[i])
							{
								multiwordPrepositionIntermediaryRelationTypeBFound = true;
							}
						}

						if(multiwordPrepositionIntermediaryRelationTypeAFound || multiwordPrepositionIntermediaryRelationTypeBFound)
						{
							//cout << "redistributeStanfordRelationsMultiwordPreposition(): multiwordPrepositionIntermediaryRelationFound relexPreposition = " << relexPreposition << ", intermediaryrelation = " << currentRelationInList2->relationType << endl;

							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching preposition of object-subject relationship

 								Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
								while(currentRelationInList3->next != NULL)
								{	
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
									if(!(currentRelationInList3->disabled))
									{			
									#endif								
										bool multiwordPrepositionSubjectOrObjectRelationFound = false;
										for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUBJOBJ_RELATIONS; i++)
										{
											//cout << "currentRelationInList3->relationType = " << currentRelationInList3->relationType << endl;
											if(currentRelationInList3->relationType == redistributionStanfordRelationsMultiwordPrepositionSubjObjRelations[i])
											{
												multiwordPrepositionSubjectOrObjectRelationFound = true;
											}
										}

										//cout << "SD1" << endl;

										if(multiwordPrepositionSubjectOrObjectRelationFound)
										{	
											//cout << "SD" << endl;

											if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
											{//found a matching preposition of object-subject relationship								

												//cout << "SD2" << endl;						
												if(!(currentRelationInList->prepositionCombinationAlreadyCreatedTemp))
												{		
													if(multiwordPrepositionIntermediaryRelationTypeAFound)
													{
														GIAEntityNode * entityContainingFirstWordOfMultiwordPreposition = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														//cout << "redistributeStanfordRelationsMultiwordPreposition(): newPrepositionName = " << newPrepositionName << endl;
														currentRelationInList->relationType = newPrepositionName;
														currentRelationInList->prepositionCombinationAlreadyCreatedTemp = true;

														currentRelationInList->relationGovernorIndex = currentRelationInList3->relationDependentIndex;
														currentRelationInList->relationGovernor =  GIAEntityNodeArray[currentRelationInList3->relationDependentIndex]->entityName;

														currentRelationInList2->disabled = true;

														disableEntityBasedUponFirstSentenceToAppearInNetwork(entityContainingFirstWordOfMultiwordPreposition);										

													}
													else if(multiwordPrepositionIntermediaryRelationTypeBFound)
													{
														GIAEntityNode * entityContainingFirstWordOfMultiwordPreposition = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														//cout << "redistributeStanfordRelationsMultiwordPreposition(): newPrepositionName = " << newPrepositionName << endl;
														currentRelationInList->relationType = newPrepositionName;
														currentRelationInList->prepositionCombinationAlreadyCreatedTemp = true;

														currentRelationInList2->disabled = true;

														disableEntityBasedUponFirstSentenceToAppearInNetwork(entityContainingFirstWordOfMultiwordPreposition);
													}

												}
											}
										}		
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
									}		
									#endif					

									currentRelationInList3 = currentRelationInList3->next;
								}
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}			
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}		
		#endif
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	
		
	//cout << "asd" << endl;
}



void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser)
{
	//eg The rabbit is 20 meters away. 	nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > _predadj(rabbit-2, away-6) 
	//OLD: nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > nsubj(away-6, rabbit-2) )

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif				
						if(currentRelationInList2->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
						{	
							if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
								{//found a matching object-subject relationship

									GIAEntityNode * oldRedundantBeEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];

									disableEntityBasedUponFirstSentenceToAppearInNetwork(oldRedundantBeEntity);

									#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
										#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ_OLD
										currentRelationInList2->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
										currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;

										currentRelationInList->disabled =  true;									
										#else
										currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
										currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
										currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationDependent = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;

										currentRelationInList2->disabled =  true;
										#endif
									#else
										currentRelationInList->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;

										currentRelationInList2->disabled = true;
									#endif
								}
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}			
					#endif							

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	
	
	//either do eg1 or eg2 ;
	//eg1 Kane is late 		nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3) 				[NB non-determinate of governer and dependent of subject relation; take as indicator of property]
	//or
	//eg2 She is the one 		nsubj(one-4, She-1) / cop(one-4, is-2) / det(one-4, the-3) -> appos(She-1, one-4)		[NB determinate of dependent of subject relation; take as an indicator of definition] 
	//or
	//eg3 The girl is tall 		nsubj(tall-6, girl-2) / cop(tall-6, is-3) / det(girl-2, The-1) -> _predadj(girl-2, tall-6) 	[NB non-plural and determinate of governer of subject relation; take as indicator of property]
	//or
	//eg4 bikes are machines  	nsubj(machines-3, bikes-1) / cop(machines-3, are-2) -> appos(bikes-1, machines-3)		[NB plural and non-determinate of governer of subject relation; take as an indicator of definition]
	//or
	//eg5 the wheels are green  	nsubj(green-6, wheels-4) / cop(green-6, are-5) -> _predadj(wheels-4, green-6)			[NB plural and determinate of governer of subject relation; take as indicator of property]
	//or
	//eg6 That is Jim.   		nsubj(Jim-3, That-1) / cop(Jim-3, is-2) -> appos(That-1, Jim-3)				
	//or
	//eg7 The time is 06:45		nsubj(06:45-4, time-2) / cop(06:45-4, is-3) / det(time-2, The-1) -> appos(time-2, 06:45-4)
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif	
					//NB this assumes "cop/aux" etc relations cannot be disabled in fillGrammaticalArraysStanford
					
						if(currentRelationInList2->relationType == RELATION_TYPE_COPULA) 	
						{	
							if(currentRelationInList2->relationDependent == RELATION_ENTITY_BE)
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
								{//found a matching object-subject relationship

									#ifndef GIA_OPTIMISE_PERFORMANCE_FOR_RELEX_PATENT_QUERIES_REPLICATION_RATHER_THAN_RELEX_PATENT_SYNTACTIC_PROTOTYPE_OUTPUT_REPLICATION
										#ifndef GIA_COLLAPSE_COP_RELATION_DEPENDENT_BE_TO_APPOS_NOT_PREDADJ_OLD`
										if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
										{
											GIAEntityNode * subjectGovernorEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
											
											bool subjectGovernorAdjectiveOrAdvebFound = false;
											for(int i=0; i<FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_TYPES; i++)
											{
												//cout << "subjectGovernorEntity->POSTemp = " << subjectGovernorEntity->POSTemp << endl;
												if(subjectGovernorEntity->POSTemp == featurePOSindicatesAdjectiveOrAdverbTypeArray[i])
												{
													subjectGovernorAdjectiveOrAdvebFound = true;
												}
											}											
											
											if(subjectGovernorAdjectiveOrAdvebFound)
											{
												currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
												currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
												currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
												currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
												currentRelationInList->relationDependent = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;							

												currentRelationInList2->disabled =  true;												
											}
											else
											{
												currentRelationInList->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
												currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
												currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
												currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
												currentRelationInList->relationDependent = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;							

												currentRelationInList2->disabled =  true;											
											}
										}
										else
										{
											cout << "warning: redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe() executed with (NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP) - performance substantially reduced" << endl;
										}
										#else
										bool detectedDeterminateOfSubjectGoverner = false;
										bool detectedDeterminateOfSubjectDependent = false;
										bool detectedDeterminate = false;
 										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{	

											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
											if(!(currentRelationInList3->disabled))
											{			
											#endif
												if(currentRelationInList3->relationType == RELATION_TYPE_DETERMINER) 	
												{	
													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
													{//found a matching object-subject relationship	
														//eg she is the one	nsubj(one-4, She-1) / cop(one-4, is-2) / det(one-4, the-3)
														detectedDeterminateOfSubjectGoverner = true;
														detectedDeterminate = true;
													}
													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex)	//redundant test
													{//found a matching object-subject relationship	
														//eg the girl is tall	nsubj(tall-6, girl-2) / cop(tall-6, is-3) / det(girl-2, The-1)
														detectedDeterminateOfSubjectDependent = true;
														detectedDeterminate = true;

														//eg the wheel is green		nsubj(green-6, wheels-4) / cop(green-6, are-5)
													}												
												}																			
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
											}			
											#endif	
											currentRelationInList3 = currentRelationInList3->next;
										}


										if(detectedDeterminateOfSubjectGoverner || !detectedDeterminate)
										{

											currentRelationInList->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
											currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
											currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
											currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
											currentRelationInList->relationDependent = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;							

											currentRelationInList2->disabled =  true;
										}
										if(detectedDeterminateOfSubjectDependent)
										{																
											currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
											currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
											currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
											currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
											currentRelationInList->relationDependent = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;							

											currentRelationInList2->disabled =  true;		
											/*//Not necessary; already disabled in fillGrammaticalArraysStanford;
											GIAEntityNode * oldRedundantBeEntity = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];							
											disableEntityBasedUponFirstSentenceToAppearInNetwork(oldRedundantBeEntity);
											*/						

										}
										#endif
									
									#else
										currentRelationInList->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
										currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
										currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
										currentRelationInList->relationDependent = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;							

										currentRelationInList2->disabled =  true;									
									#endif
								}
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}			
					#endif	

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}

}


void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg	The accident happened as the night was falling. 	advcl(happen, fall) / mark(fall, as) -> prep_as (happen, fall)
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif				
						bool partnerTypeRequiredFound = false;					
						if(currentRelationInList2->relationType == RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER)
						{
							partnerTypeRequiredFound = true;
						}

						if(partnerTypeRequiredFound)
						{		
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching object-subject relationship

								GIAEntityNode * oldPrepositionEntity = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];

								currentRelationInList2->disabled = true;
								disableEntityBasedUponFirstSentenceToAppearInNetwork(oldPrepositionEntity);

								string newRelationType = "";
								newRelationType = newRelationType + STANFORD_PARSER_PREPOSITION_PREPEND + oldPrepositionEntity->entityName;
								#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
								cout << "DEBUG: redistributeStanfordRelationsAdverbalClauseModifierAndComplement();" << endl;
								cout << "newRelationType = " << newRelationType << endl;
								#endif
								currentRelationInList->relationType = newRelationType;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}		
					#endif	
					
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}	
			

void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg	What she said makes sense. 	csubj (make, say) / dobj ( said-3 , What-1 )
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_OBJECT)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif					
						if(currentRelationInList2->relationType == RELATION_TYPE_CLAUSAL_SUBJECT)
						{		
							if(currentRelationInList2->relationDependentIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching object-subject relationship

								currentRelationInList2->relationType = RELATION_TYPE_SUBJECT;
								currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
								currentRelationInList2->relationDependent = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS	
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}			
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}				

void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			if(currentRelationInList->relationType == RELATION_TYPE_PHRASAL_VERB_PARTICLE)
			{
				//cout << "RELATION_TYPE_PHRASAL_VERB_PARTICLE" << endl;
				//eg They shut down the station. 	prt(shut, down) 			

				GIAEntityNode * governerEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAEntityNode * dependentEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
				governerEntity->entityName = governerEntity->entityName + "_" + dependentEntity->entityName;
				//cout << "governerEntity->entityName = " <<governerEntity->entityName << endl;

				disableEntityBasedUponFirstSentenceToAppearInNetwork(dependentEntity);
			}	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif			
		currentRelationInList = currentRelationInList->next;
	}
}


void redistributeStanfordRelationsConjunctionAndCoordinate(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg	I eat a pie or tom rows the boat. 	cc(pie-4, or-5)  / conj(pie-4, tom-6)
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_CONJUNCT)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif						
						bool partnerTypeRequiredFound = false;					
						if(currentRelationInList2->relationType == RELATION_TYPE_COORDINATION)
						{
							partnerTypeRequiredFound = true;
						}

						if(partnerTypeRequiredFound)
						{		
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching object-subject relationship

								string newRelationType = "";
								GIAEntityNode * coordinationDependentEntity = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
								string coordinationDependent = coordinationDependentEntity->entityName;
								if(coordinationDependent == RELATION_COORDINATION_DEPENDENT_AND)
								{
									newRelationType = RELATION_TYPE_CONJUGATION_AND;
								}
								else if(coordinationDependent == RELATION_COORDINATION_DEPENDENT_OR)
								{
									newRelationType = RELATION_TYPE_CONJUGATION_OR;
								}
								else
								{
									cout << "error redistributeStanfordRelationsConjunctionAndCoordinate(): unknown coordination dependent - " << coordinationDependent << endl;
								}
								currentRelationInList2->relationType = newRelationType;

								currentRelationInList->disabled = true;
								disableEntityBasedUponFirstSentenceToAppearInNetwork(coordinationDependentEntity);
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}			
					#endif	
					
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}
	
void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{	
	//eg	 The punter won almost $1000. 	advmod(won-3, almost-4) / pobj(almost-4, $-5) / num($-5, 1000-6)	[Relex: _obj(win[3], $[5])   / _quantity_mod($[5], almost[4])]
	//	convert to; _obj(win[3], $[5]) /  _quantity_mod($[5], almost[4])
			
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif					
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
						{		
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching object-subject relationship
							
 								Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
								while(currentRelationInList3->next != NULL)
								{	
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
									if(!(currentRelationInList3->disabled))
									{			
									#endif					
										if(currentRelationInList3->relationType == RELATION_TYPE_QUANTITY)
										{		
											if(currentRelationInList3->relationGovernorIndex == currentRelationInList2->relationDependentIndex)
											{//found a matching object-subject relationship


												int indexOfQuantityModifier = currentRelationInList->relationDependentIndex;						
												string quantityModifier = currentRelationInList->relationDependent;

												currentRelationInList->relationType = RELATION_TYPE_OBJECT;
												currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
												currentRelationInList->relationDependent = currentRelationInList2->relationDependent;

												currentRelationInList2->relationType = RELATION_TYPE_QUANTITY_MOD;
												currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
												currentRelationInList2->relationGovernor = currentRelationInList2->relationDependent;						
												currentRelationInList2->relationDependentIndex = indexOfQuantityModifier;
												currentRelationInList2->relationDependent = quantityModifier;
											}
										}
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
									}			
									#endif	
									currentRelationInList3 = currentRelationInList3->next;
								}
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}			
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}


void redistributeStanfordRelationsGenerateMeasures(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif
			//eg1	years old - npadvmod(old, years) -> _measure_time(old[7], years[6])		   {IRRELEVANT years: <NER>NUMBER</NER>} + old: <NER>DURATION</NER>
			//eg2	meters away - npadvmod(away-6, meters-5) -> _measure_distance(away[6], meter[5])  

			if(currentRelationInList->relationType == RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER)
			{
				//cout << "RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER" << endl;

				GIAEntityNode * governerEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAEntityNode * dependentEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];

				if(governerEntity->NERTemp == FEATURE_NER_DURATION)
				{
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_UNKNOWN;
					#else
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_TIME;
					#endif
				}
				else
				{
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_UNKNOWN;
				}
			}

			//eg3 dep(times-4, day-6) -> measure_dependency(times-4, day-6)			{Relex: _measure_per(times[4], day[6])}

			if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)
			{
				//cout << "RELATION_TYPE_DEPENDENT" << endl;

				GIAEntityNode * governerEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAEntityNode * dependentEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];

				if(dependentEntity->NERTemp == FEATURE_NER_DURATION)
				{
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN;
				}
				else
				{
					//do nothing
				}			
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif		
					
		currentRelationInList = currentRelationInList->next;
	}
}

void redistributeStanfordRelationsPrtAndTmod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//The disaster happened over night.   prt(happened-3, over-4) / tmod(happened-3, night-5) -> over(happened-3, night-5)

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			
			if(currentRelationInList->relationType == RELATION_TYPE_PHRASAL_VERB_PARTICLE)
			{	
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				
				while(currentRelationInList2->next != NULL)
				{					
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif	
						if(currentRelationInList2->relationType == RELATION_TYPE_TEMPORAL_MODIFIER)
						{		
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching preposition of object-subject relationship

								currentRelationInList->disabled = true;
								GIAEntityNode * oldPreposition = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
								string newPrepositionName = "";
								newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + currentRelationInList->relationDependent;	//oldPreposition->entityName
								currentRelationInList2->relationType = newPrepositionName;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}			
					#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}		
		#endif
			//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}	
}
	
#endif	




