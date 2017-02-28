// -*-c++-*-

/*!
  \file global_world_model.cpp
  \brief noiseless world model class Source File
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

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

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "global_world_model.h"

#include "global_visual_sensor.h"

#include <rcsc/common/server_param.h>
#include <rcsc/common/player_param.h>
#include <rcsc/common/player_type.h>
#include <rcsc/geom/rect_2d.h>

namespace rcsc {

/*-------------------------------------------------------------------*/
/*!

*/
GlobalWorldModel::GlobalWorldModel()
    : M_client_version( 0 ),
      M_time( -1, 0 ),
      M_see_time( -1, 0 ),
      M_our_side( NEUTRAL ),
      M_training_time( -1, 0 ),
      M_player_type_analyzer( *this ),
      M_change_count_left( 0 ),
      M_change_count_right( 0 ),
      M_last_playon_start( 0 ),
      M_freeform_allowed_count( ServerParam::i().coachSayCountMax() ),
      M_freeform_send_count( 0 )
{
    for ( int i = 0; i < 11; i++ )
    {
        M_player_types_left[i] = Hetero_Default;
        M_player_types_right[i] = Hetero_Default;
    }

    M_player_type_used_count_left.push_back( 11 );
    M_player_type_used_count_right.push_back( 11 );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::init( const SideID side,
                        const int client_version )
{
    M_our_side = side;
    M_client_version = client_version;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::setAudioMemory( boost::shared_ptr< AudioMemory > memory )
{
    M_audio_memory = memory;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::initFreeformCount()
{
    M_freeform_allowed_count = ServerParam::i().coachSayCountMax();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::setTeamName( const SideID side,
                               const std::string & name )
{
    if ( name.empty() )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " *** ERROR *** Empty team name "
                  << std::endl;
    }

    if ( side == LEFT )
    {
        M_team_name_left = name;
    }
    else if ( side == RIGHT )
    {
        M_team_name_right = name;
    }
    else
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " *** ERROR *** Invalid side = " << side
                  << std::endl;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::setPlayerType( const SideID side,
                                 const int unum,
                                 const int type )
{
    if ( side == NEUTRAL )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Illegal side = " << side
                  << std::endl;
        return;
    }

    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Illegal unum " << unum
                  << std::endl;
        return;
    }

    if ( type != Hetero_Unknown
         && ( type < Hetero_Default
              || PlayerParam::i().playerTypes() <= type ) )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Illegal player type id " << type
                  << std::endl;
        return;
    }

    if ( side == LEFT )
    {
        M_player_types_left[unum - 1] = type;

        if ( this->time().cycle() > 0 )
        {
            ++M_change_count_left;
        }

        M_player_type_used_count_left.assign( PlayerParam::i().playerTypes(), 0 );
        for ( int i = 0; i < 11; ++i )
        {
            if ( M_player_types_left[i] != Hetero_Unknown )
            {
                M_player_type_used_count_left[ M_player_types_left[i] ] += 1;
            }
        }
    }
    else
    {
        M_player_types_right[unum - 1] = type;

        if ( this->time().cycle() > 0 )
        {
            ++M_change_count_right;
        }

        M_player_type_used_count_right.assign( PlayerParam::i().playerTypes(), 0 );
        for ( int i = 0; i < 11; ++i )
        {
            if ( M_player_types_right[i] != Hetero_Unknown )
            {
                M_player_type_used_count_right[ M_player_types_right[i] ] += 1;
            }
        }
    }

    if ( side != ourSide()
         && type == Hetero_Unknown )
    {
        M_player_type_analyzer.reset( unum );
    }

}


/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::setYellowCard( const SideID side,
                                 const int unum )
{
    if ( unum < 1 || 11 < unum )
    {
        std::cerr << ourTeamName() << " coach:"
                  << " ***ERROR*** (GlobalWorldModel::setYellowCard) "
                  << " Illegal uniform number"
                  << unum << std::endl;
        return;
    }

    if ( side == LEFT )
    {
        M_yellow_card_left[unum - 1] = true;
    }
    else if ( side == RIGHT )
    {
        M_yellow_card_right[unum - 1] = true;
    }
    else
    {
        std::cerr << ourTeamName() << " coach:"
                  << " ***ERROR*** (GlobalWorldModel::setYellowCard) "
                  << " Illegal side" << std::endl;
        return;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::setRedCard( const SideID side,
                              const int unum )
{
    if ( unum < 1 || 11 < unum )
    {
        std::cerr << ourTeamName() << " coach:"
                  << " ***ERROR*** (GlobalWorldModel::setRedCard) "
                  << " Illegal uniform number"
                  << unum << std::endl;
        return;
    }

    if ( side == LEFT )
    {
        M_red_card_left[unum - 1] = true;
    }
    else if ( side == RIGHT )
    {
        M_red_card_right[unum - 1] = true;
    }
    else
    {
        std::cerr << ourTeamName() << " coach:"
                  << " ***ERROR*** (GlobalWorldModel::setRedCard) "
                  << " Illegal side" << std::endl;
        return;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::updateGameMode( const GameMode & game_mode,
                                  const GameTime & current )
{
    if ( M_game_mode.type() != GameMode::PlayOn
         && game_mode.type() == GameMode::PlayOn )
    {
        M_last_playon_start = current.cycle();
    }

    M_time = current;
    M_game_mode = game_mode;

    if ( game_mode.type() == GameMode::BeforeKickOff )
    {
        M_change_count_left = 0;
        M_change_count_right = 0;
    }

    //
    // update freeform capacity
    //
    if ( game_mode.type() != GameMode::BeforeKickOff
         && game_mode.type() != GameMode::TimeOver
         && game_mode.type() != GameMode::AfterGoal_
         && game_mode.type() != GameMode::OffSide_
         && game_mode.type() != GameMode::BackPass_
         && game_mode.type() != GameMode::FreeKickFault_
         && game_mode.type() != GameMode::CatchFault_ )
    {
        if ( current.cycle() > 0
             && ServerParam::i().nrNormalHalfs() > 0
             && current.cycle() % ( ServerParam::i().nrNormalHalfs()
                                    * ServerParam::i().halfTime() ) == 0 )
        {
            std::cerr << ourTeamName() << " coach: "
                      << current
                      << " new freeform allowed"
                      << std::endl;
            M_freeform_allowed_count += ServerParam::i().coachSayCountMax();
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GlobalWorldModel::updateAfterSeeGlobal( const GlobalVisualSensor & see_global,
                                        const GameTime & current )
{
    if ( M_see_time.cycle() != current.cycle() - 1
         && M_see_time.stopped() != current.stopped() - 1 )
    {
        // missed cycles??
        if ( M_see_time.cycle() == current.cycle()
             && M_see_time.stopped() > 0
             && current.stopped() == 0 )
        {
            // back from the stopped mode
        }
        else
        {
            std::cerr << __FILE__ << ' ' << __LINE__
                      << " missed cycles? last updated time = " << M_see_time
                      << " current = " << current
                      << std::endl;
        }
    }

    M_time = current;

    if ( M_see_time == current )
    {
        return;
    }

    M_see_time = current;

    //
    // update team names
    //

    if ( ! see_global.teamNameLeft().empty() )
    {
        M_team_name_left = see_global.teamNameLeft();
    }

    if ( ! see_global.teamNameRight().empty() )
    {
        M_team_name_right = see_global.teamNameRight();
    }

    //
    // update ball
    //

    M_ball = see_global.ball();

    //
    // update player instance variables
    //

    std::list< GlobalPlayerObject > new_players;

    const std::vector< GlobalPlayerObject >::const_iterator end = see_global.players().end();
    for ( std::vector< GlobalPlayerObject >::const_iterator it = see_global.players().begin();
          it != end;
          ++it )
    {
        bool found = false;
        std::list< GlobalPlayerObject >::iterator p = M_players.begin();
        while  ( p != M_players.end() )
        {
            if ( p->side() == it->side()
                 && p->unum() == it->unum() )
            {
                p->update( *it );

                new_players.splice( new_players.end(), M_players, p );
                found = true;
                break;
            }
            ++p;
        }

        if ( ! found )
        {
            new_players.push_back( *it );
        }
    }

    M_players.clear();
    M_players.splice( M_players.end(), new_players );

    //
    // update the vector of player's pointer
    //

    M_players_left.clear();
    M_players_right.clear();

    const std::list< GlobalPlayerObject >::iterator pend = M_players.end();
    for ( std::list< GlobalPlayerObject >::iterator p = M_players.begin();
          p != pend;
          ++p )
    {
        if ( p->side() == LEFT )
        {
            M_players_left.push_back( &(*p) );
        }
        else
        {
            M_players_right.push_back( &(*p) );
        }
    }

    //
    // analyze player type
    //

    M_player_type_analyzer.update();

    for ( int unum = 1; unum <= 11; ++unum )
    {
        int id = M_player_type_analyzer.heteroID( unum );
        if ( id != Hetero_Unknown )
        {
            if ( ourSide() == LEFT )
            {
                M_player_types_right[unum - 1] = id;
            }
            else
            {
                M_player_types_left[unum - 1] = id;
            }
        }
    }

    if ( ourSide() == RIGHT )
    {
        M_player_type_used_count_left.assign( PlayerParam::i().playerTypes(), 0 );
        for ( int i = 0; i < 11; ++i )
        {
            if ( M_player_types_left[i] != Hetero_Unknown )
            {
                M_player_type_used_count_left[ M_player_types_left[i] ] += 1;
            }
        }
    }
    else
    {
        M_player_type_used_count_right.assign( PlayerParam::i().playerTypes(), 0 );
        for ( int i = 0; i < 11; ++i )
        {
            if ( M_player_types_right[i] != Hetero_Unknown )
            {
                M_player_type_used_count_right[ M_player_types_right[i] ] += 1;
            }
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
int
GlobalWorldModel::heteroID( const SideID side,
                            const int unum ) const
{
    if ( side == NEUTRAL )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Invalid side = " << side
                  << std::endl;
        return Hetero_Unknown;
    }

    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Invalid unum = " << unum
                  << std::endl;
        return Hetero_Unknown;
    }

    return ( side == LEFT
             ? M_player_types_left[unum - 1]
             : M_player_types_right[unum - 1] );
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GlobalWorldModel::isYellowCarded( const SideID side,
                                  const int unum ) const
{
    if ( side == NEUTRAL )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Invalid side = " << side
                  << std::endl;
        return false;
    }

    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Invalid unum = " << unum
                  << std::endl;
        return false;
    }

    return ( side == LEFT
             ? M_yellow_card_left[unum - 1]
             : M_yellow_card_right[unum - 1] );
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GlobalWorldModel::isRedCarded( const SideID side,
                               const int unum ) const
{
    if ( side == NEUTRAL )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Invalid side = " << side
                  << std::endl;
        return false;
    }

    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " ***ERROR*** Invalid unum = " << unum
                  << std::endl;
        return false;
    }

    return ( side == LEFT
             ? M_red_card_left[unum - 1]
             : M_red_card_right[unum - 1] );
}

/*-------------------------------------------------------------------*/
/*!

*/
BallStatus
GlobalWorldModel::getBallStatus() const
{
    static const double WIDTH
        = ServerParam::i().goalHalfWidth()
        + ServerParam::DEFAULT_GOAL_POST_RADIUS;
    static const Rect2D GOAL_L( Vector2D( - ServerParam::DEFAULT_PITCH_LENGTH * 0.5
                                          - ServerParam::DEFAULT_GOAL_DEPTH
                                          - ServerParam::i().ballSize(),
                                          - WIDTH * 0.5 ),
                                Size2D( ServerParam::DEFAULT_GOAL_DEPTH,
                                        WIDTH ) );
    static const Rect2D GOAL_R( Vector2D( ServerParam::DEFAULT_PITCH_LENGTH * 0.5
                                          + ServerParam::i().ballSize(),
                                          - WIDTH * 0.5 ),
                                Size2D( ServerParam::DEFAULT_GOAL_DEPTH,
                                        WIDTH ) );
    static const Rect2D PITCH( Vector2D( - ServerParam::DEFAULT_PITCH_LENGTH * 0.5
                                         - ServerParam::i().ballSize() * 0.5,
                                         - ServerParam::DEFAULT_PITCH_WIDTH * 0.5
                                         - ServerParam::i().ballSize() * 0.5 ),
                               Size2D( ServerParam::DEFAULT_PITCH_LENGTH
                                       + ServerParam::i().ballSize(),
                                       ServerParam::DEFAULT_PITCH_WIDTH
                                       + ServerParam::i().ballSize() ) );


    if ( GOAL_L.contains( M_ball.pos() ) )
    {
        return Ball_GoalL;
    }

    if ( GOAL_R.contains( M_ball.pos() ) )
    {
        return Ball_GoalR;
    }

    if ( ! PITCH.contains( M_ball.pos() ) )
    {
        return Ball_OutOfField;
    }

    return Ball_InField;
}

/*-------------------------------------------------------------------*/
/*!

*/
const
GlobalPlayerObject *
GlobalWorldModel::teammate( const int unum ) const
{
    const std::vector< const GlobalPlayerObject * > & cont = teammates();

    const std::vector< const GlobalPlayerObject * >::const_iterator end = cont.end();
    for ( std::vector< const GlobalPlayerObject * >::const_iterator p = cont.begin();
          p != end;
          ++p )
    {
        if ( (*p)->unum() == unum )
        {
            return *p;
        }
    }

    return static_cast< const GlobalPlayerObject * >( 0 );
}

/*-------------------------------------------------------------------*/
/*!

*/
const
GlobalPlayerObject *
GlobalWorldModel::opponent( const int unum ) const
{
    const std::vector< const GlobalPlayerObject * > & cont = opponents();

    const std::vector< const GlobalPlayerObject * >::const_iterator end = cont.end();
    for ( std::vector< const GlobalPlayerObject * >::const_iterator p = cont.begin();
          p != end;
          ++p )
    {
        if ( (*p)->unum() == unum )
        {
            return *p;
        }
    }

    return static_cast< const GlobalPlayerObject * >( 0 );
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GlobalWorldModel::canSendFreeform() const
{
    if ( M_freeform_allowed_count >= 0
         && M_freeform_send_count >= M_freeform_allowed_count )
    {
        return false;
    }

    // if playmode is not playon, coach can send the message anytime.
    if ( gameMode().type() != GameMode::PlayOn )
    {
        return true;
    }

    // the period that coach can sent the freeform message is very restricted.

    long playon_period = time().cycle() - M_last_playon_start;

    if ( playon_period > ServerParam::i().freeformWaitPeriod() )
    {
        playon_period %= ServerParam::i().freeformWaitPeriod();
        return playon_period < ServerParam::i().freeformSendPeriod();
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GlobalWorldModel::existKickablePlayer() const
{

    const std::list< GlobalPlayerObject >::const_iterator end = M_players.end();
    for ( std::list< GlobalPlayerObject >::const_iterator it = M_players.begin();
          it != end;
          ++it )
    {
        int type = heteroID( it->side(), it->unum() );
        const PlayerType * param = PlayerTypeSet::i().get( type );
        double kickable_area = ( param
                                 ? param->kickableArea()
                                 : ServerParam::i().defaultKickableArea() );

        if ( it->pos().dist( M_ball.pos() ) < kickable_area )
        {
            return true;
        }
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
const
GlobalPlayerObject *
GlobalWorldModel::getPlayerNearestTo( const Vector2D & point ) const
{
    const GlobalPlayerObject * ptr = static_cast< GlobalPlayerObject * >( 0 );
    double max_dist2 = 200000.0;

    const std::list< GlobalPlayerObject >::const_iterator end = M_players.end();
    for ( std::list< GlobalPlayerObject >::const_iterator it = M_players.begin();
          it != end;
          ++it )
    {
        double d2 = it->pos().dist2( point );
        if ( d2 < max_dist2 )
        {
            max_dist2 = d2;
            ptr = &(*it);
        }
    }

    return ptr;
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream &
GlobalWorldModel::print( std::ostream & os ) const
{
    os << "coach world " << M_time << '\n';


    os << "Ball: " << M_ball.pos() << ' ' << M_ball.vel() << '\n';

    const std::list< GlobalPlayerObject >::const_iterator end = players().end();
    for ( std::list< GlobalPlayerObject >::const_iterator it = players().begin();
          it != end;
          ++it )
    {
        it->print( os ) << '\n';
    }

    return os << std::flush;
}

}
