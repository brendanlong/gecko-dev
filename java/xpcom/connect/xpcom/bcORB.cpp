/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Sun Microsystems,
 * Inc. Portions created by Sun are
 * Copyright (C) 1999 Sun Microsystems, Inc. All
 * Rights Reserved.
 *
 * Contributor(s):
 * Igor Kushnirskiy <idk@eng.sun.com>
 */
#include "bcORB.h"
#include "../src/ORB.h"
#include "nsIGenericFactory.h"
#include "nsIModule.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(bcORB);

static  nsModuleComponentInfo components[] =
{
    {
        "Black Connect ORB",
        BC_ORB_CID,
        BC_ORB_PROGID,
        bcORBConstructor
    }
};

NS_IMPL_NSGETMODULE("BlackConnectORB",components);



NS_IMPL_ISUPPORTS(bcORB,NS_GET_IID(bcORB));

bcORB::bcORB() :
    orb(0)
{
    NS_INIT_REFCNT();
}

bcORB::~bcORB() {
    if (orb) {
	delete orb; //nb should we destroy it?
    }
}

NS_IMETHODIMP bcORB::GetORB(bcIORB **_orb) {
    if (!_orb) {
	printf("--bcORB::GetORB\n");
	return NS_ERROR_NULL_POINTER;
    }
    if (!orb) {
	orb = new ORB();
    }
    *_orb = orb;
    return NS_OK;
}









