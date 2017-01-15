/*******************************************************************************
 *
 * File Name: GIATranslatorDefineGrammar.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j6c 01-May-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorDefineGrammar.h"
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
			
			//update feature->entityIndex using featureArrayTemp - added 1 May 2012 after Relex Failure detected 
			Feature * featureArrayTemp[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
			generateTempFeatureArray(currentSentenceInList->firstFeatureInList, featureArrayTemp);			
			Feature * featureOfQueryNode = featureArrayTemp[relationIndex[1]];
			featureOfQueryNode->entityIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
					
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
void fillGrammaticalArraysRelex(Sentence * currentSentenceInList)
{
	Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		//cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;

		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5A_RELATIONS_ASSIGN_TIME_NODES_IN_RELEX_THE_SAME_AS_STANFORD
		if((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_DATE_NAME) != -1)
		#else
		if(((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_DATE_NAME) != -1) || ((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_TIME_NAME) != -1))		
		#endif
		{
			currentFeatureInList->grammaticalIsDateOrTime = true;
			//cout << "isDate currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
		}

		for(int grammaticalTenseIndex = 0; grammaticalTenseIndex < GRAMMATICAL_TENSE_NUMBER_OF_TYPES; grammaticalTenseIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name 
			if((currentFeatureInList->grammar).find(grammaticalTenseNameArray[grammaticalTenseIndex]) != -1) 
			//if((currentFeatureInList->grammar).substr(0, grammaticalTenseNameLengthsArray[grammaticalTenseIndex]) == grammaticalTenseNameArray[grammaticalTenseIndex]) 
			{
				currentFeatureInList->grammaticalTense = grammaticalTenseIndex;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseIndex = " << urrentFeatureInList->grammaticalTense << endl;
			}			
		}
		for(int grammaticalTenseModifierIndex = 0; grammaticalTenseModifierIndex < GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name 
			if((currentFeatureInList->grammar).find(grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex]) != -1) 
			{
				//cout << "grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex] = " << grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex] << endl;
				currentFeatureInList->grammaticalTenseModifierArray[grammaticalTenseModifierIndex] = true;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseModifierIndex true = " << grammaticalTenseModifierArray[grammaticalTenseModifierIndex] << endl;
			}			
		}
		
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_2A_GRAMMAR_TREAT_PRESENT_PERFECT_AS_PAST_TENSE
		//interpret "present_perfect" relex flag as past tense
		if(currentFeatureInList->grammaticalTense == GRAMMATICAL_TENSE_PRESENT)
		{
			//cout << "a" << endl;
			if(currentFeatureInList->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PERFECT] == true)
			{
				//cout << "b" << endl;
				currentFeatureInList->grammaticalTense = GRAMMATICAL_TENSE_PAST;
			}		
		}
	#endif

		for(int grammaticalNumberIndex = 0; grammaticalNumberIndex < GRAMMATICAL_NUMBER_NUMBER_OF_TYPES; grammaticalNumberIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical number type name 
			if((currentFeatureInList->grammar).find(grammaticalNumberNameArray[grammaticalNumberIndex]) != -1) 				
			{
				currentFeatureInList->grammaticalNumber = grammaticalNumberIndex;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalNumberIndex = " << grammaticalNumberNameArray[grammaticalNumberIndex] << endl;
			}			
		}
		if((currentFeatureInList->grammar).find(GRAMMATICAL_DEFINITE_NAME) != -1)
		{
			currentFeatureInList->grammaticalIsDefinite = GRAMMATICAL_DEFINITE;
			//cout << "isDefinite currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;

		}			


		for(int grammaticalGenderIndex = 0; grammaticalGenderIndex < GRAMMATICAL_GENDER_NUMBER_OF_TYPES; grammaticalGenderIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical Gender type name 
			if((currentFeatureInList->grammar).find(grammaticalGenderNameArray[grammaticalGenderIndex]) != -1) 				
			{
				//NB it will always find "person" in relex grammar string if "person" is existant, but this will be overwritten by "feminine" or "masculine" if this is specified (not possible for bigender names like "joe")
				currentFeatureInList->grammaticalGender = grammaticalGenderIndex;
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalGenderIndex = " << grammaticalGenderNameArray[grammaticalGenderIndex] << endl;
			}			
		}

		/*
		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_COUNT) != -1)
		{
			currentFeatureInList->grammaticalHasCount = true;
			//cout << "hasCount currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
		}	
		*/

		if((currentFeatureInList->grammar).find(GRAMMATICAL_PRONOUN_NAME) != -1)
		{
			currentFeatureInList->grammaticalIsPronoun = GRAMMATICAL_PRONOUN;
			//cout << "isPronoun currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;	
		}					
	
		//fill wordNetPOS array for wordnet - added 26 April 2012
		int wordNetPOS = GRAMMATICAL_WORD_TYPE_UNDEFINED;
		convertRelexPOSTypeToWordnetWordType(&(currentFeatureInList->type), &wordNetPOS);
		currentFeatureInList->grammaticalWordType = wordNetPOS;
				
		#ifdef FILL_NER_ARRAY_AFTER_RELEX_PARSE_FOR_STANFORD_EQUIVALENT_PROPER_NOUN_DETECTION
		//fill NER array after Relex Parse for Stanford equivalent proper noun detection - added 26 April 2012
		for(int featureRelexFlagIndex = 0; featureRelexFlagIndex < FEATURE_RELEX_FLAG_NUMBER_TYPES; featureRelexFlagIndex++)
		{
			if((currentFeatureInList->grammar).find(featureRelexFlagTypeArray[featureRelexFlagIndex]) != -1) 				
			{
				currentFeatureInList->NER = featureRelexFlagIndex;
				/*
				#ifdef GIA_WORDNET_DEBUG
				cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex " << featureRelexFlagIndex << " = " << featureRelexFlagTypeArray[featureRelexFlagIndex] << endl;
				#endif
				*/
			}			
		}
		for(int i=0; i<FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_TYPES; i++)
		{
			if(currentFeatureInList->NER == featureNERindicatesProperNounTypeArray[i])
			{
				currentFeatureInList->grammaticalIsProperNoun = true;
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
			currentFeatureInList->grammaticalIsProperNoun = GRAMMATICAL_PERSON;
			//cout << "isPerson currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;

		}		
		#endif
				
		currentFeatureInList = currentFeatureInList->next;
	}
}
#endif


