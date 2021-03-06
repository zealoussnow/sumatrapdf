#include "mupdf/pdf.h"

/*
	Which fonts are embedded is based on a few preprocessor definitions.

	The base 14 fonts are always embedded.
	For font substitution we embed DroidSans which has good glyph coverage.
	For CJK font substitution we embed DroidSansFallback.

	Set NOCJK to skip all CJK support (this also omits embedding the CJK CMaps)
	Set NOCJKFONT to skip the embedded CJK font.
	Set NOCJKFULL to embed a smaller CJK font without CJK Extension A support.

	Set NODROIDFONT to use the base 14 fonts as substitute fonts.
*/

#ifdef NOCJK
#define NOCJKFONT
#endif

/* SumatraPDF: allow using NASM to include fonts as binary instead of requiring a separate generate.bat build step */
#ifdef NOASMFONTS
#include "gen_font_base14.h"
#else
/* TODO: we need a script that updates the size of the fonts when they change,
   to be run manually after font update. It could be a separate .h file
   (such as gen_font_base14.h) or we could directly change this file. */
extern const unsigned char pdf_font_Dingbats[29728];
extern const unsigned char pdf_font_NimbusMon_Bol[58025];
extern const unsigned char pdf_font_NimbusMon_BolObl[57153];
extern const unsigned char pdf_font_NimbusMon_Obl[51197];
extern const unsigned char pdf_font_NimbusMon_Reg[48993];
extern const unsigned char pdf_font_NimbusRom_Ita[44365];
extern const unsigned char pdf_font_NimbusRom_Med[44349];
extern const unsigned char pdf_font_NimbusRom_MedIta[45901];
extern const unsigned char pdf_font_NimbusRom_Reg[45093];
extern const unsigned char pdf_font_NimbusSan_Bol[31173];
extern const unsigned char pdf_font_NimbusSan_BolIta[35353];
extern const unsigned char pdf_font_NimbusSan_Ita[34733];
extern const unsigned char pdf_font_NimbusSan_Reg[31517];
extern const unsigned char pdf_font_StandardSymL[19828];
#endif

#ifndef NODROIDFONT
#include "gen_font_droid.h"
#endif

#ifndef NOCJKFONT
#ifndef NOCJKFULL
#include "gen_font_cjk_full.h"
#else
#include "gen_font_cjk.h"
#endif
#endif

unsigned char *
pdf_lookup_builtin_font(const char *name, unsigned int *len)
{
	if (!strcmp("Courier", name)) {
		*len = sizeof pdf_font_NimbusMon_Reg;
		return (unsigned char*) pdf_font_NimbusMon_Reg;
	}
	if (!strcmp("Courier-Bold", name)) {
		*len = sizeof pdf_font_NimbusMon_Bol;
		return (unsigned char*) pdf_font_NimbusMon_Bol;
	}
	if (!strcmp("Courier-Oblique", name)) {
		*len = sizeof pdf_font_NimbusMon_Obl;
		return (unsigned char*) pdf_font_NimbusMon_Obl;
	}
	if (!strcmp("Courier-BoldOblique", name)) {
		*len = sizeof pdf_font_NimbusMon_BolObl;
		return (unsigned char*) pdf_font_NimbusMon_BolObl;
	}
	if (!strcmp("Helvetica", name)) {
		*len = sizeof pdf_font_NimbusSan_Reg;
		return (unsigned char*) pdf_font_NimbusSan_Reg;
	}
	if (!strcmp("Helvetica-Bold", name)) {
		*len = sizeof pdf_font_NimbusSan_Bol;
		return (unsigned char*) pdf_font_NimbusSan_Bol;
	}
	if (!strcmp("Helvetica-Oblique", name)) {
		*len = sizeof pdf_font_NimbusSan_Ita;
		return (unsigned char*) pdf_font_NimbusSan_Ita;
	}
	if (!strcmp("Helvetica-BoldOblique", name)) {
		*len = sizeof pdf_font_NimbusSan_BolIta;
		return (unsigned char*) pdf_font_NimbusSan_BolIta;
	}
	if (!strcmp("Times-Roman", name)) {
		*len = sizeof pdf_font_NimbusRom_Reg;
		return (unsigned char*) pdf_font_NimbusRom_Reg;
	}
	if (!strcmp("Times-Bold", name)) {
		*len = sizeof pdf_font_NimbusRom_Med;
		return (unsigned char*) pdf_font_NimbusRom_Med;
	}
	if (!strcmp("Times-Italic", name)) {
		*len = sizeof pdf_font_NimbusRom_Ita;
		return (unsigned char*) pdf_font_NimbusRom_Ita;
	}
	if (!strcmp("Times-BoldItalic", name)) {
		*len = sizeof pdf_font_NimbusRom_MedIta;
		return (unsigned char*) pdf_font_NimbusRom_MedIta;
	}
	if (!strcmp("Symbol", name)) {
		*len = sizeof pdf_font_StandardSymL;
		return (unsigned char*) pdf_font_StandardSymL;
	}
	if (!strcmp("ZapfDingbats", name)) {
		*len = sizeof pdf_font_Dingbats;
		return (unsigned char*) pdf_font_Dingbats;
	}
	*len = 0;
	return NULL;
}

