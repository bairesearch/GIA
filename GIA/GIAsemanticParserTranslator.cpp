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
 * Project Version: 3d5e 11-August-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#include "GIAsemanticParserTranslator.hpp"
#ifdef GIA_SEMANTIC_PARSER_SUBSETS
#endif
#ifdef GIA_BOT
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
#endif

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_SEMANTIC_PARSER

//based on convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:
void GIAsemanticParserTranslatorClass::convertSentenceSemanticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList)
{
	translatorVariables->currentSentenceInList.relationshipEntityArtificialIndexCurrent = GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList) + SENTENCE_FIRST_ARTIFICIAL_INDEX;

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

	vector<bool> GIAentityNodeArrayFilled(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));
	vector<GIAentityNode*> GIAnetworkIndexNodeArray(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));
	vector<GIAentityNode*> GIAentityNodeArray(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));
	vector<GIAfeature*> featureArrayTemp(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));
	translatorVariables->GIAentityNodeArrayFilled = &GIAentityNodeArrayFilled;
	translatorVariables->GIAnetworkIndexNodeArray = &GIAnetworkIndexNodeArray;	
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;
	translatorVariables->featureArrayTemp = &featureArrayTemp;
	
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAnetworkIndexNodeArray[w] = NULL;
		GIAentityNodeArray[w] = NULL;
		featureArrayTemp[w] = NULL;
	}

	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	GIAbot.botSwitchFirstAndSecondPersonSemantic(translatorVariables->currentSentenceInList);
	#endif


	this->locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(translatorVariables);
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		GIAentityNodeArray[w] = GIAnetworkIndexNodeArray[w];		//set default values of GIAentityNodeArray
	}


	GIAtranslatorOperations.generateTempFeatureArray(translatorVariables->currentSentenceInList->firstFeatureInList, featureArrayTemp);	//regeneration required for Relex in case query variables detected


	#ifdef GIA_RELEX
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX) 				//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		GIAtranslatorDefineGrammar.fillGrammaticalArraysRelex(translatorVariables->currentSentenceInList);
	}
	#endif
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	if((translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER) || (translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP))	//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//fillGrammaticalArraysStanford(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser, featureArrayTemp);	//not possible as Stanford syntactical dependency relations are assumed not available

		//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
		GIAtranslatorDefineGrammar.extractGrammaticalInformationStanford(translatorVariables->currentSentenceInList->firstFeatureInList, translatorVariables->NLPfeatureParser);
		this->fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(translatorVariables);
	}
	#endif

 	GIAtranslatorDefineGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, translatorVariables->currentSentenceInList->firstFeatureInList);


	GIAtranslatorRedistributeRelations.redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(translatorVariables);

	translatorVariables->GIAfeatureTempEntityNodeArray = GIAentityNodeArray;	//required for linkAdvancedReferencesGIA/linkPronounReferencesRelex/linkPronounAndTextualContextReferencesStanfordCoreNLP
