#!/usr/bin/python
import os
import shutil

def gen_config(sleep):
	return """
  # ***** QualNet Configuration File *****


  #********************General Settings***********************************

  VERSION 5.1
  EXPERIMENT-NAME Experiment-2
  EXPERIMENT-COMMENT NONE
  SIMULATION-TIME 15M
  SEED 1

  #*******************Parallel Settings***********************************

  PARTITION-SCHEME AUTO
  GESTALT-PREFER-SHARED-MEMORY YES

  #*******************ATM Configuration***********************************

  DUMMY-ATM-LOGICAL-SUBNET-CONFIGURED NO
  ATM-STATIC-ROUTE NO

  #******************Dynamic Parameters***********************************

  DYNAMIC-ENABLED NO

  #*****************************Terrain***********************************

  COORDINATE-SYSTEM CARTESIAN
  TERRAIN-DIMENSIONS (600, 350)
  DUMMY-ALTITUDES (1500, 0)
  URBAN-TERRAIN-TYPE NONE
  WEATHER-MOBILITY-INTERVAL 100MS

  #******************Channel Properties***********************************

  PROPAGATION-CHANNEL-FREQUENCY[0] 2400000000
  PROPAGATION-MODEL[0] STATISTICAL
  PROPAGATION-PATHLOSS-MODEL[0] TWO-RAY
  PROPAGATION-SHADOWING-MODEL[0] CONSTANT
  PROPAGATION-SHADOWING-MEAN[0] 4.0
  PROPAGATION-FADING-MODEL[0] NONE
  PROPAGATION-SPEED[0] 3e8
  PROPAGATION-LIMIT[0] -111.0
  PROPAGATION-MAX-DISTANCE[0] 0
  PROPAGATION-COMMUNICATION-PROXIMITY[0] 400
  PROPAGATION-PROFILE-UPDATE-RATIO[0] 0.0

  #**************Mobility and Placement***********************************

  NODE-PLACEMENT FILE
  MOBILITY NONE

  #**************************STATISTICS***********************************

  PHY-LAYER-STATISTICS YES
  MAC-LAYER-STATISTICS YES
  ACCESS-LIST-STATISTICS NO
  ARP-STATISTICS YES
  ROUTING-STATISTICS YES
  POLICY-ROUTING-STATISTICS NO
  QOSPF-STATISTICS NO
  ROUTE-REDISTRIBUTION-STATISTICS NO
  EXTERIOR-GATEWAY-PROTOCOL-STATISTICS YES
  NETWORK-LAYER-STATISTICS YES
  INPUT-QUEUE-STATISTICS NO
  INPUT-SCHEDULER-STATISTICS NO
  QUEUE-STATISTICS YES
  SCHEDULER-STATISTICS NO
  SCHEDULER-GRAPH-STATISTICS NO
  DIFFSERV-EDGE-ROUTER-STATISTICS NO
  ICMP-STATISTICS NO
  ICMP-ERROR-STATISTICS NO
  IGMP-STATISTICS NO
  NDP-STATISTICS NO
  MOBILE-IP-STATISTICS NO
  TCP-STATISTICS YES
  UDP-STATISTICS YES
  MDP-STATISTICS NO
  RSVP-STATISTICS NO
  RTP-STATISTICS NO
  APPLICATION-STATISTICS YES
  BATTERY-MODEL-STATISTICS NO
  ENERGY-MODEL-STATISTICS YES
  CELLULAR-STATISTICS YES
  GSM-STATISTICS NO
  VOIP-SIGNALLING-STATISTICS NO
  SWITCH-PORT-STATISTICS NO
  SWITCH-SCHEDULER-STATISTICS NO
  SWITCH-QUEUE-STATISTICS NO
  MPLS-STATISTICS NO
  MPLS-LDP-STATISTICS NO
  HOST-STATISTICS NO

  #**********************PACKET TRACING***********************************

  PACKET-TRACE NO
  ACCESS-LIST-TRACE NO

  #******************Supplemental Files***********************************

  APP-CONFIG-FILE template_double.app
  DUMMY-USER-PROFILE-FILE-NUMBER 0
  DUMMY-TRAFFIC-PATTERN-FILE-NUMBER 0
  DUMMY-ARBITRARY-DISTRIBUTION-FILE-NUMBER 0

  #***********************HLA Interface***********************************

  HLA NO

  #***********************DIS Interface***********************************

  DIS NO

  #***********************AGI Interface***********************************

  AGI-INTERFACE NO

  #*********************EXata Interface***********************************

  VIRTUAL-NODE-TRIANGLE-SCALE-FACTOR-3D 200

  #**********************Physical Layer***********************************

  PHY-LISTENABLE-CHANNEL-MASK 1
  PHY-LISTENING-CHANNEL-MASK 1
  PHY-MODEL PHY802.11b
  PHY802.11-AUTO-RATE-FALLBACK NO
  PHY802.11-DATA-RATE 2000000
  PHY802.11b-TX-POWER--1MBPS 15.0
  PHY802.11b-TX-POWER--2MBPS 15.0
  PHY802.11b-TX-POWER--6MBPS 15.0
  PHY802.11b-TX-POWER-11MBPS 15.0
  PHY802.11b-RX-SENSITIVITY--1MBPS -94.0
  PHY802.11b-RX-SENSITIVITY--2MBPS -91.0
  PHY802.11b-RX-SENSITIVITY--6MBPS -87.0
  PHY802.11b-RX-SENSITIVITY-11MBPS -83.0
  PHY802.11-ESTIMATED-DIRECTIONAL-ANTENNA-GAIN 15.0
  PHY-RX-MODEL PHY802.11b
  DUMMY-ANTENNA-MODEL-CONFIG-FILE-SPECIFY NO
  ANTENNA-MODEL OMNIDIRECTIONAL
  ANTENNA-GAIN 0.0
  ANTENNA-HEIGHT 1.5
  ANTENNA-EFFICIENCY 0.8
  ANTENNA-MISMATCH-LOSS 0.3
  ANTENNA-CABLE-LOSS 0.0
  ANTENNA-CONNECTION-LOSS 0.2
  ANTENNA-ORIENTATION-AZIMUTH 0
  ANTENNA-ORIENTATION-ELEVATION 0
  PHY-TEMPERATURE 290.0
  PHY-NOISE-FACTOR 10.0
  ENERGY-MODEL-SPECIFICATION NONE

  #***************************MAC Layer***********************************

  LINK-MAC-PROTOCOL ABSTRACT
  LINK-PROPAGATION-DELAY 1MS
  LINK-BANDWIDTH 10000000
  LINK-HEADER-SIZE-IN-BITS 224
  LINK-GENERATE-AUTOMATIC-DEFAULT-ROUTE YES
  LINK-TX-FREQUENCY 13170000000
  LINK-RX-FREQUENCY 13170000000
  LINK-TX-ANTENNA-HEIGHT 30
  LINK-RX-ANTENNA-HEIGHT 30
  LINK-TX-ANTENNA-DISH-DIAMETER 0.8
  LINK-RX-ANTENNA-DISH-DIAMETER 0.8
  LINK-TX-ANTENNA-CABLE-LOSS 1.5
  LINK-RX-ANTENNA-CABLE-LOSS 1.5
  LINK-TX-POWER 30
  LINK-RX-SENSITIVITY -80
  LINK-NOISE-TEMPERATURE 290
  LINK-NOISE-FACTOR 4
  LINK-TERRAIN-TYPE PLAINS
  LINK-PROPAGATION-RAIN-INTENSITY 0
  LINK-PROPAGATION-TEMPERATURE 25
  LINK-PROPAGATION-SAMPLING-DISTANCE 100
  LINK-PROPAGATION-CLIMATE 1
  LINK-PROPAGATION-REFRACTIVITY 360
  LINK-PROPAGATION-PERMITTIVITY 15
  LINK-PROPAGATION-CONDUCTIVITY 0.005
  LINK-PROPAGATION-HUMIDITY 50
  LINK-PERCENTAGE-TIME-REFRACTIVITY-GRADIENT-LESS-STANDARD  15
  MAC-PROTOCOL MACDOT11
  MAC-DOT11-SHORT-PACKET-TRANSMIT-LIMIT 7
  MAC-DOT11-LONG-PACKET-TRANSMIT-LIMIT 4
  MAC-DOT11-RTS-THRESHOLD 0
  MAC-DOT11-STOP-RECEIVING-AFTER-HEADER-MODE NO
  MAC-DOT11-ASSOCIATION NONE
  MAC-DOT11-IBSS-SUPPORT-PS-MODE NO
  MAC-DOT11-DIRECTIONAL-ANTENNA-MODE NO
  WORMHOLE-VICTIM-COUNT-TURNAROUND-TIME NO
  MAC-PROPAGATION-DELAY 1US

  #***************Schedulers and Queues***********************************

  IP-QUEUE-PRIORITY-INPUT-QUEUE-SIZE 150000
  IP-QUEUE-SCHEDULER STRICT-PRIORITY
  IP-QUEUE-NUM-PRIORITIES 3

  #*******************QoS Configuration***********************************


  #********************Network Security***********************************

  IPSEC-ENABLED NO

  #****************Fixed Communications***********************************

  FIXED-COMMS-DROP-PROBABILITY 0.0
  DUMMY-FIXED-COMMS-DELAY NO

  #************************ROUTER MODEL***********************************

  DUMMY-ROUTER-TYPE USER-SPECIFIED
  DUMMY-PARAM NO

  #***********************NETWORK LAYER***********************************

  NETWORK-PROTOCOL IP
  IP-ENABLE-LOOPBACK YES
  IP-LOOPBACK-ADDRESS 127.0.0.1
  IP-FRAGMENT-HOLD-TIME 60S
  IP-FRAGMENTATION-UNIT 2048
  ECN NO
  ICMP YES
  ICMP-ROUTER-ADVERTISEMENT-LIFE-TIME 1800S
  ICMP-ROUTER-ADVERTISEMENT-MIN-INTERVAL 450S
  ICMP-ROUTER-ADVERTISEMENT-MAX-INTERVAL 600S
  ICMP-MAX-NUM-SOLICITATION 3
  DUMMY-ICMP-ERROR-ENABLED NO
  IPv6-ENABLE-6to4-TUNNELING NO
  MOBILE-IP NO

  #********************ROUTING PROTOCOL***********************************

  ROUTING-PROTOCOL BELLMANFORD
  STATIC-ROUTE NO
  DEFAULT-ROUTE NO
  HSRP-PROTOCOL NO

  #***************************TRANSPORT***********************************

  TRANSPORT-PROTOCOL-RSVP YES
  GUI_DUMMY_CONFIG_TCP YES
  TCP LITE
  TCP-USE-RFC1323 NO
  TCP-DELAY-SHORT-PACKETS-ACKS NO
  TCP-USE-NAGLE-ALGORITHM YES
  TCP-USE-KEEPALIVE-PROBES YES
  TCP-USE-OPTIONS YES
  TCP-DELAY-ACKS YES
  TCP-MSS 512
  TCP-SEND-BUFFER 16384
  TCP-RECEIVE-BUFFER 16384

  #**************************MPLS Specs***********************************

  MPLS-PROTOCOL NO

  #*******************Application Layer***********************************

  RTP-ENABLED NO
  MDP-ENABLED NO

  #***********************USER BEHAVIOR***********************************

  DUMMY-UBEE-ENABLED NO

  #**********************Battery Models***********************************

  BATTERY-MODEL NONE

  #*****************Adaptation Protocol***********************************

  ADAPTATION-PROTOCOL AAL5
  ATM-CONNECTION-REFRESH-TIME 5M
  ATM-CONNECTION-TIMEOUT-TIME 1M
  IP-QUEUE-PRIORITY-QUEUE-SIZE 150000
  IP-QUEUE-TYPE FIFO

  #***************** [Wireless Subnet] ***********************************

  SUBNET N8-190.0.3.0 {{1 thru 3}} 563.548 98.0411 0

  #**********************Physical Layer***********************************

  [ N8-190.0.3.0 ] PHY-MODEL PHY802.15.4
  [ N8-190.0.3.0 ] PHY802.15.4-TX-POWER 3
  [ N8-190.0.3.0 ] PHY-RX-MODEL PHY802.15.4
  [ N8-190.0.3.0 ] PHY802.15.4-MODULATION O-QPSK
  [ N8-190.0.3.0 ] PHY802.15.4-CCA-MODE CARRIER-SENSE
  [ N8-190.0.3.0 ] DUMMY-ANTENNA-MODEL-CONFIG-FILE-SPECIFY NO
  [ N8-190.0.3.0 ] ANTENNA-MODEL OMNIDIRECTIONAL
  [ N8-190.0.3.0 ] ENERGY-MODEL-SPECIFICATION USER-DEFINED
  [ N8-190.0.3.0 ] ENERGY-TX-CURRENT-LOAD 40
  [ N8-190.0.3.0 ] ENERGY-RX-CURRENT-LOAD 40
  [ N8-190.0.3.0 ] ENERGY-IDLE-CURRENT-LOAD 15
  [ N8-190.0.3.0 ] ENERGY-SLEEP-CURRENT-LOAD 0.00
  [ N8-190.0.3.0 ] ENERGY-OPERATIONAL-VOLTAGE 3.0

  #***************************MAC Layer***********************************

  [ N8-190.0.3.0 ] MAC-PROTOCOL MAC802.15.4
  [ N8-190.0.3.0 ] MAC-802.15.4-DEVICE-TYPE RFD
  [ N8-190.0.3.0 ] MAC-802.15.4-POLL-INTERVAL {0}MS
  [ N8-190.0.3.0 ] MAC-802.15.4-COORD-BO 15
  [ N8-190.0.3.0 ] MAC-802.15.4-COORD-SO 3
  [ N8-190.0.3.0 ] MAC-802.15.4-START-DEVICE-AT 0S
  [ N8-190.0.3.0 ] MAC-802.15.4-STOP-DEVICE-AT 0S

  #***********************NETWORK LAYER***********************************

  [ N8-190.0.3.0 ] NETWORK-PROTOCOL IP
  [ N8-190.0.3.0 ] IP-QUEUE-PRIORITY-QUEUE-SIZE[0] 150000
  [ N8-190.0.3.0 ] IP-QUEUE-TYPE[0] FIFO
  [ N8-190.0.3.0 ] IP-QUEUE-PRIORITY-QUEUE-SIZE[1] 150000
  [ N8-190.0.3.0 ] IP-QUEUE-TYPE[1] FIFO
  [ N8-190.0.3.0 ] IP-QUEUE-PRIORITY-QUEUE-SIZE[2] 150000
  [ N8-190.0.3.0 ] IP-QUEUE-TYPE[2] FIFO

  #********************ROUTING PROTOCOL***********************************

  [ N8-190.0.3.0 ] ROUTING-PROTOCOL NONE



  #********* [Default Wireless Subnet] ***********************************


  #*************Interface Configuration***********************************


  [1] NETWORK-PROTOCOL[0]       IP
  [1] IP-ADDRESS[0]             190.0.3.4

  [2] NETWORK-PROTOCOL[0]       IP
  [2] IP-ADDRESS[0]             190.0.3.2

  [3] NETWORK-PROTOCOL[0]       IP
  [3] IP-ADDRESS[0]             190.0.3.1
  [190.0.3.4] MAC-802.15.4-DEVICE-TYPE  FFD
  [190.0.3.4] MAC-802.15.4-FFD-MODE  PANCOORD


  #*************Hierarchy Configuration***********************************


  #******************Node Configuration***********************************


  [1 thru 3]      STATIC-ROUTE YES
  [1]             HOSTNAME host1
  [2]             HOSTNAME host2
  [3]             HOSTNAME host3
  [1]             GUI-NODE-2D-ICON /Users/kai/Documents/Studium/UCSB/CS284/Qualnet/snt/qualnet/5.1/gui/icons/HUB.png
  [1 thru 3]      ROUTING-PROTOCOL NONE
  [1 thru 3]      BATTERY-MODEL-STATISTICS YES
  [1 thru 3]      NODE-PLACEMENT FILE
  [1 thru 3]      STATIC-ROUTE-FILE /Users/kai/Documents/Studium/UCSB/CS284/repo/cs284/simulation/autorun/template/template.routes-static
  NODE-POSITION-FILE template_double.nodes
  GUI-ANNOTATION-CONFIG-FILE template_double.ann

  #*********Miscellaneous Configuration***********************************

  GUI-DISPLAY-SETTINGS-FILE template_double.display



  """.format(sleep)
  
