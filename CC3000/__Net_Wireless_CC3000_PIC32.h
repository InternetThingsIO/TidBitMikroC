
#ifndef __Net_Wireless_CC3000_H__
#define __Net_Wireless_CC3000_H__

#include "stdint.h"

#define  _NET_WIRELESS_CC3000_SUCCESS                        0
#define  _NET_WIRELESS_CC3000_ERROR                         -1

////////////////////////////////////////////////////////////////////////////////
// Wlan

#define  _NET_WIRELESS_CC3000_SEC_UNSEC                      0
#define  _NET_WIRELESS_CC3000_SEC_WEP                        1
#define  _NET_WIRELESS_CC3000_SEC_WPA                        2
#define  _NET_WIRELESS_CC3000_SEC_WPA2                       3

#define  _NET_WIRELESS_CC3000_DISABLE                        0
#define  _NET_WIRELESS_CC3000_ENABLE                         1


#define  _NET_WIRELESS_CC3000_SCAN_STATUS_VALID              1
#define  _NET_WIRELESS_CC3000_SCAN_STATUS_INVALID            0

#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_OPEN              2
#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_WEP               3
#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_WPA               4
#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_WPA2              5

#define _NET_WIRELESS_CC3000_IPCONFIG_IP_OFFSET              0
#define _NET_WIRELESS_CC3000_IPCONFIG_SUBNET_OFFSET          4
#define _NET_WIRELESS_CC3000_IPCONFIG_GW_OFFSET              8
#define _NET_WIRELESS_CC3000_IPCONFIG_DHCP_OFFSET            12
#define _NET_WIRELESS_CC3000_IPCONFIG_DNS_OFFSET             16
#define _NET_WIRELESS_CC3000_IPCONFIG_MAC_OFFSET             20
#define _NET_WIRELESS_CC3000_IPCONFIG_SSID_OFFSET            26

#define _NET_WIRELESS_CC3000_EVNT_UNSOL_CONNECT              0x8001
#define _NET_WIRELESS_CC3000_EVNT_UNSOL_DISCONNECT           0x8002
#define _NET_WIRELESS_CC3000_EVNT_UNSOL_INIT                 0x8004
#define _NET_WIRELESS_CC3000_EVNT_TX_COMPLETE                0x8008
#define _NET_WIRELESS_CC3000_EVNT_UNSOL_DHCP                 0x8010
#define _NET_WIRELESS_CC3000_EVNT_ASYNC_PING                 0x8040
#define _NET_WIRELESS_CC3000_EVNT_ASYNC_SIMPLE_CFG_DONE      0x8080
#define _NET_WIRELESS_CC3000_EVNT_KEEPALIVE                  0x8200
#define _NET_WIRELESS_CC3000_EVNT_BSD_TCP_CLOSE_WAIT         0x8800
#define _NET_WIRELESS_CC3000_EVENT_CAN_SHUT_DOWN             0x0099

#define _NET_WIRELESS_CC3000_STATUS_DISCONNECTED             0
#define _NET_WIRELESS_CC3000_STATUS_CONNECTED                3

#define _NET_WIRELESS_PATCHES_REQUEST_DEFAULT                0
#define _NET_WIRELESS_PATCHES_REQUEST_FORCE_HOST             1
#define _NET_WIRELESS_PATCHES_REQUEST_FORCE_NONE             2

typedef struct
{
  uint8_t aucIP[4];
  uint8_t aucSubnetMask[4];
  uint8_t aucDefaultGateway[4];
  uint8_t aucDHCPServer[4];
  uint8_t aucDNSServer[4];
  uint8_t uaMacAddr[6];
  uint8_t uaSSID[32];
}tNetappIpconfigRetArgs;

typedef struct
{
  uint32_t packets_sent;
  uint32_t packets_received;
  uint32_t min_round_time;
  uint32_t max_round_time;
  uint32_t avg_round_time;
} tNetappPingreportRetArgs;

typedef struct {
  uint8_t   status;
  uint16_t  netIndex;
  int8_t    rssi;
  uint8_t   securityMode;
  uint8_t   ssidNameLength;
  uint8_t   ssidName[32];
  uint8_t   bssid[6];
} tScanResult;

typedef void (*tWlanCB)(int32_t eventType, uint8_t * dat, uint8_t length );
extern int32_t Net_Wireless_CC3000_WaitHandler();

////////////////////////////////////////////////////////////////////////////////
// Socket

#define  _NET_WIRELESS_CC3000_SOCK_DOMAIN_AF_INET            2
#define  _NET_WIRELESS_CC3000_SOCK_DOMAIN_AF_INET6           23

#define  _NET_WIRELESS_CC3000_SOCK_TYPE_STREAM               1
#define  _NET_WIRELESS_CC3000_SOCK_TYPE_DGRAM                2
#define  _NET_WIRELESS_CC3000_SOCK_TYPE_RAW                  3

