/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: GIAnlg.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t2h 23-July-2013
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
 *******************************************************************************/



#include "GIAnlg.h"
#include "GIAtranslatorDefineGrammar.h"

#ifdef GIA_USE_NLG

NLGSentence::NLGSentence(void)
{
	NLGInputViewText = "";

	next = NULL;
}

NLGSentence * generateLanguageFromEntityNode(GIAentityNode * entityNode, NLGSentence * currentNLGsentence, bool isQueryAnswerContext, int isQueryAnswerContextRound)
{
	NLGSentence * currentNLGsentenceUpdated = currentNLGsentence;
	if(!(entityNode->parsedForLanguageGeneration) && !(entityNode->disabled))
	{
		#ifdef GIA_NLG_DEBUG
		if(entityNode->isSubstance)
		{
			cout << "entityNode = " << entityNode->entityName << " (is substance)" << endl;
		}
		else if(entityNode->isAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;
		}
		else if(entityNode->isCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (is condition)" << endl;
		}
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated instance is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated instance is condition)" << endl;
		}
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << endl;
		}
		//cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		//cout << "\tentityNode->isSubstance = " << entityNode->isSubstance << endl;
		//cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
		//cout << "\tentityNode->hasAssociatedInstanceIsAction = " << entityNode->hasAssociatedInstanceIsAction << endl;		
		#endif


		entityNode->parsedForLanguageGeneration = true;

		#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3a
		if(!(entityNode->sourceAddedInLanguageGeneration))
		{
		#endif
 			bool supportAdditionalLinks = true;
			if(entityNode->isAction)
			{
				if(!isQueryAnswerContext || (isQueryAnswerContextRound == 1))
				{
					#ifdef GIA_NLG_DEBUG
					cout << "\t\t(!isQueryAnswerContext || (isQueryAnswerContextRound == 1))" << endl;
					#endif
					generateThreeEntitySentenceFromEntityNode(entityNode, &(currentNLGsentenceUpdated->NLGInputViewText), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, 1, supportAdditionalLinks);
					#ifdef GIA_USE_NLG2
					NLG2generateNLGinputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));
					#endif
					NLGSentence * newNLGsentence = new NLGSentence();
					currentNLGsentenceUpdated->next = newNLGsentence;
					currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;
				}
			}
			else if(entityNode->isCondition)
			{
				if(!isQueryAnswerContext || (isQueryAnswerContextRound == 2))
				{
					#ifdef GIA_NLG_DEBUG
					cout << "\t\t(!isQueryAnswerContext || (isQueryAnswerContextRound == 2))" << endl;
					#endif
					generateThreeEntitySentenceFromEntityNode(entityNode, &(currentNLGsentenceUpdated->NLGInputViewText), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, 1, supportAdditionalLinks);
					#ifdef GIA_USE_NLG2
					NLG2generateNLGinputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));
					#endif
					NLGSentence * newNLGsentence = new NLGSentence();
					currentNLGsentenceUpdated->next = newNLGsentence;
					currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;
				}
			}
		#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3a
		}
		#endif

		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
			{
				if(nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[i])
				{
					#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3a
					if(!(entityNode->sourceAddedInLanguageGeneration))
					{
					#endif
						if(!isQueryAnswerContext || (isQueryAnswerContextRound == 3))
						{
							generateTwoEntitySentenceFromEntityConnection(entityNode, *connectionIter, &(currentNLGsentenceUpdated->NLGInputViewText), i, 1, false);

							#ifdef GIA_USE_NLG2
							NLG2generateNLGinputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));
							#endif
							NLGSentence * newNLGsentence = new NLGSentence();
							currentNLGsentenceUpdated->next = newNLGsentence;
							currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;
						}
					#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3a
					}
					#endif
				}

				if(!isQueryAnswerContext || (isQueryAnswerContext && ((*connectionIter)->entity->queryAnswerContext) || ((*connectionIter)->entity->isAnswerToQuery)))
				{
					currentNLGsentenceUpdated = generateLanguageFromEntityNode((*connectionIter)->entity, currentNLGsentenceUpdated, isQueryAnswerContext, isQueryAnswerContextRound);
				}
			}
		}

		#ifdef GIA_NLG_DEBUG
		if(entityNode->isSubstance)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is substance)" << endl;
		}
		else if(entityNode->isAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;
		}
		else if(entityNode->isCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is condition)" << endl;
		}
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance is condition)" << endl;
		}
		else if(entityNode->hasAssociatedTime)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << endl;
		}
		#endif

	}
	return currentNLGsentenceUpdated;
}


