/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: GIAcorpusTranslator.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2g8a 13-September-2014
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/





#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "GIAcorpusTranslator.h"
#include "GIAcorpusOperations.h"
#include "GIAtranslatorDefineGrammar.h"
#include "GIAtranslatorRedistributeRelexRelations.h"
#include "GIAtranslatorDefineReferencing.h"
#include "GIAtranslatorDefineSubstances.h"
#include "GIAtranslatorApplyAdvancedFeatures.h"
#ifdef GIA2_CONNECTIONIST_NETWORK
#include "GIAnlp.h"
#include "GIAcorpusDatabase.h"
#endif

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_USE_CORPUS_DATABASE

//based on convertSentenceSyntacticRelationsIntoGIAnetworkNodes():
#ifdef GIA_USE_ADVANCED_REFERENCING
void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, bool linkPreestablishedReferencesGIA,  GIACoreference * firstGIACoreferenceInList)
#else
void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser)
#endif
{
	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "convertSentenceSemanticRelationsIntoGIAnetworkNodes" << endl;
	/*
	cout << "dependency relations: " << endl;
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		string relationType = currentRelationInList->relationType;
		cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << "-" << currentRelationInList->relationGovernorIndex << ", " << currentRelationInList->relationDependent << "-" << currentRelationInList->relationDependentIndex << ")" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	*/
	#endif
	
	bool GIAentityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode * GIAconceptNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode * GIAentityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAconceptNodeArray[w] = NULL;
		GIAentityNodeArray[w] = NULL;
	}
	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "locateAndAddAllConceptEntitiesBasedOnSemanticRelations" << endl;
	#endif

	locateAndAddAllConceptEntitiesBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAconceptNodeArray, entityNodesActiveListConcepts, sentenceConceptEntityNodesList, NLPfeatureParser);
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArray[w] = GIAconceptNodeArray[w];		//set default values of GIAentityNodeArray
	}

	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "generateTempFeatureArray" << endl;
	#endif

	Feature * featureArrayTemp[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	generateTempFeatureArray(currentSentenceInList->firstFeatureInList, featureArrayTemp);	//regeneration required for Relex in case query variables detected

	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "fillGrammaticalArraysRelex" << endl;
	#endif
	
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1a; fillGrammaticalArrays" << endl;
	#endif
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX) 				//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		fillGrammaticalArraysRelex(currentSentenceInList);
	}
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if((NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER) || (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP))	//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//fillGrammaticalArraysStanford(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser, featureArrayTemp);	//not possible as Stanford syntactical dependency relations are assumed not available

		//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
		extractGrammaticalInformationStanford(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);
	}
	#endif

	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "applyGrammaticalInfoToAllEntities" << endl;
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1b; applyGrammaticalInfoToAllEntities" << endl;
	#endif
 	applyGrammaticalInfoToAllEntities(GIAentityNodeArrayFilled, GIAentityNodeArray, currentSentenceInList->firstFeatureInList);

	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllContinuousVerbs" << endl;
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout <<"redistribute Stanford Relations - correct POS tags And Lemmas Of All Continuous Verbs" << endl;
	#endif
	redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllContinuousVerbs(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);

#ifndef GIA_ADVANCED_REFERENCING_DEBUG_DISABLE_LINKING
	#ifdef GIA_USE_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (5linkAdvancedReferencesGIA)\n" << endl;
		#endif
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 3ii; link advanced references GIA (eg the red car is fast. Mike drove the red car.)" << endl;
		#endif
		linkAdvancedReferencesGIA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, firstGIACoreferenceInList, featureArrayTemp, GIAentityNodeArray, GIAconceptNodeArray);	//NB second last parameter used to be GIAfeatureTempEntityNodeArray
	}
	else
	{
	#endif
		//is pronoun referencing supported by GIA2?

		#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
		#endif
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 3i; link pronominal references Relex (eg his/her with joe/emily)" << endl;
			#endif
			linkPronounReferencesRelex(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, GIAentityNodeArray, entityNodesActiveListConcepts, featureArrayTemp);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_USE_STANFORD_CORENLP
		else if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 3i; link pronominal references Stanford CoreNLP (eg his/her with joe/emily)" << endl;
			#endif
			linkPronounAndTextualContextReferencesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, GIAentityNodeArray, entityNodesActiveListConcepts, firstSentenceInList->firstCoreferenceInList, featureArrayTemp);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		}
		#endif
		#endif
		#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	}
	#endif
