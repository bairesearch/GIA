/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslatorGrammar.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1j 22-February-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAtranslatorGrammar.hpp"


#ifdef GIA_TRANSLATOR_WITHOUT_SYN_REL_TRANSLATOR_FEATURES
//prerequisites: requires GIA_PREPROCESSOR
bool GIAtranslatorGrammarClass::calculateGrammarUsingInferredPosTypes(GIApreprocessorWord* currentWord, GIAfeature* currentFeature)
{
	bool result = true;
	
	//calculate grammar information independent of external NLP (part 1; without using det/aux semantic relations):
	
	string wordText = currentWord->tagName;	//OLD: if GIA_SEM_REL_TRANSLATOR (ie !GIA_TXT_REL_TRANSLATOR_RULES_GIA3) then semantic parser corpus header must store wordOrig values not lemma values (such that word grammar/morphology can be derived)
	currentFeature->lemma = wordText;

	//calculate the PENN pos type of all GIA pos types (no specific cases);
	currentFeature->stanfordPOS = GIApreprocessorPOStypeCrossReferenceStanfordPos[currentWord->wordPOStypeInferred];
	
	//calculate the  PENN pos type/grammatical number of nouns;
	if(currentWord->wordPOStypeInferred == GIA_SEM_REL_TRANSLATOR_POS_TYPE_NOUN)	//NB is equivalent to GIA_PREPROCESSOR_POS_TYPE_NOUN
	{
		GIApreprocessorMultiwordReductionWord* nounBaseFormFound = NULL;
		if(GIApreprocessorMultiwordReduction.determineNounPluralVariant(wordText, &nounBaseFormFound))
		{
			currentFeature->lemma = nounBaseFormFound->tagName;
			currentFeature->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;	//what about words that dont have explicit plural modification e.g. "sheep"? (this is why an external dedicated POS tagger can be useful; because it should use context to derive morphology)
			currentFeature->stanfordPOS = FEATURE_POS_TAG_NOUN_NNS;
		}
		else
		{
			currentFeature->grammaticalNumber = GRAMMATICAL_NUMBER_SINGULAR;
			currentFeature->stanfordPOS = FEATURE_POS_TAG_NOUN_NN;
		}
	}

	//calculate the PENN pos type of verbs;
	string baseNameFound = "";
	int grammaticalBaseTenseForm = INT_DEFAULT_VALUE;
	bool foundVerbCaseStandardOrAdditional = GIApreprocessorMultiwordReduction.determineVerbCaseStandardWithAdditional(wordText, &baseNameFound, &grammaticalBaseTenseForm);
	if(foundVerbCaseStandardOrAdditional)
	{
		currentFeature->lemma = baseNameFound;

		if(currentWord->wordPOStypeInferred == GIA_SEM_REL_TRANSLATOR_POS_TYPE_VERB)	//NB is equivalent to GIA_PREPROCESSOR_POS_TYPE_VERB
		{
			//calculate the grammar of the 
			currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VB;
			if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VB;
			}
			else if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT)
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VB;	//FEATURE_POS_TAG_VERB_VBP/FEATURE_POS_TAG_VERB_VBZ
			}
			else if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS)
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VBG;
			}
			else if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST)
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VBD;
			}
			else if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE)
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VBN;
			}
			//what about irregular verb cases?
		}
		#ifndef GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
		#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
		if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL)
		{
			if(currentWord->wordPOStypeInferred == GIA_SEM_REL_TRANSLATOR_POS_TYPE_ADJECTIVE)	//NB "able" words will be marked as JJ/adjective or NN/noun by POS tagger
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VBPOTENTIAL;
			}
		}
		#endif
		#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
		else if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE)
		{
			if((currentWord->wordPOStypeInferred == GIA_SEM_REL_TRANSLATOR_POS_TYPE_ADJECTIVE))	//NB "ive" words will be marked as JJ/adjective or NN/noun by POS tagger )
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VBPOTENTIALINVERSE;
			}
		}
		#endif
		#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
		else if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST)	//removed 2h2h: || (grammaticalBaseTenseForm == INT_DEFAULT_VALUE)
		{
			if(currentWord->wordPOStypeInferred == GIA_SEM_REL_TRANSLATOR_POS_TYPE_ADJECTIVE)	//NB "is ..." and "is ..ed" (not Stanford CoreNLP/Relex) verbs may be marked as JJ/adjective by POS tagger eg "It is open"/"He is tired."
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VBSTATE;
			}
		}
		#endif
		#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
		else if(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION)
		{
			if(currentWord->wordPOStypeInferred == GIA_SEM_REL_TRANSLATOR_POS_TYPE_NOUN)	//NB "ion"/"ment" words will be marked as NN/noun by POS tagger
			{
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VERB_VBDESCRIPTION;
			}
		}
		#endif
		#endif	
	}

	if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_FIRST_MALE)
	{
		currentFeature->grammaticalGender = GRAMMATICAL_GENDER_MASCULINE;
		currentFeature->grammaticalIsProperNoun = true;
	}
	if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_FIRST_FEMALE)
	{
		currentFeature->grammaticalGender = GRAMMATICAL_GENDER_FEMININE;
		currentFeature->grammaticalIsProperNoun = true;
	}
	if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_FAMILY)
	{
		currentFeature->grammaticalIsProperNoun = true;
	}
	if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_TITLE)
	{
		currentFeature->grammaticalIsProperNoun = true;
	}
	if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_PLACE)	//not yet implemented as wordlistPropernounPlace.txt+multiwordlistPropernounPlace.txt is undefined
	{
		currentFeature->grammaticalIsProperNoun = true;
	}
	//#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES_SUPPORT_UPPERCASE_PROPERNOUN_WORD_LISTS
	if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_DATE)
	{
		currentFeature->grammaticalIsProperNoun = true;
	}
	//#endif
	
	/*
	cout << "currentWord->tagName = " << currentWord->tagName << endl;
	cout << "currentFeature->lemma = " << currentFeature->lemma << endl;
	cout << "currentWord->wordPOStypeInferred = " << currentWord->wordPOStypeInferred << endl;	
	cout << "currentWord->wordPOStypeInferred = " << GIApreprocessorPOStypeNameArray[currentWord->wordPOStypeInferred] << endl;	
	*/
	
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES_SUPPORT_UPPERCASE_PROPERNOUN_WORD_LISTS
	if(currentFeature->grammaticalIsProperNoun)
	{
	}
	else
	{
	#endif
		string lemmaLowerCase = SHAREDvars.convertStringToLowerCase(&(currentFeature->lemma));
		currentFeature->lemma = lemmaLowerCase;
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES_SUPPORT_UPPERCASE_PROPERNOUN_WORD_LISTS
	}
	#endif


		
	return result;	
}
#endif


