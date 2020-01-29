#define OpPoll             0x2000   //ArtPoll packet, no other data is contained in this UDP packet.
#define OpPollReply        0x2100   //ArtPollReply Packet. It contains device status information.
#define OpDiagData         0x2300   //Diagnostics and data logging packet.
#define OpCommand          0x2400   //Used to send text based parameter commands.
#define OpDmx              0x5000   //ArtDmx data packet. It contains zero start code DMX512 information for a single Universe.
#define OpNzs              0x5100   //ArtNzs data packet. It contains non-zero start code (except RDM) DMX512 information for a single Universe.
#define OpSync             0x5200   //ArtSync data packet. It is used to force synchronous transfer of ArtDmx packets to a node’s output.
#define OpAddress          0x6000   //ArtAddress packet. It contains remote programming information for a Node.
#define OpInput            0x7000   //ArtInput packet. It contains enable – disable data for DMX inputs.
#define OpTodRequest       0x8000   //ArtTodRequest packet. It is used to request a Table of Devices (ToD) for RDM discovery.
#define OpTodData          0x8100   //ArtTodData packet. It is used to send a Table of Devices (ToD) for RDM discovery.
#define OpTodControl       0x8200   //ArtTodControl packet. It is used to send RDM discovery control messages.
#define OpRdm              0x8300   //ArtRdm packet. It is used to send all non discovery RDM messages.
#define OpRdmSub           0x8400   //ArtRdmSub packet. It is used to send compressed, RDM Sub-Device data.
#define OpVideoSetup       0xa010   //ArtVideoSetup packet. It contains video screen setup information for nodes that implement the extended video features.
#define OpVideoPalette     0xa020   //ArtVideoPalette packet. It contains colour palette setup information for nodes that implement the extended video features.
#define OpVideoData        0xa040   //ArtVideoData packet. It contains display data for nodes that implement the extended video features.
#define OpMacMaster        0xf000   //This packet is deprecated.
#define OpMacSlave         0xf100   //This packet is deprecated.
#define OpFirmwareMaster   0xf200   //ArtFirmwareMaster packet. It is used to upload new firmware or firmware extensions to the Node.
#define OpFirmwareReply    0xf300   //ArtFirmwareReply packet. It is returned by the node to acknowledge receipt of an ArtFirmwareMaster packet or ArtFileTnMaster packet.
#define OpFileTnMaster     0xf400   //Uploads user file to node.
#define OpFileFnMaster     0xf500   //Downloads user file from node.
#define OpFileFnReply      0xf600   //Server to Node acknowledge for download packets.
#define OpIpProg           0xf800   //ArtIpProg packet. It is used to reprogramme the IP address and Mask of the Node.
#define OpIpProgReply      0xf900   //ArtIpProgReply packet. It is returned by the node to acknowledge receipt of an ArtIpProg packet.
#define OpMedia            0x9000   //ArtMedia packet. It is Unicast by a Media Server and acted upon by a Controller.
#define OpMediaPatch       0x9100   //ArtMediaPatch packet. It is Unicast by a Controller and acted upon by a Media Server.
#define OpMediaControl     0x9200   //ArtMediaControl packet. It is Unicast by a Controller and acted upon by a Media Server.
#define OpMediaContrlReply 0x9300 //ArtMediaControlReply packet. It is Unicast by a Media Server and acted upon by a Controller.
#define OpTimeCode         0x9700   //ArtTimeCode packet. It is used to transport time code over the network.
#define OpTimeSync         0x9800   //Used to synchronise real time date and clock
#define OpTrigger          0x9900   //Used to send trigger macros
#define OpDirectory        0x9a00   //Requests a node's file list
#define OpDirectoryReply   0x9b00   //Replies to OpDirectory with file list