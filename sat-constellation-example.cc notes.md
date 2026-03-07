# sat-constellation-example.cc
> refrence : https://github.com/sns3/sns3-satellite/blob/master/examples/sat-constellation-example.cc

# Table of Contents 
- [Table of Contents](#table-of-contents)
- [Step](#step)
- [修改LEO環境](#修改leo環境)
- [topology](#topology)

## Step
```
cd ~/workspace/bake/source/ns-3.43
./ns3 run "sat-constellation-example --PrintHelp"
```
### Output


| 參數名稱 | 預設值      | 參數用途說明 |
| ---| ---| ---|
| `packetSize`            | `512`| Size of constant packet (bytes) |
| `--interval`         | `20ms`     | Interval to sent packets in seconds (e.g. (1s))                               |
| `--scenarioFolder`          | `constellation-eutelsat-geo-2-sats-isls`     | Scenario folder (e.g. constellation-eutelsat-geo-2-sats-isls) [constellation-leo-2-satellites]     |
| ` --OutputPath`     |  未指定 | 指定輸出統計檔案的資料夾   |


## 修改LEO環境

*若要使用LEO環境，可以透過程式碼修改，也可以透過參數修改*

[code](https://github.com/sns3/sns3-satellite/blob/master/examples/sat-constellation-example.cc) Line 48 將

```
std::string scenarioFolder = "constellation-eutelsat-geo-2-sats-isls";
```
修改為
```
std::string scenarioFolder = "constellation-leo-2-satellites";
```

> Refrence : https://github.com/sns3/sns3-data/tree/master/scenarios

---

### 執行程式碼
```
mkdir -p results/leocon-exp1                                      
./ns3 run sat-constellation-example -- --OutputPath=results/leocon-exp1
```
### Output

### 1.SATs
```
Satellites                                          # 模擬中的衛星列表
  SAT: ID = 0, at 4.39344,113.854,415387            # 衛星ID = 0。座標 (緯度 ,經度 ,高度)
    Devices to ground stations                      # 衛星上連接地面站的通訊設備
      02-06-00:00:00:00:00:01                       # 該通訊介面的 MAC 位址
        Feeder at 02-06-00:00:00:00:00:03, beam 30  # Feeder Terminal 在位址結尾為 03 ，使用beam 30
        Feeder at 02-06-00:00:00:00:00:06, beam 43  # Feeder Terminal 在位址結尾為 06 ，使用beam 43
      Feeder connected to                           # 連接 Feeder Link 的節點
        00:00:00:00:00:05                           # 地面節點 MAC 位址
        00:00:00:00:00:08
        00:00:00:00:00:11
        00:00:00:00:00:1f
        User at 02-06-00:00:00:00:00:04, beam 30    # User Terminal 在位址結尾為 04，使用 beam 30
        User at 02-06-00:00:00:00:00:07, beam 43    # User Terminal 在位址結尾為 07，使用 beam 43
      User connected to                             # Users linked to downstream devices
    ISLs                                            # Inter-Satellite Links：星際鏈路（衛星與衛星之間的通訊）
      02-06-00:00:00:00:00:2b to SAT 1              # 透過 MAC 結尾為 2b 的介面，與 SAT 1 進行連線
```
```
  SAT: ID = 1, at 17.3625,124.686,413074            # 衛星ID = 0。座標 (緯度 ,經度 ,高度)
    Devices to ground stations                      # 衛星上連接地面站的通訊設備 
      02-06-00:00:00:00:00:02                       # 該通訊介面的 MAC 位址
        Feeder at 02-06-00:00:00:00:00:0f, beam 30  # Feeder Terminal 在位址結尾為 0f ，使用beam 30
        Feeder at 02-06-00:00:00:00:00:1d, beam 43  # Feeder Terminal 在位址結尾為 1d ，使用beam 43
      Feeder connected to                           # 連接 Feeder Link 的節點
        User at 02-06-00:00:00:00:00:10, beam 30    # beam 30 覆蓋的用戶 :10
        User at 02-06-00:00:00:00:00:1e, beam 43    # beam 43 覆蓋的用戶 :1e
      User connected to                             # 連接到 SAT 1 User Link的終端設備 MAC 列表。
        00:00:00:00:00:12                           
        00:00:00:00:00:13
        00:00:00:00:00:20
        00:00:00:00:00:21
        00:00:00:00:00:22
    ISLs                                            # Inter-Satellite Links：星際鏈路（衛星與衛星之間的通訊） 
      02-06-00:00:00:00:00:2c to SAT 0              # 透過 MAC 結尾為 2c 的介面，與 SAT 0 進行連線
```
### 2.GWs
```
GWs                                                 # Ground stations (Gateways) 資訊
  GW: ID = 2, at 17.69,101.62,0                     # Gateway with ID=2，座標 (緯度, 經度 ,高度)
  Devices 
    02-06-00:00:00:00:00:08, sat: 0, beam: 43       # 介面 MAC 位址為 08。目前正與 SAT0 透過 beam 43 連接。
    02-06-00:00:00:00:00:1f, sat: 1, beam: 43       # 介面 MAC 位址為 1f。目前正與 SAT1 透過 beam 43 連接。
  GW: ID = 3, at 15.93,96.54,-9.31323e-10           # Gateway with ID=3，座標 (緯度, 經度 ,高度)
  Devices 
    02-06-00:00:00:00:00:05, sat: 0, beam: 30       # 介面 MAC 位址為 05。目前正與 SAT0 透過 beam 30 連接。
    02-06-00:00:00:00:00:11, sat: 1, beam: 30       # 介面 MAC 位址為 11。目前正與 SAT1 透過 beam 30 連接。
```

### 3.UTs
```
UTs                                                    # User Terminals (UT) 資訊
  UT: ID = 4, at 20,110,0                              # User Terminal with ID=4，座標 (緯度, 經度 ,高度)
  Devices 
    02-06-00:00:00:00:00:12, sat: 1, beam: 30. Linked to GW 02-06-00:00:00:00:00:05    # UT MAC 位址為 :12，透過beam 30 連線至 SAT 1 。連線到 GW 3 (MAC :05)
  UT: ID = 5, at 21,111,0                              # User Terminal with ID=5，座標 (緯度, 經度 ,高度)
  Devices 
    02-06-00:00:00:00:00:13, sat: 1, beam: 30. Linked to GW 02-06-00:00:00:00:00:05    # UT MAC 位址為 :13，透過beam 30 連線至 SAT 1 。連線到 GW 3 (MAC :05)
  UT: ID = 10, at 18,115,0                             # User Terminal with ID=10，座標 (緯度, 經度 ,高度)
  Devices 
    02-06-00:00:00:00:00:20, sat: 1, beam: 43. Linked to GW 02-06-00:00:00:00:00:08    # UT MAC 位址為 :20，透過beam 43 連線至 SAT 1 。連線到 GW 2 (MAC :08)
  UT: ID = 11, at 19,110,9.31323e-10                   # User Terminal with ID=11，座標 (緯度, 經度 ,高度)
  Devices 
    02-06-00:00:00:00:00:21, sat: 1, beam: 43. Linked to GW 02-06-00:00:00:00:00:08    # UT MAC 位址為 :21，透過beam 43 連線至 SAT 1 。連線到 GW 2 (MAC :08)
  UT: ID = 12, at 15,120,0                             # User Terminal with ID=12，座標 (緯度, 經度 ,高度)
  Devices 
    02-06-00:00:00:00:00:22, sat: 1, beam: 43. Linked to GW 02-06-00:00:00:00:00:08    # UT MAC 位址為 :22，透過beam 43 連線至 SAT 1 。連線到 GW 2 (MAC :08)
```

### 4.GW users & UT users
```
GW users
  GW user: ID = 20
  GW user: ID = 21
  GW user: ID = 22
UT users
  GW user: ID = 6
  GW user: ID = 7
  GW user: ID = 8
  GW user: ID = 9
  GW user: ID = 13
  GW user: ID = 14
  GW user: ID = 15
  GW user: ID = 16
  GW user: ID = 17
  GW user: ID = 18
```
- GW users (IDs 20-22) → 代表GW的終端主機應用程式。
- UT users (IDs 6-18) →  代表UT的終端主機應用程式。
- 這些 ID 並非實體設備，而是**邏輯應用程式節點(logical application nodes)**。

### 察看結果檔
```
cd ~/workspace/bake/source/ns-3.43/results/leocon-exp1
ls
```
<img width="1464" height="820" alt="image" src="https://github.com/user-attachments/assets/8db817e0-88de-47d0-9882-c3187e3decad" />


<img width="647" height="569" alt="image" src="https://github.com/user-attachments/assets/0a105803-2885-4fcd-96d2-e6efe8abe221" />

## topology

> refernce:https://github.com/sns3/sns3-data/tree/master/scenarios/constellation-leo-2-satellites/positions


### 一、[位置設定檔](https://github.com/sns3/sns3-data/tree/master/scenarios/constellation-leo-2-satellites/positions)
- [isls.txt](https://github.com/sns3/sns3-data/blob/master/scenarios/constellation-leo-2-satellites/positions/isls.txt)
```
1
0 1
```
| 行數 | 參數意思 | 
| ---| ---| 
| 1 | 有1條isl |
| 2 | 這條isl是連接sat0 sat1 |

- [tles.txt](https://github.com/sns3/sns3-data/blob/master/scenarios/constellation-leo-2-satellites/positions/tles.txt)
```
2
0 ISS Zarya
1 25544U 98067A   14273.50403866  .00012237  00000-0  21631-3 0  1790
2 25544  51.6467 297.5710 0002045 126.1182  27.2142 15.50748592907666
1 ISS Zarya 2
1 25544U 98067A   14273.50103866  .00012237  00000-0  21631-3 0  1790
2 25544  51.6467 297.5710 0002045 126.1182  27.2142 15.50748592907666
```
| 行數 | 參數意思 | 
| ---| ---|
| 1 | 有兩顆衛星 |
| 2 | sat0是採用ISS Zarya的TLE數據 |
| 3 | sat0的參數(衛星編號,分類,國際設計者代碼,曆元,平均運動一階導數,阻力項) |
| 4 | sat0的參數(軌道傾角,升交點赤經,軌道偏心率,近地點幅角,平近點角,平均運動) |
| 5 | sat1是採用ISS Zarya的TLE數據 |
| 6 | sat1的參數(衛星編號,分類,國際設計者代碼,曆元,平均運動一階導數,阻力項) |
| 7 | sat1的參數(軌道傾角,升交點赤經,軌道偏心率,近地點幅角,平近點角,平均運動) |

- [gw_position.txt](https://github.com/sns3/sns3-data/blob/master/scenarios/constellation-leo-2-satellites/positions/gw_positions.txt)
```
17.69 101.62 0.0
15.93 96.54 0.0
```
| 行數 | 參數意思 | 
| ---| ---|
| 1 | GW ID = 2的北緯東經高度 |
| 2 | GW ID = 3的北緯東經高度 |

- [UT_positions.txt](https://github.com/sns3/sns3-data/blob/master/scenarios/constellation-leo-2-satellites/positions/ut_positions.txt)
```
20 110 0.0
21 111 0.0
18 115 0.0
19 110 0.0
15 120 0.0
```
| 行數 | 參數意思 | 
| ---| ---|
| 1 | UT ID = 4的北緯東經高度 |
| 2 | UT ID = 5的北緯東經高度 |
| 3 | UT ID = 10的北緯東經高度 |
| 4 | UT ID = 11的北緯東經高度 |
| 5 | UT ID = 12的北緯東經高度 |

### 二、[sat-constellation-examples.cc](https://github.com/sns3/sns3-satellite/blob/master/examples/sat-constellation-example.cc)

<div align="center">

<img width="717" height="193" alt="image" src="https://github.com/user-attachments/assets/e8130656-6e2f-4db0-9c82-c4263b7db876" />

執行流程圖
</div>

- 1.參數初始化與全域設定
  - [設定模擬場景與封包參數](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L46)
  ```
    uint32_t packetSize = 512;                                     #封包大小為512bytes
    std::string interval = "20ms";                                 #傳輸時間間隔為20ms
    std::string scenarioFolder = "constellation-leo-2-satellites"; #使用的場景為LEO
  ```
  - [啟動再生網路模式](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L64-L65)
  ```
  Config::SetDefault("ns3::SatConf::ForwardLinkRegenerationMode",  #如果沒有規定
                       EnumValue(SatEnums::REGENERATION_NETWORK));
  ```
  - [設定 ISL 鏈路頻寬](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L72)
  ```
   Config::SetDefault("ns3::PointToPointIslHelper::IslDataRate",
                       DataRateValue(DataRate("100Mb/s")));
  ```
- 2.拓撲建構與場景資源加載
- 3.流量路徑與統計配置
- 4.模擬執行與輸出



