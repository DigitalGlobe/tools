#include "AuthSspi.h"

#ifdef TRUSTED_AUTH
#include <../common/classes/ClumpletReader.h>

namespace
{
	void makeDesc(SecBufferDesc& d, SecBuffer& b, size_t len, void* p)
	{
		b.BufferType = SECBUFFER_TOKEN;
		b.cbBuffer = len;
		b.pvBuffer = len ? p : 0;
		d.ulVersion = SECBUFFER_VERSION;
		d.cBuffers = 1;
		d.pBuffers = &b;
	}

	template<typename ToType>
		ToType getProc(HINSTANCE lib, const char* entry)
	{
		FARPROC rc = GetProcAddress(lib, entry);
		if (! rc)
		{
			Firebird::LongJump::raise();
		}
		return (ToType)rc;
	}
}

HINSTANCE AuthSspi::library = 0;

bool AuthSspi::initEntries()
{
	if (! library)
	{
		library = LoadLibrary("secur32.dll");
	}
	if (! library)
	{
		return false;
	}

	try
	{
		fAcquireCredentialsHandle = getProc<ACQUIRE_CREDENTIALS_HANDLE_FN_A>
			(library, "AcquireCredentialsHandleA");
		fDeleteSecurityContext = getProc<DELETE_SECURITY_CONTEXT_FN>
			(library, "DeleteSecurityContext");
		fFreeCredentialsHandle = getProc<FREE_CREDENTIALS_HANDLE_FN>
			(library, "FreeCredentialsHandle");
		fQueryContextAttributes = getProc<QUERY_CONTEXT_ATTRIBUTES_FN_A>
			(library, "QueryContextAttributesA");
		fFreeContextBuffer = getProc<FREE_CONTEXT_BUFFER_FN>
			(library, "FreeContextBuffer");
		fInitializeSecurityContext = getProc<INITIALIZE_SECURITY_CONTEXT_FN_A>
			(library, "InitializeSecurityContextA");
		fAcceptSecurityContext = getProc<ACCEPT_SECURITY_CONTEXT_FN>
			(library, "AcceptSecurityContext");
	}
	catch (const Firebird::LongJump&)
	{
		return false;
	}
	return true;
}

AuthSspi::AuthSspi()
	: hasContext(false), ctName(*getDefaultMemoryPool()), wheel(false)
{
	TimeStamp timeOut;
	hasCredentials = initEntries() && (fAcquireCredentialsHandle(0, "NTLM",
					SECPKG_CRED_BOTH, 0, 0, 0, 0,
					&secHndl, &timeOut) == SEC_E_OK);
}

AuthSspi::~AuthSspi()
{
	if (hasContext)
	{
		fDeleteSecurityContext(&ctxtHndl);
	}
	if (hasCredentials)
	{
		fFreeCredentialsHandle(&secHndl);
	}
}

bool AuthSspi::checkAdminPrivilege(PCtxtHandle phContext) const
{
#if defined (__GNUC__)
	// ASF: MinGW hack.
	struct SecPkgContext_AccessToken
	{
		void* AccessToken;
	};
	const int SECPKG_ATTR_ACCESS_TOKEN = 18;
#endif

	// Query access token from security context
	SecPkgContext_AccessToken spc;
	spc.AccessToken = 0;
	if (fQueryContextAttributes(phContext, SECPKG_ATTR_ACCESS_TOKEN, &spc) != SEC_E_OK)
	{
		return false;
	}

	// Query required buffer size
	DWORD token_len = 0;
	GetTokenInformation(spc.AccessToken, TokenGroups, 0, 0, &token_len);

	// Query actual group information
	Firebird::Array<char> buffer;
	TOKEN_GROUPS *ptg = (TOKEN_GROUPS *)buffer.getBuffer(token_len);
	bool ok = GetTokenInformation(spc.AccessToken,
			TokenGroups, ptg, token_len, &token_len);
	if (! ok)
	{
		return false;
	}

	// Create a System Identifier for the Admin group.
	SID_IDENTIFIER_AUTHORITY system_sid_authority = {SECURITY_NT_AUTHORITY};
	PSID domain_admin_sid, local_admin_sid;

	if (!AllocateAndInitializeSid(&system_sid_authority, 2,
				SECURITY_BUILTIN_DOMAIN_RID,
				DOMAIN_GROUP_RID_ADMINS,
				0, 0, 0, 0, 0, 0, &domain_admin_sid))
	{
		return false;
	}

	if (!AllocateAndInitializeSid(&system_sid_authority, 2,
				SECURITY_BUILTIN_DOMAIN_RID,
				DOMAIN_ALIAS_RID_ADMINS,
				0, 0, 0, 0, 0, 0, &local_admin_sid))
	{
		return false;
	}

	bool matched = false;

	// Finally we'll iterate through the list of groups for this access
	// token looking for a match against the SID we created above.
	for (DWORD i = 0; i < ptg->GroupCount; i++)
	{
		if (EqualSid(ptg->Groups[i].Sid, domain_admin_sid) ||
			EqualSid(ptg->Groups[i].Sid, local_admin_sid))
		{
			matched = true;
			break;
		}
	}

	FreeSid(domain_admin_sid);
	FreeSid(local_admin_sid);
	return matched;
}

