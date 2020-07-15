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

#ifndef FEEDBACK_FILE_INCLUDED
#define FEEDBACK_FILE_INCLUDED

#include "libqueue.h"

typedef struct feedback {

  feedback_queue_t *queue;

  struct feedback_metadata *metadata; /* We can have a void pointer for the
                                         struct here. What do you guys say? */

  struct feedback_functions *functions;

} feedback_t;

struct feedback_functions {

  float (*is_interesting)(feedback_t *, executor_t *);
  void (*set_feedback_queue)(feedback_t *, feedback_queue_t *);
  feedback_queue_t *(*get_feedback_queue)(feedback_t *);

};

typedef struct feedback_metadata {

  // This struct is more dependent on user's implementation.
  feedback_t *feedback;

} feedback_metadata_t;

// Default implementation of the vtables functions

/*TODO: Can we have a similiar implementation for the is_interesting function?*/
void              _set_feedback_queue_(feedback_t *, feedback_queue_t *);
feedback_queue_t *_get_feedback_queue_(feedback_t *);

// "Constructors" and "destructors" for the feedback
void        afl_feedback_deinit(feedback_t *);
void afl_feedback_init(feedback_t *);


static inline feedback_t * AFL_FEEDBACK_INIT(feedback_t * feedback) {

  feedback_t * new_feedback = NULL;
  
  if (feedback) afl_feedback_init(feedback);

  else {
    new_feedback = ck_alloc(sizeof(feedback_t));
    afl_feedback_init(new_feedback);
  }

  return new_feedback;

}


#define AFL_FEEDBACK_DEINIT(fbck) afl_feedback_deinit(fbck);

/* TODO: Add MaximizeMapFeedback implementation */

#endif

