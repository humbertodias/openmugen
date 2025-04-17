#include "Input.h"
#include "global.h"

// is for debug output
char* strState[] = {"stand", "crouch", "air", "liedown", "attack", "idle", "hit", "none", "unchange"};
/*
================================================================================
Main Player class which represents one Player
Handels everything the loading and acting of the player

Todo:
Change CSffManager,CAirManager,CStateManager to pointers
to add a CreateHelper function which returns a new Player
instace which will be a copy of its parent

================================================================================
*/

CPlayer::CPlayer() {
    bRightFaced       = true;
    bAlive            = true;
    bDebugInfo        = false;
    bHitDefPresit     = false;
    bHitCounterPresit = false;
    bMoveHitPresit    = false;
    bHitDef           = false;
    nLife             = 1000;
    nPower            = 1000;
    nStateTime        = 0;
    nStateType        = 0;
    nPhysic           = 0;
    nMoveType         = 0;
    x = y = 0;
    xVel  = 0;
    yVel  = 0;

    InitFunctTable();
}

CPlayer::~CPlayer() {}

void CPlayer::SetKeyBoard(int playerid) {
    char Player1_KeyArr[] = "wsadyuchjkopl";
    char Player2_KeyArr[] = "qqqqqqqqqqqqq";

    m_keyData            = (KEYBOARDDATA*)m_pAlloc->Alloc(sizeof(KEYBOARDDATA));
    m_keyData->bKeyBoard = true;
    switch (playerid) {
        case P1:
            // Process keyboard input
            for (int k = 0; k < KEY_COUNT; k++) {
                m_keyData->keyInfo[k].sdlKeycode = (Uint16)Player1_KeyArr[k];
            }
            break;
        case P2:
            // Process keyboard input
            for (int k = 0; k < KEY_COUNT; k++) {
                m_keyData->keyInfo[k].sdlKeycode = (Uint16)Player2_KeyArr[k];
            }
            break;
        default:
            break;
    }
}

// Set all the pointers to all the managers
void CPlayer::SetPointers(CVideoSystem* p, CAllocater* a, CGameTimer* t) {
    m_pAlloc       = a;
    m_pVideoSystem = p;
    m_pTimer       = t;

    m_SffManager.SetPointers(p, a, t, &m_AirManager);
    m_StateManager.SetAlloc(a);
    m_AirManager.SetAlloc(a);
    m_CmdManager.SetGameTimer(t);
}

// Load the player by his given .def filename
// ToDo:Handel the .def file loading
bool CPlayer::LoadPlayer(const char* strPlayer) {
    CStateParser StateParser;
    // rest memory allocator
    m_pAlloc->ResetAllocater();

    m_SffManager.ResetManager();
    m_StateManager.Reset();
    m_AirManager.ResetManager();

    m_CmdManager.LoadCMDFile("data/kfm/kfm.cmd");
    StateParser.ParseStateFile("data/kfm/kfm.cmd1", m_StateManager, m_pAlloc);
    StateParser.ParseStateFile("data/common1.cns", m_StateManager, m_pAlloc);
    m_AirManager.OpenAir("data/kfm/kfm.air");

    m_SffManager.LoadActToSff("data/kfm/kfm.act");
    m_SffManager.LoadSffFile("data/kfm/kfm.sff");
    // Make always masked blit
    m_SffManager.SetBltFlags(CSffManager::BLT_NORMALMASKED);
    m_SffManager.PrepareAnim(0);

    ChangeState(0);

    return true;
}

/*
================================================================================
Handles the players Physic
================================================================================
*/
void CPlayer::HandlePhysic() {
    x += xVel;
    y += yVel;
}