//startEntityIndex should be 1 for first calcs...
void generateThreeEntitySentenceFromEntityNode(GIAentityNode * entityNode0, string * generatedText, int connectionType1, int connectionType2, int startEntityIndex, bool supportAdditionalLinks)
{
	#ifdef GIA_NLG_DEBUG
	cout << "\ngenerateThreeEntitySentenceFromEntityNode: " << entityNode0->entityName << endl;
	if(supportAdditionalLinks)
	{
		cout << "\tsupportAdditionalLinks" << endl;
	}
	else
	{
		cout << "\t!supportAdditionalLinks" << endl;
	}
	cout << "\tconnectionType1 = " << connectionType1 << endl;
	cout << "\tconnectionType2 = " << connectionType2 << endl;
	#endif

	GIAentityNode * entityNode1 = NULL;
	GIAentityNode * entityNode2 = NULL;
	bool entityNodeAvailableArray[3];
	entityNodeAvailableArray[0] = true;
	entityNodeAvailableArray[1] = false;
	entityNodeAvailableArray[2] = false;
	string determinateArray[3];
	bool inputNodeFound = false;
	bool outputNodeFound = false;
	if(!(entityNode0->entityVectorConnectionsArray[connectionType1]).empty())
	{
		inputNodeFound = true;
	}
	if(!(entityNode0->entityVectorConnectionsArray[connectionType2]).empty())
	{
		outputNodeFound = true;
	}
	if(inputNodeFound && outputNodeFound)
	{
		entityNodeAvailableArray[1] = true;
		entityNodeAvailableArray[2] = true;
		entityNode1 = (entityNode0->entityVectorConnectionsArray[connectionType1]).back()->entity;
		entityNode2 = (entityNode0->entityVectorConnectionsArray[connectionType2]).back()->entity;		
	}
	else if(!inputNodeFound && outputNodeFound)
	{	
		//if((entityNode0->entityVectorConnectionsArray[connectionType2]).back()->entity->isSubjectTemp)	[not possible as isSubjectTemp is not set officially based on NLP relations, but on GIA semantic relationships ie action input/output]
		if((entityNode0->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS]).empty())
		{//added 28 Oct 2012; to handle case "Corn is eaten by.."
			entityNodeAvailableArray[1] = true;
			entityNode1 = (entityNode0->entityVectorConnectionsArray[connectionType2]).back()->entity;	
		}
		else
		{
			entityNodeAvailableArray[2] = true;
			entityNode2 = (entityNode0->entityVectorConnectionsArray[connectionType2]).back()->entity;			
		}
	}
	else if(inputNodeFound && !outputNodeFound)
	{
		entityNodeAvailableArray[1] = true;
		entityNode1 = (entityNode0->entityVectorConnectionsArray[connectionType1]).back()->entity;		
	}

	if(entityNodeAvailableArray[0])
	{
		determinateArray[0] = calcDeterminate(entityNode0);
	}
	if(entityNodeAvailableArray[1])
	{
		determinateArray[1] = calcDeterminate(entityNode1);
	}
	if(entityNodeAvailableArray[2])
	{
		determinateArray[2] = calcDeterminate(entityNode2);
	}

#ifdef GIA_USE_NLG2
	int entityIndex0 = startEntityIndex + 0;
	int entityIndex1 = startEntityIndex + 1;
	int entityIndex2 = startEntityIndex + 2;
	char entityIndex0stringCharstar[10];
	char entityIndex1stringCharstar[10];
	char entityIndex2stringCharstar[10];
	sprintf(entityIndex0stringCharstar, "%d", entityIndex0);
	sprintf(entityIndex1stringCharstar, "%d", entityIndex1);
	sprintf(entityIndex2stringCharstar, "%d", entityIndex2);
	string entityIndex0string = entityIndex0stringCharstar;
	string entityIndex1string = entityIndex1stringCharstar;
	string entityIndex2string = entityIndex2stringCharstar;

	NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);
	if(entityNodeAvailableArray[1])
	{
		NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);
	}
	if(entityNodeAvailableArray[2])
	{
		NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);
	}
#else
	string entityTextExpandedArray[3];
	for(int i=0; i<3; i++)
	{
		entityTextExpandedArray[i] = "";
	}
	entityTextExpandedArray[0] = getWordOrig(entityNode0);
	if(entityNodeAvailableArray[1])
	{
		entityTextExpandedArray[1] = getWordOrig(entityNode1);
	}
	if(entityNodeAvailableArray[2])
	{
		entityTextExpandedArray[2] = getWordOrig(entityNode2);
	}
