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
 * File Name: GIAtranslatorApplyAdvancedFeatures.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b3d 25-May-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_APPLY_ADVANCED_FEATURES
#define HEADER_GIA_TRANSLATOR_APPLY_ADVANCED_FEATURES

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
#include "GIAsemanticParserOperations.hpp"
#endif
#include "GIAtranslatorGeneric.hpp"

class GIAtranslatorApplyAdvancedFeaturesClass
{
	private: GIAconditionNodeClassClass GIAconditionNodeClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	#endif
	private: GIAtranslatorGenericClass GIAtranslatorGeneric;
	public: void applyAdvancedFeatures(GIAtranslatorVariablesClass* translatorVariables);
		private: void extractDates(GIAtranslatorVariablesClass* translatorVariables);					//Stanford Compatible
			#ifdef GIA_RELEX
			private: void extractDatesRelex(GIAtranslatorVariablesClass* translatorVariables);
			#endif
			#ifdef GIA_STANFORD_CORENLP
			private: void extractDatesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables);					//Stanford Specific	[requires NERTemp info to be set, + need to know if timeConditionNodes have already been added; if not add them now]
			#endif
				public: void addTimeToSubstance(GIAentityNode* timeConditionEntity);
		private: void extractQuantities(GIAtranslatorVariablesClass* translatorVariables);	//Stanford Compatible
			#ifdef GIA_RELEX
			private: void extractQuantitiesRelex(GIAtranslatorVariablesClass* translatorVariables);
			#endif
			#ifdef GIA_STANFORD_CORENLP
			private: void extractQuantitiesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables);		//Stanford Specific	[requires NERTemp info to be set]
			#endif

		public: void defineTenseOnlyTimeConditions(GIAtranslatorVariablesClass* translatorVariables);	//#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS || #ifdef GIA_TRANSLATOR_XML_INTERPRETATION

		#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
		private: void defineActionConcepts1(GIAtranslatorVariablesClass* translatorVariables);
		private: void defineActionConcepts2(GIAtranslatorVariablesClass* translatorVariables);
		#endif

		#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
		private: void updateConceptDesignationBasedPropertyOwnerContext(GIAtranslatorVariablesClass* translatorVariables);
		#endif
};
	

#endif
