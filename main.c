#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TAMANIOMEMORIA 16384
#define TAMANIOREGISTROS 32
#define TAMANIOREG 4
#define TAMANIOMEM 4
#define NUM_SEG 8

#define LAR 0
#define MAR 1
#define MBR 2
#define IP 3
#define OPC 4
#define OP1 5
#define OP2 6
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15
#define AC 16
#define CC 17
#define CS 26
#define DS 27

#define TIPO_NINGUNO 0
#define TIPO_REGISTRO 1
#define TIPO_INMEDIATO 2
#define TIPO_MEMORIA 3

typedef struct
{
    int16_t base;    // Direccion base del segmento
    int16_t tamanio; // Tamanio del segmento
} Segmento;

void printBits(uint8_t valor); // imprime bit a bit 8 bits, NO FINAL

void printBits16(uint16_t valor);// imprime bit a bit 16 bits

void printBits32(uint32_t valor); // imprime bit a bit 32 bits NO FINAL

void mostrarMemoria(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int desplazamiento); // muestra en pantalla una celda de memoria, NO FINAL

uint8_t getBit(int32_t valor, int8_t n); // devuelve el valor del n-ésimo del valor

int32_t setBit(int32_t valor, int8_t n, int8_t x); // setea el n-ésimo bit del valor con el valor en x

uint16_t BigEndianLittleEndian16(uint16_t valor); // transforma un valor de 16 bits de Big Endian a Litlle Endian

uint32_t BigEndianLittleEndian32(uint32_t valor); // transforma un valor de 32 bits de Big Endian a Litlle Endian

int32_t CrearPuntero(int16_t codSegmento, int16_t desplazamiento);

void LeerPuntero(int32_t puntero, int16_t *codSegmento, int16_t *desplazamiento);

int32_t ExtenderSigno24Bits(int32_t valor); // extiende el valor del operando a 32 bits

int32_t ExtenderSigno16Bits(int32_t valor);

int32_t obtenerValorOperando(int32_t valor, uint8_t tipo, int32_t registros[], int8_t memoria[], Segmento tabla_seg[]); // obtiene el valor del operando segun su tipo

void cambiarCC(int32_t registros[], int32_t valor); // cambia los bits N y Z del registro CC, dependiendo del valor

int8_t getN(int32_t registros[]); // obtiene el bit N del registro CC

int8_t getZ(int32_t registros[]); // obtiene el bit Z del registro CC

int32_t LeerOperando(int8_t memoria[], uint32_t *ip, uint8_t tipoOP, int mostrar);

int32_t ProcesarOPMemoria(int32_t valor, int32_t registros[], Segmento tabla_seg[]); // devuelve la dirección física de la memoria

int32_t LeerMemoria(int8_t memoria[], int32_t registros[], int32_t base); // lee de memoria los 4 bytes empezando de base

void GuardarEnMemoria(int8_t memoria[], int32_t registros[], int32_t base, int32_t valor); // guarda en memoria 4 bytes empezando de base

void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[], char* file); // inicializa la tabla de segmento, la IP, el CS, el DS y lee el archivo guardandolo en la memoria

