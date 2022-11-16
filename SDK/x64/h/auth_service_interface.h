/*!
* \file auth_service_interface.h
* \brief Authentication Service Interface
* 
*/
#ifndef _AUTH_SERVICE_INTERFACE_H_
#define _AUTH_SERVICE_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/*! \enum AuthResult
    \brief SDK Authentication Result.
    Here are more detailed structural descriptions.
*/
enum AuthResult
{
	AUTHRET_SUCCESS,///<Authentication is successful.
	AUTHRET_KEYORSECRETEMPTY,///<The key or secret to authenticate is empty.
	AUTHRET_KEYORSECRETWRONG,///<The key or secret to authenticate is wrong.
	AUTHRET_ACCOUNTNOTSUPPORT,///<The user account does not support.
	AUTHRET_ACCOUNTNOTENABLESDK,///<The user account is not enabled for SDK.
	AUTHRET_UNKNOWN,///<Unknown error.
	AUTHRET_SERVICE_BUSY,///<Service is busy.
	AUTHRET_NONE,///<Initial status.
	AUTHRET_OVERTIME,///<Time out.
	AUTHRET_NETWORKISSUE,///<Network issues.
	AUTHRET_CLIENT_INCOMPATIBLE, ///Account does not support this SDK version
	AUTHRET_JWTTOKENWRONG, ///<The jwt token to authenticate is wrong.
};

/*! \enum LOGINSTATUS
    \brief Login status.
    Here are more detailed structural descriptions.
*/
enum LOGINSTATUS
{
	LOGIN_IDLE,///<Unlogged in.
	LOGIN_PROCESSING,///<In process of login.
	LOGIN_SUCCESS,///<Login successful.
	LOGIN_FAILED,///<Login failed.
};

enum LoginFailReason
{
	LoginFail_None = 0,
	LoginFail_EmailLoginDisable,
	LoginFail_UserNotExist,		
	LoginFail_WrongPassword,	
	LoginFail_AccountLocked,	
	LoginFail_SDKNeedUpdate,	
	LoginFail_TooManyFailedAttempts,	
	LoginFail_SMSCodeError,		
	LoginFail_SMSCodeExpired,	
	LoginFail_PhoneNumberFormatInValid,
	LoginFail_LoginTokenInvalid,
	LoginFail_UserDisagreeLoginDisclaimer,
	LoginFail_Mfa_Required,
	LoginFail_Need_Bitrthday_ask,
	LoginFail_OtherIssue = 100, 
};

/*! \struct tagAuthParam
    \brief SDK Authentication parameter with sdk key/secret.
    Here are more detailed structural descriptions.
*/
typedef struct tagAuthParam
{
	const wchar_t* appKey;///<APP Key string.
	const wchar_t* appSecret;///<APP Secret string.
	tagAuthParam()
	{
		appKey = NULL;
		appSecret = NULL;
	}
}AuthParam;

/*! \struct tagAuthContext
    \brief SDK Authentication parameter with jwt token.
    Here are more detailed structural descriptions.
*/
typedef struct tagAuthContext
{
	const wchar_t* jwt_token; /*!JWT token. You may generate your JWT token using the online tool https://jwt.io/. **It is highly recommended to generate your JWT token in your backend server.**
								 JWT is generated with three core parts: Header, Payload, and Signature. When combined, these parts are separated by a period to form a token: `aaaaa.bbbbb.cccc`.
								 Please follow this template to compose your payload for SDK initialization:
							     ** Header
							  	 {
							  		"alg": "HS256",
							  		"typ": "JWT"
							  	 }
							     ** Payload
							   	 {
							        "appKey": "string", // Your SDK key
							        "iat": long, // access token issue timestamp
									"exp": long, // access token expire time
									"tokenExp": long // token expire time
							     }
							     ** Signature
							     HMACSHA256(
							  			base64UrlEncode(header) + "." +
										base64UrlEncode(payload),
										"Your SDK secret here"
								)
							  */
	tagAuthContext()
	{
		jwt_token = NULL;
	}

}AuthContext;
/*! \enum LoginType
	\brief User login type.
	Here are more detailed structural descriptions.
*/
enum LoginType
{
	LoginType_Unknown,///<Unknown type.
	LoginType_SSO,///<Login with SSO token.
};

/*! \struct tagWebinarLegalNoticesExplained
	\brief Webinar Legal notices explained.
	Here are more detailed structural descriptions.
*/
typedef struct tagWebinarLegalNoticesExplainedInfo
{
	const wchar_t* explained_content;
	const wchar_t* url_register_account_owner;
	const wchar_t* url_register_terms;
	const wchar_t* url_register_privacy_policy;
	tagWebinarLegalNoticesExplainedInfo()
	{
		Reset();
	}

	void Reset()
	{
		explained_content = nullptr;
		url_register_account_owner = nullptr;
		url_register_terms = nullptr;
		url_register_privacy_policy = nullptr;
	}
}WebinarLegalNoticesExplainedInfo;

