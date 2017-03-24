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
 * File Name: GIAsemanticParserTranslator.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1f 26-February-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#include "GIAsemanticParserTranslator.hpp"
#ifdef GIA2_SEMANTIC_PARSER
#endif
#ifdef GIA_BOT
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
#endif

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

//based on convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:
void GIAsemanticParserTranslatorClass::convertSentenceSemanticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA,  GIAcoreference* firstGIAcoreferenceInList);
{
	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "convertSentenceSemanticRelationsIntoGIAnetworkNodes" << endl;
	/*
	cout << "dependency relations: " << endl;
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		string relationType = currentRelationInList->relationType;
		cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << "-" << currentRelationInList->relationGovernorIndex << ", " << currentRelationInList->relationDependent << "-" << currentRelationInList->relationDependentIndex << ")" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	*/
	#endif

	/*
	cout << "TEST" << endl;
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << currentRelationInList->relationGovernor << endl;
			cout << "relationDependent = " << currentRelationInList->relationDependent << endl;
			cout << "relationGovernerIndex = " << currentRelationInList->relationGovernorIndex << endl;
			cout << "relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
		}
		currentRelationInList = currentRelationInList->next;
	}
	*/

	bool GIAentityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode* GIAnetworkIndexNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode* GIAentityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAfeature* featureArrayTemp[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	translatorVariables->GIAentityNodeArrayFilled = &GIAentityNodeArrayFilled;
	translatorVariables->GIAnetworkIndexNodeArray = GIAnetworkIndexNodeArray;	
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;
	translatorVariables->featureArrayTemp = featureArrayTemp;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAnetworkIndexNodeArray[w] = NULL;
		GIAentityNodeArray[w] = NULL;
	}

	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	GIAbot.botSwitchFirstAndSecondPersonSemantic(currentSentenceInList);
	#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations" << endl;
	#endif

	this->locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(translatorVariables);
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArray[w] = GIAnetworkIndexNodeArray[w];		//set default values of GIAentityNodeArray
	}

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "generateTempFeatureArray" << endl;
	#endif

	GIAtranslatorOperations.generateTempFeatureArray(translatorVariables->currentSentenceInList->firstFeatureInList, featureArrayTemp);	//regeneration required for Relex in case query variables detected

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "fillGrammaticalArraysRelex" << endl;
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1a; fillGrammaticalArrays" << endl;
	#endif
	#ifdef GIA_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX) 				//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		GIAtranslatorDefineGrammar.fillGrammaticalArraysRelex(translatorVariables->currentSentenceInList);
	}
	#endif
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	if((NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER) || (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP))	//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//fillGrammaticalArraysStanford(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser, featureArrayTemp);	//not possible as Stanford syntactical dependency relations are assumed not available

		//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
		GIAtranslatorDefineGrammar.extractGrammaticalInformationStanford(translatorVariables->currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		this->fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(translatorVariables);
	}
	#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "pass 1b; applyGrammaticalInfoToAllEntities" << endl;
	#endif
 	GIAtranslatorDefineGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, currentSentenceInList->firstFeatureInList);

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs" << endl;
	#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout <<"redistribute Stanford Relations - correct POS tags And Lemmas Of All Continuous Verbs" << endl;
	#endif
	GIAtranslatorRedistributeRelations.redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(translatorVariables);

	translatorVariables->GIAfeatureTempEntityNodeArray = GIAentityNodeArray;	//required for linkAdvancedReferencesGIA/linkPronounReferencesRelex/linkPronounAndTextualContextReferencesStanfordCoreNLP
