				///////////////////////////////////////////////////////////////////////////
				//				Author:	Koala											 //
				//				Release Website: Gta-Mp.ro								 //
				//				Release Date: 18/07/2014								 //
				//				Credits: -maddinat0r (Got the SDK from his github)		 //
				//						 -Incognito (I learned a lot from his github)	 //
				//						 -K.Kosako (Author of Oniguruma library)		 //
				//				Description: This plugins allows PAWN					 //
				//							use of the basic regex functions.			 //
				//                                                                       //
				//						Regular Expressions Library						 //
				//												v0.2                     //
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
#define PLUGIN_VERSION "0.2"

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

struct RegExp{
	int id;
	regex_t* RegExpr;
	OnigRegion* zreg;
}rexpression[101];
int TotalExpressions=0;


PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf("");
	logprintf(" ------------------------------");
	logprintf(" |");
	logprintf(" | -> Regular Expression Plugin v"PLUGIN_VERSION"");
	logprintf(" | -> Using Oniguruma v%s", onig_version());
	logprintf(" |");
	logprintf(" ------ (c) 2014, Koala -------");
	logprintf("");
	return 1;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("Regular Expression Plugin succesfully unloaded. (c) 2014, Koala");
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
			*addr[0]=-1;
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

cell AMX_NATIVE_CALL pawn_regex_end(AMX* amx, cell* params)
{
	for(int i=0; i<TotalExpressions; i++)
	{
		delete (int*)rexpression[i].id;
		onig_free(rexpression[i].RegExpr);
		onig_region_free(rexpression[i].zreg, 1);
	}
	//delete (int*)TotalExpressions;
	onig_end();
	return -1337;
}

