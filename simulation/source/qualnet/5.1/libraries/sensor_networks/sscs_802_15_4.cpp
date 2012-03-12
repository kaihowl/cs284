// Copyright (c) 2001-2009, Scalable Network Technologies, Inc.  All Rights Reserved.
//                          6100 Center Drive
//                          Suite 1250
//                          Los Angeles, CA 90045
//                          sales@scalable-networks.com
//
// This source code is licensed, not sold, and is subject to a written
// license agreement.  Among other things, no portion of this source
// code may be copied, transmitted, disclosed, displayed, distributed,
// translated, used as the basis for a derivative work, or used, in
// whole or in part, for any program or purpose other than its intended
// use in compliance with the license agreement as part of the QualNet
// software.  This source code and certain of the algorithms contained
// within it are confidential trade secrets of Scalable Network
// Technologies, Inc. and may not be used as the basis for any other
// software, hardware, product or service.

#include "sscs_802_15_4.h"

#define DEBUG 0

// /**
// FUNCTION   :: Sscs802_15_4StatusName
// LAYER      :: SSCS
// PURPOSE    :: Returns Status string for a given status
// PARAMETERS ::
// + status     : M802_15_4_enum: Status of request
// RETURN  :: char* : Status string
// **/
static
const char* Sscs802_15_4StatusName(M802_15_4_enum status)
{
    switch(status)
    {
        case M802_15_4_SUCCESS:
            return "SUCCESS";
        case M802_15_4_PAN_AT_CAPACITY:
            return "PAN_at_capacity";
        case M802_15_4_PAN_ACCESS_DENIED:
            return "PAN_access_denied";
        case M802_15_4_BEACON_LOSS:
            return "BEACON_LOSS";
        case M802_15_4_CHANNEL_ACCESS_FAILURE:
            return "CHANNEL_ACCESS_FAILURE";
        case M802_15_4_DENIED:
            return "DENIED";
        case M802_15_4_DISABLE_TRX_FAILURE:
            return "DISABLE_TRX_FAILURE";
        case M802_15_4_FAILED_SECURITY_CHECK:
            return "FAILED_SECURITY_CHECK";
        case M802_15_4_FRAME_TOO_LONG:
            return "FRAME_TOO_LONG";
        case M802_15_4_INVALID_GTS:
            return "INVALID_GTS";
        case M802_15_4_INVALID_HANDLE:
            return "INVALID_HANDLE";
        case M802_15_4_INVALID_PARAMETER:
            return "INVALID_PARAMETER";
        case M802_15_4_NO_ACK:
            return "NO_ACK";
        case M802_15_4_NO_BEACON:
            return "NO_BEACON";
        case M802_15_4_NO_DATA:
            return "NO_DATA";
        case M802_15_4_NO_SHORT_ADDRESS:
            return "NO_SHORT_ADDRESS";
        case M802_15_4_OUT_OF_CAP:
            return "OUT_OF_CAP";
        case M802_15_4_PAN_ID_CONFLICT:
            return "PAN_ID_CONFLICT";
        case M802_15_4_REALIGNMENT:
            return "REALIGNMENT";
        case M802_15_4_TRANSACTION_EXPIRED:
            return "TRANSACTION_EXPIRED";
        case M802_15_4_TRANSACTION_OVERFLOW:
            return "TRANSACTION_OVERFLOW";
        case M802_15_4_TX_ACTIVE:
            return "TX_ACTIVE";
        case M802_15_4_UNAVAILABLE_KEY:
            return "UNAVAILABLE_KEY";
        case M802_15_4_UNSUPPORTED_ATTRIBUTE:
            return "UNSUPPORTED_ATTRIBUTE";
        case M802_15_4_UNDEFINED:
        default:
            return "UNDEFINED";
    }
}

// /**
// FUNCTION   :: Sscs802_15_4SetTimer
// LAYER      :: MAC
// PURPOSE    :: Set a timer message.
// PARAMETERS ::
// + node           : Node*             : Pointer to node
// + interfaceIndex ; int               : Interface index of device
// + timerType      : S802_15_4TimerType: Type of the timer
// + delay          : clocktype         : Delay of this timer
// RETURN     :: None
// **/
static
void Sscs802_15_4SetTimer(Node* node,
                                  int interfaceIndex,
                                  S802_15_4TimerType timerType,
                                  clocktype delay)
{
    Message* timerMsg = NULL;
    S802_15_4Timer* timerInfo;

    // allocate the timer message and send out
    timerMsg = MESSAGE_Alloc(node,
                             MAC_LAYER,
                             MAC_PROTOCOL_802_15_4,
                             MSG_SSCS_802_15_4_TimerExpired);

    MESSAGE_SetInstanceId(timerMsg, (short)interfaceIndex);

    MESSAGE_InfoAlloc(node, timerMsg, sizeof(S802_15_4Timer));
    timerInfo = (S802_15_4Timer*) MESSAGE_ReturnInfo(timerMsg);

    timerInfo->timerType = timerType;

    MESSAGE_Send(node, timerMsg, delay);
}

