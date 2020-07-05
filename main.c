#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

struct commodity//商品信息结构体
{
    char tradename[20];
    char tradetype[20];
    char tradepro[20];
    int number;
    double price;
    double boughtprice;
    double profit;
    int push;
    int pop;
    int store;
    int swarn;
    char producetime[30];
    int bztime;
    char intime[30];
    char outtime[30];
    struct commodity *next;
};

struct Record//记录信息结构体
{
    char time[30];
    char User[20];
    int tradenum;
    char tradename[20];
    int num;
    int IO;//0为入库，1为出库;
    double price;
    struct Record *NextRecord;
};

struct User//用户信息结构体
{
    char Username[100];
    struct commodity *tradelist;
    struct Record *recordlist;
};

struct User *user;//用户全局变量
FILE *fw,*fr;//文件操作指针

char* getCurrentTimeStr()//获取当前时间
{
    time_t t = time(NULL);
    char time[25];
    strftime(time, sizeof(time)-1, "%Y-%m-%d %H:%M:%S", localtime(&t));
    return time;
}

void dorecord(int tnum,char tname[],int tp,int io,double tprice)//记录进销存
{
    struct Record *p;
    p=new Record;
    strcpy(p->time,getCurrentTimeStr());
    strcpy(p->User,user->Username);
    p->tradenum=tnum;
    strcpy(p->tradename,tname);
    p->num=tp;
    p->IO=io;
    p->price=tprice;
    p->NextRecord=user->recordlist->NextRecord;
    user->recordlist->NextRecord=p;
}

void rfilein()//记录写入文件
{
    char filename[100];
    strcpy(filename,user->Username);
    strcat(filename,"record.txt");
    fw=fopen(filename,"w");
    struct Record *q;
    q=user->recordlist->NextRecord;
    while(q)
    {
        fprintf(fw,"%s %s %d %s %d %d %f\n",q->time,q->User,q->tradenum,q->tradename,q->num,q->IO,q->price);
        q=q->NextRecord;
    }
    fclose(fw);
}

void rfileout()//记录读出文件
{
    char filename[100];
    strcpy(filename,user->Username);
    strcat(filename,"record.txt");
    fr=fopen(filename,"r");
    char ttime[30],t[20],tuser[20],tname[20];
    int tnum,tp,io;
    double tprice;
    struct Record *p,*q;
    while(fscanf(fr,"%s%s%s%d%s%d%d%lf",ttime,t,tuser,&tnum,tname,&tp,&io,&tprice)!=EOF)
    {
        ttime[10]=' ';
        ttime[11]='\0';
        strcat(ttime,t);
        p=new struct Record;
        q=user->recordlist;
        while(q->NextRecord)
        {
            q=q->NextRecord;
        }
        q->NextRecord=p;
        strcpy(p->time,ttime);
        strcpy(p->User,tuser);
        p->tradenum=tnum;
        strcpy(p->tradename,tname);
        p->num=tp;
        p->IO=io;
        p->price=tprice;
        p->NextRecord=NULL;
    }
    fclose(fr);
}

void tfilein()//商品信息写入文件
{
    char filename[100];
    strcpy(filename,user->Username);
    strcat(filename,"tradedata.txt");
    fw=fopen(filename,"w");
    char tname[20],ttype[20],tpro[20],tprotime[30],tintime[30],touttime[30];
    int tnum,tpush,tpop,tstore,tswarn,tbztime;
    double tprice,tbprice,tprofit;
    struct commodity *q;
    q=user->tradelist->next;
    while(q)
    {
        strcpy(tname,q->tradename);
        strcpy(ttype,q->tradetype);
        strcpy(tpro,q->tradepro);
        tnum=q->number;
        tprice=q->price;
        tbprice=q->boughtprice;
        tprofit=q->profit;
        tpush=q->push;
        tpop=q->pop;
        tstore=q->store;
        tswarn=q->swarn;
        strcpy(tprotime,q->producetime);
        strcpy(tintime,q->intime);
        strcpy(touttime,q->outtime);
        tbztime=q->bztime;
        fprintf(fw,"%s %s %s %d %f %f %f %d %d %d %d %s %d %s %s\n",tname,ttype,tpro,tnum,tprice,tbprice,tprofit,tpush,tpop,tstore,tswarn,tprotime,tbztime,tintime,touttime);
        q=q->next;
    }
    fclose(fw);
}

