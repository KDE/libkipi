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

#if defined KDAB_EVAL
#include "../evaldialog/evaldialog.h"
#endif

#include "KDStream.h"
#include <qcolor.h>
#include <qpalette.h>
#include <qcursor.h>
#include <qdatetime.h>
#include <qfont.h>
#include <qpen.h>
#include <qpoint.h>
#include <qsize.h>
#include <qrect.h>
#include <qregion.h>
#include <qobject.h>
#include <qstringlist.h>
#include <qmetaobject.h>
#include <qvariant.h>
#include <qpointarray.h>
#include <qsizepolicy.h>
#include <qbitarray.h>
#include <qstrlist.h>

#if (QT_VERSION >= 300 )
#include <qkeysequence.h>
#endif

/**
   \class KDStream KDStream.h
   \brief Streaming operators for Qt classes.

   When debugging Qt programs the streaming operators in this class offers
   facilities for printing out values of a number of Qt classes.

   Example:
   \code
   QPoint point(10,20);
   QString string("A test");
   QFont font = qApp->font();
   KDStream() << "the point is " << point << ", the string is " << string << ", the font is " << font << endl;
   \endcode
*/


/*!
  Creates a KDStream object.
*/
KDStream::KDStream( QString* outputString ) :_out(outputString)
{
#if defined KDAB_EVAL
    EvalDialog::checkEvalLicense( "KD Tools" );
#endif

}


/*!
  Flushes the data to the stream and destroys the KDStream object.
*/
KDStream::~KDStream()
{
  flush();
}


/*!
  Flushes buffered data to the stream.
*/
void KDStream::flush()
{
  if ( _output.isEmpty() )
    return;

  if ( _out )
      *_out += _output;
  else
      qDebug( "%s", _output.local8Bit().data() );
  _output = QString();
}


/*!
  Writes a boolean value to the stream. The value will be represented
  as either "true" or "false".
*/
KDStream& KDStream::operator<<( bool b )
{
  _output += ( b ? QString::fromLatin1("true") : QString::fromLatin1("false") );
  return *this;
}

/*!
  Writes a character value to the stream.
*/
KDStream& KDStream::operator<<( char ch )
{
  _output += QString::fromLatin1("%1").arg( ch );
  return *this;
}


/*!
  Writes a floating point value to the stream.
*/
KDStream& KDStream::operator<<( float num )
{
  _output += QString::number( num );
  return *this;
}


/*!
  Writes a double-precision floating point value to the stream.
*/
KDStream& KDStream::operator<<( double num )
{
  _output += QString::number( num );
  return *this;
}


/*!
  Writes a short value to the stream.
*/
KDStream& KDStream::operator<<( short num )
{
  _output += QString::number( num );
  return *this;
}

/*!
  Writes an unsigned short value to the stream.
*/
KDStream& KDStream::operator<<( unsigned short num )
{
  _output += QString::number( num );
  return *this;
}

/*!
  Writes an int value to the stream.
*/
KDStream& KDStream::operator<<( int num )
{
  _output += QString::number( num );
  return *this;
}

/*!
  Writes an unsigned int value to the stream.
*/
KDStream& KDStream::operator<<( unsigned int num )
{
  _output += QString::number( num );
  return *this;
}

/*!
  Writes a long value to the stream.
*/
KDStream& KDStream::operator<<( long num )
{
  _output += QString::number( num );
  return *this;
}

/*!
  Writes an unsigned long value to the stream.
*/
KDStream& KDStream::operator<<( unsigned long num )
{
  _output += QString::number( num );
  return *this;
}

/*!
  Writes a C-style string to the stream.
*/
KDStream& KDStream::operator<<( const char* ch )
{
  *this << QString::fromLocal8Bit( ch );
  return *this;
}

/*!
  Writes a pointer to the stream. The format is platform-dependent and
  defined by fprintf( ..., "%p" ).
*/
KDStream& KDStream::operator<<( const void* p)
{
  _output += QString().sprintf("%p",p);
  return *this;
}


