# OS-Project: Multi-Level Feedback Queue (MLFQ) CPU Scheduler

โปรแกรมจำลองการทำงานของระบบจัดคิวซีพียูแบบ Multi-Level Feedback Queue (MLFQ)
จัดทำขึ้นเพื่อศึกษาแนวคิดเกี่ยวกับการจัดคิวประมวลผลและการบริหารจัดการทรัพยากร CPU ในระบบปฏิบัติการ (Operating System)

## เทคโนโลยีที่ใช้

* C Language
* GCC (GNU Compiler Collection)
* Linux / WSL (Windows Subsystem for Linux)
* Git

## สิ่งที่ต้องติดตั้งก่อนใช้งาน

ก่อนเริ่มใช้งานต้องติดตั้งโปรแกรมต่อไปนี้ในเครื่อง:

* GCC Compiler
* Git

ตรวจสอบว่าติดตั้งเรียบร้อยแล้วด้วยคำสั่ง:

```bash
gcc --version
git --version
```

หากใช้ Ubuntu หรือ WSL แล้วยังไม่มีเครื่องมือดังกล่าว สามารถติดตั้งได้ด้วยคำสั่ง:

```bash
sudo apt update
sudo apt install build-essential git
```

## วิธี Clone และรันโปรเจกต์

```bash
# Clone โปรเจกต์
git clone [https://github.com/ZAG1A/OS-Project.git](https://github.com/ZAG1A/OS-Project.git)

# เข้าสู่โฟลเดอร์โปรเจกต์
cd OS-Project

# คอมไพล์โปรแกรม
gcc mlfq.c -o mlfq

# รันโปรแกรม
./mlfq
```

## โครงสร้างโปรเจกต์

```text
OS-Project/
├── mlfq.c        # ซอร์สโค้ดภาษา C ระบบจำลอง MLFQ
└── README.md     # เอกสารอธิบายรายละเอียดโปรเจกต์
```

## คุณสมบัติและการทำงานของอัลกอริทึม

* **โครงสร้างคิว 3 ระดับ (3-Tier Queue):**
  * **Queue 1:** Round Robin (RR) กำหนดค่า Time Quantum = 2
  * **Queue 2:** Round Robin (RR) กำหนดค่า Time Quantum = 4
  * **Queue 3:** First-Come First-Served (FCFS) สำหรับงานประมวลผลทั่วไป
* **การลดระดับความสำคัญ (Dynamic Demotion):** ปรับลดระดับ Process ที่ใช้เวลาในคิวปัจจุบันจนครบโควตา (Time Quantum) ลงไปคิวระดับล่าง
* **การป้องกันการอดตาย (Aging Mechanism / Starvation Prevention):** ปรับระดับความสำคัญของ Process ทั้งหมดที่ยังทำงานไม่เสร็จ กลับขึ้นไปยัง Queue 1 ทุกๆ 20 หน่วยเวลา
* **การประเมินประสิทธิภาพ (Performance Evaluation):** คำนวณและแสดงผลค่า Completion Time, Turnaround Time และ Waiting Time ของแต่ละ Process พร้อมสรุปค่าเฉลี่ยของระบบ

## ตัวอย่างผลลัพธ์การทำงาน

```text
================ Summary Report ================
Process Arrival Burst   Completion      Turnaround      Waiting
P1      0       8       20              20              12
P2      1       4       14              13              9
P3      2       9       23              21              12
P4      4       2       8               4               2
------------------------------------------------
Average Waiting Time: 8.75
Average Turnaround Time: 14.50
================================================
```