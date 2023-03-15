#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

/*
    Lmao got bored and looks like its time for me build a snake game in c 
    variables needed
    
    struct board:
        int height
        int width

        //x and y coords of cherry 
        int cherryX
        int cherryY

        //i planed to add a matrix to represnt the board but to be honest, i dont think it is worth it
    
    struct snake
        int length
        int segment[X*Y][2] = list of locations of segment. 

        //x and y coords of head
        headX 
        headY  

        //x and y coords of tail
        tailX
        tailY
*/


typedef struct boards{
    int height;
    int width;
    int* grid; //stores info aobut grid 
    //0 for empty, 1 for snek

}Board;

//self explanatory
typedef enum direction {
    up,
    down,
    left,
    right
}Direction;

//the snake
typedef struct snek{
    int headX;
    int headY;
    int tailX;
    int tailY;
    Direction dir;
    int* snek_queue; //queue datastruct for snek
    int head;
    int tail;
}Snek; 


Board board = {0,0,NULL};
Snek snek = {0,0,0,0,right,NULL,0,0};
bool continue_game = true;

void delay_time(int ns){
    
    int  ms = ns*1000;
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() <ms);
}


//ITS A PAINN TO USE but okay
void set_grid_element(int j,int i,int item){
    board.grid[j*board.width + i] =  item;
    return;
}

int get_grid_element(int j,int i){
    int results;
    results = board.grid[j*board.width+i];
    return results;
}   

void clean_board(){
    int i,j;
    for (j=0;j<board.height;j++){
        for(i=0;i<board.width;i++){
            set_grid_element(j,i,0);
        }
    }
}
//add head to the queue
void push_head(int y,int x){
    int board_size = board.width*board.height;
    snek.head++;
    if(snek.head==board_size){
        snek.head=0;
    }   
    snek.snek_queue[(snek.head)*2] = y;
    snek.snek_queue[snek.head*2+1] = x;
}

int* pop_tail(){
    int board_size = board.width*board.height;
    int y,x;
    static int return_val[2];
    snek.tail++;
    if(snek.tail==board_size){
        snek.tail=0;
    }   
    y = snek.snek_queue[(snek.tail)*2] ;
    x = snek.snek_queue[snek.tail*2+1] ;
    return_val[0] = y;
    return_val[1] = x;
    return return_val;
}

//print board
void print_board(){
    int i,j ;
    printf("wasd to move, q to quit\n\n\n");
    for(j=0;j<board.height;j++){
        for(i=0;i<board.width;i++){
            if (get_grid_element(j,i) == 0)
                printf(". ");
            else if(get_grid_element(j,i)==1)
                printf("o ");
            else
                printf("x ");
        }
        printf("\n");
    }
}

void spawn_fruit(){
    //random fruit
    int x,y,status;
    bool repeat = true;
    while(repeat){
        srand(time(NULL)); //seed a random number
        x = rand()%board.width;
        y = rand()%board.height;
            status = get_grid_element(y,x);
        if(status == 0){
            repeat = false;
        }
    }


    set_grid_element(y,x,2);

}

void setup_snek(){
    snek.snek_queue = (int*)malloc(board.height*board.width*2*sizeof(int));
}

void update_snek(){
    //north
    int snek_status = 0;
    int oldX = snek.headX;
    int oldY = snek.headY;
    int* tail;
    switch(snek.dir){
        case up:
            if(snek.headY>0) snek.headY --;
            else snek.headY = board.height-1;
            break;
        case down:
            if(snek.headY<board.height-1) snek.headY ++;
            else snek.headY = 0;
            break;
        case right:
            if(snek.headX<board.width-1) snek.headX++;
            else snek.headX=0;
            break;
        default:
            if(snek.headX>0) snek.headX--;
            else snek.headX = board.width-1;
            break;
    }  
    push_head(snek.headY,snek.headX);
    //check status of snek
    /*
        if head == fruit, tail will stay the same
        if head == another body, game over
        else, keep moving
    */
    snek_status = get_grid_element(snek.headY,snek.headX);
    if(snek_status==1){
        continue_game=false;
    }
    //empy space
    else if(snek_status==0){
        set_grid_element(snek.tailY,snek.tailX,0);
        tail = pop_tail();
        snek.tailX = tail[1];
        snek.tailY = tail[0];
    }

    set_grid_element(snek.headY,snek.headX,1);

    //set the head to 1 first so fruit wont spawn there when this condition is true
    if(snek_status==2){
        spawn_fruit();
    }


    //for now just move the hea
}


//main loop
int loop(){
    char c;
    continue_game = true;

    clean_board();
    spawn_fruit();
    setup_snek();
    while(continue_game){
        //print 
        system("cls");
        print_board();
        Sleep(100);
        update_snek();



        //updates
        if(kbhit()){
            c=getch();
            switch(c){
                case 119: //w
                    if (snek.dir != down)
                        snek.dir = up;
                    break;
                case 97:
                    if (snek.dir != right)
                        snek.dir = left;
                    break;
                case 115:
                    if (snek.dir != up)
                        snek.dir = down;
                    break;
                case 100:
                    if (snek.dir != left)
                        snek.dir = right;
                    break;
                case 113:
                    continue_game = false;
                    break;

            }
        }
    }

    free(board.grid);
    free(snek.snek_queue);
    printf("Game Over");
    scanf("%i");

    return 0;
}


int main(){
    int h,w;

    printf("Width and height of the board: ");
    scanf("%i %i",&h,&w);
    board.height = h;
    board.width = w;
    board.grid = (int*)malloc(h*w*sizeof(int));
    loop();

    return 0;
}