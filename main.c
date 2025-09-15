#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TAMANIOMEMORIA 16384
#define TAMANIOREGISTROS 32

#define TAMANIOREG 4
#define TAMANIOMEM 4
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
#define NUM_SEG 8

#define TIPO_NINGUNO 0
#define TIPO_REGISTRO 1
#define TIPO_INMEDIATO 2
#define TIPO_MEMORIA 3

typedef struct
{
    int16_t base;    // Direccion base del segmento
    int16_t tamanio; // Tamanio del segmento
} Segmento;

void printBits(uint8_t valor);

void printBits16(uint16_t valor);

void printBits32(uint32_t valor);

uint16_t BigEndianLittleEndian16(uint16_t valor);

uint32_t BigEndianLittleEndian32(uint32_t valor);

int32_t CrearPuntero(int16_t codSegmento, int16_t desplazamiento);

int32_t LeerPuntero(int32_t puntero, int16_t * codSegmento, int16_t * desplazamiento);

int32_t ExtenderSigno24Bits(int32_t valor);

void cambiarCC(int32_t registros[], int32_t valor);

int32_t LeerOperando(int8_t memoria[], uint32_t *ip, uint8_t tipoOP);

int32_t ProcesarOPMemoria(int32_t valor, int32_t registros[], Segmento tabla_seg[]);

int32_t LeerMemoria(int8_t memoria[], int32_t registros[],int32_t base);

void GuardarEnMemoria(int8_t memoria[],int32_t registros[],int32_t base, int32_t valor);

void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[]);

int32_t LeerOperando(int8_t memoria[], uint32_t *ip, uint8_t tipoOP);

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

int main()
{
    int32_t registros[TAMANIOREGISTROS];

    int8_t memoria[TAMANIOMEMORIA];

    Segmento tabla_seg[NUM_SEG];

    IniciarMaquinaVirtual(registros, memoria, tabla_seg);

    ejecutarPrograma(memoria, registros, tabla_seg);

    return 0;
}

//-------- PRINTS DE BITS ----------------------