#endif

	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "defineSubstancesBasedOnSemanticRelations" << endl;
	#endif

	defineSubstancesBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA2_SUPPORT_QUERIES
	identifyComparisonVariableBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	
	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "defineConnectionsBasedOnSemanticRelations" << endl;
	#endif
	
	defineConnectionsBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "applyAdvancedFeaturesBasedOnSemanticRelations" << endl;
	#endif
	
	applyAdvancedFeaturesBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			//record sentenceIndex for concept entity nodes also (NB can GIAconceptNodeArray here as it will include concept entity nodes for prepositions)
			GIAconceptNodeArray[w]->entityIndexTemp = w;
			//GIAentityNodeArray[w]->entityIndexTemp = w;	//why isnt this used?
			GIAentityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
			if(GIAconceptNodeArray[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
			{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
				//cout << "assigning: " <<  currentSentenceInList->sentenceIndex << endl;
				GIAconceptNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
			}
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			cout << GIAconceptNodeArray[w]->entityName << ", w = " << w << endl;
			cout << "GIAconceptNodeArray[w]->disabled = " << GIAconceptNodeArray[w]->disabled << endl;
			#endif
		}
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record sentence nodes as permanent if they are still enabled" << endl;
	#endif
	//recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAconceptNodeArray);		//this method is not sufficient, as some concept entity nodes (eg prepositions/conditions) are not contained within GIAconceptNodeArray
	recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(entityNodesActiveListConcepts);
	#ifdef GIA_TRANSLATOR_DEBUG
	/*
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter2;
	for(conceptEntityNodesListIter2 = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter2 != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter2++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListIter2->second;
		cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
	}
	*/
	#endif

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_SENTENCE
	//restore critical variables: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter;
	for(conceptEntityNodesListIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListIter->second;
		entityNode->entityAlreadyDeclaredInThisContext = false;
	}
	#endif
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	//required to reset wasReferenceTemp for next time
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNodeArray[w]->wasReferenceTemp = false;
		}
	}
	#endif

	#ifdef GIA_CORPUS_TRANSLATOR_DEBUG
	cout << "end convertSentenceSemanticRelationsIntoGIAnetworkNodes" << endl;
	#endif
}


void locateAndAddAllConceptEntitiesBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAconceptNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser)
{
	//code from locateAndAddAllFeatureTempEntities():

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		bool isDependencyRelationSecondary = false;
		for(int i=0; i<GIA2_SEMANTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationSecondaryNameArray[i])
			{
				isDependencyRelationSecondary = true;
			}
		}

		string name[2];
		name[0] = currentRelationInList->relationGovernor;
		name[1] =  currentRelationInList->relationDependent; 	//argumentName

		int relationIndex[2];
		relationIndex[0] = currentRelationInList->relationGovernorIndex;
		relationIndex[1] = currentRelationInList->relationDependentIndex;

		#ifdef GIA_TRANSLATOR_DEBUG
		//cout << "relationIndex[0]  = " << relationIndex[0] << endl;
		//cout << "relationIndex[1]  = " << relationIndex[1] << endl;
		//cout << "name[0]  = " << name[0] << endl;
		//cout << "name[1]  = " << name[1] << endl;
		#endif

		for(int i=0; i<2; i++)
		{
			if(!GIAentityNodeArrayFilled[relationIndex[i]])
			{
				GIAentityNodeArrayFilled[relationIndex[i]] = true;

				//code from locateAndAddAllConceptEntities():

				bool entityAlreadyExistant = false;
				GIAentityNode * conceptEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(&(name[i]), &entityAlreadyExistant, entityNodesActiveListConcepts);
				GIAconceptNodeArray[relationIndex[i]] = conceptEntity;

				//cout << "creating concept = " << conceptEntity->entityName << endl;

				if(isDependencyRelationSecondary)
				{
					if(i == 1)
					{
						conceptEntity->disabled = true;	//disable is/have etc nodes
					}
				}

				conceptEntity->hasAssociatedInstanceTemp = false;

				sentenceConceptEntityNodesList->push_back(conceptEntity);

				/*
				//temporarily disabled this code;
				//TODO: detect isQuery and negative from GIA semanticRelation argument (yet to implement)

				#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
				if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)	//ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					if(currentRelationInList->isQuery)		//TODO: detect isQuery from GIA semanticRelation argument (yet to implement)
					{
						conceptEntity->isQuery = true;
						setFoundComparisonVariable(true);
						setComparisonVariableNode(conceptEntity);
					}
				}
				#endif

				#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				//this is required because negative assignment is now performed during redistribution
				if(currentRelationInList->negative)	//TODO: detect negative from GIA semanticRelation argument (yet to implement)
				{
					conceptEntity->negative = true;
				}
				*/

				/*
				//cout << "filling: " << relationIndex[i] << " " << name[i] << endl;
				#ifdef GIA_USE_ADVANCED_REFERENCING
				//this is required for fillGrammaticalArraysStanford findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet()	[nb these values are applied to concept entities only]
				GIAconceptNodeArray[relationIndex[i]]->entityIndexTemp = relationIndex[i];
				GIAconceptNodeArray[relationIndex[i]]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
				#endif
				*/
			}
		}

		currentRelationInList = currentRelationInList->next;
	}
}




