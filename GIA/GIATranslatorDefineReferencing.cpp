/*******************************************************************************
 *
 * File Name: GIATranslatorDefineReferencing.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1k2c 11-May-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorDefineReferencing.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"

//unordered_map<string, GIAEntityNode*> *conceptEntityNodesList
void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser)	
{	
	bool expectToFindComparisonVariable = false;
	if(currentSentenceInList->isQuestion)
	{
		expectToFindComparisonVariable = true;
		//cout << "expectToFindComparisonVariable" << endl;
	}

	if(expectToFindComparisonVariable)
	{
		#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
		if(NLPfeatureParser != GIA_NLP_PARSER_RELEX)	//ie if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			//cout << "here1" << endl;
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{
				if(GIAEntityNodeArrayFilled[i])
				{
					GIAEntityNode * entityNode = GIAEntityNodeArray[i];
					if(entityNode->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{
						entityNode->isQuery = true;
						setComparisonVariableNode(entityNode);		
						setFoundComparisonVariable(true);
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "foundComparisonVariable" << endl;				
						#endif
					}
				}			
			}
		}
		#endif

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

					#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
					if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
					{
					#endif						
						if((currentFeatureInList->word == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->type == FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME) && (currentFeatureInList->grammar == featureQueryWordAcceptedByAlternateMethodNameArray[i]))
						{//eg 1	which	which	adj	which
							//cout << "foundQueryWordAcceptedByAlternateMethod" << endl;
							foundQueryWordAcceptedByAlternateMethod = true;
						}
					#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET	
					}
					else if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
					{
						//cannot check the word value here, as the word recorded by the Stanford parser may be capitalised
						if((currentFeatureInList->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->stanfordPOS == FEATURE_POS_TAG_WDT))
						{//eg lemma=which, POS=WHT
							//cout << "foundQueryWordAcceptedByAlternateMethod" << endl;
							foundQueryWordAcceptedByAlternateMethod = true;
						}						
					}
					#endif

				}
				if(foundQueryWordAcceptedByAlternateMethod)
				{
					bool nounFound = false;	//finds the first noun after "which"
					#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
					if(currentFeatureInList->grammaticalWordType == GRAMMATICAL_WORD_TYPE_NOUN)
					{
						nounFound = true;
					}	
					#else					
					if(currentFeatureInList->type == FEATURE_RELEX_POS_TYPE_NOUN_NAME)
					{
						nounFound = true;
					}						
					#endif		
					if(nounFound && !foundComparisonVariableAlternateMethod)
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
				#ifdef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_RELEX_USE_ORIGINAL_KNOWN_WORKING_CODE
				ownerEntity->hasPropertyTemp2 = true;	//only required for linkPronounReferencesRelex(). Not necessary otherwise, as this is set in GIATranslatorOperations.cpp
				#endif
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
					#ifdef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_RELEX_USE_ORIGINAL_KNOWN_WORKING_CODE
					ownerEntity->hasPropertyTemp2 = true;	//only required for linkPronounReferencesRelex(). Not necessary otherwise, as this is set in GIATranslatorOperations.cpp
					#endif
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
				#ifdef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_RELEX_USE_ORIGINAL_KNOWN_WORKING_CODE
				ownerEntity->isSubjectTemp2 = true;	//only required for linkPronounReferencesRelex(). Not necessary otherwise, as this is set in GIATranslatorOperations.cpp
				#endif
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
				#ifdef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_RELEX_USE_ORIGINAL_KNOWN_WORKING_CODE
				ownerEntity->isObjectTemp2 = true;	//only required for linkPronounReferencesRelex(). Not necessary otherwise, as this is set in GIATranslatorOperations.cpp
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


#ifdef GIA_USE_STANFORD_CORENLP
void linkAllReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAConceptNodeArray[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, StanfordCoreNLPCoreference * firstCoreferenceInList, Feature * featureArrayTemp[])
{
	//cout << "linkAllReferencesStanfordCoreNLP(): error - function not yet coded" << endl;
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
			int referenceSourceSentenceIndex;			
			int referenceSourceEntityNodeIndex;
							
			if(!foundReferenceSource)
			{
				if(currentMentionInList->representative)
				{
					referenceSourceSentenceIndex = currentMentionInList->sentence;  		    
					referenceSourceEntityNodeIndex = currentMentionInList->head;
					
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
							GIAEntityNode * reference = GIAConceptNodeArray[currentSentenceEntityNodeIndex];
							
							bool coreferenceIsPronoun = false;
							//use stanfordPOS information to ensure that the reference is a pronoun - NB alternatively, could use referenceTypePersonNameArray and referenceTypePossessiveNameArray (as there is only a limited set of pronouns in english)
							for(int i=0; i<FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_TYPES; i++)
							{
								if(reference->stanfordPOSTemp == featurePOSindicatesPronounTypeArray[i])
								{
									coreferenceIsPronoun = true;
								}
							}
							
							if(coreferenceIsPronoun)
							{
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "referenceSourceHasBeenFound: assigning " << GIAEntityNodeArray[currentSentenceEntityNodeIndex]->entityName << " to " << referenceSource->entityName << "." << endl;
								#endif
								//referenceSource->isReferenceEntityInThisSentence = true;
								disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAConceptNodeArray[currentSentenceEntityNodeIndex]);

								GIAEntityNodeArray[currentSentenceEntityNodeIndex] = referenceSource;		//GIAConceptNodeArray[currentSentenceEntityNodeIndex] = referenceSource;
								featureArrayTemp[currentSentenceEntityNodeIndex]->isPronounReference = true;

								applyEntityAlreadyExistsFunction(referenceSource);
							}
							#ifndef GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION
							else
							{
								//now find the property in the referenceSource concept entity that matches the referenceSource sentence/entity index
								for(vector<GIAEntityNode*>::iterator entityIter = referenceSource->AssociatedInstanceNodeList.begin(); entityIter != referenceSource->AssociatedInstanceNodeList.end(); entityIter++) 
								{
									GIAEntityNode * property = *entityIter;
									if((property->sentenceIndexTemp == referenceSourceSentenceIndex) && (property->entityIndexTemp == referenceSourceEntityNodeIndex))
									{
										//found the property in the referenceSource concept entity that matches the referenceSource sentence/entity index 
										//now only link them
										#ifdef GIA_STANFORD_CORE_NLP_CODEPENDENCIES_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
										if(reference->grammaticalDefiniteTemp || reference->grammaticalRelexPersonOrStanfordProperNounTemp)
										{
											GIAEntityNodeArray[currentSentenceEntityNodeIndex] = property;
										}
										#else
										GIAEntityNodeArray[currentSentenceEntityNodeIndex] = property;
										#endif
									}	
								}
							}	
							#endif							
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
			
void linkPronounReferencesRelex(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, Feature * featureArrayTemp[])
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

										#ifndef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_RELEX_USE_ORIGINAL_KNOWN_WORKING_CODE
										if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp2) || (currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp))
										#else
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp2)																				
										#endif
										{
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp << endl;
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
										#ifndef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_RELEX_USE_ORIGINAL_KNOWN_WORKING_CODE
										else if(((currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp2) || (currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp)) && (s2 > 0))	//NB (currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp) check is redundant as (s2 > 0)										
										#else	
										else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp2) && (s2 > 0))
										#endif
										{
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp << endl;
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
										#ifndef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_RELEX_USE_ORIGINAL_KNOWN_WORKING_CODE
										else if(((currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp2) || (currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp)) && (s2 > 0))		//NB (currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp) check is redundant as (s2 > 0)										
										#else										
										else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp2) && (s2 > 0))
										#endif
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

						GIAEntityNodeArray[w] = referenceSource;
						featureArrayTemp[w]->isPronounReference = true;
						
						applyEntityAlreadyExistsFunction(referenceSource);
					}			
				}
			}
		}
	}
}



