/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd
 * Copyright (c) 2018 CNES
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sami Rantanen <sami.rantanen@magister.fi>
 * Author: Mathias Ettinger <mettinger@toulouse.viveris.com>
 */

#include "satellite-user-helper.h"

#include <ns3/csma-helper.h>
#include <ns3/enum.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/ipv4-interface.h>
#include <ns3/ipv4-routing-table-entry.h>
#include <ns3/ipv4-static-routing-helper.h>
#include <ns3/log.h>
#include <ns3/satellite-arp-cache.h>
#include <ns3/satellite-id-mapper.h>
#include <ns3/satellite-mac.h>
#include <ns3/satellite-mobility-observer.h>
#include <ns3/satellite-net-device.h>
#include <ns3/satellite-simple-net-device.h>
#include <ns3/satellite-topology.h>
#include <ns3/satellite-typedefs.h>
#include <ns3/singleton.h>

NS_LOG_COMPONENT_DEFINE("SatUserHelper");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(SatUserHelper);

TypeId
SatUserHelper::GetTypeId(void)
{
    static TypeId tid =
        TypeId("ns3::SatUserHelper")
            .SetParent<Object>()
            .AddConstructor<SatUserHelper>()
            .AddAttribute(
                "BackboneNetworkType",
                "Network used between GW and Router, and between Router and Users in "
                "operator network",
                EnumValue(SatUserHelper::NETWORK_TYPE_SAT_SIMPLE),
                MakeEnumAccessor<SatUserHelper::NetworkType>(&SatUserHelper::m_backboneNetworkType),
                MakeEnumChecker(SatUserHelper::NETWORK_TYPE_SAT_SIMPLE,
                                "SatSimple",
                                SatUserHelper::NETWORK_TYPE_CSMA,
                                "Csma"))
            .AddAttribute("SubscriberNetworkType",
                          "Network used between UTs and Users in subscriber network",
                          EnumValue(SatUserHelper::NETWORK_TYPE_CSMA),
                          MakeEnumAccessor<SatUserHelper::NetworkType>(
                              &SatUserHelper::m_subscriberNetworkType),
                          MakeEnumChecker(SatUserHelper::NETWORK_TYPE_SAT_SIMPLE,
                                          "SatSimple",
                                          SatUserHelper::NETWORK_TYPE_CSMA,
                                          "Csma"))
            .AddAttribute("PropagationDelayGetter",
                          "Callback to retrieve propagation delay models from beam IDs",
                          CallbackValue(),
                          MakeCallbackAccessor(&SatUserHelper::m_propagationDelayCallback),
                          MakeCallbackChecker())
            .AddTraceSource("Creation",
                            "Creation traces",
                            MakeTraceSourceAccessor(&SatUserHelper::m_creationTrace),
                            "ns3::SatTypedefs::CreationCallback");
    return tid;
}

TypeId
SatUserHelper::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

SatUserHelper::SatUserHelper()
    : m_backboneNetworkType(SatUserHelper::NETWORK_TYPE_SAT_SIMPLE),
      m_subscriberNetworkType(SatUserHelper::NETWORK_TYPE_CSMA),
      m_router(0)
{
    NS_LOG_FUNCTION(this);
}

SatUserHelper::~SatUserHelper()
{
    NS_LOG_FUNCTION(this);
}

void
SatUserHelper::SetCsmaQueue(std::string type,
                            std::string name1,
                            const AttributeValue& value1,
                            std::string name2,
                            const AttributeValue& value2,
                            std::string name3,
                            const AttributeValue& value3,
                            std::string name4,
                            const AttributeValue& value4)
{
    NS_LOG_FUNCTION(this << type);

    m_csma.SetQueue(type, name1, value1, name2, value2, name3, value3, name4, value4);
}

void
SatUserHelper::SetCsmaDeviceAttribute(std::string name, const AttributeValue& value)
{
    NS_LOG_FUNCTION(this);
    m_csma.SetDeviceAttribute(name, value);
}

void
SatUserHelper::SetCsmaChannelAttribute(std::string name, const AttributeValue& value)
{
    NS_LOG_FUNCTION(this);
    m_csma.SetChannelAttribute(name, value);
}

