#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TAMANIOMEMORIA 16385
#define TAMANIOREGISTROS 32

#define TAMANIOREG 4
#define CS 26
#define DS 27

int32_t BigIndianLittleIndian32(uint32_t valor);
int16_t BigIndianLittleIndian16(uint16_t valor);
int32_t LeerMuchosBytes(int cantBytes, FILE * f);

int main()
{
    char caracter; //1 byte
    FILE * f;

    int8_t id[6];

    int8_t ver;

    int16_t tamanio;

    int8_t memoria[TAMANIOMEMORIA];

    int8_t registros[TAMANIOREGISTROS];

    int8_t num1;

    int32_t num2;

    num1=-23;

    printf("%d\n", num1);

    num2=num1;

    printf("%d\n", num2);

    f=fopen("sample.vmx", "rb");



    //IniciarMaquinaVirtual(f, id, &ver, &tamanio);

    //LeerCodigo(f, 3);

    fclose(f);

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

void IniciarMaquinaVirtual(FILE * f, int8_t id[], int8_t * ver, int16_t * tamanio, int8_t registros[], int8_t memoria[]){

    fread(id, 5, 1, f); //Leer identificador

    fread(ver, sizeof(uint8_t), 1, f); //Leer Version

    fread(tamanio, 2, 1, f); //Leer Tamaño

    Guardar16ARegistro(registros, 0, CS); //Ponemos en 0 el valor de inicio del Code Segment

    Guardar16ARegistro(registros, tamanio, DS); //Ponemos en tamanio el valor de inicio del Data Segment

    *tamanio=BigEndianLittleEndian16(*tamanio); //Pasar Tamaño a LittleEndian

    //Hay que hacerlo así porque la computadora funciona en Little-Indian, por lo tanto lee los binarios al revés.

    fread(memoria, 1, tamanio, f); //Lectura de la Memoria



    printf("Identificador: %s\n", id);

    printf("Version: %d\n", *ver);

    printf("%d", *tamanio);
}

void Guardar16ARegistro(uint8_t registros[], uint16_t dato, uint16_t posicion){

    uint8_t aux;

    posicion=posicion*TAMANIOREG;

    aux=dato;

    aux=aux>>8;

    registros[posicion+3]=dato | 0x000000ff;

    registros[posicion+2]=dato | 0x0000ff00;

    registros[posicion+1]=0 | 0x00ff0000;

    registros[posicion]=0 | 0xff000000;
}

//Cada vez que vayamos a usar un valor de un registro hay que pasarlo a LittleEndian

void ObtenerValorRegistro(uint8_t registros[], uint16_t posicion, uint8_t valor[]){

    valor=registros[registros[DS*4]+posicion];

}

void LeerCodigo(FILE * f, uint16_t tamanio, uint8_t memoria[], uint8_t registros[]){

    //fread(memoria[], tamanio, 1, f); //Leemos todas las instrucciones

}

/*
uint32_t LeerMuchosBytes(int cantBytes, FILE * f){
    uint32_t base=0;
    int aux;

    for(int i=0; i<cantBytes; i++){
        base=base << 8;
        fread(&aux, 1, 1, f);
        base=base | aux;
    }

    return base;
}
*/

