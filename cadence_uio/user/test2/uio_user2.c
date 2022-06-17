#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
//#include <asm/mmio.h>
//#include "macb.h"
#include "rte_byteorder.h"
#include "rte_io.h"
#define UIO_DEV "/dev/uio0"
#define UIO_ADDR "/sys/class/uio/uio0/maps/map0/addr"
#define UIO_SIZE "/sys/class/uio/uio0/maps/map0/size"
typedef uint32_t u32;
static char uio_addr_buf[16]={0};
static char uio_size_buf[16]={0};
/******************************************************************************************************/
/* MACB register offsets */
#define MACB_NCR		0x0000 /* Network Control */
#define MACB_NCFGR		0x0004 /* Network Config */
#define MACB_NSR		0x0008 /* Network Status */
#define MACB_TAR		0x000c /* AT91RM9200 only */
#define MACB_TCR		0x0010 /* AT91RM9200 only */
#define MACB_TSR		0x0014 /* Transmit Status */
#define MACB_RBQP		0x0018 /* RX Q Base Address */
#define MACB_TBQP		0x001c /* TX Q Base Address */
#define MACB_RSR		0x0020 /* Receive Status */
#define MACB_ISR		0x0024 /* Interrupt Status */
#define MACB_IER		0x0028 /* Interrupt Enable */
#define MACB_IDR		0x002c /* Interrupt Disable */
#define MACB_IMR		0x0030 /* Interrupt Mask */
#define MACB_MAN		0x0034 /* PHY Maintenance */
#define MACB_PTR		0x0038
#define MACB_PFR		0x003c
#define MACB_FTO		0x0040
#define MACB_SCF		0x0044
#define MACB_MCF		0x0048
#define MACB_FRO		0x004c
#define MACB_FCSE		0x0050
#define MACB_ALE		0x0054
#define MACB_DTF		0x0058
#define MACB_LCOL		0x005c
#define MACB_EXCOL		0x0060
#define MACB_TUND		0x0064
#define MACB_CSE		0x0068
#define MACB_RRE		0x006c
#define MACB_ROVR		0x0070
#define MACB_RSE		0x0074
#define MACB_ELE		0x0078
#define MACB_RJA		0x007c
#define MACB_USF		0x0080
#define MACB_STE		0x0084
#define MACB_RLE		0x0088
#define MACB_TPF		0x008c
#define MACB_HRB		0x0090
#define MACB_HRT		0x0094
#define MACB_SA1B		0x0098
#define MACB_SA1T		0x009c
#define MACB_SA2B		0x00a0
#define MACB_SA2T		0x00a4
#define MACB_SA3B		0x00a8
#define MACB_SA3T		0x00ac
#define MACB_SA4B		0x00b0
#define MACB_SA4T		0x00b4
#define MACB_TID		0x00b8
#define MACB_TPQ		0x00bc
#define MACB_USRIO		0x00c0
#define MACB_WOL		0x00c4
#define MACB_MID		0x00fc
#define MACB_TBQPH		0x04C8
#define MACB_RBQPH		0x04D4

struct macb {
	void 		*regs;
#ifdef MACB_EXT_DESC
	uint8_t hw_dma_cap;
#endif

};
struct macb_queue {
	struct macb		*bp;
	int			irq;

	unsigned int		ISR;
	unsigned int		IER;
	unsigned int		IDR;
	unsigned int		IMR;
	unsigned int		TBQP;
	unsigned int		TBQPH;
	unsigned int		RBQS;
	unsigned int		RBQP;
	unsigned int		RBQPH;
};
// arch/riscv/include/asm/mmio.h
static u_int32_t hw_readl(struct macb *bp, int offset)
{
       rte_be_to_cpu_32( rte_read32(bp->regs + offset));
       //		return readl_relaxed(bp->regs + offset);
}
/* mmio interface */
static inline void writel(u32 value, volatile void  *addr)
{
	        *(volatile u32 *)addr = value;
}

static inline u32 readl(const volatile void *addr)
{
	        return *(const volatile u32 *)addr;
}
static int macb_init(struct macb *bp, struct  macb_queue * queue)
{
			/* queue0 uses legacy registers */
			queue->ISR  = MACB_ISR;
			queue->IER  = MACB_IER;
			queue->IDR  = MACB_IDR;
			queue->IMR  = MACB_IMR;
			queue->TBQP = MACB_TBQP;
			queue->RBQP = MACB_RBQP;
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
			if (bp->hw_dma_cap & HW_DMA_CAP_64B) {
				queue->TBQPH = MACB_TBQPH;
				queue->RBQPH = MACB_RBQPH;
			}
#endif
          return 0;
}
int main(void)

{
    int uio_fd,addr_fd,size_fd;
    int uio_size;
    void *uio_addr, *access_address;
    int n=0;
    uio_fd = open(UIO_DEV,O_RDWR);
    addr_fd = open(UIO_ADDR,O_RDONLY);
    size_fd = open(UIO_SIZE,O_RDONLY);
    u_int32_t status = 0;
    if(addr_fd < 0 || size_fd < 0 || uio_fd < 0){
        fprintf(stderr,"mmap:%d\n",errno);
        exit(-1);
    }

    n=read(addr_fd,uio_addr_buf,sizeof(uio_addr_buf));
    if(n<0){
        fprintf(stderr, "%d\n", errno);
        exit(-1);
    }
    n=read(size_fd,uio_size_buf,sizeof(uio_size_buf));
    if(n<0){
        fprintf(stderr, "%d\n", errno);
        exit(-1);
    }
    uio_addr = (void*)strtoul(uio_addr_buf,NULL,0);
    uio_size = (int)strtol(uio_size_buf,NULL,0);

    access_address = mmap(NULL,uio_size,PROT_READ | PROT_WRITE,
                            MAP_SHARED,uio_fd,0);
    if(access_address == (void*)-1){
        fprintf(stderr,"mmap:%d\n",errno);
        exit(-1);
    }

    printf("The device address %p (lenth %d)\n"
        "can be accessed over\n"
        "logical address %p\n",uio_addr,uio_size,access_address);
    struct macb bp;
    struct macb_queue queue;
    bp.regs = access_address;
    macb_init(&bp, &queue);
    //status = queue_readl(queue, ISR);
    while(1)
    {
    status =  hw_readl(&bp, queue.ISR);
    if(status)
    printf("status is : 0x%08x \n", status);
    }
    return 0;
}
