#include <stdint.h>

void dma_transfer(volatile uint8_t *dma_flags, uint64_t read_addr, 
                  uint64_t write_addr, uint32_t copy_len) {
    volatile uint64_t *dma_rd_addr = (volatile uint64_t *)(dma_flags + 1);
    volatile uint64_t *dma_wr_addr = (volatile uint64_t *)(dma_flags + 9);
    volatile uint32_t *dma_copy_len = (volatile uint32_t *)(dma_flags + 17);

    // Configure DMA registers
    *dma_rd_addr = read_addr;
    *dma_wr_addr = write_addr;
    *dma_copy_len = copy_len;
    *dma_flags = 0x01; // DEV_INIT

    // Poll DMA for finish
    while ((*dma_flags & 0x04) != 0x04); // DEV_INTR
}

void start_stream_dma_transfer(volatile uint8_t *str_dma_flags, uint64_t rd_addr, uint64_t wr_addr, 
                               uint32_t rd_frame_size, uint8_t num_rd_frames, uint8_t rd_frame_buff_size, 
                               uint32_t wr_frame_size, uint8_t num_wr_frames, uint8_t wr_frame_buff_size) {
    volatile uint64_t *str_dma_rd_addr = (volatile uint64_t *)(str_dma_flags + 4);
    volatile uint64_t *str_dma_wr_addr = (volatile uint64_t *)(str_dma_flags + 12);
    volatile uint32_t *str_dma_rd_frame_size = (volatile uint32_t *)(str_dma_flags + 20);
    volatile uint8_t *str_dma_num_rd_frames = (volatile uint8_t *)(str_dma_flags + 24);
    volatile uint8_t *str_dma_rd_frame_buff_size = (volatile uint8_t *)(str_dma_flags + 25);
    volatile uint32_t *str_dma_wr_frame_size = (volatile uint32_t *)(str_dma_flags + 26);
    volatile uint8_t *str_dma_num_wr_frames = (volatile uint8_t *)(str_dma_flags + 30);
    volatile uint8_t *str_dma_wr_frame_buff_size = (volatile uint8_t *)(str_dma_flags + 31);

    // Configure Stream DMA registers
    *str_dma_rd_addr = rd_addr;
    *str_dma_wr_addr = wr_addr;
    *str_dma_rd_frame_size = rd_frame_size;
    *str_dma_num_rd_frames = num_rd_frames;
    *str_dma_rd_frame_buff_size = rd_frame_buff_size;
    *str_dma_wr_frame_size = wr_frame_size;
    *str_dma_num_wr_frames = num_wr_frames;
    *str_dma_wr_frame_buff_size = wr_frame_buff_size;
    *str_dma_flags = 0x03; // STR_DMA_INIT_RD | STR_DMA_INIT_WR
}

void poll_stream_dma_transfer(volatile uint8_t *str_dma_flags) {
    // Poll Stream DMA for finish
    while ((*str_dma_flags & 0x08) == 0x08); // STR_DMA_WR_RUNNING
}