void GIAtranslatorGrammarClass::locateAndAddAllFeatureTempEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		GIAtranslatorOperations.setFoundComparisonVariable(false);
	}

	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
		currentRelationInList->relationGovernor = performPrepositionReduction(currentRelationInList->relationGovernor);
		currentRelationInList->relationDependent = performPrepositionReduction(currentRelationInList->relationDependent);
		currentRelationInList->relationType = performPrepositionReduction(currentRelationInList->relationType);
		#endif

		string name[2];
		name[0] = currentRelationInList->relationGovernor;
		name[1] = currentRelationInList->relationDependent; 	//argumentName

		#ifdef GIA_RELEX
		if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{
			#ifdef GIA_SYN_REL_TRANSLATOR_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_PLUS_1
			if(currentRelationInList->relationDependentIndex < 0)
			{//to prevent Relex 1.4.0 error where it uses a relation argument index of '-1' very occasionally
				currentRelationInList->relationDependentIndex = (translatorVariables->currentSentenceInList->numberOfWordsInSentence) + GIA_SYN_REL_TRANSLATOR_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_PLUS_1_REPLACEMENT_INDEX;
			}
			#endif
		}
		#endif

		int relationIndex[2];
		relationIndex[0] = currentRelationInList->relationGovernorIndex;
		relationIndex[1] = currentRelationInList->relationDependentIndex;


		bool argumentIsQuery = false;
		#ifndef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
		/*presumably this code relates to the special case; if there is another relation entity (eg _%atLocation[1]) with the same feature index as $qVar (eg _$qVar[1])
		eg Where is the ball?
		_pobj(_%atLocation[1], _$qVar[1])
		_psubj(_%atLocation[1], ball[4])
		*/
		if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)	//ie if(NLPdependencyRelationsType != GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)		//updated 2d1a, OLD: if(NLPfeatureParser == GIA_NLP_PARSER_RELEX) //ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			if(name[1] == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
			{//modify relation index [to prevent overlapping of comparison variable indicies with other indicies]

				int questionComparisonVaribleIndex = (translatorVariables->currentSentenceInList->numberOfWordsInSentence) + GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
				#ifdef GIA_SYN_REL_TRANSLATOR_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
				//create a new feature and add it onto the feature list
				GIAfeature* currentFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
				while(currentFeatureInList->next != NULL)
				{
					currentFeatureInList = currentFeatureInList->next;
				}
				currentFeatureInList->entityIndex = questionComparisonVaribleIndex;
				GIAfeature* newFeature = new GIAfeature();
				currentFeatureInList->next = newFeature;
				#else
				//removed 19 July 2013 after failure to parse "Where is the ball?":
				//update feature->entityIndex using featureArrayTemp - added 1 May 2012 after Relex Failure detected [somewhere between 1j6b -> 1j6f]
				GIAfeature* featureArrayTemp[GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList)];
				GIAtranslatorOperations.generateTempFeatureArray(translatorVariables->currentSentenceInList->firstFeatureInList, featureArrayTemp);
				GIAfeature* featureOfQueryNode = (*translatorVariables->featureArrayTemp)[relationIndex[1]];
				featureOfQueryNode->entityIndex = questionComparisonVaribleIndex;
				#endif

				relationIndex[1] = questionComparisonVaribleIndex;
				currentRelationInList->relationDependentIndex = questionComparisonVaribleIndex;
				argumentIsQuery = true;
			}
		}
		#endif

		for(int i=0; i<2; i++)
		{
			if(!(*translatorVariables->GIAentityNodeArrayFilled)[relationIndex[i]])
			{
				(*translatorVariables->GIAentityNodeArrayFilled)[relationIndex[i]] = true;
				GIAentityNode* featureTempEntity = new GIAentityNode();
				featureTempEntity->entityName = name[i];
				(*translatorVariables->GIAfeatureTempEntityNodeArray)[relationIndex[i]] = featureTempEntity;

				#ifndef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
				if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)	//ie if(NLPdependencyRelationsType != GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)		//updated 2d1a, OLD: if(NLPfeatureParser == GIA_NLP_PARSER_RELEX) //ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					if(i == 1)
					{//argument index only
						if(argumentIsQuery)
						{
							(*translatorVariables->GIAfeatureTempEntityNodeArray)[relationIndex[i]]->isQuery = true;
						}
					}
				}
				#endif

				//this is required for fillGrammaticalArraysStanford findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet()	[nb these values are applied to networkIndex entities only]
				(*translatorVariables->GIAfeatureTempEntityNodeArray)[relationIndex[i]]->entityIndexTemp = relationIndex[i];
				(*translatorVariables->GIAfeatureTempEntityNodeArray)[relationIndex[i]]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
			}
		}

		currentRelationInList = currentRelationInList->next;
	}

	currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		bool prepositionFound = false;
		string prepositionName = GIAtranslatorOperations.convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound);
		if(prepositionFound)
		{
			int prepositionEntityIndex = INT_DEFAULT_VALUE;
			bool prepositionFeatureFound = GIAsynRelTranslatorGeneric.determineFeatureIndexOfPreposition(translatorVariables->currentSentenceInList, currentRelationInList, &prepositionEntityIndex);
			if(prepositionFeatureFound)
			{
				currentRelationInList->relationTypeIndex = prepositionEntityIndex;

				if(!((*translatorVariables->GIAentityNodeArrayFilled)[prepositionEntityIndex]))
				{
					(*translatorVariables->GIAentityNodeArrayFilled)[prepositionEntityIndex] = true;
					/*
					cout << "prepositionName = " << prepositionName << endl;
					cout << "prepositionEntityIndex = " << prepositionEntityIndex << endl;
					*/
					/*
					//NB if networkIndex type entity name has already been defined (GIAentityNodeArrayFilled[functionEntityIndex3]), then findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition will use it instead
					bool entityAlreadyExistant = false;
					GIAentityNode* entity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&prepositionName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);
					*/
					GIAentityNode* featureTempEntity = new GIAentityNode();
					featureTempEntity->entityName = prepositionName;
					featureTempEntity->entityIndexTemp = prepositionEntityIndex;
					featureTempEntity->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
					(*translatorVariables->GIAfeatureTempEntityNodeArray)[prepositionEntityIndex] = featureTempEntity;
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
}


void GIAtranslatorGrammarClass::locateAndAddAllNetworkIndexEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* featureTempEntityNode = (*translatorVariables->GIAfeatureTempEntityNodeArray)[w];

			bool entityAlreadyExistant = false;
			GIAentityNode* entity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&(featureTempEntityNode->entityName), &entityAlreadyExistant, translatorVariables, !(featureTempEntityNode->disabled));
			(*translatorVariables->GIAnetworkIndexNodeArray)[w] = entity;
			entity->hasAssociatedInstanceTemp = false;
			translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(entity);

			entity->entityIndexTemp = featureTempEntityNode->entityIndexTemp;
			entity->sentenceIndexTemp = featureTempEntityNode->sentenceIndexTemp;

			#ifndef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
			if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)	//ie if(NLPdependencyRelationsType != GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)		//updated 2d1a, OLD: if(NLPfeatureParser == GIA_NLP_PARSER_RELEX) //ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
			{
				if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->isQuery)
				{
					entity->isQuery = true;
					GIAtranslatorOperations.setFoundComparisonVariable(true);
					GIAtranslatorOperations.setComparisonVariableNode(entity);
				}
			}
			#endif
			#ifdef GIA_ALIASES
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->isNameQuery)
			{
				entity->isNameQuery = true;
			}
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->isName)
			{
				entity->isName = true;
			}
			#endif
			#ifdef GIA_SYN_REL_TRANSLATOR_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
			//this is required because negative assignment is now performed during redistribution
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->negative)
			{
				entity->negative = true;
			}
			#endif
			#ifndef NLC_PREPROCESSOR_MATH_OLD_NUMBER_OF_IMPLEMENTATION_USING_QVARS
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->isNumberOf)
			{
				entity->isNumberOf = true;
			}
			#endif
			#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->convertToSubClass)
			{
				entity->convertToSubClass = true;
			}
			#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->addSubClass)
			{
				entity->addSubClass = true;
			}
			#endif
			#endif
			#ifdef GIA_TRANSLATOR_EXPLETIVES
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->isExpletive)
			{
				entity->isExpletive = true;
			}	
			#endif
		}
	}
}