void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[])
{
	Relation * currentRelationInList;

	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_COMPOSITION_AUXILIARY])
			{
				//auxiliary found; eg auxiliary(it-6, had-5)	The dog must have it.
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int auxiliaryIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode * entity = GIAentityNodeArray[thingIndex];
				string auxiliaryString = GIAentityNodeArray[auxiliaryIndex]->wordOrig;	//featureArrayTemp[auxiliaryIndex]->word;

				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					if(!(currentRelationInList2->disabled))
					{
						if(currentRelationInList2->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA])
						{
							int auxiliaryIndex2 = currentRelationInList->relationGovernorIndex;
							int modalAuxiliaryIndex2 = currentRelationInList->relationDependentIndex;
							if(auxiliaryIndex2 == auxiliaryIndex)
							{
								//modal auxiliary found; eg modalAuxiliary(had-5, must-3) / modalAuxiliary(had-5, have-4) The dog must have had it.
								string modalAuxiliaryString = GIAentityNodeArray[modalAuxiliaryIndex2]->wordOrig;	//featureArrayTemp[modalAuxiliaryIndex2]->word;

								updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entity, modalAuxiliaryString);
								currentRelationInList2->disabled = true;
							}
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				updateGrammaticalValuesBasedOnCompositionAuxiliary(entity, auxiliaryString);
				currentRelationInList->disabled = true;
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
	#endif

	currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA])
			{
				//modal auxiliary [/copula] found; eg modalAuxiliary(sad-3, was-2) / eg modalAuxiliary(had-5, must-3) / modalAuxiliary(had-5, have-4) The dog must have had it.
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int modalAuxiliaryIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode * entity = GIAentityNodeArray[thingIndex];
				string modalAuxiliaryString = GIAentityNodeArray[modalAuxiliaryIndex]->wordOrig;	//featureArrayTemp[modalAuxiliaryIndex]->word;

				updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entity, modalAuxiliaryString);
				currentRelationInList->disabled = true;
			}
		}
		currentRelationInList = currentRelationInList->next;
	}


}

#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
void updateGrammaticalValuesBasedOnCompositionAuxiliary(GIAentityNode * entity, string compositionAuxiliaryString)
{
	for(int i=0; i<RELATION_GOVERNOR_COMPOSITION_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(compositionAuxiliaryString == relationGovernorCompositionPastTenseNameArray[i])
		{
			entity->grammaticalTenseTemp = GRAMMATICAL_TENSE_PAST;
		}
	}
	for(int i=0; i<RELATION_GOVERNOR_COMPOSITION_CONTINUOUS_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(compositionAuxiliaryString == relationGovernorCompositionContinuousTenseNameArray[i])
		{
			entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] = true;
		}
	}
}
#endif

