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
 * File Name: GIAnlg.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1c 26-February-2017
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NLG
#define HEADER_GIA_NLG

#include "GIAglobalDefs.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAtranslatorDefineGrammar.hpp"

class GIANLGentity
{
public:

	GIANLGentity(void);
	~GIANLGentity(void);

	string entityGrammatisedText;

	GIANLGentity* next;
};

#define GIA_NLG_GENERATE_LANGUAGE_MAX_NUM_ITERATIONS (4)	//takes into account connections (/2)

#define GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND_IMPLICIT ", "
#define GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND "and "

#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_TYPE_PROPERTY_SUBSTANCE (RELATION_ENTITY_HAVE)
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_TYPE_PROPERTY_QUALITY (RELATION_ENTITY_BE)
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_TYPE_DEFINITION (RELATION_ENTITY_BE)
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_TYPE_CONDITION (RELATION_ENTITY_BE)

//basic tense generation (no tense modifiers as yet; eg GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE)
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_SINGULAR_PRESENT "is "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_SINGULAR_PAST "was "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_SINGULAR_FUTURE "will be "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_PLURAL_PRESENT "are "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_PLURAL_PAST "were "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_PLURAL_FUTURE "will be "

#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_SINGULAR_PRESENT "has "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_SINGULAR_PAST "had "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_SINGULAR_FUTURE "will have "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_PLURAL_PRESENT "have "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_PLURAL_PAST "had "
#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_PLURAL_FUTURE "will have "

static string entityRelationshipTenseNumerosityCrossReferenceBeAuxiliaryArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES][GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {{"", "", "", ""}, {"", "", GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_SINGULAR_PRESENT, GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_PLURAL_PRESENT}, {"", "", GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_SINGULAR_PAST, GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_PLURAL_PAST}, {"", "", GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_SINGULAR_FUTURE, GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_BE_PLURAL_FUTURE}};
static string entityRelationshipTenseNumerosityCrossReferenceHaveAuxiliaryArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES][GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {{"", "", "", ""}, {"", "", GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_SINGULAR_PRESENT, GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_PLURAL_PRESENT}, {"", "", GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_SINGULAR_PAST, GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_PLURAL_PAST}, {"", "", GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_SINGULAR_FUTURE, GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_HAVE_PLURAL_FUTURE}};

static bool entityVectorConnectionGoToParentArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, true, false, true, false, true, false, true, true, false, false, false};
static bool entityVectorConnectionGoToChildArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, false, true, false, true, false, true, false, false, true, false, false};


#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_ACTION_PRESENT_APPEND "s"
//#define GIA_NLG_RELATIONSHIP_ENTITY_AUXILIARY_ACTION_PAST "did"

#define GIA_NLG_MORPH_PLURALITY_APPEND "s"
#define GIA_NLG_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET "which "
#define GIA_NLG_RELATIONSHIP_ENTITY_ACTION_REVERSE "by "

#define GIA_NLG_SUPPORT_PERSON_AND_GENDER
#define GIA_NLG_SUPPORT_TIME_CONDITIONS	//CHECKTHIS
#define GIA_NLG_TEXT_SPACE (STRING_SPACE)

#define NLG_NUMBER_OF_VOWELS (5)
static char vowelArray[NLG_NUMBER_OF_VOWELS] = {'a','e','i','o','u'};

class GIAnlgClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	
	#ifdef GIA_NLG
	public: string generateLanguageFromNonWhichQuery(GIAentityNode* comparisonVariableNode, GIAentityNode* queryAnswerNode);
	public: string generateLanguageFromWhichQuery(GIAentityNode* comparisonVariableNode, GIAentityNode* queryAnswerNode);
		private: string generateLanguageFromNLGentityList(GIANLGentity* firstNLGentityInContext);		
		private: bool generateLanguageFromTextIteration(GIAentityNode* currentEntity, GIANLGentity** currentNLGentity, bool parseSameReferenceSetOnly, bool isSameReferenceSetIteration, int interationIndex, bool isRelationshipReverseIteration);
			private: bool generateQualityPrependText(GIAentityNode* currentEntity, GIANLGentity** currentNLGentityInSection, bool testSameReferenceSet);
				private: bool generateConjunctionOfNLGentitiesInSection(int qualityCountMax, vector<GIANLGentity*>* NLGentitiesInSection);
				//private: bool generateConjunctionOfNLGentitiesInSection(int qualityCountMax, GIANLGentity* firstNLGentityInSection);
			private: bool generateRelationshipEntityLanguage(GIAentityNode* relationshipEntity, GIANLGentity** currentNLGentity, bool sameReferenceSet);
			private: bool generateRelationshipEntityLanguageActionReverse(GIAentityNode* relationshipEntity,  GIANLGentity** currentNLGentity, bool sameReferenceSet);
				private: string generateMorphologyAction(GIAentityNode* relationshipEntity);
				private: string generateMorphologyActionReverse(GIAentityNode* relationshipEntity);
				private: string generateMorphologyCondition(GIAentityNode* relationshipEntity);
				private: string generateMorphologyRelationshipAuxiliaryBe(GIAentityNode* relationshipEntity);
				private: string generateMorphologyRelationshipAuxiliaryHave(GIAentityNode* relationshipEntity);
			private: bool generateNounEntityLanguage(GIAentityNode* nounEntity, GIANLGentity** currentNLGentity, bool definite);
				private: string calcDeterminate(const GIAentityNode* entityNode, const bool definite);
				private: string calcNounWord(const GIAentityNode* entityNode);

	private: string generatePlurality(string entityName, int grammaticalNumber);

	#endif

};



#endif
