#pragma once

#include "../Common.h"

/* Arbitrary payload size for ICMP */
#define ICMP_MIN_PAYLOAD (32) 

/* TCP Flag bits struct */
typedef struct {
	struct {
		unsigned short : 7; /* data offset + reserved, not used in this struct */
		unsigned short ns : 1;
		unsigned short cwr : 1;
		unsigned short ece : 1;
		unsigned short urg : 1;
		unsigned short ack : 1;
		unsigned short psh : 1;
		unsigned short rst : 1;
		unsigned short syn : 1;
		unsigned short fin : 1;
	};
}TcpFlagBits;

/* From wikipedia TCP Header table */
/* TCP header struct */
typedef struct {
	uint16_t src;
	uint16_t dst;
	uint32_t sequence;
	uint32_t ack_num;
	union {
		uint16_t data_flags;
		struct {
			uint16_t data_offset : 4;
			uint16_t reserved : 3;
			uint16_t flags : 9;
		};
		TcpFlagBits bit;
	};
	uint16_t window_size;
	uint16_t checksum;
	uint16_t urgent_ptr;
}TcpHdr;

/* ICMP Packet structure (= icmp header + payload) */
//typedef struct {
//	SceNetIcmpHeader hdr;
//	char payload[ICMP_MIN_PAYLOAD];
//}IcmpPacket;

/* TCP Packet structure (= tcp header + icmp packet) */
//typedef struct {
//	TcpHdr hdr;
//	IcmpPacket icmp;
//}TcpPacket;