#endif

	#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS
	if(supportAdditionalLinks)
	{
		//bool entityNodeHasAdditionalConnection[3][NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS];
		//int entityNodeHasAdditionalConnectionIndex[3][NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS];
		GIAentityNode * entityNodeArray[3];
		entityNodeArray[0] = entityNode0;
		entityNodeArray[1] = entityNode1;
		entityNodeArray[2] = entityNode2;
		bool entityNode2HasCondition = false;
		#ifdef GIA_USE_NLG2
		int currentEntityIndexAdditional = entityIndex2 + 1;
		#else
		int currentEntityIndexAdditional = GIA_NLG_INDEX_IRRELEVANT;
		#endif

		for(int i=0; i<NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS; i++)
		{
			for(int entityNodeIndex=0; entityNodeIndex<3; entityNodeIndex++)
			{
				if(entityNodeAvailableArray[entityNodeIndex])
				{
					//entityNodeHasAdditionalConnection[entityNodeIndex][i] = false;
					int vectorConnectionIndex = nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i];
					if((connectionType1 != nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[0]) && (connectionType2 != nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[1]))
					{//do not add double conditions/prepositions...
						#ifdef GIA_NLG_DEBUG
						//cout << "entityNodeArray[entityNodeIndex]->entityName = " << entityNodeArray[entityNodeIndex]->entityName << endl;
						//cout << "entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() = " << entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() << endl;
						#endif

						if(entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() == 1)
						{//entity contains single substance or condition
							GIAentityConnection * entityConnectionAdditional = entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].back();
							GIAentityNode * entityNodeAdditional = entityConnectionAdditional->entity;

							bool passAdditionalExtraRequirements = false;
							if(vectorConnectionIndex == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)
							{
								if(entityNodeAdditional->isSubstanceQuality)
								{
									passAdditionalExtraRequirements = true;
								}
							}
							else
							{
								passAdditionalExtraRequirements = true;
							}

							if(passAdditionalExtraRequirements)
							{
								//entityNodeHasAdditionalConnection[entityNodeIndex][i] = true;
								//entityNodeHasAdditionalConnectionIndex[entityNodeIndex][i] = currentEntityIndexAdditional;
								if(nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i])
								{
									#ifdef GIA_NLG_DEBUG
									//cout << "entityNodeIndex = " << entityNodeIndex << endl;
									//cout << "vectorConnectionIndex = " << vectorConnectionIndex << endl;
									//cout << "entityNodeAdditional->entityName = " << entityNodeAdditional->entityName << endl;
									#endif
									#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3b
									if(!(entityNodeAdditional->sourceAddedInLanguageGeneration))
									{
									#endif
										#ifdef GIA_NLG_DEBUG
										cout << "\t\t(!(entityNodeAdditional->sourceAddedInLanguageGeneration))" << endl;
										#endif
										#ifdef GIA_USE_NLG2
										generateThreeEntitySentenceFromEntityNode(entityNodeAdditional, generatedText, nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i], nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i]+1, currentEntityIndexAdditional, false);
										#else
										generateThreeEntitySentenceFromEntityNode(entityNodeAdditional, &(entityTextExpandedArray[entityNodeIndex]), nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[0], nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[1], currentEntityIndexAdditional, false);
										#endif

										#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3c
										entityNodeArray[entityNodeIndex]->sourceAddedInLanguageGeneration = true;
										#endif
									#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3b
									}
									#endif
								}
								else
								{
									#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3b
									if(!(entityNodeArray[entityNodeIndex]->sourceAddedInLanguageGeneration))
									{
									#endif
										#ifdef GIA_NLG_DEBUG
										cout << "\t\t(!(entityNodeArray[entityNodeIndex]->sourceAddedInLanguageGeneration))" << endl;									
										#endif
										#ifdef GIA_USE_NLG2
										generateTwoEntitySentenceFromEntityConnection(entityNodeArray[entityNodeIndex], entityConnectionAdditional, generatedText, nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i], currentEntityIndexAdditional, true);
										#else
										generateTwoEntitySentenceFromEntityConnection(entityNodeArray[entityNodeIndex], entityConnectionAdditional, &(entityTextExpandedArray[entityNodeIndex]), nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i], currentEntityIndexAdditional, true);
										#endif

										#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3c
										entityNodeArray[entityNodeIndex]->sourceAddedInLanguageGeneration = true;
										#endif
									#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3b
									}
									#endif
								}

								currentEntityIndexAdditional++;
							}
						}
					}
				}
			}
		}
	}
	#endif

#ifdef GIA_USE_NLG2
	string nlgDependencyRelation1 = NLG2generateNLGinputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType1], entityIndex0string, entityIndex1string);
	string nlgDependencyRelation2 = NLG2generateNLGinputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType2], entityIndex0string, entityIndex2string);

	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode(): " << entityNode0->entityName << endl;
	cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
	cout << "nlgDependencyRelation2 = " << nlgDependencyRelation2 << endl;
	#endif
	*generatedText = *generatedText + nlgDependencyRelation1;
	*generatedText = *generatedText + nlgDependencyRelation2;
#else
	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode(): " << entityNode0->entityName << endl;
	cout << "entityTextExpandedArray[0] = " << entityTextExpandedArray[0] << endl;
	cout << "entityTextExpandedArray[1] = " << entityTextExpandedArray[1] << endl;
	cout << "entityTextExpandedArray[2] = " << entityTextExpandedArray[2] << endl;
	#endif

	entityTextExpandedArray[0] = determinateArray[0] + entityTextExpandedArray[0];
	if(supportAdditionalLinks)
	{
		entityTextExpandedArray[1] = determinateArray[1] + entityTextExpandedArray[1];
	}
	entityTextExpandedArray[2] = determinateArray[2] + entityTextExpandedArray[2];

	if(supportAdditionalLinks)
	{//is a condition link; add copular- eg the dog is near the park.
		if((connectionType1 == nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[0]) && (connectionType2 == nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[1]))
		{
			string nlgDefinitionText = determineNLGdefinitionText(entityNode1);	//added 03 August 2012
			entityTextExpandedArray[0] = nlgDefinitionText + NLG_TEXT_SPACE + entityTextExpandedArray[0];
		}
	}
	if(entityNodeAvailableArray[1])
	{
		if(supportAdditionalLinks)
		{
			*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[1];
		}
	}
	*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[0];
	if(entityNodeAvailableArray[2])
	{
		*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[2];
	}
	
	#ifdef GIA_NLG_DEBUG
	cout << "*generatedText = " << *generatedText << endl;
	#endif	