/*
================================================================================
Handles the FSM of the player
-checks all trigger
-and activate controllers
================================================================================
*/
void CPlayer::HandleFSM() {
    // ִ��״̬-1
    PLSTATEDEF* specialState = this->m_StateManager.GetStateDef(-1);
    // check every state in this statedef
    for (u16 i = 0; i < specialState->nHowManyState; i++) {
        if (CheckState(&specialState->lpState[i])) ExecuteController(&specialState->lpState[i]);
    }
    // check every state in this statedef
    for (u16 i = 0; i < lpCurrStatedef->nHowManyState; i++) {
        if (CheckState(&lpCurrStatedef->lpState[i])) ExecuteController(&lpCurrStatedef->lpState[i]);
    }
}

/*
================================================================================
Checks all the triggers in the current state
================================================================================
*/
bool CPlayer::CheckState(PLSTATE* tempState) {
    bool bTriggerAll = true;
    bool bTrigger    = false;
    u8   nTriggerType;

    for (int i = 0; i < tempState->nHowManyTriggers; i++) {
        PLTRIGGER trigger = tempState->triggers[i];
        bTrigger          = m_pVMachine->Execute(trigger.pInts);
        if (bTrigger) {
            return true;
        }
    }
    return (bTriggerAll && bTrigger);
}

void CPlayer::ExecuteController(PLSTATE* tempState) {
    PrintMessage("%s, exec func no:%d, %s", this->GetPlayerName(), tempState->nType,
                 strControllerTypes[tempState->nType]);

    // Execute the function
    (this->*pFuncTable[tempState->nType])(tempState);
}

// updates all internal stuff of the player
void CPlayer::UpDatePlayer() {
    CInput::ProcessInput(m_keyData);
    m_CmdManager.Update(m_keyData, bRightFaced);
    HandleFSM();
    HandlePhysic();
    UpDateFacing();
    nStateTime++;
}

// handles the facing of the player
void CPlayer::UpDateFacing() {
    if (bRightFaced)
        m_SffManager.SetBltFlags(CSffManager::BLT_NORMALMASKED);
    else
        m_SffManager.SetBltFlags(CSffManager::BLT_FLIPHMASKED);
}

// draw debug info of the player
void CPlayer::Debug() {
    ActionElement* Anim = m_SffManager.GetCurrAnimation();

    // Print information about the current Animation
    m_pVideoSystem->DrawText(0, 10, "Action %3i | AnimElem %3i/%3i | AnimTime %3i/%3i ", Anim->nActionNumber,
                             Anim->nCurrentImage + 1, Anim->nNumberOfElements, Anim->nCurrTime, Anim->nCompletAnimTime);

    m_pVideoSystem->DrawText(0, 20, "StateType=%s | MoveType=%s | Physic=%s", strState[nStateType], strState[nMoveType],
                             strState[nPhysic]);
    m_pVideoSystem->DrawText(0, 30, "x=%3f,y=%3f", x, y - nGround);
    m_pVideoSystem->DrawText(0, 40, "HitDef %i | State %i | StateTime %i", bHitDef, lpCurrStatedef->StateNumber,
                             this->nStateTime);
}

// Change State controller
void CPlayer::ChangeState(s32 nStateNumber) {
    // reset StateTime
    nStateTime = 0;
    // get the statedef
    lpCurrStatedef = m_StateManager.GetStateDef(nStateNumber);

    // Set StateType
    if (lpCurrStatedef->type != untouch) nStateType = lpCurrStatedef->type;
    // Set Physic
    if (lpCurrStatedef->physics != untouch) nPhysic = lpCurrStatedef->physics;

    if (lpCurrStatedef->movetype != untouch) nMoveType = lpCurrStatedef->movetype;
    // Set the Ctrl flag
    if (lpCurrStatedef->bCtrl != -1) bCtrl = lpCurrStatedef->bCtrl;

    bHitDefPresit     = lpCurrStatedef->bHitdefpersist;
    bHitCounterPresit = lpCurrStatedef->bHitcountpersist;
    bMoveHitPresit    = lpCurrStatedef->bMovehitpersist;

    // change anim if needed
    if (lpCurrStatedef->nAnim != -1) m_SffManager.PrepareAnim(lpCurrStatedef->nAnim);

    if (lpCurrStatedef->Velset.x != -3333) xVel = lpCurrStatedef->Velset.x;

    if (lpCurrStatedef->Velset.y != -3333) yVel = lpCurrStatedef->Velset.y;

    if (lpCurrStatedef->nSprpriority != 255) nSprPrio = lpCurrStatedef->nSprpriority;

    if (lpCurrStatedef->nPoweradd != -3333) nPower += lpCurrStatedef->nPoweradd;

    if (lpCurrStatedef->nJuggle != -3333) PrintMessage("TODO:Handel juggle parameter of Statedef");
}

