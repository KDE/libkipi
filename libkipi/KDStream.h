/* -*- Mode: C++ -*-
   KD Tools - a set of useful widgets for Qt
   $Id$
*/

/****************************************************************************
** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Tools library.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid commercial KD Tools licenses may use this file in
** accordance with the KD Tools Commercial License Agreement provided with
** the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.klaralvdalens-datakonsult.se/?page=products for
**   information about KD Tools Commercial License Agreements.
**
** Contact info@klaralvdalens-datakonsult.se if any conditions of this
** licensing are not clear to you.
**
**********************************************************************/

#ifndef KIPI_KDSTREAM
#define KIPI_KDSTREAM

// Forward declarations.
class QColor;
class QColorGroup;
class QPalette;
class QCursor;
class QDate;
class QDateTime;
class QTime;
class QFont;
class QPen;
class QPoint;
class QSize;
class QRect;
class QObject;
class QVariant;
class QBrush;
class QSizePolicy;
class QKeySequence;

#include <qstring.h>
#include <qvaluelist.h>
#include <qstrlist.h>
#include <qasciidict.h>
#include <qintdict.h>
#include <qptrdict.h>
#include <qdict.h>
#include <qvaluestack.h>
#include <qasciicache.h>
#include <qintcache.h>
#include <qcache.h>

#if ( QT_VERSION < 300 )
#include <qlist.h>
#include <qstack.h>
#include <qqueue.h>
#include <qvector.h>
#endif

#if ( QT_VERSION >= 300 )
#include <qptrlist.h>
#include <qptrstack.h>
#include <qptrqueue.h>
#include <qpair.h>
#include <qptrvector.h>
#include <qvaluevector.h>
#endif

// utility functions.
class KDStream;
typedef KDStream & (*KDSTREAMFUNC)(KDStream &);
KDStream& endl( KDStream& stream);
KDStream& flush( KDStream& stream);

class KDStream
{
public:
  KDStream( QString* outputString = 0);
  ~KDStream();
  void flush();

  // Standard C++ types
  KDStream& operator<<( bool );
  KDStream& operator<<( char );
  KDStream& operator<<( float );
  KDStream& operator<<( double );
  KDStream& operator<<( short );
  KDStream& operator<<( unsigned short );
  KDStream& operator<<( int );
  KDStream& operator<<( unsigned int );
  KDStream& operator<<( long );
  KDStream& operator<<( unsigned long );
  KDStream& operator<<( const char* );
  KDStream& operator<<( const void* );

  // Data holding classes.
  KDStream& operator<<( const QString& );
  KDStream& operator<<( const QCString& );
  KDStream& operator<<( const QChar& );

  KDStream& operator<<( const QColor& );
  KDStream& operator<<( const QColorGroup& );
  KDStream& operator<<( const QPalette& );
  KDStream& operator<<( const QCursor& );

  KDStream& operator<<( const QDate& );
  KDStream& operator<<( const QDateTime& );
  KDStream& operator<<( const QTime& );

  KDStream& operator<<( const QFont& );
  KDStream& operator<<( const QPen& );
  KDStream& operator<<( const QPoint& );
  KDStream& operator<<( const QSize& );
  KDStream& operator<<( const QRect& );
  KDStream& operator<<( const QBrush& );
  KDStream& operator<<( const QSizePolicy& );
  KDStream& operator<<( const QKeySequence& );

  // misc
  KDStream& operator<<( KDSTREAMFUNC );
  KDStream& operator<<( const QVariant& );
  KDStream& operator<<( const QObject& );
  KDStream& operator<<( const QStrList& list );

protected:
  QString QColor2Str( const QColor& col );

private:
  QString _output;
  QString* _out;
};


// Helper functions for KDStream.
// Defined as global functions to support
// compilers without support for member templates
// You should not need to call those yourself
template <class Iterator> void KDStream_valueListStream( KDStream& st, Iterator begin, Iterator end )
{
  st << "[";
  bool first = true;
  for ( Iterator it = begin; it != end; ++it ){
    if ( first )
      first = false;
    else
      st << ", ";
    st << *it;
  }
  st << "]";
}

template<class Iterator> void KDStream_ptrListStream( KDStream& st, Iterator it, bool doubleDeref )
{
  st << "[";
  bool first = true;
  for ( ; *it; ++ it) {
    if ( first )
      first = false;
    else
      st << ", ";

    if ( doubleDeref )
      st << *(*it);
    else {
      // QStrList ought to be a value list rather than a ptr list, one less dereference is
      // necesary here, otherwise we will only stream out a char, rather than a char *
      st << *it;
    }
  }
  st << "]";
}

