/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdio.h>

#include "sensor_module_event.h"
#include "common_module_event.h"

static char *get_evt_type_str(enum sensor_module_event_type type)
{
	switch (type) {
	case SENSOR_EVT_MOVEMENT_DATA_READY:
		return "SENSOR_EVT_MOVEMENT_DATA_READY";
	case SENSOR_EVT_ENVIRONMENTAL_DATA_READY:
		return "SENSOR_EVT_ENVIRONMENTAL_DATA_READY";
	case SENSOR_EVT_ENVIRONMENTAL_NOT_SUPPORTED:
		return "SENSOR_EVT_ENVIRONMENTAL_NOT_SUPPORTED";
	case SENSOR_EVT_SHUTDOWN_READY:
		return "SENSOR_EVT_SHUTDOWN_READY";
	case SENSOR_EVT_ERROR:
		return "SENSOR_EVT_ERROR";
	default:
		return "Unknown event";
	}
}

static void log_event(const struct event_header *eh)
{
	const struct sensor_module_event *event = cast_sensor_module_event(eh);

	if (event->type == SENSOR_EVT_ERROR) {
		EVENT_MANAGER_LOG(eh, "%s - Error code %d",
				get_evt_type_str(event->type), event->data.err);
	} else if (event->type == SENSOR_EVT_MOVEMENT_DATA_READY) {
		EVENT_MANAGER_LOG(eh, "%s - X: %.2f, Y: %.2f, Z: %.2f",
				get_evt_type_str(event->type),
				event->data.accel.values[0],
				event->data.accel.values[1],
				event->data.accel.values[2]);

	} else {
		EVENT_MANAGER_LOG(eh, "%s", get_evt_type_str(event->type));
	}
}

#if defined(CONFIG_PROFILER)

static void profile_event(struct log_event_buf *buf,
			 const struct event_header *eh)
{
	const struct sensor_module_event *event = cast_sensor_module_event(eh);

#if defined(CONFIG_PROFILER_EVENT_TYPE_STRING)
	profiler_log_encode_string(buf, get_evt_type_str(event->type));
#else
	profiler_log_encode_uint8(buf, event->type);
#endif
}

COMMON_EVENT_INFO_DEFINE(sensor_module_event,
			 profile_event);

#endif /* CONFIG_PROFILER */

COMMON_EVENT_TYPE_DEFINE(sensor_module_event,
			 CONFIG_SENSOR_EVENTS_LOG,
			 log_event,
			 &sensor_module_event_info);