unsigned char *
pdf_lookup_substitute_font(int mono, int serif, int bold, int italic, unsigned int *len)
{
#ifdef NODROIDFONT
	if (mono) {
		if (bold) {
			if (italic) return pdf_lookup_builtin_font("Courier-BoldOblique", len);
			else return pdf_lookup_builtin_font("Courier-Bold", len);
		} else {
			if (italic) return pdf_lookup_builtin_font("Courier-Oblique", len);
			else return pdf_lookup_builtin_font("Courier", len);
		}
	} else if (serif) {
		if (bold) {
			if (italic) return pdf_lookup_builtin_font("Times-BoldItalic", len);
			else return pdf_lookup_builtin_font("Times-Bold", len);
		} else {
			if (italic) return pdf_lookup_builtin_font("Times-Italic", len);
			else return pdf_lookup_builtin_font("Times-Roman", len);
		}
	} else {
		if (bold) {
			if (italic) return pdf_lookup_builtin_font("Helvetica-BoldOblique", len);
			else return pdf_lookup_builtin_font("Helvetica-Bold", len);
		} else {
			if (italic) return pdf_lookup_builtin_font("Helvetica-Oblique", len);
			else return pdf_lookup_builtin_font("Helvetica", len);
		}
	}
#else
	if (mono) {
		*len = sizeof pdf_font_DroidSansMono;
		return (unsigned char*) pdf_font_DroidSansMono;
	} else {
		*len = sizeof pdf_font_DroidSans;
		return (unsigned char*) pdf_font_DroidSans;
	}
#endif
}

unsigned char *
pdf_lookup_substitute_cjk_font(int ros, int serif, int wmode, unsigned int *len, int *index)
{
#ifndef NOCJKFONT
#ifndef NOCJKFULL
	*index = wmode;
	*len = sizeof pdf_font_DroidSansFallbackFull;
	return (unsigned char*) pdf_font_DroidSansFallbackFull;
#else
	*index = wmode;
	*len = sizeof pdf_font_DroidSansFallback;
	return (unsigned char*) pdf_font_DroidSansFallback;
#endif
#else
	*len = 0;
	return NULL;
#endif
}

/* SumatraPDF: also load fonts included with Windows */
#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

// TODO: Use more of FreeType for TTF parsing (for performance reasons,
//       the fonts can't be parsed completely, though)
#include <ft2build.h>
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TAGS_H

#define TTC_VERSION1	0x00010000
#define TTC_VERSION2	0x00020000

#define MAX_FACENAME	128

// Note: the font face must be the first field so that the structure
//       can be treated like a simple string for searching
typedef struct pdf_fontmapMS_s
{
	char fontface[MAX_FACENAME];
	char fontpath[MAX_PATH];
	int index;
} pdf_fontmapMS;

typedef struct pdf_fontlistMS_s
{
	pdf_fontmapMS *fontmap;
	int len;
	int cap;
} pdf_fontlistMS;

typedef struct _tagTT_OFFSET_TABLE
{
	ULONG	uVersion;
	USHORT	uNumOfTables;
	USHORT	uSearchRange;
	USHORT	uEntrySelector;
	USHORT	uRangeShift;
} TT_OFFSET_TABLE;

typedef struct _tagTT_TABLE_DIRECTORY
{
	ULONG	uTag;				//table name
	ULONG	uCheckSum;			//Check sum
	ULONG	uOffset;			//Offset from beginning of file
	ULONG	uLength;			//length of the table in bytes
} TT_TABLE_DIRECTORY;

typedef struct _tagTT_NAME_TABLE_HEADER
{
	USHORT	uFSelector;			//format selector. Always 0
	USHORT	uNRCount;			//Name Records count
	USHORT	uStorageOffset;		//Offset for strings storage, from start of the table
} TT_NAME_TABLE_HEADER;