/*!
  Writes a QString value to the stream.
*/
KDStream& KDStream::operator<<( const QString& str )
{
  int index = str.find( '\n' );
  if ( index == -1 )
    _output += str;
  else {
    _output += str.left( index );
    flush();
    _output += str.mid( index+1 );
  }
  return *this;
}

/*!
  Writes a QCString value to the stream.
*/
KDStream& KDStream::operator<<( const QCString& str )
{
  *this << QString( str );
  return *this;
}


/*!
  Runs a stream-processing function on the stream.
*/
KDStream& KDStream::operator<<( KDSTREAMFUNC func )
{
  return (*func)(*this);
}

/*!
  Writes a line terminator to the stream.
*/
KDStream& endl( KDStream& stream)
{
  stream << QString::fromLatin1("\n");
  stream.flush();
  return stream;
}

/*!
  Flushes the output buffers.
*/
KDStream& flush( KDStream& stream)
{
  stream.flush();
  return stream;
}

/*!
  Writes a QChar value to the stream.
*/
KDStream& KDStream::operator<<( const QChar& ch )
{
  _output += QString( ch );
  return *this;
}


/*!
  Writes a QColor value to the stream. See \QColor2Str for a
  description of the output format.
*/
KDStream& KDStream::operator<<( const QColor& col )
{
  _output += QColor2Str( col );
  return *this;
}

/*!
  Writes a QColorGroup value to the stream. Each color role output
  with its name and the corresponding color value.
*/
KDStream& KDStream::operator<<( const QColorGroup& colgrp )
{
  _output +=
    QString::fromLatin1("foreground: ") + QColor2Str(colgrp.foreground()) + QString::fromLatin1(", ") +
    QString::fromLatin1("button: ") + QColor2Str(colgrp.button()) + QString::fromLatin1(", ") +
    QString::fromLatin1("light: ") + QColor2Str(colgrp.light()) + QString::fromLatin1(", ") +
    QString::fromLatin1("dark: ") + QColor2Str(colgrp.dark()) + QString::fromLatin1(", ") +
    QString::fromLatin1("mid: ") + QColor2Str(colgrp.mid()) + QString::fromLatin1(", ") +
    QString::fromLatin1("text: ") + QColor2Str(colgrp.text()) + QString::fromLatin1(", ") +
    QString::fromLatin1("base: ") + QColor2Str(colgrp.base()) + QString::fromLatin1(", ") +
    QString::fromLatin1("background: ") + QColor2Str(colgrp.background()) + QString::fromLatin1(", ") +
    QString::fromLatin1("midlight: ") + QColor2Str(colgrp.midlight()) + QString::fromLatin1(", ") +
    QString::fromLatin1("brightText: ") + QColor2Str(colgrp.brightText()) + QString::fromLatin1(", ") +
    QString::fromLatin1("buttonText: ") + QColor2Str(colgrp.buttonText()) + QString::fromLatin1(", ") +
    QString::fromLatin1("shadow: ") + QColor2Str(colgrp.shadow()) + QString::fromLatin1(", ") +
    QString::fromLatin1("highlight: ") + QColor2Str(colgrp.highlight()) + QString::fromLatin1(", ") +
    QString::fromLatin1("highlightedText: ") + QColor2Str(colgrp.highlightedText());
  return *this;
}

/*!
  Writes a QPalette value to the stream. Each color group is output
  with its role and the corresponding QColorGroup value.
*/
KDStream& KDStream::operator<<( const QPalette& palette )
{
  *this << QString::fromLatin1("active: ") << palette.active() << endl
        << QString::fromLatin1("inactive: ") << palette.inactive() << endl
        << QString::fromLatin1("diabled: ") << palette.disabled();

  return *this;
}

