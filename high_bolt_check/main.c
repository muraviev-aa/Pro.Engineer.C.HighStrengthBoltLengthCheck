#include <stdio.h>
#include <unistd.h>
#include "temp_function.h"

#define SIZE 7 // число строк в файле

int connect_package[7];

int bolt_length_52644[31] = {40, 45, 50, 55, 60, 65, 70, 75, 80,
                             85, 90, 95, 100, 105, 110, 115, 120,
                             125, 130, 140, 150, 160, 170, 180,
                             190, 200, 220, 240, 260, 280, 300};

int bolt_length_32484_3[31] = {35, 40, 45, 50, 55, 60, 65, 70, 75,
                               80, 85, 90, 95, 100, 110, 120, 130,
                               140, 150, 160, 170, 180, 190, 200};

int flag_g;

int main(int argc, char *argv[])
{
    bolt *info = malloc(SIZE * sizeof(bolt));
    if (!info)
        printf("Error while allocating memory!\n");
    FILE *fptr;
    int rez, count;
    char *file_name;
    // При запуске приложения без аргументов открывается справка
    if (argc == 1)
    {
        print_info();
    }

    // Работа с аргументами командной строки
    while ((rez = getopt(argc, argv, "hf:m:l:s:t:w:g:n:")) != -1)
    {
        switch (rez)
        {
            case 'h':
                print_help();
                break;
            case 'f':
                file_name = optarg;
                printf("File name is %s.\n", file_name);
                if (strcmp(file_name, "52644.csv") == 0)
                    flag_g = 1;
                else if (strcmp(file_name, "32484.3.csv") == 0)
                    flag_g = 2;
                break;
            case 'm':
                // Диаметр резьбовой части болта
                connect_package[0] = atoi(optarg);
                break;
            case 'l':
                // Длина болта [мм]
                connect_package[1] = atoi(optarg);
                break;
            case 's':
                // Толщина соединяемых деталей под головкой болта [мм]
                connect_package[2] = atoi(optarg);
                break;
            case 't':
                // Толщина одной детали под гайкой [мм]
                connect_package[3] = atoi(optarg);
                break;
            case 'w':
                // Количество шайб под головкой болта
                connect_package[4] = atoi(optarg);
                break;
            case 'g':
                // Количество шайб под гайкой
                connect_package[5] = atoi(optarg);
                break;
            case 'n':
                // Количество гаек
                connect_package[6] = atoi(optarg);
                break;
            case '?':
                printf("Error found !\n");
                break;
        }
    }

    print_input_data(connect_package, flag_g);
    open_file(&fptr, file_name);
    count = read_data_file(&fptr, info);
    fclose(fptr);
    print(info, count);
    if (flag_g == 1)
        bolt_check_thread_52644(info, count, connect_package);
    free(info);
    return 0;
}
