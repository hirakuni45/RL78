//=====================================================================//
/*!	@file
	@brief	SD カードの読み書き、サンプル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "G13/port.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "common/csi_io.hpp"
#include "ff12a/mmc_io.hpp"
#include "ff12a/src/ff.h"

namespace {
	void wait_()
	{
		asm("nop");
	}

	// 送信、受信バッファの定義
	typedef utils::fifo<128> buffer;
	// UART の定義（SAU2、SAU3）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	device::itimer<uint8_t> itm_;

	// CSI(SPI) の定義、CSI00 の通信では、「SAU00」を利用、０ユニット、チャネル０
	typedef device::csi_io<device::SAU00> csi;
	csi csi_;

	// FatFS の定義
	fatfs::mmc_io<csi> mmc_(csi_);

	FATFS	fatfs_;		///< FatFs work area needed for each volume
}

const void* ivec_[] __attribute__ ((section (".ivec"))) = {
	/*  0 */  nullptr,
	/*  1 */  nullptr,
	/*  2 */  nullptr,
	/*  3 */  nullptr,
	/*  4 */  nullptr,
	/*  5 */  nullptr,
	/*  6 */  nullptr,
	/*  7 */  nullptr,
	/*  8 */  nullptr,
	/*  9 */  nullptr,
	/* 10 */  nullptr,
	/* 11 */  nullptr,
	/* 12 */  nullptr,
	/* 13 */  reinterpret_cast<void*>(uart_.send_task),
	/* 14 */  reinterpret_cast<void*>(uart_.recv_task),
	/* 15 */  reinterpret_cast<void*>(uart_.error_task),
	/* 16 */  reinterpret_cast<void*>(uart_.send_task),
	/* 17 */  reinterpret_cast<void*>(uart_.recv_task),
	/* 18 */  reinterpret_cast<void*>(uart_.error_task),
	/* 19 */  nullptr,
	/* 20 */  nullptr,
	/* 21 */  nullptr,
	/* 22 */  nullptr,
	/* 23 */  nullptr,
	/* 24 */  nullptr,
	/* 25 */  nullptr,
	/* 26 */  reinterpret_cast<void*>(itm_.task),
};


extern "C" {
	void sci_putch(char ch)
	{
		uart_.putch(ch);
	}

	void sci_puts(const char* str)
	{
		uart_.puts(str);
	}

	DSTATUS disk_initialize (BYTE drv) {
		return mmc_.disk_initialize(drv);
	}

	DSTATUS disk_status(BYTE drv) {
		return mmc_.disk_status(drv);
	}

	DRESULT disk_read(BYTE drv, BYTE* buff, DWORD sector, UINT count) {
		return mmc_.disk_read(drv, buff, sector, count);
	}

	DRESULT disk_write(BYTE drv, const BYTE* buff, DWORD sector, UINT count) {
		return mmc_.disk_write(drv, buff, sector, count);
	}

	DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void* buff) {
		return mmc_.disk_ioctl(drv, ctrl, buff);
	}

	DWORD get_fattime(void) {
		return 0;
	}
};


void list_dir()
{
	DIR dir;
	if(f_opendir(&dir, "") != FR_OK) {
		sci_puts("Can't open dir\n");

	} else {

		for(;;) {
			FILINFO fno;
			// Read a directory item
			if(f_readdir(&dir, &fno) != FR_OK) {
				sci_puts("Can't read dir\n");
				break;
			}
			if(!fno.fname[0]) break;
				if(fno.fattrib & AM_DIR) {
				utils::format("          /%s\n") % fno.fname;
			} else {
				utils::format("%8d  %s\n") % static_cast<uint32_t>(fno.fsize) % fno.fname;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	using namespace device;

	// インターバル・タイマー開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART 開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	// CSI 開始
	{
		uint8_t intr_level = 0;
		if(!csi_.start(16000000, intr_level)) {
			uart_.puts("CSI Start fail ! (Clock spped over range)\n");
		}
	}

	uart_.puts("Start RL78/G13 SD-CARD Access sample\n");

	PM4.B3 = 0;  // output

	P0.B0 = 0;
	P0.B1 = 1;
	PMC0.B0 = 0;
	PMC0.B1 = 0;
	PM0.B0 = 0;  // output
	PM0.B1 = 0;  // output;

#if 0
	f_mount(&fatfs_, "", 0);
	FIL fil;
	if(f_open(&fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
		UINT bw;
		f_write(&fil, "It works!\r\n", 11, &bw);
		f_close(&fil);
	}
#endif
	list_dir();

	uint8_t n = 0;
	uint16_t nn = 0;
	while(1) {
		itm_.sync();

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	

		++nn;
		if(nn >= 120) {
			P0.B1 = 0;
		}
	}
}
