/*
   american fuzzy lop++ - fuzzer header
   ------------------------------------

   Originally written by Michal Zalewski

   Now maintained by Marc Heuse <mh@mh-sec.de>,
                     Heiko Eißfeldt <heiko.eissfeldt@hexco.de>,
                     Andrea Fioraldi <andreafioraldi@gmail.com>,
                     Dominik Maier <mail@dmnk.co>

   Copyright 2016, 2017 Google Inc. All rights reserved.
   Copyright 2019-2020 AFLplusplus Project. All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

   This is the Library based on AFL++ which can be used to build
   customized fuzzers for a specific target while taking advantage of
   a lot of features that AFL++ already provides.

 */

#include "observationchannel.h"
#include "afl-returns.h"

afl_ret_t afl_observation_channel_init(observation_channel_t *channel) {

  (void)channel;

  return AFL_RET_SUCCESS;

}

void afl_observation_channel_deinit(observation_channel_t *channel) {

  (void)channel;

}

void flush_default(observation_channel_t *channel) {

  (void)channel;

  /* TODO: Implementation */
  return;

}

void reset_default(observation_channel_t *channel) {

  (void)channel;

  /* TODO: Implementation */
  return;

}

void post_exec(observation_channel_t *channel) {

  (void)channel;

  /* TODO: Implementation */
  return;

}

afl_ret_t afl_map_channel_init(map_based_channel_t *map_channel,
                               size_t               map_size) {

  afl_observation_channel_init(&(map_channel->base));

  if (!afl_sharedmem_init(&map_channel->shared_map, map_size)) {

    return AFL_RET_ERROR_INITIALIZE;

  }

  map_channel->extra_funcs.get_map_size = get_map_size_default;
  map_channel->extra_funcs.get_trace_bits = get_trace_bits_default;

  return AFL_RET_SUCCESS;

}

void afl_map_channel_deinit(map_based_channel_t *map_channel) {

  afl_sharedmem_deinit(&map_channel->shared_map);

  afl_observation_channel_deinit(&map_channel->base);

}

u8 *get_trace_bits_default(map_based_channel_t *obs_channel) {

  return obs_channel->shared_map.map;

}

size_t get_map_size_default(map_based_channel_t *obs_channel) {

  return obs_channel->shared_map.map_size;

}