#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
void extractPastTenseFromPOStag(string * POStag, Feature * feature)
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
		//cout << "ABC2 entityIndex = " << feature->entityIndex << endl;
		feature->grammaticalTense = GRAMMATICAL_TENSE_PAST;		//fixed in 28 April 2012
	}
	//cout << "ABC3" << endl;
}

//Preconditions: extractGrammaticalInformationStanford()/extractGrammaticalInformationFromPOStag() must be executed before relations (eg aux/cop) are processed, as they may [possibly] overwrite the tenses here established
void extractGrammaticalInformationFromPOStag(string * POStag, Feature * feature)
{
	//past tense extraction;
	//this is required for past tense verbs without auxillaries; eg He ran fast.     nsubj ( ran-2 , He-1 ), advmod ( ran-2 , fast-3 ) . 
	extractPastTenseFromPOStag(POStag, feature);		


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
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] = true;
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
		feature->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;
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
		feature->grammaticalNumber = GRAMMATICAL_NUMBER_SINGULAR;
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
		feature->grammaticalIsProperNoun = true;
	}
	
	//pronoun detection;
	bool pronounDetected = false;
	//use stanfordPOS information to extract pronoun information - NB alternatively, could use referenceTypePersonNameArray and referenceTypePossessiveNameArray (as there is only a limited set of pronouns in english)
	for(int i=0; i<FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_TYPES; i++)
	{
		if(*POStag == featurePOSindicatesPronounTypeArray[i])
		{
			feature->grammaticalIsPronoun = true;
		}
	}
					
					
}


