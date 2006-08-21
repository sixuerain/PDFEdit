#ifndef __PAGEVIEWMODE_H__
#define __PAGEVIEWMODE_H__

#include <qpainter.h>
#include <qevent.h>
#include <qwidget.h>
#include <vector>
#include "pdfoperators.h"
#include "rect2Darray.h"


using namespace pdfobjects;

namespace gui {

//  ---------------------   drawing objects  ----------------- //
class DrawingObject;

class DrawingObjectFactory {
	public:
			static DrawingObject * create( const QString & nameOfObject );
};

/** Class is use as STRATEGY patern to drawing objects in mode.
 * This class define interface and base functionality.
 */
class DrawingObject {
	public:
			/** Drawing object useing \a painter from \a p1 to \a p2.
			 * @param painter initialize painter for drawing.
			 * @param p1 start point
			 * @param p2 end point
			 *
			 * This method is mostly useing if don't moving or resizing and is pressed left button.
			 */
			virtual void drawObject ( QPainter & painter, QPoint p1, QPoint p2 );
			/** Drawing region \a reg useing \a painter for draw.
			 * @param painter initialize painter for drawing.
			 * @param reg region for drawing.
			 *
			 * This method is mostly useing for drawing selected operators (nothing button is press or in text mode).
			 */
			virtual void drawObject ( QPainter & painter, QRegion reg );
			/** Drawing rectangle \a rect useing \a painter for draw.
			 * @param painter initialize painter for drawing.
			 * @param rect rectangle for drawing.
			 *
			 * This method is mostly useing for drawing selected bounding rectangle (moving and resizing selected region).
			 */
			virtual void drawObject ( QPainter & painter, QRect rect );

			/** Standard constructor.
			 * Initialize pen.
			 */
			DrawingObject();
			/** Standard destructor. */
			virtual ~DrawingObject ();
	protected:
			/** Pen for drawing line (color, width, solid or ...) */
			QPen	pen;
};

/** Class is STRATEGY pattern to draw line as new object.
 * (Resizing, moving and draw selected region is keeping from parent (see DrawingObject).)
 */
class DrawingLine: public DrawingObject {
	public:
			/** Standard constructor.
			 * Initialize pen.
			 */
			DrawingLine ();
			/** Standard destructor. */
			virtual ~DrawingLine ();

			/** Drawing object useing \a painter from \a p1 to \a p2.
			 * @param painter initialize painter for drawing.
			 * @param p1 start point
			 * @param p2 end point
			 *
			 * This method is mostly useing if don't moving or resizing and is pressed left button.
			 */
			virtual void drawObject ( QPainter & painter, QPoint p1, QPoint p2 );
};

/** Class is STRATEGY pattern to draw rectangle as new object.
 * (Resizing, moving and draw selected region is keeping from parent (see DrawingObject).)
 */
class DrawingRect: public DrawingObject {
	public:
			/** Standard constructor.
			 * Initialize pen.
			 */
			DrawingRect ();
			/** Standard destructor. */
			virtual ~DrawingRect ();

			/** Drawing object useing \a painter from \a p1 to \a p2.
			 * @param painter initialize painter for drawing.
			 * @param p1 start point
			 * @param p2 end point
			 *
			 * This method is mostly useing if don't moving or resizing and is pressed left button.
			 */
			virtual void drawObject ( QPainter & painter, QPoint p1, QPoint p2 );
};
//  ---------------------  selection mode  --------------------- //
class PageViewMode;

class PageViewModeFactory {
	public:
			static PageViewMode * create(	const QString & nameOfMode,
											const QString & drawingObject,
											const QString & scriptFncAtMouseRelease );
};

/** Class is STRATEGY pattern for mode construction.
 * Includes method for selecting objects on page and manipulating with him.
 */
class PageViewMode: public QObject {
	Q_OBJECT
	signals:
			/** Signal generated by new selection known objects
			 * @param  objects Objects which are selected.
			 */
			void newSelectedOperators( const std::vector< boost::shared_ptr< PdfOperator > > & objects );