bool AuthSspi::request(AuthSspi::DataHolder& data)
{
	if (! hasCredentials)
	{
		data.clear();
		return false;
	}

	TimeStamp timeOut;

	char s[BUFSIZE];
	SecBuffer outputBuffer, inputBuffer;
	SecBufferDesc outputDesc, inputDesc;
	makeDesc(outputDesc, outputBuffer, sizeof(s), s);
	makeDesc(inputDesc, inputBuffer, data.getCount(), data.begin());

	ULONG fContextAttr = 0;

	SECURITY_STATUS x = fInitializeSecurityContext(
		&secHndl, hasContext ? &ctxtHndl : 0, 0, 0, 0, SECURITY_NATIVE_DREP,
		hasContext ? &inputDesc : 0, 0, &ctxtHndl, &outputDesc, &fContextAttr, &timeOut);
	switch (x)
	{
	case SEC_E_OK:
		fDeleteSecurityContext(&ctxtHndl);
		hasContext = false;
		break;
	case SEC_I_CONTINUE_NEEDED:
		hasContext = true;
		break;
	default:
		if (hasContext)
		{
			fDeleteSecurityContext(&ctxtHndl);
		}
		hasContext = false;
		data.clear();
		return false;
	}

	if (outputBuffer.cbBuffer)
	{
		memcpy(data.getBuffer(outputBuffer.cbBuffer),
			   outputBuffer.pvBuffer, outputBuffer.cbBuffer);
	}
	else
	{
		data.clear();
	}

	return true;
}

bool AuthSspi::accept(AuthSspi::DataHolder& data)
{
	if (! hasCredentials)
	{
		data.clear();
		return false;
	}

	TimeStamp timeOut;

	char s[BUFSIZE];
	SecBuffer outputBuffer, inputBuffer;
	SecBufferDesc outputDesc, inputDesc;
	makeDesc(outputDesc, outputBuffer, sizeof(s), s);
	makeDesc(inputDesc, inputBuffer, data.getCount(), data.begin());

	ULONG fContextAttr = 0;
	SecPkgContext_Names name;
	SECURITY_STATUS x = fAcceptSecurityContext(
		&secHndl, hasContext ? &ctxtHndl : 0, &inputDesc, 0,
		SECURITY_NATIVE_DREP, &ctxtHndl, &outputDesc,
		&fContextAttr, &timeOut);
	switch (x)
	{
	case SEC_E_OK:
		if (fQueryContextAttributes(&ctxtHndl, SECPKG_ATTR_NAMES, &name) == SEC_E_OK)
		{
			ctName = name.sUserName;
			ctName.upper();
			fFreeContextBuffer(name.sUserName);
			wheel = checkAdminPrivilege(&ctxtHndl);
		}
		fDeleteSecurityContext(&ctxtHndl);
		hasContext = false;
		break;
	case SEC_I_CONTINUE_NEEDED:
		hasContext = true;
		break;
	default:
		if (hasContext)
		{
			fDeleteSecurityContext(&ctxtHndl);
		}
		hasContext = false;
		data.clear();
		return false;
	}

	if (outputBuffer.cbBuffer)
	{
		memcpy(data.getBuffer(outputBuffer.cbBuffer),
			   outputBuffer.pvBuffer, outputBuffer.cbBuffer);
	}
	else
	{
		data.clear();
	}

	return true;
}

bool AuthSspi::getLogin(Firebird::string& login, bool& wh)
{
	wh = false;
	if (ctName.hasData())
	{
		login = ctName;
		ctName.erase();
		wh = wheel;
		wheel = false;
		return true;
	}
	return false;
}

#endif // TRUSTED_AUTH