#ifdef GIA_RELEX
void GIAtranslatorGrammarClass::fillGrammaticalArraysRelex(GIAsentence* currentSentenceInList)
{
	GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{

		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_5A_RELATIONS_ASSIGN_TIME_NODES_IN_RELEX_THE_SAME_AS_STANFORD
		if((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_DATE_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		#else
		if(((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_DATE_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE) || ((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_TIME_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE))
		#endif
		{
			currentFeatureInList->grammaticalIsDateOrTime = true;
		}

		for(int grammaticalTenseIndex = 0; grammaticalTenseIndex < GRAMMATICAL_TENSE_NUMBER_OF_TYPES; grammaticalTenseIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name
			if((currentFeatureInList->grammar).find(grammaticalTenseNameArray[grammaticalTenseIndex]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			//if((currentFeatureInList->grammar).substr(0, grammaticalTenseNameLengthsArray[grammaticalTenseIndex]) == grammaticalTenseNameArray[grammaticalTenseIndex])
			{
				currentFeatureInList->grammaticalTense = grammaticalTenseIndex;
			}
		}
		for(int grammaticalTenseModifierIndex = 0; grammaticalTenseModifierIndex < GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name
			if((currentFeatureInList->grammar).find(grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				currentFeatureInList->grammaticalTenseModifierArray[grammaticalTenseModifierIndex] = true;
			}
		}

	#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_SUPPORT_SPECIAL_CASE_2A_GRAMMAR_TREAT_PRESENT_PERFECT_AS_PAST_TENSE
		//interpret "present_perfect" relex flag as past tense
		if(currentFeatureInList->grammaticalTense == GRAMMATICAL_TENSE_PRESENT)
		{
			if(currentFeatureInList->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PERFECT] == true)
			{
				currentFeatureInList->grammaticalTense = GRAMMATICAL_TENSE_PAST;
			}
		}
	#endif

		for(int grammaticalNumberIndex = 0; grammaticalNumberIndex < GRAMMATICAL_NUMBER_NUMBER_OF_TYPES; grammaticalNumberIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical number type name
			if((currentFeatureInList->grammar).find(grammaticalNumberNameArray[grammaticalNumberIndex]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				currentFeatureInList->grammaticalNumber = grammaticalNumberIndex;
			}
		}
		if((currentFeatureInList->grammar).find(GRAMMATICAL_DEFINITE_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			#ifdef GIA_RELEX_FIX_DO_NOT_ASSIGN_DEFINITE_IF_UNCOUNTABLE
			if(currentFeatureInList->grammaticalNumber != GRAMMATICAL_NUMBER_UNCOUNTABLE)
			{
			#endif
				currentFeatureInList->grammaticalIsDefinite = true;
			#ifdef GIA_RELEX_FIX_DO_NOT_ASSIGN_DEFINITE_IF_UNCOUNTABLE
			}
			#endif
		}
		if(currentFeatureInList->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
		{//added 2f11a 13-July-2014
			GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{
				if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_QUANTITY)
				{
					if(currentRelationInList->relationDependent == GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL)
					{
						if(currentRelationInList->relationGovernorIndex == currentFeatureInList->entityIndex)
						{
							cout << "relex GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL detected" << endl;
							//eg found _quantity(chicken[2], some[1]) in "Some chickens are happy."
							currentFeatureInList->grammaticalIsIndefinitePlural = true;
						}
					}

				}
				currentRelationInList = currentRelationInList->next;
			}
		}

		for(int grammaticalGenderIndex = 0; grammaticalGenderIndex < GRAMMATICAL_GENDER_NUMBER_OF_TYPES; grammaticalGenderIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical Gender type name
			if((currentFeatureInList->grammar).find(grammaticalGenderNameArray[grammaticalGenderIndex]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				//NB it will always find "person" in relex grammar string if "person" is existant, but this will be overwritten by "feminine" or "masculine" if this is specified (not possible for bigender names like "joe")
				currentFeatureInList->grammaticalGender = grammaticalGenderIndex;
			}
		}

		/*
		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_COUNT) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			currentFeatureInList->grammaticalHasCount = true;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "hasCount currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			#endif
		}
		*/

		if((currentFeatureInList->grammar).find(GRAMMATICAL_PRONOUN_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			currentFeatureInList->grammaticalIsPronoun = true;
		}

		//fill grammaticalWordTypeTemp array for wordnet - added 26 April 2012
		int grammaticalWordTypeTemp = GRAMMATICAL_WORD_TYPE_UNDEFINED;
		GIAtranslatorOperations.convertRelexPOStypeToWordnetWordType(&(currentFeatureInList->type), &grammaticalWordTypeTemp, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);
		currentFeatureInList->grammaticalWordType = grammaticalWordTypeTemp;

		#ifdef GIA_SYN_REL_TRANSLATOR_FILL_NER_ARRAY_AFTER_RELEX_PARSE_FOR_STANFORD_EQUIVALENT_PROPER_NOUN_DETECTION
		//fill NER array after Relex Parse for Stanford equivalent proper noun detection - added 26 April 2012
		for(int featureRelexFlagIndex = 0; featureRelexFlagIndex < FEATURE_RELEX_FLAG_NUMBER_OF_TYPES; featureRelexFlagIndex++)
		{
			if((currentFeatureInList->grammar).find(featureRelexFlagTypeArray[featureRelexFlagIndex]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				currentFeatureInList->NER = featureRelexFlagIndex;
			}
		}
		for(int i=0; i<FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_OF_TYPES; i++)
		{
			if(currentFeatureInList->NER == featureNERindicatesProperNounTypeArray[i])
			{
				currentFeatureInList->grammaticalIsProperNoun = true;
			}
		}
		#else
		if((currentFeatureInList->grammar).find(GRAMMATICAL_PERSON_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			currentFeatureInList->grammaticalIsProperNoun = true;
		}
		#endif

		//added 2h2a
		if((currentFeatureInList->grammaticalWordType == GRAMMATICAL_WORD_TYPE_VERB) && (currentFeatureInList->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PASSIVE]) && (currentFeatureInList->grammaticalTense == GRAMMATICAL_TENSE_PRESENT))
		{
			currentFeatureInList->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_STATE] = true;	//interpret present passive verbs as state - CHECKTHIS; verify this is an acceptable implementation
		}
		//TODO;
		//detect GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE
		//detect GRAMMATICAL_TENSE_MODIFIER_INFINITIVE

		currentFeatureInList = currentFeatureInList->next;
	}
}
#endif


#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford()
void GIAtranslatorGrammarClass::fillGrammaticalArraysStanford(GIAtranslatorVariablesClass* translatorVariables)
{
	//uses Stanford specific relations (grammar related)

	GIArelation* currentRelationInList = NULL;
	#ifdef GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER
	currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//definite/indefinite extraction:
		if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_DETERMINER)
		{
			string determiner = currentRelationInList->relationDependent;
			if(SHAREDvars.textInTextArray(determiner, relationDeterminerPotentiallySingularArray, GRAMMATICAL_DETERMINER_POTENTIALLY_SINGULAR_ARRAY_NUMBER_OF_TYPES))
			{
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;
				(*translatorVariables->featureArrayTemp)[entityIndexOfNoun]->determinerPotentiallySingularDetected = true;
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
	#endif

	//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
	extractGrammaticalInformationStanford(translatorVariables->currentSentenceInList->firstFeatureInList, translatorVariables->NLPfeatureParser);

	currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			//perfect tense extraction:
			if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_MODAL_AUX)
			{
				#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg aux (died, has) 	Reagan has died.	[addtogrammar: perfect?]
				int entityIndexOfAuxiliary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;
				(*translatorVariables->featureArrayTemp)[entityIndexOfVerb]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PERFECT] = true;
				(*translatorVariables->GIAfeatureTempEntityNodeArray)[entityIndexOfAuxiliary]->disabled = true;

				/*//this shouldnt be required, as verbs are automatically assumed not to be part of same reference set [see DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS]
				#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
				findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(translatorVariables->currentSentenceInList, (*translatorVariables->GIAfeatureTempEntityNodeArray)[entityIndexOfVerb]);
				#endif
				*/

				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILIARY_TO_SET_TENSE_OF_VERB
				extractPastTense(featureArrayTemp[entityIndexOfVerb], entityIndexOfAuxiliary, translatorVariables->currentSentenceInList->firstFeatureInList, NLPfeatureParser);
				#endif

			}

			//passive tense extraction:
			if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PASSIVE_AUX)
			{
				#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg auxpass(killed, been) Kennedy has been killed. 	[addtogrammar: passive]
				int entityIndexOfAuxiliary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;
				(*translatorVariables->featureArrayTemp)[entityIndexOfVerb]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PASSIVE] = true;
				(*translatorVariables->GIAfeatureTempEntityNodeArray)[entityIndexOfAuxiliary]->disabled = true;

				/*//this shouldnt be required, as verbs are automatically assumed not to be part of same reference set [see DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS]
				#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
				findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(translatorVariables->currentSentenceInList, GIAfeatureTempEntityNodeArray[entityIndexOfVerb]);
				#endif
				*/

				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILIARY_TO_SET_TENSE_OF_VERB
				extractPastTense(featureArrayTemp[entityIndexOfVerb], entityIndexOfAuxiliary, translatorVariables->currentSentenceInList->firstFeatureInList, GIAEntityNodeGrammaticalTenseArray, NLPfeatureParser);
				#endif

			}

			//past tense extraction:
			//NB copulas take precedence over auxillaries in formation of past tense (eg he has been sick;     nsubj ( sick-4 , he-1 ) / aux ( sick-4 , has-2 ) / cop ( sick-4 , been-3 ) )
			if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_COPULA)
			{
				#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg cop(smelled, sweet) 	The rose smelled sweet. [THIS APPEARS INCORRECT: stanford currently gives; acomp ( smelled-3 , sweet-4 )]
				//eg cop(black-5, was-4) 	Alice's cookie was black.

				int entityIndexOfCopula = currentRelationInList->relationDependentIndex;
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;
				string entityNameOfNoun = currentRelationInList->relationGovernor;

				(*translatorVariables->GIAfeatureTempEntityNodeArray)[entityIndexOfCopula]->disabled = true;

				#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
				findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(translatorVariables->currentSentenceInList, entityIndexOfNoun, &entityNameOfNoun);
				#endif

				extractPastTense((*translatorVariables->featureArrayTemp)[entityIndexOfNoun], entityIndexOfCopula, translatorVariables->currentSentenceInList->firstFeatureInList, translatorVariables->NLPfeatureParser);

			}

			//future tense extraction:
			//overwrite current tense derivations with GRAMMATICAL_TENSE_FUTURE if there is an auxiliary containing 'will'
			if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_MODAL_AUX)	//|| (currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PASSIVE_AUX)
			{
				#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				int auxiliaryDependencyIndex = currentRelationInList->relationGovernorIndex;
				string auxiliaryGovernerEntity = currentRelationInList->relationDependent;
				if(SHAREDvars.textInTextArray(auxiliaryGovernerEntity, relationAuxiliaryGovernerIndicatesFutureTenseArray, GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_AUXILIARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES))
				{
					(*translatorVariables->featureArrayTemp)[auxiliaryDependencyIndex]->grammaticalTense = GRAMMATICAL_TENSE_FUTURE;
				}
			}


			//definite/indefinite extraction:
			if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_DETERMINER)
			{
				#ifndef GIA_SYN_REL_TRANSLATOR_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg det(cookie, the) 	the cookie.
				string determiner = currentRelationInList->relationDependent;
				int entityIndexOfDeterminier = currentRelationInList->relationDependentIndex;
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;

				bool definiteDeterminerFound = false;
				if(SHAREDvars.textInTextArray(determiner, relationDeterminerGovernorDefiniteArray, GRAMMATICAL_DETERMINER_GOVERNOR_DEFINITE_ARRAY_NUMBER_OF_TYPES))
				{
					definiteDeterminerFound = true;
				}
				bool indefiniteDeterminerFound = false;
				if(SHAREDvars.textInTextArray(determiner, relationDeterminerGovernorIndefiniteArray, GRAMMATICAL_DETERMINER_GOVERNOR_INDEFINITE_ARRAY_NUMBER_OF_TYPES))
				{
					indefiniteDeterminerFound = true;
				}

				if(definiteDeterminerFound || indefiniteDeterminerFound)
				{//if condition added 4 July 2013 to ensure only real determiners (the, some, a) are disabled [and not "What" in det(time-2, What-1)]
					(*translatorVariables->GIAfeatureTempEntityNodeArray)[entityIndexOfDeterminier]->disabled = true;

					#ifdef STANFORD_CORENLP_POS_TAGS_BUG_GIA_WORKAROUND_SET_DETERMINER_DEPENDENT_TO_NOUN
					string stanfordPOS = FEATURE_POS_TAG_NOUN_NN;
					(*translatorVariables->featureArrayTemp)[entityIndexOfNoun]->stanfordPOS = stanfordPOS;
					extractPOSrelatedGrammaticalInformationStanford((*translatorVariables->featureArrayTemp)[entityIndexOfNoun], GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature - it should identify the verb as an infinitive/imperative based on previousWordInSentenceIsTo
					//applyPOSrelatedGrammaticalInfoToEntity((*translatorVariables->GIAfeatureTempEntityNodeArray)[entityIndexOfNoun], (*translatorVariables->featureArrayTemp)[entityIndexOfNoun]);	//regenerate grammatical information for entity - not required
					#endif
				}

				if(definiteDeterminerFound)
				{
					(*translatorVariables->featureArrayTemp)[entityIndexOfNoun]->grammaticalIsDefinite = true;

				}
				else if(determiner == GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL)
				{//added 2f11a 13-July-2014
					(*translatorVariables->featureArrayTemp)[entityIndexOfNoun]->grammaticalIsIndefinitePlural = true;
				}
				/*
				else if(determiner == GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR)
				{
					//no marking, in accordance with RelEx; 'doesn't mark [a] "book" at all'
				}
				*/

			}
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void GIAtranslatorGrammarClass::extractPastTense(GIAfeature* featureWithEntityIndex, const int entityIndexContainingTenseIndication, const GIAfeature* firstFeatureInList, const int NLPfeatureParser)
{
	//use the copular to set the tense of the noun

	const GIAfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		if(currentFeatureInList->entityIndex == entityIndexContainingTenseIndication)
		{
			extractPastTenseFromPOStag(&(currentFeatureInList->stanfordPOS), featureWithEntityIndex);
		}
		currentFeatureInList = currentFeatureInList->next;
	}
}
void GIAtranslatorGrammarClass::extractPastTenseFromPOStag(const string* POStag, GIAfeature* feature)
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
		feature->grammaticalTense = GRAMMATICAL_TENSE_PAST;		//fixed in 28 April 2012
	}
}

void GIAtranslatorGrammarClass::extractGrammaticalInformationStanford(GIAfeature* firstFeatureInList, const int NLPfeatureParser)
{
	bool toDetected = false;
	GIAfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		//added 10 April 2014 - GIA 2e1a
		if(toDetected)
		{
			currentFeatureInList->previousWordInSentenceIsTo = true;
		}
		if(currentFeatureInList->lemma == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PREPOSITION_TO)
		{
			toDetected = true;
		}
		else
		{
			toDetected = false;
		}

		int currentFeatureIndex = currentFeatureInList->entityIndex;

		extractPOSrelatedGrammaticalInformationStanford(currentFeatureInList, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);

		if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			if((currentFeatureInList->NER == FEATURE_NER_DATE) || (currentFeatureInList->NER == FEATURE_NER_TIME))
			{
				currentFeatureInList->grammaticalIsDateOrTime = true;
			}

			#ifdef GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_ASSIGNMENT_BUG_USE_NER_VALUES
			for(int i=0; i<FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_OF_TYPES; i++)
			{
				if(currentFeatureInList->NER == featureNERindicatesProperNounTypeArray[i])
				{
					currentFeatureInList->grammaticalIsProperNoun = true;
				}
			}
			#endif


			/*//NB the GIAEntityNodeGrammaticalIsProperNounArray array for stanford core nlp does not relate to persons (only proper nouns)
			if(currentFeatureInList->NER == FEATURE_NER_PERSON)
			{
				currentFeatureInList->grammaticalIsProperNounArray = true;
			}
			*/
		}

		/*
		cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
		cout << "currentFeatureInList->stanfordPOS = " << currentFeatureInList->stanfordPOS << endl;
		cout << "currentFeatureInList->type = " << currentFeatureInList->type << endl;
		cout << "currentFeatureInList->grammaticalWordType = " << currentFeatureInList->grammaticalWordType << endl;
		*/

		currentFeatureInList = currentFeatureInList->next;
	}
}

void GIAtranslatorGrammarClass::extractPOSrelatedGrammaticalInformationStanford(GIAfeature* currentFeature, const bool grammaticallyStrict)
{
	extractGrammaticalInformationFromStanfordPOStag(&(currentFeature->stanfordPOS), currentFeature);
	GIAtranslatorOperations.convertStanfordPOStagToRelexPOStypeAndWordnetWordType(&(currentFeature->stanfordPOS), &(currentFeature->type), &(currentFeature->grammaticalWordType), grammaticallyStrict);
}

//Preconditions: extractGrammaticalInformationStanford()/extractGrammaticalInformationFromStanfordPOStag() must be executed before relations (eg aux/cop) are processed, as they may [possibly] overwrite the tenses here established
void GIAtranslatorGrammarClass::extractGrammaticalInformationFromStanfordPOStag(const string* POStag, GIAfeature* feature)
{
	//past tense extraction;
	//this is required for past tense verbs without auxillaries; eg He ran fast.     nsubj ( ran-2 , He-1 ), advmod ( ran-2 , fast-3 ) .
	extractPastTenseFromPOStag(POStag, feature);


	//progressives tense extraction;
	bool progressiveDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagVerbProgressiveArray, FEATURE_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES))
	{
		progressiveDetected = true;
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] = true;
	}

	//infinitive tense extraction (added 28 July 2013) + imperative tense extraction (added 10 April 2014)
	bool infinitiveOrImperativeDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagVerbInfinitiveOrImperativeArray, FEATURE_POS_TAG_VERB_INFINITIVE_NUMBER_OF_TYPES))
	{
		infinitiveOrImperativeDetected = true;
		if(feature->previousWordInSentenceIsTo)
		{
			feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] = true;
		}
		else
		{
			feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE] = true;
		}
	}

	//singular/plural detection;
	bool pluralDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagPluralNounArray, FEATURE_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES))
	{
		pluralDetected = true;
		feature->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;
	}
	bool singularDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagSingularNounArray, FEATURE_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES))
	{
		#ifdef GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER
		if((*POStag) == FEATURE_POS_TAG_NOUN_NN)
		{
			if(feature->determinerPotentiallySingularDetected)
			{
				singularDetected = true;
			}
		}
		else
		{
		#endif
			singularDetected = true;
		#ifdef GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER
		}
		#endif
		if(singularDetected)
		{
			feature->grammaticalNumber = GRAMMATICAL_NUMBER_SINGULAR;
		}
	}

	//proper noun detection;
	bool properNounDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagProperNounArray, FEATURE_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES))
	{
		properNounDetected = true;
		feature->grammaticalIsProperNoun = true;
	}

	//pronoun detection;
	bool pronounDetected = false;
	//use stanfordPOS information to extract pronoun information - NB alternatively, could use referenceTypePersonNameArray and referenceTypePossessiveNameArray (as there is only a limited set of pronouns in english)
	if(SHAREDvars.textInTextArray(*POStag, featurePOSindicatesPronounTypeArray, FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_OF_TYPES))
	{
		pronounDetected = true;
		feature->grammaticalIsPronoun = true;
	}

	//added 2h2a
	#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
	//not detected by POS standard
	//"potential" tense extraction;
	bool potentialDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagVerbPotentialArray, FEATURE_POS_TAG_VERB_POTENTIAL_NUMBER_OF_TYPES))
	{
		potentialDetected = true;
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_POTENTIAL] = true;
	}
	#endif
	#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
	//not detected by POS standard
	//"potential inverse" tense extraction;
	bool potentialInverseDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagVerbPotentialInverseArray, FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE_NUMBER_OF_TYPES))
	{
		potentialInverseDetected = true;
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE] = true;
	}	
	#endif
	#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
	//not detected by POS standard
	//state/affection tense extraction (need to verify that Stanford CoreNLP/POS tags as VBN and Stanford Parser tags as JJ);
	bool stateDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagVerbStateArray, FEATURE_POS_TAG_VERB_STATE_NUMBER_OF_TYPES))
	{
		stateDetected = true;
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_STATE] = true;
	}
	#endif
	#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
	//added 2h2d
	//not detected by POS standard
	//"definition" tense extraction;
	bool definitionDetected = false;
	if(SHAREDvars.textInTextArray(*POStag, posTagVerbDescriptionArray, FEATURE_POS_TAG_VERB_DESCRIPTION_NUMBER_OF_TYPES))
	{
		definitionDetected = true;
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION] = true;
	}
	#endif
}