			/** Signal is generated if page nedd repaint after changes. */
			void needRepaint ( );
			/** Signal is generated if after released right button.
			 * @param PagePos Mouse position on page.
			 */
			void popupMenu ( const QPoint & PagePos /*, Cobject & */ );
			/** Signal is generated if need execute command \a cmd in script.
			 * @param cmd Script command for executing.
			 */
			void executeCommand ( QString cmd );
	public slots:
			/** Method is calling if is need move selected region (operation 'move' is finished).
			 * @param relativeMove relativ move x and y position of selected region.
			 *
			 * This method call script.
			 *
			 * @see movedSelectedObjects( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void movedSelectedObjects ( QPoint relativeMove );
			/** Method is calling if is need resize selected region (operation 'resize' is finished).
			 * @param dleft		delta position of bounding-rectangle's left edge of selected operators.
			 * @param dtop		delta position of bounding-rectangle's top edge of selected operators.
			 * @param dright	delta position of bounding-rectangle's right edge of selected operators.
			 * @param dbottom	delta position of bounding-rectangle's bottom edge of selected operators.
			 *
			 * This method call script.
			 *
			 * @see resizedSelectedObjects( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void resizedSelectedObjects ( int dleft, int dtop, int dright, int dbottom );

			/** Method is calling if is need move selected region (operation 'move' is NOT finished).
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * @see mousePressLeftButton
			 * @see mouseReleaseLeftButton
			 * @see mouseMoveWithPressedLeftButton
			 *
			 * @see movedSelectedObjects( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void moveSelectedObjects ( QMouseEvent * e, QPainter * p, QWidget * w );
			/** Method is calling if is need move selected region (operation 'move' is finished).
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * @see movedSelectedObjects( QPoint )
			 * @see moveSelectedObjects( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void movedSelectedObjects ( QMouseEvent * e, QPainter * p, QWidget * w );

			/** Method is calling if is need resize selected region (operation 'resize' is NOT finished).
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * @see mousePressLeftButton
			 * @see mouseReleaseLeftButton
			 * @see mouseMoveWithPressedLeftButton
			 *
			 * @see resizedSelectedObjects( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void resizeSelectedObjects ( QMouseEvent * e, QPainter * p, QWidget * w );
			/** Method is calling if is need resize selected region (operation 'resize' is finished).
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * @see resizeSelectedObjects( QMouseEvent *, QPainter *, QWidget * )
			 * @see resizedSelectedObjects ( int, int, int, int )
			 */
			virtual void resizedSelectedObjects ( QMouseEvent * e, QPainter * p, QWidget * w );

			/* ------------------------------------------------------------------------------ */
			/* --- mouse press (and coresponding release) events not above selection area --- */
			/* ------------------------------------------------------------------------------ */
			virtual void mousePressLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseReleaseLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mousePressRightButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseReleaseRightButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mousePressMidButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseReleaseMidButton ( QMouseEvent * e, QPainter * p, QWidget * w );

			virtual void mouseMoveWithPressedLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );

			/* ------------------------------------------------------------------------------ */
			/* ---        mouse events - equivalents of method called in QWidget          --- */
			/* ------------------------------------------------------------------------------ */