// /**
// FUNCTION   :: Sscs802_15_4StartPANCoord
// LAYER      :: SSCS
// PURPOSE    :: Start a PAN and PAN co-ordinator
// PARAMETERS ::
// + node       : Node*         : Node receiving call
// + status     : M802_15_4_enum: Status of request
// RETURN  :: None
// **/
static
void Sscs802_15_4StartPANCoord(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;
    M802_15_4PIB t_mpib;
    PHY_PIB t_ppib;
    int i;
    //int BO;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    switch(sscs802_15_4->state)
    {
        case S802_15_4NULL:
        {
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Starting PAN Coord\n",
                       getSimTime(node), node->nodeId);
            }
            //must be an FFD
            mac->capability.FFD = TRUE;
            mac->isCoor = TRUE;
            //assign a short address for myself
            t_mpib.macShortAddress = (UInt16) node->nodeId;
            Mac802_15_4MLME_SET_request(node, interfaceIndex,
                                        macShortAddress, &t_mpib);

            //scan the channels
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Performing active "
                        "channel scan\n",
                       getSimTime(node), node->nodeId);
            }
            sscs802_15_4->state = S802_15_4SCANREQ;
            /*if (sscs802_15_4->t_BO == 15)
            {
                BO = 3;     //dirty hack, otherwise scan
                                            //duration becomes too high
            }
            else
            {
                BO = sscs802_15_4->t_BO;
            }*/
            Mac802_15_4MLME_SCAN_request(
                    node,
                    interfaceIndex,
                    0x01,
                    (UInt32)S802_15_4SCANCHANNELS,
                    S802_15_4DEF_SCANDUR);
            break;
        }
        case S802_15_4SCANREQ:
        {
            if (status != M802_15_4_SUCCESS)
            {
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : "
                            "Unable to start as a PAN coordinator: active "
                            "channel scan failed -> %s\n",
                            getSimTime(node), node->nodeId,
                            Sscs802_15_4StatusName(status));
                }
                sscs802_15_4->state = S802_15_4NULL;
                return;
            }
            //select a channel and a PAN ID (for simplicity, we just use the
            //IP address as the PAN ID)
            //(it's not an easy task to select a channel and PAN ID in
            //implementation!)
            for (i = 11; i < 27; i++)     //we give priority to 2.4G
            {
                if ((sscs802_15_4->T_UnscannedChannels & (1 << i)) == 0)
                {
                    break;
                }
            }
            if (i >= 27)
            {
                for (i = 0; i < 11; i++)
                {
                    if ((sscs802_15_4->T_UnscannedChannels & (1 << i)) == 0)
                    {
                        break;
                    }
                }
            }
            sscs802_15_4->Channel = (UInt8) i;
            //permit association
            t_mpib.macAssociationPermit = TRUE;
            Mac802_15_4MLME_SET_request(
                    node,
                    interfaceIndex,
                    macAssociationPermit,
                    &t_mpib);

            if (sscs802_15_4->t_BO < 15)    //Transmit beacons
            {
                if(DEBUG)
                {
                   printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Begin to transmit"
                            " beacons\n",
                            getSimTime(node), node->nodeId);
                }
                sscs802_15_4->state = S802_15_4STARTREQ;
                Mac802_15_4MLME_START_request(
                        node, interfaceIndex, (UInt16) node->nodeId,
                        sscs802_15_4->Channel,
                        sscs802_15_4->t_BO, sscs802_15_4->t_SO,
                        TRUE, FALSE, FALSE, FALSE);
            }
            else        //Do not transmit beacons
            {
                mac->isPANCoor = TRUE;
                mac->sfSpec2.BO = 15;
                mac->sfSpec2.SO = 15;
                mac->macBcnRxTime = 0;
                mac->macBeaconOrder2 = 15;
                mac->macBeaconOrder3 = 15;
                t_mpib.macCoordExtendedAddress = (UInt16) node->nodeId;

                mac->mpib.macCoordShortAddress = (UInt16) node->nodeId;
                Mac802_15_4MLME_SET_request(
                        node,
                        interfaceIndex,
                        macCoordExtendedAddress,
                        &t_mpib);

                t_ppib.phyCurrentChannel = i;
                Phy802_15_4PLME_SET_request(
                        node, interfaceIndex,
                        phyCurrentChannel, &t_ppib);

                //Setting current listening channel
                mac->taskP.mlme_start_request_LogicalChannel =
                    (UInt8) t_ppib.phyCurrentChannel;

                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Successfully "
                            "started a new PAN (non-beacon enabled) on "
                            "channel %d with PAN Id %d\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->Channel,
                            node->nodeId);
                }

                t_mpib.macPANId = (UInt16) node->nodeId;
                Mac802_15_4MLME_SET_request(node, interfaceIndex, macPANId,
                            &t_mpib);
                t_mpib.macBeaconOrder = 15;
                Mac802_15_4MLME_SET_request(node, interfaceIndex,
                                            macBeaconOrder, &t_mpib);
                t_mpib.macSuperframeOrder = 15;
                Mac802_15_4MLME_SET_request(node, interfaceIndex,
                                            macSuperframeOrder, &t_mpib);
                sscs802_15_4->state = S802_15_4UP;
            }
            break;
        }
        case S802_15_4STARTREQ:
        {
            if (status == M802_15_4_SUCCESS)
            {
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Successfully "
                            "started a new PAN (beacon enabled) on "
                            "channel %d with PAN Id %d\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->Channel,
                            node->nodeId);
                }
                mac->sfSpec2.BO = 15;
                mac->sfSpec2.SO = 15;
                mac->macBcnRxTime = 0;
                mac->macBeaconOrder2 = 15;
                mac->macBeaconOrder3 = 15;
                sscs802_15_4->state = S802_15_4UP;
            }
            else
            {
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Failed to "
                            "transmit beacons on channel %d with PAN Id %d "
                            "-> %s\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->Channel,
                            node->nodeId,
                            Sscs802_15_4StatusName(status));
                }
                sscs802_15_4->state = S802_15_4NULL;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