void printBits(uint8_t valor)
{
    for (int i = 7; i >= 0; i--)
    {
        // desplazamos i posiciones y sacamos el bit
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

        if (i % 8 == 0 && i != 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}

void printBits32(uint32_t valor)
{
    for (int i = 31; i >= 0; i--)
    {
        uint32_t bit = (valor >> i) & 1;
        printf("%u", bit);

        // opcional: espacio cada 8 bits (un byte)
        if (i % 8 == 0 && i != 0)
        {
            printf(" ");
        }
    }
    printf("\n");
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

int32_t CrearPuntero(int16_t codSegmento, int16_t desplazamiento){
    int32_t puntero=codSegmento;
    puntero=puntero<<16;
    puntero=puntero | desplazamiento;
    return puntero;
}

int32_t LeerPuntero(int32_t puntero, int16_t *codSegmento, int16_t *desplazamiento){
    *codSegmento=(puntero & 0xFFFF0000)>>16;
    *desplazamiento=(puntero & 0xFFFF);
}

//------------- FUNCIONES DE OPERANDOS ------------------

void cambiarCC(int32_t registros[], int32_t valor) {
    registros[CC] = 0;
    if (valor == 0) // 0 en N y 1 en Z
        registros[CC] = registros[CC] | 0x40000000;
    else if (valor < 0) //1 en N y 0 en Z
        registros[CC] = registros[CC] | 0x80000000;

    printf("CC: "); printBits32(registros[CC]);
}

int32_t LeerOperando(int8_t memoria[], uint32_t *ip, uint8_t tipoOP)
{

    int32_t valor = 0;

    for (int i = 0; i < tipoOP; i++)
    {
        valor = valor << 8;
        valor = valor | memoria[*ip];
        *ip = *ip + 1;
    }
    return valor;
}

int32_t ExtenderSigno24Bits(int32_t valor) {
    if (valor & 0x00800000){ // Si el bit 23 está en 1 entonces es un numero negativo.
        return valor | 0xFF000000;  // Se rellena con 1s
    } else {
        return valor & 0x00FFFFFF;  // No hace nada
    }
}

//---------------------------------------------------------

//--------------- FUNCIONES DE MEMORIA -------------------

int32_t ProcesarOPMemoria(int32_t valor, int32_t registros[], Segmento tabla_seg[]) //Retorna la posicion de memoria donde tiene que guardar o leer
{
    int8_t codRegistro = (valor & 0x00FF0000) >> 16;
    int16_t desplazamientoOperando = (valor & 0x0000FFFF);
    int16_t codSegmento;
    int16_t desplazamientoPuntero;
    int32_t puntero = registros[codRegistro]; //Chequear que el registro funcione antes

    LeerPuntero(puntero, &codSegmento, &desplazamientoPuntero);

    int32_t posicion = tabla_seg[codSegmento].base+(desplazamientoPuntero+desplazamientoOperando)*TAMANIOREG;

    if(posicion>TAMANIOMEMORIA || posicion<0){
        printf("ERROR: Intento de Acceso a memoria invalido\n");
        posicion=-1;
    }

    registros[LAR]=CrearPuntero(codSegmento, desplazamientoOperando+desplazamientoPuntero); //Se Inicializa el registro LAR

    registros[MAR]=TAMANIOMEM; //Tamanio estándar de la memoria (por ahora)

    registros[MAR]=registros[MAR]<<16; //Se guarda la cantidad de bytes de lectura en la parte alta del MAR

    registros[MAR]=registros[MAR]|posicion; //Se guarda la posicion en la parte baja del registro MAR

    return posicion;
}

int32_t LeerMemoria(int8_t memoria[], int32_t registros[], int32_t base){ //Se "para" en la posicion de memoria "base" y lee/concatena los 4 bytes siguientes
    int32_t aux=0;
    int i;

    if(base+4<TAMANIOMEMORIA){
        for(i=0; i<4; i++){
            aux=aux<<8;
            aux=aux|memoria[base+i];
        }

        registros[MBR]=aux;
    }else{
        printf("ERROR: Lectura de memoria");
        aux=0; //Probablemente esté mal ya que 0 es un valor válido que puede tomar aux
    }

    return aux;
}

void GuardarEnMemoria(int8_t memoria[], int32_t registros[],int32_t base, int32_t valor){ //Recibe un valor de 4 bytes y lo guarda en cada byte de memoria a partir de la dirección "base"

    if(base+4<TAMANIOMEMORIA){
        memoria[base]=(valor & 0xFF000000) >> 24;
        memoria[base+1]=(valor & 0x00FF0000) >> 16;
        memoria[base+2]=(valor & 0x0000FF00) >> 8;
        memoria[base+3]=(valor & 0x000000FF);

        registros[MBR]=valor;
    }else{
        printf("ERROR: Guardado en memoria");
    }
}

//------------------ LOOP DE EJECUCION ---------------------------------------

void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[])
{ // cambie en registro int8_t por int32_t

    FILE *f;

    int8_t id[6];

    int8_t ver;

    int16_t tamanio;

    f = fopen("prueba.vmx", "rb");

    fread(id, 5, 1, f); // Leer identificador

    if (strcmp(id, "VMX25")!=0)
    {
        perror("ERROR: Mal identificador");
    }
    else
    {
        fread(&ver, sizeof(uint8_t), 1, f); // Leer Version

        if (ver != 1)
        {
            perror("ERROR: Version erronea");
        }
        else
        {
            fread(&tamanio, 2, 1, f); // Leer Tamano

            tamanio = BigEndianLittleEndian16(tamanio); // Pasar Tamano a LittleEndian

            // Cada vez que vayamos a usar un valor de un registro hay que pasarlo a LittleEndian

            registros[CS] = CrearPuntero(0, 0); // Ponemos en 0 el valor de inicio del Code Segment

            registros[DS] = CrearPuntero(1, 0); // Ponemos en tamanio el valor de inicio del Data Segment

            // Hay que hacerlo asi porque la computadora funciona en Little-Indian, por lo tanto lee los binarios al reves.

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
        printf("EAX: "); printBits32(registros[EAX]);
        printf("EBX: "); printBits32(registros[EBX]);
        printf("CC: "); printBits32(registros[CC]);
        printf("\n \n");
    }
}

void leerInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]) //Lee la siguiente instruccion separando el código de instrucción de sus operadores
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

    registros[OP2] = registros[OP2] | (LeerOperando(memoria, &ip, tipo_opB) & 0x00ffffff);

    registros[OP1] = registros[OP1] | (LeerOperando(memoria, &ip, tipo_opA) & 0x00ffffff);

    registros[IP] = ip;

    ejecutarInstruccion(memoria, registros, tabla_seg);
}

void ejecutarInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]) //Busca y ejecuta la función respectiva al código de la función
{
    uint8_t tipo1 = (registros[OP1] & 0xFF000000) >> 24;
    uint8_t tipo2 = (registros[OP2] & 0xFF000000) >> 24;

    int32_t valor1 = ExtenderSigno24Bits(registros[OP1] & 0x00FFFFFF);
    int32_t valor2 = ExtenderSigno24Bits(registros[OP2] & 0x00FFFFFF);

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
            break;
        case 0x19: // AND
            break;
        case 0x1A: // OR
            break;
        case 0x1B: // XOR
            break;
        case 0x1C: // SWAP
            break;
        case 0x1D: // LDL
            break;
        case 0x1E: // LDH
            break;
        case 0x1F: // RND
            break;
        }
    }
    else
    {
        switch (registros[OPC])
        {
        case 0x00: // SYS
            break;
        case 0x01: // JMP
            break;
        case 0x02: // JZ
            break;
        case 0x03: // JP
            break;
        case 0x04: // JN
            break;
        case 0x05: // JNZ
            break;
        case 0x06: // JNP
            break;
        case 0x07: // JNN
            break;
        case 0x08: // NOT
            break;
        case 0x0F: // STOP
            registros[IP]=0xFFFFFFFF;
            printf("\nSTOP: Termino la ejecucion\n");
            break;
        }
    }
}

//---------------------------------------------------------------------------------------

void mov(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis1, dir_fis2;

    switch (tipo_op1)
    {
    case 1:
        switch (tipo_op2)
        {
        case 1: // reg <- reg
            registros[valor1] = registros[valor2];
            break;
        case 2: // reg <- inmediato
            registros[valor1] = valor2;
            break;
        case 3: // reg <- memoria
            dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
            registros[valor1] = LeerMemoria(memoria,registros,dir_fis2);
            break;
        default:
            printf("Error \n");
        }
        break;
    case 3:
        dir_fis1=ProcesarOPMemoria(valor1, registros, tabla_seg);

        //printBits32(dir_fis1);

        switch (tipo_op2)
        {
        case 1: // memoria <- registro
            GuardarEnMemoria(memoria,registros, dir_fis1, registros[valor2]);
            break;
        case 2: // memoria <- inmediato
            GuardarEnMemoria(memoria,registros, dir_fis1, valor2);
            break;
        case 3: // memoria <- memoria
            dir_fis2=ProcesarOPMemoria(valor2, registros, tabla_seg);
            GuardarEnMemoria(memoria,registros, dir_fis1, LeerMemoria(memoria,registros,dir_fis2));
            break;
        default:
            printf("Error 1\n");
        }
        break;
    default:
        printf("Error 2\n");
    }
}

//----------------------------------------------------------------------------------------