void
SatUserHelper::SetUtBaseAddress(const Ipv4Address& network,
                                const Ipv4Mask& mask,
                                const Ipv4Address address)
{
    NS_LOG_FUNCTION(this);

    m_ipv4Ut.SetBase(network, mask, address);
}

void
SatUserHelper::SetGwBaseAddress(const Ipv4Address& network,
                                const Ipv4Mask& mask,
                                const Ipv4Address address)
{
    NS_LOG_FUNCTION(this);

    m_ipv4Gw.SetBase(network, mask, address);
}

void
SatUserHelper::SetBeamBaseAddress(const Ipv4Address& network,
                                  const Ipv4Mask& mask,
                                  const Ipv4Address address)
{
    NS_LOG_FUNCTION(this);

    m_ipv4Beam.SetBase(network, mask, address);
}

NodeContainer
SatUserHelper::InstallUt(NodeContainer ut, uint32_t userCount)
{
    NS_LOG_FUNCTION(this << userCount);

    NodeContainer createdUsers;

    // create users and csma links between UTs and users and add IP routes
    for (NodeContainer::Iterator i = ut.Begin(); i != ut.End(); i++)
    {
        createdUsers.Add(InstallUt(*i, userCount));
    }

    return createdUsers;
}

NodeContainer
SatUserHelper::InstallUt(Ptr<Node> ut, uint32_t userCount)
{
    NS_LOG_FUNCTION(this << userCount);

    if (userCount == 0)
    {
        NS_FATAL_ERROR("User count is zero!!!");
    }

    InternetStackHelper internet;

    NodeContainer users;
    users.Create(userCount);
    NodeContainer utUsers = NodeContainer(ut, users);

    internet.Install(users);

    NetDeviceContainer nd = InstallSubscriberNetwork(utUsers);
    Ipv4InterfaceContainer addresses = m_ipv4Ut.Assign(nd);
    Ipv4StaticRoutingHelper ipv4RoutingHelper;

    for (NodeContainer::Iterator i = users.Begin(); i != users.End(); i++)
    {
        // Add the user and the UT as a new entry to the UT map
        std::pair<std::map<Ptr<Node>, Ptr<Node>>::iterator, bool> ret =
            m_utMap.insert(std::make_pair(*i, ut));
        NS_ASSERT(ret.second);

        // Add the user's MAC address to the global mapper
        NS_ASSERT_MSG((*i)->GetNDevices() == 2,
                      "Failed to get the device to subscriber network in UT user node "
                          << (*i)->GetId());
        // assuming that #0 is for loopback device and #1 is for subscriber network device
        Ptr<NetDevice> dev = (*i)->GetDevice(1);
        Singleton<SatIdMapper>::Get()->AttachMacToUtUserId(dev->GetAddress());

        // Get IPv4 protocol implementations
        Ptr<Ipv4> ipv4 = (*i)->GetObject<Ipv4>();

        // Set default route for users toward satellite (UTs address)
        Ptr<Ipv4StaticRouting> routing = ipv4RoutingHelper.GetStaticRouting(ipv4);
        routing->SetDefaultRoute(addresses.GetAddress(0), 1);
        NS_LOG_INFO("User default route: " << addresses.GetAddress(0));
    }

    m_ipv4Ut.NewNetwork();

    for (NodeContainer::Iterator it = users.Begin(); it != users.End(); it++)
    {
        Singleton<SatTopology>::Get()->AddUtUserNode(*it, ut);
    }

    return users;
}