#endif





#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
void GIAtranslatorGrammarClass::findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(GIAsentence* currentSentenceInList, const int subjectObjectEntityWithAuxiliaryEntityIndex, const string* subjectObjectEntityWithAuxiliaryEntityName)
{
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
				}
			}

			for(int i=0; i<GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
				{
					passed = true;
				}
			}

			#ifndef GIA_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILIARY_AND_SET_DIFFERENT_REFERENCE_SET
			if(passed)
			{
			#endif
				if(subjectObjectEntityWithAuxiliaryEntityIndex = currentRelationInList->relationGovernorIndex)	//CHECK THIS; this used to be currentRelationInList->relationDependentIndex [before 1 June 2012]
				{
					if(*subjectObjectEntityWithAuxiliaryEntityName == currentRelationInList->relationGovernor)	//CHECK THIS; this used to be currentRelationInList->relationDependent [before 1 June 2012]
					{//this check is redundant
						currentRelationInList->auxiliaryIndicatesDifferentReferenceSet = true;
					}
				}
			#ifndef GIA_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILIARY_AND_SET_DIFFERENT_REFERENCE_SET
			}
			#endif
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}
#endif





void GIAtranslatorGrammarClass::applyGrammaticalInfoToAllEntities(GIAtranslatorVariablesClass* translatorVariables, GIAfeature* firstFeatureInSentence)
{
	int w = GIA_NLP_START_ENTITY_INDEX;
	GIAfeature* currentFeatureInList = firstFeatureInSentence;
	while(currentFeatureInList->next != NULL)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			#ifdef GIA_ADVANCED_REFERENCING
			if(!(entity->wasReference))	//added GIA 2a5a - required for NLC; do not overwrite isDefinite=false (from "a dog") with isDefinite=true (from "the dog") when the variable is being re-referenced in context
			{
			#endif
				#ifdef GIA_WORD_ORIG
				entity->wordOrig = currentFeatureInList->word;
				#endif

				entity->hasAssociatedTime = currentFeatureInList->grammaticalIsDateOrTime;

				applyPOSrelatedGrammaticalInfoToEntity(entity, currentFeatureInList);

				entity->grammaticalDefiniteTemp = currentFeatureInList->grammaticalIsDefinite;
				entity->grammaticalIndefinitePluralTemp = currentFeatureInList->grammaticalIsIndefinitePlural;
				entity->grammaticalProperNounTemp = currentFeatureInList->grammaticalIsProperNoun;
				entity->grammaticalGenderTemp = currentFeatureInList->grammaticalGender;
				#ifdef GIA_PREDETERMINERS
				entity->grammaticalPredeterminerTemp = currentFeatureInList->grammaticalPredeterminer;
				#endif

				entity->NERTemp = currentFeatureInList->NER;
				#ifdef GIA_STANFORD_CORENLP
				entity->NormalizedNERtemp = currentFeatureInList->NormalizedNER;
				entity->TimexTemp = currentFeatureInList->Timex;
				#endif
			#ifdef GIA_ADVANCED_REFERENCING
			}
			#endif
		}

		currentFeatureInList = currentFeatureInList->next;
		w++;
	}
}

void GIAtranslatorGrammarClass::applyPOSrelatedGrammaticalInfoToEntity(GIAentityNode* entity, GIAfeature* currentFeatureInList)
{
	for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
	{
		entity->grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = currentFeatureInList->grammaticalTenseModifierArray[grammaticalTenseModifierIndex];
	}
	entity->grammaticalTenseTemp = currentFeatureInList->grammaticalTense;
	#ifdef GIA_SPECIFIC_CONCEPTS
	if(!(entity->entityType == GIA_ENTITY_TYPE_CONCEPT))
	{
	#endif
		entity->grammaticalNumber = currentFeatureInList->grammaticalNumber;
	#ifdef GIA_SPECIFIC_CONCEPTS
	}
	#endif

	entity->grammaticalPronounTemp = currentFeatureInList->grammaticalIsPronoun;
	entity->grammaticalWordTypeTemp = currentFeatureInList->grammaticalWordType;

	#ifdef GIA_STANFORD_CORENLP
	entity->stanfordPOStemp = currentFeatureInList->stanfordPOS;
	#endif
}


