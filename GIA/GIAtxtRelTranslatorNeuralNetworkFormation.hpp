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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1m 24-April-2018
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
	
	public: unordered_map<string, GIAtxtRelTranslatorRulesGroup*>* getInputLayerSectionExplicitWordMap();
	public: unordered_map<string, GIAtxtRelTranslatorRulesGroup*>* getInputLayerSectionTokensLayerMap();
	public: GIAtxtRelTranslatorRulesGroup* getFirstGroupInInputLayerSectionWordPOStype();

	public: bool createGIAtxtRelTranslatorNeuralNetwork(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
		private: bool createInputNeuronLayer(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
			private: bool createInputNeuronLayerSectionWordPOStype(GIAtxtRelTranslatorRulesGroup** currentGroupInInputLayerSectionWordPOStype, int* numberOfGroupsInSection);
				private: bool createInputNeuronLayerSectionNounVariantType(GIAtxtRelTranslatorRulesGroup* currentGroupInInputLayerSectionWordPOStype, GIAtxtRelTranslatorRulesGroup** currentGroupInInputLayerSectionWordNounVariantType, int* numberOfGroupsInSection);
				private: bool createInputNeuronLayerSectionVerbVariantType(GIAtxtRelTranslatorRulesGroup* currentGroupInInputLayerSectionWordPOStype, GIAtxtRelTranslatorRulesGroup** currentGroupInInputLayerSectionWordVerbVariantType, int* numberOfGroupsInSection);
			private: bool createInputNeuronLayerSectionExplicitWord(GIAtxtRelTranslatorRulesGroup** currentGroupInInputLayerSection, int* numberOfGroupsInSection, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);	
				private: bool createInputNeuronLayerSectionExplicitWord(vector<GIAtxtRelTranslatorRulesComponent*>* components, bool subcomponents, vector<string>* explicitWordList);	
			private: bool createInputNeuronLayerSectionTokensLayer(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroup** currentGroupInInputLayerSectionTokensLayer, int* numberOfGroupsInSectionTokensLayer);
		private: bool createNeuronLayerIntro(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
			private: bool createNeuronLayerGroupType(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesComponent* higherLevelComponent, GIAtxtRelTranslatorRulesGroupType* groupType);
				private: bool createNeuronLayerGroup(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroup* group);
					private: bool createNeuronLayerComponents(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroup* group, vector<GIAtxtRelTranslatorRulesComponent*>* components, bool subcomponents, GIAtxtRelTranslatorRulesComponent* higherLevelComponent);

	private: bool findTokensLayerClassType(string layerName, string layerClassName, string layerClassTypeName, GIAtxtRelTranslatorRulesGroup** groupFound);		
		private: bool findTokensLayerClass(string layerName, string layerClassName, GIAtxtRelTranslatorRulesGroup** layerClassGroupFound);
	private: bool findWordInList(const string word, vector<string>* explicitWordList);
	public: bool findWordInGroupMap(const string word, unordered_map<string, GIAtxtRelTranslatorRulesGroup*>* wordMap, GIAtxtRelTranslatorRulesGroup** groupFound);
	public: GIAtxtRelTranslatorRulesGroup* getInputGroupLayerSection(GIAtxtRelTranslatorRulesGroup* firstGroupInInputLayerSection, int groupIndexInSection);
	private: void addGroupToLayer(GIAtxtRelTranslatorRulesGroup** currentGroupInLayer, int* numberOfGroupsInSection);
	private: bool createGroupANNconnectionIO(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* higherLevelGroup);
	private: bool createGroupANNconnection(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* higherLevelComponent);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
		private: ANNneuronConnection* createANNconnection(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* higherLevelGroup);
		private: ANNneuronConnection* createANNconnection(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* higherLevelComponent);
		#endif
};

#endif

#endif