void
SatUserHelper::InstallGw(uint32_t userCount)
{
    NS_LOG_FUNCTION(this << userCount);

    InternetStackHelper internet;

    if (m_router == nullptr)
    {
        m_router = CreateObject<Node>();
        internet.Install(m_router);
        InstallRouter(m_router);
    }

    // create users and csma links between Router and users and add IP routes
    NodeContainer users;
    users.Create(userCount);
    NodeContainer routerUsers = NodeContainer(m_router, users);

    internet.Install(users);

    NetDeviceContainer nd = InstallBackboneNetwork(routerUsers);
    Ipv4InterfaceContainer addresses = m_ipv4Gw.Assign(nd);
    Ipv4StaticRoutingHelper ipv4RoutingHelper;

    Ptr<Ipv4> ipv4Router = m_router->GetObject<Ipv4>();
    uint32_t lastRouterIf = ipv4Router->GetNInterfaces() - 1;
    Ptr<Ipv4StaticRouting> routingRouter = ipv4RoutingHelper.GetStaticRouting(ipv4Router);
    routingRouter->SetDefaultRoute(addresses.GetAddress(1), lastRouterIf);
    NS_LOG_INFO("Router default route: " << addresses.GetAddress(1));

    for (NodeContainer::Iterator i = users.Begin(); i != users.End(); i++)
    {
        // Add the user's MAC address to the global mapper
        NS_ASSERT_MSG((*i)->GetNDevices() == 2,
                      "Failed to get the device to backbone network in GW user node "
                          << (*i)->GetId());
        // assuming that #0 is for loopback device and #1 is for backbone network device
        Ptr<NetDevice> dev = (*i)->GetDevice(1);
        Singleton<SatIdMapper>::Get()->AttachMacToGwUserId(dev->GetAddress());

        // Get IPv4 protocol implementations
        Ptr<Ipv4> ipv4 = (*i)->GetObject<Ipv4>();

        // Set default route toward router (GW) for users
        Ptr<Ipv4StaticRouting> routing = ipv4RoutingHelper.GetStaticRouting(ipv4);
        routing->SetDefaultRoute(addresses.GetAddress(0), 1);
        NS_LOG_INFO("User default route: " << addresses.GetAddress(0));

        Singleton<SatTopology>::Get()->AddGwUserNode(*i);
    }

    m_ipv4Gw.NewNetwork();
}

bool
SatUserHelper::IsGwUser(Ptr<Node> node) const
{
    NS_LOG_FUNCTION(this);

    bool isGwUser = false;

    NodeContainer gwUsers = Singleton<SatTopology>::Get()->GetGwUserNodes();

    for (NodeContainer::Iterator it = gwUsers.Begin(); ((it != gwUsers.End()) && !isGwUser); it++)
    {
        if (*it == node)
        {
            isGwUser = true;
        }
    }

    return isGwUser;
}

void
SatUserHelper::EnableCreationTraces(Ptr<OutputStreamWrapper> stream, CallbackBase& cb)
{
    NS_LOG_FUNCTION(this);

    TraceConnect("Creation", "SatUserHelper", cb);
}

