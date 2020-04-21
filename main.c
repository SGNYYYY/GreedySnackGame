//贪吃蛇第三版,新增无障碍模式
#include <stdio.h>
#include <windows.h>
#include <time.h>

#define PRINTF    printf("■");
#define PRINTFOOD printf("●");
#define EMPTY     printf("  ");
#define LINEheng  printf("_");
#define LINEshu   printf("|");
#define MapHeight 20
#define MapWide   80

typedef struct body
{
    int x,y;
    struct body* next;
} Body;

void setColor(unsigned short ForeColor,unsigned short BackGroundColor);
void introduction();
void information();
void SetPos(int x,int y);
int Menu();
void setLevel();
void countdown();
void PrintMap();
void inisnake();
void newFood();
void movesnake();
void movesnake_v2();
int JudgeWall();
int JudgeFood();
int JudgeBody();
void GameOver();
void freebody();
void HideCursor()
{
CONSOLE_CURSOR_INFO cursor_info = {1, 0};
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
int level = 1;//游戏等级
int direction;
int score;
int xx,yy;//食物坐标
Body *head;

int main(){
    srand(time(0));
    HideCursor();
    introduction();
    while(1){
        score = 0;
        int choice = 0;
        choice = Menu();
        switch(choice){
            case 1:
                setLevel();
                break;
            case 2:
            countdown();
            PrintMap();
            inisnake();
            movesnake();
            if(JudgeBody() == 0||JudgeWall() == 0){
                GameOver();
                SetPos(0,21);
                system("pause");
            }
            freebody();
            break;
            case 3:
            countdown();
            PrintMap();
            inisnake();
            movesnake_v2();
            if(JudgeBody() == 0){
                GameOver();
                SetPos(0,21);
                system("pause");
            }
            freebody();
            break;
            case 4:
                information();
                break;
            default:
                break;
        }
        if(choice == 0)
            break;
    }
    SetPos(0,21);
    return 0;
}
//设置颜色
void setColor(unsigned short ForeColor,unsigned short BackGroundColor){
    HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
    SetConsoleTextAttribute(handle,ForeColor+BackGroundColor*0x10);//设置颜色
}
//设置光标位置
void SetPos(int x,int y){
    COORD pos;
    HANDLE handle;
    pos.X=x;
    pos.Y=y;
    handle=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle,pos);
}
//游戏开始界面
void introduction(){
    for(int i = 0; i <= 15; i++){
        setColor(i,0);
        SetPos(0,0);
        printf("\n\n\n----------------------欢  迎-----------------------");
        printf("\n\n\n\n\n\n");
        printf("-------------------贪   吃   蛇---------------------\n\n\n\n\n");
        printf("           by: SGNYYYY\n\n\n");

        Sleep(300);
    }
    system("pause");
    system("cls");
}
//游戏说明
void information(){
    system("cls");
    printf("\n\n 你好! 朋友! , 这是第三代贪吃蛇,SG-贪吃蛇-version3.0\n");
    printf("\n此版本加入了wsad控制和无障碍模式, 并且修复了一些错误\n\n");
    printf("因此您可以使用wsad或者方向键控制蛇的移动方向\n\n");
    printf("    ENJOY  YOURSELF!\n");
    printf("    HAPPY   EVERY    DAY\n\n");
    system("pause");
}
//用户选择菜单
int Menu(){
reset:
    system("cls");
    int z = 0;
    char c;
    SetPos(0,0);
    printf("--------------选择菜单-------------\n\n\n");
    printf("1,选择等级\n\n");
    printf("2,经典模式\n\n");
    printf("3,无障碍模式\n\n");
    printf("4,游戏说明\n\n");
    printf("0,退出游戏\n\n");
    printf("请输入你的选择(以回车结束): ");
    while ((c = getchar()) != EOF && c != '\n'){
        z += z*10 + c - '0';
    }
    if(z != 0 && z != 1&&z != 2&&z != 3&&z != 4)
        goto reset;
    return z;
}
//设置等级
void setLevel(){
    set:
    system("cls");
    int l = 0;
    char c;
    SetPos(0,0);
    printf("1>>>>>>10");
    printf("\n\n简单>>>>>困难\n\n");
    printf("请输入等级(以回车结束): ");
    while ((c = getchar()) != EOF && c != '\n'){
        l += l*10 + c - '0';
    }
    level = l;
    if(level < 1||level > 10){
        level = 1;
        goto set;
    }
}
//开始前的倒计时
void countdown(){
    system("cls");
    int i = 0;
    SetPos(15,10);
    printf("ARE   YOU    READY?");
    Sleep(1000);
    system("cls");
    for(i = 3;i >= 1;i--){
        SetPos(20,10);
        printf("%d",i);
        Sleep(1000);
    }
}
//打印地图
void PrintMap(){
    system("cls");
    int i = 0;
    for(i = 1; i <= MapWide; i++)
    {
        SetPos(i,0);
        LINEheng
        SetPos(i,MapHeight);
        LINEheng
    }
    for(i = 1; i <= MapHeight; i++)
    {
        SetPos(1,i);
        LINEshu
        SetPos(MapWide,i);
        LINEshu
    }
    SetPos(MapWide+1,10);
    printf("your score: 0");
}

