#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

add_subdirectory_ifdef(CONFIG_BUILD_WITH_TFM tfm/zephyr)
add_subdirectory_ifdef(CONFIG_MEMFAULT memfault-firmware-sdk)
