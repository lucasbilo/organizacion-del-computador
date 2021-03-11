/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
//1
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~( ~x | ~y);
}
/*
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only ~ and &
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */
int bitMatch(int x, int y) {
  /* Creo 2 auxiliares, en el primero me quedan "prendidos" los bits que estan en 0 tanto en x como en y, 
     en el segundo lo mismo pero los bits que estan en 1 tanto en x como en y
     Finalmente hago un OR (usando ~ y &) entre los 2 auxiliares 
  */
  int aux1 = ~x & ~y; // match de 0
  int aux2 = x & y; // match de 1
  return ~(~aux1 & ~aux2);
}
/*
 * bitNor - ~(x|y) using only ~ and &
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  return (~x)&(~y);
}
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
   /* En el primer auxiliar quedan "apagados" los bits que solo estan "prendidos" en y pero no en x
     En el segundo auxiliar quedan "apagados" los bits que solo estan "prendidos" en x pero no en y
     Finalmente junto ambos auxiliares con un AND y lo devuelve negado, asi quedan "prendidos" los bits correspondientes.
  */
  int aux1 = ~(~x & y);
  int aux2 = ~(x & ~y);
  return ~(aux1 & aux2);
}
//2
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int mask = 0xAA;
  mask = mask | (mask << 8);
  mask = mask | (mask << 16);
  return !((x & mask) ^ mask);
}
/*
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
  /* Creo una mascara con todos los bits pares "prendidos"
     Luego le hago un AND con x, si el resultado es 0 significa que x no tiene ningun bit par "prendido".
  */
  int mask = 0x55;
  mask = mask | (mask << 8); // 0x55 55
  mask = mask | (mask << 16); // 0x55 55 55 55
  return !!(x & mask);
}

/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  int nByte = n << 3;
  int mByte = m << 3;
  int mask = 0xFF;

  // mantengo solamente los bits que necesito llevándolo al primer bit
  int nByteResult = ((x >> nByte) & mask) << mByte;
  int mByteResult = ((x >> mByte) & mask) << nByte;

  // borro los bytes a reemplazar en x
  x = x & ~(mask << nByte);
  x = x & ~(mask << mByte);

  // junto los resultados
  return x | nByteResult | mByteResult;
}

/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
   /* Me fijo si los 32-n bits restantes de x son todos 1 o todos 0. 
  ¿Por que? Hay unicamente 2 formas para expresar x en n bits, por ejemplo si n=3 puedo representar: 0x111..11xx o 0x000...00xx */

  int movimientos = 32 + (~n + 1); // -n == (~n+1) entonces esto es igual a hacer 32-n
  int aux = (x << movimientos) >> movimientos; // Depende el signo de x puede quedar 1111... o 0000...
  return !(aux ^ x); // El XOR es 0 si x == aux, esto significa que x se puede representar en n bits
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
/*
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
  /* En el primer auxiliar puedo obtener 2 resultados posibles: 0 si x == 0 o 1 si x != 0
     En el segundo auxiliar puedo obtener 2 resultados posibles: 0 si x es positivo o cero | -1 si x es negativo  
  */
  int aux1 = (!!x);
  int aux2 = (x >> 31); 
  return (aux1 | aux2);
}
//3
/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  // 1. si son de diferente signo siempre devuelve 1
  // 2. si son del mismo signo y no hubo overflow devuelve 1
  // 3. si son del mismo signo y hubo overflow devuelve 0
  int signX = x >> 31;
  int signY = y >> 31;
  int sumWithoutOverflow = (!((x + y) >> 31) ^ !signX);
  int equalSign = !(signX ^ signY); // mismo signo 1 - diferente signo 0
  return !(equalSign & sumWithoutOverflow);
}
/*

 * bitMask - Generate a mask consisting of all 1's
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
    /* En el primer auxiliar me van a quedar todos los bits "prendidos" menos los primeros highbit bits.
     En el segundo auxiliar me van a quedar todos los bits "prendidos" menos los primeros lowbit bits.
     Luego, le hago un corrimiento a izquierda al primer auxiliar y lo niego entonces me quedan "prendidos" los primeros highbit+1 bits
     Finalmente hago el AND y quedan solo "prendidos" los bits que estan entre lowbit y highbit
  */
  int mask = ~0;
  int aux1 = mask << highbit;
  int aux2 = mask << lowbit;
  return (~(aux1 << 1) & aux2);
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // si x == 0 entonces !x es 1 y ~0 es 0xFFFFFFFF y la mascara termina en 0.
  // si x != 0 entonces la mascara termina en 0xFFFFFFFF
  // al negar la mascara en la respuesta obtengo Z, si mask es 0xFFFFFFFF entonces x no era 0 y obtengo y
  int mask = !x + ~0;
  return (~mask & z) | (mask & y);
}

