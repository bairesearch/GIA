/*******************************************************************************
 *
 * File Name: GIATranslatorDefineGrammarAndReferencing.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i10c 12-Apr-2012
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
void fillGrammaticalArraysRelex(Sentence * currentSentenceInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[])
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

		if((currentFeatureInList->grammar).find(GRAMMATICAL_PERSON_NAME) != -1)
		{
			GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[currentFeatureInList->entityIndex] = GRAMMATICAL_PERSON;
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


#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
void extractPastTenseFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[])
{
	bool pastTenseDetected = false;
	
	//do not write if present tense found: NB copulas take precedence over auxillaries in formation of past tense (eg he has been sick;     nsubj ( sick-4 , he-1 ) / aux ( sick-4 , has-2 ) / cop ( sick-4 , been-3 )
	for(int i=0; i<GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PAST_NUMBER_OF_TYPES; i++)
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

//Preconditions: extractGrammaticalInformation()/extractGrammaticalInformationFromPOStag() must be executed before relations (eg aux/cop) are processed, as they may [possibly] overwrite the tenses here established
void extractGrammaticalInformationFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[])
{
	//past tense extraction;
	//this is required for past tense verbs without auxillaries; eg He ran fast.     nsubj ( ran-2 , He-1 ), advmod ( ran-2 , fast-3 ) . 
	extractPastTenseFromPOStag(POStag, entityIndex, GIAEntityNodeGrammaticalTenseArray);		


	//progressives tense extraction;
	bool progressiveDetected = false;
	for(int i=0; i<GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES; i++)
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
	for(int i=0; i<GIA_STANFORD_CORE_NLP_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES; i++)
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
	for(int i=0; i<GIA_STANFORD_CORE_NLP_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES; i++)
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
	for(int i=0; i<GIA_STANFORD_CORE_NLP_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagProperNounArray[i])
		{
			properNounDetected = true;
		}
	}
	if(properNounDetected)
	{
		GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[entityIndex] = true;
	}
					
}


void extractGrammaticalInformation(Feature * firstFeatureInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], string GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], int NLPfeatureParser)
{
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		Feature * currentFeatureInList = firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{	
			int currentFeatureIndex = currentFeatureInList->entityIndex;
			extractGrammaticalInformationFromPOStag(&(currentFeatureInList->POS), currentFeatureIndex, GIAEntityNodeGrammaticalTenseArray, GIAEntityNodeGrammaticalTenseModifierArray, GIAEntityNodeGrammaticalNumberArray, GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray);
			
			GIAEntityNodeNERArray[currentFeatureIndex] = currentFeatureInList->NER;
			GIAEntityNodeNormalizedNERArray[currentFeatureIndex] = currentFeatureInList->NormalizedNER;
			GIAEntityNodeTimexArray[currentFeatureIndex] = currentFeatureInList->Timex;
			
			if((currentFeatureInList->NER == FEATURE_NER_DATE) || (currentFeatureInList->NER == FEATURE_NER_TIME))
			{
				GIAEntityNodeIsDateOrStanfordTime[currentFeatureIndex] = true;
			}
			/*//NB the GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray array for stanford core nlp does not relate to persons (only proper nouns)
			if(currentFeatureInList->NER == FEATURE_NER_PERSON)
			{
				GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[currentFeatureIndex] = true;		
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
//NB GIAEntityNodeGrammaticalGenderArray and GIAEntityNodeGrammaticalIsPronounArray are not currently filled by fillGrammaticalArraysStanford() 
void fillGrammaticalArraysStanford(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], string GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], int NLPfeatureParser)
{
	//uses Stanford specific relations (grammar related)

	//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
	extractGrammaticalInformation(currentSentenceInList->firstFeatureInList, GIAEntityNodeIsDateOrStanfordTime, GIAEntityNodeGrammaticalTenseArray, GIAEntityNodeGrammaticalTenseModifierArray, GIAEntityNodeGrammaticalNumberArray, GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray, GIAEntityNodeNERArray, GIAEntityNodeNormalizedNERArray, GIAEntityNodeTimexArray, NLPfeatureParser);					

	 
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
	
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
		
		currentRelationInList = currentRelationInList->next;
	}
	
	
	

}
#endif



