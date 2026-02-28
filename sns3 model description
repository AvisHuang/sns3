### Refrence
> https://www.sns3.org/doc/satellite-design.html#model-description

# Table of Contents
- [SNS3 Design](#sns3-design)
- [Frame configuration (框架與時槽結構)](#frame-configuration-框架與時槽結構)
- [Architecture (整體架構)](#architecture-整體架構)
- [User terminal SatNetDevice](#user-terminal-satnetdevice)
  - [Additional Note (UT)](#additional-note-ut)
- [Geostationary satellite](#geostationary-satellite)
  - [Additional Note (SAT)](#additional-note-sat)
- [Gateway](#gateway)
  - [Additional Note (GW)](#additional-note-gw)
- [NCC (Network control center)](#ncc-network-control-center)
- [Channel](#channel)
- [Random access (RA) in DVB-RCS2](#random-access-ra-in-dvb-rcs2)
- [Return link packet scheduling](#return-link-packet-scheduling)
- [Demand Assignment Multiple Access (DAMA)](#demand-assignment-multiple-access-dama)
- [UT Scheduler](#ut-scheduler)
- [FWD Link Scheduler](#fwd-link-scheduler)
- [ARQ (Automatic Repeat reQuest)](#arq-automatic-repeat-request)
- [Mobility and Handover](#mobility-and-handover)

## SNS3 Design
> Refrence : https://www.sns3.org/doc/satellite-design.html#design

- **Satellite Network Simulator 3 (SNS3)** is a satellite network extension to **Network Simulator 3 (ns-3)** platform.
- **SNS3** models a full interactive multi-spot beam satellite network with a **geostationary satellite (GEO)**.
- **transparent star (bent-pipe)** payload.

- reference satellite system have **72 spot-beams with an  European coverage**.

- serverd by 5 **gateways (GWs)** and using **Ka-band frequencies**  26.5 GHz ~ 40 GHz on the feeder/user link.

- implements the **DVB-RCS2/S2** standards.
  - **DVB-RCS2 (return link) : Digital Video Broadcast - Return Channel via Satellite - 2nd generation**
  - **DVB-S2 (forword link) : Digital Video Broadcasting - Satellite - 2nd generation**

## Frame configuration (框架與時槽結構)
> Refrence : https://www.sns3.org/doc/satellite-design.html#frame-configuration

### Return Link (DVB-RCS2)
- Use **Multi-Frequency Time Division Multiple Access (MF-TDMA)**.
- Composed of:
  1. **Superframe sequences (SFS)**
  2. **Superframes**
  3. **Frames**
  4. **Time slots**
  5. **Bandwidth Time Units (BTUs)**

**Superframe sequence (SFS)** : An ordered time sequence of superframes on a given set of carriers, identifying one **MF-TDMA** resource block.  *(在一組特定頻寬上的連續 superframe 時間序列，代表一整塊 MF-TDMA 資源)*

<div align="center">
<img width="579" height="281" alt="image" src="https://github.com/user-attachments/assets/defa1270-bb8a-443e-a363-518475093956" />
</div>

<p align="center"><strong>Figure 1.</strong> Superframe sequences (SFS) </p>

<div align="center">
<img width="722" height="663" alt="image" src="https://github.com/user-attachments/assets/6f2ab7a8-6721-49d1-88e6-3d5e478d5353" />
</div>

<p align="center"><strong>Figure 2.</strong> Superframe configuration </p>

> Refrence : [EN 301 545-2 - V1.4.1](https://www.etsi.org/deliver/etsi_en/301500_301599/30154502/01.04.01_60/en_30154502v010401p.pdf) 7.5.1.2

- The used frame structures are dynamically configured by the **Network Control Center (NCC)**.
  - **Superframe Composition Table (SCT)**
  - **Frame Composition Table v2 (FCT2)**
  - **Broadcast Composition Table (BCT)**

- The satellite module does not explicitly model **SCT**, **FCT** and **BCT**.
- The frame configurations may be changed via **parametrization**.
- **NCC** models **Terminal Burst Time Plan v2 (TBTPv2)**.
  - Is capable of configuring the time slots dynamically for each **superframe** **(e.g. start times, durations, waveforms)**.

 ### Forward Link (DVB-S2)
 - Using **DVB-S2 Time Division Multiplexing (TDM)**.
 - feeder link 2 GHz bandwidth is:
   - Divided into **16 carriers**, each **125 MHz**. 
   - Each carrier is statically mapped to a user link frequency color.  *(每個 carrier 固定對應到某一個 user-beam 的 frequency color)*
   - Only one carrier per beam is supported.    *(每個 beam 只支援 一個 carrier)*
  
## Architecture (整體架構)
<img width="1300" height="537" alt="image" src="https://github.com/user-attachments/assets/5949f5b4-03a9-46ba-9677-3c0bd824779f" />
<p align="center"><strong>Figure 3.</strong> SNS3 Architecture </p>

> Refrence : https://www.sns3.org/doc/satellite-design.html#fig-satellite-general-architecture

### Communication Channels
|Channel Name	|Description|
|---|---|
|CSMA channel | Communication between ground user and UT, simulates LAN/shared media  *(模擬地面有線／LAN 通道)*    |
|SatChannel   |	Simulates satellite uplink/downlink (UT ↔ SAT ↔ GW)                      |
|Ideal channel|	Idealized channel between GW and ground user (no interference or latency) *(無干擾、延遲)*|

- All satellite nodes require a new implementation of a ```SatNetDevice```
- ```SatNetDevice``` implement:
  - **Logical Link Control (LLC)**.
  - **Medium Access Control (MAC)**.
  - **Physical (PHY)**.
- A new implementation of the Channel class ```SatChannel```.
  - To support satellite system received signal power calculations.

- Satellite module implements both spherical and geodetic coordinate systems (**WGS80** and **GRS84**). 
  - In addition to labeling latitude, longitude, altitude of UT, GEO satellite and GW.

*(支援球面/大地座標（WGS80、GRS84），方便標示 UT / SAT / GW 的經緯度與高度)*
### **1️⃣ Left side End user（地面使用者）**
- Connection :
  - Use **CSMA channel** (Carrier Sense Multiple Access) connect to User Terminal(UT).
- Protocol Stack :
  - **Data Link Layer (CSMA)** : Ethernet link to the UT.
  - **Network Layer** : An Ip layer(IPv4 or IPv6), responsible for packet routing by IPv4.
  - **Transport Layer** : Use TCP/UDP end-to-end transport protocols, provides reliability, congestion control, ports.
  - **Application Layer** : Actual applications.
- This end user simply sends and receives IP traffic over a LAN interface. *(這個 End user 只是透過 LAN 介面收送 IP 流量)*
- It does not know that its packets will cross a satellite system.

### **2️⃣ UT (User Terminal)**
- Connection :
  - Use **SatChannel** connect to Satellite node.
- Modules :
  - ```SatNetDevice``` : Special satellite network interface towards the satellite.
  - **Network layer** :
    - An IP routing/forwarding layer that connects the **LAN side (CSMA)** to the **satellite side** (```SatNetDevice```).
    - Looks like a router: one interface to the **local LAN**, one interface to the **satellite network**.
  - ```GeoPosition``` : Record the UT’s orbital position (latitude, longitude, altitude using WGS-84/GRS-84).
    *(儲存UT的軌道位置，經度/緯度/高度)*

- The UT plays the role of a customer premises terminal. *(UT 扮演的是一個 用戶端終端設備／小型路由器 的角色)*
- It routes IP packets between the **local LAN** and the **satellite link**.

### **3️⃣ Satellite node**
- Connection :
  - Use **SatChannel** to build **uplink/downlink** with  **UT** and **GW**.
- Modules :
  - ```SatGeoNetDevice``` :
    - Device representing the **satellite’s radio payload**.    *(代表衛星上的射頻酬載。)*
    - Does not run IP or transport protocols.
    - Receives bursts from UTs/GWs on the ```SatChannel```.
    - Forwards the bursts towards the opposite direction (UT↔GW).    *(將 burst 轉發到另一方向（UT ↔ GW）)*
    - Applies propagation effects and computes received signal quality (e.g., SINR).    *(套用傳播效應，計算收到訊號的品質（例如 SINR）。)*
  - ```GeoPosition``` : Record the satellite’s orbital position (latitude, longitude, altitude using WGS-84/GRS-84).
 
 - The satellite is essentially a transparent relay at the **physical**/**MAC** level. *(做透明轉發的中繼站)*

### **4️⃣ GW (GateWay)**
- Connection :
  - Use **Ideal channel** connect to **End Users** .
- Modules :
  - `SatNetDevice` : Handles data transmission to and from the satellite.
  - `CSMA` : Communicates with end users on the ground via terrestrial networks (e.g., Wi-Fi, wired LAN).
  - `GeoPosition` : Records the gateway’s geographic coordinates.

### **5️⃣ Right side End User**
- Connection :
  - Use **Ideal channel** connect to **End Users** .
  - Representing a simplified model without interference or delay. *(無延遲無干擾的簡易模型)*
 
<!--Internally includes : *(內部包含)*
    - **Logical Link Control (LLC)** : Packs/Unpacks higher-layer packets into **bursts**. *(把上層封包打包成 **burst**、或從 **burst** 還原出封包。)*
    - **Medium Access Control (MAC)** : Obeys the satellite access rules (frames, time slots, TBTP). *(遵守衛星系統的存取規則。)*
    - **Physical (PHY)** : Transmits bursts over the ```SatChannel``` using given waveforms/MODCODs. *(用指定的 waveform / MODCOD在 ```SatChannel``` 上發射 **burst**。)* 
-->

---

## User terminal SatNetDevice

<div align="center">
<img width="465" height="566" alt="image" src="https://github.com/user-attachments/assets/9e0aa6c4-350a-400f-acdc-e68d5112cc3d" />
  <p align="center"><strong>Figure 4.</strong> UT SatNetDevice </p>
</div>

>Refrence : https://www.sns3.org/doc/satellite-design.html#user-terminal

### **UT負責** : 
- DVB-RCS2（RTN）回傳鏈路：burst transmission logic（上行 burst 發送邏輯）
- DVB-S2（FWD）前向鏈路：BB frame reception logic（下行 BBFrame 接收邏輯）

### `SatNetDevice`
- UT 對衛星網路的介面
- 內部實作 DVB-RCS2 / DVB-S2 所需的 LLC、MAC、PHY 功能

### `SatPacketClassifier`
- 封包分類
- 根據封包特徵（QoS、目的地）決定後續走哪條處理路徑 (queue / module)。

### `SatUtLlc` (Logical Link Control - LLC Layer)
- `Tx` : 
  - `SatReturnLinkEncapsulator` : 將上行 IP packets 封裝成 **RLE (Return Link Encapsulation)** 格式，for **return channel transmission**.
  - `SatQueue` : 暫存上行資料（封裝後 / 等待排程）

- `Rx`（FWD 下行）:
   - `SatGenericStreamEncapsulator` : 將前向鏈路（DVB-S2）收到的資料做解封包/重新封包。
   - `SatQueue` : 暫存下行資料(收到 / 解封裝後)。

- `SatRequestManager` 
  - 監控 queue 狀態
  - 必要時送 **CR (Capacity Request)** 給 **NCC (Network Control Center)**
  - 支援 :
    - **RBDC(Rate-Based Dynamic Capacity)** 基於速率的動態容量
    - **VBDC(Volume-Based Dynamic Capacity)** 基於容量的動態容量

queue 有資料 → **SatRequestManager** 依規則產生 CR（RBDC/VBDC）→ NCC 在 TBTP 分配上行 timeslot 給 UT

### `SatUtMac` (Media Access Control - MAC Layer)
- `SatUtScheduler` : 根據 **TBTP(Terminal Burst Time Plan)** 資訊來排程 time slots
- `SatRandomAccess` : 支援 random access mechanisms
- `SatTbtpContainer` : 保存 NCC 傳送的 TBTP
  
*(遵守衛星系統的存取規則。)*

### `SatUtPhy` (PHY Layer)
- `SatPhyTx` : 處理實體層的傳輸邏輯
- `SatPhyRx` : 處理實體層的接收邏輯
  - `SatPhyRxCarrier` : 多個 carrier 的接收處理與干擾檢查
    - `SatLinkResults` : 儲存鏈路結果（例如接收品質/是否成功等結果資料）
    - `SatInterference` : 評估接收時的干擾

### **Additional Note (UT)**

**UT使用流程**
<div align="center">
<img width="233" height="666" alt="image" src="https://github.com/user-attachments/assets/939da820-8071-4a3c-8670-217766edca38" />
    <p align="center"><strong>Figure 5.</strong> UT flowchart </p>
</div>



## Geostationary satellite

<div align="center">
<img width="721" height="395" alt="image" src="https://github.com/user-attachments/assets/27381ff5-c92c-4c55-b410-db6455205ca1" />
    <p align="center"><strong>Figure 6.</strong> SatGeoNetDevice </p>
</div>

>Refrence : https://www.sns3.org/doc/satellite-design.html#geostationary-satellite

### `SatGeoUserPhy`

- 處理 **User Link PHY layer**

- SatChannel "Rtn user" :（UT → SAT）
- SatChannel "Fwd user" :（SAT → UT）
- `Tx` : Transmitter module, 負責向 **UT（User Terminal）** 發送訊號的模組。
- `Rx` : Receiver module, 接收來自**UT（User Terminal）** 的訊號。
  - `RxC` : Receiver Control module, 用於量測接收到的使用者上行品質
  - `I` : Interference Estimator, 計算**SINR (Signal-to-Interference-plus-Noise Ratio)**.
   

### `SatGeoFeederPhy` 

- 處理**Feeder Link PHY layer**

- SatChannel "Fwd feeder" :（GW → SAT）
- SatChannel "Rtn feeder" :（SAT → GW）

- 與**SatGeoUserPhy**相同，包含`Tx`、`Rx`、`RxC`、`I`
- 接收來自**GW**的訊號或向**GW**發送訊號。

### **Additional Note (SAT)**
- **Transparent Forwarding (bent-pipe)** : 衛星不對封包進行處理，只負責放大和轉發訊號。
- **SINR Calculation** : SINR分別在 **User End**和 **Feeder Ends** 進行計算，最後在 **GW** 使用複合公式進行組合。

<div align="center">
<img width="517" height="662" alt="image" src="https://github.com/user-attachments/assets/4f107856-3f0c-4778-8e36-981e0f88c3b3" />
    <p align="center"><strong>Figure 7.</strong> RTN / FWD packet flowchart</p>
</div>

## Gateway

<div align="center">
<img width="465" height="566" alt="image" src="https://github.com/user-attachments/assets/f2492a0c-a293-4d2b-b1cc-a99ebef8a262" />
    <p align="center"><strong>Figure 8.</strong> GW SatNetDevice </p>
</div>

> Refrence : https://www.sns3.org/doc/satellite-design.html#gateway

### `SatNetDevice`
- **GW** 網路設備的容器
- 和**UT**的差別 : **GW**主要負責 FWD 發送 以及 RTN 接收

### `SatPacketClassifier`
- 將上層送下來的封包做分類
- 決定後續的路由路徑和調度策略

*GW 端的入口分流器*

### `SatGwLlc` (Logical Link Control - LLC Layer)


- `Tx` : 
  - `SatGenericStre。
  - `SatPhyRxCarrier` : 多個 carrier 的接收處理與干擾檢查。
    - `SatLinkResults` : 儲存鏈路結果（例如接收品質/是否成功等結果資料）。
    - `SatInterference` : 評估接收時的干擾。


### **Additional Note (GW)**
- `SatNetDevice` 涵蓋從分類到物理傳輸的整個協定。 
- **LLC 層** 透過獨立的 Tx 和 Rx 模組支援**雙向操作 (bidirectional operations)**，確保**全雙工通訊(full-duplex communication)**。
- **MAC 層** 負責根據 MODCOD 方案產生/調度 BBFrames。
- **PHY層** 提供發送和**多載波接收(multi-carrier reception)** ，支援：
  - **鏈路品質監測(Link quality monitoring)**。
  - **干擾評估(Interference assessment)**。
- 如果沒有使用者資料可用，**GW** 會產生**虛擬 BBFrame (dummy BBFrames)** 來維持同步和 **連續幀傳輸(continuous frame transmission)**。

---

## **NCC (Network control center)**
**NCC (Network Control Centre)** 是專門負責**回傳鏈路（RTN）** 資源管理與調度的核心控制實體。

包含 :
  - **Admission Control（準入控制）** : 決定哪些回傳請求可被接受、哪些要延後或拒絕。
  - **Packet Scheduling（封包/時槽排程）** : NCC 會建立 **TBTP（Terminal Burst Time Plan）**，內容包含 : 
    - 哪些 timeslots 可供使用。
    - 每個 timeslot 的大小、start、duration。
    - 相應的 waveform（MODCOD）。
- **ACM（Adaptive Coding and Modulation）** 管理 :
  - 根據 **GW** 收到 **UT** 回報的 **channel quality（例如 C/N0）**。
  - **NCC** 決定每個時槽要使用哪種 **MODCOD（modulation & coding）**。

### Global NCC and Scheduler Design
**NCC** 為不同 **spot beam** 設置了各自的 scheduler（`SatBeamScheduler`）

因為 : 

- 每個 beam 都有不同 UT 分布。
- 不同 beam 的使用量/負載不一樣。
- channel conditions (SINR)不一樣 。

### NCC 與 GW / SatNetDevice 的互動
**NCC** 是直接附掛（attached）在每個 **GW** 與 `SatNetDevice` 上，這樣做可以 : 

- 模擬時能用理想通道（callback）來模擬。
- 之後也可以替換成真正 protocol message。


## Channel
*建模頻譜共享與同頻干擾*

- `SatChannel` 代表一個 frequency color（頻寬）。
- 將特定頻寬內的資料傳送給共享相同頻寬的所有接收器。
- SNS3實現 :
  - **同頻波束(Co-channel beams)** : 共用相同頻寬，可能會互相干擾。
  - 使用不同 frequency color 則不會互相影響。

為了模擬現實中**同頻干擾**。

<div align="center">
<img width="1137" height="474" alt="image" src="https://github.com/user-attachments/assets/35711be1-0195-4364-8a40-dac53ef61a4f" />
    <p align="center"><strong>Figure 9.</strong> Satellite channel structure with 16 beams </p>
</div>

> Refrence : https://www.sns3.org/doc/satellite-design.html#channel

### User Link
- 每個方向各有**4 個 `SatChannel` instances**。
- 每個 **instances 代表 125 MHz 頻寬**，在參考系統中 : 
  - 總共有 **72 個beams**。
  - 72 / 4 = **18 個 spot-beams 共用同一個 frequency color**。
  - 這 18 個 spot-beams 可能會相互干擾。
 
### Feeder Link
- 每個方向有 **16 個 `SatChannel` instances**。
  - **每個 instance 代表 125 MHz 頻寬**。
  - 計算方式 : 2 GHz / 0.125 GHz = 16
- 所有 **GW** 使用相同的頻寬。
- 最多 5 個 **GW** 共用同一個 channel instance



## Random access (RA) in DVB-RCS2

主要有三種 RA 模式 (DVB-RCS2)

- **CRDSA (Contention Resolution Diversity Slotted ALOHA)** : 用於**初始資料存取**和**緊急傳輸**。
- **Slotted ALOHA** : 只用在 **control messages**，payload 小。
- **MARSALA (Multi-Replica Decoding using Correlation based Localisation)** : 在 CRDSA **SIC (successive interference cancellation) cycle 結束後進一步改善性能**。

### CRDSA
**DVB-RCS2 的核心 RA 技術**，為 **UT 提供高效率的存取**。

#### use case : 

- **RA Cold Start** : UT 開機時的初始請求(沒有任何 DA 資源)。
  - 如果只用 **Slotted ALOHA** → 碰撞太嚴重、效率太差
  - GW / Satellite 端做 SIC（successive interference cancellation），大幅提升吞吐量
- **RA-DAMA Top-Up** : 配額用完前的額外容量， DA 不夠用（流量突然增加）。
- **RA-DAMA Back-Up** : 備份通訊，避免 DA 暫時不可用或失效。
- **RA IP Queue** : 對已排程的IP資料進行 Random access。
- **RA Capacity Request** : 一般頻寬請求。
- **RA for SCADA** : 用於工業遠端監控系統 (industrial remote monitoring systems)。

#### Key Feature 
- 發送多個資料包副本以提高成功率。
- 使用 **SIC (Successive Interference Cancellation) 技術解決衝突**。
- 針對 **RA Cold Start** 進行了最佳化，以減少在沒有 dedicated resources 可用時的封包延遲。

### Slotted ALOHA
由於 payload 小，只用於**control messages**。
**control messages** 包含：
- **Capacity Request (CR)**
- [**ARQ ACK** control messages](#arq-automatic-repeat-request)

### MARSALA Enhancement

- 當 **CRDSA 的 SIC decoding fails** 時啟動。
- 提高 **解碼精度(decoding accuracy)** 並**減少掉封包(packet loss)**。



## Return link packet scheduling
在DVB-RCS2，**Return Link Scheduler** 由 **Network Control Center (NCC)** 以beam為單位集中管理。

每個 **spot-beam 有獨立的scheduler**，且之間不會相互影響

### Time Slot Configuration Modes
|Mode|	Description|
|---|---|
|Conf-0	|  **固定時槽(time slots) + 固定 waveform**(MODCOD + burst length 固定)|
|Conf-1	|  **固定時槽(time slots) + 可變 MODCOD(ACM)**          |
|Conf-2	|  根據 **user demand, channel conditions 動態生成time slot**，每個 slot 可用不同 waveform|

### MODCOD and Waveform Support
- waveforms : 3 to 22
- MODCOD range : QPSK 1/3 to 16QAM 5/6
- Burst lengths : 536 or 1616 symbols

**GW 量測 C/No (Carrier-to-Noise ratio)**，回報給 **NCC 為每個 UT 選擇 MODCOD** (目標是**光譜效率最好**)

### Six Step Scheduling Procedure

<div align="center">
<img width="1314" height="550" alt="image" src="https://github.com/user-attachments/assets/f8ce7afa-7bdb-44a7-9ab4-f23b9f5de823" />
    <p align="center"><strong>Figure 10.</strong> Six step scheduling procedure </p>
</div>

## Demand Assignment Multiple Access (DAMA)
DAMA 在 **Request Manager (RM)** 模組中實現，根據需求 **動態評估**和**分配頻寬**。按照 **RC index (resource configuration index)** 實作，每個index都有單獨的設定。

- DAMA：決定**每個 UT 應該拿多少容量**
- Return-link scheduler : 把這些容量**轉成 time slots + TBTP** 


### 支援的容量分配類型(Capacity Allocation Types)
- **CRA (Constant Rate Assignment**) : **固定、長期頻寬需求**
- **RBDC (Rate-Based Dynamic Capacity)** : 根據**當前速率需求**調整，**適合持續性資料流**
- **VBDC (Volume-Based Dynamic Capacity)** : 依 **queue volume 分配**，適合 **bursty traffic**

### Configuration and Operation
- **Request Manager (RM)**  透過**底層服務參數(lower-layer service parameters)進行設定**。
- 每個 RC index 都可以獨立配置以進行 DAMA 操作。

### Evaluation and Capacity Request Process

- Request Manager (RM) 會 **定期或按需評估** 是否需要**針對給定的 RC index 發送 Capacity Request (CR)**
- Request Manager (RM) 監控 :
  - **UT packet queues** (user terminal)
  - **Incoming traffic rates**
  - 先前從 TBTPs (Terminal Burst Time Plans) 接收的 **DA resources**
- CRs(Capacity Request) are modeled as **real signaling messages**, with a **probability of transmission error considered**(考慮傳送錯誤機率).

## UT Scheduler
在`SatUtMac`中，依照 **NCC 下發的 TBTP**，把本地 queue 的資料放進指定的 timeslots。
- Primary Operation :
  The UT scheduler primarily **follows the RC (Return Channel) indices defined in the received TBTP messages**.

- Exception Handling :
  If **no packets are available in the RLE (Return Link Encapsulation) queue** for a given RC index, the UT scheduler is allowed to **freely choose another RC index to serve**, **ensuring resource efficiency and avoiding idle time slots**.

The UT scheduler combines centralized guidance from NCC with local flexibility. 

在正常情況下，它遵循NCC指令，但在 **TBTP 指定的 RC index 對應的 RLE encapsulator/queue 內沒有封包**，UT scheduler 有自由度選擇別的 RC index 來服務。


## FWD Link Scheduler
在`SatGwMac`中，負責將前向鏈路（GW → SAT → UT）的資料組織成 BBFrame，並排定下行傳送時序。

- **Periodically constructs multiple Baseband (BB) frames** and **fills them with GSE (Generic Stream Encapsulation) packets from the LLC in priority order**.
- **Assigns optimal MODCOD (Modulation and Coding) for each BB frame** based on the **UT-reported C/No** (Carrier-to-Noise ratio).
- After each scheduling round, may **downgrade the MODCOD** to **reduce the number of required BB frames and enhance spectral efficiency(頻譜效率)**.

## ARQ (Automatic Repeat reQuest)
ARQ 是在回傳鏈路（RTN）上的一種**資料層可靠傳輸機制**，用來在錯誤發生時要求重傳 burst，採用**Stop-and-Wait ARQ**，操作於[Slotted ALOHA](#random-access-ra-in-dvb-rcs2)

ARQ 不再 DVB-RCS2 官方規範內，

出於研究目的，衛星模組中實現了 **Selective Repeat ARQ mechanism**

ARQ **operates at the LLC layer**, and supports retransmissions for :\
- FWD link: using **GSE(Generic Stream Encapsulation) packets** 
- RTN link: using **RLE(Return Link Encapsulation) packets**


## Mobility and Handover

#### Two mobility models are implemented for UTs (User Terminals) :

- **Static Mode** : UT 位置不變。
- **Trace-Based Mode** : UT 的位置來自**位置序列的 trace file**, 以檔案中**相鄰兩個位置點**做**線性插值（linear interpolation）**，在需要時算出中間位置。
  當 UT 走到 trace file 的最後一個位置後，UT 就停在最後位置直到模擬結束

#### handover module 可以 attach 到任何 UT。

- handover module 會監控周圍 carriers 的功率
- 當 UT 目前**鎖定的 carrier功率掉到低於預設門檻（predefined threshold）** 
- UT 的 handover module 會向 NCC 請求授權，希望切換到其他的 carrier/beam
- NCC 授權 handover ，確保通訊連續性。
