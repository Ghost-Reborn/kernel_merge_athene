/* Copyright (c) 2012-2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _IPA_H_
#define _IPA_H_

#include <linux/msm_ipa.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <mach/sps.h>

/**
 * enum ipa_nat_en_type - NAT setting type in IPA end-point
 */
enum ipa_nat_en_type {
	IPA_BYPASS_NAT,
	IPA_SRC_NAT,
	IPA_DST_NAT,
};

/**
 * enum ipa_mode_type - mode setting type in IPA end-point
 * @BASIC: basic mode
 * @ENABLE_FRAMING_HDLC: not currently supported
 * @ENABLE_DEFRAMING_HDLC: not currently supported
 * @DMA: all data arriving IPA will not go through IPA logic blocks, this
 *  allows IPA to work as DMA for specific pipes.
 */
enum ipa_mode_type {
	IPA_BASIC,
	IPA_ENABLE_FRAMING_HDLC,
	IPA_ENABLE_DEFRAMING_HDLC,
	IPA_DMA,
};

/**
 *  enum ipa_aggr_en_type - aggregation setting type in IPA
 *  end-point
 */
enum ipa_aggr_en_type {
	IPA_BYPASS_AGGR,
	IPA_ENABLE_AGGR,
	IPA_ENABLE_DEAGGR,
};

/**
 *  enum ipa_aggr_type - type of aggregation in IPA end-point
 */
enum ipa_aggr_type {
	IPA_MBIM_16 = 0,
	IPA_HDLC    = 1,
	IPA_TLP     = 2,
	IPA_RNDIS   = 3,
	IPA_GENERIC = 4,
	IPA_QCMAP   = 6,
};

/**
 * enum ipa_aggr_mode - global aggregation mode
 */
enum ipa_aggr_mode {
	IPA_MBIM,
	IPA_QCNCM,
};

/**
 * enum ipa_dp_evt_type - type of event client callback is
 * invoked for on data path
 * @IPA_RECEIVE: data is struct sk_buff
 * @IPA_WRITE_DONE: data is struct sk_buff
 */
enum ipa_dp_evt_type {
	IPA_RECEIVE,
	IPA_WRITE_DONE,
};

/**
 * enum hdr_total_len_or_pad_type - type vof alue held by TOTAL_LEN_OR_PAD
 * field in header configuration register.
 * @IPA_HDR_PAD: field is used as padding length
 * @IPA_HDR_TOTAL_LEN: field is used as total length
 */
enum hdr_total_len_or_pad_type {
	IPA_HDR_PAD = 0,
	IPA_HDR_TOTAL_LEN = 1,
};

/**
 * struct ipa_ep_cfg_nat - NAT configuration in IPA end-point
 * @nat_en:	This defines the default NAT mode for the pipe: in case of
 *		filter miss - the default NAT mode defines the NATing operation
 *		on the packet. Valid for Input Pipes only (IPA consumer)
 */
struct ipa_ep_cfg_nat {
	enum ipa_nat_en_type nat_en;
};

/**
 * struct ipa_ep_cfg_hdr - header configuration in IPA end-point
 *
 * @hdr_len:Header length in bytes to be added/removed. Assuming
 *			header len is constant per endpoint. Valid for
 *			both Input and Output Pipes
 * @hdr_ofst_metadata_valid:	0: Metadata_Ofst  value is invalid, i.e., no
 *			metadata within header.
 *			1: Metadata_Ofst  value is valid, i.e., metadata
 *			within header is in offset Metadata_Ofst Valid
 *			for Input Pipes only (IPA Consumer) (for output
 *			pipes, metadata already set within the header)
 * @hdr_ofst_metadata:	Offset within header in which metadata resides
 *			Size of metadata - 4bytes
 *			Example -  Stream ID/SSID/mux ID.
 *			Valid for  Input Pipes only (IPA Consumer) (for output
 *			pipes, metadata already set within the header)
 * @hdr_additional_const_len:	Defines the constant length that should be added
 *			to the payload length in order for IPA to update
 *			correctly the length field within the header
 *			(valid only in case Hdr_Ofst_Pkt_Size_Valid=1)
 *			Valid for Output Pipes (IPA Producer)
 * @hdr_ofst_pkt_size_valid:	0: Hdr_Ofst_Pkt_Size  value is invalid, i.e., no
 *			length field within the inserted header
 *			1: Hdr_Ofst_Pkt_Size  value is valid, i.e., a
 *			packet length field resides within the header
 *			Valid for Output Pipes (IPA Producer)
 * @hdr_ofst_pkt_size:	Offset within header in which packet size reside. Upon
 *			Header Insertion, IPA will update this field within the
 *			header with the packet length . Assumption is that
 *			header length field size is constant and is 2Bytes
 *			Valid for Output Pipes (IPA Producer)
 * @hdr_a5_mux:	Determines whether A5 Mux header should be added to the packet.
 *			This bit is valid only when Hdr_En=01(Header Insertion)
 *			SW should set this bit for IPA-to-A5 pipes.
 *			0: Do not insert A5 Mux Header
 *			1: Insert A5 Mux Header
 *			Valid for Output Pipes (IPA Producer)
 * @hdr_remove_additional:	bool switch, remove more of the header
 *			based on the aggregation configuration (register
 *			HDR_LEN_INC_DEAGG_HDR)
 * @hdr_metadata_reg_valid:	bool switch, metadata from
 *			register INIT_HDR_METADATA_n is valid.
 *			(relevant only for IPA Consumer pipes)
 */
struct ipa_ep_cfg_hdr {
	u32  hdr_len;
	u32  hdr_ofst_metadata_valid;
	u32  hdr_ofst_metadata;
	u32  hdr_additional_const_len;
	u32  hdr_ofst_pkt_size_valid;
	u32  hdr_ofst_pkt_size;
	u32  hdr_a5_mux;
	u32  hdr_remove_additional;
	u32  hdr_metadata_reg_valid;
};

/**
 * struct ipa_ep_cfg_hdr_ext - extended header configuration in IPA end-point
 * @hdr_pad_to_alignment: Pad packet to specified alignment
 *	(2^pad to alignment value), i.e. value of 3 means pad to 2^3 = 8 bytes
 *	alignment. Alignment is to 0,2 up to 32 bytes (IPAv2 does not support 64
 *	byte alignment). Valid for Output Pipes only (IPA Producer).
 * @hdr_total_len_or_pad_offset: Offset to length field containing either
 *	total length or pad length, per hdr_total_len_or_pad config
 * @hdr_payload_len_inc_padding: 0-IPA_ENDP_INIT_HDR_n?s HDR_OFST_PKT_SIZE does
 *	not includes padding bytes size, payload_len = packet length,
 *	1-IPA_ENDP_INIT_HDR_n?s HDR_OFST_PKT_SIZE includes
 *	padding bytes size, payload_len = packet length + padding
 * @hdr_total_len_or_pad: field is used as PAD length ot as Total length
 *	(header + packet + padding)
 * @hdr_total_len_or_pad_valid: 0-Ignore TOTAL_LEN_OR_PAD field, 1-Process
 *	TOTAL_LEN_OR_PAD field
 * @hdr_little_endian: 0-Big Endian, 1-Little Endian
 */
struct ipa_ep_cfg_hdr_ext {
	u32 hdr_pad_to_alignment;
	u32 hdr_total_len_or_pad_offset;
	bool hdr_payload_len_inc_padding;
	enum hdr_total_len_or_pad_type hdr_total_len_or_pad;
	bool hdr_total_len_or_pad_valid;
	bool hdr_little_endian;
};

/**
 * struct ipa_ep_cfg_mode - mode configuration in IPA end-point
 * @mode:	Valid for Input Pipes only (IPA Consumer)
 * @dst:	This parameter specifies the output pipe to which the packets
 *		will be routed to.
 *		This parameter is valid for Mode=DMA and not valid for
 *		Mode=Basic
 *		Valid for Input Pipes only (IPA Consumer)
 */
struct ipa_ep_cfg_mode {
	enum ipa_mode_type mode;
	enum ipa_client_type dst;
};

/**
 * struct ipa_ep_cfg_aggr - aggregation configuration in IPA end-point
 *
 * @aggr_en:	Valid for both Input and Output Pipes
 * @aggr:	aggregation type (Valid for both Input and Output Pipes)
 * @aggr_byte_limit:	Limit of aggregated packet size in KB (<=32KB) When set
 *			to 0, there is no size limitation on the aggregation.
 *			When both, Aggr_Byte_Limit and Aggr_Time_Limit are set
 *			to 0, there is no aggregation, every packet is sent
 *			independently according to the aggregation structure
 *			Valid for Output Pipes only (IPA Producer )
 * @aggr_time_limit:	Timer to close aggregated packet (<=32ms) When set to 0,
 *			there is no time limitation on the aggregation.  When
 *			both, Aggr_Byte_Limit and Aggr_Time_Limit are set to 0,
 *			there is no aggregation, every packet is sent
 *			independently according to the aggregation structure
 *			Valid for Output Pipes only (IPA Producer)
 * @aggr_pkt_limit: Defines if EOF close aggregation or not. if set to false
 *			HW closes aggregation (sends EOT) only based on its
 *			aggregation config (byte/time limit, etc). if set to
 *			true EOF closes aggregation in addition to HW based
 *			aggregation closure. Valid for Output Pipes only (IPA
 *			Producer). EOF affects only Pipes configured for
 *			generic aggregation.
 */
struct ipa_ep_cfg_aggr {
	enum ipa_aggr_en_type aggr_en;
	enum ipa_aggr_type aggr;
	u32 aggr_byte_limit;
	u32 aggr_time_limit;
	u32 aggr_pkt_limit;
};

/**
 * struct ipa_ep_cfg_route - route configuration in IPA end-point
 * @rt_tbl_hdl:	Defines the default routing table index to be used in case there
 *		is no filter rule matching, valid for Input Pipes only (IPA
 *		Consumer). Clients should set this to 0 which will cause default
 *		v4 and v6 routes setup internally by IPA driver to be used for
 *		this end-point
 */
struct ipa_ep_cfg_route {
	u32 rt_tbl_hdl;
};

/**
 * struct ipa_ep_cfg_holb - head of line blocking configuration in IPA end-point
 * @en: enable(1 => ok to drop pkt)/disable(0 => never drop pkt)
 * @tmr_val: duration in units of 128 IPA clk clock cyles [0,511], 1 clk=1.28us
 */
struct ipa_ep_cfg_holb {
	u16 en;
	u16 tmr_val;
};

/**
 * struct ipa_ep_cfg_deaggr - deaggregation configuration in IPA end-point
 * @deaggr_hdr_len: Deaggregation Header length in bytes. Valid only for Input
 *	Pipes, which are configured for ?Generic? deaggregation.
 * @packet_offset_valid: - 0: PACKET_OFFSET is not used, 1: PACKET_OFFSET is
 *	used.
 * @packet_offset_location: Location of packet offset field, which specifies
 *	the offset to the packet from the start of the packet offset field.
 * @max_packet_len: DEAGGR Max Packet Length in Bytes. A Packet with higher
 *	size wil be treated as an error. 0 - Packet Length is not Bound,
 *	IPA should not check for a Max Packet Length.
 */
struct ipa_ep_cfg_deaggr {
	u32 deaggr_hdr_len;
	bool packet_offset_valid;
	u32 packet_offset_location;
	u32 max_packet_len;
};

/**
 * enum ipa_cs_offload - checksum offload setting
 */
enum ipa_cs_offload {
	IPA_DISABLE_CS_OFFLOAD,
	IPA_ENABLE_CS_OFFLOAD_UL,
	IPA_ENABLE_CS_OFFLOAD_DL,
	IPA_CS_RSVD
};

/**
 * struct ipa_ep_cfg_cfg - IPA ENDP_INIT Configuration register
 * @frag_offload_en: - 0 - IP packet fragment handling is disabled. IP packet
 *	fragments should be sent to SW. SW is responsible for
 *	configuring filter rules, and IP packet filter exception should be
 *	used to send all fragments to SW. 1 - IP packet fragment
 *	handling is enabled. IPA checks for fragments and uses frag
 *	rules table for processing fragments. Valid only for Input Pipes
 *	(IPA Consumer)
 * @cs_offload_en: Checksum offload enable: 00: Disable checksum offload, 01:
 *	Enable checksum calculation offload (UL) - For output pipe
 *	(IPA producer) specifies that checksum trailer is to be added.
 *	For input pipe (IPA consumer) specifies presence of checksum
 *	header and IPA checksum calculation accordingly. 10: Enable
 *	checksum calculation offload (DL) - For output pipe (IPA
 *	producer) specifies that checksum trailer is to be added. For
 *	input pipe (IPA consumer) specifies IPA checksum calculation.
 *	11: Reserved
 * @cs_metadata_hdr_offset: Offset in Words (4 bytes) within header in which
 *	checksum meta info header (4 bytes) starts (UL). Values are 0-15, which
 *	mean 0 - 60 byte checksum header offset. Valid for input
 *	pipes only (IPA consumer)
 */
struct ipa_ep_cfg_cfg {
	bool frag_offload_en;
	enum ipa_cs_offload cs_offload_en;
	u8 cs_metadata_hdr_offset;
};

/**
 * struct ipa_ep_cfg_metadata_mask - Endpoint initialization hdr metadata mask
 * @metadata_mask: Mask specifying which metadata bits to write to
 *	IPA_ENDP_INIT_HDR_n.s HDR_OFST_METADATA. Only
 *	masked metadata bits (set to 1) will be written. Valid for Output
 *	Pipes only (IPA Producer)
 */
struct ipa_ep_cfg_metadata_mask {
	u32 metadata_mask;
};

/**
 * struct ipa_ep_cfg_metadata - Meta Data configuration in IPA end-point
 * @md:	This defines the meta data from tx data descriptor
 * @qmap_id: qmap id
 */
struct ipa_ep_cfg_metadata {
	u32 qmap_id;
};

/**
 * struct ipa_ep_cfg - configuration of IPA end-point
 * @nat:		NAT parmeters
 * @hdr:		Header parameters
 * @hdr_ext:		Extended header parameters
 * @mode:		Mode parameters
 * @aggr:		Aggregation parameters
 * @deaggr:		Deaggregation params
 * @route:		Routing parameters
 * @cfg:		Configuration register data
 * @metadata_mask:	Hdr metadata mask
 * @meta:		Meta Data
 */
struct ipa_ep_cfg {
	struct ipa_ep_cfg_nat nat;
	struct ipa_ep_cfg_hdr hdr;
	struct ipa_ep_cfg_hdr_ext hdr_ext;
	struct ipa_ep_cfg_mode mode;
	struct ipa_ep_cfg_aggr aggr;
	struct ipa_ep_cfg_deaggr deaggr;
	struct ipa_ep_cfg_route route;
	struct ipa_ep_cfg_cfg cfg;
	struct ipa_ep_cfg_metadata_mask metadata_mask;
	struct ipa_ep_cfg_metadata meta;
};

/**
 * struct ipa_ep_cfg_ctrl - Control configuration in IPA end-point
 * @ipa_ep_suspend: 0 - ENDP is enabled, 1 - ENDP is suspended (disabled).
 *			Valid for PROD Endpoints
 * @ipa_ep_delay:   0 - ENDP is free-running, 1 - ENDP is delayed.
 *			SW controls the data flow of an endpoint usind this bit.
 *			Valid for CONS Endpoints
 */
struct ipa_ep_cfg_ctrl {
	bool ipa_ep_suspend;
	bool ipa_ep_delay;
};

/**
 * x should be in bytes
 */
#define IPA_NUM_OF_FIFO_DESC(x) (x/sizeof(struct sps_iovec))
typedef void (*ipa_notify_cb)(void *priv, enum ipa_dp_evt_type evt,
		       unsigned long data);

/**
 * struct ipa_connect_params - low-level client connect input parameters. Either
 * client allocates the data and desc FIFO and specifies that in data+desc OR
 * specifies sizes and pipe_mem pref and IPA does the allocation.
 *
 * @ipa_ep_cfg:	IPA EP configuration
 * @client:	type of "client"
 * @client_bam_hdl:	 client SPS handle
 * @client_ep_idx:	 client PER EP index
 * @priv:	callback cookie
 * @notify:	callback
 *		priv - callback cookie evt - type of event data - data relevant
 *		to event.  May not be valid. See event_type enum for valid
 *		cases.
 * @desc_fifo_sz:	size of desc FIFO
 * @data_fifo_sz:	size of data FIFO
 * @pipe_mem_preferred:	if true, try to alloc the FIFOs in pipe mem, fallback
 *			to sys mem if pipe mem alloc fails
 * @desc:	desc FIFO meta-data when client has allocated it
 * @data:	data FIFO meta-data when client has allocated it
 * @skip_ep_cfg: boolean field that determines if EP should be configured
 *  by IPA driver
 */
struct ipa_connect_params {
	struct ipa_ep_cfg ipa_ep_cfg;
	enum ipa_client_type client;
	u32 client_bam_hdl;
	u32 client_ep_idx;
	void *priv;
	ipa_notify_cb notify;
	u32 desc_fifo_sz;
	u32 data_fifo_sz;
	bool pipe_mem_preferred;
	struct sps_mem_buffer desc;
	struct sps_mem_buffer data;
	bool skip_ep_cfg;
};

/**
 *  struct ipa_sps_params - SPS related output parameters resulting from
 *  low/high level client connect
 *  @ipa_bam_hdl:	IPA SPS handle
 *  @ipa_ep_idx:	IPA PER EP index
 *  @desc:	desc FIFO meta-data
 *  @data:	data FIFO meta-data
 */
struct ipa_sps_params {
	u32 ipa_bam_hdl;
	u32 ipa_ep_idx;
	struct sps_mem_buffer desc;
	struct sps_mem_buffer data;
};

/**
 * struct ipa_tx_intf - interface tx properties
 * @num_props:	number of tx properties
 * @prop:	the tx properties array
 */
struct ipa_tx_intf {
	u32 num_props;
	struct ipa_ioc_tx_intf_prop *prop;
};

/**
 * struct ipa_rx_intf - interface rx properties
 * @num_props:	number of rx properties
 * @prop:	the rx properties array
 */
struct ipa_rx_intf {
	u32 num_props;
	struct ipa_ioc_rx_intf_prop *prop;
};

/**
 * struct ipa_ext_intf - interface ext properties
 * @excp_pipe_valid:	is next field valid?
 * @excp_pipe:	exception packets should be routed to this pipe
 * @num_props:	number of ext properties
 * @prop:	the ext properties array
 */
struct ipa_ext_intf {
	bool excp_pipe_valid;
	enum ipa_client_type excp_pipe;
	u32 num_props;
	struct ipa_ioc_ext_intf_prop *prop;
};

/**
 * struct ipa_sys_connect_params - information needed to setup an IPA end-point
 * in system-BAM mode
 * @ipa_ep_cfg:	IPA EP configuration
 * @client:	the type of client who "owns" the EP
 * @desc_fifo_sz:	size of desc FIFO
 * @priv:	callback cookie
 * @notify:	callback
 *		priv - callback cookie
 *		evt - type of event
 *		data - data relevant to event.  May not be valid. See event_type
 *		enum for valid cases.
 * @skip_ep_cfg: boolean field that determines if EP should be configured
 *  by IPA driver
 */
struct ipa_sys_connect_params {
	struct ipa_ep_cfg ipa_ep_cfg;
	enum ipa_client_type client;
	u32 desc_fifo_sz;
	void *priv;
	ipa_notify_cb notify;
	bool skip_ep_cfg;
};

/**
 * struct ipa_tx_meta - meta-data for the TX packet
 * @mbim_stream_id:	the stream ID used in NDP signature
 * @mbim_stream_id_valid:	 is above field valid?
 */
struct ipa_tx_meta {
	u8 mbim_stream_id;
	bool mbim_stream_id_valid;
	u8 pkt_init_dst_ep;
	bool pkt_init_dst_ep_valid;
	bool pkt_init_dst_ep_remote;
};

/**
 * typedef ipa_msg_free_fn - callback function
 * @param buff - [in] the message payload to free
 * @param len - [in] size of message payload
 * @param type - [in] the message type
 *
 * Message callback registered by kernel client with IPA driver to
 * free message payload after IPA driver processing is complete
 *
 * No return value
 */
typedef void (*ipa_msg_free_fn)(void *buff, u32 len, u32 type);

/**
 * typedef ipa_msg_pull_fn - callback function
 * @param buff - [in] where to copy message payload
 * @param len - [in] size of buffer to copy payload into
 * @param type - [in] the message type
 *
 * Message callback registered by kernel client with IPA driver for
 * IPA driver to pull messages from the kernel client upon demand from
 * user-space
 *
 * Returns how many bytes were copied into the buffer.
 */
typedef int (*ipa_msg_pull_fn)(void *buff, u32 len, u32 type);

/**
 * enum ipa_bridge_dir - direction of the bridge from air interface perspective
 *
 * IPA bridge direction
 */
enum ipa_bridge_dir {
	IPA_BRIDGE_DIR_DL,
	IPA_BRIDGE_DIR_UL,
	IPA_BRIDGE_DIR_MAX
};

/**
 * enum ipa_bridge_type - type of SW bridge
 *
 * IPA bridge type
 */
enum ipa_bridge_type {
	IPA_BRIDGE_TYPE_TETHERED,
	IPA_BRIDGE_TYPE_EMBEDDED,
	IPA_BRIDGE_TYPE_MAX
};

/**
 * enum ipa_rm_event - IPA RM events
 *
 * Indicate the resource state change
 */
enum ipa_rm_event {
	IPA_RM_RESOURCE_GRANTED,
	IPA_RM_RESOURCE_RELEASED
};

typedef void (*ipa_rm_notify_cb)(void *user_data,
		enum ipa_rm_event event,
		unsigned long data);
/**
 * struct ipa_rm_register_params - information needed to
 *      register IPA RM client with IPA RM
 *
 * @user_data: IPA RM client provided information
 *		to be passed to notify_cb callback below
 * @notify_cb: callback which is called by resource
 *		to notify the IPA RM client about its state
 *		change IPA RM client is expected to perform non
 *		blocking operations only in notify_cb and
 *		release notification context as soon as
 *		possible.
 */
struct ipa_rm_register_params {
	void *user_data;
	ipa_rm_notify_cb notify_cb;
};

/**
 * struct ipa_rm_create_params - information needed to initialize
 *				the resource
 * @name: resource name
 * @reg_params: register parameters, contains are ignored
 *		for consumer resource NULL should be provided
 *		for consumer resource
 * @request_resource: function which should be called to request resource,
 *			NULL should be provided for producer resource
 * @release_resource: function which should be called to release resource,
 *			NULL should be provided for producer resource
 *
 * IPA RM client is expected to perform non blocking operations only
 * in request_resource and release_resource functions and
 * release notification context as soon as possible.
 */
struct ipa_rm_create_params {
	enum ipa_rm_resource_name name;
	struct ipa_rm_register_params reg_params;
	int (*request_resource)(void);
	int (*release_resource)(void);
};

#define A2_MUX_HDR_NAME_V4_PREF "dmux_hdr_v4_"
#define A2_MUX_HDR_NAME_V6_PREF "dmux_hdr_v6_"

enum a2_mux_event_type {
	A2_MUX_RECEIVE,
	A2_MUX_WRITE_DONE
};

enum a2_mux_logical_channel_id {
	A2_MUX_WWAN_0,
	A2_MUX_WWAN_1,
	A2_MUX_WWAN_2,
	A2_MUX_WWAN_3,
	A2_MUX_WWAN_4,
	A2_MUX_WWAN_5,
	A2_MUX_WWAN_6,
	A2_MUX_WWAN_7,
	A2_MUX_TETHERED_0,
	A2_MUX_RESERVED_9,
	A2_MUX_MULTI_RMNET_10,
	A2_MUX_MULTI_RMNET_11,
	A2_MUX_MULTI_RMNET_12,
	A2_MUX_MULTI_MBIM_13,
	A2_MUX_MULTI_MBIM_14,
	A2_MUX_MULTI_MBIM_15,
	A2_MUX_MULTI_MBIM_16,
	A2_MUX_MULTI_MBIM_17,
	A2_MUX_MULTI_MBIM_18,
	A2_MUX_MULTI_MBIM_19,
	A2_MUX_MULTI_MBIM_20,
	A2_MUX_NUM_CHANNELS
};

