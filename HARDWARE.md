# 🔌 硬體接線說明

本文件詳細說明 ESP32 YoRadio 專題的所有模組接線方式與注意事項。

---

## 目錄

- [VSPI 匯流排架構](#vspi-匯流排架構)
- [FEIYANG 2.8 吋 TFT 顯示器](#feiyang-28-吋-tft-顯示器)
- [MAX98357 I2S 放大器](#max98357-i2s-放大器)
- [KY-040 旋轉編碼器](#ky-040-旋轉編碼器)
- [IR1838 紅外線接收器](#ir1838-紅外線接收器)
- [GPIO 總覽](#gpio-總覽)
- [重要注意事項](#重要注意事項)

---

## VSPI 匯流排架構

FEIYANG 2.8 吋 TFT 模組整合了顯示器、觸控和 SD 卡三個裝置，**共用同一組 VSPI 匯流排**，透過各自獨立的 CS 腳位區分。

```
ESP32 VSPI 匯流排
┌─────────────────────────────────────────────────────┐
│  SCK  = IO18 ──────┬──────────────┬──────────────   │
│  MOSI = IO23 ──────┼──────────────┼──────────────   │
│  MISO = IO19 ──────┼──────────────┼──────────────   │
│                    │              │              │   │
│              TFT 顯示器      XPT2046 觸控    SD 卡   │
│              CS = IO5        CS = IO16    CS = IO17  │
└─────────────────────────────────────────────────────┘
```

---

## FEIYANG 2.8 吋 TFT 顯示器

FEIYANG 模組共有兩排排針：
- **主排針（14 腳）**：TFT 顯示器 + XPT2046 觸控
- **SD 卡獨立排針（4 腳）**

### 主排針（14 腳）接線表

| 模組腳位 | ESP32 腳位 | 說明 |
|---------|-----------|------|
| T_IRQ | **不接** | YoRadio 使用輪詢模式，不使用 IRQ 中斷 |
| T_DO | IO19 | VSPI MISO（觸控用，與 SD 卡 MISO 共用） |
| T_DIN | IO23 | VSPI MOSI（觸控用，與 SD 卡 MOSI 共用） |
| T_CS | IO16 | 觸控 Chip Select（獨立） |
| T_CLK | IO18 | VSPI CLK（觸控用，共用） |
| SDO(MISO) | IO19 | VSPI MISO（顯示器讀取，與 T_DO 並接） |
| LED | 3.3V | 背光常亮（如需調光可接 PWM 腳位） |
| SCK | IO18 | VSPI CLK（顯示器用，共用） |
| SDI(MOSI) | IO23 | VSPI MOSI（顯示器用，共用） |
| DC | IO4 | Data/Command 選擇 |
| RESET | IO15 | 顯示器 Reset |
| CS | IO5 | 顯示器 Chip Select（獨立） |
| GND | GND | 接地 |
| VCC | 3.3V | 電源 |

### SD 卡獨立排針（4 腳）接線表

| 模組腳位 | ESP32 腳位 | 說明 |
|---------|-----------|------|
| SD_SCK | IO18 | VSPI CLK（共用） |
| SD_MISO | IO19 | VSPI MISO（共用） |
| SD_MOSI | IO23 | VSPI MOSI（共用） |
| SD_CS | IO17 | SD 卡 Chip Select（獨立） |

---

## MAX98357 I2S 放大器

MAX98357 為 D 類無濾波 I2S 數位功率放大器，輸出功率 3W@4Ω 或 2W@8Ω。

### 接線表

| 模組腳位 | ESP32 腳位 | 說明 |
|---------|-----------|------|
| VIN | 5V | 電源（使用 5V 以獲得最大輸出功率） |
| GND | GND | 接地 |
| DIN | IO27 | I2S 序列資料輸入 |
| BCLK | IO26 | I2S 位元時脈 |
| LRC / WSEL | IO25 | I2S 左右聲道選擇 |
| SD（Shutdown） | 3.3V 或不接 | 板載 1MΩ 上拉電阻，預設為開啟；接 GND 可靜音 |
| OUTP | 喇叭正極（+） | 差分輸出正端 |
| OUTN | 喇叭負極（−） | 差分輸出負端 |

> ⚠️ **喇叭接線**：MAX98357 輸出為差分訊號（OUTP/OUTN），喇叭不可接地，兩端直接接 OUTP 和 OUTN。

---

## KY-040 旋轉編碼器

KY-040 板載 10K 上拉電阻，無需外部接電阻。  
CLK 和 DT 腳位使用 ESP32 的 input-only 腳位（IO34、IO35）。

### 接線表

| 模組腳位 | ESP32 腳位 | 說明 |
|---------|-----------|------|
| VCC（+） | 3.3V | 板載 10K 上拉電阻，不需外接電阻 |
| GND | GND | 接地 |
| CLK（A） | IO34 | input-only 腳位，左旋 = 音量減 |
| DT（B） | IO35 | input-only 腳位，右旋 = 音量加 |
| SW | IO32 | 按鈕（短按選台，長按視功能設定） |

> ✅ myoptions.h 中 `ENC_INTERNALPULLUP false`，因為 KY-040 模組已有板載上拉電阻。

---

## IR1838 紅外線接收器

IR1838 為 38kHz 紅外線解調接收器，3 腳位（VCC / GND / OUT）。

### 接線表

| 模組腳位 | ESP32 腳位 | 說明 |
|---------|-----------|------|
| VCC | 3.3V | 電源 |
| GND | GND | 接地 |
| OUT | IO36（VP） | input-only 腳位，IR 解調後訊號輸出 |

> 📌 IO36 為 ESP32 的 VP（input-only）腳位，只能輸入不能輸出，適合 IR 接收訊號使用。

---

## GPIO 總覽

| GPIO | 功能 | 說明 |
|------|------|------|
| IO4 | TFT DC | Data/Command 選擇 |
| IO5 | TFT CS | 顯示器 Chip Select（strapping 腳位） |
| IO15 | TFT RST | 顯示器 Reset（strapping 腳位） |
| IO16 | 觸控 CS | XPT2046 Chip Select |
| IO17 | SD CS | SD 卡 Chip Select |
| IO18 | VSPI SCK | SPI 時脈（顯示器 + 觸控 + SD 共用） |
| IO19 | VSPI MISO | SPI 資料輸入（共用） |
| IO23 | VSPI MOSI | SPI 資料輸出（共用） |
| IO25 | I2S LRC | I2S 左右聲道選擇 |
| IO26 | I2S BCLK | I2S 位元時脈 |
| IO27 | I2S DOUT | I2S 資料輸出至 MAX98357 |
| IO32 | 編碼器 SW | KY-040 按鈕 |
| IO34 | 編碼器 CLK | KY-040 A 相（input-only） |
| IO35 | 編碼器 DT | KY-040 B 相（input-only） |
| IO36 | IR 接收 | IR1838 訊號輸出（input-only/VP） |

---

## ⚠️ 重要注意事項

### 1. Input-Only 腳位

IO34、IO35、IO36、IO39 為 **input-only** 腳位（無輸出驅動能力），這些腳位：
- 只能讀取輸入訊號，不能作為輸出
- **沒有內建上拉/下拉電阻**
- 適合用於只需讀取的訊號（編碼器、IR 接收器）

### 2. Strapping 腳位

IO5 和 IO15 為 strapping 腳位，影響開機行為：
- IO5（TFT CS）：CS 在非使用時為高電位（inactive-HIGH），不影響 ESP32 開機
- IO15（TFT RST）：TFT 模組會控制此腳位，不影響正常開機

### 3. VS1053_CS 必須宣告為 255

使用 I2S DAC（MAX98357）時，**必須在 myoptions.h 中宣告 `#define VS1053_CS 255`**，否則韌體可能嘗試初始化不存在的 VS1053 模組而導致異常。

### 4. VSPI 共用說明

TFT 顯示器、XPT2046 觸控、SD 卡共用同一組 VSPI 匯流排（IO18/IO19/IO23），透過各自的 CS 腳位分時使用，這是 ESP32 SPI 的標準用法，不會互相干擾。

### 5. KY-040 上拉電阻

KY-040 模組板載 10K 上拉電阻，myoptions.h 中設定 `ENC_INTERNALPULLUP false`，**不啟用** ESP32 的內部上拉。若使用其他沒有板載上拉電阻的編碼器模組，需改為 `true`。
