/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/
#include "precompiled.h"

char* EXT_FUNC GetClientFallback_api() {
	return com_clientfallback;
}

int* EXT_FUNC GetAllowCheats_api() {
	return &allow_cheats;
}

bool EXT_FUNC GSBSecure_api() {
	return Steam_GSBSecure() != 0;
}

int EXT_FUNC GetBuildNumber_api() {
	return build_number();
}

double EXT_FUNC GetRealTime_api() {
	return realtime;
}

int* EXT_FUNC GetMsgBadRead_api() {
	return &msg_badread;
}

cmd_source_t* EXT_FUNC GetCmdSource_api() {
	return &cmd_source;
}

void EXT_FUNC Log_api(const char* prefix, const char* msg) {
#ifdef REHLDS_FLIGHT_REC
	FR_Log(prefix, msg);
#endif
}

DLL_FUNCTIONS* EXT_FUNC GetEntityInterface_api() {
	return &gEntityInterface;
}

bool EXT_FUNC GetHitboxCorners(int hitboxId, float* /* [8*3] */ corners, int* pGroupId) {
	if (hitboxId < 0 || hitboxId >= g_studio_numhulls) {
		return false;
	}

	mplane_t* right = &studio_planes[hitboxId * 6 + 0 * 2 + 0];
	mplane_t* left = &studio_planes[hitboxId * 6 + 0 * 2 + 1];
	mplane_t* rear = &studio_planes[hitboxId * 6 + 1 * 2 + 0];
	mplane_t* front = &studio_planes[hitboxId * 6 + 1 * 2 + 1];
	mplane_t* top = &studio_planes[hitboxId * 6 + 2 * 2 + 0];
	mplane_t* bottom = &studio_planes[hitboxId * 6 + 2 * 2 + 1];

	float p[3];

	if (PIR_1 != CalcPlanesIntersection(left, front, bottom, p))
		return false;
	corners[0 * 3 + 0] = p[0]; corners[0 * 3 + 1] = p[1]; corners[0 * 3 + 2] = p[2];

	if (PIR_1 != CalcPlanesIntersection(left, front, top, p))
		return false;
	corners[1 * 3 + 0] = p[0]; corners[1 * 3 + 1] = p[1]; corners[1 * 3 + 2] = p[2];

	if (PIR_1 != CalcPlanesIntersection(right, front, top, p))
		return false;
	corners[2 * 3 + 0] = p[0]; corners[2 * 3 + 1] = p[1]; corners[2 * 3 + 2] = p[2];

	if (PIR_1 != CalcPlanesIntersection(right, front, bottom, p))
		return false;
	corners[3 * 3 + 0] = p[0]; corners[3 * 3 + 1] = p[1]; corners[3 * 3 + 2] = p[2];


	if (PIR_1 != CalcPlanesIntersection(left, rear, bottom, p))
		return false;
	corners[4 * 3 + 0] = p[0]; corners[4 * 3 + 1] = p[1]; corners[4 * 3 + 2] = p[2];

	if (PIR_1 != CalcPlanesIntersection(left, rear, top, p))
		return false;
	corners[5 * 3 + 0] = p[0]; corners[5 * 3 + 1] = p[1]; corners[5 * 3 + 2] = p[2];

	if (PIR_1 != CalcPlanesIntersection(right, rear, top, p))
		return false;
	corners[6 * 3 + 0] = p[0]; corners[6 * 3 + 1] = p[1]; corners[6 * 3 + 2] = p[2];

	if (PIR_1 != CalcPlanesIntersection(right, rear, bottom, p))
		return false;
	corners[7 * 3 + 0] = p[0]; corners[7 * 3 + 1] = p[1]; corners[7 * 3 + 2] = p[2];

	if (pGroupId) {
		*pGroupId = studio_hull_hitgroup[hitboxId];
	}

	return true;
}

void EXT_FUNC SetupHitboxTracing() {
	g_studio_numhulls = 0;
}

CRehldsServerStatic g_RehldsServerStatic;
CRehldsServerData g_RehldsServerData;
CRehldsHookchains g_RehldsHookchains;
RehldsFuncs_t g_RehldsApiFuncs = 
{
	&SV_DropClient_api,
	&SV_RejectConnection,
	&Steam_NotifyBotConnect_api,
	&GetNetMessage_api,
	&GetHostClient_api,
	&GetMsgReadCount_api,
	&SV_FilterUser,
	&NET_SendPacket_api,
	&Cmd_TokenizeString,
	&SV_CheckChallenge_api,
	&SV_SendUserReg,
	&SV_WriteDeltaDescriptionsToClient,
	&SV_SetMoveVars,
	&SV_WriteMovevarsToClient,
	&GetClientFallback_api,
	&GetAllowCheats_api,
	&GSBSecure_api,
	&GetBuildNumber_api,
	&GetRealTime_api,
	&GetMsgBadRead_api,
	&GetCmdSource_api,
	&Log_api,
	&GetEntityInterface_api,
	&EV_PlayReliableEvent_api,
	&SV_LookupSoundIndex,
	&MSG_StartBitWriting,
	&MSG_WriteBits,
	&MSG_WriteBitVec3Coord,
	&MSG_EndBitWriting,
	&SZ_GetSpace,
	&GetHitboxCorners,
	&SetupHitboxTracing,
	&SV_SetupMoveEx_api,
	&SV_RestoreMove_api
};

sizebuf_t* EXT_FUNC GetNetMessage_api()
{
	return &net_message;
}

