#ifndef HIGH_BOLT_CHECK_TEMP_FUNCTION_H
#define HIGH_BOLT_CHECK_TEMP_FUNCTION_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#define STR_LINE puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")

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

// Флаг ГОСТ
extern int flag_g;

// Флаг гаек
extern int flag_nut;

// Массив под длины болтов 52644
extern int bolt_length_52644[31];

// Массив под длины болтов 32484.3
extern int bolt_length_32484_3[23];

void print_input_data(int *arr, int flag);
void print_help(void);
void print_info(void);
int read_data_file(FILE **fptr, bolt info[]);
int open_file(FILE **fptr, char *name_file);
void add_record(bolt info[], int number, unsigned int bolt_name, double washer_thickness,
                double nut_height, unsigned int thread_length, unsigned int thread_length_add1,
                unsigned int thread_length_add2, double thread_pitch, double chamfer);
int bolt_check_thread(bolt info[], int number, int arr[], int flag);
int bolt_tip_check(bolt info[], int number, int *arr, int flag_n);
int high_bolt_check_thread(bolt info[], int number, int arr[], int flag);
void print_result_check(int res1_2, int res3);
void print(bolt *arr, int size_struct);
double print_data_thread_result(bolt info[], int number, int arr[], int flag, int i);
void print_high_bolt_result_check(int res1, int res3);
void bolt_diam_check(int diam, int flag);

#endif //HIGH_BOLT_CHECK_TEMP_FUNCTION_H
