/*
 * unique_id.hpp
 *
 *  Created on: 09.10.2014
 *      Author: Schramm
 */

#ifndef SRC_LIB_UNIQUE_ID_HPP_
#define SRC_LIB_UNIQUE_ID_HPP_

#include "crc32.hpp"

#ifdef STM32F40_41xxx
#define UNIQUE_ID0 ( * (uint32_t *) (0x1fff7A10))
#define UNIQUE_ID1 ( * (uint32_t *) (0x1fff7A14))
#define UNIQUE_ID2 ( * (uint32_t *) (0x1fff7A18))
#else
#define UNIQUE_ID0 ( * (uint32_t *) (0x1fffF7E8))
#define UNIQUE_ID1 ( * (uint32_t *) (0x1fffF7E8+4))
#define UNIQUE_ID2 ( * (uint32_t *) (0x1fffF7E8+8))
#define MCU_DEV_ID ( * (uint32_t *) (0xE0042000))
#endif


class UniqueId {
private:
	union {
		uint32_t n32[3];
		uint8_t n8[12];
	} number;

	uint32_t canId=7e0;
public:
	UniqueId() {
		setToProcessorID();
	}
	UniqueId(const uint32_t u0, const uint32_t u1, const uint32_t u2) {
		number.n32[0]=u0;
		number.n32[1]=u1;
		number.n32[2]=u2;
	}
	UniqueId(const uint8_t id[]) {
		for(uint8_t i=0; i<12; i++) {
			number.n8[i]=id[i];
		}
	}
	void setToProcessorID() {
		number.n32[0]=UNIQUE_ID0;
		number.n32[1]=UNIQUE_ID1;
		number.n32[2]=UNIQUE_ID2;
	}

	bool operator == (const UniqueId& id) const {
		for(uint8_t i=0; i<3; i++) {
			if(number.n32[i]!=id.number.n32[i]) {
				return (false);
			}
		}
		return (true);
	}

	uint8_t getByte(uint8_t offset) {
		if(offset<12) {
			return number.n8[offset];
		} else {
			return 0;
		}
	}
	uint32_t getDWord(uint8_t offset) {
		if(offset<3) {
			return number.n32[offset];
		} else {
			return 0;
		}
	}

	// use CRC32 to reduce bitsize to identify the device
	uint32_t getCRC() {
		Crc32 crc;
		for(uint8_t i=0; i<sizeof(number); i++) {
			crc.update_crc_32(number.n8[i]);
		}
		return (crc.getCRC());
	}

	uint32_t getCanId() const {
		return canId;
	}

	void setCanId(uint32_t canId = 7e0) {
		this->canId = canId;
	}
};

#endif /* SRC_LIB_UNIQUE_ID_HPP_ */