// /**
// FUNCTION   :: Sscs802_15_4StartDevice
// LAYER      :: SSCS
// PURPOSE    :: Start a Device
// PARAMETERS ::
// + node       : Node*         : Node receiving call
// + assoPermit : BOOL          : Whether association is permitted
// + status     : M802_15_4_enum: Status of request
// RETURN  :: None
// **/
static
void Sscs802_15_4StartDevice(
        Node* node,
        int interfaceIndex,
        BOOL assoPermit,
        M802_15_4_enum status)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;
    M802_15_4PIB t_mpib;
    UInt8 scan_BO;
    M802_15_4SuperframeSpec sfSpec;
    UInt8 ch;
    UInt8 fstChannel;
    UInt8 fstChannel2_4G;
    char tmpstr[30];
    int i;
    int k = 0;
    int l = 0;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    // Set the RXon Status.irrespective of beacon/non-beacon enabled
    if ( (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode < 2)
                 ) //|| sscs802_15_4->t_BO == 15 removed this for cs284!
     {
                    if (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode < 2)
                    {
                        mac->isCoor = TRUE;
                    }
                    t_mpib.macRxOnWhenIdle = TRUE;
                    Mac802_15_4MLME_SET_request(node, interfaceIndex,
                                        macRxOnWhenIdle, &t_mpib);
     }

    switch(sscs802_15_4->state)
    {
        case S802_15_4NULL:
        {
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Starting Device\n",
                       getSimTime(node), node->nodeId);
            }
            scan_BO = sscs802_15_4->t_BO + 1;
            //set FFD
            mac->capability.FFD = sscs802_15_4->t_isFFD;

            //scan the channels
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Performing active "
                        "channel scan\n",
                getSimTime(node), node->nodeId);
            }
            sscs802_15_4->state = S802_15_4SCANREQ;
            /*if (sscs802_15_4->t_BO == 15)
            {
                scan_BO = 3;     //dirty hack
            }*/
            Mac802_15_4MLME_SCAN_request(
                    node,
                    interfaceIndex,
                    0x01,
                    (UInt32)S802_15_4SCANCHANNELS,
                    S802_15_4DEF_SCANDUR);
            break;
        }
        case S802_15_4SCANREQ:
        {
            if (status != M802_15_4_SUCCESS)
            {
                sscs802_15_4->state = S802_15_4NULL;
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : "
                            "Unable to start as a Device: active "
                            "channel scan failed -> %s\n",
                            getSimTime(node), node->nodeId,
                            Sscs802_15_4StatusName(status));
                }
                //retry
                Sscs802_15_4SetTimer(node,
                                interfaceIndex,
                                S802_15_4ASSORETRY,
                                S802_15_4ASSORETRY_INTERVAL);
                return;
            }
            //select a PAN and a coordinator to join
            fstChannel = 0xff;
            fstChannel2_4G = 0xff;
            for (i = 0; i < sscs802_15_4->T_ResultListSize; i++)
            {
                sfSpec.SuperSpec =
                        sscs802_15_4->T_PANDescriptorList[i].SuperframeSpec;
                Mac802_15_4SuperFrameParse(&sfSpec);

                if (sfSpec.AssoPmt == FALSE)
                {
                    continue;
                }
                else
                {
                    if (sscs802_15_4->T_PANDescriptorList[i].LogicalChannel
                            < 11)
                    {
                        if (fstChannel == 0xff)
                        {
                            fstChannel =
                                sscs802_15_4->
                                    T_PANDescriptorList[i].LogicalChannel;
                            k = i;
                        }
                    }
                    else
                    {
                        if (fstChannel2_4G == 0xff)
                        {
                            fstChannel2_4G =
                                sscs802_15_4->
                                    T_PANDescriptorList[i].LogicalChannel;
                            l = i;
                        }
                    }
                }
            }
            if (fstChannel2_4G != 0xff)
            {
                ch = fstChannel2_4G;
                i = l;
            }
            else
            {
                ch = fstChannel;
                i = k;
            }
            if (ch == 0xff)    //cannot find any coordinator for association
            {
                sscs802_15_4->state = S802_15_4NULL;
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : "
                           "No Coordinator found for association\n",
                           getSimTime(node), node->nodeId);
                }
                Sscs802_15_4SetTimer(node,
                        interfaceIndex,
                        S802_15_4ASSORETRY,
                        S802_15_4ASSORETRY_INTERVAL);
                return;
            }
            else
            {
                //If the coordinator is in beacon-enabled mode, we may begin
                //to track beacons now.
                //But this is only possible if the network is a one-hop star;
                //        otherwise we don't know
                //which coordinator to track, since there may be more than
                //        one beaconing coordinators
                //in a device's neighborhood and MLME-SYNC.request() has no
                //        parameter telling which
                //coordinator to track. As this is an optional step, we will
                //        not track beacons here.
                t_mpib.macAssociationPermit = assoPermit;
                Mac802_15_4MLME_SET_request(node, interfaceIndex,
                        macAssociationPermit, &t_mpib);
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Sending "
                            "association request to Coordinator %d on "
                            "channel %d with PAN Id %d\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->
                                    T_PANDescriptorList[i].CoordAddress_64,
                            ch,
                            sscs802_15_4->T_PANDescriptorList[i].CoordPANId
                            );
                }
                sscs802_15_4->state = S802_15_4ASSOREQ;
                sscs802_15_4->startDevice_panDes =
                        sscs802_15_4->T_PANDescriptorList[i];
                mac->sfSpec2.SuperSpec =
                            sscs802_15_4->startDevice_panDes.SuperframeSpec;
                Mac802_15_4SuperFrameParse(&mac->sfSpec2);
                mac->macBeaconOrder2 = mac->sfSpec2.BO;
                mac->macSuperframeOrder2 = mac->sfSpec2.SO;
                Mac802_15_4MLME_ASSOCIATE_request(
                    node, interfaceIndex, ch,
                    sscs802_15_4->T_PANDescriptorList[i].CoordAddrMode,
                    sscs802_15_4->T_PANDescriptorList[i].CoordPANId,
                    sscs802_15_4->T_PANDescriptorList[i].CoordAddress_64,
                    mac->capability.cap, FALSE);
            }
            break;
        }
        case S802_15_4ASSOREQ:
        {
            sfSpec.SuperSpec =
                    sscs802_15_4->startDevice_panDes.SuperframeSpec;
            Mac802_15_4SuperFrameParse(&sfSpec);
            if (sfSpec.BO != 15)
            {
                strcpy(tmpstr, "beacon enabled");
            }
            else
            {
                strcpy(tmpstr, "non-beacon enabled");
            }
            if (status != M802_15_4_SUCCESS)
            {
                //reset association permission
                t_mpib.macAssociationPermit = FALSE;
                Mac802_15_4MLME_SET_request(
                        node,
                        interfaceIndex,
                        macAssociationPermit,
                        &t_mpib);
                sscs802_15_4->state = S802_15_4NULL;
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Association "
                            "failed on channel %d, PAN Id %d, Coordinator "
                            "%d (%s) -> %s\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->startDevice_panDes.LogicalChannel,
                            sscs802_15_4->startDevice_panDes.CoordPANId,
                            sscs802_15_4->startDevice_panDes.CoordAddress_64,
                            tmpstr,
                            Sscs802_15_4StatusName(status));
                }

                Sscs802_15_4SetTimer(node,
                            interfaceIndex,
                            S802_15_4ASSORETRY,
                            S802_15_4ASSORETRY_INTERVAL);
            }
            else
            {
                sscs802_15_4->neverAsso = FALSE;
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Association "
                            "successful on channel %d, PAN Id %d, "
                            "Coordinator %d\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->startDevice_panDes.LogicalChannel,
                            sscs802_15_4->startDevice_panDes.CoordPANId,
                            sscs802_15_4->startDevice_panDes.CoordAddress_64
                          );
                }
                if (sscs802_15_4->t_BO != 15 &&
                    sscs802_15_4->pollInt &&
                    mac->macBeaconOrder2 == 15)
                {
                    Sscs802_15_4SetTimer (node,
                                          interfaceIndex,
                                          S802_15_4POLLINT,
                                          sscs802_15_4->pollInt +
                                          (RANDOM_nrand(sscs802_15_4->seed)
                                          % (1 * SECOND)));
                }

                if (sfSpec.BO != 15)
                {
                    if(DEBUG)
                    {
                        printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Begin to "
                                "synchronize with the coordinator\n",
                                getSimTime(node), node->nodeId);
                    }
                    Mac802_15_4MLME_SYNC_request(
                        node,
                        interfaceIndex,
                        sscs802_15_4->startDevice_panDes.LogicalChannel,
                        TRUE);
                }

                if( sscs802_15_4->t_BO == 15)
                {
                        if (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode
                                < 2 )
                        {
                            mac->isCoor = TRUE;
                        }
                        sscs802_15_4->state = S802_15_4UP;
                }

                if (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode < 2 &&
                        sscs802_15_4->t_BO < 15)
                {
                    mac->isCoor = TRUE;
                    sscs802_15_4->state = S802_15_4STARTREQ;
                    if(DEBUG)
                    {
                        printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Begin to "
                                "transmit beacons\n",
                                getSimTime(node), node->nodeId);
                    }
                    Mac802_15_4MLME_START_request(
                            node,
                            interfaceIndex,
                            mac->mpib.macPANId,
                            sscs802_15_4->startDevice_Channel,
                            sscs802_15_4->t_BO,
                            sscs802_15_4->t_SO,
                            FALSE, FALSE, FALSE, FALSE);
                }
                else
                {
                    sscs802_15_4->state = S802_15_4UP;
                }
            }
            break;
        }
        case S802_15_4STARTREQ:
        {
            if (status == M802_15_4_SUCCESS)
            {
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Successfully "
                            "transmitted beacons on "
                            "channel %d with PAN Id %d\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->startDevice_Channel,
                            mac->mpib.macPANId);
                }
                sscs802_15_4->state = S802_15_4UP;
            }
            else
            {
                if(DEBUG)
                {
                    printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Failed to "
                            "transmit beacons on channel %d with PAN Id %d "
                            "-> %s\n",
                            getSimTime(node), node->nodeId,
                            sscs802_15_4->startDevice_Channel,
                            mac->mpib.macPANId,
                            Sscs802_15_4StatusName(status));
                }
                sscs802_15_4->state = S802_15_4NULL;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

