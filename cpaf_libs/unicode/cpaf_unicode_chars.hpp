#ifndef CPAF_UNICODE_CHARS_H
#define CPAF_UNICODE_CHARS_H

// https://www.rapidtables.com/code/text/unicode-characters.html
// https://www.rapidtables.com/code/text/ascii-table.html

namespace cpaf::unicode {

// 	! 	" 	# 	$ 	% 	& 	' 	( 	) 	* 	+ 	, 	- 	. 	/

/** @todo Move to cpaf_unicode_chars.hpp */
template <typename CHAR_TYPE>
struct chars {
    static constexpr CHAR_TYPE tab                      = 0x09; // TAB
    static constexpr CHAR_TYPE line_feed                = 0x0A; // LF \n
    static constexpr CHAR_TYPE newline                  = 0x0A; // LF \n
    static constexpr CHAR_TYPE carriage_return          = 0x0D; // CR \r
    static constexpr CHAR_TYPE space                    = 0x20; // ' '
    static constexpr CHAR_TYPE exclamation_mark         = 0x21; // !
    static constexpr CHAR_TYPE double_quote             = 0x22; // "
    static constexpr CHAR_TYPE number_sign              = 0x23; // #
    static constexpr CHAR_TYPE dollar                   = 0x24; // $
    static constexpr CHAR_TYPE percent                  = 0x25; // %
    static constexpr CHAR_TYPE ampersand                = 0x26; // &
    static constexpr CHAR_TYPE single_quote             = 0x27; // '
    static constexpr CHAR_TYPE left_par                 = 0x28; // (
    static constexpr CHAR_TYPE right_par                = 0x29; // )
    static constexpr CHAR_TYPE asterisk                 = 0x2A; // *
    static constexpr CHAR_TYPE plus                     = 0x2B; // +
    static constexpr CHAR_TYPE comma                    = 0x2C; // ,
    static constexpr CHAR_TYPE minus                    = 0x2D; // -
    static constexpr CHAR_TYPE dash                     = 0x2D; // -
    static constexpr CHAR_TYPE period                   = 0x2E; // .
    static constexpr CHAR_TYPE dot                      = 0x2E; // .
    static constexpr CHAR_TYPE slash                    = 0x2F; // /
    static constexpr CHAR_TYPE zero                     = 0x30; // 0
    static constexpr CHAR_TYPE one                      = 0x31; // 1
    static constexpr CHAR_TYPE two                      = 0x32; // 2
    static constexpr CHAR_TYPE three                    = 0x33; // 3
    static constexpr CHAR_TYPE four                     = 0x34; // 4
    static constexpr CHAR_TYPE five                     = 0x35; // 5
    static constexpr CHAR_TYPE six                      = 0x36; // 6
    static constexpr CHAR_TYPE seven                    = 0x37; // 7
    static constexpr CHAR_TYPE eight                    = 0x38; // 8
    static constexpr CHAR_TYPE nine                     = 0x39; // 9
    static constexpr CHAR_TYPE colon                    = 0x3A; // :
    static constexpr CHAR_TYPE semi_colon               = 0x3B; // ;
    static constexpr CHAR_TYPE less_than                = 0x3C; // <
    static constexpr CHAR_TYPE equal                    = 0x3D; // =
    static constexpr CHAR_TYPE greater_than             = 0x3E; // >
    static constexpr CHAR_TYPE question_mark            = 0x3F; // ?
    static constexpr CHAR_TYPE at_sign                  = 0x40; // @
    static constexpr CHAR_TYPE a_capital                = 0x41; // A
    static constexpr CHAR_TYPE z_capital                = 0x5A; // Z
    static constexpr CHAR_TYPE left_square_bracket      = 0x5B; // [
    static constexpr CHAR_TYPE backslash                = 0x5C; /* \ */
    static constexpr CHAR_TYPE right_square_bracket     = 0x5D; // ]
    static constexpr CHAR_TYPE caret                    = 0x5E; // ^
    static constexpr CHAR_TYPE circumflex               = 0x5E; // ^
    static constexpr CHAR_TYPE underscore               = 0x5F; // _
    static constexpr CHAR_TYPE grave_accent             = 0x60; // `
    static constexpr CHAR_TYPE a_small                  = 0x61; // a
    static constexpr CHAR_TYPE z_small                  = 0x7A; // z
    static constexpr CHAR_TYPE left_curly_bracket       = 0x7B; // {
    static constexpr CHAR_TYPE pipe                     = 0x7C; // |
    static constexpr CHAR_TYPE right_curly_bracket      = 0x7D; // }
    static constexpr CHAR_TYPE tilde                    = 0x7E; // ~

