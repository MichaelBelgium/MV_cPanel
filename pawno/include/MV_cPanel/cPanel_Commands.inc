CMD:cmds(playerid,params[])
{
	new cmds[256*3];

	strcat(cmds, "General:\nstats admins report changepassword\n\n");

	if(IsPlayerVIP(playerid)) strcat(cmds, "Vips:\nv(ip)veh v(ip)rep v(ip)repair v(ip)rep v(ip)nos v(ip)fl(ip) v(ip)unlimnos v(ip)heal v(ip)godcar v(ip)lock v(ip)chat\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) strcat(cmds, "Trial mod:\nwarn asay eject mute unmute slap spectate disarm adminduty togglechat\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_MOD)) strcat(cmds, "Moderator:\nannounce kick jail unjail freeze unfreeze burn ajetpack akill cage uncage crash\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) strcat(cmds, "Trial administrator:\nget goto ban setinterior spawn sethealth setarmour healall setskin giveweapon explode settime setweather reconnect setallskin setalltime setallweather\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_ADMIN)) strcat(cmds, "Administrator:\nsetscore setmoney armourall setallscore spawnall giveallweapon killall getall ejectall godall ungodall freezeall unfreezeall\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_OWNER)) strcat(cmds, "Owner:\nsetlevel fakechat makevip removevip p(rivate)veh(icle)\n");

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, "Available commands:", cmds, "OK", "");
	return 1;
}

CMD:commands(playerid, params[]) return cmd_cmds(playerid,params);

CMD:changepassword(playerid, params[])
{
	new pass[2][32], Cache:result;

	if(sscanf(params, "s[32]s[32]", pass[0],pass[1])) return SendClientMessage(playerid, COLOR_RED, "Usage: /changepass(word) [old password] [new password]");

	mysql_format(gCon, query, sizeof(query), "UPDATE Players SET Password = SHA2('%e', 512) WHERE Password = SHA2('%e', 512) AND Playername = '%e'", pass[1], pass[0], GetPlayerNameEx(playerid));
	result = mysql_query(gCon, query);

	if(cache_affected_rows() == 0)
		SendClientMessage(playerid, COLOR_RED, "Wrong old password.");
	else
		SendClientMessage(playerid, COLOR_GREEN, "Successfully changed password. It is recommended you reconnect to check your new password.");

	cache_delete(result);
	return 1;
}

CMD:stats(playerid,params[])
{
	new string[128], stats[256], id, title[64];

	if(sscanf(params, "u", id))
	{
		format(title, sizeof(title), "Stats of %s (%i)", GetPlayerNameEx(playerid), playerid);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\nVIP: %s\nPing: %i\nIP: %s\n\n", GetPlayerNameEx(playerid), playerid, GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerLevel(playerid), IsPlayerVIP(playerid) ? ("yes") : ("no"), GetPlayerPing(playerid), PlayerInfo[playerid][IP]);
		strcat(stats, string);

		format(string, sizeof(string), "Score: %i\nMoney: %i\n\n", GetPlayerScoreEx(playerid), GetPlayerCash(playerid));
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n\n", MsToString(NetStats_GetConnectedTime(playerid),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(playerid)/1000)+PlayerInfo[playerid][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}
	else
	{
		if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

		format(title, sizeof(title), "Stats of %s (%i)", GetPlayerNameEx(id), id);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\nVIP: %s\nPing: %i\nIP: %s\n\n", GetPlayerNameEx(id), id, GetPlayerLevelEx(GetPlayerLevel(id)), GetPlayerLevel(id), IsPlayerVIP(id) ? ("yes") : ("no"), GetPlayerPing(id), PlayerInfo[id][IP]);
		strcat(stats, string);

		format(string, sizeof(string), "Score: %i\nMoney: %i\n\n", GetPlayerScoreEx(id), GetPlayerCash(id));
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n", MsToString(NetStats_GetConnectedTime(id),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(id)/1000)+PlayerInfo[id][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, title, stats, "OK", "");
	return 1;
}

CMD:admins(playerid, params[])
{
	new string[128], admins[256*5], count = 0;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(PlayerInfo[i][Adminlevel] == 0) continue;
		format(string, sizeof(string), "Name: %s - Level: %s (%i)\n", PlayerInfo[i][Name], GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerLevel(playerid));
		strcat(admins, string);

		count++;
	}

	if(count == 0)
		admins = "No admins online";
	else
	{
		format(string, sizeof(string), "There %s %i admin%s online:\n\n", count == 1 ? ("is") : ("are"), count, count != 1 ? ("s") : (""));
		strins(admins, string, 0);
	}

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, "Admins", admins, "OK", "");
	return 1;
}