// /**
// FUNCTION   :: Sscs802_15_4StopDevice
// LAYER      :: SSCS
// PURPOSE    :: Stops a Device
// PARAMETERS ::
// + node       : Node*         : Node receiving call
// + interfaceIndex : int       : Interface Index
// + status     : M802_15_4_enum: Status of request
// RETURN  :: None
// **/
static
void Sscs802_15_4StopDevice(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    if(sscs802_15_4->state != S802_15_4UP &&
       sscs802_15_4->state != S802_15_4DISASSOREQ)
    {
        if(DEBUG)
        {
            printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : "
                    "Device not associated with a coordinator\n",
                    getSimTime(node), node->nodeId);
        }
        return;
    }
    switch(sscs802_15_4->state)
    {
        case S802_15_4UP:
        {
            sscs802_15_4->state = S802_15_4DISASSOREQ;
            Mac802_15_4MLME_DISASSOCIATE_request(
                node,
                interfaceIndex,
                mac->mpib.macCoordExtendedAddress,
                0x02,   //device wishes to leave
                FALSE);
            //reset the device
            Mac802_15_4MLME_RESET_request(
                    node,
                    interfaceIndex,
                    TRUE);
            break;
        }
        case S802_15_4DISASSOREQ:
        {
            sscs802_15_4->state = S802_15_4NULL;
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : "
                        "Device disassociated from PAN\n",
                        getSimTime(node), node->nodeId);
            }
            break;
        }
        default:
            break;
    }
}

// /**
// FUNCTION   :: Ssc802_15_4PollRequest
// LAYER      :: SSCS
// PURPOSE    :: Perform polling
// PARAMETERS ::
// + node       : Node*         : Node receiving call
// + interfaceIndex : int       : Interface Index
// RETURN  :: None
// **/
static
void Sscs802_15_4PollRequest(Node* node, int interfaceIndex)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    if(Sscs802_15_4IsDeviceUp(node, interfaceIndex))
    {
        Mac802_15_4MLME_POLL_request(
                node,
                interfaceIndex,
                M802_15_4DEFFRMCTRL_ADDRMODE64,
                mac->mpib.macPANId,
                mac->mpib.macCoordExtendedAddress,
                FALSE);
    }
    if ((sscs802_15_4->t_BO != 15 &&
        sscs802_15_4->pollInt &&
        mac->macBeaconOrder2 == 15) ||
        (sscs802_15_4->t_BO == 15 && sscs802_15_4->pollInt))
    {
    Sscs802_15_4SetTimer (
            node,
            interfaceIndex,
            S802_15_4POLLINT,
            sscs802_15_4->pollInt);
    }
    else if(mac->taskP.mlme_poll_request_STEP)
    {
        mac->taskP.mlme_poll_request_STEP = 0;
    }
}
//--------------------------------------------------------------------------
// API functions between SSCS and MAC
//--------------------------------------------------------------------------

// /**
// FUNCTION   :: Sscs802_15_4IsDeviceUp
// LAYER      :: SSCS
// PURPOSE    :: Primitive to find out whether the device is up or not.
// PARAMETERS ::
// + node           : Node*         : Node receiving call
// + interfaceIndex : int       : Interface Index
// RETURN  :: BOOL
// **/
BOOL Sscs802_15_4IsDeviceUp(
        Node* node,
        int interfaceIndex)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    if(sscs802_15_4->state == S802_15_4UP)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// /**
// FUNCTION   :: Sscs802_15_4MCPS_DATA_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report results of a request to tx a data SPDU
// PARAMETERS ::
// + node           : Node*         : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + msduHandle     : UInt8         : Handle associated with MSDU
// + status         : M802_15_4_enum: Status of data sent request
// RETURN  :: None
// **/
void Sscs802_15_4MCPS_DATA_confirm(
        Node* node,
        int interfaceIndex,
        UInt8 msduHandle,
        M802_15_4_enum status)
{
}

// /**
// FUNCTION   :: Sscs802_15_4MCPS_DATA_indication
// LAYER      :: Mac
// PURPOSE    :: Primitive to indicate the transfer of a data SPDU to SSCS
// PARAMETERS ::
// + node           : Node*         : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + SrcAddrMode    : UInt8         : Source address mode
// + SrcPANId       : UInt16        : source PAN id
// + SrcAddr        : MACADDR    : Source address
// + DstAddrMode    : UInt8         : Destination address mode
// + DstPANId       : UInt16        : Destination PAN id
// + DstAddr        : MACADDR    : Destination Address
// + msduLength     : int         : MSDU length
// + msdu           : Message*      : MSDU
// + mpduLinkQuality: UInt8         : LQI value measured during reception of
//                                    the MPDU
// + SecurityUse    : BOOL          : whether security is used
// + ACLEntry       : UInt8         : ACL entry
// RETURN  :: None
// **/
void Sscs802_15_4MCPS_DATA_indication(
        Node* node,
        int interfaceIndex,
        UInt8 SrcAddrMode,
        UInt16 SrcPANId,
        MACADDR SrcAddr,
        UInt8 DstAddrMode,
        UInt16 DstPANId,
        MACADDR DstAddr,
        int msduLength,
        Message *msdu,
        UInt8 mpduLinkQuality,
        BOOL SecurityUse,
        UInt8 ACLEntry)
{
    MESSAGE_Free(node, msdu);
}