/// \brief Account information interface.
///
class IAccountInfo
{
public:
	/// \brief Get the screen name of user.
	/// \return The return value is the displayed username. If there is no screen name of user, the return value is a string of length ZERO(0).
	virtual const wchar_t* GetDisplayName() = 0;
	/// \brief Get login type.
/// \return The return value is the account login type. For more details, see \link LoginType \endlink enum.
	virtual LoginType GetLoginType() = 0;
	virtual ~IAccountInfo(){};
};

/// \brief Authentication service callback event.
///
class IAuthServiceEvent
{
public:
	virtual ~IAuthServiceEvent() {}

	/// \brief Authentication result callback.
	/// \param ret Authentication result value.  For more details, see \link AuthResult \endlink enum.
	virtual void onAuthenticationReturn(AuthResult ret) = 0;

	/// \brief Callback of login result with fail reason.
	/// \param ret Login status. see \link LOGINSTATUS \endlink enum.
	/// \param pAccountInfo Valid when the ret is LOGINRET_SUCCESS. Otherwise NULL.
	/// \param reason Login fail reason. Valid when the ret is LOGIN_FAILED. Otherwise LoginFail_None. see \link LoginFailReason \endlink enum.
	virtual void onLoginReturnWithReason(LOGINSTATUS ret, IAccountInfo* pAccountInfo, LoginFailReason reason) = 0;
	
	/// \brief Logout result callback.
	virtual void onLogout() = 0;

	/// \brief Zoom identity has expired, please re-login or generate a new zoom access token via REST Api.
	virtual void onZoomIdentityExpired() = 0;

	/// \brief Zoom authentication identity will be expired in 10 minutes, please re-auth.
	virtual void onZoomAuthIdentityExpired() = 0;
};

class IDirectShareServiceHelper;
class IOutlookPluginIntegrationHelper;
/// \brief Authentication Service Interface.
///
class IAuthService
{
public:
	/// \brief Set the authentication service callback event handler.
	/// \param pEvent A pointer to receive authentication event. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(IAuthServiceEvent* pEvent) = 0;

	/// \brief SDK Authentication.
	/// \param authParam The parameter to be used for authentication SDK, see \link AuthParam \endlink structure. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SDKAuth(AuthParam& authParam) = 0;

	/// \brief SDK Authentication with jwt token.
	/// \param authContext The parameter to be used for authentication SDK, see \link AuthContext \endlink structure. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SDKAuth(AuthContext& authContext) = 0;

	/// \brief Get authentication status.
	/// \return The return value is authentication status. To get extended error information, see \link AuthResult \endlink enum.
	virtual AuthResult GetAuthResult() = 0;

	/// \brief Get SDK identity.
	/// \return The SDK identity.
	virtual const wchar_t* GetSDKIdentity() = 0;

	/// \brief Get SSO login web url.
	/// \param prefix_of_vanity_url, prefix of vanity url. 
	/// \return SSO login web url
	virtual const wchar_t* GenerateSSOLoginWebURL(const wchar_t* prefix_of_vanity_url) = 0;

	/// \brief Account login.
	/// \param uri_protocol For the parameter to be used for sso account login
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	///You need to call this APIs after IAuthServiceEvent::onAuthenticationReturn() return SDKErr_Success.
	virtual SDKError SSOLoginWithWebUriProtocol(const wchar_t* uri_protocol) = 0;
	
	/// \brief Account logout.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError LogOut() = 0;

	/// \brief Get login account information.
	/// \return If you has logged in your account successfully, the return value is a pointer to IAccountInfo, otherwise is NULL.
	virtual IAccountInfo* GetAccountInfo() = 0;

	/// \brief Get login status.
	/// \return The return value is login status. To get extended error information, see \link LOGINSTATUS \endlink enum.
	virtual LOGINSTATUS GetLoginStatus() = 0;

	/// \brief Get direct share service helper interface. 
	/// \return If you logged in your account successfully, the return value is the object pointer IDirectShareServiceHelper. Otherwise is NULL.
	virtual IDirectShareServiceHelper* GetDirectShareServiceHeler() = 0;

	/// Get the webinal legal notices prompt.
	virtual const wchar_t* getWebinalLegalNoticesPrompt() = 0;

	/// Get the webinal legal notices explained.
	virtual bool getWebinalLegalNoticesExplained(WebinarLegalNoticesExplainedInfo& explained_info) = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif
