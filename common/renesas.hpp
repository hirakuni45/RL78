#pragma once
//=====================================================================//
/*!	@file
	@brief	ルネサス RL78 選択
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/vect.h"
#include "common/delay.hpp"

#if defined(SIG_G13)
#include "G13/port.hpp"
#include "G13/system.hpp"
#include "G13/adc.hpp"
#include "G13/dma.hpp"
#include "G13/iica.hpp"
#include "G13/intr.hpp"
#include "G13/sau.hpp"
#include "G13/tau.hpp"
#include "G13/timer.hpp"

#else
#  error "Requires SIG_XXX to be defined"
#endif