typedef void (*a2_mux_notify_cb)(void *user_data,
		enum a2_mux_event_type event,
		unsigned long data);

/**
 * enum teth_tethering_mode - Tethering mode (Rmnet / MBIM)
 */
enum teth_tethering_mode {
	TETH_TETHERING_MODE_RMNET,
	TETH_TETHERING_MODE_MBIM,
	TETH_TETHERING_MODE_MAX,
};

/**
 * teth_bridge_init_params - Parameters used for in/out USB API
 * @usb_notify_cb:	Callback function which should be used by the caller.
 * Output parameter.
 * @private_data:	Data for the callback function. Should be used by the
 * caller. Output parameter.
 * @skip_ep_cfg: boolean field that determines if Apps-processor
 *  should or should not confiugre this end-point.
 */
struct teth_bridge_init_params {
	ipa_notify_cb usb_notify_cb;
	void *private_data;
	enum ipa_client_type client;
	bool skip_ep_cfg;
};

/**
 * struct teth_bridge_connect_params - Parameters used in teth_bridge_connect()
 * @ipa_usb_pipe_hdl:	IPA to USB pipe handle, returned from ipa_connect()
 * @usb_ipa_pipe_hdl:	USB to IPA pipe handle, returned from ipa_connect()
 * @tethering_mode:	Rmnet or MBIM
 * @ipa_client_type:    IPA "client" name (IPA_CLIENT_USB#_PROD)
 */
struct teth_bridge_connect_params {
	u32 ipa_usb_pipe_hdl;
	u32 usb_ipa_pipe_hdl;
	enum teth_tethering_mode tethering_mode;
	enum ipa_client_type client_type;
};

/**
 * struct  ipa_tx_data_desc - information needed
 * to send data packet to HW link: link to data descriptors
 * priv: client specific private data
 * @pyld_buffer: pointer to the data buffer that holds frame
 * @pyld_len: length of the data packet
 */
struct ipa_tx_data_desc {
	struct list_head link;
	void *priv;
	void *pyld_buffer;
	u16  pyld_len;
};

/**
 * struct  ipa_rx_data - information needed
 * to send to wlan driver on receiving data from ipa hw
 * @skb: skb
 * @dma_addr: DMA address of this Rx packet
 */
struct ipa_rx_data {
	struct sk_buff *skb;
	dma_addr_t dma_addr;
};

enum ipa_irq_type {
	IPA_BAD_SNOC_ACCESS_IRQ = 0,
	IPA_EOT_COAL_IRQ,
	IPA_UC_IRQ_0,
	IPA_UC_IRQ_1,
	IPA_UC_IRQ_2,
	IPA_UC_IRQ_3,
	IPA_UC_IN_Q_NOT_EMPTY_IRQ,
	IPA_UC_RX_CMD_Q_NOT_FULL_IRQ,
	IPA_UC_TX_CMD_Q_NOT_FULL_IRQ,
	IPA_UC_TO_PROC_ACK_Q_NOT_FULL_IRQ,
	IPA_PROC_TO_UC_ACK_Q_NOT_EMPTY_IRQ,
	IPA_RX_ERR_IRQ,
	IPA_DEAGGR_ERR_IRQ,
	IPA_TX_ERR_IRQ,
	IPA_STEP_MODE_IRQ,
	IPA_PROC_ERR_IRQ,
	IPA_TX_SUSPEND_IRQ = 16,
	IPA_TX_HOLB_DROP_IRQ = 17,

	IPA_IRQ_MAX
};


/**
 * typedef ipa_irq_handler_t - irq handler/callback type
 * @param ipa_irq_type - [in] interrupt type
 * @param private_data - [in, out] the client private data
 * @param interrupt_data - [out] interrupt information data
 *
 * callback registered by ipa_add_interrupt_handler function to
 * handle a specific interrupt type
 *
 * No return value
 */
typedef void (*ipa_irq_handler_t)(enum ipa_irq_type interrupt,
				void *private_data,
				void *interrupt_data);
#ifdef CONFIG_IPA

/*
 * Connect / Disconnect
 */
int ipa_connect(const struct ipa_connect_params *in, struct ipa_sps_params *sps,
		u32 *clnt_hdl);
int ipa_disconnect(u32 clnt_hdl);

/*
 * Resume / Suspend
 */
int ipa_resume(u32 clnt_hdl);

int ipa_suspend(u32 clnt_hdl);

/*
 * Configuration
 */
int ipa_cfg_ep(u32 clnt_hdl, const struct ipa_ep_cfg *ipa_ep_cfg);

int ipa_cfg_ep_nat(u32 clnt_hdl, const struct ipa_ep_cfg_nat *ipa_ep_cfg);

int ipa_cfg_ep_hdr(u32 clnt_hdl, const struct ipa_ep_cfg_hdr *ipa_ep_cfg);

int ipa_cfg_ep_hdr_ext(u32 clnt_hdl,
			const struct ipa_ep_cfg_hdr_ext *ipa_ep_cfg);

int ipa_cfg_ep_mode(u32 clnt_hdl, const struct ipa_ep_cfg_mode *ipa_ep_cfg);

int ipa_cfg_ep_aggr(u32 clnt_hdl, const struct ipa_ep_cfg_aggr *ipa_ep_cfg);

int ipa_cfg_ep_deaggr(u32 clnt_hdl,
		      const struct ipa_ep_cfg_deaggr *ipa_ep_cfg);

int ipa_cfg_ep_route(u32 clnt_hdl, const struct ipa_ep_cfg_route *ipa_ep_cfg);

int ipa_cfg_ep_holb(u32 clnt_hdl, const struct ipa_ep_cfg_holb *ipa_ep_cfg);

int ipa_cfg_ep_cfg(u32 clnt_hdl, const struct ipa_ep_cfg_cfg *ipa_ep_cfg);

int ipa_cfg_ep_metadata_mask(u32 clnt_hdl, const struct ipa_ep_cfg_metadata_mask
		*ipa_ep_cfg);

int ipa_cfg_ep_holb_by_client(enum ipa_client_type client,
				const struct ipa_ep_cfg_holb *ipa_ep_cfg);

int ipa_cfg_ep_ctrl(u32 clnt_hdl, const struct ipa_ep_cfg_ctrl *ep_ctrl);

