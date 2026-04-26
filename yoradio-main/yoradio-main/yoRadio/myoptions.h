#ifndef myoptions_h
#define myoptions_h

/*=======================================================================
  YoRadio - myoptions.h  (đã kiểm tra & xác nhận qua source code)
  Phần cứng:
    - ESP32-WROOM-32 DevKitC (TYPE-C / CH340C)
    - MAX98357 I2S Amplifier → Loa 4Ω 3W
    - FEIYANG 2.8" TFT ST7789V 240×320 + XPT2046 Touch + SD Card
    - KY-040 Rotary Encoder
    - IR1838 IR Receiver
========================================================================*/

// ── NGÔN NGỮ HIỂN THỊ ───────────────────────────────────────────────
#define L10N_LANGUAGE         EN

// ── MÀN HÌNH: ST7789V 240×320 SPI ───────────────────────────────────
// Code tự gọi init(240,320) rồi setRotation(1) → landscape 320×240
#define DSP_MODEL             DSP_ST7789   // = 4, panel 240×320, auto-rotate → 320×240
#define TFT_CS                5            // CS màn hình   → IO5
#define TFT_DC                4            // DC/RS         → IO4
#define TFT_RST               15           // RST           → IO15

// ── CẢM ỨNG: XPT2046 (VSPI chung: SCK=IO18, MOSI=IO23, MISO=IO19) ──
#define TS_MODEL              TS_MODEL_XPT2046
#define TS_CS                 16           // T_CS cảm ứng  → IO16

// ── THẺ SD (VSPI chung: SCK=IO18, MOSI=IO23, MISO=IO19) ─────────────
#define SDC_CS                17           // SD_CS         → IO17

// ── I2S DAC: MAX98357 ────────────────────────────────────────────────
#define I2S_DOUT              27           // DIN           → IO27
#define I2S_BCLK              26           // BCLK          → IO26
#define I2S_LRC               25           // LRC/WSEL      → IO25

// Vô hiệu hoá VS1053 (bắt buộc theo README khi dùng I2S DAC)
#define VS1053_CS             255

// ── ENCODER: KY-040 ──────────────────────────────────────────────────
#define ENC_BTNL              34           // CLK (quay trái / vol−) → IO34
#define ENC_BTNR              35           // DT  (quay phải / vol+) → IO35
#define ENC_BTNB              32           // SW  (nhấn nút)         → IO32
#define ENC_INTERNALPULLUP    false        // KY-040 đã có pull-up 10K sẵn

// ── ĐIỀU KHIỂN IR: IR1838 ───────────────────────────────────────────
#define IR_PIN                36           // OUT           → IO36

#endif
