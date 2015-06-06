/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#ifndef HBX_EXPORT_
#define HBX_EXPORT_ 1

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( HBX_LIBRARY_STATIC )
    #    define HBX_EXPORT
    #  elif defined( HBX_LIBRARY )
    #    define HBX_EXPORT   __declspec(dllexport)
    #  else
    #    define HBX_EXPORT   __declspec(dllimport)
    #  endif
#else
    #  define HBX_EXPORT
#endif

#endif