#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "\n\t\t\t GIA_ADVANCED_REFERENCING_DEBUG (5linkAdvancedReferencesGIA)\n" << endl;
		#endif
		#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
		cout << "pass 3ii; link advanced references GIA (eg the red car is fast. Mike drove the red car.)" << endl;
		#endif
		GIAtranslatorDefineReferencing.linkAdvancedReferencesGIA(translatorVariables, firstGIAcoreferenceInList);	//NB second last parameter used to be GIAfeatureTempEntityNodeArray
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
			#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
			cout << "pass 3i; link pronominal references Relex (eg his/her with joe/emily)" << endl;
			#endif
			GIAtranslatorDefineReferencing.linkPronounReferencesRelex(translatorVariables);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_STANFORD_CORENLP
		else if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
			cout << "pass 3i; link pronominal references Stanford CoreNLP (eg his/her with joe/emily)" << endl;
			#endif
			GIAtranslatorDefineReferencing.linkPronounAndTextualContextReferencesStanfordCoreNLP(translatorVariables);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		}
		#endif
		#endif
		#endif
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif
#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "defineSubstancesBasedOnSemanticRelations" << endl;
	#endif

	this->defineSubstancesBasedOnSemanticRelations(translatorVariables);

	#ifdef GIA2_SUPPORT_QUERIES
	this->identifyComparisonVariableBasedOnSemanticRelations(translatorVariables);
	#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "defineConnectionsBasedOnSemanticRelations" << endl;
	#endif

	this->defineConnectionsBasedOnSemanticRelations(translatorVariables);

	#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "linkEntitiesDynamic{}:" << endl;
	#endif
	GIAtranslatorLinkEntitiesDynamic.linkEntitiesDynamic(translatorVariables);
	#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "applyAdvancedFeaturesBasedOnSemanticRelations" << endl;
	#endif

	this->applyAdvancedFeaturesBasedOnSemanticRelations(translatorVariables);

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "set sentenceIndexTemp/entityIndexTemp" << endl;
	#endif

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			//#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION	//networkIndex sentenceIndex information is also required for GIAdraw.cpp
			#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
			if(GIAentityNodeArray[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)	//added condition 2j5b [check this is required]
			{
				GIAentityNodeArray[w]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
			}
			//record sentenceIndex for networkIndex entity nodes also (NB cannot use GIAnetworkIndexNodeArray here as it won't necessarily include networkIndex entity nodes for prepositions [for dynamic linking])
			if(!(GIAentityNodeArray[w]->instanceReverseNodeList->empty()))
			{
				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				GIAentityNode* instanceEntity = GIAentityNodeArray[w];
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->instanceReverseNodeList->begin(); connectionIter != instanceEntity->instanceReverseNodeList->end(); connectionIter++)
				{
					GIAentityNode* networkIndexNode = (*connectionIter)->entity;
				#else
					GIAentityNode* networkIndexNode = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNodeArray[w]);
				#endif

					if(networkIndexNode->entityIndexTemp == GIA_ENTITY_INDEX_UNDEFINED)	//added condition 2j5b [check this is required]
					{
						networkIndexNode->entityIndexTemp = w;
					}
					if(networkIndexNode->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
					{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
						networkIndexNode->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
						#ifdef GIA_DEBUG
						//cout << "networkIndexNode->sentenceIndexTemp = " << networkIndexNode->sentenceIndexTemp << endl;
						#endif
					}

				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				}
				#endif
			}
			#else
			//record sentenceIndex for networkIndex entity nodes also (NB can use GIAnetworkIndexNodeArray here as it will include networkIndex entity nodes for prepositions)
			GIAnetworkIndexNodeArray[w]->entityIndexTemp = w;
			GIAentityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
			if(GIAnetworkIndexNodeArray[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
			{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
				GIAnetworkIndexNodeArray[w]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
			}
			#endif
			//#endif

		}
	}
	#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "record sentence nodes as permanent if they are still enabled" << endl;
	#endif
	//GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray);		//this method is not sufficient, as some networkIndex entity nodes (eg prepositions/conditions) are not contained within GIAnetworkIndexNodeArray
	GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(entityNodesActiveListNetworkIndexes);
	#ifdef GIA_TRANSLATOR_DEBUG
	/*
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIter2 = entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityNodesListIter2 != entityNodesActiveListNetworkIndexes->end(); networkIndexEntityNodesListIter2++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListIter2->second;
		cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
	}
	*/
	#endif

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "prevent double links" << endl;
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


	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			entityNodesActiveListSentence->push_back(GIAentityNodeArray[w]);

			#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
			if(GIAtranslatorOperations.checkIfSentenceIsMathTextParsablePhrase(translatorVariables->currentSentenceInList))
			{
				#ifndef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				if(GIAentityNodeArray[w]->entityType == GIA_ENTITY_TYPE_CONCEPT)
				{
				#endif
					if(!(GIAentityNodeArray[w]->wasReference))	//redundant?
					{
						GIAentityNodeArray[w]->NLCmathTextParsablePhraseEntity = true;
					}
				#ifndef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				}
				#endif
			}
			#endif
		}
	}

	entityNodesActiveListSentences->insert(pair<int, vector<GIAentityNode*>*>(translatorVariables->currentSentenceInList->sentenceIndex, translatorVariables->entityNodesActiveListSentence));

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "end convertSentenceSemanticRelationsIntoGIAnetworkNodes" << endl;
	#endif
}


void GIAsemanticParserTranslatorClass::locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	//code from locateAndAddAllFeatureTempEntities{}:

	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
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

		#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
		cout << "\trelationIndex[0]  = " << relationIndex[0] << endl;
		cout << "relationIndex[1]  = " << relationIndex[1] << endl;
		cout << "name[0]  = " << name[0] << endl;
		cout << "name[1]  = " << name[1] << endl;
		#endif

		for(int i=0; i<2; i++)
		{
			if(!translatorVariables->GIAentityNodeArrayFilled[relationIndex[i]])
			{
				translatorVariables->GIAentityNodeArrayFilled[relationIndex[i]] = true;

				//code from locateAndAddAllNetworkIndexEntities{}:

				bool entityAlreadyExistant = false;
				GIAentityNode* networkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&(name[i]), &entityAlreadyExistant, translatorVariables);
				translatorVariables->GIAnetworkIndexNodeArray[relationIndex[i]] = networkIndexEntity;

				#ifdef GIA_DEBUG
				//cout << "\tcreating networkIndex = " << networkIndexEntity->entityName << endl;
				//cout << "relationIndex[i] = " << relationIndex[i] << endl;
				#endif

				if(isDependencyRelationSecondary)
				{
					if(i == 1)
					{
						networkIndexEntity->disabled = true;	//disable is/have etc nodes
					}
				}

				networkIndexEntity->hasAssociatedInstanceTemp = false;

				translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(networkIndexEntity);

				#ifdef GIA2_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
				if(relationIndex[i] >= FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
				{
					networkIndexEntity->wordOrig = networkIndexEntity->entityName;

					GIAfeature* currentFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
					bool foundFeature = false;
					while(currentFeatureInList->next != NULL)
					{
						if((currentFeatureInList->lemma == name[i]) && (currentFeatureInList->entityIndex == relationIndex[i]))
						{
							foundFeature = true;
						}
						currentFeatureInList = currentFeatureInList->next;
					}
					if(!foundFeature)
					{
						currentFeatureInList->word = name[i];
						currentFeatureInList->lemma = name[i];
						currentFeatureInList->entityIndex = relationIndex[i];
						GIAfeature* newFeature = new GIAfeature();
						currentFeatureInList->next = newFeature;
					}
				}
				#endif

				/*
				//temporarily disabled this code;
				//TODO: detect isQuery and negative from GIA semanticRelation argument (yet to implement)

				#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
				if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)	//ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					if(currentRelationInList->isQuery)		//TODO: detect isQuery from GIA semanticRelation argument (yet to implement)
					{
						networkIndexEntity->isQuery = true;
						setFoundComparisonVariable(true);
						setComparisonVariableNode(networkIndexEntity);
					}
				}
				#endif

				#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				//this is required because negative assignment is now performed during redistribution
				if(currentRelationInList->negative)	//TODO: detect negative from GIA semanticRelation argument (yet to implement)
				{
					networkIndexEntity->negative = true;
				}
				*/

				/*
				#ifdef GIA_DEBUG
				//cout << "filling: " << relationIndex[i] << " " << name[i] << endl;
				#endif
				#ifdef GIA_ADVANCED_REFERENCING
				//this is required for fillGrammaticalArraysStanford findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet()	[nb these values are applied to networkIndex entities only]
				GIAnetworkIndexNodeArray[relationIndex[i]]->entityIndexTemp = relationIndex[i];
				GIAnetworkIndexNodeArray[relationIndex[i]]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
				#endif
				*/
			}
		}

		currentRelationInList = currentRelationInList->next;
	}
}