void tfileout()//商品信息读出文件
{
    char filename[100];
    strcpy(filename,user->Username);
    strcat(filename,"tradedata.txt");
    fr=fopen(filename,"r");
    char tname[20],ttype[20],tpro[20],tprotime[30],tintime[30],touttime[30];
    int tnum,tpush,tpop,tstore,tswarn,tbztime;
    double tprice,tbprice,tprofit;
    struct commodity *p;
    while(fscanf(fr,"%s%s%s%d%lf%lf%lf%d%d%d%d%s%d%s%s",tname,ttype,tpro,&tnum,&tprice,&tbprice,&tprofit,&tpush,&tpop,&tstore,&tswarn,tprotime,&tbztime,tintime,touttime)!=EOF)
    {
        p=new struct commodity;
        p->next=user->tradelist->next;
        user->tradelist->next=p;
        p->number=tnum;
        strcpy(p->tradename,tname);
        strcpy(p->tradetype,ttype);
        strcpy(p->tradepro,tpro);
        p->price=tprice;
        p->boughtprice=tbprice;
        p->profit=tprofit;
        p->push=tpush;
        p->pop=tpop;
        p->store=tstore;
        p->swarn=tswarn;
        strcpy(p->producetime,tprotime);
        strcpy(p->intime,tintime);
        strcpy(p->outtime,touttime);
        p->bztime=tbztime;
    }
    fclose(fr);
}

void choice_UI()//选择菜单
{
    system("cls");
    printf("\n");
    printf("\n");
    printf("                                                   %s,欢迎您！\n",user->Username);
    printf("\n");
    printf("\n");
    printf("请选择所要执行的操作：\n");
    printf("1、录入商品信息\n");
    printf("2、修改商品信息\n");
    printf("3、删除商品信息\n");
    printf("4、录入进货和销售信息\n");
    printf("5、查看商品信息\n");
    printf("6、查看商品销售榜单\n");
    printf("7、查看库存不足商品\n");
    printf("8、查看进货与销售记录（最近10条）\n");
    printf("请选择1~8进行操作！^输入“0”退出^\n");
}

void getrecord()//查看记录
{
    system("cls");
    int flag=0;
    struct Record *q;
    q=user->recordlist->NextRecord;
    for(int i=0; i<10; i++)
    {
        if(q)
        {
            printf("%s %s ",q->time,q->User);
            if(q->IO==1) printf("销售");
            else printf("进货");
            printf(" %d号商品 %s %d | 其销售价格（单价：元）为：%.2f\n",q->tradenum,q->tradename,q->num,q->price);
            q=q->NextRecord;
            flag=1;
        }
        else if(!flag)
        {
            printf("暂无记录！\n");
            break;
        }
        else break;
    }
}

