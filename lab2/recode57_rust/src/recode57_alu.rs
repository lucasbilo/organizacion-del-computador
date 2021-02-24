use crate::Codepoint;
use crate::Encoding::{self, *};
use std::cmp;
// Esta función es idéntica a la versión en C, excepto que también
// devuelve el tamaño del bom que se encontró. Esto permite a main.rs
// procesar los bytes restantes.
pub fn bom_to_enc(bom: &[u8; 4]) -> (Encoding, usize) {
    match bom {
        [0xFE, 0xFF, _, _] => (UTF16BE, 2),
        [0, 0, 0xFE, 0xFF] => (UTF32BE, 4),
        [0xFF, 0xFE, 0, 0] => (UTF32LE, 4),
        [0xFF, 0xFE, _, _] => (UTF16LE, 2),
        _ => (UTF8, 0),
    }
}

pub fn write_bom(enc: Encoding, buf: &mut [u8; 4]) -> usize {
    match enc {
        UTF16BE => {
            buf[0] = 0xFE;
            buf[1] = 0xFF;
            2
        }
        UTF32BE => {
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0xFE;
            buf[3] = 0xFF;
            4
        }
        UTF32LE => {
            buf[0] = 0xFF;
            buf[1] = 0xFE;
            buf[2] = 0;
            buf[3] = 0;
            4
        }
        UTF16LE => {
            buf[0] = 0xFF;
            buf[1] = 0xFE;
            2
        }
        _ => 0,
    }
}

pub fn has_codepoint(enc: Encoding, input: &[u8], len: usize) -> bool {
    return match enc {
        UTF32BE => len >= 4,
        UTF32LE => len >= 4,
        UTF16BE => len >= 2,
        UTF16LE => len >= 2,
        UTF8 => {
            (len >= 1 && (input[0] & 0x80) == 0)
                || (len >= 2 && input[0] <= 0xDF && input[1] <= 0xBF)
                || (len >= 3 && input[0] <= 0xEF && input[1] <= 0xBF && input[2] <= 0xBF)
                || (len >= 4
                    && input[0] <= 0xF7
                    && input[1] <= 0xBF
                    && input[2] <= 0xBF
                    && input[3] <= 0xBF)
        }
    };
}

// Esta función devuelve lo mismo que su equivalente en C (el codepoint, y la
// cantidad de bytes que se usaron para producirlo), preo en una tupla (ya que
// Rust tiene soporte nativo). Además, en lugar de usar el 0 como indicador que
// no había suficientes bytes para producir un codepoint, se usa None, mucho más
// idiomático en Rust.
pub fn read_codepoint(enc: Encoding, input: &[u8]) -> Option<(Codepoint, usize)> {
    let mut cp = 0; // Variable para construir que será devuelto.
    let mut bytes = 0; // Cantidad de bytes consumidos al leer el codepoint.
    let n = input.len(); // Cantidad de bytes disponibles.
    if !has_codepoint(enc, input, n) {
        return None;
    }
    // Uso opcional: convertir el buffer de entrada a u32, para evitar casts.
    let buf = input[..cmp::min(n, 4)]
        .iter()
        .map(|&x| x as u32)
        .collect::<Vec<_>>();

    match enc {
        UTF32BE => {
            cp = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
            bytes = 4;
        }
        UTF32LE => {
            cp = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
            bytes = 4;
        }
        UTF16BE => {
            cp = (buf[0] << 8) | buf[1];
            if cp <= 0xD7FF || cp >= 0xE000 {
                bytes = 2;
            } else if n >= 4 {
                // cp hasta aca tiene "cargado" lo que seria el sustituto alto
                cp = (cp - 0xD800) << 10;
                let low_substitute = (buf[2] << 8) | buf[3];
                cp = cp | (low_substitute - 0xDC00);
                cp = cp + 0x10000;
                bytes = 4;
            }
        }
        UTF16LE => {
            cp = buf[1];
            cp = (cp << 8) | buf[0];
            if cp <= 0xD7FF || cp >= 0xE000 {
                bytes = 2;
            } else if n >= 4 {
                // cp hasta aca tiene "cargado" lo que seria el sustituto alto
                cp = (cp - 0xD800) << 10;
                let low_substitute = (buf[3] << 8) | buf[2];
                cp = cp | (low_substitute - 0xDC00);
                cp = cp + 0x10000;
                bytes = 4;
            }
        }
        UTF8 => {
            if buf[0] < 0x80 {
                cp = buf[0];
                bytes = 1;
            } else if buf[0] <= 0xDF && buf[1] <= 0xBF {
                cp = ((buf[0] & 0x1F) << 6) | (buf[1] & 0x3F);
                bytes = 2;
            } else if buf[0] <= 0xEF && buf[1] <= 0xBF && buf[2] <= 0xBF {
                cp = (buf[0] & 0xF) << 12;
                cp = cp | ((buf[1] & 0x3F) << 6);
                cp = cp | (buf[2] & 0x3F);
                bytes = 3;
            } else if buf[0] <= 0xF7 && buf[1] <= 0xBF && buf[2] <= 0xBF && buf[3] <= 0xBF {
                cp = (buf[0] & 0x7) << 18;
                cp = cp | ((buf[1] & 0x3F) << 12);
                cp = cp | ((buf[2] & 0x3F) << 6);
                cp = cp | (buf[3] & 0x3F);
                bytes = 4;
            }
        }
    };
    return Some((cp, bytes));
}

