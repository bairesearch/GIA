 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsynRelTranslatorLinkEntitiesDynamic.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SYN_REL_TRANSLATOR_LINK_ENTITIES_DYNAMIC
#define HEADER_GIA_SYN_REL_TRANSLATOR_LINK_ENTITIES_DYNAMIC

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
#include "GIAsemRelTranslatorOperations.hpp"
#endif

#ifndef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_GENERALISE_ACTION_TYPES
#define GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ACTION_TYPES_NUMBER_OF_TYPES 2
static string dynamicallyLinkFromConditionsActionTypesArray[GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ACTION_TYPES_NUMBER_OF_TYPES] = {"move", "copy"};	//must be synced with !GIA_STANFORD_PARSER_CONNECT_PREPOSITIONS_TO_ACTION_RATHER_THAN_ACTION_OBJECT_GENERALISE_ACTION_TYPES:redistributeStanfordRelationsConnectPrepositionsToActionRatherThanActionObject xml code
#endif

class GIAsynRelTranslatorLinkEntitiesDynamicClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsentenceClassClass GIAsentenceClass;
	#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	#endif
	private: GIAentityNodeClassClass GIAentityNodeClass;
	public: void linkEntitiesDynamic(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
		private: void linkEntitiesDynamicPrenominalModifierOfNoun(GIAtranslatorVariablesClass* translatorVariables);
			private: bool linkEntitiesDynamicPrenominalModifierOfNounDirection(const GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, int relationTypeIndex, const bool direction);
	#endif
	#ifdef GIA_SYN_REL_TRANSLATOR_DYNAMICALLY_LINK_FROM_CONDITIONS
		private: void linkEntitiesDynamicFromConditions(GIAtranslatorVariablesClass* translatorVariables);
	#endif
			private: bool findPreviousRelationship(GIAentityNode* instanceEntity, const GIAentityNode* comparisonEntity, GIAentityNode** targetEntityFound, bool* previousDefinitionRelationshipFound, bool* previousPropertyRelationshipFound, bool* previousConditionRelationshipFound);
			private: void connectDefinitionToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, bool sameReferenceSet);
			private: void connectPropertyToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, bool sameReferenceSet);
			private: int connectConditionToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, const string conditionEntityName, int conditionIndex, bool sameReferenceSet);	//generic function (belongs in GIAtranslatorOperations.cpp)
			private: int getEntityIndex(const GIAtranslatorVariablesClass* translatorVariables, const GIAentityNode* instanceEntity);
};		//generic function (belongs in GIAtranslatorOperations.cpp) //this function is required because entityIndexTemp reflects that of the referenceSource for references. Consider updating entityIndexTemp of referenceSource each time it is referenced (for current sentence)

#endif