// /**
// FUNCTION   :: Sscs802_15_4MCPS_PURGE_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of purge request
// PARAMETERS ::
// + node           : Node*         : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + msduHandle     : UInt8         : Handle associated with MSDU
// + status         : M802_15_4_enum: Status of purge request
// RETURN  :: None
// **/
void Sscs802_15_4MCPS_PURGE_confirm(
        Node* node,
        int interfaceIndex,
        UInt8 msduHandle,
        M802_15_4_enum status)
{
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_ASSOCIATE_indication
// LAYER      :: Mac
// PURPOSE    :: Primitive to indicate an incoming associate request
// PARAMETERS ::
// + node           : Node*         : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + DeviceAddress      : MACADDR    : Address of device requesting
//                                        association
// + CapabilityInformation : UInt8  : capabilities of associating device
// + SecurityUse    : BOOL          : Whether enable security or not
// + ACLEntry       : UInt8         : ACL entry
// RETURN  :: None
// **/
void Sscs802_15_4MLME_ASSOCIATE_indication(
        Node* node,
        int interfaceIndex,
        MACADDR DeviceAddress,
        UInt8 CapabilityInformation,
        BOOL SecurityUse,
        UInt8 ACLEntry)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;


    Mac802_15_4MLME_ASSOCIATE_response(
            node,
            interfaceIndex,
            DeviceAddress,
            (UInt16)DeviceAddress,
            M802_15_4_SUCCESS,
            FALSE);
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_ASSOCIATE_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of associate request
// PARAMETERS ::
// + node               : Node*         : Node receiving call
// + interfaceIndex     : int           : Interface Index
// + AssocShortAddress  : UInt16        : Short address allocated by coord
// + status             : M802_15_4_enum: Status of association attempt
// RETURN  :: None
// **/
void Sscs802_15_4MLME_ASSOCIATE_confirm(
        Node* node,
        int interfaceIndex,
        UInt16 AssocShortAddress,
        M802_15_4_enum status)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;
    M802_15_4PIB t_mpib;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    if (status == M802_15_4_SUCCESS)
    {
        mac->rt_myNodeID = AssocShortAddress;
        t_mpib.macShortAddress = (UInt16)node->nodeId;    //don't use cluste
        Mac802_15_4MLME_SET_request(
                node,
                interfaceIndex,
                macShortAddress,
                &t_mpib);
    }
    if (sscs802_15_4->state != S802_15_4NULL)
    {
        Sscs802_15_4StartDevice(
                node,
                interfaceIndex,
                TRUE,
                status);
    }
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_DISASSOCIATE_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of disassociate request
// PARAMETERS ::
// + node               : Node*         : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum: Status of disassociation attempt
// RETURN  :: None
// **/
void Sscs802_15_4MLME_DISASSOCIATE_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status)
{
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_BEACON_NOTIFY_indication
// LAYER      :: Mac
// PURPOSE    :: Primitive to send params contained within a beacon to SSCS
// PARAMETERS ::
// + node           : Node*             : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + BSN            : UInt8             : The beacon sequence number.
// + PANDescriptor  : M802_15_4PanEle*  : PANDescriptor for the recd beacon
// + PendAddrSpec   : UInt8             : Beacon pending address spec
// + AddrList       : MACADDR*       : list of addresses of the devices
//                                        for which beacon source has data
// + sduLength      : UInt8             : number of octets contained in the
//                                      beacon payload of the beacon frame
// + sdu            : UInt8*            : beacon payload to be transferred
// RETURN  :: None
// **/
void Sscs802_15_4MLME_BEACON_NOTIFY_indication(
        Node* node,
        int interfaceIndex,
        UInt8 BSN,
        M802_15_4PanEle* PANDescriptor,
        UInt8 PendAddrSpec,
        MACADDR* AddrList,
        UInt8 sduLength,
        UInt8* sdu)
{
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_GET_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of GET request
// PARAMETERS ::
// + node               : Node*                 : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum        : Status of GET attempt
// + PIBAttribute       : M802_15_4_PIBA_enum   : PIB attribute id
// + PIBAttributeValue  : M802_15_4PIB*         : Attribute value
// RETURN  :: None
// **/
void Sscs802_15_4MLME_GET_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status,
        M802_15_4_PIBA_enum PIBAttribute,
        M802_15_4PIB* PIBAttributeValue)
{
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_ORPHAN_indication
// LAYER      :: Mac
// PURPOSE    :: Primitive to notify presence of orphan device to SSCS
// PARAMETERS ::
// + node               : Node*         : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + OrphanAddress      : MACADDR    : Address of orphan device
// + SecurityUse        : BOOL          : Whether enabled security or not
// + ACLEntry           : UInt8         : ACL entry
// RETURN  :: None
// **/
void Sscs802_15_4MLME_ORPHAN_indication(
        Node* node,
        int interfaceIndex,
        MACADDR OrphanAddress,
        BOOL SecurityUse,
        UInt8 ACLEntry)
{
    MacData802_15_4* mac;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    if(Mac802_15_4UpdateDeviceLink(
            2,
            &mac->deviceLink1,
            &mac->deviceLink2,
            OrphanAddress) == 0)
    {
        Mac802_15_4MLME_ORPHAN_response(
                node,
                interfaceIndex,
                OrphanAddress,
                (UInt16)OrphanAddress,
                TRUE,
                FALSE);
    }
    else
    {
        Mac802_15_4MLME_ORPHAN_response(
                node,
                interfaceIndex,
                OrphanAddress,
                0,
                FALSE,
                FALSE);
    }
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_RESET_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of RESET request
// PARAMETERS ::
// + node               : Node*                 : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum        : Status of RESET attempt
// RETURN  :: None
// **/
void Sscs802_15_4MLME_RESET_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status)
{
    Sscs802_15_4StopDevice(node, interfaceIndex, status);
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_RX_ENABLE_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of RxEnable request
// PARAMETERS ::
// + node               : Node*                 : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum        : Status of RxEnable attempt
// RETURN  :: None
// **/
void Sscs802_15_4MLME_RX_ENABLE_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status)
{
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_SET_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of SET request
// PARAMETERS ::
// + node               : Node*                 : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum        : Status of SET attempt
// + PIBAttribute       : M802_15_4_PIBA_enum   : PIB attribute id
// RETURN  :: None
// **/
void Sscs802_15_4MLME_SET_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status,
        M802_15_4_PIBA_enum PIBAttribute)
{
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_SCAN_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of SCAN request
// PARAMETERS ::
// + node               : Node*             : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum    : Status of Scan attempt
// + ScanType           : UInt8             : Type of scan
// + UnscannedChannels  : UInt32            : Channels given in the request
//                                            not scanned
// + ResultListSize     : UInt8             : Number of elements returned
// + EnergyDetectList   : UInt8*            : List of energy measurements,
//                                            one for each channel
// + PANDescriptorList  : M802_15_4PanEle*  : List of PAN descriptors
// RETURN  :: None
// **/
void Sscs802_15_4MLME_SCAN_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status,
        UInt8 ScanType,
        UInt32 UnscannedChannels,
        UInt8 ResultListSize,
        UInt8* EnergyDetectList,
        M802_15_4PanEle* PANDescriptorList)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;
    M802_15_4PIB t_mpib;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    sscs802_15_4->T_UnscannedChannels = UnscannedChannels;
    sscs802_15_4->T_ResultListSize = ResultListSize;
    sscs802_15_4->T_EnergyDetectList = EnergyDetectList;
    sscs802_15_4->T_PANDescriptorList = PANDescriptorList;
    if (ScanType == 0x01)
    {
        if (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode == 0 &&
            sscs802_15_4->state != S802_15_4NULL) //PAN coordinator
        {
            Sscs802_15_4StartPANCoord(
                    node,
                    interfaceIndex,
                    status);
        }
        else if (sscs802_15_4->state != S802_15_4NULL)
        {
            Sscs802_15_4StartDevice(
                    node,
                    interfaceIndex,
                    TRUE,
                    status);
        }
    }
    if (ScanType == 0x03)
    {
        if (status == M802_15_4_SUCCESS)
        {
            //re-synchronize with the coordinator
            if(mac->macBeaconOrder2 == 15)
            {
                if(mac->isSyncLoss)
                {
                    mac->isSyncLoss = FALSE;
                    sscs802_15_4->state = S802_15_4UP;
                    return;
                }
            }

            Phy802_15_4PLME_GET_request(
                    node,
                    interfaceIndex,
                    phyCurrentChannel);
            sscs802_15_4->state = S802_15_4UP;
            Mac802_15_4MLME_SYNC_request(
                    node,
                    interfaceIndex,
                    (UInt8)mac->tmp_ppib.phyCurrentChannel,
                    TRUE);
        }
        else
        {

            t_mpib.macShortAddress = 0xffff;
            Mac802_15_4MLME_SET_request(
                        node,
                        interfaceIndex,
                        macShortAddress,
                        &t_mpib);
            t_mpib.macCoordExtendedAddress =
                        M802_15_4_COORDEXTENDEDADDRESS;
            mac->mpib.macCoordShortAddress =
                        M802_15_4_COORDSHORTADDRESS;
            Mac802_15_4MLME_SET_request(
                        node,
                        interfaceIndex,
                        macCoordExtendedAddress,
                        &t_mpib);
            sscs802_15_4->state = S802_15_4NULL;
            Sscs802_15_4StartDevice(
                        node, interfaceIndex,
                        sscs802_15_4->t_assoPermit,
                        M802_15_4_SUCCESS);
        }
    }
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_COMM_STATUS_indication
// LAYER      :: Mac
// PURPOSE    :: Primitive to indicate a communications status
// PARAMETERS ::
// + node           : Node*         : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + PANId          : UInt16        : PAN id
// + SrcAddrMode    : UInt8         : Source address mode
// + SrcAddr        : MACADDR    : Source address
// + DstAddrMode    : UInt8         : Destination address mode
// + DstAddr        : MACADDR    : Destination Address
// + status         : M802_15_4_enum: Status of Communication
// RETURN  :: None
// **/
void Sscs802_15_4MLME_COMM_STATUS_indication(
        Node* node,
        int interfaceIndex,
        UInt16 PANId,
        UInt8 SrcAddrMode,
        MACADDR SrcAddr,
        UInt8 DstAddrMode,
        MACADDR DstAddr,
        M802_15_4_enum status)
{
    if(status == M802_15_4_TRANSACTION_OVERFLOW)
    {
        if(DEBUG)
        {
            printf("%" TYPES_64BITFMT "d: Node %d: 802.15.4SSCS : Already processing a "
                    "request. Discarding one received from node %d\n",
                    getSimTime(node), node->nodeId,
                    DstAddr);
        }
    }
    else if (status == M802_15_4_SUCCESS)
    {
        MacData802_15_4* mac =
                (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
        SscsData802_15_4* sscs802_15_4 = (SscsData802_15_4*)mac->sscs;
         Mac802_15_4ChkAddDeviceLink(
            &mac->deviceLink1,
            &mac->deviceLink2,
            DstAddr,
            0);
          sscs802_15_4->stats.numAssociationAcptd++;
    }
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_START_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of START request
// PARAMETERS ::
// + node               : Node*                 : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum        : Status of START attempt
// RETURN  :: None
// **/
void Sscs802_15_4MLME_START_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;

    if (sscs802_15_4->ffdMode == 0 &&
        sscs802_15_4->state != S802_15_4NULL) //PAN coordinator
    {
        Sscs802_15_4StartPANCoord(
                node,
                interfaceIndex,
                status);
    }
    else if (sscs802_15_4->state != S802_15_4NULL)
    {
        Sscs802_15_4StartDevice(
                node,
                interfaceIndex,
                TRUE,
                status);
    }
    else
    {
        if (mac->mpib.macBeaconOrder == 15)
        {
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Beacon transmission "
                        "stopped on "
                        "channel %d with PAN Id %d\n",
                        getSimTime(node), node->nodeId,
                        mac->tmp_ppib.phyCurrentChannel,
                        mac->mpib.macPANId);
            }
        }
        else if (status == M802_15_4_SUCCESS)
        {
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Successfully "
                        "transmitted beacons on "
                        "channel %d with PAN Id %d\n",
                        getSimTime(node), node->nodeId,
                        mac->tmp_ppib.phyCurrentChannel,
                        mac->mpib.macPANId);
            }
        }
        else
        {
            if(DEBUG)
            {
                printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Failed to transmit "
                        "beacons on channel %d with PAN Id %d -> %s\n",
                        getSimTime(node), node->nodeId,
                        mac->tmp_ppib.phyCurrentChannel,
                        mac->mpib.macPANId,
                        Sscs802_15_4StatusName(status));
            }
        }
    }
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_SYNC_LOSS_indication
// LAYER      :: Mac
// PURPOSE    :: Primitive to indicate the loss of synchronization with a
//               coordinator
// PARAMETERS ::
// + node               : Node*                 : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + LossReason         : M802_15_4_enum        : Reason for Sync Loss
// RETURN  :: None
// **/
void Sscs802_15_4MLME_SYNC_LOSS_indication(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum LossReason)
{
    SscsData802_15_4* sscs802_15_4;
    MacData802_15_4* mac;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*) mac->sscs;

    mac->taskP.mlme_sync_request_tracking = FALSE;
    mac->taskP.mlme_sync_request_STEP = 0;

    if (DEBUG)
    {
        printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Synchronization loss\n",
            getSimTime(node), node->nodeId);
    }
    if(sscs802_15_4->state != S802_15_4NULL)
    {
        mac->isSyncLoss = TRUE;
        sscs802_15_4->stats.numSyncLoss++;
        sscs802_15_4->state = S802_15_4SCANREQ;
        Mac802_15_4MLME_SCAN_request(
                    node,
                    interfaceIndex,
                    0x03,
                    S802_15_4SCANCHANNELS,
                    0);
    }
}