void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode * entity, string modalAuxiliaryString)
{
	for(int i=0; i<RELATION_AUXILIARY_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(modalAuxiliaryString == relationAuxiliaryPastTenseNameArray[i])
		{
			entity->grammaticalTenseTemp = GRAMMATICAL_TENSE_PAST;
		}
	}
	for(int i=0; i<RELATION_AUXILIARY_FUTURE_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(modalAuxiliaryString == relationAuxiliaryFutureTenseNameArray[i])
		{
			entity->grammaticalTenseTemp = GRAMMATICAL_TENSE_FUTURE;
		}
	}
}

void defineSubstancesBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	#ifdef GIA_CREATE_NON_SPECIFIC_SUBSTANCE_CONCEPTS_FOR_ALL_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0a2 pass; define substances all nodes" << endl;
	#endif
	defineSubstancesAllNodes(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#else
	cout << "error: USE_GIA2 currently requires GIA_CREATE_NON_SPECIFIC_SUBSTANCE_CONCEPTS_FOR_ALL_CONCEPTS (as it simplifies coding)";
	#endif

	#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "define substances action concepts, eg 'swim' in 'To swim to the beach requires strength.'" << endl;
	#endif
	defineSubstancesActionConcepts(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NULL);
	#endif

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			GIAentityNode * entity = GIAentityNodeArray[i];
			bool isConcept = false;
			if(entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_NOUN)
			{
				bool hasDeterminer = false;
				bool indefiniteDeterminer = false;
				string determinerString = "";
				Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 				while(currentRelationInList->next != NULL)
				{
					if(!(currentRelationInList->disabled))
					{
						if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER])
						{
							int thingIndex = currentRelationInList->relationGovernorIndex;
							int determinerIndex = currentRelationInList->relationDependentIndex;
							if(thingIndex == i)
							{
								hasDeterminer = true;
								determinerString = GIAentityNodeArray[determinerIndex]->entityName;
								currentRelationInList->disabled = true;
								for(int i=0; i<GRAMMATICAL_DETERMINER_LIMITED_INDEFINITE_NUMBER_OF_TYPES; i++)
								{
									if(determinerString == grammaticalDeterminerIndefiniteArray[i])
									{
										indefiniteDeterminer = true;
									}
								}
							}
						}
					}
					currentRelationInList = currentRelationInList->next;
				}

				//case 1. see if is plural and has no determiner
				//eg Bats are animals.  (definition connection, bat = concept, animal = concept)	/ bats ride bikes
				if(!hasDeterminer)
				{
					if(entity->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
					{
						isConcept = true;
					}
					/*
					//original GIA implementation uses isProperNoun detection instead of isPlural detection; see defineSubstancesBasedOnDeterminatesOfDefinitionEntities()
					if(!(entity->grammaticalIsProperNoun))
					{
						isConcept = true;
					}
					*/
				}

				//check if special case as defined in defineSubstancesBasedOnDeterminatesOfDefinitionEntities();
				//case 2. see if has an indefinite determiner (a/an) and is part of a definition connection
				//eg A book is an animal (definition connection, bat = concept, animal = concept)
				//eg The bat is an animal (definition connection, bat = substance, animal = concept)
				if(hasDeterminer && indefiniteDeterminer)
				{
					Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 					while(currentRelationInList->next != NULL)
					{
						if(!(currentRelationInList->disabled))
						{
							if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS])
							{
								int thingIndex = currentRelationInList->relationGovernorIndex;
								int determinerIndex = currentRelationInList->relationDependentIndex;
								if((thingIndex == i) || (determinerIndex == i))
								{
									isConcept = true;
								}
							}
						}
						currentRelationInList = currentRelationInList->next;
					}
				}
			}


			/* moved to top (defineSubstancesAllNodes)
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "0a2 pass; define substances all nodes" << endl;
			#endif
			GIAentityNodeArray[i] = addSubstanceToSubstanceDefinition(param->GIAentityNodeArray[functionEntityIndex1]);
			*/

			if(isConcept)
			{
				//cout << "isConcept" << endl;
				GIAentityNodeArray[i]->isSubstanceConcept = true;
			}

			#ifndef GIA_CREATE_NON_SPECIFIC_SUBSTANCE_CONCEPTS_FOR_ALL_CONCEPTS
			//eg could create substances here if !isConcept
			#endif
		}
	}
}