#endif
	/*
	if(supportAdditionalLinks)
	{
	*/
	#ifdef GIA_NLG_DEBUG
	//cout << "generateThreeEntitySentenceFromEntityNode:" << endl;
	#endif
	if(entityNodeAvailableArray[0])
	{
		#ifdef GIA_NLG_DEBUG
		//cout << "entityNode0->sourceReferencedInLanguageGeneration: " << entityNode0->entityName << endl;
		#endif
		entityNode0->sourceReferencedInLanguageGeneration = true;
		#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3_STRINGENT
		if(supportAdditionalLinks)
		{
		#endif
			#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3
			entityNode0->sourceAddedInLanguageGeneration = true;
			#endif
		#ifdef NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3_STRINGENT
		}
		#endif
	}
	if(entityNodeAvailableArray[1])
	{
		#ifdef GIA_NLG_DEBUG
		//cout << "entityNode1->sourceReferencedInLanguageGeneration: " << entityNode1->entityName << endl;
		#endif
		entityNode1->sourceReferencedInLanguageGeneration = true;
	}
	if(entityNodeAvailableArray[2])
	{
		#ifdef GIA_NLG_DEBUG
		//cout << "entityNode2->sourceReferencedInLanguageGeneration: " << entityNode2->entityName << endl;
		#endif
		entityNode2->sourceReferencedInLanguageGeneration = true;
	}
	/*
	}
	*/

	#ifdef GIA_NLG_DEBUG
	cout << "Exiting: generateThreeEntitySentenceFromEntityNode: " << entityNode0->entityName << endl;
	#endif
}

void generateTwoEntitySentenceFromEntityConnection(GIAentityNode * entityNode1, GIAentityConnection * entityConnection, string * generatedText, int connectionType, int startEntityIndex, bool additionalLink)
{
	/*
	if(!(entityConnection->parsedForLanguageGeneration))	//&& !(entityConnection->disabled)
	{
	*/
	
	GIAentityNode * entityNode2 = entityConnection->entity;

	#ifdef GIA_NLG_DEBUG
	cout << "generateTwoEntitySentenceFromEntityConnection: " << entityNode1->entityName << endl;
	#endif

#ifdef GIA_USE_NLG2
	int entityIndex1 = startEntityIndex + 1;
	int entityIndex2 = startEntityIndex + 2;
	int entityIndex0 = startEntityIndex + 0;
	char entityIndex1stringCharstar[10];
	char entityIndex2stringCharstar[10];
	char entityIndex0stringCharstar[10];
	sprintf(entityIndex1stringCharstar, "%d", entityIndex1);
	sprintf(entityIndex2stringCharstar, "%d", entityIndex2);
	sprintf(entityIndex0stringCharstar, "%d", entityIndex0);
	string entityIndex1string = entityIndex1stringCharstar;
	string entityIndex2string = entityIndex2stringCharstar;
	string entityIndex0string = entityIndex0stringCharstar;

	bool generateTwoDependencyRelations = false;
	string nlgDependencyRelationType1 = "";
	string nlgDependencyRelationType2 = "";
	string nlgDependencyRelationSharedArgument = "";
#else
	string entityTextExpandedArray[3];
	for(int i=0; i<3; i++)
	{
		entityTextExpandedArray[i] = "";
	}
	bool generateLinkingWord = false;
	string linkingWord = "";
	bool prepend = false;
	entityTextExpandedArray[0] = "";
	entityTextExpandedArray[1] = getWordOrig(entityNode1);
	entityTextExpandedArray[2] = getWordOrig(entityNode2);
#endif



	if(entityNode2->isAction)
	{
		#ifdef GIA_NLG_DEBUG
		//cout << "warning: generateTwoEntitySentenceFromEntityConnection && (entityNode2->isAction)" << endl;
		#endif
	}
	else if(entityNode2->isCondition)
	{
		#ifdef GIA_NLG_DEBUG
		//cout << "warning: generateTwoEntitySentenceFromEntityConnection && (entityNode2->isCondition)" << endl;
		#endif
	}

	if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)
	{
		/*
		if(entityNode2->isSubstance)
		{
		*/
		#ifdef NLG_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
		bool isSubstanceQuality = false;
		if(entityNode2->isSubstanceQuality)
		{
			isSubstanceQuality = true;
			if(entityNode1->isAction)
			{
				#ifdef GIA_USE_NLG2
				nlgDependencyRelationType1 = RELATION_TYPE_ADJECTIVE_ADVMOD
				#else
				#endif
			}
			else
			{
				#ifdef GIA_USE_NLG2
				nlgDependencyRelationType1 = RELATION_TYPE_ADJECTIVE_AMOD;		//or RELATION_TYPE_ADJECTIVE_PREDADJ?
				#else
				prepend = true;
				#endif
			}
		}
		if(!isSubstanceQuality)
		{
		#endif
			//eg possessive and/or tom has a bike
			#ifdef GIA_USE_NLG2
			#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS
			generateTwoDependencyRelations = true;
			nlgDependencyRelationType1 = RELATION_TYPE_SUBJECT;
			nlgDependencyRelationType2 = RELATION_TYPE_OBJECT;
			nlgDependencyRelationSharedArgument = RELATION_ENTITY_HAVE;
			#else
			nlgDependencyRelationType1 = RELATION_TYPE_POSSESSIVE;
			#endif
			#else
			generateLinkingWord = true;
			linkingWord = determineNLGpossessionText(entityNode1);	//added 03 August 2012
			#endif
		#ifdef NLG_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
		}
		#endif
		/*
		}
		else
		{
			if(entityNode2->isConcept)
			{//isConcept
				#ifdef GIA_NLG_DEBUG
				//cout << "warning: generateTwoEntitySentenceFromEntityConnection && (entityNode2->isConcept) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)" << endl;
				#endif
			}
		}
		*/
	}
	else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS)
	{
		#ifdef GIA_USE_NLG2
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS
		generateTwoDependencyRelations = true;
		nlgDependencyRelationType1 = RELATION_TYPE_SUBJECT;
		nlgDependencyRelationType2 = RELATION_TYPE_OBJECT;
		nlgDependencyRelationSharedArgument = RELATION_ENTITY_BE;
		#else
		nlgDependencyRelationType1 = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		#endif
		#else
		generateLinkingWord = true;
		linkingWord = determineNLGdefinitionText(entityNode1);	//added 03 August 2012
		#endif
	}