IGameClient* EXT_FUNC GetHostClient_api()
{
	if (host_client == NULL)
		return NULL;

	return GetRehldsApiClient(host_client);
}

extern int* EXT_FUNC GetMsgReadCount_api()
{
	return &msg_readcount;
}

IRehldsHookRegistry_Steam_NotifyClientConnect* CRehldsHookchains::Steam_NotifyClientConnect()
{
	return &m_Steam_NotifyClientConnect;
}

IRehldsHookRegistry_SV_ConnectClient* CRehldsHookchains::SV_ConnectClient()
{
	return &m_SV_ConnectClient;
}

IRehldsHookRegistry_SV_GetIDString* CRehldsHookchains::SV_GetIDString()
{
	return &m_SV_GetIDString;
}

IRehldsHookRegistry_SV_SendServerinfo* CRehldsHookchains::SV_SendServerinfo()
{
	return &m_SV_SendServerinfo;
}

IRehldsHookRegistry_SV_CheckProtocol* CRehldsHookchains::SV_CheckProtocol()
{
	return &m_SV_CheckProtocol;
}

IRehldsHookRegistry_SVC_GetChallenge_mod* CRehldsHookchains::SVC_GetChallenge_mod()
{
	return &m_SVC_GetChallenge_mod;
}

IRehldsHookRegistry_SV_CheckKeyInfo* CRehldsHookchains::SV_CheckKeyInfo()
{
	return &m_SV_CheckKeyInfo;
}

IRehldsHookRegistry_SV_CheckIPRestrictions* CRehldsHookchains::SV_CheckIPRestrictions()
{
	return &m_SV_CheckIPRestrictions;
}

IRehldsHookRegistry_SV_FinishCertificateCheck* CRehldsHookchains::SV_FinishCertificateCheck()
{
	return &m_SV_FinishCertificateCheck;
}

IRehldsHookRegistry_Steam_NotifyBotConnect* CRehldsHookchains::Steam_NotifyBotConnect()
{
	return &m_Steam_NotifyBotConnect;
}

IRehldsHookRegistry_SerializeSteamId* CRehldsHookchains::SerializeSteamId()
{
	return &m_SerializeSteamId;
}

IRehldsHookRegistry_SV_CompareUserID* CRehldsHookchains::SV_CompareUserID()
{
	return &m_SV_CompareUserID;
}

IRehldsHookRegistry_Steam_NotifyClientDisconnect* CRehldsHookchains::Steam_NotifyClientDisconnect()
{
	return &m_Steam_NotifyClientDisconnect;
}

IRehldsHookRegistry_PreprocessPacket* CRehldsHookchains::PreprocessPacket() {
	return &m_PreprocessPacket;
}

IRehldsHookRegistry_ValidateCommand* CRehldsHookchains::ValidateCommand() {
	return &m_ValidateCommand;
}

IRehldsHookRegistry_ClientConnected* CRehldsHookchains::ClientConnected() {
	return &m_ClientConnected;
}

IRehldsHookRegistry_HandleNetCommand* CRehldsHookchains::HandleNetCommand() {
	return &m_HandleNetCommand;
}

IRehldsHookRegistry_Mod_LoadBrushModel* CRehldsHookchains::Mod_LoadBrushModel() {
	return &m_Mod_LoadBrushModel;
}

IRehldsHookRegistry_Mod_LoadStudioModel* CRehldsHookchains::Mod_LoadStudioModel() {
	return &m_Mod_LoadStudioModel;
}

IRehldsHookRegistry_ExecuteServerStringCmd* CRehldsHookchains::ExecuteServerStringCmd() {
	return &m_ExecuteServerStringCmd;
}

IRehldsHookRegistry_SV_EmitEvents* CRehldsHookchains::SV_EmitEvents() {
	return &m_SV_EmitEvents;
}

IRehldsHookRegistry_EV_PlayReliableEvent* CRehldsHookchains::EV_PlayReliableEvent() {
	return &m_EV_PlayReliableEvent;
}

IRehldsHookRegistry_SV_StartSound* CRehldsHookchains::SV_StartSound() {
	return &m_SV_StartSound;
}

IRehldsHookRegistry_PF_Remove_I* CRehldsHookchains::PF_Remove_I() {
	return &m_PF_Remove_I;
}

IRehldsHookRegistry_PF_BuildSoundMsg_I* CRehldsHookchains::PF_BuildSoundMsg_I() {
	return &m_PF_BuildSoundMsg_I;
}

int EXT_FUNC CRehldsApi::GetMajorVersion()
{
	return REHLDS_API_VERSION_MAJOR;
}

int EXT_FUNC CRehldsApi::GetMinorVersion()
{
	return REHLDS_API_VERSION_MINOR;
}

const RehldsFuncs_t* EXT_FUNC CRehldsApi::GetFuncs()
{
	return &g_RehldsApiFuncs;
}

IRehldsHookchains* EXT_FUNC CRehldsApi::GetHookchains()
{
	return &g_RehldsHookchains;
}

IRehldsServerStatic* EXT_FUNC CRehldsApi::GetServerStatic() {
	return &g_RehldsServerStatic;
}

IRehldsServerData* EXT_FUNC CRehldsApi::GetServerData() {
	return &g_RehldsServerData;
}

IRehldsFlightRecorder* EXT_FUNC CRehldsApi::GetFlightRecorder() {
	return g_FlightRecorder;
}

EXPOSE_SINGLE_INTERFACE(CRehldsApi, IRehldsApi, VREHLDS_HLDS_API_VERSION);