void push_trade()//录入商品信息
{
    system("cls");
    char tname[20];
    char ttype[20];
    char tpro[20];
    char tprotime[30];
    int tbztime;
    int tnum;
    double tprice,tbprice,tprofit;
    int tpush;
    int tpop;
    int tstore;
    int tswarn;
    struct commodity *p,*q;
    char flag='1';
    while(flag!='0')
    {
        p=new struct commodity;
        printf("请输入商品信息：\n");
        printf("商品编号：");
        scanf("%d",&tnum);
        int k=1;
        while(k)
        {
            q=user->tradelist->next;
            while(q)
            {
                if(q->number==tnum)
                {
                    printf("商品编号已存在，请重新输入：");
                    scanf("%d",&tnum);
                    break;
                }
                q=q->next;
            }
            if(!q) k=0;
        }
        printf("商品名称：");
        scanf("%s",tname);
        printf("商品类型：");
        scanf("%s",ttype);
        printf("商品产地：");
        scanf("%s",tpro);
        printf("商品销售价格(单价：元)：");
        scanf("%lf",&tprice);
        printf("商品进货价格(单价：元)：");
        scanf("%lf",&tbprice);
        tprofit=tprice-tbprice;
        printf("商品生产日期(Y-M-D)：");
        scanf("%s",tprotime);
        printf("商品保质期(单位：月)：");
        scanf("%d",&tbztime);
        printf("进货量：");
        scanf("%d",&tpush);
        printf("销售量：");
        scanf("%d",&tpop);
        tstore=tpush-tpop;
        printf("库存预警值：");
        scanf("%d",&tswarn);
        p->next=user->tradelist->next;
        user->tradelist->next=p;
        p->number=tnum;
        strcpy(p->tradename,tname);
        strcpy(p->tradetype,ttype);
        strcpy(p->tradepro,tpro);
        p->price=tprice;
        p->boughtprice=tbprice;
        p->profit=tprofit;
        p->push=tpush;
        p->pop=tpop;
        p->store=tstore;
        p->swarn=tswarn;
        strcpy(p->producetime,tprotime);
        p->intime[0]='0';
        p->intime[1]='\0';
        p->outtime[0]='0';
        p->outtime[1]='\0';
        p->bztime=tbztime;
        printf("按任意键继续录入新的商品信息...\n");
        printf("按“0”结束录入商品信息！\n");
        flag=getch();
    }
}

void new_trade()//更新商品信息
{
    system("cls");
    int tnum,tpush,tpop,tstore,tswarn,tbztime;
    char tprotime[30],tintime[30],touttime[30];
    double tprice;
    char ok='1';
    while(ok!='0')
    {
        int flag=0;
        char choice;
        struct commodity *q;
        printf("请输入商品编号：");
        scanf("%d",&tnum);
        q=user->tradelist->next;
        while(q)
        {
            if(q->number==tnum)
            {
                printf("您选择商品为：%d号商品 %s\n",q->number,q->tradename);
                printf("请选择需要修改的商品信息（请在1~9中选择,按“0”取消修改）：\n");
                printf("1、商品销售价格(单价：元)\n");
                printf("2、进货量\n");
                printf("3、销售量\n");
                printf("4、库存量\n");
                printf("5、库存预警值\n");
                printf("6、生产日期\n");
                printf("7、保质期(单位：月)\n");
                printf("8、最后进货时间\n");
                printf("9、最后销售时间\n");
                choice=getch();
                while(choice!='0')
                {
                    switch(choice)
                    {
                    case '1':
                        printf("请输入更新后该商品销售价格（单价：元）：");
                        scanf("%lf",&tprice);
                        q->price=tprice;
                        break;
                    case '2':
                        printf("请输入更新后该商品进货量：");
                        scanf("%d",&tpush);
                        q->push=tpush;
                        break;
                    case '3':
                        printf("请输入更新后该商品销售量：");
                        scanf("%d",&tpop);
                        q->pop=tpop;
                        break;
                    case '4':
                        printf("请输入更新后该商品库存量：");
                        scanf("%d",&tstore);
                        q->store=tstore;
                        break;
                    case '5':
                        printf("请输入更新后该商品库存预警值：");
                        scanf("%d",&tswarn);
                        q->swarn=tswarn;
                        break;
                    case '6':
                        printf("请输入更新后该商品生产日期(Y-M-D)：");
                        scanf("%s",tprotime);
                        strcpy(q->producetime,tprotime);
                        break;
                    case '7':
                        printf("请输入更新后该商品保质期(单位：月)：");
                        scanf("%d",&tbztime);
                        q->bztime=tbztime;
                        break;
                    case '8':
                        printf("请输入更新后该商品最后进货时间(Y-M-D)：");
                        scanf("%s",tintime);
                        strcpy(q->intime,tintime);
                        break;
                    case '9':
                        printf("请输入更新后该商品最后销售时间(Y-M-D)：");
                        scanf("%s",touttime);
                        strcpy(q->outtime,touttime);
                        break;
                    default:
                        printf("输入有误！\n");
                        break;
                    }
                    printf("如需继续修改该商品信息，请在“1”~“9”中选择，按“0”退出修改该商品信息\n");
                    choice=getch();
                }
                flag=1;
            }
            if(flag) break;
            q=q->next;
        }
        if(!flag) printf("商品编号不存在！\n");
        printf("按任意键继续选择需要修改信息的商品...\n");
        printf("按“0”退出修改商品信息！\n");
        ok=getch();
    }
}

