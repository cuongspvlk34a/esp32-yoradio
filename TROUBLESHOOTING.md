# 🛠 常見問題排除

---

## 目錄

- [上傳韌體問題](#上傳韌體問題)
- [編譯錯誤](#編譯錯誤)
- [顯示器問題](#顯示器問題)
- [音訊問題](#音訊問題)
- [網路與電台問題](#網路與電台問題)
- [字型限制](#字型限制)
- [已知限制說明](#已知限制說明)

---

## 上傳韌體問題

### ❌ Failed to connect to ESP32

**症狀：** 按 Upload 後出現 `A fatal error occurred: Failed to connect to ESP32`

**原因：** ESP32 沒有進入燒錄模式。

**解決方式：**
1. 按住 ESP32 板上的 **BOOT** 按鈕（不要放開）
2. 在 Arduino IDE 按 **Upload**（Ctrl+U）
3. 看到序列監視器或輸出欄位出現 `Connecting...` 字樣
4. **放開** BOOT 按鈕
5. 等待上傳完成

### ❌ 序列埠找不到 / COM Port 不顯示

**解決方式：**
- Windows：至裝置管理員確認 CH340C 驅動程式已安裝
- 驅動程式下載：https://www.wch.cn/downloads/CH341SER_EXE.html
- macOS：安裝 CH34x 驅動（依 macOS 版本不同）

### ❌ Upload 完成後一直重啟（boot loop）

**可能原因：** Partition Scheme 選錯。

**解決方式：**  
`Tools → Partition Scheme → Default 4MB with spiffs`（必須選這個，不能選其他）

---

## 編譯錯誤

### ❌ duplicate symbol / multiple definition

**症狀：** 編譯時出現類似：
```
multiple definition of `IRsend::send`
```
或
```
duplicate symbol '_ESPasyncTCP_...'
```

**原因：** 從 Library Manager 額外安裝了 YoRadio 已內建的函式庫。

**解決方式：** 從 Arduino 函式庫資料夾**刪除**以下任何一個（如果有安裝的話）：
- `IRremoteESP8266`（YoRadio 已在 `src/IRremoteESP8266/` 內建）
- `ESPAsyncWebServer`（YoRadio 已在 `src/AsyncWebServer/` 內建）
- `AsyncTCP`（YoRadio 已在 `src/AsyncWebServer/AsyncTCP.cpp` 內建）
- `async-mqtt-client`

Windows 函式庫位置：`C:\Users\[使用者名稱]\Documents\Arduino\libraries\`  
macOS 函式庫位置：`~/Documents/Arduino/libraries/`

### ❌ AsyncTCP 堆疊溢位 / Stack overflow

**說明：** YoRadio v0.9.337b 起，AsyncTCP 已在 `src/AsyncWebServer/AsyncTCP.cpp` 內修正（`XTASK_MEM_SIZE` 已設為 `1024*5`），**不需要手動修改任何外部檔案**。

若你看到此問題，請確認你使用的是 YoRadio 原始碼中的 AsyncTCP，而非從 Library Manager 安裝的版本。

### ❌ 找不到 DSP_ST7789 / TS_MODEL_XPT2046

**原因：** `myoptions.h` 放錯位置，或使用了舊版 YoRadio。

**解決方式：**
- 確認 `myoptions.h` 放在 `yoRadio/yoRadio.ino` **同一個資料夾**中
- 確認使用 YoRadio v0.9.337b 以上版本

### ❌ 找不到 Adafruit_GFX.h / XPT2046_Touchscreen.h

**原因：** 必要函式庫未安裝。

**解決方式：** 透過 `Sketch → Include Library → Manage Libraries` 安裝：
- `Adafruit GFX Library`
- `Adafruit ST7735 and ST7789 Libraries`
- `OneButton`
- `XPT2046_Touchscreen`

### ❌ Arduino IDE 2.x 編譯失敗

**說明：** YoRadio 需要 Arduino IDE **1.8.19**（Legacy 版本），IDE 2.x **不支援**。  
請至 https://www.arduino.cc/en/software 下載「Legacy IDE (1.8.X)」。

---

## 顯示器問題

### ❌ 顯示器全白或全黑，無任何顯示

**可能原因與解決方式：**
1. **接線錯誤**：再次對照 HARDWARE.md 確認所有 TFT 腳位接線
2. **CS/DC/RST 腳位設定錯誤**：確認 myoptions.h 中 `TFT_CS=5, TFT_DC=4, TFT_RST=15`
3. **字型檔未替換**：確認已將 `yoRadio/fonts/glcdfont.c` 覆蓋至 Adafruit GFX 函式庫

### ❌ 顯示方向錯誤（直向 vs 橫向）

**說明：** ST7789V 驅動程式在 `init(240,320)` 後會自動執行 `setRotation(1)`，顯示為橫向 320×240。  
這是韌體預設行為，無需修改。

### ❌ 觸控無反應

**可能原因：**
- T_CS 接線錯誤（應為 IO16）
- T_DO（MISO）未正確接至 IO19

---

## 音訊問題

### ❌ 無聲音

**檢查步驟：**
1. 確認 MAX98357 VIN 接 **5V**（不是 3.3V）
2. 確認 DIN=IO27, BCLK=IO26, LRC=IO25
3. 確認 SD（Shutdown）腳位接 **3.3V 或懸空**（接 GND 會靜音）
4. 確認喇叭正負極接 OUTP/OUTN（差分輸出，不可接地）
5. 確認 myoptions.h 中 `VS1053_CS 255` 已宣告

### ❌ 聲音有雜訊 / 爆音

**可能原因：**
- 電源供電不穩，建議使用獨立 5V 電源而非電腦 USB
- 喇叭阻抗不匹配（使用 4Ω 喇叭以獲得 3W 輸出）

---

## 網路與電台問題

### ❌ 找不到 yoRadioAP 熱點

**解決方式：**
1. 確認韌體上傳成功
2. 按下 ESP32 Reset 按鈕重新開機
3. 等待約 10 秒後再搜尋 WiFi

### ❌ 連上 WiFi 後電台無聲音

**可能原因：**
- 電台串流 URL 已失效（詳見 PLAYLIST_GUIDE.md）
- JWT Token URL（Zeno.fm）已過期
- 動態 chunklist URL（每次連線產生不同 URL）

**解決方式：** 使用 VLC 先驗證 URL 是否可正常播放。

### ❌ WebBoard 上傳頁面無法開啟

**解決方式：**
1. 確認 ESP32 已連上家用 WiFi（而非仍在 AP 模式）
2. 確認瀏覽器中輸入的 IP 位址正確（可從路由器 DHCP 清單確認）
3. URL 格式：`http://[IP位址]/webboard`

---

## 字型限制

### 繁體中文無法顯示

**說明：** 這是 YoRadio 的已知限制，非設定錯誤。

`glcdfont.c` 僅有 256 個 5×7 點陣字形（ASCII 範圍），無法包含 CJK 字元。完整中文字型需要 3000~6000 個字形，佔用數百 KB，ESP32-WROOM-32（無 PSRAM）的 Flash/RAM 無法容納。

**解決方式：** playlist.csv 中的電台名稱使用英文縮寫或拼音羅馬字，例如：
- ❌ `臺北廣播電台`
- ✅ `Taipei Broadcasting`

### 越文調號顯示亂碼

**說明：** `utf8Rus.cpp` 只對映西里爾字母（`0xD0/0xD1` prefix），越文調號 Unicode 範圍（`0xE1 0xBA-0xBF`）的字元不在對映表中，會消失或顯示亂碼。

**解決方式：** 使用無調號拼音，例如：
- ❌ `VOV Giao thông Hà Nội`
- ✅ `VOV Giao thong Ha Noi`

---

## 已知限制說明

### ESP32FS Plugin 已過時

**說明：** 網路上許多舊教學提到使用 ESP32FS Plugin 上傳 SPIFFS 資料。  
YoRadio **v0.9.720** 已改用內建 WebBoard 上傳器，**不需要也不應該使用 ESP32FS Plugin**。

正確做法：韌體上傳後，透過 `http://[IP]/webboard` 介面逐一上傳 `data/www/` 下的檔案。

### Core 3.x 不支援

YoRadio 目前（v0.9.720）需要 ESP32 Arduino Core **2.0.17**。  
Core 3.x 有 API 變動，不支援，請確認安裝的是 **Espressif Systems 2.0.17**。