CMD:report(playerid, params[])
{
	new id, reason[32], string[128];
	if(sscanf(params, "us[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /report [playerid / (part of) playername] [reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This playerid is not online.");

	format(string, sizeof(string), "Incomming report! %s (%i) about %s (%i): %s", GetPlayerNameEx(playerid), playerid, GetPlayerNameEx(id), id, reason);
	SendClientMessageToAdmins(COLOR_RED, string);

	SendClientMessage(playerid,COLOR_RED,"Your report has been sent to all online administrators.");
	#if LOG_REPORTS
		SaveLog(LOG_TYPE_REPORTS, GetPlayerNameEx(playerid), "", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

// ================= ADMIN COMMANDS ===========================
CMD:cpanel(playerid, params[])
{
	if(IsPlayerAdmin(playerid) && !IsPlayerAdminEx(playerid, LEVEL_OWNER))
	{
		PlayerInfo[playerid][Adminlevel] = LEVEL_OWNER;
		SendClientMessage(playerid, COLOR_GREEN, "Server assigned you as an owner (level 5)");
	}
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	
	ShowPlayerDialogEx(playerid, DIALOG_CPANEL);
	return 1;
}

CMD:a(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new msg[128], string[256];
	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_RED, "Usage: /a(chat) [message]");

	format(string, sizeof(string), COL_ADMIN_1"%s %s (%i):"COL_ADMIN_2" %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), playerid, msg);
	SendClientMessageToAdmins(-1, string);
	return 1;
}

CMD:achat(playerid, params[]) return cmd_a(playerid, params);

CMD:warn(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new id, reason[64], string[128];
	if(sscanf(params, "us[64]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /warn [playerid / (part of) playername] [reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	PlayerInfo[id][Warns]++;

	if(PlayerInfo[id][Warns] >= MAX_WARNINGS)
	{
		PlayerInfo[id][Warns] = 0;

		#if BAN_ON_WARN
			BanPlayer(id, INVALID_PLAYER_ID, "Reached max warns");
		#else
			format(string, sizeof(string), "You've been kicked for having %i or more warnings, behave better in the future!", MAX_WARNINGS);
			SendClientMessage(id, COLOR_RED, string);

			KickPlayer(id, INVALID_PLAYER_ID, "Reached max warns");
		#endif
	}
	else
	{
		format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"warned %s (%i) for %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, reason);
		SendClientMessageToAdmins(-1, string);

		format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"has warned you for: %s (%i/%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), reason, PlayerInfo[id][Warns], MAX_WARNINGS);
		SendClientMessage(id, -1, string);
	}

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "warn", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

CMD:asay(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new msg[128], string[256];
	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_RED, "Usage: /asay [message]");
	format(string, sizeof(string), COL_ADMIN_1"-[Staff message]- "COL_ADMIN_2"%s", msg);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "asay", "", msg);
	#endif
	return 1;
}

CMD:eject(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;

	new id, reason[64], string[128];
	if(sscanf(params, "uS(No reason specified)[64]", id, reason)) return SendClientMessage(playerid, -1, "Usage: /eject [playerid / (part of) playername] [Optional: reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(!IsPlayerInAnyVehicle(id)) return SendClientMessage(playerid, COLOR_RED, "This player isn't in any vehicle.");

	RemovePlayerFromVehicle(id);

	format(string,sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"ejected %s (%i) from their vehicle.",GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "eject", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

CMD:mute(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new id, minutes, reason[32];

	if(sscanf(params, "uI("#COOLDOWN_MUTED")S(Spamming)[32]", id, minutes, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /mute [playerid / (part of) playername] [Optional: minutes (= "#COOLDOWN_MUTED")] [Optional: reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	MutePlayer(id, playerid, reason, minutes);
	return 1;
}

CMD:unmute(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new id, string[128];
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /unmute [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(!IsPlayerMuted(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not muted.");

	UnmutePlayer(id);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"unmuted %s (%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "unmute", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:slap(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new id, Float:height, string[128], Float:pos[3];

	if(sscanf(params, "uF(10.0)", id, height)) return SendClientMessage(playerid, COLOR_RED, "Usage: /slap [playerid / (part of) playername] [Optional: height (= 10)]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	GetPlayerPos(id, pos[0], pos[1], pos[2]);
	SetPlayerPos(id, pos[0], pos[1], pos[2] + height);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"has slapped %s (%i)",GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", height);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "slap", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:spectate(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new id, string[128];

	KillTimer(PlayerInfo[playerid][pTimer][3]);
	if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
		TogglePlayerSpectating(playerid, false);
		format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"stopped spectating", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
		SendClientMessageToAdmins(-1, string);
		return 1;
	}

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /spectate [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(id == playerid) return SendClientMessage(playerid, COLOR_RED, "You can't spectate yourself.");

	TogglePlayerSpectating(playerid, true);
	PlayerInfo[playerid][pTimer][3] = SetTimerEx("SpectateTimer", 1000, true, "ii", playerid, id);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"started spectating %s (%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAdmins(-1, string);

	SendClientMessage(playerid, -1, COL_ADMIN_1"Use "COL_ADMIN_2"/spectate "COL_ADMIN_1"again to stop spectating.");

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "spectate", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:spec(playerid, params[]) return cmd_spectate(playerid, params);

CMD:disarm(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new id, string[128];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /disarm [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	ResetPlayerWeapons(id);
	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"disarmed %s (%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "disarm", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:adminduty(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;

	new string[128];
	if(GetPlayerSkin(playerid) == ADMIN_SKIN)
	{
		SetPlayerSkin(playerid, PlayerInfo[playerid][OldSkin]);
		SetPlayerHealth(playerid, 100.0);
		PlayerInfo[playerid][OldSkin] = -1;
		format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"now off duty.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
		SendClientMessageToAdmins(-1, string);
	}
	else
	{
		PlayerInfo[playerid][OldSkin] = GetPlayerSkin(playerid);
		SetPlayerSkin(playerid, ADMIN_SKIN);
		SetPlayerHealth(playerid, 9999.0);
		format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"now on duty.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
		SendClientMessageToAdmins(-1, string);
	}

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "adminduty");
	#endif
	return 1;
}

CMD:togglechat(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new string[128];
	ChatEnabled = !ChatEnabled;

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"%s the chat.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), ChatEnabled ? ("enabled") : ("disabled"));
	SendClientMessageToAll(-1, string);
	return 1;
}


CMD:announce(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new text[64], string[128];
	if(sscanf(params, "s[64]", text)) return SendClientMessage(playerid, COLOR_RED, "Usage: /announce [text]");
	GameTextForAll(text, 2500, 3);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"announced '%s'.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), text);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "announce", "", text);
	#endif
	return 1;
}

CMD:akill(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;

	new id, reason[32], string[128];
	if(sscanf(params, "uS(No reason specified)[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /akill [playerid / (part of) playername] [Optional: reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	SetPlayerHealth(id, 0.0);
	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"killed %s (%i). Reason: %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, reason);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "akill", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:kick(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new id, reason[32];

	if(sscanf(params, "us[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /kick [playerid / (part of) playername] [reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	KickPlayer(id, playerid, reason);
	return 1;
}

CMD:jail(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;

	new id, reason[32], string[128], minutes;

	if(sscanf(params, "us[32]I(3)", id, reason, minutes)) return SendClientMessage(playerid, COLOR_RED, "Usage: /jail [playerid / (part of) playername] [reason] [Optional: minutes]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	SetPlayerInterior(id, 6);
	SetPlayerPos(id, 264.6288,77.5742,1001.0391);
	TogglePlayerControllable(id, false);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"jailed %s (%i) for %i minutes. Reason: %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, minutes, reason);
	SendClientMessageToAll(-1, string);

	KillTimer(PlayerInfo[id][pTimer][2]);
	PlayerInfo[id][pTimer][2] = SetTimerEx("JailTimer", minutes*60*1000, false, "i", id);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "jail", GetPlayerNameEx(id), params);
	#endif
	return 1;
}

CMD:unjail(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new id, string[128];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /unjail [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	JailTimer(id);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"unjailed %s (%i).", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "unjail", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:freeze(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new id, reason[32], string[128];
	if(sscanf(params, "uS(No reason specified)[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /freeze [playerid / (part of) playername] [Optional: reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	TogglePlayerControllable(id, false);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"froze %s (%i). Reason: %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, reason);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "freeze", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

CMD:unfreeze(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new id, string[128];
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /unfreeze [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	TogglePlayerControllable(id, true);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"unfroze %s (%i).", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "unfreeze", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:burn(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new id, string[128], Float:pos[3];
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /burn [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	GetPlayerPos(id, pos[0], pos[1], pos[2]);
	CreateExplosion(pos[0], pos[1], pos[2]+3.0, 1, 10.0);
	
	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"burned %s (%i).", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "burn", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:crash(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new Float:pos[3], id, reason[32], string[128];

	if(sscanf(params, "uS(No reason specified)[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /crash [playerid / (part of) playername] [Optional: reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	GetPlayerPos(id, pos[0], pos[1], pos[2]);
	CreatePlayerObject(id, 385, pos[0], pos[1], pos[2], 0, 0, 0);
	
	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"crashed %s (%i). Reason: %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, reason);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "crash", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

CMD:ajetpack(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new string[128];
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"spawned a jetpack.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "ajetpack");
	#endif
	return 1;
}

CMD:cage(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new id, reason[32], string[128];

	if(sscanf(params, "uS(No reason specified)[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /cage [playerid / (part of) playername] [Optional: reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(IsPlayerCaged(id)) return SendClientMessage(playerid, COLOR_RED, "This player is already caged");

	CagePlayer(id);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"caged %s (%i). Reason: %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, reason);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "cage", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

CMD:uncage(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	new id, string[128];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /uncage [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(!IsPlayerCaged(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not caged");

	UncagePlayer(playerid);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"uncaged %s (%i).", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "uncage", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:get(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, string[128], Float:pos[3];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /get [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	if(IsPlayerInAnyVehicle(id))
		SetVehiclePos(GetPlayerVehicleID(id), pos[0], pos[1] + 5.0, pos[2]);
	else
		SetPlayerPos(id, pos[0], pos[1], pos[2] + 3.0);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"teleported %s (%i) to themself.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAdmins(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "get", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:goto(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, string[128], Float:pos[3];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /goto [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	GetPlayerPos(id, pos[0], pos[1], pos[2]);

	if(IsPlayerInAnyVehicle(playerid))
		SetVehiclePos(GetPlayerVehicleID(playerid), pos[0], pos[1] + 5.0, pos[2]);
	else
		SetPlayerPos(playerid, pos[0], pos[1], pos[2] + 3.0);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"teleported to %s (%i).", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAdmins(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "goto", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:ban(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, reason[32];

	if(sscanf(params, "us[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /ban [playerid / (part of) playername] [reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	BanPlayer(id, playerid, reason);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "ban", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

CMD:setinterior(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, interior, string[128];

	if(sscanf(params, "ui", id, interior)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setinterior [playerid / (part of) playername] [interiorid]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	SetPlayerInterior(id, interior);
	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"set the interior of %s (%i) to %i.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, interior);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setinterior", GetPlayerNameEx(id), params);
	#endif
	return 1;
}

CMD:spawn(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, string[128];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /spawn [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	SpawnPlayer(id);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"spawned %s (%i).", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "spawn", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:sethealth(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, Float:health, string[128];

	if(sscanf(params, "uf", id, health)) return SendClientMessage(playerid, COLOR_RED, "Usage: /sethealth [playerid / (part of) playername] [health]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(health > 100.0) return SendClientMessage(playerid, COLOR_RED, "Health can only be 100 and less");

	SetPlayerHealth(id, health);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"set %s's (%i) health to %0.2f.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, health);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%0.2f", health);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "sethealth", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:setarmour(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, Float:armour, string[128];

	if(sscanf(params, "uf", id, armour)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setarmour [playerid / (part of) playername] [armour]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(armour > 100.0) return SendClientMessage(playerid, COLOR_RED, "Armour can only be 100 and less");

	SetPlayerArmour(id, armour);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"set %s's (%i) armour to %0.2f.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, armour);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%0.2f", armour);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setarmour", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:healall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		if(IsPlayerConnected(i))
			SetPlayerHealth(i, 100.0);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"healed everyone.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "healall");
	#endif
	return 1;
}

CMD:setskin(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, skin, string[128];

	if(sscanf(params, "ui", id, skin)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setskin [playerid / (part of) playername] [skinid]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(skin > 299) return SendClientMessage(playerid, COLOR_RED, "Skinid can only be 299 and less");

	SetPlayerSkin(id, skin);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"set %s's (%i) skin to %i.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, skin);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", skin);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setskin", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:giveweapon(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, weapon, ammo, string[128];

	if(sscanf(params, "uiI(500)", id, weapon, ammo)) return SendClientMessage(playerid, COLOR_RED, "Usage: /giveweapon [playerid / (part of) playername] [weaponid] [Optional: ammo]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(IsWeaponForbidden(weapon)) return SendClientMessage(playerid, COLOR_RED, "You are not permitted to give this weapon.");

	GivePlayerWeapon(id, weapon, ammo);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"gave you a(n) %s with %i ammo.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetWeaponNameEx(weapon), ammo);
	SendClientMessage(id, -1, string);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"gave %s (%i) a(n) %s with %i ammo.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, GetWeaponNameEx(weapon), ammo);
	SendClientMessageToAdmins(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%s %i", GetWeaponNameEx(weapon), ammo);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "giveweapon", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:explode(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, string[128], Float:pos[3];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /explode [playerid / (part of) playername]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	GetPlayerPos(id, pos[0], pos[1], pos[2]);
	CreateExplosion(pos[0], pos[1], pos[2], 0, 5.0);
	CreateExplosion(pos[0], pos[1] + 3.0, pos[2], 0, 5.0);
	CreateExplosion(pos[0] + 3.0, pos[1], pos[2], 0, 5.0);
	
	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"exploded %s (%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "explode", GetPlayerNameEx(id));
	#endif
	return 1;
}

CMD:settime(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, time, string[128];

	if(sscanf(params, "ui", id, time)) return SendClientMessage(playerid, COLOR_RED, "Usage: /settime [playerid / (part of) playername] [time]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	SetPlayerTime(id, time, 0);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"changed your time to %i:00", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), time);
	SendClientMessage(id, -1, string);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"changed %s's (%i) time to %i:00", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, time);
	SendClientMessageToAdmins(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", time);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "settime", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:setweather(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, weather, string[128];

	if(sscanf(params, "ui", id, weather)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setweather [playerid / (part of) playername] [weather]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	SetPlayerWeather(id, weather);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"changed your weather to %i", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), weather);
	SendClientMessage(id, -1, string);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"changed %s's (%i) weather to %i", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, weather);
	SendClientMessageToAdmins(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", weather);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setweather", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:reconnect(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new id, reason[32], string[128];

	if(sscanf(params, "uS(No reason specified)[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /reconnect [playerid / (part of) playername] [Optional: reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	PlayerInfo[id][Reconnecting] = true;

	format(string, sizeof(string), "banip %s", PlayerInfo[id][IP]);
	SendRconCommand(string);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"forced %s (%i) to reconnect. Reason: %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, reason);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "reconnect", GetPlayerNameEx(id), reason);
	#endif
	return 1;
}

CMD:setallskin(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new skin, string[128];

	if(sscanf(params, "i", skin)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setallskin [skinid]");
	if(skin > 299) return SendClientMessage(playerid, COLOR_RED, "Skinid can only be 299 and less");

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		if(IsPlayerConnected(i))
			SetPlayerSkin(i, skin);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"changed everyone's skin to %i", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), skin);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", skin);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setallskin", "", string);
	#endif
	return 1;
}

CMD:setalltime(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new time, string[128];

	if(sscanf(params, "i", time)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setalltime [time]");
	if(time < 0 || time > 23) return SendClientMessage(playerid, COLOR_RED, "Invalid time.");

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		if(IsPlayerConnected(i))
			SetPlayerTime(i, time, 0);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"changed everyone's time to %i:00", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), time);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", time);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setalltime", "", string);
	#endif
	return 1;
}

CMD:setallweather(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	new weather, string[128];

	if(sscanf(params, "i", weather)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setallweather [weatherid]");
	
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		if(IsPlayerConnected(i))
			SetPlayerWeather(i, weather);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"changed everyone's weather to %i", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), weather);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", weather);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setallweather", "", string);
	#endif
	return 1;
}


CMD:setscore(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new id, score, string[128];

	if(sscanf(params, "ui", id, score)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setscore [playerid / (part of) playername] [score]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player isn't connected");

	SetPlayerScoreEx(id, score);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"set %s's (%i) score to %i", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, score);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", score);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setscore", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:setmoney(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new id, money, string[128];

	if(sscanf(params, "ui", id, money)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setmoney [playerid / (part of) playername] [money]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player isn't connected");

	SetPlayerCash(id, money);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"set %s's (%i) money to $%i", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, money);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", money);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setmoney", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:armourall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		SetPlayerArmour(i, 100.0);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"gave armour to everyone.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "armourall");
	#endif
	return 1;
}

CMD:setallscore(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new score, string[128];

	if(sscanf(params, "i", score)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setallscore [score]");

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		SetPlayerScoreEx(i, score);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"set everyone's score to %i", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), score);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%i", score);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setallscore", "", string);
	#endif
	return 1;
}

CMD:spawnall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		SpawnPlayer(i);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"spawned everyone.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "spawnall");
	#endif
	return 1;
}

CMD:giveallweapon(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new weapon, ammo, string[128];

	if(sscanf(params, "iI(500)", weapon, ammo)) return SendClientMessage(playerid, COLOR_RED, "Usage: /giveallweapon [weaponid] [Optional: ammo]");
	if(IsWeaponForbidden(weapon)) return SendClientMessage(playerid, COLOR_RED, "You are not permitted to give this weapon.");
	
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		GivePlayerWeapon(i, weapon, ammo);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"gave everyone a(n) %s with %i ammo.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetWeaponNameEx(weapon), ammo);
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%s %i", GetWeaponNameEx(weapon), ammo);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "giveallweapon", "", string);
	#endif
	return 1;
}

CMD:killall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		SetPlayerHealth(i, 0.0);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"killed everyone.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "killall");
	#endif
	return 1;
}

CMD:getall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new Float:pos[3], string[128];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		SetPlayerPos(i, pos[0], pos[1], pos[2] + 3.0);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"teleported everyone to themself.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "getall");
	#endif
	return 1;
}

CMD:ejectall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i) || !IsPlayerInAnyVehicle(i)) continue;
		RemovePlayerFromVehicle(i);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"ejected everyone.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "ejectall");
	#endif
	return 1;
}

CMD:godall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		SetPlayerHealth(i, 9999.0);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"gave everyone unlimited health.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "godall");
	#endif
	return 1;
}

CMD:ungodall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		SetPlayerHealth(i, 100.0);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"removed everyone's unlimited health.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "ungodall");
	#endif
	return 1;
}

CMD:freezeall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		TogglePlayerControllable(i, false);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"froze everyone.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "freezeall");
	#endif
	return 1;
}

CMD:unfreezeall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	new string[128];

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(!IsPlayerConnected(i)) continue;
		TogglePlayerControllable(i, true);
	}

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"unfroze everyone.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
	SendClientMessageToAll(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "unfreezeall");
	#endif
	return 1;
}


CMD:setlevel(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;
	new id, level, string[128], oldlevel;

	if(sscanf(params, "ui", id, level)) return SendClientMessage(playerid, COLOR_RED, "Usage: /setlevel [playerid / (part of) playername] [adminlevel]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");
	if(level == PlayerInfo[id][Adminlevel]) return SendClientMessage(playerid, COLOR_RED, "This player has already this adminlevel.");

	oldlevel = PlayerInfo[id][Adminlevel];

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"%s %s (%i) to level '%s' (%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), oldlevel < level ? ("promoted") : ("demoted"), GetPlayerNameEx(id), id, GetPlayerLevelEx(level), level);
	SendClientMessageToAdmins(-1, string);

	PlayerInfo[id][Adminlevel] = level;

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"%s you to level '%s' (%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), oldlevel < level ? ("promoted") : ("demoted"), GetPlayerLevelEx(level), level);
	SendClientMessage(id, -1, string);

	#if LOG_ADMINCOMMANDS
		format(string, sizeof(string), "%s %i", GetPlayerLevelEx(level), level);
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "setlevel", GetPlayerNameEx(id), string);
	#endif
	return 1;
}

CMD:fakechat(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;
	new id, msg[128], string[256];

	if(sscanf(params, "us[128]", id, msg)) return SendClientMessage(playerid, COLOR_RED, "Usage: /fakechat [playerid / (part of) playername] [message]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This player is not online.");

	SendPlayerMessageToAll(id, msg);

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"executed /fakechat on %s (%i) with message: %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetPlayerNameEx(id), id, msg);
	SendClientMessageToAdmins(-1, string);

	#if LOG_ADMINCOMMANDS
		SaveLog(LOG_TYPE_ACMDS, GetPlayerNameEx(playerid), "fakechat", GetPlayerNameEx(id), msg);
	#endif
	return 1;
}

CMD:makevip(playerid,params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;

	new id, duration, string[128];
	if(sscanf(params, "uI(0)", id, duration)) return SendClientMessage(playerid, COLOR_RED, "Usage: /makevip [playerid / (part of) playername] [Optional: duration (in months)]");

	if(duration == 0)
	{
		mysql_format(gCon, query,sizeof(query),"INSERT INTO Vips (Name,Duration) VALUES ('%e', 0)",GetPlayerNameEx(id));
		format(string, sizeof(string), "ever.");
	}
	else
	{
		mysql_format(gCon, query,sizeof(query),"INSERT INTO Vips (Name,Duration) VALUES ('%e', UNIX_TIMESTAMP(DATE_ADD(NOW(), INTERVAL + %d MONTH)))",GetPlayerNameEx(id),duration);
		format(string, sizeof(string), "%i months.", duration);
	}

	mysql_tquery(gCon, query);

	VipInfo[id][Duration] = duration; //as long as it's not -1 it's fine.

	format(string, sizeof(string),  COL_VIP_1"-[VIP]-"COL_VIP_2" %s has made you a VIP! You are vip for %s", GetPlayerNameEx(playerid), string);
	SendClientMessage(id, -1, string);
	return 1;
}

CMD:removevip(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;

	new id;
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /removevip [playerid / (part of) playername]");
	if(!IsPlayerVIP(id)) return SendClientMessage(playerid, COLOR_RED, "This player isn't vip");

	VipInfo[id][Duration] = -1;
	mysql_format(gCon, query, sizeof(query), "DELETE FROM Vips WHERE Name = '%e'", GetPlayerNameEx(id));
	mysql_tquery(gCon, query);

	SendClientMessage(id, -1, COL_VIP_1"-[VIP]-"COL_VIP_2" Your vip status has been removed.");
	return 1;
}

CMD:privatevehicle(playerid,params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;
	new owner, string[128];

	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You need to be in a vehicle to set that vehicle as a private one.");
	if(sscanf(params, "u", owner)) return SendClientMessage(playerid, COLOR_RED, "Usage: /privatevehicle [playerid / (part of) playername]");
	new veh = GetPlayerVehicleID(playerid);

	AddPrivateVehicle(veh, GetPlayerNameEx(owner));

	format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"added a(n) %s (%i) as private vehicle for %s (%i)", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), GetVehicleName(veh), veh, GetPlayerNameEx(owner), owner);
	SendClientMessageToAll(-1, string);
	return 1;
}

CMD:pveh(playerid, params[]) return cmd_privatevehicle(playerid, params);

// ================= VIP COMMANDS ===========================
CMD:vipveh(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	ShowPlayerDialog(playerid,DIALOG_VIP_VEH,DIALOG_STYLE_INPUT,"Spawn a Vehicle","Please fill in your vehicle Name","Spawn","Back");
	return 1;
}

CMD:vveh(playerid, params[]) return cmd_vipveh(playerid, params);

CMD:viprep(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");
	RepairVehicle(GetPlayerVehicleID(playerid));
	SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]- "COL_VIP_2"Your vehicle has been repaired.");
	return 1;
}

CMD:viprepair(playerid, params[]) return cmd_viprep(playerid, params);
CMD:vrep(playerid, params[]) return cmd_viprep(playerid, params);

CMD:vipnos(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");
	AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
	SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]- "COL_VIP_2"Nitro has been added to your vehicle.");
	return 1;
}

CMD:vnos(playerid, params[]) return cmd_vipnos(playerid, params);

CMD:vipflip(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");

	new Float:a;
	GetVehicleZAngle(GetPlayerVehicleID(playerid),Float:a);
	SetVehicleZAngle(GetPlayerVehicleID(playerid),Float:a);
	SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]- "COL_VIP_2" Your vehicle has been flipped.");
	return 1;
}

CMD:vflip(playerid, params[]) return cmd_vipflip(playerid, params);

CMD:vipunlimnos(playerid, params[])
{
	new string[128];
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");

	VipInfo[playerid][Toggle][0] = !VipInfo[playerid][Toggle][0];
	format(string, sizeof(string), COL_VIP_1"-[VIP]-"COL_VIP_2" Unlimited nitro has been turned %s", VipInfo[playerid][Toggle][0] ? ("on") : ("off"));
	SendClientMessage(playerid, -1, string);
	return 1;
}

CMD:vunlimnos(playerid, params[]) return cmd_vipunlimnos(playerid, params);

CMD:vipheal(playerid, params[])
{
	new string[128];

	if(!IsPlayerVIP(playerid)) return 0;
	if(GetTickCount() - PlayerInfo[playerid][Tick][2] > COOLDOWN_VIP_HEAL*1000)
	{
		PlayerInfo[playerid][Tick][2] = GetTickCount();

		SetPlayerHealth(playerid, 100.0);
		SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]-"COL_VIP_2" Your health has succesfully been restored.");
	}
	else
	{
		format(string, sizeof(string), "You can't do that right now. You can again in %s.",MsToString(COOLDOWN_VIP_HEAL*1000 - (GetTickCount() - PlayerInfo[playerid][Tick][2]), "%1m minute(s) and %1s second(s)"));
		SendClientMessage(playerid, COLOR_RED, string);
	}
	return 1;
}

CMD:vheal(playerid, params[]) return cmd_vipheal(playerid, params);

CMD:vipgodcar(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	new string[128];

	VipInfo[playerid][Toggle][2] = !VipInfo[playerid][Toggle][2];
	format(string, sizeof(string), COL_VIP_1"-[VIP]-"COL_VIP_2"You have %s godcar", VipInfo[playerid][Toggle][2] ? ("enabled") : ("disabled"));
	SendClientMessage(playerid, -1, string);
	return 1;
}

CMD:vgodcar(playerid, params[]) return cmd_vipgodcar(playerid, params);

CMD:viplock(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");
	new string[128];

	if(!VipInfo[playerid][Toggle][1])
		LockVehicle(GetPlayerVehicleID(playerid));
	else
		UnlockVehicle(GetPlayerVehicleID(playerid));

	format(string, sizeof(string), COL_VIP_1"-[VIP]-"COL_VIP_2" Your vehicle has been %slocked", !VipInfo[playerid][Toggle][1] ? ("") : ("un"));
	SendClientMessage(playerid, -1, string);

	VipInfo[playerid][Toggle][1] = !VipInfo[playerid][Toggle][1];
	return 1;
}

CMD:vlock(playerid, params[]) return cmd_viplock(playerid, params);

CMD:vipchat(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	new msg[128], string[256];

	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_RED, "Usage: /vipchat [message]");
	
	format(string, sizeof(string), COL_VIP_1"VIP %s (%i): "COL_VIP_2"%s", GetPlayerNameEx(playerid), playerid, msg);
	SendClientMessageToVips(-1, string);
	return 1;
}

CMD:vchat(playerid, params[]) return cmd_vipchat(playerid, params);