/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslatorDefineReferencing.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3c1c 01-June-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorDefineReferencing.hpp"

//unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes
void GIAtranslatorDefineReferencingClass::identifyComparisonVariableAlternateMethod(GIAtranslatorVariablesClass* translatorVariables)
{
	bool expectToFindComparisonVariable = false;
	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		expectToFindComparisonVariable = true;
	}

	if(expectToFindComparisonVariable)
	{
		for(int i=0; i<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); i++)
		{
			if((*translatorVariables->GIAentityNodeArrayFilled)[i])
			{
				GIAentityNode* entityNode = (*translatorVariables->GIAentityNodeArray)[i];
				if(entityNode->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{
					entityNode->isQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
			}
		}
		
		#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
		if(!GIAtranslatorOperations.getFoundComparisonVariable())
		{//define comparison variable; define required answer entity as the next noun after the question word/lemma eg "house/person"

			bool foundComparisonVariableAlternateMethod = false;

			bool foundQueryWordAcceptedByAlternateMethod = false;
			const GIAfeature* currentFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
			while(currentFeatureInList->next != NULL)
			{
				bool passed = false;
				for(int i=0; i<FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES; i++)
				{
					/*
					//Not required because FEATURE_POS_TAG_WH_DETERMINER_WDT/FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME ensures that the correct which or what equivalent query is being asked:
					bool passedDependencyRelationCheck = false;
					if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
					{
						passedDependencyRelationCheck = true;
					}
					else
					{//ie if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
						GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
						while(currentRelationInList->next != NULL)
						{
							if(currentRelationInList->relationType == RELATION_TYPE_DETERMINER)	//only supported by Stanford
							{
								if(currentRelationInList->relationDependent == featureQueryWordAcceptedByAlternateMethodNameArray[i])
								{
									passedDependencyRelationCheck = true;
								}
							}
							currentRelationInList = currentRelationInList->next;
						}
					}

					if(passedDependencyRelationCheck)
					{
					*/

						#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
						if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
						{
						#endif
							if((currentFeatureInList->word == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->type == FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME) && (currentFeatureInList->grammar == featureQueryWordAcceptedByAlternateMethodNameArray[i]))
							{//eg 1	which	which	adj	which
								foundQueryWordAcceptedByAlternateMethod = true;
							}
						#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
						}
						else if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
						{
							//cannot check the word value here, as the word recorded by the Stanford parser may be capitalised
							if((currentFeatureInList->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i]) && (currentFeatureInList->stanfordPOS == FEATURE_POS_TAG_WH_DETERMINER_WDT))
							{//eg lemma=which, POS=WHT
								foundQueryWordAcceptedByAlternateMethod = true;
							}
						}
						#endif
					/*
					}
					*/

				}
				if(foundQueryWordAcceptedByAlternateMethod)
				{
					//NB For relex the designation of query variable in the special case "What [time] is it?" is now handled by redistributeRelexRelationsCollapseSubjectAndObjectGenerateAppos - as of 3 July 2013 - it used to be handled here

					bool nounFound = false;	//finds the first noun after "which"[/"what"]
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
						/*
						string queryComparisonVariableName = currentFeatureInList->word
						bool entityAlreadyExistant = false;
						GIAentityNode* queryComparisonVariableEntityNode = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&queryComparisonVariableName, &entityAlreadyExistant);
						//CHECK THIS; !applyNetworkIndexEntityAlreadyExistsFunction
						*/
						if((*translatorVariables->GIAentityNodeArrayFilled)[currentFeatureInList->entityIndex])
						{
							GIAentityNode* queryComparisonVariableEntityNode = (*translatorVariables->GIAentityNodeArray)[currentFeatureInList->entityIndex];
							queryComparisonVariableEntityNode->isQuery = true;
							queryComparisonVariableEntityNode->isWhichOrEquivalentWhatQuery = true;
							#ifdef GIA_WHICH_QUERY_ALIAS_ANSWERS
							queryComparisonVariableEntityNode->isNameQuery = true;
							#endif
							GIAtranslatorOperations.setFoundComparisonVariable(true);
							GIAtranslatorOperations.setComparisonVariableNode(queryComparisonVariableEntityNode);
						}
						else
						{
							cout << "error: !GIAentityNodeArrayFilled[indexOfAlternateComparisonVariableEntityNode]" << endl;
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
			cerr << "error: expectToFindComparisonVariable && foundComparisonVariable" << endl;
			exit(EXIT_ERROR);
		}
	}
	*/
}



//updated 22 May 2012 with GIAnetworkIndexNodeArray+GIAentityNodeArray (and substances add)
void GIAtranslatorDefineReferencingClass::linkPronounReferencesRelex(GIAtranslatorVariablesClass* translatorVariables)
{
	#ifdef GIA_WARNINGS
	cout << "warning: linkPronounReferencesRelex{} is not robust - it requires further development" << endl;
	#endif

	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* currentGIAEntityNode = (*translatorVariables->GIAfeatureTempEntityNodeArray)[w];
			for(int i=0; i< REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES; i++)
			{
				if(((currentGIAEntityNode->entityName == referenceTypePossessiveNameArray[i]) || (currentGIAEntityNode->entityName == referenceTypePersonNameArray[i])) && (currentGIAEntityNode->grammaticalPronounTemp))
				{//pronoun required for currentGIAEntityNode
					//now go for a search in tree for given / this sentence + previous sentence until find candidate reference source

					GIAentityNode* referenceSource = NULL;
					bool referenceSourceHasBeenFound = false;
					bool stillSentencesToSearch = true;
					GIAsentence* currentSentenceInWhichReferenceSourceIsBeingSearchedFor = translatorVariables->currentSentenceInList;
					int s2 = 0;
					while(!referenceSourceHasBeenFound && stillSentencesToSearch)
					{
						GIArelation* currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->firstRelationInList;
						int maxWordLimit = 999999;
						if(s2 == 0)
						{
							maxWordLimit = w;
						}
						int w2 = 0;
						while((currentRelationInWhichReferenceSourceIsBeingSearchedFor->next != NULL) && (w2 < maxWordLimit))
						{

							long entityIndex = -1;
							bool entityAlreadyExistant = false;

							string entityName = currentRelationInWhichReferenceSourceIsBeingSearchedFor->relationDependent;

							if(entityName != "")
							{
								bool entityAlreadyExistant = false;
								GIAentityNode* currentEntityInWhichReferenceSourceIsBeingSearchedFor = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&entityName, &entityAlreadyExistant, translatorVariables);
								//CHECK THIS; !applyNetworkIndexEntityAlreadyExistsFunction

								if(entityAlreadyExistant)
								{

									bool entityPassesGrammaticalTestsForReference = true;

									//if(referenceTypePersonCrossReferenceNumberArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED)
									//if(!((referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumber != referenceTypePersonCrossReferenceNumberArray[i])
										{
											entityPassesGrammaticalTestsForReference = false;
										}
									//}
									//if(referenceTypePersonCrossReferenceGenderArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED)
									//if(!((referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp != referenceTypePersonCrossReferenceGenderArray[i])
										{
											entityPassesGrammaticalTestsForReference = false;
										}
									//}
									//if(referenceTypePersonCrossReferenceDefiniteArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED)
									//if(!((referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp != referenceTypePersonCrossReferenceDefiniteArray[i])
										{
											entityPassesGrammaticalTestsForReference = false;
										}
									//}
									//if(referenceTypePersonCrossReferencePersonArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED)
									//if(!((referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
									//{
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalProperNounTemp != referenceTypePersonCrossReferencePersonArray[i])
										{
											entityPassesGrammaticalTestsForReference = false;
										}


										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPronounTemp)
										{
											entityPassesGrammaticalTestsForReference = false;
										}



									//}
									if(entityPassesGrammaticalTestsForReference)
									{


										referenceSourceHasBeenFound = true;
										referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										
										/*
										if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp)
										{
											#ifdef GIA_PRONOUN_REFERENCING_DEBUG
											cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp << endl;
											#endif
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
										else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp) && (s2 > 0))
										{
											#ifdef GIA_PRONOUN_REFERENCING_DEBUG
											cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp << endl;
											#endif
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
										else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasSubstanceTemp) && (s2 > 0))
										{
											#ifdef GIA_PRONOUN_REFERENCING_DEBUG
											cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasSubstanceTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasSubstanceTemp << endl;
											#endif
											referenceSourceHasBeenFound = true;
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
										}
										*/
									}
								}
								else
								{
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

						GIAtranslatorOperations.disableEntity((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]);

					#ifdef GIA_ORIGINAL_PRONOMINAL_COREFERENCE_RESOLUTION_IGNORE_SUBSTANCES_TAKE_NETWORK_INDEXES_ONLY
						(*translatorVariables->GIAnetworkIndexNodeArray)[w] = referenceSource;
						#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
						referenceSource->isPronounReference = true;
						#else
						(*translatorVariables->featureArrayTemp)[w]->isPronounReference = true;
						#endif
						GIAtranslatorOperations.applyNetworkIndexEntityAlreadyExistsFunction(referenceSource, true, true);
					#else

						bool networkIndexHasASubstance = false;
						//now find the substance in the referenceSource networkIndex entity that matches the referenceSource sentence/entity index
						for(vector<GIAentityConnection*>::iterator connectionIter = referenceSource->instanceNodeList->begin(); connectionIter != referenceSource->instanceNodeList->end(); connectionIter++)
						{
							GIAentityNode* substance = (*connectionIter)->entity;
							if(!networkIndexHasASubstance)
							{//take first instance/substance in list


								//found the substance in the referenceSource networkIndex entity that matches the referenceSource sentence/entity index
								//now only link them

								#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
								substance->isPronounReference = true;
								referenceSource->isPronounReference = true;
								#else
								(*translatorVariables->featureArrayTemp)[w]->isPronounReference = true;
								#endif
								(*translatorVariables->GIAentityNodeArray)[w] = substance;
								#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
								if(referenceSource->sentenceIndexTemp != translatorVariables->currentSentenceInList->sentenceIndex)
								{//Added condition GIA 2f7a - 06 July 2014 (wasReference is only used for intersentence references)
									substance->wasReference = true;
									referenceSource->wasReference = true;
								}
								#endif
							}

							networkIndexHasASubstance = true;
						}
						if(!networkIndexHasASubstance)
						{

							#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
							referenceSource->isPronounReference = true;
							#else
							(*translatorVariables->featureArrayTemp)[w]->isPronounReference = true;
							#endif
							(*translatorVariables->GIAentityNodeArray)[w] = referenceSource;
							#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
							if(referenceSource->sentenceIndexTemp != translatorVariables->currentSentenceInList->sentenceIndex)
							{//Added condition GIA 2f7a - 06 July 2014 (wasReference is only used for intersentence references)
								referenceSource->wasReference = true;
							}
							#endif
							GIAtranslatorOperations.applyNetworkIndexEntityAlreadyExistsFunction(referenceSource, true, true);
						}
					#endif
					}
				}
			}
		}
	}
}


