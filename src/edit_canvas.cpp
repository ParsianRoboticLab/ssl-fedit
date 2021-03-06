// -*-c++-*-

/*!
  \file edit_canvas.h
  \brief main edit canvas class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGui>

#include "edit_canvas.h"

#include "edit_data.h"
#include "main_window.h"
#include "options.h"

#include <rcsc/common/server_param.h>
#include <rcsc/geom/triangle_2d.h>
#include <rcsc/math_util.h>

#include <iostream>

#include <Field.h>

#define TopGoal 0.50
#define ButtonGoal -0.50
#define FieldWidth 9.000


using namespace rcsc;
using namespace rcsc::formation;

/*-------------------------------------------------------------------*/
/*!

 */
EditCanvas::EditCanvas( QWidget * parent )
    :
      #ifdef USE_GLWIDGET
      QGLWidget( QGLFormat( QGL::SampleBuffers ), parent )
    #else
      QWidget( parent )
    #endif
    // foreground graphic context
    , M_field_color( 31, 160, 31 )
    //, M_field_brush( QColor( 31, 160, 31 ), Qt::SolidPattern )
    , M_field_dark_brush( QColor( 15, 143, 15 ), Qt::SolidPattern )
    , M_line_pen( QColor( 255, 255, 255 ), 0, Qt::SolidLine )
    //, M_triangle_pen( QColor( 255, 127, 0 ), 1, Qt::SolidLine )
    , M_triangle_pen( QColor( 255, 0, 0 ), 0, Qt::SolidLine )
    , M_constraint_pen( QColor( 255, 0, 255 ), 0, Qt::SolidLine )
    , M_triangle_font( "Sans Serif", 10 )
    , M_area_pen( QColor( 127, 127, 127 ), 0, Qt::SolidLine )
    , M_ball_pen( QColor( 255, 255, 255 ), 0, Qt::SolidLine )
    , M_ball_brush( QColor( 255, 255, 255 ), Qt::SolidPattern )
    , M_player_pen( QColor( 0, 0, 0 ), 0, Qt::SolidLine )
    , M_select_pen( Qt::white, 0, Qt::SolidLine )
    , M_player_brush( QColor( 255, 215, 0 ), Qt::SolidPattern )
    , M_symmetry_brush( QColor( 0, 255, 95 ), Qt::SolidPattern )
    , M_player_font( "Sans Serif", 10 )
    // background graphic context
    , M_background_contained_area_brush( QColor( 31, 143, 31 ), Qt::SolidPattern )
    , M_background_triangle_pen( QColor( 0, 127, 255 ), 0, Qt::SolidLine )
    , M_background_player_pen( QColor( 127, 127, 127 ), 0, Qt::SolidLine )
    , M_background_left_team_brush( QColor( 192, 251, 0 ), Qt::SolidPattern )
    , M_background_right_team_brush( QColor( 127, 20, 20 ), Qt::SolidPattern )
    , M_background_symmetry_brush( QColor( 0, 192, 31 ), Qt::SolidPattern )
    , M_background_font_pen( QColor( 0, 63, 127 ), 0, Qt::SolidLine )
{
    field = new CField();


    //this->setPalette( QPalette( M_field_brush.color() ) );
    this->setPalette( QPalette( M_field_color ) );
    this->setAutoFillBackground( true );

    this->setMouseTracking( true );
    this->setFocusPolicy( Qt::WheelFocus );

    //     double scale_w = this->width() / ( ServerParam::DEFAULT_PITCH_LENGTH + 10.0 );
    //     double scale_h =  this->height() / ( ServerParam::DEFAULT_PITCH_WIDTH + 10.0 );
    //     double scale_factor = qMin( scale_w, scale_h );
    //     M_transform.scale( scale_factor, scale_factor );
    //     M_transform.translate( ( ServerParam::DEFAULT_PITCH_LENGTH*0.5 + 5.0 ),
    //                            ( ServerParam::DEFAULT_PITCH_WIDTH*0.5 + 5.0 ) );

    //     std::cerr << "width=" << this->width()
    //               << " height=" << this->height()
    //               << "\nscale_w=" << scale_w
    //               << "  scale_h=" << scale_h
    //               << "  scale_factor=" << scale_factor << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
EditCanvas::~EditCanvas()
{
    //std::cerr << "delete EditCanvas" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::paintEvent( QPaintEvent * )
{
    QPainter painter( this );
    if ( Options::instance().autoFitMode() )
    {
        double scale_w = this->width() / ( _FIELD_WIDTH +0.5 );
        double scale_h =  this->height() / ( _FIELD_HEIGHT + 0.5 );
        double scale_factor = qMin( scale_w, scale_h );

        M_transform.reset();
        M_transform.translate( this->width() * 0.5, this->height() * 0.5 );
        M_transform.scale( scale_factor, scale_factor );
    }

    painter.setWorldTransform( M_transform );

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, true );
    }
    drawField( painter );
    if ( Options::instance().showBackgroundData() )
    {
        drawBackgroundData( painter );
    }
    drawData( painter );
    drawPlayers( painter );
    if ( Options::instance().showBackgroundData() )
    {
        drawBackgroundPlayers( painter );
    }
    drawBall( painter );
    drawConstraintSelection( painter );



}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::setAntialiasFlag( QPainter & painter,
                              bool on )
{
#ifdef USE_GLWIDGET
    //painter.setRenderHint( QPainter::HighQualityAntialiasing, false );
    painter.setRenderHint( QPainter::Antialiasing, on );
#else
    painter.setRenderHint( QPainter::Antialiasing, on );
#endif
}

