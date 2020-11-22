/*
 * Copyright (c) 2018 Dynamic Robotics Laboratory
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "cassiemujoco.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    const char modelfile[] = "../model/cassie_hfield.xml";
    cassie_sim_t *c = cassie_sim_init(modelfile, false);
    cassie_vis_t *v = cassie_vis_init(c, modelfile);

    state_out_t y;
    pd_in_t u = {0};
    
    int count = 0;
    double apply_force[6] = {0, 0, 100, 0, 0, 0};
    float* hfield_data = cassie_sim_hfielddata(c);
    int nhfielddata = cassie_sim_get_nhfielddata(c);
    srand(time(NULL));
    for (int i = 0; i < nhfielddata; i++) {
        // printf("%g\n", (float)rand()/(float)RAND_MAX);
        hfield_data[i] = (float)rand()/(float)RAND_MAX;
    }
    
    bool draw_state = cassie_vis_draw(v, c);
    while (draw_state) {
        if (!cassie_vis_paused(v)) {   
            // if (count > 500) {
            //     printf("applying perturb\n");
            //     cassie_vis_apply_force(v, apply_force, "cassie-pelvis");                
            // }
            // printf("count: %i\n", count); 
            cassie_sim_step_pd(c, &y, &u);
            count += 1;
        }
        draw_state = cassie_vis_draw(v, c);
        
    }

    cassie_sim_free(c);
    cassie_vis_free(v);
    // do {
    //     if (!cassie_vis_paused(v)) {    
    //         cassie_sim_step_pd(c, &y, &u);
    //     }
    // } while (cassie_vis_draw(v, c));

    return 0;
}