template<class Iterator> void KDStream_ptrDictStream( KDStream& st, Iterator it )
{
  st << "{";
  bool first = true;
  for ( ; it; ++ it) {
    if ( first )
      first = false;
    else
      st << ", ";

    st << (it.currentKey()) << ": " << *(it.current()) ;
  }
  st << "}";
}

// Stream operators for containers
// Defined as global functions to support
// compilers without member templates

template<class T> KDStream& operator<<( KDStream& st, const QValueList<T>& list )
{
  KDStream_valueListStream( st, list.begin(), list.end() );
  return st;
}

#if ( QT_VERSION < 300 )
template<class T> KDStream& operator<<( KDStream& st, const QList<T>& list )
{
  KDStream_ptrListStream ( st, QListIterator<T>( list ) , true );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QArray<T>& array )
{
  KDStream_valueListStream( st, array.begin(), array.end() );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QVector<T>& vector )
{
  QList<T> list;
  vector.toList( &list );

  KDStream_ptrListStream ( st, QListIterator<T>( list ), true );
  return st;
}
#endif
#if ( QT_VERSION >= 300 )
template<class T> KDStream& operator<<( KDStream& st, const QMemArray<T>& array )
{
  KDStream_valueListStream( st, array.begin(), array.end() );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QPtrList<T>& list )
{
  KDStream_ptrListStream ( st, QPtrListIterator<T>( list ), true );
  return st;
}

template<class T1, class T2> KDStream& operator<<( KDStream& st, const QPair<T1,T2>& pair )
{
  st << "(" << pair.first << "," << pair.second << ")";
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QPtrVector<T>& vector )
{
  QPtrList<T> list;
  vector.toList( &list );

  KDStream_ptrListStream( st, QPtrListIterator<T>( list ), true );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QValueVector<T>& vector )
{
  KDStream_valueListStream( st, vector.begin(), vector.end() );
  return st;
}
#endif

#if ( QT_VERSION < 300 )
template<class T> KDStream& operator<<( KDStream& st, const QStack<T>& stack )
{
  // I need a copy to look at the individual elements.
  QStack<T> copy(stack);
#else
template<class T> KDStream& operator<<( KDStream& st, const QPtrStack<T>& stack )
{

  // I need a copy to look at the individual elements.
  QPtrStack<T> copy(stack);
  /*}*/
#endif
  st << "[";
  if ( stack.count() > 1 )
    st << "top| ";
  st << " ";

  bool first = true;
  while ( !copy.isEmpty() ) {
    if (first)
      first = false;
    else
      st << ", ";
    st << *(copy.pop());
  }

  st << " ";
  if (  stack.count() > 1 )
    st << " |bottom";
  st << "]";
  return st;
}

// This function can unfortunately not be merged with the function for
// Q(Ptr)Stack, as the Q(Ptr)Stack dereferences what it pops of the stack,
// before streaming it:
// Q(Ptr)Stack: *this << *(copy.pop());
// QValueStack: *this <<   copy.pop() ;
template<class T> KDStream& operator<<( KDStream& st, const QValueStack<T>& stack )
{
  // I need a copy to look at the individual elements.
  QValueStack<T> copy(stack);
  st << "[";
  if ( stack.count() > 1 )
    st << "top| ";
  st << " ";

  bool first = true;
  while ( !copy.isEmpty() ) {
    if (first)
      first = false;
    else
      st << ", ";
    st << copy.pop();
  }

  st << " ";
  if (  stack.count() > 1 )
    st << " |bottom";
  st << "]";
  return st;
}


template<class T> KDStream& operator<<( KDStream& st, const QAsciiDict<T>& dict )
{
  KDStream_ptrDictStream( st, QAsciiDictIterator<T>( dict ) );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QIntDict<T>& dict )
{
  KDStream_ptrDictStream( st, QIntDictIterator<T>( dict ) );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QPtrDict<T>& dict )
{
  KDStream_ptrDictStream( st, QPtrDictIterator<T>( dict ) );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QDict<T>& dict )
{
  KDStream_ptrDictStream( st, QDictIterator<T>( dict ) );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QAsciiCache<T>& cache )
{
  KDStream_ptrDictStream( st, QAsciiCacheIterator<T>( cache ) );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QIntCache<T>& cache )
{
  KDStream_ptrDictStream( st, QIntCacheIterator<T>( cache ) );
  return st;
}

template<class T> KDStream& operator<<( KDStream& st, const QCache<T>& cache )
{
  KDStream_ptrDictStream( st, QCacheIterator<T>( cache ) );
  return st;
}

#endif /* KDStream */