#define _NET_WIRELESS_CC3000_SOCK_PROTO_TCP                  6
#define _NET_WIRELESS_CC3000_SOCK_PROTO_UDP                  17
#define _NET_WIRELESS_CC3000_SOCK_PROTO_RAW                  255

#define  _NET_WIRELESS_CC3000_SOCK_LEVEL                     0xffff
#define  _NET_WIRELESS_CC3000_SOCK_OPT_RCV_NONBLOCK          0
#define  _NET_WIRELESS_CC3000_SOCK_OPT_RCV_TIMEOUT           1
#define  _NET_WIRELESS_CC3000_SOCK_OPT_ACCEPT_NONBLOCK       2
#define  _NET_WIRELESS_CC3000_SOCK_OPT_NONBLOCK_ON           0
#define  _NET_WIRELESS_CC3000_SOCK_OPT_NONBLOCK_OFF          1

#define __FD_SETSIZE                                         32
#define __NFDBITS               (8 * sizeof (__fd_mask))
#define __FDELT(d)              ((d) / __NFDBITS)
#define __FDMASK(d)             ((__fd_mask) 1 << ((d) % __NFDBITS))
#define FDS_BITS(set)           ((set)->fds_bits)

////////////////////////////////////////////////////////////////////////////////
// Initializes the file descriptor set fdset to have zero bits for all file descriptors
#define FD_ZERO(set)                                                  \
  do {                                                                \
    uint32_t __i;                                                     \
    tFd_set *__arr = (set);                                           \
    for (__i = 0; __i < sizeof (tFd_set) / sizeof (__fd_mask); ++__i) \
      FDS_BITS (__arr)[__i] = 0;                                      \
  } while (0)
////////////////////////////////////////////////////////////////////////////////
// Sets the bit for the file descriptor fd in the file descriptor set fdset
#define FD_SET(d, set)          (FDS_BITS (set)[__FDELT (d)] |= __FDMASK (d))
////////////////////////////////////////////////////////////////////////////////
// Clears the bit for the file descriptor fd in the file descriptor set fdset
#define FD_CLR(d, set)          (FDS_BITS (set)[__FDELT (d)] &= ~__FDMASK (d))
////////////////////////////////////////////////////////////////////////////////
// Returns a non-zero value if the bit for the file descriptor fd is set in the 
// file descriptor set pointed to by fdset, and 0 otherwise.
#define FD_ISSET(d, set)        (FDS_BITS (set)[__FDELT (d)] & __FDMASK (d))

typedef struct
{
    uint16_t   sa_family;
    uint8_t    sa_data[14];
} tSockAddr;

typedef struct
{
    uint32_t   s_addr;
} tIn_addr;

typedef struct
{
    int16_t    sin_family;
    uint16_t   sin_port;
    tIn_addr   sin_addr;
    uint8_t    sin_zero[8];
} tSockaddr_in;


typedef int32_t __fd_mask;
typedef struct
{
    __fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
} tFd_set;

typedef int32_t tTime;
typedef int32_t tSuseconds;
typedef struct
{
  tTime         tv_sec;                  /* seconds */
  tSuseconds    tv_usec;                 /* microseconds */
} tTimeval;

////////////////////////////////////////////////////////////////////////////////
// Nvmem

//Definitions for File IDs
#define _NET_WIRELESS_CC3000_NVMEM_NVS_FILEID                0
#define _NET_WIRELESS_CC3000_NVMEM_NVS_SHADOW_FILEID         1
#define _NET_WIRELESS_CC3000_NVMEM_WLAN_CONFIG_FILEID        2
#define _NET_WIRELESS_CC3000_NVMEM_WLAN_CONFIG_SHADOW_FILEID 3
#define _NET_WIRELESS_CC3000_NVMEM_WLAN_DRIVER_SP_FILEID     4
#define _NET_WIRELESS_CC3000_NVMEM_WLAN_FW_SP_FILEID         5
#define _NET_WIRELESS_CC3000_NVMEM_MAC_FILEID                6
#define _NET_WIRELESS_CC3000_NVMEM_FRONTEND_VARS_FILEID      7
#define _NET_WIRELESS_CC3000_NVMEM_IP_CONFIG_FILEID          8
#define _NET_WIRELESS_CC3000_NVMEM_IP_CONFIG_SHADOW_FILEID   9
#define _NET_WIRELESS_CC3000_NVMEM_BOOTLOADER_SP_FILEID      10
#define _NET_WIRELESS_CC3000_NVMEM_RM_FILEID                 11
#define _NET_WIRELESS_CC3000_NVMEM_AES128_KEY_FILEID         12
#define _NET_WIRELESS_CC3000_NVMEM_SHARED_MEM_FILEID         13

#define _NET_WIRELESS_CC3000_AES128_KEY_SIZE                 16

#endif // __Net_Wireless_CC3000_H__