/*!
  Writes a QCursor value to the stream. Each cursor is output with its
  name as listed in the \a QCursor reference documentation.
*/
KDStream& KDStream::operator<<( const QCursor& cursor )
{

  QString type;
  switch ( cursor.shape() ) {
#if ( QT_VERSION < 300 )
  case ArrowCursor: type = QString::fromLatin1("ArrowCursor"); break;
  case UpArrowCursor: type = QString::fromLatin1("UpArrowCursor"); break;
  case CrossCursor: type = QString::fromLatin1("CrossCursor"); break;
  case WaitCursor: type = QString::fromLatin1("WaitCursor"); break;
  case IbeamCursor: type = QString::fromLatin1("IbeamCursor"); break;
  case SizeVerCursor: type = QString::fromLatin1("SizeVerCursor"); break;
  case SizeHorCursor: type = QString::fromLatin1("SizeHorCursor"); break;
  case SizeBDiagCursor: type = QString::fromLatin1("SizeBDiagCursor"); break;
  case SizeFDiagCursor: type = QString::fromLatin1("SizeFDiagCursor"); break;
  case SizeAllCursor: type = QString::fromLatin1("SizeAllCursor"); break;
  case BlankCursor: type = QString::fromLatin1("BlankCursor"); break;
  case SplitVCursor: type = QString::fromLatin1("SplitVCursor"); break;
  case SplitHCursor: type = QString::fromLatin1("SplitHCursor"); break;
  case PointingHandCursor: type = QString::fromLatin1("PointingHandCursor"); break;
  case ForbiddenCursor: type = QString::fromLatin1("ForbiddenCursor"); break;
  case BitmapCursor: type = QString::fromLatin1("BitmapCursor"); break;
#else
  case Qt::ArrowCursor: type = QString::fromLatin1("ArrowCursor"); break;
  case Qt::UpArrowCursor: type = QString::fromLatin1("UpArrowCursor"); break;
  case Qt::CrossCursor: type = QString::fromLatin1("CrossCursor"); break;
  case Qt::WaitCursor: type = QString::fromLatin1("WaitCursor"); break;
  case Qt::IbeamCursor: type = QString::fromLatin1("IbeamCursor"); break;
  case Qt::SizeVerCursor: type = QString::fromLatin1("SizeVerCursor"); break;
  case Qt::SizeHorCursor: type = QString::fromLatin1("SizeHorCursor"); break;
  case Qt::SizeBDiagCursor: type = QString::fromLatin1("SizeBDiagCursor"); break;
  case Qt::SizeFDiagCursor: type = QString::fromLatin1("SizeFDiagCursor"); break;
  case Qt::SizeAllCursor: type = QString::fromLatin1("SizeAllCursor"); break;
  case Qt::BlankCursor: type = QString::fromLatin1("BlankCursor"); break;
  case Qt::SplitVCursor: type = QString::fromLatin1("SplitVCursor"); break;
  case Qt::SplitHCursor: type = QString::fromLatin1("SplitHCursor"); break;
  case Qt::PointingHandCursor: type = QString::fromLatin1("PointingHandCursor"); break;
  case Qt::ForbiddenCursor: type = QString::fromLatin1("ForbiddenCursor"); break;
  case Qt::BitmapCursor: type = QString::fromLatin1("BitmapCursor"); break;
#endif
  }

  _output += type;

  return *this;
}

/*!
  Writes a QDate value to the stream. The format is the one defined by
  QDate::toString() and may be system-dependent.
*/
KDStream& KDStream::operator<<( const QDate& date )
{
  _output += date.toString();
  return *this;
}

/*!
  Writes a QDateTime value to the stream. The format is the one
  defined by QDateTime::toString() and may be system-dependent.
*/
KDStream& KDStream::operator<<( const QDateTime& datetime )
{
  _output += datetime.toString();
  return *this;
}

/*!
  Writes a QTime value to the stream. The format is the one defined by
  QTime::toString() and may be system-dependent.
*/
KDStream& KDStream::operator<<( const QTime& time )
{
  _output += time.toString();
  return *this;
}

/*!
  Writes a the raw name of a QFont value to the stream.
*/
KDStream& KDStream::operator<<( const QFont& font )
{
  _output += font.rawName();
  return *this;
}

