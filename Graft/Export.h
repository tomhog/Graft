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

#ifndef GRAFT_EXPORT_
#define GRAFT_EXPORT_ 1

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( GRAFT_LIBRARY_STATIC )
    #    define GRAFT_EXPORT
    #  elif defined( GRAFT_LIBRARY )
    #    define GRAFT_EXPORT   __declspec(dllexport)
    #  else
    #    define GRAFT_EXPORT   __declspec(dllimport)
    #  endif
#else
    #  define GRAFT_EXPORT
#endif

#endif

