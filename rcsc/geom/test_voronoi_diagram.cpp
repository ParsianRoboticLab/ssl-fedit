// -*-c++-*-

/*!
  \file test_voronoi_diagram.cpp
  \brief test code for rcsc::VoronoiDiagram
*/

/*
 *Copyright:

 Copyright (C) Hiroki Shimora

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */

#ifdef HAVE_CONFIG
#include <config.h>
#endif

#include "voronoi_diagram.h"

#include <rcsc/geom/vector_2d.h>

#include <cppunit/extensions/HelperMacros.h>

class VoronoiDiagramTest
    : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE( VoronoiDiagramTest );
    CPPUNIT_TEST( testEmpty );
    CPPUNIT_TEST( testVoronoi );
    CPPUNIT_TEST_SUITE_END();

public:

    void testEmpty();
    void testVoronoi();
};


CPPUNIT_TEST_SUITE_REGISTRATION( VoronoiDiagramTest );


/*-------------------------------------------------------------------*/
/*!

 */
void
VoronoiDiagramTest::testEmpty()
{
    rcsc::VoronoiDiagram v;

    CPPUNIT_ASSERT_EQUAL( static_cast< size_t >( 0 ), v.resultPoints().size() );
    CPPUNIT_ASSERT_EQUAL( static_cast< size_t >( 0 ), v.resultSegments().size() );
    CPPUNIT_ASSERT_EQUAL( static_cast< size_t >( 0 ), v.resultRays().size() );

    v.compute();

    CPPUNIT_ASSERT_EQUAL( static_cast< size_t >( 0 ), v.resultPoints().size() );
    CPPUNIT_ASSERT_EQUAL( static_cast< size_t >( 0 ), v.resultSegments().size() );
    CPPUNIT_ASSERT_EQUAL( static_cast< size_t >( 0 ), v.resultRays().size() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
VoronoiDiagramTest::testVoronoi()
{
    const rcsc::Vector2D p0(   0.0,   0.0 );
    const rcsc::Vector2D p1( +10.0, +10.0 );
    const rcsc::Vector2D p2( -10.0, +10.0 );
    const rcsc::Vector2D p3( -10.0, -10.0 );
    const rcsc::Vector2D p4( +10.0, -10.0 );

    const rcsc::Vector2D p5( +20.0,   0.0 );
    const rcsc::Vector2D p6(   0.0, +20.0 );
    const rcsc::Vector2D p7( -20.0,   0.0 );
    const rcsc::Vector2D p8(   0.0, -20.0 );

    //
    // input points
    //

    //                               //
    //                 |             //
    // +20             *p6           //
    //                 |             //
    //                 |             //
    //           p2    |    p1       //
    // +10        *    |    *        //
    //                 |             //
    //                 |             //
    //                 |             //
    //      p7         |             //
    //   0 --*---------*---------*-- //
    //                 |p0       p5  //
    //                 |             //
    //                 |             //
    //                 |             //
    // -10        *    |    *        //
    //           p3    |    p4       //
    //                 |             //
    //                 |             //
    //                 |             //
    // -20             *p8           //
    //                 |             //
    //                               //
    //      -20  -10   0   +10  +20  //


    rcsc::VoronoiDiagram v;

    v.addPoint( p0 );
    v.addPoint( p1 );
    v.addPoint( p2 );
    v.addPoint( p3 );
    v.addPoint( p4 );
    v.addPoint( p5 );
    v.addPoint( p6 );
    v.addPoint( p7 );
    v.addPoint( p8 );

    v.compute();


    //
    // result
    //

    //           \           /        //
    //            \    |    /         //
    // +20         \   *p6 /          //
    //              \  |  /           //
    //     \         \ | /         /  //
    //      \     p2  \|/   p1    /   //
    // +10   \     *   .    *    /    //
    //        \       /|\       /     //
    //         \     / | \     /      //
    //          \   /  |  \   /       //
    //      p7   \ /   |   \ /        //
    //   0 --*----.----*----.----*--  //
    //           / \   |p0 / \    p5  //
    //          /   \  |  /   \       //
    //         /     \ | /     \      //
    //        /       \|/       \     //
    // -10   /    *    .    *    \    //
    //      /    p3   /|\    p4   \   //
    //     /         / | \         \  //
    //              /  |  \           //
    //             /   |   \          //
    // -20        /    *p8  \         //
    //           /     |     \        //
    //          /             \       //
    //      -20  -10   0   +10  +20   //


    //
    // check points
    //
    int n_points = 0;
    const rcsc::VoronoiDiagram::Vector2DCont::const_iterator p_end = v.resultPoints().end();
    for ( rcsc::VoronoiDiagram::Vector2DCont::const_iterator p = v.resultPoints().begin();
          p != p_end;
          ++p )
    {
        n_points ++;

#ifdef DEBUG_PRINT
        std::cout << "point " << n_points << ": " << *p << std::endl;
#endif
    }


    //
    // check segments
    //
    int n_segments = 0;
    const rcsc::VoronoiDiagram::Segment2DCont::const_iterator s_end = v.resultSegments().end();
    for ( rcsc::VoronoiDiagram::Segment2DCont::const_iterator s = v.resultSegments().begin();
          s != s_end;
          ++s )
    {
        n_segments ++;

#ifdef DEBUG_PRINT
        std::cout << "segment " << n_segments << ": "
                  << (*s).origin() << ", "
                  << (*s).terminal() << std::endl;
#endif
    }
    CPPUNIT_ASSERT_EQUAL( 4, n_segments );


    //
    // check rays
    //
    int n_rays = 0;
    const rcsc::VoronoiDiagram::Ray2DCont::const_iterator r_end = v.resultRays().end();
    for ( rcsc::VoronoiDiagram::Ray2DCont::const_iterator r = v.resultRays().begin();
          r != r_end;
          ++r )
    {
        n_rays ++;
#ifdef DEBUG_PRINT
        std::cout << "ray " << n_rays << ": "
                  << "origin = " << (*r).origin() << ", "
                  << "dir = " << (*r).dir() << std::endl;
#endif
    }
    CPPUNIT_ASSERT_EQUAL( 8, n_rays );
}


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

#include <cppunit/BriefTestProgressListener.h>
//#include <cppunit/TextTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
//#include <cppunit/TextOutputter.h>
//#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

int
main( int, char ** )
{
    // create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    // add a listner that collects test results
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener( &result );

    // add a listener that prints dots as test run.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener( &progress );

    //CPPUNIT_NS::TextTestProgressListener textprog;
    //controller.addListener( &textprog );

    // add the top suite to the test runner.
    CPPUNIT_NS::TestRunner runner;
    runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    runner.run( controller );

    // output results in a compiler compatible format
    CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
    //CPPUNIT_NS::TextOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
    //CPPUNIT_NS::XmlOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
    outputter.write();

    return result.wasSuccessful() ? 0 : 1;
}
