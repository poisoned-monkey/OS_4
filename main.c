#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


void sortBooble(char*massIn)//сортировка пузырьком
{
    int tmp;
    int noSwap;
    int i=0;
    int N =strlen(massIn);
    for ( i= N - 1; i >= 0; i--)
    {
        noSwap = 1;
        for (int j = 0; j < i; j++)
        {
            if (massIn[j] > massIn[j + 1])
            {
                tmp = massIn[j];
                massIn[j] = massIn[j + 1];
                massIn[j + 1] = tmp;
                noSwap = 0;
            }
        }
        if (noSwap == 1)
            break;
    }

}



int main()
{

    pid_t pid;//идентификатор потока
    int fd1,fd2, i;

    char fname1[50];
    char fname2[50];

    int fsize=0;
    char *ptr1,*ptr2;

    printf("Make sure all files are in the same folder.\nEnter the name of the first file\n");
    gets(fname1);
    //    fname1="/Users/macbook/Desktop/OS_2/file1.txt"
    
    
    printf("\nEnter the name of the second file\n");
    gets(fname2);
    //    fname2="/Users/macbook/Desktop/OS_2/file2.txt"

    //-------------------------------------------------------------------------отображение в память первого файла
    fd1 = open(fname1, O_RDWR | O_CREAT, DEFFILEMODE);
    ptr1= mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);//настройки для записи и чтения ,доступ для всех потоков
    //-----------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------отображение в память второго файла
    fd2 = open(fname2, O_RDWR | O_CREAT, DEFFILEMODE);
    ptr2 = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);//настройки для записи и чтения ,доступ для всех потоков
    //-----------------------------------------------------------------------------------------------------------

    close(fd1);
    close(fd2);

     pid = fork();//создаем новый поток

     if (pid == -1)//поток не создан
     {
      perror("fork");
      exit(EXIT_FAILURE);
     }

     if(pid==0)//если ребенок
     {
          sortBooble(ptr2);//сортируем данные
         _exit(EXIT_SUCCESS);//выходим из потока
     }
     else
     {

        //-------------------------------------выводим данные о завершении дочернего потока
        int status;
        waitpid(pid, &status, 0);
        printf("exit normally? %s\n", (WIFEXITED(status) ? "true" : "false"));
        printf("child exitcode = %i\n", WEXITSTATUS(status));
        //---------------------------------------------------------------------------------


        sortBooble(ptr1);//сортируем текущие даннные родительского потока

        int s_data1= strlen(ptr1); //количество символов в файлах
        int s_data2 = strlen(ptr2);

        int p;
        if(s_data1>s_data2)
        p=s_data1;
        else
        p=s_data2;

        for( int i=0; i<p; i++)
        {

            if(i<s_data1)
            printf("file1->%c\n",*(ptr1+i));
            if(i<s_data2)
            printf("file2->%c\n",*(ptr2+i));
        }

        wait(NULL);                /* Ожидание потомка */
        _exit(EXIT_SUCCESS);
    }
}