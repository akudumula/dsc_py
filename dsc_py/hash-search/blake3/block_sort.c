#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

void shuffle_array(int *array, size_t size) {
  for (size_t i = 0; i < size; i++) {
    size_t j = rand() % size;
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

void bubbleSort(int array[], int n) 
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++) 
    {
        for (j = 0; j < n - i - 1; j++) 
        {
        
            if (array[j] > array[j + 1]) 
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int main() 
{


    // do something that prints to stdout


    // These are values for a 10 gb file with 4 mb blocks
    long block_size = 262144;
    int arraySize = 65536;
    int targetSum = 40960;
    int lowerBound = 0;
    int upperBound = 1;

    // Seed the random number generator
    srand(time(NULL));

    int array[arraySize];
    int currentSum=0;

    for (int i = 0; i < arraySize-1; i++) 
    {
        int value = rand() % (upperBound - lowerBound + 1) + lowerBound;
        if (currentSum+value<=targetSum)
        {
            array[i]=value;
            currentSum += value;
        }
        else if (currentSum + value> targetSum)
        {
            array[i]=0;
        }
    }

    array[arraySize - 1] = targetSum - currentSum;
    // Print the array elements for verification
    // for (int i = 0; i < arraySize; i++) {
    //     printf("%d ", array[i]);
    // }

    // printf("\n");
    // int sum=0;
    // for (int i=0;i<arraySize;i++)
    // {
    //     sum+=array[i];
    // }
    // printf("sum = %d\n",sum);

    int** prefix_index = (int**)malloc(arraySize * sizeof(int*));

    for (int i = 0; i < arraySize; i++) {
        prefix_index[i] = (int*)malloc(array[i] * sizeof(int));
    }

    int index_values[targetSum];
    for(int i=0;i<targetSum;i++)
    {
        index_values[i]=i;
    }

    shuffle_array(index_values,targetSum);

    

    int a=0;
    for (int i =0;i<arraySize-1;i++)
    {
        for (int j=0;j<array[i];j++)
        {
            prefix_index[i][j]=index_values[a];
            a++;
            
        }

    }

    for (int i=0;i<arraySize;i++)
    {
        bubbleSort(prefix_index[i],array[i]);
    }

    //print prefix index

    // for (int i =0;i<arraySize-1;i++)
    // {
    //     for (int j=0;j<array[i];j++)
    //     {
            
    //             printf("%d , ",prefix_index[i][j]);
            
    //     }

    //     printf("\n");

    // }

    printf("Its working?\n");


    int offset_start[arraySize];
    int sum=0;
    for (int i=0;i<arraySize;i++)
    {
        offset_start[i]=sum;
        sum+=array[i];
    }

    // for (int i =0;i<arraySize-1;i++)
    // {
    //     for (int j=0;j<array[i];j++)
    //     {
            
    //             printf(" %d , ",prefix_index[i][j]);
            
    //     }

    //     printf("\n");

    // }

    //free(index_values);
    
    //int prefix_positioned[arraySize];

    // for (int i=0;i<array[0];i++)
    // {
    //     printf("%d, ",prefix_index[0][i]);
    // }
    // printf("\n"); 


    // Tried sorting algo professor thought

    // FILE *fileReader;
    // char buffer[(8 * 1024)];
    // fileReader = fopen("random_data.bin","r+");
    // printf("Current position of cursor: %ld\n",ftell(fileReader));
    // fread(buffer,(8 * 1024),1,fileReader);
    // fseek(fileReader,0,SEEK_SET);
    // printf("%s\n",buffer);
    // struct timeval start_time, end_time, time_result;
    // gettimeofday(&start_time, NULL);
    // char buffer1[block_size];
    // fread(buffer1,block_size,1,fileReader);
    // fseek(fileReader,0,SEEK_SET);
    // int count=0;
    // while(1)
    // {
    //     long cursor_location=ftell(fileReader);
    //     int offset_value=cursor_location/block_size;
    //     int prefix;
    //     int offset_to_be_moved;
    //     for (int i =0;i<arraySize;i++)
    //     {
    //         if (offset_value<offset_start[i])
    //         {
    //             prefix=i-1;
    //         }
    //         break;
    //     }
    //     char flag=0;
    //     for (int i=0;i<array[prefix];i++)
    //     {
    //         if(prefix_index[prefix][i]!=-1)
    //         {
    //             offset_to_be_moved=prefix_index[prefix][i];
    //             prefix_index[prefix][i]=-1;
    //             flag=1;
    //             break;
    //         }
    //     }
    //     if(flag==1)
    //     {
    //         fseek(fileReader,(offset_to_be_moved*block_size),SEEK_SET);
    //         char buffer2[block_size];
    //         fread(buffer2,block_size,1,fileReader);
    //         fseek(fileReader,cursor_location,SEEK_SET);
    //         fwrite(buffer2,block_size,1,fileReader);
    //         fseek(fileReader,(offset_to_be_moved*block_size),SEEK_SET);
    //         count++;
    //     }
    //     if(flag==0)
    //     {
    //         fwrite(buffer1,block_size,1,fileReader);
    //         break;
    //     }
    // }


    // gettimeofday(&end_time, NULL);
    

    // timersub(&end_time, &start_time, &time_result);
    // printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec,(long int) time_result.tv_usec);
    // printf("No errors?\n");

    // printf("Number of Swap: %d\n",count);


    // fseek(fileReader,0,SEEK_SET);
    // printf("Current position of cursor: %ld\n",ftell(fileReader));
    // fread(buffer,(8 * 1024),1,fileReader);
    
    // printf("%s\n",buffer);

    // fclose(fileReader);
    int count=0;

    //Sorting algo I thought
    printf("Starting sorting\n");
    struct timeval start_time, end_time, time_result;
    gettimeofday(&start_time, NULL);
    int swap_count=targetSum;
    FILE *hash_file=fopen("random_data.bin","r+");
    char buffer[block_size];
    fread(buffer,block_size,1,hash_file);
    // fclose(hash_file);
    // FILE *hash_file=fopen("random_data.bin","r+");
    fseek(hash_file,0,SEEK_SET);
    for (int i=0;i<swap_count;i++)
    {
        if (i%10000==0)
        {  
            printf("%d\n",i);
            gettimeofday(&end_time, NULL);
            timersub(&end_time, &start_time, &time_result);
            printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec,(long int) time_result.tv_usec);
        }
        long current_offset=ftell(hash_file)/block_size;
        int offset_start_index;
        int movement_from_offset_start_index;
        short flag=0;
        for (int j=0;j<arraySize;j++)
        {
            for (int k=0;k<array[j];k++)
            {
                if (prefix_index[j][k]==current_offset)
                {
                    offset_start_index=j;
                    movement_from_offset_start_index=k;
                    flag=1;
                    break;
                }       
                else if (prefix_index[j][k]>current_offset)
                {
                    flag=2;
                    break;
                }
            }
            if(flag==2)
            {
                continue;
            }
            if (flag==1)
            {
                break;
            }
        }

        long offset_to_be_moved_to=offset_start[offset_start_index]+movement_from_offset_start_index;
        char temporary[block_size];
        if(fseek(hash_file,(offset_to_be_moved_to*block_size),SEEK_SET)!=0)
            printf("Error 2\n");
        fread(temporary,block_size,1,hash_file);
        if(fseek(hash_file,(offset_to_be_moved_to*block_size),SEEK_SET) !=0)
            printf("Error 3\n");
        fwrite(buffer,(block_size),1,hash_file);
        memcpy(buffer,temporary,block_size);
        count++;
    }


    fclose(hash_file);
    gettimeofday(&end_time, NULL);
    
    printf("No. of swaps: %d\n",count);
    timersub(&end_time, &start_time, &time_result);
    printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec,(long int) time_result.tv_usec);
    printf("No errors?\n");

    

    

    return 0;
}