/*
   (c) Copyright 2012-2013  DirectFB integrated media GmbH
   (c) Copyright 2001-2013  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Shimokawa <andi@directfb.org>,
              Marek Pikarski <mass@directfb.org>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   This file is subject to the terms and conditions of the MIT License:

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dfbapp.h"
#include "dfbimage.h"

#define FONT              "/usr/share/directfb-1.7.7/decker.ttf"
#define FIFO_PATH         "/tmp/fifo"
#define PIXEL_PER_PERCENT 6

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

          /* create fifo file */
          //if (access(FIFO_PATH, F_OK) != 0)
          //    mkfifo(FIFO_PATH, 0666);

          m_fd = open(FIFO_PATH, O_RDONLY);
          if(m_fd < 0) {
               // to do
               printf("open fifo file error! \n");
               //exit(-1);
          }

          return true;
    }

    /* render callback */
    virtual void Render( IDirectFBSurface &surface ) {
          /* clear screen */
          surface.Clear();

          m_image.PrepareTarget( surface );

          int x = ((int) surface.GetWidth()  - (int) m_image.GetWidth())  / 2;
          int y = ((int) surface.GetHeight() - (int) m_image.GetHeight()) / 2;
          surface.Blit( m_image, NULL, x, y );

          surface.SetFont( font );
          surface.SetColor(0x80, 0x80, 0xff, 0xff);
          surface.SetSrcBlendFunction( DSBF_INVSRCALPHA );
          surface.SetDstBlendFunction( DSBF_INVSRCALPHA );
          surface.DrawString ("Recovering", -1, 58, 630, DSTF_NONE);

          /* draw background rectangle */
          surface.SetColor(0xEE, 0xEE, 0xEE, 0xFF);
          surface.FillRectangle(58, 650, 604, 12);
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
          unsigned int cur_progress = m_msg;
          if ((0 < cur_progress) && (100 >= cur_progress)) {

               surface.SetColor(0xEE, 0x96, 0x11, 0xFF);
               surface.FillRectangle(60, 652, cur_progress*6, 8);

          } else {
               std::cerr << "Error progress: " << cur_progress << std::endl;
          }

          return false;
     }

private:
     std::string m_filename;
     DFBImage    m_image;
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

