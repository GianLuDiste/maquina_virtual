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
#define SP 7
#define BP 8
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
#define ES 28
#define SS 29
#define KS 30
#define PS 31

#define TIPO_NINGUNO 0
#define TIPO_REGISTRO 1
#define TIPO_INMEDIATO 2
#define TIPO_MEMORIA 3

typedef struct
{
    int16_t base;    // Direccion base del segmento
    int16_t tamanio; // Tamanio del segmento
    char cod[3];
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

void LeerPuntero(Segmento tabla_seg[], int32_t puntero, int16_t *codSegmento, int16_t *desplazamiento);

int32_t ExtenderSigno24Bits(int32_t valor); // extiende el valor del operando a 32 bits

int32_t ExtenderSigno16Bits(int32_t valor);

int32_t obtenerValorOperando(int32_t valor, uint8_t tipo, int32_t registros[], int8_t memoria[], Segmento tabla_seg[]); // obtiene el valor del operando segun su tipo

int obtenerCodSegmento(Segmento tabla_seg[], char cod[]);

void cambiarCC(int32_t registros[], int32_t valor); // cambia los bits N y Z del registro CC, dependiendo del valor

int8_t getN(int32_t registros[]); // obtiene el bit N del registro CC

int8_t getZ(int32_t registros[]); // obtiene el bit Z del registro CC

int32_t LeerOperando(int8_t memoria[], uint32_t *ip, uint8_t tipoOP, int mostrar);

int32_t ProcesarOPMemoria(int32_t valor, int32_t registros[], Segmento tabla_seg[]); // devuelve la dirección física de la memoria

int32_t LeerMemoria(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int32_t base, int cant, char codSegmento[]);

void GuardarEnMemoria(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int32_t base, int32_t valor, int cant, char codSegmento[]);

void GuardarEnRegistro(int32_t registros[], int8_t reg, int32_t valorGuardar);

void guardarImagenVmi(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint32_t tam_memoria, char nombre_archivo[]);

void leerImagenVmi(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char nombre_archivo[], uint32_t *tam_mem);

void InicializarMV(int32_t registros[], Segmento tabla_seg[], int argc, char * argv[]);

void LeerParametros(int8_t memoria[], int32_t registros[], Segmento tabla_seg[] , int argc, char * argv[], int i);

void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[], int tamano, char filevmx[]); // inicializa la tabla de segmento, la IP, el CS, el DS y lee el archivo guardandolo en la memoria

void ejecutarPrograma(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char filevmi[]);

void leerInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char filevmi[]);

void ejecutarInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char filevmi[]);

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

