#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if (CONFIG_NRF_802154_RADIO_DRIVER OR CONFIG_NRF_802154_SERIALIZATION)
  if (CONFIG_NRF_802154_SL)
    target_sources(nrf-802154-platform
      PRIVATE
        ${ZEPHYR_CURRENT_MODULE_DIR}/nrf_802154/sl/platform/hp_timer/nrf_802154_hp_timer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sl/platform/nrf_802154_platform_sl_lptimer_zephyr.c
    )
  endif ()

  if (CONFIG_MPSL)
    target_compile_definitions(nrf-802154-platform PUBLIC NRF_802154_VERIFY_PERIPHS_ALLOC_AGAINST_MPSL=1)
  endif()

  set(NRF53_SERIES  ${CONFIG_SOC_SERIES_NRF53X})
  set(SER_HOST      ${CONFIG_NRF_802154_SER_HOST})
  set(SL_OPENSOURCE ${CONFIG_NRF_802154_SL_OPENSOURCE})

  if (CONFIG_NRF_802154_SOURCE_NRFXLIB)
    add_subdirectory(${ZEPHYR_CURRENT_MODULE_DIR}/nrf_802154 nrf_802154)

    target_link_libraries(nrf-802154-driver-interface INTERFACE zephyr-802154-interface)
    target_link_libraries(nrf-802154-serialization-interface INTERFACE zephyr-802154-interface)
  endif (CONFIG_NRF_802154_SOURCE_NRFXLIB)

  target_compile_definitions(zephyr-802154-interface INTERFACE NRF_802154_ECB_PRIORITY=-1)
  target_compile_definitions(zephyr-802154-interface INTERFACE NRF_802154_SWI_PRIORITY=1)
endif (CONFIG_NRF_802154_RADIO_DRIVER OR CONFIG_NRF_802154_SERIALIZATION)