void extractGrammaticalInformationStanford(Feature * firstFeatureInList, int NLPfeatureParser)
{
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		Feature * currentFeatureInList = firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{	
			int currentFeatureIndex = currentFeatureInList->entityIndex;
			extractGrammaticalInformationFromPOStag(&(currentFeatureInList->stanfordPOS), currentFeatureInList);
				
			int wordNetPOS = GRAMMATICAL_WORD_TYPE_UNDEFINED;
			convertStanfordPOSTagToRelexPOSTypeAndWordnetWordType(&(currentFeatureInList->stanfordPOS), &(currentFeatureInList->type), &wordNetPOS);
			
			currentFeatureInList->grammaticalWordType = wordNetPOS;
			
			if((currentFeatureInList->NER == FEATURE_NER_DATE) || (currentFeatureInList->NER == FEATURE_NER_TIME))
			{
				currentFeatureInList->grammaticalIsDateOrTime = true;
			}
			/*//NB the GIAEntityNodeGrammaticalIsProperNounArray array for stanford core nlp does not relate to persons (only proper nouns)
			if(currentFeatureInList->NER == FEATURE_NER_PERSON)
			{
				currentFeatureInList->grammaticalIsProperNounArray = true;		
			}
			*/
					
			currentFeatureInList = currentFeatureInList->next;
		}	
	}
}

