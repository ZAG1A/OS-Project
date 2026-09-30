#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESS 4
#define QUANTUM_Q1 2
#define QUANTUM_Q2 4
#define AGING_TIME 20  // Aging: Move all active processes back to Queue 1 every 20 time units

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int queue_level;     // Queue level: 1, 2, or 3
    int completion_time;
    int waiting_time;
    int turnaround_time;
    bool is_completed;
} Process;

int main() {
    // Sample test processes: {id, arrival, burst, remaining, queue, completion, wait, turnaround, is_completed}
    Process p[MAX_PROCESS] = {
        {1, 0, 8, 8, 1, 0, 0, 0, false},
        {2, 1, 4, 4, 1, 0, 0, 0, false},
        {3, 2, 9, 9, 1, 0, 0, 0, false},
        {4, 4, 2, 2, 1, 0, 0, 0, false}
    };

    int current_time = 0;
    int completed_processes = 0;
    int current_quantum = 0;
    int active_process = -1;

    printf("=== MLFQ CPU Scheduler Simulation Started ===\n\n");

    while (completed_processes < MAX_PROCESS) {
        // 1. AGING SYSTEM (Prevent Starvation)
        if (current_time > 0 && current_time % AGING_TIME == 0) {
            printf("[Time %2d] *** AGING BOOST: Moving all active processes back to Queue 1 ***\n", current_time);
            for (int i = 0; i < MAX_PROCESS; i++) {
                if (!p[i].is_completed && p[i].arrival_time <= current_time) {
                    p[i].queue_level = 1;
                }
            }
        }

        // 2. PRIORITY DISPATCHER (Queue 1 -> Queue 2 -> Queue 3)
        int selected = -1;
        for (int q = 1; q <= 3; q++) {
            for (int i = 0; i < MAX_PROCESS; i++) {
                if (!p[i].is_completed && p[i].arrival_time <= current_time && p[i].queue_level == q) {
                    selected = i;
                    break;
                }
            }
            if (selected != -1) break;
        }

        // If no process is ready, CPU enters Idle state
        if (selected == -1) {
            printf("[Time %2d] CPU Idle...\n", current_time);
            current_time++;
            continue;
        }

        // Reset quantum if CPU switches to another process
        if (active_process != selected) {
            active_process = selected;
            current_quantum = 0;
        }

        // 3. CPU EXECUTION (1 Time Unit)
        p[selected].remaining_time--;
        current_quantum++;
        
        printf("[Time %2d] Running Process P%d (Remaining: %d) [In Queue %d]\n", 
               current_time, p[selected].id, p[selected].remaining_time, p[selected].queue_level);

        current_time++;

        // 4. CHECK COMPLETION STATUS
        if (p[selected].remaining_time == 0) {
            p[selected].is_completed = true;
            p[selected].completion_time = current_time;
            p[selected].turnaround_time = p[selected].completion_time - p[selected].arrival_time;
            p[selected].waiting_time = p[selected].turnaround_time - p[selected].burst_time;
            
            completed_processes++;
            printf("[Time %2d] Process P%d FINISHED!\n", current_time, p[selected].id);
            active_process = -1;
            current_quantum = 0;
        } 
        // 5. CHECK DEMOTION (Lower queue level when quantum expires)
        else {
            if (p[selected].queue_level == 1 && current_quantum >= QUANTUM_Q1) {
                p[selected].queue_level = 2;
                printf("[Time %2d] Process P%d expired Quantum Q1 -> Demoted to Queue 2\n", current_time, p[selected].id);
                active_process = -1;
                current_quantum = 0;
            } 
            else if (p[selected].queue_level == 2 && current_quantum >= QUANTUM_Q2) {
                p[selected].queue_level = 3;
                printf("[Time %2d] Process P%d expired Quantum Q2 -> Demoted to Queue 3\n", current_time, p[selected].id);
                active_process = -1;
                current_quantum = 0;
            }
        }
    }

    // 6. SUMMARY REPORT
    printf("\n================ Summary Report ================\n");
    printf("Process\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    float total_wait = 0, total_turnaround = 0;
    
    for (int i = 0; i < MAX_PROCESS; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n", 
               p[i].id, p[i].arrival_time, p[i].burst_time, 
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
        
        total_wait += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;
    }

    printf("------------------------------------------------\n");
    printf("Average Waiting Time: %.2f\n", total_wait / MAX_PROCESS);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / MAX_PROCESS);
    printf("================================================\n");

    return 0;
}
