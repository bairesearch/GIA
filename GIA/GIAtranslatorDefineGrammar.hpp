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
 * File Name: GIAtranslatorDefineGrammar.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e8e 18-December-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR
#define HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAtranslatorGeneric.hpp"
#include "GIAdatabase.hpp"

//#define GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILIARY_TO_SET_TENSE_OF_VERB	//this seems theoretically possible pased upon the examples given (ie because the tense stanfordPOS information is replicated in both the auxiliary and the verb)



//Pass A
class GIAtranslatorDefineGrammarClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAtranslatorGenericClass GIAtranslatorGeneric;
	private: SHAREDvarsClass SHAREDvars;
	public: void locateAndAddAllFeatureTempEntities(GIAtranslatorVariablesClass* translatorVariables);
	public: void locateAndAddAllNetworkIndexEntities(GIAtranslatorVariablesClass* translatorVariables);

	#ifdef GIA_RELEX
	public: void fillGrammaticalArraysRelex(GIAsentence* currentSentenceInList);
	#endif
//uses dependency relations to derive grammar [uses stanfordPOS/NER information to derive grammar, if NLPfeatureParser == Stanford Core NLP]
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	public: void fillGrammaticalArraysStanford(GIAtranslatorVariablesClass* translatorVariables);
		private: void extractPastTense(GIAfeature* featureWithEntityIndex, const int entityIndexContainingTenseIndication, const GIAfeature* firstFeatureInList, const int NLPfeatureParser);
			private: void extractPastTenseFromPOStag(const string* POStag, GIAfeature* feature);
		public: void extractGrammaticalInformationStanford(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
			public: void extractPOSrelatedGrammaticalInformationStanford(GIAfeature* currentFeature, const bool grammaticallyStrict);
				private: void extractGrammaticalInformationFromStanfordPOStag(const string* POStag, GIAfeature* feature);
	#endif

	public: void applyGrammaticalInfoToAllEntities(GIAtranslatorVariablesClass* translatorVariables, GIAfeature* firstFeatureInSentencee);
		public: void applyPOSrelatedGrammaticalInfoToEntity(GIAentityNode* entity, GIAfeature* currentFeatureInList);

	#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
	private: void findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(GIAsentence* currentSentenceInList, const int subjectObjectEntityWithAuxiliaryEntityIndex, const string* subjectObjectEntityWithAuxiliaryEntityName);
	#endif
};


#endif