//初始化蛇身
void inisnake(){
    srand(time(0));
    head = (Body*)malloc(sizeof(Body));
    Body *temp = (Body*)malloc(sizeof(Body));
    head->next = temp;
    temp->x = rand()%21+4;
    temp->y = rand()%11+4;
    direction = rand()%4+1;                     //四个方向
    for(int i = 1; i <= 2; i++)
    {
        Body *p = (Body*)malloc(sizeof(Body));
        temp->next = p;
        switch(direction)
        {
        case 1://向上延长生成
            p->x = temp->x;
            p->y = temp->y-1;
            break;
        case 2://向右延长生成
            p->y = temp->y;
            p->x = temp->x+1;
            break;
        case 3://向下延长生成
            p->x = temp->x;
            p->y = temp->y+1;
            break;
        case 4://向左延长生成
            p->y = temp->y;
            p->x = temp->x-1;
            break;
        }
        temp = p;
    }
    temp->next = NULL;
    Body *ptr = head;
    for(ptr = head->next; ptr!=NULL; ptr = ptr->next)
    {
        SetPos(ptr->x*2,ptr->y);
        PRINTF
    }
}
//生成食物
void newFood(){
retry:
    xx = rand()%30+2;
    yy = rand()%18+2;
    int judge = 0;
    for(Body *p = head->next; p != NULL; p = p->next){
        if(xx == p->x&&yy == p->y)
            judge = 1;
    }
    if(judge)
        goto retry;
    SetPos(xx*2,yy);
    PRINTFOOD
}

