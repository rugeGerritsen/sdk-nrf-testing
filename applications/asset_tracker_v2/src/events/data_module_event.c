/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdio.h>

#include "data_module_event.h"
#include "common_module_event.h"

static char *get_evt_type_str(enum data_module_event_type type)
{
	switch (type) {
	case DATA_EVT_DATA_SEND:
		return "DATA_EVT_DATA_SEND";
	case DATA_EVT_DATA_READY:
		return "DATA_EVT_DATA_READY";
	case DATA_EVT_DATA_SEND_BATCH:
		return "DATA_EVT_DATA_SEND_BATCH";
	case DATA_EVT_UI_DATA_READY:
		return "DATA_EVT_UI_DATA_READY";
	case DATA_EVT_UI_DATA_SEND:
		return "DATA_EVT_UI_DATA_SEND";
	case DATA_EVT_NEIGHBOR_CELLS_DATA_SEND:
		return "DATA_EVT_NEIGHBOR_CELLS_DATA_SEND";
	case DATA_EVT_AGPS_REQUEST_DATA_SEND:
		return "DATA_EVT_AGPS_REQUEST_DATA_SEND";
	case DATA_EVT_CONFIG_INIT:
		return "DATA_EVT_CONFIG_INIT";
	case DATA_EVT_CONFIG_READY:
		return "DATA_EVT_CONFIG_READY";
	case DATA_EVT_CONFIG_GET:
		return "DATA_EVT_CONFIG_GET";
	case DATA_EVT_CONFIG_SEND:
		return "DATA_EVT_CONFIG_SEND";
	case DATA_EVT_SHUTDOWN_READY:
		return "DATA_EVT_SHUTDOWN_READY";
	case DATA_EVT_DATE_TIME_OBTAINED:
		return "DATA_EVT_DATE_TIME_OBTAINED";
	case DATA_EVT_ERROR:
		return "DATA_EVT_ERROR";
	default:
		return "Unknown event";
	}
}

static void log_event(const struct event_header *eh)
{
	const struct data_module_event *event = cast_data_module_event(eh);

	if (event->type == DATA_EVT_ERROR) {
		EVENT_MANAGER_LOG(eh, "%s - Error code %d",
				get_evt_type_str(event->type), event->data.err);
	} else {
		EVENT_MANAGER_LOG(eh, "%s", get_evt_type_str(event->type));
	}
}

#if defined(CONFIG_PROFILER)

static void profile_event(struct log_event_buf *buf,
			 const struct event_header *eh)
{
	const struct data_module_event *event = cast_data_module_event(eh);

#if defined(CONFIG_PROFILER_EVENT_TYPE_STRING)
	profiler_log_encode_string(buf, get_evt_type_str(event->type));
#else
	profiler_log_encode_uint8(buf, event->type);
#endif
}

COMMON_EVENT_INFO_DEFINE(data_module_event,
			 profile_event);

#endif /* CONFIG_PROFILER */

COMMON_EVENT_TYPE_DEFINE(data_module_event,
			 CONFIG_DATA_EVENTS_LOG,
			 log_event,
			 &data_module_event_info);