			/** Method is call if press mouse button (see Qt::QWidget::mousePressEvent)
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * Is switch which call other method. For press left button is settings \a isPressedLeftButton, \a isMoving,
			 * \a isResizing, \a pomCur and undrawing old selectedOpRegion.
			 *
			 * @see mousePressLeftButton ( QMouseEvent *, QPainter *, QWidget * )
			 * @see mousePressRightButton ( QMouseEvent *, QPainter *, QWidget * )
			 * @see mousePressMidButton ( QMouseEvent *, QPainter *, QWidget * )
			 * @see moveSelectedObjects ( QMouseEvent *, QPainter *, QWidget * )
			 * @see resizeSelectedObjects ( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void mousePressEvent ( QMouseEvent * e, QPainter * p, QWidget * w );
			/** Method is call if release mouse button (see Qt::QWidget::mouseReleaseEvent)
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * Is switch which call other method. For press left button is settings \a isPressedLeftButton and drawing
			 * selectedOpRegion.
			 *
			 * @see mouseReleaseLeftButton ( QMouseEvent *, QPainter *, QWidget * )
			 * @see mouseReleaseRightButton ( QMouseEvent *, QPainter *, QWidget * )
			 * @see mouseReleaseMidButton ( QMouseEvent *, QPainter *, QWidget * )
			 * @see movedSelectedObjects ( QMouseEvent *, QPainter *, QWidget * )
			 * @see resizedSelectedObjects ( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void mouseReleaseEvent ( QMouseEvent * e, QPainter * p, QWidget * w );
			/** Method is call if mouse double-click (see Qt::QWidget::mouseDoubleClickEvent)
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * Call mousePressEvent.
			 *
			 * @see mousePressEvent ( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void mouseDoubleClickEvent ( QMouseEvent * e, QPainter * p, QWidget * w );
			/** Method is call if mouse is moving (see Qt::QWidget::mouseMoveEvent)
			 * @param e		Pointer to mouse event (see Qt::QMouseEvent).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * Is switch which call other method. Is settings \a pomCur and mouse cursor for widget \a w.
			 *
			 * @see moveSelectedObjects ( QMouseEvent *, QPainter *, QWidget * )
			 * @see resizeSelectedObjects ( QMouseEvent *, QPainter *, QWidget * )
			 * @see mouseMoveWithPressedLeftButton ( QMouseEvent *, QPainter *, QWidget * )
			 */
			virtual void mouseMoveEvent ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void wheelEvent ( QWheelEvent * e, QPainter * p, QWidget * w );

			/* ------------------------------------------------------------------------------ */
			/* ---         key events - equivalents of method called in QWidget           --- */
			/* ------------------------------------------------------------------------------ */
			virtual void keyPressEvent ( QKeyEvent * e, QPainter * p, QWidget * w );
			virtual void keyReleaseEvent ( QKeyEvent * e, QPainter * p, QWidget * w );

			/* ------------------------------------------------------------------------------ */
			/* ---        focus events - equivalents of method called in QWidget          --- */
			/* ------------------------------------------------------------------------------ */
			virtual void focusInEvent ( QFocusEvent *, QPainter * p, QWidget * w );
			virtual void focusOutEvent ( QFocusEvent *, QPainter * p, QWidget * w );

			/** Repaint method for draw actual state of mode (selected region, moving and resizing in action, ...).
			 * @param p		Pointer to initialized painter for draw changes (see Qt::QPainter).
			 * 				Method emit at end 'needRepaint' if \a p is NULL.
			 * @param w		Pointer to widget (see Qt::QWidget). E.g. for change mouse cursor above operators.
			 *
			 * It is only drawing method use, if viewing parent need clear repaint.
			 */
			virtual void repaint ( QPainter & p, QWidget * w  );

			virtual QRegion getSelectedRegion ();
			virtual void setSelectedRegion ( QRegion r );

			virtual void setWorkOperators ( const std::vector< boost::shared_ptr< PdfOperator > > & wOps );
			virtual void addWorkOperators ( const std::vector< boost::shared_ptr< PdfOperator > > & wOps );
			virtual void clearWorkOperators ();
			virtual void clearSelectedOperators ();
			virtual void setSelectedOperators ( const std::vector< boost::shared_ptr< PdfOperator > > & sOps );
			virtual void addSelectedOperators ( const std::vector< boost::shared_ptr< PdfOperator > > & sOps );
			virtual void actualizeSelection ();
	public:
			/** Function return if some object is selected (not in selection mode PageView::SelectRect)
			 * @return Return TRUE, if some object is selected. Otherwise return FALSE.
			 */
			virtual bool isSomeoneSelected ();