typedef struct _tagTT_NAME_RECORD
{
	USHORT	uPlatformID;
	USHORT	uEncodingID;
	USHORT	uLanguageID;
	USHORT	uNameID;
	USHORT	uStringLength;
	USHORT	uStringOffset;	//from start of storage area
} TT_NAME_RECORD;

typedef struct _tagFONT_COLLECTION
{
	ULONG	Tag;
	ULONG	Version;
	ULONG	NumFonts;
} FONT_COLLECTION;

static struct {
	char *name;
	char *pattern;
} baseSubstitutes[] = {
	{ "Courier", "CourierNewPSMT" },
	{ "Courier-Bold", "CourierNewPS-BoldMT" },
	{ "Courier-Oblique", "CourierNewPS-ItalicMT" },
	{ "Courier-BoldOblique", "CourierNewPS-BoldItalicMT" },
	{ "Helvetica", "ArialMT" },
	{ "Helvetica-Bold", "Arial-BoldMT" },
	{ "Helvetica-Oblique", "Arial-ItalicMT" },
	{ "Helvetica-BoldOblique", "Arial-BoldItalicMT" },
	{ "Times-Roman", "TimesNewRomanPSMT" },
	{ "Times-Bold", "TimesNewRomanPS-BoldMT" },
	{ "Times-Italic", "TimesNewRomanPS-ItalicMT" },
	{ "Times-BoldItalic", "TimesNewRomanPS-BoldItalicMT" },
	{ "Symbol", "SymbolMT" },
};

static pdf_fontlistMS fontlistMS =
{
	NULL,
	0,
	0,
};

static inline USHORT BEtoHs(USHORT x)
{
	BYTE *data = (BYTE *)&x;
	return (data[0] << 8) | data[1];
}

