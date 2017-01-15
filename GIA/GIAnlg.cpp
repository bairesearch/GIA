/*******************************************************************************
 *
 * File Name: GIAnlg.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n4a 21-July-2012
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
 *******************************************************************************/



#include "GIAnlg.h"

void generateLanguageFromEntityNode(GIAEntityNode * entityNode, string * generatedText)
{
	if(!(entityNode->parsedForLanguageGeneration) && !(entityNode->disabled))
	{
		#ifdef GIA_NLG_DEBUG
		if(entityNode->isProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (is property)" << endl;
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
			cout << "entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << endl;
		}
		#endif



		//cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		//cout << "\tentityNode->isProperty = " << entityNode->isProperty << endl;
		//cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
		//cout << "\tentityNode->hasAssociatedInstanceIsAction = " << entityNode->hasAssociatedInstanceIsAction << endl;


		entityNode->parsedForLanguageGeneration = true;

		if(entityNode->isAction)
		{
			generateThreeEntitySentenceFromEntityNode(entityNode, generatedText, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT);
		}
		else if(entityNode->isCondition)
		{
			generateThreeEntitySentenceFromEntityNode(entityNode, generatedText, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT);
		}	
				
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
			{
				if(nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[i])
				{
					generateTwoEntitySentenceFromEntityConnection(entityNode, *connectionIter, generatedText, i);
				}
								
				generateLanguageFromEntityNode((*connectionIter)->entity, generatedText);
			}
		}

		//cout << "a8" << endl;

		#ifdef GIA_NLG_DEBUG
		if(entityNode->isProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is property)" << endl;
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
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
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
	//cout << "a0c" << endl;
}


void generateThreeEntitySentenceFromEntityNode(GIAEntityNode * entityNode, string * generatedText, int connectionType1, int connectionType2)
{
	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode: ";
	#endif
	
	GIAEntityNode * entityNode1 = (entityNode->entityVectorConnectionsArray[connectionType1]).back()->entity;
	GIAEntityNode * entityNode2 = (entityNode->entityVectorConnectionsArray[connectionType2]).back()->entity;

	generateNLGInputViewFeatureTagsFromEntityNode(entityNode, 1, generatedText);	
	generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, 2, generatedText);	
	generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, 3, generatedText);

	nlgDependencyRelation1 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType1], "1", "2");										
	nlgDependencyRelation2 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType2], "1", "3");										

	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode: Exiting";
	#endif
}