/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /* x debe estar entre 0x30 y 0x39, por lo tanto hago 2 calculos:
     x - 0x30 y 0x39 - x, si ambos dan un numero con signo positivo es porque el numero esta entre esos rangos
     Aclaracion: -x = (~x+1)
  */
  int minimalCase = (x + (~0x30 + 1)) >> 31; // guardo solo el bit de signo
  int maximumCase = (0x39 + (~x + 1)) >> 31; // guardo solo el bit de signo
  return !(minimalCase | maximumCase);
}
/*
 * isGreater - if x > y  then return 1, else return 0
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  // nonEqualSign & ySign -> 1 si son diferentes e y es menor que 0
  // !(nonEqualSign | substractionSign) -> 1 si tienen mismo signo y la diferencia es positiva
  int xSign = x >> 31;
  int ySign = y >> 31;
  int negativeY = ~y + 1;
  int substraction = x + negativeY; // 0 si son iguales
  int substractionSign = (substraction >> 31) & 1;
  int nonEqualSign = xSign ^ ySign; // exclusivamente los signos son diferentes
  return (!!substraction) & ((nonEqualSign & ySign) | !(nonEqualSign | substractionSign));
}
/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  /* En nByte guardo el valor de n pero multiplicado por 8 (porque cada byte es de 8 bits)
     Luego desplazo los bits de c a reemplazar al byte correspondiente.
     Dejo "apagado" los bits del byte de x que quiero reemplazar.
     Finalmente los junto con un OR.
  */
  int nByte = n << 3;
  int mask = 0xFF;
  c = c << nByte;
  x = x & ~(mask << nByte);
  return x | c;
}
//4
/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int xSign = x >> 31; // x < 0 ? 0xFFFFFFFF : 0x00000000
  return (~xSign & x) | (xSign & (~x + 1));
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int bang(int x) {
  /* Si x == 0 el signo de x | -x es 0
     Si x != 0 el signo de x | -x es 1
     Luego, con el desplazamiento a derecha de 31 bits queda 0 o 0xFF FF FF FF 
     Finalmente la suma queda 1 o 0 dependiendo como quedo con el desplazamiento 
  */
  int xNegative = ~x + 1;
  int aux = (xNegative | x) >> 31;
  return aux + 1; 
}
/*
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int isNonZero(int x) {
  return (((~x + 1)| x) >> 31) & 1;
}
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
    /* Si x == 0 el signo de x | -x es 0
       Si x != 0 el signo de x | -x es 1
       Luego, con el desplazamiento a derecha de 31 bits queda 0 o 0xFF FF FF FF 
       Finalmente la suma queda 1 o 0 dependiendo como quedo con el desplazamiento.
  */
  int xNegative = ~x + 1;
  int aux = (xNegative | x) >> 31; 

  return aux + 1;
}
//float
/*
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {
  unsigned exp = (uf >> 23) & 0xFF; // obtengo los 8 bits del exponente de la representación del float
  unsigned frac = uf << 9; // saco los bits del exp y de la representación del signo y me quedo con la parte fraccionaria
  int isNaN = exp == 0xFF && frac != 0; // exp es 0xFF y frac != 0 cuando es NaN
  unsigned result = uf & ~(1 << 31); // uf & 7F FF FF FF para analizar uf sin importar el bit de signo
  return isNaN ? uf : result;
}

/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
    unsigned fracMask, mask, f, g;
    int ufIsNaN, ugIsNaN;

    mask =  0x7FFFFFFF;
    f = uf & mask; // me quedo con uf sin el bit de signo
    g = ug & mask; //  me quedo con ug sin el bit de signo

    if((f == 0) && (g == 0)){ // veo el caso de +0 y -0
      return 1;
    }
    
    fracMask = 0x7FFFFF;
    ufIsNaN = (((f >> 23) == 0xFF) && ((fracMask & uf) != 0)); // es NaN si: exp == 0xFF y frac != 0
    ugIsNaN = (((g >> 23) == 0xFF) && ((fracMask & ug) != 0)); // es NaN si: exp == 0xFF y frac != 0
    if(ufIsNaN || ugIsNaN){ //caso en el que alguna sea NaN
      return 0;
    }

    return (uf == ug);
}

/*
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {
  unsigned exp = (uf >> 23) & 0xFF; // obtengo los 8 bits del exponente de la representación del float
  unsigned frac = uf << 9; // saco los bits del exp y de la representación del signo y me quedo con la parte fraccionaria
  int isNaN = exp == 0xFF && frac != 0; // exp es 0xFF y frac != 0 cuando es NaN
  unsigned result = uf ^ (1<<31); // invierte el big de signo
  return (isNaN) ? uf : result;
}
/*
 * floatIsLess - Compute f < g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */
