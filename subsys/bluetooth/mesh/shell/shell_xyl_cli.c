/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdlib.h>
#include <bluetooth/mesh/models.h>
#include <shell/shell.h>

#include "mesh/net.h"
#include "mesh/access.h"
#include "shell_utils.h"

static struct bt_mesh_model *mod;

static void xyl_print(const struct shell *shell, int err, struct bt_mesh_light_xyl_status rsp)
{
	if (!err) {
		shell_print(shell,
			    "Light val: %d, X val: %d, "
			    "Y val %d, rem time: %d",
			    rsp.params.lightness, rsp.params.xy.x, rsp.params.xy.y,
			    rsp.remaining_time);
	}
}

static int cmd_xyl_get(const struct shell *shell, size_t argc, char *argv[])
{
	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_LIGHT_XYL_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_light_xyl_cli *cli = mod->user_data;
	struct bt_mesh_light_xyl_status rsp;

	int err = bt_mesh_light_xyl_get(cli, NULL, &rsp);

	xyl_print(shell, err, rsp);
	return err;
}

static int xyl_set(const struct shell *shell, size_t argc, char *argv[], bool acked)
{
	uint16_t light = (uint16_t)strtol(argv[1], NULL, 0);
	uint16_t x = (uint16_t)strtol(argv[2], NULL, 0);
	uint16_t y = (uint16_t)strtol(argv[3], NULL, 0);
	uint32_t time = (argc >= 5) ? (uint32_t)strtol(argv[4], NULL, 0) : 0;
	uint32_t delay = (argc == 6) ? (uint32_t)strtol(argv[5], NULL, 0) : 0;

	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_LIGHT_XYL_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_light_xyl_cli *cli = mod->user_data;
	struct bt_mesh_model_transition trans = { .time = time, .delay = delay };
	struct bt_mesh_light_xyl_set_params set = {
		.params = {
			.lightness = light,
			.xy.x = x,
			.xy.y = y,
		},
		.transition = (argc > 4) ? &trans : NULL,
	};

	if (acked) {
		struct bt_mesh_light_xyl_status rsp;
		int err = bt_mesh_light_xyl_set(cli, NULL, &set, &rsp);

		xyl_print(shell, err, rsp);
		return err;
	} else {
		return bt_mesh_light_xyl_set_unack(cli, NULL, &set);
	}
}

static int cmd_xyl_set(const struct shell *shell, size_t argc, char *argv[])
{
	return xyl_set(shell, argc, argv, true);
}

static int cmd_xyl_set_unack(const struct shell *shell, size_t argc, char *argv[])
{
	return xyl_set(shell, argc, argv, false);
}

static int cmd_xyl_target_get(const struct shell *shell, size_t argc, char *argv[])
{
	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_LIGHT_XYL_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_light_xyl_cli *cli = mod->user_data;
	struct bt_mesh_light_xyl_status rsp;

	int err = bt_mesh_light_xyl_target_get(cli, NULL, &rsp);

	xyl_print(shell, err, rsp);
	return err;
}

static void default_print(const struct shell *shell, int err, struct bt_mesh_light_xyl rsp)
{
	if (!err) {
		shell_print(shell, "Light val: %d, X val: %d, Y val %d", rsp.lightness, rsp.xy.x,
			    rsp.xy.y);
	}
}

static int cmd_xyl_default_get(const struct shell *shell, size_t argc, char *argv[])
{
	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_LIGHT_XYL_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_light_xyl_cli *cli = mod->user_data;
	struct bt_mesh_light_xyl rsp;

	int err = bt_mesh_light_xyl_default_get(cli, NULL, &rsp);

	default_print(shell, err, rsp);
	return err;
}

static int xyl_default_set(const struct shell *shell, size_t argc, char *argv[], bool acked)
{
	uint16_t light = (uint16_t)strtol(argv[1], NULL, 0);
	uint16_t x = (uint16_t)strtol(argv[2], NULL, 0);
	uint16_t y = (uint16_t)strtol(argv[3], NULL, 0);

	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_LIGHT_XYL_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_light_xyl_cli *cli = mod->user_data;
	struct bt_mesh_light_xyl set = {
		.lightness = light,
		.xy.x = x,
		.xy.y = y,
	};

	if (acked) {
		struct bt_mesh_light_xyl rsp;
		int err = bt_mesh_light_xyl_default_set(cli, NULL, &set, &rsp);

		default_print(shell, err, rsp);
		return err;
	} else {
		return bt_mesh_light_xyl_default_set_unack(cli, NULL, &set);
	}
}

static int cmd_xyl_default_set(const struct shell *shell, size_t argc, char *argv[])
{
	return xyl_default_set(shell, argc, argv, true);
}

static int cmd_xyl_default_set_unack(const struct shell *shell, size_t argc, char *argv[])
{
	return xyl_default_set(shell, argc, argv, false);
}

