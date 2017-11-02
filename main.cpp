#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main()
{
    //Read file into array
    char buffer[1000]; char *word, *line; float val;
    float import[15][200]; int array_size=0; int array_size2=0;
    //Read file into array
    FILE *fptr=fopen("C:\\Users\\Jack\\Documents\\CalPoly\\NuRad\\Spherical Harmonic Fits - Vpol Gain.csv","r");
    while((line=fgets(buffer,sizeof(buffer),fptr)) != NULL)   //Runs down lines
       {
           int j=0;
        word = strtok(line,"\t,");
        while(word != NULL)
        {
            val=strtof(word,NULL);
            if (j==2 && int(val)==0)
            {
                break;
            }
            else if (j>=2)
            {
                import[array_size][array_size2]=val;
                array_size++;
                if (array_size==13)
                {
                    array_size2++;
                    array_size=0;
                }
            }
            word = strtok(NULL,"\t,");
            j++;
        }
       }
       array_size=13;
       /*int j=0; float perfect[array_size][array_size2];
       for (j=0;j<array_size2;j++)
       {
           int i=0;
           for (i=0;i<array_size;i++)
           {
               perfect[i][j]=import[i][j];
           }
       }*/
       float perfect[array_size];
       int i=0;
       for (i=0;i<array_size;i++)
       {
           perfect[i]=import[i][0];
       }
    fclose(fptr);

    do {
    int max_gen = 0;
    int max_val = 100;
    float sum_val = 0;
    //float f_max_val = 100.0;
    float target_score=0;
    /*printf("\nMaximum Value: ");   //Query
   std::cin >> max_val; //Answer*/
    printf("\nTarget Difference (Percentage): ");   //Query
   std::cin >> target_score; //Answer
    printf("\nGeneration Limit: ");   //Query
   std::cin >> max_gen; //Answer
   char redo;
   int test_no=13; int array_length=13; //int max_val=10000;

   //Perfect ("Goal") Array
   int t; float min_val=0.0f;
   for (t=0;t<array_length;t++)
   {
       if (perfect[t]<-min_val)
       {
           min_val=fabs(perfect[t]);
       }
   }
   for (t=0;t<array_length;t++)
   {
       perfect[t]+=min_val;
       sum_val+=fabs(perfect[t]);
   }
   //Elite Array
   float elite[array_length][test_no];
   float fitness[test_no]={0.0f};
   float best_fit={1000.0f};
   float best_fit2={0.0f};
   float best_choice[array_length]={0.0f};
   float elite_error_value=10000.0f;
   float elite_array[array_length]={0.0f};
   float elite_error_percent=10000.0f;
   float best_fit_percent={0.0f};
   int winner=0; int winner2=0; int check;

   //Roulette Array
   float roulette[array_length][test_no];
   float roulette_winner[array_length]; float roulette_score=0.0f; float roulette_percentage=0.0f; int roulette_pool;
   float best_roulette_winner[array_length];
   float roulette_error_percent=10000.0f;
   float roulette_max_variance=0.0f;
   float fit_hold[test_no*max_val*array_length]={0.0f}; int fitness_length;

   //Tournament array
   float tour[array_length][test_no];
   float tour_best[array_length];
   float tour_score[test_no]={0};
   float tour_error_value=10000.0f;
   float tour_error_percent=10000.0f;
   float tour_hold[array_length];


int loop=9;
    srand (time(NULL));
while (loop<10)
{
    loop++;

   //Initialization
    for (i=0;i<test_no;i++)
    {
        int j=0;
        for (j=0;j<array_length;j++)
        {
            elite[j][i]=rand()%int(sum_val)+min_val;
            roulette[j][i]=rand()%int(sum_val)+min_val;
            tour[j][i]=rand()%int(sum_val)+min_val;
        }
    }

    //Elite Test
    int n=0;
    while ((elite_error_percent>target_score && n<max_gen)) //Generations
    {
        best_fit=float(array_length*max_val); best_fit2=float(array_length*max_val);
        for (i=0;i<test_no;i++) //Each individual in the population
        {
            int j=0; fitness[i]=0;
            for(j=0;j<array_length;j++) //Each "allele" of the individual
            {
                if (i==test_no-1)
                {
                    elite[j][i]=(float)rand()/(float)(RAND_MAX/sum_val);
                }
                else if (i==j)
                {
                    elite[j][i]=(float)rand()/(float)(RAND_MAX/sum_val);
                }
                fitness[i]+=fabs(elite[j][i]-perfect[j]);
            }
            if (fitness[i]<best_fit)
            {
                best_fit2=best_fit;
                best_fit=fitness[i]; winner=i;
            }
            else if (fitness[i]<best_fit2 && fitness[i]>=best_fit)
            {
                best_fit2=fitness[i]; winner2=i;
            }
        }

        float val_mat[test_no][test_no]={{0.0}};
        float val_mat_min=10000.0;
        int min_x[3]; int min_y[3];
        for (i=0;i<test_no;i++)
        {
            int j;
            for (j=0;j<test_no;j++)
            {
                int k;
                for (k=0;k<array_length;k++)
                {
                    val_mat[i][j]+=fabs(perfect[k]-(elite[k][i]+elite[k][j])/2);
                }
                if (val_mat[i][j]<val_mat_min)
                {
                    min_x[3]=min_x[2]; min_y[3]=min_y[2];
                    min_x[2]=min_x[1]; min_y[2]=min_y[1];
                    min_x[1]=i; min_y[1]=j;
                    val_mat_min=val_mat[i][j];
                }
            }
        }

        int j=0;
        for (j=0;j<array_length;j++)
        {
            //best_choice[j]=(elite[j][winner]+elite[j][winner2])/2.0;
            best_choice[j]=elite[j][winner];
        }
        if (best_fit<elite_error_value)
        {
            for (i=0;i<array_length;i++)
            {
                elite_array[i]=best_choice[i];
            }
            elite_error_value=best_fit;
        }
        if (n<max_gen)
        {
            for (i=0;i<round(test_no/2);i++)
            {
                for (j=0;j<array_length;j++)
                {
                    //elite[j][i]=best_choice[j];
                    elite[j][i]=(elite[j][min_x[1]]+elite[j][min_y[1]])/2;
                }
            }
            for (i=round(test_no/2)+1;i<round(3*test_no/4);i++)
            {
                for (j=0;j<array_length;j++)
                {
                    //elite[j][i]=elite[j][winner];
                    elite[j][i]=(elite[j][min_x[2]]+elite[j][min_y[2]])/2;
                }
            }
            for (i=round(3*test_no/4)+1;i<test_no;i++)
            {
                for (j=0;j<array_length;j++)
                {
                    //elite[j][i]=elite[j][winner2];
                    elite[j][i]=(elite[j][min_x[3]]+elite[j][min_y[3]])/2;
                }
            }
        }
    best_fit_percent=100*float(best_fit)/sum_val;
    elite_error_percent=100*float(elite_error_value)/sum_val;
    //printf("%f\t",elite_error_percent);
    n++;
    }
    int elite_gens=n;
   //Ranking
   //Roulette wheel
//printf("\n\n");
    n=0;
    while (roulette_error_percent>target_score && n<max_gen) //Generations
    {
        roulette_pool=test_no;
        do {
        fitness_length=0; roulette_score=0.0;
        for (i=0;i<roulette_pool;i++) //Each individual in the population
        {
            int j=0; fitness[i]=0;
            for(j=0;j<array_length;j++) //Each "allele" of the individual
            {
                if (i==j)
                {
                    //roulette[j][i]=float(rand()%int(sum_val));
                    roulette[j][i]=(float)rand()/(float)(RAND_MAX/sum_val);
                }
                fitness[i]+=fabs(roulette[j][i]-perfect[j]);
            }
            fitness_length+=(max_val*test_no-int(fitness[i]*10));
            for (j=fitness_length-(max_val*test_no-int(fitness[i]))-1;j<fitness_length;j++)
            {
                fit_hold[j]=i;
            }
        }
            check=rand()%(fitness_length-1);
            winner=fit_hold[check];
            int j;
            for (j=0;j<array_length;j++)
            {
                roulette_winner[j]=roulette[j][winner];
                roulette_score+=fabs(perfect[j]-roulette_winner[j]);
            }
            roulette_percentage=100*roulette_score/sum_val;
        roulette_pool=1;
        for (i=0;i<test_no;i++)
        {
            if(fitness[i]<fitness[winner])
            {
                roulette_pool+=1;
            }
        }
        if (roulette_pool==1)
        {
            for (i=0;i<test_no;i++)
            {
                int j;
                for (j=0;j<array_length;j++)
                {
                    roulette[j][i]=roulette[j][winner];
                }
            }
            break;
        }
        }while(1);
        if (roulette_percentage<roulette_error_percent)
        {
            roulette_max_variance=0.0f;
            int j;
            for (j=0;j<array_length;j++)
            {
                best_roulette_winner[j]=roulette_winner[j];
                    if (100*fabs(perfect[j]-roulette_winner[j])/sum_val>roulette_max_variance)
                    {
                        roulette_max_variance=100*fabs(perfect[j]-roulette_winner[j])/sum_val;
                    }
            }
            roulette_error_percent=roulette_percentage;
        }
        n++;
    //printf("%f\t",roulette_error_percent);
    }
    int roulette_gens=n;

    //Tournament
    //for (n=0;n<max_gen;n++)
    n=0;
    while (tour_error_percent>target_score && n<max_gen) //Generations
    {
        int k=rand()%(array_length);
        //int k=3;
        tour_error_value=sum_val*array_length;
        int j=rand()%array_length;
        for (i=0;i<test_no;i++)
        {
            int k_found=0;
            //while(k_found != k)
            while(k_found<array_length)
            {
                j=j+k;
                if (j>=array_length)
                {
                    j=j-array_length;
                }
                tour_score[i]+=fabs(tour[j][i]-perfect[j]);
                tour_hold[k_found]=tour[j][i];
                k_found++;
            }
            if (tour_score[i]<tour_error_value)
            {
                k_found=0; j=0;
                for (j=0;j<array_length;j++)
                {
                    tour_best[j]=tour_hold[j];
                }
                tour_error_value=tour_score[i];
            }
        }
        for (i=0;i<test_no;i++)
        {
            for (j=0;j<array_length;j++)
            {
                tour[j][i]=tour_best[j];
                if (i==j)
                {
                    tour[j][i]=(float)rand()/(float)(RAND_MAX/sum_val);
                }
            }
        }
        tour_error_percent=100*tour_error_value/sum_val;
        n++;
    }
    int tour_gens=n;

   //Print Out Values
    printf("\nGoal\n");

    int j=0;
    for (j=0;j<array_length;j++)
    {
        printf("%f ",perfect[j]);
    }
        best_fit_percent=100*float(best_fit)/(sum_val*float(array_length));
            printf("\n\nElite (%i Generations)\n",elite_gens);
            for (j=0;j<array_length;j++)
            {
                printf("%f ",elite_array[j]);
            }
        printf("\nBest Difference: %2.3f Percent\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",elite_error_percent);
        printf("\n");
            for (j=0;j<array_length;j++)
            {
                printf("%f ",best_choice[j]);
            }
        printf("\nBest Difference: %2.3f Percent\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",best_fit_percent);

        printf("\n\nRoulette (%i Generations)\n",roulette_gens);
            for (j=0;j<array_length;j++)
            {
                printf("%f ",best_roulette_winner[j]);
            }
        printf("\nBest Difference: %2.3f Percent\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",roulette_error_percent);
        printf("\nMaximum Error: %2.3f Percent\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",roulette_max_variance);

        printf("\n\nTournament (%i Generations)\n",tour_gens);
            for (j=0;j<array_length;j++)
            {
                printf("%2.3f ",tour_best[j]);
            }
        printf("\nBest Difference: %2.3f Percent\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",tour_error_percent);
}
printf("\n\nSIMULATION COMPLETE\n~~~~~~~~~~~~~~~~~~~\n");
printf("\nTry again? ");
   scanf("%s",&redo);
   if (!((redo=='y')||(redo=='Y')))
   {
       break;
   }
}while(1);
   return 0 ;
 }
