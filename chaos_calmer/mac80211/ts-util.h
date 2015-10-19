#ifndef TS_UTIL
#define TS_UTIL

#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8
#define TS_HLEN 17
#define TOT_HDR_LEN 42
#define TS_LEN 1337
#define MAC_OVERHEAD 34


#define RTP_LEN 12
#define OVERHEAD_TS 5
#define OVERHEAD_RTP 4
#define OVERHEAD_PARAM 4


#define TS_PSIZE 188

#define IS_TS(c)  (c->stream_id == 0x1234abcd)

struct tshdr{
	unsigned int stream_id;
	unsigned char bitmap;
	};

struct param_hdr{
	unsigned short frag_size;
	unsigned short prioritize;
};


struct rtp_hdr{
	unsigned short unused;
	unsigned short rtpsqnum;
};



#endif
