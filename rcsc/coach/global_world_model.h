// -*-c++-*-

/*!
  \file global_world_model.h
  \brief noiseless world model class Header File
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

#ifndef RCSC_COACH_GLOBAL_WORLD_MODEL_H
#define RCSC_COACH_GLOBAL_WORLD_MODEL_H

#include <rcsc/coach/global_object.h>
#include <rcsc/coach/player_type_analyzer.h>
#include <rcsc/game_mode.h>
#include <rcsc/game_time.h>
#include <rcsc/types.h>

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <string>

namespace rcsc {

class AudioMemory;
class GlobalVisualSensor;

/*!
  \class GlobalWorldModel
  \brief world world for coach
 */
class GlobalWorldModel {
private:

    //! client protocol version
    int M_client_version;

    //! heard info memory
    boost::shared_ptr< AudioMemory > M_audio_memory;

    //
    // game info
    //

    //! last updated time
    GameTime M_time;
    //! last see_global received time
    GameTime M_see_time;

    //! if trainer, this becomes NEUTRAL
    SideID M_our_side;

    std::string M_team_name_left; //!< left team name string
    std::string M_team_name_right; //!< right teamname string

    //! last updated playmode
    GameMode M_game_mode;

    GameTime M_training_time; //!< training start/end time for keepaway

    //
    // objects
    //

    //! seen ball info
    GlobalBallObject M_ball;

    //! the container of player's instance
    std::list< GlobalPlayerObject > M_players;

    //! the container of player's pointer for the left team
    std::vector< const GlobalPlayerObject * > M_players_left;

    //! the container of player's pointer for the right team
    std::vector< const GlobalPlayerObject * > M_players_right;

    //
    // player type management
    //

    //! player type analyzer instance
    PlayerTypeAnalyzer M_player_type_analyzer;

    //! number of player change
    int M_change_count_left;
    //! number of player change
    int M_change_count_right;

    //! left team player type Id
    int M_player_types_left[11];
    //! right team player type Id
    int M_player_types_right[11];

    //! counter of each used player type for left team
    std::vector< int > M_player_type_used_count_left;
    //! counter of each used player type for right team
    std::vector< int > M_player_type_used_count_right;

    //
    // card information
    //

    bool M_yellow_card_left[11]; //!< left team's yellow card
    bool M_yellow_card_right[11]; //!< right team's yellow card
    bool M_red_card_left[11]; //!< left team's red card
    bool M_red_card_right[11]; //!< right team's red card

    //
    // coach language management
    //

    //! last play_on start cycle
    long M_last_playon_start;

    //! the number of allowd freeform message
    int M_freeform_allowed_count;

    //! the number of send out freeform message
    int M_freeform_send_count;


    //! not used
    GlobalWorldModel( const GlobalWorldModel & );
    //! not used
    GlobalWorldModel & operator=( const GlobalWorldModel & );
public:
    /*!
      \brief init member variables
     */
    GlobalWorldModel();

    /*!
      \brief init with team side info);
      \param side side character
      \param client_version client protocol version
     */
    void init( const SideID side,
               const int client_version );

    /*!
      \brief set new audio memory
      \param memory pointer to the memory instance. This must be
      a dynamically allocated object.
     */
    void setAudioMemory( boost::shared_ptr< AudioMemory > memory );

    /*!
      \brief get audio memory
      \return co
     */
    const
    AudioMemory & audioMemory() const
      {
          return *M_audio_memory;
      }

    /*!
      \brief init max freeform message count.

      This method is called just after server_param message.
     */
    void initFreeformCount();

    /*!
      \brief set team name
      \param side team side
      \param name team name string
     */
    void setTeamName( const SideID side,
                      const std::string & name );

    /*!
      \brief update player type info.
      this method is called when change_player_type message is received.
      \param side player's side
      \param unum player's uniform number
      \param type new player type id
     */
    void setPlayerType( const SideID side,
                        const int unum,
                        const int type );

