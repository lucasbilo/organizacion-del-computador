#include "recode57.h"

enum encoding bom_to_enc(uint8_t *bom)
{
    if (bom[0] == 0xFE && bom[1] == 0xFF)
        return UTF16BE;
    else if (bom[0] == 0 && bom[1] == 0 && bom[2] == 0xFE && bom[3] == 0xFF)
        return UTF32BE;
    else if (bom[0] == 0xFF && bom[1] == 0xFE)
    {
        if (bom[2] == 0 && bom[3] == 0)
            return UTF32LE;
        else
            return UTF16LE;
    }
    else
        return UTF8;
}

size_t write_bom(enum encoding enc, uint8_t *buf)
{
    if (enc == UTF16BE)
    {
        buf[0] = 0xFE;
        buf[1] = 0xFF;
        return 2;
    }
    else if (enc == UTF32BE)
    {
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = 0xFE;
        buf[3] = 0xFF;
        return 4;
    }
    else if (enc == UTF32LE)
    {
        buf[0] = 0xFF;
        buf[1] = 0xFE;
        buf[2] = 0;
        buf[3] = 0;
        return 4;
    }
    else if (enc == UTF16LE)
    {
        buf[0] = 0xFF;
        buf[1] = 0xFE;
        return 2;
    }
    return 0;
}

// tenemos que ir agregando cada una de las validaciones a medida que vamos agregando funcionalidades
bool has_codepoint(enum encoding enc, uint8_t *buf, size_t nbytes)
{
    if (enc == UTF32BE || enc == UTF32LE)
        return nbytes >= 4;
    else if (enc == UTF16BE || enc == UTF16LE)
        return nbytes >= 2;
    else if (enc == UTF8)
    {
        if (nbytes < 1)
            return false;
        else if ((buf[0] & 0x80) == 0)
            return true;
        else if(nbytes >= 2 && buf[0] <= 0xDF && buf[1] <= 0xBF)
            return true;
        else if(nbytes >= 3 && buf[0] <= 0xEF && buf[1] <= 0xBF && buf[2] <= 0xBF)
            return true;
        else if(nbytes >= 4 && buf[0] <= 0xF7 && buf[1] <= 0xBF && buf[2] <= 0xBF && buf[3] <= 0xBF)
            return true;
    }
    return false;
}

size_t read_codepoint(enum encoding enc, uint8_t *buf, size_t nbytes, codepoint_t *dest)
{
    if (!has_codepoint(enc, buf, nbytes))
        return 0;
    
    codepoint_t codepoint;
    uint8_t bytes;
    switch (enc)
    {
    case UTF32BE:
        codepoint = buf[0];
        codepoint = (codepoint << 8) | buf[1];
        codepoint = (codepoint << 8) | buf[2];
        codepoint = (codepoint << 8) | buf[3];
        bytes = 4;
        break;
    case UTF32LE:
        codepoint = buf[3];
        codepoint = (codepoint << 8) | buf[2];
        codepoint = (codepoint << 8) | buf[1];
        codepoint = (codepoint << 8) | buf[0];
        bytes = 4;
        break;
    case UTF16BE:
        codepoint = buf[0];
        codepoint = (codepoint << 8) | buf[1];
        if (codepoint <= 0xD7FF || codepoint >= 0xE000)
        {
            bytes = 2;
        }
        else if (nbytes >= 4)
        {
            // codepoint hasta aca tiene "cargado" lo que seria el sustituto alto
            codepoint = (codepoint - 0xD800) << 10;
            uint16_t lowSubstitute = (buf[2] << 8) | buf[3];
            codepoint = codepoint | (lowSubstitute - 0xDC00);
            codepoint = codepoint + 0x10000;
            bytes = 4;
        }
        
        break;
    case UTF16LE:
        codepoint = buf[1];
        codepoint = (codepoint << 8) | buf[0];
        if (codepoint <= 0xD7FF || codepoint >= 0xE000)
        {
            bytes = 2;
        }
        else if (nbytes >= 4)
        {
            // codepoint hasta aca tiene "cargado" lo que seria el sustituto alto
            codepoint = (codepoint - 0xD800) << 10;
            uint16_t lowSubstitute = (buf[3] << 8) | buf[2];
            codepoint = codepoint | (lowSubstitute - 0xDC00);
            codepoint = codepoint + 0x10000;
            bytes = 4;
        }
        break;
    case UTF8:
        if(buf[0] < 0x80)
        {
            codepoint = buf[0];
            bytes = 1;
        }
        else if(buf[0] <= 0xDF && buf[1] <= 0xBF)
        {
            codepoint = (buf[0] & 0x1F) << 6;
            codepoint = codepoint | (buf[1] & 0x3F);
            bytes = 2;
        }
        else if(buf[0] <= 0xEF && buf[1] <= 0xBF && buf[2] <= 0xBF)
        {
            codepoint = (buf[0] & 0xF) << 12;
            codepoint = codepoint | ((buf[1] & 0x3F) << 6);
            codepoint = codepoint | (buf[2] & 0x3F);
            bytes = 3;
        }
        else if(buf[0] <= 0xF7 && buf[1] <= 0xBF && buf[2] <= 0xBF && buf[3] <= 0xBF){
            codepoint = (buf[0] & 0x7) << 18;
            codepoint = codepoint | ((buf[1] & 0x3F) << 12);
            codepoint = codepoint | ((buf[2] & 0x3F) << 6);
            codepoint = codepoint | (buf[3] & 0x3F);
            bytes = 4;
        }
        break;
    default:
        break;
    }
    *dest = codepoint;
    return bytes;
}

