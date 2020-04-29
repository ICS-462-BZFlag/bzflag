/*
 * Defines the classes used for decision trees.
 *
 * Part of the Artificial Intelligence for Games system.
 *
 * Copyright (c) Ian Millington 2003-2006. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */
#include <exception>
#include "dectree.h"
#include "playing.h" // needed for controlPanel
#define TRACE_DECTREE

namespace aicore
{


    DecisionTreeNode* DecisionPtr::makeDecision(RobotPlayer* bot, float dt)
    {
        // Choose a branch based on the getBranch method
        if (getBranch(bot, dt)) {
            // Make sure its not null before recursing.
            if (trueBranch == NULL) {
#ifdef TRACE_DECTREE
                controlPanel->addMessage("NULL true branch");
                throw "NULL true branch";
#endif
                return NULL;
            }
            else return trueBranch->makeDecision(bot, dt);
        }
        else {
            // Make sure its not null before recursing.
            if (falseBranch == NULL) {
#ifdef TRACE_DECTREE
                controlPanel->addMessage("NULL false branch");
                throw "NULL false branch";
#endif
                return NULL;
            }
            else return falseBranch->makeDecision(bot, dt);
        }
    }

    bool DecisionPtr::getBranch(RobotPlayer* bot, float dt)
    {
        if (decFuncPtr == NULL) {
#ifdef TRACE_DECTREE
            controlPanel->addMessage("NULL decFunctPtr");
            throw "NULL decFunctPtr";
#endif
        }
        return (bot->*decFuncPtr)(dt);
    }

    void DecisionPtr::runDecisionTree(DecisionPtr decTree[], RobotPlayer* bot, float dt)
    {
        // Find the decision
        DecisionTreeNode* node = decTree[0].makeDecision(bot, dt);
        void (RobotPlayer:: * actFuncPtr)(float dt) = ((ActionPtr*)node)->actFuncPtr;
        if (actFuncPtr == NULL) {
#ifdef TRACE_DECTREE
            controlPanel->addMessage("NULL action function pointer in decision tree.");
            throw "NULL action function pointer in decision tree.";
#endif // TRACE_DECTREE
        }
        else {
            (bot->*actFuncPtr)(dt);
        }
    }

