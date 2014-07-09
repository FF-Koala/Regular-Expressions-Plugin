				///////////////////////////////////////////////////////////////////////////
				//				Author:	Koala											 //
				//				Release Website: Gta-Mp.ro								 //
				//				Release Date: 9/07/2014									 //
				//				Credits: -maddinat0r (Got the SDK from his github)		 //
				//						 -K.Kosako (Author of Oniguruma library)		 //
				//				Description: This plugins allows PAWN					 //
				//							use of the basic regex functions.			 //
				//                                                                       //
				//						Regular Expresion Library						 //
				//												v0.1                     //
				//                                                                       //
				//                                                                       //
				//                                                                       //
				//                                                                       //
				///////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <string>
#include <oniguruma.h>
#include <amx/amx.h>
#include <plugincommon.h>
#define PLUGIN_VERSION "0.1"

typedef void 
    (*logprintf_t)(char* format, ...)
;

logprintf_t 
    logprintf
;

void 
    **ppPluginData
;

extern void 
    *pAMXFunctions
;


PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
   pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
   logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
   logprintf(" >>Regular Expression Plugin v"PLUGIN_VERSION" using Oniguruma v%s. Started.", onig_version());
   return 1;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	onig_end();
	logprintf("Regular Expression Plugin v"PLUGIN_VERSION" using Oniguruma v%s. Stopped.", onig_version());
}

cell AMX_NATIVE_CALL pawn_regex_match(AMX* amx, cell* params)
{
	regex_t* RegExpr;
	const char *rexp = NULL, *string = NULL;
	amx_StrParam(amx, params[1], string);
	amx_StrParam(amx, params[2], rexp);
	if(string && rexp)
	{
		int r=NULL;
		UChar* pattern = (UChar* )rexp;
		OnigErrorInfo einfo;
		r = onig_new(&RegExpr, pattern, pattern + strlen((char* )pattern), ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_PERL, &einfo);
		//logprintf("[REGEX DEBUG]: rexp %s",pattern);
		if(r != ONIG_NORMAL)
		{
			UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(s, r, &einfo);
			logprintf("[REGEX ERROR]: %s", s);
			onig_free(RegExpr);
			return -1;
		}
		UChar* str = (UChar* )string;
		OnigRegion *region;
		region = onig_region_new();
		r = onig_match(RegExpr, str, str+strlen((char*) str), str, region, ONIG_OPTION_NONE);
		//logprintf("[REGEX DEBUG]: string %s",str);
		onig_region_free(region, 1);
		onig_free(RegExpr);
		//logprintf("[REGEX DEBUG]: return %d",r);
		return r;
	}
    return -1337;
}

cell AMX_NATIVE_CALL pawn_regex_search(AMX* amx, cell* params)
{
	regex_t* RegExpr;
	const char *rexp = NULL, *string = NULL;
	cell* addr[2] = {NULL, NULL};
	amx_GetAddr(amx, params[3], &addr[0]);
    amx_GetAddr(amx, params[4], &addr[1]);
	amx_StrParam(amx, params[1], string);
	amx_StrParam(amx, params[2], rexp);
	if(string && rexp)
	{
		int r=NULL;
		UChar* pattern = (UChar* )rexp;
		OnigErrorInfo einfo;
		r = onig_new(&RegExpr, pattern, pattern + strlen((char* )pattern), ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_PERL, &einfo);
		//logprintf("[REGEX DEBUG]: rexp %s",pattern);
		if(r != ONIG_NORMAL)
		{
			UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(s, r, &einfo);
			logprintf("[REGEX ERROR]: %s", s);
			onig_free(RegExpr);
			return -1;
		}
		UChar* str = (UChar* )string;
		OnigRegion *region;
		region = onig_region_new();
		r = onig_search(RegExpr, str, str+strlen((char*) str), str, str+strlen((char*) str), region, ONIG_OPTION_NONE);
		if(r>=0)
		{
			*addr[0]=r;
			*addr[1]=region->end[region->num_regs-1]-1;
		}
		else if(r==ONIG_MISMATCH)
		{
			*addr[1]=-1;
			*addr[1]=-1;
		}
		else
		{
			UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(s, r);
			logprintf("[REGEX ERROR]: %s\n", s);
			onig_region_free(region, 1);
			onig_free(RegExpr);
			return -1;
		}
		//logprintf("[REGEX DEBUG]: string %s",str);
		onig_region_free(region, 1);
		onig_free(RegExpr);
		//logprintf("[REGEX DEBUG]: return %d",r);
		return 1;
	}
    return -1337;
}

cell AMX_NATIVE_CALL pawn_regex_replace(AMX* amx, cell* params)
{
	regex_t* RegExpr;
	const char *rexp = NULL, *string = NULL, *replace = NULL;
	cell* addr = NULL;
	amx_GetAddr(amx, params[1], &addr);
	amx_StrParam(amx, params[1], string);
	amx_StrParam(amx, params[2], rexp);
	amx_StrParam(amx, params[3], replace);
	if(string && rexp)
	{
		int r=NULL;
		UChar* pattern = (UChar* )rexp;
		OnigErrorInfo einfo;
		r = onig_new(&RegExpr, pattern, pattern + strlen((char* )pattern), ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_PERL, &einfo);
		//logprintf("[REGEX DEBUG]: rexp %s",pattern);
		if(r != ONIG_NORMAL)
		{
			UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(s, r, &einfo);
			logprintf("[REGEX ERROR]: %s", s);
			onig_free(RegExpr);
			return -1;
		}
		UChar* str = (UChar* )string;
		OnigRegion *region;
		region = onig_region_new();
		r = onig_search(RegExpr, str, str+strlen((char*) str), str, str+strlen((char*) str), region, ONIG_OPTION_NONE);
		if(r>=0)
		{
			std::string asd = std::string((char*)str);
			asd.replace(asd.begin()+r, asd.begin()+region->end[region->num_regs-1], replace);
			amx_SetString(addr, asd.c_str(), 0, 0, params[1]);
		}
		else if(r<ONIG_MISMATCH)
		{
			UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(s, r);
			logprintf("[REGEX ERROR]: %s\n", s);
			onig_region_free(region, 1);
			onig_free(RegExpr);
			return -1;
		}
		//logprintf("[REGEX DEBUG]: string %s",str);
		onig_region_free(region, 1);
		onig_free(RegExpr);
		//logprintf("[REGEX DEBUG]: return %d",r);
		return r;
	}
    return -1337;
}

AMX_NATIVE_INFO projectNatives[] =
{
	{"regex_match", pawn_regex_match},
	{"regex_search", pawn_regex_search},
	{"regex_replace", pawn_regex_replace},
	{ 0, 0 }
};


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
   return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
   return amx_Register(amx, projectNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
   return AMX_ERR_NONE;
}