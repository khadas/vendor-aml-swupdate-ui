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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <progress_ipc.h>
#include <sys/un.h>
#include <errno.h>
#include "dfbapp.h"
#include "dfbimage.h"
#include "aml_directfb_ui.h"
#include "../common/swupdate_ipc.h"

#define FONT  "/usr/share/directfb-1.7.7/decker.ttf"

/* fixed the front rectangle length by pixel_per_percent */
int pixel_per_percent = 4;

/* text start position variate */
int text_start_x = 0;
int text_start_y = 0;

/* progress bar background rectangle start position variate */
int progress_bar_bg_rect_x = 0;
int progress_bar_bg_rect_y = 0;

/* progress bar background rectangle length & width variate */
int progress_bar_bg_rect_len = 0;

/* progress bar front rectangle start position variate */
int progress_bar_front_rect_x = 0;
int progress_bar_front_rect_y = 0;

/* progress bar background rectangle width */
#define PROGRESS_BAR_BG_RECT_WID      12

/* progress bar front rectangle width */
#define PROGRESS_BAR_FRONT_RECT_WID   (PROGRESS_BAR_BG_RECT_WID - 4)

static struct sockaddr_un address;
int screen_width = 0;
int screen_height = 0;

class DFBSwupdateUI : public DFBApp {
public:
    DFBSwupdateUI() {
    }

    virtual ~DFBSwupdateUI() {
    }

private:
    IDirectFBFont font;

     /* called after initialization */
     virtual bool Setup( int width, int height ) {

          /* draw string */
          DFBFontDescription desc;
          desc.flags  = DFDESC_HEIGHT;
          desc.height = 20;

          font = m_dfb.CreateFont( FONT, desc );

          /* load image */
          m_image.LoadImage( m_filename );

          m_sock_fd = sock_conn();
          return true;
    }

    /* render callback */
    virtual void Render( IDirectFBSurface &surface ) {
          int image_width = 0;
          int image_height = 0;
          /* clear screen */
          surface.Clear();

          m_image.PrepareTarget( surface );

          screen_width = (int) surface.GetWidth();
          screen_height = (int) surface.GetHeight();
          image_width = (int) m_image.GetWidth();
          image_height = (int) m_image.GetHeight();

          /* centered the image */
          //int x = (screen_width - image_width)  / 2;
          //int y = (screen_height - image_height) / 2;
          /* The image is displayed in the upper left corner */
          int x = 0, y = 0;
          surface.Blit( m_image, NULL, x, y );

          /*
           |                                                                   |
           |Recovering                                                         |
           |***********************background*******************************   |
           |*   2                                                       2  *   |
           |* 2|----------------------front-----------------------------|2 *   |
           |*  |--------------------------------------------------------|  *   |
           |*   2                                                       2  *   |
           |****************************************************************   |
           |                                                                   |
           |________________________baseline___________________________________|
           The front rectangle's length should be a multiple of 100 because the
           progress percent is 100, so we can express percentages as integer
           pixels. The background rectangle's start position and length can
           calculated, because the background rectangle is 4 pixels longer than the front rectangle.
           */
          /* adjust the start position of the text, the height is 50 pixels from the baseline */
          text_start_x = 0;
          text_start_y = screen_height - 50;
          surface.SetFont( font );
          surface.SetColor(0x80, 0x80, 0xff, 0xff);
          surface.SetSrcBlendFunction( DSBF_INVSRCALPHA );
          surface.SetDstBlendFunction( DSBF_INVSRCALPHA );
          surface.DrawString ("Recovering", -1, text_start_x, text_start_y, DSTF_NONE);

          /* adjust the position of the progress bar here, the height is offseted 20 pixels down */
          progress_bar_bg_rect_x = text_start_x;
          progress_bar_bg_rect_y = text_start_y + 20;

          /* The background rectangle is 4 pixels longer than the front rectangle, (pixel_per_percent * 100) is the length
             of front rectangle.
           */
          progress_bar_bg_rect_len = (pixel_per_percent * 100) + 4;
          progress_bar_front_rect_x = progress_bar_bg_rect_x + 2;
          progress_bar_front_rect_y = progress_bar_bg_rect_y + 2;
          /* draw background rectangle */
          surface.SetColor(0xEE, 0xEE, 0xEE, 0xFF);
          surface.FillRectangle(progress_bar_bg_rect_x, progress_bar_bg_rect_y, progress_bar_bg_rect_len, PROGRESS_BAR_BG_RECT_WID);
    }

    bool ParseArgs( int argc, char *argv[] ) {
          /* Parse the command line. */
          if (argc != 2 || !argv[1] || !argv[1][0]) {
               std::cerr << std::endl;
               std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
               std::cerr << std::endl;
               return false;
          }

          /* Create the main interface. */
          m_filename = argv[1];

          return true;
    }

    virtual bool HandleEvent( IDirectFBSurface &surface ) {
          static int ret = 0;
          if (m_sock_fd > 0) {
               /* block receiving progress data */
               while (( ret = recv(m_sock_fd, &m_msg, sizeof(m_msg), 0)) <= 0) {
                    /* if recv() returns 0, it means SWupdate is disconnected, retry to connect */
                    if (ret == 0) {
                         if (-1 == connect(m_sock_fd, (struct sockaddr *)&address, sizeof(address))) {
                              perror("connect failed: ");
                              return false;
                         }
                    }
               }
          }

          int cur_percent = m_msg.cur_percent;
          if (cur_percent == 0)
               return true;

          if ((0 < cur_percent) && (100 >= cur_percent)) {
               surface.SetColor(0xEE, 0x96, 0x11, 0xFF);
               surface.FillRectangle(progress_bar_front_rect_x, progress_bar_front_rect_y, cur_percent * pixel_per_percent, PROGRESS_BAR_FRONT_RECT_WID);

          } else {
               std::cerr << "Error progress: " << cur_percent << std::endl;
          }

          return true;
     }

private:
     std::string  m_filename;
     DFBImage     m_image;
     int          m_sock_fd;
     progress_msg m_msg;
};

int swupdateui_run( int argc, char *argv[] )
{
     DFBSwupdateUI app;

     try {
          /* Initialize DirectFB command line parsing. */
          DirectFB::Init( &argc, &argv );

          /* Parse remaining arguments and run. */
          if (app.Init( argc, argv ))
               app.Run();
     }
     catch (DFBException *ex) {
          /*
           * Exception has been caught, destructor of 'app' will deinitialize
           * anything at return time (below) that got initialized until now.
           */
          std::cerr << std::endl;
          std::cerr << "Caught exception!" << std::endl;
          std::cerr << "  -- " << ex << std::endl;
     }

     return 0;
}