void generateTwoEntitySentenceFromEntityConnection(GIAEntityNode * entityNode, GIAEntityConnection * entityConnection, string * generatedText, int connectionType)
{
	if(!(entityConnection->parsedForLanguageGeneration))	//&& !(entityConnection->disabled)
	{	
		GIAEntityNode * entityNode1 = entityConnection->entity;

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection: ";
		if(entityNode1->isProperty)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (is property)" << endl;
		}
		else if(entityNode1->isAction)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (is action)" << endl;
		}
		else if(entityNode1->isCondition)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedInstance)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsAction)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedTime)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode1 = " << entityNode1->entityName << endl;
		}
		#endif

		
		bool generateTwoDependencyRelations = false;
		string nlgDependencyRelationType = "";	
		string nlgDependencyRelationType1 = "";	
		string nlgDependencyRelationSharedArgument = "";
		
		if(entityNode->isAction)
		{
			cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode->isAction)" << endl;
		}	
		else if(entityNode->isCondition)
		{
			cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode->isCondition)" << endl;
		}
				
		if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)
		{			
			if(entityNode->isProperty) 
			{
				#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
				bool hasQuality = false;
				if(entityNode->hasQuality)
				{
					hasQuality = true;
					bool propertyOwnerIsAction = false;
					if(!(entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->empty()))
					{
						GIAEntityNode * propertyOwner = entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->back()->entity;
						if(propertyOwner->isAction)
						{
							propertyOwnerIsAction = true;
							nlgDependencyRelationType = RELATION_TYPE_ADJECTIVE_ADVMOD;
						}
					}
					if(!propertyOwnerIsAction)
					{
						nlgDependencyRelationType = RELATION_TYPE_ADJECTIVE_AMOD;		//or RELATION_TYPE_ADJECTIVE_PREDADJ?	
					}
				}
				if(!hasQuality)
				{
				#endif
					//eg possessive and/or tom has a bike	
					#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS				
					generateTwoDependencyRelations = true;
					nlgDependencyRelationType = RELATION_TYPE_SUBJECT;
					nlgDependencyRelationType1 = RELATION_TYPE_OBJECT;
					nlgDependencyRelationSharedArgument = RELATION_ENTITY_HAVE;
					#else
					nlgDependencyRelationType = RELATION_TYPE_POSSESSIVE;
					#endif
				#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
				}
				#endif
			}	
			else
			{//isConcept
				cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode->isConcept) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)" << endl;
				nlgDependencyRelationType = RELATION_TYPE_SUBJECT;
			}
		}
		else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS)
		{
			#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS
			generateTwoDependencyRelations = true;
			nlgDependencyRelationType = RELATION_TYPE_SUBJECT;
			nlgDependencyRelationType1 = RELATION_TYPE_OBJECT;
			nlgDependencyRelationSharedArgument = RELATION_ENTITY_BE;		
			#else
			nlgDependencyRelationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
			#endif
		}
		
		if(generateTwoDependencyRelations)
		{
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode, 1, generatedText);
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, 2, generatedText);		
			nlgDependencyRelation1 = generateNLGInputViewLine(nlgDependencyRelationType, "1", "2");										
			nlgDependencyRelation2 = generateNLGInputViewLine(nlgDependencyRelationType1, "1", "3");											
		}
		else
		{
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode, 1, generatedText);
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, 2, generatedText);			
			nlgDependencyRelation1 = generateNLGInputViewLine(nlgDependencyRelationType, "1", "2");	
		}

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection: ";
		if(entityNode1->isProperty)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (is property)" << endl;
		}
		else if(entityNode1->isAction)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (is action)" << endl;
		}
		else if(entityNode1->isCondition)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated property)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedTime)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << endl;
		}
		#endif		
		
	}
}

void generateRelexFeatureTagsGenericPerSentence(string * generatedRelexTags)
{
	string relexFeatureTagPosDeterminateDefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_DEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										
	string relexFeatureTagPosDeterminateIndefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_INDEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										

	//#ifdef GIA_NLG_DEBUG
	cout << "relexFeatureTagPosDeterminateDefinite = " << relexFeatureTagPosDeterminateDefinite << endl;
	cout << "relexFeatureTagPosDeterminateIndefinite = " << relexFeatureTagPosDeterminateIndefinite << endl;
	//#endif
		
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagPosDeterminateDefinite;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagPosDeterminateIndefinite;
}