/*!
  Writes a QPen value to the stream. The format is "QPen" plus the
  width, the color, and the style as defined in the \a QPen reference
  documentation.
*/
KDStream& KDStream::operator<<( const QPen& pen )
{
  QString style;
  switch ( pen.style() ) {
  case Qt::NoPen: style = QString::fromLatin1("NoPen"); break;
  case Qt::SolidLine: style = QString::fromLatin1("SolidLine"); break;
  case Qt::DashLine: style = QString::fromLatin1("DashLine"); break;
  case Qt::DotLine: style = QString::fromLatin1("DotLine"); break;
  case Qt::DashDotLine: style = QString::fromLatin1("DashDotLine"); break;
  case Qt::DashDotDotLine : style = QString::fromLatin1("DashDotDotLine "); break;
  case Qt::MPenStyle : break; // ignore
  }

  _output += QString::fromLatin1("QPen(%1,%2,%3)")
    .arg( pen.width() )
    .arg( QColor2Str( pen.color() ) )
    .arg( style );
  return *this;
}

/*!
  Writes a QPoint value to the stream. The format is "(x,y)".
*/
KDStream& KDStream::operator<<( const QPoint& point )
{
  _output += QString::fromLatin1("(%1,%2)").arg(point.x()).arg(point.y() );
  return *this;
}

/*!
  Writes a QSize value to the stream. The format is "(w x h)".
*/
KDStream& KDStream::operator<<( const QSize& size )
{
  _output += QString::fromLatin1("%1x%2").arg(size.width()).arg(size.height());
  return *this;
}

/*!
  Writes a QRect value to the stream. The format is "(width x height
  xoffset xpos yoffset ypos)".
*/
KDStream& KDStream::operator<<( const QRect& rect )
{
  QString xplus = (rect.x() >= 0) ? QString::fromLatin1("+") : QString::fromLatin1("");
  QString yplus = (rect.y() >= 0) ? QString::fromLatin1("+") : QString::fromLatin1("");
  _output += QString::fromLatin1("%1x%2%3%4%5%6")
    .arg( rect.width() )
    .arg( rect.height() )
    .arg( xplus )
    .arg( rect.x() )
    .arg( yplus )
    .arg( rect.y() );

  return *this;
}

/*!
  This is a helper method that converts a QColor object into a
  string.  For the predefined Qt colors, their name is output, for all
  other colors, the output is in the form #RRGGBB (as defined by
  QColor::name()).
*/
QString KDStream::QColor2Str( const QColor& col )
{
  if ( col == Qt::black )
    return QString::fromLatin1("black");
  else if ( col == Qt::white )
    return QString::fromLatin1("white");
  else if ( col == Qt::darkGray )
    return QString::fromLatin1("darkGray");
  else if ( col == Qt::gray )
    return QString::fromLatin1("gray");
  else if ( col == Qt::lightGray )
    return QString::fromLatin1("lightGray");
  else if ( col == Qt::red )
    return QString::fromLatin1("red");
  else if ( col == Qt::green )
    return QString::fromLatin1("green");
  else if ( col == Qt::blue )
    return QString::fromLatin1("blue");
  else if ( col == Qt::cyan )
    return QString::fromLatin1("cyan");
  else if ( col == Qt::magenta )
    return QString::fromLatin1("magenta");
  else if ( col == Qt::yellow )
    return QString::fromLatin1("yellow");
  else if ( col == Qt::darkRed )
    return QString::fromLatin1("darkRed");
  else if ( col == Qt::darkGreen )
    return QString::fromLatin1("darkGreen");
  else if ( col == Qt::darkBlue )
    return QString::fromLatin1("darkBlue");
  else if ( col == Qt::darkCyan )
    return QString::fromLatin1("darkCyan");
  else if ( col == Qt::darkMagenta )
    return QString::fromLatin1("darkMagenta");
  else if ( col == Qt::darkYellow )
    return QString::fromLatin1("darkYellow");
  else if ( col == Qt::color0 )
    return QString::fromLatin1("color0");
  else if ( col == Qt::color1 )
    return QString::fromLatin1("color1");
  else
    return col.name();
}

