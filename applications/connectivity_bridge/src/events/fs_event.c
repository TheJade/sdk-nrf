/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdio.h>
#include <assert.h>

#include <fs_event.h>
#include <fs/fs.h>

static int log_fs_event(
	const struct event_header *eh,
	char *buf,
	size_t buf_len)
{
	const struct fs_event *event = cast_fs_event(eh);

	EVENT_MANAGER_LOG(eh, "req:%d", event->req);
	return 0;
}

int fs_event_helper_file_write(
	const char *mnt_point,
	const char *file_path,
	char const *file_contents,
	size_t contents_len)
{
	struct fs_file_t file;
	ssize_t bytes_written;
	char fname[128];
	int err;

	err = snprintf(fname, sizeof(fname), "%s/%s", mnt_point, file_path);
	if (err <= 0 || err > sizeof(fname)) {
		return -ENOMEM;
	}

	fs_file_t_init(&file);
	err = fs_open(&file, fname, FS_O_CREATE | FS_O_RDWR);
	if (err) {
		return err;
	}

	err = fs_seek(&file, 0, FS_SEEK_END);
	if (err) {
		return err;
	}

	bytes_written = fs_write(&file, file_contents, contents_len);
	if (bytes_written != contents_len) {
		return -ENOMEM;
	}

	err = fs_close(&file);
	if (err) {
		return err;
	}

	return 0;
}

EVENT_TYPE_DEFINE(fs_event,
		  IS_ENABLED(CONFIG_BRIDGE_LOG_FS_EVENT),
		  log_fs_event,
		  NULL);
