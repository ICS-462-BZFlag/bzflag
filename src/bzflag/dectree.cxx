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
        if ( getBranch(bot, dt) ) {
            // Make sure its not null before recursing.
            if (trueBranch == NULL) {
#ifdef TRACE_DECTREE
				controlPanel->addMessage("NULL true branch");
				throw "NULL true branch";
#endif
				return NULL;
			}
            else return trueBranch->makeDecision(bot, dt);
        } else {
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
		DecisionTreeNode *node = decTree[0].makeDecision(bot, dt);
		void (RobotPlayer::*actFuncPtr)(float dt) = ((ActionPtr*)node)->actFuncPtr;
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
		doUpdateMotionDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
		doUpdateMotionDecisions[0].trueBranch = &doUpdateMotionDecisions[1];
		doUpdateMotionDecisions[0].falseBranch = &doUpdateMotionActions[0];

		doUpdateMotionDecisions[1].decFuncPtr = &RobotPlayer::returnTrue;
		doUpdateMotionDecisions[1].trueBranch = &doUpdateMotionActions[0];
		doUpdateMotionDecisions[1].falseBranch = &doUpdateMotionActions[1];

		doUpdateMotionDecisions[2].decFuncPtr = &RobotPlayer::returnFalse;
		doUpdateMotionDecisions[2].trueBranch = &doUpdateMotionDecisions[3];
		doUpdateMotionDecisions[2].falseBranch = &doUpdateMotionDecisions[4];

		doUpdateMotionDecisions[3].decFuncPtr = &RobotPlayer::returnTrue;
		doUpdateMotionDecisions[3].trueBranch = &doUpdateMotionDecisions[5];
		doUpdateMotionDecisions[3].falseBranch = &doUpdateMotionActions[2];

		doUpdateMotionDecisions[4].decFuncPtr = &RobotPlayer::returnTrue;
		doUpdateMotionDecisions[4].trueBranch = &doUpdateMotionActions[2];
		doUpdateMotionDecisions[4].falseBranch = &doUpdateMotionActions[3];

		doUpdateMotionDecisions[5].decFuncPtr = &RobotPlayer::returnTrue;
		doUpdateMotionDecisions[5].trueBranch = &doUpdateMotionActions[2];
		doUpdateMotionDecisions[5].falseBranch = &doUpdateMotionDecisions[6];

		doUpdateMotionDecisions[6].decFuncPtr = &RobotPlayer::returnTrue;
		doUpdateMotionDecisions[6].trueBranch = &doUpdateMotionActions[4];
		doUpdateMotionDecisions[6].falseBranch = &doUpdateMotionActions[2];

		doUpdateMotionActions[0].actFuncPtr = &RobotPlayer::a1;
		doUpdateMotionActions[1].actFuncPtr = &RobotPlayer::a2;
		doUpdateMotionActions[2].actFuncPtr = &RobotPlayer::a3;
		doUpdateMotionActions[3].actFuncPtr = &RobotPlayer::a4;
		doUpdateMotionActions[4].actFuncPtr = &RobotPlayer::a5;

        /*
        DropFlags Decision Tree
        */
        doUpdateFlagsDecisions[0].decFuncPtr = &RobotPlayer::amAlive;
        doUpdateFlagsDecisions[0].trueBranch = &doUpdateFlagsDecisions[1];
        doUpdateFlagsDecisions[0].falseBranch = &doUpdateFlagsDecisions[0];
        
        doUpdateFlagsDecisions[1].decFuncPtr = &RobotPlayer::amHoldingFlag;
        doUpdateFlagsDecisions[1].trueBranch = &doUpdateFlagsDecisions[2];
        doUpdateFlagsDecisions[1].falseBranch = &doUpdateFlagsDecisions[0];

        doUpdateFlagsDecisions[2].decFuncPtr = &RobotPlayer::isFlagSticky;
        doUpdateFlagsDecisions[2].trueBranch = &doUpdateFlagsDecisions[3];
        doUpdateFlagsDecisions[2].falseBranch = &doUpdateFlagsDecisions[0];

        doUpdateFlagsDecisions[3].decFuncPtr = &RobotPlayer::flagNoTeam;
        doUpdateFlagsDecisions[3].trueBranch = &doUpdateFlagsActions[0];
        doUpdateFlagsDecisions[3].falseBranch = &doUpdateFlagsDecisions[4];

        doUpdateFlagsDecisions[4].decFuncPtr = &RobotPlayer::flagMyTeam;
        doUpdateFlagsDecisions[4].trueBranch = &doUpdateFlagsActions[0];
        doUpdateFlagsDecisions[4].falseBranch = &doUpdateFlagsDecisions[0];

        doUpdateFlagsActions[0].actFuncPtr = &RobotPlayer::actiondF1;
	}

	DecisionPtr DecisionTrees::doUpdateMotionDecisions[7];
	ActionPtr DecisionTrees::doUpdateMotionActions[5];
    DecisionPtr DecisionTrees::doUpdateFlagsDecisions[5];
    ActionPtr DecisionTrees::doUpdateFlagsActions[1];

}; // end of namespace
