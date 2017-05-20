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
 * File Name: GIAsemanticParserTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b1b 19-May-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_TRANSLATOR
#define HEADER_GIA_SEMANTIC_PARSER_TRANSLATOR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAsemanticParserOperations.hpp"
#include "GIAtranslatorDefineGrammar.hpp"
#include "GIAtranslatorRedistributeRelations.hpp"
#include "GIAtranslatorDefineReferencing.hpp"
#include "GIAtranslatorLinkEntitiesDynamic.hpp"
#include "GIAtranslatorDefineSubstances.hpp"
#include "GIAtranslatorApplyAdvancedFeatures.hpp"
#include "GIAnlp.hpp"
#include "GIAsemanticParserDatabase.hpp"
#include "GIAbot.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"

#ifdef GIA_SEMANTIC_PARSER

class GIAsemanticParserTranslatorClass
{
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorRedistributeRelationsClass GIAtranslatorRedistributeRelations;
	private: GIAtranslatorDefineReferencingClass GIAtranslatorDefineReferencing;
	private: GIAtranslatorLinkEntitiesDynamicClass GIAtranslatorLinkEntitiesDynamic;
	private: GIAbotClass GIAbot;
	private: GIAtranslatorDefineSubstancesClass GIAtranslatorDefineSubstances;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIAtranslatorApplyAdvancedFeaturesClass GIAtranslatorApplyAdvancedFeatures;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	private: GIAnlpClass GIAnlp;
	public: void convertSentenceSemanticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);
		private: void locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		private: void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, const string modalAuxiliaryString);
		private: void defineSubstancesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
		private: void identifyComparisonVariableBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#endif
		private: void defineConnectionsBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
			private: void invertOrDuplicateConditionsIfRequiredSemantic(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet);
			private: GIAentityNode* createNewInverseConditionEntitySemantic(GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName);
			#endif
		private: GIAentityNode* createNewRelationshipEntitySemantic(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
		private: void applyAdvancedFeaturesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void defineQuantitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void defineQualitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_SEMANTIC_PARSER_SUBSETS
	public: bool generateAllPermutationsFromSemanticRelationsFile(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
	#endif
};

#endif

#endif