static inline ULONG BEtoHl(ULONG x)
{
	BYTE *data = (BYTE *)&x;
	return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

/* A little bit more sophisticated name matching so that e.g. "EurostileExtended"
   matches "EurostileExtended-Roman" or "Tahoma-Bold,Bold" matches "Tahoma-Bold" */
static int
lookup_compare(const void *elem1, const void *elem2)
{
	const char *val1 = elem1;
	const char *val2 = elem2;
	int len1 = strlen(val1);
	int len2 = strlen(val2);

	if (len1 != len2)
	{
		const char *rest = len1 > len2 ? val1 + len2 : val2 + len1;
		if (',' == *rest || !_stricmp(rest, "-roman"))
			return _strnicmp(val1, val2, fz_mini(len1, len2));
	}

	return _stricmp(val1, val2);
}

static void
remove_spaces(char *srcDest)
{
	char *dest;

	for (dest = srcDest; *srcDest; srcDest++)
		if (*srcDest != ' ')
			*dest++ = *srcDest;
	*dest = '\0';
}

static int
str_ends_with(const char *str, const char *end)
{
	size_t len1 = strlen(str);
	size_t len2 = strlen(end);

	return len1 >= len2 && !strcmp(str + len1 - len2, end);
}

static pdf_fontmapMS *
pdf_find_windows_font_path(const char *fontname)
{
	return bsearch(fontname, fontlistMS.fontmap, fontlistMS.len, sizeof(pdf_fontmapMS), lookup_compare);
}

/* source and dest can be same */
static void
decode_unicode_BE(fz_context *ctx, char *source, int sourcelen, char *dest, int destlen)
{
	WCHAR *tmp;
	int converted, i;

	if (sourcelen % 2 != 0)
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : invalid unicode string");

	tmp = fz_malloc_array(ctx, sourcelen / 2 + 1, sizeof(WCHAR));
	for (i = 0; i < sourcelen / 2; i++)
		tmp[i] = BEtoHs(((WCHAR *)source)[i]);
	tmp[sourcelen / 2] = '\0';

	converted = WideCharToMultiByte(CP_UTF8, 0, tmp, -1, dest, destlen, NULL, NULL);
	fz_free(ctx, tmp);
	if (!converted)
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : invalid unicode string");
}

static void
decode_platform_string(fz_context *ctx, int platform, int enctype, char *source, int sourcelen, char *dest, int destlen)
{
	switch (platform)
	{
	case TT_PLATFORM_APPLE_UNICODE:
		switch (enctype)
		{
		case TT_APPLE_ID_DEFAULT:
		case TT_APPLE_ID_UNICODE_2_0:
			decode_unicode_BE(ctx, source, sourcelen, dest, destlen);
			return;
		}
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : unsupported encoding (%d/%d)", platform, enctype);
	case TT_PLATFORM_MACINTOSH:
		switch (enctype)
		{
		case TT_MAC_ID_ROMAN:
			if (sourcelen + 1 > destlen)
				fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : overlong fontname: %s", source);
			// TODO: Convert to UTF-8 from what encoding?
			memcpy(dest, source, sourcelen);
			dest[sourcelen] = 0;
			return;
		}
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : unsupported encoding (%d/%d)", platform, enctype);
	case TT_PLATFORM_MICROSOFT:
		switch (enctype)
		{
		case TT_MS_ID_SYMBOL_CS:
		case TT_MS_ID_UNICODE_CS:
		case TT_MS_ID_UCS_4:
			decode_unicode_BE(ctx, source, sourcelen, dest, destlen);
			return;
		}
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : unsupported encoding (%d/%d)", platform, enctype);
	default:
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : unsupported encoding (%d/%d)", platform, enctype);
	}
}

static void
grow_system_font_list(fz_context *ctx, pdf_fontlistMS *fl)
{
	int newcap;
	pdf_fontmapMS *newitems;

	if (fl->cap == 0)
		newcap = 1024;
	else
		newcap = fl->cap * 2;

	// use realloc/free for the fontmap, since the list can
	// remain in memory even with all fz_contexts destroyed
	newitems = realloc(fl->fontmap, newcap * sizeof(pdf_fontmapMS));
	if (!newitems)
		fz_throw(ctx, FZ_ERROR_GENERIC, "OOM in grow_system_font_list");
	memset(newitems + fl->cap, 0, sizeof(pdf_fontmapMS) * (newcap - fl->cap));

	fl->fontmap = newitems;
	fl->cap = newcap;
}

static void
append_mapping(fz_context *ctx, pdf_fontlistMS *fl, const char *facename, const char *path, int index)
{
	if (fl->len == fl->cap)
		grow_system_font_list(ctx, fl);

	if (fl->len >= fl->cap)
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : fontlist overflow");

	fz_strlcpy(fl->fontmap[fl->len].fontface, facename, sizeof(fl->fontmap[0].fontface));
	fz_strlcpy(fl->fontmap[fl->len].fontpath, path, sizeof(fl->fontmap[0].fontpath));
	fl->fontmap[fl->len].index = index;

	++fl->len;
}

static void
safe_read(fz_stream *file, int offset, char *buf, int size)
{
	int n;
	fz_seek(file, offset, 0);
	n = fz_read(file, (unsigned char *)buf, size);
	if (n != size)
		fz_throw(file->ctx, FZ_ERROR_GENERIC, "safe_read: read %d, expected %d", n, size);
}

static void
read_ttf_string(fz_stream *file, int offset, TT_NAME_RECORD *ttRecordBE, char *buf, int size)
{
	char szTemp[MAX_FACENAME * 2];
	// ignore empty and overlong strings
	int stringLength = BEtoHs(ttRecordBE->uStringLength);
	if (stringLength == 0 || stringLength >= sizeof(szTemp))
		return;

	safe_read(file, offset + BEtoHs(ttRecordBE->uStringOffset), szTemp, stringLength);
	decode_platform_string(file->ctx, BEtoHs(ttRecordBE->uPlatformID),
		BEtoHs(ttRecordBE->uEncodingID), szTemp, stringLength, buf, size);
}

static void
makeFakePSName(char szName[MAX_FACENAME], const char *szStyle)
{
	// append the font's subfamily, unless it's a Regular font
	if (*szStyle && _stricmp(szStyle, "Regular") != 0)
	{
		fz_strlcat(szName, "-", MAX_FACENAME);
		fz_strlcat(szName, szStyle, MAX_FACENAME);
	}
	remove_spaces(szName);
}

static void
parseTTF(fz_stream *file, int offset, int index, const char *path)
{
	TT_OFFSET_TABLE ttOffsetTableBE;
	TT_TABLE_DIRECTORY tblDirBE;
	TT_NAME_TABLE_HEADER ttNTHeaderBE;
	TT_NAME_RECORD ttRecordBE;

	char szPSName[MAX_FACENAME] = { 0 };
	char szTTName[MAX_FACENAME] = { 0 };
	char szStyle[MAX_FACENAME] = { 0 };
	char szCJKName[MAX_FACENAME] = { 0 };
	int i, count, tblOffset;

	safe_read(file, offset, (char *)&ttOffsetTableBE, sizeof(TT_OFFSET_TABLE));

	// check if this is a TrueType font of version 1.0 or an OpenType font
	if (BEtoHl(ttOffsetTableBE.uVersion) != TTC_VERSION1 &&
		BEtoHl(ttOffsetTableBE.uVersion) != TTAG_OTTO)
	{
		fz_throw(file->ctx, FZ_ERROR_GENERIC, "fonterror : invalid font version %x", BEtoHl(ttOffsetTableBE.uVersion));
	}

	// determine the name table's offset by iterating through the offset table
	count = BEtoHs(ttOffsetTableBE.uNumOfTables);
	for (i = 0; i < count; i++)
	{
		int entryOffset = offset + sizeof(TT_OFFSET_TABLE) + i * sizeof(TT_TABLE_DIRECTORY);
		safe_read(file, entryOffset, (char *)&tblDirBE, sizeof(TT_TABLE_DIRECTORY));
		if (!BEtoHl(tblDirBE.uTag) || BEtoHl(tblDirBE.uTag) == TTAG_name)
			break;
	}
	if (count == i || !BEtoHl(tblDirBE.uTag))
		fz_throw(file->ctx, FZ_ERROR_GENERIC, "fonterror : nameless font");
	tblOffset = BEtoHl(tblDirBE.uOffset);

	// read the 'name' table for record count and offsets
	safe_read(file, tblOffset, (char *)&ttNTHeaderBE, sizeof(TT_NAME_TABLE_HEADER));
	offset = tblOffset + sizeof(TT_NAME_TABLE_HEADER);
	tblOffset += BEtoHs(ttNTHeaderBE.uStorageOffset);

	// read through the strings for PostScript name and font family
	count = BEtoHs(ttNTHeaderBE.uNRCount);
	for (i = 0; i < count; i++)
	{
		short langId, nameId;
		BOOL isCJKName;

		safe_read(file, offset + i * sizeof(TT_NAME_RECORD), (char *)&ttRecordBE, sizeof(TT_NAME_RECORD));

		langId = BEtoHs(ttRecordBE.uLanguageID);
		nameId = BEtoHs(ttRecordBE.uNameID);
		isCJKName = TT_NAME_ID_FONT_FAMILY == nameId && LANG_CHINESE == PRIMARYLANGID(langId);

		// ignore non-English strings (except for Chinese font names)
		if (langId && langId != TT_MS_LANGID_ENGLISH_UNITED_STATES && !isCJKName)
			continue;
		// ignore names other than font (sub)family and PostScript name
		fz_try(file->ctx)
		{
			if (isCJKName)
				read_ttf_string(file, tblOffset, &ttRecordBE, szCJKName, sizeof(szCJKName));
			else if (TT_NAME_ID_FONT_FAMILY == nameId)
				read_ttf_string(file, tblOffset, &ttRecordBE, szTTName, sizeof(szTTName));
			else if (TT_NAME_ID_FONT_SUBFAMILY == nameId)
				read_ttf_string(file, tblOffset, &ttRecordBE, szStyle, sizeof(szStyle));
			else if (TT_NAME_ID_PS_NAME == nameId)
				read_ttf_string(file, tblOffset, &ttRecordBE, szPSName, sizeof(szPSName));
		}
		fz_catch(file->ctx)
		{
			fz_warn(file->ctx, "ignoring face name decoding fonterror");
		}
	}

	// try to prevent non-Arial fonts from accidentally substituting Arial
	if (!strcmp(szPSName, "ArialMT"))
	{
		// cf. https://code.google.com/p/sumatrapdf/issues/detail?id=2471
		if (strcmp(szTTName, "Arial") != 0)
			szPSName[0] = '\0';
		// TODO: is there a better way to distinguish Arial Caps from Arial proper?
		// cf. http://code.google.com/p/sumatrapdf/issues/detail?id=1290
		else if (strstr(path, "caps") || strstr(path, "Caps"))
			fz_throw(file->ctx, FZ_ERROR_GENERIC, "ignore %s, as it can't be distinguished from Arial,Regular", path);
	}

	if (szPSName[0])
		append_mapping(file->ctx, &fontlistMS, szPSName, path, index);
	if (szTTName[0])
	{
		// derive a PostScript-like name and add it, if it's different from the font's
		// included PostScript name; cf. http://code.google.com/p/sumatrapdf/issues/detail?id=376
		makeFakePSName(szTTName, szStyle);
		// compare the two names before adding this one
		if (lookup_compare(szTTName, szPSName))
			append_mapping(file->ctx, &fontlistMS, szTTName, path, index);
	}
	if (szCJKName[0])
	{
		makeFakePSName(szCJKName, szStyle);
		if (lookup_compare(szCJKName, szPSName) && lookup_compare(szCJKName, szTTName))
			append_mapping(file->ctx, &fontlistMS, szCJKName, path, index);
	}
}

static void
parseTTFs(fz_context *ctx, const char *path)
{
	fz_stream *file = fz_open_file(ctx, path);
	/* "fonterror : %s not found", path */
	fz_try(ctx)
	{
		parseTTF(file, 0, 0, path);
	}
	fz_always(ctx)
	{
		fz_close(file);
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}
}

static void
parseTTCs(fz_context *ctx, const char *path)
{
	FONT_COLLECTION fontcollectionBE;
	ULONG i, numFonts, *offsettableBE = NULL;

	fz_stream *file = fz_open_file(ctx, path);
	/* "fonterror : %s not found", path */

	fz_var(offsettableBE);

	fz_try(ctx)
	{
		safe_read(file, 0, (char *)&fontcollectionBE, sizeof(FONT_COLLECTION));
		if (BEtoHl(fontcollectionBE.Tag) != TTAG_ttcf)
			fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : wrong format %x", BEtoHl(fontcollectionBE.Tag));
		if (BEtoHl(fontcollectionBE.Version) != TTC_VERSION1 &&
			BEtoHl(fontcollectionBE.Version) != TTC_VERSION2)
		{
			fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror : invalid version %x", BEtoHl(fontcollectionBE.Version));
		}

		numFonts = BEtoHl(fontcollectionBE.NumFonts);
		offsettableBE = fz_malloc_array(ctx, numFonts, sizeof(ULONG));

		safe_read(file, sizeof(FONT_COLLECTION), (char *)offsettableBE, numFonts * sizeof(ULONG));
		for (i = 0; i < numFonts; i++)
			parseTTF(file, BEtoHl(offsettableBE[i]), i, path);
	}
	fz_always(ctx)
	{
		fz_free(ctx, offsettableBE);
		fz_close(file);
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}
}

static void
extend_system_font_list(fz_context *ctx, const WCHAR *path)
{
	WCHAR szPath[MAX_PATH], *lpFileName;
	WIN32_FIND_DATA FileData;
	HANDLE hList;

	GetFullPathName(path, nelem(szPath), szPath, &lpFileName);

	hList = FindFirstFile(szPath, &FileData);
	if (hList == INVALID_HANDLE_VALUE)
	{
		// Don't complain about missing directories
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return;
		fz_throw(ctx, FZ_ERROR_GENERIC, "extend_system_font_list: unknown error %d", GetLastError());
	}
	do
	{
		if (!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char szPathUtf8[MAX_PATH], *fileExt;
			int res;
			lstrcpyn(lpFileName, FileData.cFileName, szPath + MAX_PATH - lpFileName);
			res = WideCharToMultiByte(CP_UTF8, 0, szPath, -1, szPathUtf8, sizeof(szPathUtf8), NULL, NULL);
			if (!res)
			{
				fz_warn(ctx, "WideCharToMultiByte failed for %S", szPath);
				continue;
			}
			fileExt = szPathUtf8 + strlen(szPathUtf8) - 4;
			fz_try(ctx)
			{
				if (!_stricmp(fileExt, ".ttc"))
					parseTTCs(ctx, szPathUtf8);
				else if (!_stricmp(fileExt, ".ttf") || !_stricmp(fileExt, ".otf"))
					parseTTFs(ctx, szPathUtf8);
			}
			fz_catch(ctx)
			{
				// ignore errors occurring while parsing a given font file
			}
		}
	} while (FindNextFile(hList, &FileData));
	FindClose(hList);
}

static void
destroy_system_font_list(void)
{
	free(fontlistMS.fontmap);
	memset(&fontlistMS, 0, sizeof(fontlistMS));
}

// cf. http://blogs.msdn.com/b/oldnewthing/archive/2004/10/25/247180.aspx
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define CURRENT_HMODULE ((HMODULE)&__ImageBase)

static void
create_system_font_list(fz_context *ctx)
{
	WCHAR szFontDir[MAX_PATH];
	UINT cch;

	cch = GetWindowsDirectory(szFontDir, nelem(szFontDir) - 12);
	if (0 < cch && cch < nelem(szFontDir) - 12)
	{
		wcscat_s(szFontDir, MAX_PATH, L"\\Fonts\\*.?t?");
		extend_system_font_list(ctx, szFontDir);
	}

	if (fontlistMS.len == 0)
		fz_warn(ctx, "couldn't find any usable system fonts");

#ifdef NOCJKFONT
	{
		// If no CJK fallback font is builtin but one has been shipped separately (in the same
		// directory as the main executable), add it to the list of loadable system fonts
		WCHAR szFile[MAX_PATH], *lpFileName;
		szFile[0] = '\0';
		GetModuleFileName(CURRENT_HMODULE, szFontDir, MAX_PATH);
		szFontDir[nelem(szFontDir) - 1] = '\0';
		GetFullPathName(szFontDir, MAX_PATH, szFile, &lpFileName);
		lstrcpyn(lpFileName, L"DroidSansFallback.ttf", szFile + MAX_PATH - lpFileName);
		extend_system_font_list(ctx, szFile);
	}
#endif

	// sort the font list, so that it can be searched binarily
	qsort(fontlistMS.fontmap, fontlistMS.len, sizeof(pdf_fontmapMS), _stricmp);

#ifdef DEBUG
	// allow to overwrite system fonts for debugging purposes
	// (either pass a full path or a search pattern such as "fonts\*.ttf")
	cch = GetEnvironmentVariable(L"MUPDF_FONTS_PATTERN", szFontDir, nelem(szFontDir));
	if (0 < cch && cch < nelem(szFontDir))
	{
		int i, prev_len = fontlistMS.len;
		extend_system_font_list(ctx, szFontDir);
		for (i = prev_len; i < fontlistMS.len; i++)
		{
			pdf_fontmapMS *entry = bsearch(fontlistMS.fontmap[i].fontface, fontlistMS.fontmap, prev_len, sizeof(pdf_fontmapMS), lookup_compare);
			if (entry)
				*entry = fontlistMS.fontmap[i];
		}
		qsort(fontlistMS.fontmap, fontlistMS.len, sizeof(pdf_fontmapMS), _stricmp);
	}
#endif

	// make sure to clean up after ourselves
	atexit(destroy_system_font_list);
}

static fz_font *
pdf_load_windows_font_by_name(fz_context *ctx, const char *orig_name)
{
	pdf_fontmapMS *found = NULL;
	char *comma, *fontname;
	fz_font *font;

	fz_synchronize_begin();
	if (fontlistMS.len == 0)
	{
		fz_try(ctx)
		{
			create_system_font_list(ctx);
		}
		fz_catch(ctx) { }
	}
	fz_synchronize_end();
	if (fontlistMS.len == 0)
		fz_throw(ctx, FZ_ERROR_GENERIC, "fonterror: couldn't find any fonts");

	// work on a normalized copy of the font name
	fontname = fz_strdup(ctx, orig_name);
	remove_spaces(fontname);

	// first, try to find the exact font name (including appended style information)
	comma = strchr(fontname, ',');
	if (comma)
	{
		*comma = '-';
		found = pdf_find_windows_font_path(fontname);
		*comma = ',';
	}
	// second, substitute the font name with a known PostScript name
	else
	{
		int i;
		for (i = 0; i < nelem(baseSubstitutes) && !found; i++)
			if (!strcmp(fontname, baseSubstitutes[i].name))
				found = pdf_find_windows_font_path(baseSubstitutes[i].pattern);
	}
	// third, search for the font name without additional style information
	if (!found)
		found = pdf_find_windows_font_path(fontname);
	// fourth, try to separate style from basename for prestyled fonts (e.g. "ArialBold")
	if (!found && !comma && (str_ends_with(fontname, "Bold") || str_ends_with(fontname, "Italic")))
	{
		int styleLen = str_ends_with(fontname, "Bold") ? 4 : str_ends_with(fontname, "BoldItalic") ? 10 : 6;
		fontname = fz_resize_array(ctx, fontname, strlen(fontname) + 2, sizeof(char));
		comma = fontname + strlen(fontname) - styleLen;
		memmove(comma + 1, comma, styleLen + 1);
		*comma = '-';
		found = pdf_find_windows_font_path(fontname);
		*comma = ',';
		if (!found)
			found = pdf_find_windows_font_path(fontname);
	}
	// fifth, try to convert the font name from the common Chinese codepage 936
	if (!found && fontname[0] < 0)
	{
		WCHAR cjkNameW[MAX_FACENAME];
		char cjkName[MAX_FACENAME];
		if (MultiByteToWideChar(936, MB_ERR_INVALID_CHARS, fontname, -1, cjkNameW, nelem(cjkNameW)) &&
			WideCharToMultiByte(CP_UTF8, 0, cjkNameW, -1, cjkName, nelem(cjkName), NULL, NULL))
		{
			comma = strchr(cjkName, ',');
			if (comma)
			{
				*comma = '-';
				found = pdf_find_windows_font_path(cjkName);
				*comma = ',';
			}
			if (!found)
				found = pdf_find_windows_font_path(cjkName);
		}
	}

	fz_free(ctx, fontname);
	if (!found)
		fz_throw(ctx, FZ_ERROR_GENERIC, "couldn't find system font '%s'", orig_name);

	fz_warn(ctx, "loading non-embedded font '%s' from '%s'", orig_name, found->fontpath);

	font = fz_new_font_from_file(ctx, orig_name, found->fontpath, found->index,
		strcmp(found->fontface, "DroidSansFallback") != 0);
	font->ft_substitute = 1;
	return font;
}

static fz_font *
pdf_load_windows_font(fz_context *ctx, const char *fontname, int bold, int italic, int needs_exact_metrics)
{
	fz_font *font;
	const char *clean_name = pdf_clean_base14_name(fontname);

	if (needs_exact_metrics)
	{
		/* TODO: the metrics for Times-Roman and Courier don't match
		   those of Windows' Times New Roman and Courier New; for
		   some reason, Poppler doesn't seem to have this problem */
		unsigned int len;
		if (pdf_lookup_builtin_font(fontname, &len))
			return NULL;

		/* cf. http://code.google.com/p/sumatrapdf/issues/detail?id=2173 */
		if (clean_name != fontname && !strncmp(clean_name, "Times-", 6))
			return NULL;
	}

	font = pdf_load_windows_font_by_name(ctx, fontname);
	/* use the font's own metrics for base 14 fonts */
	if (clean_name != fontname)
		font->ft_substitute = 0;
	return font;
}

static fz_font *
pdf_load_windows_cjk_font(fz_context *ctx, const char *fontname, int ros, int serif)
{
	fz_font *font;

	/* try to find a matching system font before falling back to an approximate one */
	fz_try(ctx)
	{
		font = pdf_load_windows_font_by_name(ctx, fontname);
	}
	fz_catch(ctx)
	{
		font = NULL;
	}
	if (font)
		return font;

	/* try to fall back to a reasonable system font */
	fz_try(ctx)
	{
		if (serif)
		{
			switch (ros)
			{
			case FZ_ADOBE_CNS_1: font = pdf_load_windows_font_by_name(ctx, "MingLiU"); break;
			case FZ_ADOBE_GB_1: font = pdf_load_windows_font_by_name(ctx, "SimSun"); break;
			case FZ_ADOBE_JAPAN_1: font = pdf_load_windows_font_by_name(ctx, "MS-Mincho"); break;
			case FZ_ADOBE_KOREA_1: font = pdf_load_windows_font_by_name(ctx, "Batang"); break;
			default: fz_throw(ctx, FZ_ERROR_GENERIC, "invalid serif ros");
			}
		}
		else
		{
			switch (ros)
			{
			case FZ_ADOBE_CNS_1: font = pdf_load_windows_font_by_name(ctx, "DFKaiShu-SB-Estd-BF"); break;
			case FZ_ADOBE_GB_1:
				fz_try(ctx)
				{
					font = pdf_load_windows_font_by_name(ctx, "KaiTi");
				}
				fz_catch(ctx)
				{
					font = pdf_load_windows_font_by_name(ctx, "KaiTi_GB2312");
				}
				break;
			case FZ_ADOBE_JAPAN_1: font = pdf_load_windows_font_by_name(ctx, "MS-Gothic"); break;
			case FZ_ADOBE_KOREA_1: font = pdf_load_windows_font_by_name(ctx, "Gulim"); break;
			default: fz_throw(ctx, FZ_ERROR_GENERIC, "invalid sans-serif ros");
			}
		}
	}
	fz_catch(ctx)
	{
#ifdef NOCJKFONT
		/* If no CJK fallback font is builtin, maybe one has been shipped separately */
		font = pdf_load_windows_font_by_name(ctx, "DroidSansFallback");
#else
		fz_rethrow(ctx);
#endif
	}

	return font;
}

#endif

void pdf_install_load_system_font_funcs(fz_context *ctx)
{
#ifdef _WIN32
	fz_install_load_system_font_funcs(ctx, pdf_load_windows_font, pdf_load_windows_cjk_font);
#endif
}
