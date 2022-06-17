
# big or small

![image](https://github.com/magnate3/riscv-dpdk/blob/main/pic/big_or_small.png)

```
 if (native_io) {
                pr_err("native_io");
                bp->macb_reg_readl = hw_readl_native;
                bp->macb_reg_writel = hw_writel_native;
        } else {
                pr_err("no native_io");
                bp->macb_reg_readl = hw_readl;
                bp->macb_reg_writel = hw_writel;
        }
```

# dpdk build

![image](https://github.com/magnate3/riscv-dpdk/blob/main/pic/build.png)

## install
![image](https://github.com/magnate3/riscv-dpdk/blob/main/pic/build_install.png)


# queue_readl(queue, ISR)

## code
![image](https://github.com/magnate3/riscv-dpdk/blob/main/pic/kernel_read_code.png)

## kernel output
![image](https://github.com/magnate3/riscv-dpdk/blob/main/pic/kernel_read.png)


## user output
![image](https://github.com/magnate3/riscv-dpdk/blob/main/pic/user_read.png)



# programming



## write or read hw reg

```
#define IXGBE_READ_REG(hw, reg) \
        ixgbe_read_addr(IXGBE_PCI_REG_ADDR((hw), (reg)))

#define IXGBE_WRITE_REG(hw, reg, value) \
        IXGBE_PCI_REG_WRITE(IXGBE_PCI_REG_ADDR((hw), (reg)), (value))

```



```
#define IXGBE_PCI_REG(reg) rte_read32(reg)
#define IXGBE_PCI_REG_ADDR(hw, reg) \
        ((volatile uint32_t *)((char *)(hw)->hw_addr + (reg)))
static inline uint32_t ixgbe_read_addr(volatile void* addr)
{
        return rte_le_to_cpu_32(IXGBE_PCI_REG(addr));
}

```

## typedef
```
./drivers/net/ixgbe/base/ixgbe_osdep.h:typedef uint32_t u32
```

## arch 

### rte_read32
```
./lib/eal/arm/include/rte_io_64.h:rte_read32_relaxed(const volatile void *addr)
./lib/eal/arm/include/rte_io_64.h:rte_read32(const volatile void *addr)
./lib/eal/arm/include/rte_io_64.h:      val = rte_read32_relaxed(addr);
./lib/eal/include/generic/rte_io.h:rte_read32_relaxed(const volatile void *addr);
./lib/eal/include/generic/rte_io.h:rte_read32(const volatile void *addr);
./lib/eal/include/generic/rte_io.h:rte_read32_relaxed(const volatile void *addr)
./lib/eal/include/generic/rte_io.h:rte_read32(const volatile void *addr)
./lib/eal/include/generic/rte_io.h:     val = rte_read32_relaxed(addr);
```