void
SatUserHelper::InstallRouter(Ptr<Node> router)
{
    // Log記錄(除錯用)
    NS_LOG_FUNCTION(this);

    // 用單個sattopology呼叫getnodes得到所有的地面站並存回gwnodes
    NodeContainer gwNodes = Singleton<SatTopology>::Get()->GetGwNodes();

    // 開始跑迴圈：針對每一個GW，逐一與這台地面Router建立連線
    for (NodeContainer::Iterator i = gwNodes.Begin(); i != gwNodes.End(); i++)
    {
        //  建立一個臨時容器，把目前的 GW與地面路由器放在一起
        NodeContainer gwRouter = NodeContainer((*i), router);

        // 2. 安裝實體骨幹網路：在 GW 與 Router 之間拉一條虛擬的實體線 (Backbone)
        NetDeviceContainer nd = InstallBackboneNetwork(gwRouter);//先建通道 再把兩端節點,再分別節點裝上網卡,接著接到一起;nd為容器 裝雙方的網卡

        // 3. 分配 IP：給剛拉好的這條線兩端發 IP 位址 (GW 拿 .1, Router 拿 .2)
        Ipv4InterfaceContainer addresses = m_ipv4Gw.Assign(nd);//建立address便容器並符合Ipv4InterfaceContainer類別,透過傳入包含兩端網卡的容器nd，讓地址助手m_ipv4Gw自動為這些硬體分配IP並建立介面。

        // 4. 宣告一工具ipv4RoutingHelper
        Ipv4StaticRoutingHelper ipv4RoutingHelper;


        // 從目前的地面站(GW)節點中，提取出其掛載的 IPv4 網路層協定實例，以便後續操作 IP 與路由設定      
        Ptr<Ipv4> ipv4Gw = (*i)->GetObject<Ipv4>();

        // 6. 找出 GW 的最後那張網卡的編號
        uint32_t lastGwIf = ipv4Gw->GetNInterfaces() - 1;//使用ipv4物件呼叫GetNInterfaces()得到全部介面數量

        // 7. 取得 GW 的靜態路由表 (routingGw)
        Ptr<Ipv4StaticRouting> routingGw = ipv4RoutingHelper.GetStaticRouting(ipv4Gw);//使用ipv4RoutingHelper去從gw的ipv4協議棧得到靜態路由協定物件(包括網路目標下一跳出口介面)

        // 8. 讓 GW 將「預設出口」指向地面路由器 (地址為 addresses.GetAddress(1))
        routingGw->SetDefaultRoute(addresses.GetAddress(1), lastGwIf);

        // 記錄 GW 路由設定完成的 Log
        NS_LOG_INFO("GW default route: " << addresses.GetAddress(1));

        /* --- 以下是「設定 Router」的導航邏輯：處理下載 (Forward Link) --- */

        // 9. 內層迴圈：遍歷 GW 身上已有的所有路由路徑，準備同步給地面路由器
        for (uint32_t routeIndex = 0; routeIndex < routingGw->GetNRoutes(); routeIndex++)//宣告一routingindex做為迴圈的變數 使用routingGw物件去取得目前存在的路由總數
        {
            // A. 取得地面路由器節點的 IPv4 大腦
            Ptr<Ipv4> ipv4Router = router->GetObject<Ipv4>();//用router去getobject會得到ipv4類型的協議棧然後存進ipv4router(即可對ipv4類別的物件進行使用,e.g.GetAddress()SetDefaultRoute)

            uint32_t lastRouterIf = ipv4Router->GetNInterfaces() - 1;//會去找出剛剛所加上的router最後的介面

            // C. 取得地面路由器的靜態路由表 (routingRouter)
            Ptr<Ipv4StaticRouting> routingRouter = ipv4RoutingHelper.GetStaticRouting(ipv4Router);//使用路由助手去執行ipv4指令中的get static routing存回routingRouter
            // D. 從 GW 的路由表中，讀取第 routeIndex 條路徑資訊
            Ipv4RoutingTableEntry route = routingGw->GetRoute(routeIndex);//輸入現在現在這條路徑的index並使用getroute去得到該條路徑的資訊

            uint32_t interface = route.GetInterface();//得到出口介面

            // F. 過濾條件：如果這條路不是 Loopback (0)，也不是連往路由器的介面 (lastGwIf)
            // 代表這條路「一定是來自衛星端的用戶網段」！
            if ((interface != 0) && (interface != lastGwIf))//排出interface是自己和gw的interface
            {
                // G. 核心設定：在地面路由器增加一條路徑，指回衛星用戶
                //實際加一條路由規則
                routingRouter->AddNetworkRouteTo(route.GetDest(),//目的地:衛星用戶所在的網段地址。
                                                 route.GetDestNetworkMask(),//該網段的範圍大小(遮罩)
                                                 addresses.GetAddress(0),//下一跳 IP
                                                 lastRouterIf);//出口介面

                NS_LOG_INFO("Router network route:" << route.GetDest() << ", "
                                                 << route.GetDestNetworkMask() << ", "
                                                 << addresses.GetAddress(0));
            }
        }

        // 10. 當前這個 GW 設定完畢，宣告一個新的子網路，準備給下一個 GW 使用 (避免 IP 衝突)
        m_ipv4Gw.NewNetwork();
    }
}
NetDeviceContainer
SatUserHelper::InstallSubscriberNetwork(const NodeContainer& c) const
{
    NS_LOG_FUNCTION(this);

    NetDeviceContainer devs;

    switch (m_subscriberNetworkType)
    {
    case NETWORK_TYPE_SAT_SIMPLE:
        devs = InstallSatSimpleNetwork(c);
        break;

    case NETWORK_TYPE_CSMA:
        devs = m_csma.Install(c);
        break;

    default:
        NS_ASSERT(false);
        break;
    }

    return devs;
}