void read(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

void write(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

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

void push(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int8_t tipo, int32_t valor);

void pop(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int8_t tipo, int32_t valor);

void Dissasembler(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

void mostrarInstruccion(int8_t memoria[], int32_t * ip);

void mostrarOperador(int32_t op, uint8_t tipo);

void copiarRegistro(uint32_t reg, char registro[]);

void obtenerMnemonico(uint8_t byte_de_control, char mnemonico[]);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int32_t registros[TAMANIOREGISTROS];

    Segmento tabla_seg[NUM_SEG];

    InicializarMV(registros, tabla_seg, argc, argv);

    return 0;
}

void InicializarMV(int32_t registros[], Segmento tabla_seg[], int argc, char * argv[]){

    int i, vmi, vmx;

    char * opcional;

    int tamano;

    if(strstr(argv[1], ".vmi")!=NULL){ // guardo la posicion de cada archivo en el vector de argumentos
        //Cargamos la imagen .vmi
        vmx = 0;
        vmi = 1;
        i=2;

    }else if(strstr(argv[2], ".vmi")!=NULL){
        //Tenemos un .vmx y un .vmi
        vmx = 1;
        vmi = 2; //
        i=3;
    }else{
        //Cargamos el .vmx sólo
        vmx = 1;
        vmi = 0;
        i=2;
    }

    tamano = TAMANIOMEMORIA; //Memoria por defecto cuando no hay un -m

    //Se fija si hay un -m y si existe obtiene el valor de bytes total que va a ocupar la memoria.
    if (i < argc) {
        opcional = strtok(argv[i], "="); //Divide un string en diferentes cadenas separandolas por el segundo parámetro
        if(i<argc && strcmp(opcional, "m")==0){
            tamano = atoi(strtok(NULL, " ")); //Trae el valor de m y lo pasa a int
            tamano = tamano * 1024;
            //printf("%s: Memoria = %d \n", argv[i], tamano);
            i++;
        }
    }

    int8_t memoria[tamano];  //Se crea el array de memoria -----------------------------


    //Se fija si hay un -d y si está entonces se ejecuta el Dissasembler.
    if(i<argc && strcmp(argv[i], "-d")==0 && vmx){
        //printf("%s: disasembler activado\n", argv[i]);
        Dissasembler(memoria, registros, tabla_seg);
        i++;
    }

    //Si hay un archivo .vmx tenemos que fijarnos si hay parámetros...
    if(vmx){

        //Chequeamos si hay parámetros...
        if(i<argc && strcmp(argv[i], "-p")==0){
            //printf("SI HAY PARAMETROS\n");
            LeerParametros(memoria, registros, tabla_seg, argc, argv, i); //Leemos los parámetros y los guardamos en memoria
        }
        else {
            //printf("NO HAY PARAMETROS\n");
            registros[PS] = -1; //No hay parámetros, ponemos -1 en el Registro del Param Segment
        }

        IniciarMaquinaVirtual(registros, memoria, tabla_seg, tamano, argv[vmx]);

        if(vmi){
            ejecutarPrograma(memoria, registros, tabla_seg, argv[vmi]); //Nos pasamos el nombre del archivo vmi para poder actualizarlo en caso de encontrar BREAKPOINTS
        }else{
            ejecutarPrograma(memoria, registros, tabla_seg, ""); //No tiene vmi (ignoramos los BREAKPOINTS)
        }

    }else{
        //Leemos la imagen vmi e iniciamos la ejecución (desde el punto que dejó el vmi al registro[IP])
        leerImagenVmi(memoria, registros, tabla_seg, argv[vmi], &tamano);
        ejecutarPrograma(memoria, registros, tabla_seg, argv[vmi]);
    }
}

void LeerParametros(int8_t memoria[], int32_t registros[], Segmento tabla_seg[] , int argc, char * argv[], int i){

    int j;
    int16_t base;
    int32_t basesParametros[50];
    int cantParametros;

    tabla_seg[0].tamanio = 0;
    tabla_seg[0].base=0;
    strcpy(tabla_seg[0].cod, "PS");
    registros[PS] = tabla_seg[0].base; //0x000000

     //----------- LECTURA DE PARAMETROS ------------------------------
            printf("%s, P: \n", argv[i]);
            i++; //Salteamos -p

            base=0;
            cantParametros=0;

            for(;i<argc;i++){
                //printf("%d: %s \n", i, argv[i]); // print para mostrar cada parametro
                basesParametros[cantParametros++]=base; //Guardamos punteros a cada palabra
                for(j=0; j<=strlen(argv[i]); j++){ // usamos j<=strlen para asi se guarda el '\0'. //Vamos caracter a caracter guardando en memoria
                    tabla_seg[0].tamanio++;  //Se va aumentando el tamaño del Param Segment a medida que se agregan caracteres
                    //printf("%c", argv[i][j]); //print para ver cada letra que se va guardando
                    GuardarEnMemoria(memoria, registros, tabla_seg, base+j, argv[i][j], 1, "PS"); //Guardamos caracter por caracter (1 byte cada uno)
                }
                base+=j;
            }

            //El valor de base en este punto es donde terminan los parametros guardados en memoria
            for(int i=0; i<cantParametros; i++){
                tabla_seg[0].tamanio+=4;
                GuardarEnMemoria(memoria, registros, tabla_seg,  base, basesParametros[i], 4, "PS");
                base+=4;
            }

            //printf("tamanio: %d", tabla_seg[0].tamanio); //print para ver el tamaño final del Param Segment.

    //------------------------------------------------------
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

void mostrarMemoria(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int desplazamiento)
{
    printBits32(LeerMemoria(memoria, registros, tabla_seg ,tabla_seg[1].base + desplazamiento * TAMANIOMEM, 4, "DS"));
}

//-------- PRINTS DE BITS ----------------------

void printBits(uint8_t valor)
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
    int32_t puntero;
    if(codSegmento<0 || codSegmento>=NUM_SEG){
        printf("ERROR: Fallo de segmento");
        exit(1);
    }else{
        puntero = codSegmento;
        puntero = puntero << 16;
        puntero = puntero | desplazamiento;
    }

    return puntero;
}

void LeerPuntero(Segmento tabla_seg[], int32_t puntero, int16_t *codSegmento, int16_t *desplazamiento)
{

    *codSegmento = (puntero & 0xFFFF0000) >> 16;
    *desplazamiento = (puntero & 0xFFFF);

    if((*codSegmento<0) || (*codSegmento>=NUM_SEG) || (tabla_seg[*codSegmento].base+*desplazamiento*TAMANIOMEM<tabla_seg[*codSegmento].base) || (tabla_seg[*codSegmento].base+*desplazamiento*TAMANIOMEM>tabla_seg[*codSegmento].base + tabla_seg[*codSegmento].tamanio)){
        printf("ERROR: Fallo de segmento");
    }
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
    int cant;
    uint16_t dir_fis;
    uint32_t resultado = 0;

    switch (tipo)
    {
    case TIPO_REGISTRO:
        resultado = registros[valor & 0x0000001F]; // la mascara es para solamente obtener los ultimos 5 bits

        switch (getBit(valor, 7) + getBit(valor, 6)) // aplicamos mascara al resultado para simular que leimos solamente el sector de registro que se pedia
        {
            case 1:
                resultado = resultado & 0x000000FF;
                break;
            case 2:
                resultado = resultado & 0x0000FF00;
                break;
            case 3:
                resultado = resultado & 0x0000FFFF;
                break;
            default:
                resultado = resultado & 0xFFFFFFFF;
        }

        break;
    case TIPO_INMEDIATO:
        resultado = ExtenderSigno16Bits(valor);
        break;
    case TIPO_MEMORIA:
        dir_fis = ProcesarOPMemoria(valor, registros, tabla_seg); // la mascara a valor se aplica en la funcion
        cant = 4 - (getBit(valor, 23) * 2 + getBit(valor, 22));
        resultado = LeerMemoria(memoria, registros, tabla_seg , dir_fis, cant, "DS");
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
    int8_t codRegistro = (valor & 0x001F0000) >> 16;
    int16_t desplazamientoOperando = (valor & 0x0000FFFF);
    int16_t codSegmento;
    int16_t desplazamientoPuntero;
    int32_t puntero = registros[codRegistro];

    LeerPuntero(tabla_seg, puntero, &codSegmento, &desplazamientoPuntero);

    if(codSegmento<0 || codSegmento>=NUM_SEG){
            printf("ERROR: Fallo de segmento");
            exit(1);
    }

    int32_t posicion = tabla_seg[codSegmento].base + desplazamientoPuntero + desplazamientoOperando;

    if (posicion < 0 || posicion > tabla_seg[codSegmento].base + tabla_seg[codSegmento].tamanio || posicion < tabla_seg[codSegmento].base)
    {
        printf("ERROR: Fallo de segmento");
        exit(1);
    }

    registros[LAR] = CrearPuntero(codSegmento, desplazamientoOperando + desplazamientoPuntero); // Se Inicializa el registro LAR

    //REVISAR LA PARTE ALTA DEL MAR
    registros[MAR] = TAMANIOMEM; // Tamanio estándar de la memoria (por ahora)

    registros[MAR] = registros[MAR] << 16; // Se guarda la cantidad de bytes de lectura en la parte alta del MAR

    registros[MAR] = registros[MAR] | posicion; // Se guarda la posicion en la parte baja del registro MAR

    return posicion;
}

int32_t LeerMemoria(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int32_t base, int cant, char codSegmento[]) //cant debe ser menor o igual a 4
{ // Se "para" en la posicion de memoria "base" y lee/concatena los 'cant' bytes siguientes
    int32_t aux = 0;
    int i;

    int codSeg = obtenerCodSegmento(tabla_seg, codSegmento);

    if (base >= 0 && (base >= tabla_seg[codSeg].base) && (base + cant) <= tabla_seg[codSeg].base + tabla_seg[codSeg].tamanio)
    {
        for (i = 0; i < cant; i++)
        {
            aux = aux << 8;
            aux = aux | (uint8_t)memoria[base + i];
        }

        registros[MBR] = aux;
    }
    else
    {
        printf("ERROR: Lectura de memoria");
        exit(1);
    }

    return aux;
}

void GuardarEnMemoria(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int32_t base, int32_t valor, int cant, char codSegmento[]) //cant debe ser menor o igual a 4
{ // Recibe un valor de cant bytes y lo guarda en cada byte de memoria a partir de la dirección "base"

    int codSeg = obtenerCodSegmento(tabla_seg, codSegmento);

    if (base >= 0 && (base >= tabla_seg[codSeg].base) && (base + cant) <= tabla_seg[codSeg].base + tabla_seg[codSeg].tamanio)
    {
        for (int i = 0 ; i < cant ; i ++) {
            memoria[base + i] = (valor >> (3-i)*8 & 0xFF);
        }
        /*
        memoria[base] = (valor >> 24) & 0xFF;
        memoria[base + 1] = (valor >> 16) & 0xFF;
        memoria[base + 2] = (valor >> 8) & 0xFF;
        memoria[base + 3] = (valor & 0xFF);
        esto era antes
        */
        registros[MBR] = valor;
    }
    else
    {
        printf("ERROR: Guardado en memoria");
        exit(1);
    }
}


void guardarImagenVmi(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint32_t tam_memoria, char nombre_archivo[]) {

    char id[] = "VMI25";
    uint8_t version = 1;
    uint16_t tam_Kib = (uint16_t)(tam_memoria / 1024);

    FILE *arch = fopen(nombre_archivo, "wb");

    if (arch == NULL) {
        printf("No se pudo abrir el archivo \n");
        exit(1);
    }

    fwrite(id, sizeof(uint8_t), 5, arch); // escribo el identificador
    fwrite(&version, sizeof(uint8_t), 1, arch); // escribo la version
    fwrite(&tam_Kib, sizeof(uint16_t), 1, arch); // escribo el tamano de memoria en Kib

    fwrite(registros, sizeof(int32_t), 32, arch); // escribo los registros
    fwrite(tabla_seg, sizeof(Segmento), 8, arch); // escribo la tabla de segmentos

    fwrite(memoria, sizeof(int8_t), tam_memoria, arch); // escribo la memoria

    fclose(arch);

    printf("Se guardo exitosamente el VMI \n");
}

void leerImagenVmi(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char nombre_archivo[], uint32_t *tam_mem) {

    char id[6];
    uint8_t version;
    uint16_t tam_Kib;

    FILE *arch = fopen(nombre_archivo, "rb");

    if (arch == NULL) {
        printf("No se pudo abrir el archivo \n");
        exit(1);
    }

    fread(id, sizeof(char), 5, arch);
    id[5] = '\0'; // agrego el caracter nulo
    if (strcmp(id, "VMI25") != 0) {
        printf("El identificador es incorrecto \n");
        exit(1);
    }

    fread(&version, sizeof(uint8_t), 1, arch);
    if (version != 1) {
        printf("Version no compatible \n");
        exit(1);
    }

    fread(&tam_Kib, sizeof(uint16_t), 1, arch);
    *tam_mem = tam_Kib * 1024;

    fread(registros, sizeof(int32_t), 32, arch);
    fread(tabla_seg, sizeof(Segmento), 8, arch);
    fread(memoria, sizeof(int8_t), *tam_mem, arch);

    fclose(arch);

    printf("Se leyo exitosamente el VMI \n");
}


int obtenerCodSegmento(Segmento tabla_seg[], char cod[]) {
    int i = 0;
    while (i < NUM_SEG && strcmp(cod, tabla_seg[i].cod) != 0)
        i++;

    if(i==NUM_SEG){
        printf("Segmento no encontrado\n");
        exit(1);
    }

    return i;
}

//------------------ EJECUCION ---------------------------------------

void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[], int tamano, char filevmx[])
{
    FILE *f;
    char id[6];
    int8_t ver;
    int16_t tamCS, tamDS, tamES, tamSS, tamKS, entry_point;
    int codSegmento = 0;

    f = fopen(filevmx, "rb");

    fread(id, 5, 1, f); // Leer identificador

    if (strcmp(id, "VMX25") != 0)
    {
        printf("ERROR: Mal identificador");
    }
    else
    {
        fread(&ver, sizeof(uint8_t), 1, f); // Leer Version

        if (!(ver == 1 || ver == 2))
        {
            printf("ERROR: Version erronea");
        }
        else if (ver == 1)
        {
            fread(&tamCS, 2, 1, f); // Leer Tamano

            tamCS = BigEndianLittleEndian16(tamCS); // Pasar Tamano a LittleEndian

            registros[CS] = CrearPuntero(0, 0); // Ponemos en 0 el valor de inicio del Code Segment

            registros[DS] = CrearPuntero(1, 0); // Ponemos en tamanio el valor de inicio del Data Segment

            fread(memoria, 1, tamCS, f); // Lectura del Archivo en la Memoria

            //-- Inicializar tabla de segmentos ---

            tabla_seg[0].base = 0;
            tabla_seg[0].tamanio = tamCS;

            tabla_seg[1].base = tamCS;
            tabla_seg[1].tamanio = TAMANIOMEMORIA - tamCS; //Como es la version 1 el tamaño de la memoria es una CONSTANTE

            //-------------------------------------

            registros[IP] = registros[CS]; // IP igual al registro CS (Inicialmente 0)
        }
        else { // ver == 2

            int sumatamanos = 0;

            if (tabla_seg[0].tamanio == 0) // no hay param segment;
                codSegmento = 0;
            else{
                sumatamanos = tabla_seg[0].tamanio;
                codSegmento = 1;
            }

            //----------------------- Inicializacion del Const Segment -------------------------

            fread(&tamKS, 2, 1, f); // Const Segment
            tamKS = BigEndianLittleEndian16(tamKS);
            if (sumatamanos + tamKS >= tamano) {
                printf("El KS se excedio el tamano de la memoria\n");
                exit(1);
            }
            else
                sumatamanos += tamKS;
            if (tamKS > 0) {
                tabla_seg[codSegmento].base = tabla_seg[codSegmento - 1].base + tabla_seg[codSegmento - 1].tamanio;
                tabla_seg[codSegmento].tamanio = tamKS;
                strcpy(tabla_seg[codSegmento].cod, "KS");

                registros[KS] = CrearPuntero(codSegmento, 0);
                codSegmento++;
            }
            else
                registros[KS] = -1;

            //--------------------------------------------------------------------------

            //------------- Inicializacion Code Segment ----------------------

            fread(&tamCS, 2, 1, f); //Code Segment
            tamCS = BigEndianLittleEndian16(tamCS);
            if (sumatamanos + tamCS >= tamano) {
                printf("El CS se excedio el tamano de la memoria\n");
                exit(1);
            }
            else
                sumatamanos += tamCS;

            //No se pregunta si el tamaño es mayor porque existe si o si.

            tabla_seg[codSegmento].base = tabla_seg[codSegmento - 1].base + tabla_seg[codSegmento - 1].tamanio;
            tabla_seg[codSegmento].tamanio = tamCS;
            strcpy(tabla_seg[codSegmento].cod, "CS");

            // aca hay que poner la base del segmento o un puntero a la tabla de segmento?. Hay que poner un puntero.

            registros[CS] = CrearPuntero(codSegmento, 0);
            codSegmento++;

            //---------------------------------------------------------------

            //-------------- Inicializacion del Data Segment -------------------

            fread(&tamDS, 2, 1, f); // Data Segment
            tamDS = BigEndianLittleEndian16(tamDS);
            if (sumatamanos + tamDS >= tamano) {
                printf("El DS se excedio el tamano de la memoria\n");
                exit(1);
            }
            else
                sumatamanos += tamDS;

            if (tamDS > 0){
                tabla_seg[codSegmento].base = tabla_seg[codSegmento - 1].base + tabla_seg[codSegmento - 1].tamanio;
                tabla_seg[codSegmento].tamanio = tamDS;
                strcpy(tabla_seg[codSegmento].cod, "DS");

                registros[DS] = CrearPuntero(codSegmento, 0);
                codSegmento++;
            }
            else
                registros[DS] = -1;

            //-------------------------------------------------------------------

            //--------------- Inicializacion del Extra Segment -------------------

            fread(&tamES, 2, 1, f); // extra segment
            tamES = BigEndianLittleEndian16(tamES);

            if (sumatamanos + tamES >= tamano) {
                printf("El ES se excedio el tamano de la memoria\n");
                exit(1);
            }
            else
                sumatamanos += tamES;
            if (tamES > 0) {
                tabla_seg[codSegmento].base = tabla_seg[codSegmento - 1].base + tabla_seg[codSegmento - 1].tamanio;
                tabla_seg[codSegmento].tamanio = tamES;
                strcpy(tabla_seg[codSegmento].cod, "ES");

                registros[ES] = CrearPuntero(codSegmento, 0);
                codSegmento++;
            }
            else
                registros[ES] = -1;

            //-------------------------------------------------------------------

            //--------------- Inicializacion del Stack Segment -------------------

            fread(&tamSS, 2, 1, f); // Stack Segment
            tamSS = BigEndianLittleEndian16(tamSS);

            if (sumatamanos + tamSS >= tamano) {
                printf("El SS se excedio el tamano de la memoria\n");
                exit(1);
            }
            else
                sumatamanos += tamSS;

            //No se chequea que tengo tamaño > 0 porque siempre está presente. (Al igual que el code segment)

                tabla_seg[codSegmento].base = tabla_seg[codSegmento - 1].base + tabla_seg[codSegmento - 1].tamanio;
                tabla_seg[codSegmento].tamanio = tamSS;
                strcpy(tabla_seg[codSegmento].cod, "SS");

                registros[SP] =  CrearPuntero(tabla_seg[codSegmento].base + tabla_seg[codSegmento].tamanio, 0);

                registros[SS] = CrearPuntero(codSegmento, 0);
                codSegmento++;

            //------------------------------------------------------------------

            //-------------------- Relleno de Segmentos faltantes (para no tener que pasar la cantidad) -------------------

                while(codSegmento<NUM_SEG){
                    strcpy(tabla_seg[codSegmento].cod,"");
                    codSegmento++;
                }

            //-------------------------------------------------------------------------------------------------------------

            //--------------------- Lectura del Entry Point ----------------------------

            fread(&entry_point, 2, 1, f);
            entry_point = BigEndianLittleEndian16(entry_point);
            registros[IP] = registros[CS] + entry_point;
            // registros[ip] = comienzo del code segment + entry point

            //-------------------------------------------------------------------------


            //------------------- Lectura del código ------------------------

            int codCS = obtenerCodSegmento(tabla_seg, "CS");

            fread(&(memoria[tabla_seg[codCS].base]), sizeof(int8_t), tamCS, f); //A partir de la base de CS se guarda todo el código

            //---------------------------------------------------------------

            //------------------- Lectura de las constantes ------------------

            if (tamKS > 0) {

                int codKS = obtenerCodSegmento(tabla_seg, "KS");

                fread(&(memoria[tabla_seg[codKS].base]), sizeof(int8_t), tamKS, f); //A partir de la base de KS se guardan todas las constantes.
            }

            //----------------------------------------------------------------

            //Quizás faltaría poner los parámetros en el Stack?

            printf("Se leyo correctamente\n");
        }
    }
    fclose(f);
}

void ejecutarPrograma(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char filevmi[])
{
    while (registros[IP] < tabla_seg[0].tamanio && registros[IP] != 0xFFFFFFFF)
    {
        leerInstruccion(memoria, registros, tabla_seg, filevmi); // manda a ejecutar la siguiente instruccion mientras este IP este dentro del code segment y IP tenga valor valido
    }
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

void leerInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char filevmi[]) // Lee la siguiente instruccion separando el código de instrucción de sus operadores
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

    ejecutarInstruccion(memoria, registros, tabla_seg, filevmi);
}

void ejecutarInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], char filevmi[]) // Busca y ejecuta la función respectiva al código de la función
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
        default:
            printf("ERROR: Instruccion Invalida\n");
            exit(1);
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
        case 0x0B: // PUSH
            push(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x0C: // POP
            pop(memoria, registros, tabla_seg, tipo2, valor2);
            break;
        case 0x0F: // STOP
            registros[IP] = 0xFFFFFFFF;
            printf("\nSTOP: Termino la ejecucion\n");
            break;
        default:
            printf("ERROR: Instruccion Invalida\n");
            exit(1);
        }
    }
}