#ifdef GIA_USE_NLG2
	if(generateTwoDependencyRelations)
	{
		GIAentityNode * entityNode0 = new GIAentityNode();
		entityNode0->entityName = nlgDependencyRelationSharedArgument;
		entityNode0->isAction = true;	//NB "have"/"be" are interpreted as actions/verbs by NLP/stanford/relex (but reduced by GIA)
		NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);
		NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);
		NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);
		string nlgDependencyRelation1 = NLG2generateNLGinputViewLine(nlgDependencyRelationType1, entityIndex0string, entityIndex1string);
		string nlgDependencyRelation2 = NLG2generateNLGinputViewLine(nlgDependencyRelationType2, entityIndex0string, entityIndex2string);

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;
		cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
		cout << "nlgDependencyRelation2 = " << nlgDependencyRelation2 << endl;
		#endif
		*generatedText = *generatedText + nlgDependencyRelation1;
		*generatedText = *generatedText + nlgDependencyRelation2;

		delete entityNode0;
	}
	else
	{
		NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);
		NLG2generateNLGinputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);
		string nlgDependencyRelation1 = NLG2generateNLGinputViewLine(nlgDependencyRelationType1, entityIndex1string, entityIndex2string);

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;
		cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
		#endif
		*generatedText = *generatedText + nlgDependencyRelation1;
	}
#else

	#ifdef GIA_NLG_DEBUG
	cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;
	cout << "entityTextExpandedArray[0] = " << entityTextExpandedArray[0] << endl;
	cout << "entityTextExpandedArray[1] = " << entityTextExpandedArray[1] << endl;
	cout << "entityTextExpandedArray[2] = " << entityTextExpandedArray[2] << endl;
	#endif

	if(prepend)
	{
		//assert !generateLinkingWord
		if(additionalLink)
		{
			*generatedText = entityTextExpandedArray[2] + NLG_TEXT_SPACE + *generatedText;
		}
		else
		{
			addDeterminate(entityNode1, &(entityTextExpandedArray[1]));
			//*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[2] + NLG_TEXT_SPACE + entityTextExpandedArray[1];
			string nlgDefinitionText = determineNLGdefinitionText(entityNode1);	//added 03 August 2012
			*generatedText = *generatedText + entityTextExpandedArray[1] + NLG_TEXT_SPACE + nlgDefinitionText + NLG_TEXT_SPACE + entityTextExpandedArray[2];
		}
	}
	else
	{


		if(!additionalLink)
		{
			addDeterminate(entityNode1, &(entityTextExpandedArray[1]));
			*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[1];
		}

		if(generateLinkingWord)
		{
			GIAentityNode * entityNode0 = new GIAentityNode();
			entityNode0->entityName = linkingWord;
			entityNode0->isAction = true;	//NB "have"/"be" are interpreted as actions/verbs by NLP/stanford/relex (but reduced by GIA)
			entityTextExpandedArray[0] = linkingWord;

			addDeterminate(entityNode0, &(entityTextExpandedArray[0]));
			*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[0];

			delete entityNode0;
		}

		addDeterminate(entityNode2, &(entityTextExpandedArray[2]));
		*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[2];
	}

	#ifdef GIA_NLG_DEBUG
	cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;
	cout << "generatedText = " << *generatedText << endl;
	#endif