void add(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1){
    int16_t dir_fis1, dir_fis2;
    int32_t resultado;

    switch (tipo_op1){
        case TIPO_REGISTRO:
            switch (tipo_op2) {
                case TIPO_REGISTRO: // registro += registro
                    registros[valor1] += registros[valor2];
                    break;
                case TIPO_INMEDIATO: // registro += inmediato
                    registros[valor1] += valor2;
                    break;
                case TIPO_MEMORIA: // registro += memoria
                    dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                    registros[valor1] += LeerMemoria(memoria, registros, dir_fis2);
                    break;
                default:
                    printf("Error \n");
            }
            cambiarCC(registros, registros[valor1]);
            break;
        case TIPO_MEMORIA:
                dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
                resultado = LeerMemoria(memoria, registros, dir_fis1);
                switch (tipo_op2) {
                    case TIPO_REGISTRO: // memoria += registro
                        resultado = resultado + registros[valor2];
                        break;
                    case TIPO_INMEDIATO: // memoria += inmediato
                        resultado = resultado + valor2;
                        break;
                    case TIPO_MEMORIA: // memoria += memoria
                        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                        resultado = resultado + LeerMemoria(memoria, registros, dir_fis2);
                        break;
                    default:
                        printf("Error ADD memoria += ninguno \n");
                }
                GuardarEnMemoria(memoria, registros, dir_fis1, resultado);
                cambiarCC(registros, resultado);
            break;
        default:
            printf("Error: ADD ninguno o inmediato += cualquiera");
    }
}

void sub(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1){
    int16_t dir_fis1, dir_fis2;
    int32_t resultado;

    switch (tipo_op1){
        case TIPO_REGISTRO:
            switch (tipo_op2) {
                case TIPO_REGISTRO: // registro -= registro
                    registros[valor1] -= registros[valor2];
                    break;
                case TIPO_INMEDIATO: // registro -= inmediato
                    registros[valor1] -= valor2;
                    break;
                case TIPO_MEMORIA: // registro -= memoria
                    dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                    registros[valor1] -= LeerMemoria(memoria, registros, dir_fis2);
                    break;
                default:
                    printf("Error \n");
            }
            cambiarCC(registros, registros[valor1]);
            break;
        case TIPO_MEMORIA:
                dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
                resultado = LeerMemoria(memoria, registros, dir_fis1);
                switch (tipo_op2) {
                    case TIPO_REGISTRO: // memoria -= registro
                        resultado = resultado - registros[valor2];
                        break;
                    case TIPO_INMEDIATO: // memoria += inmediato
                        resultado = resultado - valor2;
                        break;
                    case TIPO_MEMORIA: // memoria -= memoria
                        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                        resultado = resultado - LeerMemoria(memoria, registros, dir_fis2);
                        break;
                    default:
                        printf("Error SUB memoria -= ninguno \n");
                }
                GuardarEnMemoria(memoria, registros, dir_fis1, resultado);
                cambiarCC(registros, resultado);
                break;
        default:
            printf("Error: SUB ninguno o inmediato -= cualquiera");
    }
}

void mul(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1){
    int16_t dir_fis1, dir_fis2;
    int32_t resultado;

    switch (tipo_op1){
        case TIPO_REGISTRO:
            switch (tipo_op2) {
                case TIPO_REGISTRO: // registro *= registro
                    registros[valor1] *= registros[valor2];
                    break;
                case TIPO_INMEDIATO: // registro *= inmediato
                    registros[valor1] *= valor2;
                    break;
                case TIPO_MEMORIA: // registro *= memoria
                    dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                    registros[valor1] *= LeerMemoria(memoria, registros, dir_fis2);
                    break;
                default:
                    printf("Error \n");
            }
            cambiarCC(registros, registros[valor1]);
            break;
        case TIPO_MEMORIA:
                dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
                resultado = LeerMemoria(memoria, registros, dir_fis1);
                switch (tipo_op2) {
                    case TIPO_REGISTRO: // memoria *= registro
                        resultado = resultado * registros[valor2];
                        break;
                    case TIPO_INMEDIATO: // memoria *= inmediato
                        resultado = resultado * valor2;
                        break;
                    case TIPO_MEMORIA: // memoria *= memoria
                        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                        resultado = resultado * LeerMemoria(memoria, registros, dir_fis2);
                        break;
                    default:
                        printf("Error MUL memoria *= ninguno \n");
                }
                GuardarEnMemoria(memoria, registros, dir_fis1, resultado);
                cambiarCC(registros, resultado);
                break;
        default:
            printf("Error: MUL ninguno o inmediato *= cualquiera");
    }
}