/*-------------------------------------------------------------------*/
/*!

 */

void EditCanvas::drawArc(QPainter &painter, qreal centerX, qreal centerY, qreal radius, int start, int end)
{
    painter.drawArc(QRectF(centerX-radius, centerY-radius, 2*radius, 2*radius), start*16, (end-start)*16);
}

void
EditCanvas::drawField( QPainter & painter )
{
    static bool s_first = true;
    static QPainterPath s_lines;

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, false );
    }

    //painter.fillRect( M_transform.inverted().mapRect( painter.window() ), M_field_brush );

    if ( Options::instance().showBackgroundData() )
    {
        drawBackgroundContainedArea( painter );
    }

    if ( Options::instance().showTriangulation() )
    {
        drawContainedArea( painter );
    }

    // set screen coordinates of field
    const double left_x   =  field->ourGoal().x; // - ServerParam::DEFAULT_PITCH_LENGTH * 0.5;
    const double right_x  =  field->oppGoal().x;// + ServerParam::DEFAULT_PITCH_LENGTH * 0.5;

    //
    // lines
    //

    if ( s_first )
    {
        const double top_y =  -_FIELD_HEIGHT*0.5;// - ServerParam::DEFAULT_PITCH_WIDTH * 0.5;
        const double bottom_y = _FIELD_HEIGHT*0.5;

        const double pen_top_y =  -fabs(field->oppGoalL().y);
        const double pen_bottom_y = fabs(field->oppGoalL().y);

        const double goal_area_top_y =  -fabs(field->oppGoalL().y);  //- ServerParam::DEFAULT_GOAL_AREA_WIDTH * 0.5;
        const double goal_area_bottom_y = fabs(field->oppGoalL().y); //+ ServerParam::DEFAULT_GOAL_AREA_WIDTH * 0.5;

        s_first = false;

        // side lines & goal lines
        s_lines.moveTo( left_x, top_y );
        s_lines.lineTo( right_x, top_y );
        s_lines.lineTo( right_x, bottom_y );
        s_lines.lineTo( left_x, bottom_y );
        s_lines.lineTo( left_x, top_y );

#if 1
        // center line
        s_lines.moveTo( 0.0, top_y );
        s_lines.lineTo( 0.0, bottom_y );


        // fieldCenter.setRect(-0.500, 0.500, 1.000, 1.000);
        //
        //
        // center circle
        s_lines.addEllipse(-_CENTER_CIRCLE_RAD,
                           -_CENTER_CIRCLE_RAD,
                           2*_CENTER_CIRCLE_RAD,
                           2*_CENTER_CIRCLE_RAD);
#else
        s_lines.addRect( - ServerParam::DEFAULT_KEEPAWAY_LENGTH*0.5,
                         - ServerParam::DEFAULT_KEEPAWAY_WIDTH*0.5,
                         ServerParam::DEFAULT_KEEPAWAY_LENGTH,
                         ServerParam::DEFAULT_KEEPAWAY_WIDTH );
#endif

        // left penalty area
        /* s_lines.moveTo( left_x, pen_top_y );
        s_lines.lineTo( left_x + ServerParam::DEFAULT_PENALTY_AREA_LENGTH, pen_top_y );
        s_lines.lineTo( left_x + ServerParam::DEFAULT_PENALTY_AREA_LENGTH, pen_bottom_y );
        s_lines.lineTo( left_x, pen_bottom_y );

        // right penalty area
        s_lines.moveTo( right_x, pen_top_y );
        s_lines.lineTo( right_x - ServerParam::DEFAULT_PENALTY_AREA_LENGTH, pen_top_y );
        s_lines.lineTo( right_x - ServerParam::DEFAULT_PENALTY_AREA_LENGTH, pen_bottom_y );
        s_lines.lineTo( right_x, pen_bottom_y );

        // left goal area
        s_lines.moveTo( left_x, goal_area_top_y );
        s_lines.lineTo( left_x + ServerParam::DEFAULT_GOAL_AREA_LENGTH, goal_area_top_y );
        s_lines.lineTo( left_x + ServerParam::DEFAULT_GOAL_AREA_LENGTH, goal_area_bottom_y );
        s_lines.lineTo( left_x, goal_area_bottom_y );

        // right goal area
        s_lines.moveTo( right_x, goal_area_top_y );
        s_lines.lineTo( right_x - ServerParam::DEFAULT_GOAL_AREA_LENGTH, goal_area_top_y );
        s_lines.lineTo( right_x - ServerParam::DEFAULT_GOAL_AREA_LENGTH, goal_area_bottom_y );
        s_lines.lineTo( right_x, goal_area_bottom_y );*/
        //s_lines.addRect(2.225, -0.975, 0.800, 1.950);

    }


    painter.setPen( M_line_pen );
    painter.setBrush( Qt::NoBrush );
    painter.drawPath( s_lines );
    //painter.drawArc(QRectF(2.225, -0.975, 1.600, 1.0), 90*16, 90*16);