// /**
// FUNCTION   :: Sscs802_15_4MLME_POLL_confirm
// LAYER      :: Mac
// PURPOSE    :: Primitive to report result of POLL request
// PARAMETERS ::
// + node               : Node*                 : Node receiving call
// + interfaceIndex     : int                   : Interface Index
// + status             : M802_15_4_enum        : Status of POLL attempt
// RETURN  :: None
// **/
void Sscs802_15_4MLME_POLL_confirm(
        Node* node,
        int interfaceIndex,
        M802_15_4_enum status)
{
    if(status == M802_15_4_INVALID_PARAMETER)
    {
        if(DEBUG)
        {
            printf("%" TYPES_64BITFMT "d : Node %d: 802.15.4SSCS : Polling invalid param\n",
                   getSimTime(node), node->nodeId);
        }
    }
    else if(status == M802_15_4_NO_ACK)
    {
        Sscs802_15_4MLME_SYNC_LOSS_indication(
                    node,
                    interfaceIndex,
                    status);
    }
}

// /**
// FUNCTION   :: Sscs802_15_4PrintStats
// LAYER      :: SSCS
// PURPOSE    :: Print out statistics
// PARAMETERS ::
// + node      : Node*         : Pointer to node
// + interfaceIndex : int      : Interface index
// RETURN     :: void : NULL
// **/
static
void Sscs802_15_4PrintStats(Node* node, int interfaceIndex)
{
    SscsData802_15_4* sscs802_15_4;
    MacData802_15_4* mac;
    char buf[MAX_STRING_LENGTH];

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*) mac->sscs;

    // print out # of Associations accepted
    if (sscs802_15_4->t_isFFD)
    {
        sprintf(buf, "Number of Association requests accepted = %ld",
                sscs802_15_4->stats.numAssociationAcptd);
        IO_PrintStat(node,
                    "SSCS",
                    "802.15.4",
                    ANY_DEST,
                    interfaceIndex,
                    buf);
        sprintf(buf, "Number of Association requests rejected = %ld",
                sscs802_15_4->stats.numAssociationRejctd);
        IO_PrintStat(node,
                     "SSCS",
                     "802.15.4",
                     ANY_DEST,
                     interfaceIndex,
                     buf);
    }
    sprintf(buf, "Number of SYNC loss reported = %ld",
            sscs802_15_4->stats.numSyncLoss);
    IO_PrintStat(node,
                 "SSCS",
                 "802.15.4",
                 ANY_DEST,
                 interfaceIndex,
                 buf);
}