NetDeviceContainer
SatUserHelper::InstallBackboneNetwork(const NodeContainer& c) const
{
    NS_LOG_FUNCTION(this);

    NetDeviceContainer devs;

    switch (m_backboneNetworkType)
    {
    case NETWORK_TYPE_SAT_SIMPLE:
        devs = InstallSatSimpleNetwork(c);
        break;

    case NETWORK_TYPE_CSMA:
        devs = m_csma.Install(c);
        break;

    default:
        NS_ASSERT(false);
        break;
    }

    return devs;
}

NetDeviceContainer
SatUserHelper::InstallSatSimpleNetwork(const NodeContainer& c) const
{
    NS_LOG_FUNCTION(this);

    NetDeviceContainer devs;
    Ptr<SatSimpleChannel> channel = CreateObject<SatSimpleChannel>();

    for (NodeContainer::Iterator i = c.Begin(); i != c.End(); i++)
    {
        Ptr<SatSimpleNetDevice> device = CreateObject<SatSimpleNetDevice>();
        device->SetAddress(Mac48Address::Allocate());
        (*i)->AddDevice(device);
        device->SetChannel(channel);
        devs.Add(device);
    }

    return devs;
}

std::string
SatUserHelper::GetRouterInfo() const
{
    NS_LOG_FUNCTION(this);

    std::ostringstream oss;

    Address devAddress;
    Ptr<Ipv4> ipv4 = m_router->GetObject<Ipv4>(); // Get Ipv4 instance of the node

    std::vector<Ipv4Address> IPAddressVector;
    std::vector<std::string> devNameVector;
    std::vector<Address> devAddressVector;

    oss << "---  Router info  ---" << std::endl << std::endl;

    for (uint32_t j = 0; j < m_router->GetNDevices(); j++)
    {
        Ptr<NetDevice> device = m_router->GetDevice(j);

        oss << device->GetInstanceTypeId().GetName() << " ";
        oss << device->GetAddress() << " ";
        oss << ipv4->GetAddress(j, 0).GetLocal() << " ";
    }

    return oss.str();
}

Ptr<Node>
SatUserHelper::GetRouter() const
{
    NS_LOG_FUNCTION(this);

    return m_router;
}

