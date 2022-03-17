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
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <progress_ipc.h>
#include <sys/un.h>
#include <errno.h>

#include "dfbapp.h"
#include "dfbimage.h"

#define FONT                    "/usr/share/directfb-1.7.7/decker.ttf"
#define SWUPDATE_PROGRESS_PATH  "/tmp/swupdateprog"
#define PIXEL_PER_PERCENT       6

/* text start position */
#define TEXT_START_X                  58
#define TEXT_START_Y                  630

/* progress bar background rectangle start position */
#define PROGRESS_BAR_BG_RECT_X        TEXT_START_X
#define PROGRESS_BAR_BG_RECT_Y        650

/* progress bar background rectangle length & width */
#define PROGRESS_BAR_BG_RECT_LEN      604
#define PROGRESS_BAR_BG_RECT_WID      12

/* progress bar front rectangle start position */
#define PROGRESS_BAR_FRONT_RECT_X     (PROGRESS_BAR_BG_RECT_X + 2)
#define PROGRESS_BAR_FRONT_RECT_Y     (PROGRESS_BAR_BG_RECT_Y + 2)

/* progress bar front rectangle width */
#define PROGRESS_BAR_FRONT_RECT_WID   (PROGRESS_BAR_BG_RECT_WID - 4)

static struct sockaddr_un address;

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

          m_sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
          if (m_sock_fd <= 0) {
               perror("socket failed: ");
               return false;
          }
          /* init & connect socket of progress notify server */
          address.sun_family = AF_UNIX;
          strcpy(address.sun_path, SWUPDATE_PROGRESS_PATH);

          if (-1 == connect(m_sock_fd, (struct sockaddr *)&address, sizeof(address)))
              perror("connect failed: ");

          return true;
    }

    /* render callback */
    virtual void Render( IDirectFBSurface &surface ) {
          /* clear screen */
          surface.Clear();

          m_image.PrepareTarget( surface );

          /* centered the image */
          int x = ((int) surface.GetWidth()  - (int) m_image.GetWidth())  / 2;
          int y = ((int) surface.GetHeight() - (int) m_image.GetHeight()) / 2;
          surface.Blit( m_image, NULL, x, y );

          surface.SetFont( font );
          surface.SetColor(0x80, 0x80, 0xff, 0xff);
          surface.SetSrcBlendFunction( DSBF_INVSRCALPHA );
          surface.SetDstBlendFunction( DSBF_INVSRCALPHA );
          surface.DrawString ("Recovering", -1, TEXT_START_X, TEXT_START_Y, DSTF_NONE);

          /* draw background rectangle */
          surface.SetColor(0xEE, 0xEE, 0xEE, 0xFF);
          surface.FillRectangle(PROGRESS_BAR_BG_RECT_X, PROGRESS_BAR_BG_RECT_Y, PROGRESS_BAR_BG_RECT_LEN, PROGRESS_BAR_BG_RECT_WID);
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
               while((ret = recv(m_sock_fd, &m_msg, sizeof(m_msg), 0)) <= 0) {
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
               surface.FillRectangle(PROGRESS_BAR_FRONT_RECT_X, PROGRESS_BAR_FRONT_RECT_Y, cur_percent * PIXEL_PER_PERCENT, PROGRESS_BAR_FRONT_RECT_WID);

          } else {
               std::cerr << "Error progress: " << cur_percent << std::endl;
          }

          return true;
     }

private:
     std::string m_filename;
     DFBImage    m_image;
     int                  m_sock_fd;
     progress_msg         m_msg;
};

int
main( int argc, char *argv[] )
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