void GuardarEnRegistro(int32_t registros[], int8_t reg, int32_t valorGuardar) {
    int sector;

    sector = getBit(reg, 7) * 2 + getBit(reg, 6);
    switch (sector)
    {
        case 1:
            valorGuardar = valorGuardar & 0xFF;
            break;
        case 2:
            valorGuardar = valorGuardar & 0xFF;
            valorGuardar = valorGuardar << 8;
            break;
        case 3:
            valorGuardar = valorGuardar & 0xFFFF;
            break;
    }
    registros[reg & 0x1F] = registros[reg & 0x1F] & valorGuardar;
}

void push(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int8_t tipo, int32_t valor)
{
    int codSegmentoSS = obtenerCodSegmento(tabla_seg, "SS");
    registros[SP] -= 4;
    if (registros[SP] < tabla_seg[codSegmentoSS].base) {
        printf("Stack Overflow, SP se paso de la base del SS\n");
        exit(1);
    }
    else {
        valor = obtenerValorOperando(valor, tipo, registros, memoria, tabla_seg);
        GuardarEnMemoria(memoria, registros, tabla_seg, registros[SP], valor, 4, "SS");
    }
}

void pop(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int8_t tipo, int32_t valor)
{
    int i, sector, cant, codSegmentoSS = obtenerCodSegmento(tabla_seg, "SS");
    int32_t aux = 0;
    char seg[3];

    if (registros[SP] + 4 > tabla_seg[codSegmentoSS].base + tabla_seg[codSegmentoSS].tamanio) {
        printf("Stack Underflow, no se pudo completar POP \n");
        exit(1);
    }
    else {
        for (i = 0 ; i < 4 ; i++)
        {
            aux |= memoria[registros[SP+i]];
            aux = aux << (3 - i) * 8;
        }
        if (tipo == TIPO_REGISTRO)
            GuardarEnRegistro(registros, (valor & 0xFF), aux);
        else if (tipo == TIPO_MEMORIA) {
            copiarRegistro(valor & 0x001F0000, seg);
            cant = 4 - (getBit(valor, 23) * 2 + getBit(valor, 22));
            GuardarEnMemoria(memoria, registros, tabla_seg, valor, aux, cant, seg);
        }
        else {
            printf("ERROR: se quiso hacer pop a un inmediato o ninguno\n");
            exit(1);
        }
        registros[SP] += 4;
    }
}