/*!
  Writes a QObject value to the stream. Included information is the
  class name, the object name, the properties and their types.
*/
KDStream& KDStream::operator<<( const QObject& obj )
{
  *this << QString::fromLatin1(obj.className()) + QString::fromLatin1("(") + QString::fromLatin1(obj.name()) << QString::fromLatin1("):")<< endl;
  QMetaObject* meta = obj.metaObject();
  QStrList props = meta->propertyNames(true);

  unsigned int maxWidth = 0;
  for ( QStrListIterator it(props) ; *it; ++it ) {
    maxWidth = QMAX( maxWidth, QString::fromLatin1(*it).length() );
  }

  for ( QStrListIterator it2(props) ; *it2; ++it2 ) {
    *this << QString::fromLatin1("  ") << QString::fromLatin1(*it2).leftJustify(maxWidth) << QString::fromLatin1(": [") << obj.property(*it2) << QString::fromLatin1("]") << endl;
  }
  return *this;
}

/*!
  Writes a QVariant value to the stream. The format is dependent on
  the actual contents of the QVariant object.
*/
KDStream& KDStream::operator<<( const QVariant& var)
{
  switch (var.type() )
    {
    case QVariant::Invalid: *this << QString::fromLatin1("*INVALID*"); break;
    case QVariant::Map: *this << var.toMap(); break;
    case QVariant::List: *this << var.toList(); break;
    case QVariant::String: *this << var.toString(); break;
    case QVariant::StringList: *this << var.toStringList(); break;
    case QVariant::Font: *this << var.toFont(); break;
    case QVariant::Pixmap: *this << QString::fromLatin1("-");break;

    case QVariant::Brush: *this << var.toBrush(); break;
    case QVariant::Rect: *this << var.toRect(); break;
    case QVariant::Size: *this << var.toSize(); break;
    case QVariant::Color: *this << var.toColor(); break;
    case QVariant::Palette: *this << var.toPalette(); break;
    case QVariant::ColorGroup: *this << var.toColorGroup(); break;
    case QVariant::IconSet: *this << QString::fromLatin1("-"); break;
    case QVariant::Point: *this << var.toPoint(); break;
    case QVariant::Image: *this << QString::fromLatin1("-"); break;
    case QVariant::Int: *this << var.toInt(); break;
    case QVariant::UInt: *this << var.toUInt(); break;
    case QVariant::Bool: *this << var.toBool(); break;
    case QVariant::Double: *this << var.toDouble(); break;
    case QVariant::CString: *this << var.toCString(); break;
    case QVariant::PointArray: *this << var.toPointArray(); break;
    case QVariant::Region: *this << QString::fromLatin1("-"); break;
    case QVariant::Bitmap: *this << QString::fromLatin1("-"); break;
    case QVariant::Cursor: *this << var.toCursor(); break;
    case QVariant::SizePolicy: *this << var.toSizePolicy(); break;
#if ( QT_VERSION >= 300 )
    case QVariant::Date: *this << var.toDate(); break;
    case QVariant::Time: *this << var.toTime(); break;
    case QVariant::DateTime: *this << var.toDateTime(); break;
    case QVariant::ByteArray: *this << var.toByteArray(); break;
    case QVariant::BitArray: *this << var.toBitArray(); break;
    case QVariant::KeySequence: *this << var.toKeySequence(); break;
#if ( QT_VERSION >= 0x030100 )
    case QVariant::Pen: *this << var.toPen(); break;
#endif
#endif
    }
  return *this;
}

