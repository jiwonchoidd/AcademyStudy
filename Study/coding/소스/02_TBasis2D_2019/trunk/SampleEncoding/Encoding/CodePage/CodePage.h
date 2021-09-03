#pragma once

namespace Core
{
	namespace Text
	{
		namespace eCodePage
		{
			enum Type
			{
				Default				= 0,
				ASCII				= 20127,
				Unicode				= 1200,
				UnicodeBigEndian	= 1201,
				UTF8				= 65001,

				Korean				= 949,
				Japanese			= 932,
				ChineseSimplified	= 936,
				ChineseTraditional	= 950
			};
		}

		namespace EIANACodePage
		{
			enum Type
			{
				IBM037                    		= 37,		// IBM EBCDIC (US-Canada)
				IBM437                    		= 437,		// OEM United States
				IBM500                    		= 500,		// IBM EBCDIC (International)
				ASMO_708                  		= 708,		// Arabic (ASMO 708)
				DOS_720                   		= 720,		// Arabic (DOS)
				ibm737                    		= 737,		// Greek (DOS)
				ibm775                    		= 775,		// Baltic (DOS)
				ibm850                    		= 850,		// Western European (DOS)
				ibm852                    		= 852,		// Central European (DOS)
				IBM855                    		= 855,		// OEM Cyrillic
				ibm857                    		= 857,		// Turkish (DOS)
				IBM00858                  		= 858,		// OEM Multilingual Latin I
				IBM860                    		= 860,		// Portuguese (DOS)
				ibm861                    		= 861,		// Icelandic (DOS)
				DOS_862                   		= 862,		// Hebrew (DOS)
				IBM863                    		= 863,		// French Canadian (DOS)
				IBM864                    		= 864,		// Arabic (864)
				IBM865                    		= 865,		// Nordic (DOS)
				cp866                     		= 866,		// Cyrillic (DOS)
				ibm869                    		= 869,		// Greek, Modern (DOS)
				IBM870                    		= 870,		// IBM EBCDIC (Multilingual Latin-2)
				windows_874               		= 874,		// Thai (Windows)
				cp875                     		= 875,		// IBM EBCDIC (Greek Modern)
				shift_jis                 		= 932,		// Japanese (Shift-JIS)
				gb2312                    		= 936,		// Chinese Simplified (GB2312)
				ks_c_5601_1987            		= 949,		// Korean
				big5                      		= 950,		// Chinese Traditional (Big5)
				IBM1026                   		= 1026,		// IBM EBCDIC (Turkish Latin-5)
				IBM01047                  		= 1047,		// IBM Latin-1
				IBM01140                  		= 1140,		// IBM EBCDIC (US-Canada-Euro)
				IBM01141                  		= 1141,		// IBM EBCDIC (Germany-Euro)
				IBM01142                  		= 1142,		// IBM EBCDIC (Denmark-Norway-Euro)
				IBM01143                  		= 1143,		// IBM EBCDIC (Finland-Sweden-Euro)
				IBM01144                  		= 1144,		// IBM EBCDIC (Italy-Euro)
				IBM01145                  		= 1145,		// IBM EBCDIC (Spain-Euro)
				IBM01146                  		= 1146,		// IBM EBCDIC (UK-Euro)
				IBM01147                  		= 1147,		// IBM EBCDIC (France-Euro)
				IBM01148                  		= 1148,		// IBM EBCDIC (International-Euro)
				IBM01149                  		= 1149,		// IBM EBCDIC (Icelandic-Euro)
				utf_16                    		= 1200,		// Unicode
				unicodeFFFE               		= 1201,		// Unicode (Big-Endian)
				windows_1250              		= 1250,		// Central European (Windows)
				windows_1251              		= 1251,		// Cyrillic (Windows)
				Windows_1252              		= 1252,		// Western European (Windows)
				windows_1253              		= 1253,		// Greek (Windows)
				windows_1254              		= 1254,		// Turkish (Windows)
				windows_1255              		= 1255,		// Hebrew (Windows)
				windows_1256              		= 1256,		// Arabic (Windows)
				windows_1257              		= 1257,		// Baltic (Windows)
				windows_1258              		= 1258,		// Vietnamese (Windows)
				Johab                     		= 1361,		// Korean (Johab)
				macintosh                 		= 10000,	// Western European (Mac)
				x_mac_japanese            		= 10001,	// Japanese (Mac)
				x_mac_chinesetrad         		= 10002,	// Chinese Traditional (Mac)
				x_mac_korean              		= 10003,	// Korean (Mac)
				x_mac_arabic              		= 10004,	// Arabic (Mac)
				x_mac_hebrew              		= 10005,	// Hebrew (Mac)
				x_mac_greek               		= 10006,	// Greek (Mac)
				x_mac_cyrillic            		= 10007,	// Cyrillic (Mac)
				x_mac_chinesesimp         		= 10008,	// Chinese Simplified (Mac)
				x_mac_romanian            		= 10010,	// Romanian (Mac)
				x_mac_ukrainian           		= 10017,	// Ukrainian (Mac)
				x_mac_thai                		= 10021,	// Thai (Mac)
				x_mac_ce                  		= 10029,	// Central European (Mac)
				x_mac_icelandic           		= 10079,	// Icelandic (Mac)
				x_mac_turkish             		= 10081,	// Turkish (Mac)
				x_mac_croatian            		= 10082,	// Croatian (Mac)
				x_Chinese_CNS             		= 20000,	// Chinese Traditional (CNS)
				x_cp20001                 		= 20001,	// TCA Taiwan
				x_Chinese_Eten            		= 20002,	// Chinese Traditional (Eten)
				x_cp20003                 		= 20003,	// IBM5550 Taiwan
				x_cp20004                 		= 20004,	// TeleText Taiwan
				x_cp20005                 		= 20005,	// Wang Taiwan
				x_IA5                     		= 20105,	// Western European (IA5)
				x_IA5_German              		= 20106,	// German (IA5)
				x_IA5_Swedish             		= 20107,	// Swedish (IA5)
				x_IA5_Norwegian           		= 20108,	// Norwegian (IA5)
				us_ascii                  		= 20127,	// US-ASCII
				x_cp20261                 		= 20261,	// T.61
				x_cp20269                 		= 20269,	// ISO-6937
				IBM273                    		= 20273,	// IBM EBCDIC (Germany)
				IBM277                    		= 20277,	// IBM EBCDIC (Denmark-Norway)
				IBM278                    		= 20278,	// IBM EBCDIC (Finland-Sweden)
				IBM280                    		= 20280,	// IBM EBCDIC (Italy)
				IBM284                    		= 20284,	// IBM EBCDIC (Spain)
				IBM285                    		= 20285,	// IBM EBCDIC (UK)
				IBM290                    		= 20290,	// IBM EBCDIC (Japanese katakana)
				IBM297                    		= 20297,	// IBM EBCDIC (France)
				IBM420                    		= 20420,	// IBM EBCDIC (Arabic)
				IBM423                    		= 20423,	// IBM EBCDIC (Greek)
				IBM424                    		= 20424,	// IBM EBCDIC (Hebrew)
				x_EBCDIC_KoreanExtended   		= 20833,	// IBM EBCDIC (Korean Extended)
				IBM_Thai                  		= 20838,	// IBM EBCDIC (Thai)
				koi8_r                    		= 20866,	// Cyrillic (KOI8-R)
				IBM871                    		= 20871,	// IBM EBCDIC (Icelandic)
				IBM880                    		= 20880,	// IBM EBCDIC (Cyrillic Russian)
				IBM905                    		= 20905,	// IBM EBCDIC (Turkish)
				IBM00924                  		= 20924,	// IBM Latin-1
				EUC_JP                    		= 20932,	// Japanese (JIS 0208-1990 and 0212-1990)
				x_cp20936                 		= 20936,	// Chinese Simplified (GB2312-80)
				x_cp20949                 		= 20949,	// Korean Wansung
				cp1025                    		= 21025,	// IBM EBCDIC (Cyrillic Serbian-Bulgarian)
				koi8_u                    		= 21866,	// Cyrillic (KOI8-U)
				iso_8859_1                		= 28591,	// Western European (ISO)
				iso_8859_2                		= 28592,	// Central European (ISO)
				iso_8859_3                		= 28593,	// Latin 3 (ISO)
				iso_8859_4                		= 28594,	// Baltic (ISO)
				iso_8859_5                		= 28595,	// Cyrillic (ISO)
				iso_8859_6                		= 28596,	// Arabic (ISO)
				iso_8859_7                		= 28597,	// Greek (ISO)
				iso_8859_8                		= 28598,	// Hebrew (ISO-Visual)
				iso_8859_9                		= 28599,	// Turkish (ISO)
				iso_8859_13               		= 28603,	// Estonian (ISO)
				iso_8859_15               		= 28605,	// Latin 9 (ISO)
				x_Europa                  		= 29001,	// Europa
				iso_8859_8_i              		= 38598,	// Hebrew (ISO-Logical)
				iso_2022_jp               		= 50220,	// Japanese (JIS)
				csISO2022JP               		= 50221,	// Japanese (JIS-Allow 1 byte Kana)
				iso_2022_jp_so_si          		= 50222,	// Japanese (JIS-Allow 1 byte Kana - SO/SI)
				iso_2022_kr               		= 50225,	// Korean (ISO)
				x_cp50227                 		= 50227,	// Chinese Simplified (ISO-2022)
				euc_jp                    		= 51932,	// Japanese (EUC)
				EUC_CN                    		= 51936,	// Chinese Simplified (EUC)
				euc_kr                    		= 51949,	// Korean (EUC)
				hz_gb_2312                		= 52936,	// Chinese Simplified (HZ)
				GB18030                   		= 54936,	// Chinese Simplified (GB18030)
				x_iscii_de                		= 57002,	// ISCII Devanagari
				x_iscii_be                		= 57003,	// ISCII Bengali
				x_iscii_ta                		= 57004,	// ISCII Tamil
				x_iscii_te                		= 57005,	// ISCII Telugu
				x_iscii_as                		= 57006,	// ISCII Assamese
				x_iscii_or                		= 57007,	// ISCII Oriya
				x_iscii_ka                		= 57008,	// ISCII Kannada
				x_iscii_ma                		= 57009,	// ISCII Malayalam
				x_iscii_gu                		= 57010,	// ISCII Gujarati
				x_iscii_pa                		= 57011,	// ISCII Punjabi
				utf_7                     		= 65000,	// Unicode (UTF-7)
				utf_8                     		= 65001,	// Unicode (UTF-8)
				utf_32                    		= 65005,	// Unicode (UTF-32)
				utf_32BE                  		= 65006,	// Unicode (UTF-32 Big-Endian)
			};
		}
	}
}