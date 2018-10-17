#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define INVALID -1
#define null 0

#define total_instruction 320
#define total_vp 32
#define clear_period 50

// 页面类型
typedef struct {
    int pn;         // 页号
    int pfn;        // 面号
    int counter;    // 一个周期内访问该页面次数
    int time;       // 访问时间
} pl_type;

pl_type pl[total_vp];

// 页面控制结构
struct pfc_struct {
    int pn, pfn;
    struct pfc_struct *next;
};
typedef struct pfc_struct pfc_type;
pfc_type pfc[total_vp]; // 定义用户进程虚页控制结构
pfc_type *freepf_head;  // 空页面头的指针
pfc_type *busypf_head;  // 忙页面头的指针
pfc_type *busypf_tail;  // 忙页面尾的指针

int initialize (int total_pf);  // 初始化函数，给每个相关的页面赋值
void FIFO (int total_pf);       // 计算使用FIFO算法时的命中率
void LRU (int total_pf);

int a[total_instruction];       // 指令数据组
int page[total_instruction];    // 每条指令所属页号
int offset[total_instruction];  // 每页装入10条指令后取模运算页号偏移值
int total_pf;                   // 用户进程的内存页面数
int disaffect;                  // 页面失效次数

int main () {
    int S, i, j;
    srand(getpid()*10);
    S = (float)319*rand()/32767 + 1;
    for (i = 0; i < total_instruction; i += 2) {
        a[i] = S;
        a[i + 1] = a[i] + 1;
        a[i + 2] = (float)a[i]*rand()/32767;
        a[i + 3] = a[i + 2] + 1;
        S = (float)rand()*(318 - a[i + 2]/32767) + a[i + 2] + 2;
    }
    for (i = 0; i < total_instruction; i++) {
        page[i] = a[i]/10;
        offset[i] = a[i]%10;
    }
    for (i = 4; i < 32; i++) {
        printf("%2d page frames", i);
        FIFO(i);
        LRU(i);
        printf("\n");
    }
    return 0;
}

void FIFO(int total_pf) {
    int i, j;
    pfc_type *p, *t;
    initialize(total_pf);
    busypf_head = busypf_tail = NULL;
    for (i = 0; i < total_instruction; i++) {
        if (pl[page[i]].pfn == INVALID) {
            disaffect += 1;
            if (freepf_head == NULL) {
                p = busypf_head->next;
                pl[busypf_head->pn].pfn = INVALID;
                freepf_head->next = NULL;
                busypf_head = p;
            }
            p = freepf_head->next;
            freepf_head->next = NULL;
            freepf_head->pn = page[i];
            pl[page[i]].pfn = freepf_head->pfn;
            if (busypf_tail == NULL) {
                busypf_head = busypf_tail = freepf_head;
            } else {
                busypf_tail->next = freepf_head;
            }
            busypf_tail = freepf_head;
        }
        freepf_head = p;
        printf("FIFO:%6.4f", 1-(float)disaffect/320);
    }
}
