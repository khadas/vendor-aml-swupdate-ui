/*
 * Copyright (C) 2017 Amlogic Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSEERROR_CODE_INVALID_OPERATION.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  DESCRIPTION
 *      This file implements a adaptor of audio decoder from Amlogic.
 *
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "event_ui.h"
#include <string.h>

static int cur_img_percent = 0;
const char *status_success = "SWUpdate successful!";
const char *status_fail    = "SWUpdate failure!";
const char *status_run     = "Installing image ";
char info[PRINFOSIZE] = {0};

static void status_check(lv_refresh_event_t *pdata)
{
    if (pdata->msg.status != pdata->ui_status) {
        pdata->ui_status = pdata->msg.status;
    }

    if (NULL == pdata->p_show_status)
        return ;

    if (SOURCE_DOWNLOADER == pdata->msg.source) {
        if (strncmp(info, pdata->msg.info, (PRINFOSIZE - 1))) {
            memset(info, 0, PRINFOSIZE);
            strncpy(info, pdata->msg.info, (PRINFOSIZE - 1));
            pdata->p_show_status(pdata->screen, pdata->msg.info);
        }
    }

    switch (pdata->ui_status) {
        case IDLE:
        case START:
        case RUN:
            if ((0 == cur_img_percent) && (NULL != pdata->msg.cur_image) && (0 != strlen(pdata->msg.cur_image))) {
                char buf[256] = {0};
                strncpy(buf, status_run, sizeof(buf) - 1);
                strncat(buf, pdata->msg.cur_image, (sizeof(buf) - sizeof(status_run) - 1));
                pdata->p_show_status(pdata->screen, buf);
            }
            break;
        case SUCCESS:
            pdata->p_show_status(pdata->screen, status_success);
            break;
        case FAILURE:
            pdata->p_show_status(pdata->screen, status_fail);
            break;
        case DOWNLOAD:
        case DONE:
        case SUBPROCESS:
            break;
        default:
            break;
    }
}

/* Calculate the percentage of total progress */
static int calc_perc_of_total(struct progress_msg *msg)
{
    static int cur_step_base;
    int cur_perc_convert = 0;
    int total_percent = 0;
    static last_cur_step;
    static last_total_percent;

    /* Calculate how much of each step is a percentage of the total,
     * Each step is a stage, and the percentage of completed steps plus
     * the percentage of current steps equals the percentage of total progress.
     *
     * cur_step_base is calculated by the current step represents a percentage
     * of the total step.
     * cur_perc_convert convert the percentage of the current image to
     * the total percentage
     *  */
    cur_step_base = 100 * (msg->cur_step - 1) / msg->nsteps;
    cur_perc_convert = msg->cur_percent / msg->nsteps;
    total_percent = (cur_step_base + cur_perc_convert);

    if ((msg->cur_step == last_cur_step) && (total_percent < last_total_percent)) {
        //printf("!!!Warning !!!The same step,current total percent less then last total percent !\n");
        //printf("[cur_step:%d][last_cur_step:%d][total_percent:%d][last_total_percent:%d]\n", msg->cur_step, last_cur_step, total_percent, last_total_percent);
        return last_total_percent;
    }
    last_cur_step = msg->cur_step;
    last_total_percent = total_percent;

    return total_percent;
}

void progress_handle(void *data)
{
    int ret = -1;
    int percent = 0;

    lv_refresh_event_t *pdata = (lv_refresh_event_t *)data;
    ret = progress_ipc_receive(&pdata->fd, &pdata->msg);
    if (-1 == ret) {
        printf("Get the progress value failed!\n");
        pdata->fd = progress_ipc_connect(true);
        return ;
    }

    if ((pdata->msg.cur_percent < 0) || (pdata->msg.cur_percent > 100)) {
        printf("Get the error progress, current percent is %d \n", pdata->msg.cur_percent);
        return ;
    }

    status_check(pdata);

    percent = calc_perc_of_total(&pdata->msg);
    //printf("[hnd_name:%s][msg_percent:%d][total_percent:%d]", pdata->msg.hnd_name, pdata->msg.cur_percent, percent);
    //printf("[cur_step:%d][nsteps:%d]\n", pdata->msg.cur_step, pdata->msg.nsteps);

    if (cur_img_percent == percent) {
        return;
    }

    cur_img_percent = percent;

    if ((0 != cur_img_percent) && (NULL != pdata->p_bar_refresh)) {
        pdata->p_bar_refresh(pdata->screen, cur_img_percent);
    }

}

#ifdef __cplusplus
}
#endif
