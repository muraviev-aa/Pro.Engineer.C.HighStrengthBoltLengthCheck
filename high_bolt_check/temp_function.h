#ifndef HIGH_BOLT_CHECK_TEMP_FUNCTION_H
#define HIGH_BOLT_CHECK_TEMP_FUNCTION_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#define STR_LINE puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")

typedef struct
{
    unsigned int bolt_name;            // диаметр болта
    double washer_thickness;           // толщина шайбы
    double nut_height;                 // высота гайки
    unsigned int thread_length;        // длина резьбы
    unsigned int thread_length_add1;   // доп. длина резьбы
    unsigned int thread_length_add2;   // доп. длина резьбы
    double thread_pitch;               // шаг резьбы
    double chamfer;                    // фаска
} bolt;

/* Массив под входные данные
 * connect_package[0] - диаметр болта
 * connect_package[1] - длина болта
 * connect_package[2] - толщина детали (деталей) под головкой болта
 * connect_package[3] - толщина детали под гайкой
 * connect_package[4] - количество шайб под головкой болта
 * connect_package[5] - количество шайб под гайкой
 * connect_package[6] - количество гаек
*/
extern int connect_package[7];

#endif //HIGH_BOLT_CHECK_TEMP_FUNCTION_H