    /*!
      \brief set yellow card information
      \param side punished player's side
      \param unum punished player's unum
     */
    void setYellowCard( const SideID side,
                        const int unum );

    /*!
      \brief set red card information
      \param side punished player's side
      \param unum punished player's unum
     */
    void setRedCard( const SideID side,
                     const int unum );

    /*!
      \brief update playmode using heard referee info
      \param game_mode analyzed referee info
      \param current current game time
     */
    void updateGameMode( const GameMode & game_mode,
                         const GameTime & current );

    /*!
      \brief update status using analyzed visual info
      \param see_global analyzed visual info
      \param current current game time
     */
    void updateAfterSeeGlobal( const GlobalVisualSensor & see_global,
                               const GameTime & current );

    /*!
      \brief set training start/end time
      \param t game time
     */
    void setTrainingTime( const GameTime & t )
      {
          M_training_time = t;
      }

    /*!
      \brief increment freeform send count
     */
    void incFreeformSendCount()
      {
          ++M_freeform_send_count;
      }

    /*!
      \brief get a client protocol version.
      \return client protocol version number.
     */
    int clientVersion() const
      {
          return M_client_version;
      }

    /*!
      \brief get our team side
      \return side Id
     */
    SideID ourSide() const
      {
          return M_our_side;
      }

    /*!
      \brief get opponent team side
      \return side Id
     */
    SideID theirSide() const
      {
          return ( M_our_side == LEFT ? RIGHT : LEFT );
      }

    /*!
      \brief get left team name
      \return team name string
     */
    const
    std::string & teamNameLeft() const
      {
          return M_team_name_left;
      }

    /*!
      \brief get right team name
      \return team name string
     */
    const
    std::string & teamNameRight() const
      {
          return M_team_name_right;
      }

    /*!
      \brief get our team name
      \return team name string
     */
    const
    std::string & ourTeamName() const
      {
          return ( ourSide() == LEFT
                   ? M_team_name_left
                   : M_team_name_right );
      }

    /*!
      \brief get our team name
      \return team name string
     */
    const
    std::string & theirTeamName() const
      {
          return ( ourSide() == LEFT
                   ? M_team_name_right
                   : M_team_name_left );
      }

    /*!
      \brief get last updated time
      \return const reference to the game time object
     */
    const
    GameTime & time() const
      {
          return M_time;
      }

    /*!
      \brief get last see global time
      \return const reference to the game time object
     */
    const
    GameTime & seeTime() const
      {
          return M_see_time;
      }

    /*!
      \brief get latest playmode info
      \return const reference to the GameMode object
     */
    const
    GameMode & gameMode() const
      {
          return M_game_mode;
      }

    /*!
      \brief get training start/end time
      \return game time object
     */
    const
    GameTime & trainingTime() const
      {
          return M_training_time;
      }

    /*!
      \brief get the latest ball data
      \return const reference to the ball data
     */
    const
    GlobalBallObject & ball() const
      {
          return M_ball;
      }

    /*!
      \brief get the current ball position status
      \return ball status type
     */
    BallStatus getBallStatus() const;

    /*!
      \brief
      get all players
      \return const reference to the data container
     */
    const
    std::list< GlobalPlayerObject > & players() const
      {
          return M_players;
      }

    /*!
      \brief get left players' pointer
      \return const reference to the data container
     */
    const
    std::vector< const GlobalPlayerObject * > & playersLeft() const
      {
          return M_players_left;
      }

    /*!
      \brief get right players' pointer
      \return const reference to the data container
     */
    const
    std::vector< const GlobalPlayerObject * > & playersRight() const
      {
          return M_players_right;
      }

    /*!
      \brief get teammate players' pointer
      \return const reference to the data container
     */
    const
    std::vector< const GlobalPlayerObject * > & teammates() const
      {
          return ( ourSide() == LEFT
                   ? playersLeft()
                   : playersRight() );
      }

