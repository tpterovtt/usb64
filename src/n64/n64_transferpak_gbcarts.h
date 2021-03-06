// Copyright 2020, Ryan Wendland, usb64
// SPDX-License-Identifier: MIT

#ifndef _N64_TRANSFERPAK_GBCARTS_h
#define _N64_TRANSFERPAK_GBCARTS_h

#ifdef __cplusplus
extern "C" {
#endif

//TPAK
#define TPAK_STATUS_READY 0x01
#define TPAK_STATUS_WAS_RESET 0x04
#define TPAK_STATUS_IS_RESETTING 0x08
#define TPAK_STATUS_REMOVED 0x40
#define TPAK_STATUS_POWERED 0x80

//GAMEBOY
//Header offsets - ref https://web.archive.org/web/20141105020940/http://problemkaputt.de/pandocs.htm
#define GB_LOGO_OFFSET 0x0104    //48 bytes
#define GB_TITLE_OFFSET 0x0134   //16 bytes
#define GB_MANUF_OFFSET 0x013F   //4 bytes
#define GB_MBCTYPE_OFFSET 0x0147 //1 byte
#define GB_ROMSIZE_OFFSET 0x0148 //1 byte
#define GB_RAMSIZE_OFFSET 0x0149 //1 byte

//Cart Types
#define ROM_ONLY 0x00
#define ROM_RAM 0x08
#define ROM_RAM_BAT 0x09
#define MBC1 0x01
#define MBC1_RAM 0x02
#define MBC1_RAM_BAT 0x03
#define MBC2 0x05
#define MBC2_BAT 0x06
#define MBC3 0x11
#define MBC3_RAM 0x12
#define MBC3_RAM_BAT 0x13
#define MBC3_TIM_BAT 0x0F
#define MBC3_TIM_RAM_BAT 0x10
#define MBC4 0x15
#define MBC4_RAM 0x16
#define MBC4_RAM_BAT 0x17
#define MBC5 0x19
#define MBC5_RAM 0x1A
#define MBC5_RAM_BAT 0x1B
#define MBC5_RUM 0x1C
#define MBC5_RUM_RAM 0x1D
#define MBC5_RUM_RAM_BAT 0x1E

//ROM Sizes
#define KB_32 0x00
#define KB_64 0x01
#define KB_128 0x02
#define KB_256 0x03
#define KB_512 0x04
#define KB_1024 0x05
#define KB_2048 0x06
#define KB_4096 0x07
#define KB_8192 0x08
#define KB_1152 0x52
#define KB_1280 0x53
#define KB_1536 0x54

//RAM Sizes
#define NONE 0x00 //If MBC2, 00 includes a built-in RAM of 512 x 4 bits.
#define B_2048 0x01
#define B_8192 0x02
#define B_32768 0x03
#define B_131072 0x04
#define B_65536 0x05
    
//MBC
#define ROM_BANK_SIZE  0x4000
#define CRAM_BANK_SIZE 0x2000
#define CART_RAM_ADDR  0xA000

typedef struct
{
    char title[16]; //The cart game title extracted from the ROM header
    
    //Data
    uint32_t romsize;  //The cart rom size extracted from the ROM header
    uint32_t ramsize;  //The cart ram size extracted from the ROM header
    uint8_t *ram;      //Pointer to RAM data (if used)
    uint8_t *rom;      //Pointer to rom data (if used)
    
    //MBC
    uint32_t mbc;       //What mbc the cart uses, extracted from the ROM header
    uint32_t selected_rom_bank;
    uint32_t selected_ram_bank;
    uint32_t enable_cart_ram;
    uint32_t cart_mode_select;
    uint32_t num_ram_banks;
    uint32_t num_rom_banks;

    //RTC
    union
    {
        struct
        {
            uint8_t sec;  //sec  0-59 (0-3Bh)
            uint8_t min;  //min  0-59 (0-3Bh)
            uint8_t hour; //hour 0-23 (0-17h)
            uint8_t yday; //high | yday 0-365. 9 bit register.
            uint8_t high;
        } rtc_bits;
        uint8_t rtc[5];
    };

    //FATFS
    char filename[256]; //Filename of ROM in FATFS flash
} gameboycart;

typedef struct
{
    uint32_t power_state;           //Writing 0x84 to 0x8001 turns this on. Writing 0xFE to 0x8001 turns this off
    uint32_t access_state;          //Writing 0x01 to 0xB010 turns this on. Writing 0x00 to 0xB010 turns this off.
    uint32_t access_state_changed;  //This is set if the access_state was just changed.
    uint32_t selected_mbc_bank;     //Banks to access the overall MBC space. 0x0000 up to 0x7FFF.
    gameboycart *gbcart;
} n64_transferpak;

//Prototypes
void tpak_write(n64_transferpak *tp, uint16_t raw_address, uint8_t *data);
void tpak_read(n64_transferpak *tp, uint16_t raw_address, uint8_t *data);
void tpak_reset(n64_transferpak *tpak);

void gb_init_cart(gameboycart *cart, uint8_t *gb_header, char *filename);
uint8_t gb_has_battery(uint8_t mbc_type);

#ifdef __cplusplus
}
#endif

#endif