//---------------------------------------------------------------------------------------

void mov(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
        GuardarEnRegistro(registros, (valor1 & 0xFF), valor2);
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        // revisar la longitud de valor, si es Long, word o byte
        GuardarEnMemoria(memoria, registros,tabla_seg, dir_fis, valor2, 4, "DS");
    }
    else{
        printf("Error MOV Inmediato o Ninguno a Cualquiera \n");
        exit(1);
    }

}

void add(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    char codSeg[4];
    int cant;
    int16_t dir_fis;
    int32_t aux, resultado = 0;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) + valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) + valor2;
        cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
         printf("Error ADD Inmediato o Ninguno a Cualquiera \n");
         exit(1);
    }

    cambiarCC(registros, resultado);
}

void sub(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    char codSeg[4];
    int16_t dir_fis;
    int32_t resultado = 0;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) - valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) - valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error SUB Inmediato o ninguno -= cualquiera \n");
        exit(1);
    }
    cambiarCC(registros, resultado);
}

void mul(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    char codSeg[4];
    int16_t dir_fis;
    int32_t resultado = 0;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) * valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) * valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error MUL Inmediato o ninguno *= cualquiera \n");
        exit(1);
    }


    cambiarCC(registros, resultado);
}

void dividir(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado, aux;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (valor2 == 0)
    {
        printf("ERROR: Division por cero");
        exit(1);
    }
    else
    {
        if (tipo_op1 == TIPO_REGISTRO)
        {
            aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
            resultado = aux / valor2;
            GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
            registros[AC] = aux % valor2;
        }
        else if (tipo_op1 == TIPO_MEMORIA)
        {
            aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
            resultado = aux / valor2;
            registros[AC] = aux % valor2;
            dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
            int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
            copiarRegistro((valor1 & 0x00FF0000), codSeg);
            GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
        }
        else{
            printf("Error (Inmediato o ninugno) / cualquiera \n");
            exit(1);
        }
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
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) << valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        int32_t aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = aux << valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error: (Ninguno o Inmediato) << Cualquiera \n");
        exit(1);
    }

    cambiarCC(registros, resultado);
}