void GIAsemanticParserTranslatorClass::fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList;

	currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA])
			{
				//modal auxiliary [/copula] found; eg modalAuxiliary(sad-3, was-2) / eg modalAuxiliary(had-5, must-3) / modalAuxiliary(had-5, have-4) The dog must have had it.
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int modalAuxiliaryIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode* entity = translatorVariables->GIAentityNodeArray[thingIndex];
				string modalAuxiliaryString = translatorVariables->GIAentityNodeArray[modalAuxiliaryIndex]->wordOrig;	//featureArrayTemp[modalAuxiliaryIndex]->word;

				this->updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entity, modalAuxiliaryString);
				currentRelationInList->disabled = true;
			}
		}
		currentRelationInList = currentRelationInList->next;
	}


}

void GIAsemanticParserTranslatorClass::updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, const string modalAuxiliaryString)
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

void GIAsemanticParserTranslatorClass::defineSubstancesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0a2 pass; define substances all nodes" << endl;
	#endif
	GIAtranslatorDefineSubstances.defineSubstancesAllNodes(translatorVariables);

	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "define substances action networkIndexes, eg 'swim' in 'To swim to the beach requires strength.'" << endl;
	#endif
	GIAtranslatorDefineSubstances.defineSubstancesActionNetworkIndexes(translatorVariables);
	#endif

	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(translatorVariables->GIAentityNodeArrayFilled[i])
		{
			GIAentityNode* entity = translatorVariables->GIAentityNodeArray[i];
			bool isNetworkIndex = false;
			if(entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_NOUN)
			{
				bool hasDeterminer = false;
				bool indefiniteDeterminer = false;
				string determinerString = "";
				GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
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
								determinerString = translatorVariables->GIAentityNodeArray[determinerIndex]->entityName;
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
				//eg Bats are animals.  (definition connection, bat = networkIndex, animal = networkIndex)	/ bats ride bikes
				if(!hasDeterminer)
				{
					if(entity->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
					{
						isNetworkIndex = true;
					}
					/*
					//original GIA implementation uses isProperNoun detection instead of isPlural detection; see defineSubstancesBasedOnDeterminatesOfDefinitionEntities()
					if(!(entity->grammaticalIsProperNoun))
					{
						isNetworkIndex = true;
					}
					*/
				}

				//check if special case as defined in defineSubstancesBasedOnDeterminatesOfDefinitionEntities();
				//case 2. see if has an indefinite determiner (a/an) and is part of a definition connection
				//eg A book is an animal (definition connection, bat = networkIndex, animal = networkIndex)
				//eg The bat is an animal (definition connection, bat = substance, animal = networkIndex)
				if(hasDeterminer && indefiniteDeterminer)
				{
					GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 					while(currentRelationInList->next != NULL)
					{
						if(!(currentRelationInList->disabled))
						{
							if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION])
							{
								int thingIndex = currentRelationInList->relationGovernorIndex;
								int determinerIndex = currentRelationInList->relationDependentIndex;
								if((thingIndex == i) || (determinerIndex == i))
								{
									isNetworkIndex = true;
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
			translatorVariables->GIAentityNodeArray[i] = addInstanceToInstanceDefinition(param->GIAentityNodeArray[functionEntityIndex1], GIA_ENTITY_TYPE_SUBSTANCE);
			*/

			if(isNetworkIndex)
			{
				#ifdef GIA_DEBUG
				//cout << "isNetworkIndex" << endl;
				#endif
				translatorVariables->GIAentityNodeArray[i]->entityType = GIA_ENTITY_TYPE_CONCEPT;
			}
		}
	}
}

#ifdef GIA2_SUPPORT_QUERIES
void GIAsemanticParserTranslatorClass::identifyComparisonVariableBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(translatorVariables->GIAentityNodeArrayFilled[i])
			{
				GIAentityNode* entityNode = translatorVariables->GIAentityNodeArray[i];
				if(entityNode->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{
					entityNode->isQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "foundComparisonVariable" << endl;
					#endif
				}
			}
		}
	}

	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		#ifdef GIA_DEBUG
		//cout << "isQuestion" << endl;
		#endif
		const GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{
			GIAentityNode* entityNodes[2];
			string corpusSpecialRelationIsQuery[2];
			entityNodes[0] = translatorVariables->GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
			entityNodes[1] = translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex];
			corpusSpecialRelationIsQuery[0] = currentRelationInList->corpusSpecialRelationGovernorIsQuery;
			corpusSpecialRelationIsQuery[1] = currentRelationInList->corpusSpecialRelationDependentIsQuery;

			for(int i=0; i<2; i++)
			{
				GIAentityNode* entityNode = entityNodes[i];
				#ifdef GIA_DEBUG
				//cout << "entityNode = " << entityNode->entityName << endl;
				#endif
				if(corpusSpecialRelationIsQuery[i] == GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME)
				{
					cout << GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "foundComparisonVariable" << endl;
					#endif
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME)
				{
					cout << GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isNameQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "foundComparisonVariable" << endl;
					#endif
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME)
				{
					cout << GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isWhichOrEquivalentWhatQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
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

//note entityNodesActiveListNetworkIndexes is required for GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS only / featureArrayTemp is required for GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS:GIA2_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES only
void GIAsemanticParserTranslatorClass::defineConnectionsBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			int entity1Index = currentRelationInList->relationGovernorIndex;
			int entity2Index = currentRelationInList->relationDependentIndex;
			GIAentityNode* entity1 = translatorVariables->GIAentityNodeArray[entity1Index];
			GIAentityNode* entity2 = translatorVariables->GIAentityNodeArray[entity2Index];
			bool sameReferenceSet = currentRelationInList->sameReferenceSet;

			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT])
			{
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAtranslatorGeneric.cpp:genericDependecyRelationInterpretation
				string relationshipEntityName = RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES;
				GIAentityNode* relationshipEntity = this->createNewRelationshipEntitySemantic(relationshipEntityName, translatorVariables);
				int relationshipEntityIndex = relationshipEntity->entityIndexTemp;

				translatorVariables->GIAentityNodeArray[relationshipEntityIndex] = GIAtranslatorOperations.connectPropertyToEntity(entity1, entity2, relationshipEntity, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY])
			{
				bool foundMatchingObject = false;
				GIArelation* currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					int entity2Index2 = currentRelationInList2->relationDependentIndex;
					if(currentRelationInList2->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE])
					{
						if(entity2Index == entity2Index2)
						{//found matching conditionType pair
							int entity3Index = currentRelationInList2->relationDependentIndex;
							GIAentityNode* entity3 = translatorVariables->GIAentityNodeArray[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = translatorVariables->GIAentityNodeArray[entity2IndexRelation2];

							translatorVariables->GIAentityNodeArray[entity3Index] = GIAtranslatorOperations.connectPropertyToEntity(entity1, entity2relation2, entity3, sameReferenceSet);
							foundMatchingObject = true;
							currentRelationInList2->disabled = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				if(!foundMatchingObject)
				{
					cout << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);
				}
				currentRelationInList->disabled = true;
			}			
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION])
			{
				bool foundMatchingObject = false;
				GIArelation* currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					int entity2Index2 = currentRelationInList2->relationDependentIndex;
					if(currentRelationInList2->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE])
					{
						if(entity2Index == entity2Index2)
						{//found matching conditionType pair
							int entity3Index = currentRelationInList2->relationDependentIndex;
							GIAentityNode* entity3 = translatorVariables->GIAentityNodeArray[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = translatorVariables->GIAentityNodeArray[entity2IndexRelation2];

							translatorVariables->GIAentityNodeArray[entity3Index] = GIAtranslatorOperations.connectActionToEntity(entity1, entity2relation2, entity3, sameReferenceSet);
							foundMatchingObject = true;
							currentRelationInList2->disabled = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				if(!foundMatchingObject)
				{
					translatorVariables->GIAentityNodeArray[entity2Index] = GIAtranslatorOperations.connectActionToSubject(entity1, entity2, sameReferenceSet);
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE])
			{
				translatorVariables->GIAentityNodeArray[entity2Index] = GIAtranslatorOperations.connectActionToObject(entity1, entity2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION])
			{
				bool foundMatchingObject = false;
				GIArelation* currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					int entity2Index2 = currentRelationInList2->relationDependentIndex;
					if(currentRelationInList2->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE])
					{
						if(entity2Index == entity2Index2)
						{//found matching conditionType pair
							int entity3Index = currentRelationInList2->relationDependentIndex;
							GIAentityNode* entity3 = translatorVariables->GIAentityNodeArray[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = translatorVariables->GIAentityNodeArray[entity2IndexRelation2];

							currentRelationInList2->disabled = true;
							foundMatchingObject = true;

							#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
							this->invertOrDuplicateConditionsIfRequiredSemantic(translatorVariables, entity1, entity2relation2, entity3, sameReferenceSet);
							#else
							translatorVariables->GIAentityNodeArray[entity3Index] = GIAtranslatorOperations.connectConditionToEntity(entity1, entity2relation2, entity3, sameReferenceSet);
							#endif
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}
				if(!foundMatchingObject)
				{
					translatorVariables->GIAentityNodeArray[entity2Index] = GIAtranslatorOperations.connectConditionToSubject(entity1, entity2, sameReferenceSet);
					cout << "defineConnectionsBasedOnSemanticRelations{} warning: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION]) && (!foundMatchingObject) [ie without matching GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE]" << endl;
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE])
			{
				//NB THIS SHOULDNT BE required as there is never an isolated condition-object connection declared according to current GIA specification [but due to bug, it is required for Relex with measure dependency cases eg 'He runs every hour.'];
				cout << "defineConnectionsBasedOnSemanticRelations{} warning: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE]) without matching GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION" << endl;
				translatorVariables->GIAentityNodeArray[entity2Index] = GIAtranslatorOperations.connectConditionToObject(entity1, entity2, sameReferenceSet);
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT])
			{
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAtranslatorGeneric.cpp:genericDependecyRelationInterpretation
				string relationshipEntityName = RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS;
				GIAentityNode* relationshipEntity = this->createNewRelationshipEntitySemantic(relationshipEntityName, translatorVariables);
				int relationshipEntityIndex = relationshipEntity->entityIndexTemp;

				translatorVariables->GIAentityNodeArray[relationshipEntityIndex] = GIAtranslatorOperations.connectDefinitionToEntity(entity1, entity2, relationshipEntity, relationshipEntity, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			//FUTURE FIXTHIS
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_MARK_CONNECTION_AS_ALIAS])
			{
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAtranslatorGeneric.cpp:genericDependecyRelationInterpretation
				string relationshipEntityName = RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS;
				GIAentityNode* relationshipEntity = this->createNewRelationshipEntitySemantic(relationshipEntityName, translatorVariables);
				int relationshipEntityIndex = relationshipEntity->entityIndexTemp;
				
				translatorVariables->GIAentityNodeArray[relationshipEntityIndex] = GIAtranslatorOperations.connectDefinitionToEntityMarkConnectionAsAlias(entity1, entity2, relationshipEntity, sameReferenceSet);
				currentRelationInList->disabled = true;	//CHECKTHIS
			}			
			#else
			#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_ENTITY_NODES_ADD_ALIAS])
			{
				GIAtranslatorOperations.mergeEntityNodesAddAlias(entity1, entity2);
				translatorVariables->GIAentityNodeArray[entity2Index] = translatorVariables->GIAentityNodeArray[entity1Index];
				currentRelationInList->disabled = true;
			}
			#endif
			#endif
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION])
			{
				bool foundMatchingObject = false;
				GIArelation* currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
 				while(currentRelationInList2->next != NULL)
				{
					int entity2Index2 = currentRelationInList2->relationDependentIndex;
					if(currentRelationInList2->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE])
					{
						if(entity2Index == entity2Index2)
						{//found matching conditionType pair
							int entity3Index = currentRelationInList2->relationDependentIndex;
							GIAentityNode* entity3 = translatorVariables->GIAentityNodeArray[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = translatorVariables->GIAentityNodeArray[entity2IndexRelation2];

							translatorVariables->GIAentityNodeArray[entity3Index] = GIAtranslatorOperations.connectDefinitionToEntity(entity1, entity2relation2, entity3, sameReferenceSet);
							foundMatchingObject = true;
							currentRelationInList2->disabled = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				if(!foundMatchingObject)
				{
					cout << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);
				}
				currentRelationInList->disabled = true;
			}
			else
			{

			}
		}

		currentRelationInList = currentRelationInList->next;
	}
}