/*
 * Header removal / addition
 */
int ipa_add_hdr(struct ipa_ioc_add_hdr *hdrs);

int ipa_del_hdr(struct ipa_ioc_del_hdr *hdls);

int ipa_commit_hdr(void);

int ipa_reset_hdr(void);

int ipa_get_hdr(struct ipa_ioc_get_hdr *lookup);

int ipa_put_hdr(u32 hdr_hdl);

int ipa_copy_hdr(struct ipa_ioc_copy_hdr *copy);

/*
 * Routing
 */
int ipa_add_rt_rule(struct ipa_ioc_add_rt_rule *rules);

int ipa_del_rt_rule(struct ipa_ioc_del_rt_rule *hdls);

int ipa_commit_rt(enum ipa_ip_type ip);

int ipa_reset_rt(enum ipa_ip_type ip);

int ipa_get_rt_tbl(struct ipa_ioc_get_rt_tbl *lookup);

int ipa_put_rt_tbl(u32 rt_tbl_hdl);

int ipa_query_rt_index(struct ipa_ioc_get_rt_tbl_indx *in);

/*
 * Filtering
 */
int ipa_add_flt_rule(struct ipa_ioc_add_flt_rule *rules);

int ipa_del_flt_rule(struct ipa_ioc_del_flt_rule *hdls);

int ipa_commit_flt(enum ipa_ip_type ip);

int ipa_reset_flt(enum ipa_ip_type ip);

/*
 * NAT
 */
int allocate_nat_device(struct ipa_ioc_nat_alloc_mem *mem);

int ipa_nat_init_cmd(struct ipa_ioc_v4_nat_init *init);

int ipa_nat_dma_cmd(struct ipa_ioc_nat_dma_cmd *dma);

int ipa_nat_del_cmd(struct ipa_ioc_v4_nat_del *del);

/*
 * Messaging
 */
int ipa_send_msg(struct ipa_msg_meta *meta, void *buff,
		  ipa_msg_free_fn callback);
int ipa_register_pull_msg(struct ipa_msg_meta *meta, ipa_msg_pull_fn callback);
int ipa_deregister_pull_msg(struct ipa_msg_meta *meta);

/*
 * Interface
 */
int ipa_register_intf(const char *name, const struct ipa_tx_intf *tx,
		       const struct ipa_rx_intf *rx);
int ipa_register_intf_ext(const char *name, const struct ipa_tx_intf *tx,
		       const struct ipa_rx_intf *rx,
		       const struct ipa_ext_intf *ext);
int ipa_deregister_intf(const char *name);

/*
 * Aggregation
 */
int ipa_set_aggr_mode(enum ipa_aggr_mode mode);

int ipa_set_qcncm_ndp_sig(char sig[3]);

int ipa_set_single_ndp_per_mbim(bool enable);

/*
 * SW bridge (between IPA and A2)
 */
int ipa_bridge_setup(enum ipa_bridge_dir dir, enum ipa_bridge_type type,
		     struct ipa_sys_connect_params *sys_in, u32 *clnt_hdl);
int ipa_bridge_teardown(enum ipa_bridge_dir dir, enum ipa_bridge_type type,
			u32 clnt_hdl);


/*
 * Data path
 */
int ipa_tx_dp(enum ipa_client_type dst, struct sk_buff *skb,
		struct ipa_tx_meta *metadata);

/*
 * To transfer multiple data packets
 * While passing the data descriptor list, the anchor node
 * should be of type struct ipa_tx_data_desc not list_head
*/
int ipa_tx_dp_mul(enum ipa_client_type dst,
			struct ipa_tx_data_desc *data_desc);

void ipa_free_skb(struct ipa_rx_data *);

/*
 * System pipes
 */
int ipa_setup_sys_pipe(struct ipa_sys_connect_params *sys_in, u32 *clnt_hdl);

int ipa_teardown_sys_pipe(u32 clnt_hdl);

/*
 * Resource manager
 */
int ipa_rm_create_resource(struct ipa_rm_create_params *create_params);

int ipa_rm_delete_resource(enum ipa_rm_resource_name resource_name);

int ipa_rm_register(enum ipa_rm_resource_name resource_name,
			struct ipa_rm_register_params *reg_params);

int ipa_rm_deregister(enum ipa_rm_resource_name resource_name,
			struct ipa_rm_register_params *reg_params);

int ipa_rm_add_dependency(enum ipa_rm_resource_name resource_name,
			enum ipa_rm_resource_name depends_on_name);

int ipa_rm_delete_dependency(enum ipa_rm_resource_name resource_name,
			enum ipa_rm_resource_name depends_on_name);

int ipa_rm_request_resource(enum ipa_rm_resource_name resource_name);

int ipa_rm_release_resource(enum ipa_rm_resource_name resource_name);

int ipa_rm_notify_completion(enum ipa_rm_event event,
		enum ipa_rm_resource_name resource_name);

int ipa_rm_inactivity_timer_init(enum ipa_rm_resource_name resource_name,
				 unsigned long msecs);

int ipa_rm_inactivity_timer_destroy(enum ipa_rm_resource_name resource_name);

int ipa_rm_inactivity_timer_request_resource(
				enum ipa_rm_resource_name resource_name);

int ipa_rm_inactivity_timer_release_resource(
				enum ipa_rm_resource_name resource_name);

/*
 * a2 service
 */
int a2_mux_open_channel(enum a2_mux_logical_channel_id lcid,
			void *user_data,
			a2_mux_notify_cb notify_cb);

int a2_mux_close_channel(enum a2_mux_logical_channel_id lcid);

int a2_mux_write(enum a2_mux_logical_channel_id lcid, struct sk_buff *skb);

int a2_mux_is_ch_empty(enum a2_mux_logical_channel_id lcid);

int a2_mux_is_ch_low(enum a2_mux_logical_channel_id lcid);

int a2_mux_is_ch_full(enum a2_mux_logical_channel_id lcid);

int a2_mux_get_client_handles(enum a2_mux_logical_channel_id lcid,
		unsigned int *clnt_cons_handle,
		unsigned int *clnt_prod_handle);

/*
 * Tethering bridge (Rmnet / MBIM)
 */
int teth_bridge_init(struct teth_bridge_init_params *params);

