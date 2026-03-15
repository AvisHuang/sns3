# sat-constellation-example.cc
> refrence : https://github.com/sns3/sns3-satellite/blob/master/examples/sat-constellation-example.cc

# Table of Contents 
- [Table of Contents](#table-of-contents)
- [Step](#step)
- [修改LEO環境](#修改leo環境)
- [topology](#topology)
- [routing](#routing)

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

<img width="850" height="250" alt="image" src="https://github.com/user-attachments/assets/e8130656-6e2f-4db0-9c82-c4263b7db876" />

執行流程圖
</div>

- 1.參數初始化與全域設定
  - [設定模擬場景與封包參數](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L46)
  ```
    uint32_t packetSize = 512;                                     #封包大小為512bytes
    std::string interval = "20ms";                                 #傳輸時間間隔為20ms
    std::string scenarioFolder = "constellation-leo-2-satellites"; #宣告之後的場景是用LEO的環境
  ```
  - [啟動再生網路模式](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L64-L65)
  ```
  Config::SetDefault("ns3::SatConf::ForwardLinkRegenerationMode",  #設定全域預設值;指定forwordlink用RegenerationMode
                       EnumValue(SatEnums::REGENERATION_NETWORK)); #把衛星設定成regeneration模式 同時也使衛星具備路由功能
  ```
  - [設定 ISL 鏈路頻寬](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L72)
  ```
   Config::SetDefault("ns3::PointToPointIslHelper::IslDataRate",
                       DataRateValue(DataRate("100Mb/s")));        #把所有衛星之間的ISL速度統一設定為每秒 100Mb
  ```
- 2.拓撲建構與場景資源加載
  - [加載場景資源](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L93)
  ```
  simulationHelper->LoadScenario(scenarioFolder);                  #載入scenario資料夾裡的資料(目前是LEO)會取得beam position waveform standard等資訊
  ```
  - [建立物理拓撲](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/examples/sat-constellation-example.cc#L120)
  ```
  simulationHelper->CreateSatScenario();                           #實際按照scenariofolder的位置把實際的衛星及地面站蓋出來
  ```
    1. 實體節點:([simulation-helper.cc](https://github.com/sns3/sns3-satellite/blob/master/helper/simulation-helper.cc))

        ```
        m_satHelper->LoadConstellationScenario(                      #去讀TLE檔算出裡面有幾顆衛星然後在模擬器把這些衛星建立出來(使用NodeContainer::Create()函式)
        beamInfo,                                                    #負責記錄哪些beam是啟用的
        MakeCallback(&SimulationHelper::GetNextUtUserCount, this));  #會回傳sat_helper要在UT建多少USER(分配用戶)
        ```
    2. 鋪設物理連線([satellite-point-to-point-isl-helper.cc](https://github.com/sns3/sns3-satellite/blob/master/helper/satellite-point-to-point-isl-helper.cc))

       ```
        Ptr<PointToPointChannel> channel = CreateObject<PointToPointChannel> (); #建立一個點對點通道,Ptr<PointToPointChannel>:指標名;channel:網路線的名字;CreateObject:建立物件的指令;<PointToPointChannel>:指定通道類型
        devA->Attach (channel);                                    #將sat0的網卡街道channel上
        devB->Attach (channel);                                    #將sat1的網卡接到channel上
       ```       
  
- 3.流量路徑與統計配置
  - 獲取節點容器
  ```
  NodeContainer gws = Singleton<SatTopology>::Get()->GetGwNodes(); #宣告gw,向衛星拓撲總管取得目前的權限,篩選出所有的gw節點
  NodeContainer uts = Singleton<SatTopology>::Get()->GetUtNodes(); #宣告ut,向衛星拓撲總管取得目前的權限,篩選出所有的ut節點
  NodeContainer gwUsers = Singleton<SatTopology>::Get()->GetGwUserNodes(); #宣告gwuser,向衛星拓撲總管取得目前的權限,篩選出所有的gwuser節點
  NodeContainer utUsers = Singleton<SatTopology>::Get()->GetUtUserNodes(uts); #宣告utuser,向衛星拓撲總管取得目前的權限,篩選出所有的utuser節點
  ```
  - 設定雙向 CBR 流量
  ```
  trafficHelper->AddCbrTraffic(...);
  ```
  - [配置數據收集器](https://github.com/sns3/sns3-satellite/blob/0fc2b8c74f0d9c2b0c3ee4ed132064a40ad2daf1/helper/simulation-helper.cc#L828)
  ```
  m_statContainer->->Add....
  ```
- 4.模擬執行與輸出
  - 執行模擬
  ```
  simulationHelper->RunSimulation();
  ```
 
## routing
### 衛星位置更新時間 
```
Config::SetDefault("ns3::SatSGP4MobilityModel::UpdatePositionPeriod", TimeValue(Seconds(1)));
```
### OSPFv2
- 一種網路協議,負責負責「收集資訊」和「制定規則」
  - 鄰居發現:發送hello封包確認路由器與誰連接
  - LSA泛洪：Router會交換LSA(包括:連到誰距離、彼此距離)
  - SPF計算：藉由LSA建立拓撲資料庫(LSDB)

### routing演算法
使用最短路徑演算法做計算
```
void GlobalRouteManagerImpl::SPFCalculate (Ipv4Address root) {   //GlobalRouteManagerImpl為一個計算路徑的類別;SPFCalculate (Ipv4Address root)為計算最短路徑的函式(shortest path first);Ipv4Address root:現在這顆衛星的Ip
    // 1. 初始化鏈路狀態資料庫（LSDB），確保所有的link狀態標記為未處理
    m_lsdb->Initialize (); 
    
    // 建立一個待選名單
    CandidateQueue candidate; 
    
    // 2. 建立起點節點（根節點），獲取該 IP 對應的 LSA 資訊，這是 Dijkstra 演算法的「起始點」
    v = new SPFVertex (m_lsdb->GetLSA (root)); //傳入現在ip位置 然後去lsdb看跟鄰居的LSA(連線的資訊),SPFVertex為把現在的鏈路再加上標記(距離等等)
    
    // 將起點到自己的距離設為 0，因為出發點就在這裡
    v->SetDistanceFromRoot (0); 
    
    // 開始無窮迴圈，直到所有可達的節點都被計算完畢並加入 SPF 樹
    for (;;) {
        // 3. 探測目前節點 (v) 的所有相連鄰居，計算經過 v 到鄰居的累積距離（Metric），並放入候選名單中
        SPFNext (v, candidate); 
        
        // 如果候選隊列為空，代表所有能到達的節點都已經算完了，或是剩下的節點都不可達，跳出迴圈
        if (candidate.Size () == 0) break; 
        
        // 4. 「貪婪選擇」：從候選隊列中彈出（Pop）目前累積距離最短的節點作為下一個處理對象
        v = candidate.Pop (); 
        
        // 5. 判斷該節點是否為路由器（Router），如果是，則將其正式納入最短路徑樹中
        if (v->GetVertexType () == SPFVertex::VertexRouter) {    //SPFVertex為最短路徑頂點物件,裡面有LSA,節點類型...
            // 將計算好的最短路徑與下一跳（Next Hop）資訊寫入該節點的內部路由條目中
            SPFIntraAddRouter (v); 
        }
    }
}
```

### 寫入路由表
```
void GlobalRouteManagerImpl::SPFIntraAddRouter (SPFVertex* v) {
    // 取得該節點的路由協議 (通常是 Ipv4GlobalRouting)
    Ptr<Ipv4GlobalRouting> gr = router->GetRoutingProtocol ();
    
    // 遍歷所有計算出的出口方向
    for (uint32_t i = 0; i < v->GetNRootExitDirections (); i++) {
        SPFVertex::NodeExit_t exit = v->GetRootExitDirection (i);
        // 這裡就是你看到的：將計算結果增加到路由表中
        gr->AddHostRouteTo (lr->GetLinkData (), exit.first, exit.second);
    }
}
```