#ifdef GIA2_SUPPORT_QUERIES
void identifyComparisonVariableBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	if(currentSentenceInList->isQuestion)
	{
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAentityNodeArrayFilled[i])
			{
				GIAentityNode * entityNode = GIAentityNodeArray[i];
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

	if(currentSentenceInList->isQuestion)
	{
		//cout << "isQuestion" << endl;
		Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{
			GIAentityNode * entityNodes[2];
			string corpusSpecialRelationIsQuery[2];
			entityNodes[0] = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
			entityNodes[1] = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
			corpusSpecialRelationIsQuery[0] = currentRelationInList->corpusSpecialRelationGovernorIsQuery;
			corpusSpecialRelationIsQuery[1] = currentRelationInList->corpusSpecialRelationDependentIsQuery;

			for(int i=0; i<2; i++)
			{
				GIAentityNode * entityNode = entityNodes[i];
				//cout << "entityNode = " << entityNode->entityName << endl;
				if(corpusSpecialRelationIsQuery[i] == GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME)
				{
					cout << GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					setComparisonVariableNode(entityNode);
					setFoundComparisonVariable(true);
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "foundComparisonVariable" << endl;
					#endif
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME)
				{
					cout << GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isNameQuery = true;
					setComparisonVariableNode(entityNode);
					setFoundComparisonVariable(true);
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "foundComparisonVariable" << endl;
					#endif
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME)
				{
					cout << GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isWhichOrEquivalentWhatQuery = true;
					setComparisonVariableNode(entityNode);
					setFoundComparisonVariable(true);
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "foundComparisonVariable" << endl;
					#endif
				}
			}
			currentRelationInList = currentRelationInList->next;
		}
	}
}
#endif


void defineConnectionsBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			int entity1Index = currentRelationInList->relationGovernorIndex;
			int entity2Index = currentRelationInList->relationDependentIndex;
			GIAentityNode * entity1 = GIAentityNodeArray[entity1Index];
			GIAentityNode * entity2 = GIAentityNodeArray[entity2Index];
			bool sameReferenceSet = currentRelationInList->sameReferenceSet;

			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES])
			{
				GIAentityNodeArray[entity2Index] = addOrConnectPropertyToEntity(entity1, entity2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT])
			{
				bool foundMatchingObject = false;
				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					int entity2Index2 = currentRelationInList2->relationDependentIndex;
					bool sameReferenceSet2 = currentRelationInList2->sameReferenceSet;
					if(currentRelationInList2->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT])
					{
						if(entity2Index == entity2Index2)
						{//found matching conditionType pair
							int entity3Index = currentRelationInList2->relationDependentIndex;
							GIAentityNode * entity3 = GIAentityNodeArray[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode * entity2relation2 = GIAentityNodeArray[entity2IndexRelation2];

							GIAentityNodeArray[entity3Index] = addOrConnectActionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, sameReferenceSet2);
							foundMatchingObject = true;
							currentRelationInList2->disabled = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				if(!foundMatchingObject)
				{
					GIAentityNodeArray[entity2Index] = addOrConnectActionToSubject(entity1, entity2, sameReferenceSet);
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT])
			{
				GIAentityNodeArray[entity2Index] = addOrConnectActionToObject(entity1, entity2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT])
			{
				bool foundMatchingObject = false;
				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					int entity2Index2 = currentRelationInList2->relationDependentIndex;
					if(currentRelationInList2->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT])
					{
						if(entity2Index == entity2Index2)
						{//found matching conditionType pair
							int entity3Index = currentRelationInList2->relationDependentIndex;
							GIAentityNode * entity3 = GIAentityNodeArray[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode * entity2relation2 = GIAentityNodeArray[entity2IndexRelation2];

							GIAentityNodeArray[entity3Index] = addOrConnectConditionToEntity(entity1, entity2relation2, entity3, sameReferenceSet);
							currentRelationInList2->disabled = true;
							foundMatchingObject = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}
				if(!foundMatchingObject)
				{
					GIAentityNodeArray[entity2Index] = addOrConnectConditionToSubject(entity1, entity2, sameReferenceSet);
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT])
			{//THIS SHOULDNT BE required as there is never an isolated condition-object connection declared according to current GIA specification [but due to bug, it is requierd for Relex with measure dependency cases eg 'He runs every hour.'];
				GIAentityNodeArray[entity2Index] = addOrConnectConditionToObject(entity1, entity2, sameReferenceSet);
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS])
			{
				addDefinitionToEntity(entity1, entity2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_ENTITY_NODES_ADD_ALIAS])
			{
				mergeEntityNodesAddAlias(entity1, entity2);
				GIAentityNodeArray[entity2Index] = GIAentityNodeArray[entity1Index];
				currentRelationInList->disabled = true;
			}
			#endif
			else
			{

			}
		}

		currentRelationInList = currentRelationInList->next;
	}
}

void applyAdvancedFeaturesBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser)
{
	defineQuantitiesBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	defineQualitiesBasedOnSemanticRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	//measure, dates, and quantities??
	defineTenseOnlyTimeConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
}

void defineQuantitiesBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY])
			{
				int entity1Index = currentRelationInList->relationGovernorIndex;
				int entity2Index = currentRelationInList->relationDependentIndex;
				GIAentityNode * entity1 = GIAentityNodeArray[entity1Index];
				GIAentityNode * entity2 = GIAentityNodeArray[entity2Index];
				entity1->hasQuantity = true;

				GIAentityNode * quantitySubstance = entity1;

				if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					//code copied from extractQuantitiesStanfordCoreNLP():
					//if(quantitySubstance->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).
					//{

					quantitySubstance->hasQuantity = true;
					if((quantitySubstance->NormalizedNERtemp != "") && (quantitySubstance->NormalizedNERtemp != "0.0"))		//added 0.0 for a stanford anomaly 11 May 2012
					{
						quantitySubstance->quantityNumberString = quantitySubstance->NormalizedNERtemp;
					}
					else
					{
						quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;
					}

					int quantityNumberInt = calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;
					quantitySubstance->isSubstanceConcept = false;	//added 2a11a [because defineSubstanceConcepts() does not have access to quantity data]

					/*//comparison variables not yet implemented for GIA2
					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						setComparisonVariableNode(quantitySubstance);
					}
					*/
					//}
				}
				else if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
				{
					quantitySubstance->hasQuantity = true;
					quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

					int quantityNumberInt = calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;

					/*//comparison variables not yet implemented for GIA2
					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						setComparisonVariableNode(quantitySubstance);
					}
					*/
				}

				//added 2c4c
				if(entity2->isQuery)
				{
					quantitySubstance->isQuery = true;
					if(entity2->isNameQuery)
					{
						quantitySubstance->isNameQuery = true;
					}
					if(entity2->isWhichOrEquivalentWhatQuery)
					{
						quantitySubstance->isWhichOrEquivalentWhatQuery = true;
					}
					setComparisonVariableNode(quantitySubstance);
					setFoundComparisonVariable(true);
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
}

void defineQualitiesBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser)
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode * entity = GIAentityNodeArray[w];
			if(!(entity->disabled))
			{
				if((entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ) || (entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADV))
				{
					entity->isSubstanceQuality = true;
				}
			}
		}
	}
}

