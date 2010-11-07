/*
 * Copyright 2008 Joshua Foster
 *
 * This file is part of the Win32 AppBar for Qt project.
 *
 * This project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this project.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************
 *
 * Implementation for QAppBar class
 * 
 * Author: Joshua Foster
 * Date: January 2008
 */

#include "QAppBar.h"

#include <QMouseEvent>
#include <QPainter>
#include <QLinearGradient>

#include <iostream>
using namespace std;


QAppBarResizeHandle::QAppBarResizeHandle( QAppBarEdge edge, QAppBar* appbar )
: QPushButton()
{
	this->appbar = appbar;
	setFixedWidth( 4 );
	setFlat( true );
	setCursor( Qt::SizeHorCursor );
	resizeInProgress = false;
	setEdge( edge );
}
	
void QAppBarResizeHandle::mousePressEvent( QMouseEvent* e )
{
	resizeInProgress = true;
	startX = parentWidget()->x();
	startWidth = parentWidget()->width();
	mouseStart = e->globalX();
}

void QAppBarResizeHandle::mouseMoveEvent( QMouseEvent* e )
{
	int widthDelta = e->globalX() - mouseStart;

	if( edge == QAppBarEdgeLeft )
	{
		// If left edge - add the amount the mouse has moved to the width of the appbar
		appbar->resize( startWidth + widthDelta, parentWidget()->height());
	}
	else
	{
		// If right edge - SUBTRACT the amount the mouse has moved from the width of the appbar,
		// and move the appbar by that amount to keep it against the right edge of the screen
		appbar->resize( startWidth - widthDelta, parentWidget()->height());
		// The window might not actually resize to the width we're requesting (for instance, if
		// it has a minimum width constraint), so we need to factor in the actual width here
		// to ensure we don't push the window off the edge of the screen in this move()
		// operation.
		appbar->move( startX + startWidth - parentWidget()->width(), parentWidget()->y());
	}
}

void QAppBarResizeHandle::mouseReleaseEvent( QMouseEvent* e )
{
	resizeInProgress = false;
	int widthDelta = e->globalX() - mouseStart;
	// Set the new appbar width with SetWidth - this will tell the OS adjust the desktop accordingly
	if( edge == QAppBarEdgeLeft )
	{
		appbar->setWidth( startWidth + widthDelta );
	}
	else
	{
		// No need to do a move operation here, SetWidth will make sure the taskbar is against the
		// edge of the screen
		appbar->setWidth( startWidth - widthDelta );
	}
}

void QAppBarResizeHandle::paintEvent( QPaintEvent* event )
{
	QPainter* painter = new QPainter( this );
	
	int points[2] = { 0, 1 };
	if( this->edge == QAppBarEdgeRight ) { points[0] = 1; points[1] = 0; }
	
	QLinearGradient gradient( 0, 0, this->width()-1, 0 );
	gradient.setColorAt( points[0], QColor( 0, 0, 0, 0 ));
	gradient.setColorAt( points[1], QColor( 128, 128, 128, 128 ));
	painter->fillRect( this->rect(), gradient );
}

///////////////////////////////////////////////////////////////////////////////

QAppBar::QAppBar( QAppBarEdge edge, int width )
: QWidget( 0, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint )  
{
	resizeHandle = new QAppBarResizeHandle( edge, this );
	
	contentArea = new QVBoxLayout();
	contentArea->setSpacing( 0 );
	contentArea->setContentsMargins( 0, 0, 0, 0 );
	
	hLayout = new QHBoxLayout( this );
	hLayout->setSpacing( 1 );
	hLayout->setContentsMargins( 0, 0, 0, 0 );
	hLayout->addLayout( contentArea );
	hLayout->setStretchFactor( contentArea, 1 );
	hLayout->addWidget( resizeHandle );
	
	if( edge == QAppBarEdgeRight )
	{
		hLayout->setDirection( QBoxLayout::RightToLeft );
	}
	
	abd.cbSize = sizeof( APPBARDATA );
	abd.uEdge = (int)edge;
	
	registerAppBar();
	setWidth( width );
}