pub fn write_codepoint(enc: Encoding, cp: Codepoint, outbuf: &mut [u8; 4]) -> usize {
    let mask32: u32 = 0xFF;
    let mask16: u16 = 0xFF;
    let mask_utf8: u32 = 0x3F;

    match enc {
        UTF32BE => {
            outbuf[3] = (cp & mask32) as u8;
            outbuf[2] = ((cp & (mask32 << 8)) >> 8) as u8;
            outbuf[1] = ((cp & (mask32 << 16)) >> 16) as u8;
            outbuf[0] = ((cp & (mask32 << 24)) >> 24) as u8;
            4
        }
        UTF32LE => {
            outbuf[0] = (cp & mask32) as u8;
            outbuf[1] = ((cp & (mask32 << 8)) >> 8) as u8;
            outbuf[2] = ((cp & (mask32 << 16)) >> 16) as u8;
            outbuf[3] = ((cp & (mask32 << 24)) >> 24) as u8;
            4
        }
        UTF16BE => {
            if cp <= 0xFFFF {
                outbuf[1] = (cp & mask32) as u8;
                outbuf[0] = ((cp & (mask32 << 8)) >> 8) as u8;
                2
            } else if cp > 0xFFFF && cp <= 0x10FFFF {
                let high_substitute: u16 = ((((cp - 0x10000) >> 10) & 0x3FF) + 0xD800) as u16;
                let low_substitute: u16 = (((cp - 0x10000) & 0x3FF) + 0xDC00) as u16;
                outbuf[3] = (low_substitute & mask16) as u8;
                outbuf[2] = ((low_substitute & (mask16 << 8)) >> 8) as u8;
                outbuf[1] = (high_substitute & mask16) as u8;
                outbuf[0] = ((high_substitute & (mask16 << 8)) >> 8) as u8;
                4
            } else {
                0
            }
        }
        UTF16LE => {
            if cp <= 0xFFFF {
                outbuf[0] = (cp & mask32) as u8;
                outbuf[1] = ((cp & (mask32 << 8)) >> 8) as u8;
                2
            } else if cp > 0xFFFF && cp <= 0x10FFFF {
                let high_substitute: u16 = ((((cp - 0x10000) >> 10) & 0x3FF) + 0xD800) as u16;
                let low_substitute: u16 = (((cp - 0x10000) & 0x3FF) + 0xDC00) as u16;
                outbuf[0] = (high_substitute & mask16) as u8;
                outbuf[1] = ((high_substitute & (mask16 << 8)) >> 8) as u8;
                outbuf[2] = (low_substitute & mask16) as u8;
                outbuf[3] = ((low_substitute & (mask16 << 8)) >> 8) as u8;
                4
            } else {
                0
            }
        }
        UTF8 => {
            if cp < 0x80 {
                outbuf[0] = (cp & mask32) as u8;
                1
            } else if cp >= 0x0080 && cp < 0x0800 {
                outbuf[0] = (0xC0 | ((cp & (mask_utf8 << 6)) >> 6)) as u8;
                outbuf[1] = (0x80 | (cp & mask_utf8)) as u8;
                2
            } else if cp >= 0x0800 && cp < 0x10000 {
                outbuf[0] = (0xE0 | ((cp & (mask_utf8 << 12)) >> 12)) as u8;
                outbuf[1] = (0x80 | ((cp & (mask_utf8 << 6)) >> 6)) as u8;
                outbuf[2] = (0x80 | (cp & mask_utf8)) as u8;
                3
            } else if cp >= 0x10000 && cp < 0x110000 {
                outbuf[0] = (0xF0 | ((cp & (mask_utf8 << 18)) >> 18)) as u8;
                outbuf[1] = (0x80 | ((cp & (mask_utf8 << 12)) >> 12)) as u8;
                outbuf[2] = (0x80 | ((cp & (mask_utf8 << 6)) >> 6)) as u8;
                outbuf[3] = (0x80 | (cp & mask_utf8)) as u8;
                4
            } else {
                0
            }
        }
    }
}