#workload in req/min/node
def gen_app(workload):
  num_packets = workload*15
  freq = 60*1000/workload
  return """
  SUPER-APPLICATION 1 2 DELIVERY-TYPE UNRELIABLE START-TIME DET 1 DURATION DET 0S REQUEST-NUM DET {0} REQUEST-SIZE DET 8 REQUEST-INTERVAL EXP {1}MS REPLY-PROCESS YES REPLY-NUM DET 1 REPLY-SIZE DET 8 REPLY-PROCESS-DELAY DET 100MS REPLY-INTERDEPARTURE-DELAY DET 1S APPLICATION-NAME Measurements 
  SUPER-APPLICATION 1 3 DELIVERY-TYPE UNRELIABLE START-TIME DET 1 DURATION DET 0S REQUEST-NUM DET {0} REQUEST-SIZE DET 8 REQUEST-INTERVAL EXP {1}MS REPLY-PROCESS YES REPLY-NUM DET 1 REPLY-SIZE DET 8 REPLY-PROCESS-DELAY DET 100MS REPLY-INTERDEPARTURE-DELAY DET 1S APPLICATION-NAME Measurements 
  """.format(num_packets, freq)
  
def gen(workload, sleep, number):
  shutil.copytree("../template_double", str(number))
  
  with open("%i/template_double.config"%number, "w") as f:
    f.write(gen_config(sleep))
  
  with open("%i/template_double.app"%number, "w") as f:
    f.write(gen_app(workload))
    

if __name__ == "__main__":
  os.chdir("batch")
  i = 0
  for workload in xrange(50, 300, 20):
    print "Starting with workload %i" % workload
    for sleep in xrange(20, 6000, 20):
      gen(workload, sleep, i)
      i+=1
  
  print "Generated %i experiments" % i