#ifdef GIA_STANFORD_CORENLP
void GIAtranslatorDefineReferencingClass::linkPronounAndTextualContextReferencesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables, const GIAstanfordCoreNLPcoreference* firstCoreferenceInList)
{
	const GIAstanfordCoreNLPcoreference* currentCoreferenceInList = firstCoreferenceInList;
	while(currentCoreferenceInList->next != NULL)
	{
		const GIAstanfordCoreNLPmention* firstMentionInList = currentCoreferenceInList->firstMentionInList;
		const GIAstanfordCoreNLPmention* currentMentionInList = firstMentionInList;
		GIAentityNode* referenceSource = NULL;
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


					#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING_ONLY_ACCEPT_INTRASENTENCE_STANFORD_COREFERENCES
					if(referenceSourceSentenceIndex == translatorVariables->currentSentenceInList->sentenceIndex)
					#else
					if(referenceSourceSentenceIndex <= translatorVariables->currentSentenceInList->sentenceIndex)
					#endif
					{
						GIAsentence* currentSentenceInWhichReferenceSourceIsBeingSearchedFor = translatorVariables->currentSentenceInList;
						bool stillExistsPreviousSentence = true;
						while(stillExistsPreviousSentence)
						{
							if(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->sentenceIndex == referenceSourceSentenceIndex)
							{
								GIAfeature* currentFeatureInList = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->firstFeatureInList;
								while(currentFeatureInList->next != NULL)
								{
									if(currentFeatureInList->entityIndex == referenceSourceEntityNodeIndex)
									{
										string entityName = currentFeatureInList->lemma;	//CHECK THIS; assumes [at least one instance of] the reference source will always occur as a relation argument/dependent (ie, will not find the reference source if it only occurs as a relation function/governer)

										bool entityAlreadyExistant = false;
										GIAentityNode* currentEntityInWhichReferenceSourceIsBeingSearchedFor = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&entityName, &entityAlreadyExistant, translatorVariables);
										//CHECK THIS; !applyNetworkIndexEntityAlreadyExistsFunction


										if(entityAlreadyExistant)
										{
											referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;

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

					if(currentMentionInList->sentence == translatorVariables->currentSentenceInList->sentenceIndex)
					{
						int currentSentenceEntityNodeIndex = currentMentionInList->head;
						if((*translatorVariables->GIAentityNodeArrayFilled)[currentSentenceEntityNodeIndex])
						{
							GIAfeature* referenceFeature = (*translatorVariables->featureArrayTemp)[currentSentenceEntityNodeIndex];

							bool coreferenceIsPronoun = false;
							//use stanfordPOS information to ensure that the reference is a pronoun - NB alternatively, could use referenceTypePersonNameArray and referenceTypePossessiveNameArray (as there is only a limited set of pronouns in english)
							for(int i=0; i<FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_OF_TYPES; i++)
							{
								if(referenceFeature->stanfordPOS == featurePOSindicatesPronounTypeArray[i])
								{
									coreferenceIsPronoun = true;
								}
							}

							#ifdef GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION
							if(coreferenceIsPronoun)
							{
							#endif
								bool networkIndexHasASubstance = false;
								//now find the substance in the referenceSource networkIndex entity that matches the referenceSource sentence/entity index
								for(vector<GIAentityConnection*>::iterator connectionIter = referenceSource->instanceNodeList->begin(); connectionIter != referenceSource->instanceNodeList->end(); connectionIter++)
								{
									GIAentityNode* substance = (*connectionIter)->entity;
									if((substance->sentenceIndexTemp == referenceSourceSentenceIndex) && (substance->entityIndexTemp == referenceSourceEntityNodeIndex))
									{

										//found the substance in the referenceSource networkIndex entity that matches the referenceSource sentence/entity index
										//now only link them
										#ifdef GIA_STANFORD_CORE_NLP_CODEPENDENCIES_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
										if(referenceFeature->grammaticalDefinite || referenceFeature->grammaticalRelexPersonOrStanfordProperNoun)
										{
										#endif
											(*translatorVariables->GIAentityNodeArray)[currentSentenceEntityNodeIndex] = substance;
											#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
											#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE_NETWORK_INDICES
											referenceSource->entityIndexTemp = currentSentenceEntityNodeIndex;
											#endif
											cout << "substance->entityIndexTemp = " << substance->entityIndexTemp << endl;
											substance->entityIndexTemp = currentSentenceEntityNodeIndex;
											cout << "substance->entityIndexTemp = " << substance->entityIndexTemp << endl;
											if(currentSentenceEntityNodeIndex >= translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent)
											{
												translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = currentSentenceEntityNodeIndex + 1;
											}
											#endif
											
											#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
											if(referenceSourceSentenceIndex != translatorVariables->currentSentenceInList->sentenceIndex)
											{//Added condition GIA 2f7a - 06 July 2014 (wasReference is only used for intersentence references)
												substance->wasReference = true;
												referenceSource->wasReference = true;
											}
											#endif
											if(coreferenceIsPronoun) //shifted inside if statement "if(referenceFeature->grammaticalDefinite || referenceFeature->grammaticalRelexPersonOrStanfordProperNoun)" 12 October 2012
											{
												#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
												substance->isPronounReference = true;
												referenceSource->isPronounReference = true;
												#else
												(*translatorVariables->featureArrayTemp)[currentSentenceEntityNodeIndex]->isPronounReference = true;
												#endif
											}
										#ifdef GIA_STANFORD_CORE_NLP_CODEPENDENCIES_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
										}
										#endif


									}
									networkIndexHasASubstance = true;
								}
								#ifndef GIA_ORIGINAL_PRONOMINAL_COREFERENCE_RESOLUTION_IGNORE_SUBSTANCES_TAKE_NETWORK_INDEXES_ONLY
								if(!networkIndexHasASubstance)
								{

									if(coreferenceIsPronoun)
									{
										GIAtranslatorOperations.disableEntity((*translatorVariables->GIAfeatureTempEntityNodeArray)[currentSentenceEntityNodeIndex]);
										#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
										referenceSource->isPronounReference = true;
										#else
										(*translatorVariables->featureArrayTemp)[currentSentenceEntityNodeIndex]->isPronounReference = true;
										#endif
									}

									(*translatorVariables->GIAentityNodeArray)[currentSentenceEntityNodeIndex] = referenceSource;		//GIAnetworkIndexNodeArray[currentSentenceEntityNodeIndex] = referenceSource;
									#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
									#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE_NETWORK_INDICES
									referenceSource->entityIndexTemp = currentSentenceEntityNodeIndex;
									#endif
									if(currentSentenceEntityNodeIndex >= translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent)
									{
										translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = currentSentenceEntityNodeIndex + 1;
									}
									#endif
									#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
									if(referenceSourceSentenceIndex != translatorVariables->currentSentenceInList->sentenceIndex)
									{//Added condition GIA 2f7a - 06 July 2014 (wasReference is only used for intersentence references)
										referenceSource->wasReference = true;
									}
									#endif
									GIAtranslatorOperations.applyNetworkIndexEntityAlreadyExistsFunction(referenceSource, true, true);

								}
								#endif
							#ifdef GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION
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


void GIAtranslatorDefineReferencingClass::fillExplicitReferenceSameSetTags(GIAsentence* currentSentenceInList)
{
	//eg the chicken which ate a pie rowed the boat. rcmod(chicken-2, ate-4) / nsubj(ate-4, chicken-2)		 [OLD: + the blue chicken... amod(chicken-3, blue-2)]

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER)
			{
				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if((currentRelationInList->relationDependentIndex == currentRelationInList2->relationGovernorIndex) && (currentRelationInList->relationGovernorIndex == currentRelationInList2->relationDependentIndex))
						{//found matching rcmod
							currentRelationInList2->rcmodIndicatesSameReferenceSet = true;


							//link subject and object into same reference set if required
							bool passedSubject = false;
							bool passedObject = false;
							/*
							if(currentRelationInList2->relationType == RELATION_TYPE_SUBJECT)
							{
								passedSubject = true;
							}
							else if(currentRelationInList2->relationType == RELATION_TYPE_OBJECT)
							{
								passedObject = true;
							}
							*/
							for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeSubjectNameArray[i])
								{
									passedSubject = true;
								}
							}
							for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
								{
									passedObject = true;
								}
							}

							GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
 							while(currentRelationInList3->next != NULL)
							{
								if(currentRelationInList3->relationGovernorIndex == currentRelationInList2->relationGovernorIndex)
								{
									bool passedActionPartner = false;
									/*
									if(passedSubject)
									{
										if(currentRelationInList3->relationType == RELATION_TYPE_OBJECT)
										{
											passedActionPartner = true;
										}
									}
									else if(passedObject)
									{
										if(currentRelationInList3->relationType == RELATION_TYPE_SUBJECT)
										{
											passedActionPartner = true;
										}
									}
									*/
									if(passedSubject)
									{
										for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
										{
											if(currentRelationInList3->relationType == relationTypeObjectNameArray[i])
											{
												passedActionPartner = true;
											}
										}
									}
									else if(passedObject)
									{
										for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
										{
											if(currentRelationInList3->relationType == relationTypeSubjectNameArray[i])
											{
												passedActionPartner = true;
											}
										}
									}

									if(passedActionPartner)
									{
										//found action subject pair
										currentRelationInList3->rcmodIndicatesSameReferenceSet = true;

									}
								}

								currentRelationInList3 = currentRelationInList3->next;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}


int GIAtranslatorDefineReferencingClass::identifyReferenceSets(unordered_map<string, GIAentityNode*>* sentenceNetworkIndexEntityNodesList, const bool NLPdependencyRelationsType, vector<GIAentityNode*>* referenceSetDefiniteEntityList)
{

	bool haveSentenceEntityIndexOfDeterminers = false;
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		haveSentenceEntityIndexOfDeterminers = true;
	}

	this->resetReferenceSets(sentenceNetworkIndexEntityNodesList);

	int referenceSetID = 0;

	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIter = sentenceNetworkIndexEntityNodesList->begin(); networkIndexEntityNodesListIter != sentenceNetworkIndexEntityNodesList->end(); networkIndexEntityNodesListIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListIter->second;
		this->identifyReferenceSetNetworkIndexEntityEntrance(entityNode, &referenceSetID, haveSentenceEntityIndexOfDeterminers, referenceSetDefiniteEntityList);
	}

	int numberReferenceSets = referenceSetID;
	return numberReferenceSets;

}

void GIAtranslatorDefineReferencingClass::resetReferenceSets(unordered_map<string, GIAentityNode*>* sentenceNetworkIndexEntityNodesList)
{
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIter = sentenceNetworkIndexEntityNodesList->begin(); networkIndexEntityNodesListIter != sentenceNetworkIndexEntityNodesList->end(); networkIndexEntityNodesListIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListIter->second;
		entityNode->referenceSetID = GIA_REFERENCE_SET_ID_UNDEFINED;
	}
}

void GIAtranslatorDefineReferencingClass::identifyReferenceSetNetworkIndexEntityEntrance(GIAentityNode* entityNode, int* referenceSetID, const bool haveSentenceEntityIndexOfDeterminers, vector<GIAentityNode*>* referenceSetDefiniteEntityList)
{

	if(!(entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].empty()))
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].begin(); connectionIter < entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE].end(); connectionIter++)
		{
			GIAentityNode* currentInstance = (*connectionIter)->entity;

			#ifdef GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ONLY

			bool passDefiniteSetChecks = false;
			if(currentInstance->grammaticalDefiniteTemp)
			{
				passDefiniteSetChecks = true;
			}
			#ifdef GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ACCEPT_PROPERNOUNS
			if(currentInstance->grammaticalProperNounTemp)
			{
				passDefiniteSetChecks = true;
			}
			#endif
			#ifdef GIA_SPECIFIC_CONCEPTS
			if(currentInstance->entityType == GIA_ENTITY_TYPE_CONCEPT)	//&& !(currentInstance->grammaticalDefiniteTemp) - not required, as this is assumed the case already given (currentInstance->entityType == GIA_ENTITY_TYPE_CONCEPT); NB "the red dogs" will not be designated as isConcept - only "red dogs" will
			{
				passDefiniteSetChecks = true;
			}
			#endif
			#ifdef GIA_IDENTIFY_REFERENCE_SET_NETWORK_INDEX_ENTITY_ENTRANCE_DO_NOT_ENTER_ON_AN_ACTION_NODE
			if(currentInstance->entityType == GIA_ENTITY_TYPE_ACTION)
			{
				passDefiniteSetChecks = false;
			}
			#endif

			#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DISABLE_REFERENCING_OF_SUBCLASS_ARTIFICIAL_ENTITIES
			if(currentInstance->entityIndexTemp == GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX)
			{
				passDefiniteSetChecks = false;
			}
			#endif
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_NO_ADVANCED_REFERENCING
			if((currentInstance->entityName == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT) || (currentInstance->entityName == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT))	//not required as actions are not definite entities: || (currentInstance->entityName == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA)
			{
				passDefiniteSetChecks = false;
			}
			#endif

			if(passDefiniteSetChecks)
			{
			#endif


				int minimumEntityIndexOfReferenceSet = currentInstance->entityIndexTemp;	//assume that it is not an artificial property/condition entity (so it has a meaningful entity index with respect to the sentence contents)
				#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				if(entityTypesIsPropertyOrDefinitionRelationshipArray[currentInstance->entityType])
				{
					cerr << "GIAtranslatorDefineReferencingClass::identifyReferenceSetNetworkIndexEntityEntrance{} error: entityTypesIsPropertyOrDefinitionRelationshipArray[currentInstance->entityType]" << endl;
					exit(EXIT_ERROR);
				}
				#endif

				//currentInstance->minimumEntityIndexOfReferenceSet = minimumEntityIndexOfReferenceSet;	//added 28 Sept 2013


				if(this->identifyReferenceSetDetermineNextCourseOfAction(currentInstance, true,* referenceSetID, minimumEntityIndexOfReferenceSet, NULL))
				{
					*referenceSetID	= *referenceSetID + 1;
					referenceSetDefiniteEntityList->push_back(currentInstance);
				}

			#ifdef GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ONLY
				}
			#endif

		}
	}
}


#ifdef GIA_ADVANCED_REFERENCING

//based on answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork();
void GIAtranslatorDefineReferencingClass::createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(unordered_map<string, GIAentityNode*>* sentenceNetworkIndexEntityNodesList, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAcoreference* firstGIAcoreferenceInList, vector<GIAentityNode*>* referenceSetDefiniteEntityList, const GIAsentence* currentSentenceInList)	//bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]
{

	#ifdef GIA_DATABASE_CLEAR_CACHE_EVERY_SENTENCE
	#ifdef GIA_DATABASE
	int useDatabaseOriginal = GIAdatabase.getUseDatabase();
	if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
	{
		GIAdatabase.initialiseDBentityNodesActiveListCompleteFastIndexDBcache();
	}
	#endif
	#endif

	GIAcoreference* currentGIAcoreferenceInList = firstGIAcoreferenceInList;

	int referenceSetID = 0;
	for(vector<GIAentityNode*>::iterator referenceSetDefiniteEntityIter = referenceSetDefiniteEntityList->begin(); referenceSetDefiniteEntityIter != referenceSetDefiniteEntityList->end(); referenceSetDefiniteEntityIter++)
	{
		GIAentityNode* referenceSetDefiniteEntity = *referenceSetDefiniteEntityIter;


		GIAreferenceTraceParameters referenceTraceParameters;
		referenceTraceParameters.referenceSetID = referenceSetID;

		#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
		referenceTraceParameters.skipOverPropertyAndDefinitionRelationshipEntities = false;
		#endif
	
		#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
		referenceTraceParameters.referenceSetDefiniteEntity = referenceSetDefiniteEntity;
		//referenceTraceParameters.firstSentenceInList = firstSentenceInList;
		#endif

		int maxNumberOfMatchedNodes = 0;
		GIAentityNode* queryEntityWithMaxNumberNodesMatched = NULL;
		GIAentityNode* networkEntityWithMaxNumberNodesMatched = NULL;
		bool foundAtLeastOneMatch = false;

		unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery = sentenceNetworkIndexEntityNodesList;

		#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
		if(GIAtranslatorOperations.checkIfSentenceIsMathTextParsablePhrase(currentSentenceInList))
		{
			referenceTraceParameters.logicalConditionDisableTraceConcepts = true;
		}
		#endif

		#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
		referenceTraceParameters.traceConceptsOnly = true;
		#endif
		#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
		referenceTraceParameters.intrasentenceReference = false;
		#endif
		#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
		referenceTraceParameters.testReferenceSetContext = true;
		#endif
		#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
		referenceTraceParameters.doNotParseQuerySubnetsWithConcepts = true;
		#endif
		this->createGIAcoreferenceInListBasedUponIdentifiedReferenceSet(sentenceNetworkIndexEntityNodesList, entityNodesActiveListNetworkIndexes, &referenceTraceParameters, &maxNumberOfMatchedNodes, &queryEntityWithMaxNumberNodesMatched, &networkEntityWithMaxNumberNodesMatched, &foundAtLeastOneMatch);
		#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
		#ifdef GIA_DATABASE
		GIAdatabase.setUseDatabase(GIA_DATABASE_FALSE);
		#endif
		referenceTraceParameters.intrasentenceReference = true;
		#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
		referenceTraceParameters.testReferenceSetContext = false;
		#endif
		#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
		referenceTraceParameters.doNotParseQuerySubnetsWithConcepts = false;	//unnecessary as sentences with concepts will never contain intrasentence referencing... (eg "black birds are really fast, and black birds are really tall" does not require intrasentence referencing; where as "a black bird lives is really fast, and the black bird is tall" requires intrasentence referencing)
		#endif
		bool foundAtLeastOneMatchIntraSentence = false;
		this->createGIAcoreferenceInListBasedUponIdentifiedReferenceSet(sentenceNetworkIndexEntityNodesList, sentenceNetworkIndexEntityNodesList, &referenceTraceParameters, &maxNumberOfMatchedNodes, &queryEntityWithMaxNumberNodesMatched, &networkEntityWithMaxNumberNodesMatched, &foundAtLeastOneMatchIntraSentence);	//always perform intrasentence reference detection last (as this takes priority)
		if(foundAtLeastOneMatchIntraSentence)
		{
			foundAtLeastOneMatch = true;
			referenceTraceParameters.intrasentenceReference = true;		//already the case
			#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
			referenceTraceParameters.testReferenceSetContext = false;
			#endif
		}
		else
		{
			referenceTraceParameters.intrasentenceReference = false;
			#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
			referenceTraceParameters.testReferenceSetContext = true;
			#endif
		}
		#ifdef GIA_DATABASE
		GIAdatabase.setUseDatabase(useDatabaseOriginal);
		#endif
		#endif


		//now perform the final optimised trace
		if(foundAtLeastOneMatch)
		{
			GIAqueryTraceParameters queryTraceParameters;	//irrelevant

			int numberOfMatchedNodesTemp = 0;
			int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;


			//now set the matched nodes as already passed (required such that they are not retraced...)
			#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_REPLACE_ADVANCED_SEARCH
			bool result = GIAquery.testEntityNodeForQueryOrReferenceSet2(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, true, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, TRACE_MODE_IS_QUERY_FALSE, &queryTraceParameters, &referenceTraceParameters);
			#else
			bool result = GIAquery.testEntityNodeForQueryOrReferenceSet(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, true, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, TRACE_MODE_IS_QUERY_FALSE, &queryTraceParameters, &referenceTraceParameters);
			#endif

			//#ifdef GIA_ADVANCED_REFERENCING	//assumed true
			//required to set entityCorrespondingBestMatch of  the first node (networkIndex) in the reference set trace. This should not be required as networkIndex entity nodes of the same name (ie not pronouns) don't need referencing... It is just done for algorithmic consistency sake (considering other networkIndex nodes in the reference set trace will have their entityCorrespondingBestMatch value set)
			queryEntityWithMaxNumberNodesMatched->entityCorrespondingBestMatch = networkEntityWithMaxNumberNodesMatched;		//this shouldn't be required for queries....
			//#endif


			//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
			int irrelevantInt;
			string irrelevantString = "";
			bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
			GIAquery.traceEntityNode(queryEntityWithMaxNumberNodesMatched, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);		//is this required?	//added 13 July 2012
			GIAquery.traceEntityNode(networkEntityWithMaxNumberNodesMatched, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);					//added 13 July 2012

			//this routine should now record, for each query node, a corresponding (vector of) best match entity node [this 1-x mapping should be used in the final generation of GIAcoreference* firstGIAcoreferenceInList

			#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
			currentGIAcoreferenceInList = this->generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(queryEntityWithMaxNumberNodesMatched, currentGIAcoreferenceInList, foundAtLeastOneMatchIntraSentence);
			#else
			currentGIAcoreferenceInList = this->generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(queryEntityWithMaxNumberNodesMatched, currentGIAcoreferenceInList, BOOL_IRRELEVANT);
			#endif
			GIAquery.traceEntityNode(queryEntityWithMaxNumberNodesMatched, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON, &irrelevantInt, &irrelevantString, false, NULL, true);					//added 15 July 2012	//changed traceInstantiations to true 13 October 2012
		}


		referenceSetID++;
	}

	#ifdef GIA_DATABASE_CLEAR_CACHE_EVERY_SENTENCE
	#ifdef GIA_DATABASE
	if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
	{
		GIAdatabase.clearDBentityNodesActiveListCompleteFastIndexDBcache();
	}
	#endif
	#endif
}


void GIAtranslatorDefineReferencingClass::createGIAcoreferenceInListBasedUponIdentifiedReferenceSet(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const GIAreferenceTraceParameters* referenceTraceParameters, int* maxNumberOfMatchedNodes, GIAentityNode** queryEntityWithMaxNumberNodesMatched, GIAentityNode** networkEntityWithMaxNumberNodesMatched, bool* foundAtLeastOneMatch)
{
	int referenceSetID = referenceTraceParameters->referenceSetID;


	GIAqueryTraceParameters queryTraceParameters;	//irrelevant

	unordered_map<string, GIAentityNode*>::iterator entityIterQuery = entityNodesActiveListNetworkIndexesQuery->begin();
	GIAentityNode* firstNodeNetworkIndexEntityNodesListQuery = entityIterQuery->second;

	for(entityIterQuery = entityNodesActiveListNetworkIndexesQuery->begin(); entityIterQuery != entityNodesActiveListNetworkIndexesQuery->end(); entityIterQuery++)
	{//for each node in query semantic net;

		GIAentityNode* currentQueryEntityNode = entityIterQuery->second;
		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		if(!(currentQueryEntityNode->disabled))
		{
		#endif

			#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
			int irrelevant;
			string printEntityNodeString = "";
			int maxNumberOfMatchedNodesPossible = 0;
			bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
			GIAquery.traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY, &maxNumberOfMatchedNodesPossible, NULL, false, referenceSetID, traceInstantiations);
			GIAquery.traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
			#endif


			/*
			//removed 15 July 2012 - networkIndex entities do not have assigned referenceSetIDs [as there may be more than one instance of a networkIndex entity, each having a different reference set id (all within a given sentence)]
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			cout << "currentQueryEntityNode->referenceSetID = " << currentQueryEntityNode->referenceSetID << endl;
			cout << "referenceSetID = " << referenceSetID << endl;
			#endif
			if(currentQueryEntityNode->referenceSetID == referenceSetID)
			{
			*/
				bool saveNetwork = false;
				bool foundQueryEntityNodeName = false;
				string queryEntityNodeName = currentQueryEntityNode->entityName;

				GIAentityNode* networkIndexEntityMatchingCurrentQueryEntity = GIAdatabase.findOrAddNetworkIndexEntityNodeByName(NULL, entityNodesActiveListNetworkIndexes, &queryEntityNodeName, &foundQueryEntityNodeName, false, NULL, NULL, false);

				if(foundQueryEntityNodeName)
				{


					//now start matching structure search for all substances of the identical networkIndex node (to current query entity name) in Semantic Network

					int numberOfMatchedNodesTemp = 0;
					int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;
					#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_REPLACE_ADVANCED_SEARCH
					bool exactMatch = GIAquery.testEntityNodeForQueryOrReferenceSet2(currentQueryEntityNode, networkIndexEntityMatchingCurrentQueryEntity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, TRACE_MODE_IS_QUERY_FALSE, &queryTraceParameters, referenceTraceParameters);
					#else
					bool exactMatch = GIAquery.testEntityNodeForQueryOrReferenceSet(currentQueryEntityNode, networkIndexEntityMatchingCurrentQueryEntity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, TRACE_MODE_IS_QUERY_FALSE, &queryTraceParameters, referenceTraceParameters);
					#endif

					bool matchFound = false;
					if(referenceTraceParameters->traceMode == TRACE_MODE_ONLY_RECORD_EXACT_MATCH)
					{
						if(exactMatch)
						{
							matchFound = true;
						}
					}
					else
					{
						matchFound = true;
					}
					if(matchFound)
					{
						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
						if((numberOfMatchedNodesTemp >= *maxNumberOfMatchedNodes) && (numberOfMatchedNodesTemp > 1))		//this is required		//NB need to match > 1 nodes (ie, not just match the networkIndex node)
						#else
						if(numberOfMatchedNodesTemp > *maxNumberOfMatchedNodes)
						#endif
						{
							#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
							cout << "maxNumberOfMatchedNodesPossible = " << maxNumberOfMatchedNodesPossible << endl;
							cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;
							if(numberOfMatchedNodesTemp == maxNumberOfMatchedNodesPossible)
							{
							#endif
								*foundAtLeastOneMatch = true;

								*maxNumberOfMatchedNodes = numberOfMatchedNodesTemp;
								*queryEntityWithMaxNumberNodesMatched = currentQueryEntityNode;
								*networkEntityWithMaxNumberNodesMatched = networkIndexEntityMatchingCurrentQueryEntity;


							#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
							}
							#endif
						}
					}

					//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
					int irrelevantInt;
					string irrelevantString = "";
					bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
					GIAquery.traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);
					GIAquery.traceEntityNode(networkIndexEntityMatchingCurrentQueryEntity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);
				}
			/*
			}
			*/
		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		}
		#endif
	}
}

