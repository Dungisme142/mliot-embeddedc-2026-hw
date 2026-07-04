// HW02: KIẾN TRÚC PHẦN MỀM VÀ QUẢN LÝ BỘ NHỚ SMART BIKE

#include <stdio.h>
#include <stdint.h>

// TASK 1: STRUCT, UNION & BIT-FIELDS

typedef union {
    uint16_t raw_value;
    struct {
        // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
        uint16_t PWR_ON : 1; // PWR_ON lấy 1 bit là [0]
        uint16_t ASSIST_LEVEL : 2; // ASSIST_LEVEL lấy 2 bit là [2:1]
        uint16_t LIGHT_BRIGHT : 4; // LIGHT_BRIGHT lấy 4 bit là [6:3]
        uint16_t RESERVED : 9; // RESERVED lấy 9 bit là [15:7]
        // HỌC VIÊN KẾT THÚC VIẾT CODE
    } fields;
} Bike_Status_t;


// TASK 2: FUNCTION POINTERS & TABLE-DRIVEN ARCHITECTURE

void drive_eco(void) {
    printf("Bike Mode: ECO.\n");
}
void drive_normal(void) {
    printf("Bike Mode: NORMAL.\n");
}
void drive_sport(void) {
    printf("Bike Mode: SPORT.\n");
}

// HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
typedef void (*drive_modes)(void);
static const drive_modes drive_table[] = {
    drive_eco,
    drive_normal,
    drive_sport
};

void choose_drive (uint8_t drive_index){
    // chặn driver_index không vượt quá 3
    // chọn chế độ dựa trên hàm con trỏ
    if(drive_index < 3){
        drive_table[drive_index]();
    }
}

// HỌC VIÊN KẾT THÚC VIẾT CODE


// TASK 3: ARCHITECTURE CALLBACKS

void Battery_Monitor(void (*overheat_cb)(void)) {
    int battery_temp = 45; 
    
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    if(overheat_cb == NULL) return;
    // nếu nhiệt độ hơn 40 thì callback overheat_cb()
    // overheat_cb() sẽ được gán là Critical_Battery_Handler
    if( battery_temp > 40){
        overheat_cb();
    }
    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

void Critical_Battery_Handler(void) {
    printf("[WARNING] Battery is overheated!\n");
}


// TASK 4: MEMORY MAP & CRASH LAB

const char BIKE_MODEL[] = "E-Bike X2026"; 
uint32_t total_odometer = 0;             

void crash_simulation(void) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    // in luôn ra màn hình Crash simulation
    char buffer[1024];

    printf("Buffer Address: %p\n", (void *)buffer);

    crash_simulation();

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}


// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm)

int main() {
    Bike_Status_t my_bike;
    
    // 1. Test Task 1
    my_bike.raw_value = 0x0025; 
    printf("REGISTER STRUCTURE: \n");
    printf("PWR_ON: %d | ASSIST_LEVEL: %d | LIGHT_BRIGHT: %d\n\n", 
           my_bike.fields.PWR_ON, my_bike.fields.ASSIST_LEVEL, my_bike.fields.LIGHT_BRIGHT);

    // 2. Test Task 2
    printf("ENGINE CONTROLLING: \n");
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    choose_drive(my_bike.fields.ASSIST_LEVEL);
    // HỌC VIÊN KẾT THÚC VIẾT CODE

    // 3. Test Task 3
    Battery_Monitor(Critical_Battery_Handler);
    printf("\n");

    // 4. Test Task 4
    int current_speed = 25; 
    printf("MEMORY ADDRESSES OF VARIABLES \n");
    printf("BIKE_MODEL (ROM/.rodata):   %p\n", (void*)BIKE_MODEL);
    printf("total_odometer (RAM/.bss):  %p\n", (void*)&total_odometer);
    printf("current_speed (RAM/Stack):  %p\n", (void*)&current_speed);

    // Bỏ comment dòng dưới để chạy thử bài Crash Lab
    //crash_simulation();

    return 0;
}