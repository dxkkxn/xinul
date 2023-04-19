// See LICENSE for license details.

#pragma once

typedef struct disk_device {
	void (*init)();
  void (*read_disk)();
  void (*write_disk)();
} disk_device_t;

/*
 * disk_dev global variable
 * This variable is useful to init the disk on the machine.
 */
extern disk_device_t *disk_dev;

/*
 * disk drivers
 */
extern disk_device_t virt_disk;

void register_disk(disk_device_t *dev);