#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
//based on invertOrDuplicateConditionsIfRequired{}
void GIAsemanticParserTranslatorClass::invertOrDuplicateConditionsIfRequiredSemantic(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet)
{
	bool inverseConditionRequired = false;
	bool twoWayConditionRequired = false;
	string inverseConditionName = "";
	GIApreprocessorMultiwordReduction.detectIfInverseOrTwoWayConditionRequired(entity3condition->entityName, &inverseConditionRequired, &twoWayConditionRequired, &inverseConditionName);

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
	if(inverseConditionRequired)
	{
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
		cout << "invertOrDuplicateConditionsIfRequired{}: inverseConditionRequired: inverseConditionName = " << inverseConditionName  << endl;
		#endif
		GIAentityNode* inverseConditionEntity = this->createNewInverseConditionEntitySemantic(translatorVariables, inverseConditionName);
		translatorVariables->GIAentityNodeArray[inverseConditionEntity->entityIndexTemp] = GIAtranslatorOperations.connectConditionToEntity(entity2, entity1, inverseConditionEntity, sameReferenceSet);
	}
	else
	{
		translatorVariables->GIAentityNodeArray[entity3condition->entityIndexTemp] = GIAtranslatorOperations.connectConditionToEntity(entity1, entity2, entity3condition, sameReferenceSet);
	}
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
	if(twoWayConditionRequired)
	{
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
		cout << "invertOrDuplicateConditionsIfRequired{}: twoWayConditionRequired: inverseConditionName = " << inverseConditionName << endl;
		#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
		GIAentityNode* inverseConditionEntity = this->createNewInverseConditionEntitySemantic(translatorVariables, entity3condition->entityName);
		translatorVariables->GIAentityNodeArray[inverseConditionEntity->entityIndexTemp] = GIAtranslatorOperations.connectConditionToEntity(entity2, entity1, inverseConditionEntity, sameReferenceSet);
		inverseConditionEntity->inverseConditionTwoWay = true;
		#ifdef GIA_DEBUG
		//cout << "inverseConditionTwoWay" << endl;
		#endif
		#endif
		//inverseConditionEntity->conditionTwoWay = true;	//FUTURE FIXTHIS
		#ifdef GIA_DEBUG
		//cout << "conditionTwoWay" << endl;
		#endif
	}
	#endif
}

