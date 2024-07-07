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

// Печать входных данных для ГОСТ 52644 и ГОСТ 32484.3
void print_input_data(int *arr, int flag)
{
    STR_LINE;
    printf("\t\t\t\t*** ENTERED DATA ***\n");
    printf("%s%12s%12s%15s%12s%12s%12s\n", "BoltDiam", "BoltLength", "ThickParts",
           "ThickPartNut", "WasherHead", "WasherNut", "NumberNuts");
    printf("%8d%12d%12d%15d%12d%12d%12d\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6]);
    if (flag == 1)
    {
        if (arr[1] == bolt_length_52644[11] || arr[1] == bolt_length_52644[13] ||
            arr[1] == bolt_length_52644[15] || arr[1] == bolt_length_52644[17])
            printf("\t\t!!! This bolt length is not recommended !!!\n");
    }
    STR_LINE;
}

// Проверка 1: резьба в детали (превышение 0.5t крайней к гайке детали)
// Проверка 2: резьба в шайбе (возможность закрутить гайку)
int bolt_check_thread_52644(bolt info[], int number, int arr[])
{
    double thread_result;
    int fact_thread_length;
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == arr[0])
        {
            if (arr[1] > 150)
                fact_thread_length = info[i].thread_length_add1;
            else
                fact_thread_length = info[i].thread_length;

            thread_result = arr[4] * info[i].washer_thickness + arr[2] + arr[3] - arr[1] + fact_thread_length;
            printf("\t\t*** GOST DATA (52644-2006, 52645-2006, 52646-2006) ***\n");
            printf("%s%12s%12s%14s%10s\n", "WashThick", "NutHeight", "ThreadLen", "ThreadPitch", "Chamfer");
            printf("%8.1f%12.1f%12d%14.1f%10.1f\n", info[i].washer_thickness, info[i].nut_height,
                   fact_thread_length, info[i].thread_pitch, info[i].chamfer);
            STR_LINE;
            printf("\t\t\t*** THREAD POSITION ***\n");
            if (thread_result > 0) // резьба в крайней детали
            {
                printf("Thread in detail %.1f ", fabs(thread_result));
                if (thread_result > 0.5 * arr[3])
                {
                    printf("!!! The thread goes into the part !!!");
                    return 1;
                }
            } else if (thread_result < 0) // резьба в шайбе
            {
                printf("Thread in washer %.1f ", fabs(thread_result));
                if (fabs(thread_result) > arr[5] * info[i].washer_thickness)
                {
                    printf("!!! Do not tighten the nut !!!");
                    return 2;
                }
            } else if (thread_result == 0) // резьба на границе деталей
            {
                printf("Thread at the interface between the part and the washer");
            }
        }
    }
    return 0;
}