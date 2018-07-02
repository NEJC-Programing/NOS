/*
 * Copyright (c) 2008 Mathias Gottschlag, Alexander Siol
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/projects/COPYING.WTFPL for more details.
 */

#ifndef VIDEO_H
#define VIDEO_H

#include <cdi.h>
#include <types.h>

extern struct cdi_video_driver* videodriver;

//Interface über RPC
void rpc_video_interface(pid_t pid,
                          uint32_t correlation_id,
                          size_t data_size,
                          void* data);

#endif