//based on createNewInverseConditionEntity{}
GIAentityNode* GIAsemanticParserTranslatorClass::createNewInverseConditionEntitySemantic(GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName)
{
	//requires GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW?
	int inverseConditionEntityIndex = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent - 1;

	//CHECKTHIS;
	/*//not possible as networkIndex entities have already been defined (locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations has already been executed) unlike createNewInverseConditionEntity{}
	GIAentityNode* inverseConditionEntity = new GIAentityNode();
	translatorVariables->GIAentityNodeArrayFilled[inverseConditionEntityIndex] = true;
	translatorVariables->GIAentityNodeArray[inverseConditionEntityIndex] = inverseConditionEntity;
	*/
	bool entityAlreadyExistant = false;
	GIAentityNode* inverseConditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(inverseConditionEntityIndex, &inverseConditionName, &entityAlreadyExistant, translatorVariables);
	inverseConditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	inverseConditionEntity->entityIndexTemp = inverseConditionEntityIndex;
	//why not set inverseConditionEntity->sentenceIndexTemp?

	#ifdef GIA2_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	translatorVariables->featureArrayTemp[inverseConditionEntityIndex] = new GIAfeature();
	translatorVariables->featureArrayTemp[inverseConditionEntityIndex]->word = inverseConditionName;
	translatorVariables->featureArrayTemp[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	translatorVariables->featureArrayTemp[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
	#endif

	return inverseConditionEntity;
}
#endif

//based on createNewInverseConditionEntitySemantic{}
GIAentityNode* GIAsemanticParserTranslatorClass::createNewRelationshipEntitySemantic(const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables)
{
	/*//not possible as networkIndex entities have already been defined (locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations has already been executed) unlike createNewInverseConditionEntity{}
	GIAentityNode* relationshipEntity = new GIAentityNode();
	translatorVariables->GIAentityNodeArrayFilled[relationshipEntityIndex] = true;
	translatorVariables->GIAentityNodeArray[relationshipEntityIndex] = relationshipEntity;
	*/
	
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationship(&relationshipEntityName, entityNodesActiveListNetworkIndexes, true);

	//why not set relationshipEntity->sentenceIndexTemp?

	#ifdef GIA2_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	translatorVariables->featureArrayTemp[relationshipEntityIndex] = new GIAfeature();
	translatorVariables->featureArrayTemp[relationshipEntityIndex]->word = relationshipEntityName;
	translatorVariables->featureArrayTemp[relationshipEntityIndex]->lemma = relationshipEntityName;	//is this necessary?
	translatorVariables->featureArrayTemp[relationshipEntityIndex]->entityIndex = relationshipEntity->entityIndexTemp;
	#endif

	return relationshipEntity;
}


void GIAsemanticParserTranslatorClass::applyAdvancedFeaturesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	this->defineQuantitiesBasedOnSemanticRelations(translatorVariables);

	this->defineQualitiesBasedOnSemanticRelations(translatorVariables);

	//measure, dates, and quantities??
	GIAtranslatorApplyAdvancedFeatures.defineTenseOnlyTimeConditions(translatorVariables);
}

void GIAsemanticParserTranslatorClass::defineQuantitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY])
			{
				int entity1Index = currentRelationInList->relationGovernorIndex;
				int entity2Index = currentRelationInList->relationDependentIndex;
				GIAentityNode* entity1 = translatorVariables->GIAentityNodeArray[entity1Index];
				GIAentityNode* entity2 = translatorVariables->GIAentityNodeArray[entity2Index];
				entity1->hasQuantity = true;

				GIAentityNode* quantitySubstance = entity1;

				if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					//code copied from extractQuantitiesStanfordCoreNLP{}:
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

					int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;
					quantitySubstance->entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//added 2a11a [because defineConcepts() does not have access to quantity data]

					/*//comparison variables not yet implemented for GIA2
					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					}
					*/
					//}
				}
				else if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
				{
					quantitySubstance->hasQuantity = true;
					quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

					int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;

					/*//comparison variables not yet implemented for GIA2
					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						translatorVariables->GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isQuery = false;
						GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
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
					GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
}

void GIAsemanticParserTranslatorClass::defineQualitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(translatorVariables->GIAentityNodeArrayFilled[w])
		{
			GIAentityNode* entity = translatorVariables->GIAentityNodeArray[w];
			if(!(entity->disabled))
			{
				if((entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ) || (entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADV))
				{
					entity->entityType = GIA_ENTITY_TYPE_QUALITY;
				}
			}
		}
	}
}

#ifdef GIA2_SEMANTIC_PARSER
bool GIAsemanticParserTranslatorClass::generateAllPermutationsFromSemanticRelationsFile(GIAfeature* firstFeatureInList, const int NLPfeatureParser)
{
	//code based on loadSemanticParserCorpusDatabaseFile{}:
	bool result = true;
	bool createNewSentences = true;
	bool parseGIA2file = true;
	bool isQuery = false;	//irrelevant
	#ifdef GIA2_SEMANTIC_PARSER_DEBUG
	cout << "generateAllPermutationsFromSemanticRelationsFile{}:" << endl;
	#endif

	GIArelation* firstRelationInList = NULL;
	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD	//read in the relations just written to file by writeSemanticParserCorpusFile{}
	string corpusFileName = GIAsemanticParserDatabase.semanticParserCorpusDBgenerateFileName(firstFeatureInList);
	GIAsentence* sentence = new GIAsentence();
	if(!GIAnlp.parseStanfordParserFile(corpusFileName, isQuery, sentence, createNewSentences, parseGIA2file, false))		//CHECKTHIS; need to account for corpus XyXysemanticRelations.txt having multiple entries [eg different text but identical layout]
	{
		result = false;
		cout << "generateAllPermutationsFromSemanticRelationsFile{} error: !parseStanfordParserFile" << endl;
	}
	else
	{
		firstRelationInList = sentence->firstRelationInList;
		//firstFeatureInList = sentence->firstFeatureInList;	//use original featureList (not that derived from corpus file)
		int maxNumberOfWordsInSentence = sentence->maxNumberOfWordsInSentence;
	#else
		firstRelationInList = GIAsemanticParserDatabase.getFirstRelationInSemanticParserSentenceList();
		int maxNumberOfWordsInSentence = GIAsentenceClass.calculateNumberOfWordsInSentence(firstFeatureInList);
	#endif

		GIAfeature* dummyBlankFeature = new GIAfeature();
		#ifdef GIA2_SEMANTIC_PARSER_DEBUG
		cout << "parseStanfordParserFile{} passed." << endl;
		#endif
		//now simulate GIA2 semantic relations for each subset of original sentence POS permutation
		GIAfeature* secondWordInTupleFeature = firstFeatureInList;
		int minIndexOfSecondWordInTuple = GIA2_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE;
		for(int i=GIA_NLP_START_ENTITY_INDEX; i<minIndexOfSecondWordInTuple; i++)
		{
			secondWordInTupleFeature = secondWordInTupleFeature->next;
		}

		for(int secondWordInTupleIndex=minIndexOfSecondWordInTuple; secondWordInTupleIndex<=maxNumberOfWordsInSentence; secondWordInTupleIndex++)	//secondWordInTupleIndex in subset
		{
			//NB "secondWordInTupleIndex" aka centralWordIndex
			#ifdef GIA2_SEMANTIC_PARSER_DEBUG
			cout << "secondWordInTupleIndex = " << secondWordInTupleIndex << ", " << secondWordInTupleFeature->lemma << endl;
			#endif
			GIAfeature* recordOfFeatureAfterSecondWordInTupleFeature = secondWordInTupleFeature->next;
			secondWordInTupleFeature->next = dummyBlankFeature;	//temporarily disconnect node at end of sentence subset

			if(firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)
			{
				cout << "generateAllPermutationsFromSemanticRelationsFile{} implementation error*: (firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)" << endl;
				exit(EXIT_ERROR);
			}
			#ifdef GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS
			if(!SHAREDvars.textInTextArray(secondWordInTupleFeature->word, semanticParserOptimiseBasedOnConjunctionsIllegalSecondWordInTupleArray, GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS_ILLEGAL_CENTRAL_WORD_NUMBER_OF_TYPES))
			{
				bool optimisedBasedOnContextualConjunctions = false;
				GIAfeature* firstFeatureInSentenceSubsetInitial = GIAsemanticParserOperations.generateOptimisedFeatureSubsetBasedOnContextualConjunctions(firstFeatureInList, secondWordInTupleIndex, &optimisedBasedOnContextualConjunctions);
			#else
				GIAfeature* firstFeatureInSentenceSubsetInitial = firstFeatureInList;
			#endif
				int maxIndexOfFirstWordInTuple = (secondWordInTupleFeature->entityIndex - (GIA2_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE-1));
				#ifdef GIA2_SEMANTIC_PARSER_DEBUG
				cout << "\tmaxIndexOfFirstWordInTuple = " << maxIndexOfFirstWordInTuple << endl;
				#endif
				#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
				for(int firstWordInTupleIndex = GIA_NLP_START_ENTITY_INDEX; firstWordInTupleIndex <= maxIndexOfFirstWordInTuple; firstWordInTupleIndex++)
				{
				#endif
					GIAfeature* firstFeatureInSentenceSubset = firstFeatureInSentenceSubsetInitial;
					while(firstFeatureInSentenceSubset->entityIndex <= maxIndexOfFirstWordInTuple)		//changed 2k1a for GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS compatibility: verify that feature entityIndices are not mutated by GIA referencing*	//OLD: for(int firstWordInSentenceSubsetIndex=1; firstWordInSentenceSubsetIndex<secondWordInTupleIndex; firstWordInSentenceSubsetIndex++)
					{
						int firstWordInSentenceSubsetIndex=firstFeatureInSentenceSubset->entityIndex;
						#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
						if(firstWordInTupleIndex >= firstWordInSentenceSubsetIndex)
						{
						#endif
							#ifdef GIA2_SEMANTIC_PARSER_DEBUG
							cout << "\t\tfirstWordInTupleIndex = " << firstWordInTupleIndex << endl;
							cout << "\t\t\tfirstWordInSentenceSubsetIndex = " << firstWordInSentenceSubsetIndex << ", " << firstFeatureInSentenceSubset->lemma << endl;
							#endif
							int subsetSize = secondWordInTupleIndex-firstWordInSentenceSubsetIndex+1;	//subsetSize aka maxSpread

							#ifdef GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
							//code from convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:
							string sentenceText = "";
							bool foundAtLeastOneRelation = false;
							GIArelation* currentSemanticRelationInList = firstRelationInList;
							while(currentSemanticRelationInList->next != NULL)
							{
								if((currentSemanticRelationInList->relationGovernorIndex >= firstWordInSentenceSubsetIndex) && ((currentSemanticRelationInList->relationGovernorIndex <= secondWordInTupleIndex) || (currentSemanticRelationInList->relationGovernorIndex > FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY))
								&& (currentSemanticRelationInList->relationDependentIndex >= firstWordInSentenceSubsetIndex) && ((currentSemanticRelationInList->relationDependentIndex <= secondWordInTupleIndex) || (currentSemanticRelationInList->relationDependentIndex > FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)))
								{
									//regenerate semantic relation based on parsed GIArelation object
									string GIA2semanticDependencyRelation = GIAsemanticParserOperations.generateGIA2semanticDependencyRelationText(currentSemanticRelationInList->relationGovernor, currentSemanticRelationInList->relationDependent, currentSemanticRelationInList->relationType, currentSemanticRelationInList->relationGovernorIndex, currentSemanticRelationInList->relationDependentIndex, currentSemanticRelationInList->sameReferenceSet);
									GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + STRING_NEW_LINE;
									sentenceText = sentenceText + GIA2semanticDependencyRelation;

									foundAtLeastOneRelation = true;
								}
								currentSemanticRelationInList = currentSemanticRelationInList->next;
							}
							sentenceText = sentenceText + STRING_NEW_LINE;	//required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
							if(foundAtLeastOneRelation)
							{
								GIAsemanticParserDatabase.prepareSemanticParserCorpusDatabaseFileTextForWriting();
								GIAsemanticParserDatabase.saveTextToCorpusFileString(sentenceText);
								GIAsemanticParserDatabase.writeSemanticParserCorpusFile(firstFeatureInSentenceSubset);
							}
							#endif

							#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
							bool foundAtLeastOneRelation = false;
							int firstWordInTupleIndexRelative = GIAsemanticParserOperations.calculateFirstWordInTupleIndexRelative(firstWordInTupleIndex, firstWordInSentenceSubsetIndex);
							GIArelation* currentSemanticRelationInList = firstRelationInList;
							while(currentSemanticRelationInList->next != NULL)
							{
								#ifdef GIA_DEBUG
								//cout << "currentSemanticRelationInList->relationType = " << currentSemanticRelationInList->relationType << endl;
								#endif
								int semanticDependencyRelationType = INT_DEFAULT_VALUE;
								bool sameReferenceSet = currentSemanticRelationInList->sameReferenceSet;
								if(SHAREDvars.textInTextArray(currentSemanticRelationInList->relationType, GIA2semanticDependencyRelationNameArray, GIA2_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES, &semanticDependencyRelationType))
								{
									//CHECKTHIS; how to handle these cases (currentSemanticRelationInList->relationGovernorIndex >= FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY) || (currentSemanticRelationInList->relationDependentIndex >= FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY))
										//NB GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE can't handle queries at present (or where featureIndex >= FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
									if((currentSemanticRelationInList->relationGovernorIndex == firstWordInTupleIndex) && (currentSemanticRelationInList->relationDependentIndex == secondWordInTupleIndex))
									{
										//write semantic relation data
										bool directionGovernorToDependent = true;
										GIAsemanticParserDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
										foundAtLeastOneRelation = true;	//CHECKTHIS
										#ifdef GIA2_SEMANTIC_PARSER_DEBUG
										cout << "\t\t\t\twrite semantic relation data" << endl;
										cout << "\t\t\t\tcurrentSemanticRelationInList->relationType = " << currentSemanticRelationInList->relationType << endl;
										cout << "\t\t\t\tcurrentSemanticRelationInList->relationGovernorIndex = " << currentSemanticRelationInList->relationGovernorIndex << endl;
										cout << "\t\t\t\tcurrentSemanticRelationInList->relationDependentIndex = " << currentSemanticRelationInList->relationDependentIndex << endl;
										#endif
										GIAfeature* currentFeatureInSentence = firstFeatureInList;
										while(currentFeatureInSentence->next != NULL)
										{
											string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemanticParserPOStype];
											#ifdef GIA2_SEMANTIC_PARSER_DEBUG
											cout << "\t\t\t\t\tstanfordPOS = " << currentFeatureInSentence->stanfordPOS << endl;
											cout << "\t\t\t\t\tPOStypeAbbreviationName = " << POStypeAbbreviationName << endl;
											#endif
											currentFeatureInSentence = currentFeatureInSentence->next;
										}
									}
									else if((currentSemanticRelationInList->relationGovernorIndex == secondWordInTupleIndex) && (currentSemanticRelationInList->relationDependentIndex == firstWordInTupleIndex))
									{
										//write semantic relation data
										bool directionGovernorToDependent = false;
										GIAsemanticParserDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
										foundAtLeastOneRelation = true;	//CHECKTHIS
										#ifdef GIA2_SEMANTIC_PARSER_DEBUG
										cout << "\t\t\t\twrite semantic relation data" << endl;
										cout << "\t\t\t\tcurrentSemanticRelationInList->relationType = " << currentSemanticRelationInList->relationType << endl;
										cout << "\t\t\t\tcurrentSemanticRelationInList->relationGovernorIndex = " << currentSemanticRelationInList->relationGovernorIndex << endl;
										cout << "\t\t\t\tcurrentSemanticRelationInList->relationDependentIndex = " << currentSemanticRelationInList->relationDependentIndex << endl;
										#endif

										GIAfeature* currentFeatureInSentence = firstFeatureInList;
										while(currentFeatureInSentence->next != NULL)
										{
											string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemanticParserPOStype];
											#ifdef GIA2_SEMANTIC_PARSER_DEBUG
											cout << "\t\t\t\t\tstanfordPOS = " << currentFeatureInSentence->stanfordPOS << endl;
											cout << "\t\t\t\t\tPOStypeAbbreviationName = " << POStypeAbbreviationName << endl;
											#endif
											currentFeatureInSentence = currentFeatureInSentence->next;
										}
									}
								}
								else
								{
									cout << "generateAllPermutationsFromSemanticRelationsFile{} error: illegal semantic relation type; currentSemanticRelationInList->relationType = " << currentSemanticRelationInList->relationType << endl;
									exit(EXIT_ERROR);
								}
								currentSemanticRelationInList = currentSemanticRelationInList->next;
							}
							#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_EXTENSIVE
							if(!foundAtLeastOneRelation)
							{
								GIAsemanticParserDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, SEMANTIC_RELATION_TYPE_NON_EXISTENT, BOOL_IRRELEVANT, BOOL_IRRELEVANT);
							}
							#endif
							#endif
						#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
						}
						#endif
						firstFeatureInSentenceSubset = firstFeatureInSentenceSubset->next;
					}
				#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
				}
				#endif
			#ifdef GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS
				if(optimisedBasedOnContextualConjunctions)
				{
					delete firstFeatureInSentenceSubsetInitial;	//delete artificially generated optimised sentence subset
				}
			}
			#endif

			secondWordInTupleFeature->next = recordOfFeatureAfterSecondWordInTupleFeature;	//restore temporarily disconnected node at end of sentence subset
			secondWordInTupleFeature = secondWordInTupleFeature->next;
		}
	#ifdef GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
	}
	#endif

	return result;
}

#endif



#endif