// /**
// FUNCTION     Sscs802_15_4Init
// PURPOSE      Initialization function for 802.15.4 protocol of SSCS layer
// PARAMETERS   Node* node
//                  Node being initialized.
//              NodeInput* nodeInput
//                  Structure containing contents of input file.
// RETURN       None
// NOTES        None
// **/
void Sscs802_15_4Init(
        Node* node,
        const NodeInput* nodeInput,
        int interfaceIndex)
{
    SscsData802_15_4* sscs802_15_4;
    BOOL wasFound = FALSE;
    char retString[MAX_STRING_LENGTH];
    int retVal;
    clocktype retTime;
    Address address;
    clocktype startDev;
    clocktype stopDev;
    //clocktype startBeacon;
    //clocktype stopBeacon;
    MacData802_15_4* mac;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;

    sscs802_15_4 = (SscsData802_15_4*)MEM_malloc(sizeof(SscsData802_15_4));
    ERROR_Assert(sscs802_15_4 != NULL,
                 "802.15.4: Unable to allocate memory for SSCS data struc.");
    memset(sscs802_15_4, 0, sizeof(SscsData802_15_4));

    mac->sscs = sscs802_15_4;
    RANDOM_SetSeed(sscs802_15_4->seed,
                   node->globalSeed,
                   node->nodeId,
                   MAC_PROTOCOL_802_15_4,
                   interfaceIndex);

    sscs802_15_4->neverAsso = TRUE;
    sscs802_15_4->state = S802_15_4NULL;

    NetworkGetInterfaceInfo(
            node,
            mac->myMacData->interfaceIndex,
            &address);

    IO_ReadString(
            node->nodeId,
            &address,
            nodeInput,
            "MAC-802.15.4-DEVICE-TYPE",
            &wasFound,
            retString);

    if (wasFound)
    {
        if ((strcmp(retString, "RFD") == 0))
        {
            sscs802_15_4->t_isFFD = FALSE;
        }
        else if ((strcmp(retString, "FFD") == 0))
        {
            sscs802_15_4->t_isFFD = TRUE;
            IO_ReadString(
                    node->nodeId,
                    &address,
                    nodeInput,
                    "MAC-802.15.4-FFD-MODE",
                    &wasFound,
                    retString);
            if (wasFound)
            {
                if(strcmp(retString, "PANCOORD") == 0)
                {
                    sscs802_15_4->ffdMode = 0;
                }
                else if (strcmp(retString, "COORD") == 0)
                {
                    sscs802_15_4->ffdMode = 1;
                }
                else if (strcmp(retString, "DEVICE") == 0)
                {
                    sscs802_15_4->ffdMode = 2;
                }
                else
                {
                    //default PAN Coordinator
                    ERROR_ReportWarning("Unknown FFD MODE, Takes default "
                        "Mode as PANCOORD");
                    sscs802_15_4->ffdMode = 0;
                }
            }
            else
            {
                sscs802_15_4->ffdMode = 0;
            }
        }
        else
        {
            //default = RFD
            ERROR_ReportWarning("Unknown Device type, Takes default type: "
                "RFD");
            sscs802_15_4->t_isFFD = FALSE;
        }
    }
    else
    {
        //default = RFD
        sscs802_15_4->t_isFFD = FALSE;
    }


    IO_ReadInt(
            node->nodeId,
            &address,
            nodeInput,
            "MAC-802.15.4-COORD-BO",
            &wasFound,
            &retVal);

    if (wasFound)
    {
        sscs802_15_4->t_BO = (UInt8) retVal;
        ERROR_Assert(sscs802_15_4->t_BO <=15,
                "SSCS802_15_4Init:"
                " Invalid BO value. Should be between 0 and 15");
    }
    else
    {
        //default value = 3
        sscs802_15_4->t_BO = S802_15_4DEF_BOVAL;
    }

    IO_ReadInt(
            node->nodeId,
            &address,
            nodeInput,
            "MAC-802.15.4-COORD-SO",
            &wasFound,
            &retVal);
    if (wasFound)
    {
        sscs802_15_4->t_SO = (UInt8) retVal;
        ERROR_Assert(sscs802_15_4->t_SO <=15,
                    "SSCS802_15_4Init:"
                    " Invalid SO value. Should be between 0 and 15");
    }
    else
    {
        //default value = 3
        sscs802_15_4->t_SO = S802_15_4DEF_SOVAL;
    }

    IO_ReadTime(
            node->nodeId,
            &address,
            nodeInput,
            "MAC-802.15.4-START-DEVICE-AT",
            &wasFound,
            &retTime);
    if (wasFound)
    {
        ERROR_Assert(retTime >= 0,"Invalid Device Start time");
        startDev = retTime;
    }
    else
    {
        startDev = 0;
    }

    IO_ReadTime(
            node->nodeId,
            &address,
            nodeInput,
            "MAC-802.15.4-STOP-DEVICE-AT",
            &wasFound,
            &retTime);
    if (wasFound)
    {
        ERROR_Assert((retTime >= 0) && ((retTime == 0)||
            (startDev < retTime)),"Invalid Device Stop time or Device stop "
            "time is less than Device start time");
        stopDev = retTime;
    }
    else
    {
        stopDev = 0;
    }

    //for future use
/*    IO_ReadTime(
            node->nodeId,
            &address,
            nodeInput,
            "MAC-802.15.4-START-BEACON-AT",
            &wasFound,
            &retTime);
    if (wasFound)
    {
        ERROR_Assert(retTime >= 0,"Invalid Beacon Start time");
        startBeacon = retTime;
    }
    else
    {
        startBeacon = 0;
    }

    IO_ReadTime(
            node->nodeId,
            &address,
            nodeInput,
            "MAC-802.15.4-STOP-BEACON-AT",
            &wasFound,
            &retTime);
    if (wasFound)
    {
        ERROR_Assert((retTime >= 0) && ((retTime == 0) ||
            (startBeacon < retTime )),"Invalid Beacon Stop time or Beacon "
            "Stop time less than Beacon start time");
        stopBeacon = retTime;
    }
    else
    {
        stopBeacon = 0;
    }*/

    if (!sscs802_15_4->t_isFFD ||
        (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode > 0))
    {
        // Poll interval is set when RFD or
        // in case of FFD when it is not PAN coordinator
        IO_ReadTime(
                node->nodeId,
                &address,
                nodeInput,
                "MAC-802.15.4-POLL-INTERVAL",
                &wasFound,
                &retTime);
        if (wasFound)
        {
                ERROR_Assert(retTime > 0,"Invalid Poll Interval,"
                                        " Should be greater than 0");
            sscs802_15_4->pollInt = retTime;
        }
        else
        {
           // A beacon enabled coordinator can be associated to a non-beacon
            // enabled coordinator/pan-coordinator, hence poll interval is
            // set to 1S for all. Whether to poll or not is decided at the
            // handler of S802_15_4STARTDEVICE
            sscs802_15_4->pollInt = SECOND;
        }
    }

    //schedule timers - add a random delay
    //Start device
    Sscs802_15_4SetTimer (node,
            interfaceIndex,
            S802_15_4STARTDEVICE,
            startDev + (RANDOM_nrand(sscs802_15_4->seed) % (1 * SECOND))
            );

    //for future use
    /*if (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode == 1)
    //Start Beacons
    {
        Sscs802_15_4SetTimer (node,
            interfaceIndex,
            S802_15_4STARTBEACON,
            startBeacon + (RANDOM_nrand(sscs802_15_4->seed) % (1 * SECOND))
            );

        //Stop Beacons - stopBeacon = 0, end of Simulation
        if (stopBeacon != 0)
        {
            Sscs802_15_4SetTimer (node,
                interfaceIndex,
                S802_15_4STOPBEACON,
                stopBeacon + (RANDOM_nrand(sscs802_15_4->seed) % (1 *
                    SECOND))
                );
        }
    }*/
    if (stopDev != 0)
    {
        Sscs802_15_4SetTimer (node,
                              interfaceIndex,
                              S802_15_4STOPDEVICE,
                              stopDev);
    }
    if ( (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode != 0) ||
          !sscs802_15_4->t_isFFD)
    {
        //poll timer for non-beacon enabled
        if (sscs802_15_4->t_BO == 15 && sscs802_15_4->pollInt)
        {
            Sscs802_15_4SetTimer (node,
                                  interfaceIndex,
                                  S802_15_4POLLINT,
                                  sscs802_15_4->pollInt +
                                    (RANDOM_nrand(sscs802_15_4->seed) % (1 *
                                    SECOND)));
        }
    }
}

