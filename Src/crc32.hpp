/*
 * crc32.hpp
 *
 *  Created on: 25.10.2020
 *      Author: daniel
 *      Ideas: https://create.stephan-brumme.com/crc32/#sarwate
 *
 */


#ifndef CRC32_HPP
#define	CRC32_HPP

class Crc32 {
    uint32_t internalCRC;
    const uint32_t table[16] = {
        0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC, 0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
        0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C, 0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
    };

public:
    Crc32() {
        reset();
    }

    void reset() {
        internalCRC = 0x00000000;
    }

    void update_crc_32(uint8_t data) {
        uint32_t crc = ~internalCRC;
        crc = table[(crc ^ data) & 0x0F] ^ (crc >> 4);
        crc = table[(crc ^ (data >> 4)) & 0x0F] ^ (crc >> 4);
        internalCRC = ~crc;
    }

    uint32_t getCRC() {
        return (internalCRC);
    }
};

#endif	/* CRC32_HPP */