#ifndef LARGE_FIELD
    QRectF leftPenalty, rightPenalty;
    leftPenalty.setRect(-3.025, -0.975, 0.800, 1.950);
    rightPenalty.setRect(2.225, -0.975, 0.800, 1.950);
#else
    QRectF leftPenalty, rightPenalty;
    leftPenalty.setRect(-_FIELD_WIDTH/2.0, -_FIELD_PENALTY/2.0, _GOAL_RAD, _FIELD_PENALTY);
    rightPenalty.setRect(_FIELD_WIDTH/2.0 - _GOAL_RAD, -_FIELD_PENALTY/2.0, _GOAL_RAD, _FIELD_PENALTY);
#endif

    drawArc(painter, rightPenalty.right(), -(rightPenalty.bottom()-rightPenalty.width()), rightPenalty.width(), 90, 180);
    drawArc(painter, rightPenalty.right(), -1*(rightPenalty.top()+rightPenalty.width()), rightPenalty.width(), 180, 270);
    painter.drawLine(QPointF(rightPenalty.right()-rightPenalty.width(), rightPenalty.bottom()-rightPenalty.width()), QPointF(rightPenalty.right()-rightPenalty.width(), rightPenalty.top()+rightPenalty.width() ));

    //    glCallList(drawArc(leftPenalty.left(), -1*(leftPenalty.bottom()-leftPenalty.width()), leftPenalty.width(), 0, 90));
    //    glCallList(drawArc(leftPenalty.left(), -1*(leftPenalty.top()+leftPenalty.width()), leftPenalty.width(), 90, 180));
    //    glCallList(drawLine(leftPenalty.left()+leftPenalty.width(), leftPenalty.bottom()-leftPenalty.width(), field.left()+leftPenalty.width(), leftPenalty.top()+leftPenalty.width() ));


    drawArc(painter, leftPenalty.left(), -(leftPenalty.bottom()-leftPenalty.width()), leftPenalty.width(), 0, 90);
    drawArc(painter, leftPenalty.left(), -1*(leftPenalty.top()+leftPenalty.width()), leftPenalty.width(), 270, 360);
    painter.drawLine(QPointF(leftPenalty.left()+leftPenalty.width(), leftPenalty.bottom()-leftPenalty.width()), QPointF(leftPenalty.left()+leftPenalty.width(), leftPenalty.top()+leftPenalty.width() ));



    //
    // goals
    //

    painter.setPen( Qt::NoPen );

    // left goal
    painter.fillRect( QRectF( left_x - _GOAL_DEPTH,
                              - _GOAL_WIDTH * 0.5, //goal width
                              _GOAL_DEPTH,
                              _GOAL_WIDTH ),
                              Qt::black );
    // right goal
    painter.fillRect( QRectF( right_x,
                              - _GOAL_WIDTH * 0.5,
                              _GOAL_DEPTH,
                              _GOAL_WIDTH ),
                              Qt::black );


    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, true );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawContainedArea( QPainter & painter )
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return ;
    }

    boost::shared_ptr< const Formation > f = ptr->formation();
    if ( ! f )
    {
        return;
    }

    //if ( ptr->triangulation().indexedVertices().size() < 3 )
    if ( ptr->triangulation().triangles().empty() )
    {
        return;
    }

    //std::cerr << "triangle size = " << ptr->triangulation().triangleMap().size()
    //          << std::endl;

    const Triangulation::Triangle * tri = ptr->triangulation().findTriangleContains( ptr->state().ball_ );

    if ( ! tri )
    {
        return;
    }

    const Triangulation::PointCont & points = ptr->triangulation().points();
    const Vector2D vertex_0 = points.at( tri->v0_ );
    const Vector2D vertex_1 = points.at( tri->v1_ );
    const Vector2D vertex_2 = points.at( tri->v2_ );

    const QPointF vertices[3] = {
        QPointF( vertex_0.x, vertex_0.y ),
        QPointF( vertex_1.x, vertex_1.y ),
        QPointF( vertex_2.x, vertex_2.y )
    };

    painter.setPen( Qt::NoPen );
    painter.setBrush( M_field_dark_brush );
    painter.drawConvexPolygon( vertices, 3 );

    // draw center point
    Vector2D center = Triangle2D::centroid( vertex_0, vertex_1, vertex_2 );
    const double pix = M_transform.inverted().map( QLineF( 0.0, 0.0, 1.0, 0.0 ) ).length();

    painter.setPen( Qt::red );
    painter.setBrush( Qt::red );
    painter.drawRect( QRectF( center.x - pix, center.y - pix,
                              pix*2.0, pix*2.0 ) );
    //painter.drawPoint( QPointF( center.x, center.y ) );

    // draw the triangule's circumcircle
    if ( Options::instance().showCircumcircle() )
    {
        const Vector2D circumcenter = Triangle2D::circumcenter( vertex_0, vertex_1, vertex_2 );
        double x = circumcenter.x;
        double y = circumcenter.y;
        double r = circumcenter.dist( vertex_0 );

        painter.setPen( Qt::cyan );
        painter.setBrush( Qt::cyan );
        painter.drawRect( QRectF( x - pix, y - pix, pix * 2.0, pix * 2.0 ) );
        //painter.drawPoint( QPointF( x, y ) );

        painter.setBrush( Qt::NoBrush );
        painter.drawEllipse( x - r, y - r,
                             r * 2.0, r * 2.0 );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawData( QPainter & painter )
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return;
    }

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, false );
    }

    //
    // draw triangulation or vertices
    //

    if ( Options::instance().showTriangulation() )
    {
        const Triangulation::PointCont & points = ptr->triangulation().points();

        // triangulation

        painter.setPen( M_triangle_pen );
        painter.setBrush( Qt::NoBrush );

        const Triangulation::SegmentCont::const_iterator e_end = ptr->triangulation().edges().end();
        for ( Triangulation::SegmentCont::const_iterator e = ptr->triangulation().edges().begin();
              e != e_end;
              ++e )
        {
            painter.drawLine( QLineF( points[e->first].x,
                                      points[e->first].y,
                                      points[e->second].x,
                                      points[e->second].y ) );
        }

        // constraint edges
        painter.setPen( M_constraint_pen );
        painter.setBrush( Qt::NoBrush );

        const Triangulation::SegmentSet::const_iterator c_end = ptr->triangulation().constraints().end();
        for ( Triangulation::SegmentSet::const_iterator c = ptr->triangulation().constraints().begin();
              c != c_end;
              ++c )
        {
            painter.drawLine( QLineF( points[c->first].x,
                                      points[c->first].y,
                                      points[c->second].x,
                                      points[c->second].y ) );

        }
    }
    else
    {
        // only vertices

        const double r = M_transform.inverted().map( QLineF( 0.0, 0.0, 1.0, 0.0 ) ).length();
        const double d = r * 2.0;

        painter.setPen( M_triangle_pen );
        painter.setBrush( Qt::NoBrush );

        const SampleDataSet::DataCont::const_iterator end = ptr->samples()->dataCont().end();
        for ( SampleDataSet::DataCont::const_iterator it = ptr->samples()->dataCont().begin();
              it != end;
              ++it )
        {
            painter.drawRect( QRectF( it->ball_.x - r, it->ball_.y - r, d, d ) );
        }
    }

    //
    // draw index of vertices
    //

    if ( Options::instance().showIndex() )
    {
        painter.setPen( Qt::red );
        painter.setFont( M_player_font );

        const QTransform transform = painter.worldTransform();
        painter.setWorldMatrixEnabled( false );

        int count = 1;
        const SampleDataSet::DataCont::const_iterator d_end = ptr->samples()->dataCont().end();
        for ( SampleDataSet::DataCont::const_iterator it = ptr->samples()->dataCont().begin();
              it != d_end;
              ++it, ++count )
        {
            painter.drawText( transform.map( QPointF( it->ball_.x + 0.1, it->ball_.y - 0.1 ) ),
                              QString::number( count ) );
        }

        painter.setWorldMatrixEnabled( true );
    }

    //
    // draw selected sample
    //

    if ( 0 <= ptr->currentIndex() )
    {
        SampleDataSet::DataCont::const_iterator it = ptr->samples()->dataCont().begin();
        std::advance( it, ptr->currentIndex() );

        painter.setPen( Qt::yellow );
        painter.setBrush( Qt::NoBrush );

        //painter.drawEllipse( QRectF( it->ball_.x - 1.0, it->ball_.y - 1.0, 2.0, 2.0 ) );
        painter.drawRect( QRectF( it->ball_.x - 0.05, it->ball_.y - 0.05, 0.1, 0.1 ) );
    }

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, true );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawPlayers( QPainter & painter )
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return;
    }

    Formation::ConstPtr f = ptr->formation();
    if ( ! f )
    {
        return;
    }

    painter.setFont( M_player_font );

    const double r = 0.09; // body radius
    const double d = r * 2.0;   // body diameter

    const Vector2D bpos = ptr->state().ball_;

    const QTransform transform = painter.worldTransform();

    const std::vector< Vector2D >::const_iterator s
            = ( ptr->selectType() == EditData::SELECT_PLAYER
                ? ( ptr->state().players_.begin() + ptr->selectIndex() )
                : ptr->state().players_.end() );

    int unum = 1;
    const std::vector< Vector2D >::const_iterator p_end = ptr->state().players_.end();
    for ( std::vector< Vector2D >::const_iterator p = ptr->state().players_.begin();
          p != p_end && unum < 7;
          ++p, ++unum )
    {
        if ( p == s )
        {
            painter.setPen( M_select_pen );
            painter.setBrush( f->isSymmetryType( unum )
                              ? M_symmetry_brush
                              : M_player_brush );
            painter.drawEllipse( QRectF( p->x - r, p->y - r ,
                                         d, d )  );

            QPen pen = painter.pen();
            pen.setColor(Qt::darkMagenta);
            painter.setPen(pen);

            AngleDeg theta( asin(r/p->dist(bpos)) * 180.0 / M_PI );
            AngleDeg dir = ((*p)-bpos).dir();
            Vector2D left = bpos + Vector2D::polar2vector(10,dir-theta);
            Vector2D right = bpos + Vector2D::polar2vector(10,dir+theta);
            painter.drawLine(QPointF(bpos.x,bpos.y),QPointF(left.x,left.y));
            painter.drawLine(QPointF(bpos.x,bpos.y),QPointF(right.x,right.y));
        }
        else
        {
            painter.setPen( M_player_pen );
            painter.setBrush( f->isSymmetryType( unum )
                              ? M_symmetry_brush
                              : M_player_brush );
            painter.drawEllipse( QRectF( p->x - r, p->y - r, d, d ) );
        }

        painter.setPen( Qt::white );
        painter.setWorldMatrixEnabled( false );
        painter.drawText( transform.map( QPointF( p->x + r, p->y ) ),
                          QString::number( unum ) );
        painter.setWorldMatrixEnabled( true );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawBall( QPainter & painter )
{

    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return;
    }

    painter.setPen( M_ball_pen );
    painter.setBrush( M_ball_brush );


    const Vector2D bpos = ptr->state().ball_;
    const bool enlarge = Options::instance().enlarge();
    const double r = 0.05;
    double kr = 0.0175;
    double rr = enlarge ? r : kr;

    painter.drawEllipse( QRectF( bpos.x - rr, bpos.y - rr,
                                 rr * 2.0, rr * 2.0 ) );

    QPen pen = painter.pen();
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    QBrush brush = painter.brush();
    brush.setStyle(Qt::NoBrush);
    painter.setBrush(brush);
    painter.drawEllipse( QRectF( bpos.x - .5 , bpos.y - .5 , 1 , 1 ) );
    painter.drawLine(QPointF(bpos.x,bpos.y),QPointF(-FieldWidth/2,TopGoal));
    painter.drawLine(QPointF(bpos.x,bpos.y),QPointF(-FieldWidth/2,ButtonGoal));
    painter.drawLine(QPointF(bpos.x,bpos.y),QPointF(-FieldWidth/2,0));
}


