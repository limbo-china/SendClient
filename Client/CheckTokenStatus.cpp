/*
[]=========================================================================[]

FILE:
	CheckTokenStatus.cpp

DESC:
	
[]=========================================================================[]
*/

//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CheckTokenStatus.h"


#include <iostream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define PKCS_LIB_NAME			"eps2003csp11.dll"
#define NORMLE_FLAG		7
//Initialize the token Cryptoki library:
CCheckTokenStatus::CCheckTokenStatus():m_hDll(NULL_PTR)
{
	m_path = PKCS_LIB_NAME;
}

//Free the token Cryptoki library:Finish.
CCheckTokenStatus::~CCheckTokenStatus()
{
	//if load library failed ,but m_pToke is NULL, so call C_Finalize will be wrong
	if(m_pToken != NULL)
	{
		m_pToken->C_Finalize(NULL_PTR);
		m_pToken = NULL_PTR;
	}
	
	if(m_hDll)
	{
		FreeLibrary(m_hDll);
	}
}

unsigned long CCheckTokenStatus::GetStatus()
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulCount = 0;
	CK_SLOT_ID_PTR pSlotList = NULL_PTR;
	CK_SLOT_INFO slotInfo;
	rv = m_pToken->C_GetSlotList(FALSE, NULL_PTR, &ulCount);
	if ((rv != CKR_OK) || (ulCount <= 0))
		return CKR_DEVICE_ERROR;

	pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (pSlotList == NULL_PTR)
	{
		return CKR_HOST_MEMORY;
	}

	rv = m_pToken->C_GetSlotList(FALSE, pSlotList, &ulCount);
	if ((rv != CKR_OK) || (ulCount <= 0))
	{
		delete[] pSlotList;
		pSlotList = NULL_PTR;
		return CKR_SLOT_ID_INVALID;
	}
	/*Get slot information for the first slot*/
	for (unsigned int i = 0; i < ulCount; ++i)
	{
		rv = m_pToken->C_GetSlotInfo(pSlotList[i], &slotInfo);
		if (rv != CKR_OK)
		{
			delete[] pSlotList;
			pSlotList = NULL_PTR;
			return CKR_FUNCTION_FAILED;
		}
		
	}
	delete[] pSlotList;
	pSlotList = NULL_PTR;
	
	return (slotInfo.flags == NORMLE_FLAG);

}

bool CCheckTokenStatus::LoadLib()
{

#if defined(WIN32)
	m_hDll = LoadLibraryA(m_path);
#else
	m_hDll = dlopen(m_path, RTLD_NOW);
#endif
	if(m_hDll == NULL_PTR)
	{
		printf("Load Library Error!");
		return false;
	}

	typedef CK_RV (* C_GETFUNCTIONLISTPROC)(CK_FUNCTION_LIST_PTR_PTR);
	C_GETFUNCTIONLISTPROC pC_GetFunctionList = (C_GETFUNCTIONLISTPROC)GetProcAddress(m_hDll,"C_GetFunctionList");
	if(pC_GetFunctionList == NULL_PTR)
	{
		printf("Get function list failed.\n");
		return false;
	}
	if(CKR_OK!=pC_GetFunctionList(&m_pToken))
	{
		printf("Get function list failed.\n");
		return false;
	}
	if(CKR_OK != m_pToken->C_Initialize(NULL_PTR))
	{
		printf("Call C_Initialize failed.\n");
		return false;
	}
	return true;
}