void shr(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1) // Shift-right lógico
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = (uint32_t)obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) >> valor2;
        GuardarEnRegistro(registros, (valor & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        uint32_t aux = (uint32_t)obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = aux >> valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error: (Ninguno o Inmediato) >> Cualquiera \n");
        exit(1);
    }
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
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) >> valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        int32_t aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = aux >> valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error: (Ninguno o Inmediato) SAR Cualquiera \n");
        exit(1);
    }
    cambiarCC(registros, resultado);
}

void and(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) & valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        int32_t aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = aux & valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error: (Ninguno o Inmediato) & Cualquiera \n");
        exit(1);
    }
    cambiarCC(registros, resultado);
}

void or(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) | valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        int32_t aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = aux | valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error: (Ninguno o Inmediato) | Cualquiera \n");
        exit(1);
    }

    cambiarCC(registros, resultado);
}

void xor(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    int16_t dir_fis;
    int32_t resultado;

    valor2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);

    if (tipo_op1 == TIPO_REGISTRO)
    {
        resultado = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg) ^ valor2;
        GuardarEnRegistro(registros, (valor1 & 0x1F), resultado);
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis = ProcesarOPMemoria(valor1, registros, tabla_seg);
        int32_t aux = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        resultado = aux ^ valor2;
        int cant = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x00FF0000), codSeg);
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, cant, codSeg);
    }
    else{
        printf("Error inmediato o ninguno xor cualquiera \n");
        exit(1);
    }
    cambiarCC(registros, resultado);
}

