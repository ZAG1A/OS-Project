# OS-Project: Multi-Level Feedback Queue (MLFQ) CPU Scheduler

โปรแกรมจำลองการทำงานของระบบจัดคิวซีพียูแบบ Multi-Level Feedback Queue (MLFQ) จัดทำขึ้นเพื่อศึกษาแนวคิดเกี่ยวกับการจัดคิวประมวลผลและการบริหารจัดการทรัพยากร CPU ในระบบปฏิบัติการ (Operating System) เวอร์ชันนี้รองรับการรับค่าแบบ Dynamic Input และแสดงผลลำดับการทำงานผ่าน Gantt Chart

## เทคโนโลยีที่ใช้

* C Language
* GCC (GNU Compiler Collection)
* Linux / WSL (Windows Subsystem for Linux)
* Git

## คุณสมบัติเด่นของโปรแกรม (Features)

* **Interactive Dynamic Input:** รองรับการป้อนข้อมูลจำนวน Process, ค่า Time Quantum และรายละเอียดของแต่ละ Process ผ่านทาง Terminal โดยตรง (ไม่ต้องแก้ไข Source Code)
* **Configurable Parameters:** ผู้ใช้งานสามารถกำหนดค่า Time Quantum ของ Queue 1, Queue 2 และรอบเวลาของ Aging Mechanism ได้อย่างอิสระ
* **โครงสร้างคิว 3 ระดับ (3-Tier Queue):**
  * **Queue 1:** Round Robin (RR)
  * **Queue 2:** Round Robin (RR)
  * **Queue 3:** First-Come First-Served (FCFS)
* **การลดระดับความสำคัญ (Dynamic Demotion):** ปรับลดระดับ Process ที่ใช้เวลาในคิวปัจจุบันจนครบโควตาลงไปคิวระดับล่าง
* **การป้องกันการอดตาย (Aging Mechanism):** ดึง Process ที่ยังประมวลผลไม่เสร็จทั้งหมดกลับขึ้นไปยัง Queue 1 เมื่อถึงรอบเวลาที่กำหนด เพื่อป้องกันภาวะ Starvation
* **การแสดงผล Gantt Chart:** สร้างแผนภูมิแสดงลำดับการเข้าใช้ CPU ของแต่ละ Process ตามไทม์ไลน์จริง
* **การประเมินประสิทธิภาพ (Performance Metrics):** คำนวณและแสดงผลตารางสรุปค่า Completion Time (CT), Turnaround Time (TAT), Waiting Time (WT) พร้อมค่าเฉลี่ย

## วิธีการติดตั้งและการใช้งาน

1. **Clone โปรเจกต์ และเข้าสู่โฟลเดอร์:**
   ```bash
   git clone [https://github.com/ZAG1A/OS-Project.git](https://github.com/ZAG1A/OS-Project.git)
   cd OS-Project
   ```

2. **คอมไพล์ซอร์สโค้ด:**
   ```bash
   gcc mlfq.c -o mlfq
   ```

3. **รันโปรแกรม:**
   ```bash
   ./mlfq
   ```

## ตัวอย่างการใช้งานและผลลัพธ์ (Usage Example)

เมื่อรันโปรแกรม ระบบจะให้ผู้ใช้ป้อนพารามิเตอร์และข้อมูล Process ดังนี้:

```text
Enter number of processes: 4
Enter Time Quantum for Queue 1 (e.g., 2): 2
Enter Time Quantum for Queue 2 (e.g., 4): 4
Enter Aging Interval Time (e.g., 20): 20

--- Enter Process Details ---
Process 1 (Name Arrival Burst) [e.g. P1 0 3]: p1 0 3
Process 2 (Name Arrival Burst) [e.g. P2 1 6]: p2 1 4
Process 3 (Name Arrival Burst) [e.g. P3 2 9]: p3 2 9
Process 4 (Name Arrival Burst) [e.g. P4 3 12]: p4 4 2
```

**ตัวอย่างผลลัพธ์การแสดงผล (Gantt Chart & Summary Report):**

```text
==================================================
                   Gantt Chart                    
==================================================
| p1  | p1  | p2  | p2  | p3  | p3  | p4  | p4  | p1  | p2  | p2  | p3  | p3  | p3  | p3  | p3  | p3  | p3  |
0     1      2      3      4      5      6      7      8      9      10     11     12     13     14     15     16     17     18

==================================================
                  Summary Report                  
==================================================
Process  Arrival  Burst    Completion   Turnaround   Waiting 
p1       0        3        9            9            6       
p2       1        4        11           10           6       
p3       2        9        18           16           7       
p4       4        2        8            4            2       
--------------------------------------------------
Average Waiting Time: 5.25
Average Turnaround Time: 9.75
==================================================
```