#ifdef GIA2_CONNECTIONIST_NETWORK
bool generateAllPermutationsFromSemanticRelationsFile(string corpusFileName, int NLPfeatureParser)
{
	//code based on loadCorpusFileSemanticDependencyRelations():
	bool result = true;
	Sentence * sentence = new Sentence();
	bool createNewSentences = true;
	bool parseGIA2file = true;
	bool isQuery = false;	//irrelevant
	#ifdef GIA2_CONNECTIONIST_NETWORK_DEBUG
	cout << "generateAllPermutationsFromSemanticRelationsFile():" << endl;
	#endif
	if(!parseStanfordParserFile(corpusFileName, isQuery, sentence, createNewSentences, parseGIA2file, false))		//CHECK THIS; need to account for corpus.txt having multiple entries [eg different text but identical layout]
	{
		result = false;
		cout << "generateAllPermutationsFromSemanticRelationsFile() error: !parseStanfordParserFile" << endl;
	}
	else
	{
		Feature * dummyBlankFeature = new Feature();
		#ifdef GIA2_CONNECTIONIST_NETWORK_DEBUG
		cout << "parseStanfordParserFile() passed." << endl;
		#endif
		//now simulate GIA2 semantic relations for each subset of original sentence POS permutation
		Feature * centralFeatureInSentence = sentence->firstFeatureInList;
		for(int centralWord=1; centralWord<=sentence->maxNumberOfWordsInSentence; centralWord++)	//centralWord in subset
		{
			#ifdef GIA2_CONNECTIONIST_NETWORK_DEBUG
			cout << "centralWord = " << centralWord << ", " << centralFeatureInSentence->lemma << endl;
			#endif
			Feature * recordOfFeatureAfterCentralFeatureInSentence = centralFeatureInSentence->next;
			centralFeatureInSentence->next = dummyBlankFeature;	//temporarily disconnect node at end of sentence subset

			Feature * firstFeatureInSentenceSubset = sentence->firstFeatureInList;
			for(int firstWord=1; firstWord<centralWord; firstWord++)	//firstWord in subset
			{
				#ifdef GIA2_CONNECTIONIST_NETWORK_DEBUG
				cout << "firstWord = " << firstWord << ", " << firstFeatureInSentenceSubset->lemma << endl;
				#endif
				int subsetSize = centralWord-firstWord+1;	//subsetSize aka maxSpread

				//code from convertSentenceSyntacticRelationsIntoGIAnetworkNodes():

				string sentenceText = regenerateSentenceText(firstFeatureInSentenceSubset, true, NLPfeatureParser);
				sentenceText = sentenceText + STRING_NEW_LINE;	//required to add new line at end of parsingWordsAndTags as per Stanford Parser specification (see parseStanfordParserFile)
				sentenceText = sentenceText + STRING_NEW_LINE;

				bool foundAtLeastOneRelation = false;
				Relation * currentRelationInList = sentence->firstRelationInList;
				while(currentRelationInList->next != NULL)
				{
					if((currentRelationInList->relationGovernorIndex >= firstWord) && ((currentRelationInList->relationGovernorIndex <= centralWord) || (currentRelationInList->relationGovernorIndex > FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY))
					&& (currentRelationInList->relationDependentIndex >= firstWord) && ((currentRelationInList->relationDependentIndex <= centralWord) || (currentRelationInList->relationDependentIndex > FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)))
					{
						//regenerate semantic relation based on parsed Relation object
						string GIA2semanticDependencyRelation = generateGIA2semanticDependencyRelationSimple(currentRelationInList->relationGovernor, currentRelationInList->relationDependent, currentRelationInList->relationType, currentRelationInList->relationGovernorIndex, currentRelationInList->relationDependentIndex, currentRelationInList->sameReferenceSet);
						GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + STRING_NEW_LINE;
						sentenceText = sentenceText + GIA2semanticDependencyRelation;
						foundAtLeastOneRelation = true;
					}
					currentRelationInList = currentRelationInList->next;
				}
				sentenceText = sentenceText + STRING_NEW_LINE;	//required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)

				if(foundAtLeastOneRelation)
				{
					string corpusSubsetFileName = createNewCorpusFileAndOpenItForWriting(firstFeatureInSentenceSubset);
					saveTextToCurrentCorpusFile(sentenceText);
					closeCorpusFile();
				}

				firstFeatureInSentenceSubset = firstFeatureInSentenceSubset->next;
			}
			centralFeatureInSentence->next = recordOfFeatureAfterCentralFeatureInSentence;	//restore temporarily disconnected node at end of sentence subset
			centralFeatureInSentence = centralFeatureInSentence->next;
		}
	}

	return result;
}
#endif



#endif







