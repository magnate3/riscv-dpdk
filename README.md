# write or read hw reg

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

# typedef
```
./drivers/net/ixgbe/base/ixgbe_osdep.h:typedef uint32_t u32
```