			/** Method set width of resizing zone
			 * @param width width in pixels
			 *
			 * Default is set to 2.
			 */
			void setResizingZone ( unsigned int width);

			/** Standard constructor.
			 * @param drawingObject				Text definition of drawing method (see DrawingObjectFactory)
			 * @param _scriptFncAtMouseRelease	Script command for call after select object(s)
			 */
			PageViewMode( const QString & drawingObject, const QString & _scriptFncAtMouseRelease );

			/** Standard destructor. */
			virtual ~PageViewMode();
	protected:
			/** enum of resizing parts */
			enum resizingParts	{ none = 0, left = 1, right = 2, top = 4, bottom = 8, onUnselectedObject = 16 };
			/** Set mapping cursors for viewing on the page for actual selection mode */
			virtual void setMappingCursor();

			/** Method for calculation resizing mode
			 * @param r rectsngle for resizing
			 * @param p cursor position
			 * 
			 * @return combination of enum resizingParts
			 */
			virtual int	theNeerestResizingMode ( const QRegion & r, const QPoint & p );

			void addOpsBBoxToRegion ( QRegion & r, const std::vector< boost::shared_ptr< PdfOperator > > & ops );
			QRect getBBox( const boost::shared_ptr<PdfOperator> & op ) const;
	protected:
			std::vector< boost::shared_ptr< PdfOperator > >		workOperators;
			std::vector< boost::shared_ptr< PdfOperator > >		selectedOperators;

			QRegion		workOpRegion;
			QRegion		selectedOpRegion;

			QRegion		mouseSelectedRegion;
			QPoint		pressPosition,
						releasePosition;
			QPoint		pointInRect;
			int			resizeCoefficientX;
			int			resizeCoefficientY;

			bool		isPressedLeftButton;
			bool		isMoving;
			bool		isResizing;

			/** Smart pointer for drawing selection area on page.
			 * Drawing object is ceated in constructor.
			 */
			boost::shared_ptr< DrawingObject >			drawingObject;

			/** Width of resizing zone.
			 * If selected area is [ [a,b] , [c,d] ]  ([a,b] is left top edge and
			 *  [c,d] is bottom right edge of selected region) then resizing area is
			 *  [ [a,b] , [c,d] ]  xor  [ [a+resizingZone, b+resizingZone], [c-resizingZone,d-resizingZone] ]
			 *  (c-a) > 2*resizingZone  and  (d-b) > 2*resizingZone
			 */
			int resizingZone;
			/** mapping array resizing mode to cursor shape */
			int mappingResizingModeToCursor [17];

			/** Name of script function, which will call at mouse release.
			 * Must be in this format:
			 * 		"fncName ( %1, %2, %3, %4 )"
			 * where
			 * 		%1	will be replaced with x1 (X position at mouse press)
			 * 		%2	will be replaced with y1 (Y position at mouse press)
			 * 		%3	will be replaced with x2 (X position at mouse release)
			 * 		%4	will be replaced with y2 (Y position at mouse release)
			 *
			 * @see QString::arg
			 * 
			 * Function in script:
			 * 			fncName ( x1:Number, y1:Number, x2:Number, y2:Number )  { ... }
			 */
			QString scriptFncAtMouseRelease;
			/** Name of script function, which will call at move selected objects.
			 * Must be in this format:
			 * 		"fncName ( %1, %2 )"
			 * where
			 * 		%1	will be replaced with dx
			 * 		%2	will be replaced with dy
			 *
			 * @see QString::arg
			 * 
			 * Function in script:
			 * 			fncName ( dx:Number, dy:Number )  { ... }
			 */
			QString scriptFncAtMoveSelectedObjects;
			/** Name of script function, which will call at resized selected objects.
			 * Must be in this format:
			 * 		"fncName ( %1, %2, %3, %4 )"
			 * where
			 * 		%1	will be replaced with dleft
			 * 		%2	will be replaced with dtop
			 * 		%3	will be replaced with dright
			 * 		%4	will be replaced with dbottom
			 *
			 * @see QString::arg
			 * 
			 * Function in script:
			 * 			fncName ( dleft:Number, dtop:Number, dright:Number, dbottom:Number )  { ... }
			 *
			 */
			QString scriptFncAtResizeSelectedObjects;
};