    // Set up the trees
    void DecisionTrees::init()
    {
        /*
        Do Update Motion Decision Tree
        */
        /*
        Boolean function: amAlive --checks if robot alive
        True branch: procede to next decision
        false branch: do nothing
        */
        doUpdateMotionDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
        doUpdateMotionDecisions[0].trueBranch = &doUpdateMotionDecisions[1];
        doUpdateMotionDecisions[0].falseBranch = &doUpdateMotionActions[0];

        /*
        Boolean function: isShotComing --returns if shot is about to hit
        True branch: Evade
        false branch: Folow A* algorithm
        */
        doUpdateMotionDecisions[1].decFuncPtr = &RobotPlayer::isShotComing;
        doUpdateMotionDecisions[1].trueBranch = &doUpdateMotionActions[1];
        doUpdateMotionDecisions[1].falseBranch = &doUpdateMotionActions[2];

        /*
        doNothing --literally do nothing. its jsut a print statement
        EvasiveManeuvers --Roberts Evasive Code
        followAStar --Follow the aStar algorithm
        */
        doUpdateMotionActions[0].actFuncPtr = &RobotPlayer::doNothing;
        doUpdateMotionActions[1].actFuncPtr = &RobotPlayer::EvasiveManeuvers;
        doUpdateMotionActions[2].actFuncPtr = &RobotPlayer::followAStar;

        /*
            DoUpdate Shooting Tree
        */
        /*
        Boolean function: amAlive
        True branch: proceed to next decision
        false branch: do nothing
        */
        doUpdateShootDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
        doUpdateShootDecisions[0].trueBranch = &doUpdateShootDecisions[1];
        doUpdateShootDecisions[0].falseBranch = &doUpdateShootActions[0];

        /*
        Boolean function: isFiringStatusReady
        True branch: proceed to next decision
        false branch: do nothing
        */
        doUpdateShootDecisions[1].decFuncPtr = &RobotPlayer::isFiringStatusReady;
        doUpdateShootDecisions[1].trueBranch = &doUpdateShootDecisions[2];
        doUpdateShootDecisions[1].falseBranch = &doUpdateShootActions[0];

        /*
        Boolean function: hasShotTimerElapsed
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateShootDecisions[2].decFuncPtr = &RobotPlayer::hasShotTimerElapsed;
        doUpdateShootDecisions[2].trueBranch = &doUpdateShootDecisions[3];
        doUpdateShootDecisions[2].falseBranch = &doUpdateShootActions[0];

        /*
        Boolean function: willTheShotMiss
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateShootDecisions[3].decFuncPtr = &RobotPlayer::willTheShotMiss;
        doUpdateShootDecisions[3].trueBranch = &doUpdateShootDecisions[4];
        doUpdateShootDecisions[3].falseBranch = &doUpdateShootActions[0];

        /*
        Boolean function: isBlockedByBuildings
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateShootDecisions[4].decFuncPtr = &RobotPlayer::isBlockedByBuildings;
        doUpdateShootDecisions[4].trueBranch = &doUpdateShootDecisions[5];
        doUpdateShootDecisions[4].falseBranch = &doUpdateShootActions[0];

        /*
        Boolean function: isBlockedByTeammates
        True branch: setShotTimer
        false branch: fireTheShot
        */
        doUpdateShootDecisions[5].decFuncPtr = &RobotPlayer::isBlockedByTeammates;
        doUpdateShootDecisions[5].trueBranch = &doUpdateShootActions[1];
        doUpdateShootDecisions[5].falseBranch = &doUpdateShootActions[2];

        /*
        do Nothing --literally do nothing
        setShottimer --sets the shot timer
        fireTheShot -- fires the shot
        */
        doUpdateShootActions[0].actFuncPtr = &RobotPlayer::doNothing;
        doUpdateShootActions[1].actFuncPtr = &RobotPlayer::setShotTimer;
        doUpdateShootActions[2].actFuncPtr = &RobotPlayer::fireTheShot;

        /*
            DropFlags Decision Tree
        */
        /*
        Boolean function: amlive
        True branch: preoceed to the next decision
        false branch: do nothing
        */
        doUpdateFlagsDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
        doUpdateFlagsDecisions[0].trueBranch = &doUpdateFlagsDecisions[1];
        doUpdateFlagsDecisions[0].falseBranch = &doUpdateFlagsActions[0];

        /*
        Boolean function: amHoldingFlag
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateFlagsDecisions[1].decFuncPtr = &RobotPlayer::amHoldingFlag;
        doUpdateFlagsDecisions[1].trueBranch = &doUpdateFlagsDecisions[2];
        doUpdateFlagsDecisions[1].falseBranch = &doUpdateFlagsActions[0];

        /*
        Boolean function: isFlagNotSticky
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateFlagsDecisions[2].decFuncPtr = &RobotPlayer::isFlagNotSticky;
        doUpdateFlagsDecisions[2].trueBranch = &doUpdateFlagsDecisions[3];
        doUpdateFlagsDecisions[2].falseBranch = &doUpdateFlagsActions[0];

        /*
        Boolean function: flagNoTeam --checks if the flag is of property noTeam
        True branch: drop flag
        false branch: proceed to the next decision
        */
        doUpdateFlagsDecisions[3].decFuncPtr = &RobotPlayer::flagNoTeam;
        doUpdateFlagsDecisions[3].trueBranch = &doUpdateFlagsActions[1];
        doUpdateFlagsDecisions[3].falseBranch = &doUpdateFlagsDecisions[4];

        /*
        Boolean function: FlagmyTeam --checks if the flag is my team
        True branch: Proceed next decision
        false branch: do nothing
        */
        doUpdateFlagsDecisions[4].decFuncPtr = &RobotPlayer::flagMyTeam;
        doUpdateFlagsDecisions[4].trueBranch = &doUpdateFlagsDecisions[5];
        doUpdateFlagsDecisions[4].falseBranch = &doUpdateFlagsActions[0];

        /*
        Boolean function: isShieldFlag --checks if the flag is shield
        True branch: do nothing
        False branch: proceed to next decision
        */
        doUpdateFlagsDecisions[5].decFuncPtr = &RobotPlayer::flagMyTeam;
        doUpdateFlagsDecisions[5].trueBranch = &doUpdateFlagsActions[0];
        doUpdateFlagsDecisions[5].falseBranch = &doUpdateFlagsDecisions[6];

        /*
       Boolean function: isGenocideFlag --checks if the flag is genocide
       True branch: do nothing
       False branch: drop Flag
       */
        doUpdateFlagsDecisions[6].decFuncPtr = &RobotPlayer::flagMyTeam;
        doUpdateFlagsDecisions[6].trueBranch = &doUpdateFlagsActions[0];
        doUpdateFlagsDecisions[6].falseBranch = &doUpdateFlagsActions[1];


        /*
        doNothing --literally do nothing
        aDropFlag --Drop it
        */
        doUpdateFlagsActions[0].actFuncPtr = &RobotPlayer::doNothing;
        doUpdateFlagsActions[1].actFuncPtr = &RobotPlayer::aDropFlag;

        /*
        DEFENSE TREE DECISIONS
       
        DefenseTreeDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
        DefenseTreeDecisions[0].trueBranch = &DefenseTreeDecisions[1];
        DefenseTreeDecisions[0].falseBranch = &DefenseTreeActions[0];

        DefenseTreeDecisions[1].decFuncPtr = &RobotPlayer::teamFlagOnGround;
        DefenseTreeDecisions[1].trueBranch = &DefenseTreeActions[1];
        DefenseTreeDecisions[1].falseBranch = &DefenseTreeDecisions[2];

        DefenseTreeDecisions[2].decFuncPtr = &RobotPlayer::flagOnMyTeam;
        DefenseTreeDecisions[2].trueBranch = &DefenseTreeDecisions[3];
        DefenseTreeDecisions[2].falseBranch = &DefenseTreeActions[2];

        DefenseTreeDecisions[3].decFuncPtr = &RobotPlayer::atCorner;
        DefenseTreeDecisions[3].trueBranch = &DefenseTreeActions[3];
        DefenseTreeDecisions[3].falseBranch = &DefenseTreeActions[4];
        
        DEFENSE TREE ACTIONS
        
        DefenseTreeActions[0].actFuncPtr = &RobotPlayer::doNothing;
        DefenseTreeActions[1].actFuncPtr = &RobotPlayer::aStarToFlag;
        DefenseTreeActions[2].actFuncPtr = &RobotPlayer::aStarToCurrentCorner;
        DefenseTreeActions[3].actFuncPtr = &RobotPlayer::swapCornerAndAStar;
        DefenseTreeActions[4].actFuncPtr = &RobotPlayer::aStarToFlag;
        */
    }
    
    DecisionPtr DecisionTrees::doUpdateMotionDecisions[2];
    ActionPtr   DecisionTrees::doUpdateMotionActions[3];
    DecisionPtr DecisionTrees::doUpdateFlagsDecisions[7];
    ActionPtr   DecisionTrees::doUpdateFlagsActions[2];
    DecisionPtr DecisionTrees::doUpdateShootDecisions[6];
    ActionPtr   DecisionTrees::doUpdateShootActions[3];
    //DecisionPtr DecisionTrees::DefenseTreeDecisions[4];
    //ActionPtr   DecisionTrees::DefenseTreeActions[5];

}; // end of namespace