// /**
// FUNCTION     Sscs802_15_4Layer
// PURPOSE      To handle timer events. This is called via Mac802_15_4Layer()
// PARAMETERS   Node *node
//                  Node which received the message.
//              int interfaceInde
//                  Interface index on which message is received
//              Message* msg
//                  Message received by the layer.
// RETURN       None
// NOTES        None
// **/
void Sscs802_15_4Layer(Node* node, int interfaceIndex, Message* msg)
{
    MacData802_15_4* mac;
    SscsData802_15_4* sscs802_15_4;
    S802_15_4Timer* timerInfo;

    mac = (MacData802_15_4*) node->macData[interfaceIndex]->macVar;
    sscs802_15_4 = (SscsData802_15_4*)mac->sscs;
    timerInfo = (S802_15_4Timer*) MESSAGE_ReturnInfo(msg);

    switch(timerInfo->timerType)
    {
        case S802_15_4STARTDEVICE:
        {
            if (sscs802_15_4->t_isFFD && sscs802_15_4->ffdMode == 0)
                 //PAN coordinator
            {
                Sscs802_15_4StartPANCoord(
                        node,
                        interfaceIndex,
                        M802_15_4_SUCCESS); //dummy status
            }
            else
            {
                Sscs802_15_4StartDevice(
                        node,
                        interfaceIndex,
                        TRUE,
                        M802_15_4_SUCCESS); //dummy status
            }
            break;
        }
        case S802_15_4STOPDEVICE:
        {
            Sscs802_15_4StopDevice(
                    node,
                    interfaceIndex,
                    M802_15_4_SUCCESS);
            break;
        }
        case S802_15_4STARTBEACON:
        {
            break;
        }
        case S802_15_4STOPBEACON:
        {
            break;
        }
        case S802_15_4ASSORETRY:
        {
            Sscs802_15_4StartDevice(
                    node,
                    interfaceIndex,
                    TRUE,
                    M802_15_4_SUCCESS); //dummy status
            break;
        }
        case S802_15_4POLLINT:
        {
            Sscs802_15_4PollRequest(
                    node,
                    interfaceIndex);
            break;
        }
        default:
        {
            break;
        }
    }
}

// /**
// FUNCTION     Sscs802_15_4Finalize
// PURPOSE      Called at the end of simulation to collect the results of
//              the simulation of 802.15.4 protocol of the SSCS Layer.
// PARAMETERS   Node* node
//                  Node which received the message.
// RETURN       None
// NOTES        None
// **/
void Sscs802_15_4Finalize(Node* node, int interfaceIndex)
{
    Sscs802_15_4PrintStats(node, interfaceIndex);
}