void applyGrammaticalInfoToAllConceptEntities(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], string GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[])
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
			entity->grammaticalRelexPersonOrStanfordProperNounTemp = GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[w];
			entity->grammaticalGenderTemp = GIAEntityNodeGrammaticalGenderArray[w];
			entity->grammaticalPronounTemp = GIAEntityNodeGrammaticalIsPronounArray[w];	
			
			entity->NERTemp = GIAEntityNodeNERArray[w];
			entity->NormalizedNERTemp = GIAEntityNodeNormalizedNERArray[w];
			entity->TimexTemp = GIAEntityNodeTimexArray[w];					
		}
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
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif	
		if(GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY)
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

					if(GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED)
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
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
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
						string tempString = currentRelationInList->relationDependent;
						int tempIndex = currentRelationInList->relationDependentIndex;
						currentRelationInList->relationDependent = currentRelationInList->relationGovernor;				
						currentRelationInList->relationGovernor = tempString;
						currentRelationInList->relationDependentIndex = currentRelationInList->relationGovernorIndex;				
						currentRelationInList->relationGovernorIndex = tempIndex;						
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
				if(currentMentionInList->representative = true)
				{
					int referenceSourceSentenceIndex = currentMentionInList->sentence;			
					int referenceSourceEntityNodeIndex = currentMentionInList->head;

					if(referenceSourceSentenceIndex < currentSentenceInList->sentenceIndex)
					{
						Sentence * currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInList;
						while(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous != NULL)
						{
							if(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->sentenceIndex == referenceSourceSentenceIndex)
							{
								Relation * currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->firstRelationInList;
								while(currentRelationInWhichReferenceSourceIsBeingSearchedFor->next != NULL)
								{
									long entityIndex = -1;
									bool entityAlreadyExistant = false;
									string entityName = currentRelationInWhichReferenceSourceIsBeingSearchedFor->relationDependent;	//CHECK THIS; assumes [at least one instance of] the reference source will always occur as a relation argument/dependent (ie, will not find the reference source if it only occurs as a relation function/governer)					
									vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
									long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
									long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();								
									GIAEntityNode * currentEntityInWhichReferenceSourceIsBeingSearchedFor = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &entityName, &entityAlreadyExistant, &entityIndex, false, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);

									if(entityAlreadyExistant)
									{
										referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										foundReferenceSource = true;
									}
																	
									currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentRelationInWhichReferenceSourceIsBeingSearchedFor->next;
								}
							}
							currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous;
						}
					}
				}
			}
			
			if(foundReferenceSource)
			{
				if(currentMentionInList->representative == false)
				{//continue only for references, eg pronoun (not for their source, eg name)			
					if(currentMentionInList->sentence == currentSentenceInList->sentenceIndex)
					{
						int currentSentenceEntityNodeIndex = currentMentionInList->head;
						if(GIAEntityNodeArrayFilled[currentSentenceEntityNodeIndex])
						{
							#ifdef GIA_TRANSLATOR_DEBUG
							cout << "referenceSourceHasBeenFound: assigning " << GIAEntityNodeArray[currentSentenceEntityNodeIndex]->entityName << " to " << referenceSource->entityName << "." << endl;
							#endif
							//referenceSource->isReferenceEntityInThisSentence = true;
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
							GIAEntityNodeArray[currentSentenceEntityNodeIndex]->disabled = true;
							#endif
							GIAEntityNodeArray[currentSentenceEntityNodeIndex] = referenceSource;
							GIAEntityNodeIsAReference[currentSentenceEntityNodeIndex] = true;
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
			
void linkReferences(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[])
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

void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg The rabbit is 20 meters away. 	nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > _predadj(rabbit-2, away-6) 
	//OLD: nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > nsubj(away-6, rabbit-2) )

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
				bool partnerTypeRequiredFound = false;					
				if(currentRelationInList2->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
				{
					partnerTypeRequiredFound = true;
				}

				if(partnerTypeRequiredFound)
				{	
					if((currentRelationInList->relationGovernor == RELATION_GOVERNOR_BE) && (currentRelationInList2->relationGovernor == RELATION_GOVERNOR_BE))
					{
						if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
						{//found a matching object-subject relationship

							GIAEntityNode * oldRedundantBeEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
							oldRedundantBeEntity->disabled = true;
							#endif
							
							#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
								currentRelationInList2->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
								currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
								currentRelationInList2->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
								#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
								currentRelationInList->disabled =  true;
								#endif								
								
							#else
								currentRelationInList->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
								currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;

								#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
								currentRelationInList2->disabled = true;
								#endif
							#endif
						}
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	
#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
	//eg Kane is late 	nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3) 

	currentRelationInList = currentSentenceInList->firstRelationInList;
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
				bool partnerTypeRequiredFound = false;					
				if(currentRelationInList2->relationType == RELATION_TYPE_COPULA)
				{
					partnerTypeRequiredFound = true;
				}

				if(partnerTypeRequiredFound)
				{	
					if(currentRelationInList2->relationDependent == RELATION_GOVERNOR_BE)
					{
						if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
						{//found a matching object-subject relationship

							
							currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
							currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
							currentRelationInList->relationGovernor = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
							currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
							currentRelationInList->relationDependent = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;							
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
							currentRelationInList2->disabled =  true;
							/*//Not necessary; already disabled in fillGrammaticalArraysStanford;
							GIAEntityNode * oldRedundantBeEntity = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];							
							oldRedundantBeEntity->disabled = true;
							*/
							#endif								
								
						}
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}

#endif	
}


void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg	The accident happened as the night was falling. 	advcl(happen, fall) / mark(fall, as) -> prep_as (happen, fall)
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
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
						
						#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
						currentRelationInList2->disabled = true;
						oldPrepositionEntity->disabled = true;
						#endif
						
						string newRelationType = "";
						newRelationType = newRelationType + STANFORD_PARSER_PREPOSITION_PREPEND + oldPrepositionEntity->entityName;
						#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
						cout << "DEBUG: redistributeStanfordRelationsAdverbalClauseModifierAndComplement();" << endl;
						cout << "newRelationType = " << newRelationType << endl;
						#endif
						currentRelationInList->relationType = newRelationType;
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
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
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_OBJECT)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
				bool partnerTypeRequiredFound = false;					
				if(currentRelationInList2->relationType == RELATION_TYPE_CLAUSAL_SUBJECT)
				{
					partnerTypeRequiredFound = true;
				}

				if(partnerTypeRequiredFound)
				{		
					if(currentRelationInList2->relationDependentIndex == currentRelationInList->relationGovernorIndex)
					{//found a matching object-subject relationship
						
						currentRelationInList2->relationType = RELATION_TYPE_SUBJECT;
						currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
						currentRelationInList2->relationDependent = GIAEntityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}				

void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(currentRelationInList->relationType == RELATION_TYPE_PHRASAL_VERB_PARTICLE)
		{
			//cout << "RELATION_TYPE_PHRASAL_VERB_PARTICLE" << endl;
			//eg They shut down the station. 	prt(shut, down) 			

			GIAEntityNode * governerEntity = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
			GIAEntityNode * dependentEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
			governerEntity->entityName = governerEntity->entityName + "_" + dependentEntity->entityName;
			//cout << "governerEntity->entityName = " <<governerEntity->entityName << endl;

			#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
			dependentEntity->disabled = true;
			#endif
		}			
		currentRelationInList = currentRelationInList->next;
	}
}



void redistributeStanfordRelationsNSubjAndPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//look for nsubj/prep combination, eg nsubj(next-4, garage-2) / prep_to(next-4, house-7)	=> prep_subj(next_to, house) / prep_subj(next_to, garage) 
	
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
				string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList2->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);
				
				if(stanfordPrepositionFound)
				{		
					if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
					{//found a matching preposition of object-subject relationship
					
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
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	//cout << "asd" << endl;
}


void redistributeStanfordRelationsConjunctionAndCoordinate(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	//eg	I eat a pie or tom rows the boat. 	cc(pie-4, or-5)  / conj(pie-4, tom-6)
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_CONJUNCT)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
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
						#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
						currentRelationInList->disabled = true;
						coordinationDependentEntity->disabled = true;
						#endif
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}
	
void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{	
	//eg	 The punter won almost $1000. 	advmod(won-3, almost-4) / pobj(almost-4, $-5)	[Relex: _obj(win[3], $[5])   / _quantity_mod($[5], almost[4])]
	//	convert to; _obj(win[3], $[5]) /  _quantity_mod($[5], almost[4])
			
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
				bool partnerTypeRequiredFound = false;					
				if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
				{
					partnerTypeRequiredFound = true;
				}

				if(partnerTypeRequiredFound)
				{		
					if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
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

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}


void redistributeStanfordRelationsGenerateMeasures(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
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
					
		currentRelationInList = currentRelationInList->next;
	}
}



	
	
#endif	





