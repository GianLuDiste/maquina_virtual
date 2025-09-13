#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TAMANIOMEMORIA 16384
#define TAMANIOREGISTROS 32

#define TAMANIOREG 4
#define IP 3
#define CS 26
#define DS 27
#define NUM_SEG 8

typedef struct {
    int16_t base;   // Dirección base del segmento
    int16_t tamanio;   // Tamaño del segmento
} Segmento;

int32_t BigIndianLittleIndian32(uint32_t valor);
int16_t BigIndianLittleIndian16(uint16_t valor);
void IniciarMaquinaVirtual(int8_t registros[], int8_t memoria[], Segmento tabla_seg[]);

int main()
{
    int32_t registros[TAMANIOREGISTROS];

    int8_t memoria[TAMANIOMEMORIA];

    Segmento tabla_seg[NUM_SEG];

    IniciarMaquinaVirtual(registros, memoria, tabla_seg);

    return 0;
}

uint16_t BigEndianLittleEndian16(int16_t valor){
    valor=(valor>>8)|(valor<<8);
    return valor;
}

uint32_t BigEndianLittleEndian32(int32_t valor){
    valor = ((valor>>24)&0xff) | // Mover byte 3 a byte 0
                    ((valor<<8)&0xff0000) | // Mover byte 1 a byte 2
                    ((valor>>8)&0xff00) | // Mover byte 2 a byte 1
                    ((valor<<24)&0xff000000); // Mover byte 0 a byte 3
    return valor;
}

void IniciarMaquinaVirtual(int8_t registros[], int8_t memoria[], Segmento tabla_seg[]){

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