GIAcoreference* GIAtranslatorDefineReferencingClass::generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(GIAentityNode* entityNode, GIAcoreference* currentGIAcoreferenceInList, bool intrasentenceReference)
{
	bool pass = false;
	if(!(entityNode->testedForQueryComparison))
	{

		if(entityNode->entityCorrespondingBestMatch != NULL)
		{
			if(!(entityNode->instanceReverseNodeList->empty()))
			{//do not reference networkIndex entities - condition added 16 July 2012



				//now add the GIAcoReference to the list...
				GIAMention* sourceMention = currentGIAcoreferenceInList->firstMentionInList;
				sourceMention->representative = true;
				sourceMention->idActiveList = entityNode->entityCorrespondingBestMatch->idActiveList;
				sourceMention->entityIndex = entityNode->entityCorrespondingBestMatch->entityIndexTemp; 	//this is only used for intrasentence references
				sourceMention->entityName = entityNode->entityCorrespondingBestMatch->entityName;



				GIAMention* referenceMention = new GIAMention();
				referenceMention->representative = false;
				referenceMention->idActiveList = entityNode->idActiveList;
				referenceMention->entityIndex = entityNode->entityIndexTemp;
				referenceMention->entityName = entityNode->entityName;

				#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
				if(intrasentenceReference)
				{
					sourceMention->intrasentenceReference = true;
					referenceMention->intrasentenceReference = true;
				}
				#endif
				sourceMention->next = referenceMention;

				GIAMention* newMention  = new GIAMention();
				referenceMention->next = newMention;

				GIAcoreference* newCoreference = new GIAcoreference();
				currentGIAcoreferenceInList->next = newCoreference;
				currentGIAcoreferenceInList = currentGIAcoreferenceInList->next;
			}
		}

		entityNode->testedForQueryComparison = true;

		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
			{
				#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
				currentGIAcoreferenceInList = this->generateCoreferenceListBasedUponPreviouslyMatchedEntityNode((*connectionIter)->entity, currentGIAcoreferenceInList, intrasentenceReference);
				#else
				currentGIAcoreferenceInList = this->generateCoreferenceListBasedUponPreviouslyMatchedEntityNode((*connectionIter)->entity, currentGIAcoreferenceInList, BOOL_IRRELEVANT);
				#endif
			}
		}
	}


	return currentGIAcoreferenceInList;

}




