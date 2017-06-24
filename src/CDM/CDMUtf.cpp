/*****************************************************************************
 *  CDMUtf.cpp:                                                              *
 * ----------------------                                                    *
 *  This file implements some Utf utilities for                              *
 *  Collection and Document Manager (CDM) Public API.                        *
 *                                                                           *
 *  For more details regarding CDM Version consult file "CDM_Version.h"      *
 *  CDM is the Collection and Document Manager and the core component of the *
 *  Ellogon Language Engineering Platform.                                   *
 *  CDM was designed and implemented by Georgios Petasis,                    *
 *  e-mail: petasis@iit.demokritos.gr, petasisg@yahoo.gr, petasisg@gamil.com *
 *  URL:    http://www.ellogon.org/petasis                                   *
 *                                                                           *
 *  Copyright 1998-2017 by:                                                  *
 *  Georgios Petasis,                                                        *
 *  Software and Knowledge Engineering Laboratory,                           *
 *  Institute of Informatics and Telecommunications,                         *
 *  National Center for Scientific Research (N.C.S.R.) "Demokritos",         *
 *  Athens, Greece.                                                          *
 *  E-mail: petasis@iit.demokritos.gr                                        *
 *                                                                           *
 *  This file is part of the "Ellogon" Language Engineering Platform.        *
 *  For licencing information, please refer to the licence conditions of     *
 *  "Ellogon" Language Engineering Platform.                                 *
 *****************************************************************************/

#include <cctype>
#include <locale>
#include "CDM3.h"
#include <string>

/*
 * The following functions are taken from the Tcl/Tk C library.
 */

#ifndef TCL_UTF_MAX
#define TCL_UTF_MAX                3
#endif

#if TCL_UTF_MAX > 4
    /*
     * unsigned int isn't 100% accurate as it should be a strict 4-byte value
     * (perhaps wchar_t). 64-bit systems may have troubles. The size of this
     * value must be reflected correctly in regcustom.h and
     * in tclEncoding.c.
     * XXX: Tcl is currently UCS-2 and planning UTF-16 for the Unicode
     * XXX: string rep that CDM_UniChar represents.  Changing the size
     * XXX: of CDM_UniChar is /not/ supported.
     */
typedef unsigned int CDM_UniChar;
#else
typedef unsigned short CDM_UniChar;
#endif

#ifndef UCHAR
#define UCHAR(c) (static_cast<unsigned char>(c))
#endif