#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
		GIAtranslatorDefineReferencing.linkAdvancedReferencesGIA(translatorVariables, firstGIAcoreferenceInList);	//NB second last parameter used to be GIAfeatureTempEntityNodeArray
	}
	else
	{
	#endif
		//is pronoun referencing supported by GIA2?

		#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
		#endif
			GIAtranslatorDefineReferencing.linkPronounReferencesRelex(translatorVariables);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_STANFORD_CORENLP
		else if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			GIAtranslatorDefineReferencing.linkPronounAndTextualContextReferencesStanfordCoreNLP(translatorVariables, translatorVariables->firstSentenceInList->firstCoreferenceInList);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		}
		#endif
		#endif
		#endif
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif
#endif


	this->defineSubstancesBasedOnSemanticRelations(translatorVariables);

	#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
	this->identifyComparisonVariableBasedOnSemanticRelations(translatorVariables);
	#endif


	this->defineConnectionsBasedOnSemanticRelations(translatorVariables);

	#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEMANTIC_PARSER_SEMANTIC_RELATION_GENERATION
	GIAtranslatorLinkEntitiesDynamic.linkEntitiesDynamic(translatorVariables);
	#endif


	this->applyAdvancedFeaturesBasedOnSemanticRelations(translatorVariables);


	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			//#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION	//networkIndex sentenceIndex information is also required for GIAdraw.cpp
			#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEMANTIC_PARSER_SEMANTIC_RELATION_GENERATION
			if(GIAentityNodeArray[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)	//added condition 2j5b [check this is required]
			{
				GIAentityNodeArray[w]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
				cout << "convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->sentenceIndexTemp undefined" << endl;
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
						cout << "convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->networkIndexNode->entityIndexTemp undefined" << endl;
						networkIndexNode->entityIndexTemp = w;
					}
					if(networkIndexNode->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
					{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
						networkIndexNode->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
						cout << "convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->networkIndexNode->sentenceIndexTemp undefined" << endl;
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
				cout << "convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAnetworkIndexNodeArray[" << w << "]->sentenceIndexTemp undefined" << endl;
			}
			#endif
			//#endif

		}
	}

	//GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray);		//this method is not sufficient, as some networkIndex entity nodes (eg prepositions/conditions) are not contained within GIAnetworkIndexNodeArray
	GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(translatorVariables->entityNodesActiveListNetworkIndexes);


	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	//required to reset wasReferenceTemp for next time
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNodeArray[w]->wasReferenceTemp = false;
		}
	}
	#endif


	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
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

	translatorVariables->entityNodesActiveListSentences->insert(pair<int, vector<GIAentityNode*>*>(translatorVariables->currentSentenceInList->sentenceIndex, entityNodesActiveListSentence));
}


void GIAsemanticParserTranslatorClass::locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	//code from locateAndAddAllFeatureTempEntities{}:

	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		bool isDependencyRelationSecondary = false;
		for(int i=0; i<GIA_SEMANTIC_PARSER_SEMANTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES; i++)
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

		for(int i=0; i<2; i++)
		{
			if(!(*translatorVariables->GIAentityNodeArrayFilled)[relationIndex[i]])
			{
				(*translatorVariables->GIAentityNodeArrayFilled)[relationIndex[i]] = true;

				//code from locateAndAddAllNetworkIndexEntities{}:

				bool entityAlreadyExistant = false;
				GIAentityNode* networkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&(name[i]), &entityAlreadyExistant, translatorVariables);
				(*translatorVariables->GIAnetworkIndexNodeArray)[relationIndex[i]] = networkIndexEntity;


				if(isDependencyRelationSecondary)
				{
					if(i == 1)
					{
						networkIndexEntity->disabled = true;	//disable is/have etc nodes
					}
				}

				networkIndexEntity->hasAssociatedInstanceTemp = false;

				translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(networkIndexEntity);

				#ifdef GIA_SEMANTIC_PARSER_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
				if(relationIndex[i] >= (translatorVariables->currentSentenceInList->numberOfWordsInSentence))
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
				GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[thingIndex];
				string modalAuxiliaryString = (*translatorVariables->GIAentityNodeArray)[modalAuxiliaryIndex]->wordOrig;	//featureArrayTemp[modalAuxiliaryIndex]->word;

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
	GIAtranslatorDefineSubstances.defineSubstancesAllNodes(translatorVariables);

	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	GIAtranslatorDefineSubstances.defineSubstancesActionNetworkIndexes(translatorVariables);
	#endif

	for(int i=0; i<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); i++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[i])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[i];
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
								determinerString = (*translatorVariables->GIAentityNodeArray)[determinerIndex]->entityName;
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
			(*translatorVariables->GIAentityNodeArray)[i] = addInstanceToInstanceDefinition(param->GIAentityNodeArray[functionEntityIndex1], GIA_ENTITY_TYPE_SUBSTANCE);
			*/

			if(isNetworkIndex)
			{
				(*translatorVariables->GIAentityNodeArray)[i]->entityType = GIA_ENTITY_TYPE_CONCEPT;
			}
		}
	}
}