void swap(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], uint8_t tipo_op2, uint8_t tipo_op1, int32_t valor2, int32_t valor1)
{
    char codSeg1[4], codSeg2[4];
    int16_t dir_fis1, dir_fis2;
    int32_t aux1, aux2;

    if ((tipo_op1 == TIPO_REGISTRO || tipo_op1 == TIPO_MEMORIA) && (tipo_op2 == TIPO_REGISTRO || tipo_op2 == TIPO_MEMORIA)){
        aux1 = obtenerValorOperando(valor1, tipo_op1, registros, memoria, tabla_seg);
        aux2 = obtenerValorOperando(valor2, tipo_op2, registros, memoria, tabla_seg);
    }
    else {
        printf("Error SWAP \n");
        exit(1);
    }

    if (tipo_op1 == TIPO_REGISTRO)
    {
        if (tipo_op2 == TIPO_REGISTRO) {
            GuardarEnRegistro(registros, (valor1 & 0x1F), aux2);
            GuardarEnRegistro(registros, (valor2 & 0x1F), aux1);
        }
        else if (tipo_op2 == TIPO_MEMORIA) {
            GuardarEnRegistro(registros, (valor1 & 0x1F), aux2);
            dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
            int cant = 4 - (getBit(valor2, 23) * 2 + getBit(valor2, 22));
            copiarRegistro((valor2 & 0x00FF0000), codSeg2);
            GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis2, aux1, cant, codSeg2);
        }
    }
    else if (tipo_op1 == TIPO_MEMORIA)
    {
        dir_fis1 = ProcesarOPMemoria(valor1, registros, tabla_seg);
        int cant1 = 4 - (getBit(valor1, 23) * 2 + getBit(valor1, 22));
        copiarRegistro((valor1 & 0x1F), codSeg1);

        if (tipo_op2 == TIPO_REGISTRO) {
            GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis1, aux2, cant1, codSeg1);
            GuardarEnRegistro(registros, (valor2 & 0x1F), aux1);
        }
        else if (tipo_op2 == TIPO_MEMORIA) {
            dir_fis2 = ProcesarOPMemoria(valor2, registros, tabla_seg);
            int cant2 = 4 - (getBit(valor2, 23) * 2 + getBit(valor2, 22));
            copiarRegistro((valor2 & 0x00FF0000), codSeg2);
            GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis1, aux2, cant1, codSeg1)
            GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis2, aux1, cant2, codSeg2);
        }
    }
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
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, 4, "DS");
    }
    else{
        printf("Error (Inmediato o Ninguno) LDL Cualquiera \n");
        exit(1);
    }

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
        // revisar la longitud de valor, si es Long, word o byte. 4 es la cant de celdas a guardar
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, 4, "DS");
    }
    else{
        printf("Error (Inmediato o Ninguno) LDH Cualquiera \n");
        exit(1);
    }

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
        GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, rand() % valor2, 4, "DS");
    }
    else{
        printf("Error RND Inmediato o Ninguno a Cualquiera \n");
        exit(1);
    }

}