void dividir(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1){
    int16_t dir_fis1, dir_fis2;
    int32_t resultado, aux;

    if (valor2 == 0) {
        perror("ERROR, division por cero");
        //llamar a STOP
    }
    else {
        switch (tipo_op1){
            case TIPO_REGISTRO:
                switch (tipo_op2) {
                    case TIPO_REGISTRO: // registro /= registro
                        registros[valor1] = registros[valor1] / registros[valor2];
                        registros[AC] = registros[valor1] % registros[valor2];
                        break;
                    case TIPO_INMEDIATO: // registro /= inmediato
                        registros[valor1] = registros[valor1] / valor2;
                        registros[AC] = registros[valor1] % valor2;
                        break;
                    case TIPO_MEMORIA: // registro /= memoria
                        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                        aux = LeerMemoria(memoria, registros, dir_fis2);
                        registros[valor1] = registros[valor1] / aux;
                        registros[AC] = registros[valor1] % aux;
                        break;
                    default:
                        printf("Error \n");
                }
                cambiarCC(registros, registros[valor1]);
                break;
            case TIPO_MEMORIA:
                    dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
                    resultado = LeerMemoria(memoria, registros, dir_fis1);
                    switch (tipo_op2) {
                        case TIPO_REGISTRO: // memoria /= registro
                            resultado = resultado / registros[valor2];
                            registros[AC] = resultado % registros[valor2];
                            break;
                        case TIPO_INMEDIATO: // memoria /= inmediato
                            resultado = resultado / valor2;
                            registros[AC] = resultado % valor2;
                            break;
                        case TIPO_MEMORIA: // memoria /= memoria
                            dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                            aux = LeerMemoria(memoria, registros, dir_fis2);
                            resultado = resultado / aux;
                            registros[AC] = resultado % aux;
                            break;
                        default:
                            printf("Error DIV memoria /= ninguno \n");
                    }
                    GuardarEnMemoria(memoria, registros, dir_fis1, resultado);
                    cambiarCC(registros, resultado);
                    break;
            default:
                printf("Error: DIV ninguno o inmediato /= cualquiera");
        }
    }
}

void cmp(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1){
    int16_t dir_fis1, dir_fis2;
    int32_t resultado;

    switch (tipo_op1){
        case TIPO_REGISTRO:
            resultado = registros[valor1];
            switch (tipo_op2) {
                case TIPO_REGISTRO: // registro cmp registro
                    resultado -= registros[valor2];
                    break;
                case TIPO_INMEDIATO: // registro cmp inmediato
                    resultado -= valor2;
                    break;
                case TIPO_MEMORIA: // registro cmp memoria
                    dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                    resultado -= LeerMemoria(memoria, registros, dir_fis2);
                    break;
                default:
                    printf("Error \n");
            }
            cambiarCC(registros, resultado);
            break;
        case TIPO_INMEDIATO:
            resultado = valor1;
            switch (tipo_op2) {
                case TIPO_REGISTRO: // inmediato cmp registro
                    resultado -= registros[valor1];
                    break;
                case TIPO_INMEDIATO: // inmediato cmp inmediato
                    resultado -= valor2;
                    break;
                case TIPO_MEMORIA: // inmediato cmp memoria
                    dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                    resultado -= LeerMemoria(memoria, registros, dir_fis2);
                    break;
                default:
                    printf("Error CMP 2");
            }
            break;
        case TIPO_MEMORIA:
                dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
                resultado = LeerMemoria(memoria, registros, dir_fis1);
                switch (tipo_op2) {
                    case TIPO_REGISTRO: // memoria cmp registro
                        resultado = resultado - registros[valor2];
                        break;
                    case TIPO_INMEDIATO: // memoria cmp inmediato
                        resultado = resultado - valor2;
                        break;
                    case TIPO_MEMORIA: // memoria cmp memoria
                        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                        resultado = resultado - LeerMemoria(memoria, registros, dir_fis2);
                        break;
                    default:
                        printf("Error CMP memoria cmp ninguno \n");
                }
                cambiarCC(registros, resultado);
                break;
        default:
            printf("Error: CMP ninguno CMP cualquiera");
    }
}

