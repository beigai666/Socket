#ifndef MESSAGE_H
#define MESSAGE_H
#include <arpa/inet.h>
struct tcp_info
{
	u_int8_t	tcpi_state;
	u_int8_t	tcpi_ca_state;
	u_int8_t	tcpi_retransmits;
	u_int8_t	tcpi_probes;
	u_int8_t	tcpi_backoff;
	u_int8_t	tcpi_options;
	u_int8_t	tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;

	u_int32_t	tcpi_rto;
	u_int32_t	tcpi_ato;
	u_int32_t	tcpi_snd_mss;
	u_int32_t	tcpi_rcv_mss;

	u_int32_t	tcpi_unacked;
	u_int32_t	tcpi_sacked;
	u_int32_t	tcpi_lost;
	u_int32_t	tcpi_retrans;
	u_int32_t	tcpi_fackets;

	/* Times. */
	u_int32_t	tcpi_last_data_sent;
	u_int32_t	tcpi_last_ack_sent;	/* Not remembered, sorry.  */
	u_int32_t	tcpi_last_data_recv;
	u_int32_t	tcpi_last_ack_recv;

	/* Metrics. */
	u_int32_t	tcpi_pmtu;
	u_int32_t	tcpi_rcv_ssthresh;
	u_int32_t	tcpi_rtt;
	u_int32_t	tcpi_rttvar;
	u_int32_t	tcpi_snd_ssthresh;
	u_int32_t	tcpi_snd_cwnd;
	u_int32_t	tcpi_advmss;
	u_int32_t	tcpi_reordering;

	u_int32_t	tcpi_rcv_rtt;
	u_int32_t	tcpi_rcv_space;

	u_int32_t	tcpi_total_retrans;
};
enum
{
	TCP_ESTABLISHED = 1,
	TCP_SYN_SENT,
	TCP_SYN_RECV,
	TCP_FIN_WAIT1,
	TCP_FIN_WAIT2,
	TCP_TIME_WAIT,
	TCP_CLOSE,
	TCP_CLOSE_WAIT,
	TCP_LAST_ACK,
	TCP_LISTEN,
	TCP_CLOSING   /* now a valid state */
};
typedef struct message
{
    unsigned short type;
    unsigned short cmd;
    unsigned short index;
    unsigned short total;
    unsigned int length;
    unsigned char payload[];
} Message;

Message* Message_New(unsigned short type,
                     unsigned short cmd,
                     unsigned short index,
                     unsigned short total,
                     const char* payload,
                     unsigned int length);

int Message_Size(Message* m);
Message* Message_N2H(Message* m);
Message* Message_H2N(Message* m);

#endif