int teth_bridge_disconnect(enum ipa_client_type client);

int teth_bridge_connect(struct teth_bridge_connect_params *connect_params);

int teth_bridge_set_mbim_aggr_params(struct teth_aggr_params *aggr_params,
		enum ipa_client_type client);

void ipa_bam_reg_dump(void);
bool ipa_emb_ul_pipes_empty(void);

/* mux id*/
int ipa_write_qmap_id(struct ipa_ioc_write_qmapid *param_in);

/*interrupts*/
int ipa_add_interrupt_handler(enum ipa_irq_type interrupt,
		ipa_irq_handler_t handler,
		bool deferred_flag,
		void *private_data);
int ipa_remove_interrupt_handler(enum ipa_irq_type interrupt);

int ipa_get_ep_mapping(enum ipa_client_type client);

#else /* CONFIG_IPA */

static inline int a2_mux_open_channel(enum a2_mux_logical_channel_id lcid,
	void *user_data, a2_mux_notify_cb notify_cb)
{
	return -EPERM;
}

static inline int a2_mux_close_channel(enum a2_mux_logical_channel_id lcid)
{
	return -EPERM;
}

static inline int a2_mux_write(enum a2_mux_logical_channel_id lcid,
			       struct sk_buff *skb)
{
	return -EPERM;
}

static inline int a2_mux_is_ch_empty(enum a2_mux_logical_channel_id lcid)
{
	return -EPERM;
}

static inline int a2_mux_is_ch_low(enum a2_mux_logical_channel_id lcid)
{
	return -EPERM;
}

static inline int a2_mux_is_ch_full(enum a2_mux_logical_channel_id lcid)
{
	return -EPERM;
}

static inline int a2_mux_get_client_handles(
	enum a2_mux_logical_channel_id lcid, unsigned int *clnt_cons_handle,
	unsigned int *clnt_prod_handle)
{
	return -EPERM;
}

/*
 * Connect / Disconnect
 */
static inline int ipa_connect(const struct ipa_connect_params *in,
		struct ipa_sps_params *sps,	u32 *clnt_hdl)
{
	return -EPERM;
}

static inline int ipa_disconnect(u32 clnt_hdl)
{
	return -EPERM;
}

/*
 * Resume / Suspend
 */
static inline int ipa_resume(u32 clnt_hdl)
{
	return -EPERM;
}

static inline int ipa_suspend(u32 clnt_hdl)
{
	return -EPERM;
}

/*
 * Configuration
 */
