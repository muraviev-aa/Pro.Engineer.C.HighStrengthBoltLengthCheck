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
// проверка корректности введеной длины болта
void print_input_data(int *arr, int flag)
{
    int count = 0;
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
        for (int i = 0; i < 31; i++)
        {
            if (arr[1] == bolt_length_52644[i])
                count++;
        }
    }
    if (flag == 2)
    {
        for (int i = 0; i < 23; i++)
        {
            if (arr[1] == bolt_length_32484_3[i])
                count++;
        }
    }
    if (count != 1)
        printf("\t\t\t!!! Incorrect bolt length entered !!!\n");
    STR_LINE;
}

// Проверка 1: резьба в детали (превышение 0.5t крайней к гайке детали)
// Проверка 2: резьба в шайбе (возможность закрутить гайку)
// вариант только для двух гаек
int bolt_check_thread(bolt info[], int number, int arr[], int flag)
{
    double thread_result;
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == arr[0])
        {
            thread_result = print_data_thread_result(info, number, arr, flag, i);
            STR_LINE;
            printf("\t\t\t\t*** THREAD POSITION ***\n");
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

// Проверка 1: под гайкой резьба имеет не менее одного витка
// вариант только для 1 гайки
int high_bolt_check_thread(bolt info[], int number, int arr[], int flag)
{
    double thread_result;
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == arr[0])
        {
            thread_result = print_data_thread_result(info, number, arr, flag, i);
            STR_LINE;
            printf("\t\t\t\t*** THREAD POSITION ***\n");
            if (thread_result > 0) // резьба в крайней детали
            {
                printf("Thread in detail %.1f ", fabs(thread_result));
                if (thread_result >= 2 * info[i].thread_pitch) // не менее двух витков с полным профилем
                    printf("More than two turns with full profile");
            } else if (thread_result < 0) // резьба в шайбе
            {
                printf("!!! Invalid thread position !!! Thread in washer %.1f ", fabs(thread_result));
                return 1;
            } else if (thread_result == 0) // резьба на границе деталей
                printf("!!! Invalid thread position !!! Thread at the interface between the part and the washer");
        }
    }
    return 0;
}

// Проверка 3: проверка длины конца болта (не менее одного полного витка резьбы + фаска)
// для одной и двух гаек
int bolt_tip_check(bolt info[], int number, int *arr, int flag_n)
{
    double bolt_tip;
    printf("\n");
    STR_LINE;
    printf("\t\t\t\t*** BOLT TIP ***\n");
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == arr[0])
        {
            bolt_tip = arr[1] - info[i].washer_thickness * arr[4] - arr[2] - arr[3] -
                       info[i].washer_thickness * arr[5] - flag_n * info[i].nut_height;
            printf("Bolt tip is %.1f ", bolt_tip);
            if (bolt_tip < info[i].thread_pitch + info[i].chamfer)
            {
                printf("!!! Short bolt tip !!!");
                return 1;
            }
        }
    }
    return 0;
}

// Печать результатов проверки (2 гайки)
void print_result_check(int res1_2, int res3)
{
    printf("\n");
    STR_LINE;
    printf("\t\t\t*** BOLT LENGTH CHECK RESULT ***\n");
    printf("\t%18s%18s%18s\n", "ThreadRequirement", "TighteningNut", "TipCheck");
    if (res1_2 == 0 && res3 == 0)
        printf("\t%18s%18s%18s\n", "YES", "YES", "YES");
    else if (res1_2 == 0 && res3 == 1)
        printf("\t%18s%18s%18s\n", "YES", "YES", "NO");
    else if (res1_2 == 1 && res3 == 0)
        printf("\t%18s%18s%18s\n", "NO", "YES", "YES");
    else if (res1_2 == 1 && res3 == 1)
        printf("\t%18s%18s%18s\n", "NO", "YES", "NO");
    else if (res1_2 == 2 && res3 == 0)
        printf("\t%18s%18s%18s\n", "YES", "NO", "YES");
    else if (res1_2 == 2 && res3 == 1)
        printf("\t%18s%18s%18s\n", "YES", "NO", "NO");
    STR_LINE;
}

// Печать результатов проверки (1 гайка)
void print_high_bolt_result_check(int res1, int res3)
{
    printf("\n");
    STR_LINE;
    printf("\t\t\t*** BOLT LENGTH CHECK RESULT ***\n");
    printf("\t\t%18s%18s\n", "ThreadRequirement", "TipCheck");
    if (res1 == 0 && res3 == 0)
        printf("\t\t%18s%18s\n", "YES", "YES");
    else if (res1 == 1 && res3 == 0)
        printf("\t\t%18s%18s\n", "NO", "YES");
    else if (res1 == 0 && res3 == 1)
        printf("\t\t%18s%18s\n", "YES", "NO");
    else if (res1 == 1 && res3 == 1)
        printf("\t\t%18s%18s\n", "NO", "NO");
    STR_LINE;
}

// Редактирование длины резьбы в зависимости от длины болта
double print_data_thread_result(bolt info[], int number, int arr[], int flag, int i)
{
    double thread_result;
    int fact_thread_length;
    if (flag == 1)
    {
        if (arr[1] > 150)
            fact_thread_length = info[i].thread_length_add1;
        else
            fact_thread_length = info[i].thread_length;

        thread_result = arr[4] * info[i].washer_thickness + arr[2] + arr[3] - arr[1] + fact_thread_length;
        printf("\t\t*** GOST DATA (52644-2006, 52645-2006, 52646-2006) ***\n");
        printf("%20s%12s%12s%14s%10s\n", "WashThick", "NutHeight", "ThreadLen", "ThreadPitch", "Chamfer");
        printf("%20.1f%12.1f%12d%14.1f%10.1f\n", info[i].washer_thickness, info[i].nut_height,
               fact_thread_length, info[i].thread_pitch, info[i].chamfer);
    }
    if (flag == 2)
    {
        if (arr[1] <= 125)
            fact_thread_length = info[i].thread_length;
        else if (arr[1] > 125 && arr[1] <= 200)
            fact_thread_length = info[i].thread_length_add1;
        else if (arr[1] > 200)
        {
            if (info[i].thread_length_add2 == 0)
                fact_thread_length = info[i].thread_length_add1;
            else
                fact_thread_length = info[i].thread_length_add2;
        }
        thread_result = arr[4] * info[i].washer_thickness + arr[2] + arr[3] - arr[1] + fact_thread_length;
        printf("\t\t*** GOST DATA (32484.3-2014, 32484.6-2014) ***\n");
        printf("%20s%12s%12s%14s%10s\n", "WashThick", "NutHeight", "ThreadLen", "ThreadPitch", "Chamfer");
        printf("%20.1f%12.1f%12d%14.1f%10.1f\n", info[i].washer_thickness, info[i].nut_height,
               fact_thread_length, info[i].thread_pitch, info[i].chamfer);
    }
    return thread_result;
}

// Проверка диаметра болта
void bolt_diam_check(int diam, int flag)
{
    int count = 0;
    if (flag == 1)
    {
        if (diam != 16 && diam != 20 && diam != 24 && diam != 30 && diam != 36 && diam != 42 && diam != 48)
            count++;
    } else if (flag == 2)
    {
        if (diam != 16 && diam != 20 && diam != 22 && diam != 24 && diam != 27 && diam != 30 && diam != 36)
            count++;
    }

    if (count == 1)
    {
        puts("!!! Incorrect bolt diameter entered !!!\n");
        exit(1);
    }
}