static void range_print(const struct shell *shell, int err,
			struct bt_mesh_light_xyl_range_status rsp)
{
	if (!err) {
		shell_print(shell,
			    "Status: %d, X min val: %d, X max val: %d "
			    "Y min val: %d, Y max val: %d",
			    rsp.status_code, rsp.range.min.x, rsp.range.max.x, rsp.range.min.y,
			    rsp.range.max.y);
	}
}

static int cmd_xyl_range_get(const struct shell *shell, size_t argc, char *argv[])
{
	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_LIGHT_XYL_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_light_xyl_cli *cli = mod->user_data;
	struct bt_mesh_light_xyl_range_status rsp;

	int err = bt_mesh_light_xyl_range_get(cli, NULL, &rsp);

	range_print(shell, err, rsp);
	return err;
}

static int xyl_range_set(const struct shell *shell, size_t argc, char *argv[], bool acked)
{
	uint16_t x_min = (uint16_t)strtol(argv[1], NULL, 0);
	uint16_t y_min = (uint16_t)strtol(argv[2], NULL, 0);
	uint16_t x_max = (uint16_t)strtol(argv[3], NULL, 0);
	uint16_t y_max = (uint16_t)strtol(argv[4], NULL, 0);

	if (!mod && !shell_model_first_get(BT_MESH_MODEL_ID_LIGHT_XYL_CLI, &mod)) {
		return -ENODEV;
	}

	struct bt_mesh_light_xyl_cli *cli = mod->user_data;
	struct bt_mesh_light_xy_range set = {
		.min.x = x_min,
		.min.y = y_min,
		.max.x = x_max,
		.max.y = y_max,
	};

	if (acked) {
		struct bt_mesh_light_xyl_range_status rsp;
		int err = bt_mesh_light_xyl_range_set(cli, NULL, &set, &rsp);

		range_print(shell, err, rsp);
		return err;
	} else {
		return bt_mesh_light_xyl_range_set_unack(cli, NULL, &set);
	}
}

static int cmd_xyl_range_set(const struct shell *shell, size_t argc, char *argv[])
{
	return xyl_range_set(shell, argc, argv, true);
}

static int cmd_xyl_range_set_unack(const struct shell *shell, size_t argc, char *argv[])
{
	return xyl_range_set(shell, argc, argv, false);
}

static int cmd_instance_get_all(const struct shell *shell, size_t argc, char *argv[])
{
	return shell_model_instances_get_all(shell, BT_MESH_MODEL_ID_LIGHT_XYL_CLI);
}

static int cmd_instance_set(const struct shell *shell, size_t argc, char *argv[])
{
	uint8_t elem_idx = (uint8_t)strtol(argv[1], NULL, 0);

	return shell_model_instance_set(shell, &mod, BT_MESH_MODEL_ID_LIGHT_XYL_CLI, elem_idx);
}

SHELL_STATIC_SUBCMD_SET_CREATE(instance_cmds,
			       SHELL_CMD_ARG(set, NULL, "<elem_idx> ", cmd_instance_set, 2, 0),
			       SHELL_CMD_ARG(get-all, NULL, NULL, cmd_instance_get_all, 1, 0),
			       SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(
	xyl_cmds, SHELL_CMD_ARG(get, NULL, NULL, cmd_xyl_get, 1, 0),
	SHELL_CMD_ARG(set, NULL, "<light> <x> <y> [transition_time_ms [delay_ms]]", cmd_xyl_set,
		      4, 2),
	SHELL_CMD_ARG(set-unack, NULL, "<light> <x> <y> [transition_time_ms [delay_ms]]",
		      cmd_xyl_set_unack, 4, 2),
	SHELL_CMD_ARG(target-get, NULL, NULL, cmd_xyl_target_get, 1, 0),
	SHELL_CMD_ARG(default-get, NULL, NULL, cmd_xyl_default_get, 1, 0),
	SHELL_CMD_ARG(default-set, NULL, "<light> <x> <y>", cmd_xyl_default_set, 4, 0),
	SHELL_CMD_ARG(default-set-unack, NULL, "<light> <x> <y>", cmd_xyl_default_set_unack,
		      4, 0),
	SHELL_CMD_ARG(range-get, NULL, NULL, cmd_xyl_range_get, 1, 0),
	SHELL_CMD_ARG(range-set, NULL, "<x_min> <y_min> <x_max> <y_max>", cmd_xyl_range_set, 5,
		      0),
	SHELL_CMD_ARG(range-set-unack, NULL, "<x_min> <y_min> <x_max> <y_max>",
		      cmd_xyl_range_set_unack, 5, 0),
	SHELL_CMD(instance, &instance_cmds, "Instance commands", shell_model_cmds_help),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_ARG_REGISTER(mdl_xyl, &xyl_cmds, "XYL Cli commands", shell_model_cmds_help, 1, 1);
