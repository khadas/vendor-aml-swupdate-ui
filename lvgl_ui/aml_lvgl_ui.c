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

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <lvgl/lvgl.h>
#include "../common/aml_ui_run.h"
#include "../common/event_ui.h"
#include "../lvgl_porting/lv_port_disp.h"
#include "../lvgl_porting/lv_port_fs.h"

typedef struct {
    lv_obj_t *label;
    lv_obj_t *img;
    lv_obj_t *bar;
    lv_style_t style_bg;
}lv_screen_obj;

/* Denotes the UI finished variable */
sem_t sem_ui_finish;

/* Reference to LV_DISP_DEF_REFR_PERIOD in lv_conf.h
 * Default display refresh period. LVG will redraw changed areas with this period time
*/
#define LV_DISP_REFR_PERIOD 30

/* Animations finish time, the bar will refresh is within LV_ANIM_FINISH_TIME [ms] every time */
#define LV_ANIM_FINISH_TIME 30

/* Bar's position */
#define LV_POS_BAR_X    0
#define LV_POS_BAR_Y    0

/* Label's position*/
#define LV_POS_LABEL_X  0
#define LV_POS_LABEL_Y  -30

/* Image's position*/
#define LV_POS_IMAGE_X  0
#define LV_POS_IMAGE_Y  0

/* The size of the image can be scaled */
#define LV_IMAGE_SIZE_X 300
#define LV_IMAGE_SIZE_Y 300

/* Bar's width & height */
#define LV_BAR_WIDTH    400
#define LV_BAR_HEIGHT   20

/*The label text*/
static char text[] = "Recoverying";

/**
 * @brief refresh the percent of the bar
 *
 * @param e  event object
 */
static void obj_bar_refr_anim(lv_event_t *e)
{
    int32_t percent = *(int32_t *)lv_event_get_param(e);
    lv_obj_t *bar = lv_event_get_target(e);
    lv_bar_set_value(bar, percent, LV_ANIM_ON);
}

/**
 * @brief
 *
 */
static void obj_bar_is_covered(lv_event_t *e)
{
    sem_post(&sem_ui_finish);
}

/**
 * @brief init bar's property
 *
 * @param bar bar object
 * @param style_bg bar's background style
 */
void lv_bar_init(lv_obj_t *bar, lv_style_t *style_bg)
{
    lv_style_init(style_bg);
    /*Set the padding to 6*/
    lv_style_set_pad_all(style_bg, 6);

    lv_obj_set_size(bar, LV_BAR_WIDTH, LV_BAR_HEIGHT);
    /* set animations finish time */
    lv_obj_set_style_anim_time(bar, LV_ANIM_FINISH_TIME, 0);
    lv_obj_set_style_border_color(bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 2, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 6, LV_PART_MAIN);

    lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar, 3, LV_PART_INDICATOR);
    lv_obj_add_style(bar, style_bg, LV_PART_MAIN);

    lv_bar_set_value(bar, 0, LV_ANIM_ON);
    /* Set bar's position, relative to LV_ALIGN_BOTTOM_LEFT */
    lv_obj_align(bar, LV_ALIGN_BOTTOM_LEFT, LV_POS_BAR_X, LV_POS_BAR_Y);
    lv_obj_add_event_cb(bar, obj_bar_refr_anim, LV_EVENT_REFRESH, NULL);
    lv_obj_add_event_cb(bar, obj_bar_is_covered, LV_EVENT_COVER_CHECK, NULL);
}

/**
 * @brief label init
 *
 * @param label label object
 */
void lv_label_init(lv_obj_t *label)
{
    lv_label_set_text(label, text);
    /* Set label's position, relative to LV_ALIGN_BOTTOM_LEFT */
    lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, LV_POS_LABEL_X, LV_POS_LABEL_Y);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_BLUE), 0);
}

/**
 * @brief image init
 *
 * @param img image object
 * @param path path of the image
 */
void lv_img_init(lv_obj_t *img, char *path)
{
    lv_img_set_src(img, path);
    lv_obj_align(img, LV_ALIGN_TOP_LEFT, LV_POS_IMAGE_X, LV_POS_IMAGE_Y);
    lv_obj_set_size(img, LV_IMAGE_SIZE_X, LV_IMAGE_SIZE_Y);
}

/**
 * @brief refresh bar callback function
 *
 * @param bar bar object
 * @param data percentage value
 */
void lv_bar_refresh(void *screen, int32_t data)
{
    lv_screen_obj *scr = (lv_screen_obj *)screen;
    lv_event_send(scr->bar, LV_EVENT_REFRESH, &data);
}

/**
 * @brief lvgl GUI start running
 *
 * @param argc
 * @param argv
 * @return int
 */
int swupdateui_run(int argc, char *argv[])
{
    /*Create a event instance*/
    lv_screen_obj scr_obj;
    lv_refresh_event_t ref_ent;

    /*Check the param*/
    if (argc != 2) {
        printf("Usage: %s <filename> \n", argv[0]);
        return -1;
    }

    /*Set the image path*/
    int size = strlen(argv[1]);
    char *file_path = (char *)malloc(size + 3);
    memset(file_path, 0, size + 3);
    strncpy(file_path, "P:", 2);
    strncat(file_path, argv[1], size);

    /*LittlevGL init*/
    lv_init();

    /*display init*/
    lv_port_disp_init();

    /* filesystem init */
    lv_port_fs_init();

    scr_obj.bar = lv_bar_create(lv_scr_act());
    scr_obj.label = lv_label_create(lv_scr_act());
    scr_obj.img = lv_img_create(lv_scr_act());
    ref_ent.fd = progress_ipc_connect(true);
    ref_ent.p_bar_refresh = lv_bar_refresh;

    /*Init the lable*/
    lv_label_init(scr_obj.label);

    /* Init bar config */
    lv_bar_init(scr_obj.bar, &scr_obj.style_bg);

    /*Init the image*/
    lv_img_init(scr_obj.img, file_path);

    (lv_screen_obj *)(ref_ent.screen);
    ref_ent.screen = &scr_obj;

    sem_init(&sem_ui_finish, 0, 0);
    while (1) {
        progress_handle((void *)(&ref_ent));

        usleep(LV_DISP_DEF_REFR_PERIOD*1000);
        lv_tick_inc(LV_DISP_DEF_REFR_PERIOD);
        /* Sleep at least one refresh period, reference to LV_DISP_DEF_REFR_PERIOD[ms] defined in lv_conf.h */
        lv_timer_handler();

        /* If the percentage equal to 100, swupdate ui process will be exited. */
        if (is_swupdateui_finished()) {
            /* Refresh the screen and waiting the animation finish before exit. */
            sem_wait(&sem_ui_finish);
            break;
        }
    }

    free(file_path);
    sem_destroy(&sem_ui_finish);

    return 0;
}

#ifdef __cplusplus
}
#endif