#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
void GIAsemanticParserTranslatorClass::identifyComparisonVariableBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	if(translatorVariables->currentSentenceInList->isQuestion)
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
	}

	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		const GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{
			GIAentityNode* entityNodes[2];
			string corpusSpecialRelationIsQuery[2];
			entityNodes[0] = (*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationGovernorIndex];
			entityNodes[1] = (*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationDependentIndex];
			corpusSpecialRelationIsQuery[0] = currentRelationInList->corpusSpecialRelationGovernorIsQuery;
			corpusSpecialRelationIsQuery[1] = currentRelationInList->corpusSpecialRelationDependentIsQuery;

			for(int i=0; i<2; i++)
			{
				GIAentityNode* entityNode = entityNodes[i];
				if(corpusSpecialRelationIsQuery[i] == GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME)
				{
					cout << GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME)
				{
					cout << GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isNameQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME)
				{
					cout << GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isWhichOrEquivalentWhatQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
			}
			currentRelationInList = currentRelationInList->next;
		}
	}
}
#endif

//note entityNodesActiveListNetworkIndexes is required for GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS only / featureArrayTemp is required for GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS:GIA_SEMANTIC_PARSER_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES only
void GIAsemanticParserTranslatorClass::defineConnectionsBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			int entity1Index = currentRelationInList->relationGovernorIndex;
			int entity2Index = currentRelationInList->relationDependentIndex;
			GIAentityNode* entity1 = (*translatorVariables->GIAentityNodeArray)[entity1Index];
			GIAentityNode* entity2 = (*translatorVariables->GIAentityNodeArray)[entity2Index];
			bool sameReferenceSet = currentRelationInList->sameReferenceSet;

			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT])
			{
				#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAtranslatorGeneric.cpp:genericDependecyRelationInterpretation
				string relationshipEntityName = RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES;
				GIAentityNode* relationshipEntity = this->createNewRelationshipEntitySemantic(GIA_ENTITY_TYPE_PROPERTY, relationshipEntityName, translatorVariables);
				GIAtranslatorOperations.connectPropertyToEntity(entity1, entity2, relationshipEntity, sameReferenceSet, translatorVariables);
				#else
				GIAtranslatorOperations.connectDirectPropertyToEntity(entity1, entity2, sameReferenceSet, translatorVariables);		
				#endif
				
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
						{//found matching pair
							int entity3Index = currentRelationInList2->relationDependentIndex;
							GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

							#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
							GIAtranslatorOperations.connectPropertyToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
							#else
							GIAtranslatorOperations.connectDirectPropertyToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
							#endif
							foundMatchingObject = true;
							currentRelationInList2->disabled = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				if(!foundMatchingObject)
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY]) && (!foundMatchingObject)" << endl;
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
							GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

							GIAtranslatorOperations.connectActionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
							foundMatchingObject = true;
							currentRelationInList2->disabled = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				if(!foundMatchingObject)
				{
					GIAtranslatorOperations.connectActionToSubject(entity1, entity2, sameReferenceSet, translatorVariables);
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE])
			{
				GIAtranslatorOperations.connectActionToObject(entity1, entity2, sameReferenceSet, translatorVariables);
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
							GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

							currentRelationInList2->disabled = true;
							foundMatchingObject = true;

							#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
							this->invertOrDuplicateConditionsIfRequiredSemantic(translatorVariables, entity1, entity2relation2, entity3, sameReferenceSet);
							#else
							GIAtranslatorOperations.connectConditionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
							#endif
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}
				if(!foundMatchingObject)
				{
					GIAtranslatorOperations.connectConditionToSubject(entity1, entity2, sameReferenceSet, translatorVariables);
					cout << "defineConnectionsBasedOnSemanticRelations{} warning: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION]) && (!foundMatchingObject) [ie without matching GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE]" << endl;
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE])
			{
				//NB THIS SHOULDNT BE required as there is never an isolated condition-object connection declared according to current GIA specification [but due to bug, it is required for Relex with measure dependency cases eg 'He runs every hour.'];
				cout << "defineConnectionsBasedOnSemanticRelations{} warning: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE]) without matching GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION" << endl;
				GIAtranslatorOperations.connectConditionToObject(entity1, entity2, sameReferenceSet, translatorVariables);
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT])
			{
				#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAtranslatorGeneric.cpp:genericDependecyRelationInterpretation
				string relationshipEntityName = RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS;
				GIAentityNode* relationshipEntity = this->createNewRelationshipEntitySemantic(GIA_ENTITY_TYPE_DEFINITION, relationshipEntityName, translatorVariables);
				GIAtranslatorOperations.connectDefinitionToEntity(entity1, entity2, relationshipEntity, sameReferenceSet, translatorVariables);
				#else
				GIAtranslatorOperations.connectDirectDefinitionToEntity(entity1, entity2, sameReferenceSet, translatorVariables);				
				#endif
				currentRelationInList->disabled = true;
			}
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			//FUTURE FIXTHIS
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_MARK_CONNECTION_AS_ALIAS])
			{
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAtranslatorGeneric.cpp:genericDependecyRelationInterpretation
				string relationshipEntityName = RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS;
				GIAentityNode* relationshipEntity = this->createNewRelationshipEntitySemantic(GIA_ENTITY_TYPE_DEFINITION, relationshipEntityName, translatorVariables);
				
				GIAtranslatorOperations.connectDefinitionToEntityMarkConnectionAsAlias(entity1, entity2, relationshipEntity, sameReferenceSet);
				currentRelationInList->disabled = true;	//CHECKTHIS
			}			
			#else
			#ifdef GIA_SEMANTIC_PARSER_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_ENTITY_NODES_ADD_ALIAS])
			{
				bool notAlreadyMerged = GIAtranslatorOperations.mergeEntityNodesAddAlias(entity1, entity2, translatorVariables);
				if(notAlreadyMerged)
				{
					(*translatorVariables->GIAentityNodeArray)[entity2Index] = (*translatorVariables->GIAentityNodeArray)[entity1Index];
				}
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
							GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
							int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
							GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

							#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
							GIAtranslatorOperations.connectDefinitionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
							#else
							GIAtranslatorOperations.connectDirectDefinitionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
							#endif
							foundMatchingObject = true;
							currentRelationInList2->disabled = true;
						}
					}
					currentRelationInList2 = currentRelationInList2->next;
				}

				if(!foundMatchingObject)
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION]) && (!foundMatchingObject)" << endl;
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
		GIAentityNode* inverseConditionEntity = this->createNewInverseConditionEntitySemantic(translatorVariables, inverseConditionName);
		GIAtranslatorOperations.connectConditionToEntity(entity2, entity1, inverseConditionEntity, sameReferenceSet, translatorVariables);
	}
	else
	{
		GIAtranslatorOperations.connectConditionToEntity(entity1, entity2, entity3condition, sameReferenceSet, translatorVariables);
	}
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
	if(twoWayConditionRequired)
	{
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
		GIAentityNode* inverseConditionEntity = this->createNewInverseConditionEntitySemantic(translatorVariables, entity3condition->entityName);
		GIAtranslatorOperations.connectConditionToEntity(entity2, entity1, inverseConditionEntity, sameReferenceSet, translatorVariables);
		inverseConditionEntity->inverseConditionTwoWay = true;
		#endif
		//inverseConditionEntity->conditionTwoWay = true;	//FUTURE FIXTHIS
	}
	#endif
}