void del_trade()//删除商品信息
{
    system("cls");
    int tnum;
    int flag;
    struct commodity *q,*p;
    char choice='1';
    while(choice!='0')
    {
        flag=0;
        printf("请输入要删除的商品编号：");
        scanf("%d",&tnum);
        p=user->tradelist;
        q=p->next;
        while(q)
        {
            if(q->number==tnum)
            {
                p->next=q->next;
                q->next=NULL;
                printf("您选择商品为：%d号商品 %s\n",q->number,q->tradename);
                printf("商品信息已删除！\n");
                flag=1;
                break;
            }
            p=q;
            q=q->next;
        }
        if(!flag) printf("商品编号不存在！\n");
        printf("按任意键继续删除，按“0”退出删除！\n");
        choice=getch();
    }
}

void push_pop()//进货及销售信息
{
    system("cls");
    int tnum,tpush,tpop;
    char ok='1';
    while(ok!='0')
    {
        int flag=0;
        char choice;
        struct commodity *q;
        printf("请输入商品编号：");
        scanf("%d",&tnum);
        q=user->tradelist->next;
        while(q)
        {
            if(q->number==tnum)
            {
                printf("您选择商品为：%d号商品 %s\n",q->number,q->tradename);
                printf("按“1”录入进货信息，按“2”录入销售信息，按“0”取消操作\n");
                choice=getch();
                while(choice!='0')
                {
                    switch(choice)
                    {
                    case '1':
                        printf("请输入进货商品数量：");
                        scanf("%d",&tpush);
                        q->push+=tpush;
                        q->store+=tpush;
                        strncpy(q->intime,getCurrentTimeStr(),10);
                        dorecord(tnum,q->tradename,tpush,0,q->price);
                        break;
                    case '2':
                        printf("请输入销售商品数量：");
                        scanf("%d",&tpop);
                        q->pop+=tpop;
                        q->store-=tpop;
                        strncpy(q->outtime,getCurrentTimeStr(),10);
                        dorecord(tnum,q->tradename,tpop,1,q->price);
                        break;
                    default:
                        printf("输入有误！\n");
                        break;
                    }
                    printf("如需继续录入该商品信息，请在“1”“2”中选择，按“0”退出修改该商品信息\n");
                    choice=getch();
                }
                flag=1;
            }
            if(flag) break;
            q=q->next;
        }
        if(!flag) printf("商品编号不存在！\n");
        printf("按任意键继续选择需要录入信息的商品...\n");
        printf("按“0”退出录入商品进销货信息！\n");
        ok=getch();
    }
}

