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
 * File Name: GIAposRelTranslatorSANIFormation.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l6c 21-June-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Formation
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_SANI_FORMATION
#define HEADER_GIA_POS_REL_TRANSLATOR_SANI_FORMATION

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAposRelTranslatorRules.hpp"
#include "GIAposRelTranslatorRulesGroupClass.hpp"
#include "GIAposRelTranslatorRulesComponentClass.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "ANNdisplay.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_SANI

class GIAposRelTranslatorSANIFormationClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIAposRelTranslatorRulesGroupClass GIAposRelTranslatorRulesGroupClassObject;
	private: GIAposRelTranslatorRulesClass GIAposRelTranslatorRules;
	private: ANNdisplayClass ANNdisplay;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	public: vector<string>* getExplicitWordList();
	#endif
	public: unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>* getInputLayerSectionExplicitWordMap();
	public: unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>* getInputLayerSectionTokensLayerMap();
	public: GIAposRelTranslatorRulesGroupNeuralNetwork* getFirstGroupInInputLayerSectionWordPOStype();
	#ifdef GIA_NEURAL_NETWORK
	public: GIAposRelTranslatorRulesGroupNeuralNetwork* getFirstInputGroupInNetwork();
	#endif

	public: bool createGIAposRelTranslatorSANI(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes);
		private: bool createInputNeuronLayer(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes);
			private: bool createInputNeuronLayerSectionWordPOStype(GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordPOStype, int* numberOfGroupsInSection);
				private: bool createInputNeuronLayerSectionNounVariantType(GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordNounVariantType, int* numberOfGroupsInSection);
				private: bool createInputNeuronLayerSectionVerbVariantType(GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordVerbVariantType, int* numberOfGroupsInSection);
			private: bool createInputNeuronLayerSectionExplicitWord(GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSection, int* numberOfGroupsInSection, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes);	
				private: bool createInputNeuronLayerSectionExplicitWord(vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, vector<string>* explicitWordList);	
			private: bool createInputNeuronLayerSectionTokensLayer(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionTokensLayer, int* numberOfGroupsInSectionTokensLayer);
		private: bool createNeuronLayerIntro(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes);
			private: bool createNeuronLayerGroupType(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent, GIAposRelTranslatorRulesGroupType* groupType);
				private: bool createNeuronLayerGroup(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupNeuralNetwork* group);
					private: bool createNeuronLayerComponents(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupNeuralNetwork* group, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);

	private: bool findTokensLayerClassType(string layerName, string layerClassName, string layerClassTypeName, GIAposRelTranslatorRulesGroupNeuralNetwork** groupFound);		
		private: bool findTokensLayerClass(string layerName, string layerClassName, GIAposRelTranslatorRulesGroupNeuralNetwork** layerClassGroupFound);
	private: bool findWordInList(const string word, vector<string>* explicitWordList);
	public: bool findWordInGroupMap(const string word, unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>* wordMap, GIAposRelTranslatorRulesGroupNeuralNetwork** groupFound);
	public: GIAposRelTranslatorRulesGroupNeuralNetwork* getInputGroupLayerSection(GIAposRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSection, int groupIndexInSection);
	private: void addGroupToLayer(GIAposRelTranslatorRulesGroupNeuralNetwork** currentGroupInLayer, int* numberOfGroupsInSection);
	private: bool createGroupANNconnectionIO(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup);
	private: bool createGroupANNconnectionIObasic(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup);
	public: bool createGroupANNconnection(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
			public: ANNneuronConnection* createANNconnection(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);
				public: ANNneuronConnection* createANNconnection(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent, const double connectionStrength);
					private: ANNneuronConnection* createANNconnection(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
			public: void deleteANNconnections(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* component);
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
			public: bool createANNconnectivity(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
			public: bool createANNconnectivityReset(vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes);
			#endif
			#endif
			#endif
};

#endif

#endif
