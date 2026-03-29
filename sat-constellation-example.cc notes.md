# sat-constellation-example.cc
> refrence : https://github.com/sns3/sns3-satellite/blob/master/examples/sat-constellation-example.cc

# Table of Contents 
- [說明](#說明)

- [topology](#topology)
- [routing](#routing)
## 一、說明
sat-constellation-example.cc 是一個 ns-3 衛星網路模擬的範例腳本
## 二、輸入及輸出
### 1.拓樸與環境輸入
輸入：  
輸入TLE：透過scenarioFolder讀取衛星軌道參數  
SGP4 模型：透過SGP4模型每1秒更新一次位置  
ISL 頻寬：100MB  
輸出：  
PrintTopology(sat-topology.cc)：ISL連結圖  
ShowIslMap(sat-id-mapper.cc)：印出整個星座的層級結構
### 2.流量輸入
輸入：  
packetSize：封包大小(預設 512 bytes)  
interval：發送頻率(預設 20ms)  
FWD_LINK/RTN_LINK：分別建立前向鏈路與反向鏈路的流量  
輸出：  
throughput資料  
| fwd | rtn | 
| ---| ---| 
| <img width="1000" height="600" alt="image" src="https://github.com/user-attachments/assets/a85b0fb7-c95d-4a0d-82da-a585a0565b55" /> | <img width="1000" height="600" alt="image" src="https://github.com/user-attachments/assets/1d7205ce-0812-4c00-84d4-7c3ad499fb8d" /> |

#### 重要程式碼
設定模擬時間:
```
 simulationHelper->SetSimulationTime(Seconds(30));
```
封包傳輸時間
```
Time startTime = Seconds(1.0);
Time stopTime = Seconds(29.0);
```


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
### 衛星位置更新時間 
```
Config::SetDefault("ns3::SatSGP4MobilityModel::UpdatePositionPeriod", TimeValue(Seconds(1)));
```
 
## RTN routing
> reference:https://github.com/AvisHuang/sns3/blob/main/global-routing.h

### USER->UT Routing
<div align="center">
<img width="559" height="474" alt="image" src="https://github.com/user-attachments/assets/e9db24f7-2272-49f8-b0a7-505c980f1fef" />  
</div>

```
Ptr<Ipv4StaticRouting> routing = ipv4RoutingHelper.GetStaticRouting(ipv4);//從這節點會去get靜態路由表
routing->SetDefaultRoute(addresses.GetAddress(0), 1);                     //預設的路徑(下一跳節點ip 通常是衛星,要送封包出去這個節點的介面)
NS_LOG_INFO("User default route: " << addresses.GetAddress(0));           //在終端機印出預設的路徑
```

### UT->Satellite
<div align="center">
<img width="560" height="583" alt="image" src="https://github.com/user-attachments/assets/004cd48f-566a-4ac1-8454-0181e64d8636" />  
</div>

```
for (uint32_t j = 1; j < count; j++)
{
    std::string devName = ipv4Ut->GetNetDevice(j)->GetInstanceTypeId().GetName();

    if (devName == "ns3::SatNetDevice" || devName == "ns3::SatLorawanNetDevice") //識別衛星網卡：如果是衛星設備 (SatNetDevice)

    {
        Ptr<Ipv4StaticRouting> srUt = ipv4RoutingHelper.GetStaticRouting(ipv4Ut);
        
        srUt->SetDefaultRoute(gwAddr, j);        //  設定導航：將預設出口設為地面站的 IP (gwAddr)

        
        NS_LOG_INFO("UT default route: " << gwAddr);

      
        ipv4Ut->GetInterface(j)->SetArpCache(utArpCache);    // 設定 ARP 快取：讓 UT 知道地面站的 MAC 地址
    }
}
```

### SAT->GW
因為是採用transparent mode 所以衛星不會有routing table
其封包路徑為
```
srUt->SetDefaultRoute(gwAddr, j);
```
### GW->地面路由器
<div align="center">
<img width="572" height="585" alt="image" src="https://github.com/user-attachments/assets/ea4458da-2797-4ad6-aeaa-a4638181789e" />
</div>

[reference](https://github.com/AvisHuang/sns3/blob/7337834b7f52ef22ffad8cceace44bc5c8f8f019/satellite-user-helper.cc#L322)

```
routingGw->SetDefaultRoute(addresses.GetAddress(1), lastGwIf);
```
### OSPFv2
- 一種網路協議,負責負責「收集資訊」和「制定規則」
  - 鄰居發現:發送hello封包確認路由器與誰連接
  - LSA泛洪：Router會交換LSA(包括:連到誰距離、彼此距離)
  - SPF計算：藉由LSA建立拓撲資料庫(LSDB)

### routing演算法
sns3是採用靜態指派的方法做路徑的規劃

1.多撥routing
```
void
SetMulticastRouteToSourceNetworkSatHelper::(Ptr<Node> source, Ptr<Node> dest)
{
    NS_LOG_FUNCTION(this);//degug用

    std::pair<Ptr<NetDevice>, Ptr<NetDevice>> devices;//準備一個容器來裝來源節點與目標節點之間的網卡

    if (FindMatchingDevices(source, dest, devices)) //檢查source(這台衛星)和dest(所要檢查節點)是否有相連接,如果有則放進device容器中
    {
        Ipv4StaticRoutingHelper multicast; //宣告一個helper
        Ptr<Ipv4StaticRouting> staticRouting =
            multicast.GetStaticRouting(source->GetObject<ns3::Ipv4>());   //從這台source(衛星)的IPv4網路系統中，找出靜態路由表 並在之後會進行修改

        // check if default multicast route already exists   定義組撥範圍
        bool defaultMulticastRouteExists = false;     //檢查預設的多播路由是否已經存在,若否之後會去新增一條路徑
        Ipv4Address defMulticastNetwork = Ipv4Address("224.0.0.0"); //多播起始地址
        Ipv4Mask defMulticastNetworkMask = Ipv4Mask("240.0.0.0");   //遮罩：只需要看IP地址的前4位就好（對應二進位的 11110000...）

        for (uint32_t i = 0; i < staticRouting->GetNRoutes(); i++) //會去靜態路油表找出有幾條路徑紀錄,然後去做迴圈
        {
            if (staticRouting->GetRoute(i).GetDestNetwork() == defMulticastNetwork //會去看現在所取出的目的地網路位址是不是剛才設定的 224.0.0.0,如果是就是給多撥群組專用的
                staticRouting->GetRoute(i).GetDestNetworkMask() == defMulticastNetworkMask)//會取出第i條路徑的「網路遮罩」，看看是不是等於設定的 240.0.0.0。
            {
                defaultMulticastRouteExists = true; //如果都有符合就改為TRUE
            }
        }

        // add default multicast route only if it does not exist already   
        if (!defaultMulticastRouteExists)  //如果前面迴圈沒找到符合的路
        {
            multicast.SetDefaultMulticastRoute(source, devices.first);  //helper會去source把預設的組撥ip改成devices裡面的
        }
    }
}
```