/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawConstraintSelection( QPainter & painter )
{

    const float eclipseRad = 0.10;
    const float rectHalfSide = 0.10;

    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return;
    }

    if ( ptr->selectType() != EditData::SELECT_SAMPLE )
    {
        return;
    }

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, false );
    }

    SampleDataSet::DataCont::const_iterator it = ptr->samples()->dataCont().begin();
    std::advance( it, ptr->constraintOriginIndex() );

    painter.setPen( Qt::blue );
    painter.setBrush( Qt::blue );

    painter.drawEllipse( QRectF( it->ball_.x - eclipseRad, it->ball_.y - eclipseRad, eclipseRad*2, eclipseRad*2 ) );

    if ( ptr->constraintTerminal().isValid() )
    {
        painter.drawRect( QRectF( ptr->constraintTerminal().x - rectHalfSide, ptr->constraintTerminal().y - rectHalfSide,
                                  rectHalfSide*2, rectHalfSide*2 ) );

        painter.setBrush( Qt::NoBrush );
        painter.drawLine( QLineF( it->ball_.x, it->ball_.y,
                                  ptr->constraintTerminal().x, ptr->constraintTerminal().y ) );
    }


    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, true );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawBackgroundContainedArea( QPainter & painter )
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return ;
    }

    if ( ptr->backgroundTriangulation().triangles().empty() )
    {
        return;
    }

    const Triangulation::Triangle * tri = ptr->backgroundTriangulation().findTriangleContains( ptr->state().ball_ );

    if ( ! tri )
    {
        return;
    }

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, false );
    }

    const Triangulation::PointCont & points = ptr->backgroundTriangulation().points();
    const Vector2D vertex_0 = points.at( tri->v0_ );
    const Vector2D vertex_1 = points.at( tri->v1_ );
    const Vector2D vertex_2 = points.at( tri->v2_ );

    const QPointF vertices[3] = {
        QPointF( vertex_0.x, vertex_0.y ),
        QPointF( vertex_1.x, vertex_1.y ),
        QPointF( vertex_2.x, vertex_2.y )
    };

    painter.setPen( Qt::NoPen );
    painter.setBrush( M_background_contained_area_brush );
    painter.drawConvexPolygon( vertices, 3 );

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, true );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawBackgroundData( QPainter & painter )
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return;
    }

    if ( ptr->backgroundTriangulation().triangles().empty() )
    {
        // too few kernel points
        // no valid triangulation
        return;
    }

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, false );
    }

    if ( Options::instance().showTriangulation() )
    {

        const Triangulation::PointCont & points = ptr->backgroundTriangulation().points();

        // triangulation

        painter.setPen( M_background_triangle_pen );
        painter.setBrush( Qt::NoBrush );

        const Triangulation::SegmentCont::const_iterator e_end = ptr->backgroundTriangulation().edges().end();
        for ( Triangulation::SegmentCont::const_iterator e = ptr->backgroundTriangulation().edges().begin();
              e != e_end;
              ++e )
        {
            painter.drawLine( QLineF( points[e->first].x,
                                      points[e->first].y,
                                      points[e->second].x,
                                      points[e->second].y ) );
        }
    }
    else
    {
        // only vertices

        const double r = M_transform.inverted().map( QLineF( 0.0, 0.0, 1.0, 0.0 ) ).length();
        const double d = r * 2.0;

        painter.setPen( M_triangle_pen );
        painter.setBrush( Qt::NoBrush );

        const SampleDataSet::DataCont::const_iterator d_end = ptr->backgroundFormation()->samples()->dataCont().end();
        for ( SampleDataSet::DataCont::const_iterator it = ptr->backgroundFormation()->samples()->dataCont().begin();
              it != d_end;
              ++it )
        {
            painter.drawRect( QRectF( it->ball_.x - r, it->ball_.y - r, d, d ) );
        }
    }

    //
    // index number
    //
    if ( Options::instance().showIndex() )
    {
        painter.setFont( M_player_font );
        painter.setPen( M_background_font_pen );

        const QTransform transform = painter.worldTransform();
        painter.setWorldMatrixEnabled( false );

        int count = 1;
        const SampleDataSet::DataCont::const_iterator d_end = ptr->backgroundFormation()->samples()->dataCont().end();
        for ( SampleDataSet::DataCont::const_iterator it = ptr->backgroundFormation()->samples()->dataCont().begin();
              it != d_end;
              ++it, ++count )
        {
            painter.drawText( transform.map( QPointF( it->ball_.x + 0.1, it->ball_.y - 0.1 ) ),
                              QString::number( count ) );
        }

        painter.setWorldMatrixEnabled( true );
    }

    if ( Options::instance().antialiasing() )
    {
        setAntialiasFlag( painter, true );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::drawBackgroundPlayers( QPainter & painter )
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return;
    }

    Formation::ConstPtr f = ptr->backgroundFormation();
    if ( ! f )
    {
        return;
    }

    painter.setFont( M_player_font );

    //    const bool enlarge = Options::instance().enlarge();
    const double r = 0.09;
    const int d = r * 2.0;

    const QTransform transform = painter.worldTransform();

    std::vector< Vector2D > players;
    players.reserve( 11 );
    f->getPositions( ptr->state().ball_, players );

    int unum = 1;
    for ( std::vector< Vector2D >::const_iterator p = players.begin();
          p != players.end();
          ++p, ++unum )
    {
        if ( f->isSymmetryType( unum ) )
        {
            painter.setPen( M_background_player_pen );
            painter.setBrush( M_background_symmetry_brush );
            painter.drawEllipse( QRectF( p->x - r, p->y - r, d, d ) );
        }
        else
        {
            painter.setPen( M_background_player_pen );
            painter.setBrush( M_background_left_team_brush );
            painter.drawEllipse( QRectF( p->x - r, p->y - r, d, d ) );
        }

        painter.setPen( Qt::gray );
        painter.setWorldMatrixEnabled( false );
        painter.drawText( transform.map( QPointF( p->x + r, p->y ) ),
                          QString::number( unum ) );
        painter.setWorldMatrixEnabled( true );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::setFocusPoint( const QPoint & pos )
{
    QPointF p = M_transform.inverted().map( QPointF( pos ) );
    p.setX( qBound( -_FIELD_WIDTH,
                    p.x(),
                    _FIELD_WIDTH ) );
    p.setY( qBound( -_FIELD_HEIGHT,
                    p.y(),
                    _FIELD_HEIGHT ) );

    double s = M_transform.map( QLineF( 0.0, 0.0, 1, 0.0 ) ).length();

    M_transform.reset();
    M_transform.translate( this->width() * 0.5 - p.x() * s,
                           this->height() * 0.5 - p.y() * s );
    M_transform.scale( s, s );

    Options::instance().setAutoFitMode( false );

    this->update();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::mouseDoubleClickEvent( QMouseEvent * event )
{
    if ( event->button() == Qt::LeftButton )
    {
        setFocusPoint( event->pos() );
        event->accept();
        return;
    }

    QWidget::mouseDoubleClickEvent( event );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::mousePressEvent( QMouseEvent * event )
{

    if ( event->button() == Qt::LeftButton )
    {
        M_mouse_state[0].pressed( event->pos() );


        if ( event->modifiers() == 0 )
        {
            if ( boost::shared_ptr< EditData > ptr = M_edit_data.lock() )
            {
                QPointF field_pos = M_transform.inverted().map( QPointF( event->pos() ) );
                if ( ptr->selectObject( field_pos.x(), field_pos.y() ) )
                {
                    this->update();
                }
            }
        }
    }
    else if ( event->button() == Qt::MidButton )
    {
        M_mouse_state[1].pressed( event->pos() );
    }
    else if ( event->button() == Qt::RightButton )
    {
        M_mouse_state[2].pressed( event->pos() );

        if ( event->modifiers() == 0 )
        {
            if ( boost::shared_ptr< EditData > ptr = M_edit_data.lock() )
            {
                QPointF field_pos = M_transform.inverted().map( QPointF( event->pos() ) );
                ptr->moveBallTo( field_pos.x(), field_pos.y() );
                this->update();
                emit objectMoved();
            }
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::mouseReleaseEvent( QMouseEvent * event )
{
    if ( event->button() == Qt::LeftButton )
    {
        M_mouse_state[0].released();
        if ( M_mouse_state[0].isMenuFailed() )
        {
            M_mouse_state[0].setMenuFailed( false );
        }

        if ( boost::shared_ptr< EditData > ptr = M_edit_data.lock() )
        {
            if ( ptr->selectType() == EditData::SELECT_SAMPLE
                 && ptr->constraintTerminalIndex() != size_t( -1 ) )
            {
                emit constraintSelected( ptr->constraintOriginIndex(), ptr->constraintTerminalIndex() );
            }
            else if ( ptr->releaseObject() )
            {
                this->update();
            }
        }
    }
    else if ( event->button() == Qt::MidButton )
    {
        M_mouse_state[1].released();
        if ( M_mouse_state[1].isMenuFailed() )
        {
            M_mouse_state[1].setMenuFailed( false );
        }
    }
    else if ( event->button() == Qt::RightButton )
    {
        M_mouse_state[2].released();
        if ( M_mouse_state[2].isMenuFailed() )
        {
            M_mouse_state[2].setMenuFailed( false );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::mouseMoveEvent( QMouseEvent * event )
{
    QPointF field_pos = M_transform.inverted().map( QPointF( event->pos() ) );

    if ( M_mouse_state[0].isDragged() )
    {
        if ( event->modifiers() == 0 )
        {
            boost::shared_ptr< EditData > ptr = M_edit_data.lock();
            if ( ptr
                 && ptr->moveSelectObjectTo( field_pos.x(), field_pos.y() ) )
            {
                emit objectMoved();
                this->update();
            }
        }
        else if ( event->modifiers() & Qt::ControlModifier )
        {
            //             QPointF diff = field_pos - invert.map( QPointF( M_mouse_state[0].draggedPoint() ) );
            //             M_transform.translate( diff.x(), diff.y() );
            //             Options::instance().setAutoFitMode( false );
            //             this->update();
            QPoint focus_point( this->width() / 2, this->height() / 2 );
            focus_point -= ( event->pos() - M_mouse_state[0].draggedPoint() );
            setFocusPoint( focus_point );
        }
    }

    for ( int i = 0; i < 3; ++i )
    {
        M_mouse_state[i].moved( event->pos() );
    }

    event->ignore();
    emit mouseMoved( field_pos );
}


/*-------------------------------------------------------------------*/
/*!

 */

void
EditCanvas::wheelEvent(QWheelEvent *event) {
//    if (event->delta() > 0) zoomIn();
//    else if (event->delta() < 0) zoomOut();
//    else return;
}


/*-------------------------------------------------------------------*/
/*!

 */

void
EditCanvas::resizeEvent(QResizeEvent *) {
    fitToScreen();
}

/*-------------------------------------------------------------------*/
/*!

 */



void
EditCanvas::zoomIn()
{
    if ( M_transform.map( QLineF( 0.0, 0.0, 1.0, 0.0 ) ).length() * 1.5 > 700.0 )
    {
        return;
    }

    Options::instance().setAutoFitMode( false );
    M_transform.scale( 1.5, 1.5 );

    this->update();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::zoomOut()
{
    if ( M_transform.map( QLineF( 0.0, 0.0, 1.0, 0.0 ) ).length() / 1.5 < 2.0 )
    {
        return;
    }

    Options::instance().setAutoFitMode( false );
    M_transform.scale( 1.0/1.5, 1.0/1.5 );

    this->update();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditCanvas::fitToScreen()
{
    Options::instance().setAutoFitMode( true );

    double scale_w = this->width() / ( ServerParam::DEFAULT_PITCH_LENGTH + 10.0 );
    double scale_h =  this->height() / ( ServerParam::DEFAULT_PITCH_WIDTH + 10.0 );
    double scale_factor = qMin( scale_w, scale_h );

    M_transform.reset();
    M_transform.translate( this->width() * 0.5, this->height() * 0.5 );
    M_transform.scale( scale_factor, scale_factor );

    this->update();
}