#endif

	/*
	if(!additionalLink)
	{
	*/
	#ifdef GIA_NLG_DEBUG
	//cout << "generateTwoEntitySentenceFromEntityNode:" << endl;
	//cout << "entityNode1->sourceReferencedInLanguageGeneration: " << entityNode1->entityName << endl;
	//cout << "entityNode2->sourceReferencedInLanguageGeneration: " << entityNode2->entityName << endl;
	#endif
	entityNode1->sourceReferencedInLanguageGeneration = true;
	entityNode2->sourceReferencedInLanguageGeneration = true;

	#ifdef NLG_TWO_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3_STRINGENT
	if(!additionalLink)
	{
	#endif
		#ifdef NLG_TWO_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3
		entityNode1->sourceAddedInLanguageGeneration = true;
		#endif
	#ifdef NLG_TWO_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3_STRINGENT
	}
	#endif
	/*
	}
	*/

	#ifdef GIA_NLG_DEBUG
	cout << "Exiting: generateTwoEntitySentenceFromEntityConnection" << endl;
	#endif

	/*
	}
	*/
}

#ifdef GIA_USE_NLG2
void NLG2generateNLGinputViewFeatureTagsGenericPerSentence(string * generatedNLGinputViewTags)
{
	string NLGInputViewFeatureTagPosDeterminateDefinite = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_DEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);
	string NLGInputViewFeatureTagPosDeterminateIndefinite = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_INDEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);
	string NLGInputViewFeatureTagPosPunctuationFullstop = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_FULLSTOP, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FULLSTOP);

	#ifdef GIA_NLG_DEBUG
	cout << "NLG2generateNLGinputViewFeatureTagsGenericPerSentence():" << endl;
	cout << "NLGInputViewFeatureTagPosDeterminateDefinite = " << NLGInputViewFeatureTagPosDeterminateDefinite << endl;
	cout << "NLGInputViewFeatureTagPosDeterminateIndefinite = " << NLGInputViewFeatureTagPosDeterminateIndefinite << endl;
	cout << "NLGInputViewFeatureTagPosPunctuationFullstop = " << NLGInputViewFeatureTagPosPunctuationFullstop << endl;
	#endif

	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagPosDeterminateDefinite;
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagPosDeterminateIndefinite;
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagPosPunctuationFullstop;
}

