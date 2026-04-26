# 📻 電台清單製作指南

本文件說明 `playlist.csv` 的格式規範、如何篩選可用的串流 URL，以及如何驗證 URL 是否正常運作。

---

## 目錄

- [playlist.csv 格式規範](#playlistcsv-格式規範)
- [串流 URL 篩選原則](#串流-url-篩選原則)
- [如何找到可用的串流 URL](#如何找到可用的串流-url)
- [如何用 VLC 驗證 URL](#如何用-vlc-驗證-url)
- [範例電台清單說明](#範例電台清單說明)

---

## playlist.csv 格式規範

### 基本格式

```
電台名稱[TAB]串流URL[TAB]0
```

每一行代表一個電台，三個欄位之間以 **Tab 字元**（`\t`）分隔：

| 欄位 | 說明 | 限制 |
|------|------|------|
| 第 1 欄 | 電台名稱 | **只能用 ASCII 字元**，無中文、無越文調號 |
| 第 2 欄 | 串流 URL | 完整的 HTTP/HTTPS URL |
| 第 3 欄 | 音量偏移 | 固定填 `0` |

### 注意事項

- 分隔符號**必須是 Tab**，不是空格，也不是逗號
- 電台名稱中**不可含有 Tab 字元**
- 名稱限制使用 ASCII 字元（原因見下方字型說明）
- URL 中**不可含有隱藏字元**（如 Zero-Width Space U+200B），從網頁複製 URL 時要特別注意

### 正確範例

```
ICRT Taiwan	http://stream.rcs.revma.com/ypqhjrneygzuv	0
BBC World Service	http://stream.live.vc.bbcmedia.co.uk/bbc_world_service	0
VOV1	https://str.vov.gov.vn/vovlive/vov1vov5Vietnamese.sdp_aac/playlist.m3u8	0
```

---

## 串流 URL 篩選原則

並非所有廣播電台的 URL 都能在 ESP32 上穩定播放。以下列出常見的問題類型。

### ❌ 必須剔除的 URL 類型

#### 1. Zeno.fm JWT Token URL

**特徵：** URL 含有 `?zt=eyJ...`

```
https://stream.zeno.fm/xxxxx?zt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```

**問題：** JWT Token 約 60 秒後過期，ESP32 無法在 token 過期前完成連線或重新取得新 token。

**解決：** 尋找同一電台的 Icecast / Shoutcast 直接串流 URL（通常以 port 8000 結尾）。

---

#### 2. VOH / 動態 chunklist URL

**特徵：** URL 含有 `chunklist_w[數字].m3u8`

```
https://www.voh.com.vn/hls/xxxxx/chunklist_w1234567.m3u8
```

**問題：** `chunklist_w` 後面的數字是每次 session 動態產生的 token，ESP32 使用固定 URL 無法連線。

**解決：** 尋找電台的主 `.m3u8` 播放清單 URL（不含 `chunklist_w`），或改用其他串流來源。

---

#### 3. ZMP3 / Zing CDN

**特徵：** URL 含有 `zmdcdn.me` 網域

```
https://stream.zmdcdn.me/voh-fm999/...
```

**問題：** Zing CDN 需要 auth session 驗證，非常駐開放串流，ESP32 無法通過驗證。

---

#### 4. 電視影像串流（含 H.264 視訊）

**特徵：** 通常是 `.m3u8` 格式，但串流內容含有 `#EXT-X-STREAM-INF` 的 H.264 視訊軌

**問題：** `ESP32-audioI2S` 函式庫只能解碼音訊（MP3、AAC），無法處理影像串流，播放會失敗。

**判斷方式：** 用瀏覽器開啟 `.m3u8` 檔案，如果裡面有 `CODECS="avc1..."` 就是影像串流。

---

#### 5. 重複 URL

**問題：** 同一串流 URL 出現多次（不同名稱），ESP32 記憶體有限，清單應精簡。

**解決：** 相同 URL 只保留一個名稱。

---

#### 6. 含隱藏字元的 URL

**特徵：** 從網頁複製的 URL，肉眼看不出差異但連線失敗

**常見隱藏字元：** Zero-Width Space（U+200B）、Zero-Width No-Break Space（U+FEFF）

**偵測方式：**
- 將 URL 貼入文字編輯器（如 VS Code）並啟用「顯示特殊字元」功能
- 或貼入 URL 解碼工具（如 https://www.urldecoder.org）觀察是否有異常

---

### ✅ 可以使用的 URL 類型

| 類型 | 說明 | 範例 |
|------|------|------|
| Icecast / Shoutcast 直接串流 | 最穩定，直接 MP3 輸出 | `http://stream.example.com:8000/live` |
| HLS .m3u8（純音訊） | ESP32-audioI2S v0.9.720 支援 | `https://example.com/live/audio.m3u8` |
| REVMA / RCS 串流 | 商業廣播常用，穩定 | `http://stream.rcs.revma.com/...` |
| BBC / VOA 官方串流 | 國際廣播，穩定 | `http://stream.live.vc.bbcmedia.co.uk/...` |

---

## 如何找到可用的串流 URL

### 方法 1：Radio Browser（推薦）

1. 前往 https://www.radio-browser.info
2. 搜尋電台名稱或依國家/語言篩選
3. 點選電台後複製「Stream URL」
4. 優先選擇格式為 `audio/mpeg` 或 `audio/aac` 的 URL

### 方法 2：從電台官網尋找

許多電台會在官網提供直接串流連結：
- 前往電台官網的「線上收聽」或「Listen Live」頁面
- 用瀏覽器開發者工具（F12）→ Network 頁籤
- 過濾 `.mp3`、`.aac`、`.m3u8` 類型的請求
- 複製該 URL

### 方法 3：透過 TuneIn 或其他聚合平台

TuneIn、iHeart Radio 等聚合平台通常有電台的直接串流 URL，但需要留意部分 URL 可能有 token 過期問題。

---

## 如何用 VLC 驗證 URL

在加入 playlist.csv 之前，建議先用 VLC 確認 URL 可以正常播放：

### 步驟

1. 開啟 VLC Media Player
2. 選擇 `媒體（Media）→ 開啟網路串流（Open Network Stream）`（Ctrl+N）
3. 貼上串流 URL
4. 按「播放」

### 判斷標準

| 結果 | 代表 | 是否加入 playlist |
|------|------|-----------------|
| 正常播放音訊 | URL 有效，格式正確 | ✅ 可以加入 |
| 播放約 60 秒後斷線 | 可能有 JWT Token | ❌ 勿加入 |
| 無法連線 / 無音訊 | URL 無效或需要驗證 | ❌ 勿加入 |
| 只有影像沒有聲音 | 影像串流 | ❌ 勿加入 |

> 📝 **提醒：** VLC 能播放不代表 ESP32 一定能播放（VLC 支援更多格式），但 VLC 無法播放的 ESP32 一定也不行。

---

## 範例電台清單說明

本 repo 提供的 `playlist_sample.csv` 包含以下電台（截至撰寫時可用，URL 日後可能失效）：

### 越南電台（10 個）

| 名稱 | 電台 | 格式 |
|------|------|------|
| VOV1 | 越南之聲第 1 頻道 | HLS AAC |
| VOV2 | 越南之聲第 2 頻道 | HLS AAC |
| VOV Giao thong Ha Noi | 河內交通廣播 | HLS MP3 |
| VOV Giao thong Ho Chi Minh | 胡志明市交通廣播 | HLS MP3 |
| VOH FM 99.9 | 胡志明市廣播 | Icecast |
| VTC News Radio | VTC 新聞廣播 | MP3 |
| Radio Saigon 900 | 西貢廣播 | Icecast |
| Hanoi Radio FM 90MHz | 河內廣播 | Icecast |
| DAN CA Quan Ho | 越南民歌頻道 | HLS AAC |
| Radio The Voice of Vietnam FM | 越南之聲 FM | Icecast |

### 台灣電台（10 個）

| 名稱 | 電台 | 備註 |
|------|------|------|
| ICRT Taiwan | 台北國際社區廣播 | 英語廣播 |
| BCC News Radio | 中廣新聞網 | 純音訊串流 |
| Zhengsheng Broadcasting FM 99.1 | 正聲廣播 99.1 | — |
| UFO Radio FM 92.1 | UFO 飛碟廣播 | HLS |
| HIT FM 91.1 Taipei | 台北 HIT FM | — |
| Radio Taipei International | 台北國際廣播 | 英語 |
| Bravo FM 91.3 | 台北 BRAVO FM | — |
| POP Radio FM 91.7 | POP Radio | — |
| Classical Music NTSO Radio | 國立台灣交響樂團 | 古典音樂 |
| Good News Broadcasting | 好消息廣播網 | 宗教廣播 |

---

> ⚠️ **提醒：** 串流 URL 可能因電台調整而失效。若某電台無聲音，請先用 VLC 驗證 URL 是否仍然有效。