/*!
  Writes a QBrush value to the stream. The format is "QBrush" plus the
  brush style as listed in the \a QBrush reference documentation and
  the brush color.
*/
KDStream& KDStream::operator<<( const QBrush& brush)
{
  QString style;
  switch ( brush.style() )
    {
    case Qt::NoBrush: style = QString::fromLatin1("NoBrush"); break;
    case Qt::SolidPattern: style = QString::fromLatin1("SolidPattern"); break;
    case Qt::Dense1Pattern: style = QString::fromLatin1("Dense1Pattern"); break;
    case Qt::Dense2Pattern: style = QString::fromLatin1("Dense2Pattern"); break;
    case Qt::Dense3Pattern: style = QString::fromLatin1("Dense3Pattern"); break;
    case Qt::Dense4Pattern: style = QString::fromLatin1("Dense4Pattern"); break;
    case Qt::Dense5Pattern: style = QString::fromLatin1("Dense5Pattern"); break;
    case Qt::Dense6Pattern: style = QString::fromLatin1("Dense6Pattern"); break;
    case Qt::Dense7Pattern: style = QString::fromLatin1("Dense7Pattern"); break;
    case Qt::HorPattern: style = QString::fromLatin1("HorPattern"); break;
    case Qt::VerPattern: style = QString::fromLatin1("VerPattern"); break;
    case Qt::CrossPattern: style = QString::fromLatin1("CrossPattern"); break;
    case Qt::BDiagPattern: style = QString::fromLatin1("BDiagPattern"); break;
    case Qt::FDiagPattern: style = QString::fromLatin1("FDiagPattern"); break;
    case Qt::DiagCrossPattern: style = QString::fromLatin1("DiagCrossPattern"); break;
    case Qt::CustomPattern: style = QString::fromLatin1("CustomPattern"); break;
    }
  _output += QString::fromLatin1("QBrush(%1,%2)").arg(style).arg(QColor2Str(brush.color()));
  return *this;
}

/*!
  Writes a QSizePolicy value to the stream. The output contains the
  horizontal and vertical size policy and whether the policy has a
  "height for width" setting.
*/
KDStream& KDStream::operator<<( const QSizePolicy& policy)
{
  QString hor, ver;

  switch ( policy.horData() )
    {
    case QSizePolicy::Fixed: hor=QString::fromLatin1("Fixed"); break;
    case QSizePolicy::Minimum : hor=QString::fromLatin1("Minimum "); break;
    case QSizePolicy::Maximum: hor=QString::fromLatin1("Maximum"); break;
    case QSizePolicy::Preferred: hor=QString::fromLatin1("Preferred"); break;
    case QSizePolicy::MinimumExpanding: hor=QString::fromLatin1("MinimumExpanding"); break;
    case QSizePolicy::Expanding: hor=QString::fromLatin1("Expanding"); break;
#if ( QT_VERSION >= 300 )
    case QSizePolicy::Ignored: hor=QString::fromLatin1("Ignored"); break;
#endif
    }
  switch ( policy.verData() )
    {
    case QSizePolicy::Fixed: ver=QString::fromLatin1("Fixed"); break;
    case QSizePolicy::Minimum : ver=QString::fromLatin1("Minimum "); break;
    case QSizePolicy::Maximum: ver=QString::fromLatin1("Maximum"); break;
    case QSizePolicy::Preferred: ver=QString::fromLatin1("Preferred"); break;
    case QSizePolicy::MinimumExpanding: ver=QString::fromLatin1("MinimumExpanding"); break;
    case QSizePolicy::Expanding: ver=QString::fromLatin1("Expanding"); break;
#if ( QT_VERSION >= 300 )
    case QSizePolicy::Ignored: ver=QString::fromLatin1("Ignored"); break;
#endif
    }
  QString hforw = policy.hasHeightForWidth() ? QString::fromLatin1("true") : QString::fromLatin1("false");
  _output += QString::fromLatin1("QSizePolicy(hor=%1,ver=%2, hasHeightForWidth=%3)")
    .arg(hor).arg(ver).arg(hforw);
  return *this;
}

#if ( QT_VERSION >= 300 )
/*!
  Writes a QKeySequence value to the stream. The output format is the
  string value of the key sequence.
*/
KDStream& KDStream::operator<<( const QKeySequence& keySeq)
{
  _output += QString(keySeq);
  return *this;
}
#endif

/*!
  Writes a QStrList value to the stream. The output is the individual
  strings.
*/
KDStream& KDStream::operator<<( const QStrList& list )
{
  KDStream_ptrListStream( *this, QStrListIterator( list ), false );
  return *this;
}