namespace ELEP {
  namespace CDM {
    namespace Utilities {

/*
 *----------------------------------------------------------------------
 *
 * CDM_UniCharToLower --
 *
 *        Compute the lowercase equivalent of the given Unicode character.
 *
 * Results:
 *        Returns the lowercase Unicode character.
 *
 * Side effects:
 *        None.
 *
 *----------------------------------------------------------------------
 */

CDM_UniChar
CDM_UniCharToLower(
    int ch,                        /* Unicode character to convert. */
    const std::locale& locale)            /* Locale to use */
{
    return std::tolower((CDM_UniChar) ch, locale);
}

CDM_UniChar
CDM_UniCharToLower(
    int ch,                        /* Unicode character to convert. */
    const char* locale)            /* Locale to use */
{
    return std::tolower((CDM_UniChar) ch, std::locale(locale));
}

/*
 *---------------------------------------------------------------------------
 *
 * CDM_UtfPrev --
 *
 *        Given a pointer to some current location in a UTF-8 string, move
 *        backwards one character. This works correctly when the pointer is in
 *        the middle of a UTF-8 character.
 *
 * Results:
 *        The return value is a pointer to the previous character in the UTF-8
 *        string. If the current location was already at the beginning of the
 *        string, the return value will also be a pointer to the beginning of
 *        the string.
 *
 * Side effects:
 *        None.
 *
 *---------------------------------------------------------------------------
 */

const char *
CDM_UtfPrev(
    const char *src,           /* The current location in the string. */
    const char *start)         /* Pointer to the beginning of the string, to
                                * avoid going backwards too far. */
{
    const char *look;
    int i, byte;

    src--;
    look = src;
    for (i = 0; i < TCL_UTF_MAX; i++) {
        if (look < start) {
            if (src < start) {
                src = start;
            }
            break;
        }
        byte = *((unsigned char *) look);
        if (byte < 0x80) {
            break;
        }
        if (byte >= 0xC0) {
            return look;
        }
        look--;
    }
    return src;
}


/*
 *---------------------------------------------------------------------------
 *
 * CDM_UtfToUniChar --
 *
 *        Extract the CDM_UniChar represented by the UTF-8 string. Bad UTF-8
 *        sequences are converted to valid CDM_UniChars and processing
 *        continues. Equivalent to Plan 9 chartorune().
 *
 *        The caller must ensure that the source buffer is long enough that this
 *        routine does not run off the end and dereference non-existent memory
 *        looking for trail bytes. If the source buffer is known to be '\0'
 *        terminated, this cannot happen. Otherwise, the caller should call
 *        CDM_UtfCharComplete() before calling this routine to ensure that
 *        enough bytes remain in the string.
 *
 * Results:
 *        *chPtr is filled with the CDM_UniChar, and the return value is the
 *        number of bytes from the UTF-8 string that were consumed.
 *
 * Side effects:
 *        None.
 *
 *---------------------------------------------------------------------------
 */

int
CDM_UtfToUniChar(
    register const char *src,   /* The UTF-8 string. */
    register CDM_UniChar *chPtr)/* Filled with the CDM_UniChar represented by
                                 * the UTF-8 string. */
{
    register int byte;

    /*
     * Unroll 1 to 3 byte UTF-8 sequences, use loop to handle longer ones.
     */

    byte = *((unsigned char *) src);
    if (byte < 0xC0) {
        /*
         * Handles properly formed UTF-8 characters between 0x01 and 0x7F.
         * Also treats \0 and naked trail bytes 0x80 to 0xBF as valid
         * characters representing themselves.
         */

        *chPtr = (CDM_UniChar) byte;
        return 1;
    } else if (byte < 0xE0) {
        if ((src[1] & 0xC0) == 0x80) {
            /*
             * Two-byte-character lead-byte followed by a trail-byte.
             */

            *chPtr = (CDM_UniChar) (((byte & 0x1F) << 6) | (src[1] & 0x3F));
            return 2;
        }

        /*
         * A two-byte-character lead-byte not followed by trail-byte
         * represents itself.
         */
    } else if (byte < 0xF0) {
        if (((src[1] & 0xC0) == 0x80) && ((src[2] & 0xC0) == 0x80)) {
            /*
             * Three-byte-character lead byte followed by two trail bytes.
             */

            *chPtr = (CDM_UniChar) (((byte & 0x0F) << 12)
                    | ((src[1] & 0x3F) << 6) | (src[2] & 0x3F));
            return 3;
        }

        /*
         * A three-byte-character lead-byte not followed by two trail-bytes
         * represents itself.
         */
    }
#if TCL_UTF_MAX > 3
    else if (byte < 0xF8) {
        if (((src[1] & 0xC0) == 0x80) && ((src[2] & 0xC0) == 0x80) && ((src[3] & 0xC0) == 0x80)) {
            /*
             * Four-byte-character lead byte followed by three trail bytes.
             */

            *chPtr = (CDM_UniChar) (((byte & 0x0E) << 18) | ((src[1] & 0x3F) << 12)
                    | ((src[2] & 0x3F) << 6) | (src[3] & 0x3F));
            return 4;
        }

        /*
         * A three-byte-character lead-byte not followed by two trail-bytes
         * represents itself.
         */
    }
#endif

    *chPtr = (CDM_UniChar) byte;
    return 1;
}

/*
 *----------------------------------------------------------------------
 *
 * CDM_StringCaseMatch --
 *
 *        See if a particular string matches a particular pattern. Allows case
 *        insensitivity.
 *
 * Results:
 *        The return value is 1 if string matches pattern, and 0 otherwise. The
 *        matching operation permits the following special characters in the
 *        pattern: *?\[] (see the manual entry for details on what these mean).
 *
 * Side effects:
 *        None.
 *
 *----------------------------------------------------------------------
 */

int
CDM_StringCaseMatch(
    const char *str,            /* String. */
    const char *pattern,        /* Pattern, which may contain special
                                 * characters. */
    int nocase,                 /* 0 for case sensitive, 1 for insensitive */
    const char *locale)         /* locale to be used for std::tolower */
{
    int p, charLen;
    const char *pstart = pattern;
    CDM_UniChar ch1, ch2;
    std::locale loc(locale);

    while (1) {
        p = *pattern;

        /*
         * See if we're at the end of both the pattern and the string. If so,
         * we succeeded. If we're at the end of the pattern but not at the end
         * of the string, we failed.
         */

        if (p == '\0') {
            return (*str == '\0');
        }
        if ((*str == '\0') && (p != '*')) {
            return 0;
        }

        /*
         * Check for a "*" as the next pattern character. It matches any
         * substring. We handle this by calling ourselves recursively for each
         * postfix of string, until either we match or we reach the end of the
         * string.
         */

        if (p == '*') {
            /*
             * Skip all successive *'s in the pattern
             */

            while (*(++pattern) == '*') {}
            p = *pattern;
            if (p == '\0') {
                return 1;
            }

            /*
             * This is a special case optimization for single-byte utf.
             */

            if (UCHAR(*pattern) < 0x80) {
                ch2 = (CDM_UniChar)
                        (nocase ? tolower(UCHAR(*pattern)) : UCHAR(*pattern));
            } else {
                CDM_UtfToUniChar(pattern, &ch2);
                if (nocase) {
                    ch2 = CDM_UniCharToLower(ch2, loc);
                }
            }

            while (1) {
                /*
                 * Optimization for matching - cruise through the string
                 * quickly if the next char in the pattern isn't a special
                 * character
                 */

                if ((p != '[') && (p != '?') && (p != '\\')) {
                    if (nocase) {
                        while (*str) {
                            charLen = CDM_UtfToUniChar(str, &ch1);
                            if (ch2==ch1||ch2==CDM_UniCharToLower(ch1,loc)) {
                                break;
                            }
                            str += charLen;
                        }
                    } else {
                        /*
                         * There's no point in trying to make this code
                         * shorter, as the number of bytes you want to compare
                         * each time is non-constant.
                         */

                        while (*str) {
                            charLen = CDM_UtfToUniChar(str, &ch1);
                            if (ch2 == ch1) {
                                break;
                            }
                            str += charLen;
                        }
                    }
                }
                if (CDM_StringCaseMatch(str, pattern, nocase)) {
                    return 1;
                }
                if (*str == '\0') {
                    return 0;
                }
                str += CDM_UtfToUniChar(str, &ch1);
            }
        }

        /*
         * Check for a "?" as the next pattern character. It matches any
         * single character.
         */

        if (p == '?') {
            pattern++;
            str += CDM_UtfToUniChar(str, &ch1);
            continue;
        }

        /*
         * Check for a "[" as the next pattern character. It is followed by a
         * list of characters that are acceptable, or by a range (two
         * characters separated by "-").
         */

        if (p == '[') {
            CDM_UniChar startChar, endChar;

            pattern++;
            if (UCHAR(*str) < 0x80) {
                ch1 = (CDM_UniChar)
                        (nocase ? tolower(UCHAR(*str)) : UCHAR(*str));
                str++;
            } else {
                str += CDM_UtfToUniChar(str, &ch1);
                if (nocase) {
                    ch1 = CDM_UniCharToLower(ch1, loc);
                }
            }
            while (1) {
                if ((*pattern == ']') || (*pattern == '\0')) {
                    return 0;
                }
                if (UCHAR(*pattern) < 0x80) {
                    startChar = (CDM_UniChar) (nocase
                            ? tolower(UCHAR(*pattern)) : UCHAR(*pattern));
                    pattern++;
                } else {
                    pattern += CDM_UtfToUniChar(pattern, &startChar);
                    if (nocase) {
                        startChar = CDM_UniCharToLower(startChar, loc);
                    }
                }
                if (*pattern == '-') {
                    pattern++;
                    if (*pattern == '\0') {
                        return 0;
                    }
                    if (UCHAR(*pattern) < 0x80) {
                        endChar = (CDM_UniChar) (nocase
                                ? tolower(UCHAR(*pattern)) : UCHAR(*pattern));
                        pattern++;
                    } else {
                        pattern += CDM_UtfToUniChar(pattern, &endChar);
                        if (nocase) {
                            endChar = CDM_UniCharToLower(endChar, loc);
                        }
                    }
                    if (((startChar <= ch1) && (ch1 <= endChar))
                            || ((endChar <= ch1) && (ch1 <= startChar))) {
                        /*
                         * Matches ranges of form [a-z] or [z-a].
                         */

                        break;
                    }
                } else if (startChar == ch1) {
                    break;
                }
            }
            while (*pattern != ']') {
                if (*pattern == '\0') {
                    pattern = CDM_UtfPrev(pattern, pstart);
                    break;
                }
                pattern++;
            }
            pattern++;
            continue;
        }

        /*
         * If the next pattern character is '\', just strip off the '\' so we
         * do exact matching on the character that follows.
         */

        if (p == '\\') {
            pattern++;
            if (*pattern == '\0') {
                return 0;
            }
        }

        /*
         * There's no special character. Just make sure that the next bytes of
         * each string match.
         */

        str += CDM_UtfToUniChar(str, &ch1);
        pattern += CDM_UtfToUniChar(pattern, &ch2);
        if (nocase) {
            if (CDM_UniCharToLower(ch1, loc) != CDM_UniCharToLower(ch2, loc)) {
                return 0;
            }
        } else if (ch1 != ch2) {
            return 0;
        }
    }
}

/*
 *----------------------------------------------------------------------
 *
 * CDM_StringMatch --
 *
 *        See if a particular string matches a particular pattern.
 *
 * Results:
 *        The return value is 1 if string matches pattern, and 0 otherwise. The
 *        matching operation permits the following special characters in the
 *        pattern: *?\[] (see the manual entry for details on what these mean).
 *
 * Side effects:
 *        None.
 *
 *----------------------------------------------------------------------
 */

int
CDM_StringMatch(
    const char *str,            /* String. */
    const char *pattern)        /* Pattern, which may contain special
                                 * characters. */
{
    return CDM_StringCaseMatch(str, pattern, 0);
}

    }; /* namespace ELEP::CDM::Utilities */

