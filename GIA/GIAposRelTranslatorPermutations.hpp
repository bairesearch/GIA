/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorPermutations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3m3c 01-August-2020
 * Requirements: requires plain text file
 * Description: Part-of-speech Relation Translator Permutations
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_PERMUTATIONS
#define HEADER_GIA_POS_REL_TRANSLATOR_PERMUTATIONS

#include "GIAglobalDefs.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3

//#include "SHAREDvars.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
#include "GIAposRelTranslatorHybrid.hpp"
#endif
//#include "SANIrules.hpp"
#ifdef SANI
#include "SANInodes.hpp"
#include "SANIformation.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR
#include "SANIpropagateCompactGenerate.hpp"
#else
#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_UNOPTIMISED
#include "SANIpropagateHeavyUnoptimised.hpp"
#else
#include "SANIpropagateHeavyOptimised.hpp"
#endif
#else
#ifdef SANI_LIGHT_UNOPTIMISED
#include "SANIpropagateLightUnoptimised.hpp"
#else
#include "SANIpropagateLightOptimised.hpp"
#endif
#endif
#endif
#else
#include "SANIpropagateInverse.hpp"
#endif
#include "SANIpropagateOperations.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "GIApreprocessorPOStaggerDatabase.hpp"
#include "GIApreprocessorWordClass.hpp"
#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
#include "GIAnlp.hpp"
#endif
//#include "GIAtranslator.hpp"
#include "SHAREDvars.hpp"

class GIAposRelTranslatorPermutationsClass
{
	//private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#endif
	#ifdef SANI
	private: SANInodesClass SANInodes;
	private: SANIformationClass SANIformation;
#ifdef SANI_SEQUENCE_GRAMMAR
	private: SANIpropagateCompactGenerateClass SANIpropagateCompactGenerate;
#else
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_UNOPTIMISED
	private: SANIpropagateHeavyUnoptimisedClass SANIpropagate;
	#else
	private: SANIpropagateHeavyOptimisedClass SANIpropagate;
	#endif
	#else
	#ifdef SANI_LIGHT_UNOPTIMISED
	private: SANIpropagateLightUnoptimisedClass SANIpropagate;	
	#else
	private: SANIpropagateLightOptimisedClass SANIpropagate;	
	#endif
	#endif
#endif
	#else
	private: SANIpropagateInverseClass SANIpropagateInverse;	
	#endif
	private: SANIpropagateOperationsClass SANIpropagateOperations;	//required for printComponent/printParseTreeDebugIndentation
	/*
	private: SANIrulesClass SANIrules;
	*/
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	private: GIAnlpClass GIAnlp;
	#endif
	private: SHAREDvarsClass SHAREDvars;


	public: bool executeTxtRelTranslatorWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes);
		public: bool executeTxtRelTranslatorWrapper2(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList);
		#ifndef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
		private: bool transferParseTreePOStypeInferredToWordList(GIAtranslatorVariablesClass* translatorVariables);
			private: bool transferParseTreePOStypeInferredToWordList(SANIGroupParseTree* currentParseTreeGroup, int layer);
		#endif
		public: bool executeTxtRelTranslator(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, vector<uint64_t>* POSambiguityInfoPermutation);
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
			private: bool generateParseTreeIntroWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, int* iOptimum, int* minIndexOfMatchesFoundBackupOptimum);
			#else
			private: bool generateParseTreeIntroWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<uint64_t>* POSambiguityInfoPermutation);
			#endif
				#ifdef SANI
				private: bool updatePerformanceNeuralNetwork(const int performanceTemp, int* performance, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroupTemp, const bool passedTemp, const int permutationIndex, int* performanceMaxPermutationIndex);
				private: bool deleteAllSubgroupsRecurse(SANIGroupParseTree* currentParseTreeGroup, int layer);
				#endif
			

};
#endif




#endif
