/*=======================================================================
  YoRadio - myoptions.h（已透過 source code 逐行驗證）
  版本：v0.9.720
  
  硬體組合：
    主控：ESP32-WROOM-32 DevKitC（TYPE-C USB、CH340C）
    音訊：MAX98357 I2S 數位功率放大器 + 4Ω 3W 喇叭
    顯示：FEIYANG 2.8 吋 ST7789V TFT + XPT2046 觸控 + SD 卡插槽
    輸入：KY-040 旋轉編碼器
    遙控：IR1838 紅外線接收器（38kHz）
  
  VSPI 匯流排共用腳位：
    SCK  = IO18  (TFT 顯示器 + XPT2046 觸控 + SD 卡)
    MOSI = IO23  (同上)
    MISO = IO19  (同上)
    各 CS 獨立：TFT=IO5, 觸控=IO16, SD=IO17
=========================================================================*/

#ifndef myoptions_h
#define myoptions_h

// ─── 語言設定 ────────────────────────────────────────────────────────────────
#define L10N_LANGUAGE         EN

// ─── 顯示器：ST7789V 240×320 SPI ─────────────────────────────────────────────
// 韌體呼叫 init(240,320) 後自動 setRotation(1)，實際顯示為橫向 320×240
#define DSP_MODEL             DSP_ST7789
#define TFT_CS                5     // IO5：VSPI CS0，strapping 腳位但 CS 為 inactive-HIGH，開機不影響
#define TFT_DC                4     // IO4：Data/Command 選擇
#define TFT_RST               15    // IO15：Reset，strapping 腳位（低電平=正常啟動，已由 TFT 模組控制）

// ─── 觸控：XPT2046 ───────────────────────────────────────────────────────────
// SCK=IO18, MOSI=IO23, MISO=IO19（與 TFT 和 SD 卡共用 VSPI 匯流排）
// T_IRQ 不接：YoRadio 使用輪詢模式，不使用 IRQ 中斷
#define TS_MODEL              TS_MODEL_XPT2046
#define TS_CS                 16    // IO16：觸控 Chip Select（獨立）

// ─── SD 卡 ───────────────────────────────────────────────────────────────────
// SCK=IO18, MOSI=IO23, MISO=IO19（VSPI 共用）
#define SDC_CS                17    // IO17：SD 卡 Chip Select（獨立）

// ─── I2S DAC：MAX98357 ───────────────────────────────────────────────────────
// MAX98357 為 D 類 I2S 放大器，3W@4Ω / 2W@8Ω
// SD（Shutdown）腳位接 3.3V 或不接均可（板載 1MΩ 上拉電阻，預設為開啟狀態）
#define I2S_DOUT              27    // IO27：I2S 序列資料（DIN）
#define I2S_BCLK              26    // IO26：I2S 位元時脈（BCLK）
#define I2S_LRC               25    // IO25：I2S 左右聲道選擇（LRC/WSEL）

// ─── VS1053 停用（必要）──────────────────────────────────────────────────────
// 使用 I2S DAC 時，README 明確要求宣告停用 VS1053
// 255 代表「不使用此腳位」
#define VS1053_CS             255

// ─── 輸入：KY-040 旋轉編碼器 ────────────────────────────────────────────────
// KY-040 模組板載 10K 上拉電阻，不需使用 ESP32 內部上拉
// IO34、IO35 為 input-only 腳位，只能讀取，完全適合編碼器訊號輸入
#define ENC_BTNL              34    // IO34（input-only）：CLK(A)，左旋 = 音量減
#define ENC_BTNR              35    // IO35（input-only）：DT(B)，右旋 = 音量加
#define ENC_BTNB              32    // IO32：SW，按下 = 選台/確認
#define ENC_INTERNALPULLUP    false // KY-040 已有板載上拉，不啟用 ESP32 內部上拉

// ─── 遙控：IR1838 紅外線接收器 ──────────────────────────────────────────────
// IO36（VP）為 input-only 腳位，38kHz 解調後輸出低電位，完全適合 IR 接收
// IRremoteESP8266 已內建於 src/IRremoteESP8266/，勿從 Library Manager 額外安裝
#define IR_PIN                36    // IO36（input-only/VP）：IR 訊號輸入

#endif // myoptions_h