/** */
class PageViewMode_NewObject: public PageViewMode {
	Q_OBJECT
	public slots:
			virtual void mousePressLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseReleaseLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseMoveWithPressedLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );

			virtual void repaint ( QPainter & p, QWidget * w  );
	public:
			PageViewMode_NewObject ( const QString & drawingObject, const QString & _scriptFncAtMouseRelease );
};

/** */
class PageViewMode_TextSelection: public PageViewMode {
	Q_OBJECT
	public slots:
			virtual void mousePressLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseReleaseLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseMoveWithPressedLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );

			virtual void repaint ( QPainter & p, QWidget * w  );

			virtual void setSelectedRegion ( QRegion r );

			virtual void clearWorkOperators ();
			virtual void clearSelectedOperators ();
			virtual void addWorkOperators ( const std::vector< boost::shared_ptr< PdfOperator > > & wOps );
			virtual void addSelectedOperators ( const std::vector< boost::shared_ptr< PdfOperator > > & sOps );
	public:
			PageViewMode_TextSelection ( const QString & drawingObject, const QString & _scriptFncAtMouseRelease );
	protected:
			/** Set mapping cursors for viewing on the page for actual selection mode */
			virtual void setMappingCursor();

			void updateSelection (	const BBoxOfObjectOnPage< boost::shared_ptr<PdfOperator> > *	first,
									const BBoxOfObjectOnPage< boost::shared_ptr<PdfOperator> > *	last,
									QRegion *														selOpsRegion = NULL,
									std::vector< boost::shared_ptr< PdfOperator > > *				selOps = NULL );

			const BBoxOfObjectOnPage< boost::shared_ptr<PdfOperator> >	* getNearestObject( const QPoint & point );
	protected:
			/** 2D array of bboxes objects for special selection */
			Rect2DArray< boost::shared_ptr<PdfOperator> >	arrayOfBBoxes;
			/** first selected objects */
			const BBoxOfObjectOnPage< boost::shared_ptr<PdfOperator> >	* firstSelectedObject;
			/** last selected objects */
			const BBoxOfObjectOnPage< boost::shared_ptr<PdfOperator> >	* lastSelectedObject;
};

/** */
class PageViewMode_OperatorsSelection: public PageViewMode {
	Q_OBJECT
	public slots:
			virtual void mousePressLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseReleaseLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void movedSelectedObjects ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void resizeSelectedObjects ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void resizedSelectedObjects ( QMouseEvent * e, QPainter * p, QWidget * w );
			virtual void mouseMoveWithPressedLeftButton ( QMouseEvent * e, QPainter * p, QWidget * w );

			virtual void setSelectedRegion ( QRegion r );

			virtual void addWorkOperators ( const std::vector< boost::shared_ptr< PdfOperator > > & wOps );
			virtual void clearWorkOperators ();
	public:
			PageViewMode_OperatorsSelection ( const QString & drawingObject, const QString & _scriptFncAtMouseRelease );
	protected:
			void	findOperators (	const std::vector< boost::shared_ptr< PdfOperator > >	& in_v,
									std::vector< boost::shared_ptr< PdfOperator > >			& founded,
									const QRegion	& r );
			bool	findPrevOperator (	std::vector< boost::shared_ptr< PdfOperator > >::iterator	& it,
										std::vector< boost::shared_ptr< PdfOperator > >				& v,
										bool 			& fromEnd,
										const QPoint	& p );
			// iterator for vector of workOperators
			std::vector< boost::shared_ptr< PdfOperator > >::iterator		lastSelectedOperator;

			QPoint	resizingPress;
};

} // namespace gui

#endif