    /*!
      \brief get opponent players' pointer
      \return const reference to the data container
     */
    const
    std::vector< const GlobalPlayerObject * > & opponents() const
      {
          return ( ourSide() == LEFT
                   ? playersRight()
                   : playersLeft() );
      }

    /*!
      \brief get the specified teammate.
      \param unum input uniform number.
      \return pointer to the object. if not found, NULL is returned.
     */
    const
    GlobalPlayerObject * teammate( const int unum ) const;

    /*!
      \brief get the specified teammate.
      \param unum input uniform number.
      \return pointer to the object. if not found, NULL is returned.
     */
    const
    GlobalPlayerObject * opponent( const int unum ) const;


    //
    // player type info
    //

    /*!
      \brief get the count of change_player_type message after kickoff
      \return the count of change_player_type message after kickoff
     */
    int playerTypeChangeCount( const SideID side ) const
      {
          return ( side == LEFT
                   ? M_change_count_left
                   : M_change_count_right );
      }

    /*!
      \brief get teammate player type Id
      \param side side ID of the target player
      \param unum uniform number of the target player
      \return player type id
     */
    int heteroID( const SideID side,
                  const int unum ) const;

    /*!
      \brief get the counter of used player type for left team
      \return container of counts for each player type
     */
    const
    std::vector< int > & playerTypeUsedCount( const SideID side ) const
      {
          return ( side == LEFT
                   ? M_player_type_used_count_left
                   : M_player_type_used_count_right );
      }


    //
    // card information
    //

    /*!
      \brief get yellow card status
      \param side team side
      \param unum uniform number
      \return yellow card status
     */
    bool isYellowCarded( const SideID side,
                         const int unum ) const;


    /*!
      \brief get red card status
      \param side team side
      \param unum uniform number
      \return red card status
     */
    bool isRedCarded( const SideID side,
                      const int unum ) const;

    /*!
      \brief get teammate's yellow card status
      \param unum uniform number
      \return yellow card status
     */
    bool isTeammateYellowCarded( const int unum ) const
      {
          return isYellowCarded( ourSide(), unum );
      }

    /*!
      \brief get teammate's red card status
      \param unum uniform number
      \return red card status
     */
    bool isTeammateRedCarded( const int unum ) const
      {
          return isRedCarded( ourSide(), unum );
      }

    /*!
      \brief get opponent's yellow card status
      \param unum uniform number
      \return yellow card status
     */
    bool isOpponentYellowarded( const int unum ) const
      {
          return isYellowCarded( theirSide(), unum );
      }


    /*!
      \brief get opponent's red card status
      \param unum uniform number
      \return red card status
     */
    bool isOpponentRedCarded( const int unum ) const
      {
          return isRedCarded( theirSide(), unum );
      }


    //
    // coach language info
    //

    /*!
      \brief get the playon period
      \return cycle value
     */
    long lastPlayOnStart() const
      {
          return M_last_playon_start;
      }

    /*!
      \brief get the allowed freeform message count
      \return the number of the allowd message count
     */
    int freeformAllowedCount() const
      {
          return M_freeform_allowed_count;
      }

    /*!
      \brief get the freeform message send count
      \return the number of the message send count
     */
    int freeformSendCount() const
      {
          return M_freeform_send_count;
      }

    /*!
      \brief check if coach can sent the freeform message now.
      \return the check result
     */
    bool canSendFreeform() const;


    /*!
      \brief check if kickable player exists or not
      \return true if kickable plaeyr exists
     */
    bool existKickablePlayer() const;

    /*!
      \brief get player pointer nearest to the specified point
      \param point target point
      \return const pointer to the player object
     */
    const
    GlobalPlayerObject * getPlayerNearestTo( const Vector2D & point ) const;

    /*!
      \brief put all data to the output stream
      \param os reference to the output stream
      \return reference to the output stream
     */
    std::ostream & print( std::ostream & os ) const;

};

}

#endif