//based on createNewInverseConditionEntity{}
GIAentityNode* GIAsemanticParserTranslatorClass::createNewInverseConditionEntitySemantic(GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName)
{
	int inverseConditionEntityIndex = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;

	bool entityAlreadyExistant = false;
	GIAentityNode* inverseConditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(inverseConditionEntityIndex, &inverseConditionName, &entityAlreadyExistant, translatorVariables);
	inverseConditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	inverseConditionEntity->entityIndexTemp = inverseConditionEntityIndex;
	//why not set inverseConditionEntity->sentenceIndexTemp?

	#ifdef GIA_SEMANTIC_PARSER_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex] = new GIAfeature();
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->word = inverseConditionName;
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
	#endif

	return inverseConditionEntity;
}
#endif

//based on createNewInverseConditionEntitySemantic{}
GIAentityNode* GIAsemanticParserTranslatorClass::createNewRelationshipEntitySemantic(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipEntityType, relationshipEntityName, translatorVariables);

	#ifdef GIA_SEMANTIC_PARSER_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex] = new GIAfeature();
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex]->word = relationshipEntityName;
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex]->lemma = relationshipEntityName;	//is this necessary?
	(*translatorVariables->featureArrayTemp)[relationshipEntityIndex]->entityIndex = relationshipEntity->entityIndexTemp;
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
				GIAentityNode* entity1 = (*translatorVariables->GIAentityNodeArray)[entity1Index];
				GIAentityNode* entity2 = (*translatorVariables->GIAentityNodeArray)[entity2Index];
				entity1->hasQuantity = true;

				GIAentityNode* quantitySubstance = entity1;

				if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
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
						(*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationDependentIndex]->isQuery = false;
						GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					}
					*/
					//}
				}
				else if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
				{
					quantitySubstance->hasQuantity = true;
					quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

					int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;

					/*//comparison variables not yet implemented for GIA2
					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						(*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationDependentIndex]->isQuery = false;
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
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
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

#ifdef GIA_SEMANTIC_PARSER_SUBSETS
bool GIAsemanticParserTranslatorClass::generateAllPermutationsFromSemanticRelationsFile(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAfeature* firstFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
	const int NLPfeatureParser = translatorVariables->NLPfeatureParser;
	
	//code based on loadSemanticParserCorpusDatabaseFile{}:
	bool result = true;
	bool createNewSentences = true;
	bool parseGIA2file = true;
	bool isQuery = false;	//irrelevant

	GIArelation* firstRelationInList = NULL;
	#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD	//read in the relations just written to file by writeSemanticParserCorpusFile{}
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
		int numberOfWordsInSentence = sentence->numberOfWordsInSentence;
	#else
		firstRelationInList = GIAsemanticParserDatabase.getFirstRelationInSemanticParserSentenceList();
		int numberOfWordsInSentence = translatorVariables->currentSentenceInList->numberOfWordsInSentence;	//OLD before 3b4a: GIAsentenceClass.calculateNumberOfWordsInSentence(firstFeatureInList);
	#endif

		GIAfeature* dummyBlankFeature = new GIAfeature();
		//now simulate GIA2 semantic relations for each subset of original sentence POS permutation
		GIAfeature* secondWordInTupleFeature = firstFeatureInList;
		int minIndexOfSecondWordInTuple = GIA_SEMANTIC_PARSER_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE;
		for(int i=GIA_NLP_START_ENTITY_INDEX; i<minIndexOfSecondWordInTuple; i++)
		{
			secondWordInTupleFeature = secondWordInTupleFeature->next;
		}

		for(int secondWordInTupleIndex=minIndexOfSecondWordInTuple; secondWordInTupleIndex<=numberOfWordsInSentence; secondWordInTupleIndex++)	//secondWordInTupleIndex in subset
		{
			//NB "secondWordInTupleIndex" aka centralWordIndex
			GIAfeature* recordOfFeatureAfterSecondWordInTupleFeature = secondWordInTupleFeature->next;
			secondWordInTupleFeature->next = dummyBlankFeature;	//temporarily disconnect node at end of sentence subset

			if(firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)
			{
				cerr << "generateAllPermutationsFromSemanticRelationsFile{} implementation error*: (firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)" << endl;
				exit(EXIT_ERROR);
			}
			#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS
			if(!SHAREDvars.textInTextArray(secondWordInTupleFeature->word, semanticParserOptimiseBasedOnConjunctionsIllegalSecondWordInTupleArray, GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS_ILLEGAL_CENTRAL_WORD_NUMBER_OF_TYPES))
			{
				bool optimisedBasedOnContextualConjunctions = false;
				GIAfeature* firstFeatureInSentenceSubsetInitial = GIAsemanticParserOperations.generateOptimisedFeatureSubsetBasedOnContextualConjunctions(firstFeatureInList, secondWordInTupleIndex, &optimisedBasedOnContextualConjunctions);
			#else
				GIAfeature* firstFeatureInSentenceSubsetInitial = firstFeatureInList;
			#endif
				int maxIndexOfFirstWordInTuple = (secondWordInTupleFeature->entityIndex - (GIA_SEMANTIC_PARSER_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE-1));
				#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
				for(int firstWordInTupleIndex = GIA_NLP_START_ENTITY_INDEX; firstWordInTupleIndex <= maxIndexOfFirstWordInTuple; firstWordInTupleIndex++)
				{
				#endif
					GIAfeature* firstFeatureInSentenceSubset = firstFeatureInSentenceSubsetInitial;
					while(firstFeatureInSentenceSubset->entityIndex <= maxIndexOfFirstWordInTuple)		//changed 2k1a for GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS compatibility: verify that feature entityIndices are not mutated by GIA referencing*	//OLD: for(int firstWordInSentenceSubsetIndex=1; firstWordInSentenceSubsetIndex<secondWordInTupleIndex; firstWordInSentenceSubsetIndex++)
					{
						int firstWordInSentenceSubsetIndex=firstFeatureInSentenceSubset->entityIndex;
						#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
						if(firstWordInTupleIndex >= firstWordInSentenceSubsetIndex)
						{
						#endif
							int subsetSize = secondWordInTupleIndex-firstWordInSentenceSubsetIndex+1;	//subsetSize aka maxSpread

							#ifdef GIA_SEMANTIC_PARSER_SUBSETS_UNOPTIMISED_TEXT_CORPUS
							//code from convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:
							string sentenceText = "";
							bool foundAtLeastOneRelation = false;
							GIArelation* currentSemanticRelationInList = firstRelationInList;
							while(currentSemanticRelationInList->next != NULL)
							{
								if((currentSemanticRelationInList->relationGovernorIndex >= firstWordInSentenceSubsetIndex) && ((currentSemanticRelationInList->relationGovernorIndex <= secondWordInTupleIndex) || (currentSemanticRelationInList->relationGovernorIndex > GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList)))
								&& (currentSemanticRelationInList->relationDependentIndex >= firstWordInSentenceSubsetIndex) && ((currentSemanticRelationInList->relationDependentIndex <= secondWordInTupleIndex) || (currentSemanticRelationInList->relationDependentIndex > GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList))))
								{
									//regenerate semantic relation based on parsed GIArelation object
									string GIA2semanticDependencyRelation = GIAsemanticParserOperations.generateGIA2semanticDependencyRelationText(currentSemanticRelationInList->relationGovernor, currentSemanticRelationInList->relationDependent, currentSemanticRelationInList->relationType, currentSemanticRelationInList->relationGovernorIndex, currentSemanticRelationInList->relationDependentIndex, currentSemanticRelationInList->sameReferenceSet);
									GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + STRING_NEWLINE;
									sentenceText = sentenceText + GIA2semanticDependencyRelation;

									foundAtLeastOneRelation = true;
								}
								currentSemanticRelationInList = currentSemanticRelationInList->next;
							}
							sentenceText = sentenceText + STRING_NEWLINE;	//required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
							if(foundAtLeastOneRelation)
							{
								GIAsemanticParserDatabase.prepareSemanticParserCorpusDatabaseFileTextForWriting();
								GIAsemanticParserDatabase.saveTextToCorpusFileString(sentenceText);
								GIAsemanticParserDatabase.writeSemanticParserCorpusFile(firstFeatureInSentenceSubset);
							}
							#endif

							#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
							bool foundAtLeastOneRelation = false;
							int firstWordInTupleIndexRelative = GIAsemanticParserOperations.calculateFirstWordInTupleIndexRelative(firstWordInTupleIndex, firstWordInSentenceSubsetIndex);
							GIArelation* currentSemanticRelationInList = firstRelationInList;
							while(currentSemanticRelationInList->next != NULL)
							{
								int semanticDependencyRelationType = INT_DEFAULT_VALUE;
								bool sameReferenceSet = currentSemanticRelationInList->sameReferenceSet;
								if(SHAREDvars.textInTextArray(currentSemanticRelationInList->relationType, GIA2semanticDependencyRelationNameArray, GIA_SEMANTIC_PARSER_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES, &semanticDependencyRelationType))
								{
									//CHECKTHIS; how to handle these cases (currentSemanticRelationInList->relationGovernorIndex >= GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList)) || (currentSemanticRelationInList->relationDependentIndex >= GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList)))
										//NB GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE can't handle queries at present (or where featureIndex >= FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
									if((currentSemanticRelationInList->relationGovernorIndex == firstWordInTupleIndex) && (currentSemanticRelationInList->relationDependentIndex == secondWordInTupleIndex))
									{
										//write semantic relation data
										bool directionGovernorToDependent = true;
										GIAsemanticParserDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
										foundAtLeastOneRelation = true;	//CHECKTHIS
										GIAfeature* currentFeatureInSentence = firstFeatureInList;
										while(currentFeatureInSentence->next != NULL)
										{
											string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemanticParserPOStype];
											currentFeatureInSentence = currentFeatureInSentence->next;
										}
									}
									else if((currentSemanticRelationInList->relationGovernorIndex == secondWordInTupleIndex) && (currentSemanticRelationInList->relationDependentIndex == firstWordInTupleIndex))
									{
										//write semantic relation data
										bool directionGovernorToDependent = false;
										GIAsemanticParserDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
										foundAtLeastOneRelation = true;	//CHECKTHIS

										GIAfeature* currentFeatureInSentence = firstFeatureInList;
										while(currentFeatureInSentence->next != NULL)
										{
											string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemanticParserPOStype];
											currentFeatureInSentence = currentFeatureInSentence->next;
										}
									}
								}
								else
								{
									cerr << "generateAllPermutationsFromSemanticRelationsFile{} error: illegal semantic relation type; currentSemanticRelationInList->relationType = " << currentSemanticRelationInList->relationType << endl;
									exit(EXIT_ERROR);
								}
								currentSemanticRelationInList = currentSemanticRelationInList->next;
							}
							#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_EXTENSIVE
							if(!foundAtLeastOneRelation)
							{
								GIAsemanticParserDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, SEMANTIC_RELATION_TYPE_NON_EXISTENT, BOOL_IRRELEVANT, BOOL_IRRELEVANT);
							}
							#endif
							#endif
						#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
						}
						#endif
						firstFeatureInSentenceSubset = firstFeatureInSentenceSubset->next;
					}
				#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
				}
				#endif
			#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS
				if(optimisedBasedOnContextualConjunctions)
				{
					delete firstFeatureInSentenceSubsetInitial;	//delete artificially generated optimised sentence subset
				}
			}
			#endif

			secondWordInTupleFeature->next = recordOfFeatureAfterSecondWordInTupleFeature;	//restore temporarily disconnected node at end of sentence subset
			secondWordInTupleFeature = secondWordInTupleFeature->next;
		}
	#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	}
	#endif

	return result;
}

#endif



#endif







