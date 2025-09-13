#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<string.h>

#define TAMANIOMEMORIA 16384
#define TAMANIOREGISTROS 32

#define TAMANIOREG 4
#define LAR 0
#define MAR 1
#define MBR 2
#define IP  3
#define OPC 4
#define OP1 5
#define OP2 6
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15
#define AC  16
#define CC  17
#define CS  26
#define DS  27
#define NUM_SEG 8


typedef struct {
    int16_t base;   // Dirección base del segmento
    int16_t tamanio;   // Tamaño del segmento
} Segmento;

uint32_t BigEndianLittleEndian32(uint32_t valor);
uint16_t BigEndianLittleEndian16(uint16_t valor);
void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[]);

void ejecutarPrograma(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);
void ejecutarInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]);

int main()
{
    int32_t registros[TAMANIOREGISTROS];

    int8_t memoria[TAMANIOMEMORIA];

    Segmento tabla_seg[NUM_SEG];

    IniciarMaquinaVirtual(registros, memoria, tabla_seg);

    ejecutarPrograma(memoria, registros, tabla_seg);

    return 0;
}

uint16_t BigEndianLittleEndian16(uint16_t valor){
    valor=(valor>>8)|(valor<<8);
    return valor;
}

uint32_t BigEndianLittleEndian32(uint32_t valor){
    valor = ((valor>>24)&0xff) | // Mover byte 3 a byte 0
                    ((valor<<8)&0xff0000) | // Mover byte 1 a byte 2
                    ((valor>>8)&0xff00) | // Mover byte 2 a byte 1
                    ((valor<<24)&0xff000000); // Mover byte 0 a byte 3
    return valor;
}

void IniciarMaquinaVirtual(int32_t registros[], int8_t memoria[], Segmento tabla_seg[]){ // cambie en registro int8_t por int32_t

    FILE * f;

    int8_t id[6];

    int8_t ver;

    int16_t tamanio;

    f=fopen("sample.vmx", "rb");

    fread(id, 5, 1, f); //Leer identificador

    if(strcmp(id, "VMX25")!=0){
        perror("ERROR: Mal identificador");
    }else{
        fread(&ver, sizeof(uint8_t), 1, f); //Leer Version

        if(ver!=1){
            perror("ERROR: Version erronea");
        }else{
            fread(&tamanio, 2, 1, f); //Leer Tamaño

            tamanio=BigEndianLittleEndian16(tamanio); //Pasar Tamaño a LittleEndian

            //Cada vez que vayamos a usar un valor de un registro hay que pasarlo a LittleEndian

            registros[CS]=0; //Ponemos en 0 el valor de inicio del Code Segment

            registros[DS]=tamanio; //Ponemos en tamanio el valor de inicio del Data Segment

            //Hay que hacerlo así porque la computadora funciona en Little-Indian, por lo tanto lee los binarios al revés.

            fread(memoria, 1, tamanio, f); //Lectura del Archivo en la Memoria

            //-- Inicializar tabla de segmentos ---

            tabla_seg[0].base=0;
            tabla_seg[0].tamanio=tamanio;

            tabla_seg[1].base=tamanio;
            tabla_seg[1].tamanio=TAMANIOMEMORIA-tamanio;

            //-------------------------------------

            registros[IP]=registros[CS]; //IP igual al registro CS (Inicialmente 0)
        }
    }

    fclose(f);
}




int32_t LeerOperando(int8_t memoria[], uint32_t *ip, uint8_t tipoOP){

    int32_t valor=0;
    int i=0;

    for(int i=0; i<tipoOP; i++){
        valor=valor<<8;
        valor=valor | memoria[*ip];
        *ip=*ip+1;
    }

    return valor;
}

void ejecutarInstruccion(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]) {
    uint32_t ip = registros[IP];
    uint8_t byte_de_control = memoria[ip++];
    uint8_t tipo_opB, tipo_opA;
    uint32_t op;

    registros[OP1] = 0;
    registros[OP2] = 0;

    tipo_opB = (byte_de_control >> 6) & 0x03;

    registros[OP2] = registros[OP2] | (uint32_t)tipo_opB << 24;

    tipo_opA = (byte_de_control >> 4) & 0x03;

    registros[OP1] = registros[OP1] | (uint32_t)tipo_opA << 24;

    registros[OPC] = byte_de_control & 0x1F;

    registros[OP2] = registros[OP2] | LeerOperando(memoria, &ip, tipo_opB);

    registros[OP1] = registros[OP1] | LeerOperando(memoria, &ip, tipo_opA);

    registros[IP] = ip;



}

void ejecutarPrograma(int8_t memoria[], int32_t registros[], Segmento tabla_seg[]) {
    while (registros[IP] < tabla_seg[0].tamanio && registros[IP] != 0xFFFFFFFF) {
        ejecutarInstruccion(memoria, registros, tabla_seg); // manda a ejecutar la siguiente instruccion mientras este IP este dentro del code segment y IP tenga valor valido
    }
}