void generateNLGInputViewFeatureTagsFromEntityNode(GIAEntityNode * entityNode, int entityIndex, string * generatedRelexTags)
{
	char entityIndexStringCharstar[10];
	sprintf(entityIndexStringCharstar, "%d", entityIndex);
	string entityIndexString = entityIndexStringCharstar;
	
	//lemma
	string relexFeatureTagLemma = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_LEMMA, entityIndexString, entityNode->name);									
		
	//tense
	//string wordName = entityNode->entityName;
	string relexFeatureTagTense = "";
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		GIATimeConditionNode * timeCondition = entityNode->timeConditionNode;

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
			string relexFeatureTagTense = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_TENSE, entityIndexString, tenseString);							
		}
		else
		{
			//wordName = wordName + "(" + timeCondition->conditionName + ")";
		}	
	}

	//noun_number
	string nounNumberString = "";
	if((entityNode->grammaticalNumber != grammaticalNumberNameArray[MEASURE_TYPE_UNKNOWN]) && (entityNode->grammaticalNumber != grammaticalNumberNameArray[MEASURE_TYPE_UNDEFINED]))
	{
		nounNumberString = grammaticalNumberNameArray[entityNode->grammaticalNumber];
	}
	string relexFeatureTagNounNumber = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_NOUNNUMBER, entityIndexString, nounNumberString);										


	//pos + inflection
	string posString = "";
	string inflectionString = "";
	bool isDefinite = false;	
	if(entityNode->isAction)
	{
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_VERB];
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_VERB];
	}	
	else if(entityNode->isCondition)
	{
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_PREP];	
		//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_PREP];	//no inflection for prepositions
	}
	else if(entityNode->isProperty) 
	{
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
		bool hasQuality = false;
		if(entityNode->hasQuality)
		{
			hasQuality = true;
			bool propertyOwnerIsAction = false;
			if(!(entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->empty()))
			{
				GIAEntityNode * propertyOwner = entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->back()->entity;
				if(propertyOwner->isAction)
				{
					propertyOwnerIsAction = true;
					posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADV];
					//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADV];	//no inflection for adjectives?
				}
			}
			if(!propertyOwnerIsAction)
			{
				posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADJ];
				//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADJ];	//no inflection for adverbs?
			}
		}
		if(!hasQuality)
		{
		#endif
			posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
			inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];
			isDefinite = true;
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
		}
		#endif
	}	
	else
	{//isConcept
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];
	}
	string relexFeatureTagPos = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, entityIndexString, posString);
	string relexFeatureTagInflection = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_INFLECTION, entityIndexString, inflectionString);
	
	//flag definite
	string relexFeatureTagFlagDefinite = "";
	if(isDefinite)
	{
		relexFeatureTagFlagDefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_DEFINITE, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_DEFINITE);
	}
	
	/*
	//gender
	bool isPerson = false;
	string genderString = "";	
	if(entityNode->grammaticalGenderTemp != grammaticalGenderNameArray[GRAMMATICAL_GENDER_UNDEFINED])
	{
		if(entityNode->grammaticalGenderTemp == GRAMMATICAL_GENDER_PERSON)
		{
			isPerson = true;
		}
		else
		{
			genderString = grammaticalGenderNameArray[entityNode->grammaticalGenderTemp];
		}
	}
	string relexFeatureTagGender = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_GENDER, entityIndexString, genderString);										
	
	//flag person
	if(isPerson)
	{
		relexFeatureTagFlagPerson = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_PERSON, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_PERSON);	
	}
	*/
	
	
	//#ifdef GIA_NLG_DEBUG
	cout << "relexFeatureTagLemma = " << relexFeatureTagLemma << endl;
	cout << "relexFeatureTagTense = " << relexFeatureTagTense << endl;
	cout << "relexFeatureTagNounNumber = " << relexFeatureTagNounNumber << endl;
	cout << "relexFeatureTagPos = " << relexFeatureTagPos << endl;	
	cout << "relexFeatureTagInflection = " << relexFeatureTagInflection << endl;			
	cout << "relexFeatureTagFlagDefinite = " << relexFeatureTagFlagDefinite << endl;	
	//cout << "relexFeatureTagGender = " << relexFeatureTagGender << endl;		
	//cout << "relexFeatureTagFlagPerson = " << relexFeatureTagFlagPerson << endl;																			
	//#endif
	
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagLemma;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagTense;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagNounNumber;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagPos;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagInflection;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagFlagDefinite;
	//*generatedRelexTags = *generatedRelexTags + relexFeatureTagGender;
	//*generatedRelexTags = *generatedRelexTags + relexFeatureTagFlagPerson;	
	
}	
		



string generateNLGInputViewLine(string type, string governor, string dependent)
{
	string NLGInputViewLine = type + "(" + governor + ", " + dependent + ")" + "\n";
	return NLGInputViewLine;
}