cell AMX_NATIVE_CALL pawn_regex_def_syntax(AMX* amx, cell* params)
{
	switch(params[1])
	{
		case 1:
		{
			onig_set_default_syntax(ONIG_SYNTAX_PERL);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_PERL.");
			return 1;
		}
		case 2:
		{
			onig_set_default_syntax(ONIG_SYNTAX_RUBY);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_RUBY.");
			return 1;
		}
		case 3:
		{
			onig_set_default_syntax(ONIG_SYNTAX_JAVA);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_JAVA.");
			return 1;
		}
		case 4:
		{
			onig_set_default_syntax(ONIG_SYNTAX_GNU_REGEX);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_GNU_REGEX.");
			return 1;
		}
		case 5:
		{
			onig_set_default_syntax(ONIG_SYNTAX_GREP);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_GREP.");
			return 1;
		}
		case 6:
		{
			onig_set_default_syntax(ONIG_SYNTAX_EMACS);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_EMACS.");
			return 1;
		}
		case 7:
		{
			onig_set_default_syntax(ONIG_SYNTAX_POSIX_EXTENDED);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_POSIX_EXTENDED.");
			return 1;
		}
		case 8:
		{
			onig_set_default_syntax(ONIG_SYNTAX_POSIX_BASIC);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_POSIX_BASIC.");
			return 1;
		}
		default:
		{
			onig_set_default_syntax(ONIG_SYNTAX_PERL);
			logprintf("[REGEX] Default syntax set to ONIG_SYNTAX_PERL.");
			return 1;
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL pawn_regex_build(AMX* amx, cell* params)
{
	const char *rexp = NULL;
	amx_StrParam(amx, params[1], rexp);
	if(rexp)
	{
		UChar* pattern = (UChar* )rexp;
		OnigErrorInfo einfo;
		rexpression[TotalExpressions].id = onig_new(&rexpression[TotalExpressions].RegExpr, pattern, pattern + strlen(rexp), ONIG_OPTION_NONE, ONIG_ENCODING_UTF8, ONIG_SYNTAX_DEFAULT, &einfo);
		if(rexpression[TotalExpressions].id != ONIG_NORMAL)
		{
			UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(s, rexpression[TotalExpressions].id, &einfo);
			logprintf("[REGEX ERROR]: %s | at index %d. regex_exbuild aborted.", s, TotalExpressions);
			onig_free(rexpression[TotalExpressions].RegExpr);
			return -1;
		}
		rexpression[TotalExpressions].zreg = onig_region_new();
		return TotalExpressions++;
	}
    return -1337;
}

cell AMX_NATIVE_CALL pawn_regex_set(AMX* amx, cell* params)
{
	const char *rexp = NULL;
	amx_StrParam(amx, params[1], rexp);
	if(rexp)
	{
		int aidi=(int)params[2];
		//logprintf("aidi %d", aidi);
		if(aidi>=0 && aidi<TotalExpressions)
		{
			onig_free(rexpression[aidi].RegExpr);
			onig_region_free(rexpression[aidi].zreg, 1);
			UChar* pattern = (UChar* )rexp;
			OnigErrorInfo einfo;
			rexpression[aidi].id = onig_new(&rexpression[aidi].RegExpr, pattern, pattern + strlen(rexp), ONIG_OPTION_NONE, ONIG_ENCODING_UTF8, ONIG_SYNTAX_DEFAULT, &einfo);
			if(rexpression[aidi].id != ONIG_NORMAL)
			{
				UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
				onig_error_code_to_str(s, rexpression[aidi].id, &einfo);
				logprintf("[REGEX ERROR]: %s | at index %d. regex_set aborted.", s, aidi);
				onig_free(rexpression[aidi].RegExpr);
				return -1;
			}
			rexpression[aidi].zreg = onig_region_new();
			//logprintf("aidi %d", aidi);
			return 1;
		}
		logprintf("[REGEX ERROR]: Call regex_set with undefined parameter at index %d", aidi);
		return -1;
	}
    return -1337;
}

cell AMX_NATIVE_CALL pawn_regex_exmatch(AMX* amx, cell* params)
{
	const char *string = NULL;
	amx_StrParam(amx, params[1], string);
	if(string)
	{
		int id=(int)params[2];
		if(id>=0 && id<TotalExpressions)
		{
			UChar* str = (UChar* )string;
			return onig_match(rexpression[id].RegExpr, str, str+strlen(string), str, rexpression[id].zreg, ONIG_OPTION_NONE);
		}
		logprintf("[REGEX ERROR]: Call regex_exmatch with undefined parameter at index %d", id);
		return -1;
	}
    return -1337;
}

cell AMX_NATIVE_CALL pawn_regex_exsearch(AMX* amx, cell* params)
{
	const char *string = NULL;
	cell* addr[2] = {NULL, NULL};
	amx_GetAddr(amx, params[3], &addr[0]);
    amx_GetAddr(amx, params[4], &addr[1]);
	amx_StrParam(amx, params[1], string);
	if(string)
	{
		int id=(int)params[2];
		if(id>=0 && id<TotalExpressions)
		{
			int r=NULL;
			UChar* str = (UChar* )string;
			onig_region_clear(rexpression[id].zreg);
			r = onig_search(rexpression[id].RegExpr, str, str+strlen(string), str, str+strlen(string), rexpression[id].zreg, ONIG_OPTION_NONE);
			if(r>=0)
			{
				*addr[0]=r;
				*addr[1]=rexpression[id].zreg->end[rexpression[id].zreg->num_regs-1]-1;
			}
			else if(r==ONIG_MISMATCH)
			{
				*addr[0]=-1;
				*addr[1]=-1;
			}
			else
			{
				UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
				onig_error_code_to_str(s, r);
				logprintf("[REGEX ERROR]: %s\n", s);
				return -1;
			}
			return 1;
		}
		logprintf("[REGEX ERROR]: Call regex_exsearch with undefined parameter at index %d", id);
		return -1;
	}
    return -1337;
}

cell AMX_NATIVE_CALL pawn_regex_exreplace(AMX* amx, cell* params)
{
	const char *string = NULL, *replace = NULL;
	cell* addr = NULL;
	amx_GetAddr(amx, params[1], &addr);
	amx_StrParam(amx, params[1], string);
	amx_StrParam(amx, params[3], replace);
	if(string)
	{
		int id=(int)params[2];
		if(id>=0 && id<TotalExpressions)
		{
			int r=NULL;
			UChar* str = (UChar* )string;
			onig_region_clear(rexpression[id].zreg);
			r = onig_search(rexpression[id].RegExpr, str, str+strlen(string), str, str+strlen(string), rexpression[id].zreg, ONIG_OPTION_NONE);
			if(r>=0)
			{
				std::string asd = std::string(string);
				asd.replace(asd.begin()+r, asd.begin()+rexpression[id].zreg->end[rexpression[id].zreg->num_regs-1], replace);
				amx_SetString(addr, asd.c_str(), 0, 0, params[1]);
			}
			else if(r<ONIG_MISMATCH)
			{
				UChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
				onig_error_code_to_str(s, r);
				logprintf("[REGEX ERROR]: %s\n", s);
				return -1;
			}
			return r;
		}
		logprintf("[REGEX ERROR]: Call regex_exreplace with undefined parameter at index %d", id);
		return -1;
	}
    return -1337;
}

AMX_NATIVE_INFO projectNatives[] =
{
	{"regex_match", pawn_regex_match},
	{"regex_search", pawn_regex_search},
	{"regex_replace", pawn_regex_replace},

	{"regex_end", pawn_regex_end},
	{"regex_syntax", pawn_regex_def_syntax},
	{"regex_exbuild", pawn_regex_build},
	{"regex_set", pawn_regex_set},

	{"regex_exmatch", pawn_regex_exmatch},
	{"regex_exsearch", pawn_regex_exsearch},
	{"regex_exreplace", pawn_regex_exreplace},
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