size_t write_codepoint(enum encoding enc, codepoint_t codepoint, uint8_t *outbuf)
{
    uint8_t result;
    uint8_t mask = 0xFF;
    uint8_t maskUTF8 = 0x3F;

    switch (enc)
    {
    case UTF32BE:
        outbuf[3] = codepoint & mask;
        outbuf[2] = (codepoint & (mask << 8)) >> 8;
        outbuf[1] = (codepoint & (mask << 16)) >> 16;
        outbuf[0] = (codepoint & (mask << 24)) >> 24;
        result = 4;
        break;
    case UTF32LE:
        outbuf[0] = codepoint & mask;
        outbuf[1] = (codepoint & (mask << 8)) >> 8;
        outbuf[2] = (codepoint & (mask << 16)) >> 16;
        outbuf[3] = (codepoint & (mask << 24)) >> 24;
        result = 4;
        break;
    case UTF16BE:
        if (codepoint <= 0xFFFF)
        {
            outbuf[1] = codepoint & mask;
            outbuf[0] = (codepoint & (mask << 8)) >> 8;
            result = 2;
        }
        else if (codepoint > 0xFFFF && codepoint <= 0x10FFFF)
        {
            codepoint = codepoint - 0x10000;
            uint16_t highSubstitute = ((codepoint >> 10) & 0x3FF) + 0xD800;
            uint16_t lowSubstitute = (codepoint & 0x3FF) + 0xDC00;
            outbuf[3] = lowSubstitute & mask;
            outbuf[2] = (lowSubstitute & (mask << 8)) >> 8;
            outbuf[1] = highSubstitute & mask;
            outbuf[0] = (highSubstitute & (mask << 8)) >> 8;
            result = 4;
        }
        break;
    case UTF16LE:
        if (codepoint <= 0xFFFF)
        {
            outbuf[0] = codepoint & mask;
            outbuf[1] = (codepoint & (mask << 8)) >> 8;
            result = 2;
        }
        else if (codepoint > 0xFFFF && codepoint <= 0x10FFFF)
        {
            codepoint = codepoint - 0x10000;
            uint16_t highSubstitute = ((codepoint >> 10) & 0x3FF) + 0xD800;
            uint16_t lowSubstitute = (codepoint & 0x3FF) + 0xDC00;
            outbuf[0] = highSubstitute & mask;
            outbuf[1] = (highSubstitute & (mask << 8)) >> 8;
            outbuf[2] = lowSubstitute & mask;
            outbuf[3] = (lowSubstitute & (mask << 8)) >> 8;
            result = 4;
        }
        break;
    case UTF8:
        if (codepoint < 0x80)
        {
            outbuf[0] = codepoint & mask;
            result = 1;
        }
        else if (codepoint >= 0x0080 && codepoint < 0x0800)
        {
            outbuf[0] = 0xC0 | ((codepoint & (maskUTF8 << 6)) >> 6);
            outbuf[1] = 0x80 | (codepoint & maskUTF8);
            result = 2;
        }
        else if (codepoint >= 0x0800 && codepoint < 0x10000)
        {
            outbuf[0] = 0xE0 | ((codepoint & (maskUTF8 << 12)) >> 12);
            outbuf[1] = 0x80 | ((codepoint & (maskUTF8 << 6)) >> 6);
            outbuf[2] = 0x80 | (codepoint & maskUTF8);
            result = 3;
        }
        else if (codepoint >= 0x10000 && codepoint < 0x110000)
        {
            outbuf[0] = 0xF0 | ((codepoint & (maskUTF8 << 18)) >> 18);
            outbuf[1] = 0x80 | ((codepoint & (maskUTF8 << 12)) >> 12);
            outbuf[2] = 0x80 | ((codepoint & (maskUTF8 << 6)) >> 6);
            outbuf[3] = 0x80 | (codepoint & maskUTF8);
            result = 4;
        }
        break;
    default:
        break;
    }
    return result;
}