static inline int ipa_cfg_ep(u32 clnt_hdl,
		const struct ipa_ep_cfg *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_nat(u32 clnt_hdl,
		const struct ipa_ep_cfg_nat *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_hdr(u32 clnt_hdl,
		const struct ipa_ep_cfg_hdr *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_hdr_ext(u32 clnt_hdl,
		const struct ipa_ep_cfg_hdr_ext *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_mode(u32 clnt_hdl,
		const struct ipa_ep_cfg_mode *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_aggr(u32 clnt_hdl,
		const struct ipa_ep_cfg_aggr *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_deaggr(u32 clnt_hdl,
		const struct ipa_ep_cfg_deaggr *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_route(u32 clnt_hdl,
		const struct ipa_ep_cfg_route *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_holb(u32 clnt_hdl,
		const struct ipa_ep_cfg_holb *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_cfg(u32 clnt_hdl,
		const struct ipa_ep_cfg_cfg *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_metadata_mask(u32 clnt_hdl,
		const struct ipa_ep_cfg_metadata_mask *ipa_ep_cfg)
{
	return -EPERM;
}

static inline int ipa_cfg_ep_ctrl(u32 clnt_hdl,
			const struct ipa_ep_cfg_ctrl *ep_ctrl)
{
	return -EPERM;
}

/*
 * Header removal / addition
 */
static inline int ipa_add_hdr(struct ipa_ioc_add_hdr *hdrs)
{
	return -EPERM;
}

static inline int ipa_del_hdr(struct ipa_ioc_del_hdr *hdls)
{
	return -EPERM;
}

static inline int ipa_commit_hdr(void)
{
	return -EPERM;
}

static inline int ipa_reset_hdr(void)
{
	return -EPERM;
}

static inline int ipa_get_hdr(struct ipa_ioc_get_hdr *lookup)
{
	return -EPERM;
}

static inline int ipa_put_hdr(u32 hdr_hdl)
{
	return -EPERM;
}

static inline int ipa_copy_hdr(struct ipa_ioc_copy_hdr *copy)
{
	return -EPERM;
}

/*
 * Routing
 */
static inline int ipa_add_rt_rule(struct ipa_ioc_add_rt_rule *rules)
{
	return -EPERM;
}

static inline int ipa_del_rt_rule(struct ipa_ioc_del_rt_rule *hdls)
{
	return -EPERM;
}

static inline int ipa_commit_rt(enum ipa_ip_type ip)
{
	return -EPERM;
}

static inline int ipa_reset_rt(enum ipa_ip_type ip)
{
	return -EPERM;
}

static inline int ipa_get_rt_tbl(struct ipa_ioc_get_rt_tbl *lookup)
{
	return -EPERM;
}

static inline int ipa_put_rt_tbl(u32 rt_tbl_hdl)
{
	return -EPERM;
}

static inline int ipa_query_rt_index(struct ipa_ioc_get_rt_tbl_indx *in)
{
	return -EPERM;
}

/*
 * Filtering
 */
static inline int ipa_add_flt_rule(struct ipa_ioc_add_flt_rule *rules)
{
	return -EPERM;
}

static inline int ipa_del_flt_rule(struct ipa_ioc_del_flt_rule *hdls)
{
	return -EPERM;
}

static inline int ipa_commit_flt(enum ipa_ip_type ip)
{
	return -EPERM;
}

static inline int ipa_reset_flt(enum ipa_ip_type ip)
{
	return -EPERM;
}

/*
 * NAT
 */
static inline int allocate_nat_device(struct ipa_ioc_nat_alloc_mem *mem)
{
	return -EPERM;
}


static inline int ipa_nat_init_cmd(struct ipa_ioc_v4_nat_init *init)
{
	return -EPERM;
}


static inline int ipa_nat_dma_cmd(struct ipa_ioc_nat_dma_cmd *dma)
{
	return -EPERM;
}


static inline int ipa_nat_del_cmd(struct ipa_ioc_v4_nat_del *del)
{
	return -EPERM;
}

/*
 * Messaging
 */
static inline int ipa_send_msg(struct ipa_msg_meta *meta, void *buff,
		ipa_msg_free_fn callback)
{
	return -EPERM;
}

static inline int ipa_register_pull_msg(struct ipa_msg_meta *meta,
		ipa_msg_pull_fn callback)
{
	return -EPERM;
}

static inline int ipa_deregister_pull_msg(struct ipa_msg_meta *meta)
{
	return -EPERM;
}

/*
 * Interface
 */
static inline int ipa_register_intf(const char *name,
				     const struct ipa_tx_intf *tx,
				     const struct ipa_rx_intf *rx)
{
	return -EPERM;
}

static inline int ipa_register_intf_ext(const char *name,
		const struct ipa_tx_intf *tx,
		const struct ipa_rx_intf *rx,
		const struct ipa_ext_intf *ext)
{
	return -EPERM;
}

static inline int ipa_deregister_intf(const char *name)
{
	return -EPERM;
}

/*
 * Aggregation
 */
static inline int ipa_set_aggr_mode(enum ipa_aggr_mode mode)
{
	return -EPERM;
}

static inline int ipa_set_qcncm_ndp_sig(char sig[3])
{
	return -EPERM;
}

static inline int ipa_set_single_ndp_per_mbim(bool enable)
{
	return -EPERM;
}

/*
 * SW bridge (between IPA and A2)
 */
static inline int ipa_bridge_setup(enum ipa_bridge_dir dir,
				    enum ipa_bridge_type type,
				    struct ipa_sys_connect_params *sys_in,
				    u32 *clnt_hdl)
{
	return -EPERM;
}

static inline int ipa_bridge_teardown(enum ipa_bridge_dir dir,
				       enum ipa_bridge_type type,
				      u32 clnt_hdl)
{
	return -EPERM;
}

/*
 * Data path
 */
static inline int ipa_tx_dp(enum ipa_client_type dst, struct sk_buff *skb,
		struct ipa_tx_meta *metadata)
{
	return -EPERM;
}

/*
 * To transfer multiple data packets
*/
static inline int ipa_tx_dp_mul(
	enum ipa_client_type dst,
	struct ipa_tx_data_desc *data_desc)
{
	return -EPERM;
}

static inline void ipa_free_skb(struct ipa_rx_data *rx_in)
{
	return;
}


/*
 * System pipes
 */
static inline int ipa_setup_sys_pipe(struct ipa_sys_connect_params *sys_in,
		u32 *clnt_hdl)
{
	return -EPERM;
}

static inline int ipa_teardown_sys_pipe(u32 clnt_hdl)
{
	return -EPERM;
}

/*
 * Resource manager
 */
static inline int ipa_rm_create_resource(
		struct ipa_rm_create_params *create_params)
{
	return -EPERM;
}

static inline int ipa_rm_delete_resource(
		enum ipa_rm_resource_name resource_name)
{
	return -EPERM;
}

static inline int ipa_rm_register(enum ipa_rm_resource_name resource_name,
			struct ipa_rm_register_params *reg_params)
{
	return -EPERM;
}

static inline int ipa_rm_deregister(enum ipa_rm_resource_name resource_name,
			struct ipa_rm_register_params *reg_params)
{
	return -EPERM;
}

static inline int ipa_rm_add_dependency(
		enum ipa_rm_resource_name resource_name,
		enum ipa_rm_resource_name depends_on_name)
{
	return -EPERM;
}

static inline int ipa_rm_delete_dependency(
		enum ipa_rm_resource_name resource_name,
		enum ipa_rm_resource_name depends_on_name)
{
	return -EPERM;
}

static inline int ipa_rm_request_resource(
		enum ipa_rm_resource_name resource_name)
{
	return -EPERM;
}

static inline int ipa_rm_release_resource(
		enum ipa_rm_resource_name resource_name)
{
	return -EPERM;
}

static inline int ipa_rm_notify_completion(enum ipa_rm_event event,
		enum ipa_rm_resource_name resource_name)
{
	return -EPERM;
}

static inline int ipa_rm_inactivity_timer_init(
		enum ipa_rm_resource_name resource_name,
			unsigned long msecs)
{
	return -EPERM;
}

static inline int ipa_rm_inactivity_timer_destroy(
		enum ipa_rm_resource_name resource_name)
{
	return -EPERM;
}

static inline int ipa_rm_inactivity_timer_request_resource(
				enum ipa_rm_resource_name resource_name)
{
	return -EPERM;
}

static inline int ipa_rm_inactivity_timer_release_resource(
				enum ipa_rm_resource_name resource_name)
{
	return -EPERM;
}

/*
 * Tethering bridge (Rmnetm / MBIM)
 */
static inline int teth_bridge_init(struct teth_bridge_init_params *params)
{
	return -EPERM;
}

static inline int teth_bridge_disconnect(enum ipa_client_type client)
{
	return -EPERM;
}

static inline int teth_bridge_connect(struct teth_bridge_connect_params
				      *connect_params)
{
	return -EPERM;
}

static inline int teth_bridge_set_mbim_aggr_params(
				struct teth_aggr_params *aggr_params,
				enum ipa_client_type client)
{
	return -EPERM;
}

static inline void ipa_bam_reg_dump(void)
{
	return;
}

static inline bool ipa_emb_ul_pipes_empty(void)
{
	return false;
}

/* mux id */
static inline int ipa_write_qmap_id(struct ipa_ioc_write_qmapid *param_in)
{
	return -EPERM;
}

/* interrupts */
static inline int ipa_add_interrupt_handler(enum ipa_irq_type interrupt,
		ipa_irq_handler_t handler,
		bool deferred_flag,
		void *private_data)
{
	return -EPERM;
}

static inline int ipa_remove_interrupt_handler(enum ipa_irq_type interrupt)
{
	return -EPERM;
}

static inline int ipa_get_ep_mapping(enum ipa_client_type client)
{
	return -EPERM;
}
#endif /* CONFIG_IPA*/

#endif /* _IPA_H_ */