void
SatUserHelper::PopulateBeamRoutings(NodeContainer ut,
                                    NetDeviceContainer utNd,
                                    Ptr<Node> gw,
                                    Ptr<NetDevice> gwNd)
{
    Ipv4InterfaceContainer gwAddress = m_ipv4Beam.Assign(gwNd);
    Ipv4Address gwAddr = gwAddress.GetAddress(0);
    NS_LOG_FUNCTION(this << gw << gwNd << gwAddr);

    Ipv4InterfaceContainer utIfs = m_ipv4Beam.Assign(utNd);

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4L3Protocol> ipv4Gw = gw->GetObject<Ipv4L3Protocol>();
    Ptr<Ipv4StaticRouting> srGw = ipv4RoutingHelper.GetStaticRouting(ipv4Gw);

    // Store GW NetDevice for updating routing during handover
    m_gwDevices.insert(std::make_pair(gwNd->GetAddress(), gwNd));

    // Create an ARP entry of the default GW for the UTs in this beam
    Address macAddressGw = gwNd->GetAddress();
    Ptr<SatArpCache> utArpCache = CreateObject<SatArpCache>();
    utArpCache->Add(gwAddr, macAddressGw);
    NS_LOG_INFO("UT ARP entry:  " << gwAddr << " - " << macAddressGw);
    // Store ARP cache for retrieval during handovers
    m_arpCachesToGateway.emplace(macAddressGw, utArpCache);

    // Add the ARP entries of all the UTs in this beam
    // - MAC address vs. IPv4 address
    Ptr<SatArpCache> gwArpCache = CreateObject<SatArpCache>();
    for (uint32_t i = 0; i < utIfs.GetN(); ++i)
    {
        NS_ASSERT(utIfs.GetN() == utNd.GetN());
        Ptr<NetDevice> nd = utNd.Get(i);
        Ipv4Address ipv4Addr = utIfs.GetAddress(i);
        gwArpCache->Add(ipv4Addr, nd->GetAddress());
        NS_LOG_INFO("GW ARP entry:  " << ipv4Addr << " - " << nd->GetAddress());
        // Store UT NetDevice for updating routing during handover
        m_utDevices.insert(std::make_pair(nd->GetAddress(), nd));
    }

    // Set the ARP cache to the proper GW IPv4Interface (the one for satellite
    // link). ARP cache contains the entries for all UTs within this spot-beam.
    ipv4Gw->GetInterface(gwNd->GetIfIndex())->SetArpCache(gwArpCache);
    NS_LOG_INFO("Add ARP cache to GW " << gw->GetId());

    uint32_t utAddressIndex = 0;

    for (NodeContainer::Iterator i = ut.Begin(); i != ut.End(); i++)
    {
        Ptr<Ipv4L3Protocol> ipv4Ut = (*i)->GetObject<Ipv4L3Protocol>();

        uint32_t count = ipv4Ut->GetNInterfaces();

        for (uint32_t j = 1; j < count; j++)
        {
            std::string devName = ipv4Ut->GetNetDevice(j)->GetInstanceTypeId().GetName();

            // If SatNetDevice interface, add default route to towards GW of the beam on UTs
            if (devName == "ns3::SatNetDevice" || devName == "ns3::SatLorawanNetDevice")
            {
                Ptr<Ipv4StaticRouting> srUt = ipv4RoutingHelper.GetStaticRouting(ipv4Ut);
                srUt->SetDefaultRoute(gwAddr, j);
                NS_LOG_INFO("UT default route: " << gwAddr);

                // Set the ARP cache (including the ARP entry for the default GW) to the UT
                ipv4Ut->GetInterface(j)->SetArpCache(utArpCache);
                NS_LOG_INFO("Add the ARP cache to UT " << (*i)->GetId());
            }
            else // add other interface route to GW's Satellite interface
            {
                Ipv4Address address = ipv4Ut->GetAddress(j, 0).GetLocal();
                Ipv4Mask mask = ipv4Ut->GetAddress(j, 0).GetMask();

                srGw->AddNetworkRouteTo(address.CombineMask(mask),
                                        mask,
                                        utIfs.GetAddress(utAddressIndex),
                                        gwNd->GetIfIndex());
                NS_LOG_INFO("GW Network route:  " << address.CombineMask(mask) << ", " << mask
                                                  << ", " << utIfs.GetAddress(utAddressIndex));
            }
        }

        utAddressIndex++;
    }

    m_ipv4Beam.NewNetwork();
}

void
SatUserHelper::UpdateUtRoutes(Address utAddress, Address gwAddress)
{
    NS_LOG_FUNCTION(this << utAddress << gwAddress);

    std::map<Address, Ptr<NetDevice>>::iterator gwNdIterator = m_gwDevices.find(gwAddress);
    NS_ASSERT_MSG(gwNdIterator != m_gwDevices.end(), "Unknown GW with MAC address " << gwAddress);

    Ptr<SatNetDevice> gwNd = DynamicCast<SatNetDevice>(gwNdIterator->second);
    NS_ASSERT(gwNd != nullptr);
    Ipv4Address ip =
        gwNd->GetNode()->GetObject<Ipv4L3Protocol>()->GetAddress(gwNd->GetIfIndex(), 0).GetLocal();

    std::map<Address, Ptr<NetDevice>>::iterator utNdIterator = m_utDevices.find(utAddress);
    NS_ASSERT_MSG(utNdIterator != m_utDevices.end(), "Unknown UT with MAC address " << utAddress);

    std::map<Address, Ptr<SatArpCache>>::iterator arpCacheIterator =
        m_arpCachesToGateway.find(gwAddress);
    NS_ASSERT_MSG(arpCacheIterator != m_arpCachesToGateway.end(),
                  "ARP cache not found to gateway " << gwAddress);

    Ptr<SatNetDevice> utNd = DynamicCast<SatNetDevice>(utNdIterator->second);
    NS_ASSERT(utNd != nullptr);
    Ptr<Ipv4L3Protocol> protocol = utNd->GetNode()->GetObject<Ipv4L3Protocol>();
    uint32_t utIfIndex = utNdIterator->second->GetIfIndex();

    NS_LOG_INFO("Changing ARP cache for UT " << utAddress << " pointing to " << ip << " through "
                                             << gwAddress);
    protocol->GetInterface(utIfIndex)->SetArpCache(arpCacheIterator->second);

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> routing = ipv4RoutingHelper.GetStaticRouting(protocol);
    routing->RemoveRoute(routing->GetNRoutes() - 1);
    routing->SetDefaultRoute(ip, utIfIndex);

    NS_LOG_INFO("Set default route on UT to " << ip);

    uint32_t satId = gwNd->GetMac()->GetSatId();
    uint32_t beamId = gwNd->GetMac()->GetBeamId();
    Ptr<PropagationDelayModel> flDelayModel =
        m_propagationDelayCallback(satId, beamId, SatEnums::RETURN_FEEDER_CH);
    Ptr<PropagationDelayModel> ulDelayModel =
        m_propagationDelayCallback(satId, beamId, SatEnums::RETURN_USER_CH);
    Ptr<SatMobilityObserver> observer = utNd->GetNode()->GetObject<SatMobilityObserver>();
    observer->ObserveTimingAdvance(ulDelayModel,
                                   flDelayModel,
                                   gwNd->GetNode()->GetObject<SatMobilityModel>());
}

