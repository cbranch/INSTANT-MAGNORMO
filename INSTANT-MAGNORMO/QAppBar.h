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
 * QAppBar - A Win32 Application Desktop Toolbar ("appbar") widget for Qt 4
 * 
 * Author: Joshua Foster
 * Date: January 2008
 * 
 * QAppBar is a QWidget with special functionality that registers it as a
 * Windows "Application Desktop Toolbar". Appbars are always-on-top windows that
 * stick to one edge of the screen and modify the workarea so that maximized
 * windows don't stretch over them. The Taskbar is one example.
 * 
 * The QAppBar widget has a built-in resizing mechanism. Click and drag the
 * inner edge of the appbar to resize it.
 * 
 * Look at AppBarDemo.cpp for an example.
 * 
 * Unimplemented Features:
 *  - Docking to top and bottom edge
 *  - Undock to free-floating window
 *  - Auto-Hide
 *  - ABN_STATECHANGE to mimic Taskbar states
 * 
 * Reference: http://msdn2.microsoft.com/en-us/library/bb776821(VS.85).aspx
 */
#ifdef _WIN32
#pragma once

#include <QPushButton>
#include <QVBoxLayout>

#include <windows.h>
#include <shellapi.h>

///////////////////////////////////////////////////////////////////////////////

// Edge of the screen the appbar is on. Use this in QAppBar.__init__() and
// QAppBar.setEdge().
enum QAppBarEdge { QAppBarEdgeLeft = ABE_LEFT, QAppBarEdgeRight = ABE_RIGHT };

///////////////////////////////////////////////////////////////////////////////

class QAppBar;

/* An invisible widget that allows the AppBar to be resized by dragging its
 * outside edge.  It shouldn't be accessed outside of QAppBar.
 */
class QAppBarResizeHandle : public QPushButton
{
private:

	QAppBar* appbar;
	QAppBarEdge edge;
	bool resizeInProgress;
	int mouseStart, startX, startWidth;

public:

	QAppBarResizeHandle( QAppBarEdge edge, QAppBar* appbar );
	
	inline void setEdge( QAppBarEdge edge ) { this->edge = edge; }
	
	virtual void mousePressEvent( QMouseEvent* e );
	virtual void mouseMoveEvent( QMouseEvent* e );
	virtual void mouseReleaseEvent( QMouseEvent* e );
	
	virtual void paintEvent( QPaintEvent* event );
};

///////////////////////////////////////////////////////////////////////////////

#define WM_APPBAR_CALLBACK ( WM_USER + 1 )

class QAppBar : public QWidget
{
	Q_OBJECT

public:

	QAppBar( QAppBarEdge edge = QAppBarEdgeLeft, int width = 200 );
	virtual ~QAppBar() { unregisterAppBar(); }

public slots:

	/* Moves the taskbar to either the left (QAppBarEdge.Left) or right
	 * (QAppBarEdge.Right) side of the screen
	 */
	void setEdge( QAppBarEdge e );

	/* Toggles the appbar between the left and right edges of the screen */
	void switchEdge();

	/* Sets the width of the appbar. Works with the OS to find an area on the
	 * specified side of the screen, with the specified width, not covered by
	 * any other appbars.
	 */
	void setWidth( int w );

protected:

	APPBARDATA abd;
	QAppBarResizeHandle* resizeHandle;
	QHBoxLayout* hLayout;
	QVBoxLayout* contentArea;

	/* Overridden from QWidget to catch Win32 messages related to the AppBar */
	virtual bool winEvent( MSG* message, long* result );
	
private:

	/* Registers the app bar */
	void registerAppBar();
	
	/* Unregisters the app bar */
	void unregisterAppBar();

	/* Called by winEvent to handle WM_APPBAR_CALLBACK messages */
	void appBarCallback( WPARAM msg, LPARAM lParam );

	// This is hidden because it has no meaning to an appbar and will
	// just screw things up. You should only use setWidth(), setEdge() or
	// switchEdge() to modify the size and position of the appbar.
	QWidget::setGeometry;
	
public:

	/* Returns a vertical box layout on which widgets may be placed. */
	inline QVBoxLayout* getContentArea() { return contentArea; }
};
#endif
