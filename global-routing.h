/*
 * Copyright 2008 University of Washington
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 */

/**
 * \ingroup ipv4Routing
 * \defgroup globalrouting Global Routing.
 
 * ##sns3會在模擬前先針對ip層做靜態路由計算
 * Performs pre-simulation static route computation
 * on a layer-3 IPv4 topology.
 *
 * \section model Model
 *
 * #建好拓樸後，呼叫 PopulateRoutingTables()。
 * 此時模擬器會初始化路由，並把每個節點加上靜態UNICAST FORWARDING TABLE(內含目的地、下一跳、介面)
 * #
 * ns-3 global routing performs pre-simulaion static route computation
 * on a layer-3 IPv4 topology.  The user API from the script level is
 * fairly minimal; once a topology has been constructed and addresses
 * assigned, the user may call ns3::GlobalRouteManager::PopulateRoutingTables()
 * and the simulator will initialize the routing database and set up
 * static unicast forwarding tables for each node.
 *
 *#
 *因為在同一通道的節點會直接視為可以通信，所以建議用在有線的環境下(無線會有誤差)
 *#
 * The model assumes that all nodes on an ns-3 channel are reachable to
 * one another, regardless of whether the nodes can use the channel
 * successfully (in the case of wireless).  Therefore, this model
 * should typically be used only on wired topologies.  Layer-2 bridge
 * devices are supported.  API does not yet exist to control the subset
 * of a topology to which this global static routing is applied.
 *
 *#預設情況下，拓樸改變路由不會自動改
 * 解決方法有二：
 * 1. 設定 RespondToInterfaceEvents=TRUE(介面沒有連線會response)
 * 2. 手動呼叫 RecomputeRoutingTables()。
 *#
 * If the topology changes during the simulation, by default, routing
 * will not adjust.  There are two ways to make it adjust.
 * - Set the attribute Ipv4GlobalRouting::RespondToInterfaceEvents to true
 * - Manually call the sequence of GlobalRouteManager methods to delete global
 *   routes, build global routing database, and initialize routes.
 *   There is a helper method that encapsulates this
 *   (Ipv4GlobalRoutingHelper::RecomputeRoutingTables())
 *
 * \section api API and Usage
 *
 * Users must include ns3/global-route-manager.h header file.  After the
 * IPv4 topology has been built and addresses assigned, users call
 * ns3::GlobalRouteManager::PopulateRoutingTables (), prior to the
 * ns3::Simulator::Run() call.
 *
 * There are two attributes of Ipv4GlobalRouting that govern behavior.
 * - Ipv4GlobalRouting::RandomEcmpRouting
 * - Ipv4GlobalRouting::RespondToInterfaceEvents
 *
 * \section impl Implementation
 * #
 * 跑Dijkstra演算法算出每對節點間的最短距離，然後寫入節點的轉送表
 * #
 * A singleton object, ns3::GlobalRouteManager, builds a global routing
 * database of information about the topology, and executes a Dijkstra
 * Shortest Path First (SPF) algorithm on the topology for each node, and
 * stores the computed routes in each node's IPv4 forwarding table by
 * making use of the routing API in class ns3::Ipv4.
 * 
 *#用OSPFv2的原因是因為可以直接復用現成的OSPF路由計算程式碼
 * The nodes that export data are those that have had an ns3::GlobalRouter
 * object aggregated to them.  The ns3::GlobalRouter can be thought of
 * as a per-node agent that exports topology information to the
 * ns3::GlobalRouteManager.  When it comes time to build the global
 * routing database, the list of nodes is iterated and each node with
 * an ns3::GlobalRouter object is asked to export routing information
 * concerning the links to which it is attached.
 *
 * The format of the data exported conforms to the OSPFv2 standard
 * \RFC{2328}.  In particular, the
 * information is exported in the form of ns3::GlobalLSA objects that
 * semantically match the Link State Advertisements of OSPF.
 *
 * By using a standard data format for reporting topology, existing
 * OSPF route computation code can be reused, and that is what is done
 * by the ns3::GlobalRouteManager.  The main computation functions are
 * ported from the quagga routing suite (https://www.nongnu.org/quagga/).
 *
 */