void NLG2generateNLGinputViewFeatureTagsFromEntityNode(GIAentityNode * entityNode, int entityIndex, string * generatedNLGinputViewTags)
{
	char entityIndexStringCharstar[10];
	sprintf(entityIndexStringCharstar, "%d", entityIndex);
	string entityIndexString = entityIndexStringCharstar;

	//lemma
	string NLGInputViewFeatureTagLemma = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_LEMMA, entityIndexString, getWordOrig(entityNode));

	//tense
	//string wordName = entityNode->entityName;
	string NLGInputViewFeatureTagTense = "";
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		GIAtimeConditionNode * timeCondition = entityNode->timeConditionNode;

		if(timeCondition->tenseOnlyTimeCondition)
		{
			string tenseString = "";
			if(timeCondition->tense != GRAMMATICAL_TENSE_UNDEFINED)
			{
				//GRAMMATICAL_TENSE_PRESENT, GRAMMATICAL_TENSE_PAST, GRAMMATICAL_TENSE_FUTURE
				tenseString = grammaticalTenseNameArray[timeCondition->tense];
			}
			if(timeCondition->isProgressive)
			{
				tenseString = tenseString + GRAMMATICAL_TENSE_CONCATONATOR_RELEX + grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE];
			}
			/*
			if(timeCondition->isImperative)
			{
				tenseString = tenseString + GRAMMATICAL_TENSE_CONCATONATOR_RELEX + grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE];
			}
			*/

			//eg tense(decide, past)
			string NLGInputViewFeatureTagTense = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_TENSE, entityIndexString, tenseString);
		}
		else
		{
			//wordName = wordName + "(" + timeCondition->conditionName + ")";
		}
	}

	//noun_number
	string nounNumberString = "";
	string NLGInputViewFeatureTagNounNumber = "";
	if((entityNode->grammaticalNumber != GRAMMATICAL_NUMBER_UNDEFINED) && (entityNode->grammaticalNumber != GRAMMATICAL_NUMBER_UNCOUNTABLE))
	{
		nounNumberString = grammaticalNumberNameArray[entityNode->grammaticalNumber];
		NLGInputViewFeatureTagNounNumber = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_NOUNNUMBER, entityIndexString, nounNumberString);
	}

	//pos + inflection
	string inflectionString = "";
	string posString = "";
	bool isDefinite = false;
	if(entityNode->isAction)
	{
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_VERB];
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_VERB];
	}
	else if(entityNode->isCondition)
	{
		//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_PREP];	//no inflection for prepositions
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_PREP];
	}
	else if((entityNode->isSubstance) && !(entityNode->isSubstanceConcept))
	{
		#ifdef NLG_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
		bool isSubstanceQuality = false;
		if(entityNode->isSubstanceQuality)
		{
			isSubstanceQuality = true;
			bool substanceOwnerIsAction = false;
			if(!(entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES].empty()))
			{
				GIAentityNode * substanceOwner = entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES].back()->entity;
				if(substanceOwner->isAction)
				{
					substanceOwnerIsAction = true;
					//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADV];	//no inflection for adjectives?
					posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADV];
				}
			}
			if(!substanceOwnerIsAction)
			{
				//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADJ];	//no inflection for adverbs?
				posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADJ];
			}
		}
		if(!isSubstanceQuality)
		{
		#endif
			inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];
			posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
			if(!(entityNode->sourceReferencedInLanguageGeneration))
			{
				isDefinite = true;
			}
		#ifdef NLG_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
		}
		#endif
	}
	else if(entityNode->isConcept || entityNode->isSubstanceConcept)
	{//isConcept
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
	}
	string NLGInputViewFeatureTagInflection = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_INFLECTION, entityIndexString, inflectionString);
	string NLGInputViewFeatureTagPos = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, entityIndexString, posString);

	//flag definite
	string NLGInputViewFeatureTagFlagDefinite = "";
	if(isDefinite)
	{
		NLGInputViewFeatureTagFlagDefinite = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_DEFINITE, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_DEFINITE);
	}

	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	//gender
	bool isPerson = false;
	bool hasGender = false;
	string genderString = "";
	string NLGInputViewFeatureTagFlagPerson = "";
	string NLGInputViewFeatureTagGender = "";
	if(entityNode->grammaticalGenderTemp != GRAMMATICAL_GENDER_UNDEFINED)
	{
		//relex compatible only
		if(entityNode->grammaticalGenderTemp == GRAMMATICAL_GENDER_PERSON)
		{
			isPerson = true;
		}
		else
		{
			isPerson = true;
			genderString = grammaticalGenderNameArray[entityNode->grammaticalGenderTemp];
			hasGender = true;
		}
	}
	else
	{
		//stanford/relex compatible
		//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford()	[see GIAtranslatorDefineGrammar.cpp]
		if(entityNode->grammaticalRelexPersonOrStanfordProperNounTemp)
		{
			isPerson = true;
		}

	}
	if(hasGender)
	{
		NLGInputViewFeatureTagGender = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_GENDER, entityIndexString, genderString);
	}
	else if(isPerson)
	{
		NLGInputViewFeatureTagFlagPerson = NLG2generateNLGinputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_PERSON, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_PERSON);
	}
	#endif


	#ifdef GIA_NLG_DEBUG
	cout << "NLG2generateNLGinputViewFeatureTagsFromEntityNode():" << endl;
	cout << "NLGInputViewFeatureTagLemma = " << NLGInputViewFeatureTagLemma << endl;
	cout << "NLGInputViewFeatureTagFlagDefinite = " << NLGInputViewFeatureTagFlagDefinite << endl;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	cout << "NLGInputViewFeatureTagGender = " << NLGInputViewFeatureTagGender << endl;
	#endif
	cout << "NLGInputViewFeatureTagTense = " << NLGInputViewFeatureTagTense << endl;
	cout << "NLGInputViewFeatureTagNounNumber = " << NLGInputViewFeatureTagNounNumber << endl;
	cout << "NLGInputViewFeatureTagInflection = " << NLGInputViewFeatureTagInflection << endl;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	cout << "NLGInputViewFeatureTagFlagPerson = " << NLGInputViewFeatureTagFlagPerson << endl;
	#endif
	cout << "NLGInputViewFeatureTagPos = " << NLGInputViewFeatureTagPos << endl;
	#endif
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagLemma;
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagFlagDefinite;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagGender;
	#endif
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagTense;
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagNounNumber;
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagInflection;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagFlagPerson;
	#endif
	*generatedNLGinputViewTags = *generatedNLGinputViewTags + NLGInputViewFeatureTagPos;

}




string NLG2generateNLGinputViewLine(string type, string governor, string dependent)
{
	#ifdef GIA_NLG_DEBUG_MANUALLY_HARDCODE_INTO_NLG2
	string NLGInputViewLine = "\"" + type + "(" + governor + ", " + dependent + ")" + "\\n\" +" + "\n";
	#else
	string NLGInputViewLine = type + "(" + governor + ", " + dependent + ")" + "\n";
	#endif
	return NLGInputViewLine;
}

#else


void addDeterminate(GIAentityNode * entityNode, string * entityTextExpanded)
{
	string determinate = calcDeterminate(entityNode);
	*entityTextExpanded = determinate + *entityTextExpanded;
}