void read(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[], Segmento tabla_seg[])
{
    int i;
    int32_t valor, aux;
    char bin[33], car;
    if (modo <= 0 || !(modo == 0x01 || modo == 0x02 || modo == 0x04 || modo == 0x08 || modo == 0x10)){
        printf("Error, modo de lectura invalido \n");
        exit(1);
    }else {
        for (i = 0; i < cantidad; i++)
        {
            //fflush(stdin);
            printf("[%04X]: ", dir_fis + i * tamano);
            switch (modo)
            {
                // revisar la longitud de valor, si es Long, word o byte. 4 es la cant de celdas a guardar
            case 0x01: // interpreta decimal
                scanf("%d", &valor);
                GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis + i * tamano, valor, 4, "DS");
                break;
            case 0x02: // interpreta caracteres
                scanf("%c", &car);
                scanf("%c", &aux); // si no esta esto, lee el "enter" como otro caracter
                GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis + i * tamano, car, 4, "DS");
                break;
            case 0x04: // interpreta octal
                scanf("%o", &valor);
                GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis + i * tamano, valor, 4, "DS");
                break;
            case 0x08: // interpreta hexadecimal
                scanf("%X", &valor);
                GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis + i * tamano, valor, 4, "DS");
                break;
            case 0x10: // interpreta binario
                scanf("%32s", bin);
                valor = (int32_t)strtol(bin, NULL, 2); // convierte una serie de caracteres en enteros
                GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis + i * tamano, valor, 4, "DS");
                break;
            default:
                printf("Modo para leer no valido \n");
                exit(1);
            }
            printf("\n");
        }
    }
}