    namespace Unicode {
      /* Code from:
       * https://stackoverflow.com/questions/30995246/substring-of-a-stdstring-in-utf-8-c11
       * http://www.zedwood.com/article/cpp-utf-8-mb_substr-function
       */
      bool utf8_byteOffsets(const std::string& str,
                            const size_t start, const size_t end,
                            size_t *byte_start, size_t *byte_end) {
        // std::cout << "utf8_substr: start: " << start
        //           << ", end: " << end << std::endl;
        size_t c, i, ix, q;
        size_t min=std::string::npos, max=std::string::npos;
        *byte_start = *byte_end = std::string::npos;
        for (q=0, i=0, ix=str.length(); i < ix && q < end+2 ; i++, q++) {
            if (q==start){ min=i; }
            if (q<=end || end==std::string::npos){ max=i; }

            c = (unsigned char) str[i];
            // std::cout << "i: " << i << ", q: " << q << ", max: " << max
            //           << ", " << (char) c << std::endl;
            if      (
                     //c>=0   &&
                     c<=127) i+=0;
            else if ((c & 0xE0) == 0xC0) i+=1;
            else if ((c & 0xF0) == 0xE0) i+=2;
            else if ((c & 0xF8) == 0xF0) i+=3;
            //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
            //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
            else return false;//invalid utf8
        }
        if (q<=end || end==std::string::npos){ max=i; }
        if (min==std::string::npos || max==std::string::npos) { return false; }
        // std::cout << "RESULT: ["<<min<<','<<max<< "] \""
        //           << str.substr(min,max-min) << "\"\n";
        *byte_start = min; *byte_end = max-min;
        return true;
      }; /* utf8_byteOffsets */

      std::string utf8_substr(const std::string& str,
                              size_t start, size_t end) {
        size_t min, len;
        if (utf8_byteOffsets(str, start, end, &min, &len)) {
          return str.substr(min, len);
        }
        return "";
      };/* utf8_substr */
    }; /* namespace ELEP::CDM::Unicode */
  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