string calcDeterminate(GIAentityNode * entityNode)
{

	//first letter is vowel		//added 03 August 2012
	bool firstLetterIsVowel = false;
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	string wordOrig = "";
	if(entityNode->wordOrig != "")
	{
		wordOrig = entityNode->wordOrig;		//IMPORTANT; due to bug in nlg2, nlg2 currently requires the original word, not the lemma
	}
	else
	{
		wordOrig = entityNode->entityName;
	}
	#else
	wordOrig = generateMorphology(entityNode);
	#endif
	for(int i=0; i<NLG_NUMBER_OF_VOWELS; i++)
	{
		if(tolower(wordOrig[0]) == vowelArray[i])
		{
		 	firstLetterIsVowel = true;
		}
	}

	//noun_number
	bool isPlural = false;	//added 03 August 2012
	if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
	{
		isPlural = true;
	}

	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	//gender
	bool isPerson = false;
	bool hasGender = false;
	string genderString = "";
	if(entityNode->grammaticalGenderTemp != GRAMMATICAL_GENDER_UNDEFINED)
	{
		//relex compatible only
		if(entityNode->grammaticalGenderTemp == GRAMMATICAL_GENDER_PERSON)
		{
			isPerson = true;
		}
		else
		{
			isPerson = true;
			genderString = grammaticalGenderNameArray[entityNode->grammaticalGenderTemp];
			hasGender = true;
		}
	}
	else
	{
		//stanford/relex compatible
		//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford()	[see GIAtranslatorDefineGrammar.cpp]
		if(entityNode->grammaticalRelexPersonOrStanfordProperNounTemp)
		{
			isPerson = true;
		}

	}

	bool isPronoun = false;
	if(entityNode->grammaticalPronounTemp == GRAMMATICAL_PRONOUN)
	{
		isPronoun = true;
	}
	#endif

	//determinate
	bool addDeterminate = false;
	string determinate = "";
	if(entityNode->isAction)
	{

	}
	else if(entityNode->isCondition)
	{

	}
	else if((entityNode->isSubstance) && !(entityNode->isSubstanceConcept))
	{
		if(!(entityNode->isSubstanceQuality))
		{
			if(entityNode->sourceReferencedInLanguageGeneration)
			{
				determinate = GRAMMATICAL_DETERMINER_DEFINITE;	//the
				addDeterminate = true;
			}
			else
			{	if(!(entityNode->hasQuantity))
				{
					if(isPlural)	//added 03 August 2012
					{
						determinate = GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL;	//some
					}
					else
					{
						if(firstLetterIsVowel)
						{
							determinate = GRAMMATICAL_DETERMINER_INDEFINITE_FIRST_LETTER_VOWEL;	//an
						}
						else
						{
							determinate = GRAMMATICAL_DETERMINER_INDEFINITE;	//a
						}
					}
					addDeterminate = true;
				}
			}
		}
	}
	else if((entityNode->isConcept) || (entityNode->isSubstanceConcept))
	{//isConcept

	}

	//time
	#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
	bool isTime = false;
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)	//added 4 August 2012
	{
		if(entityNode->timeConditionNode != NULL)
		{
			if(!(entityNode->timeConditionNode->tenseOnlyTimeCondition))
			{
				isTime = true;
			}
		}
	}
	#endif

	string determinateFinal = "";
	if(addDeterminate)
	{
		#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
		if(!isPerson)
		{
			if(!isPronoun)
			{
		#endif
				#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
				if(!isTime)
				{
				#endif

					determinateFinal = determinate + NLG_TEXT_SPACE;
				#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
				}
				#endif
		#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
			}
		}
		#endif
	}
	return determinateFinal;
}


#endif

string getWordOrig(GIAentityNode * entityNode)
{
	string wordOrig = "";
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	if(entityNode->wordOrig != "")
	{
		wordOrig = entityNode->wordOrig;		//IMPORTANT; due to bug in nlg2, nlg2 currently requires the original word, not the lemma
	}
	else
	{
		wordOrig = entityNode->entityName;
	}
	#else
	wordOrig = generateMorphology(entityNode);
	#endif

	//prepend quantity/negative indicator - added 25 July 2012
	if(entityNode->hasQuantity)
	{
		string quantityNumberStringTemp;
		if(entityNode->isQuery)
		{
			quantityNumberStringTemp = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
		}
		else
		{
			quantityNumberStringTemp = printQuantityNumberString(entityNode);
		}
		wordOrig = quantityNumberStringTemp + " " + wordOrig;

	}
	else if(entityNode->negative)
	{
		wordOrig = string(RELATION_TYPE_NEGATIVE_CONTEXT_1) + " " + wordOrig;
	}
	else if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)	//added 4 August 2012
	{
		if(entityNode->timeConditionNode != NULL)
		{
			if(!(entityNode->timeConditionNode->tenseOnlyTimeCondition))
			{
				wordOrig = entityNode->timeConditionNode->conditionName;
			}
		}
	}
	else
	{

	}

	return wordOrig;
}


string determineNLGdefinitionText(GIAentityNode * entityNode)
{
	string nlgDefinitionText = "";
	bool isPlural = false;
	if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
	{
		isPlural = true;
	}
	if(isPlural)
	{
		nlgDefinitionText = NLG_DEFINITION_TEXT_PLURAL;
	}
	else
	{
		nlgDefinitionText = NLG_DEFINITION_TEXT;
	}
	return nlgDefinitionText;
}

string determineNLGpossessionText(GIAentityNode * entityNode)
{
	string nlgPossessionText = "";
	bool isPlural = false;
	if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
	{
		isPlural = true;
	}
	if(isPlural)
	{
		nlgPossessionText = NLG_POSSESSIVE_TEXT_PLURAL;
	}
	else
	{
		nlgPossessionText = NLG_POSSESSIVE_TEXT;
	}
	return nlgPossessionText;
}


#endif

