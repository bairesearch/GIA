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
 * File Name: GIAtxtRelTranslatorNeuralNetworkFormation.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3i3b 24-June-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Formation
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FORMATION
#define HEADER_GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FORMATION

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

class GIAtxtRelTranslatorNeuralNetworkFormationClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: ANNdisplayClass ANNdisplay;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	public: vector<string>* getExplicitWordList();
	#endif
	public: unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* getInputLayerSectionExplicitWordMap();
	public: unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* getInputLayerSectionTokensLayerMap();
	public: GIAtxtRelTranslatorRulesGroupNeuralNetwork* getFirstGroupInInputLayerSectionWordPOStype();
	#ifdef GIA_NEURAL_NETWORK
	public: GIAtxtRelTranslatorRulesGroupNeuralNetwork* getFirstInputGroupInNetwork();
	#endif

	public: bool createGIAtxtRelTranslatorNeuralNetwork(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
		private: bool createInputNeuronLayer(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
			private: bool createInputNeuronLayerSectionWordPOStype(GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordPOStype, int* numberOfGroupsInSection);
				private: bool createInputNeuronLayerSectionNounVariantType(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordNounVariantType, int* numberOfGroupsInSection);
				private: bool createInputNeuronLayerSectionVerbVariantType(GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionWordVerbVariantType, int* numberOfGroupsInSection);
			private: bool createInputNeuronLayerSectionExplicitWord(GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSection, int* numberOfGroupsInSection, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);	
				private: bool createInputNeuronLayerSectionExplicitWord(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, vector<string>* explicitWordList);	
			private: bool createInputNeuronLayerSectionTokensLayer(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInInputLayerSectionTokensLayer, int* numberOfGroupsInSectionTokensLayer);
		private: bool createNeuronLayerIntro(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
			private: bool createNeuronLayerGroupType(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent, GIAtxtRelTranslatorRulesGroupType* groupType);
				private: bool createNeuronLayerGroup(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group);
					private: bool createNeuronLayerComponents(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, bool subcomponents, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);

	private: bool findTokensLayerClassType(string layerName, string layerClassName, string layerClassTypeName, GIAtxtRelTranslatorRulesGroupNeuralNetwork** groupFound);		
		private: bool findTokensLayerClass(string layerName, string layerClassName, GIAtxtRelTranslatorRulesGroupNeuralNetwork** layerClassGroupFound);
	private: bool findWordInList(const string word, vector<string>* explicitWordList);
	public: bool findWordInGroupMap(const string word, unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* wordMap, GIAtxtRelTranslatorRulesGroupNeuralNetwork** groupFound);
	public: GIAtxtRelTranslatorRulesGroupNeuralNetwork* getInputGroupLayerSection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstGroupInInputLayerSection, int groupIndexInSection);
	private: void addGroupToLayer(GIAtxtRelTranslatorRulesGroupNeuralNetwork** currentGroupInLayer, int* numberOfGroupsInSection);
	private: bool createGroupANNconnectionIO(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup);
	private: bool createGroupANNconnectionIObasic(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup);
	private: bool createGroupANNconnection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
		private: ANNneuronConnection* createANNconnection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup);
		private: ANNneuronConnection* createANNconnection(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);
		#endif
};

#endif

#endif