void write(int16_t dir_fis, int16_t cantidad, int16_t tamano, int32_t modo, int8_t memoria[], int32_t registros[], Segmento tabla_seg[])
{
    int i;
    int32_t valor;
    if (modo <= 0 || modo > 0x1F){
            printf("Error, modo de escritura invalido \n");
            exit(1);
    }else {
        for (i = 0; i < cantidad; i++)
        {
        valor = LeerMemoria(memoria, registros, tabla_seg, dir_fis + i * tamano, 4, "DS");

        printf("[%04X]: ", dir_fis + i * tamano);
        if (getBit(modo, 0) == 1)
            printf("0d %d ", valor);
        if (getBit(modo, 1) == 1)
            if (valor >= 32 && valor <= 126)
                printf("Aa '%c' ", valor);
            else
                printf("Aa '.' ");
        if (getBit(modo, 2) == 1)
            printf("0o %o ", valor);
        if (getBit(modo, 3) == 1)
            printf("0x %X ", valor);
        if (getBit(modo, 4) == 1){
            printf("0b ");
            printBits32(valor);
        }
        printf("\n");
        }
    }

}

void sys(int8_t memoria[], int32_t registros[], Segmento tabla_seg[], int32_t valor)
{
    int32_t modo, puntero;
    int16_t codSeg, desplazamiento, dir_fis, cantidad, tamano;
    modo = registros[EAX];
    puntero = registros[EDX];
    LeerPuntero(tabla_seg, puntero, &codSeg, &desplazamiento);
    dir_fis = tabla_seg[codSeg].base + desplazamiento;
    cantidad = registros[ECX] & 0xFFFF;
    tamano = (registros[ECX] >> 16) & 0xFFFF;
    switch (valor)
    {
    case 0x1: // Lectura
        read(dir_fis, cantidad, tamano, modo, memoria, registros, tabla_seg);
        break;
    case 0x2: // Escritura
        write(dir_fis, cantidad,tamano, modo, memoria, registros, tabla_seg);
        break;
    default:
        printf("Modo para el SYS no valido \n");
        exit(1);
    }
}

void Jump(int32_t registros[], Segmento tabla_seg[], uint32_t valor)
{

    if (valor >= 0 && valor < tabla_seg[0].tamanio)
        registros[IP] = valor;
    else{
        printf("ERROR: Se intento acceder afuera del segmento de codigo\n");
        exit(1);
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
            // revisar la cantidad de bytes a guardar en memoria
            GuardarEnMemoria(memoria, registros, tabla_seg, dir_fis, resultado, 4, "DS");
        }
        cambiarCC(registros, resultado);
    }
    else{
        printf("Error: NOT aplicado a un inmediato o ninguno \n");
        exit(1);
    }

}

void Dissasembler(int8_t memoria[], int32_t registros[], Segmento tabla_seg[])

// aca hay que buscar el Code Segment en la tabla de segmentos y recorres el ip ahi,
//ahora esta mal porque esta hecho con los segmentos arbitrarios
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
    int32_t op1,op2, opc;
    char mnem[5];

    printf("[%04X] ", *ip);

    printf("%02X ", byte_de_control);

    (*ip)+=1;

    tipo_opB = (byte_de_control >> 6) & 0x03;

    tipo_opA = (byte_de_control >> 4) & 0x03;

    opc = byte_de_control & 0x1F;

    op2 = LeerOperando(memoria, ip, tipo_opB, 1);

    if (tipo_opA != 0)
        op1 = LeerOperando(memoria, ip, tipo_opA, 1);

    printf("\t|   ");

    obtenerMnemonico(opc, mnem);

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
    case TIPO_INMEDIATO:
            op = ExtenderSigno16Bits(op);
            printf("%d", op);
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
        case SP:
            strcpy(registro, "SP");
            break;
        case BP:
            strcpy(registro, "BP");
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
        case ES:
            strcpy(registro, "ES");
            break;
        case SS:
            strcpy(registro, "SS");
            break;
        case KS:
            strcpy(registro, "KS");
            break;
        case PS:
            strcpy(registro, "PS");
            break;
        default:
            strcpy(registro, "");
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