void get_trade()//查看商品信息
{
    system("cls");
    int tnum;
    char ok='1';
    while(ok!='0')
    {
        int flag=0;
        struct commodity *q;
        printf("请输入商品编号：");
        scanf("%d",&tnum);
        q=user->tradelist->next;
        while(q)
        {
            if(q->number==tnum)
            {
                printf("该商品信息如下：\n");
                printf("商品名称：%s\n",q->tradename);
                printf("商品类型：%s\n",q->tradetype);
                printf("商品产地：%s\n",q->tradepro);
                printf("商品销售价格(单价：元)：%.2f\n",q->price);
                printf("商品进货价格(单价：元)：%.2f\n",q->boughtprice);
                printf("商品利润(单价：元)：%.2f\n",q->profit);
                printf("进货量：%d\n",q->push);
                printf("销售量：%d\n",q->pop);
                printf("库存量：%d\n",q->store);
                printf("库存预警值：%d\n",q->swarn);
                printf("生产日期：%s\n",q->producetime);
                printf("保质期(单位：月)：%d\n",q->bztime);
                if(q->intime[0]!='0') printf("最后进货时间：%s\n",q->intime);
                if(q->outtime[0]!='0') printf("最后销售时间：%s\n",q->outtime);
                flag=1;
            }
            if(flag) break;
            q=q->next;
        }
        if(!flag) printf("商品编号不存在！\n");
        printf("按任意键继续查看商品信息...\n");
        printf("按“0”退出查看商品信息！\n");
        ok=getch();
    }
}

void get_top()//查看商品销售榜单
{
    struct commodity *first,*second,*third,*q;
    first=NULL;
    second=NULL;
    third=NULL;
    q=user->tradelist->next;
    while(q)
    {
        if(!first||first->pop<q->pop)
        {
            third=second;
            second=first;
            first=q;
        }
        else if(!second||second->pop<q->pop)
        {
            third=second;
            second=q;
        }
        else if(!third||third->pop<q->pop)
        {
            third=q;
        }
        q=q->next;
    }
    system("cls");
    printf("当前商品销售榜单为：\n");
    if(first) printf("top1: %d号商品 %s 销售量：%d\n",first->number,first->tradename,first->pop);
    else printf("暂无商品销售榜单，快去买东西吧~\n");
    if(second) printf("top2: %d号商品 %s 销售量：%d\n",second->number,second->tradename,second->pop);
    if(third) printf("top3: %d号商品 %s 销售量：%d\n",third->number,third->tradename,third->pop);
}

void warning()//库存不足警告
{
    struct commodity *q;
    int tnum,tstore,tswarn;
    char tname[20];
    q=user->tradelist->next;
    int flag=0;
    while(q)
    {
        tstore=q->store;
        tswarn=q->swarn;
        if(tstore<tswarn)
        {
            if(!flag)
            {
                system("cls");
                flag=1;
            }
            tnum=q->number;
            strcpy(tname,q->tradename);
            printf("%d号商品 %s库存不足，请及时补货！\n",tnum,tname);
        }
        q=q->next;
    }
    if(!flag)
    {
        system("cls");
        printf("暂无库存不足商品！\n");
    }
}

void End_UI()//结束界面
{
    system("cls");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                                                   感谢您的使用~                                                         \n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

}

int inter_face()//初始化界面
{
    char go;
    system("cls");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                                              ________________________\n");
    printf("                                              *欢迎使用商品进销存程序*                                                   \n");
    printf("                                              ________________________\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                                                       0、结束                                                          \n");
    printf("                                                       1、注册                                                          \n");
    printf("                                                       2、登录                                                          \n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t->");
    while(1)
    {
        go=getch();
        if(go=='0') return 0;
        else if(go=='1') return 1;
        else if(go=='2') return 2;
        else continue;
    }
}

int userin()//用户登录
{
    system("cls");
    char username[100],filename[100],password[100],okpassword[100];
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                    请输入用户名(输入“tc”结束程序)：");
    while(1)
    {
        scanf("%s",username);
        if(strcmp(username,"tc")==0) return 0;
        strcpy(filename,username);
        strcat(filename,".txt");
        if((fr=fopen(filename,"r"))!=NULL)
        {
            strcpy(user->Username,username);
            fscanf(fr,"%s",okpassword);
            printf("                    请输入密码(输入“tc”结束程序)：");
            while(1)
            {
                scanf("%s",password);
                if(strcmp(password,"tc")==0) return 0;
                if(strcmp(password,okpassword)==0) return 1;
                else printf("                    密码错误！请重新输入(输入“tc”结束程序)：");
            }
        }
        else printf("                    用户名不存在！请重新输入(输入“tc”结束程序)：");
    }
}

