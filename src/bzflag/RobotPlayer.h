/* bzflag
 * Copyright (c) 1993-2018 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

 /*
  *
  */

#ifndef BZF_ROBOT_PLAYER_H
#define BZF_ROBOT_PLAYER_H

#include "common.h"

  /* system interface headers */
#include <vector>

/* interface header */
#include "LocalPlayer.h"

/* local interface headers */
#include "Region.h"
#include "RegionPriorityQueue.h"
#include "ServerLink.h"

/* lines added by David Chin */
#include "AStarNode.h" // needed for A* search
/* end of lines added by David Chin */

class RobotPlayer : public LocalPlayer
{
public:
    RobotPlayer(const PlayerId&,
        const char* name, ServerLink*,
        const char* _motto);

    float       getTargetPriority(const Player*) const;
    const Player* getTarget() const;
    void        setTarget(const Player*);
    static void     setObstacleList(std::vector<BzfRegion*>*);

    void        restart(const float* pos, float azimuth);
    void        explodeTank();
    /* code added by David Chin */
    bool		RobotPlayer::amAlive(float dt);
	bool amIAlive(float dt);
	bool flagOnMyTeam(float dt);
	bool atCorner(float dt);
	void setCorners();
	void setCurrentCorner(int input);
	bool teamFlagOnGround(float dt);
    bool amHoldingFlag(float dt);
    bool isFlagNotSticky(float dt);
    bool flagNoTeam(float dt);
    bool flagMyTeam(float dt);
	bool isShieldFlag(float dt);
    bool isGenocideFlag(float dt);
    bool isFiringStatusReady(float dt);
    bool hasShotTimerElapsed(float dt);
    bool willTheShotMiss(float dt);
    bool isBlockedByBuildings(float dt);
    bool isBlockedByTeammates(float dt);
    void setShotTimer(float dt);
    void fireTheShot(float dt);
    bool isShotComing(float dt);
    void EvasiveManeuvers(float dt);
    void followAStar(float dt);
    bool		returnTrue(float dt);
    bool		returnFalse(float dt);
    void doNothing(float dt);
    void		RobotPlayer::a1(float dt);
    void		RobotPlayer::a2(float dt);
    void		RobotPlayer::a3(float dt);
    void		RobotPlayer::a4(float dt);
    void		RobotPlayer::a5(float dt);
    void aDropFlag(float dt);
    /* end of code added by David Chin */

private:
    void        doUpdate(float dt);
    void        doUpdateMotion(float dt);
    BzfRegion* findRegion(const float p[2], float nearest[2]) const;
    float       getRegionExitPoint(
        const float p1[2], const float p2[2],
        const float a[2], const float targetPoint[2],
        float mid[2], float& priority);
    void       findPath(RegionPriorityQueue& queue,
        BzfRegion* region, BzfRegion* targetRegion,
        const float targetPoint[2], int mailbox);

    void       projectPosition(const Player* targ, const float t, float& x, float& y, float& z) const;
    void       getProjectedPosition(const Player* targ, float* projpos) const;
    /* lines added by David Chin */
    void		findHomeBase(TeamColor teamColor, float location[3]);
    bool		myTeamHoldingOpponentFlag(void);
    void		findOpponentFlag(float location[3]);
    int		computeCenterOfMass(float neighborhoodSize, float cmOut[3]);
    int		computeRepulsion(float neighborhoodSize, float repulseOut[3]);
    int		computeAlign(float neighborhoodSize, float avVOut[3], float* avAzimuthOut);
    Player* lookupLocalPlayer(PlayerId id);

    static const float		CohesionW;
    static const float		SeparationW;
    static const float		AlignW;
    static const float		PathW;
    /* end of lines added by David Chin */

private:
    const Player* target;
    std::vector<RegionPoint>    path;
    int         pathIndex;
    float       timerForShot;
    bool        drivingForward;
    static std::vector<BzfRegion*>* obstacleList;
    /* lines added by David Chin */
    std::vector< AStarNode > AstarPath; // planner result path
    AStarNode pathGoalNode;	// goal position for current planner result
/* end of lines added by David Chin */
    float targetdistance;
    float direction[3];
    bool shoot = false;
    bool evading = false;
    float shotAngle;
};

#endif // BZF_ROBOT_PLAYER_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 4 ***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