//动起来
void movesnake(){
    newFood();
    while(1){
        if((GetAsyncKeyState(VK_DOWN)&0x8000)||(GetAsyncKeyState('S')&0x8000)){
            if(direction != 3){
                direction = 1;
            }
        }
        else if((GetAsyncKeyState(VK_LEFT)&0x8000)||(GetAsyncKeyState('A')&0x8000)){
            if(direction != 4){
                direction = 2;
            }
        }
        else if((GetAsyncKeyState(VK_UP)&0x8000)||(GetAsyncKeyState('W')&0x8000)){
            if(direction != 1){
                direction = 3;
            }
        }
        else if((GetAsyncKeyState(VK_RIGHT)&0x8000)||(GetAsyncKeyState('D')&0x8000)){
            if(direction != 2){
                direction = 4;
            }
        }
        int forx,fory,tempx,tempy;
        Body *p = head->next;
        forx = p->x;               //用来存储蛇头原来的位置
        fory = p->y;
        switch(direction){         //移动头
        case 1:                    //向下移动
            p->y+=1;
            break;
        case 2:                    //向左移动
            p->x-=1;
            break;
        case 3:                    //向上移动
            p->y-=1;
            break;
        case 4:                    //向右移动
            p->x+=1;
            break;
        }
        p = p->next;
        while(p!=NULL){             //每一个身体只要往前一个身体原来的位置移动
            tempx = p->x;
            tempy = p->y;
            p->x  = forx;
            p->y  = fory;
            forx  = tempx;
            fory  = tempy;
            p = p->next;
        }
        Body *Fptr = head;
        for(p = head->next; p != NULL; p = p->next){    //打印移动后的身体
            SetPos(p->x*2,p->y);
            PRINTF
            Fptr = Fptr->next;
        }
        if(JudgeWall() == 0){                          //判断是否是墙
            break;
        }
        if(JudgeFood() == 1){                           //判断是否吃到食物
            Body *newp = (Body*)malloc(sizeof(Body));
            Fptr->next = newp;
            newp->y = fory;
            newp->x = forx;
            newp->next = NULL;
            newFood();
            SetPos(MapWide+1,10);
            printf("your score: %d",score*level);
        }else{
            SetPos(forx*2,fory);
            EMPTY
        }
        if(JudgeBody() == 0){
            break;
        }

        for(int i = 0; i < 10000000*(12-level); i++) {}
    }
}
void movesnake_v2(){                //无障碍模式移动
    newFood();
    while(1){
        if((GetAsyncKeyState(VK_DOWN)&0x8000)||(GetAsyncKeyState('S')&0x8000)){
            if(direction != 3){
                direction = 1;
            }
        }
        else if((GetAsyncKeyState(VK_LEFT)&0x8000)||(GetAsyncKeyState('A')&0x8000)){
            if(direction != 4){
                direction = 2;
            }
        }
        else if((GetAsyncKeyState(VK_UP)&0x8000)||(GetAsyncKeyState('W')&0x8000)){
            if(direction != 1){
                direction = 3;
            }
        }
        else if((GetAsyncKeyState(VK_RIGHT)&0x8000)||(GetAsyncKeyState('D')&0x8000)){
            if(direction != 2){
                direction = 4;
            }
        }
        int forx,fory,tempx,tempy;
        Body *p = head->next;
        forx = p->x;               //用来存储蛇头原来的位置
        fory = p->y;
        switch(direction){         //移动头
        case 1:                    //向下移动
            p->y+=1;
            if(p->y == MapHeight)
                p->y = 1;
            break;
        case 2:                    //向左移动
            p->x-=1;
            if(p->x == 0)
                p->x = MapWide/2-1;
            break;
        case 3:                    //向上移动
            p->y-=1;
            if(p->y == 0)
                p->y = MapHeight-1;
            break;
        case 4:                    //向右移动
            p->x+=1;
            if(p->x == MapWide/2)
                p->x = 1;
            break;
        }
        p = p->next;
        while(p!=NULL){             //每一个身体只要往前一个身体原来的位置移动
            tempx = p->x;
            tempy = p->y;
            p->x  = forx;
            p->y  = fory;
            forx  = tempx;
            fory  = tempy;
            p = p->next;
        }
        Body *Fptr = head;
        for(p = head->next; p != NULL; p = p->next){    //打印移动后的身体
            SetPos(p->x*2,p->y);
            PRINTF
            Fptr = Fptr->next;
        }
        if(JudgeFood() == 1){                           //判断是否吃到食物
            Body *newp = (Body*)malloc(sizeof(Body));
            Fptr->next = newp;
            newp->y = fory;
            newp->x = forx;
            newp->next = NULL;
            newFood();
            SetPos(MapWide+1,10);
            printf("your score: %d",score*level);
        }else{
            SetPos(forx*2,fory);
            EMPTY
        }
        if(JudgeBody() == 0){
            break;
        }

        for(int i = 0; i < 10000000*(12-level); i++) {}
    }
}
int JudgeWall(){
    if((head->next->x <= 0||head->next->x >= MapWide/2)||(head->next->y == 0 ||head->next->y == MapHeight))
        return 0;
    return 1;
}
int JudgeFood(){
    if(head->next->x == xx&&head->next->y == yy)
    {
        score++;
        return 1;
    }
    return 0;
}
int JudgeBody(){
    int toux,touy;
    toux = head->next->x;
    touy = head->next->y;
    for(Body *p = head->next->next; p != NULL; p = p->next){
        if(p->x == toux&&p->y == touy){
            return 0;
        }
    }
    return 1;
}
void GameOver(){
    system("cls");
    for(int i = 0; i <= 15; i++)
    {
        setColor(i,0);
        SetPos(0,8);
        printf("------------------GAME   OVER-------------------\n\n\n\n\n");
        printf("YOUR  SCORE:   %d",score*level);
        Sleep(200);
    }
}
void freebody(){
    Body *pr = head->next;
    while(pr!=NULL)
    {
        Body *temp = pr;
        pr = pr->next;
        free(temp);
    }
    free(head);
}