// Draw the Player
void CPlayer::DrawPlayer() {
    m_SffManager.BlitAnim((s16)x, (s16)y);

    // draw debug info
    if (bDebugInfo) Debug();
}

/*
================================================================================
Trigger helpers
================================================================================
*/
bool CPlayer::IsAnimAviable(s32 nAnim) {
    if (m_AirManager.GetAction(nAnim) == NULL)
        return false;
    else
        return true;
}

/*
================================================================================
control function
================================================================================
*/
void CPlayer::InitFunctTable() {
    /*pFuncTable[Control_AfterImage]=&CPlayer::ControlType_AfterImage;
    pFuncTable[Control_AfterImageTime]=&CPlayer::ControlType_AfterImageTime;
    pFuncTable[Control_AllPalFX]=&CPlayer::ControlType_AllPalFX;
    pFuncTable[Control_AngleAdd]=&CPlayer::ControlType_AngleAdd;
    pFuncTable[Control_AngleDraw]=&CPlayer::ControlType_AngleDraw;
    pFuncTable[Control_AngleMul]=&CPlayer::ControlType_AngleMul;
    pFuncTable[Control_AngleSet]=&CPlayer::ControlType_AngleSet;
    pFuncTable[Control_AppendToClipboard]=&CPlayer::ControlType_AppendToClipboard;
    pFuncTable[Control_AssertSpecial]=&CPlayer::ControlType_AssertSpecial;
    pFuncTable[Control_AttackDist]=&CPlayer::ControlType_AttackDist;
    pFuncTable[Control_AttackMulSet]=&CPlayer::ControlType_AttackMulSet;
    pFuncTable[Control_BGPalFX]=&CPlayer::ControlType_BGPalFX;
    pFuncTable[Control_BindToParent]=&CPlayer::ControlType_BindToParent;
    pFuncTable[Control_BindToRoot]=&CPlayer::ControlType_BindToRoot;
    pFuncTable[Control_BindToTarget]=&CPlayer::ControlType_BindToTarget;*/
    pFuncTable[Control_ChangeAnim] = &CPlayer::ControlType_ChangeAnim;
    // pFuncTable[Control_ChangeAnim2]=&CPlayer::ControlType_ChangeAnim2;
    pFuncTable[Control_ChangeState] = &CPlayer::ControlType_ChangeState;
}

void CPlayer::ControlType_ChangeAnim(PLSTATE* state) {
    float value = 0;
    for (int i = 0; i <= state->nParamCount; i++) {
        CONTROLLERPARAMS param = state->pConParm[i];
        if (param.nParam == CPN_value) {
            value = this->m_pVMachine->Execute(param.pInts);
            PrintMessage("ControlType_ChangeAnim %f", value);
            break;
        }
    }
    this->m_SffManager.PrepareAnim((int)value);
}

void CPlayer::ControlType_ChangeState(PLSTATE* state) {
    float value = 0;
    for (int i = 0; i <= state->nParamCount; i++) {
        CONTROLLERPARAMS param = state->pConParm[i];
        if (param.nParam == CPN_value) {
            value = this->m_pVMachine->Execute(param.pInts);
            PrintMessage("ControlType_ChangeState %f", value);
            break;
        }
    }
    this->ChangeState((int)(value));
}
