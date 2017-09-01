/*
* This include is only to use in a gamemode next to the MV_cPanel script.
* Warning: about score and money, do not use the default samp functions
	* for score use: GivePlayerScore, SetPlayerScoreEx and GetPlayerScoreEx
	* same with money: only use GivePlayerCash and SetPlayerCash (and GetPlayerCash)
*/

stock SendClientMessageToAdmins(color, message[])
{
	CallRemoteFunction("SendClientMessageToAdmins", "is", color, message);
}

stock IsPlayerVIP(playerid)
{
	return CallRemoteFunction("IsPlayerVIP", "i", playerid);
}

stock IsPlayerAdminEx(playerid, level)
{
	return CallRemoteFunction("IsPlayerAdminEx", "ii", playerid, level);
}

stock IsPlayerMuted(playerid)
{
	return CallRemoteFunction("IsPlayerMuted", "i", playerid);
}

stock GivePlayerScore(playerid, score)
{
	CallRemoteFunction("GivePlayerScore", "ii",playerid, score );
}

stock SetPlayerScoreEx(playerid, score)
{
	CallRemoteFunction("SetPlayerScoreEx", "ii",playerid, score );
}

stock GivePlayerCash(playerid, money)
{
	CallRemoteFunction("GivePlayerCash", "ii", playerid, money);
}

stock SetPlayerCash(playerid, money)
{
	CallRemoteFunction("SetPlayerCash", "ii",playerid, money );
}

stock GetPlayerCash(playerid)
{
	return CallRemoteFunction("GetPlayerCash", "i",playerid);
}

stock GetPlayerScoreEx(playerid)
{
	return CallRemoteFunction("GetPlayerScoreEx", "i", playerid);
}

stock IsWeaponForbidden(weaponid)
{
	return CallRemoteFunction("IsWeaponForbidden", "i", weaponid);
}

stock IsAdvertisement(string[])
{
	return CallRemoteFunction("IsAdvertisement", "s", string);
}