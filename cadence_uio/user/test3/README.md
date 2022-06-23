
 # run
 
 ```
 [root@centos7 test]# gcc test4.c  -o test
[root@centos7 test]# ./test 
val ffff1111 and sizeof 4 
val 1111ffff and sizeof 4 
[root@centos7 test]# 
 ```
 
# 1.在linux kernel的netinet/in.h中
```
# if __BYTE_ORDER == __BIG_ENDIAN
/* The host byte order is the same as network byte order,
   so these functions are all just identity.  */
# define ntohl(x)    (x)
# define ntohs(x)    (x)
# define htonl(x)    (x)
# define htons(x)    (x)
# else
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#   define ntohl(x)    __bswap_32 (x)
#   define ntohs(x)    __bswap_16 (x)
#   define htonl(x)    __bswap_32 (x)
#   define htons(x)    __bswap_16 (x)
#  endif
# endif
# endif
```
# 2. 在gcc中
#define __bswap_32(x) ((unsigned int)__builtin_bswap32(x))

# 3. 在gcc中
/* Swap bytes in 32 bit value.  */
#define __builtin_bswap32(x) \
     ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >>  8) |         \
      (((x) & 0x0000ff00u) <<  8) | (((x) & 0x000000ffu) << 24))