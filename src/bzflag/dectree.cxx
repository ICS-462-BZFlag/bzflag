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
        doUpdateShootingDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
        doUpdateShootingDecisions[0].trueBranch = &doUpdateShootingDecisions[1];
        doUpdateShootingDecisions[0].falseBranch = &doUpdateShootingActions[0];

        /*
        Boolean function: isFiringStatusReady
        True branch: proceed to next decision
        false branch: do nothing
        */
        doUpdateShootingDecisions[1].decFuncPtr = &RobotPlayer::isFiringStatusReady;
        doUpdateShootingDecisions[1].trueBranch = &doUpdateShootingDecisions[2];
        doUpdateShootingDecisions[1].falseBranch = &doUpdateShootingActions[0];

        /*
        Boolean function: hasShotTimerElapsed
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateShootingDecisions[2].decFuncPtr = &RobotPlayer::hasShotTimerElapsed;
        doUpdateShootingDecisions[2].trueBranch = &doUpdateShootingDecisions[3];
        doUpdateShootingDecisions[2].falseBranch = &doUpdateShootingActions[0];

        /*
        Boolean function: willTheShotMiss
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateShootingDecisions[3].decFuncPtr = &RobotPlayer::willTheShotMiss;
        doUpdateShootingDecisions[3].trueBranch = &doUpdateShootingDecisions[4];
        doUpdateShootingDecisions[3].falseBranch = &doUpdateShootingActions[0];

        /*
        Boolean function: isBlockedByBuildings
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateShootingDecisions[4].decFuncPtr = &RobotPlayer::isBlockedByBuildings;
        doUpdateShootingDecisions[4].trueBranch = &doUpdateShootingDecisions[5];
        doUpdateShootingDecisions[4].falseBranch = &doUpdateShootingActions[0];

        /*
        Boolean function: isBlockedByTeammates
        True branch: setShotTimer
        false branch: fireTheShot
        */
        doUpdateShootingDecisions[5].decFuncPtr = &RobotPlayer::isBlockedByTeammates;
        doUpdateShootingDecisions[5].trueBranch = &doUpdateShootingActions[1];
        doUpdateShootingDecisions[5].falseBranch = &doUpdateShootingActions[2];

        /*
        do Nothing --literally do nothing
        setShottimer --sets the shot timer
        fireTheShot -- fires the shot
        */
        doUpdateShootingActions[0].actFuncPtr = &RobotPlayer::doNothing;
        doUpdateShootingActions[1].actFuncPtr = &RobotPlayer::setShotTimer;
        doUpdateShootingActions[2].actFuncPtr = &RobotPlayer::fireTheShot;

        /*
            DropFlags Decision Tree
        */
        /*
        Boolean function: amlive
        True branch: preoceed to the next decision
        false branch: do nothing
        */
        doUpdateDropFlagDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
        doUpdateDropFlagDecisions[0].trueBranch = &doUpdateDropFlagDecisions[1];
        doUpdateDropFlagDecisions[0].falseBranch = &doUpdateDropFlagsActions[0];

        /*
        Boolean function: amHoldingFlag
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateDropFlagDecisions[1].decFuncPtr = &RobotPlayer::amHoldingFlag;
        doUpdateDropFlagDecisions[1].trueBranch = &doUpdateDropFlagDecisions[2];
        doUpdateDropFlagDecisions[1].falseBranch = &doUpdateDropFlagsActions[0];

        /*
        Boolean function: isFlagNotSticky
        True branch: proceed to the next decision
        false branch: do nothing
        */
        doUpdateDropFlagDecisions[2].decFuncPtr = &RobotPlayer::isFlagNotSticky;
        doUpdateDropFlagDecisions[2].trueBranch = &doUpdateDropFlagDecisions[3];
        doUpdateDropFlagDecisions[2].falseBranch = &doUpdateDropFlagsActions[0];

        /*
        Boolean function: flagNoTeam --checks if the flag is of property noTeam
        True branch: drop flag
        false branch: proceed to the next decision
        */
        doUpdateDropFlagDecisions[3].decFuncPtr = &RobotPlayer::flagNoTeam;
        doUpdateDropFlagDecisions[3].trueBranch = &doUpdateDropFlagsActions[1];
        doUpdateDropFlagDecisions[3].falseBranch = &doUpdateDropFlagDecisions[4];

        /*
        Boolean function: FlagmyTeam --checks if the flag is my team
        True branch: drop flag
        false branch: do nothing
        */
        doUpdateDropFlagDecisions[4].decFuncPtr = &RobotPlayer::flagMyTeam;
        doUpdateDropFlagDecisions[4].trueBranch = &doUpdateDropFlagsActions[1];
        doUpdateDropFlagDecisions[4].falseBranch = &doUpdateDropFlagsActions[0];

        /*
        doNothing --literally do nothing
        aDropFlag --Drop it
        */
        doUpdateDropFlagsActions[0].actFuncPtr = &RobotPlayer::doNothing;
        doUpdateDropFlagsActions[1].actFuncPtr = &RobotPlayer::aDropFlag;
    }

    DecisionPtr DecisionTrees::doUpdateMotionDecisions[2];
    ActionPtr DecisionTrees::doUpdateMotionActions[3];
    DecisionPtr doUpdateDropFlagDecisions[5];
    //Holds our list of actions based on flags
    ActionPtr doUpdateDropFlagsActions[2];
    //holds our list of decisions based on flags
    DecisionPtr doUpdateShootingDecisions[6];
    //Holds our list of actions based on flags
    ActionPtr doUpdateShootingActions[3];
    //Holds our list of actions for the point check

}; // end of namespace
