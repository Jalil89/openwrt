#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/crypto.h>
#include <linux/init.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <net/ip.h>
#include <net/udp.h>
#include <net/route.h>

#undef __KERNEL__
#include <linux/netfilter_ipv4.h>
#define __KERNEL__

#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8
#define TOT_HDR_LEN 28

static unsigned int pkt_mangle_begin(unsigned int hooknum,
                        struct sk_buff *skb,
                        const struct net_device *in,
                        const struct net_device *out,
                        int (*okfn)(struct sk_buff *));

static struct nf_hook_ops pkt_mangle_ops __read_mostly = {
    .pf = NFPROTO_IPV4,
    .priority = 1,
    .hooknum = NF_IP_LOCAL_OUT,
    .hook = pkt_mangle_begin,
};

static int __init pkt_mangle_init(void)
{
    printk(KERN_ALERT "\npkt_mangle module started ...");
    return nf_register_hook(&pkt_mangle_ops);
}

static void __exit pkt_mangle_exit(void)
{
    nf_unregister_hook(&pkt_mangle_ops);
    printk(KERN_ALERT "pkt_mangle module stopped ...");
}

static unsigned int pkt_mangle_begin (unsigned int hooknum,
                        struct sk_buff *skb,
                        const struct net_device *in,
                        const struct net_device *out,
                        int (*okfn)(struct sk_buff *))
{
    struct iphdr *iph;
    struct udphdr *udph;
    unsigned char *data;

    unsigned int data_len;
    unsigned char extra_data[] = "12345";
    unsigned char *temp;
    unsigned int extra_data_len;
    unsigned int tot_data_len;

    unsigned int i;

    __u16 dst_port, src_port;

    if (skb) {
        iph = (struct iphdr *) skb_header_pointer (skb, 0, 0, NULL);

        if (iph && iph->protocol &&(iph->protocol == IPPROTO_UDP)) {
            udph = (struct udphdr *) skb_header_pointer (skb, IP_HDR_LEN, 0, NULL);
            src_port = ntohs (udph->source);
            dst_port = ntohs (udph->dest);

            if (src_port == 6000) {
                printk(KERN_ALERT "UDP packet goes out");
                data = (unsigned char *) skb_header_pointer (skb, IP_HDR_LEN+UDP_HDR_LEN, 0, NULL);
                data_len = skb->len - TOT_HDR_LEN;

                temp = kmalloc(512 * sizeof(char), GFP_ATOMIC);
                memcpy(temp, data, data_len);

                unsigned char *ptr = temp + data_len - 1;
                extra_data_len = sizeof(extra_data);
                memcpy(ptr, extra_data, extra_data_len);
                tot_data_len = data_len + extra_data_len - 1;

                skb_put(skb, extra_data_len - 1);

                memcpy(data, temp, tot_data_len);

                /* Manipulating necessary header fields */
                iph->tot_len = htons(tot_data_len + TOT_HDR_LEN);
                udph->len = htons(tot_data_len + UDP_HDR_LEN);

                /* Calculation of IP header checksum */
                iph->check = 0;
                ip_send_check (iph);

                /* Calculation of UDP checksum */
                udph->check = 0;
                int offset = skb_transport_offset(skb);
                int len = skb->len - offset;
                udph->check = ~csum_tcpudp_magic((iph->saddr), (iph->daddr), len, IPPROTO_UDP, 0);

                 }
        }
    }
    return NF_ACCEPT;
}


module_init(pkt_mangle_init);
module_exit(pkt_mangle_exit);

MODULE_AUTHOR("Rifat Rahman Ovi: <rifatrahmanovi@gmail.com>");
MODULE_DESCRIPTION("Outward Packet Mangling and Decryption in Kernel Space");
MODULE_LICENSE("GPL");