void extractPastTense(Feature * featureWithEntityIndex, int entityIndexContainingTenseIndication, Feature * firstFeatureInList, int NLPfeatureParser)
{
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		//use the copular to set the tense of the noun

		Feature * currentFeatureInList = firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{	
			if(currentFeatureInList->entityIndex == entityIndexContainingTenseIndication)
			{	
				//cout << "ABC entityIndex = " << entityIndexContainingTenseIndication << endl;
				extractPastTenseFromPOStag(&(currentFeatureInList->stanfordPOS), featureWithEntityIndex);
			}
			currentFeatureInList = currentFeatureInList->next;
		}
	}
}
#endif


	
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford() 
void fillGrammaticalArraysStanford(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser, Feature * featureArrayTemp[])
{
	//uses Stanford specific relations (grammar related)

	//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
	extractGrammaticalInformationStanford(currentSentenceInList->firstFeatureInList, NLPfeatureParser);					
	 
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
				#ifndef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
				currentRelationInList->disabled = true;
				#endif
							
				//eg aux (died, has) 	Reagan has died.	[addtogrammar: perfect?]
				int entityIndexOfAuxillary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;	
				featureArrayTemp[entityIndexOfVerb]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PERFECT] = true;		
				GIAEntityNodeArray[entityIndexOfAuxillary]->disabled = true;		

				//cout << "ABC0a" << endl;
				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB
				extractPastTense(featureArrayTemp[entityIndexOfVerb], entityIndexOfAuxillary, currentSentenceInList->firstFeatureInList, NLPfeatureParser);		
				#endif	

			}

			//passive tense extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
			{
				#ifndef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
				currentRelationInList->disabled = true;
				#endif
							
				//eg auxpass(killed, been) Kennedy has been killed. 	[addtogrammar: passive]	
				int entityIndexOfAuxillary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;
				featureArrayTemp[entityIndexOfVerb]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PASSIVE] = true;		
				GIAEntityNodeArray[entityIndexOfAuxillary]->disabled = true;				

				//cout << "ABC0b" << endl;
				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB
				extractPastTense(featureArrayTemp[entityIndexOfVerb], entityIndexOfAuxillary, currentSentenceInList->firstFeatureInList, GIAEntityNodeGrammaticalTenseArray, NLPfeatureParser);		
				#endif			

			}

			//past tense extraction:
			//NB copulas take precedence over auxillaries in formation of past tense (eg he has been sick;     nsubj ( sick-4 , he-1 ) / aux ( sick-4 , has-2 ) / cop ( sick-4 , been-3 ) )
			if(currentRelationInList->relationType == RELATION_TYPE_COPULA)
			{
				#ifndef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
				currentRelationInList->disabled = true;
				#endif
							
				//eg cop(smelled, sweet) 	The rose smelled sweet. [THIS APPEARS INCORRECT: stanford currently gives; acomp ( smelled-3 , sweet-4 )]
				//eg cop(black-5, was-4) 	Alice's cookie was black.

				int entityIndexOfCopula = currentRelationInList->relationDependentIndex;
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;
				GIAEntityNodeArray[entityIndexOfCopula]->disabled = true;	

				//cout << "ABC0c" << endl;
				//cout << "entityIndexOfNoun = " << entityIndexOfNoun << endl;
				//cout << "entityIndexOfCopula = " << entityIndexOfCopula << endl;
				extractPastTense(featureArrayTemp[entityIndexOfNoun], entityIndexOfCopula, currentSentenceInList->firstFeatureInList, NLPfeatureParser);

			}

			//future tense extraction:
			//overwrite current tense derivations with GRAMMATICAL_TENSE_FUTURE if there is an auxillary containing 'will'
			if(currentRelationInList->relationType == RELATION_TYPE_MODAL_AUX)	//|| (currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
			{
				#ifndef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
				currentRelationInList->disabled = true;
				#endif
							
				int auxillaryDependencyIndex = currentRelationInList->relationGovernorIndex;
				string auxillaryGovernerEntity = currentRelationInList->relationDependent;
				for(int i=0; i<RELATION_TYPE_AUXILLARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES; i++)
				{
					if(relationAuxillaryGovernerIndicatesFutureTenseArray[i] == auxillaryGovernerEntity)
					{
						featureArrayTemp[auxillaryDependencyIndex]->grammaticalTense = GRAMMATICAL_TENSE_FUTURE;
					}
				}
			}


			//definite/indefinite extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_DETERMINER)
			{
				#ifndef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
				currentRelationInList->disabled = true;
				#endif
				
				//cout << "RELATION_TYPE_DETERMINER = " << RELATION_TYPE_DETERMINER << endl;
				//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

				//eg det(cookie, the) 	the cookie. 
				string determiner = currentRelationInList->relationDependent;
				//cout << "determiner = " << determiner << endl;
				int entityIndexOfDeterminier = currentRelationInList->relationDependentIndex;
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;	
				GIAEntityNodeArray[entityIndexOfDeterminier]->disabled = true;					
				if(determiner == GRAMMATICAL_DETERMINER_DEFINITE)
				{		
					featureArrayTemp[entityIndexOfNoun]->grammaticalIsDefinite = true;
					
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



void applyGrammaticalInfoToAllConceptEntities(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], Feature * firstFeatureInSentence)
{
	int w = 1;
	Feature * currentFeatureInList = firstFeatureInSentence;
	while(currentFeatureInList->next != NULL)
	{	
		//cout << "w = " << w << endl;
		if(GIAEntityNodeArrayFilled[w])
		{
			GIAEntityNode * entity = GIAEntityNodeArray[w];
			//cout << "entity->entityName = " << entity->entityName << endl;
			
			entity->hasAssociatedTime = currentFeatureInList->grammaticalIsDateOrTime; 
			//cout << "entity->hasAssociatedTime = " << entity->hasAssociatedTime << endl;	

			for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
			{
				entity->grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = currentFeatureInList->grammaticalTenseModifierArray[grammaticalTenseModifierIndex];				
			}			
			entity->grammaticalTenseTemp = currentFeatureInList->grammaticalTense;
			entity->grammaticalNumberTemp = currentFeatureInList->grammaticalNumber;
			entity->grammaticalDefiniteTemp = currentFeatureInList->grammaticalIsDefinite;
			entity->grammaticalRelexPersonOrStanfordProperNounTemp = currentFeatureInList->grammaticalIsProperNoun;
			entity->grammaticalGenderTemp = currentFeatureInList->grammaticalGender;
			entity->grammaticalPronounTemp = currentFeatureInList->grammaticalIsPronoun;	
			entity->wordNetPOS = currentFeatureInList->grammaticalWordType;			
			
			entity->NERTemp = currentFeatureInList->NER;			
			#ifdef GIA_USE_STANFORD_CORENLP
			entity->stanfordPOSTemp = currentFeatureInList->stanfordPOS;
			//cout << "1 entity->stanfordPOSTemp = " << entity->stanfordPOSTemp << endl;			
			entity->NormalizedNERTemp = currentFeatureInList->NormalizedNER;
			entity->TimexTemp = currentFeatureInList->Timex;
			#endif
		}
		
		currentFeatureInList = currentFeatureInList->next;
		w++;
	}
}