void shl(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1) {
    int16_t dir_fis1, dir_fis2;
    int32_t resultado;

    switch (tipo_op1){
        case TIPO_REGISTRO:
            switch (tipo_op2) {
                case TIPO_REGISTRO: // registro << registro
                    registros[valor1] = registros[valor1] << registros[valor2];
                    break;
                case TIPO_INMEDIATO: // registro << inmediato
                    registros[valor1] = registros[valor1] << valor2;
                    break;
                case TIPO_MEMORIA: // registro << memoria
                    dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                    registros[valor1] = registros[valor1] << LeerMemoria(memoria, registros, dir_fis2);
                    break;
                default:
                    printf("Error \n");
            }
            cambiarCC(registros, registros[valor1]);
            break;
        case TIPO_MEMORIA:
                dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
                resultado = LeerMemoria(memoria, registros, dir_fis1);
                switch (tipo_op2) {
                    case TIPO_REGISTRO: // memoria << registro
                        resultado = resultado << registros[valor2];
                        break;
                    case TIPO_INMEDIATO: // memoria << inmediato
                        resultado = resultado << valor2;
                        break;
                    case TIPO_MEMORIA: // memoria << memoria
                        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                        resultado = resultado << LeerMemoria(memoria, registros, dir_fis2);
                        break;
                    default:
                        printf("Error SHL memoria << ninguno \n");
                }
                GuardarEnMemoria(memoria, registros, dir_fis1, resultado);
                cambiarCC(registros, resultado);
                break;
        default:
            printf("Error: SHL ninguno o inmediato << cualquiera");
    }
}

void shr(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1) {
    int16_t dir_fis1, dir_fis2;
    int32_t resultado;

    switch (tipo_op1){
        case TIPO_REGISTRO:
            switch (tipo_op2) {
                case TIPO_REGISTRO: // registro >> registro
                    registros[valor1] = registros[valor1] >> registros[valor2];
                    break;
                case TIPO_INMEDIATO: // registro >> inmediato
                    registros[valor1] = registros[valor1] >> valor2;
                    break;
                case TIPO_MEMORIA: // registro >> memoria
                    dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                    registros[valor1] = registros[valor1] >> LeerMemoria(memoria, registros, dir_fis2);
                    break;
                default:
                    printf("Error \n");
            }
            cambiarCC(registros, registros[valor1]);
            break;
        case TIPO_MEMORIA:
                dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
                resultado = LeerMemoria(memoria, registros, dir_fis1);
                switch (tipo_op2) {
                    case TIPO_REGISTRO: // memoria >> registro
                        resultado = resultado >> registros[valor2];
                        break;
                    case TIPO_INMEDIATO: // memoria >> inmediato
                        resultado = resultado >> valor2;
                        break;
                    case TIPO_MEMORIA: // memoria >> memoria
                        dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
                        resultado = resultado >> LeerMemoria(memoria, registros, dir_fis2);
                        break;
                    default:
                        printf("Error SHR memoria >> ninguno \n");
                }
                GuardarEnMemoria(memoria, registros, dir_fis1, resultado);
                cambiarCC(registros, resultado);
                break;
        default:
            printf("Error: SHR ninguno o inmediato >> cualquiera");
    }
}
