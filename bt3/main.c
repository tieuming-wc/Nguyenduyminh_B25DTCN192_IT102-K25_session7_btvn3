#include <stdio.h>
#include <math.h>

struct UnoptimizedDevice {
    char id;
    double voltage;
    char status;
    float current;
    int warningCode;
    float powerMeasured;
    float operatingTime;
};

struct OptimizedDevice {
    double voltage;
    float current;
    float powerMeasured;
    float operatingTime;
    int warningCode;
    char status;
    char id;
};

int main() {
    printf("=== SO SANH DUNG LUONG BO NHO (SIZEOF) ===\n");
    printf("Kich thuoc Struct CHUA toi uu: %lu bytes\n", sizeof(struct UnoptimizedDevice));
    printf("Kich thuoc Struct DA toi uu  : %lu bytes\n", sizeof(struct OptimizedDevice));
    printf("===========================================\n\n");

    struct OptimizedDevice dev;

    printf("=== NHAP THONG SO THIET BI ===\n");
    printf("Nhap Dien ap U (V): ");
    scanf("%lf", &dev.voltage);

    printf("Nhap Dong dien I (A): ");
    scanf("%f", &dev.current);

    printf("Nhap Cong suat do duoc P (W): ");
    scanf("%f", &dev.powerMeasured);

    printf("Nhap Thoi gian hoat dong t (phut): ");
    scanf("%f", &dev.operatingTime);

    printf("Nhap Trang thai (1 - Bat, 0 - Tat): ");
    int statusInput;
    scanf("%d", &statusInput);
    dev.status = (char)statusInput;

    if (dev.voltage <= 0 || dev.current < 0 || dev.operatingTime < 0) {
        printf("\n[LOI]: Du lieu dien ap, dong dien hoac thoi gian khong hop le!\n");
        return 1;
    }

    if (dev.status != 0 && dev.status != 1) {
        printf("\n[LOI]: Trang thai thiet bi chi nhan gia tri 0 (Tat) hoac 1 (Bat)!\n");
        return 1;
    }

    double P_calc = dev.voltage * dev.current;

    printf("\n=== KET QUA KIEM TRA AN TOAN & TIEN ICH ===\n");
    printf("Cong suat tinh toan (P_calc): %.2f W\n", P_calc);

    if (fabs(dev.powerMeasured - P_calc) > (0.05 * P_calc)) {
        printf("[CANH BAO]: Cam bien cong suat bao sai lech!\n");
    }

    if (dev.current > 30.0f || P_calc > 6600.0) {
        printf("[CANH BAO]: QUA TAI NGUY HIEM!\n");
    }

    if (dev.status == 0 && dev.current > 0.05f) {
        printf("[CANH BAO]: RO RI DIEN HOAC TAI AN!\n");
    }

    double hours = dev.operatingTime / 60.0;
    double kWh = (P_calc * hours) / 1000.0;

    printf("Dien nang tieu thu: %.4f kWh\n", kWh);

    return 0;
}

//BÁO CÁO PHÂN TÍCH VÀ TỐI ƯU HÓA CẤU TRÚC DỮ LIỆU STRUCT

//1. Phân tích bài toán (Input/Output)

// Input (Dữ liệu đầu vào)
//Điện áp $U$ (Volt), Dòng điện $I$ (Ampere).
//Công suất đo được từ thiết bị $P_{\text{measured}}$ (Watt).
//hời gian hoạt động $t$ (phút).
//Trạng thái thiết bị `status`: $0$ (Tắt) hoặc $1$ (Bật).

//Output (Kết quả đầu ra)
//  Kích thước bộ nhớ `sizeof` so sánh trước và sau khi tối ưu struct.
//  Các thông báo cảnh báo: "Cảm biến công suất báo sai lệch", "QUÁ TẢI NGUY HIỂM", "RÒ RỈ ĐIỆN HOẶC TẢI ẨN".
//  Điện năng tiêu thụ $kWh$ (kilowatt-giờ).

// ---

//  2. Phân tích & Đề xuất Giải pháp Tái cấu trúc Struct

// Cấu trúc ban đầu (Chưa tối ưu: Khai báo xen kẽ các kiểu dữ liệu `char`, `float`, `int`, `double` làm phát sinh nhiều byte đệm (Memory Padding).
//  Giải pháp 1: Tối ưu Struct phẳng (Flat Struct Optimization):** Sắp xếp thứ tự các thuộc tính theo dung lượng kiểu dữ liệu giảm dần (`double` $\rightarrow$ `float` $\rightarrow$ `int` $\rightarrow$ `char` / `bool`). Giảm thiểu tối đa Memory Padding.
//  Giải pháp 2: Cấu trúc lồng nhau (Nested Struct):** Nhóm các thuộc tính có cùng chức năng (Điện năng, Trạng thái, Cảnh báo) thành các struct con lồng vào struct lớn. Tăng tính đóng gói và dễ quản lý.

//  3. So sánh & Lựa chọn Giải pháp

//  Tiêu chí | Giải pháp 1: Struct phẳng tối ưu | Giải pháp 2: Struct lồng nhau (Nested) |
//  Dung lượng (`sizeof`)** | **Tối ưu nhất** (ít Padding nhất, khoảng 24-32 bytes) | Trung bình (vẫn có Padding ở ranh giới giữa các struct con) |
//  Độ phức tạp truy xuất** | **Đơn giản** (truy cập 1 cấp: `device.voltage`) | Phức tạp hơn (truy cập nhiều cấp: `device.power.voltage`) |
//  Tính bảo trì & Mở rộng** | Trung bình (khi thêm nhiều thuộc tính sẽ khó quản lý) | **Rất cao** (mô-đun hóa từng nhóm thông số) |

//  Lựa chọn tối ưu:** Chọn **Giải pháp 1 (Flat Struct Optimization cho vi điều khiển IoT Gateway hạn chế RAM vì nó tiết kiệm từng byte RAM thực tế và truy xuất trực tiếp tốc độ cao.