void newuser()//注册新用户
{
    system("cls");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    char username[100],filename[100];
    char pw1[100],pw2[100];
    char ch;
    int i,len;
    int f1=0,f2=0,f3=0;
    printf("                    请输入用户名(用户名不能包含空字符)：");
    while(!f1)
    {
        i=0;
        while((ch=getchar())!='\n')
        {
            username[i]=ch;
            i++;
        }
        username[i]='\0';
        len=strlen(username);
        for(i=0; i<len; i++)
        {
            if(username[i]==' '||username[i]=='\t')
            {
                printf("                    输入有误！请重新输入(用户名不能包含空字符)：");
                break;
            }
        }
        if(i==len) f1=1;
        strcpy(filename,username);
        strcat(filename,".txt");
        if((fr=fopen(filename,"r"))!=NULL)
        {
            printf("                    账号已存在！请重新输入(用户名不能包含空字符)：");
            f1=0;
        }
    }
    printf("                    请输入密码(密码应为8~20个字符)：");
    while(!f2)
    {
        i=0;
        while((ch=getch())!='\r')
        {
            if(ch=='\b')
            {
                if(i>0) printf("\b \b",i--);
                else continue;
            }
            else
            {
                printf("*");
                pw1[i]=ch;
                i++;
            }
        }
        pw1[i]='\0';
        if(i<8||i>20)
        {
            printf("\n                    输入有误！请重新输入密码(密码应为8~20个字符)：");
            continue;
        }
        printf("\n                    请再次输入密码：");
        while(!f3)
        {
            i=0;
            while((ch=getch())!='\r')
            {
                if(ch=='\b')
                {
                    if(i>0) printf("\b \b",i--);
                    else continue;
                }
                else
                {
                    printf("*");
                    pw2[i]=ch;
                    i++;
                }
            }
            pw2[i]='\0';
            if(strcmp(pw1,pw2)==0)
            {
                f2=1;
                f3=1;
            }
            else
            {
                printf("\n                    两次输入密码不相同！请重新设置密码(密码应为8~20个字符)：");
                break;
            }
        }
    }
    system("cls");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n                    设置成功！是否注册(请按“y”和“n”进行选择)？");
    while(1)
    {
        ch=getch();
        if(ch=='y')
        {
            fw=fopen(filename,"w");
            fprintf(fw,"%s",pw1);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n                                                    注册成功！\n");
            printf("\n                                                  按任意键返回...");
            ch=getch();
            break;
        }
        else if(ch=='n')
        {
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n                                                   已取消注册！\n");
            printf("\n                                                 按任意键返回...");
            ch=getch();
            break;
        }
        else continue;
    }
}

int main()//主程序
{
    user=new struct User;
    user->tradelist=new struct commodity;
    user->recordlist=new struct Record;
    user->tradelist->next=NULL;
    user->recordlist->NextRecord=NULL;
    while(1)
    {
        int flag=inter_face();
        if(!flag)
        {
            End_UI();
            return 0;
        }
        else
        {
            if(flag==1)
            {
                newuser();
            }
            else
            {
                if(!userin())
                {
                    End_UI();
                    return 0;
                }
                else break;
            }
        }
    }
    tfileout();
    rfileout();
    char random='1';
    char choice;
    while(random!='0')
    {
        choice_UI();
        choice=getch();
        switch(choice)
        {
        case '1':
            push_trade();
            break;
        case '2':
            new_trade();
            break;
        case '3':
            del_trade();
            break;
        case '4':
            push_pop();
            break;
        case '5':
            get_trade();
            break;
        case '6':
            get_top();
            break;
        case '7':
            warning();
            break;
        case '8':
            getrecord();
            break;
        default:
            break;
        }
        if(!(choice>='0'&&choice<='9')) continue;
        if(choice=='0') break;
        printf("\n按任意键返回菜单，输入“0”结束程序\n");
        random=getch();
    }
    tfilein();
    rfilein();
    End_UI();
    return 0;
}