void GIAtranslatorDefineReferencingClass::linkAdvancedReferencesGIA(GIAtranslatorVariablesClass* translatorVariables, GIAcoreference* firstCoreferenceInList)
{
	GIAcoreference* currentCoreferenceInList = firstCoreferenceInList;
	while(currentCoreferenceInList->next != NULL)
	{

		if(currentCoreferenceInList->firstMentionInList != NULL)
		{
			GIAMention* firstMentionInList = currentCoreferenceInList->firstMentionInList;

			GIAMention* currentMentionInList = firstMentionInList;
			GIAMention* sourceMentionInList = NULL;
			GIAentityNode* referenceSource = NULL;
			GIAentityNode* referenceSourceNetworkIndexEntity = NULL;
			#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
			GIAentityNode* referenceSourceNetworkIndex = NULL;
			#endif
			bool foundReferenceSource = false;
			int intrasentenceReferenceSourceIndex = -1;
			while(currentMentionInList->next != NULL)
			{

				if(!foundReferenceSource)
				{

					if(currentMentionInList->representative)
					{

						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
						if(currentMentionInList->intrasentenceReference)
						{
							int referenceSourceEntityIndex = currentMentionInList->entityIndex;
							//referenceSource = (*translatorVariables->GIAentityNodeArray)[referenceSourceEntityIndex];
							intrasentenceReferenceSourceIndex = referenceSourceEntityIndex;
							foundReferenceSource = true;
						}
						else
						{
						#endif
							bool entityNameFound = false;

							referenceSourceNetworkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&(currentMentionInList->entityName), &entityNameFound, translatorVariables);
							if(entityNameFound)
							{
								#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE	//should this condition be enforced?
								#ifdef GIA_DATABASE
								#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
								if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
								{
									GIAdatabase.DBreadVectorConnections(referenceSourceNetworkIndexEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE);		//read the associated instance vector connections from the database for the networkIndex node of the reference node (which has not necessarily already been loaded in the case of GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE) - enabling the reference node to be found linkAdvancedReferencesGIA (added 28 November 2012b)
								}
								#endif
								#endif
								#endif

								//now find the substance in the referenceSource networkIndex entity that matches the referenceSource idActiveList (there is no networkIndex linking; as networkIndex nodes are identical for reference and referenceSource)
								for(vector<GIAentityConnection*>::iterator connectionIter = referenceSourceNetworkIndexEntity->instanceNodeList->begin(); connectionIter != referenceSourceNetworkIndexEntity->instanceNodeList->end(); connectionIter++)
								{
									GIAentityNode* instance = (*connectionIter)->entity;


									if(instance->idActiveList == currentMentionInList->idActiveList)		//NB these activeList IDs [idActiveList] are not official - they are only used for referencing - and are overwritten
									{

										referenceSource = instance;
										#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
										referenceSourceNetworkIndex = referenceSourceNetworkIndexEntity;
										#endif
										foundReferenceSource = true;
										sourceMentionInList = currentMentionInList;

										//#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE	//should this condition be enforced?
										#ifdef GIA_DATABASE
										#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
										if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
										{
											for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
											{
												GIAdatabase.DBreadVectorConnections(referenceSource, i);	//read all vector connections for the reference (this is required because writeVectorConnection() presumes entityVectorConnectionsReferenceListLoadedArray[i] to be true in all cases - ie, in case the reference is ammended with new connections by GIATranslator which it is very likely to be the case) (added 28 November 2012b)
											}
										}
										#endif
										#endif
										//#endif
									}
								}
							}
						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
						}
						#endif
					}
				}

				if(foundReferenceSource)
				{

					if(!(currentMentionInList->representative))
					{//continue only for references, eg pronoun (not for their source, eg name)

						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
						if(currentMentionInList->intrasentenceReference)
						{
							int referenceEntityIndex = currentMentionInList->entityIndex;

							//create a new substance and share it between the reference and the reference source
							/*Removed 1 October 2013
							#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
							(*translatorVariables->GIAentityNodeArray)[referenceEntityIndex]->wasReference = true;	//assign to networkIndex
							#endif
							#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
							(*translatorVariables->GIAentityNodeArray)[referenceEntityIndex]->wasReferenceTemp = true;
							#endif
							*/
							(*translatorVariables->GIAentityNodeArray)[referenceEntityIndex] = GIAtranslatorOperations.addInstanceToInstanceDefinition((*translatorVariables->GIAentityNodeArray)[referenceEntityIndex], GIA_ENTITY_TYPE_SUBSTANCE, translatorVariables);
							(*translatorVariables->GIAentityNodeArray)[intrasentenceReferenceSourceIndex] = (*translatorVariables->GIAentityNodeArray)[referenceEntityIndex];

							/*Removed GIA 2f7a - 06 July 2014 (wasReference is only used for intersentence references)
							#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
							(*translatorVariables->GIAentityNodeArray)[intrasentenceReferenceSourceIndex]->wasReference = true;	//added 1 October 2013
							#endif
							*/
							/*Removed 1 October 2013
							#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
							(*translatorVariables->GIAentityNodeArray)[intrasentenceReferenceSourceIndex]->wasReferenceTemp = true;	//added 1 October 2013
							#endif
							*/


						}
						else
						{
						#endif
							int referenceEntityIndex = currentMentionInList->entityIndex;


							/*//2f19a 23-July-2014:
							#ifndef GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
							//copy reference aliases to referenceSource
							//required for; A red dog is fat. The name of the red dog is Tom.
							GIAentityNode* reference = GIAentityNodeArray[referenceEntityIndex];
							#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
							cout << "reference->entityName = " << reference->entityName << endl;
							#endif
							if(reference->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
							{
								cout << "linkAdvancedReferencesGIA{}: reference->isNetworkIndex" << endl;
							}
							if(reference->entityName != referenceSource->entityName)	//this may be required because the reference (with referenceEntityIndex as established by advanced referencing) is still a networkIndex and has not a substance created	//this will be required depending on which node was declared the primary (alias or non alias) when the  entities where merged during the temporary execution of convertSentenceSemanticRelationsIntoGIAnetworkNodes() on the sentence (sentenceNetworkIndexEntityNodesList) for the creation of GIA advanced referencing coreference sets
							{//advanced referencing has found an alias for the reference, merge it with the referenceSource
								referenceSource->aliasList.push_back(reference->entityName);
								#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
								cout << "linkAdvancedReferencesGIA{}: add reference name (alias) to reference source alias list: " << reference->entityName << endl;
								#endif
							}
							//for(int i((reference->aliasList).size() > 0)
							for(vector<string>::iterator aliasIter = reference->aliasList.begin(); aliasIter != reference->aliasList.end(); aliasIter++)	//this will be required depending on which node was declared the primary (alias or non alias) when the  entities where merged during the temporary execution of convertSentenceSemanticRelationsIntoGIAnetworkNodes() on the sentence (sentenceNetworkIndexEntityNodesList) for the creation of GIA advanced referencing coreference sets
							{//advanced referencing has found an alias for the reference, merge it with the referenceSource
								string referenceAlias = *aliasIter;
								#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
								cout << "referenceAlias = " << referenceAlias << endl;
								#endif
								if(referenceAlias != referenceSource->entityName)
								{
									referenceSource->aliasList.push_back(referenceAlias);
									#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
									cout << "linkAdvancedReferencesGIA{}: add reference alias to reference source alias list: " << referenceAlias << endl;
									#endif
								}
							}
							//what about; A red dog is fat. The name of the red dog is Tom. Tom rides the bike. (merge must copy aliases across)
							#endif
							*/

							#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
							if((*translatorVariables->GIAentityNodeArrayFilled)[referenceEntityIndex])
							{
							#endif
							#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
							}
							else
							{
								(*translatorVariables->GIAentityNodeArrayFilled)[referenceEntityIndex] = true;	//preposition reference
								(*translatorVariables->GIAfeatureTempEntityNodeArray)[referenceEntityIndex] = referenceSource;		//added for GIA 1n7c 31-July-2012 to correct bug as resultant of advanced referencing - fill in array
								(*translatorVariables->featureArrayTemp)[referenceEntityIndex] = new GIAfeature();				//added for GIA 1n7c 31-July-2012 to correct bug as resultant of advanced referencing - fill in array
								(*translatorVariables->GIAnetworkIndexNodeArray)[referenceEntityIndex] = referenceSource;			//added for GIA 1n7c 31-July-2012 to correct bug as resultant of advanced referencing - fill in array
							}
							#endif


							(*translatorVariables->GIAentityNodeArray)[referenceEntityIndex] = referenceSource;
							#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
							referenceSource->wasReferenceTemp = true;
							#endif
							#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
							referenceSource->wasReference = true;
							#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
							referenceSourceNetworkIndex->wasReference = true;
							#endif
							#endif
							#ifdef GIA_PREDETERMINERS
							if(referenceSource->grammaticalPredeterminerTemp == GRAMMATICAL_PREDETERMINER_UNDEFINED)
							{
								referenceSource->grammaticalPredeterminerTemp = (*translatorVariables->featureArrayTemp)[referenceEntityIndex]->grammaticalPredeterminer;	//only update the default (sentence independent) predeterminer of reference if no predeterminer found previously - NB the default (sentence independent) predeterminer is used by GIAxmlConversion only at present and shouldnt be used at all by !GIA_DISABLE_CROSS_SENTENCE_REFERENCING
							}
							#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
							referenceSource->grammaticalPredeterminerTempSentenceArray.insert(make_pair(currentSentenceInList->sentenceIndex, (*translatorVariables->featureArrayTemp)[referenceEntityIndex]->grammaticalPredeterminer));	//added 2i34a
							#endif
							#endif

							
							#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
							//cout << "linkAdvancedReferencesGIA: old referenceSource->entityIndexTemp = " << referenceSource->entityIndexTemp << endl;
							#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE_NETWORK_INDICES
							referenceSourceNetworkIndexEntity->entityIndexTemp = referenceEntityIndex;
							#endif
							referenceSource->entityIndexTemp = referenceEntityIndex; 
							if(referenceEntityIndex >= translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent)
							{
								translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = referenceEntityIndex + 1;
							}
							//cout << "linkAdvancedReferencesGIA: new referenceSource->entityIndexTemp = " << referenceSource->entityIndexTemp << endl;
							#endif

							#ifdef GIA_DATABASE
							if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
							{
								bool foundReferenceSourceInActiveList = false;
								GIAentityNode* tempNode = GIAdatabase.findEntityNodesActiveListCompleteFastIndexDBactive(&(referenceSource->entityName), referenceSource->idInstance, &foundReferenceSourceInActiveList);
								if(!foundReferenceSourceInActiveList)	//OR GIAdatabase.findEntityNodesActiveListCompleteFastIndexDBactive(referenceSource) OR if(!entityInActiveListComplete(referenceSource->entityName, referenceSource->idInstance))
								{
									GIAtranslatorOperations.addInstanceEntityNodeToActiveLists(referenceSource, translatorVariables);
									//referenceSource->modified = true;	//used to re-write the node to the database (in case it has been updated)
								}
							}
							#endif

						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
						}
						#endif


					}
				}

				currentMentionInList = currentMentionInList->next;
			}
		}
		currentCoreferenceInList = currentCoreferenceInList->next;
	}
}