int floatIsLess(unsigned uf, unsigned ug) {
    unsigned expMask, mask, f, g;
    int ufIsNaN, ugIsNaN, ufSign, ugSign, case1, case2;

    mask = 0x7FFFFFFF;
    f = uf & mask; // me quedo con uf sin el bit de signo
    g = ug & mask; //  me quedo con ug sin el bit de signo

    /* Es NaN cuando: exp == 0xFF y frac != 0, es decir si tiene esta forma: x111 1111 1xxx xxxx xxxx xxxx xxxx xxxx (expMask)
       (al menos 1 de las primeros 23 bits debe ser distinto a 0)
       Por lo tanto me fijo si uf y ug (sin importar el bit de signo) son mayores a dicha mascara */
    expMask = 0x7F800000;
    ufIsNaN = f > expMask; // es NaN si: exp == 0xFF y frac != 0
    ugIsNaN = g > expMask; // es NaN si: exp == 0xFF y frac != 0

    if(ufIsNaN || ugIsNaN || ((f == 0) && (g == 0))){ //caso en el que alguno es NaN o ambas sean cero
      return 0;
    }

    ufSign = (uf >> 31); // signo de uf
    ugSign = (ug >> 31); // signo de ug
    case1 = (ufSign > ugSign); // Esto es igual a 1 si: uf es negativo y ug es positivo
    // case2 es igual a 1 si: (ambos negativos y ug < uf) o (ambos positivos y ug > uf)
    case2 = ((ufSign == ugSign) && ((ugSign && (ug < uf)) || (!ugSign && (ug > uf))));
    
    if( case1 || case2){
      return 1;
    }

    return 0;
}
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int exp = (uf >> 23) & 0xFF;
  int bias = 127;
  int frac = uf & 0x7FFFFF;
  int e = exp - bias;
  int isNegative = (uf >> 31) & 1;

  if (exp == 0xFF || e > 30) {
    // valor especial, no importa la fraccion || e se sale de su rango (overflow)
    return 0x80000000u;
  }

  // es denormalizado y fraccion 0, devuelvo 0 || redondeo a 0
  // redondeo a 0 porque o es normalizado y el exponente es muy chico y eso está cerca del 0
  // o es un de(s)normalizado y tiende a ser muy cercano a 0
  if ((exp == 0 && frac == 0) || e < 0) {
    return 0;
  }

  // redondeo solo para normalizados, si no era normalizado nunca llega a esta instancia
  frac = frac | 0x800000;
  frac = e >= 23 ? frac << (e - 23) : frac >> (23 - e);
  
  return isNegative ? ~frac + 1 : frac;
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {

    int exp = x + 127;
    // exp = E + bias (bias = 127 por que exp es de 8 bits)
    // exp puede valer desde 1 a 254
    if(exp <= 0)
      return 0;
    else if(exp >= 255)
      return 0x7F800000; // +INF
    return exp << 23; // desplazo a exp a la posicion que corresponde
}