void ejecutarPrograma(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

void leerInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

void ejecutarInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

void mov(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void add(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void sub(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void mul(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void dividir(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void cmp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void shl(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void shr(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void sar(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void and(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void or(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void xor(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void swap(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void ldl(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void ldh(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void rnd(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1);

void read(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[]);

void write(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[]);

void sys(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int32_t valor);

void Jump(int32_t registros[], Segmento tabla_seg[], uint32_t valor);

void jmp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void jz(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void jp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void jene(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void jnz(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void jnp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void jnn(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void not(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2);

void Dissasembler(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

void mostrarInstruccion(int8_t memoria[], int32_t * ip);

void mostrarOperador(int32_t op, uint8_t tipo);

void copiarRegistro(uint32_t reg, char registro[]);

void obtenerMnemonico(uint8_t byte_de_control, char mnemonico[]);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int32_t registros[TAMANIOREGISTROS];

    int8_t memoria[TAMANIOMEMORIA];

    Segmento tabla_seg[NUM_SEG];

    IniciarMaquinaVirtual(registros, memoria, tabla_seg, argv[1]);

    ejecutarPrograma(memoria, registros, tabla_seg);

    if(argv[1]!=NULL && strcmp(argv[2], "-d")==0){
        Dissasembler(memoria, registros, tabla_seg);
    }

    return 0;
}

//-------- UTILIDADES --------------------------

uint8_t getBit(int32_t valor, int8_t n)
{ // n = número de bit a retornar (0 a N-1) contando desde el bit menos significativo
    uint8_t bit;
    int64_t mascara = 2;
    mascara = pow(mascara, n);
    bit = (valor & mascara) >> n;
    return bit;
}

int32_t setBit(int32_t valor, int8_t n, int8_t x)
{ // n = número de bit a setear (0 a N-1) contando desde el bit menos significativo; x recibe el valor a setear (1 o 0)
    int64_t mascara = 2;
    mascara = pow(mascara, n);
    if (x == 0)
    {
        valor &= ~(mascara);
    }
    else
        valor |= mascara;
    return valor;
}

void mostrarMemoria(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int desplazamiento) // creo que hay que elliminarla para entregar
{
    printBits32(LeerMemoria(memoria, registros, tabla_seg[1].base + desplazamiento * TAMANIOMEM));
}

//-------- PRINTS DE BITS ----------------------

void printBits(uint8_t valor) // hay que eliminarla para entregar
{
    for (int i = 7; i >= 0; i--)
    {
        uint8_t bit = (valor >> i) & 1;
        printf("%u", bit);
    }
    printf("\n");
}

void printBits16(uint16_t valor)
{
    for (int i = 15; i >= 0; i--)
    {
        // desplazamos i posiciones y sacamos el bit
        uint16_t bit = (valor >> i) & 1;
        printf("%u", bit);
    }
}

void printBits32(uint32_t valor)
{
    for (int i = 31; i >= 0; i--)
    {
        uint32_t bit = (valor >> i) & 1;
        printf("%u", bit);
        if (i % 8 == 0 && i != 0)
            printf(" ");
    }
}

//------------------------------------------------------

//------- Big Endian a LittleEndian --------------------

uint16_t BigEndianLittleEndian16(uint16_t valor)
{
    valor = (valor >> 8) | (valor << 8);
    return valor;
}

uint32_t BigEndianLittleEndian32(uint32_t valor)
{
    valor = ((valor >> 24) & 0xff) |      // Mover byte 3 a byte 0
            ((valor << 8) & 0xff0000) |   // Mover byte 1 a byte 2
            ((valor >> 8) & 0xff00) |     // Mover byte 2 a byte 1
            ((valor << 24) & 0xff000000); // Mover byte 0 a byte 3
    return valor;
}

//------------------------------------------------------

//-------- FUNCIONES DE PUNTEROS --------------------------

int32_t CrearPuntero(int16_t codSegmento, int16_t desplazamiento)
{
    int32_t puntero = codSegmento;
    puntero = puntero << 16;
    puntero = puntero | desplazamiento;
    return puntero;
}

void LeerPuntero(int32_t puntero, int16_t *codSegmento, int16_t *desplazamiento)
{
    *codSegmento = (puntero & 0xFFFF0000) >> 16;
    *desplazamiento = (puntero & 0xFFFF);
}

//------------- FUNCIONES DE OPERANDOS ------------------

void cambiarCC(int32_t registros[], int32_t valor)
{
    registros[CC] = 0;
    if (valor == 0) // 0 en N y 1 en Z
        registros[CC] = registros[CC] | 0x40000000;
    else if (valor < 0) // 1 en N y 0 en Z
        registros[CC] = registros[CC] | 0x80000000;
}

int8_t getN(int32_t registros[]) // devuelve el valor del bit N del registro CC
{
    return getBit(registros[CC], 31);
}

int8_t getZ(int32_t registros[]) // devuelve el valor del bit Z del registro CC
{
    return getBit(registros[CC], 30);
}

int32_t LeerOperando(int8_t memoria[], uint32_t *ip, uint8_t tipoOP, int mostrar)
{
    int32_t valor = 0;

    for (int i = 0; i < tipoOP; i++)
    {
        valor = valor << 8;
        valor = valor | (uint8_t)memoria[*ip];
        if(mostrar){
            printf("%02X ", (uint8_t)memoria[*ip]);
        }
        *ip = *ip + 1;
    }

    return valor;
}

int32_t ExtenderSigno24Bits(int32_t valor)
{
    if (valor & 0x00800000)
    {                              // Si el bit 23 está en 1 entonces es un numero negativo.
        return valor | 0xFF000000; // Se rellena con 1s
    }
    else
    {
        return valor;
    }
}

int32_t ExtenderSigno16Bits(int32_t valor)
{
    if (valor & 0x00008000)
    {
        return valor | 0xFFFF0000; // Si el bit 16 está en 1 entonces es un numero negativo. // Se rellena con 1s
    }
    else
    {
        return valor;
    }
}

// obtenerValorOperando no se puede usar para el operando en el que hay que guardar los datos
int32_t obtenerValorOperando(int32_t valor, uint8_t tipo, int32_t registros[], int8_t memoria[], Segmento tabla_seg[])
{
    uint16_t dir_fis;
    uint32_t resultado = 0;

    switch (tipo)
    {
    case TIPO_REGISTRO:
        resultado = registros[valor];
        break;
    case TIPO_INMEDIATO:
        resultado = ExtenderSigno16Bits(valor);
        break;
    case TIPO_MEMORIA:
        dir_fis = ProcesarOPMemoria(valor, registros, tabla_seg);
        resultado = LeerMemoria(memoria, registros, dir_fis);
        break;
    default:
        resultado = 0;
    }
    return resultado;
}

//---------------------------------------------------------

//--------------- FUNCIONES DE MEMORIA -------------------

int32_t ProcesarOPMemoria(int32_t valor, int32_t registros[], Segmento tabla_seg[]) // Retorna la dirección física en memoria donde tiene que guardar o leer
{
    int8_t codRegistro = (valor & 0x00FF0000) >> 16;
    int16_t desplazamientoOperando = (valor & 0x0000FFFF);
    int16_t codSegmento;
    int16_t desplazamientoPuntero;
    int32_t puntero = registros[codRegistro];

    LeerPuntero(puntero, &codSegmento, &desplazamientoPuntero);

    int32_t posicion = tabla_seg[codSegmento].base + (desplazamientoPuntero + desplazamientoOperando) * TAMANIOREG;

    if (posicion > TAMANIOMEMORIA || posicion < 0)
    {
        printf("ERROR: Intento de Acceso a memoria invalido\n");
        posicion = -1;
    }

    registros[LAR] = CrearPuntero(codSegmento, desplazamientoOperando + desplazamientoPuntero); // Se Inicializa el registro LAR

    registros[MAR] = TAMANIOMEM; // Tamanio estándar de la memoria (por ahora)

    registros[MAR] = registros[MAR] << 16; // Se guarda la cantidad de bytes de lectura en la parte alta del MAR

    registros[MAR] = registros[MAR] | posicion; // Se guarda la posicion en la parte baja del registro MAR

    return posicion;
}

int32_t LeerMemoria(int8_t memoria[], int32_t registros[], int32_t base)
{ // Se "para" en la posicion de memoria "base" y lee/concatena los 4 bytes siguientes
    int32_t aux = 0;
    int i;

    if (base >= 0 && base + 3 < TAMANIOMEMORIA)
    {
        for (i = 0; i < 4; i++)
        {
            aux = aux << 8;
            aux = aux | (uint8_t)memoria[base + i];
        }

        registros[MBR] = aux;
    }
    else
    {
        printf("ERROR: Lectura de memoria");
        aux = 0; // Probablemente esté mal ya que 0 es un valor válido que puede tomar aux
    }

    return aux;
}

void GuardarEnMemoria(int8_t memoria[], int32_t registros[], int32_t base, int32_t valor)
{ // Recibe un valor de 4 bytes y lo guarda en cada byte de memoria a partir de la dirección "base"

    if (base >= 0 && base + 3 < TAMANIOMEMORIA)
    {
        memoria[base] = (valor >> 24) & 0xFF;
        memoria[base + 1] = (valor >> 16) & 0xFF;
        memoria[base + 2] = (valor >> 8) & 0xFF;
        memoria[base + 3] = (valor & 0xFF);

        registros[MBR] = valor;
    }
    else
    {
        printf("ERROR: Guardado en memoria");
    }
}

//------------------ LOOP DE EJECUCION ---------------------------------------

void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[], char file[])
{
    FILE *f;
    char id[6];
    int8_t ver;
    int16_t tamanio;

    f = fopen(file, "rb");

    fread(id, 5, 1, f); // Leer identificador

    if (strcmp(id, "VMX25") != 0)
    {
        printf("ERROR: Mal identificador");
    }
    else
    {
        fread(&ver, sizeof(uint8_t), 1, f); // Leer Version

        if (ver != 1)
        {
            printf("ERROR: Version erronea");
        }
        else
        {
            fread(&tamanio, 2, 1, f); // Leer Tamano

            tamanio = BigEndianLittleEndian16(tamanio); // Pasar Tamano a LittleEndian

            registros[CS] = CrearPuntero(0, 0); // Ponemos en 0 el valor de inicio del Code Segment

            registros[DS] = CrearPuntero(1, 0); // Ponemos en tamanio el valor de inicio del Data Segment

            fread(memoria, 1, tamanio, f); // Lectura del Archivo en la Memoria

            //-- Inicializar tabla de segmentos ---

            tabla_seg[0].base = 0;
            tabla_seg[0].tamanio = tamanio;

            tabla_seg[1].base = tamanio;
            tabla_seg[1].tamanio = TAMANIOMEMORIA - tamanio;

            //-------------------------------------

            registros[IP] = registros[CS]; // IP igual al registro CS (Inicialmente 0)
        }
    }
    fclose(f);
}

void ejecutarPrograma(int8_t memoria[], int32_t registros[], Segmento tabla_seg[])
{
    while (registros[IP] < tabla_seg[0].tamanio && registros[IP] != 0xFFFFFFFF)
    {
        leerInstruccion(memoria, registros, tabla_seg); // manda a ejecutar la siguiente instruccion mientras este IP este dentro del code segment y IP tenga valor valido
    }
}

void leerInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]) // Lee la siguiente instruccion separando el código de instrucción de sus operadores
{
    uint32_t ip = registros[IP];
    uint8_t byte_de_control = memoria[ip++];
    uint8_t tipo_opB, tipo_opA;

    registros[OP1] = 0;
    registros[OP2] = 0;

    tipo_opB = (byte_de_control >> 6) & 0x03;

    registros[OP2] = registros[OP2] | (uint32_t)tipo_opB << 24;

    tipo_opA = (byte_de_control >> 4) & 0x03;

    registros[OP1] = registros[OP1] | (uint32_t)tipo_opA << 24;

    registros[OPC] = byte_de_control & 0x1F;

    registros[OP2] = registros[OP2] | (LeerOperando(memoria, &ip, tipo_opB, 0) & 0x00ffffff);

    if (tipo_opA != 0)
        registros[OP1] = registros[OP1] | (LeerOperando(memoria, &ip, tipo_opA, 0) & 0x00ffffff);

    registros[IP] = ip;

    ejecutarInstruccion(memoria, registros, tabla_seg);
}

void ejecutarInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]) // Busca y ejecuta la función respectiva al código de la función
{
    uint8_t tipo1, tipo2;
    int32_t valor1, valor2;

    tipo1 = (registros[OP1] & 0xFF000000) >> 24;
    tipo2 = (registros[OP2] & 0xFF000000) >> 24;
    valor1 = ExtenderSigno24Bits(registros[OP1] & 0x00FFFFFF);
    valor2 = ExtenderSigno24Bits(registros[OP2] & 0x00FFFFFF);

    if ((tipo2 > 0) && (tipo1 > 0))
    {
        switch (registros[OPC])
        {
        case 0x10: // MOV
            mov(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x11: // ADD
            add(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x12: // SUB
            sub(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x13: // MUL
            mul(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x14: // DIV
            dividir(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x15: // CMP
            cmp(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x16: // SHL
            shl(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x17: // SHR
            shr(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x18: // SAR
            sar(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x19: // AND
            and(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x1A: // OR
            or(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x1B: // XOR
            xor(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x1C: // SWAP
            swap(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x1D: // LDL
            ldl(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x1E: // LDH
            ldh(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        case 0x1F: // RND
            rnd(memoria, registros, tabla_seg, tipo2, tipo1, valor2, valor1);
            break;
        }
    }
    else
    {
        switch (registros[OPC])
        {
        case 0x00: // SYS
            sys(memoria, registros, tabla_seg, valor2);
            break;
        case 0x01: // JMP
            jmp(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x02: // JZ
            jz(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x03: // JP
            jp(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x04: // JN
            jene(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x05: // JNZ
            jnz(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x06: // JNP
            jnp(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x07: // JNN
            jnn(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x08: // NOT
            not(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x0F: // STOP
            registros[IP] = 0xFFFFFFFF;
            printf("\nSTOP: Termino la ejecucion\n");
            break;
        }
    }
}

//---------------------------------------------------------------------------------------

void mov(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
        registros[valor1] = valor2;
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        GuardarEnMemoria(memoria, registros, dir_fis, valor2);
    }
    else
        printf("Error MOV Inmediato o Ninguno a Cualquiera \n");
}

void add(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado = 0;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] + valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) + valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error ADD Inmediato o ninguno += cualquiera \n");

    cambiarCC(registros, resultado);
}

void sub(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado = 0;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] - valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) - valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error SUB Inmediato o ninguno -= cualquiera \n");

    cambiarCC(registros, resultado);
}

void mul(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado = 0;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] * valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) * valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error MUL Inmediato o ninguno *= cualquiera \n");

    cambiarCC(registros, resultado);
}

void dividir(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado, aux;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (valor2 == 0)
    {
        printf("ERROR, division por cero");
        // llamar a STOP
    }
    else
    {
        if (tipo_op1 == TIPO_REGISTRO)
        {
            aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
            resultado = valor1 / valor2;
            registros[valor1] = resultado;
            registros[AC] = valor1 % valor2;
        }
        else if (tipo_op1 == TIPO_MEMORIA)
        {
            aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
            resultado = aux / valor2;
            registros[AC] = aux % valor2;
            dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
            GuardarEnMemoria(memoria, registros, dir_fis, resultado);
        }
        else
            printf("Error (Inmediato o ninugno) / cualquiera \n");

        cambiarCC(registros, resultado);
    }
}

void cmp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int32_t resultado;
    valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    resultado = valor1 - valor2;
    cambiarCC(registros, resultado);
}

void shl(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] << valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = valor1 << valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error: (Ninguno o Inmediato) << Cualquiera \n");

    cambiarCC(registros, resultado);
}

void shr(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1) // Shift-right lógico
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = (uint32_t)registros[valor1] >> valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = (uint32_t)valor1 >> valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error: (Ninguno o Inmediato) >> Cualquiera \n");

    cambiarCC(registros, resultado);
}

// En C, el operador >> agrega 0s si la variable es sin signo y agrega 1s si tiene signo.

void sar(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1) // Shift-right aritmético
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] >> valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = valor1 >> valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error: (Ninguno o Inmediato) SAR Cualquiera \n");

    cambiarCC(registros, resultado);
}

void and(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] & valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = valor1 & valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error: (Ninguno o Inmediato) & Cualquiera \n");

    cambiarCC(registros, resultado);
}

void or(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] | valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = valor1 | valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error: (Ninguno o Inmediato) | Cualquiera \n");

    cambiarCC(registros, resultado);
}

void xor(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = registros[valor1] ^ valor2;
        registros[valor1] = resultado;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = valor1 ^ valor2;
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error inmediato o ninguno ^ cualquiera \n");

    cambiarCC(registros, resultado);
}

void swap(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis1, dir_fis2;
    int32_t vaux;

    if (tipo_op1 == TIPO_REGISTRO)
    {
        vaux = registros[valor2];
        registros[valor2] = registros[valor1];
        registros[valor1] = vaux;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);

        valor1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

        GuardarEnMemoria(memoria, registros, dir_fis1, valor2);
        GuardarEnMemoria(memoria, registros, dir_fis2, valor1);
    }
    else
        printf("Error SWAP \n");
}

void ldl(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        registros[valor1] = registros[valor1] & 0xFFFF0000;
        registros[valor1] = registros[valor1] | valor2;
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) & 0xFFFF0000;
        resultado = resultado | (valor2 & 0x0000FFFF);
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error (Inmediato o Ninguno) LDL Cualquiera \n");
}

void ldh(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        registros[valor1] = registros[valor1] & 0x0000FFFF;
        registros[valor1] = registros[valor1] | (valor2 << 16);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) & 0x0000FFFF;
        resultado = resultado | (valor2 << 16);
        GuardarEnMemoria(memoria, registros, dir_fis, resultado);
    }
    else
        printf("Error (Inmediato o Ninguno) LDH Cualquiera \n");
}

void rnd(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
        registros[valor1] = rand() % valor2;
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        GuardarEnMemoria(memoria, registros, dir_fis, rand() % valor2);
    }
    else
        printf("Error RND Inmediato o Ninguno a Cualquiera \n");
}

void read(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[])
{
    int i;
    int32_t valor;
    char bin[33];
    for (i = 0; i < cantidad; i++)
    {
        fflush(stdin);
        printf("[%04X]: ", dir_fis + i * tamano);
        switch (modo)
        {
        case 0x01: // interpreta decimal
            scanf("%d", &valor);
            break;
        case 0x02: // interpreta caracteres
            scanf("%c", &valor);
            break;
        case 0x04: // interpreta octal
            scanf("%o", &valor);
            break;
        case 0x08: // interpreta hexadecimal
            scanf("%X", &valor);
            break;
        case 0x10: // interpreta binario
            scanf("%32s", bin);
            valor = (int32_t)strtol(bin, NULL, 2); // convierte una serie de caracteres en enteros
            break;
        default:
            printf("Modo para leer no valido \n");
        }
        GuardarEnMemoria(memoria, registros, dir_fis + i * tamano, valor);
        printf("\n");
    }
}

void write(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[])
{
    int i;
    int32_t valor;
    for (i = 0; i < cantidad; i++)
    {
        valor = LeerMemoria(memoria, registros, dir_fis + i * tamano);
        printf("[%04X]: ", dir_fis + i * tamano);
        if (getBit(modo, 0) == 1)
            printf("0d%d ", valor);
        if (getBit(modo, 1) == 1)
            if (valor >= 32 && valor <= 126)
                printf("Aa%c ", valor);
            else
                printf("Aa . ");
        if (getBit(modo, 2) == 1)
            printf("0o%o ", valor);
        if (getBit(modo, 3) == 1)
            printf("0x%X ", valor);
        if (getBit(modo, 4) == 1){
            printf("0b");
            printBits32(valor);
        }
        if (modo <= 0 || modo > 0x1F)
            printf("Error, modo de escritura invalido \n");
        printf("\n");
    }
}

void sys(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int32_t valor)
{
    int32_t modo, puntero;
    int16_t codSeg, desplazamiento, dir_fis, cantidad, tamano;
    modo = registros[EAX];
    puntero = registros[EDX];
    LeerPuntero(puntero, &codSeg, &desplazamiento);
    dir_fis = tabla_seg[codSeg].base + desplazamiento*TAMANIOMEM;
    cantidad = registros[ECX] & 0xFFFF;
    tamano = (registros[ECX] >> 16) & 0xFFFF;
    switch (valor)
    {
    case 0x1: // Lectura
        read(dir_fis, cantidad, tamano, modo, memoria, registros);
        break;
    case 0x2: // Escritura
        write(dir_fis, cantidad, tamano, modo, memoria, registros);
        break;
    default:
        printf("Modo para el SYS no valido \n");
    }
}

void Jump(int32_t registros[], Segmento tabla_seg[], uint32_t valor)
{

    if (valor >= 0 && valor < tabla_seg[0].tamanio)
    {
        registros[IP] = valor;
    }
    else
    {
        printf("ERROR: Se intento acceder afuera del segmento de codigo\n");
    }
}

void jmp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    Jump(registros, tabla_seg, valor2);
}

void jz(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    if (getZ(registros) == 1 && getN(registros) == 0)
        Jump(registros, tabla_seg, valor2);
}

void jp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    if (getZ(registros) == 0 && getN(registros) == 0)
        Jump(registros, tabla_seg, valor2);
}

void jene(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    if (getN(registros))
        Jump(registros, tabla_seg, valor2);
}

void jnz(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    if (getZ(registros) == 0)
        Jump(registros, tabla_seg, valor2);
}

void jnp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    if (getZ(registros) || getN(registros))
        Jump(registros, tabla_seg, valor2);
}

void jnn(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    if (getN(registros) == 0)
        Jump(registros, tabla_seg, valor2);
}

void not(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, int32_t valor2)
{
    int i;
    int16_t dir_fis;
    uint32_t bit;
    int32_t resultado = 0;

    if (tipo_op2 != TIPO_INMEDIATO && tipo_op2 != TIPO_NINGUNO)
    {
        resultado = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
        resultado = ~resultado;
        if (tipo_op2 == TIPO_REGISTRO)
            registros[valor2] = resultado;
        else
        {
            dir_fis = ProcesarOPMemoria(valor2, registros, tabla_seg);
            GuardarEnMemoria(memoria, registros, dir_fis, resultado);
        }
        cambiarCC(registros, resultado);
    }
    else
        printf("Error: NOT aplicado a un inmediato o ninguno \n");
}

void Dissasembler(int8_t memoria[], int32_t registros[], Segmento tabla_seg[])
{
    int32_t ip=tabla_seg[0].base;

    while (ip < tabla_seg[0].tamanio && ip != 0xFFFFFFFF)
    {
        mostrarInstruccion(memoria, &ip); // manda a mostrar la siguiente instruccion mientras este IP este dentro del code segment y IP tenga valor valido
    }
}

void mostrarInstruccion(int8_t memoria[], int32_t * ip) // Lee la siguiente instruccion separando el código de instrucción de sus operadores
{
    uint8_t byte_de_control = memoria[(*ip)];
    uint8_t tipo_opB, tipo_opA;
    int32_t op1,op2;
    char mnem[5];

    printf("[%04X] ", *ip);

    printf("%02X ", byte_de_control);

    (*ip)+=1;

    tipo_opB = (byte_de_control >> 6) & 0x03;

    tipo_opA = (byte_de_control >> 4) & 0x03;

    byte_de_control = byte_de_control & 0x1F;

    op2 = LeerOperando(memoria, ip, tipo_opB, 1);

    if (tipo_opA != 0)
        op1 = LeerOperando(memoria, ip, tipo_opA, 1);

    printf("\t|   ");

    obtenerMnemonico(byte_de_control, mnem);

    printf("%s\t", mnem);

    if (tipo_opA != 0){

        mostrarOperador(op1, tipo_opA);
        printf(",\t");
    }

    mostrarOperador(op2, tipo_opB);

    printf("\n");
}

void mostrarOperador(int32_t op, uint8_t tipo){
    char reg[4];
    uint8_t codRegistro;
    int16_t desplazamientoOperando;

    switch (tipo){

    case TIPO_REGISTRO:
            copiarRegistro(op, reg);
            printf("%s", reg);
            break;
    case TIPO_INMEDIATO: printf("%d", op);
            break;
    case TIPO_MEMORIA:
            codRegistro=(op >> 16) & 0xFF;
            desplazamientoOperando = (op & 0x0000FFFF);
            copiarRegistro(codRegistro, reg);

            if(desplazamientoOperando==0){
                printf("[%s]", reg);
            }else if(desplazamientoOperando>0){
                printf("[%s+%d]", reg, desplazamientoOperando);
            }else{
                printf("[%s%d]", reg, desplazamientoOperando);
            }
            break;
    }
}

void copiarRegistro(uint32_t reg, char registro[]){
    switch(reg){
        case LAR:
            strcpy(registro, "LAR");
            break;
        case MAR:
            strcpy(registro, "MAR");
            break;
        case MBR:
            strcpy(registro, "MBR");
            break;
        case IP:
            strcpy(registro, "IP");
            break;
        case OPC:
            strcpy(registro, "OPC");
            break;
        case OP1:
            strcpy(registro, "OP1");
            break;
        case OP2:
            strcpy(registro, "OP2");
            break;
        case EAX:
            strcpy(registro, "EAX");
            break;
        case EBX:
            strcpy(registro, "EBX");
            break;
        case ECX:
            strcpy(registro, "ECX");
            break;
        case EDX:
            strcpy(registro, "EDX");
            break;
        case EEX:
            strcpy(registro, "EEX");
            break;
        case EFX:
            strcpy(registro, "EFX");
            break;
        case AC:
            strcpy(registro, "AC");
            break;
        case CC:
            strcpy(registro, "CC");
            break;
        case CS:
            strcpy(registro, "CS");
            break;
        case DS:
            strcpy(registro, "DS");
            break;
        default: strcpy(registro, "");
    }
}

void obtenerMnemonico(uint8_t byte_de_control, char mnemonico[]){
    switch (byte_de_control){
        case 0x00: // SYS
            strcpy(mnemonico,"SYS");
            break;
        case 0x01: // JMP
            strcpy(mnemonico,"JMP");
            break;
        case 0x02: // JZ
            strcpy(mnemonico,"JZ");
            break;
        case 0x03: // JP
            strcpy(mnemonico,"JP");
            break;
        case 0x04: // JN
            strcpy(mnemonico,"JN");
            break;
        case 0x05: // JNZ
            strcpy(mnemonico,"JNZ");
            break;
        case 0x06: // JNP
            strcpy(mnemonico,"JNP");
            break;
        case 0x07: // JNN
            strcpy(mnemonico,"JNN");
            break;
        case 0x08: // NOT
            strcpy(mnemonico,"NOT");
            break;
        case 0x0F: // STOP
            strcpy(mnemonico,"STOP");
            break;
        case 0x10: // MOV
            strcpy(mnemonico,"MOV");
            break;
        case 0x11: // ADD
            strcpy(mnemonico,"ADD");
            break;
        case 0x12: // SUB
            strcpy(mnemonico,"SUB");
            break;
        case 0x13: // MUL
            strcpy(mnemonico,"MUL");
            break;
        case 0x14: // DIV
            strcpy(mnemonico,"DIV");
            break;
        case 0x15: // CMP
            strcpy(mnemonico,"CMP");
            break;
        case 0x16: // SHL
            strcpy(mnemonico,"SHL");
            break;
        case 0x17: // SHR
            strcpy(mnemonico,"SHR");
            break;
        case 0x18: // SAR
            strcpy(mnemonico,"SAR");
            break;
        case 0x19: // AND
            strcpy(mnemonico,"AND");
            break;
        case 0x1A: // OR
            strcpy(mnemonico,"OR");
            break;
        case 0x1B: // XOR
            strcpy(mnemonico,"XOR");
            break;
        case 0x1C: // SWAP
            strcpy(mnemonico,"SWAP");
            break;
        case 0x1D: // LDL
            strcpy(mnemonico,"LDL");
            break;
        case 0x1E: // LDH
            strcpy(mnemonico,"LDH");
            break;
        case 0x1F: // RND
            strcpy(mnemonico,"RND");
            break;
        }
}

//---------------------------------------------------------------------------------------
