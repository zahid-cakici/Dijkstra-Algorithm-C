#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int ROW = 0;    //A global variable for adjacency matrix rows

void Dijkstra(int**, int ,int);     //Dijkstra method function

int** readInput(char*);     //Reading the input function

void printAdj(int**);       //Print function for adjacency matrix 

void printMenu();       //Menu



int main(){
    int** adjMatrix;    //Adjacency matrix declaration 
    char src,dest,filename[15];
    int i,choice = 0;
    while (choice != 4){
        printMenu();
        scanf(" %d",&choice);   //Taking the choice from user and do the operations
        switch (choice){
        case 1:
            printf("Please write a filename (eg. input1.txt):\n");
            scanf("%s",filename);
            if (ROW != 0){          //If a adjacency matrix created before delete it.
                for (i = 0; i < ROW; i++){
                    int* current = adjMatrix[i];
                    free(current);
                }
                free(adjMatrix);
            }
            adjMatrix = readInput(filename);  //Call the read function by filename and assign the returning value
            break;
        case 2:
            printAdj(adjMatrix);    //Print adjacency matrix
            break;
        case 3:
            printf("Source Node:\n");
            scanf(" %c",&src);
            printf("Destination Node:\n");
            scanf(" %c",&dest);                     //Take source and destination node from user and pass them
            if(toupper(src)-65>ROW || toupper(dest)-65>ROW){
                printf("The vertexes not found");
                break;
            }
            Dijkstra(adjMatrix,toupper(src)-65,toupper(dest)-65);//to Dijkstra method as integers
            break;
        case 4:
            for (i = 0; i < ROW; i++){      //When we exiting the program
                int* current = adjMatrix[i];    //delete the whole adjacency matrix
                free(current);                  //that we dynamically allocated bedore
            }
            free(adjMatrix);
            printf("Terminating the program. Bye!\n");
            break;
        
        default:
            printf("You entered an invalid value. Try again.");
            fflush(stdin);
            break;
        }
    }

    return 0;
}

void Dijkstra(int** matrix, int src, int dest) {
    int distances[ROW][ROW];    //A matrix that holds all distance informations between nodes
    int distanceFromSrc[ROW];   //An array to maintain ditances of all nodes from the source node (index => node,value => distance)
    int path[ROW];     /*An array that keeps the shortest path from source node to destination node.
    (index => node,value => the node that reaches it in the shortest way ). For example, if the array look like for source A:
    index: A B C D E F G
    value: B B B B C C A Then the shortest path from A to F is : A->B->C->F.*/
    
    int visited[ROW];       //An array that holds the info that the node is visited before or not.
    int INFINITY = 9999999;    //An infinity value for Dijkstra method to assign the nodes that can not be reached.
    int ctr, mindistance, currentVertex, i, j;  //Some control variables for loops.

    for (i = 0; i < ROW; i++){
        for (j = 0; j < ROW; j++){
            if (matrix[i][j] != 0)              //Filling the distances matrix according to our adj matrix.
                distances[i][j] = matrix[i][j]; //If there is connection take its distance.
            else    
                distances[i][j] = INFINITY;     //If there is not assign it to infinity.
        }
    }

    for (i = 0; i < ROW; i++) {
        distanceFromSrc[i] = distances[src][i]; //We save the nodes that are directly connected to source node
        visited[i] = 0;         //We initialize the visited array as 0 since we did not visit any node for the time being
        path[i] = src;   //For the nodes that are directly connected there is no other possible shortest path
        //so we add them in path array
    }
        
        visited[src] = 1;           //We visited the source node in the for loop above
        distanceFromSrc[src] = 0;   //Assigning the distance from source to source as 0

        ctr = 1;

    while (ctr < ROW- 1) {          //We will visit other nodes
        mindistance = INFINITY;     //The minimum distance from source to the node to discover
        /*First we take minimum distance as INFINITY.
        Then we search for a node to discover. And that node needs to be smaller than 
        minimum distance and not visited before. And we update the minimum distance*/

        for (i = 0; i < ROW; i++){
            if (distanceFromSrc[i] < mindistance && visited[i] != 1) {
                mindistance = distanceFromSrc[i];
                currentVertex = i;
            }
        }
        /*After we found a vertex to discover we will update the connected vertexes to it by sum of 
        minimum distance and distance from currentVertex OR distance from source whichever is less. 
        After we update distancesFromSrc we keep it in path array.*/
        visited[currentVertex] = 1;
        for (i = 0; i < ROW; i++){
            if (visited[i] != 1){
                if (mindistance + distances[currentVertex][i] < distanceFromSrc[i]) {
                distanceFromSrc[i] = mindistance + distances[currentVertex][i];
                path[i] = currentVertex;
                }
            }
        }        
        ctr++;
    }
  
    printf("\nDistance from %c to %c: %d\n", src+65,dest+65, distanceFromSrc[dest]);
    
    /*Printing the shortest path according to our path array.First we print the destination vertex
    then we rewind our information between index and value. As an example:
    index: A B C D E F      If we want the  path from A to F we first print out the F
    value: A A B B C C      then we print the shortest way to F which is C
    Then we do the same untill we get the source vertex. Finally the path will be printed*/
    printf("The path: %c",dest+65);
    while (dest != src){
        dest = path[dest];
        printf("<-%c",dest+65);
    }
    printf("\n");
    
}

int** readInput(char* filename){
    
    FILE* file = fopen(filename,"r");

    if(!file){
        printf("File could not found");
        exit(1);
    }

    char a,b,largest='A',line[10];
    
    while (fgets(line,10,file) != NULL){
        if (!strcmp(line,"\n"))         //If there are empty lines at the end break
            break;
        sscanf(line,"%c,%c",&a,&b);         //We seacrh file to find the largest vertex
        if(largest<a)                       //in order to know the size to allocate our matrix
            largest = a;
        if(largest<b)
            largest = b;
    }

    ROW = largest-64;      //Assigning the ROW variable based on largest vertex by using ASCII (eg.F-64 => 6)
     
    int i,j;
    int **matrix = (int **)malloc(ROW * sizeof(int *));
    for (i=0; i<ROW; i++)
         matrix[i] = (int *)malloc(ROW * sizeof(int));
    



    for (i = 0; i <  ROW; i++){
      for (j = 0; j < ROW; j++)
         matrix[i][j] = 0;
        }
    //Dynamically allocate matrix and initialize it as 0

    rewind(file);   //Rewind the file cursor to go to the beginning of the file
    int c;
    while (fgets(line,10,file) != NULL){
        if (!strcmp(line,"\n"))
            break;
        sscanf(line,"%c,%c,%d",&a,&b,&c);
        i = a-65; //eg(A-65 => 0)   //Filling our adjacency matrix 
        j = b-65; //eg(B-65 => 1)   //according to file
        matrix[i][j] = c;    
        matrix[j][i] = c;    
    }
    fclose(file);
    return matrix;
}

void printAdj(int **matrix){    //Function to show adjacency matrix
    int i,j;
    printf("************Adjacency Matrix*************\n");
    printf(" ");
    for (i = 0; i< ROW; i++)
        printf("%3c",65+i);
    for (i = 0; i < ROW; i++){
        printf("\n%c",65+i);
        for (j = 0; j < ROW; j++){
            if (matrix[i][j] == 0)
                printf("  -");
            else
                printf("%3d", matrix[i][j]);
        }
            
    }
    printf("\n");
}

void printMenu(){       //A basic Menu
    printf("\n1. Read File\n");
    printf("2. Show  adjacency  matrix\n");
    printf("3. Find shortest path\n");
    printf("4. Exit\n");
    printf("\n");
}