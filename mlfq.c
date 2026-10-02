#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100
#define MAX_NAME_LEN 16
#define MAX_TIMELINE 1000

typedef struct {
    char name[MAX_NAME_LEN];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int current_queue; // คิวปัจจุบัน (1, 2, หรือ 3)
    int quantum_used;  // เวลาที่ใช้ไปในคิวปัจจุบัน
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
} Process;

// โครงสร้างคิวแบบ FIFO / Round Robin
typedef struct {
    int items[MAX_PROCESSES];
    int front;
    int rear;
    int count;
} Queue;

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isEmpty(Queue *q) {
    return q->count == 0;
}

void enqueue(Queue *q, int value) {
    if (q->count < MAX_PROCESSES) {
        q->rear = (q->rear + 1) % MAX_PROCESSES;
        q->items[q->rear] = value;
        q->count++;
    }
}

int dequeue(Queue *q) {
    if (!isEmpty(q)) {
        int item = q->items[q->front];
        q->front = (q->front + 1) % MAX_PROCESSES;
        q->count--;
        return item;
    }
    return -1;
}

int inQueue(Queue *q, int index) {
    if (isEmpty(q)) return 0;
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % MAX_PROCESSES;
        if (q->items[idx] == index) return 1;
    }
    return 0;
}

int main() {
    int num_processes;
    int q1_quantum, q2_quantum, aging_time;

    printf("==================================================\n");
    printf("   MLFQ CPU SCHEDULER SIMULATOR (DYNAMIC INPUT)   \n");
    printf("==================================================\n\n");

    // 1. รับค่า Parameter ของระบบ
    printf("Enter number of processes: ");
    if (scanf("%d", &num_processes) != 1 || num_processes <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    printf("Enter Time Quantum for Queue 1 (e.g., 2): ");
    scanf("%d", &q1_quantum);

    printf("Enter Time Quantum for Queue 2 (e.g., 4): ");
    scanf("%d", &q2_quantum);

    printf("Enter Aging Interval Time (e.g., 20): ");
    scanf("%d", &aging_time);

    Process procs[MAX_PROCESSES];

    // 2. รับข้อมูล Process ทีละตัว
    printf("\n--- Enter Process Details ---\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %d (Name Arrival Burst) [e.g. P%d %d %d]: ", i + 1, i + 1, i, (i + 1) * 3);
        scanf("%s %d %d", procs[i].name, &procs[i].arrival_time, &procs[i].burst_time);
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].current_queue = 1;
        procs[i].quantum_used = 0;
        procs[i].completion_time = 0;
        procs[i].turnaround_time = 0;
        procs[i].waiting_time = 0;
        procs[i].is_completed = 0;
    }

    printf("\n==================================================\n");
    printf("          MLFQ CPU Scheduler Simulation           \n");
    printf("==================================================\n");

    Queue q1, q2, q3;
    initQueue(&q1);
    initQueue(&q2);
    initQueue(&q3);

    int time = 0;
    int completed = 0;
    int current_idx = -1;

    char timeline_proc[MAX_TIMELINE][MAX_NAME_LEN];
    int timeline_len = 0;

    // 3. เริ่มจำลองการทำงาน
    while (completed < num_processes) {
        // กลไก Aging: ปรับ Process ทั้งหมดกลับ Queue 1 เมื่อถึงเวลาที่กำหนด
        if (time > 0 && time % aging_time == 0) {
            printf("[Time %2d] *** AGING BOOST: Moving all active processes back to Queue 1 ***\n", time);
            
            Queue new_q1;
            initQueue(&new_q1);

            if (current_idx != -1 && procs[current_idx].remaining_time > 0) {
                procs[current_idx].current_queue = 1;
                procs[current_idx].quantum_used = 0;
                enqueue(&new_q1, current_idx);
                current_idx = -1;
            }

            while (!isEmpty(&q1)) {
                int idx = dequeue(&q1);
                procs[idx].quantum_used = 0;
                enqueue(&new_q1, idx);
            }
            while (!isEmpty(&q2)) {
                int idx = dequeue(&q2);
                procs[idx].current_queue = 1;
                procs[idx].quantum_used = 0;
                enqueue(&new_q1, idx);
            }
            while (!isEmpty(&q3)) {
                int idx = dequeue(&q3);
                procs[idx].current_queue = 1;
                procs[idx].quantum_used = 0;
                enqueue(&new_q1, idx);
            }

            q1 = new_q1;
            initQueue(&q2);
            initQueue(&q3);
        }

        // เช็ค Process ใหม่ที่เพิ่งเข้ามา ณ เวลาปัจจุบัน
        for (int i = 0; i < num_processes; i++) {
            if (procs[i].arrival_time == time && !procs[i].is_completed && i != current_idx) {
                if (!inQueue(&q1, i) && !inQueue(&q2, i) && !inQueue(&q3, i)) {
                    enqueue(&q1, i);
                }
            }
        }

        // ดึง Process จากคิวลำดับความสำคัญสูงสุด
        if (current_idx == -1) {
            if (!isEmpty(&q1)) {
                current_idx = dequeue(&q1);
            } else if (!isEmpty(&q2)) {
                current_idx = dequeue(&q2);
            } else if (!isEmpty(&q3)) {
                current_idx = dequeue(&q3);
            }
        }

        // กรณีไม่มี Process ในคิวเลย (CPU Idle)
        if (current_idx == -1) {
            printf("[Time %2d] CPU Idle\n", time);
            if (timeline_len < MAX_TIMELINE) {
                strcpy(timeline_proc[timeline_len++], "IDLE");
            }
            time++;
            continue;
        }

        // ประมวลผล Process ปัจจุบัน
        printf("[Time %2d] Running Process %s (Remaining: %d) [In Queue %d]\n",
               time, procs[current_idx].name, procs[current_idx].remaining_time, procs[current_idx].current_queue);

        if (timeline_len < MAX_TIMELINE) {
            strcpy(timeline_proc[timeline_len++], procs[current_idx].name);
        }

        procs[current_idx].remaining_time--;
        procs[current_idx].quantum_used++;
        time++;

        // เช็คการเข้ามาของ Process ใหม่ ณ จังหวะสิ้นสุด 1 time tick
        for (int i = 0; i < num_processes; i++) {
            if (procs[i].arrival_time == time && !procs[i].is_completed && i != current_idx) {
                if (!inQueue(&q1, i) && !inQueue(&q2, i) && !inQueue(&q3, i)) {
                    enqueue(&q1, i);
                }
            }
        }

        // เช็คว่า Process ทำงานเสร็จหรือไม่
        if (procs[current_idx].remaining_time == 0) {
            printf("[Time %2d] Process %s FINISHED!\n", time, procs[current_idx].name);
            procs[current_idx].is_completed = 1;
            procs[current_idx].completion_time = time;
            procs[current_idx].turnaround_time = time - procs[current_idx].arrival_time;
            procs[current_idx].waiting_time = procs[current_idx].turnaround_time - procs[current_idx].burst_time;
            completed++;
            current_idx = -1;
        } else {
            // เช็คการหมด Time Quantum (Demotion)
            if (procs[current_idx].current_queue == 1 && procs[current_idx].quantum_used == q1_quantum) {
                printf("[Time %2d] Process %s expired Quantum Q1 -> Demoted to Queue 2\n", time, procs[current_idx].name);
                procs[current_idx].current_queue = 2;
                procs[current_idx].quantum_used = 0;
                enqueue(&q2, current_idx);
                current_idx = -1;
            } else if (procs[current_idx].current_queue == 2 && procs[current_idx].quantum_used == q2_quantum) {
                printf("[Time %2d] Process %s expired Quantum Q2 -> Demoted to Queue 3\n", time, procs[current_idx].name);
                procs[current_idx].current_queue = 3;
                procs[current_idx].quantum_used = 0;
                enqueue(&q3, current_idx);
                current_idx = -1;
            }
        }
    }

    // 4. แสดงผล Gantt Chart
    printf("\n==================================================\n");
    printf("                   Gantt Chart                    \n");
    printf("==================================================\n");
    printf("|");
    for (int i = 0; i < timeline_len; i++) {
        printf(" %-3s |", timeline_proc[i]);
    }
    printf("\n0");
    for (int i = 1; i <= timeline_len; i++) {
        printf("    %-2d", i); // ลด Space ตรงหน้า %-2d ลงเหลือ 4 เคาะ
    }
    printf("\n");

    // 5. แสดงตารางสรุปผลลัพธ์
    printf("\n==================================================\n");
    printf("                  Summary Report                  \n");
    printf("==================================================\n");
    printf("%-8s %-8s %-8s %-12s %-12s %-8s\n", 
           "Process", "Arrival", "Burst", "Completion", "Turnaround", "Waiting");
    
    double total_wt = 0, total_tat = 0;
    for (int i = 0; i < num_processes; i++) {
        printf("%-8s %-8d %-8d %-12d %-12d %-8d\n",
               procs[i].name, procs[i].arrival_time, procs[i].burst_time,
               procs[i].completion_time, procs[i].turnaround_time, procs[i].waiting_time);
        total_wt += procs[i].waiting_time;
        total_tat += procs[i].turnaround_time;
    }
    printf("--------------------------------------------------\n");
    printf("Average Waiting Time: %.2f\n", total_wt / num_processes);
    printf("Average Turnaround Time: %.2f\n", total_tat / num_processes);
    printf("==================================================\n");

    return 0;
}