    static constexpr CHAR_TYPE nbsp                     = 0xA0; // &nbsp; , non-breaking space
    static constexpr CHAR_TYPE inverted_exclamation_mark= 0xA1; // ¡
    static constexpr CHAR_TYPE cent                     = 0xA2; // ¢
    static constexpr CHAR_TYPE pound                    = 0xA3; // £
    static constexpr CHAR_TYPE currency                 = 0xA4; // ¤
    static constexpr CHAR_TYPE yen                      = 0xA5; // ¥
    static constexpr CHAR_TYPE broken_bar               = 0xA6; // ¦
    static constexpr CHAR_TYPE section                  = 0xA7; // §
    static constexpr CHAR_TYPE diaeresis                = 0xA8; // ¨
    static constexpr CHAR_TYPE copyright                = 0xA9; // ©
    static constexpr CHAR_TYPE feminine_ordinal         = 0xAA; // ª  (feminine ordinal indicator)
    static constexpr CHAR_TYPE left_double_quotes       = 0xAB; // «
    static constexpr CHAR_TYPE not_sign                 = 0xAC; // ¬
    static constexpr CHAR_TYPE soft_hyphen              = 0xAD; // ­
    static constexpr CHAR_TYPE registered_trademark     = 0xAE; // ®
    static constexpr CHAR_TYPE macron                   = 0xAF; // ¯
    static constexpr CHAR_TYPE degree                   = 0xB0; // °
    static constexpr CHAR_TYPE plus_minus               = 0xB1; // ±
    static constexpr CHAR_TYPE superscript_2            = 0xB2; // ²
    static constexpr CHAR_TYPE superscript_3            = 0xB3; // ³
    static constexpr CHAR_TYPE acute_accent             = 0xB4; // ´
    static constexpr CHAR_TYPE micro                    = 0xB5; // µ
    static constexpr CHAR_TYPE pilcrow                  = 0xB6; // ¶
    static constexpr CHAR_TYPE middle_dot               = 0xB7; // ·
    static constexpr CHAR_TYPE cedilla                  = 0xB8; // ¸
    static constexpr CHAR_TYPE superscript_1            = 0xB9; // ¹
    static constexpr CHAR_TYPE masculine_ordinal        = 0xBA; // º
    static constexpr CHAR_TYPE right_double_quotes      = 0xBB; // »
    static constexpr CHAR_TYPE fraction_one_quarter     = 0xBC; // ¼
    static constexpr CHAR_TYPE fraction_one_half        = 0xBD; // ½
    static constexpr CHAR_TYPE fraction_threee_quarters = 0xBE; // ¾
    static constexpr CHAR_TYPE inverted_question_mark   = 0xBF; // ¿

    static constexpr CHAR_TYPE bullet                   = 0x2022; // •
    static constexpr CHAR_TYPE triangular_bullet        = 0x2023; // ‣
    static constexpr CHAR_TYPE hyphen_bullet            = 0x2043; // ⁃
    static constexpr CHAR_TYPE euro                     = 0x20AC; // €
    static constexpr CHAR_TYPE sound_recording_copyright= 0x2117; // ℗
    static constexpr CHAR_TYPE service_mark             = 0x2120; // ℠
    static constexpr CHAR_TYPE trademark                = 0x2122; // ™
    static constexpr CHAR_TYPE bullet_operator          = 0x2219; // ∙
    static constexpr CHAR_TYPE infinity                 = 0x221E; // ∞
    static constexpr CHAR_TYPE white_bullet             = 0x25E6; // ◦


    static bool is_tab              (CHAR_TYPE c) { return c == tab;                }
    static bool is_line_feed        (CHAR_TYPE c) { return c == line_feed;          }
    static bool is_newline          (CHAR_TYPE c) { return c == newline;            }
    static bool is_carriage_return  (CHAR_TYPE c) { return c == carriage_return;    }
    static bool is_space            (CHAR_TYPE c) { return c == space;              }
    static bool is_dollar           (CHAR_TYPE c) { return c == dollar;             }
    static bool is_comma            (CHAR_TYPE c) { return c == comma;              }
    static bool is_dash             (CHAR_TYPE c) { return c == dash;               }
    static bool is_dot              (CHAR_TYPE c) { return c == dot;                }
    static bool is_digit            (CHAR_TYPE c) { return zero <= c && c <= nine;  }
    static bool is_asterisk         (CHAR_TYPE c) { return c == asterisk;           }
    static bool is_question_mark    (CHAR_TYPE c) { return c == question_mark;      }
    static bool is_pound            (CHAR_TYPE c) { return c == pound;              }

    static bool is_blank            (CHAR_TYPE c) { return tab <= c && c <= space;  }
    static bool is_comma_or_dot     (CHAR_TYPE c) { return is_comma(c) || is_dot(c);}
    static bool is_number_char      (CHAR_TYPE c) { return is_digit(c) || is_comma_or_dot(c); }
    static bool is_nbsp             (CHAR_TYPE c) { return c == nbsp;               }

    /** Whitespace are TAB, NEWLINE, CR, LF, SPACE */
    static bool is_white_space (CHAR_TYPE c)
    {
        return c == space || ( tab <= c && c <= carriage_return);
    }

    /** Whitespace are nbsp;, TAB, NEWLINE, CR, LF, SPACE */
    static bool is_white_space_html(CHAR_TYPE c)
    {
        return is_white_space(c) || is_nbsp(c);
    }

    /** Special separator chars, that are not whitespace
     *
     * ! 	" 	# 	$ 	% 	& 	' 	( 	) 	* 	+ 	,  - . 	/
     * : 	; 	< 	= 	> 	?
     * So these does NOT include SPACE TAB LF CR &nbsp;
    @todo Perhaps add more special chars
    See For which chars: https://www.rapidtables.com/code/text/unicode-characters.html
    */
    static bool is_separator (CHAR_TYPE c)
    {
        if (is_white_space_html(c)) return false;
        return  ( (1 <= c && c <= slash))                                       ||
                (colon <= c && c <= at_sign)                                    ||
                (left_square_bracket <= c && c <= grave_accent)                 ||
                (left_curly_bracket <= c && c <= tilde)                         ||
                (inverted_exclamation_mark <= c && c <= inverted_question_mark)
             ;
    }

};

using chars16 = chars<char16_t>;
using chars8  = chars<char>;



} // END namespace cpaf::unicode

#endif //CPAF_UNICODE_CHARS_H

