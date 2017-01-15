/*******************************************************************************
 *
 * File Name: GIAEntityConnectionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l1g 24-May-2012
 *
 *******************************************************************************/


#include "GIAEntityNodeClass.h"


GIAEntityConnection::GIAEntityConnection(void)
{
	#ifdef GIA_USE_ADVANCED_REFERENCING
	sameReferenceSet = false;	//CHECK THIS default value
	#endif

	#ifdef GIA_USE_DATABASE
	referenceLoaded = false;
	entityName = "";
	idInstance = -1;
	loaded = false;
	modified = false;
	added = false;
	#endif	
}
GIAEntityConnection::~GIAEntityConnection(void)
{
}
GIAEntityConnection::GIAEntityConnection(string * entityName, long idInstance)
{
	#ifdef GIA_USE_ADVANCED_REFERENCING
	sameReferenceSet = false;	//CHECK THIS default value
	#endif

	#ifdef GIA_USE_DATABASE
	referenceLoaded = false;
	entityName = "";
	idInstance = -1;
	loaded = false;
	modified = false;
	added = false;
	#endif	
}

