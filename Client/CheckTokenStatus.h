#if !defined(AFX_GETINFOS_H__DF276D83_F3BC_4371_8530_1257E8753DBB__INCLUDED_)
#define AFX_GETINFOS_H__DF276D83_F3BC_4371_8530_1257E8753DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCheckTokenStatus  
{
public:
	bool LoadLib();
	unsigned long GetStatus();
	
	CCheckTokenStatus();
	virtual ~CCheckTokenStatus();
private:
	CK_FUNCTION_LIST_PTR m_pToken;

	HMODULE m_hDll;


	char* m_path;
};


#endif // !defined(AFX_GETINFOS_H__DF276D83_F3BC_4371_8530_1257E8753DBB__INCLUDED_)
