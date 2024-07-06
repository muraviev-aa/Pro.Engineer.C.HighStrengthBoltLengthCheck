#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "temp_function.h"

// Вывод информации о приложении
void print_info(void)
{
    STR_LINE;
    puts("\tConsole application \"Checking bolt length\"");
    puts("Developer Muraviev A.A.");
    puts("Free software licence.");
    puts("For help with the program, use the -h argument.");
    STR_LINE;
    system("pause");
}

// Печать меню HELP
void print_help(void)
{
    STR_LINE;
    puts("\t\t\t\tHELP");
    puts("~~~ List keys:");
    puts("-h\t\t\thelp application;");
    puts("-f: file_name.csv\tfile with dimensions;");
    puts("-m: thread diameter\tindicate thread diameter;");
    puts("-l: bolt length\t\tindicate bolt length;");
    puts("-s: thickness of parts\ttotal thickness of parts on the bolt head side;");
    puts("-t: thickness of part\tthickness of one part from the nut side.");
    puts("-w: washer/head\t\twasher under the bolt head;");
    puts("-g: washer/nut\t\twasher under the nut;");
    puts("-n: nut\t\t\tnumber of nuts;");
    STR_LINE;
}

// Открыть файл
int open_file(FILE **fptr, char *name_file)
{
    if ((*fptr = fopen(name_file, "r")) == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    return 0;
}

// Добавление записей
void add_record(bolt info[], int number, unsigned int bolt_name, double washer_thickness,
                double nut_height, unsigned int thread_length, unsigned int thread_length_add1,
                unsigned int thread_length_add2, double thread_pitch, double chamfer)
{
    info[number].bolt_name = bolt_name;
    info[number].washer_thickness = washer_thickness;
    info[number].nut_height = nut_height;
    info[number].thread_length = thread_length;
    info[number].thread_length_add1 = thread_length_add1;
    info[number].thread_length_add2 = thread_length_add2;
    info[number].thread_pitch = thread_pitch;
    info[number].chamfer = chamfer;
}

// Считываем данные из файла *.csv
int read_data_file(FILE **fptr, bolt info[])
{
    unsigned int bolt_name, thread_length, thread_length_add1, thread_length_add2;
    double washer_thickness, nut_height, thread_pitch, chamfer;
    int count = 0;
    while (fscanf(*fptr, "%d;%lf;%lf;%d;%d;%d;%lf;%lf", &bolt_name, &washer_thickness, &nut_height,
                  &thread_length, &thread_length_add1, &thread_length_add2, &thread_pitch, &chamfer) > 0)
    {
        add_record(info, count, bolt_name, washer_thickness, nut_height, thread_length, thread_length_add1,
                   thread_length_add2, thread_pitch, chamfer);
        count++;
    }
    return count;
}

// Функция печати данных из массива структур по индексу
void print(bolt *arr, int size_struct)
{
    for (int i = 0; i < size_struct; i++)
        printf("%d %.1f %.1f %d %d %d %.1f %.1f \n",
               arr[i].bolt_name,
               arr[i].washer_thickness,
               arr[i].nut_height,
               arr[i].thread_length,
               arr[i].thread_length_add1,
               arr[i].thread_length_add2,
               arr[i].thread_pitch,
               arr[i].chamfer);
}

// Печать входных данных для 52644
void print_input_data_52644(int *arr)
{
    STR_LINE;
    printf("\t\t\t*** ENTERED DATA ***\n");
    printf("%s%12s%12s%15s%12s%12s%12s\n", "BoltDiam", "BoltLength", "ThickParts",
           "ThickPartNut", "WasherHead", "WasherNut", "NumberNuts");
    printf("%8d%12d%12d%15d%12d%12d%12d\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6]);
    if (arr[1] == bolt_length_52644[11] || arr[1] == bolt_length_52644[13] ||
        arr[1] == bolt_length_52644[15] || arr[1] == bolt_length_52644[17])
        printf("\t\t!!! This bolt length is not recommended !!!\n");
    STR_LINE;
}