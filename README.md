# 📻 ESP32 YoRadio 網路廣播電台

基於 [YoRadio](https://github.com/e2002/yoradio) v0.9.720 的 ESP32 網路廣播電台實作專題。  
本專題使用 ESP32-WROOM-32、MAX98357 I2S 放大器與 FEIYANG 2.8 吋 TFT 觸控顯示器，可收聽全球串流廣播電台。

![版本](https://img.shields.io/badge/YoRadio-v0.9.720-blue)
![ESP32 Core](https://img.shields.io/badge/ESP32_Core-2.0.17-green)
![Arduino IDE](https://img.shields.io/badge/Arduino_IDE-1.8.19-orange)

---

## 📋 目錄

- [硬體清單](#硬體清單)
- [快速開始](#快速開始)
- [軟體安裝步驟](#軟體安裝步驟)
- [myoptions.h 設定](#myoptionsh-設定)
- [接線說明](#接線說明)
- [電台清單格式](#電台清單格式)
- [字型限制說明](#字型限制說明)
- [常見問題](#常見問題)

---

## 🔧 硬體清單

| # | 模組 | 規格 | 功能 |
|---|------|------|------|
| 1 | ESP32-WROOM-32 DevKitC | TYPE-C USB、CH340C、WiFi+BT、雙核 240MHz、4MB Flash | 主控制器 |
| 2 | MAX98357 I2S 數位功率放大器 | D 類無濾波、3W@4Ω / 2W@8Ω、I2S 數位輸入 | 音訊放大 |
| 3 | 喇叭 4Ω 3W | 3525 腔體、1.25mm 端子 | 音訊輸出 |
| 4 | FEIYANG 2.8 吋 TFT 顯示器 | ST7789V、240×RGB×320、SPI VSPI、XPT2046 電阻式觸控、SD 卡插槽 | 顯示器＋觸控 |
| 5 | KY-040 旋轉編碼器 | 360 度、5V、CLK+DT+SW 三腳、板載 10K 上拉電阻 | 音量調整＋選台 |
| 6 | IR1838 紅外線接收器 | 38kHz、3.3V、3 腳（VCC/GND/OUT） | 遙控器接收 |

---

## ⚡ 快速開始

```
1. 安裝 Arduino IDE 1.8.19（必須用 Legacy 版本）
2. 安裝 ESP32 Board Core 2.0.17
3. 安裝 4 個必要函式庫 + ESP32-audioI2S
4. 複製 myoptions.h 到 yoRadio/ 資料夾
5. 替換 Adafruit GFX 的 glcdfont.c 字型檔
6. 設定 Board 為 ESP32 Dev Module、Partition Scheme 為 Default 4MB with spiffs
7. 上傳韌體（Ctrl+U）
8. 連接 AP 設定 WiFi
9. 透過 WebBoard 上傳網頁資料
10. 開始收聽！
```

詳細說明請見下方各節。

---

## 💾 軟體安裝步驟

### 步驟 1：Arduino IDE

- **必須使用 Arduino IDE 1.8.19**（Legacy 版本）
- 下載網址：https://www.arduino.cc/en/software → 選「Legacy IDE (1.8.X)」
- ⛔ **IDE 2.x 完全不支援，無法編譯**

### 步驟 2：ESP32 Board Core

1. `File → Preferences → Additional Boards Manager URLs` 加入：
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
2. `Tools → Board → Boards Manager` → 搜尋 `esp32` → 安裝 **Espressif Systems 2.0.17**
3. ⛔ **Core 3.x 目前不支援，只用 2.x**

### 步驟 3：必要函式庫

透過 `Sketch → Include Library → Manage Libraries` 安裝以下 4 個：

| 函式庫名稱 | 說明 |
|-----------|------|
| `Adafruit GFX Library` | 圖形基礎函式庫 |
| `Adafruit ST7735 and ST7789 Libraries` | TFT 顯示器驅動 |
| `OneButton` | 按鈕處理 |
| `XPT2046_Touchscreen` | 觸控螢幕驅動 |

透過 `Sketch → Include Library → Add .ZIP Library` 安裝：

| 函式庫 | 下載網址 |
|--------|---------|
| `ESP32-audioI2S`（schreibfaul1） | https://github.com/schreibfaul1/ESP32-audioI2S/archive/refs/heads/master.zip |

### 步驟 4：⛔ 絕對不可另外安裝的函式庫

以下函式庫已**內建於 YoRadio 專案中**，額外安裝會造成 `duplicate symbol` 編譯錯誤：

- `IRremoteESP8266`（位於 `src/IRremoteESP8266/`）
- `ESPAsyncWebServer`（位於 `src/AsyncWebServer/`）
- `AsyncTCP`（位於 `src/AsyncWebServer/AsyncTCP.cpp`）
- `async-mqtt-client`

### 步驟 5：替換字型檔

> **此步驟必要**，否則顯示器部分字元無法正常顯示。

| 項目 | 路徑 |
|------|------|
| 來源 | `yoradio-main/yoRadio/fonts/glcdfont.c` |
| 目標（Windows） | `C:\Users\[使用者名稱]\Documents\Arduino\libraries\Adafruit_GFX_Library\glcdfont.c` |
| 目標（macOS） | `~/Documents/Arduino/libraries/Adafruit_GFX_Library/glcdfont.c` |

覆蓋後**重新啟動 Arduino IDE**。

### 步驟 6：放置 myoptions.h

```
yoradio-main/
└── yoRadio/
    ├── yoRadio.ino       ← 主程式
    ├── myoptions.h       ← 放在這裡（與 .ino 同一資料夾）
    └── data/
        └── data/
            └── playlist.csv   ← 電台清單放這裡（可選）
```

本專題的 `myoptions.h` 已在此 repo 根目錄，直接複製即可。

### 步驟 7：Board 設定

`Tools` 選單設定如下：

| 選項 | 設定值 |
|------|--------|
| Board | ESP32 Dev Module |
| CPU Frequency | 240MHz (WiFi/BT) |
| Flash Size | 4MB (32Mb) |
| **Partition Scheme** | **Default 4MB with spiffs** ← 必須選這個 |
| PSRAM | Disabled |
| Upload Speed | 921600 |
| Port | COMx 或 /dev/ttyUSBx |

### 步驟 8：上傳韌體

1. 開啟 `yoRadio.ino` → 按 **Upload**（Ctrl+U）
2. 若出現「**Failed to connect**」：
   - 按住 ESP32 上的 **BOOT 按鈕** → 按 Upload
   - 看到 `Connecting...` 字樣再放開 BOOT

### 步驟 9：上傳網頁資料（WebBoard 方式）

v0.9.720 版本使用內建 WebBoard，**不需要 ESP32FS Plugin**。

1. ESP32 開機後，手機或電腦連接 WiFi：`yoRadioAP`（密碼：`12345987`）
2. 瀏覽器開啟：`http://192.168.4.1`
3. 輸入家用 WiFi 帳號密碼 → ESP32 連上網路
4. 瀏覽器開啟：`http://[ESP32 IP 位址]/webboard`
5. 逐一上傳 `yoRadio/data/www/` 內的 11 個檔案：

   ```
   dragpl.js.gz       ir.css.gz          ir.js.gz
   irrecord.html.gz   logo.svg.gz        options.html.gz
   player.html.gz     script.js.gz       style.css.gz
   theme.css          updform.html.gz
   ```

6. 上傳完成後 ESP32 自動重啟，網頁介面完整啟動

### 步驟 10：新增電台

- 瀏覽器開啟：`http://[ESP32 IP 位址]/`
- 點選「Edit Playlist」新增電台
- 或參考 `playlist_sample.csv` 直接上傳

---

## ⚙️ myoptions.h 設定

```cpp
#ifndef myoptions_h
#define myoptions_h

// 語言設定
#define L10N_LANGUAGE         EN

// 顯示器：ST7789V 240×320 SPI
#define DSP_MODEL             DSP_ST7789
#define TFT_CS                5
#define TFT_DC                4
#define TFT_RST               15

// 觸控：XPT2046（VSPI 共用 SCK=IO18, MOSI=IO23, MISO=IO19）
#define TS_MODEL              TS_MODEL_XPT2046
#define TS_CS                 16

// SD 卡（VSPI 共用）
#define SDC_CS                17

// I2S DAC：MAX98357
#define I2S_DOUT              27
#define I2S_BCLK              26
#define I2S_LRC               25

// 停用 VS1053（使用 I2S DAC 時必須宣告）
#define VS1053_CS             255

// KY-040 旋轉編碼器
#define ENC_BTNL              34   // CLK 左旋/音量減
#define ENC_BTNR              35   // DT  右旋/音量加
#define ENC_BTNB              32   // SW  按鈕
#define ENC_INTERNALPULLUP    false // KY-040 已有 10K 上拉

// IR1838 紅外線接收器
#define IR_PIN                36   // input-only 腳位（VP）

#endif
```

完整含中文詳細註解版本請見本 repo 的 `myoptions.h`。

---

## 🔌 接線說明

詳細接線說明請見 [HARDWARE.md](HARDWARE.md)。

### 快速對照表

#### FEIYANG 2.8 吋 TFT 顯示器（14 腳主排針）

| 模組腳位 | ESP32 | 說明 |
|---------|-------|------|
| T_DO | IO19 | VSPI MISO（觸控，共用） |
| T_DIN | IO23 | VSPI MOSI（觸控，共用） |
| T_CS | IO16 | 觸控 Chip Select |
| T_CLK | IO18 | VSPI CLK（觸控，共用） |
| LED | 3.3V | 背光常亮 |
| SCK | IO18 | VSPI CLK（顯示器，共用） |
| SDI(MOSI) | IO23 | VSPI MOSI（顯示器，共用） |
| DC | IO4 | Data/Command |
| RESET | IO15 | 顯示器 Reset |
| CS | IO5 | 顯示器 Chip Select |
| GND | GND | — |
| VCC | 3.3V | — |

#### MAX98357 I2S 放大器

| 模組腳位 | ESP32 | 說明 |
|---------|-------|------|
| VIN | 5V | 電源 |
| GND | GND | — |
| DIN | IO27 | I2S 數據 |
| BCLK | IO26 | I2S 位元時脈 |
| LRC/WSEL | IO25 | I2S 左右聲道選擇 |
| SD | 3.3V（或不接） | 板載 1MΩ 上拉，預設開啟 |

#### KY-040 旋轉編碼器

| 模組腳位 | ESP32 | 說明 |
|---------|-------|------|
| VCC(+) | 3.3V | 板載 10K 上拉，不需外接 |
| GND | GND | — |
| CLK(A) | IO34 | input-only，左旋/音量減 |
| DT(B) | IO35 | input-only，右旋/音量加 |
| SW | IO32 | 按鈕 |

#### IR1838 紅外線接收器

| 模組腳位 | ESP32 | 說明 |
|---------|-------|------|
| VCC | 3.3V | — |
| GND | GND | — |
| OUT | IO36 | input-only 腳位（VP） |

---

## 📻 電台清單格式

`playlist.csv` 格式：

```
電台名稱[TAB]串流URL[TAB]0
```

範例：

```
ICRT Taiwan	http://stream.rcs.revma.com/ypqhjrneygzuv	0
BBC World Service	http://stream.live.vc.bbcmedia.co.uk/bbc_world_service	0
```

**重要規則：**

- 電台名稱**必須使用 ASCII 字元**（無中文、無越文調號）
- 分隔符號必須是 **Tab**（`\t`），不是空格
- 第三欄固定為 `0`（音量偏移值）

範例電台清單請見 [playlist_sample.csv](playlist_sample.csv)，詳細說明請見 [PLAYLIST_GUIDE.md](PLAYLIST_GUIDE.md)。

---

## 🔤 字型限制說明

### 繁體中文

YoRadio 使用的 `glcdfont.c` 只有 256 個 5×7 點陣字形（標準 ASCII），**無法顯示中文**。  
ESP32-WROOM-32（無 PSRAM）的記憶體無法容納完整 CJK 點陣字型（需要數百 KB）。  
→ 電台名稱請使用英文縮寫或拼音羅馬字，**音訊播放完全正常**。

### 越文調號

`utf8Rus.cpp` 只對映西里爾字母（`0xD0/0xD1` prefix），越文調號範圍（`0xE1 0xBA-0xBF`）不支援，調號字元會消失或亂碼。  
→ playlist.csv 中的越文電台名稱請改用**無調號拼音**（如 `VOV Giao thong Ha Noi`）。

---

## ❓ 常見問題

請見 [TROUBLESHOOTING.md](TROUBLESHOOTING.md)。

---

## 📚 相關資源

| 資源 | 連結 |
|------|------|
| YoRadio 原始碼 | https://github.com/e2002/yoradio |
| myoptions 設定產生器 | https://e2002.github.io/docs/myoptions-generator.html |
| ESP32-audioI2S 函式庫 | https://github.com/schreibfaul1/ESP32-audioI2S |
| Radio Browser | https://www.radio-browser.info |

---

## 📄 授權

本專題為個人學習實作，YoRadio 原始碼授權請見 [YoRadio GitHub](https://github.com/e2002/yoradio)。