void
SatUserHelper::UpdateGwRoutes(Address ut, Address oldGateway, Address newGateway)
{
    NS_LOG_FUNCTION(this << ut << oldGateway << newGateway);

    std::map<Address, Ptr<NetDevice>>::iterator utNdIterator = m_utDevices.find(ut);
    if (utNdIterator == m_utDevices.end())
    {
        NS_FATAL_ERROR("Unknown UT with MAC address " << ut);
    }

    std::map<Address, Ptr<NetDevice>>::iterator oldGwNdIterator = m_gwDevices.find(oldGateway);
    if (oldGwNdIterator == m_gwDevices.end())
    {
        NS_FATAL_ERROR("Unknown GW with MAC address " << oldGateway);
    }

    std::map<Address, Ptr<NetDevice>>::iterator newGwNdIterator = m_gwDevices.find(newGateway);
    if (newGwNdIterator == m_gwDevices.end())
    {
        NS_FATAL_ERROR("Unknown GW with MAC address " << newGateway);
    }

    uint32_t utIfIndex = utNdIterator->second->GetIfIndex();
    Ptr<Ipv4L3Protocol> utProtocol = utNdIterator->second->GetNode()->GetObject<Ipv4L3Protocol>();
    Ipv4Address utIpAddress = utProtocol->GetAddress(utIfIndex, 0).GetLocal();

    Ptr<Node> oldGatewayNode = oldGwNdIterator->second->GetNode();
    uint32_t oldIfIndex = oldGwNdIterator->second->GetIfIndex();
    Ptr<Node> newGatewayNode = newGwNdIterator->second->GetNode();
    uint32_t newIfIndex = newGwNdIterator->second->GetIfIndex();

    Ptr<ArpCache> arpCache;
    // Clear old ARP cache
    arpCache = oldGatewayNode->GetObject<Ipv4L3Protocol>()->GetInterface(oldIfIndex)->GetArpCache();
    for (ArpCache::Entry* entry : arpCache->LookupInverse(ut))
    {
        arpCache->Remove(entry);
    }
    // Add entry in new ARP cache
    arpCache = newGatewayNode->GetObject<Ipv4L3Protocol>()->GetInterface(newIfIndex)->GetArpCache();
    ArpCache::Entry* entry = arpCache->Add(utIpAddress);
    entry->SetMacAddress(ut);
    entry->MarkPermanent();

    // Change routes on GW
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    if (oldGatewayNode == newGatewayNode)
    {
        // intra-GW handover
        Ptr<Ipv4StaticRouting> routing =
            ipv4RoutingHelper.GetStaticRouting(oldGatewayNode->GetObject<Ipv4L3Protocol>());

        // purge old routes
        for (uint32_t routeIndex = routing->GetNRoutes(); routeIndex > 0; --routeIndex)
        {
            // Note: keeping routeIndex 1-off because we are using unsigned values
            if (routing->GetRoute(routeIndex - 1).GetGateway() == utIpAddress)
            {
                routing->RemoveRoute(routeIndex - 1);
            }
        }

        // add new ones
        for (uint32_t ifIndex = 1; ifIndex < utProtocol->GetNInterfaces(); ++ifIndex)
        {
            Ipv4Address address = utProtocol->GetAddress(ifIndex, 0).GetLocal();
            Ipv4Mask mask = utProtocol->GetAddress(ifIndex, 0).GetMask();

            if (ifIndex == utIfIndex)
            {
                mask = Ipv4Mask("/32");
            }

            routing->AddNetworkRouteTo(address.CombineMask(mask), mask, utIpAddress, newIfIndex);
        }
    }
    else
    {
        // inter-GW handover
        Ptr<Ipv4StaticRouting> routing =
            ipv4RoutingHelper.GetStaticRouting(oldGatewayNode->GetObject<Ipv4L3Protocol>());
        Ptr<Ipv4StaticRouting> routingRouter =
            ipv4RoutingHelper.GetStaticRouting(m_router->GetObject<Ipv4L3Protocol>());

        // purge old routes
        for (uint32_t routeIndex = routing->GetNRoutes(); routeIndex > 0; --routeIndex)
        {
            // Note: keeping routeIndex 1-off because we are using unsigned values
            Ipv4RoutingTableEntry gwRoute = routing->GetRoute(routeIndex - 1);
            if (gwRoute.GetGateway() == utIpAddress)
            {
                routing->RemoveRoute(routeIndex - 1);
                // search for corresponding route on terrestrial router
                for (uint32_t routerIndex = 0; routerIndex < routingRouter->GetNRoutes();
                     ++routerIndex)
                {
                    Ipv4RoutingTableEntry route = routingRouter->GetRoute(routerIndex);
                    if (route.GetDestNetwork() == gwRoute.GetDestNetwork() &&
                        route.GetDestNetworkMask() == gwRoute.GetDestNetworkMask())
                    {
                        routingRouter->RemoveRoute(routerIndex);
                        break;
                    }
                }
            }
        }

        // add new ones
        Ptr<Ipv4L3Protocol> gwProtocol = newGatewayNode->GetObject<Ipv4L3Protocol>();

        // start by looking up GW IP as seen by the terrestrial router
        Ipv4Address gwAddress;
        for (uint32_t ifIndex = 1; ifIndex < gwProtocol->GetNInterfaces(); ++ifIndex)
        {
            Ptr<NetDevice> gwNd = gwProtocol->GetNetDevice(ifIndex);
            if (gwNd->GetInstanceTypeId().GetName() != "ns3::SatNetDevice" &&
                gwNd->GetInstanceTypeId().GetName() != "ns3::SatLorawanNetDevice")
            {
                gwAddress = gwProtocol->GetAddress(ifIndex, 0).GetLocal();
                break;
            }
        }

        // find interface on the terrestrial router to send messages to GW
        uint32_t routingIfIndex = routingRouter->GetNRoutes();
        for (uint32_t routeIndex = 0; routeIndex < routingRouter->GetNRoutes(); ++routeIndex)
        {
            Ipv4RoutingTableEntry route = routingRouter->GetRoute(routeIndex);
            if (route.GetGateway() == gwAddress)
            {
                routingIfIndex = route.GetInterface();
                break;
            }
        }

        NS_ASSERT_MSG(routingIfIndex != routingRouter->GetNRoutes(),
                      "Couldn't find interface on the terrestrial router to the new gateway.");

        // add routes to the new GW and the terrestrial router
        routing = ipv4RoutingHelper.GetStaticRouting(gwProtocol);
        for (uint32_t ifIndex = 1; ifIndex < utProtocol->GetNInterfaces(); ++ifIndex)
        {
            Ipv4Address address = utProtocol->GetAddress(ifIndex, 0).GetLocal();
            Ipv4Mask mask = utProtocol->GetAddress(ifIndex, 0).GetMask();

            if (ifIndex == utIfIndex)
            {
                mask = Ipv4Mask("/32");
            }

            routing->AddNetworkRouteTo(address.CombineMask(mask), mask, utIpAddress, newIfIndex);
            routingRouter->AddNetworkRouteTo(address.CombineMask(mask),
                                             mask,
                                             gwAddress,
                                             routingIfIndex);
        }
    }
}

} // namespace ns3
