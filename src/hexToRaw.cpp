/*
 * hexToRaw
 *
 *  Created on: Dec 11, 2013
 *      Author: Wush Wu
 *
 *  The source code is modified from PHP
 */

#include <Rcpp.h>

namespace Encoding {

const static char bin2hextab[] = "0123456789abcdef";
const static int hex2bintab[] = {
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,
                -1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};
const std::string
hex_decode(const std::string& src) {
        if (src.size() % 2) {
                throw std::invalid_argument("hex_decode: the input is not a valid hex code");
        }
        std::string retval;
        retval.resize(src.size()/2);
        unsigned char *pretval( reinterpret_cast<unsigned char*>(&retval[0]) );
        const unsigned char *psrc( reinterpret_cast<const unsigned char*>(&src[0]) );
        int first, second;
        for (size_t i = 0; i < src.size()/2; i++) {
                first = hex2bintab[*psrc++];
                if (first == -1) {
                        throw std::invalid_argument("hex_decode: the input is not a valid hex code");
                }
                second = hex2bintab[*psrc++];
                if (second == -1) {
                        throw std::invalid_argument("hex_decode: the input is not a valid hex code");
                }
                first = first << 4;
                *pretval++ = first + second;
        }
        return retval;
}

const std::string
hex_encode(const std::string& src) {
        std::string retval(0, src.size() * 2);
        for (size_t i(0), j(0); i < src.size();i++) {
                retval[j++] = bin2hextab[src[i] >> 4];
                retval[j++] = bin2hextab[src[i] & 15];
        }
        return retval;
}


}

namespace Encoding {

static const char base64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
                '\0' };

static const char base64_pad = '=';

static const short base64_reverse_table[256] = { -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -1, -1, -2, -2, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62,
                -2, -2, -2, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2,
                -2, -2, -2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2, -2, 26, 27, 28,
                29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                47, 48, 49, 50, 51, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 };

inline unsigned char*
unsigned_char_cast(char* src) {
        return reinterpret_cast<unsigned char*>(src);
}

inline const unsigned char*
const_unsigned_char_cast(const char* src) {
        return (const unsigned char*) src;
}

const std::string base64_encode(const std::string& src) {
        std::string retval;
        std::string::size_type length(src.size());
        retval.resize(((length + 2) / 3) * 4);
        unsigned char *p(unsigned_char_cast(&retval[0]));
        const unsigned char *current(const_unsigned_char_cast(&src[0]));

        while (length > 2) {
                *p++ = base64_table[current[0] >> 2];
                *p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
                *p++ = base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
                *p++ = base64_table[current[2] & 0x3f];

                current += 3;
                length -= 3; /* we just handle 3 octets of data */
        }

        if (length != 0) {
                *p++ = base64_table[ current[0] >> 2];
                if (length > 1) {
                        *p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
                        *p++ = base64_table[(current[1] & 0x0f) << 2];
                        *p++ = base64_pad;
                }
                else {
                        *p++ = base64_table[(current[0] & 0x03) << 4];
                        *p++ = base64_pad;
                        *p++ = base64_pad;
                }
        }

        retval.resize( (std::string::size_type) (p - unsigned_char_cast(&retval[0]) ) );
        return retval;
}

const std::string base64_decode(const std::string& src) {
        std::string::size_type length(src.size());

        std::string retval;
        retval.resize(length, 0);
        int ch, i(0), j(0), k;

        const unsigned char *current(const_unsigned_char_cast(&src[0]));
        unsigned char *result(unsigned_char_cast(&retval[0]));

        while( length-- > 0 )
        {
                ch = *current++;

                if ( ch == base64_pad ) {
                        if (*current != '=' && (i%4) == 1) {
                                throw std::invalid_argument("Input is an invalid base64 encoded string");
                        }
                        continue;
                }

                ch = base64_reverse_table[ch];
                if (ch == -1) {
                        continue;
                }
                else if (ch == -2) {
                        throw std::invalid_argument("Input is an invalid base64 encoded string");
                }

                switch (i % 4)
                {
                case 0:
                        result[j] = ch << 2;
                        break;
                case 1:
                        result[j++] |= ch >> 4;
                        result[j] = (ch & 0x0f) << 4;
                        break;
                case 2:
                        result[j++] |= ch >>2;
                        result[j] = (ch & 0x03) << 6;
                        break;
                case 3:
                        result[j++] |= ch;
                        break;
                }
                i++;
        }

        k = j;
        if (ch == base64_pad) {
                switch(i % 4)
                {
                case 1:
                        throw std::invalid_argument("Input is an invalid base64 encoded string");
                case 2:
                        k++;
                case 3:
                        result[k] = 0;
                }
        }
        retval.resize(j);
        return retval;
}

}




using namespace Rcpp;

RcppExport SEXP hexToRaw(SEXP Rsrc) {
  BEGIN_RCPP
  CharacterVector src(Rsrc);
  const char* psrc = CHAR(wrap(src[0]));
  std::string binary(Encoding::hex_decode(psrc));
  std::string retval(Encoding::base64_encode(binary));
  return wrap(retval);
  END_RCPP
}