#endif


#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
void GIAtranslatorDefineReferencingClass::identifyReferenceSetsSpecificConceptsAndLinkWithConcepts(GIAtranslatorVariablesClass* translatorVariables)
{
	int referenceSetID = 0;

	for(vector<GIAentityNode*>::iterator entityNodesActiveListCompleteIter = translatorVariables->entityNodesActiveListComplete->begin(); entityNodesActiveListCompleteIter != translatorVariables->entityNodesActiveListComplete->end(); entityNodesActiveListCompleteIter++)
	{
		GIAentityNode* currentSpecificConcept = *entityNodesActiveListCompleteIter;

		if(!(currentSpecificConcept->disabled))
		{
			if(currentSpecificConcept->entityType == GIA_ENTITY_TYPE_CONCEPT)
			{//implies !(currentSpecificConcept->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)

				int minimumEntityIndexOfReferenceSet = currentSpecificConcept->entityIndexTemp;	//assume that it is not an artificial property/condition entity (so it has a meaningful entity index with respect to the sentence contents)

				if(this->identifyReferenceSetDetermineNextCourseOfAction(currentSpecificConcept, true, referenceSetID, minimumEntityIndexOfReferenceSet, NULL))
				{
					bool traceModeIsQuery = false;

					GIAreferenceTraceParameters referenceTraceParameters;
					referenceTraceParameters.referenceSetID = referenceSetID;
					referenceTraceParameters.linkSpecificConceptsAndActions = true;

					#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
					int irrelevant;
					string printEntityNodeString = "";
					int maxNumberOfMatchedNodesPossible = 0;
					bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
					GIAquery.traceEntityNode(firstNodeNetworkIndexEntityNodesListQuery, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY, &maxNumberOfMatchedNodesPossible, NULL, false, referenceSetID, traceInstantiations);
					GIAquery.traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
					#endif

					for(vector<GIAentityNode*>::iterator entityNodesActiveListCompleteIter2 = translatorVariables->entityNodesActiveListComplete->begin(); entityNodesActiveListCompleteIter2 != translatorVariables->entityNodesActiveListComplete->end(); entityNodesActiveListCompleteIter2++)
					{
						
						GIAentityNode* entityNode = *entityNodesActiveListCompleteIter2;
						
						if(!(entityNode->disabled))
						{
							if(!(entityNode->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX))
							{
								GIAqueryTraceParameters queryTraceParameters;		//not used

								int numberOfMatchedNodesTemp = 0;
								int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;
								//bool exactMatch = testEntityNodeForQueryOrReferenceSet2(currentSpecificConcept, entityNode, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParameters, &referenceTraceParameters);
								bool exactMatch = GIAquery.testReferencedEntityNodeForExactNameMatch2(currentSpecificConcept, entityNode, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParameters, &referenceTraceParameters);

								if(exactMatch)
								{
									if(numberOfMatchedNodesTemp > 0)
									{
										#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
										if(numberOfMatchedNodesTemp == maxNumberOfMatchedNodesPossible)
										{
										#endif
											bool sameReferenceSet = false;

											#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS_ADVANCED
											if(entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT)
											{
												//eg3 Blue birds are tall. The happy eagle is a blue bird.

												/*
												//this method may not be appropriate for GIA_ADVANCED_REFERENCING; if concepts advanced reference each other in the future:
												if(!(entityNode->definitionReverseNodeList->empty())
												{
													GIAentityNode* instanceEntity = (entityNode->definitionReverseNodeList->begin())->entity;	//take the first entity, and ensure its sentenceID is identical to that of its substance; indicated it was the original definition declared in the sentence

												}
												*/
												GIAentityNode* instanceEntity = NULL;;
												for(vector<GIAentityConnection*>::iterator definitionNodeReverseListIterator = entityNode->definitionReverseNodeList->begin(); definitionNodeReverseListIterator < entityNode->definitionReverseNodeList->end(); definitionNodeReverseListIterator++)
												{
													if((*definitionNodeReverseListIterator)->entity->sentenceIndexTemp == entityNode->sentenceIndexTemp)
													{
														instanceEntity = (*definitionNodeReverseListIterator)->entity;
													}
												}
												if(instanceEntity != NULL)
												{
													translatorVariables->sentenceIndex = instanceEntity->sentenceIndexTemp;	//sentenceIndex here is artifical, could equally use currentSpecificConcept->sentenceIndexTemp
													GIAtranslatorOperations.addDefinitionRelationshipToEntity2(instanceEntity, currentSpecificConcept, sameReferenceSet, translatorVariables);
													#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
													entityNodesActiveListCompleteIter = find(translatorVariables->entityNodesActiveListComplete->begin(), translatorVariables->entityNodesActiveListComplete->end(), currentSpecificConcept);	//reset iterator - added 3a1a
													entityNodesActiveListCompleteIter2 = find(translatorVariables->entityNodesActiveListComplete->begin(), translatorVariables->entityNodesActiveListComplete->end(), entityNode);   //reset iterator - added 3a1a
													#endif
												}
											}
											else
											{
											#endif
												//eg1 Blue pies have bikes. The blue pie is happy.
												//eg2 The yellow banana is a fruit. The yellow fruit is tasty.

												//found instance in network matching concept...
													
												translatorVariables->sentenceIndex = entityNode->sentenceIndexTemp;	//sentenceIndex here is artifical, could equally use currentSpecificConcept->sentenceIndexTemp
												GIAtranslatorOperations.addDefinitionRelationshipToEntity2(entityNode, currentSpecificConcept, sameReferenceSet, translatorVariables);
												#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
												entityNodesActiveListCompleteIter = find(translatorVariables->entityNodesActiveListComplete->begin(), translatorVariables->entityNodesActiveListComplete->end(), currentSpecificConcept);	//reset iterator - added 3a1a
												entityNodesActiveListCompleteIter2 = find(translatorVariables->entityNodesActiveListComplete->begin(), translatorVariables->entityNodesActiveListComplete->end(), entityNode);   //reset iterator - added 3a1a
												#endif
												
											#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS_ADVANCED
											}
											#endif
										#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
										}
										#endif
									}
								}
								else
								{
								}

								//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
								int irrelevant;
								string printEntityNodeString = "";
								bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
								GIAquery.traceEntityNode(currentSpecificConcept, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
								GIAquery.traceEntityNode(entityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
							}
						}
					}

					referenceSetID	= referenceSetID + 1;
				}
			}
		}
	}

}
#endif

bool GIAtranslatorDefineReferencingClass::identifyReferenceSetDetermineNextCourseOfAction(GIAentityNode* entityNode, const bool sameReferenceSet, int referenceSetID, int minimumEntityIndexOfReferenceSet, const GIAentityConnection* connection)
{
	bool result = false;
	if(sameReferenceSet)
	{
		bool referenceSetAlreadyAssigned = false;
		if(entityNode->referenceSetID != GIA_REFERENCE_SET_ID_UNDEFINED)		//do not retrace reference sets
		{
			referenceSetAlreadyAssigned = true;
		}


		bool isProperty = false;
		if(connection != NULL)
		{
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
			if((connection->entityOrigin->entityType == GIA_ENTITY_TYPE_PROPERTY) && (connection->connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT))
			{
				isProperty = true;
			}
			#else
			if(connection->connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY)
			{
				isProperty = true;
			}		
			#endif
		}
		
		bool pass = true;
		#ifdef GIA_ADVANCED_REFERENCING_ASSERT_MINIMUM_SENTENCE_INDEX_OF_REFERENCE_SET
		/*
		NB1 entityNode->minimumEntityIndexOfReferenceSet is only assigned when referenceSetAlreadyAssigned (referenceSetID != GIA_REFERENCE_SET_ID_UNDEFINED)
		NB2 (!referenceSetAlreadyAssigned || (minimumEntityIndexOfReferenceSet < entityNode->minimumEntityIndexOfReferenceSet)): only replace reference set id when definining an earlier (ie larger) reference set in the sentence
		NB3 (entityNode->entityIndexTemp >= minimumEntityIndexOfReferenceSet) || wasPropertyLink: all entities in reference set must occur after minimumEntityIndexOfReferenceSet in the sentence, unless it is a property eg "the blue cars"
		word order must be used carefully,
		eg1; "placed in the book of the house". For reference set to be assigned: (the node must not already be assigned a reference set OR the minimumEntityIndexOfReferenceSet is less than that used during the node's previous reference set assignment) AND the node must occur after minimumEntityIndexOfReferenceSet
		     imagine if house was parsed first by GIA which it is not;
		     eg reference set 1 identification: the house (!referenceSetAlreadyAssigned  && entityNode->entityIndexTemp >= minimumEntityIndexOfReferenceSet)
		     eg reference set 1 reidentification: the book of the house (referenceSetAlreadyAssigned for "house", but; minimumEntityIndexOfReferenceSet < entityNode->minimumEntityIndexOfReferenceSet && entityNode->entityIndexTemp >= minimumEntityIndexOfReferenceSet) {overwrites smaller reference set 'the house'}
		*/
		if(!(!referenceSetAlreadyAssigned || (minimumEntityIndexOfReferenceSet < entityNode->minimumEntityIndexOfReferenceSet)))
		{
			pass = false;
		}
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		if(!entityTypesIsPropertyOrDefinitionRelationshipArray[entityNode->entityType])
		{
		#endif
			if(!((entityNode->entityIndexTemp >= minimumEntityIndexOfReferenceSet) || isProperty))
			{
				pass = false;
			}
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		}
		#endif
		//added 2f19e 24-July-2014
		if(entityNode->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX)
		{
			pass = false;
		}
		#else
		if(referenceSetAlreadyAssigned)
		{
			pass = false;
		}
		#endif
		#ifdef GIA_ADVANCED_REFERENCING_ENSURE_QUANTITY_MATCHES
		#ifdef GIA_NUMBER_OF
		if(entityNode->isNumberOf)
		{
			pass = false;	//added 2m2b - do not create reference set for number of references (prevents gia advanced referencing of number of expressions, and enables gia advanced referencing of subexpressions eg "mountain" in "the number of blue birds near the mountain")
		}
		#endif		
		#endif
		if(pass)
		{
			result =  true;
			this->identifyReferenceSet(entityNode, referenceSetID, minimumEntityIndexOfReferenceSet);
		}
	}

	return result;
}

void GIAtranslatorDefineReferencingClass::identifyReferenceSet(GIAentityNode* entityNode, int referenceSetID, int minimumEntityIndexOfReferenceSet)
{

	entityNode->referenceSetID = referenceSetID;
	entityNode->minimumEntityIndexOfReferenceSet = minimumEntityIndexOfReferenceSet;

	#ifdef GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ACCEPT_PROPERNOUNS_ISOLATE
	if(!(entityNode->grammaticalProperNounTemp))
	{
	#endif
		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter < entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
			{
				this->identifyReferenceSetDetermineNextCourseOfAction((*connectionIter)->entity, ((*connectionIter)->sameReferenceSet), referenceSetID, minimumEntityIndexOfReferenceSet, (*connectionIter));
			}
		}
	#ifdef GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ACCEPT_PROPERNOUNS_ISOLATE
	}
	#endif
}