/* Classes that do not need to be supported:

QCollection - abstract class // Qt 2
QBitArray - QArray implemented.
QByteArray - QAray implemented.
QPointArray - QAray implemented.
QGArray - internal class // Qt 2
QStringList - It's just a QValueList.

QGCache // Qt 2
QGDict // Qt 2
QGList // Qt 2
QGVector // Qt 2
QGCacheIterator // Qt 2
QAsciiCacheIterator
QCacheIterator
QIntCacheIterator
QGDictIterator // Qt 2
QAsciiDictIterator
QDictIterator
QIntDictIterator
QPtrDictIterator
QListIterator // Qt 2
QStrListIterator
QGListIterator // Qt 2
QMapConstIterator
QMapIterator
QBitVal
QValueListConstIterator
QValueListIterator
QPtrListIterator// Qt 3
QPtrCollection// Qt 3
QSortedList - Depricated // Qt 2

*/

/* Qt classes not yet supported:
QRegion
QAccel
QAccessible // Qt 3
QAccessibleInterface// Qt 3
QAccessibleObject// Qt 3
QAction
QActionGroup
QApplication
QAsyncIO // Qt 2
QBitmap
QBoxLayout
QBuffer
QButton
QButtonGroup
QCDEStyle
QCanvas
QCanvasEllipse
QCanvasItem
QCanvasItemList// Qt 3
QCanvasLine
QCanvasPixmap
QCanvasPixmapArray
QCanvasPolygon
QCanvasPolygonalItem
QCanvasRectangle
QCanvasSpline// Qt 3
QCanvasSprite
QCanvasText
QCanvasView
QCheckBox
QCheckListItem
QCheckTableItem// Qt 3
QChildEvent
QClipboard
QCloseEvent
QColorDialog
QColorDrag
QComboBox
QComboTableItem// Qt 3
QCommonStyle
QComponentFactory// Qt 3
QComponentFactoryInterface// Qt 3
QComponentInterface// Qt 3
QComponentServerInterface// Qt 3
QContextMenuEvent// Qt 3
QCopChannel
QCustomEvent
QCustomMenuItem
QDataBrowser// Qt 3
QDataPump // Qt 2
QDataSink // Qt 2
QDataSource // Qt 2
QDataStream
QDataTable// Qt 3
QDataView// Qt 3
QDateEdit// Qt 3
QDateTimeEdit// Qt 3
QDesktopWidget// Qt 3
QDial
QDialog
QDir
QDns
QDockArea// Qt 3
QDockWindow// Qt 3
QDomAttr
QDomCDATASection
QDomCharacterData
QDomComment
QDomDocument
QDomDocumentFragment
QDomDocumentType
QDomElement
QDomEntity
QDomEntityReference
QDomImplementation
QDomNamedNodeMap
QDomNode
QDomNodeList
QDomNotation
QDomProcessingInstruction
QDomText
QDoubleValidator
QDragEnterEvent
QDragLeaveEvent
QDragMoveEvent
QDragObject
QDropEvent
QDropSite // Qt 2
QEditorFactory// Qt 3
QErrorMessage// Qt 3
QEucJpCodec // Qt 2
QEucKrCodec // Qt 2
QEvent
QFeatureListInterface// Qt 3
QFile
QFileDialog
QFileIconProvider
QFileInfo
QFilePreview
QFocusData
QFocusEvent
QFontDatabase
QFontDialog
QFontInfo
QFontManager// Qt 3
QFontMetrics
QFrame
QFtp
QGL
QGLColormap// Qt 3
QGLContext
QGLFormat
QGLWidget
QGLayoutIterator
QGbkCodec // Qt 2
QGrid
QGridLayout
QGridView// Qt 3
QGroupBox
QGuardedPtr
QHBox
QHBoxLayout
QHButtonGroup
QHGroupBox
QHeader
QHideEvent
QHostAddress
QHttp// Qt 3
QIMEvent// Qt 3
QIODevice
QIODeviceSource // Qt 2
QIconDrag
QIconDragItem
QIconSet
QIconView
QIconViewItem
QImage
QImageConsumer
QImageDecoder
QImageDrag
QImageFormat
QImageFormatType
QImageIO
QInputDialog
QIntValidator
QInterlaceStyle
QJisCodec // Qt 2
QJpUnicodeConv // Qt 2
QKeyEvent
QLCDNumber
QLNode // Qt 2
QLabel
QLayout
QLayoutItem
QLayoutIterator
QLibrary// Qt 3
QLibraryInterface// Qt 3
QLineEdit
QListBox
QListBoxItem
QListBoxPixmap
QListBoxText
QListView
QListViewItem
QListViewItemIterator
QLocalFs
QLock// Qt 3
QMainWindow
QMap
QMemArray// Qt 3
QMenuBar
QMenuData
QMessageBox
QMetaObject
QMetaProperty
QMimeSource
QMimeSourceFactory
QMotifPlusStyle
QMotifStyle
QMouseEvent
QMoveEvent
QMovie
QMultiLineEdit // Qt 2
QMutex
QNPInstance
QNPStream
QNPWidget
QNPlugin
QNetworkOperation
QNetworkProtocol
QObject
QPNGImagePacker
QPaintDevice
QPaintDeviceMetrics
QPaintEvent
QPainter
QPicture
QPixmap
QPixmapCache
QPlatinumStyle
QPluginManager// Qt 3
QPopupMenu
QPrinter
QProcess// Qt 3
QProgressBar
QProgressDialog
QPushButton
QRadioButton
QRangeControl
QRegExp
QRegExpValidator// Qt 3
QResizeEvent
QSGIStyle
QScreen// Qt 3
QScreenCursor // Qt 2
QScrollBar
QScrollView
QSemaphore
QSemiModal // Qt 2
QServerSocket
QSessionManager
QSettings// Qt 3
QShared // Qt 2
QShowEvent
QSignal
QSignalMapper
QSimpleRichText
QSizeGrip
QSjisCodec // Qt 2
QSlider
QSocket
QSocketDevice
QSocketNotifier
QSound
QSpacerItem
QSpinBox
QSplitter
QSql// Qt 3
QSqlCursor// Qt 3
QSqlDatabase// Qt 3
QSqlDriver// Qt 3
QSqlEditorFactory// Qt 3
QSqlError// Qt 3
QSqlField// Qt 3
QSqlForm// Qt 3
QSqlIndex// Qt 3
QSqlPropertyMap// Qt 3
QSqlQuery// Qt 3
QSqlRecord// Qt 3
QSqlResult// Qt 3
QStatusBar
QStoredDrag
QStyle
QStyleSheet
QStyleSheetItem
QTab
QTabBar
QTabDialog
QTabWidget
QTable
QTableItem
QTableSelection
QTableView // Qt 2
QTabletEvent// Qt 3
QTextBrowser
QTextCodec
QTextDecoder
QTextDrag
QTextEdit// Qt 3
QTextEncoder
QTextIStream
QTextOStream
QTextStream
QTextView // Qt 2
QThread
QTimeEdit// Qt 3
QTimer
QTimerEvent
QToolBar
QToolButton
QToolTip
QToolTipGroup
QTranslator
QTranslatorMessage
QTsciiCodec // Qt 2
QUnknownInterface// Qt 3
QUriDrag
QUrl
QUrlInfo// Qt 3
QUrlOperator
QUuid// Qt 3
QVBox
QVBoxLayout
QVButtonGroup
QVGroupBox
QValidator
QVariant
QWMatrix
QWSDecoration
QWSKeyboardHandler
QWSMouseHandler
QWSServer
QWSWindow
QWaitCondition
QWhatsThis
QWheelEvent
QWidget
QWidgetFactory// Qt 3
QWidgetItem
QWidgetStack
QWindowsMime// Qt 3
QWindowsStyle
QWizard
QWorkspace
QXmlAttributes
QXmlContentHandler
QXmlDTDHandler
QXmlDeclHandler
QXmlDefaultHandler
QXmlEntityResolver
QXmlErrorHandler
QXmlInputSource
QXmlLexicalHandler
QXmlLocator
QXmlNamespaceSupport
QXmlParseException
QXmlReader
QXmlSimpleReader
QXtApplication
QXtWidget
Qt
*/