void QAppBar::registerAppBar()
{
	abd.hWnd = (HWND)winId();
	abd.uCallbackMessage = WM_APPBAR_CALLBACK;
	
	if( SHAppBarMessage( ABM_NEW, &abd ) != TRUE )
		cout << "Error: QAppBar: SHAppBarMessage( ABM_NEW ) failed" << endl;
}

void QAppBar::unregisterAppBar()
{
	// Delete the 'reserved area' for this appbar and stretch the desktop back
	// to normal.
	SHAppBarMessage( ABM_REMOVE, &abd );
}

void QAppBar::setWidth( int w )
{
	// Positioning an appbar is a three-step process:
	// 1) We ask Windows what area is available for appbars by sending it an
	//    ABM_QUERYPOS with a RECT covering the entire desktop; it returns
	//    a modified RECT that excludes the taskbar and other appbars.
	// 2) We come up with a position and size for the appbar (based on
	//    this.edge and this.width), staying inside this RECT, and pass it
	//    to Windows using ABM_SETPOS.
	// 3) On the slim chance that another appbar appears between ABM_QUERYPOS
	//    and ABM_SETPOS, we'll get a modified RECT back.  Use this final
	//    RECT to setGeometry().
	
	// If child widgets have mandated a minimum width on this window, it
	// must be respected
	if( minimumWidth() != 0 ) w = max( w, minimumWidth());

	// Get the area of the entire desktop
	abd.rc.left = abd.rc.top = 0;
	abd.rc.right = GetSystemMetrics( SM_CXSCREEN );
	abd.rc.bottom = GetSystemMetrics( SM_CYSCREEN );
	
	// Let Windows subtract the amount being used by other appbars and the
	// taskbar
	SHAppBarMessage( ABM_QUERYPOS, &abd );
	
	// Position our window on either the left or right side of the screen
	if( abd.uEdge == ABE_LEFT ) abd.rc.right = abd.rc.left + w;
	else abd.rc.left = abd.rc.right - w;
	
	// Pass the final bounding rectangle to the system
	SHAppBarMessage( ABM_SETPOS, &abd );
	
	// Set our actual position based on what came back from ABM_SETPOS
	RECT& rc = abd.rc;
	setGeometry( rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top );
	resizeHandle->setFixedHeight( rc.bottom - rc.top );
}

void QAppBar::setEdge( QAppBarEdge edge )
{
	if( abd.uEdge != (uint)edge )
	{
		abd.uEdge = (uint)edge;
		resizeHandle->setEdge( edge );
		setWidth( width());
		// Reverse the layout so the resize handle always appears on the inner edge of the appbar
		((QHBoxLayout*)layout())->setDirection( edge == QAppBarEdgeLeft ? QBoxLayout::LeftToRight : QBoxLayout::RightToLeft );
	}
}

void QAppBar::switchEdge()
{
	if( abd.uEdge == QAppBarEdgeLeft ) setEdge( QAppBarEdgeRight );
	else setEdge( QAppBarEdgeLeft );
}

bool QAppBar::winEvent( MSG* msg, long* result )
{
	*result = 0;

	switch( msg->message )
	{
	case WM_APPBAR_CALLBACK:
		appBarCallback( msg->wParam, msg->lParam );
		break;

	case WM_ACTIVATE:
		SHAppBarMessage( ABM_ACTIVATE, &abd );
		break;

	case WM_WINDOWPOSCHANGED:
		SHAppBarMessage( ABM_WINDOWPOSCHANGED, &abd );
		break;

	default:
		return false;
	}
	
	return true;
}

void QAppBar::appBarCallback( WPARAM msg, LPARAM lParam )
{
	if( msg == ABN_FULLSCREENAPP )
	{
		// An application is going fullscreen (lParam = 1) or all fullscreen apps
		// have ended (lParam = 0).  We are supposed to use this to remove or re-add
		// the 'topmost' style attribute.  But apparently being a Qt.Tool window
		// takes care of this? Tried one fullscreen app (Windows Media Player) and 
		// the appbar hid, minimized it and the appbar reappeared as topmost.
	}
	else if( msg == ABN_POSCHANGED )
	{
		// The taskbar or another appbar has changed its size or position, we might
		// need to move now.
		setWidth( width());
	}
}
